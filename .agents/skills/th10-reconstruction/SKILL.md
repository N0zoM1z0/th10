---
name: th10-reconstruction
description: Reconstruct complete TH10 v1.00a owners with local Ghidra and pinned VC7.1 byte Oracles; prioritize exactness.
---

# TH10 reconstruction

Follow `AGENTS.md`, the current `docs/RE_HANDOFF.md` entry and relevant
source/ledger rows. Consult `docs/RE_WORKFLOW.md` and `docs/ORACLES.md` for the
active comparison lane. Recover dirty work first.

Use local tools directly, including read-only `scripts/ghidra.py` when needed;
no Factory transport or documentation prerequisite. Verify the target before
target work and attest Ghidra before using its observations. A maintenance-only
batch needs neither Ghidra nor a game rebuild.

Compare the first complete natural source early. Preserve full extents, all
fields, fixed local destinations, real ABI and cold reproducibility before
claiming exactness. Keep origin/source/product/runtime/port status separate.

Do not add allocation-failure/null/exception host tests, lifecycle simulations,
sanitizer/leak runs or mutation campaigns for ordinary exact work. Use a small
probe only to resolve a concrete exact mismatch. Replay only affected exact
owners; run default lightweight CI, plus affected comparator self-tests when
changing the Oracle. Product and semantic runtime tests belong to later phases.

Keep scratch in `.analysis/`, accepted TH10 facts in the repository and a
coherent local checkpoint using `gpt-6.1-sol:`. Never push.
