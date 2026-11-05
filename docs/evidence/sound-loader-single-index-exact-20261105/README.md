# TH10 SoundDataLoaderThread: restore retail single-index register schedule

Target identity: original Japanese v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original owner `0x0043D080..0x0043D0EA` is a **complete 107-byte**
source-authored leaf, with plain `RET`; the CreateThread entry-point cast is
not a claim that the thread body takes an explicit WINAPI argument.

## Source and observed original schedule

The original uses a *single* `ESI` logical index: both the static 37-name
pointer table and the 37 output slots are addressed as `[esi*4 + base]`,
followed by `inc esi / cmp esi,37`. The previous maintained C++ produced a
112-byte linked PDB contribution with two induction variables: `EDI` as
logical slot and `ESI` as byte offset, plus a redundant extra register save.
The actual installed `SoundDataLoaderThread` source now binds one natural
`unsigned char **const slots = owner->soundFileData` alias before the loop,
then writes/tests `slots[i]`. This preserves publication of each allocation
(including NULL) before the failure test, the same path-name reload on failure,
37 entries, cancellation behavior, log call, and Sleep(1) loop. There is no
register-forcing ABI, synthetic padding, target-byte copy, or original name
claim. A normal mutable array pointer alias lets VC7.1 naturally choose the
single retail index schedule.

## Original-bound first native proof

A fresh real **5-translation-unit** VC7.1 SP1 `/GL` graph compiles the
current source with `Main.cpp`, `FileSystem.cpp`, `PbgArchive.cpp`, and
`GameErrorContext.cpp` under the existing Sound `EHsc` support profile.
The PDB independently identifies one complete `SoundDataLoaderThread`
contribution of **107 bytes**. The strict target comparison checks all 107
original bytes, including both exits, and **11/11 independently specified
real-world relocation/link fields**, rather than just masking candidate
field offsets. Field identities bind all three `g_MainSoundOwner +0x5224`
reads, the distinct `+0x5230` slot store, the two filename-table reads,
`FileSystem::OpenFile`, the Sleep import, diagnostic string, error-context
instance, and `GameErrorContext::Log`. **107/107 bytes and all 11 fields
compare exact, with no differences.**

Private proof: `.analysis/gpt-web/20261105-turn-sound/verify-strict.py`,
`build/gpt-web/20261105-turn-sound/indexed-slots/source.exe` with matching
MAP/PDB, and the source-pinned `trial-result.json`. The current maintained
`Sound.cpp` SHA-256 is
`63936e47ae782558d08c2b7cedcba749a63723d8236052c65bac53d3228cff57`.
This source checkpoint is pending independent cold-replay registration as a
canonical match unit; it does not claim any complete game product, runtime,
full-ABI restoration, or portability.
