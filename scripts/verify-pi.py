#!/usr/bin/env python3
"""Verify Position Independence (PI) of the TH10 v1.00a product build.

Checks:
1. Map displacement AND independent PE section table cross-check
   (pe.get_section_by_rva) between build/windows-i386/th10.exe and
   build/windows-i386/th10-reordered.exe across .text, .rdata, and .data.
2. Fail-closed shared-storage identity for ALL 65 /alternatename alias
   bindings declared in src/ProductGlobals.cpp (plus core alias groups) in
   both builds.
3. Contiguous struct-interior offset preservation across reordered links
   (g_GameScoreState, g_FrontEndInput, g_MainSupervisorView).
4. Strict IMAGE_REL_BASED_HIGHLOW (type == 3) PE base-relocation coverage
   and target address/string verification for:
   - 5 initialized code/self pointer slots in src/ProductGlobals.cpp
   - all 37 g_SoundFileNames[0..36] string pointer slots in src/Sound.cpp
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
SOUND_CPP = ROOT / "src" / "Sound.cpp"

IMAGE_REL_BASED_HIGHLOW = 3
EXPECTED_PRAGMA_COUNT = 65


def parse_map(map_path: Path) -> dict[str, tuple[int, int]]:
    """Return {symbol: (map_segment_index, va)} from a VC7.1 .map file."""
    syms: dict[str, tuple[int, int]] = {}
    pat = re.compile(r"^\s*([0-9a-fA-F]{4}):[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s+")
    for line in map_path.read_text(errors="replace").splitlines():
        m = pat.match(line)
        if m:
            syms[m.group(2)] = (int(m.group(1), 16), int(m.group(3), 16))
    return syms


def get_reloc_map(pe: pefile.PE) -> dict[int, int]:
    """Return {rva: relocation_type} from PE base relocation directory."""
    relocs: dict[int, int] = {}
    if hasattr(pe, "DIRECTORY_ENTRY_BASERELOC"):
        for block in pe.DIRECTORY_ENTRY_BASERELOC:
            for entry in block.entries:
                if entry.type != 0:
                    relocs[entry.rva] = entry.type
    return relocs


def pe_section_name_for_va(pe: pefile.PE, va: int) -> str:
    rva = va - pe.OPTIONAL_HEADER.ImageBase
    sec = pe.get_section_by_rva(rva)
    if sec is None:
        return "<none>"
    return sec.Name.rstrip(b"\x00").decode("ascii", errors="replace")


def read_pe_cstr(pe: pefile.PE, va: int, limit: int = 128) -> str:
    rva = va - pe.OPTIONAL_HEADER.ImageBase
    raw = pe.get_data(rva, limit)
    nul = raw.find(b"\x00")
    if nul != -1:
        raw = raw[:nul]
    return raw.decode("ascii", errors="replace")


def parse_alternatename_pragmas(cpp_path: Path) -> list[tuple[str, str]]:
    pat = re.compile(r'#pragma\s+comment\(linker,\s*"/alternatename:([^=]+)=([^"]+)"\)')
    return pat.findall(cpp_path.read_text(errors="replace"))


def parse_sound_filenames(sound_cpp: Path) -> list[str]:
    txt = sound_cpp.read_text(errors="replace")
    m = re.search(r"g_SoundFileNames\[SE_COUNT\]\s*=\s*\{(.*?)\};", txt, re.S)
    if not m:
        raise ValueError("could not locate g_SoundFileNames[SE_COUNT] in src/Sound.cpp")
    return re.findall(r'"([^"]+)"', m.group(1))


def main() -> int:
    for p in (STD_EXE, STD_MAP, REV_EXE, REV_MAP):
        if not p.is_file():
            print(f"error: missing {p}; run scripts/build.py and scripts/build.py --reverse-link first", file=sys.stderr)
            return 1

    std_syms = parse_map(STD_MAP)
    rev_syms = parse_map(REV_MAP)
    std_pe = pefile.PE(str(STD_EXE))
    rev_pe = pefile.PE(str(REV_EXE))
    failures: list[str] = []

    # 1. Displacement + independent PE section table cross-check
    probe_syms = [
        (1, ".text", "_WinMain@16"),
        (1, ".text", "?PlayerShotTrackEnemy@@YIHPAXPAUPlayerShotRuntimeView@@@Z"),
        (1, ".text", "?InstallAnmVmVertexBuffer@@YIHPAUAnmVmView@@PBI@Z"),
        (2, ".rdata", "?g_MainWindowTitle@@3QBDB"),
        (2, ".rdata", "?g_AnmSpecialVmDescriptors@@3QBUAnmSpecialVmDescriptorView@@B"),
        (3, ".data", "?g_BulletCullBottom@@3MA"),
        (3, ".data", "?g_GameScoreState@@3UGameScoreStateView@@A"),
        (3, ".data", "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA"),
        (3, ".data", "?g_GameErrorContext@th10@@3UGameErrorContext@1@A"),
        (3, ".data", "?g_AnmRenderManagerView@@3PAUAnmRenderManagerView@@A"),
        (3, ".data", "?g_Player@@3PAUPlayer@@A"),
        (3, ".data", "?g_EnemySoundCueMetadata_head@@3IA"),
        (3, ".data", "?g_SoundCueMetadata_storage@@3PAIA"),
    ]
    print("=== 1. Symbol Displacement & Real PE Section Check ===")
    for exp_seg, exp_sec_name, sym in probe_syms:
        a = std_syms.get(sym)
        b = rev_syms.get(sym)
        if a is None or b is None:
            failures.append(f"missing probe symbol {sym}: std={a} rev={b}")
            continue
        seg_a, va_a = a
        seg_b, va_b = b
        pe_sec_a = pe_section_name_for_va(std_pe, va_a)
        pe_sec_b = pe_section_name_for_va(rev_pe, va_b)
        sec_ok = (seg_a == exp_seg) and (seg_b == exp_seg) and (pe_sec_a == exp_sec_name) and (pe_sec_b == exp_sec_name)
        moved = va_a != va_b
        print(f"  [{pe_sec_a:6s}] {sym[:50]:50s} 0x{va_a:08X} -> 0x{va_b:08X} (delta={va_b - va_a:+d}, sec_ok={sec_ok})")
        if not sec_ok:
            failures.append(f"symbol {sym} section mismatch: map=({seg_a},{seg_b}) pe=({pe_sec_a},{pe_sec_b}) expected=({exp_seg},{exp_sec_name})")
        if not moved:
            failures.append(f"symbol {sym} did not move under reverse-link (0x{va_a:08X})")

    # 2. Fail-closed shared-storage identity proof
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
    print("\n=== 2. Shared-Storage Identity (Fail-Closed) ===")
    for label, group in alias_groups:
        std_vals = {std_syms[s][1] if s in std_syms else None for s in group}
        rev_vals = {rev_syms[s][1] if s in rev_syms else None for s in group}
        ok = len(std_vals) == 1 and None not in std_vals and len(rev_vals) == 1 and None not in rev_vals
        print(f"  {label:20s} std=0x{next(iter(std_vals)) or 0:08X} rev=0x{next(iter(rev_vals)) or 0:08X} ok={ok}")
        if not ok:
            failures.append(f"alias group {label} diverged: std={std_vals} rev={rev_vals}")

    pragmas = parse_alternatename_pragmas(GLOBALS_CPP)
    if len(pragmas) != EXPECTED_PRAGMA_COUNT:
        failures.append(f"expected {EXPECTED_PRAGMA_COUNT} /alternatename pragmas in ProductGlobals.cpp, found {len(pragmas)}")
    verified_pragmas = 0
    for alias, target in pragmas:
        sa, st = std_syms.get(alias), std_syms.get(target)
        ra, rt = rev_syms.get(alias), rev_syms.get(target)
        if sa is None or st is None or ra is None or rt is None:
            failures.append(f"fail-closed /alternatename missing from map: {alias}={sa}/{ra}, {target}={st}/{rt}")
            continue
        if sa != st or ra != rt:
            failures.append(f"/alternatename mismatch: {alias} ({sa}/{ra}) != {target} ({st}/{rt})")
            continue
        verified_pragmas += 1
    print(f"  ProductGlobals.cpp /alternatename pragmas: {verified_pragmas}/{len(pragmas)} fail-closed verified in both maps")

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

    # 4. Strict IMAGE_REL_BASED_HIGHLOW relocation check on 5 code/self pointers + 37 g_SoundFileNames
    pointer_contracts = [
        ("g_GameErrorContext.m_BufferEnd", "?g_GameErrorContext@th10@@3UGameErrorContext@1@A", 0x2000, "?g_GameErrorContext@th10@@3UGameErrorContext@1@A"),
        ("g_AnmSpecialVmDescriptors[0].cb", "?g_AnmSpecialVmDescriptors@@3QBUAnmSpecialVmDescriptorView@@B", 0x04, "?InstallAnmVmVertexBuffer@@YIHPAUAnmVmView@@PBI@Z"),
        ("g_PlayerShotSpawnCallbacks[1]", "?g_PlayerShotSpawnCallbacks@@3PAPAXA", 0x04, "?PlayerShotAcquireTarget@@YIHPAUPlayer@@PAUPlayerShotRuntimeView@@H@Z"),
        ("g_PlayerShotUpdateCallbacks[1]", "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA", 0x04, "?PlayerShotTrackEnemy@@YIHPAXPAUPlayerShotRuntimeView@@@Z"),
        ("g_PlayerShotUpdateCallbacks[2]", "?g_PlayerShotUpdateCallbacks@@3PAP6IXPAUPlayer@@PAUPlayerShotRuntimeView@@@ZA", 0x08, "?PlayerShotSetOptionOrigin@@YIHPAUPlayer@@PAUPlayerShotRuntimeView@@@Z"),
    ]
    sound_names = parse_sound_filenames(SOUND_CPP)
    if len(sound_names) != 37:
        failures.append(f"expected 37 entries in g_SoundFileNames, found {len(sound_names)}")

    print("\n=== 4. Strict HIGHLOW Pointer Relocations (.reloc) ===")
    for exe_path, pe, syms in ((STD_EXE, std_pe, std_syms), (REV_EXE, rev_pe, rev_syms)):
        base = pe.OPTIONAL_HEADER.ImageBase
        relocs = get_reloc_map(pe)
        for label, owner_sym, off, target_sym in pointer_contracts:
            slot_va = syms[owner_sym][1] + off
            slot_rva = slot_va - base
            val = struct.unpack("<I", pe.get_data(slot_rva, 4))[0]
            exp = syms[target_sym][1]
            rtype = relocs.get(slot_rva)
            ok = (rtype == IMAGE_REL_BASED_HIGHLOW) and (val == exp)
            print(f"  {exe_path.name:20s} {label:32s} val=0x{val:08X} exp=0x{exp:08X} highlow={rtype == IMAGE_REL_BASED_HIGHLOW} ok={ok}")
            if not ok:
                failures.append(f"{exe_path.name}: {label} val=0x{val:08X} exp=0x{exp:08X} rtype={rtype}")

        # Verify all 37 g_SoundFileNames[0..36] string pointers
        se_sym = "?g_SoundFileNames@@3PAPBDA"
        if se_sym not in syms:
            failures.append(f"{exe_path.name}: missing {se_sym}")
            continue
        se_base_va = syms[se_sym][1]
        se_ok = 0
        for idx, expected_name in enumerate(sound_names):
            slot_va = se_base_va + idx * 4
            slot_rva = slot_va - base
            ptr_va = struct.unpack("<I", pe.get_data(slot_rva, 4))[0]
            rtype = relocs.get(slot_rva)
            tgt_sec = pe_section_name_for_va(pe, ptr_va)
            actual_str = read_pe_cstr(pe, ptr_va) if tgt_sec == ".rdata" else "<bad-sec>"
            if rtype == IMAGE_REL_BASED_HIGHLOW and tgt_sec == ".rdata" and actual_str == expected_name:
                se_ok += 1
            else:
                failures.append(
                    f"{exe_path.name}: g_SoundFileNames[{idx}] rtype={rtype} sec={tgt_sec} str={actual_str!r} expected={expected_name!r}"
                )
        print(f"  {exe_path.name:20s} g_SoundFileNames[0..36]          verified={se_ok}/{len(sound_names)} (HIGHLOW + .rdata + exact string)")

    # 5. Contiguous 47-entry Sound Metadata Table Contract (376 bytes = head 4B + storage 372B)
    print("\n=== 5. Contiguous Sound Metadata Table Contract (47 x 8B = 376B) ===")
    sm_head = "?g_EnemySoundCueMetadata_head@@3IA"
    sm_stor = "?g_SoundCueMetadata_storage@@3PAIA"
    sm_enemy = "?g_EnemySoundCueMetadata@@3PAUEnemySoundCueMetadataView@@A"
    sm_play = "?g_SoundPlaybackMetadata@@3PAUSoundPlaybackMetadata@@A"
    sm_cue = "?g_SoundCueMetadata@@3PAUSoundCueMetadata@@A"
    for exe_path, pe, syms in ((STD_EXE, std_pe, std_syms), (REV_EXE, rev_pe, rev_syms)):
        base = pe.OPTIONAL_HEADER.ImageBase
        missing = [s for s in (sm_head, sm_stor, sm_enemy, sm_play, sm_cue) if s not in syms]
        if missing:
            failures.append(f"{exe_path.name}: missing sound metadata symbols: {missing}")
            continue
        va_head = syms[sm_head][1]
        va_stor = syms[sm_stor][1]
        delta = va_stor - va_head
        alias_ok = (
            syms[sm_enemy][1] == va_head
            and syms[sm_play][1] == va_head
            and syms[sm_cue][1] == va_stor
        )
        sec_head = pe_section_name_for_va(pe, va_head)
        sec_stor = pe_section_name_for_va(pe, va_stor)
        sec_ok = sec_head == ".data" and sec_stor == ".data"
        raw = pe.get_data(va_head - base, 47 * 8)
        first_dw = struct.unpack("<I", raw[0:4])[0]
        last_dw = struct.unpack("<I", raw[372:376])[0]
        bytes_ok = (len(raw) == 376) and (first_dw == 0x00000000) and (last_dw == 0x0032FCE0)
        ok = (delta == 4) and alias_ok and sec_ok and bytes_ok
        print(
            f"  {exe_path.name:20s} head=0x{va_head:08X} stor=0x{va_stor:08X} "
            f"delta=+{delta} sec=({sec_head},{sec_stor}) tail=0x{last_dw:08X} ok={ok}"
        )
        if not ok:
            failures.append(
                f"{exe_path.name}: Sound metadata layout contract failed: "
                f"delta={delta} alias_ok={alias_ok} sec=({sec_head},{sec_stor}) "
                f"first=0x{first_dw:08X} tail=0x{last_dw:08X}"
            )

    if failures:
        print("\nFAILURES:", file=sys.stderr)
        for f in failures:
            print(f"  - {f}", file=sys.stderr)
        return 1
    print("\nALL POSITION-INDEPENDENCE CHECKS PASSED.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
