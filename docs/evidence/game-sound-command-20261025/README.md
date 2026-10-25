# Sound command declaration and real caller, 2026-10-25

## Maintained source and target facts

`Sound.hpp` now shares the existing complete `SoundPlayerCommand` and partial
`SoundPlayerView` declarations. The declarations are byte-identical moves;
all Sound implementations and existing size/offset assertions remain unchanged.
GameManager uses the real `QueueCommand` member instead of the untyped
`GameAssignMusic` facade. Its canonical owner is **0x492590**, not the separate
Supervisor at **0x491C28**. No duplicate global, ABI attribute, forced inlining,
profile change or canonical graph migration was introduced. The source-aware
result-transition fixture extracts the two declarations from the new header;
its function extraction still uses actual Sound.cpp implementations.

Target caller **0x4185E7** invokes **0x43E460**, a complete **104-byte** owner:
EAX carries the owner, EDX the dummy path at 0x46DF98, and two stack arguments
carry opcode 3/4 and argument 0; the callee returns with RET8. Existing source
scans 31 of 32 physical queue slots at +0x1F88, stride 0x10C, writes the command,
copies the string and clears step; full queues silently drop the request.
Target and native receiver/path/argument roles were independently reviewed.
This is bounded reconstruction evidence, not product/runtime validation.

## First complete comparison and controlled context

The ignored four-TU graph uses GameManager, ScriptViewerWorker, AnmManager and
Sound, in that order, with the inherited /GL-without-/GS OnUpdate recipe.
Complete parent **1569/1544** and QueueCommand **90/104** remain **nonexact**.
Parent inventory covers 114 fields, 55 native local branches and 36 calls;
QueueCommand has zero external fields and four native local branches. All seven
required same-image guards pass **453 bytes / all 14 fields**. Independent
review preserves 57 original artifacts and validates complete PDB extents,
fixed local destinations and the naturally generated real member edge.

The eight accessible optional Sound contracts have four exact results
(StartLoad, JoinWorkerThreads, ReleaseOwner, FindWaveChunk), three field-position
rejections (StopBgm, ReopenBgm, BgmPlayerThread), and one SetupThread mismatch.
These are **not** passed checks. A separate graph-only baseline using unchanged
pre-adoption sources plus Sound has parent 1572 and QueueCommand 90. Independent
whole-contribution comparison proves all **26 emitted Sound owners / 5114 bytes**
preserve extent, ordinary bytes, local destinations and source-supported
symbol/member-addend identities across the proposal. Seven are raw-equal.
Thus all four optional failures preexist this declaration/caller change in
this graph; the graph does not replace their original canonical contexts.
The baseline Tick observer initially selected absent StageScript.cpp; a
separate selector-only correction to GameManager.cpp passes 72/all 4, preserving
the original failed output. An initial symbol-only differential observer also
rejected unnamed member operands; its corrected explicit-member review and
original rejection are both retained, without nearest-symbol inference.

## Canonical regression closure and failure recovery

All **41 Sound-exposed canonical units / 10753 bytes** and all **five
GameManager units / 223 bytes** pass in their original profiles/graphs:
**46 existing units / 10976 bytes**, with no new credit. The original aggregate
cold run did **not** pass: after the first 45 units, its final Faith build failed
when Wine page-fault/debugger text contaminated a support-object pathname,
causing linker LNK1104. MCP calls timed out while the operation held the repo
lock; no duplicate compiler was started. Terminal exit 1 and failed artifacts
are retained, not relabeled successful.

After terminal recovery and a no-compiler check, separate comparisons recovered
all 45 completed cold artifacts exactly (10911 bytes). Only the failed Faith
unit was cold-retried, with unchanged source/profile, and passed 65/all 1, exit 0.
No diagnostic text was filtered to manufacture a successful compile. The
original pre-adoption Faith PE/MAP/PDB and failed-link objects/logs are retained
separately. The current three-TU canonical Faith image also has a 1569-byte
nonexact parent, but its unresolved QueueCommand anchor is not the four-TU
real-body proof; equal sizes do not equate those contexts.

## Validation and credit

- Result-transition host baseline plus **32 assertion-identified mutations** pass.
- Music-room host suite: **3 tests** pass.
- Target/toolchain execution attestation, Factory-native Ghidra check, tracking,
  toolchain declaration check, progress generation and whitespace checks pass.
- Full target-independent CI: **112 stages, exit 0**, verified **2026-10-25
  04:59:48 UTC**; all **1499 frozen inputs** preserved. Subsequent changes are
  documentation only and receive their separate reference/whitespace checks.
- Existing source/exact/denominator ledgers and canonical recipes are unchanged.
  **Zero new whole-owner exact bytes and zero new source-present owners.**

[Compact summary](summary.json) pins source and receipts. Raw disassembly,
PE/PDB artifacts and diagnostic runners remain ignored under the four named
`game-sound-*20261025` analysis roots and corresponding build roots. Their
combined retained footprint is under 6 MiB; they reproduce unresolved extents,
context differences and the initial failure. The old prepared-only and failed
observer records remain historical, not silently rewritten as current results.
Nine pre-existing D3D adapter files were preserved and excluded from staging.

Original class/TU ownership, full global storage lifetime/alias validity and
Windows i386 audio/product/runtime closure remain unknown. Park this nonexact
body/context result; do not reopen neutral flag/noinline panels. The project
remains **active-incomplete**, with later medium/large work selected separately.
