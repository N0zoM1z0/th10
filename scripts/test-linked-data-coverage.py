#!/usr/bin/env python3
"""Target-independent coverage and selector-layout regression controls.

Synthetic PE/MAP/PDB fixtures compare every byte, including unlinked data.
The decoder-stop seam models a real stop at an owned jump-table boundary.
No original executable or reconstruction artifact is modified.
"""
import copy
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


class RunTableLayout(unittest.TestCase):
    def setUp(self):
        self.d = module("diagnose-ecl-run-native.py")
        d = self.d
        self.base = 0x501000
        def field(offset, relative):
            return dict(offset=offset, width=4, type="DIR32",
                        candidate_target=hex(self.base + relative),
                        target_candidate=hex(d.RUN_ADDRESS + relative))
        self.report = dict(candidate_address=hex(self.base), decoded_candidate_bytes=7019,
                           normalization_complete=False,
                           linkage_fields=[field(97, d.RUN_SELECTOR_OFFSET),
                                           field(104, d.RUN_CODE_SIZE)] +
                           [field(d.RUN_CODE_SIZE + i * 4, i * 4) for i in range(60)])
        self.selector = bytes(i % 60 for i in range(88))

    def spans(self):
        outer = self
        class Image:
            def read_address(self, address, size):
                outer.assertEqual(address, outer.base + outer.d.RUN_SELECTOR_OFFSET)
                outer.assertEqual(size, 88)
                return outer.selector
        return self.d.run_data_spans(Image(), self.report)

    def test_last_selector_byte(self):
        self.assertEqual(self.spans(), [{"offset": 7019, "width": 1}])

    def test_stop_at_or_inside_jump_table(self):
        for decoded in (6692, 6693, 6932):
            with self.subTest(decoded=decoded):
                self.report["decoded_candidate_bytes"] = decoded
                self.assertEqual(self.spans(), [{"offset": 6932, "width": 88}])

    def test_dense_decode_needs_no_annotation(self):
        self.report.update(decoded_candidate_bytes=7020, normalization_complete=True)
        self.assertEqual(self.spans(), [])

    def test_stopping_inside_code_is_rejected(self):
        self.report["decoded_candidate_bytes"] = 6691
        with self.assertRaisesRegex(ValueError, "executable body"):
            self.spans()

    def test_shifted_reference_is_rejected(self):
        for key in ("candidate_target", "target_candidate"):
            with self.subTest(side=key):
                original = copy.deepcopy(self.report)
                field = self.report["linkage_fields"][0]
                field[key] = hex(int(field[key], 0) + 4)
                with self.assertRaisesRegex(ValueError, "dispatch-table reference"):
                    self.spans()
                self.report = original

    def test_missing_or_duplicate_table_entry_is_rejected(self):
        self.report["linkage_fields"][-1]["offset"] -= 4
        with self.assertRaisesRegex(ValueError, "60-entry"):
            self.spans()

    def test_noncode_destination_is_rejected(self):
        self.report["linkage_fields"][-1]["candidate_target"] = hex(
            self.base + self.d.RUN_CODE_SIZE)
        with self.assertRaisesRegex(ValueError, "executable body"):
            self.spans()

    def test_link_field_inside_selectors_is_rejected(self):
        field = copy.deepcopy(self.report["linkage_fields"][-1])
        field["offset"] = self.d.RUN_SELECTOR_OFFSET
        self.report["linkage_fields"].append(field)
        with self.assertRaisesRegex(ValueError, "selector data.*link field"):
            self.spans()

    def test_selector_domain_is_checked(self):
        self.selector = bytes([60]) + self.selector[1:]
        with self.assertRaisesRegex(ValueError, "selector data.*domain"):
            self.spans()


if __name__ == "__main__":
    unittest.main()
