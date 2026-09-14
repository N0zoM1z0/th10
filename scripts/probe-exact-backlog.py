#!/usr/bin/env python3
"""Cold-compile and structurally probe the authored TH10 exact backlog."""

from __future__ import annotations

import argparse
from collections import defaultdict
import json
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "probe-backlog"
PROFILE = ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc"]


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


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )


def json_command(command: list[str], label: str) -> dict[str, object]:
    completed = run(command)
    try:
        report = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise ValueError(
            f"{label} did not return JSON: stdout={completed.stdout!r} "
            f"stderr={completed.stderr!r}"
        ) from exc
    if completed.returncode != 0 or report.get("result") == "error":
        raise ValueError(f"{label} failed: {json.dumps(report, sort_keys=True)}")
    return report


def load_backlog(sources: list[str]) -> list[dict[str, object]]:
    command = [sys.executable, "scripts/report-exact-backlog.py", "--json"]
    for source in sources:
        command.extend(["--source", source])
    report = json_command(command, "exact backlog report")
    items = report.get("items")
    if not isinstance(items, list) or not all(isinstance(item, dict) for item in items):
        raise ValueError("exact backlog report has invalid items")
    return items


def cold_object(source: str) -> tuple[Path, list[str]]:
    source_path = (ROOT / source).resolve()
    source_path.relative_to(ROOT.resolve())
    if not source_path.is_file():
        raise ValueError(f"missing source: {source}")
    BUILD.mkdir(parents=True, exist_ok=True)
    stem = source.replace("/", "_").replace("\\", "_")
    object_path = (BUILD / Path(stem).with_suffix(".obj").name).resolve()
    object_path.relative_to(BUILD.resolve())
    removed = []
    for path in (object_path, object_path.with_suffix(".pdb")):
        if path.exists():
            if not path.is_file():
                raise ValueError(f"cold-build output is not a file: {path}")
            path.unlink()
            removed.append(str(path.relative_to(ROOT)))
    built = run(
        [
            str(ROOT / "scripts" / "compile-probe.sh"),
            str(source_path),
            str(object_path),
            *PROFILE,
        ]
    )
    if built.returncode != 0:
        raise ValueError(
            f"compile failed for {source}: {built.stderr.strip() or built.stdout.strip()}"
        )
    if not object_path.is_file():
        raise ValueError(f"compile produced no object for {source}")
    return object_path, removed


def probe_source(source: str, backlog: list[dict[str, object]]) -> dict[str, object]:
    object_path, removed = cold_object(source)
    inventory = json_command(
        [
            sys.executable,
            "scripts/compare-coff-function.py",
            str(object_path),
            "--list-functions",
            "--json",
        ],
        f"COFF inventory for {source}",
    )
    functions = inventory.get("functions")
    if not isinstance(functions, list) or not all(
        isinstance(function, dict) for function in functions
    ):
        raise ValueError(f"COFF inventory for {source} has invalid functions")
    by_hint: dict[str, list[dict[str, object]]] = defaultdict(list)
    for function in functions:
        hint = function.get("source_name_hint")
        if isinstance(hint, str) and hint:
            by_hint[hint].append(function)

    reports = []
    for item in backlog:
        name = str(item["name"])
        matches = by_hint.get(name, [])
        base = {
            "address": item["address"],
            "target_size": item["size"],
            "name": name,
            "source": source,
        }
        if len(matches) != 1:
            reports.append(
                {
                    **base,
                    "result": "unresolved",
                    "reason": (
                        "no unique external COFF symbol"
                        if not matches
                        else "multiple external COFF symbols"
                    ),
                    "symbols": [match["symbol"] for match in matches],
                    "acceptance_authority": "none",
                }
            )
            continue
        function = matches[0]
        if function.get("size") is None:
            reports.append(
                {
                    **base,
                    "result": "unresolved",
                    "reason": "COFF function extent unavailable",
                    "symbols": [function["symbol"]],
                    "acceptance_authority": "none",
                }
            )
            continue
        compared = run(
            [
                sys.executable,
                "scripts/compare-coff-function.py",
                str(object_path),
                str(function["symbol"]),
                str(item["address"]),
                str(item["size"]),
                "--json",
            ]
        )
        try:
            comparison = json.loads(compared.stdout)
        except json.JSONDecodeError as exc:
            raise ValueError(
                f"probe {name} did not return JSON: stdout={compared.stdout!r} "
                f"stderr={compared.stderr!r}"
            ) from exc
        if comparison.get("result") == "error":
            raise ValueError(
                f"probe {name} failed: {json.dumps(comparison, sort_keys=True)}"
            )
        if compared.returncode not in (0, 1):
            raise ValueError(f"probe {name} returned {compared.returncode}")
        reports.append({**base, **comparison})
    counts: dict[str, int] = defaultdict(int)
    for report in reports:
        counts[str(report["result"])] += 1
    return {
        "source": source,
        "object": str(object_path.relative_to(ROOT)),
        "cold_removed": removed,
        "function_count": len(reports),
        "counts": dict(sorted(counts.items())),
        "functions": reports,
    }


def render_text(report: dict[str, object], show: str) -> None:
    for source_report in report["sources"]:
        print(f"{source_report['source']}: {source_report['counts']}")
        functions = source_report["functions"]
        for function in functions:
            result = str(function["result"])
            if show != "all" and result != show:
                continue
            if result == "unresolved":
                detail = str(function["reason"])
            else:
                detail = (
                    f"object={function['object_size']} target={function['size']} "
                    f"comparable={function['matched_comparable_bytes']}/"
                    f"{function['comparable_bytes']} relocs="
                    f"{len(function['relocations'])}"
                )
            print(
                f"  {result:16s} {function['address']} "
                f"{function['name']}  {detail}"
            )
    print(
        f"backlog probe complete: {report['function_count']} function(s), "
        f"{report['counts']}"
    )
    print("probe results are diagnostic and grant no exactness credit")


def main() -> int:
    args = parse_args()
    try:
        backlog = load_backlog(args.source)
        by_source: dict[str, list[dict[str, object]]] = defaultdict(list)
        for item in backlog:
            by_source[str(item["source"])].append(item)
        source_reports = [
            probe_source(source, items) for source, items in sorted(by_source.items())
        ]
        counts: dict[str, int] = defaultdict(int)
        for source_report in source_reports:
            for result, count in source_report["counts"].items():
                counts[result] += int(count)
        report: dict[str, object] = {
            "schema_version": 1,
            "result": "ok",
            "acceptance_authority": "none",
            "profile": PROFILE,
            "source_count": len(source_reports),
            "function_count": len(backlog),
            "counts": dict(sorted(counts.items())),
            "sources": source_reports,
        }
    except (OSError, KeyError, TypeError, ValueError) as exc:
        if args.json:
            print(json.dumps({"result": "error", "error": str(exc)}, indent=2))
        else:
            print(f"error: exact backlog probe failed: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        render_text(report, args.show)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
