# TH10 startup configuration dialog: command lifetime improvement (2026-11-02)

MainConfigurationDialogProc @ original 0x0043A3A0..0x0043A488 owns
233 bytes and remains NONEXACT. The real VC7.1 SP1 /GL /GS four-TU
WinMain graph now reproduces 216/233 bytes, up from 208/233 in a
previously compiled baseline, with all 14 external fields at the same
positions. No partial or whole-owner exact credit is claimed.

The original WM_COMMAND path saves ESI before reading the low word of
wparam. The earlier equivalent C++ switch saved ESI after discriminating
between commands 201 and 203. A compiler-only _ReadWriteBarrier()
immediately before switch (LOWORD(wparam)) recovers the target stack-home
placement of that input, repairing eight ordinary bytes. The barrier
emits no runtime lock, and its presence does not prove original source
qualifiers or synchronization intent. All dialog-message behavior is
unchanged.

Remaining ordinary differences total 17 bytes: initial ESI save offsets
+0x10..+0x13 and +0x16, and four return epilogues at
+0x59..+0x5B, +0x72..+0x74, +0xA5..+0xA7 and +0xBE..+0xC0.
The target XOR EAX,EAX / POP ESI order differs from native
POP ESI / XOR EAX,EAX. These real instructions remain compared.

This is a complete 233-byte PDB owner with 14 decoded external fields,
not a matching prefix. Current-source cold replay selected only two
existing exact windows owners, WindowProc 265 bytes/16 fields and
ActivateWindow 8 bytes/one field, and passes 273/273 bytes in the
same four-TU WinMain profile (Main.cpp, GameErrorContext.cpp,
FileSystem.cpp, Midi.cpp). See native-summary.json for target/source
and PE hashes, field observations, remaining differences and guard
receipt. Failed early-return alternatives shortened the full owner to
225 bytes, so they were not retained.

No full cold replay, source provenance, full product or runtime claim.

Reproduce with:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit main-window-proc --unit main-activate-window --json
    scripts/repo-python scripts/ci.py
    git diff --check
