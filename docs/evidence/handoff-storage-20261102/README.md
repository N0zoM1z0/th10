# TH10 handoff storage cleanup (2026-11-02)

This is a storage-maintenance action, not codegen or runtime evidence.
The ignored build/gpt-web workspace had expanded again with thousands of
exploratory PE, COFF, PDB and intermediate compile objects after the
2026-10-31 archival pass. These are regenerable candidates rather than
the canonical selected build/match-linked source products.

Only this ignored experimental build tree and the disposable
build/pycache bytecode tree were pruned. The cleanup removed:

- **4,252** regenerable build files, **205,594,083 bytes** of file payload.
- The bytecode cache: **4,547,738 bytes** of generated files.
- **210,141,821 bytes total (200.41 MiB)** of file payload.

Ninety files (**4,861,456 bytes**) in six valuable bounded native
candidate directories remain expanded, including the ANM executor,
ECL Run, the just-reconstructed GameManager constructor graph,
GUI draw and Sound sample loader. They are useful diagnostics,
**not** current exact acceptance merely because the files exist.

The following were not removed: original hash-attested resources/th10.exe,
ghidra-project, the pinned .tools toolchain/Wine environment, canonical
build/match-linked, historical build/gpt-6.1-sol witnesses, the
.analysis/archive private evidence and inventories, the original
.analysis/gpt-6.1-sol unresolved-boss contexts, and unrelated D3D
adapter evidence. The target SHA-256 was checked after cleanup.

The ignored name/size/SHA-256 inventory of removed scratch payloads is
.analysis/maintenance-20261102/pruned-build-inventory.jsonl.gz.
That inventory records **names and fingerprints, not the deleted bytes**.
These candidates must be regenerated from current source and selected
compiler recipes if ever needed. The permanent machine-readable summary
is [cleanup-summary.json](cleanup-summary.json).

A historical source pin, a cached original-candidate image, or a
previous PE/PDB hash must not be presented as a current-source proof
after source changes. For exactness always re-run just the relevant
canonical --unit selectors. The cleanup did not execute a full
cold replay.
