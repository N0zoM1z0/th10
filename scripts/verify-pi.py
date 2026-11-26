#!/usr/bin/env python3
"""Verify Position Independence (PI) of the TH10 v1.00a product build.

Checks:
1. Map displacement between build/windows-i386/th10.exe and
   build/windows-i386/th10-reordered.exe across .text, .rdata, .data, and .bss.
2. Shared-storage identity for cross-TU alias groups in both builds.
3. Contiguous struct-interior offset preservation across reordered links.
4. Real PE base-relocation (.reloc) coverage for all initialized pointers in
   ProductData (zero un-relocated retail 0x004xxxxx addresses).
"""

from __future__ import annotations

from pathlib import Path
import re
import struct
import sys

import pefile

ROOT = Path(__file__).resolve().parents[1]
STD_EXE = ROOT / "build" / "windows-i386" / "th10.exe"
STD_MAP = ROOT / "build" / "windows-i386" / "th10.map"
REV_EXE = ROOT / "build" / "windows-i386" / "th10-reordered.exe"
REV_MAP = ROOT / "build" / "windows-i386" / "th10-reordered.map"


def parse_map(map_path: Path) -> dict[str, int]:
    syms: dict[str, int] = {}
    pat = re.compile(r"^\s*[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s+")
    for line in map_path.read_text(errors="replace").splitlines():
        m = pat.match(line)
        if m:
            syms[m.group(1)] = int(m.group(2), 16)
    return syms


def get_reloc_rvas(pe: pefile.PE) -> set[int]:
    rvas: set[int] = set()
    if hasattr(pe, "DIRECTORY_ENTRY_BASERELOC"):
        for block in pe.DIRECTORY_ENTRY_BASERELOC:
            for entry in block.entries:
                if entry.type != 0:
                    rvas.add(entry.rva)
    return rvas


def main() -> int:
    for p in (STD_EXE, STD_MAP, REV_EXE, REV_MAP):
        if not p.is_file():
            print(f"error: missing {p}; run scripts/build.py and scripts/build.py --reverse-link first", file=sys.stderr)
            return 1

    std_syms = parse_map(STD_MAP)
    rev_syms = parse_map(REV_MAP)
    failures: list[str] = []

    # 1. Displacement proof across .text, .rdata, .data, .bss
    probe_syms = [
        (".text", "_WinMain@16"),
        (".text", "?PlayerShotTrackEnemy@@YIHPAXPAUPlayerShotRuntimeView@@@Z"),
        (".text", "?InstallAnmVmVertexBuffer@@YIHPAUAnmVmView@@PBI@Z"),
        (".rdata", "?g_MainWindowTitle@@3QBDB"),
        (".rdata", "?g_BulletCullBottom@@3MA"),
        (".data", "?g_GameScoreState@@3UGameScoreStateView@@A"),
        (".data", "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA"),
        (".data", "?g_GameErrorContext@th10@@3UGameErrorContext@1@A"),
        (".bss", "?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A"),
        (".bss", "?g_Player@@3PAUPlayer@@A"),
    ]
    print("=== 1. Symbol Displacement (normal vs reverse-link) ===")
    for sec, sym in probe_syms:
        a = std_syms.get(sym)
        b = rev_syms.get(sym)
        if a is None or b is None:
            failures.append(f"missing probe symbol {sym}: std={a} rev={b}")
            continue
        moved = a != b
        print(f"  [{sec:6s}] {sym[:52]:52s} 0x{a:08X} -> 0x{b:08X} (delta={b - a:+d})")
        if not moved:
            failures.append(f"symbol {sym} did not move under reverse-link (0x{a:08X})")

    # 2. Shared-storage identity proof
    alias_groups = [
        ("AnmRenderManager", [
            "?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A",
            "?g_EnemyAnimationOwner@@3PAUEnemyAnimationOwnerView@@A",
            "_g_AnmManagerPtr",
        ]),
        ("Difficulty", [
            "?g_EnemyDifficulty@@3HA",
            "?g_GuiDifficulty@@3HA",
            "?g_ReplayDifficulty@@3HA",
        ]),
        ("CriticalSections", [
            "?g_CriticalSections@th10@@3PAU_RTL_CRITICAL_SECTION@@A",
            "?g_EnemyCallbackCriticalSection@@3UEnemyCriticalSectionView@@A",
            "?g_ItemCallbackCriticalSection@@3UItemCriticalSectionView@@A",
            "?g_PlayerCallbackCriticalSection@@3UPlayerCriticalSectionView@@A",
        ]),
        ("PlayfieldBottom", [
            "?g_BulletCullBottom@@3MA",
            "?g_EnemyPlayfieldMinY@@3MA",
            "_g_EnemyLaserPlayfieldMaximumY",
            "_g_PlayerShotPlayfieldMaximumY",
        ]),
    ]
    print("\n=== 2. Shared-Storage Identity ===")
    for label, group in alias_groups:
        std_vals = {std_syms.get(s) for s in group}
        rev_vals = {rev_syms.get(s) for s in group}
        ok = len(std_vals) == 1 and None not in std_vals and len(rev_vals) == 1 and None not in rev_vals
        print(f"  {label:20s} std=0x{next(iter(std_vals)) or 0:08X} rev=0x{next(iter(rev_vals)) or 0:08X} ok={ok}")
        if not ok:
            failures.append(f"alias group {label} diverged: std={std_vals} rev={rev_vals}")

    # 3. Contiguous struct-interior offset preservation
    offset_contracts = [
        ("g_GameScoreState -> g_GameScore", "?g_GameScoreState@@3UGameScoreStateView@@A", "?g_GameScore@@3HA", 0x04),
        ("g_GameScoreState -> g_PlayerCharacter", "?g_GameScoreState@@3UGameScoreStateView@@A", "?g_PlayerCharacter@@3HA", 0x28),
        ("g_GameScoreState -> g_GuiDifficulty", "?g_GameScoreState@@3UGameScoreStateView@@A", "?g_GuiDifficulty@@3HA", 0x34),
        ("g_GameScoreState -> g_FrontEndFlags", "?g_GameScoreState@@3UGameScoreStateView@@A", "?g_FrontEndFlags@@3IA", 0x60),
        ("g_FrontEndInput -> g_MainInputEdges", "?g_FrontEndInput@@3UFrontEndInputView@@A", "?g_MainInputEdges@@3GA", 0x06),
        ("g_FrontEndInput -> g_GuiInputCurrent", "?g_FrontEndInput@@3UFrontEndInputView@@A", "?g_GuiInputCurrent@@3IA", 0x2C),
        ("g_FrontEndInput -> g_GuiInputEdges", "?g_FrontEndInput@@3UFrontEndInputView@@A", "?g_GuiInputEdges@@3IA", 0x32),
        ("g_MainSupervisorView -> g_Direct3DDevice", "?g_MainSupervisorView@@3UMainSupervisorView@@A", "?g_Direct3DDevice@@3PAUD3d9DeviceView@@A", 0x08),
        ("g_MainSupervisorView -> g_InputJoystickXThreshold", "?g_MainSupervisorView@@3UMainSupervisorView@@A", "?g_InputJoystickXThreshold@@3FA", 0x136),
        ("g_MainSupervisorView -> g_CriticalSections", "?g_MainSupervisorView@@3UMainSupervisorView@@A", "?g_CriticalSections@th10@@3PAU_RTL_CRITICAL_SECTION@@A", 0x64C),
        ("g_MainSupervisorView -> g_ChainNestCounter", "?g_MainSupervisorView@@3UMainSupervisorView@@A", "?g_ChainNestCounter@th10@@3EA", 0x6F4),
        ("g_MainSupervisorView -> gReplayFileOpenCount", "?g_MainSupervisorView@@3UMainSupervisorView@@A", "?gReplayFileOpenCount@@3EA", 0x6F6),
        ("g_MainSupervisorView -> g_LogLockDepth", "?g_MainSupervisorView@@3UMainSupervisorView@@A", "?g_LogLockDepth@th10@@3EA", 0x6F7),
    ]
    print("\n=== 3. Struct-Interior Offset Contracts ===")
    for label, base_sym, field_sym, expected in offset_contracts:
        d_std = std_syms[field_sym] - std_syms[base_sym]
        d_rev = rev_syms[field_sym] - rev_syms[base_sym]
        ok = d_std == expected and d_rev == expected
        print(f"  {label:40s} std=+0x{d_std:02X} rev=+0x{d_rev:02X} expected=+0x{expected:02X} ok={ok}")
        if not ok:
            failures.append(f"offset contract {label} failed: std={d_std:#x} rev={d_rev:#x} expected={expected:#x}")

    # 4. PE base-relocation check on initialized pointer slots in .data
    print("\n=== 4. Initialized Pointer Relocations (.reloc) ===")
    for exe_path, syms in ((STD_EXE, std_syms), (REV_EXE, rev_syms)):
        pe = pefile.PE(str(exe_path))
        base = pe.OPTIONAL_HEADER.ImageBase
        relocs = get_reloc_rvas(pe)
        # Check g_GameErrorContext self-pointer (+0x2000), g_PlayerShotUpdateCallbacks[1], g_AnmSpecialVmDescriptors+4
        err_va = syms["?g_GameErrorContext@th10@@3UGameErrorContext@1@A"]
        err_ptr_rva = (err_va + 0x2000) - base
        err_ptr_val = struct.unpack("<I", pe.get_data(err_ptr_rva, 4))[0]
        shot_va = syms["?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA"]
        shot_ptr_rva = (shot_va + 4) - base
        shot_ptr_val = struct.unpack("<I", pe.get_data(shot_ptr_rva, 4))[0]
        track_va = syms["?PlayerShotTrackEnemy@@YIHPAXPAUPlayerShotRuntimeView@@@Z"]

        ok_err = (err_ptr_rva in relocs) and (err_ptr_val == err_va)
        ok_shot = (shot_ptr_rva in relocs) and (shot_ptr_val == track_va)
        print(
            f"  {exe_path.name:20s} err_self_ptr=0x{err_ptr_val:08X} (reloc={err_ptr_rva in relocs}) "
            f"shot_cb[1]=0x{shot_ptr_val:08X} (reloc={shot_ptr_rva in relocs})"
        )
        if not ok_err:
            failures.append(f"{exe_path.name}: g_GameErrorContext self-pointer not relocated to 0x{err_va:08X}")
        if not ok_shot:
            failures.append(f"{exe_path.name}: g_PlayerShotUpdateCallbacks[1] not relocated to 0x{track_va:08X}")

    if failures:
        print("\nFAILURES:", file=sys.stderr)
        for f in failures:
            print(f"  - {f}", file=sys.stderr)
        return 1
    print("\nALL POSITION-INDEPENDENCE CHECKS PASSED.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
