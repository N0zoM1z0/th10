#!/usr/bin/env python3
"""Cold-probe and rank the authored TH10 exact backlog in both codegen lanes."""

from __future__ import annotations

import argparse
from fractions import Fraction
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th10.exe"
TARGET_CONFIG = ROOT / "config" / "target.toml"
IDENTITY_INPUTS = (
    "config/functions.csv",
    "config/function-origins.csv",
    "config/reccmp-functions.csv",
    "config/matches.csv",
    "config/target.toml",
    "config/tools.lock.toml",
    "scripts/compare-coff-function.py",
    "scripts/linked_image.py",
    "scripts/ltcg_link.py",
    "scripts/report-exact-backlog.py",
    "scripts/probe-exact-backlog.py",
    "scripts/probe-ltcg-backlog.py",
    "scripts/rank-exact-backlog.py",
)
RESULT_ORDER = {"structural-exact": 0, "mismatch": 1, "unresolved": 2}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source",
        action="append",
        default=[],
        help="rank one exact source path (repeatable); default: all authored backlog",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=20,
        help="maximum ranked rows to emit; zero emits all (default: 20)",
    )
    parser.add_argument("--json", action="store_true", help="emit one JSON report")
    args = parser.parse_args()
    if args.limit < 0:
        parser.error("--limit must be non-negative")
    return args


def run_json(command: list[str], label: str) -> dict[str, object]:
    completed = subprocess.run(
        command,
        cwd=ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        report = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise ValueError(
            f"{label} did not return JSON: stdout={completed.stdout!r} "
            f"stderr={completed.stderr!r}"
        ) from exc
    if completed.returncode != 0 or report.get("result") == "error":
        raise ValueError(f"{label} failed: {json.dumps(report, sort_keys=True)}")
    if report.get("acceptance_authority") not in (None, "none"):
        raise ValueError(f"{label} unexpectedly claims acceptance authority")
    return report


def selected_command(script: str, sources: list[str]) -> list[str]:
    command = [sys.executable, script]
    for source in sources:
        command.extend(["--source", source])
    command.append("--json")
    return command


def verified_target() -> str:
    with TARGET_CONFIG.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != int(expected["size"]) or digest != str(expected["sha256"]):
        raise ValueError(f"target identity mismatch: size={len(data)} sha256={digest}")
    return digest


def input_identity(sources: set[str]) -> str:
    paths = [ROOT / relative for relative in IDENTITY_INPUTS]
    paths.append(TARGET)
    paths.extend(path for path in (ROOT / "src").rglob("*") if path.is_file())
    for source in sorted(sources):
        path = (ROOT / source).resolve()
        try:
            path.relative_to(ROOT.resolve())
        except ValueError as exc:
            raise ValueError(f"source leaves repository: {source}") from exc
        paths.append(path)
    digest = hashlib.sha256()
    for path in sorted(set(paths)):
        if not path.is_file():
            raise ValueError(f"ranking identity input is missing: {path}")
        relative = str(path.relative_to(ROOT)).replace("\\", "/")
        digest.update(relative.encode("utf-8"))
        digest.update(b"\0")
        digest.update(path.read_bytes())
        digest.update(b"\0")
    return digest.hexdigest()


def target_size(item: dict[str, object]) -> int:
    return int(item["target_size"] if "target_size" in item else item["size"])


def backlog_key(item: dict[str, object]) -> tuple[str, str, str, int]:
    address = f"0x{int(str(item['address']), 0):08X}"
    return address, str(item["name"]), str(item["source"]), target_size(item)


def flatten(report: dict[str, object], label: str) -> dict[tuple[str, str, str, int], dict[str, object]]:
    sources = report.get("sources")
    if not isinstance(sources, list):
        raise ValueError(f"{label} lacks source reports")
    flattened: dict[tuple[str, str, str, int], dict[str, object]] = {}
    for source in sources:
        if not isinstance(source, dict) or not isinstance(source.get("functions"), list):
            raise ValueError(f"{label} has an invalid source report")
        for function in source["functions"]:
            if not isinstance(function, dict):
                raise ValueError(f"{label} has an invalid function report")
            key = backlog_key(function)
            if key in flattened:
                raise ValueError(f"{label} repeats backlog key {key!r}")
            flattened[key] = function
    return flattened


def profile_metrics(profile: str, function: dict[str, object]) -> tuple[dict[str, object], Fraction | None]:
    result = str(function["result"])
    if result not in RESULT_ORDER:
        raise ValueError(f"unsupported {profile} result {result!r}")
    expected_size = target_size(function)
    candidate_field = "object_size" if profile == "coff" else "candidate_size"
    if result == "unresolved":
        return (
            {
                "result": result,
                "candidate_size": None,
                "target_size": expected_size,
                "size_delta": None,
                "comparable_bytes": None,
                "mismatched_comparable_bytes": None,
                "matched_comparable_percent": None,
                "field_count": None,
                "distance": None,
                "reason": str(function.get("reason", "unresolved")),
            },
            None,
        )
    candidate_size = int(function[candidate_field])
    comparable = int(function["comparable_bytes"])
    matched = int(function["matched_comparable_bytes"])
    if expected_size <= 0 or comparable < 0 or not 0 <= matched <= comparable:
        raise ValueError(f"invalid {profile} comparison metrics")
    mismatch = comparable - matched
    size_delta = abs(candidate_size - expected_size)
    distance = Fraction(size_delta, expected_size)
    if comparable:
        distance += Fraction(mismatch, comparable)
    fields = function.get("relocations" if profile == "coff" else "linkage_fields")
    if not isinstance(fields, list):
        raise ValueError(f"{profile} comparison lacks a field list")
    return (
        {
            "result": result,
            "candidate_size": candidate_size,
            "target_size": expected_size,
            "size_delta": size_delta,
            "comparable_bytes": comparable,
            "mismatched_comparable_bytes": mismatch,
            "matched_comparable_percent": (
                round(100.0 * matched / comparable, 2) if comparable else 100.0
            ),
            "field_count": len(fields),
            "distance": round(float(distance), 6),
        },
        distance,
    )


def rank_candidates(
    backlog: list[dict[str, object]],
    coff: dict[tuple[str, str, str, int], dict[str, object]],
    ltcg: dict[tuple[str, str, str, int], dict[str, object]],
) -> list[dict[str, object]]:
    expected = {backlog_key(item) for item in backlog}
    if len(expected) != len(backlog):
        raise ValueError("current backlog contains duplicate function keys")
    if set(coff) != expected:
        raise ValueError("normal-COFF probe keys differ from the current authored backlog")
    if set(ltcg) != expected:
        raise ValueError("LTCG probe keys differ from the current authored backlog")

    ranked = []
    sort_keys = []
    for item in backlog:
        key = backlog_key(item)
        profiles = {}
        distances: dict[str, Fraction | None] = {}
        for profile, source in (("coff", coff), ("ltcg", ltcg)):
            profiles[profile], distances[profile] = profile_metrics(profile, source[key])

        def profile_key(profile: str) -> tuple[object, ...]:
            metrics = profiles[profile]
            distance = distances[profile]
            return (
                RESULT_ORDER[str(metrics["result"])],
                distance if distance is not None else Fraction(1 << 30),
                (
                    int(metrics["field_count"])
                    if metrics["field_count"] is not None
                    else 1 << 30
                ),
                0 if profile == "coff" else 1,
            )

        best = min(("coff", "ltcg"), key=profile_key)
        best_metrics = profiles[best]
        best_distance = distances[best]
        gain = None
        if distances["coff"] is not None and distances["ltcg"] is not None:
            gain = round(float(distances["coff"] - distances["ltcg"]), 6)
        row = {
            "address": key[0],
            "name": key[1],
            "source": key[2],
            "target_size": key[3],
            "recommended_profile": best,
            "best_result": best_metrics["result"],
            "best_distance": best_metrics["distance"],
            "ltcg_gain": gain,
            "profiles": profiles,
        }
        ranked.append(row)
        sort_keys.append(
            (
                RESULT_ORDER[str(best_metrics["result"])],
                best_distance if best_distance is not None else Fraction(1 << 30),
                (
                    int(best_metrics["field_count"])
                    if best_metrics["field_count"] is not None
                    else 1 << 30
                ),
                key[3],
                int(key[0], 0),
            )
        )
    order = sorted(range(len(ranked)), key=lambda index: sort_keys[index])
    result = []
    for rank, index in enumerate(order, 1):
        result.append({"rank": rank, **ranked[index]})
    return result


def render_text(report: dict[str, object]) -> None:
    print(
        "rank address    bytes best distance size-delta mismatch fields LTCG-gain source :: name"
    )
    for row in report["candidates"]:
        profile = str(row["recommended_profile"])
        metrics = row["profiles"][profile]
        if metrics["result"] == "unresolved":
            detail = "unresolved"
            delta = mismatch = fields = "-"
        else:
            detail = f"{float(row['best_distance']):.3f}"
            delta = str(metrics["size_delta"])
            mismatch = (
                f"{metrics['mismatched_comparable_bytes']}/"
                f"{metrics['comparable_bytes']}"
            )
            fields = str(metrics["field_count"])
        gain = "-" if row["ltcg_gain"] is None else f"{float(row['ltcg_gain']):+.3f}"
        print(
            f"{int(row['rank']):4d} {row['address']} {int(row['target_size']):6d} "
            f"{profile:4s} {detail:>8s} {delta:>10s} {mismatch:>10s} "
            f"{fields:>6s} {gain:>9s} {row['source']} :: {row['name']}"
        )
    print(
        f"ranked {report['emitted_count']} of {report['function_count']} current "
        "authored backlog function(s)"
    )
    print(
        "distance = abs(candidate-target)/target + comparable mismatches/comparable; "
        "positive LTCG-gain means LTCG is closer"
    )
    print("ranking is diagnostic and grants no exactness or physical-owner credit")


def main() -> int:
    args = parse_args()
    try:
        target_sha256 = verified_target()
        backlog_report = run_json(
            selected_command("scripts/report-exact-backlog.py", args.source),
            "current exact backlog",
        )
        items = backlog_report.get("items")
        if not isinstance(items, list) or not all(isinstance(item, dict) for item in items):
            raise ValueError("current exact backlog has invalid items")
        sources = {str(item["source"]) for item in items}
        identity_before = input_identity(sources)
        coff_report = run_json(
            selected_command("scripts/probe-exact-backlog.py", args.source),
            "normal-COFF backlog probe",
        )
        ltcg_report = run_json(
            selected_command("scripts/probe-ltcg-backlog.py", args.source),
            "LTCG backlog probe",
        )
        identity_after = input_identity(sources)
        if identity_after != identity_before:
            raise ValueError("ranking inputs changed while the two probes were running")
        ranked = rank_candidates(
            items,
            flatten(coff_report, "normal-COFF backlog probe"),
            flatten(ltcg_report, "LTCG backlog probe"),
        )
        emitted = ranked if args.limit == 0 else ranked[: args.limit]
        report: dict[str, object] = {
            "schema_version": 1,
            "result": "ok",
            "acceptance_authority": "none",
            "target_sha256": target_sha256,
            "input_sha256": identity_before,
            "selection": {"sources": sorted(args.source)},
            "ranking_method": (
                "prefer structural-exact, then minimize "
                "abs(candidate_size-target_size)/target_size + "
                "mismatched_comparable_bytes/comparable_bytes; break ties by "
                "field count, target size, and address"
            ),
            "function_count": len(ranked),
            "emitted_count": len(emitted),
            "probe_counts": {
                "coff": coff_report.get("counts", {}),
                "ltcg": ltcg_report.get("counts", {}),
            },
            "candidates": emitted,
        }
    except (OSError, KeyError, TypeError, ValueError) as exc:
        if args.json:
            print(json.dumps({"result": "error", "error": str(exc)}, indent=2))
        else:
            print(f"error: exact backlog ranking failed: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        render_text(report)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
