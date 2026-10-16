#!/usr/bin/env python3
"""Synthetic read-only recognizer cases; use scripts/repo-python from repo root."""
import importlib.util
from pathlib import Path
import struct
import sys
sys.dont_write_bytecode = True
spec = importlib.util.spec_from_file_location(
    "options_topology", Path(__file__).with_name("report-options-owner-topology.py"))
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)


def run_cases(module):
    import capstone
    import tomllib
    module.verify_capstone(tomllib.loads(Path("config/tools.lock.toml").read_text())["capstone"])
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    base = 0x401000

    class Image:
        def __init__(self, raw):
            self.raw = raw
        def read_address(self, address, size):
            assert address == base and size == len(self.raw)
            return self.raw

    cases = [
        ("adjacent", b"", True, False),
        ("pushes_and_other_register_mov", bytes.fromhex("565789d9"), True, False),
        ("twelve_nops", b"\x90" * 12, True, False),
        ("thirteen_nops", b"\x90" * 13, False, False),
        ("index_write", bytes.fromhex("89d8"), False, False),
        ("low_byte_write", bytes.fromhex("b001"), False, False),
        ("high_byte_write", bytes.fromhex("b401"), False, False),
        ("word_write", bytes.fromhex("66b80100"), False, False),
        ("implicit_index_write", bytes.fromhex("93"), False, False),
        ("intervening_jump", bytes.fromhex("eb00"), False, False),
        ("intervening_call", bytes.fromhex("e800000000"), False, False),
        ("intervening_ret", bytes.fromhex("c3"), False, False),
        ("bad_table_targets", b"", False, True),
    ]
    cases = [(name, b"", body, expected, bad) for name, body, expected, bad in cases]
    cases += [
        ("pre_guard_saved_pushes", bytes.fromhex("5657"), bytes.fromhex("be0600000031db"), True, False),
        ("four_pre_guard_pushes", bytes.fromhex("56575657"), b"", True, False),
        ("five_pre_guard_pushes", bytes.fromhex("5657565756"), b"", False, False),
        ("pre_guard_add", bytes.fromhex("83c101"), b"", False, False),
        ("pre_guard_inc", bytes.fromhex("41"), b"", False, False),
        ("pre_guard_popf", bytes.fromhex("9d"), b"", False, False),
        ("pre_guard_other_mov", bytes.fromhex("89d9"), b"", False, False),
    ]
    for name, before_guard, body, expected, bad_table in cases:
        default = base + 12 + len(before_guard) + len(body)
        table = default + 1
        raw = (b"\x83\xf8\x04" + before_guard + b"\x77" + bytes([7 + len(body)]) + body
               + b"\xff\x24\x85" + struct.pack("<I", table) + b"\xc3"
               + struct.pack("<5I", *([default + int(bad_table)] * 5)))
        result = module.switch_tail(Image(raw), base, len(raw), decoder)
        assert bool(result) == expected, (name, result)
    print(f"PASS: {len(cases)} synthetic switch-tail cases")


if __name__ == "__main__":
    run_cases(module)
