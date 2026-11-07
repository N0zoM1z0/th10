# ECL StartSubroutine: native word-sized CALL argument and stack offsets

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmContext::StartSubroutine` processes an inline counted subroutine name
and the subsequent tagged script argument pairs, updates the VM stack and
calls the *real* `EclVmScriptDatabase::FindSubroutine` provider. The target
function is **550 bytes**; the current complete PDB-owned candidate remains
**551 bytes / four candidate call fields** and is **NONEXACT**. The related
`EclVmContext::Run` target/candidate length is 7,020 with 189 fields in
this isolated ECL graph, also not a new exact owner.

## Source semantics

The previous C++ used `previousTop + 8` and
`metadataOffset += sizeof(EclVmCallArgumentView)` in signed `int`
intermediates. Signed integer overflow is undefined in C++, although
native 32-bit i386 ADD preserves the low 32 bits even for malformed or
large script count/stack metadata. The installed source now computes
three cursor positions in the actual **unsigned 32-bit word domain** and
publishes the native low word back as `int`:

- initial metadata cursor: counted inline name bytes plus the unsigned
  first-argument dword offset and prefix
- initial stack argument byte offset: previous signed top plus 8
- per-argument metadata cursor update: descriptor stride in low word

The original read/compare order, signed metadata consumer, serialized
record layout, stack-pointer uses, local buffer lifetime, real callee,
branch destinations and argument semantics are otherwise unchanged.
These are native i386/VC7.1 codegen semantics, **not** an added bounds
check, safety validation, original source spelling or portability proof.

## Whole-owner trial and current-source original-target guard

Five genuine VC7.1 SP1 `/GL` source variants were compiled and linked in
the real `EclVmContext::Run` one-TU producer: each operation separately,
all three combined, and an alternative byte-pointer word address view.
Each complete native graph retains unchanged **551-byte StartSubroutine
with four candidate fields**, **7,020-byte Run with 189 fields**, and
**136-byte FindSubroutine with zero fields**. Every field offset/type/width
and every ordinary candidate byte is unchanged. The already exact
FindSubroutine separately proves **136/136 original bytes** under the
strict comparator. The pointer-word alternative was not installed.
All experiments used the current source epoch and restored temporary
changes byte-for-byte.

The installed combined source was then selected for strict original-target
cold replay in both the one-TU ECL graph and the separate real ECL/Enemy
host graph. Four previously accepted original-bound units passed:

| Exact owner | Original-bound bytes |
| --- | ---: |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-host-run` | 146/146 |

**609/609**, all original linked field identities, two actual native build
artifacts. This selected guard does **not** make the 551-byte StartSubroutine
or large 7,020-byte ECL Run exact.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-int \
  --unit ecl-vm-read-float --unit ecl-vm-host-run --json
```

Changed source SHA-256:
`da854e5838cebc2ee834796bdec88b136a87614496dda34f5a2bdaf88ed09e4f`.
Native trial source/PE/MAP/PDB and selected strict guard receipts are
retained under ignored `.analysis/gpt-web/20261107-ecl-start-word/`,
`build/gpt-web/20261107-ecl-start-word/`, and
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
No partial code credit or whole-historical cold replay.
