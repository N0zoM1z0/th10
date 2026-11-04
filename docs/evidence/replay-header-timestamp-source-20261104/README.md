# TH10 replay header timestamp and completion setup, full 36-byte source

Target is original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The standalone CC-delimited target `0x00429B30..0x00429B53` has a complete
**36-byte** body. There is no independently known incoming direct call,
original source spelling, or production TU, and the compiler-private native
arguments arrive in EDI (manager) and ESI (completion selector). Original
source author-versus-compiler provenance remains indeterminate.

Fresh direct native disassembly identifies a true `ReplayManager::replayData`
read at **+0x18**, then CRT `_time` called at `0x00452B71` with the
`ReplayDataHeader::timestamp` pointer at **replayData+0x0C**. Its return is
not used. The routine forms `completionSelector+7` and, when the selector is
zero, overwrites it with the existing `g_GameStartupMode` storage at
`0x00474C7C` (the same storage also used as ScriptViewer stage). It writes
that final int to `ReplayDataHeader::completionState` at **replayData+0x5C**
and returns zero. The function's own null-pointer and allocation/failure
behavior is not changed or guessed.

The maintained `src/ReplayHeaderStamp.cpp` now models the *whole* function
using the real existing `ReplayManager`/`ReplayDataHeader` types, compile-time
`offsetof` guards for both `+0x0C` and `+0x5C`, the genuine `_time` call,
existing global declaration, and native wrap-width selector addition.
No alternate data structure, copied target bytes, or fake private ABI shim
is introduced.

## Fresh native VC7.1 comparison: NONEXACT

The isolated canonical pinned VC7.1 SP1 `/GL` production-profile diagnostic
compiled and linked the natural function, retaining the real `_time` and
`g_GameStartupMode` external *symbol identities* (two code fields). The
ordinary C++ source ABI is not the original private EDI/ESI ABI. An initial
single-expression conditional produced a **56-byte PDB contribution**;
rewriting it as `state=selector+7; if(selector==0) state=global; store(state)`
produced a **46-byte PDB contribution**, preserving the single-store target
branch structure and semantic fields. **Target remains 36 bytes; no matching
extent, field-offset normalization or exactness credit is claimed.** The
potential compiler-private transport must be recovered from genuine callers
before exact admission; no invented EDI/ESI ABI was used to force bytes.

Repeat this noncanonical diagnostic with
`.analysis/gpt-web/20261104-next/replay-stamp-first.py`; its complete
PE/PDB and original comparison are under
`build/gpt-web/20261104-next-replay-stamp/`, with private logs
`replay-stamp-first.log` and `replay-stamp-common-store.log`.
`config/matches.csv` is deliberately unchanged. This commit advances source
presence only; neither whole-image product closure nor runtime or portability
follows.
