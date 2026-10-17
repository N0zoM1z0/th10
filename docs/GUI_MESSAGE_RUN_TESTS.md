# Fresh actual-source repaired interpreter tests

This v2 fixture, driver and mutant catalog were reconstructed after filesystem replacement. Every reported result must come from the new manifests, never the former packet's historical 13/28 counts.

## Reproduce

- Sanitized positives: `./test-gui-message-run.py --root ../test-root --sanitize --no-mutations`
- Unsanitized positives and strict mutants: `./test-gui-message-run.py --root ../test-root --out evidence-unsanitized`

Source resolution uses `<root>/src` then `--fallback-root`, which defaults to the fresh clean source snapshot when present. An installed full repository resolves its own maintained files first. `--fixture` selects another fixture; otherwise the driver looks beside itself or under `fixtures/`. Package the driver and `gui_message_run_mutations.py` together, and the fixture beside the driver or in scripts/fixtures.

All local compiles serialize through `/tmp/gui-message-host-tests.lock` and run at nice level 10. No remote calls, production writes, native builds, target execution, ABI claim or byte-exactness credit occur.

## Retained source

The full actual Run and complete local helper namespace are extracted unchanged. Full canonical headers retain every field. Bytecursor accessors execute unchanged on live bounded byte arrays, including odd-offset records and integers. There are no record accessor seams in this repaired fixture.

Actual selected dependency bodies are extracted from maintained source files:
- AnmManager: FindVm, SetVmPosition, MarkVmForDeletion, timer Add
- AnmVmId: SetInterrupt, SetSprite, SetSpriteWithAnm
- FrontEnd: ResultMenuFindScript, ResultMenuPendingThree, FrontEndPlayMusic
- Player: PlayerSetShotVmInterrupt2
- EnemyEclDispatcher: EnemyMarkPendingInterrupt and QueueSoundSample
- Main: SetNextGameMode

Canonical GameChainOwnerView and sound metadata definitions are extracted from their source. Complete MainSupervisorView is retained; only SDK types come from an explicit windows.h stub. Actual inline timer SetCurrent/Tick and production scalar-width assertions remain, with independent runtime CHAR_BIT, scalar width and endian checks.

## Coverage

Thirteen selections cover all 24 opcodes, unknown24/255, END, future time, multiple same-time records, odd-offset payloads, cooldown, skip/flag behavior, wait pending/edge/expiry/skip, portrait resources and script matrices, both sides, child sprite tables, text lines/alternation, music-before-UI, enemy names, stage completion and effects.

Stage-six/seven matrices cover five difficulties, signed-short power, signed clear counters -1/99998/99999/100000, mode-owner bypass, exact unlock bytes and profile sentinels, effect priority and all logical arguments, and stage-seven ending-state preservation. Invalid character/stage/difficulty inputs are target-guard or defined-scalar cases; ordinary-game reachability is not claimed.

Synthetic callbacks change only live records/resources/owners to observe sprite argument/resource/find-set reload, lookup-before-decode, asymmetric cached/reloaded text manager ownership, dispatch-tail cursor reload and effect/transition profile/scalar/GUI reload. These check target-derived reload and ordering contracts under synthetic host callbacks; production reentrancy is not established.

The cached manager and live activeSide between actual SetVmPosition calls have no callback seam here and remain source-review-only. Unconditional zero-ID lookup versus skipping FindVm is also unobservable here; no runtime coverage claim is made for those distinctions.

## Explicit host seams and exclusions

- Three timer constructor flags(0) seams remain. Raw-integer member-construction validity is unproved. Canonical Player has static storage, so guaranteed language-level zero-initialization precedes its unchanged raw-flag member constructors; this is not raw-prefill or an additional header seam.
- The real message constructor/destructor is not exercised here. Message and other out-of-line owner constructor/destructor definitions are explicit host seams. All consumed fields are initialized and all objects have real lifetimes. Legacy GUI resource shadows deliberately hold a distinct live decoy resource.
- Creation and rendering/resource application are observational seams. Created IDs refer to live registered VMs. Real QueueSoundSample executes on a canonical object placement-constructed in aligned g_MainSoundOwner storage.
- Front-end queued sound commands are observed. No physical audio or graphics occurs.
- Fn00417010 is an explicit bounded plain-text decode seam, not its real algorithm. Fn004175E0 records the exact live g_GameStageState byte-array receiver and already-published mode only. A source check also requires its canonical declaration and call. Native +0x3c stage increment/cap/descriptor publication remains decode-only evidence and is not implemented or claimed in the host seam.
- GameCompleteStageTransition, GameQueueScaledSoundCommand and StageCreateTransitionEffect are explicit external seams. Private ABI/runtime closure remains unresolved.
- Target-layout assertions are omitted. Malformed/truncated buffers, score arithmetic overflow, target execution and native exactness remain outside scope.
- ASan/UBSan runs disable LeakSanitizer because it is unsupported under this host's ptrace environment. No leak claim follows.

## Strict evidence

Each positive process must exit 0 with empty stdout/stderr. Each source mutant must compile, then exit exactly 1, emit no stdout and emit exactly the assigned CHECK plus one newline. Compiler errors, crashes, signals, timeouts, unexpected CHECKs and extra output are failures of the campaign, not accepted detections. The classifier tests those rejection cases.

Fresh manifests contain source/Run/driver/fixture/catalog hashes, compiler commands and exact outcomes. Generated headers, complete extracted bodies, binaries and compile/run logs remain beside the manifests. Each mutant has separate inputs and logs. No historical result is substituted for a new run.

## Fresh verified results

The final layered-root campaign passes all 13 selections both with ASan/UBSan and without sanitizers. The 28 source mutants were executed **without sanitizers**; all compiled and each exited 1 with only its assigned exact CHECK. The sanitized campaign runs positives only.

- Sanitized positive manifest: `repaired-tests/evidence/manifest.json`
- Unsanitized positive and 28-mutant manifest: `repaired-tests/evidence-unsanitized/manifest.json`
- Separate exact mutant outcomes: `repaired-tests/evidence-unsanitized/mutations.json`
- Packaging/source hash verification: `evidence/gui-message-run-verification.json`

Run the packaged driver from the packet/repository root with `python3 scripts/test-gui-message-run.py --root test-root --sanitize --no-mutations --out repaired-tests/evidence`, or use `--out repaired-tests/evidence-unsanitized` without `--sanitize`/`--no-mutations` for the full unsanitized campaign. In an installed full repository, omit `--root test-root`.
