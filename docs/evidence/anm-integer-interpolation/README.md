# Exact ANM alpha evaluator and integer-family boundary review

The complete alpha evaluator at **0x00441950/370 bytes** reproduces TH10 in
two independent complete canonical cold builds, including all **12 address
fields**. This adds 370 authored exact bytes. RGB Evaluate820 and its
ExecuteScript9964 caller remain nonexact.

See [independent target bindings](target-bindings.json),
[two-cold native proof](native-summary.json), and
[28 scoped diagnostic controls](controls.json).

## Corrected source behavior

Scalar alpha adds the initial value to the eased floating-point delta before
truncating the complete sum. At coefficient0.5, fading255 to0 yields127;
the previous product-only truncation yielded128. The target ends this path
with `FIMUL`, `FIADD`, and a tail jump to `__ftol2`. A normal initial-value
snapshot and a cast enclosing the affine sum reproduce that path.

Hermite uses four named endpoint/tangent weights and a shared `t+t`. A named
floating-point result accumulates final endpoint, final tangent, initial tangent,
then initial endpoint, before one integer conversion. Direct sum/product-order
controls remain six ordinary bytes different; stepwise accumulation reproduces
all370 bytes and frame8. Source spelling is a candidate reconstruction, not
proof of original template/class/TU identity.

RGB has a different observed conversion boundary: each eased product is
truncated before integer addition. Its four Hermite vector products also
truncate each component before summation. Those rules remain unchanged.
The executor's RGB byte stores now follow the directly observed red/green/blue
order. Only this byte-store reorder and the alpha body are retained.

## Evidence and replay

Local Ghidra runs read-only/noanalysis with rollback, one CPU and768MiB.
The full mapped range401000..465A80 inclusive is checked before and after:
412289 bytes, SHA256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Disk identity remains the original JP1.00a target. Existing physical extents
and origin classifications are unchanged.

The alpha target-field inventory is independently decoded from the complete
original body. It contains eight absolute fields and four external relative
fields, including the final jump. Literal identities are additionally verified
against target bits; game-speed, curve and `__ftol2` identities use existing
TH10 mappings. Full PDB-owned native extents and every declared field compare
with zero difference; no partial similarity result grants credit.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-evaluate-alpha-interpolation --json
scripts/repo-python scripts/test-anm-alpha-interpolation.py
scripts/repo-python scripts/report-anm-execute-table.py --check
```

Both cold links compile AnmManager.cpp, RandomMath.cpp, AnmVmCreate.cpp and
AnmVmId.cpp with `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`, using the
real ExecuteScript entry. All11 recursive project inputs are frozen before
and after each build. Distinct PDB identities establish independent colds.
Twelve existing complete guards preserve4265 bytes/all174 fields in each
same image: Initialize, SetupVertexBuffer, primary-alpha start, four variable
helpers, scale start, radial trail, both float-vector evaluators and Int3 scale.
Their actual four-TU contexts/output overrides are explicit; existing canonical
recipes are not migrated. Including alpha, each image strictly passes
**4635 bytes/all186 fields across13 complete owners**.

The actual alpha body, owner declaration and mode enum are extracted for a
bounded host test. Twelve scenarios cover rising/falling affine rounding,
completion, additive/acceleration state and Hermite's whole-sum truncation.
Eight mutants must compile successfully and fail their exact named assertion
with exit17; build/sanitizer failures do not count. The positive also passes
ASan/UBSan with leak detection. Timer and linear curve are explicit bounded
roles; this is not original/native-product runtime or x87-control-state proof.
The new test enters standard CI. The transition host's RNG/timer/viewport
extractions remain byte-identical; only its reviewed whole-CPP pin changes.

## Open large-function work

RGB's target private ABI is EAX receiver plus stack output, RET4. Its target
Hermite path has four real calls to Int3 scale441E50/56; the current native
candidate inlines all four and has sixteen `__ftol2` calls overall instead of
four. A previously exact standalone scale leaf does not prove parent outlining.
By-value return spelling and moving the normal scale definition into the same
/GL TU do not solve this. Global `/Ob1` recovers four scale calls but changes
other owners and the parent graph, so it is restored. The reference repository's
inliner investigation is hypothesis material; its forced attributes are not
adopted. Gain real caller/TU/profile evidence before another boundary change.

Complete ExecuteScript remains9964/frameFC, with89 target-sized physical
intervals and absolute gap14; aligned diagnostic7184/7995. Whole ordinary
agreement changes1846 to1850 of8564 through RGB store order; it grants no
parent exact credit. The remaining unequal intervals are61(+1),60(-7),2(+6).
Scale whole-value casts and explicit raw flag OR do not repair its common
copy/flag seam and are restored. Small score variations do not establish a
better storage model. Keep large-owner work focused on these actual boundaries,
value lifetimes and common updates.

Whole Windows i386 product/runtime closure and later semantic/port gates remain
open. The95% authored-byte exact objective remains active. Validation and
artifact cleanup are recorded in [validation summary](validation-summary.json).

All96 standard CI steps pass with source/config frozen. Target/table, toolchain
declarations, tracking, generated progress and whitespace pass. Post-CI raw
source and native artifact hashes remain unchanged. About28.2MB is pruned:
35 raw snapshots become verified line-based deltas,27 native diagnostic dirs
and superseded full reports are removed. Selected bounded evidence and one
current canonical native carrier remain. Pre-existing D3D work and prior ECL
carrier are untouched. Later proof/documentation-only edits receive focused
checks; they do not change compiler inputs.
