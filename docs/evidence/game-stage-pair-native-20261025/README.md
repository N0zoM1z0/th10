# Stage-pair first complete diagnostic, 2026-10-25

## Scope and complete owners

At **ef5ca8d**, maintained source and metrics are unchanged. This ignored model
adds two complete missing bodies to staged GameManager.cpp and expands the
reviewed worker two-TU context to **four TUs**: staged GameManager,
ScriptViewerWorker, AnmManager and TransitionEffect, in that order. All retain
**/GL without /GS**, real OnUpdate entry and the standard diagnostic harness.
It is a combined source/context experiment, not a pure differential, canonical
recipe migration or original production partition. No score overlay is reused.

| Complete owner | Native / target bytes | Native fields / locals / calls |
| --- | --- | --- |
| GamePrepareAnimationSlots, 0x404450 | 184 / 214 | 1 / 5 / 1 |
| GameEnterMenuState, 0x404530 | 263 / 119 | 8 / 3 / 3 |
| GameManagerUpdateBody | 1605 / 1544 | 114 / 55 / 36 |
| InitializeForLoadedScript, 0x404F30 | 200 / 202 | 2 / 0 / 2 |

All four are **nonexact**. The reset target retains its complete **214-byte
extent including internal alignment**; no slice or trimmed owner is accepted.
Inventories and extent differences are not target mismatch scores. The VM child
is InitializeForLoadedScript, not the separate InitializeVm at 0x449870.

## Original failure and independent same-image supplement

Cold build exits **0**. Original comparison/root exits **1** remain preserved:
an inherited positional menu observer expected CMP, but this image naturally
uses TEST ESI,ESI. Independent review confirms the null/bit-8 gates and the same
captured ESI for cleanup/delete. It validates the retained image without another
compile; this supplement does not relabel the original failed invocation.

Required existing guards pass: manager 92+7+28 = **127/all 2**, Tick **72/all 4**
and Stop **101/all 3**. Optional TransitionEffect factory **structurally rejects
at 81 bytes versus 82**, and remains failed. A separate supplement observer's
exception-versus-result handling error is also retained, not hidden or mistaken
for a native failure. **53 original artifacts are preserved.**

The reset's signed widths, reloads, 32-bit counters, short store and unconditional
reset pass bounded review. Its target stack/RET4 interface becomes native EDI/RET;
the child VM receiver changes target ESI to native EDX, changing spills/alignment.
The transition naturally inlines allocation/factory/kind-2 initialization and
uses EBX rather than target ESI. These observations identify codegen differences,
not a demonstrated source defect or proof of complete semantic equivalence.
No independently grounded source/profile/ABI correction was found.

## Identities, limits and separate work

- Staged GameManager.cpp:
  `fd88c5a79b1a7e560feba310e7cbb3aebf7b017dbcaf45ac08c4377238d4fc0b`
- Retained image:
  `8691400fb3dbce91f85f63d096fc12c6a73712f444118b6f236b8cb0818bda4c`
- Independent supplement receipt:
  `b7003b3ccf54eb8460a038d1ba552ca13f48c5b03edc75d61ab1ed61f9595582`

[Summary JSON](summary.json) supplies compact graph and receipt identities under
`.analysis/gpt-dots/game-stage-pair-native-20261025`. No target payload or
disassembly is copied here. Original class/TU/ABI provenance, shared storage
and runtime closure remain unresolved. **Zero new maintained source owners,
exact bytes or denominator bytes; no new full-CI claim.** Prior CI scopes remain.

Park unsupported Stage source/profile/ABI panels. A separately prepared,
source-byte-fixed **GUI-only /Ob1 control in the nine-TU score context** is not
Stage profile tuning, adoption or a result of this experiment. No result for
that separate control is asserted here.
