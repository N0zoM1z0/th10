# Replay SaveReplay capture correction, 2026-10-25

## Result and limits

Fresh complete baseline at source checkpoint `9d9742bf`: **1748/1681 bytes**.
Separate postcopy-only and prepass-only models produce **1807** and **1744**;
the reviewed combined source and fresh canonical graph produce **1794/1681**.
These are full PDB-owned extents, not mismatch-byte counts. SaveReplay remains
**nonexact**, with no source-owner, exact-byte or denominator admission.

Target `0x429B60..0x42A1F0` is the existing reviewed authored 1681-byte owner.
The unchanged two-TU `filesystem-open-write-file` graph compiles FileSystem.cpp
then ReplayManager.cpp, with real SaveReplay entry and existing VC7.1 /GL /GS
profiles, anchors and link settings. No flags, annotations, ABI declarations,
canonical recipes or comparison gates change. The standalone opener remains
**exact 137/all 12** in each image and in a separate fresh canonical cold build;
see [canonical receipt](canonical-opener.json).

## Target-backed changes

1. **Postcopy:** records capture frame at 429D80, read its end at 429D82, copy,
   then reread the same captured frame at 429DB4. FPS repeats this at 429DF0,
   429DF2 and 429E1B. Baseline used cached copy lengths for both cursor advances.
   Two ordinary expressions now reread the captured frame, not `link->value`.
   Combined native reloads at 402A76 and 402ADB follow their respective copies.
2. **Prepass alias lifetime:** target reloads the stage-header pointer for the
   payload update. After payload store 429C8E, it reloads `link->value` at 429C91,
   reads the record end at 429C93 and reloads the stage header at 429C99 before
   updating record count. The source now expresses these separate evaluations.
   Combined native payload store 40294C precedes link reload 40294F and header
   reload 402957. This is separate from the postcopy capture model.

The target also rereads record/FPS ends for payload after computing allocation
size (429C60/429C76). No intervening target write or call exists: only a header
load and arithmetic separate those reads. The natural repeated expressions are
CSE'd in both prepass and combined native models, so this aspect remains
**unrecovered**. No volatile, forced register, noinline or profile panel follows.

Payload size still resets, record count does not. The serializer still packs
all nodes' records before all nodes' FPS samples. The four writes still ignore
BOOL, compare output count and preserve repeated close/decrement behavior.

## Complete diagnostic scope

Every image decodes through its complete contribution. Baseline/postcopy/
prepass/combined have respectively 41/41/40/41 internal branch destinations;
all are instruction starts. Each has 95 linked fields (69 DIR32, 26 REL32) and
46 calls. All candidate field identity multisets and call identities remain
consistent across these models; register changes on indirect leave-critical-
section calls were independently followed to the same anchor.

Target has 41 calls, 38 local branches, 27 REL32 fields and 56 syntactic absolute
address fields. Its stripped relocation directory is not a zero-absolute-field
claim. These inventories are not accepted semantic bindings. Target RET4 versus
candidate RET8, the inlined opener versus target explicit call and structural
extent/local/call differences remain blockers. No prefix/fragment is credited,
and no whole-parent exact comparison contract is claimed to pass.

Independent read-only review approves the limited source corrections, not exact
admission. Combined overlay changed only ReplayManager.cpp and has SHA-256
`c36918f724269293acb48c0bb8f61a945df84de7559e634fe19a6adcaf18cac0`.
Fresh canonical parent is raw-identical across all 1794 bytes to that reviewed
combined image. All 384 baseline source/config/script/protected-file pins were
unchanged before maintained application. Nine unrelated D3D artifacts remain
untouched.

## Tests, reproducibility and parking

The existing source-extracted payload-order test remains. A third host test
uses an explicit synthetic copy observer to change the captured frame's end and
replace link->value after copying. Two positives distinguish the reread of the
captured frame from four negative cached-length/recaptured-link controls. This
is an expression regression, not normal memcpy aliasing or game runtime proof.
All three staged and applied tests pass.

The ignored packet is `.analysis/gpt-dots/replay-save-20261025/`, with frozen
baseline pins, per-model source overlays, native producer scripts, exit logs,
complete disassembly/inventories, four opener receipts and canonical replay.
Native images are under `build/gpt-dots/replay-save-20261025/`; each native build
was serial, bounded, one CPU at nice10. Canonical cold and comparison both exit0.
No failed native build was overwritten. A preliminary read-only discovery
command failed on two absent guessed script/artifact paths before any build;
its durable command is `3c59845e4897455b98b583af1e605668`.

Keep unsupported CSE/register/flag tails parked. Reopen only with independently
supported target lifetime, alias, callee or real-context evidence and a specific
prediction. Product/runtime/shared storage and original TU/profile/ABI remain
unknown. Full checkpoint validation is recorded in the validation summary.

All **112 CI stages pass**, exit0, verified2026-10-25 05:25:39 UTC, with all
1504 frozen inputs preserved. Transport timeouts were recovered from durable
logs without rerunning. Only CI-result documentation was updated afterward.
