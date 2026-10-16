# Hint entry consumption and per-frame update

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
