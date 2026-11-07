# ANM ExecuteScript: opcode-91 argument lifetime structural controls

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Full original `AnmRenderManagerView::ExecuteScript @0x0043EE30` is
**9,964 bytes**. Source SHA-256 for this trial:
`339c95eba4063aab4a62da3d496cb07de2c6240197f6957761392579f93b435f`.

## Current-source five-TU original-bound baseline

The actual pinned VC7.1 SP1 `/GL /GS` compiler graph was rebuilt from
`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`,
and `AnmInt3Scale.cpp` (`/Ob1` on the last source), rather than trusting
historical artifacts. The fresh `AnmRgbInterpolation` PE/MAP/PDB contains
one complete 9,964-byte `ExecuteScript` function and all **350 independently
target-bound** code/data linkage fields (including local destinations,
tables and other original references). Its strict whole-function comparison
is **9,962/9,964** matching original bytes. The two differences are
`+0x113` and `+0x115`: original uses `MOV EDX,[EBP+0x20]` then `PUSH EDX`
for opcode 91 child render layer, while candidate uses ECX.

## Seven new natural C++ structural trials

Seven actual source changes to `ANM_OP_CREATE_CHILD_91` were separately
compiled with the same five-TU WPO context, reusing four verified support
objects only with fixed source and header pins. The complete PDB extent,
original instruction fields, code bytes and local table obligations were
checked after each link, not only the affected six instruction bytes.

| Natural source variant | Full original-bound comparison |
| --- | --- |
| Explicit branch for VM script-value resolution | **9,962/9,964** and 350 fields; unchanged |
| Explicit conditional operator for script-value resolution | **9,962/9,964** and 350 fields; unchanged |
| Local pointer to `renderLayer` field | **9,962/9,964** and 350 fields; unchanged |
| Aggregate initialized child script/layer pair | **9,962/9,964** and 350 fields; unchanged |
| Pointer to named child-argument aggregate | **9,960/9,964**, adds differences at `+0x2483`, `+0x248A` |
| Explicit layer value and saved animation file | **9,962/9,964** and 350 fields; unchanged |
| Typed inline aggregate forwarding helper | **10,044-byte** PDB owner; strict 9,964-byte boundary rejected |

The last source form's PDB body is explicitly measured as 0x273C bytes,
not accepted as a 9,964-byte owner. These experiments refute the simple
hypothesis that another local argument-record alias or named file lifetime
naturally selects EDX. They do **not** establish the original C++ source
spelling, register assignment cause, or unique optimizer transformation.

Every temporary edit was restored byte-for-byte; none produced an exact
improvement. The target remains **NONEXACT**, no canonical match count or
source codegen credit is granted. No full historical cold replay or product
runtime test was run.

Private trial program and per-variant actual PE/MAP/PDB receipts:
`.analysis/gpt-web/20261107-turn3/anm-child91-structure-probe.py`,
`anm-9964-baseline.json`, `anm91-structure-results.json`,
`build/gpt-web/20261107-turn3-anm91/{variant}/`;
base image `build/match-linked/AnmRgbInterpolation/`.
