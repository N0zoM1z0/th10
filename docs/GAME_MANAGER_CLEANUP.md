# GameManager cleanup owner

The source at `GameManagerCleanup` covers the complete reviewed target
`0x417C80–0x4180D0` (1105 bytes), including transition setup, normal destruction,
transition11 reuse, manager-chain removal, and sound/final-state publication.
It takes one stack owner and the target returns with RET4. Its original source
name/class/TU and private compiler ABI provenance remain unknown.

The target direct callers at418155 and4203FC separately release the manager
allocation. Cleanup itself clears477810 but does not release its argument.
Normal destruction calls genuine `CleanupHintOwner` at417EB7 and then releases
the saved pointer at417EBD after Hint cleanup has cleared its global. The reuse
branch retains Hint. This is a real normal cleanup/export caller graph; no
artificial address escape or injected failure is used for retention.

## Preserved contracts

- Statistics finalization precedes low-runtime-flag clearing and timer-scale reset.
- Transition10/11/4/15/13 effects, coordinates, stage comparison, and state reloads
  remain target-observed. Replay is retained for transitions14/15.
- Normal subsystem destruction order and individual null guards are retained.
  Calls use maintained destructor declarations for Replay, Stage, Player, Item and Enemy.
- Reuse resets GUI, destroys secondary Stage, republishes the primary Stage,
  clears controller/animation callback bit2, clears Replay playbackChain+1CC
  and drawChain+C bit2, and wipes exactly0x21CEA0 bytes of Item records at+14.
- GameManager chain removal snapshots element and current Chain before each
  lock; increment follows Enter, decrement follows Leave; update precedes draw.
- Final manager publication precedes optional "dummy" sound command; command
  opcode is3/4 according to491D78 bit0x10. Runtime flags are reread after sound.
  Final state is4918A4=1 and4923A8=0 or0xFF000000 according to runtime bit1.

`ItemManager.hpp` and `ReplayChain.hpp` share existing real declarations and
layout assertions lifted intact from their original maintained TUs. They do
not invent new ownership classes or change those methods' implementation.
Existing GameManager partial pointer aliases remain explicit target-storage
obligations; typed casts do not make separate C++ globals physically alias.

## Genuine open dependencies

The new caller leaves42B1E0,422220,4145F0,405F70,41C1C0,42B570,414570,40AF00,
405620,408AF0 genuinely external. Existing `GameClearEnemyVms` is the candidate
409F90 boundary and needs explicit binding verification. No no-op native
implementations are supplied. Observed EBX/EAX private receivers do not justify
invented source calling conventions. CRT release identity, globals/aliases,
Win32 synchronization and full runtime closure remain separate obligations.

## Host evidence and limits

`scripts/test-game-manager-teardown.py` executes the complete actual lexical
source region with explicitly audited platform/subsystem host seams. It checks
70 defined-domain scenarios and50 compiled mutations. Mutant success requires
exit1 and exactly one intended CHECK expression at its generated source line;
compile errors, signals, sanitizer output, ambiguous sites and timeouts fail
the harness. Python `-O` does not disable its validation.

Natural delete and raw allocation release expressions are instrumented only
after mutation to verify cleanup/lifetime order and perform matching releases.
The item range uses an explicit byte-storage seam; mock destructors do not
prove the corresponding subsystem implementations. In particular, the Hint
boundary is mocked here, so these tests do not execute its exporter or prove
native exporter retention. Real Hint lifecycle/export tests remain separate.

Local O0/O2 and O1 ASan+UBSan Python-optimized runs, plus an independent O1
sanitized review, passed. LeakSanitizer is disabled because the execution host
uses ptrace; other sanitizer diagnostics remain fatal. Existing item-sound
(1 test) and replay-payload (2 tests) host fixtures also passed after header moves.
This is source-presence/defined-domain caller evidence, not native instruction
identity, accepted exactness, product execution or physical storage closure.

## Native normal-cleanup retention control

A single normal-profile LTCG diagnostic used the actual `GameManagerCleanup`
entry, primary GameManager.cpp and ordered Hint.cpp, Chain.cpp, AnmManager.cpp,
FileSystem.cpp support. PE SHA256
`5f10b4c4bb64c6bf9edf9833ef9559defdd2da59336fc29b9eff57707332a040`.
Its complete PDB contributions are GameManagerCleanup401CE0/1121,
CleanupHintOwner403720/222, ExportAutomaticHints402210/2669 and
ClearHintLists402F30/79. The cleanup target is1105 bytes, so this owner is
nonexact; contribution size is not a similarity score.

Independent complete decoding verifies the real normal-path call at401F1F to
Hint cleanup, its runtime hintMode==2 call at4037E3 to the actual exporter,
list clearing at4037EC, and canonical Hint-global clear at4037F1. The caller
then pushes the same saved ESI at401F24 and calls the mapped operator-delete
boundary at401F25, rather than reloading the cleared global. The transition11
reuse path bypasses those operations. These Hint bodies belong to the actual
Hint support TU, not synthetic native substitutes.

This is static retained-call evidence. The standard diagnostic's unresolved
allocator/global/other-subsystem symbols are nonrunnable DATA anchors; their
addresses do not establish physical storage aliases, original private ABIs,
actual memory release, or product execution. Other known subsystem destructor
bodies were deliberately outside this narrow Hint-retention support set.
No new canonical exact unit or exact-byte credit is assigned to this owner.

All18 affected canonical units across11 serialized cold producer groups passed, retaining7402 previously tracked exact bytes. No new exact credit. The final group completed during a connection interruption; its complete2545-byte report was recovered with no native processes remaining, so no duplicate build was launched.

Full target-independent CI passed2026-10-17 00:18:14 UTC. Only source, shared declarations, tests, ledgers and compact documentation are retained; generated native artifacts remain ignored.
