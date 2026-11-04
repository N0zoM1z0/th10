# TH10 laser update callback: original callee return path, source-only refinement

Original pinned Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete physical callback at **0x0041C480..0x0041C4DA** is **91 bytes**.
It is the real update Chain callback registered by the laser manager at
`0x0041C157`, reading the actual `g_GameManager` +0x58 flags from
`0x00477810`; its original compiler/source TU and authored provenance
remain indeterminate.

Native disassembly shows three distinct return paths. When flags bit 0/2
or bit 10 blocks updates it loads **EAX=1** and returns. For the
paused speed lane it saves `g_AnmGameSpeed`, zeroes it, calls actual
`EnemyLaserManagerStorageView::UpdateLinkedLasers` (`0x0041C330`), restores
speed and **returns with callee EAX intact**. For the unpaused lane it
calls the same list updater and returns, likewise without overwriting
callee EAX. The maintained list updater itself returns one on normal
exit, but this *observed callback exit behavior* is narrower than
saying the callback unconditionally overwrites EAX with one.

`src/EnemyLaserUpdateList.cpp` now expresses the separate gate tests and
returns the genuine `UpdateLinkedLasers()` value, retaining the saved speed
across the paused call. This is a source-level return-flow correction;
there is no fake ABI, injected data, assembly rewrite or binary patch.
The current native candidate does **not** improve the codegen frontier.

## Fresh complete native comparison and scoped exact guards

A new four-input pinned VC7.1 SP1 `/GL` diagnostic compiled the current
`EnemyLaserUpdateList.cpp` with real `GameManager.cpp`,
`EnemyLaserManager.cpp`, and `Chain.cpp` support. The full PDB
contributions and complete target boundaries are:

| Owner | Fresh candidate / target | Preliminary ordinary agreement |
| --- | --- | ---: |
| `EnemyLaserUpdateCallback` | **87 / 91**, NONEXACT | **2 / 67** |
| `UpdateLinkedLasers` | **275 / 274**, NONEXACT | **39 / 254** |
| `EnemyLaserDrawCallback` | **46 / 46** structural-equal | **42 / 42** |

The earlier retained callback source produced 88/91; the explicit return
path **regresses size by 1 byte** but better states what the observed
machine-code return path does. This is not a new exact owner, and neither
callback nor list updater has been entered into `config/matches.csv`.
The six candidate callback linkage fields have not been admitted against
full original target geometry and private ABI. A direct source equality
or a 46-byte draw callback alone never grants credit to the 91-byte owner.

With the maintained source, selected canonical **cold** regressions pass:
laser manager Initialize **146/146**, manager Create **91/91**, draw callback
**46/46**, and draw-list **43/43**, total **326/326 accepted bytes**.
No all-unit cold replay was run. The original manager, update list, and
read-only private target resources remain uncommitted.

Reproduction for accepted affected owners only:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-laser-manager-initialize --unit enemy-laser-manager-create \
  --unit enemy-laser-draw-callback-0041c4e0 \
  --unit enemy-laser-draw-list-0041c450 --json
scripts/repo-python scripts/ci.py
```

Private current receipts:
`.analysis/gpt-web/20261104-next/laser-return-maintained-result.json`,
`laser-return-scoped-guards.json`, and
`laser-callback-return-results.json`.
The target's original authored C++ spelling, full game runtime, product
closure and portability have not been established.
