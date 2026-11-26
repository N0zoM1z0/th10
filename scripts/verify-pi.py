#!/usr/bin/env python3
"""Verify Position Independence (PI) of the TH10 v1.00a product build.

Checks:
1. Map displacement AND actual PE section index between
   build/windows-i386/th10.exe and build/windows-i386/th10-reordered.exe
   across .text (0001), .rdata (0002), and .data/.bss (0003).
2. Shared-storage identity for ALL /alternatename alias bindings declared in
   src/ProductGlobals.cpp (plus core multi-symbol groups) in both builds.
3. Contiguous struct-interior offset preservation across reordered links
   (g_GameScoreState, g_FrontEndInput, g_MainSupervisorView).
4. Real PE base-relocation (.reloc) coverage and target address match for all
   initialized code/self pointers in src/ProductGlobals.cpp.
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
GLOBALS_CPP = ROOT / "src" / "ProductGlobals.cpp"

SECTION_NAMES = {1: ".text", 2: ".rdata", 3: ".data/.bss"}


def parse_map(map_path: Path) -> dict[str, tuple[int, int]]:
    """Return {symbol: (section_index, va)} from a VC7.1 .map file."""
    syms: dict[str, tuple[int, int]] = {}
    pat = re.compile(r"^\s*([0-9a-fA-F]{4}):[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s+")
    for line in map_path.read_text(errors="replace").splitlines():
        m = pat.match(line)
        if m:
            syms[m.group(2)] = (int(m.group(1), 16), int(m.group(3), 16))
    return syms


def get_reloc_rvas(pe: pefile.PE) -> set[int]:
    rvas: set[int] = set()
    if hasattr(pe, "DIRECTORY_ENTRY_BASERELOC"):
        for block in pe.DIRECTORY_ENTRY_BASERELOC:
            for entry in block.entries:
                if entry.type != 0:
                    rvas.add(entry.rva)
    return rvas


def parse_alternatename_pragmas(cpp_path: Path) -> list[tuple[str, str]]:
    pat = re.compile(r'#pragma\s+comment\(linker,\s*"/alternatename:([^=]+)=([^"]+)"\)')
    return pat.findall(cpp_path.read_text(errors="replace"))


def main() -> int:
    for p in (STD_EXE, STD_MAP, REV_EXE, REV_MAP):
        if not p.is_file():
            print(f"error: missing {p}; run scripts/build.py and scripts/build.py --reverse-link first", file=sys.stderr)
            return 1

    std_syms = parse_map(STD_MAP)
    rev_syms = parse_map(REV_MAP)
    failures: list[str] = []

    # 1. Displacement + actual PE section verification
    probe_syms = [
        (1, "_WinMain@16"),
        (1, "?PlayerShotTrackEnemy@@YIHPAXPAUPlayerShotRuntimeView@@@Z"),
        (1, "?InstallAnmVmVertexBuffer@@YIHPAUAnmVmView@@PBI@Z"),
        (2, "?g_MainWindowTitle@@3QBDB"),
        (2, "?g_AnmSpecialVmDescriptors@@3QBUAnmSpecialVmDescriptorView@@B"),
        (3, "?g_BulletCullBottom@@3MA"),
        (3, "?g_GameScoreState@@3UGameScoreStateView@@A"),
        (3, "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA"),
        (3, "?g_GameErrorContext@th10@@3UGameErrorContext@1@A"),
        (3, "?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A"),
        (3, "?g_Player@@3PAUPlayer@@A"),
    ]
    print("=== 1. Symbol Displacement & Section Check (normal vs reverse-link) ===")
    for exp_sec, sym in probe_syms:
        a = std_syms.get(sym)
        b = rev_syms.get(sym)
        if a is None or b is None:
            failures.append(f"missing probe symbol {sym}: std={a} rev={b}")
            continue
        sec_a, va_a = a
        sec_b, va_b = b
        sec_ok = (sec_a == exp_sec) and (sec_b == exp_sec)
        moved = va_a != va_b
        sec_label = SECTION_NAMES.get(sec_a, f"sec{sec_a}")
        print(f"  [{sec_label:10s}] {sym[:50]:50s} 0x{va_a:08X} -> 0x{va_b:08X} (delta={va_b - va_a:+d}, sec_ok={sec_ok})")
        if not sec_ok:
            failures.append(f"symbol {sym} section mismatch: std={sec_a} rev={sec_b} expected={exp_sec}")
        if not moved:
            failures.append(f"symbol {sym} did not move under reverse-link (0x{va_a:08X})")

    # 2. Shared-storage identity proof (core groups + ALL ProductGlobals.cpp /alternatename bindings)
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
        std_vals = {std_syms[s][1] if s in std_syms else None for s in group}
        rev_vals = {rev_syms[s][1] if s in rev_syms else None for s in group}
        ok = len(std_vals) == 1 and None not in std_vals and len(rev_vals) == 1 and None not in rev_vals
        print(f"  {label:20s} std=0x{next(iter(std_vals)) or 0:08X} rev=0x{next(iter(rev_vals)) or 0:08X} ok={ok}")
        if not ok:
            failures.append(f"alias group {label} diverged: std={std_vals} rev={rev_vals}")

    pragmas = parse_alternatename_pragmas(GLOBALS_CPP)
    checked_pragmas = 0
    for alias, target in pragmas:
        # Only check aliases that survived /opt:ref in the map
        if alias in std_syms or alias in rev_syms:
            checked_pragmas += 1
            if std_syms.get(alias) != std_syms.get(target):
                failures.append(f"std /alternatename mismatch: {alias}={std_syms.get(alias)} != {target}={std_syms.get(target)}")
            if rev_syms.get(alias) != rev_syms.get(target):
                failures.append(f"rev /alternatename mismatch: {alias}={rev_syms.get(alias)} != {target}={rev_syms.get(target)}")
    print(f"  ProductGlobals.cpp /alternatename pragmas: {len(pragmas)} declared, {checked_pragmas} live in map verified")

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
        d_std = std_syms[field_sym][1] - std_syms[base_sym][1]
        d_rev = rev_syms[field_sym][1] - rev_syms[base_sym][1]
        ok = d_std == expected and d_rev == expected
        print(f"  {label:48s} std=+0x{d_std:02X} rev=+0x{d_rev:02X} expected=+0x{expected:02X} ok={ok}")
        if not ok:
            failures.append(f"offset contract {label} failed: std={d_std:#x} rev={d_rev:#x} expected={expected:#x}")

    # 4. PE base-relocation check on all initialized code/self pointer slots
    pointer_contracts = [
        ("g_GameErrorContext.m_BufferEnd", "?g_GameErrorContext@th10@@3UGameErrorContext@1@A", 0x2000, "?g_GameErrorContext@th10@@3UGameErrorContext@1@A"),
        ("g_AnmSpecialVmDescriptors[0].cb", "?g_AnmSpecialVmDescriptors@@3QBUAnmSpecialVmDescriptorView@@B", 0x04, "?InstallAnmVmVertexBuffer@@YIHPAUAnmVmView@@PBI@Z"),
        ("g_PlayerShotSpawnCallbacks[1]", "?g_PlayerShotSpawnCallbacks@@3PAPAXA", 0x04, "?PlayerShotAcquireTarget@@YIHPAUPlayer@@PAUPlayerShotRuntimeView@@H@Z"),
        ("g_PlayerShotUpdateCallbacks[1]", "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA", 0x04, "?PlayerShotTrackEnemy@@YIHPAXPAUPlayerShotRuntimeView@@@Z"),
        ("g_PlayerShotUpdateCallbacks[2]", "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA", 0x08, "?PlayerShotSetOptionOrigin@@YIHPAUPlayer@@PAUPlayerShotRuntimeView@@@Z"),
    ]
    print("\n=== 4. Initialized Pointer Relocations (.reloc) ===")
    for exe_path, syms in ((STD_EXE, std_syms), (REV_EXE, rev_syms)):
        pe = pefile.PE(str(exe_path))
        base = pe.OPTIONAL_HEADER.ImageBase
        relocs = get_reloc_rvas(pe)
        for label, owner_sym, off, target_sym in pointer_contracts:
            slot_va = syms[owner_sym][1] + off
            slot_rva = slot_va - base
            val = struct.unpack("<I", pe.get_data(slot_rva, 4))[0]
            exp = syms[target_sym][1]
            ok = (slot_rva in relocs) and (val == exp)
            print(f"  {exe_path.name:20s} {label:32s} val=0x{val:08X} exp=0x{exp:08X} reloc={slot_rva in relocs} ok={ok}")
            if not ok:
                failures.append(f"{exe_path.name}: {label} val=0x{val:08X} exp=0x{exp:08X} reloc={slot_rva in relocs}")

    if failures:
        print("\nFAILURES:", file=sys.stderr)
        for f in failures:
            print(f"  - {f}", file=sys.stderr)
        return 1
    print("\nALL POSITION-INDEPENDENCE CHECKS PASSED.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
