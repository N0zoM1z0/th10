# Replay initialization: real construction and shared state — 2026-11-06

`ReplayManager::Initialize @428F60..42949F` owns1344 original bytes, ending
plain RET. Its actual sole caller, Create429610, supplies EBX=manager,
EAX=mode and ECX=path. Boundary and authored-origin reviews are unchanged.
The initializer remains **NONEXACT**, with **zero new exact credit**.
[native-result.json](native-result.json) pins the target, current source,
complete comparisons and affected existing contracts.

## Installed source

The pinned MIT reference
[`d1f3121`](https://github.com/levzzz5154/th10-decomp/commit/d1f31212514781c928fba145d8207397c8fb5650),
`matching2/msvc/src/game/replay/replay_runtime_bootstrap.cpp`, supplies
lifecycle and alias-model leads. Its synthetic callers, escape function,
stand-ins and barriers are not an acceptance recipe. Attribution is retained
in the source and [MIT license](../../licenses/th10-decomp-MIT.txt).

The source now uses real shared game configuration, score-state, RNG and
application fields. In particular RNG seed/generation count are4918B0/+4;
new-game state is491C28+39C; score-state is474C40, with selected stage at+3C
and continues at+50. Unknown1B8 remains a separate global beyond that view.
The configuration defaults call at428FED precedes the enclosing replay-data
header's zero reset. Local construction views preserve24/64-byte serialized
base layouts without changing shared headers; they do not prove original
C++ member types or lifetime ownership. The real stage-header constructor
already clears its storage, so the duplicate reset is removed.

Six real ChainElem allocations retain construction, flags, callback, argument,
null added/deleted callbacks and actual Calc/Draw registration. Each manager
chain pointer is published after registration, as observed in the target.
The playback tail rereads the current stage's header after faith/timer setters;
the load-only path shares the original error footer. No fake ABI, forced
inlining, new barrier, volatile control, copied bytes or compiler-profile
change is introduced. Shared Chain.cpp is unchanged and still contains its
older constructor barriers; this is not a barrier-free-source claim.

## Complete comparisons and rejected controls

An independent original-image inventory covers the full1344 bytes:52 DIR32
and22 REL32 fields. Direct local destinations remain ordinary compared bytes;
there is no owned jump table. Original destinations, existing canonical
bindings, reviewed shared storage and complete equal NUL literals establish
bindings independently of the candidate's encoded values.

| Actual context / source epoch | Complete native owner | Result |
| --- | --- | --- |
| Historical Save21 cached source | 1406, with EH | Historical diagnostic only |
| Fresh26 real inputs, original source | 1318 /68 fields | Nonexact |
| Same26, corrected construction | 1325 /73 | Nonexact |
| Same26, installed live-stage/error tail | 1344 /74 | Strict rejection: stage DIR32 field at+3C5 instead of original+3CB |
| Old Create32, installed source, selected cold | 1414, with EH | Direct Create also186 versus185; rejected |
| Create34 with real configuration providers, selected cold | 1342 /74 | Init rejected; existing Create185/all12 restored |
| Same34 plus real FileSystem/PbgArchive,36-input fresh link | 1344 /74 | Same strict+3C5/+3CB rejection as26; no admission |

In the26-input candidate the remaining shuffle is two loads: original reads
`[stageHeader+1B4]` before selected-stage, while the candidate reverses them.
An explicitly rebased diagnostic finds9 ordinary byte differences at+3C4..3CD;
it has no acceptance authority. Continues snapshots, paired scalar captures,
signed stage, a typed counter view and a complete typed setup-state view are
byte-neutral and restored. Do not repeat these spelling controls without a
new falsifiable compiler/dataflow lead.

The34-input graph lacks real FileSystem/PbgArchive implementations. Its
LoadReplay receives path in ECX and ends plain RET, whereas the26-input graph
retains the original stack path/RET4 contract. Two missing one-byte pushes
account for Init1342; static Load is similarly180 versus181. This is an
observed context-dependent private ABI, not a source-only improvement.

Adding the real FileSystem/PbgArchive inputs under their existing profiles
restores stack path/RET4 and Init1344 in the36-input context. Every input IL
object is reused from the just-completed source/header-pinned34/21 colds;
object hashes and profiles are checked before a fresh nonincremental link.
This is **not** an independent36-input cold. All74 candidate field positions
repeat the26-input result, including the rejection at+3C5. The explicitly
rebased diagnostic is byte-identical to26 and still has exactly9 differences.
Existing static Load181/all12 and Create185/all12/EH both pass in this image.
This falsifies missing file-system context as a solution to the remaining
load-order mismatch; further scalar spelling/context sweeps are parked.

## Existing exact contracts and checkpoint scope

Create185 was **already accepted** before this work. Its old32-input recipe
omitted the newly used configuration-default providers and now fails186/185.
Only that unit's recipe adds the real GameManagerLifecycle/MainConfiguration
sources with their existing `/GL /GS` profiles, retaining every old input,
other profile, binding and entry. The dedicated34-input selected cold passes
all185 bytes/all12 fields, fixed locals and existing one-allocation EH binding.
It grants no additional credit. The historical32-input proof remains an older
source epoch, not a current-source certificate.

The26-input image preserves existing Save1681, static Load181, Destroy18,
configuration defaults125, Calc131, Draw131, stage clear93, allocation139,
faith73 and timer59:2631 existing bytes. Create185 separately passes with its
complete EH binding. The34-input cold's static Load180 is explicitly rejected;
its eight other selected guards pass769 bytes. It does not replace Load's
own recipe. Save's current-source21-input selected cold passes1681/all83; static Load181
and Destroy18 also pass in that same cold image without another rebuild.

Compiler/link jobs run serially at nice10 on one CPU with bounded runtimes
and retained exit logs. A Wine path-conversion fault interrupted one signed
counter probe; the subsequent toolchain execute smoke passed normal COFF,
LTCG, resource and PE32 checks, and the retained object was successfully
relinked. Failed attempts are not reported as successful builds.

Private reproducible workbench: `.analysis/gpt-web/20261106-replay-initialize/`;
native candidates: `build/gpt-web/20261106-replay-initialize/`;
selected Create: `build/match-linked/ReplayManagerCreateDefaults/`;
selected Save: `build/match-linked/EnemyReadIntOperand/`.
No comparator, original binary, shared header or reviewed boundary changes.
No product, runtime, semantic dual-Oracle or portability claim is made.
