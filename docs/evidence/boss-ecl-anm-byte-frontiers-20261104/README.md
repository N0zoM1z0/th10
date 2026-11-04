# TH10 complete ECL and ANM dispatcher byte frontiers, 2026-11-04

All experiments read the private, SHA-256-attested Japanese TH10 1.00a target
(`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`),
used pinned VC7.1 SP1 source compilation rather than copied target instructions,
and returned the maintained source to its original bytes in `finally`.
**Neither large owner is exact; there is no new canonical credit here.**
These are current source-epoch observations, not inherited assumptions.

## ECL `EclVmContext::Run` @0x0044E1A0: 7,020 bytes

The true `src/EclVm.cpp` + `src/Enemy.cpp` `/GL` selected source graph retains
a complete 7,020-byte PDB owner, 189 original layout/field obligations,
and the historically verified **6,997/7,020** ordinary matching bytes.
The 23 remaining, *normalization-free ordinary-byte* discrepancies were
redecoded from **both** the original and the actual existing linked candidate
using Capstone. They cluster into distinct instruction families:

| Offset(s) | Original | Candidate | Discriminator |
| --- | --- | --- | --- |
| `+0xAF`, `+0x1FB`, `+0x151D` | EDI/EAX base-index forms | EAX/EDI base-index forms | Three byte-equivalent x86 addressing encodings |
| `+0xEC`, `+0xF1` | EDX `mov` / `push` in `SPAWN_THREAD_WITH_ID` | EDI pair | Temporary register allocation, not a different thread ID |
| `+0x12A`, `+0x12C` | EAX in stopped-thread dereference/store | EDX | Existing context pointer register choice |
| `+0x1838`, `+0x183C`, `+0x1840`, `+0x1844` | x87 first squares scalar at `[esp+0x14]`, then `[esp+0x18]` | opposite square order | `LENGTH_SQUARED` expression scheduling; same two float inputs |
| `+0x1899`, `+0x18B6` | first point-angle subtraction stack home `+0x14` | `+0x18` | `POINT_ANGLE` temporary slot placement |
| `+0x18E6`, `+0x18F8`, `+0x191C`, `+0x1932`, `+0x1957`, `+0x199E`, `+0x19A8`, `+0x19BA` | FORMAT scratch pointer `+0x14`, argument offset `+0x18` | reversed | Two distinct live stack slots, not external relocations |
| `+0x19E6`, `+0x19E8` | EDX reload/dereference of advanced instruction | EAX | Loop-tail register choice |

This partition accounts for **all 23 bytes**, retaining the original 60-entry
jump table and selector bytes as part of the full owner. It does not claim
that arbitrary target-vs-candidate field locations can be masked: the known
baseline's independent original field bindings are documented separately.

**New actual compiler trials** changed only one natural expression/lifetime
at a time. Results below are preliminary ordinary-byte comparisons after
checking all **189 field offsets and types** against the pre-existing
original-target contract; they do **not** independently rebind all external
callee identities and therefore cannot be exact admissions:

| Source variant | PDB size | Field geometry | Ordinary match | Decision |
| --- | ---: | --- | ---: | --- |
| `y*y + x*x` in `LENGTH_SQUARED` | 7,020 | 189 unchanged | 6,997 | Compiler folds commutative ordering; reject |
| Scoped context pointer in `SET_THREAD_FLAG` | 7,020 | 189 unchanged | 6,997 | Byte-neutral; reject |
| Split script operand and thread ID into named locals | 7,020 | displaced call field at `+0xF5` versus `+0xF7` | not valid | Wrong field geometry; reject |
| Scoped context pointer in `STOP_THREAD` | 7,020 | 189 unchanged | 6,997 | Byte-neutral; reject |
| Move FORMAT argument index into the loop declaration | 7,020 | 189 unchanged | 6,997 | Byte-neutral; reject |

A fresh disassembly of the previous scratch-first FORMAT allocation variant
also confirmed it only moves the **`malloc` REL32 field from `+0x18DA` to
`+0x18D7`**, without changing the reversed scratch/index stack homes.
It is not a valid original-field match. All tested sources were restored.

Reproducible *ignored* raw files and focused drivers:
`.analysis/gpt-web/20261104/ecl-inspect.py`, `ecl-inspect.txt`,
`ecl-compare-variants.py`, `ecl-register-controls.py`,
`ecl-scope-controls.py`, `ecl-scratch-first-diagnostics.py`, and their JSON
or text outputs. See also [prior complete owner proof](../ecl-length-squared-stack-20261102/README.md).

## ANM `ExecuteScript` @0x0043EE30: 9,964 bytes

The current five-source `/GL /GS` graph uses `AnmManager.cpp`,
`RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`, and
`AnmInt3Scale.cpp` with that last support compiled `/Ob1`.
A real fresh November 4 baseline produces **9,962/9,964** and passes the
**entire strict 350-field binding contract**, including **95 fixed local
and data destinations**. The two residual byte offsets are **only**:

- `+0x113`: native `MOV EDX,[EBP+0x20]` (`0x55` ModRM), candidate
  `MOV ECX,[EBP+0x20]` (`0x4D` ModRM).
- `+0x115`: native `PUSH EDX` (`0x52`), candidate `PUSH ECX` (`0x51`).

These are the child opcode 91 layer argument path, not corruption of the
73-byte already-exact `AnmLoadedView::CreateVmVariant2` leaf. An additional
**fresh complete** VC7.1 compile rearranged the temporary
`AnmChildScriptLayer` field layout and assigned `script = GET_INT_VAR(0)`
*before* reading the VM layer, retaining the original observable read order.
The linked PDB remains **9,964 bytes, 9,962 ordinary matching bytes**, and
all **350/350 strict fields and 95/95 local obligations** pass. Compiler
register choice remains ECX, not the target EDX; the variant is rejected,
and the source is restored. Reordering a private temporary record alone is
not a supported exact solution.

This cold graph and strict comparison are reproduced by
`.analysis/gpt-web/20261104/anm-op91-reversed-record.py` (ignored), with
result JSON and linked PDB under `build/gpt-web/20261104-anm-op91-reversed-record/`.
The strict baseline graph and source epoch are recorded in
`.analysis/gpt-web/20261104/anm/baseline-result.json`.
Existing [ANM owner proof and prior source controls](../anm-executor-two-byte-frontier-20261031/README.md)
remain authoritative only for the exact source and compiler context they
actually tested.

No full all-unit cold replay was performed. Neither large owner is linked
as a complete native game nor accepted as exact codegen, authored provenance,
runtime behavior or portability.
