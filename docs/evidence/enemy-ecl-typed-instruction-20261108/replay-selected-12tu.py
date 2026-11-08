from __future__ import annotations

import json
from pathlib import Path
import sys
import time
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))

import ltcg_link as L
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics

UNIT = "enemy-apply-bullet-clear"
SYMBOL = "?DispatchEclInstruction@EnemyRuntimeView@@QAEHXZ"
TARGET_ADDRESS = 0x0040E770
OUT = ROOT / "build/gpt-web/20261108-enemy-ecl-typed-instruction-cold"

with (ROOT / "config/match-units.toml").open("rb") as stream:
    unit = tomllib.load(stream)["units"][UNIT]
source = ROOT / unit["source"]
support = [ROOT / item for item in unit["support_sources"]]
support_profiles = {
    Path(name): profile for name, profile in unit["support_profiles"].items()
}
link, environment = L.tool_environment()
started = time.monotonic()
L.cold_link(
    source,
    OUT,
    unit["entry_symbol"],
    link,
    environment,
    profile=unit["profile"],
    support_sources=support,
    support_profiles=support_profiles,
)

functions = linked_functions(
    OUT / "source.exe", OUT / "source.map", OUT / "source.pdb"
)["functions"]
function = next(
    row for row in functions
    if row["symbol"] == SYMBOL
    and row["map_object"] == "support-08-EnemyEclDispatcher.ltcg.obj"
)
image = PEImage(OUT / "source.exe")
target = PEImage(ROOT / "resources/th10.exe")
address = int(function["address"], 0)
size = int(function["size"])
candidate = image.read_address(address, size)
original = target.read_address(TARGET_ADDRESS, size)
report = linked_code_fields(image, map_publics(OUT / "source.map"), address, size)
fields = report["fields"]
field_bytes = {
    offset
    for row in fields
    for offset in range(int(row["offset"]), int(row["offset"]) + int(row["width"]))
}
plain_differences = [
    offset for offset in range(size)
    if offset not in field_bytes and candidate[offset] != original[offset]
]
field_types: dict[str, int] = {}
for row in fields:
    field_types[str(row["type"])] = field_types.get(str(row["type"]), 0) + 1
source_pins = {
    str(path.relative_to(ROOT)): L.sha256(path)
    for path in [source, *support]
}
header_pins = {
    name: L.sha256(ROOT / name)
    for name in ("src/Enemy.hpp", "src/EclVm.hpp")
}
object_paths = [OUT / "source.ltcg.obj"] + [
    OUT / f"support-{index:02d}-{Path(name).stem}.ltcg.obj"
    for index, name in enumerate(unit["support_sources"])
]
object_pins = {
    path.name: L.sha256(path) for path in object_paths
}
result = {
    "target_sha256": L.sha256(ROOT / "resources/th10.exe"),
    "source_pins": source_pins,
    "header_pins": header_pins,
    "object_pins": object_pins,
    "function": {
        "symbol": SYMBOL,
        "target_address": f"0x{TARGET_ADDRESS:08X}",
        "candidate_address": function["address"],
        "target_size": size,
        "candidate_pdb_size": size,
        "extent_source": function["extent_source"],
        "candidate_fields": len(fields),
        "candidate_field_bytes": len(field_bytes),
        "candidate_field_types": field_types,
        "normalization_complete": report["normalization_complete"],
        "decoded_bytes": report["decoded_bytes"],
        "raw_difference_bytes": sum(a != b for a, b in zip(candidate, original)),
        "plain_difference_bytes": len(plain_differences),
        "matched_plain_bytes": size - len(field_bytes) - len(plain_differences),
        "candidate_image_sha256": L.sha256(OUT / "source.exe"),
        "candidate_map_sha256": L.sha256(OUT / "source.map"),
        "candidate_pdb_sha256": L.sha256(OUT / "source.pdb"),
        "exactness": "NONEXACT",
        "candidate_fields_are_original_bound": False,
        "new_exact_credit": 0,
    },
    "baseline_same_graph": {
        "dispatcher_source_sha256": "85f22f44df1d412068e47ded39d06cae20c929ce1581a27d20d9db79ba958517",
        "enemy_header_sha256": "372ef4e3ca68687434092efa8b50573ef6ebcf8aa0fc0bb2e14058d60b85f668",
        "candidate_pdb_size": 14416,
        "candidate_fields": 720,
        "plain_difference_bytes": 10284,
        "matched_plain_bytes": 1252,
    },
    "selected_graph": {
        "primary_source": str(source.relative_to(ROOT)),
        "support_sources": [str(path.relative_to(ROOT)) for path in support],
        "seconds": round(time.monotonic() - started, 1),
    },
}
print(json.dumps(result, indent=2))
