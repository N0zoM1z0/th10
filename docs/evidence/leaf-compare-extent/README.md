# Complete RET4 extent for the comparison leaf at404E90

2026-10-22, starting at `fdc0240`. Fresh target-bound inventory diagnostics
contradict the old claim that this leaf ends at404E9E. The complete target
instruction at404E9E is RET4, three bytes through404EA0; CC alignment begins
at404EA1. Dense decoding covers all17 bytes. No neighboring tracked extent
overlaps them. Source, member ABI and indeterminate origin remain unchanged.

The old canonical unit already compared all17 native/target bytes using
`size=15, compare_size=17`, but the function, boundary and match ledgers credited
an extent ending at the RET opcode. They now consistently use17 bytes and the
redundant compare_size is removed. This corrects existing whole-owner accounting;
it adds two overall exact bytes but no authored bytes, functions or source.
It does not establish source authorship, original class or physical TU.

A fresh pinned VC7.1 `/GL` cold build passes17/17, dense decoding17, zero linked
fields and raw equality across the complete PDB contribution. No compiler flag,
source expression, ABI, target byte or acceptance rule changes. The focused
command is:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit game-decomphelp-compare-00404e90 --json
```

Three negative controls use only a temporary copied native image/manifest.
Changing either immediate byte independently yields16/17, while declaring15
bytes without compare_size is rejected against the17-byte PDB contribution.
The copied positive remains17/17. Original target, carrier and source hashes
are unchanged. Temporary copied binaries are removed. The native carrier is
diagnostic, with no product/runtime credit. [Full evidence](evidence.json)
records the old/new rows, cold result, controls and hashes.

The session also refreshed read-only/noanalysis Ghidra architecture and complete
412289-byte mapped-text attestation at maxcpu1/768 MiB. The independent global
`report-boundary-inventory.py --check-ledger` still fails: its automatically
generated metadata and some states differ from historical manual reviews.
The evidence records current counts. Those differences include reviewed owned
tables, isolated entries and overlapping runtime entries; they are not proof
that every manual boundary is wrong. No wholesale rewrite or gate relaxation
is performed. The corrected leaf now has the same17-byte geometry and reviewed
state in the fresh automatic audit. Ordinary tracking and CI checks do not
resolve the remaining global audit drift.

All105 standard CI checks pass for the corrected source/ledger baseline.
[Validation and cleanup](validation-summary.json) records the log identity,
focused checks and removed reproducible baseline/CI outputs. No unclassified
legacy artifacts, target, toolchain or independent D3D evidence are removed.
