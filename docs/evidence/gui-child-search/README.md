# GUI child-ID search and restored Run controls (2026-10-20)

`ResultMenuFindScript @ 0x004497D0` closes the existing complete **70-byte**
authored owner. Its boundary, maintained source file and origin are unchanged. The canonical
unit is `frontend-find-script`; no parent or partial-case bytes are credited.

## Target and source

The original Japanese TH10 v1.00a executable has SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Read-only local Ghidra independently attests that input, the full mapped text
hash and unchanged bytes. The existing body is4497D0..449815, with52 direct
incoming calls and no outside interior references in the reviewed database.

The helper takes the ID pointer in ESI, script in EBX and hidden aggregate
output in EDI, returning that output pointer with plain RET. It includes the
root VM, follows the child chain and returns the first matching script ID or
zero. A failed lookup clears the input ID but still forms the VM+10 list
address; the existing valid-VM precondition is retained.

Two ordinary source changes reproduce the target: pass the existing ID object
to FindVm and finish the child search before constructing the returned ID.
The first removes the integer-to-ID temporary; the second lets the compiler
reload node->owner at the success exit. The original code kept the previously
loaded owner. No aggregate is replaced by a scalar; signatures, compiler
profiles, shared methods and other function bodies are unchanged. No volatile,
assembly, forced compiler controls or manual alignment are introduced.

## Complete native replay

Two independent full cold14-TU /GL links select the real Gui HUD updater entry.
All42 source/header hashes, support order and profiles agree. Qualified MAP/PDB
ownership selects support-05-FrontEnd.ltcg.obj and its whole70-byte contribution.
The strict comparator consumes every byte and exhaustively binds both fields:

| Offset | Field | Independently reviewed target |
| --- | --- | --- |
| 04 | DIR32 manager pointer | 00491C10 |
| 0A | REL32 FindVm call | 004491C0 |

Each image passes143 complete bytes/all2 fields: the new70-byte helper and the
existing73-byte relocation-free FindVm in its actual caller graph. The latter
is a guard, with no duplicate credit or change to its canonical recipe.
`native-replay.json` records the two proofs, graph and project input hashes.

```sh
scripts/repo-python scripts/build-match-unit.py --unit frontend-find-script
scripts/repo-python scripts/compare-linked-function.py --unit frontend-find-script --json
```

## Whole Run investigation; all parent controls restored

The current14-TU retained baseline has Run4480/target4480, frameD8,148/150 CALLs
and complete24/7/7/5 tables. Its ordinary diagnostic remains253/3160;
HUD remains3772/3836 and527/3552. Size agreement grants no exactness.
The following controls were cumulative, diagnostic object reuses, not cold
proofs. Forty or41 unchanged project inputs and the reused objects were
hash-checked; the helper source was the sole additional mutable support input.

| Cumulative Run control | Bytes | Frame | CALLs |
| --- | ---: | --- | ---: |
| Target physical outer-case order | 4496 | D8 | 149 |
| Separate character0/1 creation arms | 4528 | DC | 150 |
| Pass actual ID objects | 4504 | DC | 150 |
| Sprite operands in consuming full expressions | 4464 | DC | 150 |
| Natural loop/wait predicates and sound branches | 4464 | D8 | 151 |
| Packed skippable-bit assignment | 4468 | D8 | 151 |

Every whole-Run control remains nonexact and Gui.cpp is restored byte-for-byte.
The extra CALL in the final control is in opcode20: the native copies of
SetNextGameMode(4) remain separate while target shares that call. Parent
receiver/lifetime/optimizer context remains open. The prepared replay-branch
rewrite was not compiled and adds no evidence. Current canonical native
artifacts retain the whole restored Run/HUD failure reproducers; duplicate
experimental images are removed.

The pinned MIT reference c6791d82dc9f02e55eca45a5314f900a85481efd suggests case
ordering, operand placement and positive branch shapes in
matching2/msvc/build_support/contexts/equivalent/game/front_script_tick.cpp and
its recovery note. These are hypotheses. Its forced controls, context escape
helpers and published exactness claims are not adopted as TH10 evidence.

## Validation and limits

The existing actual-source Run suite passes13 selections under ASan/UBSan with
its documented synthetic dependencies. The legacy result-menu fixture now
uses the canonical AnmVmIdView value parameter for its synthetic FindVm; its
existing state/order assertions are unchanged. This fixes its initial compile
failure without modifying production code or repeating native proofs. Full repository CI, toolchain checks,
tracking, progress and whitespace checks pass; validation-summary.json records
the completed scope and cleanup. The pre-existing D3D evidence is untouched.
Original C++ spelling/class ownership, production data/link closure, target
runtime, semantic and port gates remain open. GUI/FrontEnd source-completeness
gates pass independently from this bounded exact result.
