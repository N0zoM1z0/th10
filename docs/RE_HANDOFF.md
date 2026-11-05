# TH10 reconstruction — next-agent handoff (2026-11-05, updated source epoch)

**Start here.** At this audit the clean `main` HEAD is **`2061ddf`**
(`gpt-web: bind ANM executor variable argument masks to typed script words`).
The older statement that `0dd8b20` was the latest source commit is **obsolete**.
Check live `git status` and source hashes before building: the remote branch,
local experiments and ignored binaries may differ from this recorded epoch.
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
**None** of the three most recent feature-source commits added exact credit.

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
| ANM | Low-halfword VM flags, 16-bit instruction argument-mask raw/typed union at header +6, and full interpreter macros | [Typed ANM mask](evidence/anm-instruction-variable-mask-overlay-20261105/README.md): full five-source strict 350-field owner; six selected exact units **1,323/1,323**. Expanded `build/gpt-web/20261105-anm-flags-halfword/current-5-tu/`. |
| Enemy/ECL | Five lifecycle `currentTime = 0.0f` writes (same binary32 zero), bit 0 of the thread signal and typed CALL/stack/SCPT structures | [Typed Enemy time](evidence/enemy-ecl-time-reset-typed-20261105/README.md): fresh 11-source graph and two-source complete PDB owners; six selected exact units **432/432**. Expanded `build/gpt-web/20261105-enemy-time-lifecycle/{fresh-11-tu,two-source}/`. [ECL signal proof](evidence/ecl-thread-signal-bit-20261105/README.md). |
| Bullet | Both state `value1Bits` and runtime `angleBits` overlap binary32 payloads; full 335-byte direction updater preserved | [Runtime angle](evidence/bullet-runtime-angle-word-20261105/README.md): genuine seven-source PDB field geometry, two remaining register bytes; two selected original-bound exact units **312/312**. Expanded `build/gpt-web/20261105-continuation-bullet/runtime-angle-overlay/`. |

Source SHA-256 at this HEAD (verify before reuse):

```text
Enemy.cpp          87293321fd46faeb2189fc5c12e21d6bbf580f5dde5f7e16e3ff0c45e33caea5
EclVm.cpp          5553a713690c2378e94e783be8f6c8fec7b80c6100905f58a62a0ec34219f766
EclVm.hpp          97d0e02605e6bb0e7ba7af34bad1b8beab372e00220157bf9c692186f019f34c
AnmManager.cpp     a246090b1e5e3a4df61fc3fd291548dc8b2c148110d510bb708fe51d56c851c0
AnmManager.hpp     57c8ec82ae92116c4edea9fd475558843ee5752e5a234c1749f5f38392771e49
BulletManager.cpp  2bda79b9659785c784bb33d7c9b453c4cfefb249d37631d63cc20956d18f25aa
BulletManager.hpp  725f281ee37e77f7490d339c1072eaecd6177816aae1b52969f2ac977915fd53
```

The typed ANM sources are committed at `e885fb5` (and its follow-up
`2061ddf`, which adds evidence only); Bullet at `9634285`; Enemy at
`ceed3ea`. Earlier commits such as `0dd8b20`, `d556496`, and `de9d141`
remain relevant to the **history**, not current HEAD.

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

Use [private artifact inventory and selective restore](RE_ARTIFACTS.md),
not old counts embedded in historical evidence. Original target,
compiler/Wine `.tools/`, `.analysis/archive/` (ignored compressed
experiments), private Ghidra project and `_reference/` **are not in Git**.
`build/match-linked/` contains canonical accepted producers and is kept.
**Transferring only the Git repo is not enough** for a new machine; private
inputs require a separately authorized transfer, followed by SHA checks.
Old `.analysis/gpt-web/` source snapshots and scripts are useful as
hypotheses but have **historical source hashes**, not current proof.
No whole-game link, end-to-end runtime verification or portability claim.
