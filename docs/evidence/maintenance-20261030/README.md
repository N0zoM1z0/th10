# Handoff and storage maintenance — 2026-10-30

This batch changes documentation and ignored artifact storage only. No game
source, canonical recipe, comparator, ledger or progress input changes. The
existing exact totals remain 103,090 authored bytes across 503 owners, and
171,194 codegen bytes across 1,380 owners. Product/runtime/portability remain open.

## Corrected continuation information

The previous 875-line handoff mixed multiple source/build epochs and stale
“current” totals. Its complete body remains in
[the dated history](../../RE_HANDOFF_HISTORY_20261030.md); the
[current brief](../../RE_HANDOFF.md) separates live counts, latest source,
unfinished diagnostics and historical frontiers. The artifact index's October
24 “current” Player/ANM graphs are superseded. Old private archive paths are now
nested in the October 28 archive; [recovery instructions](../../RE_ARTIFACTS.md)
include that missing outer restoration step.

The knowledge base's blanket claim that `.analysis/` is disposable was too
broad: archives, inventories and unresolved witnesses require retention.
Its old inventory counts are explicitly historical. README navigation now
points to current source-epoch and storage recovery information.

## Source-pin audit

[context-audit.json](context-audit.json) summarizes all 584 one-level
`*context.json` manifests with nonempty `pins` maps in the retained analysis
root before compaction. Fourteen agree with current source/header hashes;
570 do not. The public rows include every pin-compatible context and selected
stale examples. The full audit remains privately compressed. This is a cache
compatibility check, not native acceptance or proof that all referenced images
and objects remain expanded.

Examples corrected in the handoff: old ANM executor final context has a changed
AnmManager.hpp pin; replay-save and music-helper contexts have a changed
Enemy.cpp pin; Player main-callee context has multiple changed inputs. Historical
pins are preserved rather than refreshed. Future agents must use actual graph
and source epochs before incremental reuse.

## Retained unfinished ECL investigation

[ecl-script-data.json](ecl-script-data.json) records the existing, unadopted
AddScriptData diagnostic. The unchanged EclVm.cpp Run-entry /GL graph grows
from one input to two by adding real Enemy.cpp caller/vtable definitions. Its
528-byte private-ABI candidate becomes 547 bytes with target ECX this, one stack
argument and RET4. Complete target extent remains 589; AddScriptData is
**NONEXACT**, without a canonical unit or match admission. No source rewrite
was tested or retained in that investigation.

Existing canonical `ecl-vm-find-subroutine` passed 136 bytes before the
maintenance batch. The diagnostic emits those same 136 bytes; its structural
result alone grants no new credit. Original terminal logs, target/native
listings, profiles, source/header hashes and bounded runners remain expanded
under `.analysis/gpt-6.1-sol/ecl-script-data-20261030/`.

## Verified compaction

[cleanup-summary.json](cleanup-summary.json) pins the private archive and
inventory. All 13,305 members were read back and checked by size/SHA-256; an
individual context was restored to a scratch file and verified. Every original
was rehashed before removal. Only Git-ignored regular files in the two explicit
analysis/build campaign roots were eligible. No symlink, tracked file or active
compiler job was included. Compaction ran bounded on CPU0 at nice10.

Seven source-compatible campaigns and canonical linked producer outputs remain
expanded. Older terminal controls, copied objects, source snapshots and bulky
reports are recoverable from the archive. Original target, Ghidra database,
toolchain/Wine, references and D3D adapter evidence are preserved. Rebuildable
Python cache removal is accounted separately from lossless compaction.

[Retained-context checks](retained-context-checks.json) also verify the primary
Ending, Enemy operand and ECL script-data source pins, recorded object/image
hashes and MAP/PDB presence after compaction. This checks storage integrity;
it does not rerun the native comparator or grant exactness credit.

## Validation

Run the default seven lightweight checks and whitespace check:

```sh
scripts/repo-python scripts/ci.py
git diff --check
```

No game compilation, Ghidra observation, target-dependent analysis, comparator
self-test or unchanged progress regeneration is needed for this maintenance.
