#!/usr/bin/env python3
"""Review target spell stage/chapter addresses independently of source names."""

from __future__ import annotations

import argparse
import json
import struct
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG, X86_REG_EAX

from linked_image import PEImage
from target_identity import resolve_target, verify_target


def review() -> dict[str, object]:
    path = resolve_target()
    observed, problems = verify_target(path)
    if problems:
        raise ValueError("; ".join(problems))
    image = PEImage(path)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True

    def decode(address: int, size: int):
        instructions = list(decoder.disasm(image.read_address(address, size), address))
        if sum(instruction.size for instruction in instructions) != size:
            raise ValueError(f"incomplete target decode at {address:#x}")
        return instructions

    selector = image.read_address(0x00411EE0 + 0x158 - 0x100, 1)[0]
    if selector >= 108:
        raise ValueError("chapter opcode selector leaves the dispatcher table")
    case_address = struct.unpack("<I", image.read_address(0x00411D30 + selector * 4, 4))[0]
    # This complete target case ends in its own dispatcher return.
    caller = decode(case_address, 44)
    calls = [index for index, instruction in enumerate(caller)
             if instruction.mnemonic == "call" and instruction.operands[0].type == X86_OP_IMM
             and instruction.operands[0].imm == 0x00412ED0]
    if len(calls) != 1 or calls[0] == 0:
        raise ValueError("chapter case lacks its unique setter call")
    receiver = caller[calls[0] - 1]
    if (receiver.mnemonic != "mov" or receiver.operands[0].type != X86_OP_REG
            or receiver.operands[0].reg != X86_REG_EAX
            or receiver.operands[1].type != X86_OP_IMM):
        raise ValueError("chapter setter receiver is not an immediate EAX address")
    receiver_address = receiver.operands[1].imm
    setter = decode(0x00412ED0, 16)
    expected = [("cmp", "dword ptr [eax + 0x44], ecx"),
                ("mov", "dword ptr [eax + 0x44], ecx"),
                ("je", "0x412edf"),
                ("mov", "dword ptr [eax + 0x4c], 0"), ("ret", "")]
    if [(instruction.mnemonic, instruction.op_str) for instruction in setter] != expected:
        raise ValueError("chapter setter offsets or conditional timer reset changed")

    references = []
    for instruction in decode(0x00409280, 2403):
        for operand in instruction.operands:
            if (operand.type == X86_OP_MEM and operand.mem.base == 0
                    and operand.mem.index == 0 and operand.mem.disp in (0x00474C7C, 0x00474C84)):
                references.append({"address": hex(instruction.address), "instruction": instruction.mnemonic,
                                   "operands": instruction.op_str, "global": hex(operand.mem.disp)})
    if receiver_address != 0x00474C40 or [row["global"] for row in references] != [
            "0x474c7c", "0x474c7c", "0x474c84"]:
        raise ValueError("target stage/chapter scalar relationship changed")
    return {"result": "passed", "target_sha256": observed["sha256"],
            "acceptance_authority": "target scalar/address constraints only; no codegen or ownership claim",
            "set_chapter_call": hex(caller[calls[0]].address),
            "partial_view_receiver": hex(receiver_address),
            "stage": {"global": "0x474c7c", "view_offset": "0x3c"},
            "chapter": {"global": "0x474c84", "view_offset": "0x44"},
            "chapter_timer": {"global": "0x474c8c", "view_offset": "0x4c"},
            "spell_references": references}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        report = review()
    except (OSError, ValueError) as error:
        print(f"error: spell-state review failed: {error}", file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print("spell-state target review passed: stage +0x3C and chapter +0x44 are distinct")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
