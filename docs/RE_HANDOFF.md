# TH10 handoff — 2026-10-30

## Current checkpoint

PlayerUpdateShots428280 now emits the complete876-byte owner, versus738 at
entry, and a fresh12-source cold compilation reproduces all876 native bytes.
It remains nonexact: one LEA is at+111 instead of+142, its skip edge enters+145
instead of+142, and two call fields shift3. Real shared callees, full two-list
secondary release including zero-ID clear, typed ID values and callback/row/
motion lifetimes are restored. Ten strict same-image helpers pass785/all22;
five explicit canonical cold guards pass422/all11. Seven previous native
candidates remain byte/field-identical. No new exact-byte credit. See the
[complete caller evidence](evidence/player-shots-context-20261030/README.md).

The actual ANM WAIT callee Decrement428DD0 is complete20/all1 exact in two
independent canonical cold compilations after preserving Add's observed call
boundary. Add88/all4 and six direct existing exact callers remain exact: combined
3229/all164. Parent ExecuteScript remains9960/9964 with the same four residual
bytes, all350 fields/95 locals. Wrapper origin remains indeterminate; authored
bytes do not increase. See the [call-boundary proof and Wine recovery](evidence/anm-timer-boundary-20261030/README.md).

PlayerFloat3::GetPlanarBounds4285F0 is complete55/all4 exact in two independent
canonical colds. Shared planar member calls preserve the previous nonexact
1172-byte damage and2412-byte root candidates. Origin remains indeterminate;
this adds55 codegen bytes and one mapping, with zero authored-byte credit.
See the [planar source proof and rejected controls](evidence/player-planar-bounds-20261030/README.md).

Player::SpawnEffectRow427B50 is complete181/all1 exact in two independent
three-input canonical cold links. Natural member source restores EDX receiver
and RET20; a captured position pointer restores the remaining stores. The first
cold also preserves the old interrupt helper65/all2. Damage improves to1172/1170
with325/355 diagnostic instructions, still nonexact. See the
[effect-row proof and damage frontier](evidence/player-effect-row-context-20261030/README.md).

EnemyScalarInterpolationView::Evaluate412AC0 is now complete652-byte exact,
including all10 fields, in two independent six-input canonical cold links.
Natural Float2 value operations recover the target temporary/rounding schedule
and ESI/EDI ABI. Its directly affected old runtime constructor145 remains exact
in the unchanged EnemySpawn recipe. See the [scalar proof](evidence/enemy-scalar-context-20261030/README.md).

The complete Stage lifecycle is exact in two independent canonical cold links:
constructor188, Initialize522, destructor499, StageCreate151 and LoadData308.
All1668 bytes/all91 fields pass. This adds1480 authored exact bytes; constructor
origin remains indeterminate. See the [complete proof](evidence/stage-initialize-context-20261029/README.md).

Current generated [progress](PROGRESS.md): **96,725/275,729 authored owned bytes
exact,483 authored exact owners**. Total1359 exact owners/164,822 codegen bytes;
1199 mappings,681 authored mappings,198 authored mapped backlog. Inventory and
whole-product/runtime/portability closure remain separate and incomplete.

The affected old selection passes14 units/2262 bytes/all156 fields in its existing
recipes. It includes the actual Chain interface callers, camera330, Update304
and MainClearPresentTwice135. The900-second multi-recipe batch stopped in its
last FrontEnd build; seven completed units were revalidated from frozen native
outputs, and only the unfinished FrontEnd unit was rebuilt. No completed group
or full historical cohort was rebuilt for recovery.

## Entry rules

Read [AGENTS.md](../AGENTS.md), this handoff and the relevant source/ledger rows.
Use [RE_WORKFLOW.md](RE_WORKFLOW.md) and [ORACLES.md](ORACLES.md) for the selected
lane. Documentation/verification records are retained; old code copies and
rebuildable outputs are disposable, not dependencies of the current workflow.

- Work locally; no Factory transport, receipt or provider prerequisite.
- Verify ignored/read-only resources/th10.exe before target work. Required SHA:
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Use scripts/repo-python. Toolchain declarations must pass --check; execution
  smoke is for setup/change or an actual tool problem. Attest Ghidra before its
  observations; this lifecycle batch used local PE/Capstone/compiler evidence.
- Native jobs stay serial, CPU0/nice10, bounded and logged. Use explicit --unit
  selectors for new/affected owners. Sharing a source/header is not a replay gate.
- Complete owned extents, every field, fixed local destinations and real ABI
  are mandatory. Structural matches and equal sizes grant no partial credit.
- Default CI is seven lightweight checks. Select only affected Oracle self-tests
  when that mechanism changes. No host failure/lifecycle/sanitizer campaigns.
- Commit substantive work as gpt-6.1-sol:, specified author/coauthor; never push.

## Current source facts

- StageCameraStateView::rockingOffset is a real Float3 at+3C; users select x/y/z.
  Four ordinary vector assignments recover Initialize's full temporary schedule.
- copySize/vmBytes are actual size_t allocation requests in LoadData. Their
  capture restores the two target EAX preparations; no register hints are used.
- Chain::AddToCalcChain/AddToDrawChain are real member operations. Actual ABI
  remains ESI element, EDI priority and stack Chain receiver/RET4. Their old131
  bytes remain exact; only source declarations/call syntax and symbols change.
- StageLifecycle includes Main.hpp for SDK IAT imports and uses the actual Main
  critical-section/lock-count fields. It captures the chain before each lock
  and uses real ReleaseAnm, including unsigned bounds and the slot reload/delete.
- The accepted13-input StageCreate recipe adds real GameErrorContext/PbgArchive
  visibility. Only StageLifecycle gains /EHs for observed unwind frames;
  TransitionEffect keeps its existing GL profile. Do not apply these overrides
  globally or infer unique original TU/compiler spelling from them.
- New Stage-array EH binding validates complete37/64-byte generated regions,
  all four/six fields, immutable one/two-state metadata, saved this EBP+4,
  arrays180/1F08, counts8/3, stride3AC and the real34-byte VM destructor/free.
  Factory reuses the existing21-byte allocation binding. No generated-helper,
  runtime or extra ownership credit is granted. The68 focused guards pass
  normally and under Python -O.
- Main's two stale slot0AC references now name Clear; arguments/cast and D3D
  evidence are unchanged. The full old135-byte helper remains exact.

## Working inputs

The current twelve-input Enemy/Player warm context is:
`.analysis/gpt-6.1-sol/player-shots-context-20261030/final-context.json`.
Revalidate all43 recursive project pins,12 object hashes and image hash before
reuse. Actual VmIdInterrupt/FrontEnd definitions use their recorded pinned
profiles; no optimizer flags were swept. A no-reuse cold context is retained
beside it. The older ten-input Player/Enemy context below is historical after
the shared Enemy vector declaration and shot caller changes.

The current five-input ANM warm context is:
`.analysis/gpt-6.1-sol/anm-executor-context-20261030/final-context.json`.
All13 project pins/five object hashes must be revalidated. It retains the actual
/O2 /GL /GS graph and four parent residuals;20/88 pass strict same-image binding.
The older Stage and Player contexts below predate Add's boundary annotation and
retain their own header/source epochs; rebuild changed inputs before reuse.

The retained Stage warm context is:

`.analysis/gpt-6.1-sol/stage-initialize-context-20261029/final-context.json`

It pins source/includes at its recorded epoch, actual profiles and cached objects.
Revalidate every pin before reuse. Its thirteen-input image is diagnostic, not
an executable product. Canonical acceptance remains in config/match-units.toml.
Two independent lifecycle cold witnesses and current records are retained;
older snapshot code/compiled outputs can be regenerated rather than restored.
Cleanup removed4942 unused code/build files (198,807,947 bytes), preserving all
document/verification records, the existing record archive and D3D evidence.
No new compressed archive or historical input dependency is introduced.

The retained ten-input Enemy runtime warm context is separately pinned at
`.analysis/gpt-6.1-sol/player-damage-context-20261030/final-context.json`.
Its36 project inputs, actual profiles and ten object hashes must be revalidated
before cache reuse. It descends from the retained ten-input cold context, recompiling
changed Player/header consumers and reusing only unchanged pinned objects.
At its recorded epoch, damage1172/1170 remains nonexact; planar55/all4,
allocator181/all1 and interrupt65/all2 pass strict same-image checks. Both helper
families have two recorded colds in the three-input Player-constructor recipe:
first55+181+65 passes301/all7, followed by planar55/all4 alone. Older scalar/ECL
witnesses remain retained with their own source epochs.
Older scalar/motion/world/damage contexts are historical inputs; revalidate pins.

The private probe accepts a prior context and optional desired graph separately.
Cache objects are identified using the prior source order, and desired profiles
are compared against the actual prior profiles. Never relabel an old artifact
with a new profile. The earlier profile-cache error is not accepted evidence.

config/transition-effect-host-inputs.json is **retired provenance**: current
exact-stage scripts do not consume those old pins. Do not refresh them for
ordinary source changes or revive removed host suites as acceptance gates.

## Remaining work

StageExecuteScript remains1868/1868 nonexact; VM initializer200/202,
binder197/203 and default camera282/287 also remain nonexact. Typed packed-color
members restore a107-byte dispatcher prefix, not the complete owner. Parked
wrapper/parameter/graph controls receive no credit and need new discriminating
TH10 evidence before repetition. All five lifecycle owners are now closed;
older notes calling them nonexact describe earlier source/build epochs.

For another substantial owner, inspect the current complete ANM executor or
Stage dispatcher against its actual recipe first. Do not assume old four-byte
residuals or archived native sizes still describe current sources. Useful older
[easing](evidence/easing-context-20261029/README.md),
[fog](evidence/stage-fog-evaluate-context-20261029/README.md) and
[projected-bounds](evidence/stage-bounds-context-20261029/README.md) proofs remain
records; their old warm contexts are not current inputs.

Fresh diagnostics on2026-10-30 restore all Stage/ANM experiments. Stage remains
1868/1868 nonexact; definition order, typed instruction colors and initializer TU
placement are neutral, while a precomputed cubic member pointer grows it1904.
The real RGB callee graph reproduces ANM's previously bound9960/9964 frontier;
five-input and fourteen-input images have the same four residual offsets.
The current Add-boundary successor retains them while closing Decrement20; older
68-byte callee records describe previous source epochs.
Record/value/whole-vector permutations repeat old closed controls and are parked.
The current runtime caller emits2412/2412 bytes but remains nonexact. World-vector
composition, bounds, readonly literals, direct direction predicates, animation,
flash/cue and timer forms recover647/671 address-normalized instructions, versus
440/671 at the previous2346-byte checkpoint. Direction's unordered route now
selects zero as in the target. No partial owner or exact-byte credit is granted.
Remaining leads are world Y/Z operand order, early eight-entry count/manager
lifetimes, SetSubroutine name ABI and life-register scheduling. A vector snapshot
grows the body2432; a database capture is neutral. Both are restored and parked.
Motion160/171 is unchanged. See the [current complete runtime record](evidence/enemy-runtime-world-20261030/README.md)
and the earlier [vector recovery](evidence/enemy-runtime-vectors-20261030/README.md).

The early eight-count move is neutral and restored. The damage callee preserves
EBX already; its missing caller count is not explained by simple EBX destruction.
Four full-vector bound homes and a consumed shot iterator retain damage frame40.
The new rectangle-first geometry, lazy Y extents, negative-half literals, row/radius
iterator and direct VM return temporary recover325/355 diagnostic instructions,
versus276 at the preceding1175-byte checkpoint. Damage now1172/1170 remains
nonexact: bound homes, memory-add, rotated Y arithmetic and alignment still differ.
The allocator member is now181 exact, with its actual EDX/RET20 interface.
The root retains its prior normalized bytes and89 local destinations. Whole-vector
operand swap fixes no Y/Z difference; compound snapshot grows2435 and is restored.
Constructed rotation/separated comparison controls are parked. See the
[effect-row evidence](evidence/player-effect-row-context-20261030/README.md) and
[previous bound-storage checkpoint](evidence/player-damage-bounds-20261030/README.md).

Shared planar calls now preserve the complete damage candidate, including41 local
branch destinations, while closing the separate55-byte helper. Original inline
ancestry and helper origin remain unknown. Separate shot-type branches1169,
whole-vector arithmetic1172 and in-place rotation1174 are rejected; nested hit
scope and coordinate-first multiplication are neutral. These controls are parked.
See [current planar evidence](evidence/player-planar-bounds-20261030/README.md).

Player shots now have the complete target-sized876-byte candidate with one
moved motion-pointer LEA and its changed skip destination. Moving the pointer
later or retaining only velocity changes the induction anchor and emits829;
these controls are parked. Do not relax field offsets, branch destinations or
ABI to accept the near match. The shared motion helper160/171 is still open.
The full secondary release must clear its ID even on zero input. The current
43-pin/12-object context and scoped cold proofs are linked above.

The unrelated [D3D adapter-format hypothesis](evidence/d3d-adapter-format/README.md)
remains unadopted and its evidence is preserved. Do not resume its consumed
one-shot experiment from this cleanup. Physical data aliases, original names,
production closure, runtime and portability remain independent unknowns.
