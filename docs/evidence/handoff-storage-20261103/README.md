# TH10 handoff storage cleanup — 2026-11-03

**Completed, not merely planned:** removed **543 ignored, regenerable files**
with **15,661,348 bytes (14.94 MiB) of payload** and **16,596,992 bytes
(15.83 MiB) of allocated disk blocks**. This was a bounded local cleanup
in the live TH10 worktree after the accepted GDI font cleanup commit.
It did **not** change version-controlled game source, the original target,
any canonical match-unit producer, toolchain, reverse-engineering project,
private archive or D3D adapter evidence.

## Narrow deletion scope

Only generated PE/OBJ/PDB files (`*.exe`, `*.obj`, `*.pdb`) were eligible
inside *superseded* ignored `build/gpt-web/` experiment directories, and
only `.pyc`/`.pyo` bytecode inside separately verified cache roots
(`build/pycache`, `scripts/__pycache__`, small gpt-web analysis caches and
two old evidence/probe caches). Every candidate was checked for Git-ignore
status, regular-file type, size and SHA-256 **before unlinking**. No source
snapshot, C/C++ input, MAP file, linker response file, retained experiment
log, source receipt or evidence archive was removed. No whole directory was
blindly pruned. Blank cache directories were removed only after verifying
their contents were exclusively bytecode.

The audited [tracked summary](cleanup-summary.json) contains the exact
count/bytes per deleted root, the before/after retained-tree and target
fingerprints, and the SHA-256 of the private compressed per-file inventory.
The complete ignored inventory and executable cleanup guard remain in
`.analysis/maintenance-20261103/`, with each deleted path, length, allocated
size and source-file SHA-256. **This inventory is not a payload archive**.
Deleted native images and objects must be *regenerated* from the maintained
source and pinned VC7.1 toolchain, not extracted from that inventory. The
private inventory will not be present in a plain Git clone.

## Explicitly retained

All canonical `build/match-linked/` outputs, including new
`TextFontRelease`, `InputWinmmCapabilities`, and `Enemy`, remain in place.
The original read-only `resources/th10.exe` still has SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`.tools/`, `ghidra-project/`, `_reference/`, the `.analysis/archive/` original
payloads, all historical `build/gpt-6.1-sol/` contexts, the maintained
`.analysis/gpt-web/` experiment notes (except Python bytecode), and unrelated
D3D adapter evidence remain intact. Before/after contents and counts of all
eight protected root families were independently checked in the summary.

The **six historically retained nonexact witnesses** from November 2 still
exist under `build/gpt-web`: `anm-executor-current-20261102`,
`anm-executor-independent-20261031`,
`ecl-run-stack-home-20261102/len-volatile-x`,
`game-owner-create-init-20261102/current`,
`gui-draw-20261102/baseline`, and
`current-turn/sound-sample-captured-remaining`.
Five more unresolved larger-owner contexts were deliberately protected:
`game-cleanup-warm-20261102`, `popup-browser-context-20261102`,
`bomb-full-caller-20261102`, `bomb-laser-integrated-20261102`, and
`popup-item-same-owner-20261102`. They are not certified current-source
images: verify source/header pins before applying diagnostic scores.

## Reproduction and handoff caution

Use [RE_HANDOFF.md](../../RE_HANDOFF.md) and
[PROGRESS.md](../../PROGRESS.md) rather than stale experimental PE timestamps.
Regenerate one missing exact producer with its `config/match-units.toml`
unit, e.g. `scripts/repo-python scripts/build-match-unit.py --unit
input-winmm-joystick-capabilities`; run a *selected* cold replay if its
exactness is relevant. Reconstruct discarded near-match contexts from the
recorded `.analysis/gpt-web` source experiment and local `ltcg_link.py`
recipe only if the next experiment actually requires them. Do not run a
full historical cold replay to replace disposable caches.

After deletion, Git remained clean; the original target hash was rechecked
and the protected sources were verified unchanged. The exact bytes in this
report are storage metadata, **not progress or reconstructed-code credit**.

**Post-validation cache note:** executing the repository Python wrapper or
lightweight CI may regenerate ignored `build/pycache` and
`scripts/__pycache__` entries. Remove these regenerated **bytecode only**
after the final validation pass, using the same regular-file and Git-ignore
checks. These repeated cache deletions are not counted again in the 543-file
initial cleanup inventory or the 14.94 MiB payload tally.
