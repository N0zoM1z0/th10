# Small measured results

All sizes below are complete attributed native owner sizes in bytes, not match
percentages. None equals its target and no bytes-exact credit is claimed.

| Experiment | Caller (target 113) | Keyboard (target 850) | Mapper (target 833) |
|---|---:|---:|---:|
| a001 | 105 | 251 | 649 |
| a002 | 105 | 718 | 649 |
| a003 (/GS) | 105 | 745 | 709 |

- a001: command `79f17dcff8b045a88d918f09675a9075`; manifest SHA-256
  `667169a2be9636303a43c4e9917ccfc83b00d41ec33d434e1f4f2527e3a9527d`.
- a002: command `f1502ccae178430c869ee9dfddb7d302`; manifest SHA-256
  `df30450689a3d16fefd6f3454321b35ad26a8428be3234427d5914566185fd59`.
- a003: command `427b45a0e8d148c0b5b42c4969f49ce1`; manifest SHA-256
  `da62e0af06bdba356e6bba76296b4061ca2b536833fab4e4aedbfcfd2c53623e`;
  elapsed 52.68 seconds, 125 unresolved harness dependencies; production clean.

The a001→a002 source correction moved ordinary key predicates into their genuine
owner, removing independent 208/223-byte helpers and the artificial high-bit
wrapper. Call-role inspection `20a05487c6f74d488deea4110d6a08be` showed one
UpdateInput call to each mapping helper. The standalone seven-byte IsDown owner
had no observed calls; its existence alone proves no call relationship. Helper
sizes are never added to caller matching credit. The actual edited key blocks
passed 270,848 cases and eight semantic mutation checks, rerun for this package.

Read-only a003 structural command `a200c95e58e6401b8a81cffab9b07b33` resolves
recognized security-cookie calls in both candidate owners and both target owners.
The mapper's explicit eight-byte alignment and fixed prologue reservation of 340
bytes agree. Keyboard fixed reservations are candidate 260 versus target 264.
These are fixed reservations, not total frame sizes. Total frames, simultaneous
API-buffer layout and parameter/index specialization remain unresolved. This
structural observation does not justify a new source change or prove matching
bytes. The a002 metadata had none of three recognized security symbols; that was
not a universal proof of absence of all possible security instrumentation.

For evidence lookup, a003 image/MAP/PDB SHA-256 values respectively are:
- `5004f974a7321d184ba7a1179b962fa5dcb95768450dd8d8b90abd7ce95f3142`
- `deb85560f76b348a5d900de7d16eebab222092b73a3e703455de27b85c36f352`
- `9f080dec9a86f07c1a30bc33961de3666999ca1856142fd8cca6596eaaeaf397`

Full artifacts remain remote. Receipt observations were reduced to this small
record rather than duplicating transient diagnostic histories or certificates.
