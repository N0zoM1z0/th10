# TH10 ResultMenuResume: full authored 74-byte closure (2026-11-02)

The verified Japanese TH10 v1.00a owner ResultMenuResume at
0x00422C30..0x00422C79 has a complete 74-byte target extent.
It is now strict **74/74 canonical exact**, including all seven independently
bound DIR32/REL32 fields, and is reviewed as authored in the existing origin
ledger. The native profile is the genuine 17-input VC7.1 SP1 /GL /GS /EHs
EndingCreate compile/link graph, with FrontEnd.cpp as a real support source.

## Root cause

The target clears ResultGameManagerView.flags58 & 0x10, sends UnPause
through the ordinary SoundPlayer queue, requests the auxiliary ANM VM
deletion, then zeroes menu->auxiliaryVm.value before copying
menu->savedSpeed into the global ANM game speed. The earlier
same-sized 74-byte compiled candidate moved the saved-speed load before
the zero ID store, producing twelve different ordinary byte locations.

A single C++ compiler memory-order boundary following the actual
ID clear recovers the target layout:

    g_AnmRenderManagerView->MarkVmForDeletion(menu->auxiliaryVm);
    menu->auxiliaryVm.value = 0;
    _ReadWriteBarrier();
    g_AnmGameSpeed = menu->savedSpeed;

The barrier has no machine instruction of its own, is local to the
observed publication boundary, and does not introduce runtime
synchronization. It models target compiler ordering, not proof
that the original source used this intrinsic.

## Complete evidence

Strict comparison checks target 0x00422C30, native symbol
?ResultMenuResume@@YAXPAUResultMenuView@@@Z, complete PDB
contribution, all decoded instructions and branches, and precisely
these original bound fields:

| Offset | Type | Original target role |
| --- | --- | --- |
| +0x01 | DIR32 | Result manager pointer 0x00477810 |
| +0x13 | DIR32 | UnPause string 0x0046E0B8 |
| +0x18 | DIR32 | Main sound owner 0x00492590 |
| +0x1D | REL32 | SoundPlayerView::QueueCommand 0x0043E460 |
| +0x29 | DIR32 | ANM render-manager pointer 0x00491C10 |
| +0x2F | REL32 | AnmRenderManagerView::MarkVmForDeletion 0x004492A0 |
| +0x45 | DIR32 | ANM game-speed scalar 0x00476F78 |

The native extent is 74 bytes with zero remaining ordinary differences
after strict seven-field binding. No original target binary bytes,
inert padding, fabricated register ABI or return behavior changed.

Two independent selected cold builds reproduce ResultMenuResume
74 bytes/all seven fields plus existing EndingCreate 99 bytes/five
fields: **173/173 per build**. Native PE hashes and PDB GUIDs differ.
The source/target hashes, field identities and scoped receipts
are retained in [native-summary.json](native-summary.json).

Other ResultMenuUpdatePause, RequestCaptureRectangle and ANM
ExecuteScript large owners remain open. There is no full cold
cohort replay, product closure, runtime or original source/physical
translation-unit provenance claim.

Reproduce this scope:

    scripts/repo-python scripts/verify-target.py
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
      --unit result-menu-resume --unit ending-create --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit result-menu-resume --json
    scripts/repo-python scripts/ci.py
    git diff --check
