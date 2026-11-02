# TH10 GameCleanupOwner40AF00: authored full 137-byte source closure

The previously source-unmapped but authored/target-reviewed owner
0x0040AF00..0x0040AF88 is now fully reconstructed as
GameCleanupOwner40AF00 in the maintained GameManager.cpp. The
complete 137-byte PDB-owned native function has zero ordinary
differences and all 15 original external address destinations
strictly bound in the actual two-TU VC7.1 SP1 /GL /GS /EHs
GameManager.cpp plus Chain.cpp caller graph.

## Correct target identity, not a borrowed destructor

An initial source-independent scan noticed that this 137-byte
target has the same ordinary instruction layout as the existing
ScriptViewerBombOwnerView destructor. That alone is **not**
proof of equivalent object identity: the original 0x0040AF00
target finally zeroes global **0x004776FC**, whereas the
ScriptViewer bomb destructor clears 0x004776EC. The actual
GameManagerCleanup caller reaches the published
g_GameOwner4776FC and calls this separate cleanup owner.

The new natural GameCleanupOwner40AF00 function reads the
owner's two callback pointers at +8 and +12, captures the real
global Chain pointer, and conditionally unregisters each
callback. Each arm enters/leaves the same supervisor critical
section and increments/decrements the lock-depth byte in the
observed order; after both it clears the correct global
g_GameOwner4776FC. The caller owns the memory deletion.
This reproduces the original observable state transitions
without adding unrelated deletion or resource reclamation.

One-source LTCG without Chain.cpp produced a wrong-sized 149-byte
owner: the compiler had only a declaration for UnregisterElem
and used another calling-convention/register schedule, including
separate count load/increment/store. Compiling the real Chain.cpp
with the source produces the target's compact 137-byte owner,
with private ECX/EDX receiver scheduling and one-byte memory
count updates, matching every instruction and branch. The
function remains an actual separate noinline owner with RET4.

## Complete field and source proof

The 15 external fields include EnterCriticalSection and
LeaveCriticalSection IAT addresses, two real Chain references,
two real calls to UnregisterElem at 0x00449F60, eight
critical-section/lock-depth member accesses rooted at
g_MainSupervisorView 0x00491C28 (addends 0x64C/0x6F4),
and the final g_GameOwner4776FC store. Every target role,
addend, source symbol and original address is recorded
in native-summary.json. No general zero-byte masks or
wrong-object symbol substitution are accepted.

Two separate selected cold builds each replay the full 137-byte
owner/all 15 fields, and their PE hashes and PDB GUIDs differ.
Additionally, a selected original GameManager.cpp source graph
preserves MarkGameChains28 and StageTransitionDelay92,
**120/120** bytes in one regression group.

This admits **one reviewed authored exact target owner and
137 authored/canonical bytes**. The much larger
GameManagerCleanup at 0x00417C80 remains NONEXACT; in
this two-TU diagnostic its PDB owner is 1110 bytes against
the original 1105 bytes, so it receives no partial/full credit.
Neither exact codegen nor the descriptive source name establish
the game's original translation-unit partition, product build,
runtime equivalence or portability.

Reproduce only selected scopes:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit game-cleanup-owner-40af00 --json
    scripts/repo-python scripts/replay-exact-units.py \
      --unit game-manager-mark-chains \
      --unit game-stage-transition-delay --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit game-cleanup-owner-40af00 --json
    scripts/repo-python scripts/ci.py
    git diff --check
