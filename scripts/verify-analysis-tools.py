#!/usr/bin/env python3
"""Verify the selected shared Ghidra and JDK surfaces."""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
LOCK = ROOT / "config" / "tools.lock.toml"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def require_hash(path: Path, expected: str) -> None:
    if not path.is_file():
        raise FileNotFoundError(path)
    observed = sha256(path)
    if observed != expected:
        raise ValueError(f"tool identity mismatch: {path}")


def verify(full: bool) -> dict[str, str]:
    with LOCK.open("rb") as stream:
        lock = tomllib.load(stream)
    ghidra = (ROOT / ".tools" / "ghidra").resolve(strict=True)
    jdk = (ROOT / ".tools" / "jdk").resolve(strict=True)
    require_hash(
        ghidra / "Ghidra" / "application.properties",
        lock["ghidra"]["application_properties_sha256"],
    )
    require_hash(
        ghidra / "support" / "analyzeHeadless",
        lock["ghidra"]["analyze_headless_sha256"],
    )
    require_hash(jdk / "bin" / "java", lock["temurin_jdk"]["java_sha256"])
    require_hash(jdk / "release", lock["temurin_jdk"]["release_sha256"])
    if full:
        require_hash(jdk / "lib" / "modules", lock["temurin_jdk"]["modules_sha256"])
    completed = subprocess.run(
        [str(jdk / "bin" / "java"), "-version"],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        errors="replace",
        check=True,
    )
    if lock["temurin_jdk"]["banner_substring"] not in completed.stdout:
        raise ValueError("JDK version banner does not match the lock")
    return {
        "ghidra_home": str(ghidra),
        "jdk_home": str(jdk),
        "ghidra_version": lock["ghidra"]["version"],
        "jdk_version": lock["temurin_jdk"]["version"],
        "mode": "full" if full else "quick",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--quick", action="store_true")
    args = parser.parse_args()
    try:
        result = verify(full=not args.quick)
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as error:
        print(f"error: analysis tool verification failed: {error}", file=sys.stderr)
        return 1
    print(
        "analysis tools OK: "
        f"Ghidra {result['ghidra_version']}, JDK {result['jdk_version']} "
        f"({result['mode']})"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
