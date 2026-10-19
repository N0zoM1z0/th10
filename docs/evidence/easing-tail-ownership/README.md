# Complete easing extent and internal-link validation (2026-10-19)

CalculateAnmInterpolation owns576 `.text` bytes at0x44C350..0x44C58F:
509 executable bytes, three internal alignment bytes and a64-byte dispatch
table. This corrects the prior509-byte code-only ledger entry. It adds67
owned bytes to the authored denominator and **zero exact bytes**; source,
origin and exact dispositions remain unchanged.

The same review exposes and fixes a linked Oracle false positive. Local
addresses must preserve their contribution-relative destination, in addition
to staying within the compared contributions. The three existing canonical
units using local fields still pass2,704 complete bytes/all60 fields, including
all18 internal destinations, using their retained PE/MAP/PDB artifacts.

## Target ownership

The pinned JP1.00a target, fresh local read-only Ghidra and pinned Capstone
agree on the following independent facts:

- Entry0x44C350 receives mode in EAX and two stack floats; all17 RETs pop8.
- DEC EAX, unsigned bound15 and index scale4 select exactly16 dword entries.
- The table occupies0x44C550..0x44C58F. Every destination is an instruction
  start in the509-byte executable body, including the shared linear return.
- RET at0x44C54A ends the code at0x44C54C. A three-byte LEA alignment instruction
  separates code from the table. The reviewed tail's only incoming reference
  is the indexed jump's data reference to0x44C550.
- A separately decoded57-byte body starts immediately at0x44C590. It receives
  no new ledger entry or origin attribution here. No recorded owner overlaps
  the corrected576-byte extent.

The target has no PE base-relocation directory. Its table binding comes from
the complete bounded selector, raw values, instruction starts and Ghidra refs;
no target relocation evidence is invented. A complete-file scan for absolute
literals into all67 tail bytes finds only the indexed jump's table-base value;
this does not exclude computed references. Diagnostic candidate fields are
independently enumerated from their own PE base relocations.

Mapped `.text` is verified before/after against SHA-256
`c1611c9becb832a4e2ba456afc58218fb5b8fafd6c85d19e3addbebb0c946e0a`.
Temporary Ghidra metadata is rolled back. The fresh standard architecture
export records the509-byte reachable body; reachable code and owned physical
extent are distinct. Only the two affected ledger rows are edited; unrelated
inventory evidence is not regenerated. `target-ownership.json` records all55
absolute fields,16 destinations, complete hashes and the exact boundary delta.

## Oracle false positive and correction

A synthetic real PE/MAP/PDB fixture compares a complete24-byte contribution
at candidate0x501000 against target0x401000. Its code references a table at+16,
whose two entries point to code at+6 and+7. With comparator06106ea, swapping
those two candidate entries still reports24/24 `exact`: both destinations are
internal and normalization overwrites the changed values. The new comparator
rejects the first changed destination offset. This is reproduced against both
immutable old code and the live corrected comparator in the same fixture.

The guard checks both semantic bases after accounting for the declared addend:
each must lie in its corresponding compared contribution, and their relative
offsets must be equal. Relocated images with the same internal destinations
remain valid. Changed code labels, shifted internal data/table bases,
out-of-range target bases and misleading addends are rejected before any field
is rewritten. Nonlocal and qualified-static binding policies are unchanged.

Five end-to-end fixture tests pass normally and under Python `-O`; the existing
eight linked-context controls also pass. The fixture mocks control-plane target
identity only; actual PE, MAP, PDB, relocation enumeration and byte comparison
run. It never uses or changes the private game binary. Public CI includes the
new controls. `oracle-review.json` records the old false pass and the exact
retained-artifact audit of every existing local-field canonical unit:

| Unit | Complete bytes | All fields | Local destinations |
| --- | ---: | ---: | ---: |
| enemy-spawn | 577 | 12 | 7 |
| anm-texture-alpha-bleed | 1463 | 6 | 6 |
| script-viewer-draw | 664 | 42 | 5 |

## Bounded compiler control

The prior source already aligns40 arithmetic instruction paths, but its default
complete contribution remains612 bytes. One causal size-policy control keeps
all current source unchanged, the real Stage Update entry, five-TU graph,
`/GL /GS`, and all original flags; only AnmManager.cpp gains `/Os` through the
existing explicit support-profile mechanism. Its result is560 bytes:494 code
bytes, two alignment bytes and64 table bytes. It merges all return sites to
one RET8, whereas the target has17. The EAX/two-float call protocol survives.
This prediction is falsified for complete equality, so the profile is not
adopted and no further flags are swept.

The Fog evaluator stays691/688. Other changed diagnostic extents are recorded
as nonexact observations, never additional credits. No source, canonical
compiler profile, match unit or exact ledger is modified. A single cold build
uses13 frozen project inputs; resource limits are one native job at nice10,
Ghidra768MiB and one active processor. `compiler-control.json` pins the graph,
source inputs, temporary override and observed shapes.

Focused Oracle checks are the five new internal-link tests and the three
existing local-field comparisons. Their retained artifacts are checked without
rebuilding a historical native cohort. Whole product/runtime, original compiler
partition, semantic and portability gates remain open. Rotate to a different
complete authored owner; these snapshot/API/size-policy controls are closed
without additional TH10 evidence.

Full public CI, toolchain declarations, tracking, generated progress and whitespace
checks pass. The discarded size-policy build releases504,119 bytes. Current
exact authored coverage is59,142/268,564 owned bytes (22.02%); no exact bytes are
added or removed. The pre-existing D3D experiment remains untouched.
