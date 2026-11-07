# TransitionShakeEnvelope: shared unsigned second-stage boundary restores original field schedule

Target: Japanese TH10 v1.00a (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
Original `TransitionShakeEnvelope @0x0043C710` owns **349 bytes**.
The complete current source-generated static PDB owner is now **350 bytes**.
It is still **NONEXACT** and contributes **zero new exact credit**.

## Maintained source change

Following the earlier source recovery of the original SHR/OR/TEST pause gate
and memory-resident x87 amplitude, the callback's second and third time
conditions shared the unsigned expression `value0 + value1`, but the
compiler previously recomputed its members with an extra temporary
register. The current natural C++ writes:

```cpp
const unsigned int secondBound = effect->value0 + effect->value1;
// Stage 2:
if (effect->timer.current < static_cast<int>(secondBound))
    // ...
// Stage 3:
if (effect->timer.current <
    static_cast<int>(secondBound + effect->value2))
    // ...
```

The actual installed source retains the original `if/else if/else` and
calculation order, with the named value after the timer tick and before the
first phase test. `value0` and `value1` are already 32-bit unsigned fields;
the shared expression has native wrap semantics. The source does not add a
range check, an indirect stub, an ABI/register override, or any new data
allocation. Original declaration spelling is not proved.

A new compiler probe from the *current* 352-byte source epoch serially
compiled the real `TransitionEffect.cpp` input under pinned VC7.1 SP1
`/GL`, and linked unchanged support objects from the exact graph. The
changed source yields a complete **350-byte** callback and all **16**
code-address fields. Independently reviewed original field positions are:

`+02,+17,+43,+6A,+94,+A9,+B8,+C5,+DC,+E8,+F0,+FE,+103,+120,+12D,+13B`.

**Correction after independent original-byte decode:** the previous source
note listed `+16` as the original game-manager global. The actual target
instruction at original offset `+15` is `8B 0D 10 78 47 00`,
`MOV ECX,[0x00477810]` with its DIR32 field at **`+17`**. The compiled
candidate is `A1 40 06 43 00`, `MOV EAX,[g_ResultGameManager]`,
with its field at **`+16`**. Moreover, original Y positive publication
is the one-byte direct-accumulator opcode `A3`, field at **`+12D`**,
whereas the candidate uses the two-byte `89 15` store through EDX, field
at **`+12E`**. Its following Y zero-store field is original **`+13B`**,
candidate **`+13C`**. Thus **13/16**, not 14/16, positions align.
The other thirteen field identities and locations are unchanged. Each
field still resolves to the corresponding intended source/global/CRT/RNG
identity, but this is NONEXACT, not a masked partial acceptance. The
remaining mismatches include register selection and code scheduling;
field-position alignment alone does not prove machine-code exactness.

Other compiled natural spellings using a `const` game-manager pointer,
read-only member reference, X/Y byte-wise `memcpy`, bit word access,
explicit nested second-limit scope, and an alternative float numerator
were rejected or code-neutral. Five further current-source variations
placed the shared secondBound assignment inside the middle-phase test,
snapshotted the game pointer, or used a volatile game-manager pointee.
The four non-volatile cases kept the full **350-byte/16-field** candidate
unchanged in size and field positions; the volatile-pointee case emitted
**352 bytes**. None removes the three remaining field-location mismatches,
and all temporary source edits were restored. The actual trial reports
are recorded in the same `results.json` and PE/PDB directories. One isolated Wine link produced a page
fault and is recorded as failure, not recast as a successful native build.
All unselected source trials were restored; the 350-byte form is installed.

## Full current-source scoped exact guard

The `transition-effect-initialize` original-target address contract at
`+0xF5` now describes the actual **350-byte** static callee contribution
(`static_address_candidate_size = 0x15E`); its **original target** owner
remains 349 bytes (`0x15D`). A fresh pinned 14-TU selected cold replay of
the installed source passed all four existing, fully original-bound
owners, totaling **689/689** bytes:

| Unit | Strict original-bound outcome |
| --- | --- |
| `transition-draw-screen` | 134/134, 9 fields |
| `transition-effect-initialize` | 396/396, 37 fields |
| `transition-fade-in` | 133/133, 5 fields |
| `transition-delete` | 26/26, 2 fields |

No full historical cold replay was invoked. This is source/codegen
reconstruction progress, **not** whole-function acceptance for the
350-byte callback or a product/runtime guarantee.

Ignored private evidence:
`.analysis/gpt-web/20261107-shake-envelope-current/trial.py`,
`results.json`, `installed-guards.json`; native image
`build/match-linked/GuiTransitionEffectFactory/` and per-variant linked
PE/MAP/PDB under `build/gpt-web/20261107-shake-envelope-current/`.
