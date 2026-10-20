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

    def tail_jump(self):
        data = bytearray(self.image.read_bytes())
        data[0x200] = 0xe9
        self.image.write_bytes(data)
        self.field['static_tail_jump'] = True
        (self.root / 'config/functions.csv').write_text(
            'address,size,source_file,proposed_name\n'
            '0x00401020,8,src/Private.cpp,LocalOwner\n')

    def test_explicit_tail_jump_binds_complete_callee(self):
        self.tail_jump()
        report = self.compare()
        self.assertEqual(report['result'], 'exact')
        self.assertTrue(report['linkages'][0]['static_tail_jump'])
        self.assertEqual(report['linkages'][0]['static_owner']['size'], 8)

    def test_tail_jump_requires_literal_true_and_complete_descriptor(self):
        for value in (False, None, 0, 1, 'true'):
            self.field['static_tail_jump'] = value
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, 'explicitly true'):
                self.compare()
        self.field['static_tail_jump'] = True
        for missing in ('static_source', 'static_size'):
            saved = self.field.pop(missing)
            with self.subTest(missing=missing), self.assertRaisesRegex(ValueError, 'source and whole size'):
                self.compare()
            self.field[missing] = saved

    def test_tail_jump_opt_in_rejects_call_and_conditional_branch(self):
        self.field['static_tail_jump'] = True
        with self.assertRaisesRegex(ValueError, 'direct JMP'):
            self.compare()
        data = bytearray(self.image.read_bytes())
        data[0x200:0x208] = b'\x0f\x85' + struct.pack('<i', 0x20 - 6) + b'\xc3\x90'
        self.image.write_bytes(data)
        self.field['offset'] = 2
        with self.assertRaisesRegex(ValueError, 'direct JMP'):
            self.compare()

    def test_tail_jump_rejects_interior_candidate_destination(self):
        self.tail_jump()
        data = bytearray(self.image.read_bytes())
        struct.pack_into('<i', data, 0x201, 0x21 - 5)
        self.image.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'enter its qualified callee'):
            self.compare()

    def test_tail_jump_preserves_callee_byte_and_boundary_checks(self):
        self.tail_jump()
        self.test_static_callee_bytes_and_fields_checked()
        self.test_target_boundary_cannot_be_cropped_or_provisional()

    def test_tail_jump_requires_target_source_and_name(self):
        self.tail_jump()
        for source, name in [('src/Wrong.cpp', 'LocalOwner'),
                             ('src/Private.cpp', 'Other'), ('src/Private.cpp', '')]:
            (self.root / 'config/functions.csv').write_text(
                f'address,size,source_file,proposed_name\n0x00401020,8,{source},{name}\n')
            with self.subTest(source=source, name=name), self.assertRaisesRegex(ValueError, 'source mapping'):
                self.compare()

    def test_cached_call_does_not_bypass_tail_mapping_check(self):
        fixtures = module('test-static-owner-selection.py')
        evidence = fixtures.Evidence(self.out)
        evidence.image, evidence.map, evidence.pdb = [self.out / ('source.' + x) for x in ['exe', 'map', 'pdb']]
        evidence.publics = [fixtures.row('_Public', 0, 'source.ltcg.obj')]
        evidence.statics = [fixtures.row(obj='support-00-Private.ltcg.obj')]
        evidence.modules = [str(self.out / 'source.ltcg.obj'), str(self.out / 'support-00-Private.ltcg.obj')]
        evidence.contributions[0] = (0, 12, fixtures.CODE, 0)
        evidence.write()
        data = bytearray(evidence.image.read_bytes())
        data[0x200:0x20c] = (b'\xe8' + struct.pack('<i', 0x20 - 5)
                             + b'\xe9' + struct.pack('<i', 0x20 - 10) + b'\x90\x90')
        evidence.image.write_bytes(data)
        self.unit['size'] = 12
        self.unit['linkages'] = [dict(self.field), dict(self.field, offset=6, static_tail_jump=True)]
        with self.assertRaisesRegex(ValueError, 'source mapping'):
            self.compare()
        (self.root / 'config/functions.csv').write_text(
            'address,size,source_file,proposed_name\n0x00401020,8,src/Private.cpp,LocalOwner\n')
        report = self.compare()
        self.assertEqual(report['result'], 'exact')
        self.assertNotIn('static_tail_jump', report['linkages'][0])
        self.assertTrue(report['linkages'][1]['static_tail_jump'])


class StaticAddresses(unittest.TestCase):
    """A relocated callback pointer binds an entry, not the callee's codegen."""

    boundary = StaticCalls.boundary

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix='linked-address-')
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.out = self.root / 'build/fixture'
        self.out.mkdir(parents=True)
        fixtures = module('test-static-owner-selection.py')
        e = fixtures.Evidence(self.out)
        e.image, e.map, e.pdb = [self.out / ('source.' + x) for x in ['exe', 'map', 'pdb']]
        e.map_base = 0x500000
        e.publics = [fixtures.row('_Public', 0, 'source.ltcg.obj', va=0x501000)]
        e.statics = [fixtures.row(obj='support-00-Private.ltcg.obj', va=0x501020)]
        e.modules = [str(self.out / 'source.ltcg.obj'), str(self.out / 'support-00-Private.ltcg.obj')]
        e.write()
        data = bytearray(e.image.read_bytes())
        struct.pack_into('<I', data, 0x98 + 28, 0x500000)
        struct.pack_into('<II', data, 0x98 + 96 + 5 * 8, 0x1180, 12)
        struct.pack_into('<II2H', data, 0x380, 0x1000, 12, 0x3001, 0)
        data[0x200:0x208] = b'\xb8' + struct.pack('<I', 0x501020) + b'\xc3\x90\x90'
        e.image.write_bytes(data)
        self.image = e.image
        self.target_path = self.root / 'target.exe'
        struct.pack_into('<I', data, 0x98 + 28, 0x400000)
        struct.pack_into('<I', data, 0x201, 0x401020)
        # Intentionally different complete callee. Pointer exactness must not
        # silently promote the callback's eight bytes.
        data[0x220:0x228] = b'\x40\xc3' + b'\x90' * 6
        self.target_path.write_bytes(data)
        (self.root / 'config').mkdir()
        (self.root / 'config/functions.csv').write_text(
            'address,size,proposed_name,source_file\n0x00401020,8,LocalOwner,src/Private.cpp\n')
        self.boundary('reviewed', 0x401027)
        self.field = dict(offset=1, type='DIR32', symbol=fixtures.SYMBOL, target=0x401020,
                          static_address_source='src/Private.cpp',
                          static_address_candidate_size=8, static_address_target_size=8)
        self.unit = dict(artifact_kind='linked-pe', source='src/Public.cpp',
                         support_sources=['src/Private.cpp'], output_dir='build/fixture',
                         functions=['Public'], symbol='_Public', target_address=0x401000,
                         size=8, linkages=[self.field])
        self.c = module('compare-linked-function.py')

    def compare(self):
        manifest = dict(target_sha256='fixture', units={'caller': self.unit})
        original = Path.open
        def opened(path, *args, **kwargs):
            if path in (self.c.UNITS_MANIFEST, self.c.TARGET_MANIFEST, self.c.TOOLS_LOCK):
                return io.BytesIO()
            return original(path, *args, **kwargs)
        with patch.object(self.c, 'ROOT', self.root), \
             patch.object(self.c.tomllib, 'load', side_effect=[manifest, {'target': {'sha256': 'fixture'}},
                                                            tomllib.loads((ROOT/'config/tools.lock.toml').read_text())]), \
             patch.object(self.c, 'verified_target', return_value=PEImage(self.target_path)), \
             patch.object(Path, 'open', opened):
            return self.c.compare_unit('caller')

    def test_qualified_address_and_parent_only_credit(self):
        report = self.compare()
        self.assertEqual(report['result'], 'exact')
        self.assertEqual(report['matched_bytes'], 8)
        self.assertFalse(report['raw_equal'])
        field = report['linkages'][0]
        self.assertEqual(field['static_address_owner']['size'], 8)
        self.assertTrue(field['static_address_binding_only'])

    def test_no_implicit_fallback_or_incomplete_descriptor(self):
        saved = dict(self.field)
        for key in ['static_address_source', 'static_address_candidate_size', 'static_address_target_size']:
            del self.field[key]
            with self.subTest(key=key), self.assertRaises(ValueError):
                self.compare()
            self.field.clear(); self.field.update(saved)
        for key in ['static_address_source', 'static_address_candidate_size', 'static_address_target_size']:
            del self.field[key]
        with self.assertRaisesRegex(ValueError, 'resolve uniquely'):
            self.compare()

    def test_reject_types_mixed_modes_sizes_and_unqualified_sources(self):
        for changes in [dict(type='REL32'), dict(local=True), dict(addend=1),
                        dict(static_source='src/Private.cpp', static_size=8),
                        dict(static_address_candidate_size=7), dict(static_address_candidate_size=True),
                        dict(static_address_target_size=7), dict(static_address_target_size=False),
                        dict(static_address_source='src/Absent.cpp'),
                        dict(static_address_source='../Private.cpp')]:
            saved = dict(self.field); self.field.update(changes)
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                self.compare()
            self.field.clear(); self.field.update(saved)

    def test_reviewed_boundary_and_source_mapping_are_required(self):
        for state, end in [('provisional', 0x401027), ('reviewed', 0x401026)]:
            self.boundary(state, end)
            with self.subTest(state=state, end=end), self.assertRaisesRegex(ValueError, 'complete reviewed'):
                self.compare()
        self.boundary('reviewed', 0x401027)
        for value in ['', 'src/Public.cpp']:
            (self.root / 'config/functions.csv').write_text(
                f'address,size,proposed_name,source_file\n0x00401020,8,LocalOwner,{value}\n')
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, 'source mapping'):
                self.compare()
        (self.root / 'config/functions.csv').write_text(
            'address,size,proposed_name,source_file\n0x00401020,8,OtherCallback,src/Private.cpp\n')
        with self.assertRaisesRegex(ValueError, 'source name'):
            self.compare()

    def test_entry_cannot_be_interior(self):
        data = bytearray(self.image.read_bytes()); struct.pack_into('<I', data, 0x201, 0x501021)
        self.image.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'qualified function entry'):
            self.compare()

    def test_complete_dense_candidate_and_nonexact_callee_are_independent(self):
        data = bytearray(self.image.read_bytes()); data[0x227] = 0x0f; self.image.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'complete dense'):
            self.compare()
        data[0x220:0x228] = b'\xe8' + struct.pack('<i', 0x10) + b'\xc3\x90\x90'
        self.image.write_bytes(data)
        self.assertTrue(self.compare()['linkages'][0]['static_address_binding_only'])

    def test_independent_whole_target_size_and_dense_target(self):
        self.field['static_address_target_size'] = 9
        (self.root / 'config/functions.csv').write_text(
            'address,size,proposed_name,source_file\n0x00401020,9,LocalOwner,src/Private.cpp\n')
        self.boundary('reviewed', 0x401028)
        report = self.compare()
        self.assertEqual(report['linkages'][0]['static_address_owner']['size'], 8)
        self.assertEqual(report['linkages'][0]['static_address_target_size'], 9)
        data = bytearray(self.target_path.read_bytes()); data[0x228] = 0x0f
        self.target_path.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'target.*complete dense'):
            self.compare()


if __name__ == '__main__':
    unittest.main()
