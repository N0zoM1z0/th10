# ECL FORMAT: replace a punning counter with its native unsigned word

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This change affects the actual **7,020-byte** `EclVmContext::Run @0x0044E1A0`
interpreter, specifically the two data-dependent paths inside the FORMAT
opcode. No original target bytes or compiler flags are modified.

Before this change, the local `int argumentIndex` was incremented through
`++*reinterpret_cast<unsigned int *>(&argumentIndex)` in both the float and
integer FORMAT paths. The original i386 updates a 32-bit index word; the
source now expresses that contract directly as `unsigned int argumentIndex`
and `++argumentIndex`. The operand-record lookup was already performed using
unsigned native-word multiplication and addition. This is a natural C++
representation of the observed modulo-2^32 index without aliasing a signed
local through an unrelated pointer expression or relying on signed overflow.
It does **not** add a format-length/bounds guard absent from the retail VM.

## Source-specific comparison

A new pinned VC7.1 SP1 two-translation-unit `/GL` build, using the real
EclVm/Enemy graph, reconstructs **7,020/7,020 bytes of owned extent** and
**189/189 independently retail-bound local and external code/data fields**.
The full owner is **6997/7020 exact bytes, NONEXACT**; the complete 23
remaining mismatch offsets are identical to the pre-edit source. The
171-byte same-image `ReadFloatValue` guard remains exact. A separate
`int`-plus-unsigned-assignment source variant regressed by one byte and was
rejected. The installed unsigned local is the codegen-neutral variant.

The four affected accepted ECL units were rebuilt together from the
installed source using selected-only cold replays:

| Accepted unit | Complete original bytes |
| --- | ---: |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-host-run` (small host entry, **not** the Run boss) | 146/146 |
| **Total** | **575/575** |

Reproduction:

```sh
scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-host-run
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-find-subroutine --unit ecl-vm-stack-pop \
  --unit ecl-vm-read-float-value --unit ecl-vm-host-run --json
scripts/repo-python scripts/ci.py
```

The noncanonical Run diagnostic exits 1 because the owner is intentionally
still nonexact. Its complete native comparison is retained in ignored
`build/gpt-web/20261109/ecl-unsigned-index-installed.json`, the focused
source experiments in `.analysis/gpt-web/20261109/ecl-format-natural-index-variants.py`
with results in `build/gpt-web/20261109/ecl-natural-index-angle/results.jsonl`,
and the selected exact-unit receipts in
`build/gpt-web/20261109/ecl-unsigned-index-guards.json`. No whole cold replay,
original source identity, native runtime equivalence, or new exact credit is
claimed.
