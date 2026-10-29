#!/usr/bin/env python3
"""Replay one canonical VC7.1 linked-PE function against the TH10 target."""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
from pathlib import Path
import struct
import tomllib

from linked_image import (
    PEImage,
    linked_code_fields,
    linked_functions,
    static_owner_selector,
    static_linkage_selector,
    static_callee_unit_reference,
    validate_static_callee_units,
    static_address_selector,
    STATIC_ADDRESS_KEYS,
    map_publics,
    verify_capstone,
)
from ltcg_link import ANCHOR_PREFIX, support_profile_overrides
from linked_allocation_eh import allocation_eh_selector, allocation_eh_target
from linked_stack_file_eh import stack_file_eh_selector, stack_file_eh_target
from linked_worker_member_eh import worker_member_eh_selector, worker_member_eh_target
from linked_chain_member_eh import chain_member_eh_selector, chain_member_eh_target
from linked_stage_array_eh import stage_array_eh_selector, stage_array_eh_target


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
        allocation_eh_selector(unit, raw)
        stack_file_eh_selector(unit, raw)
        worker_member_eh_selector(unit, raw)
        chain_member_eh_selector(unit, raw)
        stage_array_eh_selector(unit, raw)
        kind = str(raw["type"])
        width = int(raw.get("width", FIELD_WIDTHS.get(kind, 0)))
        if kind not in FIELD_WIDTHS or width != FIELD_WIDTHS[kind]:
            raise ValueError(f"unsupported linked field {kind!r}/{width}")
        result.append(
            {
                "offset": int(raw["offset"]),
                "width": width,
                "type": kind,
                "symbol": "<local>" if bool(raw.get("local", False)) else str(raw["symbol"]),
                "local": bool(raw.get("local", False)),
                "target": int(raw["target"]),
                "addend": int(raw.get("addend", 0)),
                **({"allocation_eh_handler": True} if raw.get("allocation_eh_handler") else {}),
                **({"allocation_eh_storage_offset": raw["allocation_eh_storage_offset"]}
                   if "allocation_eh_storage_offset" in raw else {}),
                **({"stack_file_eh_handler": True} if raw.get("stack_file_eh_handler") else {}),
                **({"worker_member_eh_handler": True} if raw.get("worker_member_eh_handler") else {}),
                **({"chain_member_eh_handler": True} if raw.get("chain_member_eh_handler") else {}),
                **({"stage_array_eh_handler": True} if raw.get("stage_array_eh_handler") else {}),
                **({"static_source": raw["static_source"], "static_size": raw["static_size"]}
                   if static_linkage_selector(unit, raw) is not None else {}),
                **({"static_callee_unit": raw["static_callee_unit"]} if "static_callee_unit" in raw else {}),
                **({"static_tail_jump": True} if raw.get("static_tail_jump") else {}),
                **({key: raw[key] for key in STATIC_ADDRESS_KEYS}
                   if static_address_selector(unit, raw) is not None else {}),
            }
        )
    return sorted(result, key=lambda row: (int(row["offset"]), str(row["type"])))


def expected_data_spans(
    unit: dict[str, object], compare_size: int, occupied: set[int]
) -> list[dict[str, int]]:
    raw_spans = unit.get("data_spans", [])
    if not isinstance(raw_spans, list):
        raise ValueError("linked unit data_spans must be a list")
    result: list[dict[str, int]] = []
    for raw in raw_spans:
        if not isinstance(raw, dict):
            raise ValueError("linked unit contains an invalid data span")
        offset = int(raw["offset"])
        width = int(raw["width"])
        extent = set(range(offset, offset + width))
        if (
            width <= 0
            or offset < 0
            or offset + width > compare_size
            or occupied & extent
        ):
            raise ValueError("linked unit contains an invalid data-span extent")
        occupied.update(extent)
        result.append({"offset": offset, "width": width})
    result.sort(key=lambda row: int(row["offset"]))
    if result:
        cursor = compare_size - sum(int(row["width"]) for row in result)
        for row in result:
            if int(row["offset"]) != cursor:
                raise ValueError(
                    "linked unit data spans must form one contiguous trailing extent"
                )
            cursor += int(row["width"])
        if cursor != compare_size:
            raise ValueError("linked unit data spans leave a trailing gap")
    return result


def require_decode_coverage(
    field_report: dict[str, object], data_spans: list[dict[str, int]], size: int
) -> None:
    if field_report["normalization_complete"]:
        if data_spans:
            raise ValueError(
                "linked unit declares data spans despite complete Capstone normalization"
            )
        return
    decoded = int(field_report["decoded_bytes"])
    if not 0 <= decoded <= size:
        raise ValueError("linked decoder extent leaves the compared contribution")
    if not data_spans:
        raise ValueError(f"Capstone decoded {decoded} of {size} bytes")
    covered = set(range(decoded))
    # Absolute table entries are already accounted for by authoritative PE
    # relocations. An unlinked selector suffix may follow those entries.
    for field in field_report["fields"]:
        if field["type"] == "DIR32" and int(field["width"]) == 4:
            covered.update(range(int(field["offset"]), int(field["offset"]) + 4))
    for span in data_spans:
        if int(span["offset"]) < decoded:
            raise ValueError("data spans overlap the decoded instruction extent")
        covered.update(range(int(span["offset"]), int(span["offset"]) + int(span["width"])))
    if not set(range(size)).issubset(covered):
        raise ValueError(
            "complete PE fields and declared data spans do not cover the undecoded trailing extent"
        )


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


def static_call_target(
    unit: dict[str, object], field: dict[str, object], directory: Path,
    image: PEImage, publics: list[dict[str, object]], target: PEImage,
    *, unit_manifest=None, active_units=(),
) -> dict[str, object]:
    """Prove a whole static callee: legacy raw body or explicit unit replay."""
    selector = static_linkage_selector(unit, field)
    if selector is None:
        raise ValueError("static call target lacks an explicit selector")
    address, size = int(field["target"]), int(selector["size"])
    with (ROOT / "config/functions.csv").open(newline="") as stream:
        owners = [row for row in csv.DictReader(stream) if int(row["address"], 0) == address]
    with (ROOT / "config/function-boundaries.csv").open(newline="") as stream:
        bounds = [row for row in csv.DictReader(stream) if int(row["address"], 0) == address]
    if (len(owners) != 1 or len(bounds) != 1 or int(owners[0]["size"]) != size
            or bounds[0]["state"] != "reviewed"
            or int(bounds[0]["span_end"], 0) != address + size - 1):
        raise ValueError("static callee lacks the complete reviewed target boundary")
    if field.get("static_tail_jump") or "static_callee_unit" in field:
        name = owners[0].get("proposed_name")
        if (owners[0].get("source_file") != field["static_source"] or not name
                or not str(field["symbol"]).startswith("?" + name + "@@")):
            raise ValueError("static tail callee differs from its maintained source mapping")
    linked = linked_functions(directory / "source.exe", directory / "source.map",
                              directory / "source.pdb", static_owner=selector)
    functions = [row for row in linked["functions"]
                 if row["symbol"] == selector["symbol"] and row.get("owner_visibility") == "static"]
    if len(functions) != 1:
        raise ValueError("static callee must have one qualified complete owner")
    function = functions[0]
    candidate = int(function["address"], 0)
    if "static_callee_unit" in field:
        if unit_manifest is None:
            raise ValueError("static callee unit requires the same manifest context")
        reference = static_callee_unit_reference(unit, field, unit_manifest["units"])
        def artifact_identity():
            return {suffix: hashlib.sha256((directory / ("source." + suffix)).read_bytes()).hexdigest()
                    for suffix in ("exe", "map", "pdb")}
        before = artifact_identity()
        if (before["exe"] != linked["image_sha256"]
                or before["exe"] != hashlib.sha256(image.data).hexdigest()):
            raise ValueError("static callee artifact changed during owner selection")
        proof = compare_unit(reference, _active_units=active_units, _unit_manifest=unit_manifest)
        if artifact_identity() != before:
            raise ValueError("static callee artifact changed during recursive replay")
        if (proof["result"] != "exact" or proof["size"] != size
                or proof["compared_size"] != size or proof["matched_bytes"] != size
                or proof["matched_compared_bytes"] != size
                or proof["candidate_address"] != function["address"]
                or proof["image_sha256"] != before["exe"]
                or proof["pdb_guid"] != linked["pdb_guid"] or proof["pdb_age"] != linked["pdb_age"]
                or not proof["normalization_complete"] or proof["decoded_bytes"] != size):
            raise ValueError("static callee unit lacks an exact whole same-artifact proof")
        return {**function, "callee_unit_proof": proof, "artifact_sha256": before}
    fields = linked_code_fields(image, publics, candidate, size)
    if (not fields["normalization_complete"] or fields["decoded_bytes"] != size
            or fields["fields"]):
        raise ValueError("static callee binding requires a complete zero-field body")
    if image.read_address(candidate, size) != target.read_address(address, size):
        raise ValueError("static callee body differs from the complete target")
    return function


def static_address_target(
    unit: dict[str, object], field: dict[str, object], directory: Path,
    image: PEImage, publics: list[dict[str, object]], target: PEImage,
) -> dict[str, object]:
    """Bind a qualified function entry, independently of its byte exactness."""
    selector = static_address_selector(unit, field)
    if selector is None:
        raise ValueError("static address target lacks an explicit selector")
    address = int(field["target"])
    target_size = int(field["static_address_target_size"])
    with (ROOT / "config/functions.csv").open(newline="") as stream:
        owners = [row for row in csv.DictReader(stream) if int(row["address"], 0) == address]
    with (ROOT / "config/function-boundaries.csv").open(newline="") as stream:
        bounds = [row for row in csv.DictReader(stream) if int(row["address"], 0) == address]
    if (len(owners) != 1 or len(bounds) != 1 or int(owners[0]["size"]) != target_size
            or bounds[0]["state"] != "reviewed"
            or int(bounds[0]["span_end"], 0) != address + target_size - 1):
        raise ValueError("static address lacks the complete reviewed target boundary")
    if (owners[0].get("source_file") != field["static_address_source"]
            or not owners[0].get("proposed_name")):
        raise ValueError("static address lacks its maintained target source mapping")
    # This lane deliberately covers ordinary TU-static C++ functions only.
    # Bind the maintained name as well as its TU; another callback in the same
    # source must not become an interchangeable address-normalization target.
    if not str(field["symbol"]).startswith("?" + owners[0]["proposed_name"] + "@@"):
        raise ValueError("static address differs from its maintained target source name")
    linked = linked_functions(directory / "source.exe", directory / "source.map",
                              directory / "source.pdb", static_owner=selector)
    functions = [row for row in linked["functions"]
                 if row["symbol"] == selector["symbol"] and row.get("owner_visibility") == "static"]
    if len(functions) != 1:
        raise ValueError("static address must have one qualified complete owner")
    function = functions[0]
    candidate = int(function["address"], 0)
    for label, artifact, symbols, start, size in [
        ("candidate", image, publics, candidate, int(selector["size"])),
        ("target", target, [], address, target_size),
    ]:
        decoded = linked_code_fields(artifact, symbols, start, size)
        if not decoded["normalization_complete"] or decoded["decoded_bytes"] != size:
            raise ValueError(f"static address {label} must be a complete dense code owner")
    return function


def compare_unit(name: str, *, _active_units=(), _unit_manifest=None) -> dict[str, object]:
    if name in _active_units:
        raise ValueError("static callee unit dependency cycle")
    active_units = (*_active_units, name)
    if _unit_manifest is None:
        with UNITS_MANIFEST.open("rb") as stream:
            unit_manifest = tomllib.load(stream)
    else:
        unit_manifest = _unit_manifest
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
    validate_static_callee_units(units)
    unit = units[name]
    if unit.get("artifact_kind") != "linked-pe":
        raise ValueError("this comparator requires an explicit linked-PE artifact kind")
    support_profile_overrides(
        [ROOT / value for value in unit.get("support_sources", [])],
        tuple(ROOT / value for value in unit.get("normal_support_sources", [])),
        unit.get("support_profiles", {}),
    )

    static_owner = static_owner_selector(unit)

    with TOOLS_LOCK.open("rb") as stream:
        decoder_identity = verify_capstone(tomllib.load(stream)["capstone"])

    directory = below_build(unit["output_dir"], "linked output directory")
    image_path = directory / "source.exe"
    map_path = directory / "source.map"
    pdb_path = directory / "source.pdb"
    linked = (linked_functions(image_path, map_path, pdb_path) if static_owner is None
              else linked_functions(image_path, map_path, pdb_path, static_owner=static_owner))
    primary_source = str(unit["source"]).replace("\\", "/")
    pdb_source = str(unit.get("pdb_source", primary_source)).replace("\\", "/")
    if pdb_source == primary_source:
        expected_object = "source.ltcg.obj"
    else:
        support_sources = [
            str(value).replace("\\", "/")
            for value in unit.get("support_sources", [])
        ]
        try:
            support_index = support_sources.index(pdb_source)
        except ValueError as exc:
            raise ValueError(
                f"linked unit pdb_source {pdb_source!r} is not its primary or support source"
            ) from exc
        support_kind = (
            "normal"
            if pdb_source in {
                str(value).replace("\\", "/")
                for value in unit.get("normal_support_sources", [])
            }
            else "ltcg"
        )
        expected_object = (
            f"support-{support_index:02d}-{Path(pdb_source).stem}.{support_kind}.obj"
        )

    source_matches = []
    for function in linked["functions"]:
        object_name = str(function["pdb_object"]).replace("\\", "/").rsplit("/", 1)[-1]
        if (
            function["symbol"] == unit["symbol"]
            and object_name.lower() == expected_object.lower()
        ):
            source_matches.append(function)
    if len(source_matches) != 1:
        raise ValueError(
            f"expected one PDB-owned {expected_object} contribution for "
            f"{unit['symbol']!r}, found {len(source_matches)}"
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
    actual = list(field_report["fields"])
    expected = expected_fields(unit)
    occupied = {
        offset
        for row in expected
        for offset in range(int(row["offset"]), int(row["offset"]) + int(row["width"]))
    }
    data_spans = expected_data_spans(unit, compared_size, occupied)
    require_decode_coverage(field_report, data_spans, compared_size)
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
    target = verified_target()
    static_calls = {}
    static_addresses = {}
    parent_function = dict(function)
    for field in expected:
        key = (int(field["offset"]), str(field["type"]), int(field["width"]))
        observed = actual_by_key[key]
        semantic = str(field["symbol"])
        expected_candidate_address = int(observed["candidate_target"]) - int(
            field["addend"]
        )
        if bool(field["local"]):
            if not candidate_address <= expected_candidate_address < candidate_address + compared_size:
                raise ValueError(
                    f"local linked field {key!r} leaves the candidate contribution: "
                    f"target={expected_candidate_address:#x}"
                )
            target_base = int(unit["target_address"])
            semantic_target = int(field["target"])
            if not target_base <= semantic_target < target_base + compared_size:
                raise ValueError(
                    f"local linked field {key!r} leaves the target contribution: "
                    f"target={semantic_target:#x}"
                )
            candidate_offset = expected_candidate_address - candidate_address
            target_offset = semantic_target - target_base
            if candidate_offset != target_offset:
                raise ValueError(
                    f"local linked field {key!r} changes its destination offset: "
                    f"candidate={candidate_offset:#x} target={target_offset:#x}"
                )
            linked_symbol = "<local>"
        elif field.get("allocation_eh_handler"):
            allocation_eh_evidence = allocation_eh_target(
                unit, field, directory, candidate_image, publics, aliases,
                target, parent_function, expected_candidate_address)
            linked_symbol = semantic
        elif field.get("stack_file_eh_handler"):
            stack_file_eh_evidence = stack_file_eh_target(
                unit, field, directory, candidate_image, publics, aliases,
                target, parent_function, expected_candidate_address)
            linked_symbol = semantic
        elif field.get("worker_member_eh_handler"):
            worker_member_eh_evidence = worker_member_eh_target(
                unit, field, directory, candidate_image, publics, aliases,
                target, parent_function, expected_candidate_address)
            linked_symbol = semantic
        elif field.get("chain_member_eh_handler"):
            chain_member_eh_evidence = chain_member_eh_target(
                unit, field, directory, candidate_image, publics, aliases,
                target, parent_function, expected_candidate_address)
            linked_symbol = semantic
        elif field.get("stage_array_eh_handler"):
            stage_array_eh_evidence = stage_array_eh_target(
                unit, field, directory, candidate_image, publics, aliases,
                target, parent_function, expected_candidate_address)
            linked_symbol = semantic
        elif "static_source" in field:
            instruction = "jmp" if field.get("static_tail_jump") else "call"
            if observed.get("instruction") != instruction:
                raise ValueError(f"static linkage must be a decoded direct {instruction.upper()}")
            static_key = (semantic, str(field["static_source"]), int(field["static_size"]),
                          int(field["target"]), bool(field.get("static_tail_jump")), field.get("static_callee_unit"))
            if static_key not in static_calls:
                static_calls[static_key] = static_call_target(
                    unit, field, directory, candidate_image, publics, target,
                    unit_manifest=unit_manifest, active_units=active_units)
            function = static_calls[static_key]
            if int(function["address"], 0) != expected_candidate_address:
                raise ValueError("static call does not enter its qualified callee")
            linked_symbol = semantic
        elif "static_address_source" in field:
            address_key = (semantic, *(field[key] for key in STATIC_ADDRESS_KEYS), int(field["target"]))
            if address_key not in static_addresses:
                static_addresses[address_key] = static_address_target(
                    unit, field, directory, candidate_image, publics, target)
            function = static_addresses[address_key]
            if int(function["address"], 0) != expected_candidate_address:
                raise ValueError("static address does not name its qualified function entry")
            linked_symbol = semantic
        else:
            linked_symbol = aliases.get(semantic, semantic)
            if linked_symbol.startswith(ANCHOR_PREFIX) and aliases.get(semantic) != linked_symbol:
                raise ValueError("manifest cannot name a diagnostic anchor directly")
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
                "local": bool(field["local"]),
                "candidate_target": f"0x{int(observed['candidate_target']):08X}",
                "target": f"0x{int(field['target']):08X}",
                "addend": int(field["addend"]),
                "target_encoded_value": encoded_text,
                **({"allocation_eh": allocation_eh_evidence} if field.get("allocation_eh_handler") else {}),
                **({"stack_file_eh": stack_file_eh_evidence} if field.get("stack_file_eh_handler") else {}),
                **({"worker_member_eh": worker_member_eh_evidence} if field.get("worker_member_eh_handler") else {}),
                **({"chain_member_eh": chain_member_eh_evidence} if field.get("chain_member_eh_handler") else {}),
                **({"stage_array_eh": stage_array_eh_evidence} if field.get("stage_array_eh_handler") else {}),
                **({"static_owner": static_calls[static_key]}
                   if "static_source" in field else {}),
                **({"static_tail_jump": True} if field.get("static_tail_jump") else {}),
                **({"static_address_owner": static_addresses[address_key],
                    "static_address_binding_only": True,
                    "static_address_target_size": field["static_address_target_size"]}
                   if "static_address_source" in field else {}),
            }
        )

    target_address = int(unit["target_address"])
    original = target.read_address(target_address, compared_size)
    differences = [
        {"offset": f"0x{offset:X}", "linked": left, "target": right}
        for offset, (left, right) in enumerate(zip(candidate, original))
        if left != right
    ]
    function = parent_function  # Callee lookup must not replace caller extent metadata.
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
        **({"owner_selection": {
            "visibility": "static", "pdb_source": pdb_source,
            "expected_object": static_owner["object"],
            "map_object": function["map_object"], "pdb_object": function["pdb_object"],
            "section_number": function["section_number"],
            "section_offset": function["section_offset"],
        }} if static_owner is not None else {}),
        "decoded_bytes": field_report["decoded_bytes"],
        "normalization_complete": field_report["normalization_complete"],
        "data_spans": data_spans,
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
