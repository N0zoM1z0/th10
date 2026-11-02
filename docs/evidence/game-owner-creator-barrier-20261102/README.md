# Game owner creator: target-sized source with a scoped ordering barrier

The 86-byte target `GameCreateOwner40AF90` at `0x0040AF90..0x0040AFE5`
starts with allocation, a null test, six DWORD zero-initialization stores,
then a direct `OR DWORD PTR [EAX],2` and publication to `0x004776FC`.
This target disassembly was independently reread from the verified original
Japanese v1.00a EXE, not inferred from earlier experiment descriptions.

The natural `new GameOwner40AF90View()` retained from the preceding checkpoint
emitted a 90-byte caller: it forwarded the known-zero field through a
separate ECX read/OR/write. A compiler-only `_ReadWriteBarrier()` immediately
before setting the ownership flag makes the VC7.1 LTCG compiler respect the
actual zero-initialization memory boundary; the flag update now compiles to
the original three-byte memory `OR` instruction. The source preserves the
real allocation, initialization, success publication and failure cleanup.

**Current status: NONEXACT, 86 target bytes / 86 PDB-owned native bytes.**
The target places the EAX-to-EBX handoff after the global publication, then
branches around an explicit `XOR EBX,EBX` to reach the Initialize call.
The native compiler currently selects `MOV EBX,EAX` *before* the flag OR and
moves the null branch to the function tail. This changes ordinary bytes and
link field offsets, so same-size and a matching prefix grant **zero exact
credit**. A separate `publishedOwner` local was compiled but regressed to a
92-byte body with an extra branch, and was discarded. No match/ledger entry
was added. The source barrier is a codegen ordering model, **not** proof that
the original game used this intrinsic or a concurrency contract.

Native comparison uses the real GameManager.cpp + Chain.cpp + AnmManager.cpp
+ GameErrorContext.cpp `/GL /GS /EHs` link and a complete PDB contribution.
The neighboring 134-byte initializer still independently returns `exact`
for all 14 target-bound fields in the same producer. No original TU identity,
whole-game product, runtime or portability claim follows.

Narrow reproduction:

    scripts/repo-python scripts/verify-target.py
    nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit game-owner-40ae70-initialize
    scripts/repo-python scripts/compare-linked-function.py --unit game-owner-40ae70-initialize --json
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit game-owner-40ae70-initialize --unit game-cleanup-owner-40af00 --json
    scripts/repo-python scripts/ci.py
    git diff --check
