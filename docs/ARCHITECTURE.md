# Target architecture

## Observed executable identity

The only target is the original Japanese TH10 1.00a executable pinned in
`config/target.toml`. It is a relocation-stripped PE32 GUI program for Intel
i386 with image base `0x00400000`, entry point `0x004537DC`, four sections,
and image size `0x0009C000`.

| Section | RVA | Virtual size | Raw offset | Raw size |
| --- | ---: | ---: | ---: | ---: |
| `.text` | `0x00001000` | `412,289` | `0x00000400` | `412,672` |
| `.rdata` | `0x00066000` | `51,782` | `0x00065000` | `52,224` |
| `.data` | `0x00073000` | `156,492` | `0x00071C00` | `16,896` |
| `.rsrc` | `0x0009A000` | `4,832` | `0x00075E00` | `5,120` |

Observed imports include Direct3D 9, D3DX 9.31, DirectInput 8, DirectSound,
WinMM, and standard Win32 DLLs. This establishes a platform surface, not source
ownership or a complete dependency recipe.

## Toolchain observation

The PE linker version is 7.10. The Rich header contains 232 records at build
6030 across product IDs 15, 90, 95, 96, and 100, plus explicitly retained older
records. This supports a VC7.1 SP1-era compiler-family hypothesis. It does not
yet identify the exact compiler binaries or any per-unit profile.

The following remain unknown until compiler and link Oracles reproduce them:

- exact compiler, linker, librarian, headers, and libraries;
- per-translation-unit flags and calling-convention overrides;
- translation-unit partition and source order;
- static libraries and selected members;
- resource compilation inputs;
- linker switches and input order;
- ownership of compiler-generated, library, padding, and data extents.

## Function and boundary inventories

`config/functions.csv` is a navigation inventory exported from the exact,
target-attested Ghidra database and augmented by target review. Ghidra names are
provisional, and an entry in this file does not by itself prove a physical
boundary or source origin.

`config/function-origins.csv` records origin independently.
`config/function-boundaries.csv` records the current boundary review state for
each candidate. `scripts/report-boundary-inventory.py` reproduces the automatic
part of that ledger from the verified target, the pinned Capstone decoder, and,
when available, Ghidra address-set ranges exported by
`python3 scripts/ghidra.py architecture`. A boundary may be `reviewed`,
`provisional`, or `needs_review`; only the first state closes boundary review.

Shared tails, switch tables, EH metadata, thunks, alignment, interleaved data,
and remote owned chunks can make a Ghidra function extent incomplete or
over-broad. Ghidra's address set is therefore exported as individual ranges,
rather than collapsed to `entry..maximum-address`. Review those surfaces before
classifying origin or accepting bytes.
