#!/usr/bin/env python3
"""Reject the full nonexact TH10 BGM preload owner with all fields bound."""

from __future__ import annotations

import argparse
import copy
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "scripts"))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--artifact-dir", required=True, type=Path)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to((ROOT / "build").resolve())

    with (ROOT / "config/match-units.toml").open("rb") as stream:
        manifest = tomllib.load(stream)
    unit = copy.deepcopy(manifest["units"]["sound-process-queues"])
    unit.update(
        output_dir=str(directory.relative_to(ROOT)),
        functions=["SoundPlayerView::PreloadBgm"],
        symbol="?PreloadBgm@SoundPlayerView@@QAEHHPBD@Z",
        target_address=0x0043D7D0,
        size=369,
        linkages=[
            {"offset": 0x05B, "type": "DIR32", "symbol": "?g_MainSoundOwner@@3PAEA", "target": 0x492590, "addend": 0x4108},
            {"offset": 0x06F, "type": "DIR32", "symbol": "?g_MainSupervisorView@@3UMainSupervisorView@@A", "target": 0x491C28, "addend": 0x150},
            {"offset": 0x095, "type": "REL32", "symbol": "_free", "target": 0x452422},
            {"offset": 0x0C2, "type": "DIR32", "symbol": "__imp__CreateFileA@28", "target": 0x4660C4},
            {"offset": 0x0D1, "type": "REL32", "symbol": "?GetFmtIndexByName@SoundPlayerView@@QAEHPBD@Z", "target": 0x43D2A0},
            {"offset": 0x0EC, "type": "DIR32", "symbol": "__imp__SetFilePointer@16", "target": 0x4660C8},
            {"offset": 0x0FC, "type": "REL32", "symbol": "_malloc", "target": 0x452706},
            {"offset": 0x10C, "type": "DIR32", "symbol": "__imp__CloseHandle@4", "target": 0x4660CC},
            {"offset": 0x130, "type": "DIR32", "symbol": "__imp__ReadFile@20", "target": 0x4660D0},
            {"offset": 0x137, "type": "DIR32", "symbol": "__imp__CloseHandle@4", "target": 0x4660CC},
        ],
    )
    manifest["units"]["diagnostic-preload"] = unit
    compare = runpy.run_path(str(ROOT / "scripts/compare-linked-function.py"))["compare_unit"]
    result = compare("diagnostic-preload", _unit_manifest=manifest)
    # Rejection is mandatory. The full owner, all ten address fields and
    # fixed control transfers must be checked, not a cropped matching span.
    assert result["result"] == "mismatch", result
    assert result["matched_bytes"] == 365, result
    assert result["size"] == 369, result
    assert [x["offset"] for x in result["first_differences"]] == [
        "0x121", "0x127", "0x12A", "0x12B"
    ], result
    print(json.dumps({"result": "complete-owner-nonexact", "target_size": 369,
                      "native_size": 369, "field_count": 10,
                      "matched_bytes": result["matched_bytes"],
                      "ordinary_differences": result["first_differences"],
                      "exact_credit": 0}, indent=2))


if __name__ == "__main__":
    main()
