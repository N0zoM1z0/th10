# TH10 FileSystem::Encrypt: native replay XOR stream address words

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `FileSystem::Encrypt @0x0044B220` is a **307-byte**
code owner. The maintained VC7.1 source is still nonexact: normal-COFF
compiles it to **279 bytes** with two CRT call relocations. The adjacent
`FileSystem::Decrypt @0x0044B0D0` is likewise original **323** versus
candidate **278** bytes. These are true extent gaps, not partial exact
codegen acceptance.

TH10's original replay XOR shuffler advances the input cursor backwards
through alternating byte positions, while advancing output forward. The
original i386 uses native `ADD/SUB/LEA` low-32-bit address words for
those unchecked offsets. Source `inputCursor += chunkSize-1`,
`inputCursor -= 2`, `inputCursorBackup + chunkSize-2`, and `outputCursor++`
formed potentially out-of-array C++ pointers. Encrypt now reuses the
existing forced-inline `NativeCryptoByteOffset` to form these native
Win32 cursor words without adding any bounds check.

Like Decrypt, the unencrypted-byte adjustment, chunk remainder and
maximum-byte decrements, and `chunkSize+1` odd-half length now explicitly
use modulo-32-bit integer arithmetic, while keeping the original signed
loop tests and divisions, both XOR passes, byte order, and failure paths.
This is faithful target-specific arithmetic, not a claim of safe malformed
input handling or portable 64-bit pointer behavior.

Four independent natural C++ source variants (pointer addressing only,
size counter words only, odd-half count only, and their combination) were
normal-COFF compiled using the pinned VC7.1 SP1 toolchain. Every one
kept the **complete 279-byte Encrypt** and **278-byte Decrypt** candidate
byte-identical, including the same two CRT relocation positions for each.
After installing and formatting the combination, both complete functions
were compiled once more: zero candidate ordinary differences and identical
field geometry relative to the pre-edit installed source. The existing
original-target ordinary-byte matches remain **8/299** (Encrypt) and
**11/315** (Decrypt), *not* a new exact byte claim.

The five already accepted FileSystem/Archive owners are checked with
original-bound cold replay from the installed source:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-find-archive --unit filesystem-load-archive \
  --unit filesystem-path-read --unit filesystem-path-size \
  --unit pbgarchive-load --json
scripts/repo-python scripts/ci.py
```

Their selected comparison must pass **760/760** original bytes across
five functions with all 45 independently original-bound references.
No full historical replay, executable build or runtime proof is claimed.

Private trial evidence:
`.analysis/gpt-web/20261107-active/filesystem-encrypt-native-probes.py`,
`filesystem-encrypt-native-results.json` and complete normal COFF
`build/gpt-web/20261107-filesystem-encrypt-words/`.
