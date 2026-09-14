#!/usr/bin/env python3
"""Cold-build shared normal-COFF objects and replay canonical exact units."""

from __future__ import annotations

import argparse
from collections import defaultdict
import json
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "match-units.toml"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    selection = parser.add_mutually_exclusive_group()
    selection.add_argument(
        "--unit",
        action="append",
        default=[],
        help="replay only this unit (repeatable); default: all units",
    )
    selection.add_argument(
        "--source",
        action="append",
        default=[],
        help="replay every unit for this source (repeatable); default: all sources",
    )
    parser.add_argument("--json", action="store_true", help="emit one JSON report")
    return parser.parse_args()


def load_units() -> dict[str, dict[str, object]]:
    with MANIFEST.open("rb") as stream:
        units = tomllib.load(stream).get("units")
    if not isinstance(units, dict):
        raise ValueError("match-units.toml [units] must be a table")
    if not all(isinstance(unit, dict) for unit in units.values()):
        raise ValueError("every match unit must be a table")
    return units


def select_units(
    units: dict[str, dict[str, object]], args: argparse.Namespace
) -> list[str]:
    if args.unit:
        unknown = sorted(set(args.unit) - set(units))
        if unknown:
            raise ValueError(f"unknown unit(s): {unknown}")
        wanted = set(args.unit)
        return [name for name in units if name in wanted]
    if args.source:
        available = {str(unit["source"]) for unit in units.values()}
        unknown = sorted(set(args.source) - available)
        if unknown:
            raise ValueError(f"unknown source(s): {unknown}")
        wanted = set(args.source)
        return [name for name, unit in units.items() if unit["source"] in wanted]
    return list(units)


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )


def replay(
    units: dict[str, dict[str, object]], names: list[str]
) -> dict[str, object]:
    groups: dict[tuple[str, tuple[str, ...], str], list[str]] = defaultdict(list)
    for name in names:
        unit = units[name]
        key = (
            str(unit["source"]),
            tuple(str(flag) for flag in unit["profile"]),
            str(unit["object"]),
        )
        groups[key].append(name)

    reports = []
    total_bytes = 0
    for (source, profile, object_name), group_names in groups.items():
        object_path = (ROOT / object_name).resolve()
        object_path.relative_to((ROOT / "build").resolve())
        pdb_path = object_path.with_suffix(".pdb")
        removed = []
        for path in (object_path, pdb_path):
            if path.exists():
                if not path.is_file():
                    raise ValueError(f"cold-build output is not a file: {path}")
                path.unlink()
                removed.append(str(path.relative_to(ROOT)))

        build_unit = group_names[0]
        built = run(
            [sys.executable, "scripts/build-match-unit.py", "--unit", build_unit]
        )
        group_report: dict[str, object] = {
            "source": source,
            "profile": list(profile),
            "object": object_name,
            "build_unit": build_unit,
            "cold_removed": removed,
            "build_returncode": built.returncode,
            "units": [],
        }
        if built.returncode != 0:
            group_report["build_stdout"] = built.stdout
            group_report["build_stderr"] = built.stderr
            reports.append(group_report)
            raise ValueError(
                f"build failed for {source}: {built.stderr.strip() or built.stdout.strip()}"
            )

        for name in group_names:
            compared = run(
                [
                    sys.executable,
                    "scripts/compare-coff-function.py",
                    "--unit",
                    name,
                    "--json",
                ]
            )
            try:
                report = json.loads(compared.stdout)
            except json.JSONDecodeError as exc:
                raise ValueError(
                    f"unit {name!r} did not return JSON: "
                    f"stdout={compared.stdout!r} stderr={compared.stderr!r}"
                ) from exc
            report["returncode"] = compared.returncode
            group_report["units"].append(report)
            if compared.returncode != 0 or report.get("result") != "exact":
                reports.append(group_report)
                raise ValueError(
                    f"unit {name!r} failed exact replay: "
                    f"{json.dumps(report, sort_keys=True)}"
                )
            total_bytes += int(report["size"])
        reports.append(group_report)

    return {
        "result": "exact",
        "unit_count": len(names),
        "source_count": len(groups),
        "matched_bytes": total_bytes,
        "groups": reports,
    }


def main() -> int:
    args = parse_args()
    try:
        checked = run(
            [sys.executable, "scripts/build-match-unit.py", "--check"]
        )
        if checked.returncode != 0:
            raise ValueError(checked.stderr.strip() or checked.stdout.strip())
        units = load_units()
        names = select_units(units, args)
        if not names:
            raise ValueError("no exact units selected")
        report = replay(units, names)
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        if args.json:
            print(json.dumps({"result": "error", "error": str(exc)}, indent=2))
        else:
            print(f"error: exact replay failed: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        for group in report["groups"]:
            print(
                f"{group['source']}: exact {len(group['units'])} unit(s) "
                f"from one cold object build"
            )
        print(
            f"canonical exact replay passed: {report['unit_count']} unit(s), "
            f"{report['matched_bytes']} bytes across "
            f"{report['source_count']} source object(s)"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
