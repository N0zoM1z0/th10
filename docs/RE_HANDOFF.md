# TH10 reconstruction — next-agent handoff (2026-11-05, updated source epoch)

**Start here.** The latest tested feature-source checkpoint is **`588851e`**
(`gpt-web: unify ECL stack scalar storage through full VM readers`), after
`79ac30a` (ANM pointer mask) and `0b51d9d` (Enemy callback life). The
older `2061ddf` and `0dd8b20` source-epoch statements are **obsolete**.
Always check live `git status` and source hashes: a documentation checkpoint
may move HEAD without changing these source inputs, and private artifacts
can belong to older source epochs.
The task is target-exact reconstruction, **not** approximate behavior or a
runnable full-game claim. Work on the large interpreters **and** independent
leaves; checkpoint substantive source progress as `gpt-web: ...` with the
repository's `AGENTS.md` author/trailer convention. Never push without
explicit authorization.

## First five minutes

```sh
git status --short --branch && git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/ci.py
```

- Only `resources/th10.exe`, Japanese TH10 v1.00a, **487,936 bytes**, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
  is the original oracle. It is **private/ignored**: never commit it.
- Inspect **all** dirty files; do not discard a change because it looks like
  another agent's work. Read `AGENTS.md` for native toolchain and source rules.
- Genuine VC7.1 SP1 `/GL` LTCG/normal COFF owner extent, original-bound
  relocations, local branches/tables and ABI are part of byte-level proof.
  Candidate size, candidate-to-candidate stability and a Git commit alone
  grant **zero new exact credit**.
- For exact regressions, use `scripts/repo-python scripts/replay-exact-units.py`
  with explicit **`--unit`** selectors; **never run historical full cold
  replay**. Build only the affected source graph, with fresh recompilation or
  hash-attested transitive support objects.

## Current canonical ledger

`docs/PROGRESS.md` (generated from the ledgers) controls acceptance:
**1,829/1,829** target boundaries reviewed; **1,309** source-present
mappings; **1,482** fully original-bound exact functions / **189,797**
exact codegen bytes; **574** confirmed-authored exact functions / **119,761**
authored exact bytes; **176** confirmed-authored source-present nonexact
backlog. These are neither source completeness nor whole-product closure.
**No new canonical exact units were admitted by the three latest
feature-source commits**; affected existing exact leaves were selectively
replayed under the new source revisions.

## Latest whole-owner frontiers: do not confuse size with target agreement

| Original owner | Original / current complete candidate | Verified frontier and unfinished work |
| --- | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | **14,416 / 14,352** | Fresh **11 actual translation units**, **716 candidate fields**. 64-byte physical gap; independently bind the original switch/table targets before acceptance. **NONEXACT**. |
| `EnemyBeginSpell @ 0x00409280` | **2,432 / 2,432** | 108 candidate fields; former 2,424 candidate is stale. Same size **does not** establish target-byte identity. **NONEXACT**. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | **9,964 / 9,964** | **9,962/9,964 original bytes**, full **350 independently target-bound fields**, 95 local destinations. Opcode 91 two register bytes **+0x113/+0x115** (target EDX, candidate ECX). **NONEXACT**. |
| `EclVmContext::Run @ 0x0044E1A0` | **7,020 / 7,020** | **6,997/7,020** original bytes, **189 original-bound fields**. 23 remaining ordinary bytes: operand/register choices, `LENGTH_SQUARED` x87 square order, `POINT_ANGLE`, FORMAT stack homes. **NONEXACT**. |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @ 0x004078E0` | **335 / 335** | **333/335 preliminary ordinary bytes** after masking 8 candidate linkage fields; remaining `+0xD7/+0xDD` EAX vs target EDX. Complete independent field-identity binding remains unaccepted. **NONEXACT**. |
| `BulletRuntimeView::UpdateVectorAcceleration @ 0x00407560` | **305 / 285** installed | See *active lead* below: genuine seven-source trial reached **305 candidate bytes** but only **196/305 positional bytes**; **not installed** and **not exact**. |
| `EnemyFullObjectView` constructor `@ 0x0040D830` | **673 / 672** | 5 candidate fields, frame/register schedule differs. **NONEXACT**. |
| `EclVmScriptDatabase::AddScriptData` | **589 / 570** | Needs full script-table insertion/control-flow recovery. **NONEXACT**. |

Other independent leaves worth revisiting **with a new falsifiable lead**:
`CMemoryPbgFile::Seek` original/candidate **107/107**, **100/107** ordinary
bytes (7 register/jump bytes); `SoundDataLoaderThread` **107/112**;
`Chain::AllocElem` **74/74**, **52/74** original bytes in its verified
context; `GameStatisticsOwner` ctor **132/137**;
`TextRenderBufferView::InvertAlpha` original **157**, real LTCG
**138/157** but normal COFF **112/157** (the retained `JS` at `+0x69`
is already correct). They must **not** be marked exact.

## Current-source proofs and candidate epoch pins

| Source family | Installed source recovery | Relevant retained original-bound proof / witness |
| --- | --- | --- |
| ANM | Header +6 has one typed 16-bit variable-argument mask. Now **all four** value/lvalue instruction macros in the full 9,964-byte executor use `variableArgs.bits` | [Typed pointer mask](evidence/anm-typed-pointer-operand-mask-20261105/README.md): fresh five-source strict **350/350** original-bound fields, including 95 locals; **9,962/9,964** original bytes, still NONEXACT. Affected pointer leaves **288/288 exact**. Private `build/gpt-web/20261105-anm-pointer-mask/current/`. Earlier full-mask [proof](evidence/anm-instruction-variable-mask-overlay-20261105/README.md) belongs to its own source epoch. |
| Enemy dispatcher / GUI | A single typed `itemDrops` 0x3C-byte packet at runtime +0x13CC; named life-gauge baseline +0x13C4 and callback-life difference +0x13C8 across lifecycle, ECL dispatcher, callback helper and GUI | [Current callback life](evidence/enemy-next-callback-life-20261105/README.md): fresh 11-TU complete ECL dispatcher **14,352 candidate bytes / 716 candidate fields**, byte-neutral against the preceding candidate, still NONEXACT versus 14,416 original. The callback helper now compiles standalone with a necessary forward declaration; selected GUI leaf **46/46 exact**. [Nested drop packet](evidence/enemy-item-drop-packet-20261105/README.md). |
| Generic ECL VM | Runtime thread signal bit, typed CALL/SCPT records, and now the shared 32-bit `EclVmScalar` word in six value/lvalue stack accessors | [Typed stack scalar](evidence/ecl-typed-stack-scalar-20261105/README.md): fresh two-source full Run **7,020/7,020**, **189/189** original-bound fields, **6,997/7,020** ordinary bytes; six affected leaf owners **773/773 exact**, Run NONEXACT. Private `build/gpt-web/20261105-ecl-stack-scalar/current/`. |
| Bullet | Both state `value1Bits` and runtime `angleBits` overlap binary32 payloads; full 335-byte direction updater preserved | [Runtime angle](evidence/bullet-runtime-angle-word-20261105/README.md): seven-source native proof of 8 candidate field positions, two remaining register bytes (+0xD7/+0xDD), existing leaf replays **312/312 exact**. Local outgoing-angle union trial on this current source was rejected with no byte improvement. |

Source SHA-256 at this HEAD (verify before reuse):

```text
Enemy.cpp          c08272bf324217b9e6e4ffa91cd13fb2555aeb37b44be6231075082ebfcac5d2
Enemy.hpp          f7b23495539c4ab85b6a37ff02580714d5ab64c77117f7d615052d2b3f513c11
EnemyCallback.cpp  705b2537af9e117946e4626de97fd195efb7ee592681df4e82002d7d282ac745
EnemyEclDispatcher.cpp a553dd30b547f96d753e0e5edd5c4adccdfce1de0c40ed9e8336534188ad87f4
Gui.cpp            375d5b39c583fce37b4a5f29b25f28de7d5f7400940ef580e8fd47e7650efc09
EclVm.cpp          21ebfab3057a242dda80ae10f6dba894452580abd25c34dc8d7b145dd81f54eb
EclVm.hpp          97d0e02605e6bb0e7ba7af34bad1b8beab372e00220157bf9c692186f019f34c
AnmManager.cpp     c05add280091300b1dd76b6cff422d322a4da0fe1e21808c1156829e0bec9776
AnmManager.hpp     57c8ec82ae92116c4edea9fd475558843ee5752e5a234c1749f5f38392771e49
BulletManager.cpp  2bda79b9659785c784bb33d7c9b453c4cfefb249d37631d63cc20956d18f25aa
BulletManager.hpp  725f281ee37e77f7490d339c1072eaecd6177816aae1b52969f2ac977915fd53
```

Current source edits are `0b51d9d` (Enemy), `79ac30a` (ANM), and
`588851e` (ECL). The older typed ANM edit at `e885fb5` and its evidence
follow-up `2061ddf`, Bullet at `9634285`, and Enemy at `ceed3ea`
remain useful **historical** source epochs, not current HEAD. Private
witnesses must be matched to the relevant file hashes, not guessed by date.

## Next experiments: what the immediately preceding source investigation found

- **Promising nonexact Bullet vector path:** `UpdateVectorAcceleration`
  has original `SUB ESP,0xC`, then x87 multiplications for X/Y/Z before
  their respective writes, spilling Z to `[esp+8]`; original then **reloads
  X/Y from Bullet memory** for `fabs`/`atan2`. The current installed source
  interleaves multiply-and-store and compiles to **285 bytes**, while a
  trial *expiry-first + three independently volatile delta locals*
  compiled a full **305-byte** PDB owner with 10 candidate fields but
  only **196/305** preliminary masked ordinary bytes. Crucially it leaves
  x87 results live and emits extra `fst/fld st/fstp` around `fabs`. A
  *compiler-barrier products + volatile memory reload* trial emitted
  **303 bytes** (still nonexact). Neither trial is committed. The exact
  trial sources and output JSON/logs are under ignored
  `.analysis/gpt-web/20261105/vector-accel-*`, with last native PDB at
  `build/gpt-web/20261105-vector-accel/expiry-first-volatiles/` (or
  selectively restore its archived copy per `RE_ARTIFACTS.md`). Do not
  promote either based on matching size.
- **ANM last 2 bytes:** first validate 350 bound fields, **including all
  local table/branch destinations**. Direct/volatile/ref/`memcpy` opcode-91
  layer rewrites and an `__forceinline` mask helper were already tried;
  the helper grew `ExecuteScript` to 10,252 bytes, rejected. Current typed
  `variableArgs.bits` field overlay restores 9,962/9,964.
- **ECL Run 23 bytes:** `volatile x/y` trials regressed to 6,995/7,020;
  calculating the second square first grew the PDB owner to 7,024.
  Earlier FORMAT pointer-lifetime rewrites displaced original fields.
  Read [instruction-family byte map](evidence/boss-ecl-anm-byte-frontiers-20261104/README.md)
  and measure a **new** x87 hypothesis rather than repeating these controls.
- **Seek & Sound leaves:** 15 recent `Seek` CURRENT/END natural forms stayed
  100/107 (one grew to 111). `SoundDataLoaderThread` cached-filename
  trial shrank to 104 bytes; unsigned-index trial grew to 127; do/while and
  failure-reload variants remained 112. The source was restored after
  all of them. Original target's private ESI/ECX/EAX ABIs cannot be
  fabricated by a source-only exactness claim.

## Current-source rejected experiments (not installed)

- **ECL Run FORMAT local stack homes:** two separately compiled declaration/
  initialization-order trials stayed **6,997/7,020**, all **189** original
  fields passing; two local-record trials changed PDB owner sizes to **6,928**
  or **7,028** and failed the original-size contract. All were reverted.
  Private drivers: `.analysis/gpt-web/20261105-ecl-format-stack/`.
- **Bullet absolute-angle outgoing word:** a new local int/float union in
  the real seven-TU build retained **335** bytes, **8** candidate field
  positions/types and **333/335** preliminary ordinary bytes, with the same
  EAX-versus-EDX register pair at `+0xD7/+0xDD`. Rejected and restored.
  Private driver: `.analysis/gpt-web/20261105-bullet-argument/trial.py`.
- **ECL StartSubroutine CALL descriptor pointer lifetime:** a single typed
  pointer held across value-service calls kept Run at **6,997/7,020** but
  *regressed another full owner*: the 551-byte `StartSubroutine` candidate
  moved four REL32 call fields from the baseline offsets `0x9E/0xD2/0xE8/0x1DE`
  to `0x9D/0xC5/0xD9/0x1DA`. Original field positions include
  `0x9E/0xD2/0xE8/0x1DD`. Reverted; keep per-access typed descriptor
  reads. Private native proof in `build/gpt-web/20261105-ecl-call-descriptor/current/`.
  An older independent rejection is documented in
  [CALL argument records](evidence/ecl-call-argument-record-20261105/README.md).
  This is why a Run-only byte comparison is insufficient for helper edits.

For selected exact regression on these latest changes, use only relevant
unit names (not the full cohort), e.g.:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-get-int-var --unit anm-get-float-var \
  --unit anm-get-int-var-ptr --unit anm-get-float-var-ptr \
  --unit anm-vm-initialize-private \
  --unit anm-loaded-set-and-execute-script-index --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-update-horizontal-wrap --unit bullet-update-deceleration --json
```

## Storage / transfer

Use [private artifact inventory and selective restore](RE_ARTIFACTS.md)
and the [latest verified 1,016-file cleanup](evidence/handoff-current-cleanup-20261105/README.md),
not old counts embedded in historical evidence. At this cleanup checkpoint,
**437 witness files / 19,819,118 content bytes** remain expanded, and the
separate new archive plus regenerated cache pruning reclaimed **42,033,152
allocated disk bytes**; treat these as measured snapshots, not fixed live
inventory totals. Original target,
compiler/Wine `.tools/`, `.analysis/archive/` (ignored compressed
experiments), private Ghidra project and `_reference/` **are not in Git**.
`build/match-linked/` contains canonical accepted producers and is kept.
**Transferring only the Git repo is not enough** for a new machine; private
inputs require a separately authorized transfer, followed by SHA checks.
Old `.analysis/gpt-web/` source snapshots and scripts are useful as
hypotheses but have **historical source hashes**, not current proof.
No whole-game link, end-to-end runtime verification or portability claim.
