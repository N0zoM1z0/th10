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
