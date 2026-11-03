# TH10 ECL enemy world-to-screen anchor @0x00418810 — 64-byte source, NONEXACT

Original Japanese TH10 1.00a, 64-byte function `0x00418810..0x0041884F`,
complete native RET and reviewed source-absent boundary. The function is
**really called twice** in the 14,416-byte `EnemyRuntimeView::DispatchEclInstruction`
at native `0x0040EE75` and `0x0040EF6B`. Its native register-private input
is EAX = `EnemyRuntimeView::worldMotion.position` (+0x2C), with no explicit
stack arguments, and its result in EAX points to shared storage 0x00497D80.

The original first reads the low byte of the state word at 0x00497D8C,
conditionally sets bit zero in that same word, then publishes three floats:
`source->x + 224.0f` to 0x00497D80, `source->y + 16.0f` to 0x00497D84,
and a raw Z copy to 0x00497D88. The two binary32 offset values are taken
from original `.rdata` at 0x00470B4C and 0x00470B48. Both call sites copy
these resulting coordinates into ANM VM position offsets at VM+0x340;
a separate flagged branch copies the original world position instead.
The state flag is **part of the function**, not a dead global write.

New `src/EclWorldAnchor.cpp` implements the complete typed source surface
as `PublishEclWorldAnchor`, using the existing 12-byte `AnmFloat3View`
layout and descriptive linkable global views. The static temporary
`EnemyGetAnmPosition` still present in `EnemyEclDispatcher.cpp` models
part of the same target behavior but **does not set the flag** and has
unproved physical data identity. The new exact-target-scoped implementation
preserves that missing observable operation. Its original C++ source name,
actual data owner and compiler-private function declaration are not proven.

## Full-extent compiler experiments

- Pinned VC7.1 `/GL` selected single-source PDB owner: **64 vs 64** bytes,
  **21/32** ordinary comparable bytes and 8 candidate address fields.
- Retaining the actual source X/Y input reads via two scoped const-volatile
  references matches the original `FLD input; FADD constant` order and
  improves the selected owner to **64/64 size, 26/32** ordinary bytes.
- A genuine **two-TU** experiment temporarily rewired all three maintained
  Enemy ECL source references from its old static helper to this new owner
  and compiled with the actual `EnemyEclDispatcher.cpp` WPO producer.
  It retained **64-byte size, 26/32 ordinary bytes and 8 fields**;
  six register-encoding bytes remain different (native EAX input/ECX flag
  versus candidate ECX input/EAX flag). This is a compiler ABI/context
  difference, not permission to mask unreviewed bytes. The temporary
  changes to `EnemyEclDispatcher.cpp` were restored byte-for-byte.

Private reproducible two-source experiment and exact mismatch inventory:
`.analysis/gpt-web/20261103/ecl-anchor-real-caller-experiment.py` and
`ecl-anchor-real-caller.json`. A full 64-byte match and eight **independently
target-bound** fields have **not** been proven. No canonical unit/matches
row or source-authorship inference follows; this is a **source-present
NONEXACT** recovery. Existing ANM/Stage neighbors get scoped cold replay
only; the 14,416-byte parent ECL dispatcher remains NONEXACT. No full cold
product or historical cohort replay is used.
