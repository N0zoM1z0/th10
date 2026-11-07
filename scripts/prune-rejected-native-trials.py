#!/usr/bin/env python3
"""Audited one-time pruning of *ignored rejected* native trial binaries.

Dry run by default. `--apply` deletes only .exe/.obj/.pdb under the explicit
per-epoch allowlist below, never original target, toolchain, archives, source,
JSON, MAP, canonical match-linked graphs or experiment drivers. No archive
is made: each removed binary is regenerable from pinned trial sources.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess

REPO = Path(__file__).resolve().parent.parent
BUILD = REPO / "build/gpt-web"
PRIVATE = REPO / ".analysis/maintenance-next-agent-20261107"
NATIVE_EXTENSIONS = frozenset({".exe", ".obj", ".pdb"})

# Reviewed retired source-variant families. This is an allowlist, not a
# blanket build/ cleanup. No current accepted cold graph is in this list.
REJECTED_FAMILIES = (
    "20261107-shake-envelope",
    "20261107-shake-output-register",
    "20261107-shake-switch",
    "20261107-shake-envelope-scope",
    "20261107-shake-linear",
    "20261107-anm91-retries",
    "20261107-anm-child91",
    "20261107-anm-op91-fresh",
    "20261107-bullet-angle-caller",
    "20261107-bullet-final-register",
    "20261107-ecl-pop-sib-native",
    "20261107-ecl-length-struct",
    "20261107-ecl-point-angle",
    "20261107-ecl-length-order",
    "20261107-format-stackhomes-new",
    "20261107-ecl-run-return-spawn",
    "20261107-sound-new-reads",
    "20261107-sound-register-choices",
    "20261107-enemy-setsub-caller",
    "20261107-ecl-format-scalar-selector",
    "20261107-sound-preload",
)


def sha256(path: Path) -> str:
    sha = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            sha.update(block)
    return sha.hexdigest()


def disk_usage(path: Path) -> int:
    return int(subprocess.check_output(["du", "-s", "-B1", str(path)], text=True).split()[0])


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="perform the audited removal")
    args = parser.parse_args()
    if REPO.name != "th10" or not BUILD.is_dir() or BUILD.is_symlink():
        raise SystemExit("this script only operates on the registered TH10 build/gpt-web tree")
    if subprocess.check_output(["git", "ls-files", "--", "build/gpt-web"], text=True, cwd=REPO).strip():
        raise SystemExit("build/gpt-web contains tracked files; refuse deletion")
    if args.apply and subprocess.check_output(["git", "status", "--porcelain"], text=True, cwd=REPO).strip():
        # The script itself can be untracked before its first commit.
        result = subprocess.check_output(["git", "status", "--short"], text=True, cwd=REPO)
        if result.strip() != "?? scripts/prune-rejected-native-trials.py":
            raise SystemExit("save other tracked/dirty work before applying cleanup")
    files = []
    summary = {}
    for family in REJECTED_FAMILIES:
        root = BUILD / family
        if not root.is_dir() or root.is_symlink() or root.resolve().parent != BUILD.resolve():
            raise SystemExit(f"missing/symlinked/non-whitelisted experiment root: {root}")
        group_files = []
        for file in sorted(root.rglob("*")):
            if not file.is_file() or file.suffix.lower() not in NATIVE_EXTENSIONS:
                continue
            if file.is_symlink() or not file.resolve().is_relative_to(root.resolve()):
                raise SystemExit(f"unsafe native trial path: {file}")
            # Accepted-source current installed witnesses belong in their
            # selected graph / match-linked; never remove explicitly retained
            # names even if a rejected family acquired such a directory.
            if {"installed-cold", "accepted-exact", "canonical-cold"} & set(file.relative_to(root).parts):
                continue
            group_files.append(file)
        files.extend(group_files)
        summary[family] = {"files": len(group_files), "payload_bytes": sum(p.stat().st_size for p in group_files)}
    if not files:
        receipt = PRIVATE / "cleanup-result.json"
        if receipt.exists() and not args.apply:
            print("Already pruned: no remaining eligible native outputs in the 21 reviewed families")
            return
        raise SystemExit("nothing to remove / --apply must never be repeated")
    if len(files) != len(set(files)):
        raise SystemExit("duplicate paths in proposed native deletion")
    total = sum(f.stat().st_size for f in files)
    print(json.dumps({"mode": "apply" if args.apply else "dry-run", "roots": len(summary),
                      "native_files": len(files), "payload_bytes": total,
                      "payload_MiB": round(total / (1024 * 1024), 2),
                      "largest_roots": sorted(summary.items(), key=lambda r: -r[1]["payload_bytes"])[:12]}, indent=2))
    if not args.apply:
        return

    PRIVATE.mkdir(parents=True, exist_ok=True)
    manifest = PRIVATE / "removed-native.jsonl"
    receipt = PRIVATE / "cleanup-result.json"
    if manifest.exists() or receipt.exists():
        raise SystemExit("one-time cleanup already recorded; do not re-run --apply")
    protected = [
        REPO / "resources/th10.exe",
        REPO / "build/match-linked/BulletHorizontalWrap/source.exe",
        REPO / "build/match-linked/EclVmHostRunEnemy/source.exe",
        REPO / "build/match-linked/SoundDataLoaderThread/source.exe",
        REPO / "build/match-linked/GameStatisticsTail/source.exe",
    ]
    frozen = {str(p.relative_to(REPO)): sha256(p) for p in protected}
    preserved_dirs = [REPO / p for p in ("build/match-linked", ".analysis/archive", ".tools", "_reference", "ghidra-project")]
    retained_sizes = {str(d.relative_to(REPO)): disk_usage(d) for d in preserved_dirs}
    before = disk_usage(BUILD)
    deleted_bytes_allocated = 0
    with manifest.open("w", encoding="utf-8") as output:
        for f in files:
            stat = f.stat()
            record = {"path": str(f.relative_to(REPO)), "bytes": stat.st_size,
                      "allocated_bytes": stat.st_blocks * 512, "sha256": sha256(f)}
            deleted_bytes_allocated += record["allocated_bytes"]
            output.write(json.dumps(record, sort_keys=True) + "\n")
        output.flush()
    # The SHA manifest is *not* an archive and cannot restore old bytes.
    for f in files:
        f.unlink()
    after = disk_usage(BUILD)
    for key, expected in frozen.items():
        if sha256(REPO / key) != expected:
            raise SystemExit(f"protected exact/target artifact unexpectedly changed: {key}")
    for key, expected in retained_sizes.items():
        if disk_usage(REPO / key) != expected:
            raise SystemExit(f"protected directory unexpectedly changed: {key}")
    if before <= after:
        raise SystemExit("no disk saving measured")
    data = {"date": "2026-11-07", "scope": "ignored regenerable rejected experiments only",
            "families": summary, "deleted_files": len(files), "payload_bytes": total,
            "allocated_file_bytes": deleted_bytes_allocated,
            "build_gpt_web_before_bytes": before, "build_gpt_web_after_bytes": after,
            "build_gpt_web_reclaimed_bytes": before - after,
            "removed_manifest_path": str(manifest.relative_to(REPO)),
            "removed_manifest_sha256": sha256(manifest),
            "protected_artifact_sha256": frozen, "protected_directory_sizes": retained_sizes,
            "archive_created": False,
            "preserved": ["all source/config/docs", "original EXE", "all build/match-linked",
                          "all .analysis/archive", "all trial driver/JSON/MAP/log/source snapshots",
                          "compiler, Ghidra and reference trees"],
            "warning": "The manifest proves deletion, not a restorable binary. Rebuild variants from drivers."}
    receipt.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")
    print(f"DELETED {len(files)} regenerated-native artifacts; reclaimed {before-after:,} allocated bytes")
    print(f"RECORDED {receipt.relative_to(REPO)} with manifest SHA {data['removed_manifest_sha256']}")


if __name__ == "__main__":
    main()
