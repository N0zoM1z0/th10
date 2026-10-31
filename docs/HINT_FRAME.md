# Hint entry consumption and per-frame update

## Current exact-stage checkpoint — 2026-10-31

The historical source/model and host-test reports below describe their dated
checkpoints. They are not current validation gates. The maintained frame
expressions now use natural float operands and branch-local height evaluation;
the old explicit-double source model is superseded. The [current native proof](evidence/hint-frame-values-20261031/README.md)
records complete Update571/571 NONEXACT and the exact10-byte registered update
callback, whose origin remains indeterminate. The [consumption proof](evidence/hint-consume-context-20261031/README.md)
records1308/1317 NONEXACT. No shared declarations change in these two checkpoints.

## Scope and evidence

This source checkpoint reconstructs natural `HintOwnerView::ConsumeEntries` and `Update` interfaces for target 419120/1317 and 419650/571. It is based on the pinned Japanese 1.00a decode and the maintained sources at clean repository HEAD c94bd4c56edd6695f0d9cbf5aa37d110461fb0d6. It adds no fake registered callback, renderer, resource, global storage, or forced private calling convention. Callback registration and factory/lifecycle integration remain a separate contribution.

The source calls the maintained ANM creation, lookup, deletion marking, interpolation, and variadic text APIs. Their prior exactness does not transfer to these owners. No native executable run, resource/script identity, native binary match, original declaration provenance, or product closure is claimed here.

## Layout and shared declarations

`HintEntryView::position` now uses the existing `AnmFloat3View`. Loader and exporter accesses change to x/y/z without changing bytes or behavior. This bounded type correction permits the target-observed direct position pointer to reach VM creation, avoiding an invented temporary or an aliasing cast.

The owner names its two counters, base comparison value, ten IDs, cached positions and widths, and signed slot index. Existing 32-bit assertions check size 1A8 and the newly used offsets. The loader/export lists and entry storage remain unchanged.

`GameEffectUpdateView` moves unchanged in identity from GameManager.cpp to a shared header, exposing `loadedResource` at 899C while preserving `firstVmId` at 89A0 and `secondVmId` at 89A4. The pointer `g_GameEffectUpdate` remains one external canonical object. Likewise, the local stage/base roles reuse `g_GameStartupMode` and `g_GameFrameRestorePending`; they introduce no hint-specific storage or global renaming.

The shared header move requires rechecking the canonical GameManager draw and update callback producer groups. The existing Hint clear-lists 79-byte contribution must retain its full canonical result. The declaration extraction and new vector field can affect optimization even where source behavior is unchanged; host tests are not evidence that these native regressions passed.

## Behavioral boundaries retained

- Countdown decrements only for zero/matching base. An already due nonmatching entry is still emitted.
- Cache next before mutation; advance the slot and unlink before optional deletion. `remaining` is untouched.
- Select scale category before creation, then reread scale for post-creation configuration. NaN takes the large category. Interpolation endpoints are explicit float objects with initial and final in the maintained order.
- Creation receives the actual entry position, after the y adjustment. The slot and manager reload points follow the observed lifetime boundaries around creation, interpolation, text, and deletion.
- Cache the created VM's `positionOffset`, not its position or the input position. Invalid alignments do not draw or update cache fields.
- Entry text is the format argument itself, with no added `%s`. Fixture mocks inspect identity and never perform uncontrolled expansion.
- Failed creation/lookup still clears the ID and proceeds into the native invalid VM dereference. No quiet recovery branch is added. Tests use valid created VMs and do not execute this unsafe path.
- Stage bounds and slot bounds are not silently added. Signed slot remainder and strict alpha boundaries are retained. No clamp, diagonal blending, or epsilon is introduced.
- Zero IDs bypass the corresponding manager method calls; no generic manager-null recovery is introduced. Active primary alpha interpolation suppresses proximity updates. `currentBase` is not overwritten by Update.

## Floating-point qualification

The source retains explicit float assignments for mutated y, interpolation endpoints, cached x, dy, half-width, and outer half-width. Double expressions represent retained x87 intermediates for adjusted player position, dx, and height. This models PC53 calculations and distinguishes known float-store boundaries; it is not a statement that C++ double on every host reproduces all target x87 executions. MSVC may retain a double expression in x87 registers. Native x87 precision and rounding control have not been established; eager floating-expression evaluation and floating-trap equivalence are outside this model. PC64 cases can differ from a binary64 host, including a documented near-boundary discriminator.

The separate arithmetic witnesses exercise conditional PC53/PC64 round-to-nearest models, explicitly label stores, and demonstrate where universal float promotion or universal float rounding changes outcomes. They are numerical evidence, not target execution.

Fade q uses signed division, only on a selected fade path. Ordinary rendered alpha originates as 0..255. Tests of fade arithmetic remain in defined integer/conversion ranges. Native wraparound in signed counters/counts, large 3*A overflow, and x87-helper conversions of NaN/out-of-range values are not established as portable C++ behavior by these fixtures. Byte-only interior/restore paths do not evaluate q.

## Local validation and test seams

The existing loader and exporter fixtures still compile their actual source region, ending at the new frame-owner section. They receive the unchanged production Hint header and a fixture-only ANM dependency header whose vector declaration is extracted verbatim from the production ANM header. They also copy the actual VM-ID header. Their prior mutation suites remain enabled. This is a declared host dependency seam, not a replacement production header or a whole-TU/native ABI test.

The frame fixture compiles the actual new source region with instrumented dependency declarations and exact method signatures. Synthetic entries are initialized by the fixture; constructor behavior remains covered by the loader suite. Synthetic dependency mutations make captured-versus-reloaded state observable; they are contract witnesses, not evidence that actual game scripts or text methods perform those mutations. A mutation passes rejection only if compilation succeeds, execution exits exactly 1, and stderr is one labeled CHECK diagnostic. Crashes, sanitizer errors, and compiler failures are not successful mutations. The fixture substitutes only the entry-deletion dependency boundary with a pre-release observer. It checks link fields while the entry is alive, then performs the real matching delete; it does not read typed entry fields after lifetime ends.

Native integration compiled the actual production headers with their i386 layout assertions. The bounded diagnostic and three complete canonical regressions are recorded below.

For lightweight CI, each host suite accepts `--skip-mutations` to execute only its baseline fixture. Full mutation controls remain enabled by default and are required for the functional mutation evidence reported with this proposal.


## Native integration checkpoint

One pinned VC7.1 diagnostic uses the real Update member as entry, Hint.cpp as
primary source, and ordered support FileSystem.cpp, AnmManager.cpp,
AnmVmCreate.cpp, AnmVmId.cpp, RandomMath.cpp and EnemyEclDispatcher.cpp.
This is a nonruntime diagnostic, not the still-unreconstructed callback
registration or complete factory graph. PE SHA-256:
1d01d519dc979d9826ef3df713ffb8100f5474b5a831cb5e22c2c7cce276f536.

PDB-owned contributions are ConsumeEntries 1,222 versus target 1,317 bytes,
and Update 539 versus target 571 bytes. Both are nonexact. Sizes are not
matching percentages. Existing loader 1,556 and exporter 2,669 sizes remain
unchanged in this graph, without promoting their exactness.

All three affected canonical units were separately cold-built and remain
exact: GameManager OnDraw 31 bytes, GameManager update callback 7 bytes, and
Hint list cleanup 79 bytes, totaling 117 bytes. The shared effect view retains
its global/type identity and all observed offsets. No newly exact contribution
is claimed by this frame-owner checkpoint.

The maintained source fixture passes its ASan/UBSan/float-cast-overflow
baseline and all 49 intended-CHECK functional controls. Legacy loader 22 and
exporter 12 controls pass against the same source. Optimized-Python frame
baseline and rational arithmetic checks also pass. Baseline-only CI is explicit;
it must not be reported as a full mutation run. The pre-release observation
seam inspects live objects and uses ordinary matching delete, without
lifetime-DSE suppression or post-lifetime typed field observations.

## Lexical scale branches and old-VM ownership

`ConsumeEntries` now retains three ordinary lexical creation/configuration branches: ordered scale ≤1 uses script slot+2 and 15-pixel glyphs; ordered scale ≤2 after that uses slot+12 and truncated scale×15 glyphs; otherwise, including unordered scale, uses slot+12 and 30-pixel glyphs. Each branch calls the maintained creation, lookup and interpolation methods with branch-local interpolation endpoints. Category selection remains before creation; fields are read at the existing post-call boundaries. Only the medium branch refreshes the captured manager immediately after interpolation. The shared alignment, color, slot advance, unlink and deletion tail is preserved.

This structure is informed by native creation sites 419233/419311/4193BD and the medium interpolation/reload path 41939B. Separate lexical calls do not prescribe emitted call count: the unchanged compiler may inline a lookup or merge interpolation tails. They are source-ownership candidates, not recovered original declarations or proof of exact machine code.

The old-ID phase now owns the field operations observed at 419166–4191EF rather than calling the maintained noinline `MarkVmForDeletion` helper. A zero ID skips lookup. A nonzero ID is resolved through the captured manager's actual `FindVm`, which searches primary before secondary. A resolved VM receives `flags35C |= 0x04000000u`; only a VM whose `layerNode.previous` is null propagates that bit along its flat `layerNode.next` chain. The selected owner slot is cleared afterward, including for a stale ID. Unrelated flags, the original slot lifetime and manager lifetime are preserved. No shared helper, inlining annotation, ABI, translation-unit arrangement or compiler option is changed; no recursive traversal or invalid-chain recovery is introduced.

### Host contract evidence

The frame fixture removes the artificial Mark-call event. It extracts the maintained `FindVm` body with a lookup-entry observer and copies the real `AnmVmLayerNodeView` declaration. Audited live list nodes exercise primary/secondary lookup, duplicate-ID priority, zero and stale IDs, root/no-child and root/multiple-child cases, nonroot self-only marking, already-marked children and unrelated flags. VM flags and old slot clearing are checked at the creation boundary, before creation can obscure an omitted operation. Manager-refresh assertions observe distinguishable old-ID/post-create lookup positions.

All 49 prior named controls remain, with four expected CHECK expressions deliberately adapted to the replacement lookup observations. Six new controls cover omitted self/child marking, reversed root gating, nonroot child marking, first-child-only traversal and clobbered unrelated flags. The sanitized actual-source baseline and all 55 exact intended-CHECK controls pass. The live pre-delete observer still inspects and snapshots the entry before ordinary matching deletion. Compile failures, crashes, sanitizer diagnostics and unexpected output remain harness errors, never mutation kills. Loader 22, export 12 and lifecycle 65 controls also pass; the O2 sanitized frame baseline and focused Python-O controls pass.

These host seams establish bounded source-contract sensitivity, not native execution, ABI identity or real callback reentrancy. The conditional PC53/PC64 round-to-nearest arithmetic witnesses remain unchanged; the native x87 control word, exception behavior and general host/native floating-point equivalence are unresolved. Failed post-create resolution and invalid indices, chains, overflowing counts or invalid float conversions are not exercised as host evidence. Native function extents and call/expansion structure are recorded separately below; host evidence alone establishes no native outcome.

### Unchanged factory-graph native observations

Two single controls used the same ordinary LTCG profile, factory entry and ordered support sources as the accepted lifecycle graph. The three-branch control produced PE SHA256 `7fed54a1d637bc325f0d3abf566e560c04a3b022be8a948c9e12e3d4bd214b4f`: ConsumeEntries at402680/1234, Update at402B60/541. The following lexical-marking control produced `03955a13514f1593532c5b3d03bb5ca9a56b1e89c30a8d2c5dd92d353ef2cbea`: ConsumeEntries at402680/1340, Update at402BC0/541. Target extents remain1317 and571. These are complete PDB contributions, not similarity percentages.

The latter contribution contains the old-ID lookup/root/flat-child flag walk at4026C9–40274F and removes the artificial MarkVmForDeletion call. Three Create calls and two interpolation calls remain, including the small/large shared interpolation tail. All three post-create Find paths still expand, whereas the target has two out-of-line post-create Find calls. This remaining compiler-context distinction is unresolved. Retain these source-owned phases based on actual control/data structure; neither main owner receives exactness credit. Shared helpers, flags, ABI and source graph were unchanged. Factory67, draw6 and clear79 contribution sizes remain unchanged; their canonical regression results must be recorded separately.

Canonical cold regressions after the lexical controls passed for all three affected units: hint-create-owner67 and hint-draw-callback6 in shared factory PE `4860594c769f50f595471ce7f5476d7209493e5abe1ae85947107c9931d1bfe1`, and hint-clear-lists79 in loader PE `d0457c828a67e6ca63a3871d60a7f5fd7ea4e7c82988b89033e1d6031b03fa84`. Thus152 previously tracked bytes retain their canonical comparisons; none are new credit. Full target-independent CI passed2026-10-16 23:48:17 UTC. These local cold regression results do not refresh prior Factory accepted-fact snapshots or prove complete runtime closure.
