# TH10 ECL Run FORMAT prefix and signed argument-stride ownership

Verified target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This change affects the **entire** `EclVmContext::Run` instruction interpreter
at `0x0044E1A0`, original extent **7,020 bytes**.

The original FORMAT opcode carries an inline byte-count word followed by
literal text. A new `EclVmFormatInlinePrefixView` binds that byte count at
`+0` and the text origin at `+4` through a compile-time `offsetof` check.
This is a variable-length **serialized prefix**, not a fixed-sized
allocation or a source-name reconstruction claim. The following argument
records have independently established eight-byte extent (`type` byte and
32-bit scalar at `+4`); the source now derives argument/tag stepping from
`sizeof(EclVmFormatArgumentView)` rather than unrelated `8`/`2` literals.

**Important signed-index finding.** C++ `sizeof` has an unsigned type.
Using it directly in `inlineBytes + argumentIndex * sizeof(record)` changes
arithmetic conversions for the original signed operand/index, even though
the size is the same integer eight. We actually built two variants that
collapsed the complete owner to **7,008 bytes** and altered linked-field
positions. Those were rejected. Explicitly converting `sizeof`-derived
values to `int` preserves the original signed index calculation, including
unusual negative byte-count/index cases. A separate build first recovered
7,020 bytes without the typed text cursor, then a final build showed that
**both** record overlay and signed sizes coexist without codegen regression.

The installed source was compiled by pinned VC7.1 SP1 `/GL` in its real
`EclVm.cpp` + `Enemy.cpp` link graph. The full PDB contribution stays
**7,020 bytes**, all **189** linked field positions and types match the
independent target-bound contract, and exactly the previous **23** ordinary
target byte differences remain. The boss is **NONEXACT**; there is no new
exact credit, and a same-sized body is not treated as proof of source identity.
Selected original-target cold replays of `ecl-vm-host-run`,
`ecl-vm-read-int`, and `ecl-vm-read-float` remain **473/473 exact** across
all three pre-existing units. Seven-stage CI and whitespace check passed.
No whole-historical-cohort replay, native product closure or runtime proof.

Local native images under ignored
`build/gpt-web/20261105-continuation-ecl-run/ecl-format-*`; machine-readable
outcomes in [native-result.json](native-result.json).
