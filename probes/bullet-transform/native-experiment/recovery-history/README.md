# A004, R001 and A005 archival recipes

recipes.json stores hash-checked runner/source text splices and manifest deltas against the exact existing A003 archived runner, manifest and overlay. Historical b635 direction-only source and72aa vector-corrected source are reconstructed from archived da1 source bytes. Current production source is never read, so later source corrections and normal CRLF checkouts cannot break old packet replay.

Offline reconstruction, from repository root:

    python3 probes/bullet-transform/native-experiment/recovery-history/reconstruct.py a005 --output /tmp/new-a005-archive

The output directory must not exist and must be outside the repository. This writes three files and never executes them. Historical execution_ready fields preserve original packet identity; approvals were consumed. Reconstruction gives no authority for another build or relink. Original paths, HEAD pins and A004/R001 packets remain exactly reproducible, without full source or runner copies.

Pure tests:

    python3 -m unittest discover -s probes/bullet-transform/native-experiment/recovery-history -p 'test_*.py'

The immutable A003 overlay is preserved raw by its existing archive attributes. Production source attributes remain unchanged; C++ checkout may use CRLF and host checks canonicalize that separately. No normalization is performed on pinned archival inputs. See docs/BULLET_VECTOR_CORRECTION.md and the earlier direction note for results and limits. Native artifacts/logs/argv remain absent.
