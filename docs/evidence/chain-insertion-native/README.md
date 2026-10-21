# Chain insertion native matches

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This batch adds two canonical linked-PE matches, 262 complete owned bytes.
Physical source/data ownership and the Windows i386 product remain open.

| Target owner | Complete extent | Exhaustive linked fields |
| --- | ---: | ---: |
| AddToCalcChain 449AE0..449B62 | 131 | 7 |
| AddToDrawChain 449B70..449BF2 | 131 | 7 |

## Target observations

Fresh local Ghidra reads attest all 412289 mapped text bytes 401000..465A80
before/after, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The headless job uses readOnly/noanalysis, a rolled-back transaction, one CPU
and 768 MiB heap. Ghidra's signatures remain provisional. Independent target
decoding consumes each entire 131-byte body through RET4; no switch/data suffix
or extra comparison extent is needed.

The physical inputs are ESI element, EDI signed priority and stack Chain owner.
The optional added callback receives its ECX argument before locking; its result
is retained and its callback field cleared. Under the shared lock, the priority
is stored and insertion stops at the first existing priority greater than or
equal to the new priority. Equal-priority newcomers precede older elements.
Calc/Draw head offsets 14h/38h differ; list-node and argument offsets remain 14h/20h.

The reviewed lock 492274 and byte counter 49231C are already represented by
MainSupervisor base 491C28+64Ch/+6F4h. The source uses those existing members in
the two insertion functions. No global definition, type, signature, algorithm,
volatile, padding or private-register declaration is introduced. Original source
names, class/TU/profile, counter signedness and physical storage aliases remain
unknown. Reproducing codegen does not settle those separate claims.

## Compiler controls and exact acceptance

Both old scalar bodies already had 131 bytes. Their counter fields at 29h/3Bh
differed from target 25h/31h, so the unchanged exhaustive-field inventory refused
both bodies. Using the existing supervisor members recovers the target
instruction scheduling. A private one-Chain-object/13-reused-object link then
passes all 131 bytes and seven fields for each function. That private model is
a candidate experiment, not an independent full cold.

`chain-add-calc` and `chain-add-draw` use the existing 14-TU ScriptViewerCreate
context and shared canonical output. Chain owns the PDB contributions; all
declared `/GL` profiles and support overrides remain unchanged. The seven fields
cover two lock pointers, two IAT references and three counter references; every
supervisor member binds the reviewed base and its explicit addend. Target field
locations are independently decoded, never copied from candidate relocations.
All ordinary bytes and all linked destinations are compared by the unchanged
canonical Oracle.

The first actual-source cold recompiles all 14 TUs and passes the new 262 bytes
plus both existing frame dispatchers 576: four units, 838 bytes. Its complete
output is snapshotted and hashed before the second cold. The second independently
recompiles all 14 TUs and again passes the new pair, 262 bytes and all 14 fields.
Only 262 new bytes enter the exact numerator. No other old helper or large-owner
cold is claimed for this batch. See [evidence](evidence.json) and
[validation results](validation-summary.json).

## Host scope and retained context

The existing host storage-role projections now require exactly 12 occurrences
each, accounting for the four newly adopted lock/counter accesses. Only the
active Chain.cpp LF pin changes; the other six dependencies, exact replacement
checks and CRLF-to-LF-only rejection policy remain unchanged. The fresh baseline
passes 1375 ASan/UBSan assertions, eight strict faulty-code controls and 19 EOL
guards. Host observations do not establish native layout, Windows concurrency or
engine runtime. All108 full CI groups pass with all368 frozen inputs unchanged.
Classified cleanup removes14120146 logical file bytes from eight owned output
trees and one duplicate disassembly, retaining both actual cold carriers and
compact evidence. Independent D3D work and provider/legacy state are preserved.

All 1240 prior canonical unit dictionaries and the existing large 32 source-input/
15 artifact hashes are freshly checked unchanged. Chain.cpp is outside that
ten-TU large graph; Main.hpp is unchanged. The whole canonical manifest hash
changes by adding two units, so older whole-file fingerprints are historical.
The earlier runner proof's shared second carrier is superseded by these colds;
its first snapshot/receipts retain historical scope and the runners are freshly
replayed in this batch's first cold.

Authored exact is now 401 functions/73225 bytes; overall 1242/139735. Authored
source 610, nonexact 209, absent 124, authored denominator 734/273579 and unknown
524 retain their meanings. Product/runtime closure remains open; semantic
reconstruction and portability have not started. Private recovery:
`.analysis/gpt-6.1-sol/chain-insertion-codegen-20261022/RECOVERY.md`.
