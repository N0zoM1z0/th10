# Final bounded comparison: hypothesis parked unadopted

Actual read-only command e0b81b4be4214e44a6417b8a7c04ec94 compared the complete130-byte candidate helper against the authored original130-byte owner at442300. Result:94 equal raw positions and36 different. Candidate inventory identified one4-byte DIR32 field; target inventory identified zero fields; both decoded all130 bytes. Zero target identified fields is only decoder inventory, not proof of no globals.

Any normalization restricted to those four identified candidate positions can affect at most four mismatching positions, leaving at least32 mismatches. This is a conservative bound, not a normalized comparison result. No normalization ran. No signature or ABI conclusion follows.

The candidate had both canonical same-image guards passing, ExecuteScript still9948 bytes versus9964, and complete alpha130 versus130. Production remained clean and unchanged. Candidate is unadopted with zero exactness credit. No further helper builds or normalization are planned.

The first comparison attempt failed during parser import and established no comparison result. Its exact pinned parser import failure was reproduced source-only, then fixed by registering the module before executing it and rolling back registration on failure. See FAILURE_DIAGNOSIS.md and the final reviewed V2 source/tests. No duplicate failed script is needed in the compact archive.
