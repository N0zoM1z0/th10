# Game owner creator: preserve the target value-initialization shape (2026-11-02)

The original Japanese TH10 1.00a creator `GameCreateOwner40AF90` at
`0x0040AF90..0x0040AFE5` owns **86 bytes**. This remains **NONEXACT**.

Independent target disassembly starts with `PUSH EBX`, a 24-byte operator-new
call, `ADD ESP,4`, `TEST EAX,EAX`, a null branch, `XOR ECX,ECX`,
`MOV EDX,EAX`, and six DWORD stores of ECX to `[EDX+0..0x14]`.
Those are the semantics of value-initializing a trivially constructible
24-byte owner. The maintained source formerly used default `new` plus
an explicit `memset` in the caller. Under the actual GameManager/Chain/
AnmManager/GameErrorContext VC7.1 `/GL /GS /EHs` graph that spelled an
83-byte caller, and selected EBX immediately following operator new.

Using `new GameOwner40AF90View()` and removing the redundant explicit
`memset` restores the target's **allocation, null-test and six-store
instruction sequence through +0x23**, aside from the expected unresolved
REL32 call field and a conditional displacement whose destination is later
in the still-different function. The target's instruction at +0x24 is a
single `OR DWORD PTR [EAX],2`; the new candidate instead reads/ORs/writes
the flag via ECX and places the EBX publication/null path differently.
The full linked PDB contribution is **90 bytes**, versus target 86. No
subextent is accepted and no exact credit or ledger row is added. The new
spelling is a natural source model, not evidence of the original source text.

Controls: explicit six-field C++ constructor, inlined `memset` constructor,
volatile and bitfield flag declarations did not recover the target full owner
(in one case they introduced a saved ESI). The unchanged initializer's
selected native comparison still returns `exact` for all **134 bytes and
14 fields**. Use explicit selected cold replays of the initializer and
cleanup as the regression scope; there is no full cold replay.

Reproduce:

    scripts/repo-python scripts/verify-target.py
    nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit game-owner-40ae70-initialize
    scripts/repo-python scripts/compare-linked-function.py --unit game-owner-40ae70-initialize --json
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit game-owner-40ae70-initialize --unit game-cleanup-owner-40af00 --json
    scripts/repo-python scripts/ci.py
    git diff --check
