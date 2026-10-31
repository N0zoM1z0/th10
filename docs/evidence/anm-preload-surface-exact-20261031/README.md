# ANM surface preload — complete 169-byte exact owner (2026-10-31)

`AnmRenderManagerView::PreloadSurface @ 0x00447EC0..0x00447F68` is **canonical linked-PE exact: 169/169 physical bytes, ten/ten address/control fields** in the original Japanese TH10 v1.00a target SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This adds **one already reviewed authored function and 169 exact authored bytes**, with unchanged target span, source mapping and original executable. The proof includes both early-release and staging-success/error-return branches and both stack-cookie checks.

## Correcting two independently falsified historical source assumptions

The old maintained source declared `char filePath[256]` and used `g_AnmErrorLoggerView.Log(...)`, an artificial distinct class symbol with no actual implementation in this graph. Its full original-sized 169-byte candidate had **eight ordinary differing bytes**. The target at offsets `+0x58/+0x5D` instead references `th10::g_GameErrorContext @0x00474F70` and calls the **actual `GameErrorContext::Fatal @0x0044B8E0`**, a previously separately accepted 201-byte original game function. The two mistaken source names were *not* target evidence: both occurred at the same external storage address only because of the earlier analysis hypothesis. The correct source already includes the real `GameErrorContext.hpp` and calls its existing `__fastcall` variadic Fatal method directly.

A one-variable controlled experiment in the authentic Stage source graph replaced only the fabricated logger with `th10::g_GameErrorContext.Fatal(...)`. This recovered the original **private EDI error receiver** and the exact error-call setup, leaving six mismatches, all displaced stack-frame operands: current `sub esp, 0x108` versus target `sub esp, 0x10C`, associated security cookie home and return-path stack deallocations. Changing only the actual path buffer extent from `char filePath[256]` to **`char filePath[260]`** restores the target 260-byte local array, `0x10C` stack frame, and complete **169/169** body without padding, register forcing, dummy instructions, altered linker flags or target byte modifications. No out-of-bounds safety improvement is inferred; the original uses `sprintf` without a length check.

## Exhaustive ten-field target contract

| Complete owner offset | Original target, actual role |
| --- | --- |
| `+0x007` | DIR32 security cookie `0x00473660` |
| `+0x026` | REL32 `AnmRenderManagerView::ReleaseSurface @0x00447F70` |
| `+0x030` | DIR32 literal format `"%s" @0x0046E1C8` |
| `+0x036` | REL32 CRT `_sprintf @0x00452AE8` |
| `+0x049` | REL32 real `FileSystem::OpenFile @0x0044B360` |
| `+0x053` | DIR32 original Japanese error string `0x00470150` |
| `+0x058` | DIR32 `th10::g_GameErrorContext @0x00474F70` |
| `+0x05D` | REL32 `th10::GameErrorContext::Fatal @0x0044B8E0` |
| `+0x073`, `+0x09E` | two independent REL32 `__security_check_cookie @0x00458EA5` |

The unchanged strict linked-PE comparator requires a unique complete 169-byte `src/AnmManager.cpp` PDB contribution and binds all ten fields to the original typed source symbols and exact addresses. Neither symbol aliases nor coincidental address overlap are accepted as proof of unrelated storage or callback identity. The full body includes both control-flow branches and exact RET termination; no partial fragment or separately compiled helper receives additional credit.

## Scoped independent native cold verification

The existing real `StageCreate`-rooted graph contains 13 source files (`StageScript.cpp`, `StageLifecycle.cpp`, `StageDraw.cpp`, `Chain.cpp`, `AnmManager.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`, `RandomMath.cpp`, `FileSystem.cpp`, `MainViewport.cpp`, `TransitionEffect.cpp`, `GameErrorContext.cpp`, `PbgArchive.cpp`) under its *unchanged* pinned per-source VC7.1 SP1 `/GL /GS`/`/EHs` profiles and original LINK harness. Two independent **unit-selected cold links** of only `anm-preload-surface` and the preexisting `stage-lifecycle-initialize` reproduce **691/691 complete original bytes, 31 typed fields**, both in a single shared Stage producer: original initialization **522/522** and new ANM preload **169/169**. The PDB GUIDs are `eb84c56e0ef76a4c8b00c2fdb72ce53d` and `f573a75c6442354180d838c37358bd09`. The ignored receipts are `.analysis/gpt-web/current-turn/anm-preload-cold-{1,2}.json`.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit anm-preload-surface --unit stage-lifecycle-initialize --json
scripts/repo-python scripts/ci.py
git diff --check
```

No full historical cold replay was run. The result establishes codegen and typed call/data addresses, **not** original physical C++ source/TU provenance, full ANM manager state ownership, a complete standalone GameErrorContext object model, runtime exception semantics or whole-product EXE.
