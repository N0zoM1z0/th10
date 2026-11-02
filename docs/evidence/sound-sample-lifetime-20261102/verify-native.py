#!/usr/bin/env python3
"""Full-owner location-normalized nonexact TH10 LoadSoundSample diagnostic."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))
from linked_image import PEImage, linked_functions, map_publics, linked_code_fields

TARGET_START = 0x0043D390
SIZE = 684
SYMBOL = "?LoadSoundSample@SoundPlayerView@@QAEHHPBD@Z"
FIELD_OFFSETS = [
    0x004, 0x031, 0x066, 0x076, 0x0A8, 0x0AE, 0x0BF, 0x0C4, 0x0C9,
    0x0CF, 0x0E4, 0x0FA, 0x100, 0x111, 0x11E, 0x12B, 0x158, 0x161,
    0x171, 0x176, 0x17B, 0x1EC, 0x223, 0x28E, 0x2A2,
]
EXPECTED_DIFFS = list(range(0x122, 0x128))

def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--artifact-dir", type=Path, required=True)
    args = parser.parse_args()
    artifacts = args.artifact_dir.resolve()
    artifacts.relative_to((ROOT / "build").resolve())

    with (ROOT / "config/target.toml").open("rb") as stream:
        expected_target = tomllib.load(stream)["target"]
    target_bytes = (ROOT / "resources/th10.exe").read_bytes()
    assert hashlib.sha256(target_bytes).hexdigest() == expected_target["sha256"]
    target = PEImage(ROOT / "resources/th10.exe")
    image_path = artifacts / "source.exe"
    map_path = artifacts / "source.map"
    pdb_path = artifacts / "source.pdb"
    native = PEImage(image_path)
    linked = linked_functions(image_path, map_path, pdb_path)
    owners = [f for f in linked["functions"] if f["symbol"] == SYMBOL]
    assert len(owners) == 1, owners
    owner = owners[0]
    assert owner["size"] == SIZE and "support-13-Sound" in owner["pdb_object"]
    assert owner["extent_source"] == "pdb-section-contribution"
    native_address = int(owner["address"], 0)

    decoded = linked_code_fields(native, map_publics(map_path), native_address, SIZE)
    assert decoded["normalization_complete"] is True
    assert decoded["decoded_bytes"] == SIZE
    fields = decoded["fields"]
    assert [int(f["offset"]) for f in fields] == FIELD_OFFSETS, fields

    candidate = bytearray(native.read_address(native_address, SIZE))
    original = bytearray(target.read_address(TARGET_START, SIZE))
    assert original[0x11A:0x11D] == candidate[0x11A:0x11D] == b"\x83\xc5\xf4"
    for field in fields:
        at, width = int(field["offset"]), int(field["width"])
        candidate[at:at + width] = bytes(width)
        original[at:at + width] = bytes(width)

    differences = [
        {"offset": f"0x{i:X}", "native": int(a), "target": int(b)}
        for i, (a, b) in enumerate(zip(candidate, original)) if a != b
    ]
    assert [int(row["offset"], 0) for row in differences] == EXPECTED_DIFFS, differences
    print(json.dumps({
        "result": "full-owner-nonexact",
        "target_size": SIZE,
        "native_size": owner["size"],
        "native_owner": owner["address"],
        "native_image_sha256": hashlib.sha256(image_path.read_bytes()).hexdigest(),
        "complete_instruction_decode": True,
        "candidate_field_locations": len(fields),
        "matched_after_field_location_normalization": SIZE - len(differences),
        "remaining_ordinary_differences": differences,
        "exact_credit": 0,
        "field_role_proof": "not claimed by location-only normalization",
    }, indent=2))

if __name__ == "__main__":
    main()
