# ECL Run: native counted FORMAT inline-text origin

Original target: Japanese TH10 v1.00a
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The 7,020-byte `EclVmContext::Run @0x0044E1A0` uses a variable-length
FORMAT instruction. Its inline text starts four bytes after the counted
prefix in the instruction's operand stream; the `text[1]` declaration is
only a wire-layout overlay, not a bounded C++ string allocation. Maintained
code now computes this address as an unsigned 32-bit word before converting
to `const char *`, preserving native i386 wraparound and the original lack
of bounds checks. This does not make malformed scripts safe.

## Complete original-bound trials

Seven distinct ordinary C++ source forms were compiled and linked in the
real EclVm+Enemy two-TU `/GL` context. The direct pointer-add, constant
`sizeof(int)` addition, whole-instruction origin, and separate typed-base
expressions produced **6,996/7,020** original bytes with all **189** target
fields. The compiler moved two old differences but introduced three new
ones; these forms were rejected. Computing the final address first in a
native unsigned-word local and then casting to the text pointer preserved
**6,997/7,020**, **189/189** independently bound target fields, the full
60-entry dispatch table and 88-byte selector suffix, and the separately
exact ReadFloatValue callee. It was selected and rebuilt again from final
source. The original typed prefix expression also preserved 6,997/7,020,
but retained C++ flexible-array pointer semantics.

The complete Run remains **NONEXACT** with 23 ordinary byte differences;
no new exact codegen or canonical acceptance is claimed. After installation,
scoped canonical cold replay retained `ecl-vm-host-run` **146/146**,
`ecl-vm-spawn-thread` **142/142**, and `ecl-vm-read-float-value` **171/171**.
`scripts/ci.py` and `git diff --check` also passed.

Private source-specific build/compare evidence is under
`.analysis/gpt-web/20261108-current/`, notably
`ecl-format-prefix-trials.py`, `ecl-format-word-cold.json`, and
`ecl-format-word-complete.json`.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit ecl-vm-read-float-value --json
scripts/repo-python scripts/diagnose-ecl-run-native.py \
  --unit ecl-vm-host-run --artifact-dir build/match-linked/EclVmHostRunEnemy
```

The latter diagnostic exits 1 for this still-nonexact large function.
