# TH10 reconstruction — next-agent handoff (2026-11-05)

**Start with this page, not a dated experiment log.** The last reviewed
**source** checkpoint before this handoff cleanup is **`77dc82a`**; later
commits may only update documentation. Always inspect the actual `git HEAD`,
`git status` and current source hashes before interpreting any old receipt.
This workspace's last checked local `main` was clean, **29 commits ahead of
`origin/main`**, and had **not been pushed**. A different checkout may differ.

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
| `TextRenderBufferView::InvertAlpha @ 0x00437160` | **157** | **157**, one normal COFF byte off | Conditional `JL` vs original `JS`; prior natural rewrites often grow other diffs |
| `SoundDataLoaderThread @ 0x0043D080` | **107** | **112** in real five-source graph | Extra index/register lifetime; NONEXACT |

Other open owners: `EclVmScriptDatabase::AddScriptData` 589 target vs
570 candidate, Popup Draw 540 target vs 537, `ItemManagerView::Draw`
281 vs 281 (x87/linked fields differ), `ResultMenuMarkTasks` 28 vs 26,
`GameStatisticsOwner` constructor 132 vs 137, and the 335-byte absolute
bullet-direction changer with two register-choice bytes. Those values
come from particular prior compiler contexts; **recheck their current
source/header pins before claiming a new measurement**.

## Latest real native witness and source-epoch cautions

- **Fresh current ECL graph (11 TUs, no stale support objects):**
  `build/gpt-web/20261105-handoff-current/complete-ecl/` (ignored).
  `src/EnemyEclDispatcher.cpp` SHA-256
  `9c270b89dd61dda01896261de742b5d4ff2b1dc3405ab102af933bcf62efa0fc`.
  It confirms complete 2,432-byte `EnemyBeginSpell`, 14,352-byte dispatcher,
  162-byte death finalizer, 672-byte constructor and 330-byte camera owner.
  Compared to an **older candidate**, only the ECL dispatcher's SIB encoding
  at `+0x271F` differs; this is *not* a strict original-target match.
  Receipt: [current native frontiers](evidence/handoff-maintenance-20261105/current-native-frontiers.json).
- **ANM near-exact five-source graph:**
  `build/gpt-web/20261105-followup-anm/typed-instruction-next/` (ignored).
  Installed `AnmManager.cpp` SHA-256
  `84e2d02b5dba569beace359926316473a4d784a40c4166f5b67468d38f007b10`;
  `AnmManager.hpp` SHA-256
  `ffb3ea5d5493607d9e4f97cbec8ed8f568d7972aa0d3fccfe5882878055bd69b`.
  Strict two-byte result is not a canonical exact admission. See
  [typed variable-mask leaves](evidence/anm-typed-variable-mask-leaves-20261105/README.md).
- **ECL Run last dedicated comparison:**
  `build/gpt-web/20261105-continuation-ecl-run/ecl-format-int-and-float-words/`
  (ignored). Its direct ECL source is at a different graph epoch; re-pin
  dependent headers/support objects before further target claims. See
  [typed FORMAT numeric operands](evidence/ecl-vm-format-typed-scalar-words-20261105/README.md).
- **Full ECL Boss source improvements already installed:** typed Type-0/1
  laser request fields, positions and pattern copies, real dynamic ANM
  resource arrays, typed 13 binary32 spawn operands, native bullet-pattern
  `0x2C4` base / `0x210` stride, cancel flag and laser control word.
  See [laser work](evidence/enemy-ecl-typed-type1-laser-20261105/README.md),
  [pattern arithmetic](evidence/enemy-ecl-native-pattern-arithmetic-20261105/README.md),
  and [cancel flags](evidence/enemy-ecl-spell-flags-laser-control-20261105/README.md).
- **Shared state now spans several source owners:** spell-history
  `captures +0x80` vs `attempts +0x84` are *different* counters;
  `GameScoreStateView.history` types statistics groups at +0x28/+0x2C;
  constructor/ECL/item drop use the same 0x3C mixed drop packet.
  [Score selector evidence](evidence/shared-score-history-group-fields-20261105/README.md),
  [spell node lifetime](evidence/enemy-spell-second-child-branch-20261105/README.md),
  [drop layout](evidence/enemy-shared-drop-packet-layout-20261105/README.md).

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
ECL POINT_ANGLE volatile/staged values (codegen regression); Text alpha
mask/sign rewrites (disrupt other instruction bytes); and SoundDataLoader
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
