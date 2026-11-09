# Exact ANM 243-byte texture uploader (0x00446C70)

Target: pinned Japanese TH10 v1.00a
SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

## Whole-function exact proof

- Retail physical extent: 0x00446C70..0x00446D62 inclusive, 243 bytes.
- Maintained source: AnmRenderManagerView::LoadTexture, src/AnmManager.cpp.
- Pinned MSVC 7.1 SP1 /GL LTCG source: AsciiManagerView::Initialize entry.
- Full PDB contribution is **243 bytes**, completely decoded.
- Two independent selected-only canonical linked-PE builds each report
  **243/243 exact**, with no ordinary byte differences and eight original
  external fields independently bound (211/211 ordinary bytes).
- Raw addresses differ because diagnostic linkage anchors are not the retail
  addresses. The canonical unit tests symbol roles, binding targets,
  decoded original instructions and relocation kinds rather than masking
  unknown data or copying retail bytes into the source.

## Original target address contract

| Offset | Type | TH10 retail target | Identity |
| --- | --- | --- | --- |
| 0x005 | DIR32 | 0x00491D78 | hardware format restriction |
| 0x013 | DIR32 | 0x0046F288 | ANM format table |
| 0x06C | REL32 | 0x004521D0 | D3DXLoadSurfaceFromFileInMemory |
| 0x0AD | DIR32 | 0x0046F2A0 | ANM bytes-per-pixel table |
| 0x0B3 | DIR32 | 0x0046F288 | ANM format table |
| 0x0C9 | REL32 | 0x004521A6 | D3DXLoadSurfaceFromMemory |
| 0x0D9 | REL32 | 0x004465B0 | ApplyTextureAlphaBleed |
| 0x0E4 | DIR32 | 0x0046F2A0 | ANM bytes-per-pixel table |

## Natural C++ reconstruction and compiler fingerprint

The earlier valid source generated 242 bytes, rather than the 243-byte target.
The difference was in constructing the embedded header pointer: the old
candidate used MOV EAX,[ESI+0x30]; ADD EAX,ESI while retail used
MOV ECX,[ESI+0x30]; LEA EAX,[ECX+ESI]. No dummy statements, raw opcodes,
register hints, ABI stubs, target code copying or relocated-byte tolerance
were added.

After recovering the related 320-byte LoadTextureRegion, we tested the same
natural THTX accessor on this adjacent uploader. Putting the typed
AnmGetThtx helper before both uploaders and using it for the first width
read (while retaining the cached header for the remaining fields) restored
the retail codegen and all downstream register choices. Both independently
located target function extents are now exact.

## Selected cold replay and preserved neighbors

After the first successful selected linked-PE proof, only the shared ANM
resource-producer output directory was removed and rebuilt from source.
The new second-cold-report.json again gives **243/243 exact**.

The same new image independently reports all four adjacent exact guards:

- LoadTextureRegion: 320/320, 8 fields.
- CreateTextureFromFile: 132/132, 7 fields.
- CreateTextureFromAnm: 264/264, 9 fields.
- CreateEmptyTexture: 41/41, 4 fields.

Together these five distinct target owners cover **1000/1000 original bytes**,
with all 36 independently reviewed call and data binding occurrences.
Evidence: first-link-report.json, second-cold-report.json,
guard-*.json and source-pins.json.

To reproduce, run only the selected ANM resource graph:

    scripts/repo-python scripts/build-match-unit.py --unit anm-resource-load-texture
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-load-texture --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-load-texture-region --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-create-texture-from-file --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-create-texture-from-anm --json
    scripts/repo-python scripts/compare-linked-function.py --unit anm-resource-create-empty-texture --json

This establishes exactness for the five individually bounded code owners,
not the original historical source layout, whole application executable,
runtime outcomes or full product build.
