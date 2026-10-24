# WinMain lifecycle recovery, 2026-10-24

## Current result

Four separately measured models after **152492a** leave WinMain **1421 versus
1389 bytes, nonexact**: 152 fields, 72 calls and 45 local branches. These are
complete-owner extents and inventory counts, not matching/mismatching-byte
counts. This batch adds **zero exact bytes and zero source owners**; tracking
metrics are unchanged. All 112 target-independent CI stages passed for this source, with exit 0 and
frozen inputs preserved, verified at 21:18:54 UTC. The separate automatic
boundary-ledger divergence remains unresolved. Only CI-result documentation
changed after that run.

Current `src/Main.cpp` SHA-256:
`af0bc3965b9b5a22c642d8dfa99f3e1884c3dc1504c87031a9ffb2ef6bc11507`.
Current image SHA-256:
`21310a5263f7753e7d1ae8964cb0457c8cd4abeaf329bfb991e06cfd0fffb37e`.
[Machine-readable summary](summary.json) records every source transition,
image identity and receipt hash without copying binaries or disassembly.

## Individually measured models

All four use the retained ten-TU LTCG graph and preserve the prior logging work.

| Model | Bytes / fields / calls / locals | Tested observation |
| --- | --- | --- |
| Post-Reset device recheck | 1420 / 155 / 72 / 46 | Fresh device load and null test guard Release and the device-null store. |
| Instance publication | 1420 / 155 / 72 / 46 | Supervisor HINSTANCE publication follows successful startup; the rejection path bypasses it. GameWindow publication is unchanged. |
| Registry startup | 1435 / 154 / 72 / 45 | Existing POD uses actual operator new; nonnull storage is fully zeroed over 0xA004 bytes, including a retained redundant flag-zero store, before publication. Null allocation skips initialization and publishes null. |
| Registry teardown | 1421 / 152 / 72 / 45 | Capture the registry at teardown, retain owner/flag/entry null gates and all 0x2800 slots, call real free for entries and scalar delete for the container, with no final global clear. |

The teardown native runner exits **0**. Independent read-only replay also
**passes**, preserving all **116 original artifacts**. Its receipt is
`.analysis/gpt-dots/winmain-registry-teardown-20261024/independent-review/receipt.json`,
SHA-256 `0f7b5fbc8626681a97fcf1c59ab910684978b60c7250a3209ae8ffdb0dcd9d95`.

All four validated artifacts preserve nine complete guard owners totaling
**1758 bytes / all 94 fields**, plus Log, Fatal and WriteFile totaling
**620 bytes / all 43 fields**. Latest replay retains startup, publication,
device, logging and teardown checks together. These same-image checks preserve
existing contracts; they grant no new parent credit or other-graph recertification.

## Original failures remain distinct

- Instance trial: native compilation succeeded, but the original audit exited
  **1** on two semantically different stores at overlapping diagnostic anchors.
  Separate `supplemental-v2/receipt.json` passes using HINSTANCE stack/value
  provenance and full CFG ordering, not a blanket alias normalization. Original
  receipt, exit and log remain unchanged.
- Registry startup: the first **exit 1** was a preflight-only refusal, with no
  compiler run. The separately logged first native attempt also exited **1**:
  an inherited device audit lost a zero fact across an identity LEA. Separate
  `supplemental-v3/receipt.json` passes on that same image by preserving only
  full-width `LEA r,[r]` identity. No native rebuild or source change was needed
  for this supplemental proof; neither original failure was overwritten.

Startup controls cover both allocation outcomes and reject wrong allocator,
short zeroing, out-of-bounds flag store and inverted-null models. Teardown
controls reject swapped free/delete identities, truncated traversal and inverted
entry-null branching. Its five decoded scenarios include null owner, empty
flag, all-null, all-nonnull and alternating entries. Entry frees deliberately
poison the global and clobber volatile registers, testing the captured pointer
rather than assuming global invariance. This is static instruction-model/ABI
evidence, not Windows runtime validation.

## Evidence and next hypothesis

Ignored packets below `.analysis/gpt-dots/`, not supplied by a Git clone:

- `winmain-device-recheck-20261024/`: `trial-receipt.json`, full inventory and
  device role proof.
- `winmain-instance-publication-20261024/`: original receipt/log/exit and
  `supplemental-v2/receipt.json` with `alias-provenance.json`.
- `winmain-registry-startup-20261024/`: `root-preflight-recovery.json`, original
  `trial-root.exit`, native `trial-root-v2.exit`, `trial-receipt.json`,
  `supplemental-v3/receipt.json` and startup controls.
- `winmain-registry-teardown-20261024/`: `trial-receipt.json`,
  `trial-teardown-proof.json`, `audit-tests.json` and independent-review receipt.

The **music null-gate model is pending and untried**. Preserve each distinct
baseline and test one coherent falsifiable model with complete-owner and callee
gates. Original class/constructor/method names, delete versus delete[] spelling,
production TU and shared-global/runtime closure remain unproven. Do not infer
those identities from code equality or overlapping diagnostic data anchors.
