#!/usr/bin/env python3
"""Cold-build shared compiler artifacts and replay canonical exact units."""

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
    groups: dict[tuple[object, ...], list[str]] = defaultdict(list)
    for name in names:
        unit = units[name]
        kind = str(unit["artifact_kind"])
        if kind == "coff":
            key = (
                kind,
                str(unit["source"]),
                tuple(str(flag) for flag in unit["profile"]),
                str(unit["object"]),
            )
        elif kind == "linked-pe":
            key = (
                kind,
                str(unit["source"]),
                tuple(str(flag) for flag in unit["profile"]),
                str(unit["output_dir"]),
                str(unit["entry_symbol"]),
                tuple(str(flag) for flag in unit["link_profile"]),
                str(unit["harness"]),
                tuple(str(value) for value in unit.get("support_sources", [])),
            )
        else:
            raise ValueError(f"unit {name!r} has an unsupported artifact kind")
        groups[key].append(name)

    reports = []
    total_bytes = 0
    for key, group_names in groups.items():
        kind, source, profile, artifact_name = key[:4]
        artifact_path = (ROOT / str(artifact_name)).resolve()
        artifact_path.relative_to((ROOT / "build").resolve())
        removed = []
        if kind == "coff":
            for path in (artifact_path, artifact_path.with_suffix(".pdb")):
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
            "artifact_kind": kind,
            "source": source,
            "profile": list(profile),
            "artifact": artifact_name,
            "build_unit": build_unit,
            "build_returncode": built.returncode,
            "units": [],
        }
        if kind == "coff":
            group_report["cold_removed"] = removed
        else:
            group_report["cold_driver"] = "scripts/ltcg_link.py:cold_link"
        if built.returncode != 0:
            group_report["build_stdout"] = built.stdout
            group_report["build_stderr"] = built.stderr
            reports.append(group_report)
            raise ValueError(
                f"build failed for {source}: {built.stderr.strip() or built.stdout.strip()}"
            )

        for name in group_names:
            comparator = (
                "scripts/compare-coff-function.py"
                if kind == "coff"
                else "scripts/compare-linked-function.py"
            )
            compared = run(
                [
                    sys.executable,
                    comparator,
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
        "source_count": len({str(units[name]["source"]) for name in names}),
        "artifact_count": len(groups),
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
                f"from one cold {group['artifact_kind']} build"
            )
        print(
            f"canonical exact replay passed: {report['unit_count']} unit(s), "
            f"{report['matched_bytes']} bytes across "
            f"{report['artifact_count']} cold artifact build(s) and "
            f"{report['source_count']} source file(s)"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
