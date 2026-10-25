# Player shot creation: complete isolated pilot, 2026-10-25

Status: **parked, structural rejection; no maintained source or exact credit**.
Baseline HEAD421750893d94b9b5d36a5fcfd22ea101697bb00f. The reviewed authored
717-byte owner427E90..42815C and real105-byte caller428160..4281C8 remain
source-absent in the canonical ledger. No174-byte upstream expansion.

## Complete early comparison

| Natural proposal | Creator / target717 | Real caller / target105 | Result |
|---|---:|---:|---|
| first, indexed scan / initially incorrect cdecl cleanup | 770 | 123 | structural rejection |
| while pointer scan / corrected ordinary stdcall | 633 | 120 | structural rejection |
| counted pointer scan / stdcall | 772 | 120 | structural rejection |
| target test-first scan / natural motion-subobject scope | 618 | 120 | structural rejection |

Each is a full implementation, not a target-sized fragment. Their candidate
field/branch counts are respectively17/32,15/17,17/32,15/17; target17/25.
The target has22 distinct local destinations,10 direct calls, one indirect
callback, RET12 and no table. All25 branch encodings and all17 address fields
are retained in target-facts.json. All candidate fields, locals, calls and
independently PDB-bounded extents are retained in native-summary.json.
Different extents/local layouts prohibit an ordinary mismatch-byte score.

The first native comparison was early, before source-shape controls. Its
indexed scan and later counted-pointer spelling unroll four-way. A scalar
while form changes optimizer decisions: FindVm and secondary CreateVmVariant0
remain calls rather than the target's inline operations. The final source expresses the target's test-first scan semantics with immediate
exhaustion return and
isolates only the flags/polar velocity calculation in a natural helper.
It still emits618 bytes, with nonmatching inlining and a rotated emitted
scan load/test (cached first state plus bottom compare), not the target's
single top compare. No forced register ABI,
noinline addition, inline forcing, profile matrix or invented null safety was
used. Caller EDI/EBX private-register lowering is not forced by the source.

The actual creator ABI is three stack parameters and RET12, independently
observed in both creator and caller. Ordinary __stdcall corrects the first
proposal's cleanup mistake; this is not a register-allocation hint. The
first proposal is retained as rejected evidence, not recommended source.

## Recovered semantics

- Preserve prior shot fields and flags: scan128 records, state=1, retain the
  input descriptor and call existing timer SetCurrent(0); no memset.
- Signed sourceIndex selects index-minus-one; position uses replayPair1 (not
  pair0), separate float temporaries multiplied by0.01, and z=0.
- Type3 source latch, speed/angle setup, flags-sensitive polar calculation,
  then spawn-offset-minus-velocity position compensation.
- Existing resource creator exposes allocation, initialization and AddVm.
  Capture resource before each allocation, reload manager after initializer.
  Direct FindVm, clear primary ID when null, then unconditional VM flag access.
- Callback receives player/shot/frame, ignores its return. Retain the original
  descriptor across it; reread signed soundId and shot.x afterward.
- The real caller reads unsigned16 power, divides by20, clamps to4 and applies
  mode+5, then walks34-byte descriptors with signed interval sentinel/modulo.

The motion helper contains no position advance. Existing Player motion-layout
evidence motivates its scope, but does not establish an original helper name.
EnemyRuntimeVectorView::FromAngleMagnitude44C5D0 is the exact30-byte dependency;
AnmFloat3's distinct4458B0 helper is not substituted.

## Build and guards

Five TUs: isolated proposal, AnmManager.cpp, AnmVmCreate.cpp, Enemy.cpp and
EnemyEclDispatcher.cpp. Existing TH10 VC7.1 /GL profile, no /GS on primary/
Enemy/ANM, established /GS override for dispatcher. Root is actual105-byte
PlayerFireShots, ordinary parameters. Unrelated unresolved symbols are the
existing diagnostic data anchors; these images are not runnable products.

All four images preserve complete WrapAngle94/all6, polar30/all0, AddVm118/all0,
FindVm73/all0 and sound149/all3: **464 bytes / all9 fields per image**.
These are existing guards, not new credit. Allocator/initializer were already
nonexact dependencies; no claim of their exactness or lack of regression.

Reproduce a proposal using the pinned repository Python, one CPU, nice10 and
a bounded native timeout, retaining the exit and log:

    scripts/repo-python docs/evidence/player-shot-creation-20261025/reproduce.py       --trial motion --output-dir build/gpt-dots/player-shot-motion-reproduction

The output path must be new. Script verifies the target, records the actual
cold-link graph and PDB-owned functions. Reproduction does not admit exactness.
Original image/inputs and observer hashes are frozen in artifact-hashes.json.
All four frozen support/header/toolchain input sets were rechecked unchanged.

## Execution receipts and recovery

- Preflight target, executed toolchain, target-required tracking and status:
  repository-command:b8e143c3ca4846c9a24967bf642ad77f, exit0.
  Native Ghidra check07:10:46 UTC passed, target:th10-main,
  factory-native-command.
- First compile repository-command:a6bf23f16d014d02abdaaebce8fbec12, exit1:
  including Main.hpp after ANM caused Windows critical-section declaration
  conflict. Replaced only that include with the existing g_MainSoundOwner
  extern declaration. Original failed source/log retained.
- First-retry repository-command:da4b1beaa79745f986bbfd51175fd034, exit1:
  compile/link succeeded; JSON metadata failed on PosixPath dictionary keys.
  Separate PE/map/PDB recovery observed770/123, without rerunning or overwriting
  failure. Later runner serializes keys correctly.
- While repository-command:29c613c17ba74460bbf5d4d154158fec, exit0.
- Counted repository-command:104b78e8b75a4d5fbe1ab688da3a0f77, exit0.
- Final repository-command:eaac423d0477433784e3290b3011749a, exit0.
- Complete inventories/five guards on all images:
  repository-command:6721884c423442058eb89bb7d8f3ea0f, exit0.
- Target field supplement and frozen-input checks:
  repository-command:ce04fd64c467446ca126e09d6097baef, exit0.

No timeout or duplicate native run occurred. All native calls were serialized,
one CPU at nice10 and bounded300 seconds. Original logs and build outputs are
retained under .analysis/gpt-dots/player-shot-creation-20261025 and four named
build/gpt-dots/player-shot-* directories; historical artifacts untouched.
Nine unrelated D3D evidence files remain excluded from staging.

## Decision and reopening condition

Do not permute loop spelling, registers or flags. Reopen only with independent
TH10 evidence explaining the actual motion/creation optimizer context or a
specific remaining semantic discrepancy. The first full comparison exposed a
real cleanup/scan-model issue, but none of the corrected complete models
matches. No canonical recipe migration, source/exact ledger edit, denominator
change, runtime claim or phase promotion follows. Previous full CI is not
relabeled for this diagnostic-only packet.


Independent read-only review approves this evidence-only parked packet:
repository-command:0072298a4e54448fa60f02debcec57fd. It checked complete final618
and caller120, exact helper scope, all17 target fields,25 branches/no table,
and all four images' five guards464/all9. No new semantic discrepancy was found.
Final artifact verification preserves122 original hashes
(repository-command:5680913d469d4cd3b9a53c1e1d7f316d).
