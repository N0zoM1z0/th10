# TH10 TransitionShakeEnvelope: recover original pause gate and x87 amplitude lifetime

Target: verified original Japanese TH10 1.00a
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original complete `TransitionShakeEnvelope @0x0043C710..0x0043C86C` is **349 bytes**.
The maintained source belongs to the actual `TransitionEffect.cpp` support TU
in the 14-TU `GuiTransitionEffectFactory` `/GL` graph. This checkpoint is
**NONEXACT**; no new canonical functions/bytes are credited.

## Substantive source restoration

The older original 297-byte native candidate compacted the pause-bit check
into `TEST BYTE PTR [EAX+58],77`, unlike the target, which loads the whole
32-bit flag word, shifts right two, ORs the shifted and unshifted values,
tests bit zero, then independently tests the remaining `0x72` bits.
The installed source now writes those two checks explicitly using unsigned
word operations and the same null short circuit. This preserves the
same relevant bits (`0x77`) while restoring the target's intermediate
`SHR` / `OR` / `TEST` sequence.

The original later calculates the final signed/unsigned envelope in x87,
then executes `FILD [effect+1c]`, `FMUL ST(1)`, `FSTP [esp+8]`, and
`FSTP ST(0)`, leaving the resulting single-precision amplitude in a
stack home for two successive RNG-driven X/Y displacement switches.
The old source held the float on the x87 register stack instead. The
installed `const volatile float amplitude` is a *compiler lifetime model*
that reproduces the memory materialization and use. It is **not** a claim
of original volatile source, concurrency, or a hardware synchronization
requirement.

The changed real 14-TU VC7.1 SP1 `/GL` graph now produces a **352-byte**
complete static MAP/PDB owner, from the old 297, against target 349.
The 16 native linked fields are captured from the PDB-owned function
extent, but there are still ordinary instruction/register-order mismatches
and a **three-byte extent gap**; equal semantic behavior or field count
does not establish exactness. In particular the current third-stage
limit emits a preserved EBX pair while target uses ESI. Trial controls
with a named second-stage limit, a volatile view of the third field, and
inverted end branches produced 350, 350, and 348-byte shapes respectively,
but did **not** have complete original-bound equality. All nonselected
trial variants were restored from the actual source.

## Affected original-bound cold guard

Only selected affected existing canonical exact owners were replayed from
the **installed current source** in the pinned 14-TU graph (one bounded
cold compilation, not the full historical cohort). Initializer's one
static callback-address contract at `+0xF5` now specifies actual candidate
size `0x160` for the 352-byte envelope; its independent *target* size
remains `0x15D` = 349. The full current graph passes:

| Existing accepted owner | Target bytes | Verified native fields |
| --- | ---: | ---: |
| `TransitionDrawScreen` | **134/134** | 9 |
| `TransitionEffectView::Initialize` | **396/396** | 37 |
| `TransitionFadeIn` | **133/133** | 5 |
| `TransitionDelete` | **26/26** | 2 |

Total existing exact guard: **689/689** in a fresh image. These units
remain exact; **zero new exact credit** is claimed for this work.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit transition-effect-initialize --unit transition-fade-in \
  --unit transition-delete --unit transition-draw-screen --json
scripts/repo-python scripts/ci.py
```

Current-source trial evidence is in ignored
`.analysis/gpt-web/20261107-shake-envelope/results.json`, `trial.py`,
`current-guard.json` plus distinct linked PE/MAP/PDB under
`build/gpt-web/20261107-shake-envelope/` and the current accepted guard at
`build/match-linked/GuiTransitionEffectFactory/`. No partial byte-credit,
source-provenance, product/runtime, or portable-FP assertion is made.
