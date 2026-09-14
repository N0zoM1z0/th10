#!/usr/bin/env python3
"""Replay one canonical VC7.1 linked-PE function against the TH10 target."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import tomllib

from linked_image import (
    PEImage,
    linked_code_fields,
    linked_functions,
    map_publics,
    verify_capstone,
)
from ltcg_link import ANCHOR_PREFIX


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th10.exe"
TARGET_MANIFEST = ROOT / "config" / "target.toml"
UNITS_MANIFEST = ROOT / "config" / "match-units.toml"
TOOLS_LOCK = ROOT / "config" / "tools.lock.toml"
FIELD_WIDTHS = {"DIR32": 4, "REL8": 1, "REL16": 2, "REL32": 4}


def below_build(raw: object, label: str) -> Path:
    path = (ROOT / str(raw)).resolve()
    try:
        path.relative_to((ROOT / "build").resolve())
    except ValueError as exc:
        raise ValueError(f"{label} must stay below build/") from exc
    return path


def verified_target() -> PEImage:
    with TARGET_MANIFEST.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != int(expected["size"]) or digest != str(expected["sha256"]):
        raise ValueError(f"target identity mismatch: size={len(data)} sha256={digest}")
    return PEImage(TARGET)


def anchor_aliases(path: Path) -> dict[str, str]:
    if not path.exists():
        return {}
    aliases: dict[str, str] = {}
    for line in path.read_text(encoding="ascii").splitlines():
        prefix = "/alternatename:"
        if not line.lower().startswith(prefix):
            raise ValueError("linked alias response contains an unsupported directive")
        pair = line[len(prefix) :]
        if pair.count("=") != 1:
            raise ValueError("linked alias response contains an invalid binding")
        source, anchor = pair.split("=", 1)
        if not source or not anchor.startswith(ANCHOR_PREFIX):
            raise ValueError("linked alias response contains an invalid anchor binding")
        if source in aliases and aliases[source] != anchor:
            raise ValueError(f"linked symbol {source!r} has multiple anchor bindings")
        aliases[source] = anchor
    if len(set(aliases.values())) != len(aliases):
        raise ValueError("linked alias response reuses a diagnostic anchor")
    return aliases


def expected_fields(unit: dict[str, object]) -> list[dict[str, object]]:
    result = []
    raw_fields = unit.get("linkages")
    if not isinstance(raw_fields, list):
        raise ValueError("linked unit lacks an exhaustive linkage list")
    for raw in raw_fields:
        if not isinstance(raw, dict):
            raise ValueError("linked unit contains an invalid linkage row")
        kind = str(raw["type"])
        width = int(raw.get("width", FIELD_WIDTHS.get(kind, 0)))
        if kind not in FIELD_WIDTHS or width != FIELD_WIDTHS[kind]:
            raise ValueError(f"unsupported linked field {kind!r}/{width}")
        result.append(
            {
                "offset": int(raw["offset"]),
                "width": width,
                "type": kind,
                "symbol": str(raw["symbol"]),
                "target": int(raw["target"]),
                "addend": int(raw.get("addend", 0)),
            }
        )
    return sorted(result, key=lambda row: (int(row["offset"]), str(row["type"])))


def pack_linked_value(
    code: bytearray,
    field: dict[str, object],
    instruction_end: int | None,
    target_address: int,
) -> int:
    kind = str(field["type"])
    width = int(field["width"])
    destination = int(field["target"]) + int(field["addend"])
    if kind == "DIR32":
        if not 0 <= destination <= 0xFFFFFFFF:
            raise ValueError("DIR32 replay value leaves the 32-bit address space")
        encoded = destination
        code[int(field["offset"]) : int(field["offset"]) + width] = encoded.to_bytes(
            width, "little"
        )
        return encoded
    if instruction_end is None:
        raise ValueError(f"{kind} field lacks a decoded instruction end")
    encoded = destination - (target_address + instruction_end)
    minimum = -(1 << (width * 8 - 1))
    maximum = (1 << (width * 8 - 1)) - 1
    if not minimum <= encoded <= maximum:
        raise ValueError(f"{kind} replay displacement is out of range")
    code[int(field["offset"]) : int(field["offset"]) + width] = encoded.to_bytes(
        width, "little", signed=True
    )
    return encoded


def compare_unit(name: str) -> dict[str, object]:
    with UNITS_MANIFEST.open("rb") as stream:
        unit_manifest = tomllib.load(stream)
    with TARGET_MANIFEST.open("rb") as stream:
        target_manifest = tomllib.load(stream)
    if unit_manifest.get("target_sha256") != target_manifest["target"]["sha256"]:
        raise ValueError("match-unit target hash differs from config/target.toml")
    units = unit_manifest.get("units", {})
    if (
        not isinstance(units, dict)
        or name not in units
        or not isinstance(units[name], dict)
    ):
        raise ValueError(f"unknown match unit {name!r}")
    unit = units[name]
    if unit.get("artifact_kind") != "linked-pe":
        raise ValueError("this comparator requires an explicit linked-PE artifact kind")

    with TOOLS_LOCK.open("rb") as stream:
        decoder_identity = verify_capstone(tomllib.load(stream)["capstone"])

    directory = below_build(unit["output_dir"], "linked output directory")
    image_path = directory / "source.exe"
    map_path = directory / "source.map"
    pdb_path = directory / "source.pdb"
    linked = linked_functions(image_path, map_path, pdb_path)
    source_matches = []
    for function in linked["functions"]:
        object_name = str(function["pdb_object"]).replace("\\", "/").rsplit("/", 1)[-1]
        if function["symbol"] == unit["symbol"] and object_name.lower() == "source.ltcg.obj":
            source_matches.append(function)
    if len(source_matches) != 1:
        raise ValueError(
            f"expected one PDB-owned source.ltcg.obj contribution for {unit['symbol']!r}, "
            f"found {len(source_matches)}"
        )
    function = source_matches[0]
    size = int(unit["size"])
    compared_size = int(unit.get("compare_size", size))
    if int(function["size"]) != compared_size:
        raise ValueError(
            f"PDB contribution size {int(function['size']):#x} differs from manifest "
            f"{compared_size:#x}"
        )

    candidate_image = PEImage(image_path)
    candidate_address = int(str(function["address"]), 0)
    publics = map_publics(map_path)
    field_report = linked_code_fields(
        candidate_image, publics, candidate_address, compared_size
    )
    if not field_report["normalization_complete"]:
        raise ValueError(
            f"Capstone decoded {field_report['decoded_bytes']} of {compared_size} bytes"
        )
    actual = list(field_report["fields"])
    expected = expected_fields(unit)
    actual_key = sorted(
        (int(row["offset"]), str(row["type"]), int(row["width"])) for row in actual
    )
    expected_key = [
        (int(row["offset"]), str(row["type"]), int(row["width"])) for row in expected
    ]
    if actual_key != expected_key:
        raise ValueError(
            f"linked fields differ from manifest: actual={actual_key!r} "
            f"expected={expected_key!r}"
        )

    public_addresses: dict[str, set[int]] = {}
    for public in publics:
        public_addresses.setdefault(str(public["symbol"]), set()).add(
            int(public["address"])
        )
    aliases = anchor_aliases(directory / "link-aliases.rsp")
    actual_by_key = {
        (int(row["offset"]), str(row["type"]), int(row["width"])): row
        for row in actual
    }
    candidate = bytearray(
        candidate_image.read_address(candidate_address, compared_size)
    )
    replayed = []
    for field in expected:
        key = (int(field["offset"]), str(field["type"]), int(field["width"]))
        observed = actual_by_key[key]
        semantic = str(field["symbol"])
        linked_symbol = aliases.get(semantic, semantic)
        if linked_symbol.startswith(ANCHOR_PREFIX) and aliases.get(semantic) != linked_symbol:
            raise ValueError("manifest cannot name a diagnostic anchor directly")
        expected_candidate_address = int(observed["candidate_target"]) - int(
            field["addend"]
        )
        addresses = public_addresses.get(linked_symbol, set())
        if addresses != {expected_candidate_address}:
            raise ValueError(
                f"linked field {key!r} does not resolve uniquely through {semantic!r}: "
                f"publics={sorted(addresses)!r} expected={expected_candidate_address:#x}"
            )
        encoded = pack_linked_value(
            candidate,
            field,
            (
                int(observed["instruction_end"])
                if observed["instruction_end"] is not None
                else None
            ),
            int(unit["target_address"]),
        )
        encoded_width = int(field["width"])
        encoded_mask = (1 << (encoded_width * 8)) - 1
        encoded_text = f"0x{encoded & encoded_mask:0{encoded_width * 2}X}"
        replayed.append(
            {
                "offset": f"0x{int(field['offset']):X}",
                "type": field["type"],
                "width": field["width"],
                "symbol": semantic,
                "linked_symbol": linked_symbol,
                "candidate_target": f"0x{int(observed['candidate_target']):08X}",
                "target": f"0x{int(field['target']):08X}",
                "addend": int(field["addend"]),
                "target_encoded_value": encoded_text,
            }
        )

    target_address = int(unit["target_address"])
    original = verified_target().read_address(target_address, compared_size)
    differences = [
        {"offset": f"0x{offset:X}", "linked": left, "target": right}
        for offset, (left, right) in enumerate(zip(candidate, original))
        if left != right
    ]
    coverage_differences = sum(int(row["offset"], 0) < size for row in differences)
    return {
        "unit": name,
        "artifact_kind": "linked-pe",
        "result": "exact" if not differences else "mismatch",
        "symbol": unit["symbol"],
        "target_address": f"0x{target_address:08X}",
        "size": size,
        "matched_bytes": size - coverage_differences,
        "compared_size": compared_size,
        "matched_compared_bytes": compared_size - len(differences),
        "image": str(image_path.relative_to(ROOT)),
        "image_sha256": linked["image_sha256"],
        "pdb_guid": linked["pdb_guid"],
        "pdb_age": linked["pdb_age"],
        "candidate_address": f"0x{candidate_address:08X}",
        "extent_source": function["extent_source"],
        "pdb_module_index": function["pdb_module_index"],
        "decoded_bytes": field_report["decoded_bytes"],
        "linkages": replayed,
        "raw_equal": candidate_image.read_address(candidate_address, compared_size)
        == original,
        "first_differences": differences[:32],
        "capstone_version": decoder_identity["version"],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--unit", required=True)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        report = compare_unit(args.unit)
    except (OSError, KeyError, TypeError, ValueError, struct.error, tomllib.TOMLDecodeError) as error:
        report = {"result": "error", "error": str(error)}
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0 if report["result"] == "exact" else 1


if __name__ == "__main__":
    raise SystemExit(main())
