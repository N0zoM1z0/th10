# TH10 ECL FindSubroutine: native first-instruction return pointer

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmScriptDatabase::FindSubroutine @0x00450470` occupies a complete
**136-byte original** function. After binary search finds the eight-byte
subroutine entry, the target returns `header+0x10` using the native i386
`ADD EAX,0x10` at function offset `+0x80`. The maintained source formerly
formed `mid->header + offsetof(EclVmSubroutineStreamView, firstInstruction)`
through a C++ byte-array pointer addition, which assumes valid array
bounds even on corrupt native script records. The installed natural C++
now forms the same low-32-bit word through unsigned integer addition then
casts back to `EclVmInstruction *`. No bounds check, changed call ABI or
new return path is introduced; this is a Win32 native semantics claim,
not a malformed-input safety or portable 64-bit behavior guarantee.

## Complete original-bound real two-context tests

Two independent C++ source variants were tested: direct unsigned-word
address construction and a named unsigned-word local. Each was compiled
using pinned VC7.1 SP1 `/GL` under **both** the original standalone
`EclVmStack` linked graph and the genuine two-TU `EclVm.cpp` + `Enemy.cpp`
`EclVmHostRunEnemy` graph. Both variants preserve:

- **FindSubroutine: 136/136 exact**, zero relocation fields.
- **Stack Pop: 122/122 exact**, one original CRT field.
- **Stack Push: 96/96 exact**, zero relocation fields.
- Full **EclVmContext::Run @0x0044E1A0**: 7,020-byte original and candidate
  extent, **6,997/7,020 exact original bytes**, all **189 original-bound
  linkage fields**, exact ReadFloatValue callee. The remaining 23 ordinary
  bytes are unchanged, so **Run stays NONEXACT**.

The direct natural form was installed. A fresh current-source canonical
selected cold replay in the actual linked graphs passed **eight existing
exact VM units totaling 1,081/1,081 original bytes**. A separate full
source-epoch `Run` diagnostic confirmed the 7,020-byte extent, 6,997
matched bytes, all 189 target fields and exact float callee. That
large-owner diagnostic intentionally exits nonzero because Run remains
nonexact; this is not a compiler or linker failure.

This is a source semantic recovery checkpoint, **zero new exact owner or
canonical original-byte credit**. No historical all-unit cold replay,
whole-game binary or runtime equivalence is asserted.

Selected reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-float-value \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-read-int --unit ecl-vm-read-int-value --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run
scripts/repo-python scripts/ci.py
```

Private experiment script, PE/MAP/PDB artifacts and final receipts:
`.analysis/gpt-web/20261108-ecl-find-return-word.py`,
`.analysis/gpt-web/20261107-active/ecl-find-native-return-results.json`,
`.analysis/gpt-web/20261108-ecl-find-native-installed-replay.json`,
`.analysis/gpt-web/20261108-ecl-find-native-installed-run.json`,
`build/gpt-web/20261108-ecl-find-native-return/`.
Source SHA-256 `f711207c627fa33814337dcd1453ce60aee4158654c22d14f8b8e5b59a60b002`.
