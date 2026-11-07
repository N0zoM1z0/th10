# ECL Stack Push: native i386 low-word type and value addresses

Target original TH10 Japanese 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `EclVmStackView::Push @0x004505B0` is a complete **96-byte
exact** method. The native code writes a type-byte using `MOV [ECX+EAX],DL`
at `+0x23`, advances `stackTop` by four, and uses the resulting machine-word
address with `REP MOVSD/MOVSB` to copy the scalar value. Both destinations
come from an actual `data` buffer base plus the signed stack-top dword,
not from a guaranteed in-bounds C++ array subobject. The previous C++
`data + stackTop` pointer formation could be undefined beyond that single
array's extent even though original i386 simply forms a low-word sum.

The maintained C++ now expresses the **two** real destinations as
`reinterpret_cast<unsigned int>(data) + static_cast<unsigned int>(stackTop)`
and then casts the result back to the same `unsigned char *`; the existing
`EclVmStackTypeSlotView` overlay, `memcpy` call, original type tests,
capacity check, unsigned-word counter advances, and return behavior remain.
This is a pinned Win32 i386 source-semantic reconstruction, not an added
bounds check, portable pointer arithmetic promise or historical source
spelling claim.

## Full original-bound and large-parent proof

Nine independent natural-source VC7.1 SP1 `/GL` variants compiled the
*complete* `Push`, `Pop`, `EnterFrame`, and `LeaveFrame` owners, varying
both Push destinations, the EnterFrame saved-word pointer and increment,
and the LeaveFrame saved-word pointer, separately and together. **All nine**
retain strict original exact **Push96, Pop122, EnterFrame64, LeaveFrame41**
with every declared field. The same one-TU full `Run` retains its own
**6,996/7,020** and **189 original-bound fields** without any regression.
This one-TU score differs from the separate ECL/Enemy two-TU source context;
do not confuse them or promote any partial codegen credit.

After installing **Push only**, a *fresh selected original cold replay*
compiled six relevant existing exact owners across two linked artifacts:

| Owner | Original bytes |
| --- | ---: |
| `ecl-vm-stack-push` | 96/96 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-stack-enter-frame` | 64/64 |
| `ecl-vm-stack-leave-frame` | 41/41 |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-host-run` | 146/146 |

**593/593** original bytes; five actual linked fields remain accepted.
The independently original-target-bound **complete two-TU** `Run` retains
**6,997/7,020**, **189/189** real original fields and the 171-byte
`ReadFloatValue` callee exact. `Run` is **NONEXACT** and adds zero credit.
No historical all-unit cold or game executable link was run.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-stack-enter-frame --unit ecl-vm-stack-leave-frame \
  --unit ecl-vm-read-int-value --unit ecl-vm-host-run --json
```

Installed `src/EclVm.cpp` SHA-256:
`393f5c55cd14ad9d1eecbaba3ec9945e6954f05f672516350af2c001440046e3`.
Real trial native PE/MAP/PDB and installed-source target cold:
`.analysis/gpt-web/20261107-ecl-frame-remaining-word/`,
`build/gpt-web/20261107-ecl-frame-remaining-word/` and
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
