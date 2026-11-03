# TH10 startup version/archive loader @0x00420100 — full semantic source, NONEXACT

Original TH10 JP v1.00a function `0x00420100..0x004201AD` is **174 bytes**
and ends in `RET`. Its original x86 code, native target SHA-256 and Ghidra
view were independently checked rather than accepting an older inferred
source owner. It has no recorded explicit arguments and performs the startup
archive/version payload preparation, not a generic filesystem fallback.

The maintained `src/GameStartupVersion.cpp` now calls the *existing real*
`PbgArchive::Load` on the main archive at `0x00497990` with `th10.dat`
(`0x0046E000`). On archive load failure, it reports the original message
at `0x0046DFA0` through `GameErrorContext::Fatal` at `0x0044B8E0` and returns
-1. On success it formats `th10_%.4x%c.ver` with binary arguments 0x100
and ASCII 'a' into a local 128-byte buffer, passes the filename and an
out-size to the real `FileSystem::OpenFile` (`0x0044B360`) in mode zero,
then publishes the returned byte pointer at `0x0049238C` and size at
`0x00492388`. A nonnull result returns zero; a null result triggers the
separate fatal message at `0x0046DFC8` and returns -1. Native /GS stack
cookie checks protect both return paths. No invented disk retry, memcpy or
alternative header parsing was added.

The global string and state declarations are *linkable views*: original
physical data allocation/TU identity and original function/class spelling
remain unknown. This is a complete semantic source mapping, **not** exact
codegen or a runnable retail binary claim.

## Native compiler exploration

A pinned VC7.1 `/GL /GS` isolated source link gave **181/174** PDB bytes;
real `PbgArchive.cpp`, `FileSystem.cpp` and `GameErrorContext.cpp` support
initially gave **184/174**. The latter revealed a compiler-retained
redundant explicit null write in the error path absent from target. Removing
that redundant write, without changing any observable result, reduced the
full owner to **174/174**, 61/114 preliminary comparable ordinary bytes,
with 15 candidate fields. A natural one-error-label source variation yielded
173/174 and was discarded. Expanding the real source context with PbgFile
and normal-COFF ResFile at `/EHsc` maintained **174/174, 61/114**; no new
exactness or field binding was inferred. In particular, original PbgArchive
private call transport, /GS prologue scheduling and return-branch ordering
remain different despite the matching extent size.

Local diagnostic logs under `.analysis/gpt-web/20261103/startup-version/`
and `.analysis/gpt-web/20261103/startup-version-*.log` capture the actual
trials. `config/matches.csv` is **not** modified and no exact codegen credit
is claimed. Only relevant existing exact PbgArchive and error-log owners
are cold-replayed; no historical full replay is performed.
