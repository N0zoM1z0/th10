# Exact complete ANM capture rectangle (162 bytes)

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

**New confirmed-authored exact owner**:
`AnmRenderManagerView::RequestCaptureRectangle @0x00424480..0x00424521`
(162 bytes). Reconstructed source is installed in `src/FrontEnd.cpp`, with
the target's observed `AnmRenderManagerView` layout and the original
non-null assumptions. It receives a private ESI manager and an `AnmVmIdView`
plus four rectangle values on the stack, then returns with `RET 20`.
It resolves the VM through the *current global* ANM manager, converts the
sprite's dimensions/crop to `int` via actual `__ftol2`, rejects a capture
request if the cached receiver already has a pending capture surface, and
records the destination rectangle and source crop without changing the
presence checks, frame state or VM identity.

## Concrete source/codegen discovery

A current-source two-TU `FrontEnd.cpp` + `VmIdInterrupt.cpp` native link
first gave a **complete 162-byte PDB contribution**, six matching field
positions, but only **147/162** comparable original bytes. After testing
multiple *natural orders* for the ten independent member assignments,
writing the destination rectangle before the source crop moved the real
native candidate to **161/162**. The one remaining ordinary byte was at
`+0x06`: original `MOV EDX,[g_AnmRenderManagerView]`, candidate `MOV ECX`.

Adding actual `AnmManager.cpp` as a **third real translation unit** makes
`FindVm`'s *real private EDX-receiver implementation* visible to VC7.1 LTCG.
Without changes to calling conventions, source annotations or assembly, the
compiler now emits the target `MOV EDX` and the caller itself becomes
**162/162 exact**. The original order of the independent destination and
source field writes follows the same schedule. The actual `FindVm` callee
is also source present and matches its own **73/73** complete target bytes.
This is a real caller/callee ownership correction, not a one-byte patch.

## Complete original-bound field contract

The original function's six 32-bit addresses were separately decoded from
the original PE; the canonical unit rebinds them by type, identity and exact
location. The complete function body, branch destinations and final `RET 20`
are compared against target bytes, without partial-window masking:

| Original owner offset | Field | Original destination |
| --- | --- | --- |
| `+0x07` | `DIR32 g_AnmRenderManagerView` | `0x00491C10` |
| `+0x10` | `REL32 AnmRenderManagerView::FindVm` | `0x004491C0` |
| `+0x20`, `+0x2A`, `+0x36`, `+0x42` | Four individual `REL32 __ftol2` fields | `0x00463B2C` each |

**Two independent three-source cold builds** of the *installed, unchanged*
`FrontEnd.cpp` under the pinned VC7.1 SP1 `/TP /MT /O2 /Gy /GF /Oi
/DNDEBUG /Isrc /GL` profile and actual
`ResultMenuUpdateResult` entry both pass complete original-bound owner checks:

| Owner (same two fresh images) | Cold 1 | Cold 2 |
| --- | --- | --- |
| `RequestCaptureRectangle`, 6 fields | 162/162 | 162/162 |
| `ResultMenuClose`, 4 fields | 89/89 | 89/89 |
| `AnmRenderManagerView::FindVm`, 0 fields | 73/73 | 73/73 |

These zero-reuse images have **different native image SHA-256 hashes and
PDB GUIDs** (cold1 PE `83582cd6...`; cold2 PE `a2ed86a5...`). The same
source SHA-256 is `0bab7e7448c0762a1410d627301652bfc8cfe22e7aaa88b76e979151cdaaf8c6`.

Reproducible complete receipts, actual PE/MAP/PDB and source pins:
`build/gpt-web/20261106-chat-th10-capture/installed-cold1/proof.json`,
`installed-cold2/proof.json`, plus
`.analysis/gpt-web/20261106-chat-th10-capture/verify-installed.py`.
The new canonical linked-PE match unit is
`anm-request-capture-rectangle`, which has all six separately verified
original link fields and the real three-TU build context.

Only the new match and its affected previous exact owner are selected in
canonical cold replay (not the entire graph or historical cohort):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-request-capture-rectangle --unit result-menu-close --json
```

Result **exact 251/251** original bytes (162 new + 89 existing) in two
selected compiler graphs. Private final receipt:
`.analysis/gpt-web/20261106-chat-th10-capture/canonical-selected.json`.
This adds **one 162-byte confirmed-authored exact owner** to the canonical
ledger and reduces the confirmed-authored exact backlog by one. The larger
ANM executor still remains separately NONEXACT. Native whole-game executable,
actual game behavior, physical original TU/source spelling and portability
are independent open questions; this exact codegen does not establish them.
