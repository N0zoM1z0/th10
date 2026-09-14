#!/usr/bin/env python3
"""Compare one normal VC7.1 COFF function against the attested TH10 target."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import tomllib


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th10.exe"
TARGET_MANIFEST = ROOT / "config" / "target.toml"
UNITS_MANIFEST = ROOT / "config" / "match-units.toml"
RELOCATION_IDS = {"DIR32": 0x0006, "REL32": 0x0014}


def c_string(data: bytes) -> str:
    return data.split(b"\0", 1)[0].decode("ascii")


def symbol_name(raw: bytes, strings: bytes) -> str:
    if raw[:4] == b"\0\0\0\0":
        offset = struct.unpack_from("<I", raw, 4)[0]
        if not 4 <= offset < len(strings):
            raise ValueError(f"invalid COFF string-table offset {offset}")
        return c_string(strings[offset:])
    return c_string(raw)


def coff_object(
    path: Path,
) -> tuple[bytes, list[dict[str, object]], dict[int, dict[str, object]]]:
    data = path.read_bytes()
    if len(data) < 20:
        raise ValueError("truncated COFF object")
    machine, section_count, _, symbol_offset, symbol_count, optional_size, _ = struct.unpack_from(
        "<HHIIIHH", data, 0
    )
    if machine != 0x14C or optional_size != 0:
        raise ValueError("expected a normal i386 COFF object, not LTCG or PE")
    sections = []
    for index in range(section_count):
        offset = 20 + index * 40
        if offset + 40 > len(data):
            raise ValueError("truncated COFF section table")
        fields = struct.unpack_from("<8sIIIIIIHHI", data, offset)
        sections.append(
            {
                "name": c_string(fields[0]),
                "size": fields[3],
                "raw_offset": fields[4],
                "reloc_offset": fields[5],
                "reloc_count": fields[7],
                "flags": fields[9],
            }
        )
    strings_offset = symbol_offset + symbol_count * 18
    if strings_offset + 4 > len(data):
        raise ValueError("truncated COFF symbol table")
    strings_size = struct.unpack_from("<I", data, strings_offset)[0]
    if strings_size < 4 or strings_offset + strings_size > len(data):
        raise ValueError("invalid COFF string table")
    strings = data[strings_offset : strings_offset + strings_size]
    symbols: dict[int, dict[str, object]] = {}
    raw_index = 0
    while raw_index < symbol_count:
        offset = symbol_offset + raw_index * 18
        raw_name, value, section, type_id, storage, aux_count = struct.unpack_from(
            "<8sIhHBB", data, offset
        )
        if section > section_count:
            raise ValueError("COFF symbol references an unknown section")
        if offset + 18 * (aux_count + 1) > strings_offset:
            raise ValueError("truncated COFF auxiliary symbol records")
        aux = data[offset + 18 : offset + 18 * (aux_count + 1)]
        symbols[raw_index] = {
            "name": symbol_name(raw_name, strings),
            "value": value,
            "section": section,
            "type": type_id,
            "storage": storage,
            "aux_count": aux_count,
            "aux": aux,
        }
        raw_index += 1 + aux_count
    return data, sections, symbols


def object_functions(path: Path, contains: str = "") -> list[dict[str, object]]:
    data, sections, symbols = coff_object(path)
    needle = contains.lower()
    defined = [
        symbol
        for symbol in symbols.values()
        if int(symbol["section"]) > 0
        and int(symbol["type"]) == 0x20
        and int(sections[int(symbol["section"]) - 1]["flags"]) & 0x20
    ]
    functions_per_section: dict[int, int] = {}
    for symbol in defined:
        section_number = int(symbol["section"])
        functions_per_section[section_number] = (
            functions_per_section.get(section_number, 0) + 1
        )
    result = []
    for symbol in defined:
        name = str(symbol["name"])
        section_number = int(symbol["section"])
        if needle and needle not in name.lower():
            continue
        section = sections[section_number - 1]
        size = None
        extent_source = None
        if int(symbol["aux_count"]) >= 1 and len(symbol["aux"]) >= 8:
            auxiliary_size = struct.unpack_from("<I", symbol["aux"], 4)[0]
            if auxiliary_size:
                size = auxiliary_size
                extent_source = "function-definition-aux"
        if (
            size is None
            and functions_per_section[section_number] == 1
            and int(symbol["value"]) == 0
            and int(section["flags"]) & 0x1000
        ):
            size = int(section["size"])
            extent_source = "single-function-comdat-section"
        relocation_count = None
        if size is not None:
            start = int(symbol["value"])
            stop = start + size
            relocation_count = 0
            for index in range(int(section["reloc_count"])):
                offset = int(section["reloc_offset"]) + index * 10
                virtual_address = struct.unpack_from("<I", data, offset)[0]
                if start <= virtual_address < stop:
                    relocation_count += 1
        result.append(
            {
                "symbol": name,
                "size": size,
                "extent_source": extent_source,
                "relocation_count": relocation_count,
                "section": section["name"],
                "section_number": section_number,
                "section_offset": int(symbol["value"]),
                "storage_class": int(symbol["storage"]),
            }
        )
    result.sort(
        key=lambda row: (
            int(row["section_number"]),
            int(row["section_offset"]),
            str(row["symbol"]),
        )
    )
    return result


def object_function(
    path: Path, wanted: str
) -> tuple[bytearray, list[dict[str, object]]]:
    data, sections, symbols = coff_object(path)
    matches = [
        symbol
        for symbol in symbols.values()
        if symbol["name"] == wanted and int(symbol["section"]) > 0
    ]
    if len(matches) != 1:
        raise ValueError(f"expected one defined symbol {wanted!r}, found {len(matches)}")
    symbol = matches[0]
    section = sections[int(symbol["section"]) - 1]
    if int(symbol["type"]) != 0x20 or not int(section["flags"]) & 0x20:
        raise ValueError("selected COFF symbol is not a code function")
    size = 0
    if int(symbol["aux_count"]) >= 1 and len(symbol["aux"]) >= 8:
        size = struct.unpack_from("<I", symbol["aux"], 4)[0]
    if not size:
        section_number = int(symbol["section"])
        section_functions = [
            candidate
            for candidate in symbols.values()
            if int(candidate["section"]) == section_number
            and int(candidate["type"]) == 0x20
        ]
        if (
            len(section_functions) == 1
            and int(symbol["value"]) == 0
            and int(section["flags"]) & 0x20
            and int(section["flags"]) & 0x1000
        ):
            size = int(section["size"])
    if not size:
        raise ValueError("function symbol lacks a usable definition extent")
    start = int(section["raw_offset"]) + int(symbol["value"])
    section_end = int(section["raw_offset"]) + int(section["size"])
    if start + size > section_end or section_end > len(data):
        raise ValueError("function extends beyond its COFF section")
    code = bytearray(data[start : start + size])
    relocations = []
    for index in range(int(section["reloc_count"])):
        offset = int(section["reloc_offset"]) + index * 10
        virtual_address, target_index, type_id = struct.unpack_from("<IIH", data, offset)
        local = virtual_address - int(symbol["value"])
        if not 0 <= local < size:
            continue
        if local + 4 > size:
            raise ValueError("relocation leaves the COFF function extent")
        target = symbols.get(target_index)
        if target is None:
            raise ValueError("relocation references an auxiliary symbol")
        relocations.append(
            {
                "offset": local,
                "type_id": type_id,
                "type": next(
                    (
                        name
                        for name, value in RELOCATION_IDS.items()
                        if value == type_id
                    ),
                    f"0x{type_id:04X}",
                ),
                "symbol": target["name"],
            }
        )
    return code, relocations


def verified_target() -> bytes:
    with TARGET_MANIFEST.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != int(expected["size"]) or digest != expected["sha256"]:
        raise ValueError(f"target identity mismatch: size={len(data)} sha256={digest}")
    return data


def target_bytes(data: bytes, address: int, size: int) -> bytes:
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe : pe + 4] != b"PE\0\0":
        raise ValueError("target lacks a PE signature")
    section_count = struct.unpack_from("<H", data, pe + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe + 20)[0]
    optional = pe + 24
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    rva = address - image_base
    for index in range(section_count):
        offset = optional + optional_size + index * 40
        _, virtual_size, section_rva, raw_size, raw_offset = struct.unpack_from(
            "<8sIIII", data, offset
        )
        if section_rva <= rva and rva + size <= section_rva + virtual_size:
            relative = rva - section_rva
            if relative + size > raw_size:
                raise ValueError("target extent leaves section raw data")
            return data[raw_offset + relative : raw_offset + relative + size]
    raise ValueError("target extent is outside PE sections")


def compare_unit(name: str) -> dict[str, object]:
    with UNITS_MANIFEST.open("rb") as stream:
        units = tomllib.load(stream).get("units", {})
    if not isinstance(units, dict) or name not in units or not isinstance(units[name], dict):
        raise ValueError(f"unknown match unit {name!r}")
    unit = units[name]
    if unit.get("artifact_kind") != "coff":
        raise ValueError(
            "this comparator requires an explicit normal-COFF artifact kind"
        )
    size = int(unit["size"])
    compared_size = int(unit.get("compare_size", size))
    if compared_size < size:
        raise ValueError("comparison extent is smaller than the claimed extent")
    object_path = (ROOT / str(unit["object"])).resolve()
    object_path.relative_to((ROOT / "build").resolve())
    code, actual = object_function(object_path, str(unit["symbol"]))
    if len(code) != compared_size:
        raise ValueError(
            f"object function size {len(code):#x} differs from manifest {compared_size:#x}"
        )
    expected = []
    for raw in unit.get("relocations", []):
        kind = str(raw["type"])
        if kind not in RELOCATION_IDS:
            raise ValueError(f"unsupported relocation type {kind!r}")
        expected.append(
            {
                "offset": int(raw["offset"]),
                "type_id": RELOCATION_IDS[kind],
                "type": kind,
                "symbol": str(raw["symbol"]),
                "target": int(raw["target"]),
            }
        )
    actual_key = sorted(
        (int(row["offset"]), int(row["type_id"]), str(row["symbol"]))
        for row in actual
    )
    expected_key = sorted(
        (row["offset"], row["type_id"], row["symbol"]) for row in expected
    )
    if actual_key != expected_key:
        raise ValueError(
            f"COFF relocations differ from manifest: actual={actual_key!r} "
            f"expected={expected_key!r}"
        )
    address = int(unit["target_address"])
    original = target_bytes(verified_target(), address, compared_size)
    replayed = []
    for relocation in expected:
        offset = relocation["offset"]
        addend = struct.unpack_from("<I", code, offset)[0]
        if relocation["type_id"] == RELOCATION_IDS["DIR32"]:
            value = relocation["target"] + addend
        else:
            value = relocation["target"] + addend - (address + offset + 4)
        struct.pack_into("<I", code, offset, value & 0xFFFFFFFF)
        replayed.append(
            {
                "offset": f"0x{offset:X}",
                "type": relocation["type"],
                "symbol": relocation["symbol"],
                "target": f"0x{relocation['target']:08X}",
                "addend": f"0x{addend:08X}",
            }
        )
    differences = [
        {"offset": f"0x{index:X}", "object": left, "target": right}
        for index, (left, right) in enumerate(zip(code, original))
        if left != right
    ]
    coverage_differences = sum(
        int(row["offset"], 0) < size for row in differences
    )
    return {
        "unit": name,
        "artifact_kind": "coff",
        "result": "exact" if not differences else "mismatch",
        "symbol": unit["symbol"],
        "target_address": f"0x{address:08X}",
        "size": size,
        "matched_bytes": size - coverage_differences,
        "compared_size": compared_size,
        "matched_compared_bytes": compared_size - len(differences),
        "object": str(object_path.relative_to(ROOT)),
        "relocations": replayed,
        "first_differences": differences[:32],
    }


def compare_probe(
    path: Path, symbol: str, address: int, size: int
) -> dict[str, object]:
    code, relocations = object_function(path, symbol)
    original = target_bytes(verified_target(), address, size)
    relocation_candidates = []
    for relocation in relocations:
        row = dict(relocation)
        offset = int(relocation["offset"])
        if offset + 4 > size:
            raise ValueError(f"relocation at {offset:#x} leaves the probe extent")
        addend = struct.unpack_from("<I", code, offset)[0]
        encoded = struct.unpack_from("<I", original, offset)[0]
        row["object_addend"] = f"0x{addend:08X}"
        row["target_encoded_value"] = f"0x{encoded:08X}"
        if relocation["type_id"] == RELOCATION_IDS["DIR32"]:
            candidate = encoded - addend
        elif relocation["type_id"] == RELOCATION_IDS["REL32"]:
            displacement = struct.unpack_from("<i", original, offset)[0]
            candidate = address + offset + 4 + displacement - addend
        else:
            candidate = None
        row["candidate_target"] = (
            f"0x{candidate & 0xFFFFFFFF:08X}" if candidate is not None else None
        )
        relocation_candidates.append(row)
    ignored = {
        index
        for relocation in relocations
        for index in range(
            int(relocation["offset"]),
            min(int(relocation["offset"]) + 4, size),
        )
    }
    differences = []
    for index, right in enumerate(original):
        if index in ignored:
            continue
        left = code[index] if index < len(code) else None
        if left != right:
            differences.append(
                {"offset": f"0x{index:X}", "object": left, "target": right}
            )
    comparable = size - len(ignored)
    return {
        "result": (
            "structural-exact"
            if len(code) == size and not differences
            else "mismatch"
        ),
        "symbol": symbol,
        "target_address": f"0x{address:08X}",
        "size": size,
        "object_size": len(code),
        "comparable_bytes": comparable,
        "matched_comparable_bytes": comparable - len(differences),
        "relocations": relocation_candidates,
        "first_differences": differences[:32],
        "acceptance_authority": "none",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", nargs="?", type=Path)
    parser.add_argument("symbol", nargs="?")
    parser.add_argument("address", nargs="?", type=lambda value: int(value, 0))
    parser.add_argument("size", nargs="?", type=lambda value: int(value, 0))
    parser.add_argument("--unit")
    parser.add_argument(
        "--list-functions",
        action="store_true",
        help="list defined code symbols and their COFF extents",
    )
    parser.add_argument(
        "--contains",
        default="",
        help="case-insensitive symbol filter for --list-functions",
    )
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        if args.list_functions:
            if args.unit or args.object is None or any(
                value is not None for value in (args.symbol, args.address, args.size)
            ):
                raise ValueError(
                    "--list-functions requires only OBJECT and optional --contains"
                )
            object_path = args.object.resolve()
            functions = object_functions(object_path, args.contains)
            report = {
                "result": "ok",
                "artifact_kind": "coff",
                "object": str(object_path),
                "function_count": len(functions),
                "functions": functions,
                "acceptance_authority": "none",
            }
        elif args.unit:
            if any(value is not None for value in (args.object, args.symbol, args.address, args.size)):
                raise ValueError("--unit cannot be combined with probe arguments")
            if args.contains:
                raise ValueError("--contains requires --list-functions")
            report = compare_unit(args.unit)
        else:
            if None in (args.object, args.symbol, args.address, args.size):
                raise ValueError("probe mode requires OBJECT SYMBOL ADDRESS SIZE")
            if args.contains:
                raise ValueError("--contains requires --list-functions")
            report = compare_probe(
                args.object.resolve(), args.symbol, args.address, args.size
            )
    except (OSError, KeyError, TypeError, ValueError, struct.error, tomllib.TOMLDecodeError) as error:
        report = {"result": "error", "error": str(error)}
    if args.list_functions and not args.json and report["result"] == "ok":
        for function in report["functions"]:
            size = "?" if function["size"] is None else str(function["size"])
            relocations = (
                "?"
                if function["relocation_count"] is None
                else str(function["relocation_count"])
            )
            print(
                f"{function['symbol']}  size={size}  relocations={relocations}  "
                f"section={function['section']}"
            )
        print(f"defined COFF functions: {report['function_count']}")
        print("symbol inventory is diagnostic and grants no exactness credit")
    elif args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        print(json.dumps(report, indent=2, sort_keys=True))
    return 0 if report["result"] in {"ok", "exact", "structural-exact"} else 1


if __name__ == "__main__":
    raise SystemExit(main())
