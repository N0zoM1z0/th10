# Three complete lifecycle owners recovered in the real ScriptViewer graph — 2026-10-31

The pinned Japanese TH10 v1.00a target SHA-256 is `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Three reviewed-authored, source-present functions previously recorded as nonexact have **zero ordinary byte differences and fully bound address fields** when rebuilt under an already existing genuine `ScriptViewerCreate` producer, not the isolated standalone source conditions that informed historical notes.

| Newly exact complete owner | Physical target extent | Bytes | Independently bound fields |
| --- | --- | ---: | ---: |
| `PlayerCreate` | `0x00425020..0x00425064` | 69 | 5 |
| `EnemyManagerCreate` | `0x0040D6B0..0x0040D70E` | 95 | 5 |
| `AnmRenderManagerView::AllocateVm` | `0x00449950..0x004499F0` | 161 | 4 |

**New authored exact credit: 3 complete owners, 325 bytes, 14 fields.** All three source bodies were already maintained; no fake ABI declaration, extra return, inert padding, binary patch or copied original code is introduced. The previously provisional 69/95/161-byte boundary extents are unchanged, and no function or origin inventory denominator change is claimed.

## Real cross-TU ownership and compiler profile

The canonical `ScriptViewerLifecycle-cold-2` producer has `src/ScriptViewer.cpp` as the `ScriptViewerCreate` `/GL /GS /EHs` entry, plus thirteen existing support sources including the genuine `Player.cpp`, `Enemy.cpp` and `AnmManager.cpp` owners. The exact existing profile override for `Enemy.cpp` remains ordinary `/GL` without `/GS /EHs`. The three new units use explicit `pdb_source` selectors to demand the unique original producer-local module contributions: `support-03-Player.ltcg.obj`, `support-05-Enemy.ltcg.obj` and `support-00-AnmManager.ltcg.obj`. These module identities were checked in each generated PE/MAP/PDB; a matching length or unrelated same-named synthetic stub does not count.

The target `PlayerCreate` retains allocation of its `0x4478`-byte Player, constructor, `PlayerInitialize`, failed-initialize destructor/free and successful return. Its **five** fixed REL32 roles are original `operator new @0x00452493`, `Player::Player @0x004246C0`, `PlayerInitialize @0x004247F0`, `Player::~Player @0x00424ED0` and `operator delete @0x004524A1`. The older normal single-TU diagnostic generated a 72-byte source contribution and did not determine the real private ABI. Under this graph, natural source produces all 69 bytes and all five roles without editing the factory.

The target `EnemyManagerCreate` allocates the `0x68`-byte manager, publishes the actual `g_EnemyManager @0x00477704`, invokes `EnemyManagerView::Initialize @0x0040D280`, and on failure destroys/frees through `EnemyManagerView::~EnemyManagerView @0x0040D530` and `operator delete @0x004524A1`. Together with `operator new @0x00452493`, these are **five** reviewed fields (four REL32 and one DIR32). The previous ordinary-source candidate was 97/95; the real graph reproduces the full 95-byte owner.

The target `AnmRenderManagerView::AllocateVm` traverses its actual in-object 4,096-entry VM occupancy pool with wrap and a heap fallback. The natural source's complete **161 bytes** match, including `operator new @0x00452493`, `AnmVmView::AnmVmView @0x00402050`, and the two separate `AnmVmView::Initialize @0x00401DE0` calls (four REL32 fields). Target code retains the fast reuse path, the two-slot search and construction/initialization fallback. No pool-size, null-path or layout assumption is repaired by manually editing the executable.

## Strict scoped independent acceptance

The original linked archive already provided complete-size, zero-ordinary-difference candidates; **this was a discovery hint only**. The new canonical units each passed the unchanged strict complete-owner comparator with every linkage's original target/callee address. Two independent selected cold replays then rebuilt only the shared 14-input ScriptViewer producer, not the entire history. They also selected the already exact `ScriptViewerView::ScriptViewerView` 234-byte constructor and `ScriptViewerCreate` 67-byte factory as actual lifecycle guards. The complete result is **five units, 626/626 bytes and 21 bound fields** in *each* run; their independent PDB GUIDs are `c4e5c7dacb40cc468781780f24971c69` and `4b270e670f99074c81f8eea6d2388270`. Reports are retained under `.analysis/gpt-web/current-turn/lifecycle-scoped-cold-{1,2}.json` and the image under `build/match-linked/ScriptViewerLifecycle-cold-2/`.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit script-viewer-player-create --unit script-viewer-enemy-manager-create --unit script-viewer-anm-allocate-vm --unit script-viewer-create --unit script-viewer-constructor --json
scripts/repo-python scripts/ci.py
git diff --check
```

The link-context closure does not establish original source TU/provenance, standalone helper/destructor exactness, complete product/data ownership, runtime semantic equivalence or portability. Source codegen exactness and such phases remain separate.
