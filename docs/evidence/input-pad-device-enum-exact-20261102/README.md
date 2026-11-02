# TH10 DirectInput EnumDevices callback: complete 52-byte exact owner

The Japanese TH10 v1.00a original at `0x0043BAF0..0x0043BB23` was **missing
from the target-function inventory**. The real `0x0043B8D0` DirectInput
initializer registers this address as an `EnumDevices` callback at
`0x0043BA19`. The complete body is a distinct 52-byte function ending in
`RET 8`; it is separated from the preceding 83-byte input-thread owner by
`CC` bytes `0x43BAE3..0x43BAEF` and from the independently exact 98-byte
`EnumObjects` range callback by `CC` bytes `0x43BB24..0x43BB2F`.
The boundary is now explicitly reviewed and tracked, not guessed from
source symbol alignment.

## Recovered device-creation behavior

The maintained implementation in `src/InputDirectInputDeviceEnum.cpp` takes
the DirectInput device-instance record and unused context as two stack
arguments. If `g_InputPadDevices[0]` at original **0x00491C3C** is already
set, it returns 0 to stop enumeration. Otherwise it reads the real
`g_InputDirectInput8` global at **0x00491C34** and calls its `CreateDevice`
virtual slot **+0x0C** with the incoming instance GUID at byte offset +4,
the exact output device slot, and a null outer object. It returns 1 if the
creation result is negative (continue enumeration), otherwise returns 0.
The source uses the existing `InputDeviceView` interface and an ordinary
view of DirectInput8's COM vtable, not a fabricated mock call or target
byte copy. This is a game-authored device selection policy, not an unrelated
compiler/runtime thunk.

## Complete native acceptance

A genuine VC7.1 SP1 `/GL /GS /EHs` source link emits a **52-byte unique PDB
contribution** identical to all 52 target bytes after strictly binding
exactly three original data fields: `+0x01`/`+0x17` to
`g_InputPadDevices @0x00491C3C`, and `+0x0E` to
`g_InputDirectInput8 @0x00491C34`. There are no unclassified differences or
unresolved target control-flow destinations. Two *independent* selected cold
links reproduce the exact function with distinct PE hashes and PDB GUIDs,
pinned in [native-summary.json](native-summary.json). The first selected
cold also validates the previously accepted **98/98 exact** range callback
at `0x0043BB30`, giving **150/150 exact** across the two callbacks without
a full historical cold replay.

The **439-byte** input-device initializer which registers both callbacks is
still not exact or source-mapped at this checkpoint. No original source
spelling, physical translation-unit ownership, product link, device runtime,
asset behavior or portability is inferred from the exact adapter bytes.

Reproduce these bounded claims:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-pad-device-enum-callback --unit input-axis-range-enum-callback --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-pad-device-enum-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
