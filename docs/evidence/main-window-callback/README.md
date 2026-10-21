# Recover window callbacks and MIDI completion

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `aa02121`. Reviewed on 2026-10-21.

Two complete authored owners become exact: WindowProc4390E0/265 and the
retained ActivateWindow43A1B0/8 wrapper, totaling273 bytes/all17 fields.
MainConfigurationDialogProc43A3A0/233 gains maintained source, but its same-size
candidate is nonexact. Main now has source for all20 reviewed authored owners
(6825 bytes);14 are exact and six remain nonexact. Source closure is separate
from exact, original source ownership and native product closure.

WindowProc now calls the existing MidiOutputView::UnprepareHeader through the
actual music owner, replacing the guessed declaration-only music callback.
Target43AEB0 is that maintained79-byte callee. Compiling the real Midi.cpp in
the declared graph naturally recovers the caller's private EDX owner/ESI header
arguments. A source-only interface is insufficient evidence for that convention.
The complete callee and all three fields pass in both final images.

The other necessary change is one common final DefWindowProcA source return.
Each relevant message case breaks to it. VC7.1 clones that delegate into three
physical return paths while keeping all four callback parameters live in the
target registers. Explicit per-case returns in the same real four-TU graph
instead produce276 bytes and a different WM_ACTIVATEAPP tail. The older guessed
music interface produces280 bytes. LRESULT, the standard four-argument stdcall
signature, /GL /GS and real WinMain entry remain unchanged. Alternate int return,
callback entry, /Zi and /G6 or /G7 controls do not repair the per-case source;
/Os regresses. No fixed registers, assembly, padding or ABI fiction is added.

The natural static HWND wrapper is inlined for WM_LBUTTONDOWN; its retained
8-byte contribution receives HWND in EAX and calls SetForegroundWindow. The
raw target ends43A1B7, followed by eight CC bytes. It has no direct/literal
entry xref. Exact code generation does not identify its original class, return
spelling, source file or production compiler partition.

## Complete evidence

Two independent complete Main.cpp + GameErrorContext.cpp + FileSystem.cpp +
Midi.cpp /GL /GS cold links through genuine WinMain freeze the same14 recursive
project inputs. Every image strictly replays the new273/all17 plus seven prior
owners1351/all81: startup442, shortcut272, D3D45, Fatal201, Log201, file check111
and MIDI unprepare79. Total coverage is nine complete owners1624/all98 per image.
The [native receipt](native-summary.json) pins inputs, PE/MAP/PDB/objects,
complete replay hashes, field bindings and controls. The first final cold is
`maintained-final-cold1`; an earlier misleadingly named `maintained-cold1` is
explicitly a before-adoption baseline and is not counted.

The second cold canonically replays the two new units in one artifact group.
The affected preexisting CreateGameWindow341 unit also cold-replays in its
original separate context, because its WindowProc entry changed. Three units
and614 bytes pass across two groups. No complete historical replay is run.

Fresh direct local Ghidra review uses read-only/noanalysis, rollback, one CPU
and768 MiB. The disk target and complete inclusive mapped text401000..465A80
agree before/after:412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Temporary raw helper/dialog functions are rolled back. WindowProc ends4391E8
with RET16, followed by seven CC bytes. Its registration, four physical
arguments and cleanup reject the decompiler's provisional extra this parameter.
GameWindow4924F0+0C/+10 are DWORD active/inactive fields; supervisor491C28+13D
is the windowed byte, +3C0 music owner and +3CC flags. Nearby anchor labels
coinciding with field addresses do not establish identity. Wrong active-field
addend and wrong foreground IAT controls are rejected by the final Oracle.
TH09 source suggests the common delegate pattern; only TH10 target/compiler
results establish this bounded claim.

The dialog target handles WM_INITDIALOG by checking control202, and command
low words201/203 by copying that checkbox into supervisor options bit100 and
ending with IDYES6/IDNO7. Every path returns zero with RET16. Its candidate is
233 bytes but differs in register saves, epilogues and field positions; it
receives source-presence credit only. Its target ends43A488 before seven CC bytes.

## Scope and reproduction

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit main-activate-window --unit main-window-proc \
  --unit main-create-game-window --json
```

The existing startup and MIDI recipes keep their original contexts. Only the
two new units declare the complete four-TU graph. Diagnostic data-anchor images
are not runnable products. Windows product/runtime, original physical source
ownership, semantic and port gates remain open.

Main.hpp changes one of the large nine-TU graph's30 inputs. Only
EnemyEclDispatcher.cpp consumes it. A necessary primary-only recompile reuses
eight unchanged support objects after verifying their recursive dependency and
artifact hashes. The complete three large bodies and16-byte integer adapter
remain equal to the preceding native carrier after verified field rewrites;
21 complete prior guards retain2415 bytes/all52 fields. Current target diagnostics
remain ANM9964 (7478/7995 aligned,90/92 target-sized groups,gap2),
ECL7020/all189 (27 ordinary differences), and Enemy14468/14416
(40/108 target-sized groups,gap652,frame2C4,19 sentinels). All three remain
nonexact. This incremental dependency check is separate from both full colds.

Two source mappings are added without denominator, boundary or origin changes.
Canonical exact becomes1221; authored exact becomes380 functions/69136 bytes.
Final checks and attributable cleanup are in the [validation receipt](validation-summary.json).
Private controls and recovery are below
`.analysis/gpt-6.1-sol/main-window-callback-20261021/`.

All100 standard CI checks pass in one invocation, followed by final focused
checks. Cleanup removes20 inspected terminal native directories/5801419 bytes;
first actual final cold, canonical cold, MIDI failure and current large carrier
remain. Other private source controls are compacted into verified deltas.
