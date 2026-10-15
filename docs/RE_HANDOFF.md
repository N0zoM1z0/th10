# TH10 reconstruction handoff

Updated 2026-10-14. This is a concise resume brief, not an experiment journal.
Keep live totals in the generated [`PROGRESS.md`](PROGRESS.md), durable target
facts and negative results in [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md), and
experiment chronology in Git history. Only explicitly dated, scoped
diagnostics belong in the checkpoint table; do not copy unpinned probe scores,
scratch paths, or build hashes into this file.

## Authority and recovery

- Target only the original Japanese TH10 v1.00a executable at the ignored
  `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Use `scripts/repo-python` for repository Python commands and keep target
  identity, boundary, origin, source presence, exact codegen, whole-product
  closure, runtime validation, semantics, and portability as separate claims.
- Work from the live worktree, including dirty changes. Inspect and continue,
  deliberately supersede, or reject existing work; do not create a clean side
  tree merely to avoid understanding it.
- Operator-requested commits use `gpt-web: ...`. Do not push from this worktree.
- Treat Ghidra, decompiler output, adjacent games, compiler diagnostics, and
  similarity scores as evidence or hypotheses according to their Oracle
  boundary. Only canonical replay can grant exact-codegen credit.
- Read current repository-wide counts from `docs/PROGRESS.md` or regenerate
  them. Do not reuse counts copied from an older commit or experiment.

## Validated large-owner checkpoints (not exact claims)

These are **dated, compiler-context-specific diagnostics**, not live exact
ledger entries. Rebuild each probe after changing a supporting translation unit.

| Owner | Last checked source context | Observed candidate / target | Open work |
| --- | --- | --- | --- |
| ECL `EclVmContext::Run` | `7d77b95`, EclVmStack linked /GL Run probe | 7,020 / 7,020 owned bytes; 59 / 59 dispatch destinations aligned; historical 6,209 raw matches must not be divided by the 6,264 comparable-byte denominator | Remaining register, stack-home, and opcode byte differences; non-exact |
| ANM `ExecuteScript` | `d2cddda`, ANM /GL /GS probe | 9,964 / 9,964 bytes, 0xFC target frame, 92 physical groups in order, 84 of 92 spans target-sized; 2,986 / 8,572 raw comparable bytes | Eight non-equal group spans and aggregate/RNG private ABI; non-exact |
| Enemy `DispatchEclInstruction` | `d2cddda`, selected ECL + ANM support graph | 14,384 / 14,416 bytes; target-sized 181-byte selector, all 108 group destinations in physical order; 0x2BC vs 0x2C4 stack allocation | ECL argument adapters, spawn/laser group lifetimes, stack allocation; non-exact |

The 2026-10-14 bounded Run refinement nests `ReadIntValue` in the
`SpawnThread` call, restoring target argument-push scheduling. Two cold
`EclVmHost::Run` entry probes reproduce 7,020 owned bytes, all 59 aligned
dispatch destinations, 6,223 / 7,020 raw matches and 6,115 / 6,264
matched-comparable bytes (baseline 6,210 and 6,102 respectively).
`normalization_complete=false`: the final 88-byte selector table is neither
decoded code nor DIR32 relocation data. The current checker deliberately
rejects that suffix; no checker or policy change was made. Register, stack-home
and FORMAT differences also remain. This is zero new exact credit.
Only Run was modified and cold-probed; prior helper units were not replayed.

A later isolated FORMAT store-before-null-check move is also reproduced by
two cold real-host probes: 6,233 / 7,020 raw matches and 6,125 / 6,264
matched-comparable bytes, with every dispatch offset still aligned. The
percent/scratch stack homes remain swapped and normalization remains
incomplete. This scheduling improvement adds zero exact credit.

The later `62b0980` change is restricted to the non-exact
`EnemyCancelBulletRecord` timer-flag dataflow (200 / 199 candidate/target
bytes; 131 / 183 comparable bytes), with all 14 source-scope canonical exact
units passing. The dispatcher-size checkpoint above predates this adjacent
source change and must be regenerated before making a dispatcher-size claim.

## Bounded ANM closure (2026-10-14)

`AnmLoadedView::Release` (242 bytes) and `ServicePreloadedAnms` (135 bytes)
pass two focused cold canonical linked-PE replays with all 11 fields bound.
The source change preserves one texture-entry pointer across COM Release/free,
matching target side effects and recovering both private-register allocations.
The local ledger gains 377 authored bytes; these local passes are not Factory
receipt acceptance. The broader historical ANM regression was not run.

## Bounded Bullet closure (2026-10-14)

`UpdatePolarAcceleration` at `0x004076A0` passes two focused canonical
cold /GL /GS replays: 209 bytes and all eight linkage fields, complete
normalization. The retained changes restore the expired-duration-first branch
and scale-before-subframe x87 load order, including FST before conversion.
Only this owner was modified and replayed. `UpdateVerticalWrap` remains
non-exact; the duplicated-branch control was reverted. Factory acceptance
for this unit passed at commit `b6fadac1f786801dbc4cdbd9b5e4558844e02b17`:
receipt `6c364031e1979cc5647b8d4b4b39e98c425d783307df6c4f1dc60a45c29ce364`,
verdict pass and decision accepted, bound to snapshot
`8d6643829b54e3d8744f18c81decaef505a876130cbf656d4ed93172b91d27f3`.

### Prior ANM receipt bindings

Both prior ANM units received pass/accepted Factory receipts at clean commit
`11a9ab346a81341ce37ca7d823630e1f467a461d`, source snapshot
`850fff77f0604b27e516465989888b857232331f3aa18baa079a8bf8ba7e5f28`:

- Service: `receipt:5caf3240ab78421312f231b5fc2101b80520b8dd0108eff9cc3fde582e5bd30c`.
- Release: `receipt:134b6af1da960c7d23a71b3f14e6333609b57a1b2a7f328a693536368a0447e3`.

These attest that source snapshot only; imported ledger totals are not a
fresh replay of every historical function. No unchanged historical unit was
replayed in the Bullet batch.

## Recent bounded negative controls (2026-10-14)

- The ECL percent-qualifier-only and conversion-switch-only controls each
  grew the owner to 7,024 bytes and worsened matching. Both were reverted;
  keep the isolated store-before-check refinement and stop that FORMAT branch.
- Radial initializer `0x004452F0` is a stronger medium-owner follow-up:
  the existing ExecuteScript /GL /GS graph with RandomMath, AnmVmCreate and
  AnmVmId yields 803 / 803 bytes, complete normalization and 623 / 631
  comparable matches. Three targeted source controls regressed and were
  reverted. Eight instruction/scheduling bytes remain; no exact credit.
  Do not reuse the unrelated service-entry 571-byte probe as its baseline.

## Active reconstruction order

1. **ECL Run — `EclVmContext::Run @ 0x0044E1A0`**
   - Keep the full target-owned 7,020-byte function in scope; do not reduce the
     problem to nearby leaf functions.
   - Rebuild diagnostics from the current source before judging a change.
   - The FORMAT/shared-tail placement and all 59 dispatch destinations were
     aligned in the selected checkpoint. Concentrate on remaining raw-byte
     differences, stack homes, and private register allocation; do not treat
     target-sized layout as exactness.
   - Replay only the canonical exact units actually affected by a bounded edit.
   - The six operand/read/resolve helpers at `0x0044FDB0-0x004500CC`, plus `SpawnThread`, `FindThread`, `StopAllThreads`, `Push`, `Pop`, `EnterFrame` and `LeaveFrame`, are canonical exact constraints; do not trade them away for a runner score.

2. **ANM ExecuteScript — `AnmRenderManagerView::ExecuteScript @ 0x0043EE30`**
   - Preserve the reviewed 92 physical dispatch groups in target order.
   - **Correction:** target and present candidate both restore EDI at the
     shared loop tail (the missing-EDI claim was stale).
   - Retain the recovered 0xFC frame and Y-before-X SCALE_TIME dataflow;
     investigate the remaining eight case spans and aggregate/RNG lifetimes
     instead of reshuffling the 92 dispatch groups.
   - Rebuild the linked diagnostic after support-source changes.

3. **Enemy dispatcher — `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770`**
   - Always regenerate the diagnostic baseline from current source first.
   - Preserve the current 181/181 selector and 108-group ordering;
     prioritize private ECL argument calling conventions, laser construction,
     0x2C4 target stack allocation, rank arithmetic and x87 homes.

4. **Batch closure**
   - `BulletRuntimeView::UpdateHorizontalWrap @ 0x00407DA0` is canonical exact
     (148 bytes); `UpdateVerticalWrap @ 0x00407E40` is target-sized but remains
     non-exact. Preserve the full `BulletManager.cpp` exact gate when refining it.
   - Interleave smaller non-exact Bullet/ECL functions when they constrain the
     large owners, but do not turn the project into a leaf-only queue.
   - After a coherent batch, run the broader tracking/progress/CI checks and the
     required focused exact replays.

## Recovery and verification

Start every resumed session by recovering the real worktree:

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

For a bounded source change, use the narrowest relevant diagnostic first.
Before committing a coherent batch, run:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Factory receipt acceptance is separate from a local replay or Git commit.
Never report a diagnostic size, similarity score, successful compile, or
checkpoint commit as exactness.

## Scratch and cleanup policy

- `.analysis/` and `build/` are disposable working state, not durable records.
  Keep scratch only while an unresolved claim needs it.
- Before pruning scratch, preserve retained source edits in Git and move durable
  conclusions into source, ledgers, reusable scripts, or `KNOWLEDGE_BASE.md`.
- Remove superseded probes, decompiler dumps, logs, generated PE/PDB/OBJ files,
  stale build trees, and Python caches after a bounded batch.
- Live knowledge entries must not depend on deleted scratch files.
  Historical experiment paths and superseded scores belong in Git history,
  not in the maintained knowledge index.
- Point-in-time audit snapshots belong in Git history once their durable accepted
  conclusions have been absorbed into source, ledgers, and `KNOWLEDGE_BASE.md`;
  do not keep obsolete snapshot reports on the live documentation surface.
- Keep `ghidra-project/`, `.tools/`, `_reference/`, and `resources/th10.exe`;
  they are private analysis state, pinned tools/reference material, and the
  canonical private target rather than generic build debris.

## Scoped sound checkpoint receipts (2026-10-14)

- Worker join175 bytes: Factory PASS/ACCEPTED receipt
  `1f8cc90920a046710d4b10fe7d1be9a3eeb374cdb568eea4d74a1f39131f0ac4`,
  source commit `ff61a67382296b2502cb95f84096946621867a31`.
- Owner release301 bytes: Factory PASS/ACCEPTED receipt
  `4e18ec3886d85cb999ae4432033b6ada2c1bd1d2696426f90e6be4d120dbaa3a`,
  source commit `4bc97d04f93d405e1894ee75aba13474ea52e291`.
- These are immutable source-bound results, not fresh acceptance of later
  source snapshots. Setup-thread57 bytes received Factory PASS/ACCEPTED at commit
  `755a568ac5fdb07368dd031180b40366e0b7d843`, receipt
  `f51e5a704c099135eaf4463d53d35506d44b9631aa67401193294fc81f32dcbb`.
- Initializer786 and loader107 remain non-exact. The260-byte primary-format
  helper has official SDK lineage evidence; do not add authored exact credit
  or silently change denominator/classification. Sample-loader684-byte source
  and private ABI were subsequently recovered; see the next checkpoint.

## WAV sample reconstruction (2026-10-15)

The 684-byte sample-loader now has maintained source and target-derived private
EDX/ECX/RET4 ABI under real Initialize and guarded WinMain visibility. Its
684-byte diagnostic remains non-exact, with eight ordinary bytes differing. The necessary 71-byte chunk scanner
passes two canonical cold replays including its strncmp binding. Independent
source/ABI review passed; Factory PASS/ACCEPTED for this helper is bound to
commit `da5c5c2974d89d09345b4ffc500b9c88c5e69987`, source snapshot
`27c79c62286fa9e3fc29cd406a537c68572b767ee7f493543db290a949b89db0`,
receipt `fcf98ab2d8bfcdc910c5f79a75cadb93a7635d07bc398b43e9150c2d3543071b`. No loader or initializer partial exact credit is assigned.

Recovery found unchanged clean 755a568 with upstream parity (ahead/behind 0);
this was observed external state, with no ref change or push by this batch.
The former memcmp draft and cdecl helper control were corrected from target
callee identity and both observed callee-clean call sites, not retained.

The subsequent advancing-cursor refinement is reproduced in two cold real-entry
diagnostics: loader 684/684 bytes, 605 raw and 576/584 comparable, complete
normalization. No partial exact credit. The negative-increment control is
byte-neutral and reverted; the remaining eight-byte ADD/SUB and argument-setup
scheduling cluster is parked. Helper71 and setup57 remain structurally equal
in this diagnostic graph, without a fresh Factory receipt for this later source.

## File writer checkpoint (2026-10-15)

FileSystem::WriteFile at 0x0044B540 now has natural source and the genuine
maintained WinMain configuration-save caller. Two canonical cold replays match
all 218 bytes and 20 fields after independent review. Factory acceptance is
PASS/ACCEPTED at commit `1efe88f2d728f1c0aeabc0bd2b92952fe7bf83dd`,
receipt `a4b2598934f011e58c2b8fdc858119d80bf117236c1e4a13856f235ea6d2199c`,
snapshot `e59404205a5694d15a1d5bd7f92a743f73c09db19c189c6644b7d69920d8deec`.
The second target log-flush call remains an
unresolved MainFlushLog seam; filesystem storage and runnable closure remain
open. No file-writing runtime side effects were executed.

The adjacent BGM setup241 candidate is parked because its StartupThread caller
and 702-byte streaming helper both lack source. The213-byte streaming reset
has likely SDK lineage requiring origin scrutiny; it was not implemented.

OpenWriteFile137 now also passes two full canonical cold checks with12 fields
and independent source/ABI review. Factory acceptance is pending at this
checkpoint; it subsequently received Factory PASS/ACCEPTED at commit
`c5a3892199599cfd240ff6e499006af3030d833b`, receipt
`4eee7896a5f8bf2e5d60c001e0243bea7627d0afb782c7bbf669f396af0f90a0`,
snapshot `7a4fed5d558eddf6d4a97c18b6476174026d5edd2028c205048723daacc26c6e`.
Its guarded SaveReplay entry inlines the source call; the separate
137-byte owner remains naturally emitted under the existing pinned harness.
SaveReplay itself remains1511/1681 nonexact, and persistent storage plus the
WriteToOpenFile seam remain open. No runtime file IO was executed.

## Replay serialization correction and priority (2026-10-15)

SaveReplay now serializes each stage's complete records stream before its FPS
stream, correcting the previous per-node interleave. Four target-observed inline
write protocols now have source; no separate owner boundary or byte credit was
created. The synthetic source-extracted copy-loop regression distinguishes the
old format and passes on the available host compiler. Full caller remains
1748/1681 bytes and nonexact; shared storage/runtime closure remain open.

The user reprioritized important large functions. Do not resume a small-owner
queue. Next work should select a concrete missing semantic/structural component
in ECL Run, ANM ExecuteScript, Enemy dispatch or a similarly important owner.
Small prerequisites are allowed only to support that selected major owner.
Keep parked FORMAT, radial and sample-loader spelling controls parked.

## Active large-owner ANM checkpoint (2026-10-15)

Fresh ExecuteScript baseline was 9900/9964 with79/92 target-sized groups, not
the earlier historical9964/84-group checkpoint. Two target-backed changes are
retained: mode9 calls now bind the established script polar owner, and the
F_SET_RANDOM branches decode operands before selecting each concrete RNG
receiver. The latter recovers the complete130-byte group and removes the
unsupported RNG-pointer lifetime across GetFloatVar.

Two cold diagnostics and independent review reproduce9948/9964 full owner,
9572/9588 pre-table span,0xF8 vs target0xFC frame,94 selectors/92 groups in
order,83 target-sized groups and6331/8000 case-aligned comparable bytes.
Full-owner comparison remains1212/8564 comparable (1386 raw); zero exact credit.
Nine size gaps remain: opcode61 +1,48 +9,63 +1,58 -3,60 -14,27 -9,9 +9,
11 -9,2 -1. Next bounded hypothesis is SCALE_TIME60 aggregate materialization:
target preserves decodedY separately, retainsX on x87, then builds/copies XY.
Do not blindly restore older rejected scalar/spelling controls.

SCALE_TIME follow-up controls are now parked: a natural constructor on the
existing Float2 failed VC7 C2620 because of its vertex-union use. A constructor-
free by-value factory compiled but introduced an extra aggregate copy, frame
0x104, owner9980/9964,81 same-sized groups and6073/8008 case-aligned matches.
Both controls are reverted. Retain the reviewed9948-byte/83-group source.
Select a fresh major-owner structural/semantic gap rather than extend this
aggregate spelling search.

## Enemy major-owner laser prerequisite checkpoint (2026-10-15)

Recovered the missing factory and bounded construction seam behind four laser
dispatch groups (1485 target bytes), sharing request definitions across the
dispatcher and six external caller owners. Target object sizes/VM offsets are
asserted and actual timer/AnmVm constructors are used. Independent review
approved partial semantic retention, with zero new exact bytes.

Fresh full owner baseline14384/14416 became14480/14416; frame remains2BC/2C4.
Raw/comparable scores894 and625/11584; normalization complete. All181 selectors
and108 ordered destinations match. Table reporter still has the preexisting
nested-case duplicate warning; do not call its overall result a pass.

The153-byte factory still has stack request/type and plain RET rather than
target EDI request/RET4. Placement construction adds null checks and leaves VM
calls out-of-line. External19-slot tables remain diagnostic anchors. Raw-storage
lifetime and the non-instantiated virtual-prefix cast are explicitly partial.
No runnable closure or recovered full19-method C++ interface is claimed.
Existing exact initializer leaves40CF40/41C030 retain their provenance; the
23-byte request initializer is raw-equal here but adds no credit. Constructor
origin remains indeterminate. Next work should establish a defensible natural
object-lifetime graph from real virtual contracts, not padding/register controls.

Reproduction artifacts: .analysis/gpt-dots-enemy-laser/probe.py, baseline.json,
integrated.json and matching -table.json; distinct guarded image/map/PDB under
build/probe-ltcg/EnemyLaser-{baseline,integrated}. The integrated image SHA256 is
24f23d99e94b570d3e399c993c77e21f159237e94115f1a57ff9e8c99ef56139.
The compiler rejects nonmember __thiscall pointers (C4234); do not repeat that
representation control or infer a portable ABI from the virtual-prefix view.
Focused existing EnemyLaser canonical units (polar/bounds) still replay exact,
121/121 bytes across two artifacts. Tracking, pinned toolchain declarations,
progress, CI (including entry and replay-payload regressions), and whitespace
checks passed. This is regression evidence only, with no new Factory claim.

## Large missing-owner checkpoint: GameResetGui (2026-10-15)

Complete1965-byte reset owner413BC0-41436C is now maintained in Gui.cpp and
connected to both existing GameManager reset paths. Independent source/ABI
review passed. Existing GuiView supplies all six VM arrays and ID/resource
fields; creation/ID/interrupt dependencies use maintained ANM source.
The target discards the second front-VM ID (no9E5C write) and interrupts9E50
again after creating9E54. Both quirks are preserved explicitly. Lives are not
clamped and signed power division remains target-faithful.

Cold graph Gui+GameManager+AnmManager+AnmVmCreate+AnmVmId, pinned /GL /GS,
guarded GameManagerUpdateBody entry402EA0: new owner402540 is1786/1965,
421 raw and399/1745 comparable, normalization_complete=true, EDI/plain RET.
No exact bytes or Factory claim added. External data storage/runtime and
full dependency codegen remain open. Initial signed-return entry misspelling
was rejected before anchoring; the corrected unsigned symbol is verified.
Evidence: .analysis/gpt-dots-gui-reset/recovered-entry-fixed.json, target.txt,
synthetic-test.log; distinct build/probe-ltcg/GuiReset-recovered-entry-fixed.
The full-body synthetic test runs with actual source and stub dependency
sinks, covering event order, resource/array/sprite destination identity, ID
quirks, signed digits, and independent startup/global short-circuit gates.
It is wired into CI; no game code/resource IO is run.

Parked core audits: Enemy laser natural-lifetime closure still lacks full
contracts for slots0/8/9-18. Target manager calls slot4, unlinks and directly
deletes; derived slot4 is only xor eax,eax/ret, with no AnmVm destruction.
Do not introduce a virtual destructor or fabricate19 method signatures.
Read-only inventory lives under .analysis/gpt-dots-enemy-laser/.
Fresh ECL source/target audit found no new semantic gap in return/thread,
argument/control or dispatch paths. Current source equals retained store
snapshot. Generic binary was overwritten: old report401CD0 is stale; actual
map401CC0 matches all60 relative destinations and88 literal selector bytes.
This historical readdressing is not fresh codegen authority. Do not repeat
rejected FORMAT/volatile/switch spelling controls.

GameResetGui interrupt structure refinement: target contains two expanded
lookup/root-child walks rather than calls to the general ID helper. A local
helper control stayed out-of-line (1761bytes), so it was rejected. Direct
lexical blocks restore28 direct calls and both in-owner walks. Latest full
owner1981/1965,217 raw,198/1753 comparable, normalization complete; no exact
credit. Prefix1560/1541 and first-region120/123 are LENGTHS; final140-byte
raw-equal tail is only a diagnostic subregion, not an independent owner.
Independent semantic/test review passed. Tests execute actual reset blocks
and actual FindVm on synthetic primary/secondary/root/child cases.

Test audit correction: independent startup/transition combination fixtures
claimed at26fceebb were absent because a text-insertion assertion failed and
the shell still ran the old test. Do not rely on that historical coverage
claim. Current fail-fast source verification, focused test, full CI and
independent read confirmation establish all four startup gates and all four
transition/runtime-flag combinations, with resource/destination assertions.
No historical commit was rewritten. Source/tests now use fail-fast commands
and inspect exit/stderr before reporting success.
Evidence: .analysis/gpt-dots-gui-reset/lexical-interrupt.json, lexical-regions.json,
lexical-interrupt-test.log, lexical-ci.log; distinct GuiReset-lexical-interrupt
build image SHA b015b9de780232313e4b9eac18797fbbb988d81c1ee617f787cc8dc5c899591f.

Music-room full source recovery (2026-10-15): FrontEndUpdateMusicRoom at433EF0
now covers all four states and its necessary59/92-byte line parsers and206-byte
position initializer. Independent target/source and strengthened-test review
passed. Guarded maintained FrontEnd::Update entry RVA2320; full candidate2560
versus ledger2778,135 raw and118/2378 comparable, normalization complete.
Target code ends4349C9, then8BFF alignment and four-entry table4349CC-4349DB;
physical2796-byte comparison118/2396. Literal table0e3f4300464343001b4443004e494300
and ordered dispatch identity are audited separately. Existing ownership rows
and denominator remain unchanged; no exact claim or Factory promotion.
Position initializer206-byte structural comparison178/178 is only diagnostic:
component anchor symbols do not prove coherent zero-vector aggregate storage.
Music command/load/play and other storage externs leave runtime closure open.
Preserve original count/@/unbounded-copy quirks, null-path closing, track-zero
profile write, confirm precedence, double cursor pop and unordered position
copy. These are fidelity observations, not safe parser/API recommendations.
Only synthetic fixtures have sentinel padding; actual OpenFile mode0 allocates
exact file size. Zero-count output tests cannot establish read-before-check.
Actual-source fixtures cover four states, CP932 copying, warnings and receivers,
12-track scroll visibility,39.999/40/40.001/NaN and simultaneous confirm/cancel.
Dependency/test strings are stubs; separate target audit verifies all9 actual
CP932 literals, not these stubs. Full runtime/precision equivalence is unproved.
Artifacts: .analysis/gpt-dots-music-room/unordered-fixed.json,
owner-table-audit.json, strengthened-test.log; image retained separately in
build/probe-ltcg/MusicRoom-unordered-fixed/source.exe, SHA256
33b28b51989b47dac155571524b93ba3f60f995ad19759ea378f9c786ca951bd.

Music-room interrupt refinement: the general ID helper stayed out of line in
both paths. A manager-wrapper control still retained the confirm wrapper call
and was rejected. Direct lexical bodies naturally produce target call depths:
confirm calls FindVm then walks root children; close expands both two-list
lookup and root-child walk. General helpers/compiler flags are unchanged.
Latest contribution2808 =2791 code +1NOP at406467 +16 table, versus target
2796 =2778 code +2alignment +16 table. Whole physical comparison309 raw,
286/2396 comparable, normalization complete; no exact credit. Code-region
lengths candidate/target: entry30/30, state0 1082/1080, state1 213/213,
state2 1302/1331, state3/shared epilogue164/124. Original table ownership
remains provisional; no denominator/boundary edits. Independent delta review
passed actual source/list tests for both paths, primary/secondary precedence,
zero/missing IDs, root children/nonroot suppression and unchanged IDs/flags.
All previous parser/state/warning/scroll/NaN/precedence fixtures remain.
Evidence .analysis/gpt-dots-music-room/lexical-interrupt.json,
lexical-table-audit.json, lexical-regions.json, lexical-tests.log; retained
image SHA d676ad755efb8203d8956c406fa4320884ea023c391244c9e9d88339c0032c7d.

MusicRoom closing semantic correction and bounded prerequisites (2026-10-15):
Target42C770 only resolves/marks pending interrupts and root children. Historical
InterruptVmNow instead executes scripts immediately. Only the two MusicRoom
closing calls now use recovered FrontEndMarkVmInterrupt; other callers are
explicitly unaudited. Tests observe zero immediate ExecuteScript calls, pending
root/child and nonroot suppression before an explicit later synthetic tick.
Load420A90 copies into256-byte local buffer, finds last dot, overwrites exactly
three extension bytes without truncating a longer suffix, queues mode1/slot,
returns1/RET8. Play420B10 optionally queues4 then2/slot, publishes the specified
profile-track byte even on full queue, returns0/RET8. No invalid-input repair.
Queue43E460 uses the existing SoundPlayerView:32 physical slots at1F88, stride10C,
but admission scans only31. Opcode/argument/step/path offsets0/4/8/12 asserted.
It writes opcode/argument, unbounded-copies name then clears step; full queue
silently drops. Global sound-owner storage remains extern-only. Natural real
caller graph recovers EAX owner/EDX path/RET8, not an anchor ABI assertion.
Six-TU guarded diagnostic: MusicRoom2772=2755code+1padding+16table versus target
2796=2778+2+16. Physical329raw,300/2380comparable, normalization complete. Table
ownership remains provisional with no denominator change. Mark65/65 has50/57
comparable; queue90/104 has3/104. Load120 and play73 structural matches are only
diagnostic with open bindings/storage and no standalone exact credit.
Independent semantic review passed. Existing MusicRoom synthetic test still
mocks music calls; a separate test extracts actual load/play/queue bodies and
checks holes/full queue/reserved slot31, last-dot/long suffix preservation,
input immutability, command order and profile publication despite dropped
commands. Both tests pass from a fresh temporary root without build directory
via shared setUp; no game IO or actual audio execution.
Evidence .analysis/gpt-dots-music-room/prerequisites.json,
prerequisite-table-audit.json, prerequisite-owners.json, prerequisites-tests.log,
clean-root-test.log. Image SHA
3bc0a6b50cc32d84b4d8b3180ee6789142bfe27274d63a4cda5e6296516a4835.

MusicRoom final warning structure control: replace arithmetic ternary mode
selection with target-observed separate mode4 and mode3 queue/publication/return
arms. This is semantic-neutral structural fidelity, not a bug fix. Independent
review verifies both call roles and actual-source fixtures (one command, no
playback, warning/timer publication). Full physical2804/2796, code2787/2778,
padding1/2 andtable16/16;454raw and412/2384comparable, normalization complete.
All state call counts now agree; candidate/target region lengths1082/1080,
213/213,1334/1331,128/124. These lengths are not exact-byte counts. Residual
state3+4 includes unresolved EnemyMarkPendingInterrupt call ABI; no additional
graph expansion or register/size spelling control is justified here. Park
remaining near-match shape; zero exact credit and table ownership unchanged.
Evidence warning-arms.json, warning-table-audit.json, warning-regions.json,
warning-arms-tests.log under .analysis/gpt-dots-music-room; image SHA
7da9a53933bae764948b3ccb1fc30c68b2d731b07661e8d868bd5201646dee6b.

Stage timed-script recovery (2026-10-15): new StageScript.cpp/.hpp implement
all15 commands at403C80, real callback402720, active-object updater403990 and
fog-value constructor405040. Signed opcode/record-size semantics, time-relative
jump, opcode0 yield without pointer consumption/timer tick, direction-before-
position-before-fog evaluation and rocking behavior are preserved. Opcode14
uses actual maintained ANM initialization; negative script index only clears
flag1. Layout assertions bind two Float3 interpolators,0x8C fog interpolation,
eight script VMs,three auxiliary VMs and280-byte camera state. Callback gates,
normalization, object VM update, dispatcher,8 VMs,optional3 at game speed1,
speed restore, auxiliary clear, camera publication and frame increment follow
target source order. Object updater reads post-execution instruction state.
Fog evaluator688 remains a declared unresolved seam; no private ABI validation
from that declaration. Published camera is an extern-only overlapping aggregate
view of target491D7C; existing smaller ANM aliases are not closed ownership.
No source/byte claim for a complete stage registration/rendering subsystem.

Ledger dispatcher1806 ends40438D;2-byte8BFF alignment and15-slot table404390-
4043CB imply provisional physical1868. Literal table and destinations saved;
no boundary/origin/denominator edits. Guarded callback entry401AA0 belongs to
maintained source. Dispatcher2192=2129code+3pad+60table; physical92raw89/1804
comparable, normalization complete. Callback294/304,175raw170/268comparable.
Updater154/154150/150comparable and fogctor94/9478/78 are diagnostic only, no
standalone credit. Native dispatcher is EBX/plainRET versus target stack/RET4;
native callback is ECX versus target EAX. Native callback copies camera before
clearing auxiliaryActive and hoists frame read, unlike target machine order.
Source order is correct but no emitted-order/ABI closure is claimed. Rocking
x87 sequence independently reviewed; host tests do not prove x87 equivalence.

Independent source and strengthened-test review passed. Test compiles actual
stage source plus actual shared timer source against synthetic ANM/interpolation
and normalization sinks. It checks signed backward record size in allocated
fixtures, jump/yield/time, setup selection and all Float3 components, object
inactive/zero-count/empty primitives/VM-driven termination, and dependent
normalize/object/dispatcher/8+3 VM/camera-publication behavior. Fog initial/end/
all channels are not exhaustively asserted. Publication after VM mutations is
observed; relative ordering among final clear/copy/frame/speed-restoration is
not independently established by host sinks. No invalid-index/overflow safety,
standard object lifetime, x87, target ABI, real game IO or runtime claim.
Evidence .analysis/gpt-dots-stage-script/attested-decompile.json,
layout-corrected.json, owner-table-audit.json, strengthened-test.log; image SHA
aa87fbff37b064f94ceb8699304af55caaca7c41401e37212f8923daec8da645.

Stage fog dependency recovery and precision correction (2026-10-15):
The0x1C aggregate is six float channels(start,end,B,G,R,A) plus derived packed
BGRA, not seven independent floats. Target arithmetic constructors retain
extended x87 color results after FST stores and __ftol2 consumes those results;
low bytes wrap without saturation. Storedfloat1 can therefore coexist with
packed0 for an extended result1-2^-26. Tests describe that representation
witness separately and never claim ordinary host operators reproduce x87.
Recovered evaluator modes7 additive,17 acceleration,8 Hermite and maintained
CalculateAnmInterpolation easing, with positive-duration tick/completion and
unmodified zero/negative-duration behavior. Completion returns initial only
for mode7, otherwise final, copying packed dword rather than repacking.
Real dispatcher call recovers EBX/output-stack/RET4 for evaluator, no anchor
ABI assertion. Full evaluator664/688,234raw228/600comparable, normalization
complete.92/101/92 arithmetic structural matches are diagnostic prerequisites,
not independent credits. No new canonical replay/Factory acceptance attempted.

Independent review found initial Hermite expressions lost target's(t-1)float32
spill and used sequential3-t-t. One natural shared twice=t+t correction restores
both: candidate401DF9 FSTP and401E05/09 memory FMUL match target404B34/40/44,
with retained doubled-t grouping and subtraction. No volatile/flags/coercion.
This verified numerical improvement is distinct from remaining whole-owner
nonexactness. Two source tests pass: dispatcher keeps explicit fog sink;
separate actual fog/timer/easing bodies cover all six arithmetic channels,
packed finite boundaries, linear/ease-in/Hermite/add/acceleration/constants/
default, scaled timer and completion modes0..17. That completion loop does
not test all easing curves. Nonbinaryt0.1 differentiates grouped/sequential
subtraction in the host model; native equivalence comes from PE instruction
review. Invalid/out-of-range conversion, ABI and alias/lifetime proof excluded.

With real fog source, full stage dispatcher2052=1991code+1pad+60table versus
provisional1868=1806+2+60;162raw155/1788comparable, normalization complete.
Camera aggregate storage, callback native publication ordering and full stage
ABI remain open; ownership/denominator unchanged. Evidence fog-hermite-grouped.json,
fog-grouped-owner.json,fog-grouped-candidate.txt,fog-grouped-table-audit.json,
fog-grouped-tests.log under .analysis/gpt-dots-stage-script. Retained image SHA
f4b4a878cbf3aa992d1b80fb42f4888e711427c5904446e6e4ee9758365688cb.

Stage direct XYZ setter refinement: opcodes2/4/6 now perform target-observed
sequential field reads/stores rather than snapshotting constructor temporaries.
Opcode2 preserves old-position capture and subsequent x87 delta computation;
its residual4 ordinary-byte differences are a cached EBX constant, not camera
arithmetic. Independent native review verifies target load/store order and
retained fog precision fix. Region spans113/113,32/32,32/32 with105/109,28/28,
28/28 comparable are not independent owners or credits. Full1972physical=
1911code+1padding+60table versus1868target;203raw193/1776comparable,
normalization_complete=true. Physical instruction decoder reports1971/1972:
normalization completeness does not imply every table byte is an instruction.
Actual code ends RET4 before padding/table, each table remains literal evidence.
No valid runtime overlapping-record/camera alias is claimed; no artificial
undefined-overlap fixture added. Both prior actual-source tests pass unchanged.
Evidence direct-setters.json,direct-table-audit.json,direct-cases.json,
direct-dispatcher.txt,direct-setters-tests.log under .analysis/gpt-dots-stage-script;
image SHA75a3aae387723d53b70c5b22e0fef0c347b5925620189a07092ce7da27135a13.

Player update state fidelity (2026-10-15, gpt-dots): fresh target-attested audit
of the maintained 3,089-byte owner 0x425730 corrects five misidentified calls to
0x405410. They are existing PlayerTimerView::SetCurrent, not sound playback:
state 1 highlightTimer=270; state 4 updateTimer1=60 then highlightTimer=200;
respawn highlightTimer=280 then updateTimer1=0. All five native candidate calls
now supply the target-observed timer offset in EAX. State 4 also restores the
missing RebuildPlayerOptions after subtracting 20 power and before the display
update. Respawn drawPosition.y is 480.0 (target bits 43F00000), matching the
fixed-point positionY=48000; the old source used 240.0. The obsolete sole-use
PlayerPlaySound declaration was removed. Other descriptive extern contracts,
including PlayerStartPowerDisplayTimer, were not independently recovered here.

Independent review passed the affected state paths and native call evidence.
The new CI regression extracts the actual state switch and actual timer setter;
it covers all five receiver/value pairs, 19/20 power thresholds, option/display
order, independent suppression paths, timer lazy initialization/custom scale,
and effect-mutated XYZ saved before respawn position replacement. The time-8
fixture observes the transition-helper call only; its stub leaves a quiet
state-2 time and does not independently prove fallthrough. Tests stop at switch
exit, before the common tail decrements highlight and increments update timers.
They are not whole-callback, x87, ABI, or runtime proof. Candidate/test globals
for g_PlayerTimerScale and g_AnmGameSpeed remain separate even though the target
aliases both at 0x476F78; speed-reset/alias closure is explicitly unproved.

Focused /GL baseline and corrected graph retain the genuine registered
PlayerUpdateCallback entry with guarded PE/map/PDB identity. Baseline physical
contribution 2776, raw184/comparable152 of2693; corrected physical2808,
raw166/comparable146 of2697. Both normalization_complete=true, both nonexact.
Corrected image SHA714b89a04b39b035cb524a22c1a721b1802759e6ea1d1dc0ccfbe580189ad674:
body0x402B00, 2786 code bytes through RET, two alignment bytes, five-entry20-byte
table at0x4035E4. Target remains3089 code bytes through RET4, three alignment
bytes and separate20-byte table at0x426344, provisional physical3112. Candidate
private body EAX/RET differs from target stack receiver/RET4; the callback is
real source visibility, not target ABI closure. Normalization is not full
instruction decoding or binding acceptance. Tables/layout remain explicit in
.analysis/gpt-dots-player-update/ownership.json; no ledger extent/denominator
change. Other evidence: attested.json, baseline.json, respawn.json,
respawn-call-contexts.txt, test-reviewed.log. Zero new exact credit.

Stage tail is parked: remaining318-vs220-byte region includes candidate-inlined
timer methods versus target calls. No genuine semantic/context cause was found
that justifies forcing an inline boundary; no new spelling/flag controls ran.

Stage projected bounds and instance drawing (2026-10-15, gpt-dots): recovered
full authored owners0x403090/1984 and0x403A30/592 in StageDraw.cpp. Fresh raw
hash-checked target decode, not the stack-corrupted decompile, establishes the
camera/object/instance contracts. Bounds compares center displacement with
squared distance, projects eight corners in binary XYZ sign order, admits only
0<=z<=1, preserves original else-if extrema and inclusive32..416/16..464 overlap.
NaN distance does not early-reject; NaN projected depth is excluded. Return0 is
visible. Natural source/host fixtures do not close all x87 spill precision.

The real caller publishes the camera, calls the still external EDI-private
0x4215A0 setup contract, sets viewport/current camera mode, compares signed
layer, and preserves VM position/scaling, fog flush/state order, flags and
visible/culled/draw counters. Instance stride16, quad position+8, dimensions
+14/+18; existing280-byte camera now names matrices+4C/+8C, viewport+CC and
three rocking components without changing layout. D3D viewport slot+BC is typed
and asserted. Official D3DX9 declaration references:
https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxvec3projectarray
https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixtranslation
No upstream1099/929 draw owners expanded. No D3D/game IO executes.

Independent source/layout/target review passed. Actual-source fixtures verify
corners/projection arguments, distance threshold/NaN, depth/clipping boundaries,
lone-point extrema quirk, signed layer, primitive selection, VM mutations and
negative-path sentinel preservation, full-dword fog, Flush/state/Draw order and
counters. Projection/rendering are controlled sinks, not native D3DX or lifetime
proof. Prior stage tests remain intact. Pinned /GL /GS real-caller graph:
bounds1855/1984,167raw158/1864comparable; caller565/592,58raw53/516comparable.
Both normalization_complete=true and fully decoded, neither exact. Candidate
bounds plainRET differs from targetRET4; callerRET8 remains. Guarded entry
0x4017F0; image SHAe54792ed0a112f6af36a27dafe3eb406172def2407509b4fbbd89f0bff72ae42.
Evidence .analysis/gpt-dots-stage-draw: attested.json, bounds-target.txt,
draw-target.txt, loaded-sprite.json, test-reviewed.log, prior-tests.log.
Initial compile used an incorrect member name sprite; corrected to existing
loadedSprite before the successful diagnostic, without compiler controls.

Camera setup, actual D3DX imports and shared globals remain nonrunnable anchors.
Camera/view-prefix reinterpretation and Main/ANM globals do not establish C++
lifetime, alias or allocated-storage closure. Target fog0x492378 is a dword;
Main retains its byte view, while this graph uses a documented extern dword,
not a new allocation. No ownership/denominator changes, helper credit or Factory
promotion. Native exact remains47733/266187. The preceding Item2320 read-only
audit found no new justified semantic correction and was parked without build.

Stage camera prerequisite connected (2026-10-15, gpt-dots): StageConfigureCamera
0x4215A0/330 now has maintained source in the real StageDrawInstances graph,
replacing its private-ABI-only anchor. Target and native candidate both use EDI
camera/plainRET. It flushes an existing renderer, builds LookAtLH from eye
(position+rocking), at(position+direction), up; builds PerspectiveFovLH using
FOV+48, unsigned viewport width/height, near30/far1800; publishes view then
projection; normalizes direction-cross-up into cameraRight+30; reloads renderer
before copying float shake offsets+E8/+EC. Existing camera280 layout is preserved
with static assertions; these fields replace previously unnamed/int views.
D3DX signatures were checked against Microsoft documentation:
https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh

Independent raw/native review passed. Aspect native instructions preserve target
FILD unsigned-correction(+2^32), FDIVP, then float store after division; the host
high-dimension fixture uses exactly representable inputs and is not precision
proof. Actual-body tests verify arguments/order and renderer/device replacement,
including a renderer appearing during an initially-null call and disappearance
before publication. D3DX is mocked, not a native implementation test. Existing
bounds/draw tests retain a camera sink; the separate new test executes actual
camera body. Global camera/view aliases and actual D3DX/import storage still
remain unclosed; recovered helper source is not executable product closure.

Fresh /GL /GS image SHA83fb28bceb10c28a35a491d8fdcf8079cfd413d149558a3ac97f1e0c1f6f5ee9,
guarded draw entry0x401940. Camera330/330,260raw250/290comparable; bounds1855/1984,
167raw158/1864comparable; caller564/592,57raw53/520comparable. All normalization
complete and full decode; all nonexact. No separate helper credit or Factory
promotion. Evidence .analysis/gpt-dots-stage-draw/camera.json, camera-target.txt,
camera-candidate.txt, camera-test-reviewed.log, camera-stage-tests.log.

Stage bounds precision and depth correction (2026-10-15, gpt-dots): independent
native review exposed two substantive gaps in the first source, now corrected.
Target rounds all center XYZ sums and all delta XYZ differences through float32
storage before the squared-distance test. Reusing one ordinary Float3 aggregate
for center then delta restores those six boundaries; an ordinary XYZ dot-product
expression emits the target native (Z*Z+Y*Y)+X*X accumulation in this pinned graph.
Two bounded controls only, no volatile, forced calls, flag or padding changes.
Geometry prefix through projection return was845target/702firstcandidate; the
other143-byte gap was not all register coloring. Corner identities and extrema
behavior remain unchanged. Full code still differs, so no exact claim.

Precision witnesses are explicitly models: center16777216+1 rounded tofloat
before subtracting16777216 yields0 rather than retained1; with squared X=2^64
and Y=Z=1, x87 64-significand-bit target grouping yields2^64+2 while the old
ordering can yield2^64. Host tests illustrate these distinctions and actual
host center behavior, not execution of the pinned binary. Native decoded
instructions, independently reviewed, establish the retained target sequence.

Correction to earlier notes/review: depth upper bound is INCLUSIVE1.0, not
strictly less than1. After FCOMP1, target TEST AH,41 / JP skip admits equality
(AH=40 has odd parity). The previous strict-depth conclusion and fixture were
wrong. Current source uses0<=Z<=1; all eight native checks now match the target
branch semantics, including NaN exclusion. Fixtures accept1 and reject the next
representable float above1; invalid/lone-point fixtures use that latter value.
Historical commits were not rewritten; current documentation is corrected.

Final image18ee0f848d0e8eefd57bbe745b4c28f32638505d31272a690f03151a83fe93e6,
guarded draw entry0x401990. Bounds1925/1984,146raw144/1864comparable; caller564/592,
57raw53/520; camera330/330,260raw250/290. Normalization complete/full decode but
nonexact, target bounds RET4 versus candidate RET remains open. All six round
boundaries and native sum order survive the depth correction. Evidence under
.analysis/gpt-dots-stage-draw: aggregate-delta.json, aggregate-xyz.json,
inclusive-depth.json, inclusive-native.txt, depth-test.log. Final independent
review passed; zero exact credit and all prior closure caveats remain.


## 2026-10-15 Stage early/late draw pass source recovery

Recovered complete 0x402850/1099-byte early and 0x402CA0/929-byte late bodies
with their necessary seven-byte registration adapters in StageDraw.cpp.
Early owns layers 0..7, late owns 8..11 with ANM layers 17/18 preceding them.
Raw target verification covers D3D Clear slot 0xAC, depth/color clear arguments,
fog/Z-write/Z-function ordering, camera publication, mix-color restoration,
transition allocation failure, fractional countdown, and repeated final state
writes. Ordinary local source helpers express these operations without forced
inlining or ABI controls. No executable game or D3D calls were run.

Read-only initializer 0x402230 evidence establishes adapter registration with
priorities initializer argument+7 and +10, node owner at offset 0x20. Initializer
source is not recovered here. The existing viewport helper, supervisor fog
helper, and effect factory remain externs: respectively target ESI/EBX,
EDI, and EBX priority plus five stack arguments/RET20. Ordinary declarations
and diagnostic anchors do not validate those private ABIs or allocate actual
global alias storage. No runtime or registration closure is claimed.

Diagnostic passes.json / build/probe-ltcg/StageDraw-passes/source.exe SHA256
cfbcb229fbb33cfd000da595b9422ad01d1138f0d9c650d1355fc59564ac1c33:
early 918/1099 bytes, 67 raw equal, 57/867 comparable; late 917/929,
135 raw equal, 109/645 comparable. Guarded early entry 0x4024C0 pushes ECX;
late adapter 0x4024D0 pushes EAX while both target adapters push ECX. Late
callback ABI remains open. All metrics are full-owner diagnostics, zero exact
credit and zero adapter credit. Prior bounds/camera closure limits persist.

Independent full source/target review passed. Actual-source host trace tests
cover both full passes, gates, VM0-null suppression, active tint, transition
failure and thresholds, fractional Add with previous/subframe values, and
fog snapshot before Flush with device replacement after Flush. CI runs this
fixture. Synthetic host sinks establish source event/state contracts, not
native D3D, x87, private ABI, alias storage or product behavior. Evidence under
.analysis/gpt-dots-stage-draw includes pass-early-target.txt,
pass-late-target.txt, pass-registration-target.txt, passes.json and
passes-test.log. Complete source retention is provisional and nonexact.


## 2026-10-15 Stage lifecycle supporting the full draw/script graph

StageLifecycle.cpp recovers the coherent five-owner graph: constructor
0x402160/188, Initialize0x402230/522, destructor0x402440/499,
StageCreate0x402640/151, and LoadData0x403850/308. Header layout exposes the
three chain slots, original cache/size, 0x90 STD header and mutable tables.
Existing maintained Stage, ANM, FileSystem and Chain types are reused.
Constructor origin remains indeterminate; no ownership/denominator change.

Target contracts: 0x2B64 allocation; eight plus three real VM constructors
before whole-object zeroing; separately cached original and mutable relocated
file copies; ANM slot (startupMode&1)+4; raw object-VM malloc; camera defaults;
three callback registrations with base+12/+7/+10 priorities and owner+0x20;
three unregister operations caching the chain before lock entry; data/cache/VM
free order; runtime flag-gated ANM Release followed by slot reload/delete;
matching global-pointer clears; then automatic reverse 3+8 VM destruction.
Target unchecked allocation, path, counts and null-new Initialize behavior is
preserved and documented, not recommended or made safe by the host fixtures.
Factory failure invokes normal destructor/delete and returns null.

Fresh pinned image3cdb24e01a358885578ed92411778638098f0720547ad565e9769d25bad9b564
has genuine StageCreate entry0x401770. Complete sizes: factory85/151,
constructor291/188, initializer478/522, loader308/308, destructor451/499.
Raw/comparable pairs respectively2 and0/131;15 and15/188;48 and43/438;
263 and252/264;20 and12/351. All mismatch; zero exact credit. Native constructor
EBX and destructor EAX differ from target stack/RET4; exception handling also
remains open. The full graph retains early918/1099, late917/929 and the prior
bounds/camera mismatch limits. Ordinary declarations, typed alias externs and
anchors do not establish global storage, full caller ABI or executable closure.

Independent source/target and retained-image review passed. Actual-source host
fixtures cover source construction before clear, pre-captured reverse member
addresses, successful/failing factory, cached reload, relocated pointers,
callback priorities/owner flags, null raw-VM allocation, conditional ANM retain,
replacement-slot deletion identity, and chain capture across lock mutation.
The dispatcher-only fixture explicitly isolates lifecycle; native layout and
EH/x87/private ABI are not host-test claims. Unsafe null factory allocation,
negative counts and malformed input are not executed. CI includes the new test.

Evidence .analysis/gpt-dots-stage-draw/lifecycle-*-target.txt,
lifecycle-attested.json, lifecycle.json, lifecycle-test.log. Corrected report
metadata initially mislabeled the primary source as StageDraw.cpp; it now
names StageLifecycle.cpp, regenerated against the unchanged PE/map/PDB and
all ten contributions verified equal. No compiler rerun was needed for that
metadata-only repair. Required source/storage caveats remain explicit.

Final CI also exposed an older nondeterministic bounds host fixture: actual
Float3 default construction leaves components unspecified. Synthetic camera
aggregates (including the precision witness) and object data are now explicitly
zeroed before chosen values are assigned. No maintained behavior changed.
Focused checks and all 22 CI tests pass without skips after this fixture repair.


## 2026-10-15 Genuine GameManager-to-Stage source binding

GameManagerInitialize0x417870/1009 now calls maintained StageCreate through
its shared header at the two target call sites0x417A9E/0x417B17. Previously
both called the unresolved GameCreateMenu alias. Path and zero priority,
short-circuit order, replay branch and failure destinations are unchanged.
The unrelated zero-argument GameCreateStageOwner (target0x419090) is untouched.
This is a caller identity/source graph repair, not a new startup logic claim.

Fresh GameManager-entry graph: image
86160b10e99afdb1326ed8e534a2da4ad23a1270bf85e9635a199cdabe1a3946,
guarded entry0x4018B0, full initializer1037/1009. The 14 complete contributions
are saved in .analysis/gpt-dots-stage-draw/game-stage.json with matching
PE/map/PDB under build/probe-ltcg/StageDraw-game-stage. Stage body sizes remain
unchanged. StageExecuteScript1972physical compares with provisional1868
(1806 ledger code plus separately audited padding/table); this does not change
ledger ownership or denominator. No exact credit is claimed.

Context-qualified ABI finding: actual callback registration now makes both
draw adapters push ECX, and update adapter convert ECX to EAX before tail jump,
as in the target. This already held in the lifecycle image3cdb24...b564 and is
retained here. The earlier EAX late-adapter mismatch belonged to the draw-only
imagecfbcb2...1c33; it is not a property of this current graph. Other private
ABIs, constructor/destructor EH, global aliases/storage, external dependencies
and runtime closure remain unresolved. Independent full caller binding and
14-contribution/entry review passed. No further Stage spelling controls planned.


## 2026-10-15 Enemy runtime callback scale lifetime correction

Full EnemyRuntimeUpdate0x40DC80/2412 audit found a substantive callback-state
mismatch: source reused the scale cached before the first ECL run when invoking
ECL again after ReleaseEclAllocations, ResetEclState and EnemyRunCallbackEcl.
Target0x40E1F0..0x40E205 reloads both runtime+0x128 scale pointer and its pointed
value after those calls. Only the second argument expression is corrected;
first-run capture/order, bounds, damage and all other logic remain unchanged.

Fresh same-context pinned baselines use the real EnemyManagerUpdateCallback
entry and existing canonical profile. Full owner2358/2412 remains nonexact;
raw-equal191→201, comparable174→182 of2156. Corrected image
97960b3c91145ea44e2a0ff888d2f4d65d49c4677c14175b4f1a14799f631f83
loads [EBP+0x128] then [EDX] at0x401F6D/0x401F73 immediately after callback and
before second ECL call. Complete instructions/normalization checked; ECL helper
calls still use ordinary declarations/data anchors, so private callee ABI and
runnable closure are not established. No exact credit.

Independent source/native review passed. The host regression extracts the
actual affected ECL/damage block and distinguishes pointed-value mutation from
pointer replacement, verifies first-scale ordering, both ECL failure returns,
no-callback/no-damage/gated paths and death return. Movement, collision, VM and
common timer tail are deliberately outside this fixture; host behavior does
not prove native x87 or ABI. CI includes it. Evidence is retained under
.analysis/gpt-dots-enemy-runtime: target.txt, baseline.json, scale-reload.json,
scale-native.txt and scale-test.log. No repeated register-spelling experiment.


## 2026-10-15 Enemy callback installation source identity

Both EnemyRuntimeUpdate callback checkpoints now use shared
EnemyEclHostBaseView::SetSubroutine, target0x450700/30 and inline40E29C..40E2B7.
This resolves the two disconnected RunCallback/InstallCallback aliases: lookup
through the pre-call database, then reload active context for instruction and
again for time zero. Installation does not execute ECL. An explicit lookup-result
local enforces sequencing; the initial assignment-only expression failed the
host context-replacement fixture despite matching the pinned reload sequence.
The retained local preserves the same complete native owner contribution hash.
Existing shared Enemy/Ecl database declaration is reused; no new layout or ODR
view is introduced. The old ledger renderer-state description was incorrect.

Real EclVm.cpp support changes the context baseline from2358 anchor-only bytes
to2367; compare the retained change only against that fresh real-source baseline.
Final owner2375/2412, raw196, comparable181/2156, normalization complete and all
2375 candidate instruction bytes decoded. Entry0x402F50 is the genuine maintained
EnemyManagerUpdateCallback. Final PE SHA256
b3ae1242ec6a15cf79365d5daa7bf9f170c25b045e57143d99a681b9ca5fa409;
reports/probe and native decode are under .analysis/gpt-dots-enemy-runtime/
subroutine-sequenced.json and subroutine-sequenced-native.txt, with separate
ECL-context baseline and earlier expression control retained. Actual Run and
FindSubroutine contributions come from EclVm.cpp, not diagnostic anchors.
First installation is out of line, second naturally inline; both post-lookup
context reloads and prior scale fix survive. The 30-byte installer takes name
in EAX versus target ECX, so private ABI/exactness is open. Other externs,
partial runtime views, global storage and product closure remain open.

Independent source/native/entry review passed. Actual-source host fixtures
retain earlier scale/return tests and add both installation checkpoints,
lookup-side context/database replacement, null lookup and absent callback,
and no additional ECL execution. Lookup mutation is a synthetic lifetime witness,
not a claim that the maintained lookup normally mutates the host. No host test
proves native x87/ABI or observes a mutation between the two plain stores.
No exact credit, helper promotion, or denominator change.


## 2026-10-15 Enemy interpolation and score caller identities

EnemyRuntimeUpdate now calls the maintained AnmVmFloat3InterpolationView::Evaluate
at both target404610 seams, consumes its returned pointer, and calls the existing
GuiScoreView::Add at death publication before EnemyFinalizeDeath. Target score
receiver is474C40. The two undefined free-function aliases are removed. The exact
existing GuiScoreView class is shared via GuiScore.hpp by Enemy, Gui and Item;
Item's token-identical duplicate was also removed and its TU compiled with the
pinned toolchain. Score arithmetic, including existing signed-overflow limits,
is unchanged. ANM header inclusion replaces conflicting local C critical-section
prototypes with its existing void-pointer declarations; actual imports/calls and
Enemy critical-section storage are unchanged.

Enemy and ANM position interpolation views have asserted matching size and all
member offsets. The two reinterpret call seams remain partial object-lifetime/
alias views, not a complete type unification. No dispatcher assignment rewrite.
The diagnostic now links actual EnemyCallback, EclVm, AnmManager and Gui sources.
Fresh same-graph baseline2374/2412 (raw189,174/2156 comparable) becomes2358/2412
(raw178,158/2152 comparable), complete normalization/instruction decode. This
is meaningful source binding recovery, not a byte-match improvement. Final PE
SHA256 dd9cbf04c54a7d9f58c00abdd19150d3817202cc430a65b26313e25c0aa53ac9;
.analysis/gpt-dots-enemy-runtime/connected-identities.json and connected-native.txt
retain full owner/PDB evidence. Both Evaluate calls use EAX receiver/EDI output,
then consume returned EAX. Score uses ECX receiver and RET4; callback resolver
is the real606-byte EnemyCallback contribution, not an anchor. g_GuiScore still
has diagnostic-only anchor storage; no global alias/runtime closure claimed.

Independent review passed. Actual-source tests retain all callback/scale paths,
check score is already updated at death, and test both interpolation call bodies
with distinct returned versus output pointers and lookup-side position mutation.
These are synthetic seam witnesses, not full interpolation arithmetic or complete
Enemy runtime tests. Existing 46-byte score exactness is not newly credited.
All helpers and the full2412-byte owner receive zero new exact credit.


## 2026-10-15 Enemy movement prerequisites, provisional numeric source

Recovered the complete652-byte target two-component evaluator412AC0 and171-byte
motion updater44C2A0 in their real EnemyRuntimeUpdate2412 graph. Four scalar
interpolation calls now use the declared member; three motion calls have real
source. Timer completion/lazy initialization, additive and acceleration modes,
Hermite/shared easing, Cartesian/polar state and floor-to-hundredths are covered.
The target returns the evaluator output pointer. No new subsystem/leaf credit.

Pinned final PE bed76ceb85d0d6f6e67f57317f110073be54da3c21df23723c4517c9dc7faab5,
real EnemyManagerUpdateCallback entry403270. Full runtime2326/2412 raw206,
194/2152 comparable; scalar551/652 raw26,26/612; motion160/171 raw75,73/143.
All three complete candidate extents decode and normalize, but all mismatch.
Reports/native/raw target and precision classification are saved under
.analysis/gpt-dots-enemy-runtime/movement-components*,
motion-interpolation-target.txt and movement-precision.txt.

Native timer FST retains the sum before __ftol2; Hermite t-minus-one float32
spill/reload and3-(t+t) grouping match target. Motion preserves QWORD floor
arguments after multiply100, then float0.01 multiply/store, including negatives.
Known numerical mismatch remains: candidate removes target Hermite term-product
spills and reassociates weighted sums, and removes eased-Y delta/product float32
boundaries. Target locations412C85/8C/95/9B/CA4/CBA/CC8 and412D16/D22 are recorded
against candidate401C00..C3F and401C75..C7D. No equivalence or precision closure
claim. Candidate scalar receiverEAX differs from targetESI; motionESI agrees.
Earlier aggregate control807/652 introduced extra roundings; no more spelling,
volatile/barrier or flag controls pursued. Global aliases/storage remain open.

Independent provisional source/native/test review passed. Actual-source host
fixtures test completion modes0..17 (not active coverage of all easing curves),
fractional and lazy-completion timers, repeatedADD/ACCEL and returned pointers,
nonbinary Hermite both components, Cartesian and nonzero-angle polar motion,
extra flag bits and positive/negative floor behavior. These do not prove x87,
precise threshold/alias behavior or host trig/native equivalence. The fixture
uses a static zero-initialized interpolation seed copied before setup, avoiding
indeterminate flag reads by the maintained constructor; production is unchanged.
CI includes the test. No exact numerator or denominator change.


## 2026-10-15 Enemy runtime tail receiver identities

The two eight-VM loops now call existing ANM world/screen-position setters and
reuse the same cached manager through the later FindVm, matching target EBX
lifetime after loading491C10. The falsely named EnemySpawnDamageEffect alias
is replaced with the actual sound QueueSoundCue call on492590; sound selection
and publication precede both existing ANM timer Add(-1) calls. Null VM clears
stored id, but flash/damage gates still require a valid VM just as target;
this is not a blanket null-safety repair. Partial VM/timer reinterpret views
and global aliases/storage remain unclosed.

Existing queue class is shared among Enemy, EnemyEclDispatcher and Item via
EnemySoundQueue.hpp, with no method implementation change. This is not repo-wide
ODR closure: EnemyLaser/BulletManager/BulletRuntime retain distinct preexisting
views outside this graph. Item also retains its preexisting pointer declaration
for g_MainSoundOwner while Main/Sound/Enemy use array storage; this important
identity risk is queued for its own target audit, not silently repaired here.
The header therefore shares only the class. Item's affected TU compiles pinned.

Real graph now includes EnemyEclDispatcher for sound/angle helpers in addition
to EclVm, EnemyCallback, AnmManager and Gui. Same-context baseline2326/2412
(raw205,193/2152 comparable), final2356/2412 (raw187,175/2144 comparable).
Full decode/normalization complete, no exact claim. Final PE
b66b83e834400966e44baa02a3353e8923b2b099b2baaf38d915889d39f68056,
guarded EnemyManagerUpdateCallback entry4032A0. Evidence is
.analysis/gpt-dots-enemy-runtime/tail-final.json and tail-final-native.txt.
Actual native calls retain EDX manager/ESI position/stack-id setters and FindVm,
ESI sound-owner/EBX sound-id/stack-position queue, and ESI timer/stack-delta Add.
Existing helper exact status is not new credit. Earlier interpolation numeric
mismatches remain open; adding real sources does not establish runnable closure.

Independent full owner/source/native/test review passed. Actual tail, maintained
setter bodies and timer Add are tested with synthetic lookup/sound boundaries:
first lookup replaces the global manager yet all nine calls use the captured
receiver; root-child propagation, non-root suppression, both position spaces,
missing VM with mutation gates off, sound thresholds and timer gating, and sound
mutating timer before fractional decrement are exercised. The sound fixture
owns a real object behind a synthetic byte pointer; it does not prove production
array storage/alias lifetimes. Player selection and dangerous null-gated paths
are not fully covered, and host tests do not establish x87 equivalence.
No exact numerator/denominator changes.

## ITEM sound-owner identity correction (2026-10-15)

The three collection sound calls in the 2,320-byte ItemManagerUpdateBody owner now use the existing `unsigned char g_MainSoundOwner[0x52d0]` array declaration instead of an incompatible pointer declaration. Fresh hash-checked target calls 0x41B446/0x41B68C/0x41B72D load immediate ESI=0x492590 and cues 29/29/20. The former declaration emitted an extra pointer load from the owner itself. Pickup amounts, ordering and the existing shared queue implementation are unchanged.

Reproducible focused evidence: `.analysis/gpt-dots-item-sound/probe.py`, baseline.json, corrected.json, native.py/native.txt and test.log. Real ItemManagerRegisterCallbacks entry and EnemyEclDispatcher QueueSoundCue source are present. Baseline PE f0420bcbd7b53dde57c35347ae037f77b7b05756bb87151ea280cd9b7f5545c1 has a 2276-byte owner, raw115 and 84/1760 comparable. Corrected PE ee7cc215df8b22d68de117dbd1495bcedc934594e22ddc643df31852f5108f58 has guarded entry RVA0x1DA0 and owner0x401490/2272 bytes, raw120 and96/1760 comparable. All three receiver loads are now immediate addresses and bind the real149-byte queue contribution0x402E40. Normalization is complete, but decoding covers2271/2272 physical bytes; the full target ownership remains2320, including alignment and selector tables. This is a semantic receiver correction, not exactness or complete table/ABI closure.

The source-extracted synthetic regression covers both power-collection arms (kinds1/10 and4/11, changed/unchanged) plus the common pickup sound, checking receiver identity, cue/order, item deactivation and preserved0xA5 backing bytes. The actual declaration is compiled with an array definition: the old pointer declaration is rejected at compile time. The0xA5 case is a positive identity/storage-preservation fixture, not a runtime negative control of the old implementation. Other pickup/movement branches are outside this fixture. Diagnostic storage is still a four-byte anchor, not a production0x52D0 allocation. Independent review passed; zero exact-byte credit and no runtime closure.
