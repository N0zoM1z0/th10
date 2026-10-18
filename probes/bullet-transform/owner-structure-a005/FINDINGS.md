# A005 complete transform-owner structure

The complete generated BulletRuntimeView::AdvanceTransformProgram owner is **1540 bytes: 1392 reachable instruction bytes plus 148 inline table bytes (128-byte remap and 20-byte destination table)**. There are no fill bytes or unclassified bytes. The closed static CFG has 327 instructions, five selected dispatch destinations, 369 edges, 16 opaque returning calls and one return. Every selected destination is an in-owner instruction boundary; code/table disjointness and the full-CFG guard no-bypass checks pass.

This is complete-owner structure evidence, with zero exactness credit. The authoritative generated extent remains 1540 bytes, while the original owner remains 1454 bytes. Neither extent is cropped. The original proof classified all 1454 owner bytes as instructions and placed its 148 table bytes outside that owner. No helper sizes are added to either side.

The generated remap SHA256 a82463843ab4bcf47c8611e41926084c1b55330f6675fac171965ad29e4e3717 matches the previously established original remap hash. This establishes those bounded remap bytes' equality; it does not prove selector origins, case-body equivalence, runtime semantics or an exact function match.

The model assumes immutable code/table contents, valid normal non-exception memory access, and opaque calls that return normally. It does not execute the target or generated runtime.

## Verified evidence

- A005 generated complete-owner SHA256: 72917a0ae941e4e20a0418c93db0bf9810446934d2acf38c7df03d9bc552fb52
- Candidate instruction-byte-set SHA256: d6951457606ce5ed927c1baef862601ff97e4c308d21272f9ccad3d85400c354
- Candidate pointer-table SHA256: 02eeb51684fbd4d0e5478e11777f311e047f412d40b5386b8ce1b8f56ba2da64
- Preflight command: repository-command:1065df57873c4d25a82ddfce5256af8b
- Closure command: repository-command:bc1928cd802048969eb5ac0402278a00
- Approved closure source SHA256: 9d06f848eff7377887a3cd2abb9616b1d6fc50024a754caefdf4ab3874758836
- Production HEAD before/after both commands: 75536f63312688610002392067ab8604cd33844b, clean

## Final replay: layout and direct helper CALL

Final command repository-command:6d89607bee1146c1a3b09e84078f8193 completed successfully on the same clean production HEAD and unchanged A005 artifacts. Executed collector SHA256: 192fb613360eb374453a2fbc617b34e0245656eec215ee61160b5fce8cfd8fe8. Its source is frozen.

All four layout predicates are true: the 1392 instruction bytes form one contiguous owner prefix; the 148 table bytes form one contiguous trailing suffix; the 20-byte pointer table precedes the 128-byte remap; and the tables end at the owner end. These are accounting predicates only. They do not authorize a configuration, normalization or acceptance change.

The closed caller CFG contains **one actual direct CALL instruction** targeting the exact static helper symbol ?BulletResolveTransformAngle@@YAMPAUBulletRuntimeView@@M@Z. Its instruction boundary and helper-entry identity are proved under the stated CFG model. Unique MAP/PDB attribution places the complete **68-byte helper owner** in the same pinned normal object as the caller. No helper instruction bytes were sliced, decoded or hashed, and helper runtime behavior remains unproven. Caller and helper sizes are not combined.

This direct-call evidence is stronger than the earlier relocation/E8-form observation, which did not by itself establish an instruction boundary or reached CALL edge. It does not establish semantic equivalence with the original target or any exact-match credit.
