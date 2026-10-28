# Fresh constructor / interpreter / destructor host integration

Historical test record: the construction/lifecycle host suites cited here were
retired on 2026-10-28. Commands for deleted scripts apply to commit `47d9993`,
not the current tree. Their results are historical evidence, not requirements
for exact reconstruction; follow [the current workflow](RE_WORKFLOW.md).

These files and evidence were rebuilt after the workspace replacement. No lost v1 output is presented as fresh evidence.

This is local host semantic coverage. It does not build or execute the native target or establish target ABI/layout, runtime dependency closure, binary equivalence, original source identity or exactness.

## Reproduction and files

- `../scripts/test-gui-message-lifecycle.py`: portable source extractor and strict driver
- `../scripts/fixtures/gui-message-lifecycle.cpp`: complete-header lifecycle fixture
- `evidence/manifest.json`: normal host results, source/owner/driver/fixture hashes and negative control
- `evidence-sanitized/manifest.json`: fresh ASan/UBSan results with the same checks
- Each evidence directory contains generated canonical headers, source-extracted owners/dependencies, compilation logs and execution logs
- `negative-control-omit-sixth-delete/` holds the distinct mutant source, executable, compiler and execution logs

From a complete maintained checkout:

    python scripts/test-gui-message-lifecycle.py --root /path/to/checkout --out /tmp/gui-lifecycle
    python scripts/test-gui-message-lifecycle.py --root /path/to/checkout --out /tmp/gui-lifecycle-sanitized --sanitize

The tested recovery root is `gui-message-interpreter-v2/test-root`, a full baseline with the reviewed candidate overlay. Source selection starts at `--root/src`. Any optional repeated `--fallback-root` directories must be explicitly supplied; there is no hard-coded baseline or implicit fallback. `--fixture` can select the fixture path. Every selected input and its hash appears in the manifest.

Each compiler invocation independently takes `flock /tmp/gui-message-host-tests.lock`, then uses `nice -n 10 g++`. There is no outer lock, native build or remote write.

## Real source exercised

Complete maintained definitions are extracted unchanged:

1. Message constructor and EnemyKillAll from GuiMessage.cpp
2. Typed EnemyReadDialog factory from EnemyEclDispatcher.cpp
3. Entire Gui.cpp prefix through Run, including full local helpers and all 24 opcode cases
4. Message destructor from Gui.cpp
5. FindVm, MarkVmForDeletion, SetVmPosition and timer Add from AnmManager.cpp
6. ID interrupt/sprite methods from AnmVmId.cpp
7. Specialized interrupt, child-lookup and music helpers from FrontEnd.cpp, Player.cpp and EnemyEclDispatcher.cpp
8. MainSupervisorView::SetNextGameMode from Main.cpp
9. Actual sound metadata declaration and QueueSoundSample implementation from EnemyEclDispatcher.cpp

Only WAIT and END are executed in the interpreter here. Other opcode bodies remain present and linked, rather than replaced with a model. The broad interpreter test suite provides separate opcode coverage.

Complete canonical headers are recursively retained. Only target-layout assertions are removed. GuiMessageScalarWidths is preserved, and the fixture independently checks 8-bit bytes, 16-bit unsigned short, 32-bit int and little-endian storage. A Windows SDK-only type shim supplies the platform declarations needed by the complete Main.hpp; no game types are replaced.

## Lifecycle coverage

- The actual typed factory uses ordinary new to run the actual constructor. Allocation/initialization/insertion callbacks observe the owner still unpublished and dialog globals unchanged.
- The constructor's two created text IDs resolve through actual primary linked-list lookup. Both glyph dimensions, callback sequence and three timer initial states are checked.
- Actual EnemyKillAll executes with an empty live enemy list and advances its real owner timer.
- WAIT and END occupy a real bounded unsigned-char record subarray. The factory's offset table is a separate aligned live int subarray in the same actual aggregate. No fabricated instruction object, accessor replacement or sidecar record is used.
- Two actual WAIT calls remain pending while keeping owner/cursor live and instruction/lifetime timers unchanged. A third call completes WAIT, performs actual sound queue array mutations and reaches END.
- Actual END clears the message state and returns before destruction; publication remains live until the driver deletes it.
- A second timing scenario uses a future END timestamp. Actual Run ticks the instruction timer twice, then returns terminal without an extra tick. The lifetime timer stays unchanged because its advancement belongs to the updater.
- An independent all-six-ID scenario seeds four nontext IDs after the constructor, into complete live VM owners on the secondary list. The two text IDs remain actual constructor outputs. One seeded root also has a live canonical child node.
- Actual destructor deletion marks the two lists and child chain, preserves an unrelated VM and frees the message allocation exactly once.
- The deletion observer verifies six ordered calls, including zero IDs; the preceding IDs are zero by the next call. The last ID's zeroing is retained in source and never read after object lifetime ends.
- The observer changes the published global manager to another complete live canonical manager. All six destructor calls still use the originally cached receiver.
- A synchronous throw at the second initialization callback frees the ordinary message allocation once, skips the complete destructor, keeps the owner unpublished and retains the first insertion effect.

The driver explicitly deletes and then clears its owner publication. It does not claim to execute the complete production GUI updater.

## Observation and host seams

The GNU host linker wraps `_ZN20AnmRenderManagerView17MarkVmForDeletionE11AnmVmIdView`. Every observed call forwards to the unchanged maintained deletion body in a separate translation unit. The wrapper records/checks count, receiver, IDs and publication, and applies the disclosed live-manager global mutation to test receiver caching. It is not a replacement deletion implementation and makes no native calling-convention claim.

Exactly three temporary-header constructors use flags(0): AnmVmTimerView, PlayerTimerView and EnemyInterpolationTimerView. All actual timer SetCurrent/Tick/Add methods remain unchanged. Validity of unchanged maintained raw-integer default construction remains unproved. No raw-byte prefill or lifetime optimization workaround is used.

Explicit host lifecycle bodies: AnmVmView constructor/destructor; AnmLoadedView constructor; AnmRenderManagerView constructor/destructor; AsciiManagerView constructor/destructor/GetSize; GuiView destructor; EnemyManagerView constructor/destructor. Message constructor and destructor are actual source.

Constructor callback seams: AllocateVm, InitializeVm and AddVmVariant0. The insertion seam links complete live canonical nodes; lookup/deletion thereafter are actual maintained implementations. Global ordinary new/delete are malloc-backed observation seams. The sound owner is the canonical type placement-constructed in aligned declared sound byte storage and explicitly destroyed. Every consumed field is initialized before use. Unexecuted full-opcode globals exist only for linking and carry no layout claim.

Rejecting, unexecuted boundaries: EnemyCancelBulletRecord; CreateVmAtWorldVariant0; EnemyEffectWaitNode unknown virtuals 0/1/2/3/4/6 and ApplyPatternCancel/ApplyClear; CreateVmVariant0; SetSprite; DrawTextLeft; FrontEndQueueSoundCommand; GameCompleteStageTransition; GameQueueScaledSoundCommand; StageCreateTransitionEffect; leaf Fn00417010/Fn004175E0. The full canonical laser vtable seams supply RTTI required by UBSan; the live laser list is empty.

## Strict control and sanitizers

The negative control removes only the sixth destructor deletion call. Compilation must succeed. Execution must return exactly 1, empty stdout and exactly:

    CHECK destructor.six_calls_and_one_free

Crashes, signals, timeouts, compiler errors, wrong CHECKs, unexpected output and accidental passes are never accepted. Explicit classifier self-tests verify these exclusions.

The sanitizer build enables AddressSanitizer and UndefinedBehaviorSanitizer with fatal errors. `ASAN_OPTIONS=detect_leaks=0` is recorded explicitly because LeakSanitizer is excluded from this managed ptrace host workflow. No LSan/general leak-freedom claim is made. The fixture's one-message allocation/free assertions are independent checks.
