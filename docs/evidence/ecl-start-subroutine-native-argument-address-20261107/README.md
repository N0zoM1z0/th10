# TH10 ECL CALL: real 32-bit byte addresses for stack and tagged arguments

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmContext::StartSubroutine @0x0044DF70` is the original complete
**550-byte** routine handling a counted inline subroutine name, its tagged
8-byte argument records, the destination VM stack, the saved frame word,
and `EclVmScriptDatabase::FindSubroutine`. The maintained PDB candidate is
**551 bytes** with four real call fields; it remains **NONEXACT**.

The previous C++ had signed script-provided byte positions used in `data +
argumentOffset`, `argument += 4`, `operands + metadataOffset`, and
`data + previousTop - 4`. These C++ single-object array pointer expressions
can be undefined for a negative offset or a displacement crossing the
current object's bounds, while the original i386 uses low 32-bit address
word addition. Source now expresses **four independent native address
formations**, retaining the exact original byte positions and integer
wrap semantics:

- Initial destination argument pointer, from stack bytes and the already
  computed low-word `argumentOffset`.
- Each four-byte argument cursor advance.
- All three source/destination tag accesses inside the loop, based on
  the serialized metadata cursor rather than a fabricated fixed array.
- Saved caller frame word placement at `stack.data + previousTop - 4`.

This source does not add bounds validation, shift operand calls, change
serialization, modify stack ownership or force ABI registers. The result
is explicitly pinned to 32-bit MSVC codegen, not portable pointer validity
or proof of the historical C++ spelling.

## Actual complete-parent native trials

Seven independent current-epoch source variants were really compiled and
linked with pinned VC7.1 SP1 `/GL /GS` using the real `EnemyRuntimeUpdate`
entry in the complete two-TU ECL/Enemy graph: each of four expressions,
argument+increment, argument+saved word, and all four together. Each
trial's entire PDB-owned `StartSubroutine` remains **551 bytes**, preserving
**4/4 existing candidate call-field offset/type/width obligations** and
**zero ordinary byte differences from the same pinned baseline**. These
are candidate-vs-candidate comparisons, not original exactness.

Every trial separately compared the **entire original 7,020-byte**
`EclVmContext::Run` and its 189 independently original-bound fields:
**6,997/7,020 ordinary original bytes** and **189/189 original fields**
remain, along with exact `ReadFloatValue @0x0044FF80` (171 bytes).
Neither nonexact owner is promoted.

## Installed-source focused cold, no full historical replay

The actual combined source was installed and independently cold-replayed
for eight related canonical exact units across one-TU and two-TU graphs:

| Existing exact owner | Original bytes |
| --- | ---: |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-stack-push` | 96/96 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |

**1,081/1,081** strict original bytes with their actual field identities.
The final selected native ECL/Enemy image was also compared against the
complete original `Run`: again **6,997/7,020**, all **189** fields, float
callee 171 exact. **Zero newly exact functions or codegen bytes.**

Reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-float-value \
  --unit ecl-vm-read-int-value --unit ecl-vm-stack-push \
  --unit ecl-vm-stack-pop --unit ecl-vm-read-int --json
```

Final installed `src/EclVm.cpp` SHA-256:
`40d43d41bddcb6daa0a1bdec9d6515f498b06114d30776f056a6f0e74ab5ee46`.
Full trial source hashes and real PE/MAP/PDB are retained in ignored
`.analysis/gpt-web/20261107-ecl-start-pointer-address/`,
`build/gpt-web/20261107-ecl-start-pointer-address/`, and selected cold
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
No full game EXE, original compiler partition or runnable runtime claim.
