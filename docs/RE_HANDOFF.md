# TH10 exact reconstruction handoff

Updated 2026-10-02. This file is the live recovery snapshot for ongoing exact
reconstruction. It is deliberately short: chronological experiments and durable
negative results belong in `docs/KNOWLEDGE_BASE.md` and Git history.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: `resources/th10.exe`.
- Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Work from the live Git worktree, including dirty changes. Do not assume dirty
  edits belong to another session; inspect and either finish, record, or revert
  them deliberately.
- Treat decompiler output, adjacent games, ignored build products and
  `.analysis/` as hypothesis/evidence only. Exactness authority is the canonical
  match ledger plus its replayable units.
- Never use an old candidate address or normalized score after a support source
  changes. Regenerate the selected link graph first.
- Commit substantive progress with `gpt-6.1-sol: ...`.

Before reconstruction work:

    git status --short --branch
    git diff --check
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --execute
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/report-reconstruction-status.py

For this campaign, the operator selects direct Bash/Ghidra instead of Factory
MCP. Attest the read-only local project with
`scripts/repo-python scripts/ghidra.py check` before target analysis. Keep
focused cold replays scoped to the changed source and its exact caller seams.

Current ledger checkpoint (2026-10-02): 1,730 reviewed candidates, 844 source
mappings, 1,084 canonical exact functions, and 102,583 canonical exact `.text`
bytes. Confirmed authored physical ownership is 265,691 bytes after the
29-byte spell-start table/alignment boundary correction; the authored source-present
exact backlog is 130 functions, and the Windows i386 product build remains
open. This checkpoint promotes the 472-byte `SetupVertexBuffer` linked-PE unit
after two independent cold exact replays, in addition to the 202-byte
`CWaveFile::Read` closure. The same broader checkpoint adds 37 source-less VC7.1
`libcmt.lib`/`libcpmt.lib` archive units covering 1,386 exact bytes, replayed
through the archive comparator; it does not claim a full product cold replay.

## Current roadmap

Use this order unless new target evidence changes the dependency graph:

1. **ECL Run first.** Keep the 7,020/7,020 physical owner and exact surrounding
   seams while attacking x87 stack homes, format opcode 0x1E and private register
   allocation. Work on `StartSubroutine` and `ReadInt` when they explain caller
   allocation; do not reduce the task to leaf-only work.
2. **ANM ExecuteScript second.** Keep all 92 physical groups, 85 EDI restores,
   the recovered shared-tail order and current POSITION/POSITION_TIME structure.
   Continue on local lifetimes, interpolation stack homes and remaining block
   placement.
3. **Enemy dispatcher third.** Rebuild a fresh diagnostic from current HEAD before
   evaluating any change. Then address ECL reader conventions, laser construction
   and whole-function stack allocation. Old dispatcher scores are not a baseline.
4. **Close smaller Bullet/ECL backlog in parallel.** Exact leaves are useful when
   they constrain a giant owner, but do not avoid the giant owners. After a batch,
   run focused exact replays; at milestones run full replay/CI.

## Recent focused exact closure

- A direct IDA Pro MCP review identified 37 previously excluded source-less
  runtime bodies: CRT startup/wrappers, C++ EH member-call adapters, and the
  `std::string`/standard-exception family at `0x00462301-0x00462DF5`.
  Hash-attested VC7.1 `libcmt.lib`/`libcpmt.lib` members reproduce all 1,386
  target bytes and every declared DIR32/REL32 field in focused cold replays;
  these remain `library/exclude` and add no source mappings.

- `AsciiManagerView::EnsureSelectionVm @ 0x0040C540` is now canonical exact at
  75/75 bytes under the real `FrontEndControllerView::UpdateShotType` `/GL`
  entry. The maintained source is a value-returning ASCII-manager member; this
  recovers the target private ESI receiver and replaces the earlier 85-byte
  anonymous global helper.
- The unit `ascii-manager-ensure-selection-vm` was cold-built twice and replays
  its sole REL32 field to `CreateVmAtScreenVariant0 @ 0x00448D50`
  zero-difference. Tracking now closes with 252 exact mappings.
- The four `CreateVmAtScreenVariant0/1/2/3` screen-position creators remain
  non-exact at 93/95 bytes in the same real FrontEnd caller graph. Do not
  promote them from the caller result; their target EDI position, EBP hidden
  return and EBX script-index allocation remains unrecovered.
- `PlayerResetRuntimeState @ 0x00424D90` is now canonical exact: the linked-PE
  unit rooted at the `Player::Player` `/GL` entry reproduces all 273 bytes and
  six declared DIR32/REL32 fields, including the private ESI reset ABI and the
  tail jump into `GuiSetLivesDisplayCount`. The unit is focused-replayed after
  the source/manifest change; no full cold replay is required for this batch.
- `Lzss::AddString @ 0x00436000` is now canonical exact: the linked-PE unit
  rooted at `CompressData` reproduces all 516 bytes and 27 declared
  `g_DecompressionRing`/`g_LzssTree` DIR32 fields. The maintained static member
  uses the target-proved `__stdcall` callee-pop surface, recovering EDX
  `newNode` plus the stack `matchPosition` without byte-level tricks.
- `CompressData @ 0x004359B0` is now canonical exact in the same LZSS owner:
  its target `__stdcall` entry and `matchLength <= 2` break-even spelling
  reproduce all 1,029 bytes plus 37 malloc/ring/tree/helper link fields.
- `GameWindowView::CreateGameWindow @ 0x00439730` is now canonical exact:
  the target-bound linked-PE unit rooted at `GameWindowView::WindowProc`
  reproduces all 341 bytes and 20 DIR32 fields twice. Natural source preserves
  the target's private EBX HINSTANCE seam and writes
  `g_GameWindowView.window` before `g_MainSupervisorView.gameWindow`.
- `CSound::Play @ 0x0044D440` is now canonical exact at 145 bytes. Direct IDA
  confirms its private EAX receiver and five helper calls; natural
  `CWaveFile::Read` and `CSound::FillBufferWithSound` bodies restore the real
  callee graph. The linked unit is rooted at source-local `ProbePlayRoot`,
  reproduces all five target-bound REL32 edges, and passes two cold replays.
- `CSound::FillBufferWithSound @ 0x0044D110` is now canonical exact at 483
  bytes. In the same target-bound `/GL /GS` graph, a scoped `inline_depth(0)`
  around only the repeat-loop `ResetFile(false)` call preserves the target's
  first-reset inline and repeat-reset direct-call shape. The new linked unit
  reproduces the complete PDB extent and six target-bound linkage fields in two
  independent cold replays. `ProbePlayRoot` remains compiler-context evidence,
  not a retail-entry ownership claim.
- `SoundPlayerView::StopBgm @ 0x0043DAB0` is now canonical exact at 176 bytes.
  The maintained null-guarded destruction form recovers the target's final
  branch shape. The linked unit uses the real `ReopenBgm` `/GL` entry with
  `src/ZWave.cpp` support and reproduces the complete body plus its `CSound::Stop`
  and three Win32 import linkage fields in two independent cold replays.
- `CWaveFile::Read @ 0x0044DE10` is now canonical exact at 202 bytes. The
  maintained file-mode `else` branch and direct post-`ReadFile` `bytesRead`
  store reproduce the target's shared-epilogue CFG under the real
  `CSound::FillBufferWithSound` `/GL` entry. Two independent cold replays
  reproduce the complete body and its imported `ReadFile` DIR32 field; a
  source-scoped cold replay keeps all 13 configured ZWave units exact.
- `AnmRenderManagerView::SetupVertexBuffer @ 0x004462F0` is now canonical
  exact at 472 bytes. The maintained natural source uses target-supported
  three-float/raw-dword views for the independent background-vertex mirror and
  the target initialization order. The linked unit uses the ABI-correct
  `ExecuteScript` `/GL /GS` graph with `RandomMath.cpp`, `AnmVmCreate.cpp` and
  `AnmVmId.cpp` support, reproduces all 22 target-bound DIR32 fields, and
  passes two independent cold replays.

## Active giant-owner frontiers

All three owners below remain non-exact. Equal size, equal table order or a high
normalized byte score is not exactness.

| Owner | Current live frontier |
| --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | ENEMY-073 six-source `/GL /GS` graph with the real Spawn/GetVm seams and integer spell-name length: candidate 14,384/14,416 bytes, frame 0x2BC/0x2C4, whole-owner 580/11,544 normalized comparable bytes; selector 181/181, physical case order 108/108, pre-table 13,728/13,760, suffix 43/43. Case-aligned diagnostics are 2,475/11,135, with 36 target-sized spans and absolute gap sum 740. The frame gap remains unresolved; no exactness follows. |
| `EnemyBeginSpell @ 0x00409280` | ENEMY-082 current six-TU `/GL /GS` graph: 2,448/2,432 bytes and 520/2,016 structural agreement. Target statistics index reloads and first name-copy loop shape are restored through the existing partial receiver view. Seven-entry table, RET 0x10 and all eight direct-call kinds/counts agree. Initializer private ABIs and allocation remain open; no canonical exactness follows. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | ANM-083 selected diagnostic graph: 9,964-byte PDB contribution; 1,646/8,599 whole-owner normalized comparable bytes; target `0xFC` frame; pre-table 9,588/9,588; 92/92 physical groups in target order; all 85 `OR EDI,-1` restores present. An int sentinel with an explicit short cast only at the opcode comparison restores both interrupt comparison widths. The 203-byte STOP span agrees on all 187 structural comparable bytes; case-aligned agreement improves to 5,324/8,069. Neither interior result grants canonical exactness. |
| `EclVmContext::Run @ 0x0044E1A0` | ECLVM-047: 7,020/7,020; pre-table 6,692/6,692; 931/6,264 normalized comparable bytes; all 59 physical groups retain target order and 56 have target-sized spans. Sum of absolute case-gap deltas falls from 154 to 10 bytes. Case-aligned diagnostics improve from 4,021/5,769 to 5,359/5,730; these are not exact bytes. `StartSubroutine` stays 551/550 and 329/534. |

Current campaign artifacts are under `.analysis/gpt-6.1-sol/` in
`20261002-enemy-spell/`, `20261002-enemy-tail/`, `20261002-enemy-frame/`,
`20261002-anm-execute/`
and `20261002-ecl-start/`.
They are convenience snapshots, not acceptance authority.

## ECL runner: current recovery point

Current retained source facts:

- `Run` remains 7,020/7,020 with a 6,692/6,692 pre-table span and target physical
  opcode-group order. ECLVM-047 retains the target-observed guarded format loop
  and directly reads float operands instead of materializing a union temporary.
  This repairs the large arithmetic/comparison span drift: 56/59 case spans
  now have target lengths, with only JUMP -2, FORMAT +6 and shared NOP/advance
  -2 remaining. The full-owner diagnostic decreases from 975 to 931/6,264
  because most middle destinations remain displaced by two bytes; independently
  aligning physical cases improves diagnostic agreement from 4,021/5,769 to
  5,359/5,730. Neither diagnostic grants exactness. The table reporter now
  exposes aggregate span and destination counts alongside its existing rows.
- `EclVmContext::StartSubroutine @ 0x0044DF70` is now 551/550 with
  329/534 normalized comparable bytes in the selected graph (fresh starting
  HEAD was 151/534). An argument-count guard scopes both loop locals to the
  non-empty argument path and restores EDI=callerInstruction,
  EBP=metadataOffset and EBX=argumentIndex. The initial empty-stack argument
  offset is the target-observed 12. Non-empty return-state saving precedes the
  empty branch; the empty branch explicitly pushes two zero words. Initializing
  preservedValue before previousTop recovers the target initialization order.
  This is a source/codegen checkpoint, not an exact promotion.
- Both target callers consequently emit `MOV EAX,ESI` before `StartSubroutine`.
  `EclVmHost::SpawnThread @ 0x004500D0` is now canonical exact at 142/142 and
  must be protected.
- `ReadInt @ 0x0044FDB0` remains 144/144 with four ordinary comparable bytes
  open. The target reuses ESI for the second typed-stack decrement; the current
  candidate materializes ECX instead.
- Format opcode 0x1E and the arithmetic/comparison cases are allocator-coupled.
  ECLVM-047 supersedes the old guard rejection for the combined guarded loop,
  conversion conditional and direct float read. A guard alone still regresses.
  The former ECLVM-038 displaced stack-home chain is largely repaired in this
  combination; remaining ordinary stack/register differences stay open.
  Arithmetic interleaving still breaks target physical handler order.
- New target machine-code review corrects Ghidra's provisional byte-sized format
  counter: the target initializes EBX=1, increments EBX, and passes the full
  register. Keep the source counter as int. Target format state uses EBX for
  flagIndex, EBP for valueWord, and stack homes +0x10/+0x14/+0x18 for percent,
  scratch and metadataOffset. The candidate instead retains percent in EBP,
  valueWord in EBX and spills flagIndex at +0x34; this is the next allocator gap.
- ECLVM-048 adds a reproducible encoded-ESP diagnostic. On the retained graph,
  56 cases have identical instruction layouts; 355 paired stack operands contain
  94 displacement differences. These are instruction fields, not 94 distinct
  locals or exactness credit. For float ADD, the first-pop home is +0x60 in both
  images, but the second-pop home is target +0x30 versus candidate +0x24, with
  seven differing ESP operands. Other pairs differ by +4, +8, +12 or reverse
  direction; there is no uniform frame-offset correction. Irregular JUMP,
  FORMAT and advance spans are excluded rather than paired heuristically.
- A fresh canonical EnemyRuntimeUpdate-rooted build emits the same runner,
  StartSubroutine and ReadInt diagnostics as the selected Host::Run root;
  Host::Run and SpawnThread independently replay exact in that artifact. The
  remaining gaps are not explained by choosing between these two entry roots.
- The remaining StartSubroutine gaps are now localized: argument/value-offset
  initialization scheduling, integer-source conversion block order, float-argument
  load register, and post-loop EBP=caller / EDX=4 versus target EDX=caller /
  EBX=4. The extra extent byte is the time-word load at +0x163: candidate
  `MOV ECX,[EBP+0]` is three bytes, target `MOV ECX,[EDX]` is two.
  The loop's formerly open EBP/EBX roles are recovered in the retained
  graph. A pointer-first initialization alternative reproduces all first 100
  raw bytes but falls to 303/534 overall; its compact patch remains in the
  campaign for further coupled allocation work.
- Target zero-stack return semantics stay correct: explicit branch-local Push
  calls save two zero words for the empty destination, and caller time plus
  instruction for the non-empty destination. A new scalar return-instruction
  local remains a false WPO frontier (523-byte helper, Run 1,163/6,264, caller
  promoted to ESI). Equal-sized pointer-slot/ternary alternatives retain the
  wrong return-state CFG and must not be selected solely for their 550 bytes.
- Campaign state is **active-incomplete**. Canonical exact totals have not
  increased; the 95% objective remains open. The next bounded route is Run's
  format-state allocation and JUMP/advance cursor lifetimes, followed by the
  remaining StartSubroutine integer-conversion and return-state gaps.

Closed ECL directions that should not be repeated without new evidence:

- An explicit scalar returnInstruction local, or repurposing preservedValue to
  hold the scalar caller instruction after the branch, is a false WPO frontier:
  Run rises to 1,163/6,264, but StartSubroutine collapses and exact SpawnThread
  regresses to 145 bytes because caller is promoted into ESI.

- moving `argumentIndex` before the initial stack-reservation branch creates a
  tempting 1,169/6,264 `Run` score but breaks the target helper ABI (`RET 4`,
  500-byte helper) and regresses `SpawnThread`;
- merely splitting `StartSubroutine` into another `/GL` translation unit does
  not recover the private receiver seam;
- earlier guarded/do-while/goto rewrites shrank the helper toward ~521 bytes.
  ECLVM-046 supersedes the blanket guard rejection only for its scoped guarded
  `for`: the new guarded `do/while` still regresses Run to 793/6,264;
- `volatile firstArgument` and `argumentOffset += 4` are diagnostic-only:
  volatile reaches a misleading 550-byte helper with the wrong EBP/EBX/EDI
  roles, while the offset rewrite over-optimizes the helper to 517 bytes;
- signed `firstArgument`, local identifier renames, declaration-only moves,
  `register argumentIndex`, pre-increment spelling for `firstArgument`, and
  adding `EnemyEclDispatcher.cpp` to the ECL `/GL` support graph leave the
  retained register blocker unchanged;
- stack-object aliases and explicit/specialized `ReadInt` pop expansions cause
  major codegen regressions;
- format-parser declaration permutations and direct arithmetic-local ordering
  probes do not independently solve the whole-function allocator problem.
- On ECLVM-047, refreshing current at JUMP or using a scoped cursor local
  shrinks Run to 6,948 bytes; reading both operands through instructionCursor
  gives 6,956. All retain SpawnThread's diagnostic ABI seam but regress the
  runner and are reverted. The prior jump-reload rejection remains valid for
  these newly tested combinations too.
- Primitive-to-union arithmetic storage is byte-neutral. Format byte/char
  counters, early cursor advancement, mutable cursor parsing, outer percent
  scope and a prefix helper do not improve the retained combined candidate.
  StartSubroutine conversion continues, return-state inline helpers and
  argument-index declarations inside the guard do not improve ECLVM-046.
- ECLVM-048 rechecks arithmetic wrappers under the new format lifetime shape:
  left/both PopFloat wrappers shrink Run to 6,848 bytes, the right wrapper grows
  it to 7,036, and PushInt/PushFloat arithmetic wrappers regress the owner.
  Shared accumulator locals, arithmetic helpers, a Run-wide stack alias,
  pointer/reference cursor rewrites and format for-loop variants also regress
  or remain neutral. Retain the explicit case-local Pop/mutate/Push source.
  Across 54 serialized compiler probes including the baseline, no source
  variant improves the retained owner.
  A context-only formatter argument raises whole-owner agreement to 947/6,264
  but lowers case-aligned agreement to 5,334/5,730, so it is reverted. The final
  scoped replay still passes all 15 configured EclVm exact units across three
  artifacts for 1,473/1,473 bytes; this batch adds no canonical exact bytes.

- the earlier unguarded-loop `sizeof(preservedValue) + 8` probe is a
  context-specific negative: the helper fell to 541/550 and 137/534 normalized
  comparable bytes,
  versus the retained 551/550 and 151/534, while `Run` and `ReadInt` were
  unchanged. ECLVM-046 now retains 12 with the recovered loop-local lifetimes;
  the earlier negative does not prohibit that new combination.

Focused ECL diagnostic:

    analysis_dir=.analysis/gpt-6.1-sol/20261002-ecl-start
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/EclVm.cpp \
      --entry 'src/EclVm.cpp=EclVmHost::Run' \
      --support 'src/EclVm.cpp=src/Enemy.cpp' \
      --json > "$analysis_dir/ecl-run-probe.json"

Read the fresh `candidate_address` for target `0x0044E1A0`, then run:

    scripts/repo-python scripts/report-ecl-vm-table.py \
      --candidate build/probe-ltcg/src_EclVm.cpp/source.exe \
      --candidate-function-address "$candidate_address" \
      --json > "$analysis_dir/ecl-run-layout.json"

Add `--stack-displacements` to diagnose literal ESP offsets in matching
instruction layouts. The report pins Capstone and carries
`acceptance_authority=none`; it identifies neither source locals nor dataflow.
Its target self-comparison pairs 58 cases with zero differences and skips the
final TERMINATE span because candidate gap measurement includes two alignment
bytes that the target code-gap measurement excludes.

After any ECL edit:

    scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp

## ANM executor: current recovery point

ANM-083 supersedes ANM-082's short-only sentinel checkpoint. Fresh read-only
Ghidra and target bytes show `CMP CX,DI` for the opcode and
`CMP [ESI+8],EDI` for the 32-bit fallback argument. The maintained sentinel is
now int, with an explicit short cast only in the opcode comparison. This removes
the candidate's fallback MOV/load/sign-extension sequence and recovers the
complete 203-byte STOP physical span: all 187 normalized structural comparable
bytes agree, and the interior scanner `0x0043F635-0x0043F65F` reproduces all 43
raw bytes. These interior diagnostics have no exactness acceptance authority.

The selected graph remains 9,964 bytes with a `0xFC` frame, 9,588-byte pre-table,
92 physical groups in target order and 85 EDI restores. Whole-owner agreement
falls from 2,839 to 1,646/8,599 because following destinations move by five
bytes; case-aligned agreement improves from 5,188/8,073 to 5,324/8,069, and the
sum of absolute physical-span deltas drops from 298 to 288. Preserve the recovered
comparison widths instead of choosing solely by the whole-owner score.

The final source-scoped cold replay passes all 103 configured AnmManager units
across 20 artifacts for 18,879/18,879 bytes. Reusing its fresh canonical
SetupVertexBuffer artifact reproduces the same non-exact ExecuteScript frontier.
The pinned encoded-ESP diagnostic pairs 68 complete instruction layouts and 66
stack operands, with 51 literal displacement differences; these are fields, not
distinct locals or a dataflow claim. A target self-comparison pairs all 92
layouts with zero differences. The separate 21-byte GetVm private-ABI seam also
passes a focused cold replay. Canonical coverage is unchanged.

New negative probes under this mixed-width shape: boolean-not FLIP assignments
and const/mutable F_SET value locals are byte-neutral. Raw FLIP masks still
shrink the pre-table by 16 bytes and reduce case-aligned agreement, so they are
reverted. F_SET's three stack accesses use target +0x58 versus candidate +0x98;
the longer displacement encodings account for its 84/75-byte span. A named
value alone does not move that home. The next ANM route is coupled float-local
lifetimes and interpolation homes, while retaining both interrupt comparisons.

Current artifacts: `.analysis/gpt-6.1-sol/20261002-anm-execute/`.

ANM-084 closes seven new float-lifetime comparisons plus one compile rejection
under the mixed-width checkpoint. A SCALE_TIME value-returning vector factory
grows the frame to `0x108` and pre-table by 16 bytes. Constructor/output-parameter
forms keep the 9,964-byte owner but grow the frame to `0x100`, despite raising
target-sized spans from 76 to 79; reject that geometry-only frontier. A constructor
on the shared Float2 view first fails VC7.1 C2620 in the vertex UV union; separating
that probe's raw UV pair permits compilation but still gives the wrong `0x100`
frame. This is compiler evidence, not original type/ownership evidence. Generic
float-reader helpers shrink Run to 8,596 bytes and break physical case order.
All source and header edits are reverted; the retained ANM-083 Oracle artifacts
still reproduce their source hashes and frontier. Do not repeat these shapes
without new coupled allocation evidence. Compact matrix/reproducers are in
`float-lifetimes/` under the current artifact root.

ANM-069 is the retained **source/TU partition** checkpoint. ANM-070 is the
historical **diagnostic graph**, before ANM-082/083, and supersedes ANM-069's
1,723-byte agreement score:

- the retained source/TU partition uses AnmManager.cpp as the primary TU with
  RandomMath.cpp and AnmVmCreate.cpp as support; the selected ANM-070 diagnostic
  graph additionally includes AnmVmId.cpp to preserve the exact GetVm private
  receiver;
- AnmLoadedView::CreateVmVariant0 at 0x00448D00 remains in AnmManager.cpp,
  while AnmLoadedView::InitializeVm at 0x00449870 is split to
  AnmVmCreate.cpp; maintained TU names are descriptive only;
- ExecuteScript keeps the target 0xFC stack frame and exact 9,588/9,588
  pre-table span; the PDB contribution remains 9,964 bytes including the
  376-byte absolute jump table;
- the retained source/TU partition can produce 1,723/8,599 in an incomplete
  graph, but that number is not a valid baseline because it omits the
  target-exact AnmVmIdView::GetVm private-ABI context;
- 92/92 physical selector groups remain in target order and all 85 target
  OR EDI,-1 loop-tail restores remain present;
- a fresh source-scope cold replay remains exact at 92/92 configured
  src/AnmManager.cpp units across 17 artifacts, 17,331 matched bytes; the
  four 73-byte CreateVmVariant0/1/2/3 exact units are included;
- tracking validation closes with 251 exact mappings;
- ANM-068's target-observed SCALE_TIME Y-before-X source form remains retained.
  The TU/WPO split improves whole-owner allocation rather than changing script
  semantics;
- ANM-070 adds AnmVmId.cpp to the diagnostic graph because target-exact
  AnmVmIdView::GetVm uses a private ESI receiver. The ABI-correct graph is
  1,676/8,599 while preserving the 9,964-byte owner, 0xFC frame,
  9,588/9,588 pre-table and 92/92 selector order. This is a historical score;
  use the fresh ANM-083 frontier above for further allocator work;
- remaining open work is allocator coloring. Rebuild the saved-game-speed and
  F_MOD/F_COS/POSITION/SCALE slot map from the ABI-correct graph.


Do not regress VM-id semantics or exact render-layer creators to raw integer
APIs just to change layout. The current open classes are POSITION/interpolation
float stack homes, remaining block placement, END/stop ownership and private
helper ABI effects.

Recent negative ANM experiments that are closed absent new evidence include:

- moving `savedGameSpeed` declaration scope alone;
- explicit POSITION `z/y/x` locals;
- introducing a noinline interpolation `ResetTimer` helper;
- aggregate final-position temporaries that expand the pre-table past target.
- direct flag-mask writes for FLIP X/Y (`(flags ^ mask) | 8`) are also closed:
  the local block became target-like, but the selected graph fell to 9,948
  bytes, 9,572/9,588 pre-table bytes and 1,280/8,584 normalized comparable
  bytes. The natural bitfield source is retained.

Focused ANM diagnostic:

    analysis_dir=.analysis/gpt-web/current
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/AnmManager.cpp \
      --entry 'src/AnmManager.cpp=AnmRenderManagerView::ExecuteScript' \
      --support 'src/AnmManager.cpp=src/RandomMath.cpp' \
      --support 'src/AnmManager.cpp=src/AnmVmCreate.cpp' \
      --support 'src/AnmManager.cpp=src/AnmVmId.cpp' \
      --profile-flag=/GS \
      --json > "$analysis_dir/anm-execute-probe.json"

Read the fresh candidate address for target `0x0043EE30`, then use
`scripts/report-anm-execute-table.py` on the fresh linked image. Compare physical
case groups and tails, not only contribution size.

After any ANM edit:

    scripts/repo-python scripts/replay-exact-units.py --source src/AnmManager.cpp

## Enemy dispatcher: current recovery point

The target owner is `0x0040E770`, 14,416 bytes. Maintained source covers the
0x100..0x1B4 dispatcher. The 2026-10-02 source checkpoint connects its spawn
calls to the existing `EnemySpawn` implementation, computes signed spawn
argument indices before clearing the request, and stores projected Y directly
before loading Z. The later ENEMY-073 checkpoint corrects the spell-name length
to an integer load and restores the source-pointer decode and difficulty switch
shape. Its frame is now eight bytes short; the earlier float conversion must
not be restored merely to recover frame size. The owner remains non-exact.

Target-backed constraints to preserve:

- the three calls to `EnemySpawn @ 0x0040CFB0` pass the request in EAX and
  manager/name on the stack; the callee returns with `RET 8`. The former
  ordinary-C `EnemySpawnFromEclInstruction` declaration had no implementation
  and was a different unresolved symbol, so its caller graph was insufficient;
- include `Enemy.cpp` as LTCG support to observe the real spawn seam. Its
  maintained `__stdcall` prototype lets LTCG select the private EAX argument;
  do not encode a false source calling convention or write a shim;
- target spawn length bias precedes `REP STOSD`, with the final shift after it.
  Natural signed division before clearing restores three separate spawn calls
  in the selected graph. The candidate still shifts before clearing and keeps
  instruction/index in ESI/EDI versus target EDI/ESI;
- target projected Y uses one memory `FADD` and one vector store before Z.
  The retained direct assignment removes the extra candidate `FLD`/`FSTP ST(0)`;
- target has five calls to `AnmVmIdView::GetVm @ 0x00449450`, using ESI. Include
  `AnmVmId.cpp` to recover that callee ABI. The selected six-TU graph keeps
  all five calls; the unchanged-source five-TU control inlines one;
- float cases call generic `EclVmContext::ReadFloat`; the old 0x00412A60 adapter
  is not a dispatcher call seam;
- rank-float selection uses shared ResolveFloat/store tails;
- opcode 0x1A8 writes through the typed 0x210-byte bullet-pattern record;
- target `ReadIntArgument @ 0x00412A00` uses private EAX=runtime, ECX=index;
- opcode 0x1B4 contains two distinct difficulty-selection trees with one generic
  integer-reader join per tree;
- central `EnemyFireLaser @ 0x0041C510` uses ESI=manager, EDI=request and one
  stack type argument (`RET 4`);
- spell-start variants load the signed length directly from instruction+0x1C,
  test it with `JLE`, and decode bytes from instruction+0x20. No x87 conversion
  is present. Target destination is `ESP+0x248`; a named source pointer lets
  VC7.1 recover the target source-minus-destination addressing shape;
- spell difficulty dispatch sign-extends the opcode and uses `SUB 0x165` and
  two `DEC` tests. The retained inner switch recovers this shape. Operand 2 is
  still read before operand 1; its observed spill does not establish a fifth
  `EnemyBeginSpell` argument. Preserve the call without inventing a parameter;
- `EnemyBeginSpell @ 0x00409280` now has a complete maintained body in
  `EnemyEclDispatcher.cpp`, including all seven stage arms; it remains non-exact.
  Its physical owner is 2,432 bytes through `0x00409BFF`: 2,403 executable bytes,
  one NOP and a seven-entry switch table at `0x00409BE4`. The table is indexed
  by the global at `0x00474C7C` minus one after an unsigned upper guard of six;
  all destinations are internal instruction boundaries. The next independent
  owner begins at `0x00409C00`. Future exact comparison must include the table;
- the spell helper takes four stack arguments and ends in `RET 0x10`.
  Ghidra omits private arguments at several calls: sound id 14 is live in EDI,
  and loaded-script setup consumes the explicit VM receiver and script index.
  Neither missing decompiler arguments nor the caller's extra operand read
  justify inventing a fifth parameter;
- entry scratch and stack-home lifetime remain whole-owner problems. Do not
  use dummy padding, fake volatile dependencies or target-byte patches.

Prior evidence under `.analysis/gpt-6.1-sol/20261002-enemy-dispatch/`:

- the prior four-TU graph is reproducible from starting HEAD `2ce82ff`:
  14,228 bytes, 820/11,556 normalized comparable bytes, 181/181 selector
  entries and all 108 physical groups in target order;
- the ENEMY-071 six-TU `/GL /GS` graph emits 14,340 bytes, frame `0x2C4`,
  677/11,540 whole-owner normalized comparable bytes, 13,684/13,760 pre-table
  bytes, 181/181 selector entries and all 108 physical groups in target order;
  both suffixes remain 43 bytes. All three spawn calls have the target private
  argument contract; existing canonical field/data declarations replay the
  linked Spawn and GetVm contributions at 577/577 and 21/21 bytes;
- separate case-aligned diagnostics compare 2,574/11,125 bytes, with 34 spans
  having target sizes and an absolute span-gap sum of 754. These scores have
  no acceptance authority. The shared position/life tail is still after the
  absolute-create block instead of before it; request base is candidate
  `ESP+0x240` versus target `ESP+0x248`;
- the smaller real-spawn/direct-Y candidate merges three spawn calls into two
  and is superseded despite its 14,148-byte extent. Repeated-tail variants
  reduce some aggregate gaps but duplicate the target shared-tail topology;
  absolute-first source order breaks the physical group order. Extra spawn
  locals do not recover register roles. Retained patches/matrix distinguish
  these negative results from the current checkpoint;
- focused canonical source replay protects all 14 existing dispatcher units
  across three artifacts, 860/860 bytes.
  No dispatcher or additional canonical bytes are promoted. The diagnostic
  graph does not replace the existing exact-unit manifests.

Current evidence under `.analysis/gpt-6.1-sol/20261002-enemy-tail/`:

- source SHA-256 `a8daaa62869a65467ceac8b1dd5f334368d9b3f5ea99b59af5899a8747fc35a6`
  emits 14,384 bytes in the same six-TU graph. Candidate frame is `0x2BC`
  versus target `0x2C4`; whole-owner agreement is 580/11,544, pre-table
  13,728/13,760, selector 181/181, physical order 108/108 and suffix 43/43;
- case-aligned diagnostics are 2,475/11,135, 36 target-sized spans and absolute
  gap sum 740. Spell start is 222/219 bytes without an x87 conversion. Name
  and spawn-request bases are now candidate `ESP+0x238` versus target
  `ESP+0x248`; register/home allocation and shared-tail placement remain open;
- three Spawn and five GetVm calls remain. The frozen selected image is
  `build/analysis-enemy-tail-selected/source.exe`, SHA-256
  `32d9fa527002341194415c6e7fd9f0b3249c950bea5a5eaacc00a64fbde90a09`.
  Existing canonical seam declarations replay 577/577 and 21/21 bytes;
- focused canonical source cold replay passes 14 units across three artifacts,
  860/860 bytes. No new canonical matches or bytes are promoted;
- scoped/reused integer locals, cursor variants and a shared-word union do
  not recover the frame. The union has no independent TH10 ownership evidence
  and is discarded. Compact trial results and reproducible drivers remain;
- `ghidra.py query OUTPUT disassemble_from COUNT ADDRESS...` starts at an
  instruction boundary inside its containing function. Existing `disassemble`
  still starts at the function entry. The wrapper requires a query completion
  marker and rejects Ghidra script errors even when Ghidra exits zero. Actual
  checks pass for an interior spell query and the legacy operation, and reject
  a middle-of-instruction address and an address without a function.

Further frame investigation under `20261002-enemy-frame/` reproduces the
retained frontier from `7aac8a6`. Unsigned-short opcode forms, a scoped decode
for-loop and scoped cipher locals leave the measured frontier unchanged. A signed-int opcode raises
whole-owner agreement to 920/11,544 but lowers case-aligned agreement to
2,431/11,135 and retains the wrong frame. Separating the name buffer from the
request overlay, while preserving its current 0x84 capacity, recovers the
219-byte spell span but grows the frame to 0x304 and the parent to 14,432 bytes;
it is rejected. All source trials are restored. The instruction-sequence ESP
diagnostic pairs only cookie loads in this graph and does not locate a missing
source local. Callee name copies do not prove dispatcher buffer capacity.

Earlier spell checkpoint evidence under `20261002-spell-inline/`:

- the previous body used `chapter +0x44` for both the bonus stage and stage
  selector. This was wrong despite its normalized structural diagnostics.
  Target ECL opcode 0x158 supplies receiver `0x00474C40` to `EnemySetChapter`,
  which writes +0x44 and conditionally resets +0x4C. Spell bonus and stage
  selection instead read `0x00474C7C` (+0x3C); stage 7 compares
  `0x00474C84` (+0x44) with 24. The source now separates stage and chapter,
  with offset assertions. `report-enemy-spell-state.py` reproduces these
  target-only relationships; full object/data ownership remains unknown;
- remove the blanket FindVm noinline declaration so the spell's three searches
  can inline. Preserve target calls with scopes at seven manager and six VM-id
  call sites, and retain the spell's final AddVmVariant0 call with its own
  scope. These compiler controls do not prove original source settings;
- a blanket declaration-only removal broke an existing interrupt unit at
  116/54 bytes. A scope around EnemyMarkPendingInterrupt's whole initializer
  also stopped its temporary VM-id constructor from inlining, producing
  82/65 bytes. That unnecessary scope is removed; its natural body replays
  65/65. Keep these negative controls when changing the policy;
- source SHA-256
  `94cc84a6af620f0848e5e7faed54b3c71c70a97b0e2cced8c44d3ef9062c3f11`, with
  all seven input hashes in `final-input-hashes.json`, emits 2,416/2,432 spell
  bytes in the fresh six-TU `/GL /GS` graph, structural agreement 245/2,004.
  Its complete owner has the seven-entry table and `RET 0x10`. All eight
  static direct-call kinds/counts now agree, including zero FindVm calls and
  one final AddVmVariant0. Static sites include alternative arms;
- the two stage reads and chapter comparison are checked by their arithmetic
  and control roles against the resolved partial-view anchor. Diagnostic data
  anchors are four bytes each; large view addends can overlap other anchors,
  so absolute candidate-address filtering alone is ambiguous. Normalized
  structural agreement is not a source-global binding proof;
- the parent retains 14,384 bytes, frame 0x2BC, 580/11,544 whole-owner agreement,
  2,475/11,135 case-aligned agreement, 181 selectors and all 108 physical groups.
  Three Spawn and five GetVm calls remain. Whole-owner address normalization
  is incomplete in this size-mismatched parent; the score has no acceptance
  authority. Frame/home/tail issues remain open;
- frozen `build/analysis-spell-inline-final/source.exe` has SHA-256
  `1c81ba0efece75531089a189f27321855e8f397a71b014cdd51bb45a110d1818`.
  Six existing complete canonical seam declarations replay 977/977 bytes in
  this graph: Spawn, GetVm, QueueSoundSample, FindVm, EnemyMarkPendingInterrupt
  and AddVmVariant0. Canonical source cold replay passes 14 dispatcher units
  across three artifacts, 860/860; focused lookup-dependent replays protect
  16 units, 971/971 bytes. These sets overlap at EnemyMarkPendingInterrupt.
  A final replay also protects VM-id Release (21 bytes), for 30 unique existing
  units/1,787 bytes;
- `build-final.py`, `inspect-final.py`, `score-final.py`,
  `review-final-bindings.py` and `replay-final-seams.py` reproduce the selected
  graph/reviews. ANM initialization private ABIs, statistics reload/copy-loop
  allocation and original class/TU/data ownership remain unresolved. No new
  canonical matches, runtime validation or whole-product closure are claimed.

The preceding reset/context checkpoint is retained under
`20261002-spell-anm-abi/`:

- ANM-085 supersedes the old separate reset overload model. Target reset
  `0x00401DE0` uses EDX for the VM, overwrites ECX before using it, and writes
  the low flag word. The ordinary `AnmVmView::Initialize()` now owns the
  complete reset body; the static unused-context overload is removed.
  `/GL` naturally emits the target EDX ABI without a forced declaration;
- `anm-vm-initialize-private` keeps its existing unit ID and target extent,
  but now proves the actual member in the ExecuteScript `/GL /GS` graph.
  Two independent cold canonical replays pass the complete raw-equal
  273-byte member with no link fields and preserve SetupVertexBuffer's
  472 bytes. Focused caller replay preserves RemoveVm and four base creators,
  560 bytes. The target's nine direct reset calls span seven owners; only
  RemoveVm is already canonical exact among those direct caller bodies;
- the six canonical units rooted at the ordinary reset member and all
  fourteen dispatcher-owned units also cold-replay exact: twenty units,
  four artifacts, 1,310 bytes. Combined with the reset/setup/caller subset,
  this protects 27 distinct canonical units and 2,615 bytes; no full-game
  cold replay is performed;
- the final current-source six-TU Enemy image is
  `build/analysis-spell-anm-final/source.exe`, SHA-256
  `691abb1b73fd14663e395fba81bbf268f32e779e894b366c205152eb2a18ebb1`.
  Seven complete existing seam declarations pass 1,250/1,250 bytes, including
  the actual reset member. All source/header hashes are in
  `final-input-hashes.json`; AnmManager.cpp is
  `a28276a33b0400d85578fc5e7a4e43ccce1c9cb9101faa0546008b40851e9f8e`
  and AnmManager.hpp is
  `d5449aee1016df88ad3ff297a0cc7150f3c855f9a2776ce14caba989d900cc5e`;
- EnemyBeginSpell still emits 2,416/2,432 bytes with 245/2,004 structural
  agreement; its eight direct-call kinds/counts agree. The parent still
  emits 14,384 bytes with frame 0x2BC, 181 selectors/108 physical groups,
  three Spawn/five GetVm calls and 2,475/11,135 case-aligned agreement.
  Whole-owner address normalization remains incomplete;
- real RandomMath and BulletManager/BulletTransform support, all retained
  `/GL`, and a typed spell receiver leave the measured frontier unchanged.
  Rooting InitializeVm restores its incoming public ABI but emits 207/217
  bytes; natural flag forms and `/G6` do not improve it, and `/G7` emits 204.
  Do not infer normal-COFF ownership from its standard incoming ABI. The
  initializer private binder conventions and statistics reload/copy shape
  remain the next coupled route. `matrix.json` distinguishes completed
  controls from unrun branches in retained trial drivers;
- this checkpoint replaces an existing exact proof and removes duplicate
  source, so canonical exact totals remain unchanged. It does not prove
  original source ownership, native product closure or runtime behavior.

The current scalar/binder checkpoint is retained under
`20261002-anm-initializer-binders/`:

- ENEMY-082 places the spell bonus input and two statistics indices at
  +0x0C/+0x28/+0x2C in the existing partial receiver view at 0x00474C40.
  Their old independent global aliases let the compiler retain the first
  record pointer across strcpy. The target reads both indices again after
  that copy; the maintained view restores those reads and the target's
  two-pointer-increment copy loop. Offset assertions retain the existing
  stage/chapter/timer layout; original data/class ownership remains unknown;
- the current spell emits 2,448/2,432 bytes, 520/2,016 structural agreement,
  seven-entry table and RET 0x10. All eight direct-call kinds/counts agree.
  The parent retains the 14,384-byte, 0x2BC-frame frontier above. These are
  diagnostic measurements with no exactness promotion;
- `selected-input-hashes.json` binds all seven source/header inputs.
  EnemyEclDispatcher.cpp is
  `92da4c27a2105399d482374b08d94196d698e70c4db23a4c701f899ba4827b55`;
  the frozen selected image is `build/analysis-anm-binder-selected/source.exe`,
  SHA-256 `4c7078b7043dab527a4209df0320b8816d316205cda6cb5d3f1b2378aef3464b`.
  Role-aware scalar reviews are necessary because four-byte diagnostic data
  anchors can overlap large member addends. Eight complete existing canonical
  seam declarations pass 1,444/1,444 bytes in this graph, including the ordinary
  reset member and SetAndExecuteScriptIndex;
- ANM-086 closes flags-first and member-shaped-spell controls as raw-neutral
  across seven complete contributions. Separate reset and loaded-initializer
  TUs, retaining /GL for every input, do not improve helper ABIs or the owner
  frontier. One missing-include compile rejection is recorded separately;
  all trial partitions are restored. Fresh complete target decoding corrects
  InitializeVm's old ledger note: four base creators call 0x00449870, while
  eight screen/world-position creators call 0x0043E710. The existing creator
  /GL graph gives InitializeVm a standard incoming ABI but still 207/217 bytes;
- BULLET-016 repairs two pre-existing canonical failures discovered by the
  affected-source replay. Cold old-source controls also yield 424/431 bytes in
  UpdateAimedDirectionChange and 247/249 in UpdateBullets: shared Tick loads
  scale before subframe, while these target owners load subframe first.
  Direct aimed expansion and a plain local remain mismatched. Ordered reads
  of the actual subframe input scoped to these owners' scaled paths restore
  all 431 and 249 bytes with twelve and six existing linkage fields in first
  cold builds. The shared header is unchanged; original volatile qualification
  and source factoring remain unknown;
- final focused protection covers 31 distinct canonical units across eleven
  artifacts, 4,676/4,676 bytes. After the fail-fast manager discovery, eighteen
  successful cold-prefix units are replayed against unchanged dependencies;
  only the thirteen Bullet-dependent units are rebuilt after its repair.
  `final-protection.json` binds both sets and avoids repeating unrelated builds.
  Each repaired timer owner passes two independent cold builds against the
  final BulletManager.cpp source, SHA-256
  `f8411f6f85ffafef638aef607d774d2e057635942320932f4e8ba24c6525d24a`;
  see `timer-repair-two-cold-builds.json` for the separate receipts.
  No full TH10 cold replay or new canonical byte promotion is performed;
- `build-selected.py`, `inspect-selected.py`, `score-selected.py`,
  `review-selected-bindings.py` and `replay-selected-seams.py` reproduce the
  selected graph/reviews. Initializer ABIs, later allocation, original
  production partition and native product/runtime closure remain open.

The focused target-bound boundary replay passes and the fresh inventory now
assigns the seven-entry spell table to its caller, eliminating the 29-byte gap.
The unmodified global `report-boundary-inventory.py --check-ledger` already
failed before this correction at `0x00401100` because historical manual ledger
evidence text differs from regenerated audit text. No broad ledger rewrite is
performed; unrelated reviews and existing global audit diagnostics remain.
The next coupled route is the existing spell-start body's remaining ANM
initializer private ABIs and later allocation, with the corrected scalar and physical
boundary. Canonical exact totals are
unchanged and the 95% objective remains open.

Query the target spell block directly:

    scripts/repo-python scripts/ghidra.py query \
      .analysis/gpt-6.1-sol/20261002-enemy-tail/from-spell.txt \
      disassemble_from 70 0x00410E7B

Rebuild the selected diagnostic:

    analysis_dir=.analysis/gpt-6.1-sol/20261002-enemy-tail
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/EnemyEclDispatcher.cpp \
      --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' \
      --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmVmId.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/Enemy.cpp' \
      --profile-flag=/GS \
      --json > "$analysis_dir/retained-probe.json"

Use the fresh candidate address/extent with `report-ecl-dispatch-table.py`.
Never reuse an address from an overwritten image. Retained artifact bindings,
seam assembly and trial summaries are recorded in the campaign manifest.

## Smaller backlog routing

Use the generated backlog instead of copying a long static task list here:

    scripts/repo-python scripts/report-exact-backlog.py
    scripts/repo-python scripts/rank-exact-backlog.py --source src/EclVm.cpp
    scripts/repo-python scripts/rank-exact-backlog.py --source src/BulletManager.cpp
    scripts/repo-python scripts/rank-exact-backlog.py --source src/BulletTransform.cpp

Important current routing facts:

- ECL `SpawnThread` is exact; do not list it as backlog again.
- The Bullet runtime/collision pair already has canonical exact coverage; spawn,
  transform and several direction/wrap helpers remain open.
- Smaller exact units should be pursued when they constrain call graphs or close
  real source-present backlog, but not as a substitute for the three giant owners.

## Regression gates and checkpoint discipline

Protect exact units sourced from every translation unit you edit. At minimum:

    scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp
    scripts/repo-python scripts/replay-exact-units.py --source src/AnmManager.cpp
    scripts/repo-python scripts/replay-exact-units.py --source src/EnemyEclDispatcher.cpp

At a broader milestone run:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --execute
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/report-reconstruction-status.py
    scripts/repo-python scripts/ci.py
    git diff --check

Commit substantive, verified progress promptly with `gpt-6.1-sol: ...`.

## Local analysis retention

`.analysis/` is disposable scratch. On 2026-09-27 the active
`.analysis/gpt-web/` campaign was compacted from roughly 63 MiB of one-off
experiments (85 non-current directories plus 225 root snapshots/scripts) to a
single `.analysis/gpt-web/current/` directory of about 1.2 MiB.

The legacy `.analysis/gpt-web/current/` snapshot contains:

- `README.txt`
- `ecl-run.json`, `ecl-run-probe.json`, `ecl-run-layout.json`,
  `ecl-exact-replay.json`
- `anm-execute.json`, `anm-execute-probe.json`,
  `anm-execute-layout.json`, `anm-exact-replay.json`

The compact JSON files identify the selected graph and frontier; the raw files
preserve the corresponding probe/layout/replay evidence. They are still not
exactness authority: canonical exactness lives in the tracked ledgers and
replayable match units.

The active ECL checkpoint is `.analysis/gpt-6.1-sol/20261002-ecl-start/`.
Its retained probe, layout and focused replay bind to this source checkpoint;
`target-prologue-alternative.diff` preserves the unresolved pointer-first
initialization alternative. Do not trust the last experiment's mutable build
image: regenerate the retained graph before using addresses.

Historical `.analysis/...` paths in `docs/KNOWLEDGE_BASE.md` are provenance
labels and may no longer exist. If an old result becomes relevant again,
regenerate it from current source rather than relying on an old candidate image.
Do not preserve `*-before.cpp`, ad-hoc slot-map scripts, rejected permutation
matrices or stale candidate PE/PDB/MAP files merely because an old note mentions
them.

Do not delete the canonical target, pinned toolchain, tracked build/config files
or provider-owned Ghidra state as part of scratch cleanup.
