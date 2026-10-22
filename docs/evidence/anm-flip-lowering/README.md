# ANM flip operation and flag-lifetime controls

Starting2c4f17c. Four actual private ANM-source controls test the remaining
FlipX and operand-group register choices in the complete ExecuteScript owner.
No source, header, profile, recipe or exact row is adopted.

Raw target inspection separates two independent operations: XOR changes the
flags35C direction bits; scaleX/scaleY are updated with x87 FMUL through470B60,
whose four bytes000080BF encode float-1.0. XOR is not a float-sign toggle.
The initial unary-minus experiment therefore tests a different compiler
operation: it emits FCHS, and is rejected. It does not justify substituting
unary negation for the observed multiplication or asserting their full floating
point equivalence. FlipX's flags use target EAX/five-byte XOR versus baseline
EDX/six-byte XOR; FlipY already uses the accumulator form.

| Private model | ANM size | Aligned matching positions | Same-size groups | Absolute group gap |
| --- | ---: | --- | ---: | ---: |
| Current baseline |9964|7541/7995|90|2 |
| Unary negation for both scales |9948|7307/8004|88|20 |
| Raw OR4 instead of rotation bitfield assignment in49/53 |9964|7541/7995|90|2 |
| FlipX scale multiplication/store before flag update |9964|7533/7995|90|2 |
| FlipX multiplication result retained until after flag update |9964|7541/7995|90|2 |

Unary FlipX/FlipY spans become49/48 versus target52/52. Scale-first leaves
FlipX53 and loses eight aligned positions there. The raw rotation flags and
pending-result models are complete raw-byte identical to the baseline across
ANM9964, ECL7020 and Enemy14352, independently hashed rather than inferred from
scores. The scale-first ECL/Enemy bodies are also raw-identical; ANM is not.
Case scores remain navigation diagnostics with acceptance authority none.

Each actual run recompiles only ANM with GL/GS and reuses nine pinned source
objects plus two pinned harness files. All four pass35 complete guards4424/
all118 and eight bounded child-routing checks. Each also passes the separately
checked full194-byte/all3 direct caller, for36 whole guards4618/all121 per
image. Five original normal-COFF contracts remain additional linked guards,
without changing canonical artifact kind or credit. ECL retains complete7020/
6993,189 fields/62 locals and the complete171-byte float callee. Its final
selector-byte annotation remains; generic normalization false is not waived.

Fresh disk target/toolchain execute/tracking and direct local Ghidra pass.
Ghidra uses readOnly/noanalysis, rollback, CPU1/heap768 MiB and verifies all412289
mapped text bytes unchanged. Maintained108 source files,32 large-context inputs,
canonical baseline artifacts/recipes and9 unrelated D3D files remain hash-equal.
Focused checks pass; no full CI per operator scope. The first extra-caller
manifest used an absolute PDB-source path while supports were relative; it was
rejected before comparison, corrected, and all four actual comparisons pass.
This preparation error supplies no target or compiler evidence.

[Evidence, complete input hashes and scope](evidence.json) and the four selected
source-span patches retain the experiments. Hash-checked cleanup removes48
superseded files/3197978 logical bytes. Retain the9948 unary failure, canonical
baseline, all compact sources/results/producers and the target instruction
review. Private recovery:
`.analysis/gpt-6-astra-medium/anm-sign-lowering-20261022/RECOVERY.md`.
Next investigate the operand service's real caller-dependent register context;
do not repeat these four forms or remove the target FMUL. Large owners remain
nonexact, with no small-function closure substituted. Product/runtime and
semantic/port gates remain open.
