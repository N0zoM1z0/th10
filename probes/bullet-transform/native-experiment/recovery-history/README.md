# A004 and R001 archival recipes

recipes.json stores hash-checked text splices and manifest deltas against the exact existing a003 runner/manifest. It reproduces A004 and R001 without duplicating the full runner or83-source inventory. The overlay comes from the now-reviewed production BulletTransform.cpp only if its raw hash still matches; --overlay can name a preserved matching source fixture later.

Offline reconstruction, from repository root:

    python3 probes/bullet-transform/native-experiment/recovery-history/reconstruct.py r001 --output /tmp/new-r001-archive

The output directory must not already exist. This writes three source/config files and never runs them. Historical execution_ready fields preserve original packet identity; approvals have already been consumed. Reconstruction gives no authority for a new build/relink or historical directory reuse. Original environment paths and HEAD pins intentionally remain historical.

Pure tests:

    python3 -m unittest discover -s probes/bullet-transform/native-experiment/recovery-history -p 'test_*.py'

See docs/BULLET_DIRECTION_CORRECTION.md and evidence for separate A004 failure and R001 recovery. The R001 recipe retains fail-closed complete attribution, immutable-input checks, exact canonical885 guard, zero compilation/cold_link and one linker invocation. Native artifacts, logs and argv are absent from this archive.

Production source attributes remain unchanged. Existing C++ checkout rules may yield CRLF; host checks canonicalize line endings independently. Historical native reconstruction requires exact raw LF overlay bytes. If the current checkout is CRLF, extract the integrated source Git blob to a temporary file and supply --overlay with that file. The reconstructor deliberately rejects an unpinned raw checkout rather than normalizing it silently.
