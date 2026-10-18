# ECL opcode-82 source-order investigation checkpoint

The unchanged-source baseline already has the same conditional local Resolve → Read → Wrap order as the target. The destination-first source candidate was canceled without a candidate build. No source sequencing mismatch or behavior bug was demonstrated. Production source remains unchanged; there is no byte-exactness credit from this investigation.

Reproducible core code, frozen manifests, receipts and bounded interpretations are under ../../../probes/ecl-vm-op82/ from this document's directory (repository root path: probes/ecl-vm-op82/). Start with FINAL_RESULT.md and README.md there. The successful baseline selects the complete Run owner at 7020 bytes and replays both same-image canonical normalized guards at 146 and 142 bytes. Full owner byte exactness is untested.

The added host-replay adapter takes an explicitly supplied source root, validates frozen script/patch/manifest and relevant raw source pins, and preserves original host script bytes and archived results. It optionally checks an explicitly requested clean Git HEAD. The default host-only replay does not require the historical HEAD indefinitely or claim an older source snapshot is current production.

All native instructions in the archive are historical, previously executed recipes; this checkpoint grants no rerun authority. The diagnostic baseline image is non-runnable and its Wrap-labelled call uses a bound data anchor with only a hypothetical normal-return interpretation for structural analysis. Runtime opcode association, complete instruction semantics, real Wrap execution and full-owner equivalence remain unproved.

ENEMY_SOURCE_NO_GO.md records the prior bounded source-only negative review without retaining full source trees or redundant native experiments.
