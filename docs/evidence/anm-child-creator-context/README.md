# ANM child routing and real creator caller context

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `f1596fa`. Reviewed on2026-10-21.

The maintained child routing is correct. Three old function-ledger comments
reversed opcodes90/91 and are corrected. A new bounded caller checker validates
source/target/candidate call identities. Adding the real Player caller restores
four complete73-byte creator guards in the larger graph. **No maintained C++,
header, profile, recipe or exact total is changed.** The original three large
functions and all later phase gates remain open.

## Target-bound call routes

Fresh target jump-table and CALL decoding establish:

| Opcode | Creator entry | Maintained name | Manager list operation |
| --- | --- | --- | --- |
| 88 | 0x448D00 | CreateVmVariant0 | primary tail |
| 90 | 0x448F60 | CreateVmVariant1 | secondary tail |
| 91 | 0x448E30 | CreateVmVariant2 | primary head |
| 92 | 0x449090 | CreateVmVariant3 | secondary head |

Variant names are maintained descriptions. Numeric opcode order is not a
callee-identity Oracle. Source already expresses these four routes; the incorrect
90/91 comments at448AC0,448E30 and448F60 did not establish a code bug.

[report-anm-child-routing.py](../../../scripts/report-anm-child-routing.py)
derives each source creator from its case and joins the independently reviewed
canonical creator/helper address bindings. It reads the target's94-entry table,
decodes the complete contiguous child region and follows both sides of each
conditional. Every selected static path contains the optional integer getter,
then its creator, GetVm449450 and InsertAfter413200 in order. Only creator
selection is parsed from C++; the helper sequence is the reviewed route contract.
Candidate mode checks the emitted helper calls. This is not a general C++
semantic parser. Calls are assumed
returning; predicates are unconstrained. There are two bounded paths per opcode,
eight per image. This is not original/native-product execution or whole-function
control-flow/byte equivalence.

Optional candidate checking binds unique PE/MAP/PDB owners and actual CALL
entries. It explicitly does not establish whole callee bytes: a74-byte callee
can have correct call routing while failing its73-byte exact contract. Both the
current ten-TU carrier and the real-Player eleven-TU image pass the route check.
The [native summary](native-summary.json) keeps routing and complete-byte results
separate, rather than allowing masked case diagnostics to imply callee identity.

The target-independent suite covers24 physical layouts/192 static paths and16
strict negatives, including a cycle, instruction interior, indirect/far calls,
unknown helper, unsupported loop/halt, ambiguity and incomplete decoding. Two
actual wrong-source cases and two copied-native CALL-field mutations also reject
at the intended routing condition. The copied creator and resolver changes are
valid direct CALL encodings; PE/MAP/PDB identity remains usable. Only diagnostic
copies are mutated, never the original executable. Compilation, file or unrelated
failures cannot count as these rejections.

## Current compiler/caller evidence

The accepted ten-TU graph passes41/45 complete guards4978/all120 when the ten
creator/list/id helpers are added to its previous35 guards. The four creators
are complete74-byte contributions, so their73-byte contracts reject before an
ordinary-byte score. Their initializer uses unlike private input registers.
The existing canonical creator recipe is primary AnmManager plus RandomMath
and AnmVmCreate, /GL without /GS. It is a separate declared context; these
74-byte diagnostic results do not establish failure of that canonical recipe.

An incremental eleven-TU control appends unchanged `src/Player.cpp` to the real
dispatcher graph. All33 recursive project inputs are frozen: the prior32 are
unchanged and Player.cpp is the only added input. Only that TU is compiled;
ten hash-verified game objects are reused and the normal diagnostic anchors
are regenerated. /GL,/GS and the scale-only /Ob1 override remain. Entry remains
the real Enemy dispatcher. No return/ABI annotation or artificial root is added.

InitializeVm naturally regains incoming ECX loaded-resource plus two stack
arguments/RET8; the four creators now strictly match all73 bytes/all5 fields each.
The image passes **45 complete existing helpers5270/all140**. The initializer
itself stays207/217, and world creator0 stays116/118; neither receives credit.
Fresh Ghidra lists46 incoming initializer calls, including the seven explicitly
restored Player calls. This is not a claim that all46 source callers or original
production partitions are closed. A standard incoming ABI alone does not prove
normal-COFF ownership.

Eight executed controls include that caller addition, two child91 value-lifetime
forms, flag lifetime/order and vector copy/storage hypotheses. Eighteen game TUs
are compiled in total; the one shared vector-header experiment recompiles all11
TUs consistently. Every other control reuses nine or ten verified game objects.
The private SDK8 storage-inheritance analogy is hypothesis material only; it
does not identify TH10's SDK9 classes. No duplicate mixed type definitions are
used in the full private type experiment.

Direct temporary.GetVm at child91 regresses ANM case-aligned7478->7460/7995
without changing9964,90/92 same-sized groups or gap2; call routing remains correct.
A named script operand preserves all three complete native parent bodies raw.
Flag snapshot, explicit copy blocks and the storage-base assignment preserve
the corresponding complete parents and207-byte initializer. The complete private
storage-base cold reproduces all five measured native bodies raw, including
world116. Moving the whole flag update before the third vector changes the
207-byte initializer but preserves all three parent bodies. No shorter owner is
cropped or scored as217. Prepared height-order alternatives are not executed.

Current fresh ANM remains9964,7478/7995 case-aligned,90/92 target-sized groups,
gap2. The eleven-TU dispatcher remains14464/14416, frame2C4,19 sentinels, matching
181selectors/108groups,42 target-sized groups and gap650. These are noncanonical
diagnostics without parent/case credit. All eight complete Run comparisons bind
189 fields/62locals, retain their exact171-byte callee and enumerate the same27
ordinary differences. Thus all7020 target-replayed bytes remain equal, SHA-256
`830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472`.

Two direct local Ghidra reviews use readOnly/noanalysis, rollback, CPU1 and768 MiB.
Disk identity and complete mapped text401000..465A80 agree before/after:
412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Generated dumps remain private. Original class/TU/profile and data ownership
remain unknown.

## Reproduction and retained state

```sh
scripts/repo-python scripts/report-anm-child-routing.py \
  --artifact-dir build/match-linked/EnemyItemDrops --json
scripts/repo-python scripts/test-anm-child-routing.py
```

No canonical replay is run for unchanged production bodies/interfaces. The
current canonical carrier/source pins remain live. The additional eleven-TU
real-Player image and selected207-byte failure are retained, with33 source,
15 canonical artifact and32 additional native artifact pins. Eleven inspected
terminal/unexecuted directories are pruned/8444651 bytes; source/producer/guard,
complete extent and mutation reports remain. Private recovery is
`.analysis/gpt-6.1-sol/anm-child-routing-20261021/RECOVERY.md`.
The [validation receipt](validation-summary.json) records final gates and cleanup.

All105 standard CI checks pass in one invocation. Final route checks on both
images,16 strict synthetic fixtures,45 complete helper guards5270/all140, fresh
ANM9964 case diagnosis and expected Run7020/27 pass, together with toolchain/
tracking/progress/docs/whitespace gates. All33 maintained inputs,15 canonical
artifacts and32 retained additional native artifacts remain hash-current.
