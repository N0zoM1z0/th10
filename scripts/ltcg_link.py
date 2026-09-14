"""Cold VC7.1 LTCG diagnostic-link harness shared by probe and replay tools."""

from __future__ import annotations

import hashlib
import os
from pathlib import Path
import re
import subprocess
import tomllib


ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / "build" / "probe-ltcg"
TOOLS_LOCK = ROOT / "config" / "tools.lock.toml"
NORMAL_PROFILE = ["/TP", "/MT", "/O2", "/Gy", "/GF", "/Oi", "/DNDEBUG", "/Isrc"]
LTCG_PROFILE = [*NORMAL_PROFILE, "/GL"]
LINK_PROFILE = [
    "/ltcg",
    "/nodefaultlib",
    "/subsystem:console",
    "/incremental:no",
    "/fixed:no",
    "/base:0x00400000",
    "/debug",
    "/debugtype:cv",
    "/opt:noref",
    "/opt:noicf",
    "/safeseh:no",
]
ANCHOR_PREFIX = "_th10_ltcg_probe_anchor_"
HARNESS_KIND = "unresolved-symbol-data-anchors-v1"
UNRESOLVED_MARKER = "unresolved external symbol "


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


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def tool_environment() -> tuple[Path, dict[str, str]]:
    with TOOLS_LOCK.open("rb") as stream:
        lock = tomllib.load(stream)["msvc71"]
    msvc_root = Path(
        os.environ.get("TH10_MSVC71_ROOT", ROOT / str(lock["selection"]))
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
        try:
            source_label = source.relative_to(ROOT)
        except ValueError:
            source_label = source
        raise ValueError(f"compile failed for {source_label}: {completed.stdout.strip()}")


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
        *LINK_PROFILE,
        f"/entry:{entry}",
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


def write_anchors(
    directory: Path, symbols: list[str]
) -> tuple[Path, Path, dict[str, str]]:
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
        "".join(
            f"/alternatename:{symbol}={anchor}\n"
            for symbol, anchor in bindings.items()
        ),
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
    profile: list[str] | None = None,
) -> dict[str, object]:
    if profile is None:
        profile = LTCG_PROFILE
    if not any(flag.lower() == "/gl" for flag in profile):
        raise ValueError("LTCG link source profile must contain /GL")
    directory = directory.resolve()
    try:
        directory.relative_to((ROOT / "build").resolve())
    except ValueError as exc:
        raise ValueError("LTCG link directory must stay below build/") from exc
    directory.mkdir(parents=True, exist_ok=True)
    ltcg_object = directory / "source.ltcg.obj"
    image = directory / "source.exe"
    map_path = directory / "source.map"
    pdb = directory / "source.pdb"
    anchor_source = directory / "link-anchors.c"
    anchor_object_path = directory / "link-anchors.obj"
    alias_path = directory / "link-aliases.rsp"
    old_logs = sorted(directory.glob("link-attempt-*.log"))
    removed = remove_files(
        [
            image,
            map_path,
            pdb,
            image.with_suffix(".ilk"),
            anchor_source,
            anchor_object_path,
            anchor_object_path.with_suffix(".pdb"),
            anchor_object_path.with_suffix(".idb"),
            alias_path,
            *old_logs,
        ]
    )
    compile_source(source, ltcg_object, list(profile))
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
