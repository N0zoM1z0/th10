# Enemy sentinel comparison lowering

2026-10-22, starting553e118. Two equivalent operand-order changes in the large
Enemy dispatcher remove redundant x87 stack copies. The target groups
0x121/0x123 and0x12a/0x12b use direct FCOM where the previous candidate used
FLD ST(0) followed by FCOMP. Writing the float operand on the left recovers
those target forms. The negated condition remains negated, preserving unordered
comparison behavior; it is not rewritten as a less-than-or-equal test.

The maintained changes are only `!(value > -999999.0)` and
`value > -999999.0` at the two reviewed sites. No helper, field layout, profile,
ABI, canonical recipe, or target byte changes. **No new exact credit**.

## Measurements and limits

| Control | Parent /14416 | 0x121 group /525 | 0x12a group /92 | Aligned bytes | Absolute group gap |
| --- | ---: | ---: | ---: | ---: | ---: |
| Previous maintained | 14352 | 532 | 95 | 3251/11134 | 336 |
| Two operand-order changes | 14336 | 530 | 93 | 3245/11128 | 328 |
| Local initialized const opcode only | 14352 | 532 | 95 | 3251/11134 | 336 |
| Fresh maintained-path rebuild | 14336 | 530 | 93 | 3245/11128 | 328 |

The total size deficit grows from64 to80 bytes even as the two instruction
sequences and aggregate group sizes improve. Whole-size distance and aligned
scores are diagnostics, not the acceptance criterion. All181 selector bytes,
108 ordered physical groups,45 target-sized groups and2C4 frame remain.
Other size changes: the spell group226 ->215 against219 and the default final
region26 ->25 against24. Every changed group moves closer in size, but neither
size nor an FCOM match proves the remaining registers, scheduling, fields,
control flow or whole owner exact.

All19 absolute eight-byte sentinel references were read from both actual images
and verified as double -999999.0. Their ordered FCOM/FCOMP forms now agree in
all seven physical groups. At the two changed sites the target uses test AH,41;
the negated condition branches JE and the positive condition JNE. Candidate
0x121 still schedules unrelated register loads/stores between FCOM and FNSTSW,
and the two groups retain register and stack-location differences.

A separate natural-source control moves the hoisted short opcode into a const
local initialized at its first use. All four complete dispatcher/ANM/ECL/factory
raw bodies remain identical to the old baseline. It is not adopted. Current
prologue inspection finds target packet ESI/opcode EAX versus native packet
EDI/opcode ESI; historical register notes describe their older compiler contexts.
Changing this declaration alone does not repair current register allocation.

## Validation and reproduction

Target original Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh disk, executed pinned VC7.1 toolchain and direct read-only Ghidra checks
pass. Ghidra uses one CPU/768 MiB, no analysis and a rolled-back transaction;
all412289 mapped text bytes remain unchanged, hash
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

Each private build and the maintained-path rebuild compiles one primary object
serially at nice10 and reuses nine hash-verified support objects and two harness
files. This is not a full cold pair. The selected private and maintained rebuilds
produce raw-identical complete dispatcher/ANM/ECL/factory bodies. Each passes36
complete guards4618/all121. The maintained image additionally passes both
existing laser constructors202/219 bytes and four fields:38 complete guards,
5039 bytes and125 fields. ECL stays7020/6993,27 differences, all189 fields/62
local destinations and exact171-byte float callee. ANM stays9964 with7541/7995,
90/92 target-sized groups. Full CI was not run, per operator preference.

[Evidence](evidence.json) contains input hashes, compiler controls, whole-body
hashes, size transitions and all19 sentinel forms. Patches are relative to the
starting source. Private producers/manifests/reports are under
`.analysis/gpt-6-astra-medium/enemy-sentinel-lowering-20261022/`.
Before adoption,19 old native files were archived under
`build/gpt-6-astra-medium/enemy-sentinel-lowering-20261022/baseline-before-adoption/`
and32 old source/header inputs under private `baseline-source/`. The baseline
report's original canonical pathname now contains the new build; its original
bytes were verified in that archive. The original manifest is historical; the
validation manifest precedes the notes-only ledger update.

The rejected opcode carrier was hash-inventoried and removed:16 files,
1066539 bytes. Keep selected private carrier, old archive, current canonical
image and compact reproduction inputs. Nine unrelated D3D evidence files remain
untouched. Product/runtime gates and exact/origin/boundary ledgers are unchanged.
