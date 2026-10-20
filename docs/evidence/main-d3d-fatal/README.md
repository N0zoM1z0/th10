# D3D initialization uses the real Fatal context

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `e5e63aa`. Reviewed on 2026-10-21.

The complete retained GameWindowView::InitD3DInterface owner at439700/45 is
now exact. Main.cpp's declaration-only MainFatal call omitted the target's
error-context receiver. It now calls the maintained GameErrorContext::Fatal
method on g_GameErrorContext. Ordinary C++ generates the observed EDI receiver,
its save/restore, format argument and both return paths. No register assignment,
new ABI attribute, padding or assembly is added.

The existing Fatal body at44B8E0/201 is compiled from its real support TU.
Its complete same-image replay passes independently in both new images.
Other diagnostic anchors remain; these images are not runnable products.

## Complete replay and target review

Two independent complete Main.cpp + GameErrorContext.cpp cold builds use
`/GL /GS` and the genuine WinMain entry. All nine recursive project inputs
agree. Each image reproduces the new45-byte owner and all five fields, plus
the existing201-byte Fatal owner and all eleven fields:246 bytes/all16 total.
The second cold uses the new canonical recipe. Only45 bytes receive new
credit; the Fatal guard retains its existing credit and recipe.

The [native receipt](native-summary.json) pins source, objects, PE/MAP/PDB,
contribution identities and complete replay hashes. The old baseline emits38
bytes and fails the complete45-byte contract. A wrong supervisor addend also
fails before comparison.

One diagnostic anchor happens to share the address encoded by the supervisor
field. Its nearest name is g_MainWindowTitle, which is not binding evidence.
The canonical field instead uses the independently known supervisor base491C28
plus the asserted d3dInterface offset4, producing target491C2C. The format is
46F6A0, the real error context474F70 and the Fatal call44B8E0. The first call
reaches the reviewed Direct3DCreate9 import thunk452176/IAT4662B4.

Fresh direct local Ghidra review is read-only/noanalysis, with rollback,
one CPU and768 MiB. Disk identity and the complete inclusive mapped text
401000..465A80 agree before/after:412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra still has no function at439700. The independently reviewed raw extent
is45 bytes through RET43972C, followed by three CC bytes. Complete raw E8/E9
and literal-entry searches find no references to this retained helper.
WinMain contains the same D3D creation and receiver/format/Fatal sequence,
including its call at438BDB. These observations do not identify original
class names, physical TUs or production flags.

## Scope and reproduction

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit main-init-d3d-interface --json
```

The changed inlined parent remains nonexact: the actual two-TU WinMain grows
1298->1307 against target1389. No parent credit is claimed. Boundary, origin
and source-mapping denominators are unchanged. Canonical exact becomes1217;
authored exact becomes376 functions/68149 bytes.

The three large requested interpreters remain open. ECL operand, result-scope
and caller-context experiments are retained privately, with source restored.
A separate completed pinned-backend observation confirms actual stack-color
membership; it does not establish original source or exactness. No complete
historical replay is run. Only the new unit is cold-replayed canonically.
Final checks and cleanup are recorded in [validation](validation-summary.json).
All100 CI checks are covered across92 initially passed checks and eight resumed
checks after supplying the newly referenced validation file. The initial
documentation failure and both segments are recorded; no passed functional
check is rerun. Final focused replay, toolchain, tracking, progress, documentation
and whitespace checks pass. Cleanup removes17 terminal owned native directories,
4073413 bytes; source/control inputs and the selected proof carriers remain.
Private producers and recovery material are below
`.analysis/gpt-6.1-sol/main-d3d-fatal-context-20261021/`.
