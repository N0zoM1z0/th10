# TH10 game owner initializer: exact 134-byte function; creator remains open

The reviewed game-authored original TH10 Japanese 1.00a target at
0x0040AE70..0x0040AEF5 is now reconstructed as
GameOwner40AF90View::Initialize in src/GameManager.cpp:
**134/134 full bytes exact**, with **14 original call/data fields**
strictly bound. Its complete 134-byte PDB extent and all instructions
are checked in the true GameManager.cpp, Chain.cpp, AnmManager.cpp and
GameErrorContext.cpp VC7.1 SP1 /GL /GS /EHs source graph.

The source preloads bullet.anm at ANM index 7 and preserves the
failure-message pointer/Log path. The successful branch allocates
two real Chain callback nodes using original target callback entries
0x0040B050 and 0x0040B060, then registers them on the
calculation/draw chains with priorities 23 and 32. It saves both
nodes in the owner, with the original private EBX receiver.

Two independent selected cold builds reproduce the whole 134 bytes
and 14 target identities. Their PE SHA-256 and PDB GUIDs differ.
A separately selected cold guards GameCleanupOwner40AF00
**137/137 exact**, including the correct final g_GameOwner4776FC
publication. All pins and field bindings are in native-summary.json.

The related GameCreateOwner40AF90 at original 0x0040AF90 has been
implemented in natural source too, including its 24-byte allocation,
member publication, init call, and cleanup/deallocation on failure.
**It remains NONEXACT:** the same real four-source link emits a
full 83-byte creator against the original 86-byte function, with
different allocation/null-branch register scheduling. The nonexact
caller gets zero canonical or authored byte credit.

One reviewed authored exact function and 134 authored/canonical codegen
bytes are added. Names and original physical source partition are
descriptive, and no runnable whole game or runtime equivalence is
implied. Only affected units were cold-replayed.

Reproduce the narrow scope:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit game-owner-40ae70-initialize --json
    scripts/repo-python scripts/replay-exact-units.py \
      --unit game-cleanup-owner-40af00 --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit game-owner-40ae70-initialize --json
    scripts/repo-python scripts/ci.py
    git diff --check
