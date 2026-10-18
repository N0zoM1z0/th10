# Parent-only one-shot invocation

Transfer only run.py and manifest.proposed.json byte-for-byte to the remote packet directory below. Verify both returned SHA256 values before execution. No source overlay is transferred. Do not rerun prepare_plan.py: it deliberately regenerates a NOT-READY proposal.

Proposed packet directory:
/home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-execute-baseline-packet-20261018-v1

Proposed fresh run directory (must not exist; existing directory is a hard stop):
/home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-execute-baseline-20261018-a001

Run from the TH10 repository root. Use one240-second remote command window and normal python3, not optimized Python. Set PYTHONPATH explicitly; no unpinned interpreter-selection wrapper is required. First verify production linked_image.py and tools.lock.toml against the manifest pins, then perform the pinned decoder preflight in the same shell/environment before start:

export PYTHONPATH="$PWD/.tools/capstone"
python3 -c 'import sys,tomllib; from pathlib import Path; sys.path.insert(0,str(Path.cwd()/"scripts")); from linked_image import verify_capstone; verify_capstone(tomllib.loads(Path("config/tools.lock.toml").read_text())["capstone"]); print("pinned_capstone_verified")'

Only proceed if this check succeeds. It checks the locked Capstone version and component hashes without printing payload. The same PYTHONPATH is inherited by worker and guard subprocesses; the pinned comparator verifies Capstone again. Then:

python3 .analysis/anm-execute-baseline-packet-20261018-v1/run.py start --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --packet /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-execute-baseline-packet-20261018-v1 --run /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-execute-baseline-20261018-a001 --expected-runner-sha256 a59bfec32f6f52a5ac722f79ef05736b5ae36fd65f4ef7d95703d672f145cb63 --expected-manifest-sha256 9738c814281e531774a97d3700ed5bf865ce5cbe7cd87b210a4a3f7092f71f56

All later checks use status, never start or worker:

python3 .analysis/anm-execute-baseline-packet-20261018-v1/run.py status --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --packet /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-execute-baseline-packet-20261018-v1 --run /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-execute-baseline-20261018-a001

A fresh complete owner measurement need not equal the historical sizes9948 and128. If it differs, stop after the baseline/guards and reconcile context. Do not alter graph/source/profile to recover those sizes. No candidate, overlays, runtime execution or exactness credit is authorized by this freeze.
