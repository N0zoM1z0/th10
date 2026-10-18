# A004, R001, A005 and A006 archival recipes

recipes.json stores hash-checked runner/source text splices and manifest deltas against the exact existing A003 archived runner, manifest and overlay. Historical b635 direction-only source and72aa vector-corrected source are reconstructed from archived da1 source bytes. Current production source is never read, so later source corrections and normal CRLF checkouts cannot break old packet replay.

Offline reconstruction, from repository root:

    python3 probes/bullet-transform/native-experiment/recovery-history/reconstruct.py a005 --output /tmp/new-a005-archive

The output directory must not exist and must be outside the repository. This writes three files and never executes them. Historical execution_ready fields preserve original packet identity; approvals were consumed. Reconstruction gives no authority for another build or relink. Original paths, HEAD pins and A004/R001 packets remain exactly reproducible, without full source or runner copies.

Pure tests:

    python3 -m unittest discover -s probes/bullet-transform/native-experiment/recovery-history -p 'test_*.py'

The immutable A003 overlay is preserved raw by its existing archive attributes. Production source attributes remain unchanged; C++ checkout may use CRLF and host checks canonicalize that separately. No normalization is performed on pinned archival inputs. See docs/BULLET_VECTOR_CORRECTION.md and the earlier direction note for results and limits. Native artifacts/logs/argv remain absent.

A006 adds an isolated ownership-expansion experiment, not an adopted production change. Its e698 overlay is derived from the same immutable archived A003 source, independently of production. The complete1596-byte result is statically accounted as1445 decoded instruction bytes,148 table bytes and3 unclassified bytes; those3 are not inferred padding/data. Dynamic reachability and generated branch semantics remain unproved, with no exactness credit. See docs/BULLET_A006_OWNERSHIP_EXPERIMENT.md.

## A007 historical failed-regression experiment

Recipe a007 reconstructs the exact typed sound packet from A003 raw inputs, independently of production source. Native diagnostic link completed, but canonical885 failed: runtime owner878, baseline_valid=false. This is a FAILED regression experiment regardless of orchestration completed/exit0. Source824e remains isolated and unadopted. Historical execution_ready records prior single-run authorization; reconstruction does not authorize reruns. See docs/BULLET_A007_SOUND_EXPERIMENT.md.

## A008 altered-graph experiment

Recipe a008 reconstructs the frozen packet from immutable A003 inputs. Experimental full885 comparison failed: runtime884, transform1612. Both canonical_acceptance and experimental_graph_valid are false. No adoption or size-based exactness claim; no rerun authorization. See docs/BULLET_A008_LTCG_BOUNDARY.md.
