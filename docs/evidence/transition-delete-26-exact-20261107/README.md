# TransitionDelete: original-bound 26-byte static callback exact

The independent original Japanese TH10 1.00a callback
`TransitionDelete @ 0x0043C870..0x0043C889` owns **26 bytes**.
Target identity: SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Its dense native code stores the original ECX argument in ESI, checks for
NULL, invokes the *real* `TransitionEffectView::Cleanup`, calls the ordinary
CRT `operator delete`, and returns 0. The existing natural C++ body lives in
`src/TransitionEffect.cpp` as a static `__fastcall` scheduler callback. It
forwards to the natural `DestroyTransitionEffect` helper; no new ABI shim,
register forcing, target-byte transplant or fake return path was necessary.

## Full linked static-owner binding

The existing native `GuiTransitionEffectFactory` graph compiles `Gui.cpp`
with thirteen real support translation units and the pinned MSVC 7.1 SP1
`/GL` profile. The original-bound callback is present as an *internal*
static symbol `?TransitionDelete@@YIHPAX@Z` in `support-09-TransitionEffect.ltcg.obj`.
Public-only symbol enumeration does not identify it as a separate function.
The verified `owner_visibility = "static"` locator instead resolves exactly
one same-start original-source MAP symbol and **one complete 26-byte PDB
section contribution**, without modifying the function's source linkage.
The original reviewed boundary ends in `RET` at `+0x19`, with no overlap.

Complete original-target linkage manifest (the only two fields):

| Offset | Kind | Actual linked source symbol | Independently reviewed target |
| --- | --- | --- | --- |
| `+0x09` | `REL32` | `?Cleanup@TransitionEffectView@@QAEXXZ` | `0x0043CB00` |
| `+0x0F` | `REL32` | `??3@YAXPAX@Z` | `0x004524A1` |

All **26/26 original bytes** compare exact after resolving these two
*real, independently target-bound calls*. Local conditional branch target
and original ESI/ECX register transport are checked without masking. The
separate already-accepted `TransitionEffectView::Initialize` original owner
points to this static callback at its `+0x10E` function-address field;
that caller binding was also strictly replayed. The callback's original
source authorship is still **indeterminate**: an exact native function can
be compiler-generated or authored. The origin ledger remains `unknown`.

## Two independent current-source colds

The maintained current source, not a historical cached image, was compiled
from scratch twice, with the *selected* original-bound tests only:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit transition-delete --unit transition-effect-initialize --json
```

| Cold | Full callback | Existing initializer guard | PDB GUID, age 1 |
| --- | --- | --- | --- |
| First | **26/26**, 2/2 fields | **396/396**, 37 fields | `38d3b1fd8d84e94c9cb7a63bcb8e7e4f` |
| Second | **26/26**, 2/2 fields | **396/396**, 37 fields | `8b6c7cf1b8e63347a2f96cd5b399ae0d` |

Both independently built PDBs record the complete callback and pass
**422/422** original-bound bytes in their shared selected graph. The
callback adds **one canonical exact owner / 26 bytes**, but **zero confirmed
original-authored bytes** until its provenance is separately proven.

Source input snapshot: `src/TransitionEffect.cpp`
`d114571cc8111ba3fa36be41ce51321e9a3747871bf34aaa9ba60002a22e315f`;
`src/Gui.cpp` `375d5b39c583fce37b4a5f29b25f28de7d5f7400940ef580e8fd47e7650efc09`.
Local original-bound cold receipts:
`.analysis/gpt-web/20261107-transition-delete-cold-{1,2}.json`, with native
`build/match-linked/GuiTransitionEffectFactory/` PE, MAP and PDB.
These prove neither the full executable, physical original translation-unit
partition, runtime behavior, nor portability.
