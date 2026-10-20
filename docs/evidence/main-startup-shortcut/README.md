# Reconstruct shortcut resolution and the startup check

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `97e1d5f`. Reviewed on 2026-10-21.

Two complete authored owners become exact: CheckForRunningGameInstance439FF0/442
and ResolveShortcut43A290/272, totaling714 bytes. The resolver previously had
only a declaration. Its maintained SDK-typed implementation initializes COM,
creates ShellLink, queries IPersistFile, converts/loads the pathname, obtains
the destination and releases its temporary storage/interfaces on the observed
paths. Null destination returns before COM initialization. The target calls
operator new452493 and release4524A1; allocation/runtime behavior is not newly
validated by these codegen comparisons.

The startup caller uses the real GameErrorContext::Fatal receiver, keeps the
nonnull-title path first, and records the path-difference flag as one byte at
GameWindow+28. Target492518 has one direct byte read and one direct byte write.
The remaining three bytes retain an explicitly unknown layout; the total58-byte
view, following2C field and field width/offset are asserted. Original bool/type,
identifiers and class ownership remain unknown.

The target resolver receives destination/capacity in private EBX/EDI and one
stack pathname, then returns with RET4. Ordinary static stdcall source generates
that convention naturally through the genuine startup caller. A fastcall
control produces the same body; this does not recover the original annotation.
The default cdecl control emits268 bytes and fails the complete272-byte contract.
No fixed register, noinline directive, assembly, manufactured return or code
padding is added.

## Complete evidence and context

Two independent complete Main.cpp + GameErrorContext.cpp + FileSystem.cpp cold
links use /GL /GS and the real WinMain entry. Both strictly reproduce the new
714 bytes/all38 independently bound fields. Four existing same-image guards
retain558 bytes/all40 fields: D3D helper45, Fatal201, Log201 and file check111.
Total coverage in each image is1272 owned bytes/all78 fields. Only714 receive
new credit. The second cold uses the two new canonical recipes in one build.

The [native receipt](native-summary.json) records all recursive project inputs,
PE/MAP/PDB/object identities, complete replay hashes and semantic bindings.
The three target GUID payloads independently identify CLSID_ShellLink,
IID_IShellLinkA and IID_IPersistFile; SDK declarations corroborate the names.
The four COM/conversion import slots are checked against the target PE table.
The window flag uses known GameWindow4924F0+28 and supervisor flags use
491C28+3CC. Coinciding nearby data-anchor names are not binding evidence.
A swapped GUID address and wrong window-field addend are both rejected.

Fresh direct local Ghidra review is read-only/noanalysis, with rollback,
one CPU and768 MiB. Disk identity and the complete inclusive mapped text
401000..465A80 agree before/after:412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The resolver's complete boundary is43A290..43A39F through RET4; the next entry
starts43A3A0. The startup extent439FF0..43A1A9 includes internal alignment;
43A1AA..43A1AF is padding. Its sole resolver call is43A0EE. Decompiler argument
recovery is provisional; raw instructions establish the private registers.
TH09/TH095 source supplied hypotheses only, never TH10 addresses or acceptance.

## Scope and reproduction

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit main-resolve-shortcut --unit main-check-running-instance --json
```

Only those two units are cold-replayed canonically. The preexisting D3D, log
and file-check canonical recipes remain unchanged. WinMain1311/1389,
InitD3DRendering1051/1159, checksum199/207 and real OpenFile297/360 in this
three-TU carrier remain nonexact; different declared contexts retain their
separate canonical claims. The diagnostic image still has data anchors and is
not a runnable product. Original physical TUs/profiles, Windows runtime/product,
semantic and port gates remain open. All three requested large interpreters
also remain nonexact. Their C++ sources are unchanged, while Main.hpp changes
one of the nine-TU graph's30 inputs. Only EnemyEclDispatcher.cpp consumes that
header. Recompiling that TU and hash-reusing the eight unaffected support objects
keeps the complete three large bodies equal to the preceding native carrier
after verified field rewrites. Twenty-one existing complete guards retain2415
bytes/all52 fields. Current measurements remain ANM9964 (7478/7995 aligned,
gap2), ECL7020/all189 (27 ordinary differences) and Enemy14468/14416 (gap652,
40/108 target-sized cases,2C4 frame,19 sentinels). This incremental diagnostic
is separate from the two independent complete startup colds.

One source mapping is added, without a boundary/origin/denominator change.
Canonical exact becomes1219; authored exact becomes378 functions/68863 bytes.
Main still has two source-absent reviewed owners, so module source closure is
not claimed. Final checks and owned-artifact cleanup are in the
[validation receipt](validation-summary.json). Private producer/control recovery
is below `.analysis/gpt-6.1-sol/main-shortcut-20261021/`.

All100 standard CI checks pass in one invocation. Final focused comparison,
toolchain, tracking, progress, documentation and whitespace checks pass.
Cleanup removes9 owned terminal native directories/2126392 bytes; the two final
cold carriers, cdecl failure and verified source/control deltas are retained.
No full historical replay is performed.
