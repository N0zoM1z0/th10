#!/usr/bin/env python3
"""Target-independent coverage and selector-layout regression controls.

Synthetic PE/MAP/PDB fixtures compare every byte, including unlinked data.
The decoder-stop seam models a real stop at an owned jump-table boundary.
No original executable or reconstruction artifact is modified.
"""
import importlib.util
from pathlib import Path
import struct
import unittest
from unittest.mock import patch

from linked_image import PEImage


ROOT = Path(__file__).resolve().parents[1]


def module(name):
    spec = importlib.util.spec_from_file_location(name.replace("-", "_"), ROOT / "scripts" / name)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


class TrailingCoverage(unittest.TestCase):
    def setUp(self):
        self.fixture = module("test-linked-local-fields.py").LocalDestinations()
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)
        f = self.fixture
        # Keep the complete PE field at16; replace the former field at20
        # with four ordinary selector bytes in both synthetic images.
        for path in (f.image, f.target_path):
            data = bytearray(path.read_bytes())
            struct.pack_into("<H", data, 0x38C, 0)
            data[0x214:0x218] = bytes([1, 2, 3, 4])
            path.write_bytes(data)
        f.fields.pop()
        f.unit["data_spans"] = [{"offset": 20, "width": 4}]
        self.report = f.c.linked_code_fields(PEImage(f.image), [], 0x501000, 24)
        self.assertEqual(len(self.report["fields"]), 2)
        self.report.update(decoded_bytes=16, normalization_complete=False)

    def compare(self):
        with patch.object(self.fixture.c, "linked_code_fields", return_value=self.report):
            return self.fixture.compare()

    def test_pe_gap_and_unlinked_suffix_cover_the_whole_owner(self):
        result = self.compare()
        self.assertEqual(result["result"], "exact")
        self.assertEqual(result["matched_bytes"], 24)
        self.assertEqual(result["compared_size"], 24)
        self.assertEqual(len(result["linkages"]), 2)

    def test_unlinked_data_is_compared(self):
        path = self.fixture.image
        data = bytearray(path.read_bytes())
        data[0x215] = 9
        path.write_bytes(data)
        result = self.compare()
        self.assertEqual(result["result"], "mismatch")
        self.assertEqual(result["matched_bytes"], 23)
        self.assertEqual(result["first_differences"][0]["offset"], "0x15")

    def test_selectors_before_a_second_jump_table(self):
        f = self.fixture
        for path, destination in ((f.image, 0x501007), (f.target_path, 0x401007)):
            data = bytearray(path.read_bytes())
            struct.pack_into("<HH", data, 0x38A, 0x3014, 0)
            data[0x210:0x214] = bytes([1, 2, 3, 4])
            struct.pack_into("<I", data, 0x214, destination)
            path.write_bytes(data)
        f.fields[-1].update(offset=20, target=0x401007)
        f.unit["data_spans"] = [{"offset": 16, "width": 4}]
        self.report = f.c.linked_code_fields(PEImage(f.image), [], 0x501000, 24, 16)
        builder = module("build-match-unit.py")
        builder.validate_fields("local", f.unit, 24)
        self.assertEqual(self.compare()["result"], "exact")

        f.pointer(20, 0x501006)
        self.report["fields"][-1]["candidate_target"] = 0x501006
        with self.assertRaisesRegex(ValueError, "local.*destination"):
            self.compare()

        f.fields.pop()
        with self.assertRaisesRegex(ValueError, "uncovered tail"):
            builder.validate_fields("local", f.unit, 24)

    def test_missing_pe_word_leaves_an_uncovered_gap(self):
        self.report["fields"].pop()
        with self.assertRaisesRegex(ValueError, "do not cover.*undecoded"):
            self.compare()

    def test_relative_field_cannot_cover_the_gap(self):
        self.report["fields"][-1]["type"] = "REL32"
        with self.assertRaisesRegex(ValueError, "do not cover.*undecoded"):
            self.compare()

    def test_partial_pe_word_cannot_cover_the_gap(self):
        self.report["fields"][-1]["width"] = 2
        with self.assertRaisesRegex(ValueError, "do not cover.*undecoded"):
            self.compare()

    def test_annotation_cannot_overlap_decoded_bytes(self):
        self.report["decoded_bytes"] = 22
        with self.assertRaisesRegex(ValueError, "overlap.*decoded"):
            self.compare()

    def test_missing_data_annotation_is_rejected(self):
        self.fixture.unit.pop("data_spans")
        with self.assertRaisesRegex(ValueError, "Capstone decoded"):
            self.compare()


class _FlatImage:
    """Minimal image stand-in: bytes at one address and no base relocations."""
    def __init__(self, address, code):
        self.address, self.code = address, code
    def read_address(self, address, size):
        start = address - self.address
        return self.code[start:start + size]
    def base_relocations(self):
        return []


class DeclaredDataStopsDecoding(unittest.TestCase):
    # ret; then five data bytes that would decode as CALL rel32 out of extent.
    CODE = bytes([0xC3, 0xE8, 0x00, 0x10, 0x00, 0x00])

    def fields(self, limit):
        from linked_image import linked_code_fields
        return linked_code_fields(_FlatImage(0x401000, self.CODE), [], 0x401000,
                                  len(self.CODE), limit)

    def test_unlimited_decode_reports_a_false_call_field(self):
        report = self.fields(None)
        self.assertEqual(report["decoded_bytes"], 6)
        self.assertEqual([f["type"] for f in report["fields"]], ["REL32"])

    def test_declared_data_ends_the_instruction_stream(self):
        report = self.fields(1)
        self.assertEqual(report["decoded_bytes"], 1)
        self.assertEqual(report["fields"], [])
        self.assertFalse(report["normalization_complete"])

    def test_limit_outside_the_extent_is_rejected(self):
        from linked_image import LinkedImageError
        with self.assertRaises(LinkedImageError):
            self.fields(7)

if __name__ == "__main__":
    unittest.main()
