# TH10 FrontEnd constant-2/3 VM interrupt adapters: source ownership, nonexact

**Superseded frontier (2026-11-04):** A later genuine VC7.1 `/GL /GS`
source correction to natural `(int index, FrontEndControllerView *controller)`
argument order and `AnmVmIdView` reference passes now reproduces **both
entire 27/27 target owners**, four address fields and two independent
canonical cold proofs. The earlier 32-byte codegen tests in this document
are retained as falsified historical controls, **not** the current status.
See [new exact evidence](../frontend-vm-interrupt-adapters-exact-20261104/README.md).

Target: pinned original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The separate 27-byte owners at **0x0042C730..0x0042C74A** and
**0x0042C750..0x0042C76A** are not generic unnamed leaves. Both load
`FrontEndControllerView::vmIds[index]` from the genuine base `+0x2C4`,
read the actual ANM render-manager pointer `0x00491C10`, and call
`AnmRenderManagerView::SetVmPendingInterruptAndExecute` at `0x00449250`
with fixed interrupt **2** and **3**, respectively. They return with
plain `RET`. Real native callers use **EAX=controller**, **ECX=index**,
with EDI preserved by these wrappers; the original source declaration and
production TU are **not** established by these private ABI observations.

## Independent direct original call sites

A fresh read-only, original-target-attested Ghidra `xrefs_to` query found:

| Target | Original call sites | FrontEnd owner |
| --- | --- | --- |
| `0x42C730` / interrupt 2 | `0x42E531`, `0x42E53D` | `FrontEndControllerView::UpdateOptions` |
| `0x42C750` / interrupt 3 | `0x42D5E7`, `0x42D6E6`, `0x42D6F2` | `FrontEndUpdateMainMenuReturn` |
| `0x42C750` / interrupt 3 | `0x42DCA4` | `FrontEndControllerView::UpdateOptions` |
| `0x42C750` / interrupt 3 | `0x430487`, `0x43085B`, `0x430DCC` | `UpdateDifficulty`, `UpdateCharacter`, `UpdateShotType` |

Those **2 + 7** original direct call sites have been mapped to separate
natural `FrontEndInterruptVmNowTwo/Three` source functions in the existing
`src/FrontEnd.cpp`. Other calls to `InterruptVmNow` remain generic rather
than being routed to a wrapper without target xref evidence. No extra data
object, register-forcing shim, duplicate ANM callee, or copied target bytes
were introduced. Both actual 27-byte target bodies were decoded and
compared against their source implementation, with original metadata
`config/function-origins.csv` retaining *indeterminate* ancestry.

## Genuine VC7.1 results and exactness limits

In a real VC7.1 SP1 `/GL /GS` `FrontEnd.cpp` plus seven support TUs,
`UpdateOptions` entry, the selected source compiles the two independent
helper definitions and retains linker-map code entries. Their generated
natural C++ bodies are **32 bytes each**, versus **27 target** each.
The **maintained nine-call** `UpdateOptions` source graph retains the
internal linker-map function entries `0x004014C0` and `0x004014E0`,
each ending at the next 32-byte-spaced map public. Its PDB does **not**
provide separate static-function contribution rows: the full 32-byte map
extent was examined diagnostically, not admitted as a canonical PDB owner.
Each candidate has **2/22** ordinary positional bytes agreeing, a real
manager `DIR32` candidate at `+0x0B` and real ANM callee `REL32` at
`+0x1A`; those positions/bytes are not the complete original field
geometry. A *rejected* 22-call control did expose distinct 32-byte PDB
contributions, but overconnected real caller paths and was restored.
The candidate's argument movement differs from the original EAX/ECX/EDI
private call convention, and its fields do **not** establish target exactness.
These are **source-present, NONEXACT**, and neither contributes to
`config/matches.csv`. The 3999-byte `RefreshOptionsDisplay` remains a distinct
existing exact owner; its match does not prove these helpers exact.

Selected canonical cold regressions with the *maintained changed source*
pass eight **distinct** existing exact owners, not two differently labeled
copies of a matching helper:

| Affected exact unit | Whole native bytes |
| --- | ---: |
| `frontend-options-refresh` | 3,999/3,999 |
| `frontend-set-screen-state`, `frontend-set-screen` | 80/80, 83/83 |
| `frontend-delete-vm` | 83/83 |
| `frontend-cursor-push`, `frontend-cursor-pop`, `frontend-cursor-move` | 66/66, 47/47, 128/128 |
| `frontend-input-repeated` | 33/33 |
| **Unique existing exact total** | **4,519/4,519** |

These results came from three selected scoped cold compiler graphs; the
separate 4,245-byte and 4,356-byte reports share owners and **must not**
be added together. All external bindings in these accepted units still
pass strict comparison. No all-units replay was run. Source coverage,
original caller xrefs, near-match helper codegen, and actual original ABI
remain independent proof categories.

Reproduce only the affected accepted units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-options-refresh --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-set-screen-state --unit frontend-set-screen \
  --unit frontend-delete-vm --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-cursor-push --unit frontend-cursor-pop \
  --unit frontend-cursor-move --unit frontend-input-repeated \
  --unit frontend-delete-vm --json
scripts/repo-python scripts/ci.py
```

Ignored receipts under `.analysis/gpt-web/20261104-next/`:
`frontend-wrapper-xrefs.txt`, `front27-global-root-results.json`,
`front27-selected-options-guard.json`, `front27-sibling-guards.json`,
`front27-selected-cursor-guards.json`, and `front27-selected-map-probe.json`.
This is complete semantic source recovery and direct-caller attribution,
**not** original authorship, exact codegen, runnable product, or runtime proof.
