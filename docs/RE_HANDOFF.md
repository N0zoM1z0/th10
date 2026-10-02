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

Current ledger checkpoint (2026-10-02): 1,730 reviewed candidates, 843 source
mappings, 1,084 canonical exact functions, and 102,583 canonical exact `.text`
bytes. Confirmed authored code is 265,662 bytes; the authored source-present
exact backlog is 129 functions, and the Windows i386 product build remains
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
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | Retained 2026-10-01 four-source `/GS` case-order graph: target 14,416 bytes, candidate 14,228 bytes, 820/11,556 normalized comparable bytes; selector 181/181, physical case order 108/108, pre-table 13,760 vs 13,572, suffix 43/43. The owner remains non-exact. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | ANM-082 selected diagnostic graph: 9,964-byte PDB contribution; 2,839/8,599 normalized comparable bytes; target `0xFC` frame; pre-table 9,588/9,588; 92/92 physical groups in target order; all 85 `OR EDI,-1` restores present. The retained `short interruptSentinel` reproduces the target's direct 16-bit opcode comparison; the remaining fallback comparison sign-extends that short against the target's 32-bit interrupt argument lane. |
| `EclVmContext::Run @ 0x0044E1A0` | ECLVM-047: 7,020/7,020; pre-table 6,692/6,692; 931/6,264 normalized comparable bytes; all 59 physical groups retain target order and 56 have target-sized spans. Sum of absolute case-gap deltas falls from 154 to 10 bytes. Case-aligned diagnostics improve from 4,021/5,769 to 5,359/5,730; these are not exact bytes. `StartSubroutine` stays 551/550 and 329/534. |

The current campaign artifacts are under
`.analysis/gpt-6.1-sol/20261002-ecl-start/`. They are convenience snapshots, not
acceptance authority.

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

ANM-069 is the retained **source/TU partition** checkpoint. ANM-070 is the
selected **diagnostic graph** and supersedes ANM-069's 1,723-byte agreement
score:

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
  9,588/9,588 pre-table and 92/92 selector order. This is the only score to use
  for further allocator work;
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
0x100..0x1B4 dispatcher. A fresh selected graph now exists; its remaining gap
is coupled to entry scratch lifetime and per-case layout rather than a missing
dispatcher boundary.

Target-backed constraints to preserve when the dispatcher campaign resumes:

- float cases call generic `EclVmContext::ReadFloat`; the old 0x00412A60 adapter
  is not a dispatcher call seam;
- rank-float selection uses shared ResolveFloat/store tails;
- opcode 0x1A8 writes through the typed 0x210-byte bullet-pattern record;
- target `ReadIntArgument @ 0x00412A00` uses private EAX=runtime, ECX=index;
- opcode 0x1B4 contains two distinct difficulty-selection trees with one generic
  integer-reader join per tree;
- central `EnemyFireLaser @ 0x0041C510` uses ESI=manager, EDI=request and one
  stack type argument (`RET 4`);
- entry scratch and stack-home lifetime are whole-owner problems. Do not use
  dummy padding, fake volatile dependencies or byte patches.

Fresh 2026-10-01 evidence:

- the `/GS` graph with `EclVm.cpp`, `AnmManager.cpp` and `AnmVmCreate.cpp` as
  support, with the retained spawn-case order, emits 14,228 bytes against the
  14,416-byte target and matches 820/11,556 normalized comparable bytes
  (1,087 raw bytes); selector equality is 181/181 and all 108 physical case
  groups retain target order;
- the target pre-table is 13,760 bytes versus 13,572 in the candidate, while
  both suffixes are 43 bytes. The first material layout divergences remain the
  CREATE_ENEMY and CREATE_ENEMY_ABSOLUTE bodies, so no one-line reader or
  difficulty-case edit is currently justified. The fresh raw/layout reports are
  `20261001-enemy-dispatch-case-order-current.json` and
  `20261001-enemy-dispatch-case-order-current-table.json` under
  `.analysis/gpt-web/`.

Fresh dispatcher diagnostic:

    analysis_dir=.analysis/gpt-web
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/EnemyEclDispatcher.cpp \
      --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' \
      --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' \
      --profile-flag=/GS \
      --json > "$analysis_dir/20261001-enemy-dispatch-case-order-current.json"

Use the fresh candidate address with `scripts/report-ecl-dispatch-table.py`.
Never hard-code an address from an old linked image.

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
