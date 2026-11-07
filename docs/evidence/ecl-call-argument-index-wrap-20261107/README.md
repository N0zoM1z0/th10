# TH10 ECL CALL: native argument-counter increment

Target identity: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The 550-byte original `EclVmContext::StartSubroutine @0x0044DF70`
reads a count of tagged, eight-byte CALL argument records. Its
`argumentIndex` is initially `firstArgument + 1` and advances once per
record. The source previously used signed `++argumentIndex`, which would
be undefined at `INT_MAX`; the original i386 native increment instead
advances the full 32-bit counter word modulo 2^32. The installed source
publishes an unsigned word addition back to the signed local, without
changing the signed operand-count comparison, type conversions, data
addresses, argument loop order, or VM stack semantics. This source still
uses the native original unchecked CALL format and adds no safety clamp.

## Entire-function native experiments

Three separate natural VC7.1 SP1 `/GL` two-TU ECL/Enemy variant builds
were linked from the current source: unsigned-word assignment to the
existing signed variable, unsigned lvalue increment, and a fully unsigned
counter declaration. The first two emitted the **same complete 551-byte
StartSubroutine candidate and four candidate call field positions**;
the unsigned variable changed two candidate ordinary bytes and was
rejected. The original StartSubroutine is **550 bytes**, so these are
candidate-only comparisons; neither name nor codegen receives exact
credit. Each experiment separately checked the complete original-bound
`EclVmContext::Run` (7,020 bytes): **6,997/7,020 original ordinary bytes,
all 189 original field bindings** and the exact ReadFloatValue callee.

After installing the existing-signed-local, unsigned-add form, a fresh
selected original-bound canonical cold replay passed eight previously
accepted units totaling **1,081/1,081 bytes** in two build graphs:
`ecl-vm-read-int` 144, `ecl-vm-read-int-value` 124,
`ecl-vm-read-float-value` 171, `ecl-vm-find-subroutine` 136,
`ecl-vm-stack-push` 96, `ecl-vm-stack-pop` 122,
`ecl-vm-host-run` 146 and `ecl-vm-spawn-thread` 142.

Reproduce only the selected affected units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-float-value \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-read-int --unit ecl-vm-read-int-value --json
scripts/repo-python scripts/ci.py
git diff --check
```

Private trial source hashes/PE/MAP/PDB and selected exact receipt are
in `.analysis/gpt-web/20261107-active/{ecl-call-counter-results.json,
ecl-call-index-exact-replay.json}` and
`build/gpt-web/20261107-active-ecl-call-counter/`. This does **not**
claim a complete game build, runtime semantic closure or any new exact
function.
