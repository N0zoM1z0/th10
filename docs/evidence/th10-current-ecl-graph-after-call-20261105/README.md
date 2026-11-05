# TH10 current-source ECL graph after typed CALL and stack reconstruction

Verified original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This is the **cross-source current-epoch** proof after code checkpoints
`cbe03f9` (4-byte stack tag slots), `58ffb13` (typed CALL name offset) and
`0dd8b20` (8-byte CALL descriptor record). Earlier compiler graph source
hashes in historical documents must not be confused with this source epoch.

A pinned MSVC 7.1 SP1 `/GL` **eleven-source full Enemy ECL graph** was
freshly recompiled, including all support objects affected by `EclVm.hpp`.
Complete PDB contributions preserve candidate-to-candidate byte and field
geometry relative to the prior post-context-time graph:

- `EnemyRuntimeView::DispatchEclInstruction`: **14,352 candidate bytes**,
  **716** candidate address fields, zero ordinary differences. Original is
  **14,416** bytes; **NONEXACT**, still 64 bytes short.
- `EnemyBeginSpell`: **2,432 candidate bytes**, 108 unchanged candidate
  address fields. Still **NONEXACT** despite equal target physical extent.
- Enemy death finalizer: 162 bytes, nine unchanged fields.
- Enemy viewport owner: 330 bytes, ten unchanged fields.
- Full Enemy object constructor: 672 candidate bytes, five unchanged fields;
  original 673 bytes, still NONEXACT.

Separately, a freshly compiled real two-source `EclVm.cpp + Enemy.cpp`
`/GL` graph strictly compares the entire **7,020-byte** generic ECL VM
interpreter with **all 189 independently original-target-bound fields**,
including local/table destinations. Its result remains **6,997/7,020**
ordinary target bytes, with the same 23 remaining differences, NONEXACT.
The 551-byte `StartSubroutine` candidate is also still one byte larger
than the original 550-byte function, although the typed CALL descriptor
source preserves its four candidate call fields exactly.

For the **combined latest installed source and header epoch**, eleven named
canonical original-target exact units were selected and cold-replayed.
**11/11 pass, 1,129/1,129 previously credited bytes**; no newly credited
whole function, no all-unit historical cold replay. Individual complete
unit receipts, pinned sources and independent original mismatch offsets are
in `native-result.json`.

An additional *real* five-source ANM opcode-91 control copied the layer field
through a 4-byte `memcpy` after `GET_INT_VAR`. It retained the 9,964-byte
PDB owner, all 350 original-bound fields and **9,962/9,964** ordinary target
bytes, with the same EDX-vs-ECX two-byte difference. Source was restored.
This negative control is not a new exact match. Previous hoisted resource
pointer and volatile-layer experiments remain separately documented.

Full native PE/MAP/PDB under ignored
`build/gpt-web/20261105-ecl-current-epoch/fresh-11-tu/` and
`build/gpt-web/20261105-ecl-call-argument/per-access-tags/`.
No runnable full game, original source identity or runtime equivalence claim.
