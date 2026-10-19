#!/usr/bin/env python3
"""Fail-closed controls for per-TU profiles and whole static call targets.

Synthetic PE/MAP/PDB fixtures never use the private target or native compiler.
"""
from copy import deepcopy
import importlib.util
import io
import json
from pathlib import Path
import struct
import tempfile
import tomllib
import unittest
from unittest.mock import patch

from ltcg_link import ROOT, support_profile_overrides
from linked_image import PEImage, static_linkage_selector


def module(name):
    spec = importlib.util.spec_from_file_location(name.replace('-', '_'), ROOT / 'scripts' / name)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


class SupportProfiles(unittest.TestCase):
    def test_selected_ltcg_override_and_rejections(self):
        source = ROOT / 'src/Enemy.cpp'
        selected = [source]
        self.assertEqual(support_profile_overrides(selected, (), {'src/Enemy.cpp': ['/TP', '/GL']}),
                         {source: ['/TP', '/GL']})
        for raw in [[], {'src/Gui.cpp': ['/GL']}, {'src/Enemy.cpp': []},
                    {'src/Enemy.cpp': '/GL'}, {'src/Enemy.cpp': ['/O2']},
                    {'src/Enemy.cpp': ['/GL', '/GL-']}, {'src/Enemy.cpp': ['/GL', '/GL']},
                    {'src/Enemy.cpp': ['/GL', None]}]:
            with self.subTest(raw=raw), self.assertRaises(ValueError):
                support_profile_overrides(selected, (), raw)
        with self.assertRaises(ValueError):
            support_profile_overrides(selected, (source,), {'src/Enemy.cpp': ['/GL']})
        with self.assertRaises(ValueError):
            support_profile_overrides(selected, (), {'src/Enemy.cpp': ['/GL'], source: ['/GL']})

    def test_build_groups_include_profile_overrides(self):
        driver = module('build-match-unit.py')
        with (ROOT / 'config/match-units.toml').open('rb') as stream:
            canonical = tomllib.load(stream)
        with (ROOT / 'config/target.toml').open('rb') as stream:
            target = tomllib.load(stream)
        a = deepcopy(canonical['units']['script-viewer-constructor'])
        b = deepcopy(canonical['units']['script-viewer-clear-names'])
        a.update(output_dir='build/test-context-a', support_profiles={'src/Enemy.cpp': ['/GL']})
        b.update(output_dir='build/test-context-b', support_profiles={'src/Enemy.cpp': ['/GL', '/GS']})
        manifest = dict(schema_version=1, target_sha256=canonical['target_sha256'], units={'a': a, 'b': b})
        def load():
            with patch.object(Path, 'open', side_effect=lambda *args, **kwargs: io.BytesIO()), \
                 patch.object(driver.tomllib, 'load', side_effect=[target, manifest]):
                return driver.load()
        load()  # Different profiles legitimately create different artifacts.
        b['output_dir'] = a['output_dir']
        with self.assertRaisesRegex(ValueError, 'different|share|artifact|output'):
            load()


class StaticCalls(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix='linked-context-')
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.out = self.root / 'build/fixture'
        self.out.mkdir(parents=True)
        fixtures = module('test-static-owner-selection.py')
        e = fixtures.Evidence(self.out)
        e.image, e.map, e.pdb = [self.out / ('source.' + suffix) for suffix in ['exe', 'map', 'pdb']]
        e.publics = [fixtures.row('_Public', 0, 'source.ltcg.obj')]
        e.statics = [fixtures.row(obj='support-00-Private.ltcg.obj')]
        e.modules = [str(self.out / 'source.ltcg.obj'), str(self.out / 'support-00-Private.ltcg.obj')]
        e.write()
        data = bytearray(e.image.read_bytes())
        data[0x200:0x208] = b'\xe8' + struct.pack('<i', 0x20 - 5) + b'\xc3\x90\x90'
        e.image.write_bytes(data)
        self.image = e.image
        (self.root / 'config').mkdir()
        (self.root / 'config/functions.csv').write_text('address,size\n0x00401020,8\n')
        self.boundary('reviewed', 0x401027)
        self.field = dict(offset=1, type='REL32', symbol=fixtures.SYMBOL, target=0x401020,
                          static_source='src/Private.cpp', static_size=8)
        self.unit = dict(artifact_kind='linked-pe', source='src/Public.cpp',
                         support_sources=['src/Private.cpp'], output_dir='build/fixture',
                         functions=['Public'], symbol='_Public', target_address=0x401000,
                         size=8, linkages=[self.field])
        self.c = module('compare-linked-function.py')

    def boundary(self, state, end):
        (self.root / 'config/function-boundaries.csv').write_text(
            f'address,span_end,state\n0x00401020,0x{end:08X},{state}\n')

    def compare(self):
        manifest = dict(target_sha256='fixture', units={'caller': self.unit})
        with patch.object(self.c, 'ROOT', self.root), \
             patch.object(self.c.tomllib, 'load', side_effect=[manifest, {'target': {'sha256': 'fixture'}},
                                                            tomllib.loads((ROOT/'config/tools.lock.toml').read_text())]), \
             patch.object(self.c, 'verified_target', return_value=PEImage(self.image)):
            # Only control-plane opens are replaced; PE, MAP, PDB and CSV stay real.
            original = Path.open
            def opened(path, *args, **kwargs):
                if path in (self.c.UNITS_MANIFEST, self.c.TARGET_MANIFEST, self.c.TOOLS_LOCK):
                    return io.BytesIO()
                return original(path, *args, **kwargs)
            with patch.object(Path, 'open', opened):
                return self.c.compare_unit('caller')

    def test_whole_callee_and_parent_bind(self):
        report = self.compare()
        self.assertEqual(report['result'], 'exact')
        self.assertEqual(report['linkages'][0]['static_owner']['size'], 8)

    def test_no_implicit_static_lookup(self):
        del self.field['static_source']; del self.field['static_size']
        with self.assertRaisesRegex(ValueError, 'resolve uniquely'):
            self.compare()

    def test_incomplete_metadata_and_interior_entry(self):
        for changes in [dict(static_size=None), dict(addend=1), dict(type='DIR32'),
                        dict(local=True), dict(static_source='src/Absent.cpp')]:
            saved = dict(self.field)
            self.field.update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                self.compare()
            self.field.clear(); self.field.update(saved)
        del self.field['static_size']
        with self.assertRaises(ValueError): self.compare()

    def test_target_boundary_cannot_be_cropped_or_provisional(self):
        for state, end in [('provisional', 0x401027), ('reviewed', 0x401026)]:
            self.boundary(state, end)
            with self.subTest(state=state, end=end), self.assertRaisesRegex(ValueError, 'complete reviewed'):
                self.compare()

    def test_static_callee_bytes_and_fields_checked(self):
        baseline = self.image.read_bytes()
        data = bytearray(baseline)
        data[0x220:0x225] = b'\xe8' + struct.pack('<i', 0x450000 - 0x401025)
        self.image.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'zero-field'):
            self.compare()
        self.image.write_bytes(baseline)
        target = PEImage(self.image)
        original = target.read_address
        target.read_address = lambda address, size: b'\xcc'*size if address == 0x401020 else original(address, size)
        with patch.object(self.c, 'verified_target', return_value=target):
            # Direct callee check avoids replacing the parent test's target factory.
            with patch.object(self.c, 'ROOT', self.root):
                with self.assertRaisesRegex(ValueError, 'body differs'):
                    self.c.static_call_target(self.unit, self.field, self.out, PEImage(self.image), [], target)

    def test_call_cannot_be_a_jump(self):
        data = bytearray(self.image.read_bytes()); data[0x200] = 0xe9; self.image.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'direct CALL'):
            self.compare()


if __name__ == '__main__':
    unittest.main()
