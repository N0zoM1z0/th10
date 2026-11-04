# TH10 FrontEnd constant-interrupt adapters: two real-call-site source owners

Target: original Japanese TH10 v1.00a executable pinned by SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original has two distinct, CC-delimited **27-byte** functions:
`0x0042C730..0x0042C74A` (interrupt **2**) and
`0x0042C750..0x0042C76A` (interrupt **3**). They load the real VM ID
from `FrontEndControllerView::vmIds[index]` at controller +0x2C4,
then call `AnmRenderManagerView::SetVmPendingInterruptAndExecute`
at `0x00449250` using the actual ANM manager pointer at `0x00491C10`.
The original transports controller privately in EAX, index in ECX,
interrupt in EDI, and ANM manager in EDX. These registers are observations,
not an invented C++ ABI. Source-written versus optimizer-generated
ownership of the retained physical functions is still indeterminate.

## Independent target evidence and call-site recovery

After hash attesting the read-only local Ghidra project, its `xrefs_to`
operation established exactly **2** actual direct calls to `0x0042C730`
and **7** to `0x0042C750`:

| Native callee | Native call sites | Caller areas |
| --- | --- | --- |
| `0x0042C730`, interrupt 2 | `0x0042E531`, `0x0042E53D` | Options closing, VM slots 0x5A/0x5B |
| `0x0042C750`, interrupt 3 | `0x0042D5E7`, `0x0042D6E6`, `0x0042D6F2`, `0x0042DCA4`, `0x00430487`, `0x0043085B`, `0x00430DCC` | Main-menu return, options cancel, difficulty/character/shot selection |

Native disassembly at each caller independently confirms the live-in
controller/index pair before the actual call. This supersedes any
assumption that these adapters lack direct callers. A previous compiler
experiment routed **22** constant-interrupt calls through specialized
helpers, but the original call graph proves only these nine wrapper
calls. The current maintained source preserves other generic/inlined
calls and changes exactly **nine** verified semantic sites.

## Native source and comparison — deliberately NONEXACT

`src/FrontEnd.cpp` now contains two natural, non-inlined,
constant-interrupt C++ functions:
`FrontEndInterruptVmNowTwo` and `FrontEndInterruptVmNowThree`.
Each uses the **existing** `FrontEndControllerView::vmIds` and actual ANM
manager member interface; no parallel storage, target-byte copying or
register-forcing function declaration was added. The maintained
`UpdateOptions`, `FrontEndUpdateMainMenuReturn`, `UpdateDifficulty`,
`UpdateCharacter`, and `UpdateShotType` paths call the appropriate
specialization only at the reviewed original direct-call locations.

A fresh pinned VC7.1 SP1 `/GL /GS` native graph rooted at the real
`UpdateOptions` with the existing seven source inputs retains two
**32-byte** internal functions in the linker map: source image VAs
`0x004014C0` and `0x004014E0`. Both are CC-delimited map entries.
This focused build's PDB does **not** expose separate contributions for
these static symbols; therefore we use a **diagnostic** map/Capstone
comparison, not a canonical PDB-owner proof. Each candidate is
**32 vs 27 target bytes**, with **2/22** ordinary comparable bytes and
two candidate fields (manager `DIR32` at +0x0B and callee `REL32`
at +0x1A). The original's private register scheduling and callee field
geometry are not recovered; neither target enters `config/matches.csv`.
The different 22-call trial did expose distinct 32-byte PDB contributions,
but its source graph was deliberately not adopted: it connected more
calls than the native target establishes.

## Affected accepted-unit cold regression

The selected canonical exact replay from the changed source passed the
**3,999/3,999-byte** `frontend-options-refresh` owner. A separate
selected replay passed `frontend-cursor-push` 66, `frontend-cursor-pop` 47,
`frontend-cursor-move` 128, `frontend-input-repeated` 33, and
`frontend-delete-vm` 83: **357/357**. Total **4,356/4,356** exact
existing bytes in the affected selected graphs. No full historical cohort
or whole-game product replay was run.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-options-refresh --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-cursor-push --unit frontend-cursor-pop \
  --unit frontend-cursor-move --unit frontend-input-repeated \
  --unit frontend-delete-vm --json
scripts/repo-python scripts/ci.py
```

Ignored evidence under `.analysis/gpt-web/20261104-next/` includes
`frontend-wrapper-xrefs.txt`, `front27-global-root-results.json`,
`front27-selected-options-guard.json`,
`front27-selected-cursor-guards.json`, and
`front27-selected-map-probe.json`. This source-presence work does not
prove original authored provenance, byte-exact owner codegen, runtime,
whole-product closure or portability.
