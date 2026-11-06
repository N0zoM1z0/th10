# TH10 startup version loader — complete 174-byte exact owner

Original Japanese TH10 v1.00a: `GameLoadStartupVersion` at
`0x00420100..0x004201AD`, **174 bytes**, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The source in `src/GameStartupVersion.cpp` loads the main PBG archive, formats
the version filename using `0x100` and `'a'`, opens that file, publishes its
pointer and size, and returns zero on success or reports the appropriate
archive/version error and returns -1. The original has no disk fallback.

The complete source now follows the original control flow: archive success
contains the version-file work and an explicit null-failure branch; archive
failure follows the successful return. This recovers original `JE` at +0x26,
`JNE` at +0x62, the version-error argument at +0x64 and the shared Fatal tail
at +0x88. All actual failure paths and stack-cookie checks remain in the
complete comparison. There are no injected failure scenarios or fake ABI
annotations.

The six-source graph retains the real `ArchiveOrDiskPathView::Read` entry
from `FileSystem.cpp`, PBG archive/file support, `GameErrorContext.cpp`, and
normal-COFF `ResFile.cpp`. The pinned `/GL /GS /EHsc` profile and PBG `/Ob1`
support profile follow the existing PBG path recipe. That real context
recovers the original ECX archive receiver; the simpler four-source graph
had emitted 177 bytes with EBX and remains rejected.

The first final-source link used five source/object-hash-checked support
objects and freshly compiled the changed primary source. Its strict complete
comparison passed **174/174 bytes and 15/15 fields**. Every field destination
was independently checked in the original executable: archive/string/data
addresses, PBG Load, sprintf, OpenFile, Fatal, and both cookie checks. Every
ordinary byte and fixed local branch destination is compared as well.

A separate canonical cold command then recompiled **all six source inputs**
and independently reproduced **174/174 and 15/15**, zero differences:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-startup-version --json
```

Canonical output: `build/match-linked/GameStartupVersion/`. Full private
receipts and drivers: `.analysis/gpt-web/20261106-startup-version/`.
[The compact native record](native-result.json) pins both distinct images
and the installed source hashes. **New confirmed-authored exact credit:
one complete function / 174 bytes.** Other startup owners receive no credit
from this comparison; original TU/data ownership and product/runtime closure
remain independent. No historical cohort was replayed.
