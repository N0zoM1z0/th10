# Archived Bullet transform native experiments

A002 baseline and A003 conditional source experiment used base commit e94559cc95d583b702660b72bbc605e5dfc5743a. A003 was not adopted: complete transform size increased from 1476 to 1596 versus target 1454. Both canonical normalized 885-byte guards passed. No transform exactness test or acceptance credit follows.

This directory is source-only reproducibility material, not permission to run another experiment. Historical execution_ready fields describe approvals already consumed by one-shot runs. Do not invoke start/worker or reuse historical run directories. Native execution and native artifact access require separately approved parent-controlled scope. The retained scripts contain original absolute tool/prefix paths; do not silently rewrite or repin them.

Pure local checks, from this directory:

    python3 check.py
    python3 a002/reconstruct_manifest.py > /tmp/bullet-a002-manifest.json

The last command reconstructs the exact historical manifest, verifying both input and output hashes before printing. It does no native work. A003's runner tests mock native tools; owner tests use synthetic objects. The helper collector tests use synthetic COFF records and explicitly preserve the E8-in-data limitation. Normal Python is required for the native runner; optimized Python is intentionally rejected there.

A003's original seven-file delivery manifest is preserved verbatim. Its source overlay is only an experiment fixture. A002 retains its exact runner and a compact delta from A003's manifest rather than another full copy of the 83-source inventory. The evidence baseline document references identical source, include-closure and canonical-unit fields in A003's manifest; all run/tool/artifact hashes remain explicit.

Existing probes/bullet-transform host tests remain a separate finite source-behavior suite. Do not infer complete target equivalence from these or the conditional child audit. See docs/BULLET_TRANSFORM_NATIVE_EXPERIMENTS.md for chronology, recovery limits and decision.

check.py reconstructs the hash-pinned A001 manifest in a temporary sibling layout for the original unchanged A003 input-identity test. It neither edits archived packets nor invokes native tools.
