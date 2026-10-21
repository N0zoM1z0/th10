# TH10 reconstruction handoff

Updated 2026-10-21. This file describes the current checkpoint. Historical
experiments remain in Git, the linked evidence packs and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md); they are not a live work queue.

## Resume safely

- Target only original Japanese TH10 v1.00a at ignored `resources/th10.exe`:
  SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Recover tracked and untracked work before editing. The pre-existing untracked
  `docs/evidence/d3d-adapter-format/` is independent, unadopted work; preserve it.
- Use `scripts/repo-python`. Inspect `AGENTS.md`, [RE_WORKFLOW.md](RE_WORKFLOW.md)
  and [ORACLES.md](ORACLES.md) before target-dependent work.
- Current operator-authorized checkpoints use English `gpt-6.1-sol: ...`
  messages and are pushed to `origin/main` after validation.
  The operator requests direct local tools, not Factory MCP. This session uses
  Bash and, when needed, read-only Ghidra headless.
- Serialize native builds at nice10 or lower priority. Ghidra review uses
  read-only/noanalysis, rollback, one CPU and 768 MiB. Verify disk identity and
  complete mapped text before/after temporary metadata work:
  401000..465A80 inclusive, 412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  The old 64A80-byte hash is a truncated prefix, not complete mapped text.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current checkpoint

The operator has reopened reconstruction for both large and small functions,
including ANM ExecuteScript, ECL Run and Enemy DispatchEclInstruction. This work is active;
none of the three is exact yet. Re-evaluate old source/API assumptions against
fresh target and compiler evidence. The first bounded changes and measurements
are recorded in [large-owner call order](evidence/large-owner-call-order/README.md).

Live totals are generated in [PROGRESS.md](PROGRESS.md): 1775 reviewed
candidates, 734 authored functions owning 273274 bytes, 1080 source mappings,
1222 canonical exact functions overall. Authored exact is **381 functions,
69343/273274 bytes (25.37%)**. The 216 mapped authored nonexact functions,
523 indeterminate origins and unresolved inventory gaps remain open; the
authored denominator is provisional.

The latest checkpoint closes CalcExecutableChecksum43A1C0/207/all9 fields.
Natural signed division, pointer countdown, outer success path and cursor/base
snapshot restore the target register/save lifetimes. Two complete five-TU
Main/error/file/MIDI/archive /GL /GS colds through real WinMain freeze17 equal
inputs and strictly replay11 owners2191/all133 per image. The second cold
replays only the new207-byte recipe plus the affected original OpenFile360
recipe in its separate three-TU context:567 bytes across two groups.
Main now has15 exact owners out of20 source-present authored owners; five
remain nonexact, owning3442 bytes. See
[checksum proof](evidence/main-executable-checksum/README.md).
All100 standard CI steps pass in one invocation. Thirteen owned terminal
native discovery directories are pruned; ANM's measured portion is5712627 bytes,
Main's deleted-byte total is explicitly unrecorded. All source snapshots and
artifact identities are retained, with checksum cold proofs and one negative.

A separate unadopted follow-up finds a natural body for the missing
MainReleaseAnmSurfaces438A30/43; its complete native body is raw-equal, but
inlining grows Present339->371/236. Physical target release/FileCheck calls
remain outlined. Private source, current input/object pins and call inventory
are in `.analysis/gpt-6.1-sol/main-surfaces-20261021/RECOVERY.md`. Next test those
actual outlining boundaries; do not adopt the caller regression or grant43
credit from one warm diagnostic. Production checksum checkpoint excludes it.

Main.hpp is unchanged and Main.cpp is not a dependency of the current large
nine-TU graph; all30 source and four artifact pins remain unchanged. No extra
large rebuild is required. ANM9964/gap2, ECL7020/all189/27 differences and
Enemy14468/gap652 remain nonexact; separate two-TU Run remains26.

Eight new ANM source/interface controls are rejected. The complete four actual
getters and Float2/Float3/alpha/RGB evaluators pass as part of28 owners5180/all168.
Member and ordinary free stdcall GetFloatVar both reproduce424/all44, so source
ownership is not recovered by the private ESI protocol. A full nine-TU ordinary
unused-receiver manager-member ExecuteScript control also preserves its entire
native9964-byte owner and Run7020, with28 guards intact. G6/G7 and one natural
local-pointer rename are neutral. Wrapper/cdecl variants regress. Do not repeat
these hypotheses or infer target exactness from native equality; no ANM source
change is adopted. The source/control/artifact identities are in the checksum
proof and private `.analysis/gpt-6.1-sol/anm-execute-flags-20261021/RECOVERY.md`.

The preceding checkpoint closes WindowProc4390E0/265 and retained
ActivateWindow43A1B0/8:273 new authored exact bytes/all17 fields. A real
MidiOutputView::UnprepareHeader call plus common final DefWindowProcA source
return recovers the target private MIDI arguments and four live callback
parameters. Standard LRESULT/stdcall source and /GL /GS remain unchanged.
Two full Main/GameErrorContext/FileSystem/Midi colds through real WinMain freeze
14 identical inputs and strictly replay nine owners1624/all98 per image. The
second cold replays the two new units together and separately replays affected
CreateGameWindow341 in its original context: three units614 bytes total.
MainConfigurationDialogProc233 gains source but stays nonexact even at the same
size. Main has source for all20 reviewed authored owners;14 are exact and six
remain nonexact. See [window proof](evidence/main-window-callback/README.md).
One changed header input requires a new primary-only large-graph rebuild with
eight verified unchanged support objects. Complete large native bodies remain
equal after verified field rewrites,21 existing guards2415/all52 pass, and
ANM9964/gap2, Run7020/all189/27 differences and Enemy14468/gap652 remain nonexact.
The current large carrier is
`build/gpt-6.1-sol/main-window-callback-20261021/large-header-context`;
source/artifact pins are in the window proof. Product/runtime gates remain open.
All100 standard CI checks pass in one invocation; final focused gates pass.
Cleanup removes20 owned terminal native directories/5801419 bytes, retaining
the first actual final cold, canonical cold, MIDI negative and current large
carrier. Source deltas, input pins and reports remain. Three large goals stay active.

The preceding checkpoint closes CheckForRunningGameInstance439FF0/442 and
ResolveShortcut43A290/272:714 new authored exact bytes, one new source mapping.
The real SDK COM body recovers its private EBX/EDI arguments naturally. The
caller restores its Fatal receiver, uses the target byte-wide GameWindow+28
path flag, and keeps the nonnull-title path first. Two independent complete
Main/GameErrorContext/FileSystem /GL /GS colds through real WinMain strictly
replay714/all38 plus four existing guards558/all40,1272/all78 per image.
The second cold canonically replays only the two new units in one build.
Target GUIDs/imports, full boundaries and byte-field accesses are independently
reviewed; cdecl, swapped-GUID and wrong-addend controls fail. Stdcall and
fastcall source produce the same private body, so original annotation/TU/profile
remain unknown. Main source closure still lacks8-byte ActivateWindow and
233-byte dialog code. WinMain1311/1389, rendering1051/1159, checksum199/207 and
OpenFile297/360 are separate nonexact observations in this three-TU carrier.
See [startup source proof](evidence/main-startup-shortcut/README.md).
All100 standard CI checks pass in one invocation. Cleanup removes9 owned
terminal native directories/2126392 bytes, retaining both final cold carriers,
one cdecl failure and verified source/control deltas. Final focused gates pass.
The original ANM/Enemy/ECL C++ sources are unchanged, but Main.hpp changes
one of30 nine-TU inputs. Only EnemyEclDispatcher.cpp consumes it. A necessary
primary-TU recompile with eight hash-verified unchanged support objects keeps
all3 complete large native bodies equal after verified field rewrites;21
existing complete guards retain2415/all52. Current ANM9964/gap2, Run7020/all189
with27 differences and dispatcher14468/gap652/frame2C4/19 sentinels persist.
This is an incremental dependency check, separate from both startup colds;
all three large exact goals stay active.

The preceding checkpoint closes the retained45-byte InitD3DInterface439700 owner.
Calling the existing GameErrorContext::Fatal restores its omitted error-context
receiver and target EDI save/setup/restore. Two independent complete Main.cpp +
GameErrorContext.cpp /GL /GS colds through real WinMain freeze9 inputs and
strictly reproduce45/all5 fields plus the same-image existing Fatal201/all11
guard. The second cold uses the new canonical main-init-d3d-interface recipe.
Only45 authored bytes are credited; no denominator/source-map changes.
WinMain1298->1307/1389 remains nonexact. Supervisor491C28+4 is bound from the
known base and actual layout, not a coinciding neighboring data-anchor name.
See [D3D error-context proof](evidence/main-d3d-fatal/README.md).
All100 CI checks are covered in92+8 segments after correcting a missing new
validation-file link; code and recipes remain frozen. Final focused checks pass.
Cleanup removes17 owned terminal native directories/4073413 bytes; selected
proof/failure carriers and source/control inputs remain.

Fresh ECL controls leave production EclVm/Enemy source unchanged. A completed
read-only pinned-c2 stack-packing observation plus complete Run7020/all189
replay retains26 differences in the two-TU graph. FloatResult/logicalResult,
FORMAT offset and X share one color(weight36); scratch/Y/DX share another
(weight9). This is not just an equal-weight X/Y slot tie. Scoped float results
restore geometry/FORMAT homes but move integer arithmetic/logical homes and
retain32 ordinary differences. Direct/scoped results, bool types, operand
references/unions, installer declarations and the real callback support are
not accepted. Native/control recovery is under
`.analysis/gpt-6.1-sol/ecl-operand-representation-20261021/`.
FileSystem/PbgArchive real-callee and /Ob1 diagnostics also remain nonexact;
no caller is invented for retained LoadArchive4357D0, which has no raw direct
or literal-entry reference. These controls remain separate from canonical
recipes. All three original large functions remain active.

The preceding checkpoint supplies real AnmInt3Scale.cpp in the dispatcher graph,
migrating the six shared canonical recipes from eight to nine TUs. Only that
support uses the already reproduced /GL /GS /Ob1 profile; no C++ source or
semantic target bindings change. Two independent full nine-TU colds strictly
replay21 existing complete owners2415 bytes/all52 fields with all30 source pins
equal. RGB Evaluate780->820 and scale56 now match in this carrier. ANM
ExecuteScript remains9964 but aligned diagnostic positions improve7250->7478
of7995;90/92 target-sized spans and absolute gap2 remain. Run7020/all189,
62 local destinations retains27 ordinary differences; dispatcher14468/14416
retains gap652,40/108 target-sized spans, frame2C4 and19 sentinels. No new exact
or denominator credit. See [real dependency proof](evidence/enemy-real-scale-support/README.md).
ECL diagnostics now accept an explicit context manifest while retaining
canonical target bindings/callee templates; wrong-target and poisoned-template
controls pass. New carrier identities supersede the eight-TU image identities.
The separate RGB and other canonical contexts remain unchanged. Remaining
anchors keep the diagnostic image non-runnable; all three large owners are open.

The preceding checkpoint changes only two final rank-count reads to local generic
host views. Target EBX owner reloads for indices10/8 and the rank-five interval's
eleven integer calls/five returns are recovered. Two independent full eight-TU
colds reproduce the dispatcher at14468/14416 and retain19 complete existing
exact owners1539/all32, with all29 source pins equal. Rank-five is434/420;
absolute case-size gap652 instead of690, with40/108 target-sized spans,
frame2C4 and19 sentinels. Root generic integer calls103->105 versus target106;
the remaining physical-call deficit is specifically1B4, target3/candidate2.
Native-to-native case comparison binds internal destinations explicitly and
finds101/108 old/new spans equal, leaving seven changed or unbound. Two copied
byte/destination controls and a stale-field-cache control reject false equality.
Run7020/all189 retains27 ordinary differences. No new exact or denominator
credit. See [rank-count evidence](evidence/enemy-rank-count-owner/README.md).
The canonical `build/match-linked/AnmReleaseSlot` carrier has this newer proof's
identities. Difficulty-count host and conditional-call controls recover its
third call but regress rank-five to nine calls/four returns and are restored.
Generic macro/prologue views are neutral; biased pattern-pointer, direct-rank
and boolean controls are rejected. All five adapter bodies are unchanged.
The eight-TU carrier still anchored AnmInt3View::operator* despite the maintained
AnmInt3Scale.cpp body. The separate RGB recipe uses /GL /GS /Ob1 for that support. Adding the real known body in a separate declared dispatcher
diagnostic graph was subsequently tested in the real dependency proof above.
It establishes the declared bounded context, without production closure.
All100 standard CI checks pass. Cleanup reclaims9148041 bytes from nine
owned superseded native directories and eight cloned analysis files.

The preceding source checkpoint closes five Enemy operand adapters at412A00/16,
412A60/14,412A70/14,412A80/17 and412AA0/26:87 bytes total. Viewing the loaded
full owner as EclVmHost before reading activeContext recovers the target
intermediate loads, private registers and scheduling. Both target-proven
dereferences remain; runtime itself is not the host. Two independent full
eight-TU colds pass19 complete owners1539 bytes/all32 fields, with all29 source
pins equal. Five origins remain indeterminate, so authored exact stays375/68104.
The canonical carrier shares `build/match-linked/AnmReleaseSlot`; older carrier
identities are historical. Source/API and /GL /GS profiles are retained.
Dispatcher is now14404/14416,40/108 same-sized cases, absolute case-size gap690,
frame2C4 and19 sentinel comparisons. Polar399/405 and other differences remain.
Run7020 retains27 ordinary differences/all189 fields in this actual eight-TU
graph; the separate two-TU26 measurement remains distinct. ANM source is unchanged
and its bytes are not replayed in this bounded checkpoint. See
[operand host-view proof](evidence/enemy-operand-host-view/README.md).
Separate-TU and definition-order integer-adapter controls are neutral and
restored; the direct raw-float expression has the wrong ECX receiver.
All three original large owners remain open. The direct integer-read macro
was subsequently tested in the rank-count proof above and was byte-neutral.
All100 standard CI checks pass. Inspected cleanup reclaims8639051 bytes from
seven superseded native directories and29 intermediate analysis files.

The latest diagnostic checkpoint repairs Run table coverage when Capstone stops
inside the jump table instead of near the end of the selector bytes. Complete
PE fields plus explicit unlinked suffix coverage still compare every owned byte;
16 synthetic tests and three copied-image controls guard rejection behavior.
A fresh full two-TU /GL cold confirms Run7020/all189 fields with26 ordinary
differences. Fifteen same-image existing guards retain1587 bytes/all21 fields.
All source/type/API controls are restored. EnemySpawn cold-replays577/all12;
all100 standard CI checks pass. Cleanup removes2,592,154 bytes from10 owned
obsolete directories. StartSubroutine is currently551/550;
the older522 diagnostic is stale. No new exact credit. See
[current ECL evidence](evidence/ecl-current-dependencies/README.md).

The latest source checkpoint reconstructs ASCII camera/viewport effects where
an earlier interface had only a declaration. Both draw owners now capture the
viewport index before flush, publish/configure the camera, reload active/device
state, set the viewport and then write the configured flag. Native offsets are
asserted partial views; allocated storage aliases remain open. Newline paths
now skip spacing, and each regular glyph marks dirty bit8 after copying
dimensions. Real StageDraw support gives regular726/738 and GUI572/570
bytes; high callback7/10 remains nonexact. Eighteen same-image complete guards
pass6596 bytes/all266 fields; two existing canonical units cold-replay337 bytes.
Host viewport checks pass72 scenarios/eight strict mutants; glyph checks pass198/three,
all with sanitizers. No new exact credit. All99 standard CI checks pass; cleanup removes9,058,987
bytes from10 owned superseded native directories, in addition to the explicitly
cancelled earlier CI fixture. See
[viewport source evidence](evidence/ascii-viewport-transitions/README.md).
Original large-owner work remains active; integer signatures, FORMAT state,
and two FLIP_X controls from this round are restored. The new two-TU ECL baseline supersedes its old carrier; the eight-TU Enemy
carrier has since been cold-rechecked in the operand host-view proof above.

The preceding checkpoint changes ReadFloatValue to ordinary explicit-context
static source, updating its three real callers. Two independent full two-TU
cold builds preserve Run7020/all189 fields and reduce ordinary differences
28->26. The existing171-byte callee retains canonical exactness in two further
single-TU colds; all16 existing ECL units retain1617 bytes. The actual eight-TU
Enemy graph reduces Run29->27 and retains identical complete dispatcher14524
and raw-float forwarder26 hashes. No new exact credit is claimed. The original
static declaration and production TU remain unknown. See
[float call order](evidence/ecl-float-value-call-order/README.md). All97 standard CI
checks pass. Cleanup removes3,376,996 bytes from five owned superseded outputs.

The preceding checkpoint closes HintLinkView::Append41AB10/52, formerly
maintained as free HintAppendLink. Ordinary member source recovers the complete
raw-equal EAX-head/EDX-node body, including ESI and loop alignment; cdecl versus
fastcall on the free source was machine-code neutral. Two full four-TU colds
replay seven complete owners1038 bytes/all36 fields, with all18 input pins and
seven owner hashes equal. Original class/name/TU remain unknown. See
[list member proof](evidence/hint-list-member/README.md). The current carrier
is still `build/match-linked/HintReadLine`, now pinned by this newer proof.
Loader1533/1498 and constructor51/nonexact remain open. An aggregate link-init
control grows the constructor to57 and is restored.

The preceding checkpoint closes HintReadLine419F40/260 through the real
four-TU Hint/Main/FileSystem /GL /GS graph. Ordinary stdcall source recovers
both RET12 exits. Two full colds replay six complete owners986 bytes/all36
fields, including the existing OpenFile360/all26 guard. All18 source pins and
six owner hashes agree. The host parser suite explicitly models x87 evaluation
and still kills22 strict mutants; the original game control word/runtime remain
unknown. Packed BGRA storage reduces the nonexact constructor72->51 bytes;
float-expression source and the dependency context reduce LoadHintFile to1533
versus1498. See [line-reader proof](evidence/hint-read-line/README.md).
The current carrier is `build/match-linked/HintReadLine`; prior HintLoad pins
are historical. The constructor's origin stays indeterminate with no credit.

The preceding checkpoint closes HintSplitLine41A050/75. An explicit
colonless early return recovers target branch layout. Two full cold builds
replay the complete static owner and all4 fields, plus three existing guards
291 bytes/all2 fields. Explicit static tail JMP binding additionally requires
the whole raw-equal trim callee and maintained target source/name mapping;
the original static CALL default remains unchanged. See
[split proof](evidence/hint-split-tail/README.md). The canonical HintLoad
carrier and14 source pins in this proof supersede the prior helper carrier.
The ECL two-float value-object control grows Run7020->7024/frame108->110;
it changes the second spill to10C instead of target14 and is restored.

The preceding checkpoint closes HintTrimWhitespace41A0A0/114 and
HintLookupValue4198C0/98 through the real LoadHintFile two-TU graph. Two
independent cold builds reproduce both complete static owners raw-equal and
retain ClearHintLists79/all2 fields. Natural stdcall lookup source recovers
the target callee cleanup; whitespace source already matched. Host parser
checks pass with22 strict mutants. See [helper proof](evidence/hint-small-helpers/README.md).
Seven restored negative large-owner build directories were pruned (5,399,291
bytes); their source controls and reports remain. The three large owners
remain nonexact.

The preceding checkpoint closes RGB Evaluate441600/820. Two independent
five-TU /GL /GS canonical colds reproduce all17 target fields. Only the new
`src/AnmInt3Scale.cpp` support TU uses /Ob1; its migrated56-byte operator
retains existing credit. Twelve same-image existing guards retain4579 bytes
and183 fields. Each image therefore passes14 complete owners5455 bytes/all203
fields. Actual-source host checks pass13 scenarios,9 strict mutants and
ASan/UBSan/leaks. See [RGB proof](evidence/anm-rgb-interpolation/README.md).
Earlier Float2/Float3 and alpha exact units remain intact in these guards.
Original physical source/TU/profile identity is still unknown. All97 standard
CI steps pass after the final source freeze; current raw inputs and native
artifact hashes remain unchanged. Cleanup removes about977 MB of verified
obsolete outputs; historical source/report evidence remains.

The whole Windows i386 product graph is open. Diagnostic anchor images are
not runnable products; host tests are not original/native-product execution.
Semantic reconstruction and portability have not started.

## Open large owners

These are scoped historical diagnostics, not exact claims. Rebuild the actual
graph after any dependency changes before comparing scores.

| Owner | Last measured context | Remaining work |
| --- | --- | --- |
| ANM ExecuteScript43EE30/9964 | Current two full nine-TU dispatcher colds with real scale support:9964 bytes,90 target-sized groups, absolute gap2, aligned7478/7995 | Nonexact. Old eight-TU anchor graph had7250/7995 despite same size. Unequal intervals61(+1),2(−1); registers, flag scheduling and common tail remain open. |
| ECL Run44E1A0/7020 | Current full two-TU cold afterccd47a6: strict all189 fields and6994/7020 bytes;15 same-image guards1587/all21 | Exactly26 ordinary differences remain. Eight-TU Enemy context has27, including an additional SIB byte. Static float-value adapter repairs FORMAT register setup while retaining171-byte canonical callee; no new exact credit. [Current evidence](evidence/ecl-float-value-call-order/README.md). |
| Enemy DispatchEclInstruction40E770/14416 | Current two full nine-TU colds with real scale support:14468 bytes, rank-five434/420, frame2C4,181 selectors and108 groups; case-size gap652 | Generic integer calls105/106; missing physical call in1B4. Two final rank owner reloads recovered; lower three, instruction lifetime, storage, register scheduling and stack homes remain open. No parent or case exact credit. |

Old rejected controls are scoped observations, not proof that an API or source
shape is correct. Require a concrete target hypothesis and compare corresponding
instructions; an unchanged unaligned score does not establish code neutrality.
Gain new TH10 caller, lifetime, partition or profile evidence. Keep target
observations, cross-game hypotheses, compiler observations and unknowns distinct.

## Reproduction and retained local state

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-evaluate-rgb-interpolation --unit anm-int3-scale --json
scripts/repo-python scripts/test-anm-rgb-interpolation.py
scripts/repo-python scripts/report-anm-execute-table.py --check
```

The latest operator instruction explicitly requests only necessary affected
replays, serialized resource-conscious builds and periodic artifact cleanup.
Replay only the necessary affected units; no full historical replay was run
for this checkpoint. Existing guard recipes retain their original contexts;
the RGB pack records explicit additional same-image guard overrides. The
scale recipe alone was migrated to the five-TU graph.

The retained canonical RGB carrier is `build/match-linked/AnmRgbInterpolation`.
Its source pins are historical after the ASCII changes; the current diagnostic
carrier and focused guards are in the newer ASCII proof.
Verify all12 raw project-input hashes and four artifact hashes against
`docs/evidence/anm-rgb-interpolation/native-summary.json` cold2 before reuse.
Older ANM carrier/source hashes are historical. The replaced
`AnmSetupVertexBuffer` carrier and nine discovery build directories are pruned.
Cold1 output was superseded by cold2; both complete reports and identities
remain in the private pack. Any rebuild replaces this cache and its pins.

Private review/recovery material is under
`.analysis/gpt-6.1-sol/th10-anm-rgb-value-graph-20261020/`: final reports,
source pins, target/Ghidra review, compact controls and verified source deltas.
Read its `RECOVERY.md` before using old runners: they can restore the old
starting source in `finally`; do not rerun them over accepted work.

The issue reference remains an ignored shallow sparse checkout at
`_reference/th10-decomp`, levzzz5154/th10-decomp,
commit `c6791d82dc9f02e55eca45a5314f900a85481efd` (MIT).
Adjacent TH08/TH09/TH095 and reference source are hypotheses only.

## Checkpoint completion

Latest focused native results are in the checksum proof; RGB results retain
their historical scope. Run the standard gates
before the next commit; regenerate progress after ledger changes:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Clean only inspected, attributable artifacts after retaining reproducible
evidence. Preserve unknown work, D3D evidence, private target, databases,
toolchains and reference checkout. Final validation and cleanup totals are in
[the current validation summary](evidence/main-executable-checksum/validation-summary.json).
