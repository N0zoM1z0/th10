# Replay callbacks and FPS exact closure (2026-10-20)

Natural FPS drawing closes182 authored bytes at429A70. The retained frame-control
body429A30/58 and draw adapter42A430/28 also reproduce complete target extents;
their origin stays indeterminate. No dummy register argument, forced attribute,
assembly source, target bytes, padding or compiler flag sweep is introduced.

## Target and source protocols

The canonical Japanese TH10 v1.00a SHA-256 is
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local read-only Ghidra verifies it and the full mapped .text hash before/after,
rolling back temporary function metadata. The target's full ranges, padding and
references are reviewed independently of source and exactness. No denominator,
boundary or origin classification changes are made.

All three chain callbacks consume the replay owner inECX. This agrees with
maintained generic ChainCallback and target dispatch; the replay-local cdecl
pointer typedef and declarations were inconsistent and now use fastcall. The
24-byte element layout and fields remain unchanged. Four source mappings are
added, including the retained58-byte control body; their original identifiers,
source/adapter origin and physical TU ownership remain unresolved.

The10-byte process adapter forwards to4297D0. Its current native receiver uses
ESI while the target usesEDI; the adapter and process body remain nonexact.
The68-byte frame-control callback first suppresses work when the runtime's+58
bit4 is set. Its shared member gates on the same non-NULL runtime, playback
mode1, **current input474E30 bit100**, and signed frameCounter%4!=0. It returns6
for qualifying skipped frames and1 otherwise. This corrects the older ledger's
vague global-behavior-flag description; pressed/repeated input is not this role.
The58-byte retained body shares this logic but lacks the outer suppression gate.
The callback currently remains62/68; a result-local expression gives63 and is
restored, without fake output slots or register annotations.

Draw callback42A430/28 suppresses drawing on runtime+58 bit4, otherwise calls the
ordinary DrawFps member. The real caller restores the target private EDX member
receiver naturally. The FPS body preserves separate record exclusion and
playback selection, matching the target's two mode checks; the earlier switch
collapsed them and produced172/182. Existing ReplayRuntimeState supplies the
already observed pointer slot477810 and+58 prefix, removing the duplicate opaque
GameManager pointer declaration. This does not establish the complete runtime
object, data storage owner or original C++ type.

FPS182 and callback28 preserve the existing byte-valued FPS,30/50 thresholds,
383/450 position,%3d formatting, pre-format color and fresh global ASCII reset.
Their only target code edge is callback42A447 to429A70. All other callback entry
references are initializer-held function pointers; no external interior code
entry was observed. The retained control body has no modeled entry reference;
its source/exact match grants no authored classification.

## Complete native proof

Two independent full18-TU /GL /GS /EHs cold links freeze the same53 source/header
inputs in the real front-end callback graph. Each strict PE/MAP/PDB replay
passes **1112 complete bytes/all32 fields**:

| Scope | Bytes | Fields |
| --- | ---: | ---: |
| New authored FPS body | 182 | 7 |
| New indeterminate retained control | 58 | 2 |
| New indeterminate draw adapter | 28 | 2 |
| Existing load/Destroy/formatter/queue/input/cursor/screen guards | 844 | 21 |

Only182 authored and86 indeterminate exact bytes are added. The existing
recipes are not migrated; the shared replay carrier is refreshed from current
proof bytes. The formatter85-byte guard binds the actual dependency used by
FPS. Existing load181/all12 still passes its allocation-EH protocol. Compact
[proof metadata](native-proof.json) records both runs. Unresolved symbol anchors
keep the image non-runnable; original partition, product and runtime stay open.

```sh
scripts/repo-python scripts/report-replay-load.py
scripts/repo-python scripts/build-match-unit.py --unit replay-draw-fps
scripts/repo-python scripts/compare-linked-function.py --unit replay-draw-fps --json
scripts/repo-python scripts/compare-linked-function.py --unit replay-control-playback-frames --json
scripts/repo-python scripts/compare-linked-function.py --unit replay-draw-fps-callback --json
scripts/repo-python scripts/test-frontend-replay-save.py --sanitize
```

Private probes reuse input- and object-hash-checked supports, recompiling the
affected TUs only. They are diagnostics. Source/chain correction restores the
FPS receiver and adapter; explicit mode gates restore the whole FPS body. The
shared ordinary control member closes the retained58-byte body. No speculative
register ABI, original inline/TU identity or calling convention is inferred
from the reference's unused ECX parameter. The existing save page1444/1460 and
its draw961/955 remain nonexact with no partial credit.

## Host and recovery scope

The suite consumes the actual callback pointer typedef, runtime prefix and
callback/shared-control/FPS bodies alongside the existing save/factory/queue
source. ProcessFrame remains an explicit lower role when testing its forwarder;
the actual process body is not replaced in production and receives no semantic
or exact credit from that host role. Callback tests use actual function pointers
and cover runtime NULL/present, flags0/4/8, invalid/record/play/load modes,
current input0/100/200, pressed-input traps, signed counters-5/-4/-1/0/1/3/4/5,
forwarded return values and draw suppression. Existing FPS format, color and
formatter-driven global replacement witnesses remain active.

ASan/UBSan/leak detection pass the positive suite and17 strict named mutants,
including wrong input source, modulo sense and draw delegation. Existing
SDK/layout/prefill/int32-time/I/O/renderer/allocation/queue-observation roles
remain explicit host limits. GameManager teardown now consumes the actual
fastcall replay typedef, rather than reproducing the old cdecl declaration.
Its affected baseline also passes. No native ABI, x87 or target-runtime proof
follows from host callbacks.

The MIT pinned reference remains hypothesis material, with its
[license retained](../../licenses/th10-decomp-MIT.txt). Pre-existing D3D evidence
is untouched. Generated dumps remain ignored, superseded native probes are
removed and one current input-bound support cache is retained for follow-up.
Product/runtime, dual-Oracle semantic and port gates remain open.
