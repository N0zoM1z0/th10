# SCPT ECL table loading: native i386 32-bit file and name byte addresses

Original target: Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmScriptDatabase::AddScriptData @0x00450220` parses one SCPT header,
its include payload, a counted table of **32-bit instruction offsets**,
the consecutive subroutine-name bytes, and a sorted merged name map. Both
the original native function and current linked candidate contain **589
bytes**. The existing _malloc/_free call fields at `+0x6C` / `+0x101` are
independently bound to the original symbols and positions. The owner is
**NONEXACT**: **502/581** other comparable original bytes remain identical,
79 differ. No partial byte or new exact owner credit is granted.

The source previously expressed several byte-stream operations as C++
single-object pointer additions or increments. Serialized offsets and
lengths need not designate elements of one C++ array object, even though
the original i386 hardware forms an address by adding 32-bit words. The
maintained source now publishes the original *low 32-bit* results for:

- Initial SCPT offset-table cursor: file pointer + actual `0x24` header
  size + the encoded include length.
- Name-stream origin: offset table address + (count times four bytes).
- Both subroutine-header pointers: file pointer + the actual serialized
  `instructionOffset` dword, not a guessed fixed script struct.
- Both per-entry four-byte offset-cursor advances.
- Both null-terminated name-string advances by `strlen(name) + 1` bytes.
- Include-package address: original file pointer + the header's real size.

The **existing parameter-stack slot** is still the offset cursor with its
compiler-observable `const volatile` read view; the original merge/control
and counted-name loops are unchanged. This does not invent bounds checks,
source data, ABI tricks or a portable host-pointer guarantee. Win32 i386
semantics are intentionally distinguished from C++ object-based arithmetic.

## Eight genuine native source controls

Six scoped natural C/C++ address forms were tested **individually**, then
the header-offset/cursor pair and **all six together**, by recompiling the
actual `EclVm.cpp` TU and linking its real pinned `Enemy.cpp` support using
VC7.1 SP1 `/GL /GS` with current-source hashes. Every complete
`AddScriptData` PDB contribution stayed **589 bytes**, both real CRT field
positions/types remained **+0x6C/+0x101**, and the strict original
comparison stayed **502/581 ordinary bytes**. Every complete linked
`EclVmContext::Run` stayed **6,997/7,020** with all **189 independently
original-bound fields** and the real 171-byte `ReadFloatValue` callee exact.
The candidate-to-candidate 589-byte code was also byte-identical under the
same fixed original relocation mask for all eight trials. **Neither large
owner is exact**, and no redefined relocation mask was applied.

The combined source was installed and **independently cold-replayed** for
six previously canonical exact units in only two affected native graphs:

| Existing exact unit | Original-bound bytes |
| --- | ---: |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-host-run` | 146/146 |

**904/904** original bytes, all declared original linked fields. The
selected final ECL/Enemy source image was checked again independently:
AddScriptData still **589-byte candidate**, both CRT fields at the target
positions, 79 other mismatches; complete Run **6,997/7,020**, 189 fields.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-float --unit ecl-vm-read-int \
  --unit ecl-vm-read-int-value --unit ecl-vm-read-float-value \
  --unit ecl-vm-find-subroutine --unit ecl-vm-host-run --json
```

Installed `src/EclVm.cpp` SHA-256:
`83cbd5227e9b366181970dbd2cceb6ceaeabfde056c7c8cdda3a5744900fc70b`.
Full real trial PDB/MAP/PE source hashes and fresh selected cold receipts:
ignored `.analysis/gpt-web/20261107-ecl-add-script-word/`,
`build/gpt-web/20261107-ecl-add-script-word/`, and
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
No all-historical cold, game-product reconstruction, runtime-equivalence
claim, original physical C++ source identity or extra exact promotion.
