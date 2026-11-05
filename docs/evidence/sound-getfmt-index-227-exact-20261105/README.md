# SoundPlayerView::GetFmtIndexByName: indexed table search codegen exact

Original target Japanese TH10 v1.00a (`0x0043D2A0`), SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target's complete **227-byte** music-format name lookup uses a 128-byte
basename buffer. It searches `/` and then `\\` delimiters, copies the basename,
walks native 0x34-byte format records until matching the requested name or the
first empty slot, and falls back to index zero when no entry matches. Its
register schedule initializes the zero logical index **before** the delimiter
search, then keeps EBP as the logical index and EDI as the record-table base.

The previous maintained source placed `int index = 0` immediately before the
format-table loop; the compiler chose an extra early register save and a
longer **236-byte** PDB contribution with the opposite register roles. The
source now moves that *same* integer initialization to immediately after the
`char basename[128]` declaration, before the first `strrchr`. This is an
ordinary, natural C++03 declaration-order correction: zero changes to the
control flow, string comparisons, table extents, buffers, or missing-name
behavior. The real VC7.1 SP1 compiler now emits the target schedule.

## First original-bound full-body native proof

A fresh **five-translation-unit** `/GL` source graph, including modified
`Sound.cpp` and original-profile `Main.cpp`, `FileSystem.cpp`, `PbgArchive.cpp`,
`GameErrorContext.cpp`, produced the complete **227-byte** PDB contribution.
A strict target-original comparator independently checks all four true link
identities and all 227 bytes, not just candidate geometry:

- `+0x007`: `___security_cookie` at target `0x00473660`
- `+0x01E`, `+0x02D`: `_strrchr` at target `0x00452930`
- `+0x0D6`: `@__security_check_cookie@4` at target `0x00458EA5`

Result: **227/227 exact**, 4/4 real target-bound fields, zero differences;
no ignored branch, local destination or disassembly tail. Crucially the same
new graph retained the already accepted `SoundDataLoaderThread` owner at
**107/107** bytes with all 11 field locations unchanged.

Private trial source and original-bound proof:
`.analysis/gpt-web/20261105-next-sound-fmt/trial.py`,
`.analysis/gpt-web/20261105-next-sound-fmt/verify-strict.py`,
`build/gpt-web/20261105-next-sound-fmt/index-before-search/`.

The source checkpoint precedes independent canonical unit cold replay.
It does not claim whole-game product, runtime equivalence or portability.
