# Ending reload and creation contracts (2026-10-20)

`EndingView::LoadScript @ 0x40B480` closes **106 authored exact bytes** with
unchanged maintained source. A target-supported linked context restores its
observed file-call ABI. Separately, the nonexact Run owner now uses the correct
primary-list creator and explicitly completes creation before selecting the
live destination. Run receives no exact credit.

## Target and observations

The original Japanese TH10 v1.00a input is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh local read-only Ghidra verifies that identity and the complete mapped
text hash before/after review. The existing LoadScript boundary is unchanged:
106 bytes through RET4, followed by padding. No origin, boundary or denominator
change is made.

LoadScript copies a requested name into the shared path buffer, opens it,
then reads/frees the current member buffer, clears that member and publishes
the new result. Freeing occurs even when opening fails. The old buffer must
remain a live member read after the dependency returns. Host regression retains
a replacement-buffer witness for this order.

The target OpenFile call passes filename in EAX and two zero stack operands
for sizeOut/mode. The previous EndingCreate-rooted diagnostic instead lowers
mode into EBX, adding a push/pop pair and producing108 bytes. That private
optimization is a context difference, not evidence that the target used a
different source signature or a normal non-GL FileSystem partition.

The new diagnostic uses the real maintained Main executable-checksum entry,
whose file call uses mode1, and includes the actual PbgArchive dependency.
Ending.cpp remains primary. The complete ordered18-TU graph and unchanged
`/GL /GS /EHs` profiles are explicit in `ending-load-script`. This combined
context reproduces the target EAX/stack/RET8 OpenFile interface and the complete
106-byte loader. No claim is made that this graph is minimal or identifies the
original production TU partition. No flags, signatures, forced helpers or
target bytes are changed.

## Complete native scope

Two independent fresh18-TU cold links freeze52 identical source/header inputs.
Complete PE/MAP/PDB ownership, instruction decoding, ordinary bytes and all
five independently reviewed fields pass:

| Offset | Field | Target |
| --- | --- | --- |
| 0x08 | DIR32 shared path | 0x497C38 |
| 0x19 | DIR32 shared path | 0x497C38 |
| 0x3B | DIR32 shared path | 0x497C38 |
| 0x42 | REL32 FileSystem::OpenFile | 0x44B360 |
| 0x52 | REL32 free | 0x452422 |

Each carrier also strictly passes the actual OpenFile360/all26 fields,
primary-list creator73/all5 fields, ReleaseAnm55/all2 fields and the six existing
Ending exact owners349/all19 fields. This is **837 existing guard bytes**,
without duplicate credit or migration of their canonical recipes. Combined
validation covers **943 complete bytes/all57 fields per image**.

```sh
scripts/repo-python scripts/build-match-unit.py --unit ending-load-script
scripts/repo-python scripts/compare-linked-function.py --unit ending-load-script --json
```

The carrier still uses unique non-runnable data anchors for unresolved symbols.
These are function-level native proofs, not a product build/runtime claim.
Initializer584/584 remains nonexact, as do Run, the text constructor,
destructors and the bank loader. Size agreement alone grants no credit.

## Corrections to the earlier Run recovery

Raw TH10 calls in opcodes8/15/16/17 go to **0x448D00**, the independently exact
`CreateVmVariant0`, which inserts into the primary VM list. The previous source
used Variant1 at0x448F60, which inserts into the secondary list. The earlier host
stub routed that call into the primary list and therefore could not distinguish
the error. The maintained source now calls the correct actual creator.

In those same four branches the target reads the destination operand after
creation returns. Before C++17, the aggregate assignment expression did not
guarantee that ordering. The old VC7.1 output visibly reads the slot before the
call, while the C++17 host test evaluates the RHS first and passed. Explicit
ordinary `AnmVmIdView created` statements now sequence the dependency before
the live destination lookup. No helper, output-slot parameter, volatile alias,
ABI annotation or forced inlining is introduced. Opcode8 retains a reference
to the actual old ID for its independent deletion/clear stage. Opcode7 publishes
the background path before its busy flag, following target stores.

These corrections supersede the earlier broad source-fidelity wording for
these branches; the earlier six bounded exact owners remain separately proved.
Run is still nonexact. The retained larger extent is1576/1568 in this graph,
with no case-level or partial exact credit.

## Host regression and controls

The host suite now extracts the **actual production Variant0 and Variant1
creator bodies**. Allocation, script initialization and primary/secondary list
insertion remain explicit role seams; the two list roles are distinct. The
cursor-change callback runs inside script initialization, so the real creators
execute around it. Every creation opcode checks the resulting primary-list
membership, script42, layer15 and creation flag.

ASan/UBSan and leak detection pass the positive suite plus five strict mutants:
stale text cursor, stale destination, wrong secondary creator, skipped first
reloaded command and held input replacing pressed input. Each mutant must
produce its exact named failure. Host vptr-preserving clears, defined timer/node
prefill, omitted native layout assertions and simulated thread startup retain
their previous explicit limits. Native whole-owner comparison separately
checks the historical compiler; host C++17 success cannot establish C++03
evaluation order or native code generation.

`audit-ending-run-native.py` additionally checks the actual historical-compiler
output against the target for these four creation calls and the post-call
cursor/operand/store dependency. The earlier carrier fails both properties
in all four cases; both final carriers pass. It resolves unique PE/MAP/PDB
owners, verifies the exact target and checks register dependencies in the
dense case bodies. Its scope is these native contracts, with no parent exact
credit. The strict complete73-byte creator comparison remains independent.

```sh
scripts/repo-python scripts/audit-ending-run-native.py --directory build/match-linked/EndingLoadScript
```

Private source controls compile one Ending TU and reuse16 hash-checked support
objects. Auxiliary-ID reference, named aggregate/reference/scalar and ordinary
publication-helper alternatives remain nonexact. The helper and scalar controls
are not adopted. The file-context probe additionally compiles the real archive
dependency; it is diagnostic evidence, separate from the two full fresh colds.
Only the necessary changed/new functions and interfaces are replayed.

Product/runtime, dual-Oracle semantics and portability remain open. The known
global401100 metadata-only boundary audit discrepancy is not rewritten, and
pre-existing D3D evidence remains untouched.
