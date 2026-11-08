# Typed leaf receiver layouts — 2026-11-08

This change replaces four raw byte-offset bodies in `DecomphelpLeafAccessors.cpp` with named project-view fields while preserving the target code bytes.

## Layout evidence and limits

- `0x00404C90` and `0x00404D30` access `StageFogInterpolationView::duration` at `+0x84`. `StageScriptLayoutChecks` in `StageScript.hpp` asserts that offset.
- `0x00404F20` clears bit 0 through `AnmVmView::visible`; `AnmVmFlagsAt35C` in `AnmManager.hpp` asserts `flags35C` at `+0x35c`.
- `0x004087D0` writes `AnmVmView::pendingInterrupt` at `+0x304`; `AnmVmScriptStateAt304` in `AnmManager.hpp` asserts that offset.

The target boundary ledger records no code callers or xrefs for these four leaves. The typed receivers therefore express layout-compatible views supported by current project structures; they do **not** establish the original class owner, original source name, or authorship. The semantic names describe the observed field operation, not a claim that the original binary had these C++ names.

## Target-bound exact replay

Target identity: `resources/th10.exe`, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Command:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-decomphelp-fulltu-00404c90 \
  --unit game-decomphelp-fulltu-00404d30 \
  --unit game-decomphelp-fulltu-00404f20 \
  --unit game-decomphelp-fulltu-004087d0 --json
```

Result: `exact`; all four units report `raw_equal: true`, no differing bytes, and complete PDB-section extents of 7, 11, 8, and 8 bytes respectively. The shared linked image SHA-256 was `eb8b8e08825ad940fb30057e397603bf9e2809b886a1a96bc8897ab66edf20e5`.

This was a selective leaf-unit replay, not a full cold replay. A broader source-wide replay also attempted unrelated leaf-identity artifacts and stopped on a Wine/LINK `LNK1104` failure in `DecomphelpLeafIdentity00438400`; the four requested units were subsequently replayed independently and passed. No runtime or whole-image claim is made.
