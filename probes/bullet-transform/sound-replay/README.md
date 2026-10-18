# Bounded sound-caller replay

This source-only package audits fixed sound-call contracts in TH10's 1454-byte
AdvanceTransformProgram owner. It contains no target bytes or translated trace.

Install the pinned official decoder with `python3 -m pip install -r requirements.txt`.
Run `python3 test_synthetic.py`. The authored fixtures exercise decoder effects,
field provenance, signed/index gates, affine record identity, entry witnesses,
initializer-required site assignment, and coverage bookkeeping. They are finite
adversarial controls, not an exhaustive theorem about every instruction encoding.

The standalone verifier accepts `--root REPOSITORY --target REPOSITORY/resources/th10.exe`.
Only the authorized parent environment should run it against the target. It checks
historical HEAD, source, target, ledger, helper and decoder pins, clean worktree,
owner bounds, exact table hashes and closed CFG before publishing bounded results.
It refuses Python optimization. No native build is performed.

The pins intentionally describe the audited checkpoint. A later production change
requires a separately reviewed repin; do not silently rewrite historical evidence.
See AUDIT.md for results, premises and remaining obligations.
