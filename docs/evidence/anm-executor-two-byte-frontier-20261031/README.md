# ExecuteScript: complete two-byte frontier — 2026-10-31

`AnmRenderManagerView::ExecuteScript @ 0x0043EE30..0x0044151B`
(9,964 authored bytes) is **still NONEXACT**, but the maintained source
now reproduces **9,962/9,964** complete target bytes after strict binding of
**all 350** original address/control fields, including **95** fixed local or
table destinations. Previously the complete source candidate passed
9,960/9,964 in this same 5-input compiler graph. This checkpoint does not
add a match unit, exact credit, or a partial owner; all four-byte comparisons
are of the complete PDB section contribution.

## Two observed native arithmetic schedules

The first discrepancy was in the second generated radial vertex:
target `+0x2498` loads the owner's Z at `VM+0x33C` before adding the offset Z
at `VM+0x348`, while the old source reversed those memory operands. Capturing
the actual position Z through a scoped `const volatile float&` and assigning
it to the existing three-component translation before the existing vector
addition restores the two target operand bytes at `+0x249A/+0x24A0`.
The X and Y vector operations, both generated vertex results, all resource
bindings and the adjacent angle-normalization call remain present. The original
qualifier/source spelling is unknown; the volatile reference is a codegen
model of the observed read, not a threading assertion.

That change alone leaves an incorrect x87 load/add order in the script timer
at `+0x2547..+0x254C`. `AnmVmTimerView::Tick` has other existing callers,
so changing its header inline body globally is too broad. The controlled
header-level volatile variant repairs ExecuteScript's tail but changes the
existing 887-byte Float3 and 652-byte Float2 interpolation codegen. It is
**not retained**. Instead `TickScriptTimerScale` is an ordinary fully inlined
local helper used by ExecuteScript only. It implements the observed current,
previous, subframe and scale checks and retains the scale-first x87 load
and the actual truncation call. This restores all four timer instruction bytes
at `+0x2548..+0x254B` without changing the shared header or the other two
interpolation functions. No synthesized return, dummy operand, data mutation,
register-forcing declaration, binary patch or linker/profile change is used.

## Strict complete comparison

In the current five-input `/GL /GS` context, with `AnmInt3Scale.cpp` retaining
its existing `/Ob1` override, the only remaining ordinary differences are:

| Offset | Target | Native | Meaning |
| --- | --- | --- | --- |
| `+0x113` | `0x55` | `0x4D` | Child opcode 91 loads `renderLayer` into EDX versus ECX |
| `+0x115` | `0x52` | `0x51` | The corresponding push uses EDX versus ECX |

The other 9,962 bytes and every bound field pass the unchanged full comparator.
Two independent images reproduce the result: a hash-checked, one-primary-input
warm recompile using four unchanged validated support objects, and a fresh
**five-input scoped cold link** (not the historical full cold cohort).
The helper at `docs/evidence/anm-child-request-record/binding-contract.json`
provides the independently reviewed full 350-field contract; this checkpoint's
`verify-native.py` enforces all of it and **expects nonexact rejection**.
The target is original Japanese 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

```sh
scripts/repo-python docs/evidence/anm-executor-two-byte-frontier-20261031/verify-native.py \
  --artifact-dir build/gpt-web/anm-executor-independent-20261031
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-timer-decrement --unit anm-vm-timer-add --json
scripts/repo-python scripts/ci.py
```

The affected existing Add 88 and Decrement 20 owners cold-pass **108/108**
through their selected real producer. In the same diagnostic ANM image, the
820-byte color interpolation, 887-byte Float3 interpolation, 652-byte Float2
interpolation, 370-byte alpha interpolation and 56-byte integer-scale owner
retain structural equality, as do Add and Decrement. Only established canonical
units receive exact credit; structural checks are not new admissions.

## Controls and next frontier

Before the arithmetic fix, ordinary child argument const qualifiers and
read/write barriers before or after the argument record were byte-neutral;
splitting script/layer locals changed more instructions. Natural reversed
vector addition restores Z but regresses X/Y; manual component assembly and
local vector overwrites also regress or are neutral. These observations are
retained under `.analysis/gpt-web/anm/` with complete candidate inventories.
Do **not** assume the old child controls remain neutral at this new source
epoch: the final two bytes still merit fresh, bounded compiler experiments.
No full product-build, runtime-equivalence, original TU/ABI-source ancestry or
portability claim follows from 9,962/9,964.

## 2026-11-03 independent follow-up on the full 9,964-byte owner

This checkpoint **reran real VC7.1 `/GL /GS` links**, not a textual
re-evaluation, for opcode 91 under the current maintained source epoch.
Two independent groups tested the following source lifetime variants:
`const AnmChildScriptLayer` record, dedicated `AnmLoadedView * const`
file receiver, two-element argument array, pointer-indirected record and
read-order forms around the 91 child branch. Every compiling variant kept
**9,964** complete PDB bytes; the best repeated **9,962/9,964** with all
**350** independently bound fields, and the same native EDX versus linked
ECX at +0x113/+0x115. The pointer-indirected record regressed two more
ordinary bytes to 9,960/9,964. A `const AnmVmIdView` return attempt did
**not compile** because the real `GetVm()` member requires mutable `this`;
this failure is not counted as a codegen experiment. All actual candidate
sources were restored byte-for-byte in `finally`, preserving the exact
child factory and the already proven 9,962-byte frontier.

Fully bounded results and replay recipes:
`.analysis/gpt-web/anm/child91-20261103-new-forms-results.json`,
`.analysis/gpt-web/anm/gptweb-20261103-child91-fresh-results.json`, and
`.analysis/gpt-web/anm/gptweb-20261103-boss-followup-results.json`.
No exact owner was admitted, no field omitted, no full cold replay invoked.
The remaining two target bytes require evidence about source-level
register allocation across `GET_INT_VAR`/`CreateVmVariant2`; code length
alone is no evidence.
