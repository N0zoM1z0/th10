# Exact 320-byte ANM texture-region uploader (0x00446D70)

Original TH10 Japanese v1.00a SHA-256:
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

## Exact owner, not a prefix match

- Target: 0x00446D70..0x00446EAF inclusive, 320 bytes.
- Maintained source: AnmRenderManagerView::LoadTextureRegion in src/AnmManager.cpp.
- Selected entry: AsciiManagerView::Initialize; pinned MSVC 7.1 SP1 /GL LTCG.
- Actual PDB contribution: 320 bytes, complete extent and dense decode.
- Independent selected-only linked-PE replays: **320/320 exact** in both.
- Eight original-target 32-bit address/call fields; **288/288 ordinary bytes**.
- No ordinary mismatches, omitted bytes or unidentified local table fields.

The complete, target-specific relocation contract is embedded in the
[units.anm-resource-load-texture-region] match unit. Independently inspected
target instruction fields:

| Byte offset | Kind | True target | Meaning |
| --- | --- | --- | --- |
| 0x00E | DIR32 | 0x00491D78 | hardware/format flags |
| 0x020 | DIR32 | 0x0046F288 | ANM format table |
| 0x09D | REL32 | 0x004521D0 | D3DXLoadSurfaceFromFileInMemory |
| 0x0F1 | DIR32 | 0x0046F2A0 | ANM pixel stride table |
| 0x0F7 | DIR32 | 0x0046F288 | ANM format table |
| 0x10E | REL32 | 0x004521A6 | D3DXLoadSurfaceFromMemory |
| 0x123 | REL32 | 0x004465B0 | ApplyTextureAlphaBleed |
| 0x12E | DIR32 | 0x0046F2A0 | ANM pixel stride table |

## Source recovery

The old source generated only 319 bytes. In the embedded-THTX branch, it
generated MOV EAX,[EDI+0x30] then ADD EAX,EDI. Retail instead generated
MOV ECX,[EDI+0x30] then LEA EAX,[ECX+EDI], making the complete body
one byte longer. Fourteen cached-pointer/index forms, repeatedly inlined
accessors and field declaration variations were compiled and rejected.

A natural source-level THTX accessor solves this. The first source-width
read is expressed as AnmGetThtx(rawEntry)->width, while the other header
fields still read the cached pointer. This precise expression lifetime makes
VC7.1 LTCG emit the retail MOV/LEA pair and its downstream ECX/EDX register
schedule. Three independent width-accessor placements gave 320 bytes with
all eight fields at retail offsets and no ordinary differences.

The implementation uses normal typed C++ and no inline assembly, copied
machine bytes, dummy operands or relaxed comparator masks.

## Neighbor preservation and reproduction

The three adjacent texture creators still exactly match their own target
function extents in the same recompiled single-TU producer:

- CreateTextureFromFile 132/132 with 7 target-bound fields.
- CreateTextureFromAnm 264/264 with 9 target-bound fields.
- CreateEmptyTexture 41/41 with 4 target-bound fields.

Together: **437/437** exact existing guard bytes; the recovered region is
a new independent **320-byte exact owner**.

Reports: first-link-report.json and second-cold-report.json, plus the three
guard-*.json receipts. source-pins.json attests the modified source, compile
unit and target EXE. The second proof removed only the affected ANM resource
producer output directory and rebuilt that producer from scratch; it was not a
full project replay.

Reproduce using only the affected graph:

    scripts/repo-python scripts/build-match-unit.py --unit anm-resource-load-texture-region
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-load-texture-region --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-create-texture-from-file --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-create-texture-from-anm --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-create-empty-texture --json

Neither this proof nor the source reconstruction establishes original
historical TU ownership, whole-game execution, or runtime equivalence.
