# DirectInput initialization: complete 439-byte parent and 83-byte thread source

**Checkpoint:** 2026-11-02, Japanese TH10 v1.00a target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This is a substantial real-source reconstruction, **not** an exact-byte
admission. It keeps the previously unmapped 439-byte DirectInput setup owner
at `0x0043B8D0..0x0043BA86` and its actual input-thread entry at
`0x0043BA90..0x0043BAE2` independently mapped and executable within the
selected original-compiler source graph.

## Restored controller layout, calls and failure paths

`src/InputDirectInputInit.cpp` models the actual input supervisor layout:
DirectInput8 interface at +0x0C, keyboard and joystick devices at +0x10/+0x14,
HWND at +0x48, and `configuration.options`/bit8 at +0x150. The base of this
view is the maintained `g_MainSupervisorView` at original `0x00491C28`;
we do not create a second owner allocation. The body first resolves the
window's instance through imported `GetWindowLongA(..., -6)`, rejects the
configured disabled-input bit, and calls the **actual DirectInput8Create
import thunk** with revision 0x800 and its IID. It constructs a keyboard
through `IDirectInput8::CreateDevice`, sets its data format through device
vtable +0x2C, sets cooperative level 0x16 through slot +0x34, acquires the
keyboard, and registers the real **52-byte exact EnumDevices callback** at
`0x43BAF0`. If the callback creates a joystick, it installs the joystick
data format, cooperative level 0x0A, 44-byte capabilities buffer, and the
independently exact **98-byte EnumObjects axis callback** at `0x43BB30`.

Each target branch also has its own cleanup: failed DirectInput8Create resets
interface and logs; failed keyboard construction releases the interface;
failed data-format setup releases keyboard and interface; failed cooperative
level releases keyboard, invokes the observed controller-release helper and
logs. The success path logs keyboard and optional joystick initialization.
Opaque imported GUID, input-layout data and error-string globals remain
*unresolved-symbol anchors* in this source-only selected link; they are not
invented real addresses.

The thread callback `InputDeviceInitThread` clears bits 0x200 and 0x400 in
`g_FrontEndSupervisorFlags`, invokes the actual setup owner on the existing
supervisor, and computes the two availability flags with the target's XOR/AND
mask pattern. It returns the resulting flags word. Original source spellings
and physical TU are not known; this is a compiler-testable view of the
observed game behavior.

## Bounded codegen measurements and rejected controls

The three actual source files (`InputDirectInputInit.cpp`,
`InputDirectInputDeviceEnum.cpp`, and `InputDirectInputRange.cpp`) compile and
link with pinned **VC7.1 SP1 `/GL /GS /EHs`**. The joint graph reports:

| Complete original owner | Original | PDB-native | Verdict |
| --- | ---: | ---: | --- |
| `InputInitializeDevices` @0x43B8D0 | 439 | 419 | **NONEXACT** |
| `InputDeviceInitThread` @0x43BA90 | 83 | 85 | **NONEXACT** |
| `InputEnumPadDeviceCallback` @0x43BAF0 | 52 | 52 | complete bytes equal after three known field slots |
| `InputEnumAxisRangeCallback` @0x43BB30 | 98 | 98 | complete bytes equal after one known field slot |

The exact callbacks also have their **independent canonical two-cold
receipts** with strict original target identities in the neighboring
evidence directories. The combined graph was checked against the original
whole target extents; no short prefix or guessed padding was admitted.

An initializer compiled without the real thread caller produces 411 bytes.
Restoring the caller moves the function to 419 bytes, but the compiler still
uses **EBX** as a saved private receiver instead of the original **EAX**.
The thread has an extra saved/restored EBX pair (85 instead of 83). A trial
that reinterpreted the parent as a non-inlined C++ member and a separate
`__fastcall` trial both retained these length differences; neither is kept.
The source still differs in the original private error-logging call schedule
and error-path layout. The **29 parent field slots** and **7 thread field
slots** are diagnostics, **not** a strict original linkage contract for
these two NONEXACT owners. No match row or exact byte credit was added.

## Bounded recheck

The maintained files and one real native graph are pinned in
[native-summary.json](native-summary.json). Original source ancestry,
production complete linking, hardware behavior, security and portability are
not established. To revalidate the existing *exact* child owners without
running a historical all-unit cold replay:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-pad-device-enum-callback --unit input-axis-range-enum-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
