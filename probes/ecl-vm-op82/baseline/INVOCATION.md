# Parent-only reviewed one-shot baseline invocation

Transfer only run.py and manifest.proposed.json, byte-for-byte, to /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-packet-20261018-v2. Verify identities and ignored status first. Do not transfer fixture sources or run prepare_plan.py. Run directory /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-20261018-a002 must not exist; existing directory is a hard stop.

Ready runner SHA256: cb83691809cd57f4e6157987fc81dfd5194b5a426c6fa70cb018e4add3b07755
Ready manifest SHA256: 3756f6c56689cdba2ea9c66c598bacb98aadc8280ce3e2e3732a8416ff56081e
Reviewed NOT-READY manifest SHA256: 33ea23fa79f4eb7fd5594e100b061fc7704fdbde06128a22e549d475b17bd31f

Parent alone executes after final exact independent verification. Use normal Python, no -O, and a 240-second remote command window. Set decoder search path explicitly in the same shell so worker and comparator inherit it. Full component identity is now checked inside the worker before any cold link, then checked again by canonical replay.

cd /home/pentester/coding/codex_ida/th10-reconstruction/th10
export PYTHONPATH="$PWD/.tools/capstone"
timeout 240s python3 /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-packet-20261018-v2/run.py start --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --packet /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-packet-20261018-v2 --run /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-20261018-a002 --expected-runner-sha256 cb83691809cd57f4e6157987fc81dfd5194b5a426c6fa70cb018e4add3b07755 --expected-manifest-sha256 3756f6c56689cdba2ea9c66c598bacb98aadc8280ce3e2e3732a8416ff56081e

After any uncertain completion, timeout, or interruption, inspect only durable status; never rerun start or worker:

python3 /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-packet-20261018-v2/run.py status --repo /home/pentester/coding/codex_ida/th10-reconstruction/th10 --packet /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-packet-20261018-v2 --run /home/pentester/coding/codex_ida/th10-reconstruction/th10/.analysis/ecl-run-baseline-20261018-a002

Resource limits remain one CPU, nice increment10, CPU120s, worker wall150s, inherited unlimited Wine address space. One cold_link call. Only allowlisted status may leave remote. No payload export, overlays, candidate build, runtime execution or source changes. Owner extent equal to7020 is never byte-exactness evidence.
