#!/usr/bin/env python3
"""Target-independent tests for the frontier ratchet in check-frontier.py."""
import contextlib
import importlib.util
import io
import sys
import tempfile
import tomllib
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("check_frontier", ROOT / "scripts/check-frontier.py")
frontier = importlib.util.module_from_spec(spec)
spec.loader.exec_module(frontier)

ENTRY = """[frontier.sample]
function = "Sample"
unit = "transition-shake-linear"
size = 16
candidate_size = 16
target_sha256 = "00"
residual = ["4-5", "9"]
"""


class Ratchet(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.config = Path(self.directory.name) / "frontiers.toml"
        self.config.write_text(ENTRY)
        self.saved = (frontier.CONFIG, frontier.measure, frontier.verified_target)
        frontier.CONFIG = self.config
        frontier.verified_target = lambda: None
        self.result = (16, {4, 5, 9})

        def measure(target, entry):
            size, differing = self.result
            return size, set(differing), b"\0" * 16, b"\0" * size
        frontier.measure = measure

    def tearDown(self):
        frontier.CONFIG, frontier.measure, frontier.verified_target = self.saved
        self.directory.cleanup()

    def run_main(self, *argv):
        out = io.StringIO()
        old = sys.argv
        sys.argv = ["check-frontier.py", "--no-build", *argv]
        try:
            with contextlib.redirect_stdout(out), contextlib.redirect_stderr(out):
                code = frontier.main()
        finally:
            sys.argv = old
        return code, out.getvalue()

    def test_offsets_round_trip(self):
        offsets = {1, 2, 3, 10, 0xFF}
        self.assertEqual(frontier.format_offsets(offsets), ["1-3", "A", "FF"])
        self.assertEqual(frontier.parse_offsets(frontier.format_offsets(offsets)), offsets)

    def test_unchanged_residual_passes(self):
        code, out = self.run_main("sample")
        self.assertEqual(code, 0)
        self.assertIn("OK", out)

    def test_new_difference_regresses_and_is_not_recorded(self):
        self.result = (16, {4, 5, 9, 12})
        code, out = self.run_main("--update", "sample")
        self.assertEqual(code, 1)
        self.assertIn("newly differing offsets: C", out)
        self.assertEqual(self.config.read_text(), ENTRY)

    def test_improvement_is_recorded_with_update(self):
        self.result = (16, {9})
        code, out = self.run_main("sample")
        self.assertEqual(code, 0)
        self.assertIn("IMPROVED", out)
        self.assertEqual(self.config.read_text(), ENTRY)
        code, out = self.run_main("--update", "sample")
        self.assertEqual(code, 0)
        entry = tomllib.loads(self.config.read_text())["frontier"]["sample"]
        self.assertEqual(entry["residual"], ["9"])
        self.assertEqual(entry["function"], "Sample")

    def test_exact_candidate_is_reported(self):
        self.result = (16, set())
        code, out = self.run_main("sample")
        self.assertEqual(code, 0)
        self.assertIn("EXACT", out)

    def test_unknown_name_is_rejected(self):
        code, out = self.run_main("missing")
        self.assertEqual(code, 2)
        self.assertIn("unknown frontier(s): missing", out)


if __name__ == "__main__":
    unittest.main()
