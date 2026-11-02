# TH10 DirectInput object-axis calibration callback: 98/98 exact

The original Japanese TH10 v1.00a routine at `0x0043BB30..0x0043BB91`
(**98 bytes**) is registered by the larger input-device initializer at
`0x0043BA66`. The original `EnumObjects` indirect vtable call receives this
function pointer. Our maintained `InputEnumAxisRangeCallback` in
`src/InputDirectInputRange.cpp` is **98/98 full-PDB-owner exact**, with all 98
bytes decoded and its single data field `+0x17` strictly bound to the actual
`g_InputPadDevices` global at `0x00491C3C`. No unrelated byte masking,
synthetic padding, target code patch or forced register ABI was used.

## Real implementation and original interface

The callback takes a device-object record and unused user context as two
stack arguments, returns a boolean-style integer in EAX, and ends in `RET 8`.
It reads the object type at `+0x18`, skipping objects whose type does not
have either of its low two bits set. For selected objects, it constructs a
24-byte DirectInput range property with a 16-byte header: object type as
identifier, mode **2** (`DIPH_BYID`), minimum **-1000**, and maximum **1000**.
It calls `SetProperty` using the existing input pad device at the original
global and the real device virtual slot **+0x18**, passing property identity
**4** and the range by reference. A negative status returns 0; otherwise the
callback returns 1.

This function initially compiled to **100 bytes** because a provisional
source retrieved the `SetProperty` pointer into a local and performed an
extra register-indirect call. Modelling the *actual* vtable slot with its
function-pointer type naturally emits the original three-byte indirect
`CALL [ECX+0x18]`, restores the precise argument and local-field scheduling,
and reduces the entire body to **98 bytes raw-equal outside the one bound
original address field**. This source-model correction applies only to the
callback's own TU; existing `InputPolling.hpp` vtable layout and polling
logic are unchanged.

## Strict cold verification and limitations

Two independently selected, real VC7.1 SP1 `/GL /GS /EHs` colds reproduce all
98 bytes, with distinct PE SHA-256 and PDB GUID values (pinned in
[native-summary.json](native-summary.json)). The first selected replay also
rebuilds the already accepted `FrontEndGetControllerState` with its own
`InputPolling.cpp` profile and retains **264/264 exact bytes, all 12 source
fields**. Therefore the two distinct scoped producers contribute **362/362
verified bytes** without a historical all-unit cold replay.

The **439-byte** caller at `0x0043B8D0` remains source-absent / NONEXACT at
this checkpoint. It is independent from this exact callback and does not
acquire exact credit automatically. Original authored source spelling,
translation-unit placement, full game link, assets, device runtime and
portability are not established by a codegen comparison.

To reproduce the accepted bounded claims:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-axis-range-enum-callback --unit input-controller-buttons --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-axis-range-enum-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
