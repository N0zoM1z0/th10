#!/usr/bin/env python3
"""Reject changed internal destinations before linked-address normalization.

Real PE/MAP/PDB containers are synthetic, target-independent fixtures. No
historical compiler, private target or production reconstruction is modified.
"""
import importlib.util
import io
from pathlib import Path
import struct
import tempfile
import tomllib
import unittest
from unittest.mock import patch

from linked_image import PEImage
from ltcg_link import ROOT


def module(name):
    spec = importlib.util.spec_from_file_location(name.replace('-', '_'), ROOT / 'scripts' / name)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


class LocalDestinations(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory(prefix='linked-local-')
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.out = self.root / 'build/fixture'
        self.out.mkdir(parents=True)
        fixtures = module('test-static-owner-selection.py')
        e = fixtures.Evidence(self.out)
        e.image, e.map, e.pdb = [self.out / ('source.' + x) for x in ['exe', 'map', 'pdb']]
        e.map_base = 0x500000
        e.publics = [fixtures.row('_Local', 0, 'source.ltcg.obj', va=0x501000)]
        e.statics = []
        e.modules = [str(self.out / 'source.ltcg.obj')]
        e.contributions = [(0, 24, fixtures.CODE, 0)]
        e.write()
        data = bytearray(e.image.read_bytes())
        struct.pack_into('<I', data, 0x98 + 28, 0x500000)
        struct.pack_into('<II', data, 0x98 + 96 + 5 * 8, 0x1180, 16)
        struct.pack_into('<II4H', data, 0x380, 0x1000, 16, 0x3001, 0x3010, 0x3014, 0)
        data[0x200:0x218] = (b'\xb8' + struct.pack('<I', 0x501010) +
                             b'\xc3' + b'\x90' * 10 + struct.pack('<II', 0x501006, 0x501007))
        e.image.write_bytes(data)
        self.image = e.image
        self.target_path = self.root / 'target.exe'
        struct.pack_into('<I', data, 0x98 + 28, 0x400000)
        for offset, value in [(1, 0x401010), (16, 0x401006), (20, 0x401007)]:
            struct.pack_into('<I', data, 0x200 + offset, value)
        self.target_path.write_bytes(data)
        self.fields = [dict(offset=o, type='DIR32', local=True, target=t)
                       for o, t in [(1, 0x401010), (16, 0x401006), (20, 0x401007)]]
        self.unit = dict(artifact_kind='linked-pe', source='src/Local.cpp',
                         output_dir='build/fixture', symbol='_Local', target_address=0x401000,
                         functions=['Local'], size=24, linkages=self.fields)
        self.c = module('compare-linked-function.py')

    def pointer(self, offset, destination, target=False):
        path = self.target_path if target else self.image
        data = bytearray(path.read_bytes())
        struct.pack_into('<I', data, 0x200 + offset, destination)
        path.write_bytes(data)

    def compare(self):
        manifest = dict(target_sha256='fixture', units={'local': self.unit})
        with patch.object(self.c, 'ROOT', self.root), \
             patch.object(self.c.tomllib, 'load', side_effect=[manifest, {'target': {'sha256': 'fixture'}},
                                                            tomllib.loads((ROOT/'config/tools.lock.toml').read_text())]), \
             patch.object(self.c, 'verified_target', return_value=PEImage(self.target_path)):
            original = Path.open
            def opened(path, *args, **kwargs):
                if path in (self.c.UNITS_MANIFEST, self.c.TARGET_MANIFEST, self.c.TOOLS_LOCK):
                    return io.BytesIO()
                return original(path, *args, **kwargs)
            with patch.object(Path, 'open', opened):
                return self.c.compare_unit('local')

    def test_relocated_internal_code_and_data_destinations(self):
        report = self.compare()
        self.assertEqual(report['result'], 'exact')
        self.assertFalse(report['raw_equal'])
        self.assertEqual(len(report['linkages']), 3)

    def test_table_permutation_is_rejected(self):
        self.pointer(16, 0x501007)
        self.pointer(20, 0x501006)
        with self.assertRaisesRegex(ValueError, 'local.*offset|local.*destination'):
            self.compare()

    def test_shifted_table_base_is_rejected(self):
        self.pointer(1, 0x501011)
        with self.assertRaisesRegex(ValueError, 'local.*offset|local.*destination'):
            self.compare()

    def test_target_and_candidate_bounds_are_both_checked(self):
        self.pointer(1, 0x501018)
        with self.assertRaisesRegex(ValueError, 'candidate contribution'):
            self.compare()
        self.pointer(1, 0x501010)
        self.fields[0]['target'] = 0x401018
        self.pointer(1, 0x401018, target=True)
        with self.assertRaisesRegex(ValueError, 'target contribution'):
            self.compare()

    def test_addend_does_not_hide_a_different_base(self):
        self.fields[0].update(target=0x40100f, addend=1)
        self.assertEqual(self.compare()['result'], 'exact')
        self.pointer(1, 0x501011)
        with self.assertRaisesRegex(ValueError, 'local.*offset|local.*destination'):
            self.compare()


if __name__ == '__main__':
    unittest.main()
