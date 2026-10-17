# Result-transition host contracts

This packet tests a source hypothesis. A passing host harness is not proof of native calling convention, storage ownership/alias legality, target layout, native generated code, or game runtime behavior.

## Run

From the repository root:

The default invocation runs the actual-body baseline and classifier self-tests;
mutations are opt-in. Public CI uses that same default beside the result/pause
controller tests. To reproduce the baseline, the optional strict mutation campaign,
and the complete target-independent CI suite, respectively:

```sh
python3 scripts/test-result-transition.py --manifest /tmp/result-transition-baseline.json
python3 scripts/test-result-transition.py --mutations --manifest /tmp/result-transition-mutations.json
python3 scripts/ci.py
```

Manifests are external evidence, not tracked inputs. A passing focused baseline
does not imply that the full CI suite or native/runtime checks passed.

Source ownership changes must keep `config/functions.csv`,
`config/reccmp-functions.csv`, and `config/implemented.csv` consistent. Regenerate
progress from those ledgers through the repository workflow, then validate it:

```sh
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py --check
```

The generated source-present counts may increase; canonical exact counts and
bytes receive no credit from these decompiled owners.

`--root` selects the repository root. Optional `--baseline-root` supplies missing source only when explicitly provided. `--fixture` defaults to scripts/fixtures/result-transition.cpp beside the driver. The runner extracts each definition afresh and rejects missing/duplicate definitions. It does not modify production inputs. The emitted file is a generated C++ harness, not native assembly. Capture stdout externally if needed; preliminary diagnostics are not evidence of a mutation kill.

## Actual definitions executed

- `GameCompleteStageTransition` and `GameQueueScaledSoundCommand` from proposal `FrontEnd.cpp`
- `CompleteStageFromMessage` from proposal `Gui.cpp`, including both real calls to the transition owner
- `AnmRenderManagerView::RequestCaptureRectangle`, `FrontEndLoadMusic`, and `FrontEndPlayMusic` from proposal `FrontEnd.cpp`
- `AnmVmTimerView`, including its actual constructor and inline `SetCurrent`, from unchanged `AnmManager.hpp`
- Actual `ResultMenuView`, `FrontEndCursorView`, `AnmVmIdView`, `ResultGameChainView`, and `ResultGameManagerView` definitions
- Actual `SoundPlayerCommand`, `SoundPlayerView`, and `SoundPlayerView::QueueCommand` from unchanged `Sound.cpp`

The transition itself keeps its direct queue calls. The separately executed `FrontEndPlayMusic` helper is dependency regression coverage, not a substituted transition implementation.

## Explicit host seams

- ANM manager allocation, resource initialization, ID publication, deletion, and lookup are observational stubs. They operate on valid live host VMs and can change selected global pointers at defined callback boundaries. Allocation failure and invalid sprite/resource preconditions are outside the tests.
- `FrontEndQueueSoundCommand` is an observation/forwarding seam: it records the call and runs a callback, then invokes the actual queue method on a live `SoundPlayerView`. The production raw global-storage wrapper is not executed. This seam makes music-time supervisor changes, profile-pointer reload, completion ordering, and speed sampling observable without pretending the real queue has callbacks.
- GUI score accumulation, stage-clear counters, mode publication, transition effect creation, and the alternate next-stage helper are stubs. The two relevant GUI-to-transition calls execute the real transition body; unrelated GUI paths are not presented as covered runtime integration.
- ANM sprite/VM/manager/GUI/ASCII views are field-minimal host fixtures, not target-sized objects. Windows/DirectSound pointer/scalar names in the actual SoundPlayerView are host aliases or forward declarations. Target offset/size assertions are deliberately not extracted.
- `g_GameOwner477830` retains the production pointer-to-incomplete-`GameChainOwnerView` declaration. The fixture stores a round-trip cast of a pointer to a live `ResultMenuView`; it never dereferences that pointer as a `GameChainOwnerView`. This avoids testing unrelated object lifetime or invented inheritance, but proves no production alias/storage legality.
- All real timer-containing objects have static storage, so their bytes are zero-initialized before the actual constructor reads and clears its flags bit. Reset is fieldwise on those existing live objects; no uninitialized automatic timer is constructed. GCC's generic constructor `-Wuninitialized` warning is suppressed only around that extracted declaration, with the actual body unchanged. Other warnings remain errors.

The cached game pointer is source-reviewed, but no artificial callback is inserted into the actual inlined timer. Runtime interleaving between that initial read and the flags store is therefore not established by this harness.

## Coverage

- Game initialization mode exactly 1: configuration bit `0x1000` selects transition 2/4, with no menu/queue/profile/timer/speed side effects. Modes 0, 2, and -1 take the normal path. The configuration/global distinction is independently mutation-tested.
- State 6, timer previous/current/subframe and active/inactive scale/flag handling, and preservation of other game flags before allocation.
- Capture/front resources cached across callbacks; initial manager reload occurs after initialization, capture uses the cached receiver but current global lookup, and the second allocation uses the capture manager. Both published IDs and VM setup are checked.
- Actual capture source truncation and exact destination rectangle; busy capture does not modify capture fields and does not prevent the owner finishing.
- Script 0 followed by script 129, exact dependency ordering, music filename, optional release opcode 4, play opcode 2, and supervisor flags re-read after music loading.
- Profile pointer re-read after the last queue callback, exact index `0x1d8a3`, completion value, speed sampled after callbacks, then speed reset to 1.
- Byte snapshots verify all ResultMenuView bytes outside explicitly mutable host members remain unchanged. Early-return cases permit no menu byte changes. This is host object-representation coverage, not a target-layout claim.
- Real first-free queue behavior, exact path/opcode/argument/step, holes, occupancy 29/30/31, reserved slot 31, and full-queue drops while completion/profile publication continues.
- Scaled-sound selected finite samples: signed zero speed, fractional/unit/above-unit/negative speeds, positive/negative/fractional/zero inputs, truncation, queue drop, empty path/opcode/return, and a sample distinguishing a premature float intermediate from the current double expression.
- Actual GUI early completion route, game-mode early return through that route, and stage-seven caller ordering around score updates/transition/count publication.

## Floating-point limit

No universal finite/in-range equivalence to x87 is asserted. For example, magnitude bits `0x4efdffff` and speed bits `0x3f7fffff` produce an exact quotient `2130706431 - 1/16777215`; binary64 can round across that integer while extended precision truncation differs. The double-versus-float mutation tests the selected C++ host contract only. NaN, infinity, overflowing conversions, native x87 stack/rounding state, and the precise runtime behavior of `0x463B2C` remain unresolved.

## Strict mutation rules

1. The unmodified assembled baseline must compile and pass before any mutant is classified.
2. Every mutant starts from the unchanged extracted baseline and replaces exactly one occurrence in exactly one named body. A zero/multiple replacement count fails the run.
3. A compile failure is an error and never a kill. Mutations must be valid C++ and use valid objects, indices, finite representable conversions, and safe sample domains.
4. Each mutant has a predeclared expected contract ID. A kill requires exit 86, empty stdout, and exactly `CONTRACT_FAILURE: <expected>\n` on stderr. Wrong assertions, signals, other exits, and extra diagnostics fail classification.
5. Classifier self-tests require one accepted positive control and twelve rejected negatives, including a compile failure, abort/segmentation signals, wrong assertions, extra stdout/stderr, duplicate markers, malformed newline, and absent output.
6. The final JSON records compiler/version/options, every expected/observed mutation failure, and SHA-256 hashes for all production extraction inputs plus the driver and fixture.

Native builds and target execution are outside this host campaign. Record fresh results and compiler/options in external manifests; generated harnesses, binaries and full logs are not repository deliverables. The NEXT local campaign includes a separate UBSan baseline at -O1 -fsanitize=undefined -fno-sanitize-recover=all; this does not expand native proof claims.

## Integration and remaining gates

Based on checkpoint 2a254c553ec53c1b2945351237fcece3662aaa82. The current Run/lifecycle drivers were reconciled with only ResultMenu.hpp header-copy additions and matching fixture signatures. Non-PIE sanitizer options and truthful --no-mutations summaries remain intact. The follow-up adds one baseline-only result-transition hook to the existing scripts/ci.py, beside the result/pause tests, preserving all other CI and portability changes. The two recovered FrontEnd.cpp owners remain decompiled at 0.00; host results confer no native match credit.

423370 target receiver is EDI with plain RET; the natural ResultMenuView* source interface is not native ABI closure. 420C30 numerical width/order/conversion and physical storage bindings require native inspection. No effect-family implementation or whole-Run closure is claimed.
