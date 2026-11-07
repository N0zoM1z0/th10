# TH10 ECL subroutine binary search: native 32-bit index arithmetic

Original target: Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete original `EclVmScriptDatabase::FindSubroutine @0x00450470` is
**136 bytes**. It is already canonical exact; this work improves the source
arithmetic model rather than claiming a new exact owner.

The original i386 binary search decrements the loaded subroutine count,
computes its interval delta and signed half, adds back the lower index,
and increments/decrements the selected index as native 32-bit dwords.
Previously the maintained C++ used signed subtraction/addition for `upper`,
`middle`, and search-bound updates. These expressions have undefined behavior
at signed overflow even though the original processor retains the low word.
The source now explicitly computes those additions/subtractions as unsigned
`int` words, reinterprets them as `int` for the original signed comparisons
and the crucial **signed** `difference / 2`, and otherwise retains the same
array indexing, `strcmp`, early return and real `header + 0x10` instruction
prefix. It does not add safety bounds, alter sorting, change memory layout,
force registers, invent a native callee or copy target opcodes.

## Actual independent original-target and surrounding-owner trials

Five independent natural source variations were compiled using pinned
VC7.1 SP1 `/GL` with the actual `EclVmContext::Run` entry: unsigned upper
bound, unsigned interval subtraction, fully wrapped midpoint, wrapped
selected-bound updates and all four groups combined. Every trial produced
one **complete 136-byte PDB contribution matching all 136 original bytes**,
zero link fields. The same source image retains `EclVmContext::Run`
**7,020 candidate bytes/189 candidate fields**, and `AddScriptData`
**588 candidate bytes/two fields in this one-TU graph**, each identical to
the prior *same-graph* candidate in its complete ordinary-byte and field
geometry. This is candidate-preservation evidence only for those two
nonexact large owners; their original-bound status is **not** promoted.

The selected combined source was installed, formatted, and **cold-replayed
again from the final source**. The three original-bound, previously exact
`ecl-vm-find-subroutine`, `ecl-vm-read-int` and `ecl-vm-read-float` owners
pass **136/136 + 144/144 + 183/183 = 463/463** native bytes, with all
existing original field identities. A separate complete linked image check
of the installed formatting against the actual combined-trial PE/PDB
confirms zero ordinary changes across Run7020, AddScriptData588, and
FindSubroutine136 and all original candidate linkage positions.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-int \
  --unit ecl-vm-read-float --json
scripts/repo-python scripts/ci.py
```

Source SHA-256:
`33f9eee6ed7df8df500a1c17bf7af38ad23bde80b1759e4ea7946d451c173c66`.
Ignored local full trial/evidence:
`.analysis/gpt-web/20261107-ecl-binarysearch/` with source/pin results,
`formatted-selected.json`, and the pinned PE/MAP/PDB under
`build/gpt-web/20261107-ecl-binarysearch/` and
`build/match-linked/EclVmStack/`.
No new exact byte credit, all-unit cold, complete executable product,
portable C++ pointer guarantee or original physical source identity is claimed.
