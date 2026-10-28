# TH10 agent handoff — 2026-10-28

## Start here

Exact source baseline: **`fb521c1`**, complete Sound queue and streaming reset.
The subsequent handoff cleanup changes documentation/artifacts only. Source
matches that baseline; the unfinished experiment is preserved as a patch.

Read [AGENTS.md](../AGENTS.md), this handoff and the relevant source/ledger rows.
Consult [workflow](RE_WORKFLOW.md) and [Oracle policy](ORACLES.md) for the lane
being used. [Historical sessions](RE_HANDOFF_HISTORY_20261028.md) are reference
material, not a startup reading requirement.

Operator direction:

- Advance substantial functions and useful leaves; compare complete source early.
- Use local tools directly, including `scripts/ghidra.py`; no Factory provider.
- English local commits: `gpt-6.1-sol: ...`, specified author/coauthor in AGENTS.
  Save substantive progress and never push.
- Recover dirty work. Only replay new/affected exact units, not the whole cohort.
- Compile/link serially, one CPU at nice10, bounded with retained exit logs.
- No routine host failure/null/exception scenarios, sanitizer/leak sweeps or
  mutation campaigns. Default CI is seven lightweight repository checks.
- Keep complete extents, every field, fixed local targets, callee bindings and
  ABI strict. Size equality/near matches grant no partial exact credit.

Target: original Japanese TH10 v1.00a, ignored/read-only `resources/th10.exe`,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Use `scripts/repo-python`; verify the target before target work and attest local
Ghidra before using its observations. No `/mnt` search or target substitution.

## Current facts

Authoritative counts are generated in [PROGRESS.md](PROGRESS.md):
**80,328/275,467 reviewed authored owned bytes exact, 438 exact authored owners**;
647 authored source mappings. Total: 1807 candidates, 1149 mappings, 1298 exact.
The denominator remains provisional; product build/runtime closure is still open.

Latest [Sound proof](evidence/sound-queues-20261028/README.md):

| Complete owner | Bytes | Fields | Canonical unit |
| --- | ---: | ---: | --- |
| ProcessQueues `43DDF0` | 1452 | 43 | `sound-process-queues` |
| Streaming Reset `44DAD0` | 213 | 4 | `streaming-sound-reset` |

Two independent Sound+ZWave cold images pass 2192 bytes/all 60 fields, including
two old guards. The real Main 15-input context also passes both plus five old
guards: 2582 bytes/all 82 fields. It recompiled three affected inputs and reused
12 verified objects; this is not full-product or whole-Main exactness. The queue
owns 1419 code + 1 NOP + 32 table bytes, resolving the old adjacent-table
uncertainty. New SDK-family 19/52-byte candidates at `44D3E0`/`44D400` stay
indeterminate, unmapped and nonexact.

## Immediate continuation: Sound Initialize

Target **43CD30..43D041/786**, retained native 754; still nonexact.
[Pending experiment and patch](evidence/sound-initialize-pending-20261028/README.md)
preserve the one-line explicit `format.cbSize = 0` hypothesis. The working source
is clean at the proven baseline. Apply the patch deliberately before a new trial.

The original producer ended with exit 1 at a cache-runner KeyError for generated
`link-anchors.c`; it produced no linked comparison. It does not falsify the
source hypothesis. The private runner's known-map guard is repaired and syntax
checked, **not rerun**. Use a new variant name and keep the original failed log.

Active recipe/listings/logs:
`.analysis/gpt-6.1-sol/sound-initialize-20261028/`.
Its base receipt is `.analysis/gpt-6.1-sol/sound-queues-20261028/final-main-current.json`
and warm objects are `build/gpt-6.1-sol/sound-queues-20261028/final-main-current/`.
Only Sound.cpp may change when reusing all 14 other objects; header/other-input
changes need an affected rebuild. The real Main context supplies Log's actual
ECX ABI; the two-input anchor diagnostic cannot establish that ABI.

Target raw writes suggest the explicit cbSize zero; target cleanup is also a
cold tail after the success body. These are untested source models. Start with
one change and a complete comparison, not a register/flag sweep.

## Other open work

| Owner | Current native/target extent | State |
| --- | --- | --- |
| Sound PreloadBgm / LoadBgm | 369/369; 304/298 | Nonexact; parked scratch-register controls |
| Sound sample loader | 684/684 | Nonexact; size equality grants nothing |
| WinMain / Render | 1533/1389; 419/425 | Nonexact, real queue call now connected |
| Ending Run / outer destructor | 1564/1568; 362/362 | Nonexact; parked register/return-buffer panels |
| ANM manager constructor | 2305/2315 | Nonexact; counter/operator-view and neutral loop controls rejected |
| Large ANM/ECL interpreters | See exact-first method and evidence | Parked residuals; require a new premise |

See [durable knowledge](KNOWLEDGE_BASE.md) and
[exact-first method](TH10_EXACT_FIRST_METHOD_20261024.md) for established facts.
Do not turn old hypotheses into facts or replay parked spelling controls without
new target/callee/capture/context evidence. Selecting another substantial owner
is allowed when it has a concrete uncertainty and a complete comparison path.

## Artifacts and checkpoint

Current active artifacts and the verified historical archive are indexed in
`.analysis/README.md`; cleanup audit is `.analysis/handoff-20261028/`.
Canonical `build/match-linked`, `build/match` and `build/match-runtime` caches,
current Sound witnesses and warm Main inputs are retained. Obsolete host/probe
builds are removed. Old analysis is hash-verified before compression/removal;
extract only a needed historical subtree. Old scratch paths are not guaranteed
live. The private target, toolchain links, Ghidra database and unrelated D3D
adapter evidence are untouched.

Cleanup reduced `.analysis/` from 738 MiB to about 128 MiB and `build/` from
478 MiB to about 81 MiB after lightweight CI regenerated its Python cache.
The historical archive retains 26,144 leaves in about
117 MiB; every archived file was hash-verified before its working copy was
removed. The audit records all 198 removed build roots and protected-file checks.
The cleanup checkpoint passed all seven lightweight CI stages; no native build
or new exact comparison was run for this documentation/artifact batch.

For ordinary checkpoints run the affected native Oracle and lightweight
`scripts/repo-python scripts/ci.py`, then `git diff --check`. Regenerate progress
only when its ledger inputs change. Product, dual-runtime semantic and port
validation belong to later phases. Do not revive retired test requirements.
