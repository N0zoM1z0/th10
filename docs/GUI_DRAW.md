# GUI draw and untextured rectangle recovery

## Current exact follow-up — 2026-10-30

The complete483-byte rectangle is now canonical exact in two independent colds,
including all14 fields and the actual EDI/EBX interface. Real XYZ temporary
construction and descending W/color stores recover the target; see the
[complete rectangle proof](evidence/rectangle-vector-exact-20261030/README.md).
This grants no exact credit to the724-byte GUI drawing owner. Sections below
retain the October17 source/native history and its former407-byte rectangle;
the archived host/regression commands are not current checkpoint requirements.

## Scope and status

This local-only packet recovers two complete substantial target owners: GUI draw 0x00415800–0x00415AD3, 724 bytes, and its rectangle renderer 0x0043BDA0–0x0043BF82, 483 bytes. It also preserves the real 7-byte registered callback 415AF0 as source context, without claiming a separately authored/exact wrapper or changing its ledger origin. No GUI initializer/factory is invented. During packet preparation on 2026-10-17 UTC, no native build, remote source edit or target execution was performed. At preparation, byte exactness, original source/TU, private native ABI and runtime/GPU closure remained open; the later integration/native sections distinguish what has since been verified.

Pinned target SHA256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040. Capstone 5.0.6 and all four decoder hashes matched config/tools.lock.toml. Draw dense decode covers 173 instructions / 724 bytes; rectangle covers its entire 483-byte reviewed/high extent. Source baseline HEAD a3aafea0a8fba4e4daf2d5b9b9751152bcb624a4 includes the parent's dirty named-slot lifecycle edits. Existing Gui.hpp baseline hash c11e2d1cba86a7d91768e696db12286ddca23c1fd1e1dbcf62eb010394021530 was reverified before packaging. That whole file is preserved except the packed-color field, its layout assertion and the draw method declaration.

Source organization was chosen before native diagnostics by responsibility: GuiDraw.cpp owns GUI composition; AnmRectangle.cpp/AnmRectangle.hpp own the untextured primitive. The new rectangle API is separate from AnmManager.hpp so existing ANM producers do not acquire unnecessary header changes. Existing files retain CRLF; new C++/header files use CRLF. No original TU claim follows from these names.

## Source contracts

The GUI draws 40 unconditional VMs: ten interleaved 0/1 pairs, then arrays 2, 3, 5. It reloads the canonical ANM manager for every call. A nonzero signed state 474C58 draws black/white-alpha rectangles; alpha is reread between calls and the second endpoint reads the actual stored rectangle after the first renderer returns. Ordered-positive bossGaugeDisplayed draws its background/foreground, followed by four independently read nonzero segments, preserving negative/NaN predicates, packed color, selected minimum and intermediate endpoint stores. Marker and final spell-pair gates use actual EnemyManagerView.specialEnemySlots[0] and EnemyFullObjectView.runtime.flags. The enemy manager is reread after marker draw; final-pair gates run once and each pair draw still reloads ANM manager. Return is 1.

The rectangle renderer flushes before coordinate reads; creates four local genuine AnmUntexturedVertexView vertices; uses existing D3d9 interfaces/constants; reloads the device for every call; ignores HRESULT failures; invalidates six caches on the freshly reloaded manager after DrawPrimitiveUP; and restores the four observed texture-stage settings. It does not use manager-owned vertex storage, restore unobserved state or substitute the existing generic untextured-strip routine. Input is not written by the renderer itself; the mutable pointer preserves callback-visible timing in the GUI.

The callback target bytes are 51 E8 0A FD FF FF C3 at 415AF0: push ECX; call 415800; ret. Natural int __fastcall GuiDrawCallback(void*) forwards to int __stdcall GuiView::DrawStageElements(GuiView*). This explains the observed ECX-to-stack bridge, and the later fixed native diagnostic verifies the emitted bridge ABI/contribution without granting exactness credit. It is the real callback address registered by 413980, not a fabricated retention root.

## Packed-color and canonical-storage audit

GuiBossGaugeValueView previously had float value and float unknown04. Complete source scan plus target field-reference scan establishes second word as packed 32-bit color:

- ECL SET_LIFE_MARKER target 40FC8B stores integer color to 9E98+index*8; 40FC96 stores float ratio to 9E94+index*8. Maintained EnemyEclDispatcher.cpp lines 1937–1938 already implement those distinct stores.
- ECL SET_LIFE target 40FF41–40FF6B clears all four value/color pairs. Maintained lines 2017–2024 perform raw 32-bit clears.
- GUI update target 415114/1E/28/32 clears only the four values; maintained Gui.cpp leaves colors untouched.
- Draw 415A00 loads the second word directly as packed renderer color. Constructor 413810's whole-owner zero also covers these fields.

No numeric floating consumer of the second word was found. Rename to unsigned int color preserves raw representation; no float numeric cast is introduced. The existing large ECL dispatcher remains unedited; its raw-offset alias debt is explicitly retained. The bounded target displacement scan is evidence for these known consumers, not proof that every possible dynamically aliased access has been discovered.

New draw uses canonical g_EnemyManager at 477704, g_AnmRenderManagerView at 491C10, g_GuiState4C58 at 474C58 and existing ANM primaryColor.alpha at 8393. Existing Gui.cpp partial enemy alias g_GuiEnemyManager has the same physical target slot but is not duplicated or replaced in this packet. Source storage definitions/alias binding are still external. GuiDraw.cpp asserts the actual specialEnemySlots, runtime.flags and alpha offsets using real headers.

## Host tests and limits

Commands after integration (or against this proposal directory):

    python3 -O scripts/test-gui-draw.py --mutations --asan-no-leaks
    python3 -O scripts/test-anm-rectangle.py
    python3 -O scripts/test-gui-draw-arithmetic.py
    python3 -O scripts/test-gui-draw-layout.py

GUI fixture extracts the complete maintained owner and callback plus actual GuiView, gauge and rectangle declarations. Preparation runs on 2026-10-17 UTC: thirteen scenario groups passed O0/O2/UBSan and ASan without leak checks; 36 compiled mutants must fail at their unique intended CHECK identifier, exact generated line and expression, exit 41, empty stdout and exact stderr. ANM/Enemy boundary objects are explicitly pointer-neutral mocks with valid lifetimes, so these are not full-layout or native execution tests.

Rectangle fixture extracts complete maintained source and actual rectangle/vertex/D3D declarations, retaining an explicitly bounded manager mock. Preparation runs on 2026-10-17 UTC: 128 cases passed at O0/O2, with 16 intended CHECK mutants at each optimization; 128 ASan+UBSan positives passed. A kill requires exit 86, exact generated CHECK line/expression, empty stdout and exact stderr. The initial rectangle worker classifier incorrectly accepted arbitrary nonzero failures; it was rejected and replaced. Corrected omitted-flush uses valid device storage and fails the intended flush-count assertion. Python -O cannot bypass either suite. Compiler errors, timeouts, crashes, unrelated assertions and sanitizer diagnostics are never valid kills. Leak detection is explicitly disabled because local ptrace prevents LSan; no leak-safety claim.

The 2026-10-17 UTC GNU-i386 full-header diagnostic compiled both complete production source files with every existing target size/offset assertion enabled. Only declaration-only stddef/string headers and Microsoft calling-convention/attribute macros are supplied to avoid a multilib libc dependency; no game type is replaced. This supports source/layout compatibility under GNU i386, not VC7.1 layout, PDB, calling convention or codegen equivalence.

Arithmetic witnesses use exact rational finite models and explicit nearest-even binary32 stores. They distinguish multiply/add retention, stored-right subtraction and early integer conversion beyond 2^24. They do not establish target x87 control word, compiler excess precision, signaling NaNs or denormal behavior. Ordinary int*float source leaves native FILD and multiply/add lowering to the actual compiler; host SSE success is not native arithmetic proof. No forced double/volatile precision workaround or source tuning was introduced.

## Existing producer impact and next native step

Only Gui.hpp changes among existing headers. The preparation-stage recursive source/support include inventory matches 54 configured exact units across 28 canonical producer groups (26 linked-PE, 2 COFF), identical to the lifecycle packet's set. Compact unit/group selections are in GUI_DRAW_PRODUCERS.json; profiles, sources, harnesses and entry symbols are read from the pinned canonical config/match-units.toml rather than duplicated; the existing match-unit configuration SHA256 6379c4d2b0ef4bc4d636d0ecefebc8cc37ff1375bfe2b69984a8712b9ebc413f was independently reverified. Existing profiles must remain unchanged. The new isolated rectangle header avoids expanding impact to 252 ANM-dependent units / 92 output paths.

Direct source closure of the two new owners uses maintained AnmRenderManagerView::Draw at 4451C0 and FlushVertexBuffer at 442F50. There is no still-source-empty direct callee after the rectangle implementation. D3D virtual calls, imports, external globals and data-anchor initialization are not runtime closure. Future fixed native diagnostic should root the genuine GuiDrawCallback and compile GuiDraw.cpp, AnmRectangle.cpp, AnmManager.cpp in that responsibility-based order, using the existing ANM renderer /GL profile and nonexecuting anchor harness. See the separate native-command proposal; do not create a canonical exact unit or change existing regression profiles merely to run that diagnostic.

## Integration observations

Integrated after lifecycle checkpoint 9756b3e on 2026-10-17. Remote host checks passed at 02:43 UTC: GUI 13 scenario groups and 36 strict mutants, rectangle 128 cases and 16 strict mutants at each of O0/O2, sanitizer positives, rational witnesses and complete-header GNU i386 assertions. GUI ASan uses host-only -fno-pie/-no-pie, matching the rectangle runner and earlier lifecycle environment observation. An earlier combined run was terminated with SIGTERM and empty buffered output; it is not a pass or a mutation kill, and its root cause is unproved. The rerun uses no native-profile changes. Leak detection remains disabled in these new host sanitizer checks.

The fixed genuine-callback diagnostic completed at 02:45 UTC: PE SHA256 8610fba5e6b7808826c7c58d4c1b78ca00f288ea5632df058f33ddf7ed46f963. Full PDB contributions retain GUI draw 708 bytes versus target 724, rectangle 407 versus target 483, genuine callback 7, ANM Draw 216 and FlushVertexBuffer 133. Both main owners remain nonexact; the detailed native review below is complete; all existing-unit regressions subsequently passed as recorded below. Reference corroboration and limitations are documented in GUI_REFERENCE_NOTES.md.

## Fixed native diagnostic, 2026-10-17 UTC

The single fixed build described in `GUI_DRAW_NATIVE_PROPOSAL.md` completed using the genuine callback entry, `GuiDraw.cpp`, `AnmRectangle.cpp`, and `AnmManager.cpp`, with the existing ANM renderer profile unchanged. The diagnostic executable was not run. Its SHA256 is `8610fba5e6b7808826c7c58d4c1b78ca00f288ea5632df058f33ddf7ed46f963`; map SHA256 is `4be2a08154c4e7fbc9f5ee288828dd66817ab70f25494a7042d980efeba46128`; PDB SHA256 is `6fd2f1b0789618eff51677a2e97e5f5ad3b796b0aa4fb15ad5dd3422318051c8`. Target identity remains the pinned SHA256 above. PE, map, and PDB identities agree.

Complete PDB-owned contributions are GUI draw at `401000`, 708 bytes versus target 724; rectangle at `4012E0`, 407 versus 483; callback at `4012D0`, 7; FlushVertexBuffer at `409CD0`, 133; and ANM Draw at `40BD70`, 216. Both substantial owners are non-exact. No target-sized window was extended into neighboring contributions, and no new exactness claim is made for the callback or renderer helpers.

The real caller graph survives linking. The callback emits PUSH ECX, CALL GUI draw, RET; GUI draw returns 1 with RET 4. Rectangle receives its pointer in EDI and color in EBX, then uses plain RET. Its Flush call and every GUI-to-ANM Draw call reach maintained implementations, not anchors. These observations establish the emitted private ABI for this diagnostic, without proving runtime closure.

Native arithmetic preserves signed DWORD FILD at draw offset `C0`, retained x87 multiplication/addition, and the five endpoint FSTP boundaries at offsets `F8`, `123`, `186`, `1B4`, and `225`. Subsequent bars reload the stored endpoint before subtraction. All referenced floating literals match byte-for-byte. Rectangle coordinates use integer MOV copies after Flush, preserving payload bits. Startup floating-point control state and actual graphics behavior remain untested.

Ordinary residuals remain visible. The first score rectangle schedules LEA and the bottom-coordinate store differently around FMUL/FADD. Target boss flags use a DWORD load, shift/not operations, and two tests; the candidate combines them into BYTE TEST `0x11`. Registers, branches, and alignment subsequently differ. Rectangle uses an 80-byte frame and direct vertex-field initialization, whereas the target uses a 92-byte frame with temporary/copy operations. Chained store ordering, vertex-pointer displacement, and frame cleanup also differ. None of these differences was normalized as linkage.

The complete draw, rectangle, and callback audit binds respectively 33, 14, and 1 address fields through actual symbols or verified literal bytes. Global bindings remain diagnostic anchors. All 12 observed direct rectangle callers ignore or overwrite EAX; both target and current void helper incidentally leave the final HRESULT there. An int return or const pointer is therefore not compelled by the observed callers.

A follow-up audit resolved the ANM Draw accounting question without finding a verifier hole: its 216-byte contribution includes a switch table, so 213 linearly decoded bytes is not 216 instruction bytes. The canonical manifest explicitly binds all ten four-byte table fields at B0..D7, including the undecoded suffix, to verified symbol/addend identities. The comparator checks the complete field set, full PDB extent and all 216 normalized bytes. Here normalization_complete means decoded or authoritative PE-relocation-accounted coverage. This audit does not grant a fresh exact receipt for the present diagnostic.

Natural source is preserved; remaining source-shaping questions are deferred. The existing 54-unit, 28-producer regression set completed under unchanged profiles at 03:23 UTC on 2026-10-17: all 12,420 bytes remain exact. These cold replay results are preservation checks, not new accepted Factory receipts or new-byte credit. The command document above provides the repeatable build and inspection procedure. No canonical acceptance or exact-byte credit follows from this diagnostic.

Final target-independent CI passed on 2026-10-17 at 03:24 UTC, including the new draw, rectangle, arithmetic and complete-header checks. No new canonical exact unit or Factory acceptance was added.
