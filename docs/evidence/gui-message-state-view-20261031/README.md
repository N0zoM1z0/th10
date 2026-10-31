# GUI message state views and captured text inputs — 2026-10-31

The complete `GuiMessageVmView::Run @415E90..41700F` remains **NONEXACT**:
4472 native bytes against4480 target bytes. No exact credit or canonical recipe
is added. The [previous complete checkpoint](../gui-message-call-closure-20261031/README.md)
is freshly reproduced at4476 bytes before this campaign.

## Retained source and bounded improvements

Run now reads character, shot, difficulty and stage through the existing
`g_GameStageState` storage at474C40, with explicit offsets28/2C/34/3C. This is
a provisional scalar view, not a recovered original class. In opcode20, normal
byte stores through the profile pointer naturally require the four intervening
state reads. Four forced volatile integer observations and the volatile profile
pointer are removed. The second flag computation now has the target's complete
45-byte arithmetic/register sequence after explicit field binding. The first
flag computation still differs.

The first alternating-text lookup explicitly captures its existing typed VM
ID and ordinary manager pointer, then uses a compiler memory barrier before
lookup and possible ID clearing. This replaces the manager's volatile read.
It restores the observed ESI manager lifetime and also restores both physical
side-setting schedules. The manager read and ID-address calculation still
appear in the opposite order locally. Original barriers, qualifiers and helper
boundaries remain unknown; no threading claim follows.

The evidence verifier checks both entire side-setting opcode bodies,82 and88
bytes, and the45-byte second flag calculation. Every ordinary byte, all14 linkage
fields and both local exits to the actual instruction-advance block bind
explicitly. These215 bytes are observations inside one nonexact
owner, never separate match units. It also checks all15 selection reads before
opcode20 against the semantic state base plus explicit addends.

Diagnostic anchor labels can coincide with state-base-plus-offset addresses.
The verifier resolves the named state base and addend directly rather than
accepting a coincident scalar or D3D label. This does not establish production
data ownership or runtime storage closure.

## Complete comparison and validation

Warm and zero-reuse21-input cold images reproduce the same full native receipt:
4472 owned bytes,4300 code bytes with no trailing alignment,1145 decoded
instructions,337 fields,121 fixed local branches,150 calls, two RET4 exits at
10A3/10C9 and43 selector words. The target remains4306 executable bytes plus
two alignment and172 selector bytes. The D8 frame,24 physical opcode groups,
all150 target call identities in order, all54 hidden EDI output slots, stage7
portrait ordering, both flag-before-score writes and actual transition setup
remain verified. Every direct callee has an executable PDB-owned body.

The eight-byte extent gap is not a count of differing bytes. Shared text setup,
the remaining alternating-text schedule, sprite registers, wait zero caching,
first profile indexing, counter scheduling/tail placement, special-VM clearing
and the instruction cursor remain open. The script binder remains197/203.
The target's empty relocation directory exempts no field from binding.

```sh
scripts/repo-python docs/evidence/gui-message-state-view-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-state-view-20261031/reviewed-candidate
scripts/repo-python docs/evidence/gui-message-state-view-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-state-view-20261031/candidate-cold
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit gui-update-stage-elements --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --unit transition-effect-factory --json
```

Both candidate images preserve seven existing/provisional guards5370/all109:
GUI3836/all72, damage1170/all28, auxiliary202/all6, decoder41/raw,
stage-state32/raw, transition factory82/all2 and provisional callback7/all1.
The separately scoped canonical cold selects GUI, its coupled damage pair and
the transition factory in their existing two producers, passing5290/all108.
Its terminal result is recorded in [results.json](results.json). No SpellPanel replay, historical
cohort, all-consumer sweep, host failure/lifecycle tests or Oracle changes.

One wrapper argument error precedes native work. The first native canonical
attempt fails when a Wine path-conversion page fault contaminates an object
path. Neither is a comparison result. The required toolchain execute smoke
passes normal COFF, LTCG, resource and PE32 link; the fresh scoped retry then
passes all four units. Compiler profiles stay unchanged. Default seven-stage
CI and whitespace checks pass.

The packet retains64 source/header pins, the unchanged real21-input graph,
complete native field/local/table receipts, hidden return slots, explicit region
bindings and compact terminal control hashes. Builds are serial on CPU0 at
nice10 with bounded runtimes and retained exits under the private campaign
`gui-state-view-20261031`. No original binary, database or decompiler text is
committed. Names, source/TU/data provenance, product closure, runtime and
portability remain independent and open.

## Controls parked

| Control | Native bytes | Result |
| --- | ---: | --- |
| Shared completion state |4472|Restores second flag calculation without four forced observations; retained |
| Barrier after shot selection |4480|Extra difficulty resample and wrong schedules; restored |
| Ordinary profile pointer |4472|Complete native inventory unchanged; retained as simpler source |
| Shared selection view throughout Run |4472|Ordinary bytes unchanged; explicit semantic bindings retained |
| Captured difficulty plus barrier |4476|First read order improves; registers and second index differ; restored |
| Named first-record pointer |4472|Complete inventory unchanged; restored |
| Captured ordinary line ID/manager |4472|ESI and both side schedules recovered; retained |
| Volatile captured manager |4472|Undoes manager and side gains; restored |
| Two observed first-selection loads |4476|Initial loads agree; arithmetic and second index differ; restored |
| Direct volatile pair expression |4476|Complete inventory unchanged from preceding control; restored |

These ten controls are complete owner comparisons. Equal extents and normalized
hashes are diagnostic statements, not target exactness. Reopen the parked
controls only with new evidence and a falsifiable prediction.
