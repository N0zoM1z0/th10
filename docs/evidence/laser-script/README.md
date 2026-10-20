# Laser command parser

EnemyLaserLineView::RunCommands at41CA80 now has maintained source for its
complete **1356-byte authored owner**. It was admitted in the previous laser
checkpoint; this batch changes neither the denominator nor exact credit.
The parser remains **nonexact**. Production construction, data ownership,
runtime reachability and the Windows i386 product remain open.

## Target observations

The original Japanese TH10 v1.00a target is bound by SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Fresh local Ghidra runs read-only/noanalysis with768 MiB and one active CPU.
Disk and the full412289-byte mapped text are attested, temporary metadata is
rolled back, and all mapped bytes remain unchanged. Raw target instructions
independently verify the provisional decompilation and helper ABIs.

The physical owner remains1269 code-span bytes, including ten internal
alignment bytes omitted by Ghidra1259, plus3 tail alignment,20 pointer bytes
and64 selector bytes. The previous [boundary auditor](../laser-line/README.md)
retains the complete five-destination/64-selector review. Its target-only JSON
now omits historical source-absence counts, which are ledger state rather than
target observations. `scripts/report-laser-script.py` separately checks the
command fields, five timer calls, two sample-sound calls, pan/aim/polar/VM call
identities, live PC reload, sentinel constants and selected payload stores.

The commands begin at450 inside the existing type-0 request424. Eighteen
records have24-byte stride: four operand words, opcode at10 and gate at14.
The request retains its original108 raw words as a union alias; existing raw
copy callers and the1DC request size are unchanged. Pattern records similarly
retain opaque bytes while exposing eight integer/float words after their
14-byte timers. Native assertions and all five compiled TUs check the layout;
these views do not establish original source types or serialized C++ lifetimes.

The target uses a **signed PC upper bound only**, so negative PCs are not
rejected. Zero opcode stops without advancing. An active flag plus zero gate
stops before dispatch. Skip80000000 advances and continues within the same
frame. Unknown opcodes advance, ID1000000 preserves the full operand word,
and jump2000000 installs the requested PC without an extra increment.

| Opcode | Observed effect |
| --- | --- |
| 1 | Set flag1, reset record0 timer, clear its word at7C |
| 10 | Record1 acceleration, angle choice, timer and polar X/Y; optional sample sound |
| 20 | Record2 operands/timer; optional sample sound |
| 40 /80 /100 | Shared record3 operand0 word, growth value, timer and two duration words |
| 400 /800 /8000000 | Positive live command counter only; record4 value/state, decrement the actual operand |
| 4000 | Real embedded-VM InitializeForLoadedScript404F30 |
| 8000 | Record5 timer from integer operand2 |
| 10000 | Store3 to the existing unknown word atC |
| 20000 | Actual QueueSoundCue with operand2 and current line X |
| 100000 /200000 | Both set **record6**, not separate6/7 timers |
| 4000000 | Record8 two coordinates, timer and duration word |

Angle<=-990 keeps the line angle; angle>=990 uses the actual EnemyAngleFromPlayer
helper; intermediate/unordered values are literal. This corrects the reference
comment's unsupported threshold1. Zero aim delta returns the actual helper's
pi/2. The40/80/100 fallback is strict value>-999, otherwise current growSpeed48;
the400-family fallback requires ordered value>=0. NaN therefore takes different
paths in these two protocols. The raw operand/counter words remain word
reinterprets, not numeric float-to-int conversions.
The recovered0040 consumer identifies operand0 as a float angular delta;
meanings in the other motion slots remain provisional.

The script base array474170 is the same physical table read by the existing
Bullet transform caller40708C, so the maintained g_BulletSpriteScriptBase
declaration is reused. The resource is4776F0+3E0B50 and the VM is line+600.
After the actual initializer returns, the parser reloads current PC, advances,
then checks the next live gate/flags. Pure timers, polar conversion, aiming and
sound queueing have no invented callbacks.

## Compiler and host scope

The complete five-TU /GL /GS diagnostic uses LaserScript.cpp, AnmManager.cpp,
EnemyLaser.cpp, EnemyEclDispatcher.cpp and Enemy.cpp, with the actual virtual
RunCommands entry. All25 recursive project inputs are hash-frozen. Initial
cached-angle source emits1364/1356. Target-observed opcode/angle reads and the
conditional record4 store emit1368/1356 and are retained: native angle compares
now pop/reload the field as the target does. The complete parent is still
nonexact, including shared-tail scheduling and the VM initializer's private
receiver protocol; size proximity is not a matching percentage.

Existing SetCurrent57, aim67, polar30, sample-sound123 and cue149 are structurally
equal in this carrier; these are diagnostics with no new helper credit. VM
initialization emits200/202 and the caller uses a private EDX receiver where
the target uses ESI. No ABI coercion or profile/attribute/placement sweep is
used. A separate target-backed Draw pointer-lifetime control emits173/173 but
only86/133 comparable bytes; it is rejected and the earlier source restored.
Motion0010 receives a focused canonical cold replay because its shared header
changed. No full historical cohort replay is claimed.

`scripts/test-laser-script.py` executes the actual parser, canonical timer body,
EnemyAngleFromPlayer, both sound bodies and InitializeForLoadedScript wrapper.
Host adaptations omit i386 assertions and define constructor prefills. Live
typed Player/resource/line/sound objects are used. Polar math and lower VM
boundaries are roles. VM reset is a pure entry observation; callbacks occur
only at actual script binding/execution. Checks cover every opcode family, signed upper bound, gates,
finite skip/jump programs, mutation of the original command counter, timer
integer/float state and selected scale, negative sound IDs, primary resource/
script capture, and post-script live PC/gate sampling. ASan/UBSan/leak checks
pass with23 strict named mutants; crashes, compile errors and other assertion
labels do not qualify as rejection.

The tested domain has valid operand indices, live storage and finite command
programs. Negative PCs, infinite cycles, overflow, invalid pointers, original
C++ union/object lifetime, native x87 bits/exceptions and runtime reachability
remain outside this host proof. No native gameplay or portability claim follows.
All89 public CI checks and final focused target/tracking/toolchain/progress/
whitespace checks pass. About4.2 MB of superseded native controls and host
binaries are removed; current input-hashed parser/Motion carriers are retained.

The MIT [reference repo](https://github.com/levzzz5154/th10-decomp), pinned at
c6791d82dc9f02e55eca45a5314f900a85481efd under ignored _reference/th10-decomp,
supplied laser_script_run.cpp hypotheses. Its code placement, reconstruction-
only scheduler type and other compiler controls are not imported. The existing
TH10 BulletTransform.cpp supplies a local adjacent-protocol comparison; target
instructions decide the laser's distinct opcodes/fields. [MIT attribution](../../licenses/th10-decomp-MIT.txt)
is retained. Pre-existing D3D evidence remains untouched.

```sh
scripts/repo-python scripts/report-laser-line.py
scripts/repo-python scripts/report-laser-script.py
scripts/repo-python scripts/test-laser-script.py --sanitize
scripts/repo-python scripts/build-match-unit.py --unit laser-line-motion-0010
scripts/repo-python scripts/compare-linked-function.py --unit laser-line-motion-0010 --json
scripts/repo-python scripts/probe-ltcg-backlog.py --source src/LaserScript.cpp \
  --entry src/LaserScript.cpp=EnemyLaserLineView::RunCommands \
  --support src/LaserScript.cpp=src/AnmManager.cpp \
  --support src/LaserScript.cpp=src/EnemyLaser.cpp \
  --support src/LaserScript.cpp=src/EnemyEclDispatcher.cpp \
  --support src/LaserScript.cpp=src/Enemy.cpp --profile-flag /GS --json
```
