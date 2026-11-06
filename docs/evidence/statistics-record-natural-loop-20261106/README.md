# Statistics record: natural kind loop restores the complete 274-byte extent

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete authored owner `InitializeGameStatisticsRecord` occupies
`0x0042ACB0..0x0042ADC1`, **274 bytes**, stack record pointer / RET4.
This checkpoint adds **zero exact credit**.

The fresh installed-source normal-COFF baseline emits302 bytes with12 fields.
Its hand-unrolled five kind/index pairs collapse into one induction variable,
leaving extra address arithmetic. An ordinary110-iteration loop naturally
auto-unrolls five ways under pinned VC7.1 SP1, recovering the original three
induction chains and0x2D0 stride. Keeping the existing cursor at record+628
also restores the original field displacements. The complete result becomes
**274 bytes /8 fields** in both normal COFF and a separate single-input
`/GL /GS` cold link. Their complete code is identical after binding addresses.

All eight original operands are independently decoded, including both
hoisted table bases. The nine consumed name bytes are eight hyphens and NUL;
the110 signed table reads retain their external view. No original physical
data allocation is added. The diagnostic resolves each actual candidate
relocation to the independently checked original referent at its own actual
field position, then compares **all274 bytes**. It leaves original bytes and
every local branch displacement untouched. It reports **174/274 matching,
100 different bytes**; original and candidate field positions also disagree.
This is **NONEXACT**, not a shortened comparison or canonical acceptance.

The remaining score loop keeps separate active/name induction cursors and
spills its five-group counter. The original keeps one cursor and ESI count.
The kind tail also retains two different SIB encodings. Fixed literal-name
and aggregate-name copies are byte-neutral after address binding; an ordinary
score initializer method emits280 and indexed score loops fully unroll to791.
Those controls are restored. No volatile, register/ABI, padding, noinline or
compiler-profile panel is adopted.

[Compact results](native-result.json) pin both complete artifacts, sources,
the eight original/candidate bindings and rejected source forms. The changed
source has **no canonical consumers** and receives no match-unit or matches.csv
entry. Existing boundary, origin, source-mapping count and exact totals stay
unchanged. The ledger's old “source absent” wording is corrected; the older
isolated274-byte proposal is not promoted to current proof.

Private source controls, retained compiler logs, read-only decodes and the
full diagnostic are under
`.analysis/gpt-web/20261106-statistics-record/`. Final normal object:
`build/gpt-web/20261106-statistics-record/kind-cursor.obj`; independent linked
image: `build/gpt-web/20261106-statistics-record/final-linked/`.
No historical cohort, host failure simulations, game execution or product
closure was tested. Original source/TU/data ownership and portability remain
separate questions.
