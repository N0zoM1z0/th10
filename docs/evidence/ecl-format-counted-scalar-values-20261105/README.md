# TH10 ECL Run FORMAT: typed counted inline text and 32-bit scalar read path

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ECL `FORMAT` opcode carries an inline-byte count at the first
32-bit operand word, then counted text, then encoded eight-byte argument
records. Those records have a tag at +0 and a four-byte scalar payload
at +4, with three unclassified bytes between. The same source's generic
stack and other ECL operand readers already model a 4-byte int/float
bit carrier in `EclVmScalar`; `FORMAT` was still using raw `int`/`float`
arrays to fetch the actual argument value.

The maintained source now overlays the complete eight-byte format record
with the shared `EclVmScalar` (asserted at +4 with width 4), reads the
count through the existing typed `EclVmFormatInlinePrefixView`, and
uses `scalarOperands[index].real` / `.integer` for the two value forms.
Critically, the original `inlineBytes / sizeof(int) + 2 + index * 2`
indexing, including integer division by four, is unchanged. A direct
`argument->scalar` replacement would incorrectly assume every inline
text length is four-byte aligned, so this change deliberately preserves
the actual byte-index conversion. It does not add alignment, bounds,
allocation, target instruction bytes, or guessed original C++ symbols.

## Complete original-bound and dependent exact source proof

Fresh pinned VC7.1 SP1 two-source `/GL` native build of `EclVm.cpp` and
`Enemy.cpp` retains the *entire* original-bound **7,020-byte**
`EclVmContext::Run`, **189/189** target address/local branch fields and
**6,997/7,020** matching original ordinary bytes. The existing 23
ordinary differences remain; **NONEXACT** and no new ledger credit.
`AddScriptData` remains a separate nonexact 585-byte PDB owner (original
589); `FindSubroutine` remains 136 bytes. The 551-byte `StartSubroutine`
continues to preserve all four call field positions at
`0x9E/0xD2/0xE8/0x1DE` and all 551 normalized candidate bytes versus the
preceding pinned graph, without being misclassified as target-exact.

Two directly related existing accepted value readers were independently
cold replayed with explicit unit selectors:
`ecl-vm-read-int-value` **124/124** and `ecl-vm-read-float-value`
**171/171**, total **295/295** original-target exact. Seven lightweight
CI checks and `git diff --check` pass; no full historical cold replay.
No full-game product, ABI, runtime or portability claim is made.

Private reproduction:
`.analysis/gpt-web/20261105-ecl-format-scalar/verify.py`,
`build/gpt-web/20261105-ecl-format-scalar/current/native-result.json`,
`.analysis/gpt-web/20261105-ecl-format-scalar/selected.json`.

Source SHA-256: `EclVm.cpp` `afbc6c03fedbedb4102765ce528f54a1c7100a2404b6a3e4df89d38b9a3b05a7`, `EclVm.hpp` `669fcc7f18c7be8ef83f47bde5cb2e7f09bbd88578e9d73a98f18acbb5e9c06f`.
