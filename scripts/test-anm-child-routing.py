#!/usr/bin/env python3
"""Target-independent fixtures for the bounded ANM child call-route checker."""
from pathlib import Path
import importlib.util
import itertools
import struct

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location(
    "anm_child_routes", ROOT / "scripts/report-anm-child-routing.py")
routing = importlib.util.module_from_spec(spec)
spec.loader.exec_module(routing)

BASE = 0x1000
GETTER, RESOLVER, INSERT = 0x2000, 0x2010, 0x2020
VARIANTS = {88: 0, 90: 1, 91: 2, 92: 3}
FACTORIES = {op: 0x3000 + variant * 0x10 for op, variant in VARIANTS.items()}
HELPERS = [GETTER, RESOLVER, INSERT]


class Image:
    def __init__(self, code):
        self.code = bytes(code)

    def read_address(self, address, size):
        offset = address - BASE
        if offset < 0 or offset + size > len(self.code):
            raise ValueError("fixture read outside its live buffer")
        return self.code[offset:offset + size]


def fixture(order=(88, 91, 90, 92)):
    code = bytearray()
    labels, fields, fixups = {}, {}, []

    def relative(prefix, destination, key=None):
        code.extend(prefix)
        offset = len(code)
        code.extend(b"\0" * 4)
        fixups.append((offset, destination))
        if key:
            fields[key] = offset

    for opcode in order:
        labels[opcode] = BASE + len(code)
        code.extend(b"\x85\xc0")  # TEST EAX,EAX: an unconstrained fixture predicate.
        relative(b"\x0f\x84", ("fast", opcode))
        relative(b"\xe8", GETTER, ("getter", opcode))
        labels[("fast", opcode)] = BASE + len(code)
        relative(b"\xe8", FACTORIES[opcode], ("creator", opcode))
        relative(b"\xe9", ("tail", opcode in (91, 92)), ("tail", opcode))
    for family in (False, True):
        labels[("tail", family)] = BASE + len(code)
        relative(b"\xe8", RESOLVER, ("resolver", family))
        relative(b"\xe8", INSERT, ("insert", family))
        relative(b"\xe9", 0x9000)
    upper = BASE + len(code)
    for offset, destination in fixups:
        if isinstance(destination, tuple):
            destination = labels[destination]
        code[offset:offset + 4] = struct.pack("<i", destination - BASE - offset - 4)
    table = [upper] * 94
    for opcode in order:
        table[opcode + 1] = labels[opcode]
    return code, table, fields, labels


def patch_call(code, offset, destination):
    result = bytearray(code)
    result[offset:offset + 4] = struct.pack("<i", destination - BASE - offset - 4)
    return result


def require_rejection(name, expected_error, operation):
    try:
        operation()
    except ValueError as error:
        assert expected_error in str(error), (name, error)
    else:
        raise AssertionError(f"{name}: invalid routing accepted")


layouts = 0
for order in itertools.permutations(VARIANTS):
    code, table, fields, labels = fixture(order)
    result = routing.inspect_routes(Image(code), table, FACTORIES, HELPERS)
    assert len(result["routes"]) == 4
    for row in result["routes"]:
        creator = FACTORIES[row["opcode"]]
        assert row["static_paths"] == 2
        assert {tuple(p["calls"]) for p in row["paths"]} == {
            tuple(f"0x{x:08X}" for x in path) for path in (
                (creator, RESOLVER, INSERT), (GETTER, creator, RESOLVER, INSERT))}
    layouts += 1
assert layouts == 24

actual = routing.source_factories((ROOT / "src/AnmManager.cpp").read_text())
assert actual == {op: f"CreateVmVariant{variant}" for op, variant in VARIANTS.items()}
source = "\n".join(
    f"case ANM_OP_CREATE_CHILD_{op}: {{ auto id = loaded->CreateVmVariant{variant}(x,y); }}"
    for op, variant in VARIANTS.items())
assert routing.source_factories(source + "\n// CreateVmVariant3(ignored);\n") == actual
code, table, fields, labels = fixture()

def check(changed_code=code, changed_table=table, expected=FACTORIES):
    return routing.inspect_routes(Image(changed_code), changed_table, expected, HELPERS)

wrong_source = source.replace("CreateVmVariant2(x,y)", "CreateVmVariant1(x,y)")
wrong_names = routing.source_factories(wrong_source)
wrong_factories = {op: 0x3000 + int(name[-1]) * 0x10 for op, name in wrong_names.items()}
require_rejection("source creator swap", "child calls differ",
                  lambda: check(expected=wrong_factories))
require_rejection("native creator swap", "child calls differ", lambda: check(
    patch_call(code, fields[("creator", 91)], FACTORIES[90])))
require_rejection("shared resolver replaced", "child calls differ", lambda: check(
    patch_call(code, fields[("resolver", True)], FACTORIES[88])))
require_rejection("layer link replaced", "child calls differ", lambda: check(
    patch_call(code, fields[("insert", False)], RESOLVER)))
require_rejection("unknown optional call", "child calls differ", lambda: check(
    patch_call(code, fields[("getter", 88)], 0x7777)))
indirect = bytearray(code)
offset = fields[("creator", 88)] - 1
indirect[offset:offset + 5] = b"\xff\xd0\x90\x90\x90"
require_rejection("indirect call", "indirect control flow", lambda: check(indirect))
require_rejection("internal call", "internal child CALL", lambda: check(
    patch_call(code, fields[("creator", 88)], labels[("tail", False)])))
require_rejection("instruction interior", "instruction interior", lambda: check(
    patch_call(code, fields[("tail", 88)], labels[91] + 1)))
require_rejection("cycle", "contains a cycle", lambda: check(
    patch_call(code, fields[("tail", 88)], labels[88])))
require_rejection("missing source case", "does not have one case", lambda:
                  routing.source_factories(source.replace("CHILD_91", "CHILD_93")))
require_rejection("ambiguous source creator", "does not have one creator", lambda:
                  routing.source_factories(source.replace(
                      "CreateVmVariant0(x,y);",
                      "CreateVmVariant0(x,y); loaded->CreateVmVariant1(x,y);")))
split_table = list(table)
split_table[2] = BASE + 2
require_rejection("noncontiguous cases", "contiguous physical group",
                  lambda: check(changed_table=split_table))
require_rejection("undecoded suffix", "completely decoded", lambda:
                  routing.call_paths(Image(code + b"\x0f"), labels[88],
                                     BASE, BASE + len(code) + 1))
unsupported = bytearray(code)
offset = labels[88] - BASE
unsupported[offset:offset + 2] = b"\xe2\xfe"
require_rejection("unmodeled loop", "unsupported child control flow",
                  lambda: check(unsupported))
halt = bytearray(code)
halt[offset:offset + 2] = b"\xf4\x90"
require_rejection("unmodeled halt", "unsupported child control flow",
                  lambda: check(halt))
far_call = bytearray(code)
far_call[offset:offset + 8] = b"\x9a\x00\x40\x00\x00\x00\x00\x90"
require_rejection("far call", "unsupported child control flow",
                  lambda: check(far_call))
print("PASS: 24 layouts / 192 static paths, real source routing, 16 strict negative fixtures")
