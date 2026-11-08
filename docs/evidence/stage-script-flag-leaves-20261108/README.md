# TH10 StageScript flag leaves

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Two small leaf bodies operate on the already recovered `StageScriptView::flags`
word. The target instructions are direct and complete:

- `0x00408840`: `and dword ptr [eax+0x2a18],0xfffffffe; ret` (clear bit 0)
- `0x00408850`: `or dword ptr [eax+0x2a18],1; ret` (set bit 0)

`StageScript.hpp` declares `unsigned int flags` at that offset and
`StageScriptLayoutChecks` independently asserts `offsetof(StageScriptView,
flags) == 0x2a18`. The maintained leaf bodies now use
`StageScriptView *` and `stage->flags` rather than an untyped byte pointer.

The target boundary ledger reports no callers or xrefs for these leaf entries.
The type is therefore a layout-compatible project view supported by the
existing stage structure, not proof of the original class owner, original
source names, or authorship. The new names explicitly describe only the
observed bit-0 operations.

Focused canonical replay:

| Unit | Exact bytes |
| --- | ---: |
| `game-decomphelp-fulltu-00408840` | 8 / 8 |
| `game-decomphelp-fulltu-00408850` | 8 / 8 |

No full cold replay, whole-image claim, or runtime claim is made.
