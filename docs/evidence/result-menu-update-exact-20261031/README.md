# Complete result-menu dispatcher — 206 exact bytes (2026-10-31)

`ResultMenuUpdate @ 0x004223F0..0x004224BD` is **canonical linked-PE exact**, 206/206 physical bytes and 16/16 independently bound address/control fields in two cold VC7.1 SP1 `/GL` builds. The valid target is original Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This adds a single reviewed authored owner, 206 authored exact bytes, and 27 newly reviewed physical bytes to the provisional authored inventory. It does not add a new function or source mapping.

## Correcting the old boundary hypothesis

The prior reviewed ledger bounded `0x004223F0..0x004224A2` (179 bytes), assuming code ends at the final RET. Independent raw target review identifies the actual linked physical section contribution as `0x004223F0..0x004224BD` (206 bytes), followed by two CC bytes at `0x004224BE..0x004224BF`; the next independent function starts at `0x004224C0`. The 27 retained post-RET bytes are **one NOP at `+0xB3`, three absolute jump-table pointers at `+0xB4/+0xB8/+0xBC`, and fourteen byte-sized case selectors at `+0xC0..+0xCD`**. The selector table base `0x004224B0` is referenced by the target code at `+0xB`, and the jump table base `0x004224A4` at `+0x12`. The three destination pointers resolve to `0x00422406`, `0x00422442`, `0x0042244A`. None of these bytes is classified as unrelated padding or discarded just because the CPU would not execute it sequentially.

## Entire-function evidence

The maintained natural switch/timer code in `src/FrontEnd.cpp` was not rewritten. In the pinned `/GL` FrontEnd link group, with the existing real `ResultMenuSynchronizeScore` entry and unresolved-symbol anchor harness, a unique source.ltcg.obj PDB contribution reproduces the full **206 bytes** and all **16 fields**. Two local DIR32 fields point to the selector and jump tables, three table DIR32 fields have fixed original local destinations, and the remaining eleven fields bind the game-state globals, input state, actual pause/result handlers, float constants and CRT `__ftol2`. Source/target local offsets are checked as written; the comparator does not waive local branch displacements or normalize changed table destinations. Physical extent, not a 179-byte prefix, is the acceptance boundary.

The first standalone strict `scripts/compare-linked-function.py --unit result-menu-update` returned `exact`. Two independently rebuilt cold links, each selecting only `result-menu-update`, return `exact`, 206 matched bytes, 16 fields with distinct PDB GUIDs `8c607c73bd4a2f4a8bd71430ec622dec` and `651be4aad0b65e498867f6027043ae39`. Their full ignored JSON output is `.analysis/gpt-web/current-turn/result-update-cold-{1,2}.json`. This is the complete source-codegen claim; original production TU naming, exact input and target data ownership, full product build, runtime correctness and portability remain independent.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit result-menu-update --json
scripts/repo-python scripts/ci.py
git diff --check
```

Neither the full historical cold replay nor unrelated exact functions were run. The canonical matcher checks the fixed local target positions, including all absolute table words; no target bytes were copied into source or patched in the linked image.
