# Parent-only one-shot candidate invocation

Parent authorized exactly one isolated candidate run on 2026-10-18. Independent exact ready-identity approval is required before transfer/start. Production adoption remains unauthorized.

Transfer only run.py and manifest.proposed.json byte-for-byte to the remote packet directory below. Verify both returned SHA256 values before execution. No full source overlay is transferred; run.py materializes the two hash-pinned candidate files only in its copied ignored tree.

Proposed packet directory:
/home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-alpha-int-candidate-packet-20261018-v1

Proposed fresh run directory (must not exist; existing directory is a hard stop):
/home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-alpha-int-candidate-20261018-a001

Run from the TH10 repository root. Use one240-second remote command window and normal python3, not optimized Python. Set PYTHONPATH explicitly; no unpinned interpreter-selection wrapper is required. First verify production linked_image.py and tools.lock.toml against the manifest pins, then perform the pinned decoder preflight in the same shell/environment before start:

export PYTHONPATH="$PWD/.tools/capstone"
python3 -c 'import sys,tomllib; from pathlib import Path; sys.path.insert(0,str(Path.cwd()/"scripts")); from linked_image import verify_capstone; verify_capstone(tomllib.loads(Path("config/tools.lock.toml").read_text())["capstone"]); print("pinned_capstone_verified")'

Only proceed if this check succeeds. It checks the locked Capstone version and component hashes without printing payload. The same PYTHONPATH is inherited by worker and guard subprocesses; the pinned comparator verifies Capstone again. Then:

python3 .analysis/anm-alpha-int-candidate-packet-20261018-v1/run.py start --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --packet /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-alpha-int-candidate-packet-20261018-v1 --run /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-alpha-int-candidate-20261018-a001 --expected-runner-sha256 440f0071ab7324d9a788b2c31fafa7162e951f8377ca72801ea794c8e92d1ac7 --expected-manifest-sha256 550addeeb5dc774e923a8af9f86e967f66603e57ef8f7330b1f98341f063bd7f

All later checks use status, never start or worker:

python3 .analysis/anm-alpha-int-candidate-packet-20261018-v1/run.py status --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --packet /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-alpha-int-candidate-packet-20261018-v1 --run /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/anm-alpha-int-candidate-20261018-a001

The immutable baseline is complete ExecuteScript9948 and alpha128. Candidate measurements may differ. Do not alter graph/source/profile to recover any size. candidate_valid requires both complete unique owners and both full same-artifact guards; it grants no whole-owner exactness or adoption credit. On any failure, report and stop; no retry of start. No production adoption is authorized.
