# TH10 reconstruction handoff

## Current phase

Bootstrap: the original Japanese target and private Ghidra project are
attested; the provisional function inventory is initialized; boundary and
origin review has not started. No authored source, exact function, closed
Windows i386 product, runtime semantic result, or port is claimed.

## Restart commands

```bash
git status --short --branch
git diff --stat
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

From GPT-web, discover `th10-ghidra`, call `check` with `{}`, and require
the registered target plus `factory-native-command` attestation before using
any semantic output.

## Verified baseline

- Exact target identity and PE/Rich layout are recorded in
  `config/target.toml`.
- The pinned version database identifies the target as TH10 v1.00a original.
- VC7.1-era build 6030 is observed in the Rich record stream; exact compiler
  binaries and detailed build-shape fields remain unknown.
- The shared Ghidra/JDK installation and game-specific project are private,
  ignored inputs; semantic analysis receives no exactness credit.
- Every imported function origin is unknown, every disposition is review, and
  the exact ledgers are empty.
- `config/build.toml` exists from day one and correctly reports an open graph.

## Next bounded work

Review high-information architecture roots before writing source: CRT entry,
main loop, supervisor chains, archive loading, ANM/ECL dispatch, player and
stage state, audio, rendering, and shutdown. Select work by references and
subsystem closure, not by choosing only small functions. Commit coherent local
checkpoints; GPT-web must not push.
