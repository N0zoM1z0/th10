# Result transition and scaled audio: fixed native observation

## Scope and result

One source-bound code-generation observation on Run checkpoint
`2a254c553ec53c1b2945351237fcece3662aaa82` plus reconciled source/test packet
`89d4ccc325a02e72e0e4dd720bfe493d36a0b16109ccb5d59a880e119a4968c0`.
Behavioral/non-exact candidate; no new exactness credit, target execution or
runtime closure. Host tests, native observation, canonical replay and acceptance
are separate gates. Do not add disjoint helper sizes to owner credit.

The genuine updater→Run graph retained the exact prior ten translation units:
Gui primary, then AnmManager, AnmVmCreate, AnmVmId, EnemyEclDispatcher, Player,
FrontEnd, DecomphelpLeafAccessors, Main and Sound. Canonical `gui-score-add`
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, link profile and data-anchor harness
were unchanged. No synthetic root, forced retention/noinline/private ABI,
normal-support exception, profile/TU sweep or expression tuning was used.

## Frozen reproduction

Reviewed files are `scripts/diagnostics/result-transition/diagnose-result-transition-native.py`
and adjacent `native-inputs.json`. Their SHA-256 values are respectively:

- `324716d4b208c39043e878b8a41e04351993e035d299905924133232e0015f0b`
- `ccbacd23c05ad732b034edb0ac6e8edf1a5df838af01c91406090065164569ae`

The executed map pins44 native inputs and10 historical candidate paths. The later
contracts-doc follow-up changes one historical pin without changing native inputs.
`reproduction-inputs.json` is separately reviewed, **unexecuted metadata**, differing
from the executed map only in that final doc hash and a fresh output directory.
All44 native pins, graph, settings and driver bytes are identical. Its retained
candidate patch/manifest identifiers describe the original source packet, not the
later docs-only revision. No build/result is attributed to this new map. For a
commit guard, verify all44 `input_sha256` entries independently.
The35-path graph/header
closure is unchanged. Source files use actual Git-applied bytes, including
committed CRLF unchanged dependencies. The canonical manifest remains sole
settings authority. `config/result-transition-producer-impact.json` independently
selects15 producers/21 units (14 linked-PE,1 COFF) using old/current Run plus new
transitive include union and all units sharing each affected producer.

After externally verifying hashes and running normal pinned toolchain/target
preflight, the current committed checkout can prepare a fresh ignored driver copy
using only maintained files (an authorized build is still required):

```sh
set -eu
work=build/reproduce-result-transition-2a254c553ec5-e4d7dbaca4b0
test ! -e "$work" && mkdir -p "$work"
cp scripts/diagnostics/result-transition/diagnose-result-transition-native.py "$work/"
cp scripts/diagnostics/result-transition/reproduction-inputs.json "$work/native-inputs.json"
PYTHONDONTWRITEBYTECODE=1 scripts/repo-python -B "$work/diagnose-result-transition-native.py" --root . --check-only
PYTHONDONTWRITEBYTECODE=1 nice -n 10 scripts/repo-python -B "$work/diagnose-result-transition-native.py" --root .
```

The new output is `build/diagnostic-result-transition-repro-2a254c553ec5-e4d7dbaca4b0`.
Builds are serial; nonempty CL, _CL_, LINK or _LINK_ options are rejected. Both
check-only and build require their configured output directory absent or empty.
The executed historical map still points at
`build/diagnostic-result-transition-2a254c553ec5-89d4ccc325a0`; its original docs pin
and nonempty completed output intentionally prevent accidental reuse. Preserve
that original observation. Do not delete artifacts or edit hashes to force retries.
Further attempts require separately reviewed fresh metadata/output and authorization.
Preserve outer stdout/stderr because preflight errors precede `run-build.json`.
Never execute `source.exe`.

## Durable observation identities

- Report: `c575d8b2be29137f5539a048f860375416d45ed3c0247513dc2e14589b2fa6fe`
- PE: `0ecdf52cb99da8db97dfe04f1325d1cad953a1d0aa17fedf367de81f071a03b9`
- Map: `73d42806f4489dcd1561ce8f2e12d96b2eadd6b8e1481faa8858f99e39118184`
- PDB: `73411d48f7cc7e1efd55bec6509ed483069f21fdb07beb45ecf01fe7e0fefa38`

Map timestamp/base, PE CodeView and PDB GUID
`fc6fe67e5c09474e995065d9ba0ab54f`, age1 agree. Same-start map/PDB contributions
and aligned full-byte decoding establish the following; next-symbol subtraction
was not used. All33 required-real fragments have positive original-TU PDB/object
provenance; all ten actual object hashes and source pins were verified.

## Whole transition and actual callers

Candidate transition `[41EE20,41EFBD)` is413 bytes, with18 HIGHLOW relocations,
versus complete target `[423370,42350B)`,411 bytes. Neither owner contains a table
or trailing padding. Both completion routes load EDI at401627/401775 and call
real41EE20 at40162D/40177B. The callee consumes EDI and returns plain RET, naturally
matching this target boundary. The global remains a data anchor, not physical477830.

The413-v411 difference is structural, not merely relocated addresses. Complete
aligned block accounting gives these nonzero deltas:

- Entry/mode branch +6:8-byte local reservation and mode load/register compare,
  versus target4-byte PUSH ECX reservation and memory compare; early release differs.
- Secondary ID/capture −3: ID read through returned EAX into ECX, versus target
  stack-temp load and pushed ID. Real capture41EBC0 consumes ECX/ESI and RET16.
- Second VM setup −1: accumulator OR encoding; flags/priority store ordering differs.
- Main ID/music −2: returned-pointer dereference and EAX path argument, versus
  target stack-temp read and pushed path. Real loader41D050 consumes EAX and RET4.
- Final stack release +2: ADD ESP,8/plain RET versus POP ECX/plain RET.

The remaining eight blocks have equal byte lengths but include register/store
scheduling and address-field differences. All blocks sum411→413. Manager reloads,
resource/ID publication, script0/129, supervisor-tested opcode4 then opcode2,
profile/completion/speed publication retain their reviewed order. Verified zero,
one, music, dummy and empty-string data agree. Physical aliases remain unproved.

Real original-TU dependencies include AllocateVm405180, InitializeVm40E980,
AddVmVariant14052A0/118, capture41EBC0/168, LoadMusic41D050/90,
PlayMusic41D0B0/73 and Sound QueueCommand427E40/90. Wrapper427EA0/13 also exists;
actual callers can inline it. Capture still performs anchored conversions and
LoadMusic uses anchored strrchr. Matching transition EDI does not close these
private dependent ABIs or real rendering/audio behavior.

## Whole audio and arithmetic limits

Candidate `[41D100,41D14E)` is78 bytes, target `[420C30,420C84)`84; both fully
decode with no table/padding. Candidate loads magnitude before comparisons,
increasing peak additional x87 depth3 versus2. Its division stays in x87 without
a binary64 spill, improving that specific emitted-precision concern. Conversion
still calls `__ftol2` data anchor429764; target converter463B2C and runtime
precision control remain unproved. Preserve the source finite counterexample
(magnitude4EFDFFFF, scale3F7FFFFF); do not infer general numerical equivalence.

After TEST AH,41, target JE420C60 skips division only for masked status00;
candidate JP41D12A skips for00 and41. Thus unordered status reaching TEST selects
different paths. Target FLD420C57 intervenes between FCOMP and FNSTSW and does not
specify C0/C2/C3 architecturally, so this proven predicate difference is not an
unconditional hardware-wide NaN-input claim. Exception/load ordering also differs.
Observed float-stack/RET4 and real queue EAX/EDX/two-stack-argument/RET8 routing
are coherent, but neither conversion nor the target queue body is validated here.

## Run, tables and unresolved boundaries

Run remains1244 bytes:1146 executable,2-byte8BFF alignment,24 selector words at
401C6C. Updater remains3772:3741 executable,3-byte8D4900 alignment,7 words at402BE0.
Every candidate table destination is an in-owner instruction start and every word
has a HIGHLOW relocation; owner relocation counts51/50. Target Run4480 includes
4306 executable bytes,2 padding and172 table bytes (24/7/7/5); all target destinations
were checked. Next owner417010 is excluded. Original factoring/table differences
persist. Updater still passes Run in EAX/plain RET rather than target stack/RET4.

441 unresolved symbols remain:204 globals,47 imports,190 other function/data
symbols. Effect remains a data-anchor call at401682→4293D4 with six pushed logical
arguments. Storage aliases, CRT conversions, OS/renderer/audio dependencies,
effect lifetime and whole-Run runtime remain open. Canonical replay/full CI and
any eventual acceptance are separate parent-owned operations; this document does
not claim their results. No evidence dumps, binaries, PDBs or logs belong in Git.

## Checkpoint validation

The final applied source passed the actual-body baseline, all32 strict mutation
controls and12 classifier rejection controls, plus a separate UBSan baseline.
The parent then cold-replayed all15 affected producers/21 canonical exact units;
all passed with unchanged canonical settings and no new exact claim. Full pinned
`nice -n 10 scripts/repo-python scripts/ci.py` passed, including the new baseline
hook,970 source mappings,1126 configured exact units and current progress.
All44 native input hashes remained unchanged after tracking/test/docs integration.

Compact reproduction inputs and core source/tests are retained. Generated host
harnesses, raw logs, native binaries/PDBs and replay reports remain ignored.
