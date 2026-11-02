# TH10 popup owner: full 134-byte constructor exact, real 0xB884 allocation

Japanese TH10 v1.00a target `0x0042B430..0x0042B4B5` is **134 fully decoded
bytes**. The complete newly source-mapped `ScriptViewerPopupOwnerView` constructor
in `src/ScriptViewerPopupCreate.cpp` emits **134/134 exact** in a real VC7.1
SP1 `/GL /GS /EHs` four-source graph (`ScriptViewerPopupCreate.cpp`,
`Chain.cpp`, `ScriptViewerLifecycle.cpp`, `AnmManager.cpp`). There is one
externally bound global address at **+0x7E**:
`?g_ScriptViewerPopups@@3PAXA` / original `0x00477840`. There are no skipped
ordinary bytes, unbound native relocations or size-only exactness claims.

The recovered owner has the actual **0xB884-byte** storage layout with
`AnmVmView` embedded at `+0x18`. The actual ANM constructor and nine embedded
timer/bitfield initializations are expanded by LTCG; these preceding stores
are **not** a superfluous source loop. The popup then clears ownership bit 0
across the target's `0x2D3` stride-`0x40` records starting at `+0x3F0`, clears
the complete object (`0x2E21` DWORDs), publishes bit 1 at `+0`, and sets
`g_ScriptViewerPopups`. The final zeroing deliberately overwrites earlier
member state, as the original executable does. Nothing was simplified away
as an inferred semantic repair.

## Complete-object native and scoped cold verification

The actual PDB contribution is exactly 134 bytes, attributed to
`src/ScriptViewerPopupCreate.cpp`. Both independent real-link selected colds
match all 134 target bytes and the one target-bound field. Their PE digests and
PDB GUIDs differ; see [native-summary.json](native-summary.json).
The first cold also builds the directly affected `ScriptViewerBombOwnerView`
destructor through its maintained two-source graph: **137/137 exact** with
all existing 15 target fields preserved. No full historical cold replay was
requested.

The corresponding other lifecycle owners are **not exact**: the new full
`ScriptViewerPopupOwnerView::Initialize` source is currently 141 native bytes
versus 145 original; the factory `GameCreateOwner42B660` has the correct
71-byte PDB extent, allocation size and failure cleanup path but differs in
21 ordinary instruction bytes after normalizing five native linked field
positions. This is neither a verified five-field target-binding proof nor
partial exact credit. The destructor at `0x0042B570` retains its earlier
separate nonexact status. The constructor's original authored vs synthesized
origin remains indeterminate, even though its codegen is fully recovered;
physical original TU, complete game link, runtime and portability remain open.

Reproduce the accepted bounded claim:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-popup-constructor --unit script-viewer-bomb-destructor --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-popup-constructor --json
scripts/repo-python scripts/ci.py
git diff --check
```

**Later 2026-11-02 extension:** The same source graph now also recovers the separate popup-number tick/update owner **134/134** and its **7/7** fastcall callback. These are independently matched code functions with seven new target bindings; see [the update proof](../script-viewer-popup-update-exact-20261102/README.md). This does not resolve the nonexact popup initializer, factory or 540-byte Draw.
