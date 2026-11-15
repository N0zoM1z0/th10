#!/usr/bin/env python3
"""Ratchet check for source-present TH10 functions that are not exact yet.

Each entry in config/frontiers.toml names a function, the manifest unit whose
linked /GL graph builds it, the original extent (address, size, SHA-256) and
the byte offsets that currently differ from the original after the
candidate's link-resolved fields (REL32 branch/call operands and DIR32 base
relocations) are masked. The check rebuilds each graph once, then fails if a
byte that matched before now differs. A strictly smaller residual is reported
as an improvement; record it with --update. This is diagnostic only: exact
credit still requires a canonical unit, compare-linked-function.py and a cold
replay.

Usage:
  scripts/repo-python scripts/check-frontier.py                check every frontier
  scripts/repo-python scripts/check-frontier.py NAME...        check selected frontiers
  scripts/repo-python scripts/check-frontier.py --no-build     measure existing artifacts
  scripts/repo-python scripts/check-frontier.py --update NAME  record an improved residual
  scripts/repo-python scripts/check-frontier.py --show NAME    also list differing instructions
  scripts/repo-python scripts/check-frontier.py --add NAME --unit UNIT --function F \\
      [--symbol S --address A --size N]                       record a new frontier
"""
from __future__ import annotations

import argparse
import hashlib
import subprocess
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))

from linked_image import (  # noqa: E402
    PEImage,
    linked_code_fields,
    linked_functions,
    map_publics,
    map_statics,
    pdb_section_contributions,
    static_owner_selector,
)

CONFIG = ROOT / "config/frontiers.toml"
MANIFEST = ROOT / "config/match-units.toml"
TARGET = ROOT / "resources/th10.exe"
TARGET_MANIFEST = ROOT / "config/target.toml"


def parse_offsets(items: list[str]) -> set[int]:
    offsets: set[int] = set()
    for item in items:
        low, _, high = item.partition("-")
        offsets.update(range(int(low, 16), int(high or low, 16) + 1))
    return offsets


def format_offsets(offsets: set[int]) -> list[str]:
    runs: list[str] = []
    start = previous = None
    for value in sorted(offsets) + [None]:
        if start is not None and value is not None and value == previous + 1:
            previous = value
            continue
        if start is not None:
            runs.append(f"{start:X}" if start == previous else f"{start:X}-{previous:X}")
        start = previous = value
    return runs


def verified_target() -> PEImage:
    expected = tomllib.loads(TARGET_MANIFEST.read_text())["target"]
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != int(expected["size"]) or digest != str(expected["sha256"]):
        raise SystemExit(f"target identity mismatch: size={len(data)} sha256={digest}")
    return PEImage(TARGET)


def resolve(entry: dict, units: dict) -> dict:
    """Fill symbol/address/size from the unit when the entry measures the unit's own owner."""
    unit = units.get(entry["unit"])
    if unit is None:
        raise ValueError(f"unknown unit {entry['unit']!r}")
    resolved = dict(entry)
    resolved.setdefault("symbol", unit["symbol"])
    resolved.setdefault("address", unit["target_address"])
    resolved.setdefault("size", unit["size"])
    resolved["output_dir"] = unit["output_dir"]
    resolved["static_owner"] = (
        static_owner_selector(unit) if resolved["symbol"] == unit["symbol"] else None
    )
    return resolved


def build(unit_name: str) -> None:
    result = subprocess.run(
        ["nice", "-n", "10", sys.executable, "scripts/build-match-unit.py", "--unit", unit_name],
        cwd=ROOT, capture_output=True, text=True,
    )
    if result.returncode != 0:
        raise RuntimeError(f"{unit_name}: build failed\n{result.stdout[-2000:]}{result.stderr[-2000:]}")


def measure(target: PEImage, entry: dict) -> tuple[int, set[int], bytes, bytes]:
    address, size = int(entry["address"]), int(entry["size"])
    original = target.read_address(address, size)
    digest = hashlib.sha256(original).hexdigest()
    if "target_sha256" in entry and digest != entry["target_sha256"]:
        raise ValueError(f"original extent hash changed: {digest}")
    directory = ROOT / entry["output_dir"]
    image_path = directory / "source.exe"
    static_owner = entry["static_owner"]
    if static_owner is not None:
        # The strict selector pins the target size; a frontier candidate may
        # differ, so select it with its own PDB extent instead.
        starts = [(r["section_number"], r["section_offset"]) for r in map_statics(directory / "source.map")
                  if r["symbol"] == static_owner["symbol"]]
        sizes = [int(c["size"]) for c in pdb_section_contributions(directory / "source.pdb")["contributions"]
                 if (c["section_number"], c["section_offset"]) in starts]
        if len(sizes) == 1:
            static_owner = dict(static_owner, size=sizes[0])
    linked = linked_functions(
        image_path, directory / "source.map", directory / "source.pdb",
        static_owner=static_owner,
    )
    matches = [f for f in linked["functions"] if f["symbol"] == entry["symbol"]]
    if len(matches) != 1:
        raise ValueError(f"{entry['symbol']} has {len(matches)} linked extents in {entry['output_dir']}")
    start, length = int(matches[0]["address"], 0), int(matches[0]["size"])
    image = PEImage(image_path)
    candidate = image.read_address(start, length)
    fields = linked_code_fields(image, map_publics(directory / "source.map"), start, length)["fields"]
    masked = {int(f["offset"]) + i for f in fields for i in range(int(f["width"]))}
    differing = {i for i in range(min(size, length))
                 if i not in masked and original[i] != candidate[i]}
    differing.update(range(min(size, length), max(size, length)))
    return length, differing, original, candidate


def classify(entry: dict, length: int, differing: set[int]) -> tuple[str, set[int]]:
    """Return REGRESSED/EXACT/IMPROVED/OK and the newly differing offsets."""
    new = differing - parse_offsets(entry["residual"])
    if new:
        return "REGRESSED", new
    if not differing and length == int(entry["size"]):
        return "EXACT", new
    if differing < parse_offsets(entry["residual"]) or length != int(entry["candidate_size"]):
        return "IMPROVED", new
    return "OK", new


def show(original: bytes, candidate: bytes, differing: set[int]) -> None:
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    ours = {i.address: i for i in md.disasm(candidate, 0)}
    shown = 0
    for ins in md.disasm(original, 0):
        if any(o in differing for o in range(ins.address, ins.address + ins.size)):
            mine = ours.get(ins.address)
            print(f"    +{ins.address:04X} original: {ins.mnemonic} {ins.op_str:<34}"
                  f" ours: {mine.mnemonic + ' ' + mine.op_str if mine else '(misaligned)'}")
            shown += 1
            if shown == 40:
                print("    ...")
                return


def entry_text(name: str, entry: dict) -> str:
    lines = [f"[frontier.{name}]"]
    for key in ("function", "unit", "symbol", "address", "size", "candidate_size", "target_sha256"):
        if key not in entry:
            continue
        value = entry[key]
        if key == "address":
            lines.append(f"address = 0x{int(value):08X}")
        elif isinstance(value, int):
            lines.append(f"{key} = {value}")
        else:
            lines.append(f'{key} = "{value}"')
    lines.append("residual = [" + ", ".join(f'"{r}"' for r in entry["residual"]) + "]")
    return "\n".join(lines) + "\n"


def replace_entry(text: str, name: str, new: str) -> str:
    head = f"[frontier.{name}]\n"
    start = text.index(head)
    end = text.find("\n[frontier.", start + len(head))
    end = len(text) if end < 0 else end + 1
    return text[:start] + new + text[end:]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("names", nargs="*", help="frontier names (default: all)")
    parser.add_argument("--no-build", action="store_true", help="measure existing unit artifacts")
    parser.add_argument("--update", action="store_true", help="record improved residuals")
    parser.add_argument("--show", action="store_true", help="print differing instructions")
    parser.add_argument("--add", metavar="NAME", help="record a new frontier from its current measurement")
    parser.add_argument("--unit")
    parser.add_argument("--function")
    parser.add_argument("--symbol")
    parser.add_argument("--address", type=lambda v: int(v, 0))
    parser.add_argument("--size", type=lambda v: int(v, 0))
    args = parser.parse_args()

    units = tomllib.loads(MANIFEST.read_text())["units"]
    text = CONFIG.read_text()
    config = tomllib.loads(text).get("frontier", {})

    if args.add:
        if args.add in config:
            print(f"frontier {args.add!r} already exists", file=sys.stderr)
            return 2
        if not args.unit or not args.function:
            print("--add needs --unit and --function", file=sys.stderr)
            return 2
        entry = {"function": args.function, "unit": args.unit, "residual": []}
        for key in ("symbol", "address", "size"):
            if getattr(args, key) is not None:
                entry[key] = getattr(args, key)
        resolved = resolve(entry, units)
        if not args.no_build:
            build(args.unit)
        target = verified_target()
        length, differing, original, _ = measure(target, resolved)
        entry.update(candidate_size=length, target_sha256=hashlib.sha256(original).hexdigest(),
                     residual=format_offsets(differing))
        CONFIG.write_text(text.rstrip("\n") + "\n\n" + entry_text(args.add, entry))
        print(f"recorded {args.add}: {length}/{resolved['size']} bytes, {len(differing)} differ")
        return 0

    names = args.names or sorted(config)
    unknown = [n for n in names if n not in config]
    if unknown:
        print(f"unknown frontier(s): {', '.join(unknown)}; known: {', '.join(sorted(config))}",
              file=sys.stderr)
        return 2
    resolved = {n: resolve(config[n], units) for n in names}
    if not args.no_build:
        built: set[str] = set()
        for name in names:
            directory = resolved[name]["output_dir"]
            if directory not in built:
                print(f"building {config[name]['unit']} ({directory})", flush=True)
                build(config[name]["unit"])
                built.add(directory)
    target = verified_target()

    status = 0
    updates: dict[str, tuple[int, set[int]]] = {}
    for name in names:
        entry = resolved[name]
        length, differing, original, candidate = measure(target, entry)
        verdict, new = classify(entry, length, differing)
        recorded = parse_offsets(entry["residual"])
        label = (f"{name}: {length}/{entry['size']} bytes, {len(differing)} differ "
                 f"(recorded {len(recorded)})")
        if verdict == "REGRESSED":
            status = 1
            print(f"REGRESSED {label}\n    newly differing offsets: {', '.join(format_offsets(new))}")
        elif verdict == "EXACT":
            print(f"EXACT     {label}: add linkages and run compare-linked-function.py")
            updates[name] = (length, differing)
        elif verdict == "IMPROVED":
            print(f"IMPROVED  {label}" + ("" if args.update else " (run with --update to record)"))
            updates[name] = (length, differing)
        else:
            print(f"OK        {label}")
        if args.show and differing:
            show(original, candidate, differing)

    if args.update and updates and status == 0:
        for name, (length, differing) in updates.items():
            entry = dict(config[name], candidate_size=length, residual=format_offsets(differing))
            text = replace_entry(text, name, entry_text(name, entry))
        CONFIG.write_text(text)
        print(f"recorded {len(updates)} improved residual(s) in {CONFIG}")
    return status


if __name__ == "__main__":
    raise SystemExit(main())
