# Parent-only invocation conditions

Do not execute while manifest.execution_ready is false. Transfer only the reviewed measure.py and final manifest.json, verify their exact SHA-256 hashes, and use a fresh packet directory. No artifact copies, builds, Wine tools, source overlays, trace files, or native content exports are needed.

The parent must use one 60-second outer wall-time window with forced cleanup on timeout, explicit PYTHONPATH pointing to the existing pinned .tools/capstone installation, and normal Python3 (optimized Python is rejected). The script itself sets one-CPU affinity, niceness +10 and CPU limit30 seconds. It has no child processes other than bounded existing read-only git metadata checks inside the frozen producer postcheck. Capture at most20KiB stdout; the script independently enforces this output bound. Do not print unfiltered intermediate results or tracebacks.

Arguments: --repo (the absolute current TH10 repository root), --expected-script-sha256 (final reviewed measure.py hash), --expected-manifest-sha256 (final ready manifest.json hash).

The repo must still be clean at e534a6f6c77115017e613721353308a41cde9b6a and the baseline run must remain .analysis/anm-execute-baseline-20261018-a001. The wrapper verifies source/tool/target inventory, frozen producer, artifacts and objects before and after. The source remains the unsigned-byte-formal baseline; the int-formal experiment is parked and excluded.

Only JSON count/hash/boolean and opcode-keyed size rows may be returned. Failure returns only phase/error class. A successful report is a physical-span-size diagnostic, not whole-owner equality or project credit. If any gate fails, stop and inspect safe metadata; do not broaden inputs or rebuild automatically.

Final reviewed script SHA-256: 7ab9d1ede4535db90f0d4a936d3e80ac9d9da1f26f2dbd4118df0829f1a4744e
Final ready manifest SHA-256: 90c3504556907f92e39debdcd78e5c3358732f43ef3aaac4032c82f96999ca1c

Proposed fresh packet directory: .analysis/anm-execute-size-diagnostic-packet-20261018-v1

After remote upload and exact hash verification, from the TH10 repository root, under a60-second parent command window:

PYTHONPATH="$PWD/.tools/capstone" timeout --signal=TERM --kill-after=2s 58s python3 .analysis/anm-execute-size-diagnostic-packet-20261018-v1/measure.py --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --expected-script-sha256 7ab9d1ede4535db90f0d4a936d3e80ac9d9da1f26f2dbd4118df0829f1a4744e --expected-manifest-sha256 90c3504556907f92e39debdcd78e5c3358732f43ef3aaac4032c82f96999ca1c
