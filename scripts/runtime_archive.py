"""Pinned VC7.1 static-runtime archive helpers for target-bound replays."""

from __future__ import annotations

import hashlib
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ARCHIVE_HASHES = {
    "libcmt.lib": "6bed2e73e7ecf52818133011058e240cd38b122f51302c0c0194d5ff3167357e",
    "libcpmt.lib": "d93f56389e8b78661a00fe63aa4575f121a76c14c83ed9f87ca97acb8874ad16",
}


def archive_members(path: Path):
    """Yield ``(member_name, payload)`` pairs from a COFF archive."""

    data = path.read_bytes()
    if data[:8] != b"!<arch>\n":
        raise ValueError(f"not a COFF archive: {path}")
    position = 8
    longnames = b""
    while position < len(data):
        header = data[position : position + 60]
        if len(header) != 60 or header[58:60] != b"`\n":
            raise ValueError(f"invalid archive member at {position}")
        size = int(header[48:58].strip())
        payload_start = position + 60
        payload = data[payload_start : payload_start + size]
        if len(payload) != size:
            raise ValueError(f"truncated archive member at {position}")
        raw_name = header[:16].decode("ascii").strip()
        if raw_name == "//":
            longnames = payload
        elif raw_name.startswith("/") and raw_name[1:].isdigit():
            start = int(raw_name[1:])
            end = longnames.find(b"\x00", start)
            name = longnames[start:end].decode("ascii") if end >= 0 else raw_name
            yield name, payload
        elif raw_name != "/":
            yield raw_name.rstrip("/"), payload
        position += 60 + size + (size & 1)


def verified_archive(raw_path: str | Path) -> Path:
    """Resolve and hash-attest one of the pinned VC7.1 runtime archives."""

    path = (ROOT / str(raw_path)).resolve()
    try:
        path.relative_to((ROOT / ".tools" / "msvc710-sp1").resolve())
    except ValueError as exc:
        raise ValueError(f"runtime archive leaves the pinned toolchain: {path}") from exc
    expected = ARCHIVE_HASHES.get(path.name.lower())
    if expected is None:
        raise ValueError(f"unsupported runtime archive: {path.name}")
    if not path.is_file():
        raise ValueError(f"runtime archive does not exist: {path}")
    actual = hashlib.sha256(path.read_bytes()).hexdigest()
    if actual != expected:
        raise ValueError(f"wrong VC7.1 runtime archive hash for {path}: {actual}")
    return path


def extract_member(raw_archive: str | Path, member: str, output: Path) -> None:
    """Extract one hash-attested archive member into a build-local object."""

    archive = verified_archive(raw_archive)
    payload = None
    for name, candidate in archive_members(archive):
        if name == member:
            if payload is not None:
                raise ValueError(f"runtime archive member is not unique: {member}")
            payload = candidate
    if payload is None:
        raise ValueError(f"runtime archive member not found: {member}")
    if payload[:2] != b"L\x01":
        raise ValueError(f"runtime archive member is not normal i386 COFF: {member}")
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(payload)

