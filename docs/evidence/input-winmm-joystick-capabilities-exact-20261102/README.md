# TH10 WinMM joystick capabilities probing: full 105-byte exact owner

Original Japanese TH10 v1.00a `0x0044A0C0..0x0044A128` is a **105-byte**
reviewed authored input helper, called from the game's input initialization.
The maintained `InputReadWinmmJoystickCapabilities` in
`src/InputWinmmJoystickCapabilities.cpp` now emits **105/105 full-owner
codegen exact** when its genuine logging implementation from
`src/GameErrorContext.cpp` participates in the pinned VC7.1 SP1
`/GL /GS /EHs` link. The unique PDB section contribution decodes all
105 bytes; all six external address roles are strictly bound to the
original executable. There is no target-byte copy, ad hoc padding or
unclassified relocation masking.

## Real behavior and the source-context correction

The routine creates a 52-byte `InputJoystickInfoView` (`JOYINFOEX`) on the
stack and initializes only `size=0x34` and `flags=0xFF`. It calls the actual
`joyGetPosEx` import for device **0**, then device **1** if the first call
fails. If both fail it logs the original diagnostic message and returns
**1 in AX**. On either successful query it calls the real
`joyGetDevCapsA(0, g_InputJoystickCaps, 0x194)` and returns **0 in AX**.
The return type is deliberately `unsigned short`, matching the two-byte
EAX writes in the original body; the upper half is not invented or cleared.

A narrow source-only experiment without the actual log callee initially
produced a complete **105-byte candidate with only two ordinary differences**:
it pushed the logger object pointer, then invoked a distinct unresolved
variadic call. The original, however, has one pushed format argument and a
private **ECX** logger receiver. Adding the *real maintained*
`GameErrorContext.cpp` support to the same LTCG graph (without changing
the game's input code) naturally restores exactly the target
`MOV ECX,0x474F70; CALL GameErrorContext::Log` form and yields 105/105.
The original C++ declaration and physical TU remain unproven; this is
verified codegen and actual input functionality in the selected context.

## Six strict fields and independent local cold replays

| Original offset | Strict target identity | Target address |
| --- | --- | --- |
| +0x06 | WINMM `joyGetPosEx` IAT | `0x00466280` |
| +0x35 | failure diagnostic text | `0x00470580` |
| +0x3A | `th10::g_GameErrorContext` | `0x00474F70` |
| +0x3F | actual `GameErrorContext::Log` | `0x0044B810` |
| +0x55 | `g_InputJoystickCaps` | `0x004918B8` |
| +0x5D | WINMM `joyGetDevCapsA` IAT | `0x0046627C` |

The canonical `input-winmm-joystick-capabilities` unit was built and
strictly compared against all 105 original bytes. **Two independently
selected cold links** each produce 105/105 exact; their PE SHA-256 and
PDB GUIDs are different. The first cold also rebuilds the existing
`input-controller-buttons` source owner **264/264**, preserving its 12
strict external fields, for a selected total of **369/369 bytes**.
The independent receipts and original body hash are retained in
[native-summary.json](native-summary.json). No full historical cold replay,
whole-game link, hardware runtime, assets or portability test was run.

To reproduce just the selected native units:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-winmm-joystick-capabilities --unit input-controller-buttons --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-winmm-joystick-capabilities --json
scripts/repo-python scripts/ci.py
git diff --check
```
