# Enemy ECL spell-name byte stream: native input and output address words

Original executable: TH10 Japanese 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The shared `START_SPELL` opcode family in the complete **14,416-byte**
`EnemyRuntimeView::DispatchEclInstruction` reads a counted encrypted name
from script bytes, XOR-decodes it using the original evolving two-byte key,
and writes to the local spell-request byte overlay. The name length is not
clamped to the C++ array extent. Original i386 emits flat 32-bit addressing
(`+0x273B`: `8A 0C 31`, byte read through ECX+ESI) without a buffer range
check. The prior C++ `local_90.bytes[iVar17] = encryptedName[iVar17] ^ bVar3`
formed potentially out-of-array pointers for both the input and output
on invalid or oversized byte counts.

Both accesses now use the already-present typed `EnemyNativeWordIndex`
with **one-byte stride** and modulo-32-bit address arithmetic. The
script-decoding byte, key progression, positive-length gate, and write
order have not changed. This is native Win32 behavior, not a new bounds
check, buffer-safety assertion, or portability proof.

## Fresh full-owner native trials and installed-source validation

Starting from the fresh source-pinned **11-TU** `/GL` graph, five separately
compiled source variants changed the source read, output write, both,
read through a named local, or address-index scalar. Each preserved the
complete **14,352-byte** dispatcher *candidate*, its 716 candidate linked
fields, and the four neighboring complete candidate owners (2,432 / 162 /
330 / 672 bytes). The installed both-word form is byte-for-byte neutral
in this candidate graph; this does **not** establish original-bound exactness.

The original dispatcher remains **14,416 bytes** and NONEXACT, a 64-byte
physical extent gap. An additional three **full 11-TU** compiler trials
combining the native name words with `SET_MAIN_ANM_SCRIPT` / `PLAY_SELECTED_ANM`
VM ID word indices each changed one ordinary byte in the candidate's
name-decryption loop at `+0x271F`, `8A 0C 31` to `8A 0C 0E`, while
original at `+0x273B` requires `8A 0C 31`. These three combinations were
rejected, not installed. Candidate-linked fields are not original-bound
claims.

Installed-source selected canonical original-bound cold replay passes
five ECL operand helpers: **16+14+14+17+26 = 87/87** original bytes and
all their declared original code fields. Default lightweight CI passes
seven stages; no all-unit cold or whole-product build ran.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
scripts/repo-python scripts/ci.py
git diff --check
```

Current native artifacts and compiler/byte comparisons are kept under
`build/gpt-web/20261107-active-enemy-graph/`. Trial source and metadata:
`.analysis/gpt-web/20261107-active/enemy-spell-name-results.json`,
`enemy-name-vm-combo-results.json`, and `enemy-spell-name-exact.json`.
