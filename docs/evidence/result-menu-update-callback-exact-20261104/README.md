# TH10 ResultMenu update-chain callback: complete 10-byte canonical exact owner

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The independent CC-delimited **10-byte** callback `0x00422A90..0x00422A99`
is the updater actually installed by `ResultMenuView::Initialize` in
`src/ResultMenuLifecycle.cpp`. It receives the menu in ECX, saves ESI,
transfers the menu to ESI, calls the real `ResultMenuUpdate` at
**0x004223F0**, restores ESI and returns with plain `RET`. The physical
source-era authored/outlined distinction and production TU remain
**indeterminate**; its function and exact codegen are separate claims.

The maintained source body was *already present* as the natural C++
`int __fastcall ResultMenuUpdateCallback(ResultMenuView *owner)`, forwarding
`return ResultMenuUpdate(owner)`. It was wrongly absent from the function
source ledger. Previous isolated two-TU lifecycle WPO generated a distinct
10-byte callback but with a stack-parameter adapter (`PUSH ECX`, call,
`ADD ESP,4`, `RET`), which is not the original ESI receiver protocol.
The current exactness result is **not** a renamed older 10-byte size match.

## Real producer / target-bound acceptance

A fresh VC7.1 SP1 `/GL` link uses **primary `src/FrontEnd.cpp`**, real support
`src/ResultMenuLifecycle.cpp`, and actual entry
`ResultMenuSynchronizeScore`. This lets compiler WPO see the *existing*
206-byte `ResultMenuUpdate` callee and use its private ESI receiver naturally.
No inline assembly, ABI fake, reference instruction bytes or dummy caller is
used. The retained callback has its own **10-byte PDB section contribution**
inside the support TU and exact target instruction bytes except the one
legitimate `CALL` relocation:

| Original callback field | Kind | Independently bound target |
| --- | --- | --- |
| `+0x04` | `REL32` | `ResultMenuUpdate` `0x004223F0` |

All **6/6 ordinary bytes, one/one original fields and all 10/10 owner bytes**
pass canonical `compare-linked-function.py` with zero differences. The complete
uncropped 206-byte **callee in the very same selected cold graph** also passes
**206/206 bytes and all 16 target fields**, including five local/data
addresses and the original selector/jump-table contribution. In this new
compiled context Capstone covers the entire 206-byte span without an explicit
data-span declaration, and every byte is still checked; unlike a cropped
`RET` prefix, the terminal case selectors remain in the comparison.

Two separate **canonical selected cold replays**, without reusing the first
PE/PDB, both report `exact`:

| Cold | PE SHA-256 | PDB GUID |
| --- | --- | --- |
| 1 | `38df1eceb9d942ef476be5248ec8ab1344cc21ea6766466e7b102c5a73c43f03` | `dd51dcb377313b4b98b6b7fa0eceabd5` |
| 2 | `0baa821f5ab7cfc8196e89c0341eeba52129cf537e073825220e9d7d177e432c` | `cfb57802dbddfe49a69e614af95eee4b` |

PE hashes and PDB GUIDs both differ. The separate 206-byte parent remains an
already-accepted exact owner, not a second new credit. **Only the 10-byte
callback** adds one exact physical function in this checkpoint. This is
complete target-bound codegen proof, not a native runnable product, original
source authorship, runtime semantics or portability gate.

Reproduce only this new scoped owner (not a full cold cohort):

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit result-menu-update-callback-00422a90 --json
scripts/repo-python scripts/ci.py
```

Ignored native evidence under `.analysis/gpt-web/20261104-next/`:
`result-menu-callback-real-wpo.json`, `result-menu-callback-strict.json`,
`result-menu-update-callback-cold1.json`,
`result-menu-update-callback-cold2.json`, and
`result-menu-same-cold-parent.json`.
