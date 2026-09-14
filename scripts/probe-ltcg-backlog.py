#!/usr/bin/env python3
"""Compile, link, and structurally probe TH10 source backlog under VC7.1 LTCG."""

from __future__ import annotations

import argparse
from collections import defaultdict
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import tomllib

import capstone
from capstone import CS_ARCH_X86, CS_GRP_CALL, CS_GRP_JUMP, CS_MODE_32, Cs
from capstone.x86_const import X86_OP_IMM

from linked_image import LinkedImageError, PEImage, linked_functions, map_publics
from msvc_symbols import source_name_hint


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "probe-ltcg"
TARGET = ROOT / "resources" / "th10.exe"
TARGET_CONFIG = ROOT / "config" / "target.toml"
TOOLS_LOCK = ROOT / "config" / "tools.lock.toml"
NORMAL_PROFILE = ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc"]
LTCG_PROFILE = [*NORMAL_PROFILE, "/GL"]
ANCHOR_PREFIX = "_th10_ltcg_probe_anchor_"
UNRESOLVED_MARKER = "unresolved external symbol "


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source",
        action="append",
        default=[],
        help="probe one exact source path (repeatable); default: all authored backlog",
    )
    parser.add_argument(
        "--show",
        choices=("all", "structural-exact", "mismatch", "unresolved"),
        default="all",
        help="filter text output; JSON always contains every result",
    )
    parser.add_argument("--json", action="store_true", help="emit one JSON report")
    return parser.parse_args()


def run(
    command: list[str],
    *,
    environment: dict[str, str] | None = None,
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
    )


def json_command(command: list[str], label: str) -> dict[str, object]:
    completed = run(command)
    try:
        report = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise ValueError(f"{label} did not return JSON: {completed.stdout!r}") from exc
    if completed.returncode != 0 or report.get("result") == "error":
        raise ValueError(f"{label} failed: {json.dumps(report, sort_keys=True)}")
    return report


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def verified_target() -> PEImage:
    with TARGET_CONFIG.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    if TARGET.stat().st_size != int(expected["size"]) or sha256(TARGET) != str(
        expected["sha256"]
    ).lower():
        raise ValueError("target identity mismatch")
    return PEImage(TARGET)


def tool_environment() -> tuple[Path, dict[str, str]]:
    with TOOLS_LOCK.open("rb") as stream:
        lock = tomllib.load(stream)["msvc71"]
    msvc_root = Path(
        os.environ.get("TH10_MSVC71_ROOT", ROOT / ".tools" / "msvc710-sp1")
    ).resolve()
    linker = msvc_root / "Vc7" / "bin" / "link.exe"
    if not linker.is_file() or sha256(linker) != str(lock["linker_sha256"]).lower():
        raise ValueError("pinned VC7.1 SP1 linker identity mismatch")
    prefix = Path(
        os.environ.get(
            "TH10_WINEPREFIX",
            os.environ.get("WINEPREFIX", ROOT / ".tools" / "wineprefix-msvc710-sp1"),
        )
    ).resolve()
    environment = os.environ.copy()
    environment.update(
        {
            "WINEARCH": "win32",
            "WINEPREFIX": str(prefix),
            "TH10_WINEPREFIX": str(prefix),
            "WINEDEBUG": "-all",
            "WINEDLLOVERRIDES": "mscoree,mshtml=",
        }
    )
    return linker, environment


def windows_path(path: Path, environment: dict[str, str]) -> str:
    converted = run(["winepath", "-w", str(path.resolve())], environment=environment)
    if converted.returncode != 0:
        raise ValueError(f"winepath failed for {path}: {converted.stdout.strip()}")
    return converted.stdout.strip().replace("\r", "")


def remove_files(paths: list[Path]) -> list[str]:
    removed = []
    for path in paths:
        if not path.exists():
            continue
        if not path.is_file():
            raise ValueError(f"cold-build output is not a file: {path}")
        path.unlink()
        removed.append(str(path.relative_to(ROOT)))
    return removed


def compile_source(source: Path, output: Path, profile: list[str]) -> None:
    remove_files([output, output.with_suffix(".pdb"), output.with_suffix(".idb")])
    completed = run(
        [
            str(ROOT / "scripts" / "compile-probe.sh"),
            str(source),
            str(output),
            *profile,
        ]
    )
    if completed.returncode != 0 or not output.is_file():
        raise ValueError(
            f"compile failed for {source.relative_to(ROOT)}: {completed.stdout.strip()}"
        )


def load_backlog(sources: list[str]) -> list[dict[str, object]]:
    command = [sys.executable, "scripts/report-exact-backlog.py", "--json"]
    for source in sources:
        command.extend(["--source", source])
    report = json_command(command, "exact backlog report")
    items = report.get("items")
    if not isinstance(items, list) or not all(isinstance(item, dict) for item in items):
        raise ValueError("exact backlog report has invalid items")
    return items


def normal_inventory(source: Path, output: Path) -> list[dict[str, object]]:
    compile_source(source, output, NORMAL_PROFILE)
    report = json_command(
        [
            sys.executable,
            "scripts/compare-coff-function.py",
            str(output),
            "--list-functions",
            "--json",
        ],
        f"normal COFF inventory for {source.relative_to(ROOT)}",
    )
    functions = report.get("functions")
    if not isinstance(functions, list) or not all(
        isinstance(function, dict) for function in functions
    ):
        raise ValueError("normal COFF inventory has invalid functions")
    return functions


def unresolved_symbols(output: str) -> list[str]:
    result = []
    for line in output.splitlines():
        if UNRESOLVED_MARKER not in line:
            continue
        tail = line.split(UNRESOLVED_MARKER, 1)[1]
        decorated = re.search(r"\((\S+)\)(?:\s+referenced.*)?$", tail)
        symbol = decorated.group(1) if decorated else tail.split()[0]
        if symbol not in result:
            result.append(symbol)
    return sorted(result)


def link_command(
    linker: Path,
    environment: dict[str, str],
    source_object: Path,
    entry: str,
    image: Path,
    map_path: Path,
    pdb: Path,
    anchor_object: Path | None,
    aliases: Path | None,
) -> list[str]:
    command = [
        str(ROOT / "scripts" / "run-headless-wine.sh"),
        os.environ.get("WINE", "wine"),
        str(linker),
        "/nologo",
        "/ltcg",
        "/nodefaultlib",
        "/subsystem:console",
        f"/entry:{entry}",
        "/incremental:no",
        "/fixed:no",
        "/base:0x00400000",
        "/debug",
        "/debugtype:cv",
        "/opt:noref",
        "/opt:noicf",
        "/safeseh:no",
        f"/map:{windows_path(map_path, environment)}",
        f"/pdb:{windows_path(pdb, environment)}",
        f"/out:{windows_path(image, environment)}",
        windows_path(source_object, environment),
    ]
    if anchor_object is not None:
        command.append(windows_path(anchor_object, environment))
    if aliases is not None:
        command.append("@" + windows_path(aliases, environment))
    return command


def write_anchors(directory: Path, symbols: list[str]) -> tuple[Path, Path, dict[str, str]]:
    source = directory / "link-anchors.c"
    output = directory / "link-anchors.obj"
    aliases = directory / "link-aliases.rsp"
    bindings = {
        symbol: f"{ANCHOR_PREFIX}{index:04d}"
        for index, symbol in enumerate(symbols)
    }
    source.write_text(
        "/* Diagnostic-only, non-runnable unresolved-symbol address anchors. */\n"
        + "".join(
            f"unsigned char {anchor[1:]}[4] = {{0}};\n"
            for anchor in bindings.values()
        ),
        encoding="ascii",
    )
    aliases.write_text(
        "".join(f"/alternatename:{symbol}={anchor}\n" for symbol, anchor in bindings.items()),
        encoding="ascii",
    )
    compile_source(source, output, ["/TC", "/O2", "/Gy", "/GS-", "/DNDEBUG"])
    return output, aliases, bindings


def cold_link(
    source: Path,
    directory: Path,
    entry: str,
    linker: Path,
    environment: dict[str, str],
) -> dict[str, object]:
    ltcg_object = directory / "source.ltcg.obj"
    image = directory / "source.exe"
    map_path = directory / "source.map"
    pdb = directory / "source.pdb"
    old_logs = sorted(directory.glob("link-attempt-*.log"))
    removed = remove_files(
        [
            image,
            map_path,
            pdb,
            image.with_suffix(".ilk"),
            *old_logs,
        ]
    )
    compile_source(source, ltcg_object, LTCG_PROFILE)
    symbols: list[str] = []
    bindings: dict[str, str] = {}
    logs = []
    anchor_object = None
    aliases = None
    for attempt in range(1, 9):
        remove_files([image, map_path, pdb, image.with_suffix(".ilk")])
        if symbols:
            anchor_object, aliases, bindings = write_anchors(directory, symbols)
        linked = run(
            link_command(
                linker,
                environment,
                ltcg_object,
                entry,
                image,
                map_path,
                pdb,
                anchor_object,
                aliases,
            ),
            environment=environment,
        )
        log = directory / f"link-attempt-{attempt}.log"
        log.write_text(linked.stdout, encoding="utf-8", errors="replace")
        logs.append(log)
        if linked.returncode == 0:
            break
        observed = unresolved_symbols(linked.stdout)
        new_symbols = sorted(set(observed) - set(symbols))
        if not observed:
            raise ValueError(
                "LTCG link failed without parseable unresolved symbols: "
                f"{linked.stdout.strip()}"
            )
        if not new_symbols:
            raise ValueError(
                "anchored LTCG link repeated the same unresolved symbols: "
                + ", ".join(observed)
            )
        symbols = sorted(set(symbols) | set(new_symbols))
    else:
        raise ValueError("anchored LTCG link exceeded eight resolution attempts")
    if not all(path.is_file() for path in (image, map_path, pdb)):
        raise ValueError("LTCG link did not produce image, map, and PDB")
    return {
        "object": ltcg_object,
        "image": image,
        "map": map_path,
        "pdb": pdb,
        "cold_removed": removed,
        "unresolved_symbols": symbols,
        "anchor_bindings": bindings,
        "link_logs": logs,
    }


def target_field_value(
    target: bytes,
    target_address: int,
    offset: int,
    width: int,
    kind: str,
    instruction_end: int | None,
) -> tuple[str | None, str | None]:
    if offset < 0 or offset + width > len(target):
        return None, None
    raw = target[offset : offset + width]
    encoded = "0x" + raw[::-1].hex().upper()
    if kind == "DIR32":
        candidate = int.from_bytes(raw, "little")
    else:
        assert instruction_end is not None
        displacement = int.from_bytes(raw, "little", signed=True)
        candidate = target_address + instruction_end + displacement
    return encoded, f"0x{candidate & 0xFFFFFFFF:08X}"


def structural_compare(
    candidate_image: PEImage,
    publics: list[dict[str, object]],
    candidate_address: int,
    candidate_size: int,
    target_image: PEImage,
    target_address: int,
    target_size: int,
) -> dict[str, object]:
    candidate = candidate_image.read_address(candidate_address, candidate_size)
    target = target_image.read_address(target_address, target_size)
    symbols_by_address: dict[int, list[str]] = defaultdict(list)
    for public in publics:
        symbols_by_address[int(public["address"])].append(str(public["symbol"]))
    fields: list[dict[str, object]] = []
    for address in candidate_image.base_relocations():
        if candidate_address <= address and address + 4 <= candidate_address + candidate_size:
            offset = address - candidate_address
            destination = struct.unpack_from("<I", candidate, offset)[0]
            encoded, target_candidate = target_field_value(
                target, target_address, offset, 4, "DIR32", None
            )
            fields.append(
                {
                    "offset": offset,
                    "width": 4,
                    "type": "DIR32",
                    "candidate_target": f"0x{destination:08X}",
                    "candidate_symbols": sorted(symbols_by_address.get(destination, [])),
                    "target_encoded_value": encoded,
                    "target_candidate": target_candidate,
                }
            )

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    decoded = 0
    for instruction in decoder.disasm(candidate, candidate_address):
        if instruction.address != candidate_address + decoded:
            break
        decoded += instruction.size
        is_control = instruction.group(CS_GRP_CALL) or instruction.group(CS_GRP_JUMP)
        if not is_control or not instruction.operands:
            continue
        operand = instruction.operands[0]
        if operand.type != X86_OP_IMM or not instruction.imm_size:
            continue
        destination = int(operand.imm) & 0xFFFFFFFF
        if candidate_address <= destination < candidate_address + candidate_size:
            continue
        offset = instruction.address - candidate_address + instruction.imm_offset
        width = instruction.imm_size
        kind = f"REL{width * 8}"
        encoded, target_candidate = target_field_value(
            target,
            target_address,
            offset,
            width,
            kind,
            instruction.address - candidate_address + instruction.size,
        )
        fields.append(
            {
                "offset": offset,
                "width": width,
                "type": kind,
                "instruction": instruction.mnemonic,
                "candidate_target": f"0x{destination:08X}",
                "candidate_symbols": sorted(symbols_by_address.get(destination, [])),
                "target_encoded_value": encoded,
                "target_candidate": target_candidate,
            }
        )
    normalization_complete = decoded == len(candidate)
    fields.sort(key=lambda field: (int(field["offset"]), str(field["type"])))
    candidate_occupied: set[int] = set()
    occupied: set[int] = set()
    for field in fields:
        candidate_extent = set(
            range(
                int(field["offset"]),
                int(field["offset"]) + int(field["width"]),
            )
        )
        if candidate_occupied & candidate_extent:
            raise ValueError("overlapping linked-image fields")
        candidate_occupied.update(candidate_extent)
        occupied.update(offset for offset in candidate_extent if offset < target_size)
    differences = []
    raw_matched = 0
    for offset, right in enumerate(target):
        left = candidate[offset] if offset < len(candidate) else None
        if left == right:
            raw_matched += 1
        if offset not in occupied and left != right:
            differences.append({"offset": f"0x{offset:X}", "candidate": left, "target": right})
    comparable = target_size - len(occupied)
    return {
        "result": (
            "structural-exact"
            if normalization_complete
            and candidate_size == target_size
            and not differences
            else "mismatch"
        ),
        "candidate_address": f"0x{candidate_address:08X}",
        "candidate_size": candidate_size,
        "target_address": f"0x{target_address:08X}",
        "target_size": target_size,
        "decoded_candidate_bytes": decoded,
        "normalization_complete": normalization_complete,
        "raw_matched_bytes": raw_matched,
        "comparable_bytes": comparable,
        "matched_comparable_bytes": comparable - len(differences),
        "linkage_fields": fields,
        "first_differences": differences[:32],
        "acceptance_authority": "none",
    }


def probe_source(
    source_name: str,
    backlog: list[dict[str, object]],
    target: PEImage,
    linker: Path,
    environment: dict[str, str],
) -> dict[str, object]:
    source = (ROOT / source_name).resolve()
    source.relative_to(ROOT.resolve())
    if not source.is_file():
        raise ValueError(f"missing source: {source_name}")
    directory = BUILD / source_name.replace("/", "_").replace("\\", "_")
    directory.mkdir(parents=True, exist_ok=True)
    inventory = normal_inventory(source, directory / "source.normal.obj")
    symbols_by_hint: dict[str, list[str]] = defaultdict(list)
    for function in inventory:
        hint = function.get("source_name_hint")
        if isinstance(hint, str) and hint:
            symbols_by_hint[hint].append(str(function["symbol"]))
    entry_candidates = [
        symbols_by_hint[str(item["name"])][0]
        for item in backlog
        if len(symbols_by_hint.get(str(item["name"]), [])) == 1
    ]
    if not entry_candidates:
        raise ValueError(f"no external entry symbol can be derived for {source_name}")
    linked = cold_link(source, directory, entry_candidates[0], linker, environment)
    linked_report = linked_functions(linked["image"], linked["map"], linked["pdb"])
    publics = map_publics(linked["map"])
    candidate_image = PEImage(linked["image"])
    linked_by_symbol: dict[str, list[dict[str, object]]] = defaultdict(list)
    for function in linked_report["functions"]:
        pdb_object_name = str(function["pdb_object"]).replace("\\", "/").rsplit("/", 1)[-1]
        if pdb_object_name.lower() == "source.ltcg.obj":
            linked_by_symbol[str(function["symbol"])].append(function)

    reports = []
    for item in backlog:
        name = str(item["name"])
        normal_matches = symbols_by_hint.get(name, [])
        base = {
            "name": name,
            "source": source_name,
            "address": item["address"],
            "size": item["size"],
        }
        if len(normal_matches) != 1:
            reports.append(
                {
                    **base,
                    "result": "unresolved",
                    "reason": "no unique normal-COFF decorated-symbol bridge",
                    "symbols": normal_matches,
                    "acceptance_authority": "none",
                }
            )
            continue
        symbol = normal_matches[0]
        linked_matches = linked_by_symbol.get(symbol, [])
        if len(linked_matches) != 1:
            reports.append(
                {
                    **base,
                    "result": "unresolved",
                    "reason": "no unique PDB-owned linked contribution",
                    "symbols": [symbol],
                    "linked_match_count": len(linked_matches),
                    "acceptance_authority": "none",
                }
            )
            continue
        function = linked_matches[0]
        comparison = structural_compare(
            candidate_image,
            publics,
            int(str(function["address"]), 0),
            int(function["size"]),
            target,
            int(str(item["address"]), 0),
            int(item["size"]),
        )
        reports.append(
            {
                **base,
                **comparison,
                "symbol": symbol,
                "extent_source": function["extent_source"],
                "pdb_module_index": function["pdb_module_index"],
            }
        )
    counts: dict[str, int] = defaultdict(int)
    for report in reports:
        counts[str(report["result"])] += 1
    return {
        "source": source_name,
        "entry_symbol": entry_candidates[0],
        "normal_profile": NORMAL_PROFILE,
        "ltcg_profile": LTCG_PROFILE,
        "link_harness": "diagnostic-unresolved-symbol-data-anchors",
        "physical_ownership": "unknown",
        "object": str(Path(linked["object"]).relative_to(ROOT)),
        "image": str(Path(linked["image"]).relative_to(ROOT)),
        "map": str(Path(linked["map"]).relative_to(ROOT)),
        "pdb": str(Path(linked["pdb"]).relative_to(ROOT)),
        "unresolved_symbol_count": len(linked["unresolved_symbols"]),
        "anchor_bindings": linked["anchor_bindings"],
        "unowned_code_contribution_count": linked_report[
            "unowned_code_contribution_count"
        ],
        "function_count": len(reports),
        "counts": dict(sorted(counts.items())),
        "functions": reports,
    }


def render_text(report: dict[str, object], show: str) -> None:
    for source in report["sources"]:
        print(
            f"{source['source']}: {source['counts']} "
            f"anchors={source['unresolved_symbol_count']}"
        )
        for function in source["functions"]:
            result = str(function["result"])
            if show != "all" and result != show:
                continue
            if result == "unresolved":
                detail = function["reason"]
            else:
                detail = (
                    f"linked={function['candidate_size']} target={function['target_size']} "
                    f"comparable={function['matched_comparable_bytes']}/"
                    f"{function['comparable_bytes']} fields={len(function['linkage_fields'])}"
                )
            print(
                f"  {result:16s} {function['address']} "
                f"{function['name']}  {detail}"
            )
    print(
        f"LTCG backlog probe complete: {report['function_count']} function(s), "
        f"{report['counts']}"
    )
    print("stubbed linked-image diagnostics grant no exactness credit")


def main() -> int:
    args = parse_args()
    try:
        backlog = load_backlog(args.source)
        by_source: dict[str, list[dict[str, object]]] = defaultdict(list)
        for item in backlog:
            by_source[str(item["source"])].append(item)
        target = verified_target()
        linker, environment = tool_environment()
        source_reports = [
            probe_source(source, items, target, linker, environment)
            for source, items in sorted(by_source.items())
        ]
        counts: dict[str, int] = defaultdict(int)
        for source in source_reports:
            for result, count in source["counts"].items():
                counts[result] += int(count)
        report: dict[str, object] = {
            "schema_version": 1,
            "result": "ok",
            "acceptance_authority": "none",
            "artifact_kind": "stubbed-linked-pe-diagnostic",
            "decoder": {"name": "capstone", "version": capstone.__version__},
            "source_count": len(source_reports),
            "function_count": len(backlog),
            "counts": dict(sorted(counts.items())),
            "sources": source_reports,
        }
    except (
        KeyError,
        LinkedImageError,
        OSError,
        struct.error,
        TypeError,
        ValueError,
        tomllib.TOMLDecodeError,
    ) as exc:
        report = {"result": "error", "error": str(exc)}
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    elif report["result"] == "error":
        print(f"error: LTCG backlog probe failed: {report['error']}", file=sys.stderr)
    else:
        render_text(report, args.show)
    return 0 if report["result"] == "ok" else 1


if __name__ == "__main__":
    raise SystemExit(main())
