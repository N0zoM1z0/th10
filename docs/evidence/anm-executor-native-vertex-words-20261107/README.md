# ANM ExecuteScript: generated-vertex native count and cursor words

Original Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `AnmRenderManagerView::ExecuteScript` at `0x0043EE30`
has a reviewed **9,964-byte** target extent, including its absolute
94-entry jump table. This owner is **NONEXACT**; the maintained candidate
matches **9,962/9,964** original bytes with all **350 original-bound
address/control fields**. The only remaining code bytes are `+0x113`
(target `0x55`, candidate `0x4D`) and `+0x115` (target `0x52`, candidate
`0x51`), corresponding to the opcode-91 child-layer EDX/ECX choice.

## Installed source recovery

The mode-9 radial generated-vertex path reads `vertexCount` directly
from script-exposed VM state. Retail i386 evaluates the three distinct
`vertexCount - 1` operations (angle divisor, UV divisor, loop bound)
with a native 32-bit decrement. The former signed C++ subtraction had
undefined behavior for `vertexCount == INT_MIN`; all three sites now
subtract in `unsigned int` before casting the same low word back to
signed `int`. This preserves the original float conversions, loop
comparison and the three separate evaluation sites, without adding
validation or clamping.

The same strip loop takes two steps through variable-length generated
vertex storage. Its two former `++vertex` C++ pointer increments were
undefined if an unchecked script count made the vertex walk leave the
allocated object. The maintained source adds `sizeof(*vertex)` to the
cursor's unsigned native address word and casts to the same typed pointer.
No claim is made that invalid script inputs become safe, nor that this
syntax is the historical game source.

## Target-bound bounded experiments

A **fresh current-source five-TU** VC7.1 SP1 `/GL /GS` baseline was
compiled, using a single `/Ob1` override for `AnmInt3Scale.cpp`.
Separate attempts examined six child-91 record type/layout alternatives:
five preserved only the preexisting 9,962 target bytes and one regressed
to 9,954. They were rejected; forcing a specific register is not a
sound source-reconstruction method.

Four strip-pointer variants (first, second, both explicit, both concise)
all preserved **9,962/9,964** target bytes and 350 target-bound fields.
On the retained two-pointer form, independent trials of each of the
three unsigned count decrements and of all three together also preserved
**9,962/9,964** with 350 fields. A named single count local failed the
original binding/geometry contract and was rejected; it may combine
common subexpressions differently than the target's independently
observed sites.

The **final installed, naturally formatted C++** was compiled again
independently, using the four source/hash-pinned support objects. It
retains the exact same 9,962/9,964, 350/350 original-bound result, with
no other code difference. Separately selected canonical cold replays of
three preaccepted ANM helpers (`anm-vm-timer-add`,
`anm-vm-timer-decrement`, `anm-int3-scale`) pass **164/164** target bytes
across two scoped artifacts. The seven lightweight CI checks pass.
This supplies no new exact owner, full binary link or runtime proof.

Reproduction using the installed source and its ignored artifacts:

```sh
scripts/repo-python .analysis/gpt-web/20261107-active/anm-installed-compare.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-timer-decrement --unit anm-vm-timer-add \
  --unit anm-int3-scale --json
scripts/repo-python scripts/ci.py
```

Detailed test receipts and complete native PE/MAP/PDB are in ignored
`.analysis/gpt-web/20261107-active/` and
`build/gpt-web/20261107-active-anm-graph/`.
