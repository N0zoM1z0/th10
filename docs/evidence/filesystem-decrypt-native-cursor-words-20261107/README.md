# TH10 FileSystem::Decrypt: native stream cursor and chunk-counter words

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The 323-byte target `FileSystem::Decrypt @0x0044B0D0` shuffles chunks of
XOR-decoded replay data with Win32 i386 address and integer arithmetic.
The current natural VC7.1 source is not yet codegen exact for this owner:
a single normal-COFF build emits **278 bytes**, not 323. It retains
two CRT call relocations (`malloc`, `free`). No physical target owner
or full-game exactness is inferred from the retained source.

The original i386 executes native 32-bit `ADD`, `SUB`, `LEA` for the
unchecked encrypted-byte stream cursor and buffer offsets. Existing
source constructs potentially out-of-array C++ pointers at
`outputCursor += chunkSize - 1`, `outputCursor -= 2`, both second-half
pointer resets and `inputCursor++`. The maintained source now uses a
small forced-inline `NativeCryptoByteOffset` helper to form each
32-bit address word modulo 2^32. The two chunk-local size counters,
`unencryptedBytes`, and the odd-block `(chunkSize + 1)` count are also
expressed as unsigned-word operations with the original signed loop
bounds/divisions. No validation, changed XOR order, new byte swap or
promise of malformed-input safety was added.

## Live original-target and full candidate controls

Four separate VC7.1 SP1 normal-COFF source trials were compiled:
Decrypt pointer words, Decrypt size counters, odd-block count, and the
installed combination. All four preserve the **entire 278-byte**
Decrypt candidate with both CRT relocations unchanged at their original
candidate locations. The neighboring 279-byte Encrypt candidate and its
two CRT fields are also unchanged. Each is still far from the original
323-byte and 307-byte owners, and the ordinary original-target comparison
is **11/315 and 8/299 comparable bytes**, respectively. These low
scores are **not** represented as partial exact credit.

After source reformatting, the final complete 278/279-byte normal-COFF
candidates and all field symbols/offsets were compared against a fresh
pre-edit baseline again: **zero candidate ordinary differences** in
both functions. Five previously exact FileSystem/Archive owners were
cold-replayed in their proper linked graphs from installed source:

| Existing exact owner | Original bytes |
| --- | ---: |
| FindArchive | 212/212 |
| LoadArchive | 36/36 |
| ArchiveOrDiskPathView::Read | 214/214 |
| ArchiveOrDiskPathView::GetSize | 190/190 |
| PbgArchive::Load | 108/108 |
| **Total** | **760/760** |

This is Win32 source-semantic recovery, **not** a new exact owner.
Selected reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-find-archive --unit filesystem-load-archive \
  --unit filesystem-path-read --unit filesystem-path-size \
  --unit pbgarchive-load --json
scripts/repo-python scripts/ci.py
```

Private source/COFF and selected original-bound evidence is in
`.analysis/gpt-web/20261107-active/filesystem-crypto-native-pointer-probes.py`,
`filesystem-crypto-native-trials.json`, `filesystem-crypto-decrypt-replay.json`,
and `build/gpt-web/20261107-filesystem-crypto-words/`.
