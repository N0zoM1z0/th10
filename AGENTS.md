# TH10 reconstruction agent rules

This repository targets only the original Japanese TH10 version 1.00a
executable identified by SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Never substitute a localized, modified, Steam, trial, or earlier executable.

The canonical private game file is
`/home/pentester/coding/codex_ida/th10-reconstruction/th10/resources/th10.exe`
(`resources/th10.exe` from the repository root). It is a read-only, ignored
operator input copied into WSL from the Windows game directory; it must never be
committed. Repository tools use this path by default. Do not search `/mnt`, add
a Windows game-directory mount, or set `TH10_TARGET_PATH` during normal Factory
work. That environment variable is only an explicit local override.

## Mandatory session recovery

Before changing anything:

1. Read `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, and the
   relevant source or ledger rows.
2. Run `git status --short --branch`, inspect tracked and untracked work, and
   review the diff. A previous Web session may have disconnected while leaving
   valuable dirty work. Understand, recover, finish, or deliberately supersede
   it; do not silently work around it.
3. In the repository shell, use `scripts/repo-python` for repository Python
   commands. It selects an interpreter only after verifying the exact pinned
   Capstone decoder in `config/tools.lock.toml`; a Web shell may otherwise use
   `/usr/bin/python3` with Ubuntu's incompatible Capstone 4.0.2. Run
   `scripts/repo-python scripts/verify-target.py`,
   `scripts/repo-python scripts/verify-toolchain.py --execute`,
   `scripts/repo-python scripts/validate-tracking.py --require-target`, and
   `scripts/repo-python scripts/report-reconstruction-status.py` before
   target-dependent work. From GPT-web, attest Ghidra separately through the Factory-native
   `th10-ghidra` provider: discover its operation schemas, then call
   `check` and require a passed attestation for `target:th10-main` with
   `attestation.provider_transport=factory-native-command`. The preflight call's
   arguments are exactly `{}`.
4. Work on one bounded unit or one coherent infrastructure batch and leave a
   reviewable local checkpoint.

When available, also read the Factory guidance at
`/home/pentester/coding/codex_ida/touhou-reconstruction-factory/docs/ontology.md`,
`docs/semantic-reconstruction.md`, `docs/agent-autonomy.md`, and
`docs/worktree-recovery-and-analysis-artifacts.md` below that Factory root.
These explicit paths are authoritative even when a client does not inject a
plugin skill.

## Adjacent-game hypothesis sources

The Factory mounts three adjacent reconstructions read-only for TH10 work:

- `/home/pentester/coding/codex_ida/th08-reconstruction/th08`
- `/home/pentester/coding/codex_ida/th09-reconstruction/th09`
- `/home/pentester/coding/codex_ida/th095-reconstruction/th095`

Inspect their source, history, scripts, and game-local knowledge when a TH10
subsystem has a plausible analogue. They may suggest source shape, names, engine
structure, compiler idioms, and useful probes, but they are hypothesis material
only. TH095 is itself undergoing semantic reconstruction, so its semantic names,
owners, layouts, and abstractions are especially provisional. No adjacent
game's addresses, bytes, layouts, ownership, behavior, or exactness claims are
TH10 evidence. Accept every TH10 conclusion only from TH10-local target, Ghidra,
xref, ABI, compiler, exact-byte, build, or runtime evidence. When the games
differ or the comparison is inconclusive, keep the TH10 fact unknown.

## Evidence and state

- Keep target observations, Ghidra observations, compiler observations, runtime
  observations, cross-game corroboration, inference, and unknowns distinct.
- Ghidra names, types, boundaries, and decompilation are provisional semantic
  evidence. They do not establish authorship, object ownership, or exactness.
- Keep boundary review, origin, source presence, exact codegen, native build
  closure, runtime validation, and port status independent.
- Only a target-bound, complete, reproducible VC7.1 comparison may enter
  `config/matches.csv`. Near matches remain candidates.
- The target contains normal C, normal C++, and LTCG C++ build-6030 inputs.
  `scripts/compare-coff-function.py` proves only declared normal-COFF units.
  Never remove `/GL` merely to make an LTCG-owned function fit that Oracle;
  keep its exact state unknown until a linked-image extent workflow exists.
- Use natural source. Never copy target bytes, manufacture fake returns, add
  inert padding, lie about the ABI, or patch target code to force equality.
- Record durable TH10 facts in `docs/KNOWLEDGE_BASE.md`. Cross-game promotion
  belongs to a later Factory review and is not performed by a game session.

## Phase gates

The required order is exact reconstruction, faithful Windows i386 product
closure, semantic reconstruction with target-plus-native dual Oracles, and only
then portability. Do not treat exact units as proof that the executable builds
or runs. Audit data owners, static initialization, resources, library objects,
link order, and exercised runtime paths before opening the semantic gate.

## Autonomy and tools

Agents may use broad repository Bash and compose small Ghidra, compiler, Wine,
comparison, and Git tools as needed. The framework exists to improve feedback,
accuracy, and resumability, not to restrict competent investigation. Each
important operation must still fail closed on the wrong target.

The current Factory-native Ghidra provider is read-only. Never patch target
bytes. The private database is working state; mirror durable conclusions into
the repository.

## Checkpoints and artifacts

- Use concise, coherent local commits, with `gpt-web: ...` for Web-authored
  checkpoints. GPT-web may commit in this repository but must not push.
- Keep decompiler dumps, logs, experiments, screenshots, and generated reports
  below `.analysis/`; keep builds below `build/`, shared-tool links below
  `.tools/`, and the private database below `ghidra-project/`.
- Do not accumulate scratch indefinitely. At the end of a bounded unit, retain
  only evidence needed to reproduce an unresolved claim, turn reusable work
  into a script or durable note, and remove superseded disposable artifacts.
- Never commit the original executable, game archives/data, Ghidra databases,
  toolchains, generated decompiler text, credentials, or private endpoints.
- Finish with the focused Oracle, `scripts/repo-python scripts/verify-toolchain.py --check`,
  `scripts/repo-python scripts/validate-tracking.py`,
  `scripts/repo-python scripts/progress.py`, `scripts/repo-python scripts/ci.py`,
  and `git diff --check`.
