# ECL Run FORMAT scalar payloads: native 32-bit byte addresses

Original: Japanese TH10 v1.00a (`resources/th10.exe`, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
The complete original `EclVmContext::Run @0x0044E1A0` is **7,020 bytes**
with **189 independently original-bound** code/data fields. It is
**NONEXACT** and remains at **6,997/7,020** matching original bytes.

`ECL_VM_EVALUATE_FORMAT_OPERANDS` reads a counted inline format string and
then eight-byte type/scalar records. Two live calls (float and integer
readers) indexed `instruction->scalarOperands[index]`, but the declaration
is a *one-element overlay of a variable-length serialized stream*, not a
C++ array of that dynamic length. When the script carries multiple FORMAT
operands, ordinary array indexing can invoke C++ out-of-bounds pointer
arithmetic despite the original flat i386 address being well-defined as a
machine word.

The maintained source now uses the real wire offsets and unsigned 32-bit
address words for both accesses:

- Base `reinterpret_cast<unsigned int>(instruction)` plus actual
  `offsetof(EclVmInstruction, scalarOperands) == 0x10`;
- Signed byte-count division by four **before** conversion to a word;
- Native `+2` scalar header words, plus the eight-byte record's two
  four-byte words per argument;
- Final dword index times four using unsigned arithmetic.

No extra range check, new allocation, fake ABI, or target byte is inserted.
It remains pinned to the real VC7.1 i386 pointer-width model rather than
claiming cross-platform portability or malformed-script safety.

## Native experiments and exact regression

A fresh actual current-source pinned VC7.1 SP1 `/GL` **two-TU** control
(`EclVm.cpp` + `Enemy.cpp`) was compiled and compared to the **complete**
7,020-byte original owner before editing: 6,997 matching bytes, all 189
original fields, and an independently exact 171-byte `ReadFloatValue`.
Six distinct typed FORMAT source variants were separately compiled from
the real worktree and fully original-bound compared. Float-only, int-only,
their combination, a different signed-index address expression and a
common helper all regressed to **6,996/7,020** and were rejected. The
selected **direct unsigned byte-address** form retains the full original
extent, all 189 original-bound fields, the same 23 mismatch offsets and
exact float callee. This original-bound acceptance is *nonexact* for Run,
not candidate-vs-candidate masking and not new canonical credit.

The installed source, with final comments and formatting, was **cold
replayed** in only eight previously accepted affected ECL units:
`ecl-vm-host-run`, `ecl-vm-read-float`, `ecl-vm-read-int`,
`ecl-vm-read-float-value`, `ecl-vm-stack-pop`, `ecl-vm-resolve-float`,
`ecl-vm-find-thread` and `ecl-vm-spawn-thread`. All **1,037/1,037**
original bytes and their declared links passed. The same freshly compiled
source was separately compared against the entire `Run` parent: still
**6,997/7,020** and 189 fields. No full historical replay was run.

Ignored native evidence: `.analysis/gpt-web/20261107-turn2/
ecl-format-pointer-trials.py`, `ecl-format-pointer-results.json`,
`ecl-format-installed-guards.json`, `ecl-format-installed-run.json`;
linked trial PE/MAP/PDB in `build/gpt-web/20261107-turn2-ecl-format/`.
No product build/runtime or physical original-TU inference follows.
