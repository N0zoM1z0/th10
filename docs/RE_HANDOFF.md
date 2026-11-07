# TH10 reconstruction — next-agent continuation brief

**Updated 2026-11-07.** This is the **current operational handoff**, not a
chronological trial log. Check live `git status`, `git log`, source hashes and
the generated [progress ledger](PROGRESS.md) before acting. Historical
source-epoch notes belong in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) and
`docs/evidence/`; a recorded image/size from an old test does **not** certify
the current source. Older versions of this handoff remain in Git history.

## Start on the real worktree

```sh
git status --short --branch && git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/ci.py
```

- Only ignored/read-only `resources/th10.exe`, Japanese TH10 v1.00a,
  **487,936 bytes**, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
  is the original target. Keep `AGENTS.md` rules, pinned VC7.1 SP1 and
  actual LTCG `/GL` profiles. **Never push.**
- Every dirty change is part of this worktree: inspect and continue it;
  never reset it simply because an earlier note says the tree was clean.
- Make substantive source fixes in both large interpreters and useful leaves.
  Commits are `gpt-web: ...`, with repo author and co-author trailer.
- Accept **only complete, independently original-bound exact** code, all
  fields and branch/table targets, from a reproducible *current-source* cold
  build. Matching size, 100% normalized candidate-vs-candidate code and
  repeated guard replays confer **zero new exact credit**. Build only affected
  units using explicit `--unit` selectors. **Never cold replay all units.**

## Resume at this checkpoint

**Newest accepted full owner:** `TransitionFadeIn @0x0043C230`, **133/133**
with **5/5 original-bound fields** in two independent current-source 14-TU
/GL colds. Natural `(flags >> 2 | flags) & 1` restores the retail SHR/OR/TEST
pause-bit schedule and full length from the prior 124-byte candidate; same
selected artifact keeps Init396 and Delete26 exact (555/555). This is a new
confirmed-authored owner. [Full native proof](evidence/transition-fade-in-133-exact-20261107/README.md).

**Earlier accepted large owner:** `GameFinalizeStatistics42B1E0 @0x0042B1E0`
**564/564** with all **24 original-bound fields**. The installed
`src/GameStatistics.cpp` was verified in **two independent five-source
cold builds** before its caller changed. The **current six-source** actual
constructor graph (`GameStatisticsLoad.cpp`, `GameStatistics.cpp`,
`GameStatisticsRecordDefaults.cpp`, `FileSystem.cpp`, `Decompress.cpp`,
`RandomMath.cpp`) then passed selected canonical original-bound
finalizer **564/564**, loader **423/423**, tail initializer **142/142**
(**1,129/1,129**); same-image statistics destructor **45/45** remains exact.
The two earlier five-source PE receipts are real evidence but are **not**
the same source/build-graph epoch as the current six-source canonical image.
[Full finalizer proof](evidence/game-statistics-finalize-exact-20261107/README.md)
and [current caller proof](evidence/statistics-record-real-caller-20261107/README.md).

**Current loader word-shift correction:** `Load42B030` now uses the
native modulo-32-bit `SHL 2` on the header's uncompressed-size word.
The selected six-source cold keeps loader423, finalizer564 and tail142 exact
(1,129/1,129); this adds no new exact credit.
[Source and proof](evidence/game-statistics-alloc-word-wrap-20261107/README.md).

**Immediate open statistics work:** the constructor still emits **137 bytes
vs original 132**, factory **98 vs 97**, and record defaults **274 vs 274
but NONEXACT**. `GameStatisticsOwner` originally receives owner in **EAX**
and returns `RET`; current constructor uses a stack owner/`RET 4`. An ordinary
constructor declaration annotated `__fastcall` or `__cdecl` was byte-neutral;
this VC7.1 compiler rejects explicit `__thiscall` on constructors. Do not
force a fake ABI merely to match. The record source's natural 8+1 byte name
copy recovered **7/8 original global field positions**, but its second
name-word operand is still `+0x50` vs original `+0x53`; its 75→36 ordinary
difference result uses **one fixed diagnostic mask**, not canonical equality.
The real `__stdcall` record callee is now linked instead of a stand-in.
[Record copy proof](evidence/statistics-record-name-copy-schedule-20261107/README.md).

**Current transition-envelope 349-byte boss frontier:** installed
`TransitionShakeEnvelope` first recovered the target `SHR/OR/TEST` pause-word
and x87 amplitude stack home, then named the second-stage unsigned boundary
for reuse in both comparisons. The source-present candidate progresses from
297 → 352 → **350 bytes**, against original **349**, with **16/16 actual
code-reference identities and 13/16 original positions**. Register allocation,
Y-store encoding and one-byte extent gap remain NONEXACT. A fresh selected
14-TU original-bound cold protects **689/689** previously exact
Init396/FadeIn133/Delete26/DrawScreen134 bytes; zero new exact credit.
[Latest code and proof](evidence/transition-envelope-shared-second-limit-20261107/README.md);
[earlier recovery](evidence/transition-envelope-pause-x87-20261107/README.md).

**Current Enemy ECL pattern/spawn word correction:** the giant
`DispatchEclInstruction` now also evaluates five inline script-name count
advances, nine byte-stride products/addresses, and four rank-bias expressions
through 32-bit unsigned words, retaining native signed consumers. Four
separate real 11-TU source variants plus the installed formatted source
preserve complete **14,352-byte/716-candidate-field** geometry and four
additional owner bodies byte-identically; selected original-bound ECL
argument leaves remain **87/87** exact. The target is still **14,416 bytes**;
this is **NONEXACT and zero new credit**.
[Compiler experiments and field evidence](evidence/enemy-ecl-spawn-pattern-index-words-20261107/README.md).

**Current Enemy ECL native pointer-word correction:** both shared
pattern/overlapping-extra record addressing helpers now add their typed
record offset as unsigned 32-bit i386 words, eliminating signed C++
overflow at the final address formation. A fresh actual 11-TU graph
keeps the full dispatcher **14,352/716 candidate-field geometry** and four
adjacent native owners byte-identical to its prior source. Selected real
argument leaves remain **87/87 original-bound exact**. This adds **zero**
new canonical credit; dispatcher target remains 14,416 bytes.
[Source and native evidence](evidence/enemy-ecl-pattern-final-address-word-20261107/README.md).

**Current ANM JUMP_DEC native word correction:** `ExecuteScript` now
performs its script-variable predecrement through the unsigned 32-bit
lvalue, matching the native low-word wrap at `INT_MIN` without signed-C++
overflow UB. The full owner stays **9,962/9,964** with **350/350** bound
original fields; four selected exact variable-access guards pass **856/856**.
No new exact credit. [Source and whole-owner proof](evidence/anm-jump-dec-word-wrap-20261107/README.md).

**Current ANM frame timer native word correction:** the script-timer
fast branch increments `timer.current` through its unsigned 32-bit field
view; together with the already committed JUMP_DEC word decrement this
removes C++ signed overflow UB while retaining the full **9,962/9,964**
original-bound executor (350 fields, only opcode-91 EDX/ECX mismatch).
Six selected current-source exact timer/operand helpers pass **964/964**
in two native cold artifacts, no new exact credit.
[Full owner and selected guard](evidence/anm-timer-tick-word-wrap-20261107/README.md).

**Current ECL binary-search word correction:** the real 136-byte
`EclVmScriptDatabase::FindSubroutine` now spells its upper bound, signed
midpoint delta and bound updates as native unsigned 32-bit words with
signed comparison/division semantics. Five actual source probes preserve the
**complete 136/136 exact owner** and surrounding Run7020/189-field graph;
selected current-source original-bound cold keeps Find136, ReadInt144 and
ReadFloat183 at **463/463**. No additional exact credit.
[Complete source/whole-owner evidence](evidence/ecl-find-subroutine-native-word-20261107/README.md).

**Current ECL CALL stack/metadata cursor words:**
`EclVmContext::StartSubroutine` now computes its serialized argument offset,
stack argument offset and per-argument descriptor advance through unsigned
32-bit low-word arithmetic, retaining the original signed consumers.
Five actual source graph variants keep the full **551-byte candidate** (550
target) and Run7020/189 geometry byte-identical. Current-source selected
one-TU and ECL/Enemy exact guards pass **609/609**, no new exact credit.
[Source and complete-owner trial](evidence/ecl-start-subroutine-word-offset-20261107/README.md).

**Current memory-file Seek remaining-word correction:** FILE_CURRENT
now computes its signed available-byte count through Win32 unsigned 32-bit
address subtraction, matching native arithmetic when a previous allowed
negative seek places `m_Current` before the C++ array boundary. Nine direct
normal-COFF source variants were compiled; the installed complete 107-byte
function keeps the prior **100/107** original comparison with zero external
fields. Ten affected accepted memory/resource owners selected-cold-pass
**491/491** original bytes; no new exact credit.
[Complete native proof](evidence/resfile-seek-native-remaining-word-20261107/README.md).

**Current bullet transform timer native increments:** all seven
per-frame `state.timer.current` fast-path increments now use an unsigned
32-bit word view, matching i386 low-word wrap rather than C++ signed UB.
Three real current-source seven-TU variants preserve complete code/field
geometry of seven transform owners. The original absolute-direction boss
remains **333/335** with all eight original-bound fields; selected current-
source deceleration/horizontal/vertical owners cold-pass **484/484**.
**Zero new exact credit.**
[Source and complete-native evidence](evidence/bullet-transform-timer-word-wrap-20261107/README.md).

**Current bullet manager main-loop state timer:** the **249-byte
original-bound exact** `BulletManagerView::UpdateBullets` now increments
each surviving bullet's signed `stateTimer.current` through its actual
unsigned 32-bit i386 field word. Its updated full native main-loop body,
its 40-byte callback, the 158-byte Deactivate and 109-byte bounds helper
all remain **556/556** previously exact original bytes in two selected
current-source artifacts. No new exact credit; it is separate from the seven
transform timer words already committed.
[Complete main-loop proof](evidence/bullet-manager-state-timer-word-wrap-20261107/README.md).

**Current ECL native script-jump addresses:** the variable-length
`EclVmInstruction::AtByteOffset` uses low 32-bit unsigned address words
before publishing its pointer. This keeps original negative relative JUMP
byte displacements without C++ out-of-array pointer addition. Four actual
native 2-TU forms preserve the entire **Run 6,997/7,020** / **189/189**
original field frontier and float callee exact171. Installed-source selected
one- and two-TU original-bound exact guards pass **1,037/1,037** across
8 owners; **zero new exact credit**.
[Full proof](evidence/ecl-native-script-jump-address-20261107/README.md).

**Current ECL stack-frame word arithmetic:** `EnterFrame` and
`LeaveFrame` now use unsigned 32-bit intermediate words for signed frame-top
addition, saved-slot bounds and four-byte restore decrement. Seven actual
single-TU source variations keep Enter64, Leave41, Push96, Pop122
**original-bound exact**, with Run's complete same-graph normalized code
unchanged. Installed-source selected six-unit original-bound cold is
**605/605**; the separate two-TU interpreter remains **6,997/7,020**,
189 fields, NONEXACT. Zero new exact credit.
[Real native proof](evidence/ecl-stack-frame-native-word-20261107/README.md).

**Current ECL stack Push native words:** the stack data-size signed
bound check, four-byte tag-slot advance and final top advance now use
i386 low-32-bit unsigned intermediates. Nine native source probes show
all four stack leaf owners remain exact; Push-only forms leave the whole
same-graph Run unchanged, while Pop variants *regress* 7,020-byte Run and
were explicitly rejected. Installed-source selected seven exact owners
pass **776/776**. The separate two-TU Run stays **6,997/7,020** with all
189 target fields. **No new exact credit**.
[Native and rejected-variant evidence](evidence/ecl-stack-push-native-word-20261107/README.md).

**Current ECL FORMAT native-word cursor correction:** the complete
7KB ECL `Run` interpreter's counted format-text path now uses i386
unsigned 32-bit words for eight-byte argument record strides, flag-index
+1u, percent-minus-cursor byte displacement and next-token cursor +2u,
retaining signed inline-count division. **31** true two-TU source variants
show why global unsigned scalar indexing repairs two bytes but introduces
three new wrong bytes, and why individually neutral changes can regress in
combination. The installed selected combination preserves the **complete
6,997/7,020** Run with all **189** target fields, and **1,037/1,037**
previously exact bytes in an eight-unit selected native cold. No new exact
credit. [Source/trial evidence](evidence/ecl-format-native-word-cursors-20261107/README.md).

**Current ECL CALL pointer-word addresses:** the 551-byte candidate
`StartSubroutine` now uses unsigned i386 address words for the destination
argument pointer, +4 cursor advances, three typed metadata reads and the
saved caller frame slot. Seven genuine two-TU C++ trials preserve the entire
551-byte/four-field candidate and **6,997/7,020**, **189/189** original-bound
Run boss. Installed-source eight selected exact leaves/host owners cold-pass
**1,081/1,081**; original StartSubroutine is still **550 bytes, NONEXACT**.
**Zero new exact credit.**
[Source, trials and field proof](evidence/ecl-start-subroutine-native-argument-address-20261107/README.md).

**Current SCPT script loader address words:** the 589-byte nonexact
`EclVmScriptDatabase::AddScriptData` now forms file-header, counted offset,
subroutine header, typed offset cursor, name and include-package addresses
using original low-32-bit i386 word additions. Eight actual two-TU source
controls preserve full **589 bytes**, both original CRT field positions
+6C/+101 and **502/581** comparable original bytes, while the related
`Run` remains **6,997/7,020**, all 189 original fields. Installed-source
six selected exact owners original-bound cold-pass **904/904**. No new
canonical exact credit.
[Full source and real native proof](evidence/ecl-serialized-script-native-address-20261107/README.md).

**Current ANM variable-stream address words:** `AnmRawInstructionView`
now computes both encoded `Next()` and signed relative-jump byte addresses
as low 32-bit Win32 words, preserving native negative branch displacements.
Four genuine fresh VC7.1 SP1 five-TU native control builds plus installed
final-source cold preserve the **complete 9,962/9,964** `ExecuteScript`
with **350/350** original target fields. Seventeen additional opcode-91
register-lifetime variants still do not solve the two ECX/EDX bytes.
Selected seven exact helpers cold-pass **1,037/1,037**. No new exact credit.
[Complete current-source proof](evidence/anm-variable-stream-native-address-20261107/README.md).

**Current ECL indirect stack scalar reads:** the `ReadIntValue`,
`ReadInt`, `ReadFloatValue`, and `ReadFloat` leaf owners now form signed
frame-base + script-operand addresses as native low-32-bit unsigned dword
additions before their original scalar reads; the two Resolve* lvalue
owners are not yet changed. Eleven real single-TU source controls retain
all six original exact helper bodies and the full same-graph Run. Final
installed two-TU **Run 6,997/7,020** with all **189** original fields,
and selected eight exact readers/host owners **1,061/1,061** pass.
No new codegen credit.
[Full native source proof](evidence/ecl-indirect-read-native-stack-words-20261107/README.md).

**Current ECL indirect lvalue word addresses:** the two
`ResolveInt @0x00450030` and `ResolveFloat @0x00450070` leaf functions
now form the actual stack/frame/script-operand address in native unsigned
32-bit i386 words, preserving their positive-operand tests and real host
fallbacks. Two independent compiler variant paths retained strict
**58/58** and **93/93** original leaf codegen, and their installed source
combines with the prior four reader changes without regressing the large
**Run 6,997/7,020 / 189 fields**. Selected nine exact units cold-pass
**1,197/1,197** original bytes, **zero new exact credit**.
[Source and native proof](evidence/ecl-indirect-resolve-native-stack-words-20261107/README.md).

**Current ECL tagged -1 integer pop:** `ReadInt @0x0044FDB0`
now computes its signed stack-top minus-four guard, two four-byte pops and
two payload/tag addresses via native unsigned i386 low words. Six real
one-TU full-owner source trials keep all six scalar getter/resolver leaves
original-bound exact and the entire same-graph Run byte-neutral.
Installed-source selected ten exact owners cold-pass **1,319/1,319**;
separate two-TU **Run 6,997/7,020** with **189** original fields remains
NONEXACT. **No new exact codegen credit.**
[Current-source native proof](evidence/ecl-readint-tagged-pop-native-words-20261107/README.md).

**Current enemy combat life low-word subtraction:** the real
`EnemyRuntimeUpdate` now expresses its target-confirmed
`SUB [enemy+0x13C0],ECX` as unsigned 32-bit subtraction while retaining
signed life comparisons. Five actual current two-TU source trials preserve
the complete **2,371-byte/69-field nonexact** Enemy update, four adjacent
whole owners, and independent ECL `Run 6,997/7,020` with **189** original
fields. Installed-source four selected exact owners cold-pass **404/404**;
original Enemy update is 2,412 bytes, still NONEXACT and zero new credit.
[Source and full-native proof](evidence/enemy-runtime-life-native-word-20261107/README.md).

**Current Enemy flash timer native decrement:** original
`EnemyRuntimeUpdate +0x8B8` uses `DEC DWORD [EBP+0x1414]` for
`damageFlashFrames`; the maintained source now decrements its corresponding
unsigned 32-bit word, preserving original zero tests and flash behavior.
The complete two-TU **2,371/69 candidate** Enemy update is byte/field
identical to the pinned baseline and still NONEXACT vs original 2,412;
original-bound `Run` remains **6,997/7,020** and 189 fields. Installed
source selected five exact owners cold-pass **570/570**; zero new exact
codegen credit. [Native evidence](evidence/enemy-runtime-flash-native-word-20261107/README.md).

**Current Enemy animation script index dword:** the original
`EnemyRuntimeUpdate +0x6E5 ADD EDX,ECX` now has explicit low-32-bit
unsigned source operands for `animationBaseScript + scriptOffset`, after
the separately committed native life `SUB` and flash `DEC`. The full
nonexact Enemy candidate remains **2,371 bytes/69 fields** and the
independently original-bound ECL `Run` remains **6,997/7,020** with all
189 target bindings. Installed-source five selected exact owners cold-pass
**570/570**, zero new canonical codegen credit.
[Original instruction and native proof](evidence/enemy-runtime-animation-script-word-20261107/README.md).

**Current ECL Stack Push native destination words:** the 96-byte exact
`Push` leaf now forms the existing byte type slot and value data copy
destinations as native i386 low-32-bit stack-base+top addresses. Nine
original-compiler whole-leaf source probes preserve Push96, Pop122,
EnterFrame64, LeaveFrame41 and complete same-graph Run; installed-source
selected six-unit original cold **593/593**. Separate two-TU Run remains
**6,997/7,020** with all **189** independently original-bound fields,
NONEXACT, zero new canonical credit.
[Full native proof](evidence/ecl-stack-push-native-word-address-20261107/README.md).

**Other large active frontiers:** ANM `ExecuteScript` **9,962/9,964** with
all 350 original-bound fields, ECL `Run` **6,997/7,020** with all 189,
and Enemy ECL dispatcher **14,352 candidate bytes vs 14,416 target**
(its 716 fields are *candidate* inventory, not fully original-bound).
Try a new concrete compiler/source lead, not the already-failed aliases,
volatile permutations or lifetime reorderings. Eight genuine two-TU
`StartSubroutine` cursor/increment source variations failed to improve the
**550 target / 551 candidate**, while `Run` remained 6,997/7,020 and its
189 fields unchanged. Bullet direction and score/faith/timer integer-word
semantics were improved without claiming unearned new whole-owner exactness.

Previously accepted Replay Save1681, Ending Run1568 and ANM capture162 are
already credited; their replays are regression guards, not new exact code.
The neighboring TH10 decomp reference pinned at `d1f3121` is a hypothesis
source, never a local codegen acceptance Oracle. Preserve existing ignored
witnesses and research drivers; inspect source hashes before reusing them.
**Do not run the full historical cold replay.** Replay only explicitly
changed/affected units. For storage transfer, see [RE_ARTIFACTS.md](RE_ARTIFACTS.md).

## Acceptance ledger — generated truth, not a runnable-game claim

As of this source checkpoint, [PROGRESS.md](PROGRESS.md) reports **1,829
reviewed function boundaries**, **1,309 source-present mappings**,
**1,498 complete exact functions / 195,967 original bytes**, including
**588 confirmed-authored exact / 125,851 bytes**. There are **162 authored
source-present nonexact** functions. Original author/source provenance is
not interchangeable with a reconstructed function's exact codegen. Product
build, actual game execution, semantic dual-Oracle and portability are
separate later work, **not established** by these figures.

Recent *accepted* full owners (replay only one affected cohort when needed):

| Exact original owner | Strict acceptance | Evidence |
| --- | --- | --- |
| `TransitionFadeIn @0x0043C230` | **133/133**, five independently bound original source/global/CRT/timer fields, full control flow and real static PDB extent; two distinct 14-TU colds, Init396 and Delete26 guards exact | [FadeIn exact proof](evidence/transition-fade-in-133-exact-20261107/README.md) |
| `TransitionDelete @0x0043C870` | **26/26**, two complete original-bound calls, unique internal static PDB contribution; two independent 14-TU colds; initializer396 same-image guard exact; **authorship indeterminate** | [complete static callback](evidence/transition-delete-26-exact-20261107/README.md) |
| `GameFinalizeStatistics42B1E0 @0x0042B1E0` | **564/564**, 24/24 original-bound fields; two independent unchanged-finalizer five-TU colds and a newer **six-TU** current-source canonical 1129/1129 across finalizer564/loader423/tail142; same-image destructor45 exact | [complete finalizer](evidence/game-statistics-finalize-exact-20261107/README.md); [six-TU caller](evidence/statistics-record-real-caller-20261107/README.md) |
| `AnmRenderManagerView::RequestCaptureRectangle @0x00424480` | 162/162, six target-bound fields; two independent real FrontEnd/VmIdInterrupt/AnmManager three-TU colds; same-image FindVm73 and result close89 exact | [capture rectangle](evidence/anm-capture-rectangle-exact-20261106/README.md) |
| `EndingTextVmView::Run @0x0040BD80` | 1568/1568; 92/92 fields; 53 direct locals and all18 owned table words; independent 21-input cold; existing guards953/all24 pass | [ending execution](evidence/ending-run-1568-exact-20261106/README.md) |
| `ReplayManager::SaveReplay @0x00429B60` | 1681/1681; 83/83 fields; independent 21-input cold; same-image replay/file guards412/all33 and shared-artifact ReadIntOperand820 pass | [replay save](evidence/replay-save-1681-exact-20261106/README.md) |
| `EnemyLaserType1View::Draw @0x0041EA40` | 173/173; 10/10 fields; independent two-input cold; same-image angle94/renderer216 pass | [type-1 draw](evidence/laser-type1-draw-173-exact-20261106/README.md) |
| `BulletRuntimeView::UpdateVerticalWrap @0x00407E40` | 172/172; 8/8 fields; independent seven-input cold with horizontal148 guard | [vertical wrap](evidence/bullet-vertical-wrap-20261106/README.md) |
| `BulletRuntimeView::UpdateVectorAcceleration @0x00407560` | 305/305; 10/10 fields; final seven-input cold with horizontal148 guard | [vector acceleration](evidence/bullet-vector-acceleration-20261106/README.md) |
| `GameLoadStartupVersion @0x00420100` | 174/174 bytes; 15/15 fields; independent final six-TU cold, real PBG path entry | [startup version](evidence/startup-version-174-exact-20261106/README.md) |
| `AnmFloatRectView::DrawGradient @0x0043BFA0` | 501/501 bytes; 14/14 source/callee/global fields, real 14-TU context | [gradient](evidence/anm-gradient-rect-501-exact-20261106/README.md) |
| `BulletManagerView::SpawnBulletPattern @0x004073E0` | 194/194; 6/6 fields; real four-TU context | [bullet](evidence/bullet-spawn-pattern-194-exact-20261106/README.md) |
| `EnemyLaserManagerStorageView::AccumulatePointChecks @0x0041C880` | 54/54 *unmasked* bytes, zero external fields; **original authored provenance indeterminate** | [laser](evidence/enemy-laser-point-checks-54-exact-20261106/README.md) |
| `SoundPlayerView::GetFmtIndexByName @0x0043D2A0` | 227/227; four bound fields | [format-name](evidence/sound-getfmt-index-227-exact-20261105/README.md) |
| `SoundDataLoaderThread @0x0043D080` | 107/107; 11 bound fields | [sound loader](evidence/sound-loader-single-index-exact-20261105/README.md) |
| `MidiOutputView::StopPlayback @0x0043AE20` | 129/129; five bound fields; separate 30-TU selected destructor guards passed | [MIDI](evidence/midi-stop-playback-single-owner-20261105/README.md) |

The gradient **503-byte isolated** and bullet-pattern **195-byte isolated**
older candidates were superseded by exact *integrated* graph results. Laser
original authorship remains unresolved, so its 54 bytes do **not** count as
confirmed authored code.

## Current large and near-exact **nonexact** frontiers

| Target | Original vs current complete owner | What is known; next meaningful investigation |
| --- | --- | --- |
| `AnmRenderManagerView::ExecuteScript @0x0043EE30` | **9,964 / 9,964** | **9,962/9,964** original bytes, **350/350 independently original-bound fields**, 95 local targets. Opcode 91 registers at +0x113/+0x115 (original EDX, candidate ECX). Current source explicitly wraps three integer assignment and three in-place arithmetic ops; full original-bound 350-field owner stays 9962/9964 with only opcode91 register differences, and the selected four variable access guards remain 856/856 ([new proof](evidence/anm-inplace-word-wrap-20261106/README.md)). Six additional current-source natural child91 file/operand lifetime controls confirmed the same two-byte frontier (one regressed); all restored ([trial proof](evidence/anm-child91-lifetime-20261107/README.md)). Do not retry failed aggregate, volatile, member-order or layer-lifetime permutations without a new compiler lead. [Current-source proof](evidence/anm-integer-assignment-wrap-20261106/README.md). |
| `EclVmContext::Run @0x0044E1A0` | **7,020 / 7,020** | **6,997/7,020**, **189/189 bound fields**. Remaining 23 ordinary byte differences include register/stack homes, `LENGTH_SQUARED`, `POINT_ANGLE`, FORMAT and loop/callback paths. `STORE_INT`/`STORE_FLOAT`, CALL, JUMP and stack scalar work are installed; add/subtract/multiply/negate/post-decrement now explicitly preserve 32-bit wrap without new exact credit; SPAWN_THREAD_WITH_ID inline byte-count+4 now also wraps as an unsigned x86 word (complete 7,020/189 target-bound owner unchanged, selected exact441/441; [proof](evidence/ecl-spawn-index-word-wrap-20261107/README.md)) ([proof](evidence/ecl-integer-word-wrap-20261106/README.md)). [Current full-source proof](evidence/ecl-counted-offset-cursor-20261106/README.md). |
| `EnemyRuntimeView::DispatchEclInstruction @0x0040E770` | **14,416 / 14,352** | 64-byte length gap; **716 candidate** fields. Fresh 11-TU source/PDB codegen checks are mostly candidate-vs-candidate, **not** a full independently bound target acceptance. Typed spawn record, position, word clear and bullet call are installed. Three bullet-count and one integer rank interpolation operations now use native 32-bit wrapping; real 11-TU codegen/716 fields unchanged and five related exact guards87/87 pass ([new proof](evidence/enemy-ecl-rank-word-wrap-20261106/README.md)). [Caller record](evidence/enemy-pattern-bullet-descriptor-boundary-20261106/README.md). |
| `EnemyBeginSpell @0x00409280` | **2,432 / 2,432** | **108 candidate** fields but many ordinary code differences. Source bonus arithmetic now explicitly wraps modulo 2^32 before the signed cap. Same extent is not exact. [Bonus proof](evidence/enemy-begin-spell-bonus-wrap-20261106/README.md). |
| `EclVmScriptDatabase::AddScriptData @0x00450220` | **589 / 589** | **510/589**, **2/2 original-bound fields**. Count-reference growth and read-only loop view of the actual input-parameter cursor restore0x10 frame, old-table EBP, REP MOVSD+FB and both CRT call positions. First count/fill/merge-tail register scheduling remains nonexact; no identity selector. Both count growth and old-record subtraction now use native word-wrap source expressions; full 589/2 and Run7020/189 geometry unchanged ([new proof](evidence/ecl-count-word-wrap-20261106/README.md)). Early count/whole-cursor/force-inline controls are parked. [Current proof](evidence/ecl-counted-offset-cursor-20261106/README.md). |
| `EclVmContext::StartSubroutine` | **550 / 551** | Four caller call-field positions preserved. Eight further real two-source cursor/loop declaration-order variants did not improve the complete owner; all were restored and `Run` stayed 6997/7020, 189 fields. Do not retry identical proposals without a new lead. **NONEXACT**. [CALL proof](evidence/ecl-call-counted-inline-name-20261106/README.md). |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @0x004078E0` | **335 / 335** at Vector source epoch | Last complete measurement **333/335**, **8/8 independently original-bound fields**, at BulletManager.cpp `504af18f...` before the vertical-wrap change. EAX/EDX at +0xD7/+0xDD remain the nonexact frontier; the three direction-change counter increments now preserve native unsigned 32-bit wrap with 1,394/1,394 selected exact guard bytes and the absolute owner still 333/335 ([fresh proof](evidence/bullet-direction-counter-wrap-20261106/README.md)). [Pinned proof](evidence/bullet-vector-acceleration-20261106/README.md). |
| `SoundPlayerView::LoadSoundSample` / `PreloadBgm` | **684 / 684**; **369 / 369** | Respectively **678/684** and **365/369** ordinary bytes in suitable five-TU graphs; mainly call argument register order. Many simple declaration/alias and wrapper trials already failed. [Sound wire evidence](evidence/sound-riff-chunk-prefix-20261105/README.md). |
| `PollInputFrame @0x0044A5F0` | **850 / 850** | Split immutable published return from destructive repeat cursor; fresh baseline855 shrinks to850. **807/850 bytes**, **10/10 independently bound fields**. Remaining43 ordinary differences start +0x2DD in repeat/edge registers and scheduling. Related264-byte query cold passes. XOR/countdown/wider-mask/snapshot controls give no further improvement and are restored. [Current proof](evidence/input-frame-published-mask-20261106/README.md). |
| `StageResetPublishedCameras @0x004216F0` | **465 / 533** | Restored omitted secondary/published +0xE4 dword assignments, respectively 1/0; camera layout stays 0x118. Both fresh `/GL` and normal-COFF complete owners emit 533 bytes /40 fields; vector temporary reuse and global-store scheduling remain nonexact. [Word-store proof](evidence/stage-camera-default-words-20261106/README.md). |
| `ReplayManager::Initialize @0x00428F60` | **1,344 / 1,344** in real26/36-input contexts | Construction order, real shared state/ChainElem registration and current-stage header reload restored. Full74 candidate fields; strict rejection at the selected-stage DIR32 position (+3C5 vs original+3CB), with9 diagnostic ordinary differences from two reordered loads. Create34 gives1342 because loader path promotes from stack/RET4 to ECX/RET; adding real FileSystem/PbgArchive restores1344/RET4 in36 inputs but leaves identical9 differences. Repeated scalar/typed-view controls are byte-neutral and parked. Existing Create185 now uses two real configuration-default providers; selected Create34/Save21 colds pass with no new credit. [Current source and ABI proof](evidence/replay-initialize-constructors-20261106/README.md). |
| `EnemyLaserType1View::CheckCollisionCircle @0x0041EFA0` | **1,108 / 1,178** | Real virtual slot46DA10+1C now owns the body. Unordered radial acceptance, post-add sample Z clear and radius parameter home restored; typed vectors recover0x32C frame. Current22-input real context restores the independent registration118 call; factory still expands and full30-field extent rejects original29. Request-member factory control gives157/RET8 and is restored. Existing polar30/bounds91/registration118 pass239/all4; affected Update832 and Draw173 use their own selected cold recipes. [Current proof](evidence/laser-type1-circle-owner-20261106/README.md). |
| `EnemyLaserType1View::CheckCollisionBox @0x0041EB00` | **1,171 / 1,225** in current22-input circle context | Body unchanged since the earlier four-input1209/20-field proof, which belongs to pre-circle EnemyLaser.cpp408659cf. Current broader context emits1225/20 and remains nonexact; graph-dependent sizes are not source-only regressions. Actual slot18, unordered XY rejects, post-add Z clear and0x344 frame remain. Factory type-0 branch still inlines. [Prior four-input proof](evidence/laser-type1-box-owner-20261106/README.md); [current context](evidence/laser-type1-circle-owner-20261106/README.md). |
| `EnemyLaserType0StorageView::Bounce @0x0041CFD0` | **410 / 510** at pre-box source epoch | Measured at EnemyLaser.cpp `0eb005f5...`, before the box-owner change; Bounce body is unchanged. Actual type-0 vtable46DA60+3C and Update call41D45F bind this owner. Real generated virtual ownership restores ECX/RET; typed XYZ copies and ordered Y predicates are installed. One FireLaser path inlines; factory private arguments/registers remain nonexact. Original Update-entry canonical guard1001/all31 and native polar30/sound123 pass. Bounce-as-entry is rejected (same510; Update1009). Fresh stdcall and complete manager-member controls both give factory157/RET8 with stack request; restored/parked. [Current proof](evidence/laser-bounce-virtual-owner-20261106/README.md). |
| `GameStatisticsOwner::GameStatisticsOwner @0x0042AE60` / `CreateStatistics42AF20 @0x0042AF20` | **132 / 137**; **97 / 98** | **Both NONEXACT.** Original ctor EAX owner/RET; native candidate stack owner/RET4, even after resolving its actual record initializer. Explicit `__fastcall`/`__cdecl` constructor annotations are byte-neutral; VC7.1 disallows explicit `__thiscall` for this declaration. Six-TU current-source selected guards1129 and destructor45 exact. Revisit real caller ABI/context, not artificial register-forcing. [Real caller/guard proof](evidence/statistics-record-real-caller-20261107/README.md). |
| `InitializeGameStatisticsRecord @0x0042ACB0` | **274 / 274** | Earlier natural110-iteration rank-kind loop restored size274/8 fields but had only4/8 original field offsets. The current **split 8+1 name copy with extra word between** remains size274, validates all eight referenced original data identities and now aligns **7/8 original field positions**. Under the same 230-byte diagnostic ordinary comparison mask, differences drop **75→36**; name dword2 is still `+0x50` rather than original `+0x53`. **NONEXACT; zero credit**. Constructor now resolves its actual six-source `__stdcall` call, with selected exact1129 still passing ([real caller](evidence/statistics-record-real-caller-20261107/README.md)). [Current copy proof](evidence/statistics-record-name-copy-schedule-20261107/README.md); [prior loop](evidence/statistics-record-natural-loop-20261106/README.md). |
| `TransitionShakeLinear @0x0043C550` | **439 / 439** | Fresh existing14-input `/GL` graph: **412/439**, **20/20 original-bound fields**. `FIDIV` is at +0xB6 instead of target +0xCE. Named float-product controls (outer conversion; then explicit inner/outer conversions) both move it to +0xAA and displace two fields; rejected/restored. Initializer396/all37 guard passes from the restored-source baseline image. [Reference review and pins](evidence/upstream-th10-decomp-review-20261106/README.md#local-camera-experiment). |
| `TransitionShakeEnvelope @0x0043C710` | **349 original / 350 current candidate** | Original pause SHR/OR/test and x87 float stack home retained; the named shared unsigned second-stage boundary improves the full PDB extent 352→350 and aligns **13/16** independently decoded original field positions; original game-pointer +17 vs candidate +16, Y positive +12D vs +12E and Y zero +13B vs +13C. One-byte size and register schedule remain nonexact. Current selected original-bound cold protects Init396/FadeIn133/Delete26/DrawScreen134 **689/689**, **zero new credit**. [Latest proof](evidence/transition-envelope-shared-second-limit-20261107/README.md). |
| `TextRenderBufferView::InvertAlpha @0x00437160` | **157 / 157** | **Current source** two-TU `/GL /GS` measures **138/157**, zero fields; EAX/ECX receiver/count/selector choice accounts for19 differences. Separate pitch capture/multiply is byte-neutral in normal COFF112/157 and GL138/157; restored. Current installed signed-overflow-free text row and geometry words preserve all ordinary codegen; fresh GL/GS still measures138/157 and selected exact text owners1799/1799 pass ([native proof](evidence/text-native-word-geometry-20261106/README.md)). An *older different source/graph* had only `JS` vs `JL` at +0x69; **that one-byte near-exact result is not current**. Private proofs: `.analysis/gpt-web/20261106-text-current-exact/` and `.analysis/gpt-web/20261106-text-alpha-owner/`. |

Updated external reference: `_reference/th10-decomp-d1f3121` preserves the
November 1 head alongside the October 18 checkout. At review start its 16
promotions mapped to 15 open local entries. SaveReplay and Ending Run are now
accepted here; 13 remain open. Prioritize the three substantial laser
cutters and ECL count-reference model using our real callees; synthetic
caller/escape/stand-in contexts confer no local exact credit. See the
[review/crosswalk](evidence/upstream-th10-decomp-review-20261106/README.md).

**Current memory-file signed remaining word:** `CMemoryPbgFile::Seek`
now also computes `FILE_CURRENT` remaining bytes from unsigned Win32
`DWORD` address-word addition/subtraction before its original signed
comparison. Nine actual complete 107-byte source trials confirm the full
owner stays **100/107**, zero linked fields; ten selected current-source
exact file owners cold-pass **491/491**. No new exact codegen credit.
[Source and current cold proof](evidence/resfile-seek-remaining-native-word-20261107/README.md).

Other unaccepted leaves worth *new evidence-led* work: `CMemoryPbgFile::Seek`
107/107 but 100 matching ordinary bytes (its CURRENT/END cursor now uses
explicit native Win32 unsigned-word addresses, with all original behavior
and a **491/491** selected exact file-owner cold; see
[previous cursor evidence](evidence/resfile-seek-native-cursor-word-20261107/README.md),
and [current remaining-word proof](evidence/resfile-seek-native-remaining-word-20261107/README.md)); `AnmLoadedView::SetAndExecuteScriptIdx`
287/287 with about 22 instruction bytes differing; `EnemyEclHostBaseView::SetSubroutine`
30/30 with register/ABI disagreement; `EnemyFullObjectView` ctor target
673 vs native 672. The 20-TU `enemy-end-spell` replay can exceed execution
window: the last two attempts produced **no receipt**, so do not report it
as having passed during that particular experiment.

## Current-source proofs and candidate epoch pins

These SHA-256 values identify the *installed source bytes*, not historical
archive images. Recheck after any source change. Current-source/replay
receipts are ignored private files, not Git attachments.

| Installed source | SHA-256 |
| --- | --- |
| `src/GameStatistics.cpp` | `dc31f5c888d68d363571a74457ed3da000c2a36386e790cd9234b0798c229666` |
| `src/FrontEnd.cpp` | `0bab7e7448c0762a1410d627301652bfc8cfe22e7aaa88b76e979151cdaaf8c6` |
| `src/AnmManager.cpp` | `339c95eba4063aab4a62da3d496cb07de2c6240197f6957761392579f93b435f` |
| `src/AnmManager.hpp` | `7baea2b709d535639ab5a3d44a8bb1adb6eefd9c26b70f50a89fc248c5ce0bc3` |
| `src/EclVm.cpp` | `393f5c55cd14ad9d1eecbaba3ec9945e6954f05f672516350af2c001440046e3` |
| `src/EclVm.hpp` | `96764231991c72ff5eebb2d56884fbaafef6406d87d3a751cb53cbf96679d1fc` |
| `src/Ending.cpp` | `ed016273f068605e633f3faaff6a89556c953ec3a30b724ec52b8c54706a23ff` |
| `src/EnemyEclDispatcher.cpp` | `1823e96cd57454d3a630f9ae738674918d8c3414363f42691cbff753ec41e286` |
| `src/Enemy.cpp` | `5b9ca4dfcd672a8ca6cc7cabb276f1ca831594b0c816755d6ece8914677e8380` |
| `src/Enemy.hpp` | `2e2168b1d84e6dab64abb51c2d6dcdf0498c6f36856c9b1a25f18637ed35eea2` |
| `src/EnemyLaser.cpp` | `7d5f898f3fe4718ec4d44637b71883f300cd9493771ff2296538baf986adead7` |
| `src/EnemyLaserType1.hpp` | `dfd675f59897f541739888e99067a4306fcbe67b6773ec184b8baca90984facc` |
| `src/LaserType1.cpp` | `b7f759886f4897a9c1e2cd8fb99007f045e932d7a3826417ecd194f9006ead51` |
| `src/ItemManager.cpp` | `b029539ab91eba926525a1de3dcb6baaa05d3b6c53e181702b325b8a4ad32fcc` |
| `src/BulletManager.cpp` | `34cb5ceb0c88620a77e3aa9fcf8ab7f19754fc5bbf4bcf292ad844f6da35bba9` |
| `src/Sound.cpp` | `cb5cdabd1732dfbd93d6da73938df42dc541356fe7c34fd20a33950576b5b3b0` |
| `src/StageCameraDefaults.cpp` | `4ac0d9c1eeb14e1763f5ed642e50895e5081fdf2f41dcda819a6ca7ee7255fb8` |
| `src/StageScript.hpp` | `3a1a5402fdd969c150844102680d007f1080f6e38e44f429e70f0594b537bf5b` |
| `src/GameStartupVersion.cpp` | `125b80a073015e9d0fd2694b989a0983bf3f3777fb29634825818ced0f125b4f` |
| `src/GameStatisticsLoad.cpp` | `788036ed9a8881282db93f878bb2b899dd949b3c268bd45f5a7591e402650ce8` |
| `src/GameStatisticsRecordDefaults.cpp` | `2c0c4d290118b9a65a781fef030825b07b89dde090a24cbc1252c200cf3173df` |
| `src/InputPolling.cpp` | `f3bb9e5230f9db7e141e187b054e9bbd68d1282c7cd27f02723176f4415d641f` |
| `src/ReplayManager.cpp` | `7c6a50d945a9c83f694bda661880d9211b29b1970c99a8a4ddd4844dab6fcaf3` |
| `src/TextRenderer.cpp` | `2e95a09f2ea5f0aef8fe3b22f487fce776ea4ae5a918606f464ebf6a4e2c0cc4` |
| `src/TransitionEffect.cpp` | `711fb19a08a7bdfa45cd600a8f3bb82af5d2c90f3292d214e91e63e3625ebcd8` |

Preferred current native witnesses (only if the source pin matches):

- Ending Run exact: `build/match-linked/EndingTextRun/`,
  [complete source/image, original fields, table and guard pins](evidence/ending-run-1568-exact-20261106/native-result.json).
- Replay Save exact: `build/match-linked/EnemyReadIntOperand/` (shared real graph),
  [original acceptance](evidence/replay-save-1681-exact-20261106/native-result.json);
  [current Initialize-source selected cold pins](evidence/replay-initialize-constructors-20261106/native-result.json).
- Existing Replay Create exact: `build/match-linked/ReplayManagerCreateDefaults/`,
  current34-input recipe includes real configuration defaults and preserves185/all12/EH;
  Init remains nonexact. [Current source and context pins](evidence/replay-initialize-constructors-20261106/native-result.json).
- Type-1 Draw exact: `build/match-linked/LaserType1Draw/`,
  [current-header selected cold pins](evidence/laser-type1-circle-owner-20261106/native-result.json);
  [original source/callee proof](evidence/laser-type1-draw-173-exact-20261106/README.md).
- Type-1 circle/current box context:
  `build/gpt-web/20261106-laser-type1-circle/real-caller-context/`,
  [complete candidates, source pins and selected Update/Draw protection](evidence/laser-type1-circle-owner-20261106/native-result.json).
- Linear camera-shake candidate: `build/gpt-web/20261106-upstream-review/camera-baseline/`,
  [complete baseline, rejected controls and restored-source guard](evidence/upstream-th10-decomp-review-20261106/camera-experiments.json).
- Statistics-record candidate:
  `build/gpt-web/20261106-statistics-record/final-linked/`,
  [full original-bound diagnostic and source pins](evidence/statistics-record-natural-loop-20261106/native-result.json).
- Type-0 Bounce candidate and selected Update guard:
  `build/match-linked/LaserLineUpdate/`,
  [source/image and actual virtual-slot pins](evidence/laser-bounce-virtual-owner-20261106/native-result.json).
- Bullet vertical/horizontal wrap: canonical
  `build/match-linked/BulletHorizontalWrap/`, private
  `.analysis/gpt-web/20261106-bullet-vertical-wrap/canonical-replay.json`,
  [source/image pins](evidence/bullet-vertical-wrap-20261106/native-result.json).
- ANM five-TU whole executor:
  `build/gpt-web/20261106-handoff-anm-wrap/current/gptweb-proof.json`,
  `docs/evidence/anm-integer-assignment-wrap-20261106/README.md`.
- ECL counted-offset cursor589 and whole Run7020:
  `build/gpt-web/20261106-ecl-count-reference/final/`,
  [current source/image and complete comparisons](evidence/ecl-counted-offset-cursor-20261106/native-result.json).
- Earlier ECL two-TU `Run` source epoch (check its pins):
  `build/gpt-web/20261106-ecl-store-word/current/native-result.json`,
  `docs/evidence/ecl-stores-scalar-lvalues-20261106/README.md`.
- Enemy 11-TU dispatcher/BeginSpell:
  `build/gpt-web/20261106-enemy-bullet-spawn-alias/current/native-result.json`,
  `docs/evidence/enemy-pattern-bullet-descriptor-boundary-20261106/README.md`.

Targeted examples, **not** a request to run all units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-get-int-var --unit anm-get-int-var-ptr --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-int --unit ecl-vm-stack-pop --json
```

## Storage, evidence, and next-agent transfer

Consult [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the **2026-11-07**
expanded/archived storage snapshot and selective recovery. The
[2026-11-07 cleanup](evidence/agent-handoff-maintenance-20261107/README.md)
removed rejected *rebuildable* native variants (76,615,680 allocated bytes)
without creating a duplicate archive; its private file-hash manifest is
**not** a restorable backup. Keep the original EXE, `.tools/`,
`ghidra-project/`, `_reference/`, `.analysis/archive/`, current proof
scripts/receipts and **all `build/match-linked/` canonical artifacts**.
Deleted failed *regenerable* experiments are not missing Git source; re-run
their `.analysis/gpt-web/` driver, or restore a verified archived member
when needed. **Do not mix old PDB/MAP/OBJ with current source hashes.**
The ignored original, private toolchain, database and archives are **not in
Git**; transferring just the Git branch is insufficient for a new machine.

For documentation/metadata-only changes run `scripts/repo-python scripts/ci.py`
and `git diff --check`. No full historical cold replay is needed for
handoff documentation. Store new source results in small separate
`gpt-web:` commits, then update this concise table rather than appending
another dated mini-report. Do not claim whole-game link, actual runtime,
portability, or proof of original source author identity from exact bytes.
