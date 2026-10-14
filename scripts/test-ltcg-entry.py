"""Target-independent regression tests for fail-closed LTCG entry identity."""
import struct
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from ltcg_link import reject_unresolved_entry, validate_link_entry


class EntryIdentityTests(unittest.TestCase):
    def check(self, *, rva=0x1000, flags=None, owner='source.ltcg.obj',
              pdb_owner=None, executable=True, size=32, symbol='_WinMain@16'):
        data = bytearray(256)
        struct.pack_into('<I', data, 0x3c, 64)
        struct.pack_into('<I', data, 64 + 24 + 16, rva)
        image = SimpleNamespace(data=data, image_base=0x400000,
            section=lambda _: SimpleNamespace(characteristics=0x20000000 if executable else 0))
        public = dict(symbol=symbol, address=0x401000, flags=['f'] if flags is None else flags,
                      object=owner, section_number=1)
        function = dict(symbol=symbol, address='0x00401000', size=size,
                        pdb_object=pdb_owner or owner)
        with patch('linked_image.PEImage', return_value=image), \
             patch('linked_image.map_publics', return_value=[public]), \
             patch('linked_image.linked_functions', return_value={'functions': [function]}):
            return validate_link_entry(Path('source.exe'), Path('source.map'),
                                       Path('source.pdb'), 'WinMain@16', [Path('source.ltcg.obj')])

    def test_valid_source_entry(self):
        self.assertEqual(self.check()['symbol'], '_WinMain@16')

    def test_unresolved_entry_cannot_be_anchored(self):
        for entry in ('WinMain@16', '_WinMain@16', '?Root@@YAXXZ'):
            for symbol in (entry, '_' + entry):
                with self.assertRaises(ValueError):
                    reject_unresolved_entry(entry, [symbol])
        reject_unresolved_entry('WinMain@16', ['__imp__Sleep@4'])

    def test_wrong_pe_entry(self):
        with self.assertRaises(ValueError): self.check(rva=0x43)

    def test_anchor_owner(self):
        with self.assertRaises(ValueError): self.check(owner='link-anchors.obj')

    def test_data_public(self):
        with self.assertRaises(ValueError): self.check(flags=[])

    def test_non_executable_section(self):
        with self.assertRaises(ValueError): self.check(executable=False)

    def test_pdb_owner_disagreement(self):
        with self.assertRaises(ValueError): self.check(pdb_owner='link-anchors.obj')

    def test_zero_extent(self):
        with self.assertRaises(ValueError): self.check(size=0)

    def test_wrong_symbol(self):
        with self.assertRaises(ValueError): self.check(symbol='_Other@16')


if __name__ == '__main__':
    unittest.main()
