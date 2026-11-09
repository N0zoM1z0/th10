# Handoff ledger reconciliation: two exact ANM owners admitted

On 2026-11-09 the current `config/functions.csv` already marked two
native ANM functions exact, but `config/matches.csv` had no entries for
either, so generated `docs/PROGRESS.md` under-counted both functions and
420 original code bytes.

The two functions were independently rebuilt *twice each* from their
current-source selected linked-PE units under pinned VC7.1 SP1 LTCG.
Each attempt compared the complete target PDB-owned extent, exhaustive
original-target link fields, and decoded instructions:

| Target | Source / exact unit | Extent | Linked fields | Reports |
| --- | --- | ---: | ---: | --- |
| `0x00449870` | `AnmLoadedView::InitializeVm` / `anm-loaded-initialize-vm` | 217/217 | one REL32 to `0x0043E7E0` | `initvm-first.json`, `initvm-second.json` |
| `0x0043E710` | `LoadScript_0043e710` / `anm-loaded-start-script-wave11` | 203/203 | four bound REL32/DIR32 fields | `loadscript-first.json`, `loadscript-second.json` |

All four focused comparisons report **exact**, complete PDB contributions,
complete decoding, and zero first-difference offsets. This adds no new
source functions: it corrects the *accepted matches ledger* for exact
implementations already in the repository.

The `0x0043E710` result is **compiler-context specific**. Its naturally
authored body is 203 bytes in the selected two-TU context but grows to
216 bytes in a larger seven-TU spell-HUD graph. It must not be used as
proof that the seven-TU graph or the 2,432-byte `StartSpell` is exact.
See `../anm-loaded-start-script-wave11-20261109/README.md` and
`../anm-vm-initialize-449870-20261109/README.md`.

Only those two selected units were rebuilt. No full historical cold replay.
