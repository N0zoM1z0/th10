# TH10 complete 186-byte runtime bootstrap — canonical exact

Target original Japanese TH10 1.00a 0x004201B0..0x00420269 is a reviewed,
CC-delimited **186-byte** game startup function. The target executable matches
`config/target.toml`. Previous Ghidra function inventory missed this pointer-
called boundary; the complete original instructions were inspected directly,
not inferred from a guessed C++ source filename.

The new `src/GameStartupRuntime.cpp` implements the full startup sequence:

- Call real `GameLoadStartupVersion` at 0x00420100 (a separately source-
  present, nonexact 174-byte owner); set game speed to 1.0 and text default
  color to `0xFF000000`; invoke real `StageResetPublishedCameras` at
  0x004216F0.
- Call **WinMM timeGetTime** via original imported IAT at 0x00466270,
  publish tick at 0x00491FF8, and copy its low 16 bits to both existing
  `RngView` seeds at 0x004918B0 and 0x004918A8.
- Create the sound-file loading thread through real `CreateThread` IAT at
  0x00466118, with genuine `SoundDataLoaderThread` at 0x0043D080 and sound
  owner 0x00492590; store its handle at 0x004977AC.
- Create FPS/replay timing owner via full 151-byte function at 0x00413350.
  This callback is source-present but currently nonexact; do not mislabel it
  as exact just because its *callsite* matches.
- Stop the real `g_MainScanWorker` at 0x00474DD4 via the exact 0x0044C150
  `ScriptViewerWorkerView::Stop` owner. Preserve the original worker fields
  +0x18 entry, +0x10 running, +0x0C stopRequested, +0x08 thread ID, and +4
  thread handle while passing the actual `InputDeviceInitThread` at
  0x0043BA90 and the real `g_MainSupervisorView` pointer 0x00491C28 to
  CRT `_beginthreadex` at 0x00453683. The six arguments are prepared in
  the original retained caller, not hidden in the shared `Start` method.
- Invoke real `AnmRenderManagerView::SetupVertexBuffer` at 0x004462F0 and
  `TextHelperView::CreateTextBuffer` at 0x00437A00; return zero.

The source uses actual maintained worker, ANM, RNG, sound/thread and text
interfaces, not synthetic x86 instruction blobs, forced register variables,
missing resource stubs, or magic original executable pointers. New global
storage names are explicitly descriptive; there is **no** claim about their
physical original TU declarations or complete runnable retail game.

## Actual compiler-context recovery and proof

An isolated pinned VC7.1 `/GL` candidate had a shorter 130-byte extent;
adding real `ScriptViewerWorker.cpp` produced 133 bytes because a reusable
`Start` function did not expand the six-parameter CRT thread start path.
Writing that same supported worker-state launch sequence directly in the
bootstrap produced 187 bytes, with correct control/data operations. Adding
the **actual unchanged `src/AnmManager.cpp`** alongside the worker source
made the optimizer use the original private EAX `SetupVertexBuffer` ABI and
schedule the worker handle/renderer loads as in target, producing **186/186**.

The canonical `game-startup-runtime` unit declares that genuine three-source
`/GL` compiler graph and the actual `GameInitializeRuntimeSystems` entry.
Two distinct zero-reuse serial VC7.1 cold PE/PDB artifacts compare **186/186
exact** with **78/78 ordinary bytes**, no ordinary mismatches and **27/27**
independently target-bound fields (all original call destinations, APIs,
fixed globals, and explicit +4/+8/+0x0C/+0x10/+0x18 worker field addends).
All fields are covered; none is masked as a fake source data span. The
full strict proof receipts are retained at
`.analysis/gpt-web/20261103/startup-runtime-cold{1,2}.json`.

This receives **186 authored codegen-exact bytes**, not exactness credit for
any dependency. Original runtime behavior, product integration, and source
file/class ancestry are still unresolved. Selected affected exact units are
cold-replayed locally, not the full historical or product cold cohort.
