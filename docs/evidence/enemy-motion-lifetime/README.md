# Enemy motion lifetime and comparison lowering

2026-10-22, starting e260222. Six compiler controls isolate two useful changes
inside the large Enemy dispatcher: explicit component sums eliminate a spilled
motion pointer, and writing the Y comparison as `y > -999999.0` produces the
target's direct non-popping FCOM. Both changes are adopted together.
The 0x118/0x11a group shrinks from 193 to 180 bytes against target 179.
**This is not a one-byte mismatch or an exact group**: opcode/pointer registers,
stack offsets and instruction scheduling still differ.

The complete parent changes from 14368 to 14352 bytes against target 14416,
so its total size deficit grows from 48 to 64. Aggregate absolute group gap
improves 348 to 336. These are separate diagnostics; neither total size nor
aligned-byte score is an exactness claim. No new exact credit is recorded.

## Target observations and falsifiable controls

The target identity is original Japanese v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh disk and direct read-only Ghidra attestation passed. Ghidra uses one CPU,
768 MiB and a rolled-back transaction; the complete 412289 mapped bytes remain
unchanged (SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`).
A fresh executed pinned toolchain preflight also passed.

At 40F14A, the target selects the motion pointer into ESI before two float reads.
It compares X from its float spill and compares the remaining Y directly with
the double sentinel via FCOM. It then adds corresponding base/offset position
components and copies the three results to world position. The previous native
shape saves the motion pointer on the stack, keeps offsetMotion in EDI through
the calls, and duplicates ST(0) before the Y comparison.

| Control | Whole parent | Motion group /179 | Aligned bytes | Absolute group gap |
| --- | ---: | ---: | ---: | ---: |
| Previous maintained | 14368 | 193 | 3250/11139 | 348 |
| Branch-local motion pointer | 14368 | 193 | 3253/11139 | 348 |
| Local pointer and float operands | 14368 | 193 | 3253/11139 | 348 |
| Integer motion address | 14368 | 193 | 3253/11139 | 348 |
| Explicit three-component sum | 14352 | 182 | 3246/11132 | 336 |
| Explicit sum and double Y | 14352 | 182 | 3246/11132 | 336 |
| **Explicit sum and Y-left comparison** | **14352** | **180** | **3251/11134** | **336** |

The local-pointer and integer-address controls have identical raw bytes across
all four complete dispatcher/ANM/ECL/factory bodies. Local operand declarations
change dispatcher bytes despite unchanged scores. Explicit sums remove pointer
stack traffic and recover target base/offset addressing, but float Y still
lowers as FLD ST(0) followed by FCOMP. Changing Y to double instead loads the
constant, compares ST(1), and branches on parity. It does not recover the target,
despite identical size and aggregate score. Keeping float and reversing the
comparison operands produces the target FCOM / test AH,41 / JNE sequence.

The adopted source preserves the existing field reads, writes, float operand
calls and double sentinel. It changes no ABI, helper, storage layout, memset,
profile or canonical recipe. It retains the spawn-field improvement from the
preceding checkpoint. The source form is a compiler hypothesis, not proof of
original authorship. Collateral size changes are explicit: 0x119/0x11b shrinks
306 to 304 against 313 (worse); the spell group shrinks 227 to 226 against 219
(better). All 181 selector bytes, 108 physical groups, 45 target-sized groups and
the 2C4 frame remain. Full transitions are in [evidence.json](evidence.json).

## Validation and reproduction

Each experiment compiles one primary object serially at nice10 and reuses nine
hash-verified support objects and two harness files. Each passes 36 complete
guards (4618 bytes, 121 fields). ECL remains 7020/6993 with all 189 fields,
62 local destinations and its exact complete 171-byte float callee. ANM remains
9964, 7541/7995 aligned and 90/92 target-sized groups. No full cold pair or
full CI is claimed. The fresh maintained-path primary rebuild produces raw-identical complete
dispatcher/ANM/ECL/factory bodies to the selected private candidate. Its 36
complete guards and both existing 202/219-byte laser constructors pass:
38 complete comparisons, 5039 bytes and 125 fields. These validate the bounded
rebuild, not a new parent exact match.

Private producers, overlays, reports, instruction review and manifests are in
`.analysis/gpt-6-astra-medium/enemy-motion-lifetime-20261022/`.
Patches here are relative to the pinned starting source. Before adoption, the
19 previous canonical native files were archived under
`build/gpt-6-astra-medium/enemy-motion-lifetime-20261022/baseline-before-adoption/`
and 32 source/header inputs under private `baseline-source/`. The baseline
report's original canonical path now denotes the replacement image; use the
archive for its original bytes. The original manifest is historical; the
validation manifest precedes the later notes-only ledger update.

Four rejected carriers were hash-inventoried and removed: 64 files, 4265743
bytes. Keep explicit-sum and selected Y-left carriers, the historical archive,
current canonical image and compact reproduction inputs. Nine unrelated D3D
files are preserved. Exact ledgers and product/runtime gates remain unchanged.
