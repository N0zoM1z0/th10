# TH10 reconstruction — next-agent handoff (2026-11-05)

**Start with this page, not a dated experiment log.** The latest reviewed
**source** checkpoint in this local pass is **`0bf7005`** (ECL boss typed
extra-record cursor). `3935b6e` improves Text alpha native codegen,
`4e26e11` types the SCPT offset records and `3735f01` restores the Chain
task-removal bit. Each has selected native evidence; earlier source work
`d901fca`, `b5b3d1b`, `6856ea0` recovered ECL boss and FORMAT layouts. This paragraph is a source epoch, not a
promise that any long-lived historical witness still represents the current
transitive header graph. Recheck live `git HEAD`, `git status`, and hashes;
work through any unfinished dirty changes. Remote refs and ahead counts
may advance independently; **do not push** without explicit authorization.

## Five-minute recovery and rules

```sh
git status --short --branch
git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/ci.py
```

- Only the private **Japanese TH10 v1.00a** original at `resources/th10.exe`
  is an accepted target. Size **487,936**; SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Keep the original read-only, ignored and out of Git.
- Actual source, Git history, unfinished dirty changes and independently
  accepted exact units take priority over old notes. Do not dismiss dirty
  files as another agent's work; inspect and complete them.
- Make natural VC7.1 source changes to **complete large and small functions**.
  Commit substantive checkpoints as **`gpt-web: ...`**, author
  `N0zoM1z0 <1705004398@qq.com>`, with
  `Co-authored-by: gensokyo-dev <342580529+gensokyo-dev@users.noreply.github.com>`.
  **Do not push** without explicit authorization.
- Always specify selected `--unit` names for an existing exact regression;
  **never** invoke the full historical cold replay by omitting selectors.
  Check the complete linked PDB/COFF owner, not a cropped function fragment.
- Equal candidate sizes, unchanged candidate machine code, source presence
  and old Ghidra names **do not** grant exact credit. New exact acceptance
  requires the entire original body, correctly bound relocations/fields,
  local/table destinations and callee/ABI obligations.

## Current canonical ledger (from `docs/PROGRESS.md`)

| Counted measure | Value |
| --- | ---: |
| Reviewed target boundaries | **1,829 / 1,829** |
| Source-present function mappings | **1,309** |
| Canonical original-target exact functions | **1,482** |
| Canonical original-target exact codegen bytes | **189,797** |
| Confirmed-authored functions / bytes (provisional) | **750 / 276,120** |
| Confirmed-authored exact functions / bytes | **574 / 119,761** |
| Confirmed-authored source-present nonexact backlog | **176** |
| Origin-excluded / origin-indeterminate entries | **520 / 559** |

These are generated ledger values, **not** cumulative estimates from
individual evidence README files. Recompute `scripts/repo-python
scripts/progress.py` only after a ledger edit. They say nothing about
original handwritten provenance or a linked/runnable Windows product.

## Important incomplete owners — verified status, not wishful exactness

| Target owner | Original bytes | Best documented candidate | Evidence and next challenge |
| --- | ---: | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | **14,416** | **14,352**; 716 candidate linked fields | Current **fresh 11-source** local graph below. Still NONEXACT; 64-byte size gap, target-field binding work remains |
| `EnemyBeginSpell @ 0x00409280` | **2,432** | **2,432**; 108 linked fields | **Corrected from stale 2,424**. Two child-node owner reloads recovered target-sized body and seven switch-table destinations; still NONEXACT |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | **9,964** | **9,962/9,964** original bytes; 350 fully target-bound fields, 95 local destinations | Last two differences `+0x113`, `+0x115`: opcode 91 layer EDX vs ECX; still NONEXACT |
| `EclVmContext::Run @ 0x0044E1A0` | **7,020** | **7,020**; 189 positioned fields, 23 ordinary target-byte differences | Typed byte cursor, FORMAT tag **and numeric operands** retained; still NONEXACT |
| `EnemyFullObjectView` constructor @ `0x0040D830` | **673** | **672**; 5 linked fields | Target frame/register/initialization scheduling differs; NONEXACT |
| `BulletRuntimeView::AdvanceTransformProgram @ 0x00406D90` | **1,454** | **1,540** normal COFF; 32 relocations | Signed halfword in child pattern restored; real large owner NONEXACT |
| `TextRenderBufferView::InvertAlpha @ 0x00437160` | **157** | **157**, 112/157 raw target bytes in current normal COFF | New explicit alpha-bit test recovers original `JS` at `+0x69`; 45 other differences remain. Still NONEXACT; see current evidence below |
| `SoundDataLoaderThread @ 0x0043D080` | **107** | **112** in real five-source graph | Extra index/register lifetime; NONEXACT |

Other open owners: `EclVmScriptDatabase::AddScriptData` 589 target vs
570 candidate, Popup Draw 540 target vs 537, `ItemManagerView::Draw`
281 vs 281 (x87/linked fields differ), `ResultMenuMarkTasks` 28 vs 26,
`GameStatisticsOwner` constructor 132 vs 137, and the 335-byte absolute
bullet-direction changer with two register-choice bytes. Those values
come from particular prior compiler contexts; **recheck their current
source/header pins before claiming a new measurement**.

## Latest real native witness and source-epoch cautions

- **Fresh post-SCPT ECL boss graph (11 TUs, installed source epoch):**
  `build/gpt-web/20261105-ecl-extra-slot/baseline/` (all 11 source inputs
  freshly compiled) and
  `build/gpt-web/20261105-ecl-extra-slot/typed-cursor/` (the changed
  `EnemyEclDispatcher.cpp` recompiled with 10 source+header SHA-pinned support
  objects) are the current complete local witnesses. Installed
  `src/EnemyEclDispatcher.cpp` SHA-256 is
  `2a5b2b0ee483736aa75704b8d126980fcf294960f50898ef30119408d2f98795`,
  with `src/EclVm.cpp` at
  `b476f4a535203f522f0d69036f67bcc1f32db9faab9779ab75f5377bef4186e0`.
  The 14,352-byte **candidate** ECL dispatcher has **716** same-position,
  same-type **candidate** fields and zero ordinary candidate differences;
  the **original** owner has 14,416 bytes and is **NONEXACT**.
  `EnemyBeginSpell` retains its 2,432-byte candidate (108 unchanged fields),
  and the 162-byte death finalizer and 330-byte viewport owner preserve
  candidate codegen. Five selected **original-bound** exact owners replayed
  **340/340 previously accepted bytes**, with zero new exact credit.
  A hoisted extra-record pointer moved 223 linked-field positions and was
  rejected; the retained typed per-access inline cursor preserves geometry.
  [Current boss proof](evidence/enemy-ecl-inline-extra-cursor-20261105/README.md).
  The earlier `20261105-typed-strides` graph is a prior source epoch, **not**
  the current ECL/Chain transitive-header witness.
- **ANM near-exact five-source graph:**
  `build/gpt-web/20261105-followup-anm/typed-instruction-next/` (ignored).
  Installed `AnmManager.cpp` SHA-256
  `84e2d02b5dba569beace359926316473a4d784a40c4166f5b67468d38f007b10`;
  `AnmManager.hpp` SHA-256
  `ffb3ea5d5493607d9e4f97cbec8ed8f568d7972aa0d3fccfe5882878055bd69b`.
  Strict two-byte result is not a canonical exact admission. See
  [typed variable-mask leaves](evidence/anm-typed-variable-mask-leaves-20261105/README.md).
- **ECL Run last strict original-bound comparison:**
  `build/gpt-web/20261105-continuation-ecl-run/ecl-format-signed-typed-prefix/`
  (ignored). The 7,020-byte source contains the typed FORMAT prefix and
  signed eight-byte argument record stride. Its 189 target-bound field
  positions/types remain valid, but the 23 ordinary target-byte differences
  remain. **Newer local SCPT source epoch:** `src/EclVm.cpp` SHA-256
  `b476f4a535203f522f0d69036f67bcc1f32db9faab9779ab75f5377bef4186e0`
  and `EclVm.hpp` SHA-256
  `7fedc7debe07d742abbacb0670d3ab16e232fb06cd6f77d1151c5fc3f10df5f0`.
  A fresh two-TU compilation gives the same 7,020-byte candidate and 189
  candidate fields; it does **not** newly rebind every field to the target.
  `AddScriptData` remains 570/589 candidate bytes. Selected original-bound
  exact guards passed **609/609**; see
  [SCPT record proof](evidence/ecl-scpt-subroutine-offset-record-20261105/README.md)
  and [signed FORMAT recovery](evidence/ecl-format-signed-prefix-overlay-20261105/README.md).
- **Full ECL Boss source improvements already installed:** typed Type-0/1
  laser request fields, positions and pattern copies, real dynamic ANM
  resource arrays, typed 13 binary32 spawn operands, native bullet-pattern
  `0x2C4` base / `0x210` stride, cancel flag and laser control word.
  See [laser work](evidence/enemy-ecl-typed-type1-laser-20261105/README.md),
  [pattern arithmetic](evidence/enemy-ecl-native-pattern-arithmetic-20261105/README.md),
  and [cancel flags](evidence/enemy-ecl-spell-flags-laser-control-20261105/README.md).
  Current type-derived extra-record stride (`0x210 / 0x18 = 22`),
  bullet-offset vector cursor (`0x1344`, 12-byte stride) and six-field
  overlapping record are documented in
  [native strides](evidence/enemy-ecl-native-stride-closure-20261105/README.md)
  and [overlay](evidence/enemy-ecl-bullet-extra-overlay-20261105/README.md).
- **Shared state now spans several source owners:** spell-history
  `captures +0x80` vs `attempts +0x84` are *different* counters;
  `GameScoreStateView.history` types statistics groups at +0x28/+0x2C;
  constructor/ECL/item drop use the same 0x3C mixed drop packet.
  [Score selector evidence](evidence/shared-score-history-group-fields-20261105/README.md),
  [spell node lifetime](evidence/enemy-spell-second-child-branch-20261105/README.md),
  [drop layout](evidence/enemy-shared-drop-packet-layout-20261105/README.md).

- **Shared Chain/ResultMenu task flag recovery:** the independently
  original-observed bit 1 of `ChainElem::flags` is now the typed
  `pendingRemoval` bit used by ResultMenu registration and retirement.
  Four selected native exact owners still pass **621/621**; the independent
  28-byte `ResultMenuMarkTasks` remains a 26-byte nonexact native candidate
  due to a private receiver/register difference. See
  [typed task ownership](evidence/result-menu-chain-pending-removal-20261105/README.md).
- **Text alpha branch correction in current source:** normal-COFF native
  `InvertAlpha` is 157 bytes, with `TEST AH,AH / JS` now matching the target
  at `+0x69`. This is **112/157 original ordinary bytes**, not an exact
  acceptance. Selected existing text owners passed **1,799/1,799**; see
  [alpha sign-branch evidence](evidence/text-alpha-native-sign-branch-20261105/README.md).
  A separate current `/GL /GS` real `TextBufferLifecycle` link gives
  **138/157** raw original bytes with the same target `JS` branch, but a
  different EAX private receiver instead of target ECX. A scoped `noinline`
  trial produced **identical 138/157** codegen, so it was rejected and
  restored. Its ignored witness is
  `build/gpt-web/20261105-text-leaf/noinline-ltcg/`.
- **Failed current ANM opcode 91 lifetime trial:** capturing the ANM resource
  before the child-script read kept a 9,964-byte candidate and 350 fields,
  but **moved linked field positions** and fails the full 350-field target
  manifest. Source restored; do not interpret size equality as exactness.
  Local receipt: `.analysis/gpt-web/20261105/anm-child-resource-live-trial-result.json`.
- **Current failed controls, source restored:** seven 107-byte Seek-source
  variants retained the same seven target-vs-candidate register/jump byte
  differences (one other variant grew to 118); two `EclVmContext::ReadInt`
  alternative float-bit forms broke its already exact 144-byte owner and
  were reverted. Three natural Popup factory creation variants remained
  67 or 71 candidate bytes, not the original 71-byte instruction sequence.
  Ignored focused trials: `.analysis/gpt-web/20261105/seek-new-trials.py`,
  `popup-creator-trial.py` and the ECL read-int selected replay JSONs.
  Do **not** reintroduce a regressed exact leaf to force a codegen hypothesis.

## Specific bounded replay and checking workflow

```sh
# Name the exact units actually touched; never omit --unit.
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-set-laser-position --unit enemy-find-laser --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-end-spell --json
scripts/repo-python scripts/ci.py
git diff --check
```

A shared public header change may require recompiling affected support
translation units for a fresh 11-source graph. Reuse an object **only** after
verifying its original source/profile **and transitive header** inputs; do
not mix objects from different source epochs. In particular the earlier
`20261105-float-ops` graph is *not* the latest compiled source even though
an older report compares its bytes to the current boss.

Previously failed variants worth **not** repeating without a new lead:
ANM opcode-91 direct-call, volatile layer, field-order and sign changes;
ECL FORMAT fully typed parameter-pointer rewrite (reduced function size);
ECL POINT_ANGLE volatile/staged values (codegen regression); *most* Text
alpha sign/mask rewrites (the installed explicit high-bit test is the
exception that recovers the target JS); and SoundDataLoader
unsigned-index/volatile alternatives. Local scripts and receipts are under
ignored `.analysis/gpt-web/`, with older compiler graphs selectively archived.

## Storage, preservation and transfer

Read [RE_ARTIFACTS.md](RE_ARTIFACTS.md) and the
[latest cleanup receipt](evidence/handoff-maintenance-20261105/README.md).
The new guarded cleanup reduced expanded `build/gpt-web` to **311 files /
14,079,534 payload bytes**, retaining the frontier graphs above. Another
**1,581** historical/failed/older-epoch files were **verified and archived**
in `.analysis/archive/gpt-web-extra-graphs-20261105.tar.zst` (local, ignored),
with per-file private SHA-256 manifests and selective restore instructions.
Other protected private archives also remain retained.

**Git alone cannot transfer** `resources/th10.exe`, `.tools/` (VC7.1 and
Wine), Ghidra project, `_reference/`, ignored compiled witnesses, archives
or manifests. A new machine needs those inputs transferred separately and
verified; do not commit them. Never delete the old archives or
`build/match-linked/` merely to obtain extra space. The previous storage
figures of **13 expanded witnesses / ~9 MB** were historical, not current.
