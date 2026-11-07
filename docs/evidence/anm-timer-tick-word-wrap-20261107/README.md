# ANM ExecuteScript frame-timer increment: native 32-bit wrapping word

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original complete `AnmRenderManagerView::ExecuteScript @0x0043EE30` is
**9,964 bytes**, with 350 independently bound fields and 95 local target
obligations. This is a source-level semantic recovery **not** a new codegen
exactness claim: the full parent still has the two opcode-91 byte differences.

The end of the executor calls `TickScriptTimerScale(&vm->scriptTimer)`.
The fast unit-speed branch previously used `++timer->current` on a signed
`int` field. Native x86 increments the complete low 32-bit word even at
`INT_MAX`, where an ordinary signed C++ increment has undefined behavior.
The installed natural C++ now increments the actual field through its
corresponding `unsigned int` word view:

```cpp
++*reinterpret_cast<unsigned int *>(&timer->current);
```

The original timer's previous/current store order, subsequent float tick,
unit-speed predicate, existing nonunit-speed fallback and `EclVm` globals
are unchanged. No new counter, bounds check, padding, assembly or ABI
forcing is introduced. The actual i386 semantics are established in the
pinned compiler context, not as a whole-system portability assertion.

## Complete original-backed 9,964-byte parent

Four source controls were compiled under the actual VC7.1 SP1 `/GL /GS`
five-TU ANM executor graph, with reviewed `/Ob1` support for
`AnmInt3Scale.cpp`: unsigned JUMP_DEC alone, `-=1u` syntax, unsigned timer
increment alone, and the *combined* JUMP_DEC and timer change now installed.
All complete PDB owners retain **9,962/9,964 original bytes**, exactly
**350/350 original-linked address fields** and their 95 local destinations.
Only child-script opcode 91 at `+0x113`/`+0x115` (EDX original vs ECX
candidate) remains nonidentical. The `both-jump-and-timer` actual source
SHA-256 is `339c95eba4063aab4a62da3d496cb07de2c6240197f6957761392579f93b435f`.
This preserves the nonexact large owner rather than taking partial credit.

The final installed source was then independently selected for a new,
**original-target** two-artifact cold replay. All six previously credited
ANM helper functions are exact:

| Owner | Exact original bytes |
| --- | ---: |
| `anm-vm-timer-add` | 88/88 |
| `anm-vm-timer-decrement` | 20/20 |
| `anm-get-float-var` | 424/424 |
| `anm-get-int-var` | 144/144 |
| `anm-get-float-var-ptr` | 156/156 |
| `anm-get-int-var-ptr` | 132/132 |

**964/964** strict original bytes, all declared fields. Only the selected
owners were cold replayed, **not** the entire historical cohort:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-timer-add --unit anm-vm-timer-decrement \
  --unit anm-get-float-var --unit anm-get-int-var \
  --unit anm-get-float-var-ptr --unit anm-get-int-var-ptr --json
```

Full parent comparisons and native input/image SHA-pinned trial artifacts:
`.analysis/gpt-web/20261107-anm-timer-jump-word/`,
`build/gpt-web/20261107-anm-timer-jump-word/`. Actual final selected cold
receipt: `.analysis/gpt-web/20261107-anm-timer-jump-word/both-selected.json`.
No new exact function, production EXE, runtime behavior or original compiler
source partition is claimed.
