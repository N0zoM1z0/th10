# Hint lifecycle initialization: complete 131-byte exact owner, bounded source partition (2026-10-31)

`HintOwnerView::Initialize @ 0x00418E30..0x00418EB2` is now **canonical linked-PE exact: 131/131 physical bytes with all 13 original DIR32/REL32 fields bound** against the verified original Japanese TH10 v1.00a (SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`). The reviewed authored function boundary and original target remain unchanged. This admits **one complete authored owner and 131 exact code bytes**, not an additional helper or a cropped fragment.

## Challenge and independently falsified hypotheses

The target's actual chain registration records update callback `0x004198A0`, draw callback `0x004198B0`, the real Chain allocation and priority-specific registration calls, then clears flag bit 2 and conditionally requests both auto/user hint files. The true caller has an **EBX private owner receiver**, while `LoadHintFile @0x00419960` takes **three stack inputs and RET12**. In the previously accepted monolithic `src/Hint.cpp` compiler graph, VC7.1 inlined/interprocedurally specialized the loader boundary, producing a 131-byte initialization section with four ordinary register-argument differences. An explicit `noinline` declaration and export annotations were rejected when they did not independently recover the target. That old arrangement and previous exploratory documentation were treated as hypotheses, not authoritative source boundaries.

Rather than force a register or manufacture a dummy call, the maintained C++ is partitioned by the real responsibility boundary: `src/HintLoad.cpp` now contains the existing `LoadHintFile` parser, anonymous-namespace hint-name tables, and its `HintTrimWhitespace`, `HintReadLine`, `HintSplitLine`, `HintLookupValue` static helpers. `Hint.cpp` retains `HintOwnerView::Initialize`, `CreateHintOwner`, chain registration, `ClearHintLists`, `HintLinkView::Append`, `ExportAutomaticHints`, update and draw functions. No statement inside the parser/initialization logic is invented or silently deleted; the tables have internal linkage and are local in each source. The actual lifecycle graph uses `HintLoad.cpp` as a **normal compilation support source**, with the other eight original source inputs unchanged. VC7.1 then preserves the original true three-stack-parameter `LoadHintFile` call rather than optimizing its private ABI into a different register schedule. This is an **observed codegen model**, not a claim that the original physical translation unit must have been partitioned identically.

The complete 13-field target contract is:

| Owner offsets | Original target identity |
| --- | --- |
| `+0x03`, `+0x2B` | DIR32 `HintOwnerView::OnUpdate @0x004198A0`, `HintOwnerView::OnDraw @0x004198B0` |
| `+0x08`, `+0x33` | REL32 `th10::Chain::AllocElem @0x00449ED0` |
| `+0x12`, `+0x39` | DIR32 actual `th10::g_Chain @0x00491BE4` |
| `+0x26`, `+0x4D` | REL32 `Chain::AddToCalcChain @0x00449AE0`, `Chain::AddToDrawChain @0x00449B70` |
| `+0x5C` | DIR32 `g_MainSupervisorView @0x00491C28` plus **`0x142`**, resolving to target `0x00491D6A` flag location |
| `+0x69`, `+0x76` | DIR32 original auto and user hint path literals at `0x0046D6D8` / `0x0046D6C4` |
| `+0x6F`, `+0x7C` | two independent REL32 real loader calls `LoadHintFile @0x00419960` |

The natural lifecycle graph's **complete** original byte, field and local branch comparisons pass; no field is waived by normalized-address masking alone.

## All affected preexisting exact owners protected

The split changes the maintained `source_file` ledger paths for the actual `LoadHintFile` body and all **four** moved static helpers; the loader stays **1498-byte NONEXACT** and receives zero new credit. For `HintLookupValue`, the anonymous-namespace mangled name is taken from the new source/compiler PDB (`?A0xa4777fd6`), never guessed from the previous TU; static owner/tail-call binding checks use the new path. `ClearHintLists` and `HintLinkView::Append` stay physically owned by the original `Hint.cpp` source but are compiled as support under the file-loader root. All changed native graph groups were verified twice by **unit-selected** independent cold replays:

| Scoped source graph | Strict complete existing units | Bytes/fields, each independent run |
| --- | --- | --- |
| Lifecycle `CreateHintOwner` with normal-compiled parser | New `Initialize` 131; existing `CreateHintOwner` 67, `OnUpdate` callback 10, `OnDraw` callback 6, frame `Update` 571 | **785/785**, 36 bound fields |
| Loader `/GL` with parser source and actual Hint/FileSystem supports | `ClearHintLists` 79, `HintTrimWhitespace` 114, `HintSplitLine` 75, `HintLookupValue` 98 | **366/366**, 6 bound fields |
| Loader `/GL /GS` with the actual FileSystem/PbgArchive/Main supports | `HintReadLine` 260 and `HintLinkView::Append` 52 | **312/312**, 4 bound fields |

After trimming an extra blank line at new-file EOF, the final retained source was independently rebuilt twice in each affected graph. The lifecycle PDB GUIDs in these final two runs are `293b2ff6ccb497438b7ebd441c7d9d5c` and `1dae3d97cc3a20469e4227ba7e779360`. The loader graph GUIDs similarly differ: `05251278880b5c439b21e685b2cee418` / `3f07cfd227e56542824e124c16132c63`, and `/GS` `3fa64f5c95101f4bbf1c32cd5132d603` / `da516fe61443af458e9cd02019d6cca5`. An additional targeted replay of `hint-add-caution` (104) and `filesystem-write-to-open-file` (76) passes **180/180**, but these unchanged existing owners are never counted twice. Detailed independent receipts are retained in ignored `.analysis/gpt-web/current-turn/hint-final-lifecycle-{1,2}.json`, `hint-final-helpers-{1,2}.json`, and `hint-final-aux-guard.json`.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit hint-initialize --unit hint-create-owner --unit hint-update-callback --unit hint-draw-callback --unit hint-frame-update --json
scripts/repo-python scripts/replay-exact-units.py --unit hint-lookup-value --unit hint-trim-whitespace --unit hint-split-line --unit hint-clear-lists --json
scripts/repo-python scripts/replay-exact-units.py --unit hint-read-line --unit hint-append-link --json
scripts/repo-python scripts/ci.py
git diff --check
```

This proof establishes exact linked machine code for the complete new owner, real target call/data identity, and scoped regression closure for its source partition. It does **not** establish original TU filenames, the 1498-byte parser itself as exact, source-level ABI provenance, the full runtime parser/product or portable behavior; it is not a whole-corpus cold replay.
