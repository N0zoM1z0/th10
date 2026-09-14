# TH10 Reconstruction Handoff

## Session state

This checkpoint reconstructs the reviewed TH10 hostile runtime update owner at
`0x0040DC80-0x0040E5EB`. The session started from
`877383708f98e53a6159010e2ab3692d58e393b1` (`main`, ahead 8 / behind 0) with
zero staged, unstaged, untracked, or conflicted tracked-worktree paths. The
checkpoint commit is intentionally not predicted in this pre-commit handoff;
the ignored campaign manifest is bound to the actual commit after creation.
Nothing is pushed.

Recovery was performed from live Factory state. All required repository and
Factory guidance files were present and read in full. Existing ignored
`.analysis/`, `.tools/`, Wine, Ghidra, build/cache, and private-target state was
preserved. There was no interrupted tracked work, unrelated tracked dirty work,
or unknown untracked work to recover or exclude.

The private target remains the operator-supplied ignored file at
`resources/th10.exe`. It was only read through repository verification and
bounded target inspection; it was not modified, relocated, staged, or committed.
No `/mnt` search and no `TH10_TARGET_PATH` override was used.

## Mandatory identity and preflight

The selected repository/provider/target are `th10`, `th10-ghidra`, and
`target:th10-main`. Fresh native Ghidra `check {}` attestations passed for the
selected target with `attestation.provider_transport=factory-native-command`.

Entry repository preflights all passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`

The pinned candidate is VC7.1 SP1 compiler/linker build 6030. The execute smoke
covered normal COFF, LTCG, resource, and PE32 production-shape capabilities.
The target Rich records remain the established 131 normal C, 15 normal C++, and
52 LTCG C++ inputs at build 6030. Per-function production profile, original TU
partition, libraries, resources, and link order remain unresolved.

## Reviewed hard packet: 0x0040DC80-0x0040E5EB

Ledger state at selection was `unknown/review`. Fresh Ghidra reports a 2,412-byte
body whose reachable/body span is exactly `0x0040DC80-0x0040E5EB`. Raw target
inspection closes the physical boundary: all exits use `RET 4`, and
`0x0040E5EC-0x0040E5EF` is four bytes of `CC` padding before the independent
candidate at `0x0040E5F0`. No internal unreachable alignment gap was identified.

Two direct TH10 callers establish the owner relation. `0x0040CFB0` and
`0x0040D750` each pass full enemy object `+0x103C` as the sole stack argument.
The `0x0040D830` constructor starts its large clear at that same offset and
clears exactly `0x537` dwords = `0x14DC` bytes to the end of a `0x2518`-byte
allocation. It also stores the full-object back-pointer at runtime `+0x14D8`
(full object `+0x2514`). The maintained machine boundary is therefore recorded
as one stack `EnemyRuntimeView*` argument with `RET 4`. The source uses
`__stdcall` as the natural VC7 spelling for that observed boundary, but the
original identifier, member/free-function abstraction, declaration, TU, and
optimizer owner remain unknown.

The maintained source introduces exact-size views:

- `sizeof(EnemyRuntimeView) == 0x14DC`
- `sizeof(EnemyFullObjectView) == 0x2518`
- full object runtime tail at `+0x103C`
- owner back-pointer at runtime `+0x14D8`

Target-observed runtime fields include four consecutive `0x2C` motion records at
`+0x000/+0x02C/+0x058/+0x084`, two collision-size pairs at `+0x0B0/+0x0B8`,
eight managed VM ids at `+0x0C0`, the main timer at `+0x11C`, two `0x4C`
position-interpolation records at `+0x13C`, four `0x3C` scalar-interpolation
records at `+0x1D4`, visibility and movement-bound scalars at
`+0x13A4..+0x13B8`, score reward at `+0x13BC`, life at `+0x13C0`, an age counter
at `+0x13C8`, damage-flash frames at `+0x1414`, two additional timers at
`+0x141C/+0x1430`, flags at `+0x1444`, animation script selectors at
`+0x1448/+0x144C`, and the full-object owner pointer at `+0x14D8`.

`EnemyRuntimeUpdate` maintains the target-observed update sequence: movement and
interpolation, optional global motion offset, world/base/offset composition,
movement-bound clamping, offscreen retirement, animation-mode switching, ECL
execution, Player-shot damage, Player/game-state damage scaling, life/death and
score reward, callback ECL handling, Player collision, directional animation,
eight managed-VM position updates, Player tracked-enemy selection, four-frame
damage flash/effect state, and timer advancement. Descriptive helper declarations
hide target-private register conventions instead of claiming conventional ABIs.

The same owner proves that Player `+0x3504/+0x3508`, previously neutral update
scratch, stores a full enemy-object tracked-target pointer plus a per-frame valid
byte. `Player.hpp` and the central Player reset are updated without changing the
Player object layout.

During source audit, target raw control flow corrected three easy-to-miss details:
the callback-ECL install path does not test the installer return, the tracked
target x-distance comparison is strict rather than inclusive, and the managed-VM
damage-flash path preserves the target's non-defensive pointer behavior after a
failed VM resolution. Readability-only static helpers that VC7 emitted as extra
COMDATs were removed and their target-inline logic was folded back into the
runtime owner.

## Compiler and target-bound diagnostic

The final maintained source passes the pinned compiler under both:

- `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`
- the same profile plus `/GL`

Both `src/Enemy.cpp` and `src/Player.cpp` pass normal and `/GL` focused compiles.
The final normal `/Gy` symbol is
`?EnemyRuntimeUpdate@@YGHPAUEnemyRuntimeView@@@Z`; its true owned `.text` section
is `0x935` = 2,357 bytes. The reviewed target owner is 2,412 bytes.

The relocation-aware standalone normal diagnostic over the full target extent is:

- result: `mismatch`
- matched comparable bytes: 134
- comparable bytes: 2,160
- object relocations: 63
- `acceptance_authority=none`

Fixed normal COFF is therefore demonstrably non-exact. `/GL` compilation passes
and preserves the target Rich-record LTCG/interprocedural hypothesis, but there
is no target-bound linked-image extent Oracle for this candidate, so `/GL`
exactness remains unknown. No canonical exact match or match unit is added.

## Ledger and tracked source state

The reviewed candidate `0x0040DC80` is promoted from `unknown/review` to
`authored_game` / source-present with descriptive name `EnemyRuntimeUpdate`.
`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` are not
changed by this packet.

Current reconstruction status after the packet:

- candidates: 1,249
- origin/boundary pending: 1,144
- authored: 97
- authored bytes: 33,494
- excluded: 8
- source-present: 70
- exact: 0

Tracked packet files are `src/Enemy.cpp`, `src/Enemy.hpp`, `src/Player.cpp`,
`src/Player.hpp`, `config/functions.csv`, `config/function-origins.csv`,
`config/implemented.csv`, `config/reccmp-functions.csv`,
`docs/KNOWLEDGE_BASE.md`, `docs/PROGRESS.md`, `docs/RE_HANDOFF.md`, and
`resources/progress.svg`.

Durable knowledge added in this packet is `ENEMY-001`, `ENEMY-002`, and
`TOOLCHAIN-024`.

## Adjacent-game hypothesis use

Adjacent source was consulted only after the TH10-local object/update evidence
was established.

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean at observation.
  Committed `src/EnemyManagerUpdate.cpp` and `src/EnemyManager.hpp` were read only
  for broad source-family/naming hypotheses.
- TH09: HEAD `3a08724b6b321596905ad083cd72cf0751b09989`, clean at observation, main
  ahead 9. No TH09 fact was promoted to TH10.
- TH095: HEAD `923b0f50ac57195c28c21c1024703f5841a7031d`, main ahead 9, with unrelated
  modified `src/FrontEndController.cpp` and unrelated untracked
  `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`,
  and `scripts/runtime-diff.py`. None of that dirty content was read or used.

TH08 only corroborates the broad enemy-update/damage source family. No adjacent
address, layout, ABI, owner, exactness, or completion state is transferred.

## Verification planes

Source presence: reviewed and present for `0x0040DC80-0x0040E5EB`.

Exactness: not established. Fixed normal COFF mismatches; `/GL` exactness remains
unknown. Canonical exact count remains zero.

Whole build: `python3 scripts/build.py` was exercised after the source change and
returned RC 2 / explicitly open because production flags, TU partition,
libraries, resources, and link order remain unknown. `scripts/build.py --check`
passes the intentionally open graph. This is not a source/CI failure and is not
product closure.

Runtime validation: not performed.

Factory Truth: a fresh read-only accepted snapshot succeeded at sequence 0 with
`accepted_count=0`, empty accepted facts, and empty evidence manifest. No
acceptance submission or replay was performed, so this packet has no Factory
accepted fact claim.

Public/focused verification: target verification, toolchain declaration check,
tracking validation, reconstruction status, public CI, zero-unit match-graph
check, open build-graph check, focused normal/LTCG compiles, diff whitespace
check, and fresh selected-target Ghidra attestation all pass before checkpoint.

## Analysis scratch

Entry `.analysis/` inventory was 127 regular files / 1,092,618 regular bytes /
1,436 KiB allocated, with zero files over 64 MiB.

Immediately before checkpoint finalization it is 134 regular files / 1,146,973
regular bytes / 1,504 KiB allocated, again with zero files over 64 MiB. The
current campaign `.analysis/gpt-web/20260914-hostile-runtime/` contains seven
files / 54,355 regular bytes. It retains the final Enemy normal and LTCG probes,
final target diagnostic, whole-build stdout/stderr, and manifest. Current-session
Player normal/LTCG header-sanity probes and an obsolete baseline comparator were
deleted because they are reproducible and no longer referenced. No legacy,
unknown, provider, toolchain, target, Wine-prefix, IDB, or other process output
was deleted.

## Next connected hard frontier

Continue with the connected Enemy lifecycle cohort surrounding this runtime tail:

- `0x0040CFB0`, 501 bytes: spawn/allocation path
- `0x0040D750`, 166 bytes: manager update/caller path
- `0x0040D830`, 673 bytes: constructor/initialization owner
- `0x0040DAE0`, 386 bytes: destructor/teardown owner
- `0x0040E5F0`, 162 bytes: death/finalization owner

Fresh Ghidra function queries confirm those candidate extents, for a combined
1,888 target bytes. Any decompiler calling conventions remain provisional until
raw entry/call/exit reconciliation. Treat the cohort as one lifetime/ownership
packet because it constrains the newly recovered `0x2518` full object,
`+0x103C` runtime tail, allocation, rollback, teardown, and death transitions.
Do not split it merely to obtain easy function-count gains.

The much larger unresolved `0x0040E770` candidate (13,759 bytes) remains a later
hard runtime/dispatcher frontier. The recent scheduling balance remains
hard-frontier oriented: 1,435-byte Player initialization, 911-byte lifecycle
cohort, 1,591-byte movement/options owner, 1,170-byte Player damage owner, and
this 2,412-byte hostile runtime owner. The next 1,888-byte lifecycle cohort is
selected for structural connectivity, not minimum size.

The exact-reconstruction phase is not complete. The operator, not GPT-web,
decides later phase completion after denominator and whole-product audit.
