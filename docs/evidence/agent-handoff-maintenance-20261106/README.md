# Agent handoff maintenance — 2026-11-06

This is documentation/skill and ignored-artifact maintenance after source
checkpoint `f3a6d2f`. Game C++ source, ledgers, exact counts and comparison
infrastructure are unchanged. [Current handoff](../../RE_HANDOFF.md) is the
continuation entry; [storage/recovery](../../RE_ARTIFACTS.md) is the private
transfer entry. [cleanup-summary.json](cleanup-summary.json) records scope,
source epochs, preservation and storage measurements.

## Corrections

- README routes to current accepted full owners and the authoritative handoff.
- TOOLS uses local commands, routine toolchain declaration checks and explicit
  unit selectors; obsolete Factory routing and an unsupported target-copy
  provenance statement are removed.
- Oracle documentation allows the existing primary/support PDB owner routing.
- Reconstruction skill and the old method's execution note use `gpt-web:`.
  Adoption-era scores are explicitly historical. The skill validator passes.
- Handoff identifies the latest source checkpoint, the Init 9-byte frontier,
  parked controls and already-credited Save/Ending results. All 20 listed
  source hashes match the unchanged current files.
- Storage tables distinguish this event from the earlier Nov6 cleanup.

## Verified cleanup

Archived 731 ignored PE/OBJ/PDB files from 39 selected experiment directories,
**65,780,410 payload bytes**, in a real **14,075,697-byte** tar.zst.
All 731 archived member hashes/sizes pass; an isolated PDB restoration passes.
The archive hash is `574dcaa16c912444b241974f8b43593f3ef5ae2f8b181e6a86064b2ba70b44ec`. Native maps/logs/JSON, source
snapshots and drivers remain available. Python caches are separately discarded
as regenerable files, without pretending their hash inventory is an archive.

The original, Ghidra database, all canonical artifacts, existing archives,
selected unresolved witnesses and D3D evidence are among **3113 protected
files** whose hashes/sizes remain unchanged after native pruning. All 8 prior
archive-registry hashes also match. `.tools/`, references and private inputs
are outside the deletion scope. Ten selected current/first-exact experiment
graphs remain expanded alongside the other unresolved witnesses.

Net allocated reduction for `build` + `.analysis`: **63,229,952 bytes
(60.3 MiB)**, after accounting for the new archive and cleanup metadata.
The receipt states the measurement baseline. Private archive, manifests,
verification and cleanup drivers live under `.analysis/` and are not in Git.

No game rebuild, cold replay or Ghidra/toolchain execution is part of this
maintenance. The default seven lightweight repository checks, skill validation
and `git diff --check` pass. Unchanged progress was not regenerated.
