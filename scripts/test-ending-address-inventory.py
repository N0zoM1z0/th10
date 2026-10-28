#!/usr/bin/env python3
"""Diagnostic address recognition must include BSS without inventing bytes."""
import runpy
from pathlib import Path
from types import SimpleNamespace

ROOT = Path(__file__).resolve().parents[1]
module = runpy.run_path(str(ROOT / 'scripts/audit-ending-run-native.py'))
section = module['mapped_section']
inventory = module['inventory_run']


class Image:
    image_base = 0x100000
    def __init__(self, destination):
        import struct
        self.sections = [SimpleNamespace(rva=0x1000, virtual_size=0x1000,
                         raw_size=0x1000, name='.text'),
                         SimpleNamespace(rva=0x3000, virtual_size=0x200,
                         raw_size=0x100, name='.data')]
        # Generic synthetic switch: MOV ECX, address; JMP [EAX*4+table]; RET.
        self.code = (b'\xb9' + struct.pack('<I', destination) + b'\xff\x24\x85'
                     + struct.pack('<I', 0x10100d) + b'\xc3'
                     + struct.pack('<18I', *([0x10100c] * 18)))
    def read_address(self, address, size):
        if 0x101000 <= address and address + size <= 0x101000 + len(self.code):
            return self.code[address-0x101000:address-0x101000+size]
        raise ValueError('synthetic bytes are not file-backed')


def main():
    im = Image(0x103180)
    assert section(im, 0x103180).name == '.data'
    for address in [0x100fff, 0x102000, 0x102800, 0x103200, 0xffffffff]:
        assert section(im, address) is None
    im.sections[1].raw_size = 0x300
    assert section(im, 0x103250) is None  # Raw padding is not a declared virtual address.
    im.sections[1].raw_size = 0x100
    for destination, count in [(0x103080, 20), (0x103180, 20), (0x103200, 19)]:
        image = Image(destination)
        r = inventory(image, 0x101000, len(image.code))
        assert r['linked_field_count'] == count
        assert len(r['virtual_only_immediate_fields']) == (destination == 0x103180)
        assert r['table_words'] == 18 and len(r['table_local_offsets']) == 18
        if destination == 0x103180:
            # The old byte-read heuristic would have dropped this legitimate address.
            try: image.read_address(destination, 1)
            except ValueError: pass
            else: raise AssertionError('fixture unexpectedly supplies BSS bytes')
    print('Ending full-address inventory: file-backed/BSS positives, bounds and raw-padding controls pass')


if __name__ == '__main__':
    main()
