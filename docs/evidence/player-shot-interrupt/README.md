# Player shot pending interrupt (2026-10-21)

`PlayerSetShotVmInterrupt2`, **0x0040C480–0x0040C4C0, 65 bytes**, is now
canonical exact. This adds one authored owner and65 exact bytes. Maintained
C++ is unchanged. The original three large functions remain nonexact.

Fresh local Ghidra runs read-only/noanalysis with one CPU and768 MiB, verifies
the original Japanese target and complete mapped text before/after review,
and rolls back its transaction. Eight incoming direct calls are observed.
Target code reads the ID through EAX, calls FindVm with manager in EDX and
one pushed ID, stores a16-bit pending interrupt2 at VM+304, and propagates it
through root children only. It neither clears the ID nor executes the script.
The complete reviewed boundary already existed; its origin/size are unchanged.
See [target facts](target-facts.json).

The current eleven-TU real-Player image first exposed a complete structural
match. The existing three-TU Player constructor recipe independently exposes
the same body, allowing the focused canonical recipe to reuse that smaller
context: Player.cpp, AnmManager.cpp, Gui.cpp, all `/GL`, existing `/O2` profile
without `/GS`, entry Player constructor. No profile/entry/calling-convention
change or artificial root is introduced. Original production ownership is
still unknown.

Two independent cold builds each recompile all three TUs from23 recursively
frozen CPP/quoted-header inputs. The strict linked comparator verifies the
complete PDB-owned65-byte contribution and both fields, DIR32+04 to491C10 and
REL32+0A to4491C0. All65 target-replayed bytes agree in both builds. The shared
existing PlayerResetRuntimeState guard also remains273/273 with all six fields.
Both native owner bodies reproduce byte-for-byte across the two builds.
Address anchors make these diagnostic images nonrunnable; exactness establishes
neither product closure nor runtime validation.

Focused reproduction:

```sh
nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit player-set-shot-vm-interrupt2
scripts/repo-python scripts/compare-linked-function.py --unit player-set-shot-vm-interrupt2 --json
scripts/repo-python scripts/compare-linked-function.py --unit player-reset-runtime-state --json
```

The same bounded investigation remeasures the adjacent ANM script helpers in
the unchanged real eleven-TU graph. The194-byte binder remains exact; the
initializing helper is197/203 and receives no ordinary-byte score. Script
restart is287 bytes with all four fields bound and265/287 replay bytes;22
ordinary differences chiefly reflect swapped script-index/script-pointer
registers and the resulting prologue schedule.

Three private one-TU controls test a guard before the script snapshot, existing
flip/scale bitfields, and a combined valid-path block. The first and third
retain the complete265/287 result. Bitfields remain287 bytes but move the
first address field, so strict comparison rejects field locations and awards
no byte score. Every control preserves the raw complete native bodies of all
three large functions (9964/7020/14464). None is adopted. A prepared fourth
local-scope source was never compiled and is not an executed result.

Only the new unit and the one existing unit sharing its rebuilt artifact are
replayed; no full historical replay is needed because production source is
unchanged. Two neutral native directories and the unexecuted source are
removed, freeing2,603,728 bytes. The first cold, final canonical image, selected
bitfield failure, frozen sources, producer hashes and reports remain private.
Independent pre-existing D3D evidence is untouched.

[Native proof](native-summary.json) pins both colds, all23 inputs, the unchanged
33-input large context and16 retained artifacts, controls and cleanup.
[Validation](validation-summary.json) records final focused and standard gates.
Private recovery: `.analysis/gpt-6.1-sol/anm-script-restart-20261021/RECOVERY.md`.
