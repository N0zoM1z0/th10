# Statistics cleanup — 2026-10-29

Two complete statistics cleanup owners reproduce **121 bytes/all7 fields** in
independent final-source canonical nineteen-input cold links. Both retain
indeterminate origin; this adds no authored exact bytes or authored denominator.

| Target owner | Bytes | Fields | Canonical selector |
| --- | ---: | ---: | --- |
| `42AEF0..42AF1C` destructor | 45 | 2 | `statistics-owner-destructor` |
| `42AF90..42AFDB` nullable global release | 76 | 5 | `statistics-release` |

`GameStatisticsOwner` now owns the header and loaded-payload buffers in its real
destructor. Each non-null buffer is freed and cleared independently. The global
release deletes the nullable owner and then clears47783C. Startup teardown uses
the destructor through ordinary `delete`, replacing its duplicated buffer cleanup.
No calling convention or register declaration is introduced: LTCG selects the
observed private ESI receiver for the retained45-byte destructor.

## Boundary and origin review

These entries were absent from the ledger and Ghidra function inventory. Raw
whole-extent decoding establishes both RET boundaries and adjacent CC padding.
Every byte was queried in the attested read-only Ghidra database; neither extent
has observed references. The complete65-byte scalar deleting form at42AFE0 through
42B020 independently expands both releases, tests delete flag1, optionally deletes,
returns the owner and ends RET4. It is recorded as a compiler-generated exclusion,
without a source mapping or exact credit.

The absence of references does not establish original authorship or original TU.
The destructor and global release remain origin indeterminate; their exact codegen
and maintained source mappings are independent claims. Three new reviewed extents
add186 bytes:121 indeterminate and65 compiler-excluded. Existing authored rows and
the reviewed authored denominator are unchanged.

## Native proof

The existing Main/Startup nineteen-input graph is retained, with pinned VC7.1
profiles and `/GL`. Two canonical cold runs use explicit selectors for the two
new owners and five affected existing Startup owners. All seven pass556 bytes
and all33 fields. No objects are reused by either cold build. Complete PDB
contributions, fixed internal destinations, exhaustive fields and actual symbol
bindings are compared without exceptions to the byte comparison.

The same images also pass23 explicit older contracts6235/all179, including
startup preload, worker Start/Stop/destructor, the Sound streaming creators,
queue/reset and actual callee interfaces. This is a focused comparison of the
changed context; it is not a historical-cohort, product or runtime claim.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit statistics-owner-destructor --unit statistics-release \
  --unit startup-worker-constructor --unit startup-register-tasks \
  --unit startup-worker-create --unit startup-release-resources \
  --unit startup-draw-callback --json
```

Private recipes, source snapshots, full receipts, listings and exit logs are
under `.analysis/gpt-6.1-sol/startup-update-20261029/`. The first independent
witness is `build/gpt-6.1-sol/startup-update-20261029/cold-a/`; the second is
canonical `build/match-linked/StartupWorkerLifecycle/`. Public identities and
source pins are in [results.json](results.json).

## Startup investigation and remaining mismatch

The complete startup destructor remains433/417. Factoring the real statistics
destructor preserves its existing bytes, but does not resolve the parent.
The target lacks the candidate's normal-path state0 write before VM cleanup;
zero-register lifetime and argument scheduling also differ. The two-state unwind
observations remain diagnostic, with no accepted parent or handler binding.

An ordinary Update member plus an inlined fastcall adapter remains67/63 for
both bodies and is restored. A natural readiness-bitfield control remains67;
volatile readiness remains67 and changes the already-exact constructor128 to132;
both are restored. Explicit `throw()` on the VM destructor also leaves the
parent433 and the unwanted state write unchanged; both declarations are restored.
These controls are parked pending a new premise. Draw218/220 and thread292/299
remain nonexact. No partial exact credit follows from these investigations.

The first private producer failed its Wine path-identity guard before compilation;
that run is excluded. The pinned compiler/resource/LTCG execution smoke passed,
then all subsequent diagnostic and canonical runs completed successfully.
Lightweight CI and whitespace checks pass. No comparator mechanism changed.
