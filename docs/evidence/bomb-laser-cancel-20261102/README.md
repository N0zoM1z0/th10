# Bomb pattern cancellation graph: 76-byte Laser owner exact, 124-byte Bomb helper recovered

**Target:** Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `0x0041C800..0x0041C84B` laser-effect owner is now maintained as
`EnemyEffectWaitManager::ApplyPatternCancel` in `src/EnemyLaserPatternCancel.cpp`.
Its entire original **76-byte** PDB-contribution body is **RAW BYTE EQUAL**:
**76/76, zero unbound external fields, zero masks**. The original private
EAX manager / EBX center / RET 8 ABI emerges from natural C++ in its real
Bomb-cancel caller's VC7.1 LTCG context. No fake register declarations,
assembly patches, target-byte literals or padding were used.

## Actual semantics and codegen-critical read/write dependency

The manager first saves the linked-list head at offset `+0x18`, then copies the
caller-supplied three world-space float coordinates into the manager's cancel
center at `+0x440`. It walks live `EnemyEffectWaitNode` entries via `next +0x08`;
for every node whose `state +0x0C` is not one, it calls the real virtual
`ApplyPatternCancel` vtable slot `+0x1C`, passing center, radius and mode,
accumulates the return count, and returns that total (including zero for an
empty list). The previous immediate-center-copy source ordering compiled to
an 85-byte candidate, not the target's 76. Moving the actual `head` read
before the center copy restores the target's load/store dependency and full
instruction schedule. The plain single-source context also emitted 86 bytes
under the public C++ ABI: it is **not** an acceptable proof of original codegen.

The selected actual six-source cold graph contains
`ScriptViewerBombEmit.cpp` as the caller plus
`EnemyLaserPatternCancel.cpp`, `ScriptViewerBombCreate.cpp`, `Chain.cpp`,
`ScriptViewerLifecycle.cpp` and `AnmManager.cpp`, compiled with pinned
VC7.1 SP1 `/GL /GS /EHs`. Both independent target-selected colds produce
**76/76 exact** and distinct PE hashes/PDB GUIDs; the first also replays the
already-accepted **10/10** Bomb callback with its strict original REL32 target.
See [native-summary.json](native-summary.json) for exact byte and source hashes,
PDB extents and separate cold identifiers.

## The larger Bomb cancel parent remains genuinely nonexact

The original `0x00405AC0..0x00405B3B` function is **124 bytes**. A full
natural-source `ScriptViewerBombOwnerView::EmitAuxiliary` is now in
`src/ScriptViewerBombEmit.cpp`. It returns early when mode at `+0x28` is zero;
otherwise it reads spell flags and IDs from original global `0x004776F4`,
selects the spell-specific cancellation mode (IDs 93–96 when flag bit zero is
set, or ID 109), and passes the owner world-space center `+0x30` and
VM-derived radius bits `+0x3C` to the existing real Bullet Pattern Cancel
(`0x00408100`, manager global `0x004776F0`) and to the above Laser Pattern
Cancel (`0x0041C800`, laser manager global `0x0047781C`). The second call
reloads the spell state and reevaluates the inverse flag bit as in the target.

The actual selected six-source candidate for this **complete** helper is
**128/124 byte-length NONEXACT**, with six native linked field slots; the
standalone linked caller graph emitted 126. Differences include private
receiver/center register schedules and original condition-selection layout;
no exact or partial credit is given. Its `0x00405750` Bomb Update caller also
remains **220/223 NONEXACT** in that producer. An alternate fully linked
graph produced a target-sized parent but regressed the accepted callback's
codegen and had numerous ordinary differences; that experiment was rejected.
Neither 124-byte nor 223-byte parent is marked exact, and the physical
original translation units, source spellings and complete game runtime remain
unknown.

Only bounded verification was run; no historical all-unit cold replay:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-effect-wait-pattern-cancel --unit script-viewer-bomb-update-callback --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-effect-wait-pattern-cancel --json
scripts/repo-python scripts/ci.py
git diff --check
```
