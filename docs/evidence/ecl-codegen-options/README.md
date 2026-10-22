# ECL codegen options and float-pop storage

2026-10-22, starting7f7897d. Three bounded compiler experiments leave maintained
source and canonical recipes unchanged. None improves the complete ECL Run
match. Current Run remains7020 bytes with27 ordinary differences, after all189
fields and62 local destinations are bound and the171-byte float callee passes.
No exact credit is added.

Fresh instruction review locates all27 residual bytes inside same-sized
instructions with equal mnemonics. They represent four thread register bytes,
five commuted-addressing bytes, sixteen stack-home bytes and two loop-latch
register bytes. In particular, differences at12A/12C are MOV register/address
encodings, not PUSH constant differences. The complete per-byte instruction
inventory is in [evidence.json](evidence.json).

## Executed experiments

| Control | Complete Run | Existing ECL guards | Decision |
| --- | --- | --- | --- |
| Unchanged source, append /Zi to ECL profile |7020/6993; same27 differences|9 exact,1137 bytes/all18 fields|No effect; reject as an explanation of these differences|
| Unchanged source, append /Os to ECL profile |2980/7020; complete extent rejected|All9 contracts rejected|Not the target shape in this graph|
| PopFloat uses plain float, retaining current stackView pointer |7020/6991;29 differences|9 exact,1137 bytes/all18 fields|No old difference repaired; two added|

The /Zi build has identical raw bytes across complete Run, ANM ExecuteScript
and Enemy dispatcher bodies. The compiler accepts /Zi without an ignored-option
warning. It creates an additional debug-information artifact, but changes none
of the27 target-replayed offsets or byte values. This is evidence for this
compiler graph, not proof of original debug settings.

The /Os build changes the optimization of the ECL object. Its Run extent is
2980, not7020, so it receives no ordinary-byte similarity score or cropped
comparison. Examples of rejected whole guards: ReadFloat133 versus183,
ReadInt124 versus144 and ReadIntValue93 versus124. ANM and Enemy extents remain
9964 and14336; equal extents do not establish equality of their relocated bytes.
No profile or acceptance-rule change is adopted.

Historical commit4fd3388 introduced the union in PopFloat; bfe3ad5 subsequently
introduced its explicit stackView pointer. The new control isolates the union
storage choice in the current ten-TU context, preserving the newer pointer
form and call order. All original27 differences retain the same offsets and
values, with two additional commuted-addressing differences at Run+F9A/+FA6.
ANM and Enemy remain raw-identical. Thus the union still removes two differences
in this context; this does not establish its original source authorship or
exclude other aggregate/lifetime hypotheses. The private patch is supplied
beside this file and is not applied to maintained code.

## Evidence, validation and retention

Target original Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh disk, executed VC7.1 toolchain and direct read-only Ghidra checks pass.
Ghidra uses maxcpu1/768 MiB, no analysis and transaction rollback. All412289
mapped bytes remain unchanged, hash
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

Each trial compiles only the ECL support object serially at nice10 and reuses
nine hash-verified game objects plus two harness files. GL/GS remain enabled.
The source copy is byte-identical for both option controls; the storage control
changes only PopFloat. Compiler/link logs, profiles, PE/MAP/PDB-bound extents,
source/producer hashes and artifact inventories are preserved. Guard descriptors
are checked against current canonical descriptors. These are partial rebuilds,
not full cold pairs. Full CI was not run, following operator preference.

Private producers, manifests, source copies and complete reports live under
`.analysis/gpt-6-astra-medium/ecl-codegen-options-20261022/`. The maintained
source,108 source pins,32 context inputs,19 canonical native files, canonical
recipes and nine unrelated D3D files are verified unchanged. Keep the current
canonical baseline and primitive-pop29-difference carrier. The two option
carriers were hash-inventoried and removed:33 files,2209593 bytes. Compact
source/report inputs remain for reproduction. The canonical diagnostic is:

```sh
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit enemy-drop-item-counts
```

Its expected exit1 denotes the remaining27 ordinary differences. The Windows
product/runtime gate and subsequent semantic/portability gates remain open.
