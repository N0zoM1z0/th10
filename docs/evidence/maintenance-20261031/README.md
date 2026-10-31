# Handoff and storage cleanup — 2026-10-31

This maintenance follows the exact Hint frame checkpoint `9bd5b01`. Game source,
recipes and comparison infrastructure are unchanged by the cleanup. The handoff
is reduced from858 lines to a113-line resume brief; detailed
history remains in Git and the linked owner proofs.

Corrected current descriptions include the now-exact571-byte Hint frame,
already-exact3836-byte GUI updater, latest4472/4480 GUI interpreter snapshot,
the Caution-to-AddAutomaticEntry call edge, and stale Ending/ECL/GUI cache labels.
HINT_FRAME now describes maintained native source; the old explicit-double
model and host fixture/mutation reports are historical, not current gates.

## Private archive and reclaimed storage

The new private archive is
`.analysis/archive/terminal-experiments-20261031.tar.xz`. It contains5,370 files
from41 ended/superseded analysis/build roots:213,017,412 payload bytes compressed
to4,162,508 bytes. Every member's path, size and SHA-256 was checked against a
per-file inventory before each source was rehashed and the expanded copy
removed. Net payload reduction, including the compressed inventory, is
208,645,752 bytes (about199 MiB). Earlier cleanup savings are excluded.
Regenerable `build/pycache/` is removed after the final CI run; its separate
payload/allocation counters and the combined reduction are in the summary.

The detailed inventory is private:
`.analysis/maintenance-20261031/archive-inventory.jsonl.gz`. Its digest, archive
digest, root list and counts are in [cleanup-summary.json](cleanup-summary.json).
The compactor, terminal log, plan and full pin audit remain beside that inventory.
Use [RE_ARTIFACTS.md](../../RE_ARTIFACTS.md) for selective restoration.

Current Hint artifacts and nine other unresolved-witness campaigns remain
expanded. Canonical producer outputs, toolchain/Wine and reference checkouts
are preserved. All136 individually hashed target/database/D3D files remain
unchanged. No game binary, database, generated decompiler text, toolchain or
private archive is committed.

## Follow-up source-epoch audit and build pruning

After the later PreloadSurface and HintInitialize source commits, the original
pin audit is no longer a current-source statement. The retained Hint frame
context is 34/36, GUI draw 14/15, GUI Run 60/64, while the latest ANM
post-PreloadSurface context is 13/13. Hint frame/callback were independently
replayed twice against the current split source (581/581 both runs). See
RE_HANDOFF.md and RE_ARTIFACTS.md for the per-file mismatches and reuse rules.

The follow-up pruned **2,650 ignored, untracked files**
(115,052,465 payload bytes / 109.72 MiB; allocated
120,496,128 bytes / 114.91 MiB): current-turn
variant outputs, superseded FrontEnd permutations, old ASCII/seek scratch
outputs and regenerable Python bytecode. The final-capture-cold witness remains
expanded (11 files, 808,795 bytes). The .analysis scripts, receipts and source
snapshots remain. Canonical match-linked producers, retained research
campaigns, archives, target, Ghidra, toolchain and D3D inputs were preserved.
The exact per-root counts are in cleanup-summary.json and the private
maintenance inventory.

## Cache scope and validation

The [pin audit](context-audit.json) covers319 recognized pre-cleanup expanded
context manifests; only five match their recorded source/header pins. This is
not a test of every context schema or proof of current complete acceptance.
The ECL actual-caller context is 14/17 (AnmManager.hpp, Enemy.cpp and
GameScoreState.hpp changed); its pin-equal one-source entry is the wrong
graph/ABI. GUI Run is 60/64, GUI draw 14/15, cleanup-leaf 74/84,
message-pairs 78/84, and Ending final-source 57/70. These are historical
candidate images, not current-source reusable artifacts.

Pin equality alone never authorizes reuse: compare graph/source order, compile
and link profiles, actual object/image hashes and required ABI too. Restoring
an old witness and compiling current source are different experiments.

No game rebuild is needed for this metadata/storage batch. Default seven-stage
repository CI and whitespace checks cover the tracked changes. The preceding
source checkpoint's complete warm/cold and scoped581-byte frame/callback proof
remains the native acceptance evidence.
