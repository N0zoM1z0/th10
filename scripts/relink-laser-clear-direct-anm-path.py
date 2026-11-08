#!/usr/bin/env python3
"""Focused diagnostic LTCG replay for EnemyLaserLineView::Clear's direct ANM path.

This is not a canonical replay and its unresolved-anchor image is not runnable.
It compiles only the seven source TUs in the Clear/binder caller graph.
"""
from pathlib import Path
import hashlib
import json
import sys
import time

sys.path.insert(0, "scripts")
import ltcg_link as L
from linked_image import (
    PEImage,
    linked_functions,
    linked_code_fields,
    map_publics,
)

ROOT = Path("build/gpt-web/20261108-laser-clear-direct-anm-path").resolve()
OBJDIR = ROOT / "objects"
OBJDIR.mkdir(parents=True, exist_ok=True)

SOURCES = [
    ("src/AnmManager.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL", "/GS"]),
    ("src/Gui.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL", "/GS", "/EHs"]),
    ("src/EnemyLaser.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL", "/GS"]),
    ("src/LaserClear.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL", "/GS"]),
    ("src/LaserType1Clear.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL", "/GS"]),
    ("src/Player.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL"]),
    ("src/EnemyEclDispatcher.cpp", ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc", "/GL", "/GS"]),
]
ENTRY = "?Clear@EnemyLaserLineView@@UAEHH@Z"
WANTED = [
    ("?Clear@EnemyLaserLineView@@UAEHH@Z", 0x41E260, 609),
    ("?InitializeAndExecuteScriptIndex@AnmLoadedView@@QAEXPAUAnmVmView@@H@Z", 0x43E710, 203),
    ("?Initialize@AnmVmView@@QAEXXZ", 0x401DE0, 273),
    ("?CreateVmAtWorldVariant0@AnmLoadedView@@QAE?AUAnmVmIdView@@HPBUAnmFloat3View@@@Z", 0x448DB0, 118),
]

started = time.monotonic()
link, env = L.tool_environment()
objects = []
input_rows = []
for i, (source, profile) in enumerate(SOURCES):
    path = Path(source)
    obj_name = "source.ltcg.obj" if i == 0 else f"support-{i-1:02d}-{path.stem}.ltcg.obj"
    obj = OBJDIR / obj_name
    L.compile_source(path, obj, profile)
    objects.append(obj)
    input_rows.append({
        "path": source,
        "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
        "object": str(obj.relative_to(ROOT)),
        "object_sha256": L.sha256(obj),
        "profile": profile,
    })

image = ROOT / "source.exe"
map_path = ROOT / "source.map"
pdb = ROOT / "source.pdb"
symbols = []
anchor = None
aliases = None
for attempt in range(1, 9):
    for path in (image, map_path, pdb, image.with_suffix(".ilk")):
        if path.exists():
            path.unlink()
    if symbols:
        anchor, aliases, _ = L.write_anchors(ROOT, symbols)
    linked = L.run(
        L.link_command(link, env, objects, ENTRY, image, map_path, pdb, anchor, aliases),
        environment=env,
    )
    (ROOT / f"link-attempt-{attempt}.log").write_text(
        linked.stdout, encoding="utf-8", errors="replace"
    )
    if linked.returncode == 0:
        break
    observed = L.unresolved_symbols(linked.stdout)
    L.reject_unresolved_entry(ENTRY, observed)
    new_symbols = sorted(set(observed) - set(symbols))
    if not observed or not new_symbols:
        raise RuntimeError("focused link stalled: " + linked.stdout[-2500:])
    symbols = sorted(set(symbols) | set(new_symbols))
else:
    raise RuntimeError("focused link exceeded eight anchor passes")

functions = linked_functions(image, map_path, pdb)["functions"]
linked_image = PEImage(image)
target_image = PEImage("resources/th10.exe")
publics = map_publics(map_path)
function_rows = []
for symbol, target_address, target_size in WANTED:
    matches = [row for row in functions if row["symbol"] == symbol]
    if not matches:
        continue
    function = max(matches, key=lambda row: int(row["size"]))
    address = int(function["address"], 0)
    size = int(function["size"])
    candidate = linked_image.read_address(address, size)
    target = target_image.read_address(target_address, min(size, target_size))
    fields = linked_code_fields(linked_image, publics, address, size)["fields"]
    field_bytes = {
        offset
        for field in fields
        for offset in range(int(field["offset"]), int(field["offset"]) + int(field["width"]))
    }
    compared = min(size, target_size)
    differences = [
        offset for offset in range(compared)
        if offset not in field_bytes and candidate[offset] != target[offset]
    ]
    function_rows.append({
        "symbol": symbol,
        "address": function["address"],
        "size": size,
        "target_address": hex(target_address),
        "target_size": target_size,
        "map_object": function["map_object"],
        "link_fields": len(fields),
        "field_bytes": len(field_bytes),
        "ordinary_matches": compared - len(field_bytes.intersection(range(compared))) - len(differences),
        "ordinary_bytes": compared - len(field_bytes.intersection(range(compared))),
        "ordinary_differences": len(differences),
        "first_differences": [hex(offset) for offset in differences[:50]],
        "candidate_bytes": candidate.hex(" ") if size <= 250 else candidate[:160].hex(" "),
    })

result = {
    "target_sha256": hashlib.sha256(Path("resources/th10.exe").read_bytes()).hexdigest(),
    "entry_symbol": ENTRY,
    "image_sha256": L.sha256(image),
    "anchor_count": len(symbols),
    "elapsed_seconds": round(time.monotonic() - started, 1),
    "compile_profile_family": "VC7.1 /GL /GS focused seven-TU diagnostic",
    "input_sources": input_rows,
    "status": "diagnostic-only; unresolved anchors; no exactness credit",
    "same_graph_previous_indirect_baseline": {
        "previous_head": "ee73d42fada111913fb0cd60071c9787d475fd0d",
        "candidate_size": 630,
        "target_size": 609,
        "link_fields": 22,
        "field_bytes": 88,
        "ordinary_matches": 24,
        "ordinary_bytes": 521,
        "image_sha256": "d3f7dc6e4bd56cefe4528f0634e8fd528aae04e3b3cfd07509aa3562f848ffa0",
    },
    "target_call_order": [
        "0x41E2F8: load signed request.color",
        "0x41E2FF-0x41E305: load current effectResource",
        "0x41E30B: increment sample count",
        "0x41E310: compute 2*color+0x11 in EBX",
        "0x41E314: AllocateVm",
        "0x41E346: ECX=resource",
        "0x41E35E: EAX=VM, EBX=script; call InitializeAndExecuteScriptIndex",
    ],
    "candidate_call_order": [
        "0x411544: load signed request.color",
        "0x41154B-0x411551: load current effectResource",
        "0x411557: increment sample count",
        "0x41155C: compute 2*color+0x11",
        "0x411560: AllocateVm",
        "0x4115A6-0x4115A8: EDX=VM, EAX=script; call binder with resource in ESI",
    ],
    "functions": function_rows,
}
evidence = Path("docs/evidence/laser-clear/direct-anm-call-20261108.json")
evidence.write_text(json.dumps(result, indent=2) + "\n")
print(json.dumps(result, indent=2))
