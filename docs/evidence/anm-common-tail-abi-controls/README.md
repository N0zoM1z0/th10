# ANM common-tail alignment and source ABI controls

Date: 2026-10-22. Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting commit: `ca13d551d26ad977612f18541ce7c7187c1e32a0`.

The complete `ExecuteScript` owner remains nonexact: target `43EE30..44151B`,
9,964 bytes. No maintained C++ source, header, interface, canonical recipe or
exact row changes. The ledger note qualifies the maintained source view without
changing its signature, machine ABI, boundary, origin or source mapping.
[Facts and source patches](facts.json) record the bounded observations;
[validation](validation-summary.json) records the completed checks.

## Target observations

The common case-2 span is 1,441 target bytes versus 1,440 baseline bytes. Its
actual backward conditional loop branch identifies the loop entry; a preceding
forward jump skips the intervening instruction fill. This gives the following
complete length accounting:

| Portion | Target | Baseline |
| --- | ---: | ---: |
| Before skipped fill | 960 | 960 |
| Skipped fill | 13 | 12 |
| Loop entry through span end | 468 | 468 |

Target JMP `4411C1` reaches `4411D0`, skipping `4411C3..4411CF`. Baseline JMP
`40FD42` reaches `40FD50`, skipping `40FD44..40FD4F`. Both destinations are
16-byte aligned. The alignment purpose is an inference from this control flow;
the skip distances and instruction bytes are directly observed. Both function
jump tables begin at offset 9,588. The one-byte case-span length difference is
entirely in the skipped fill, which remains owned and compared for exactness.

An address-omitting instruction navigation view, after removing only this
measured skipped fill, has 372 instructions on each side. Its sole differing
pair is the slow timer's FLD/FADD operand order: target loads the scale first,
baseline loads the subframe first. This is navigation only: control-flow and
absolute addresses are omitted, and it supplies no target equality credit.
An initial first-RET locator was rejected because that RET is an early return;
the corrected loop-edge method supersedes it.

FlipX case 61 uses EAX in the target, allowing a five-byte accumulator XOR.
Baseline uses EDX and a six-byte XOR, accounting for its 53 versus 52 bytes.
This machine observation does not identify an original source expression.

Fresh local Ghidra review covers the existing entry and incoming references,
FlipX and the complete common tail. Read-only/no-analysis execution rolls back
with one CPU and 768 MiB heap. All 412,289 mapped text bytes retain SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra signatures, types and body ranges remain provisional and unpromoted.

## Actual compiler controls

The baseline has 9,964 bytes, aligned diagnostic score 7,541/7,995, 90/92
equal-sized groups and aggregate group-size gap 2. These are diagnostic counts,
not complete function equality or accepted case-level exactness.

The first three controls freshly compile only the ANM GL input and reuse nine
hash-pinned source objects. Their source patches and producer hashes are in
`facts.json`; none is a full ten-source cold build.

| Source hypothesis | Bytes / aligned score | Whole-body comparison to baseline |
| --- | --- | --- |
| Raw-word compound FlipX operations | 9964 / 7541 of 7995 | Four ordinary radial Y/Z operand-order bytes change; FlipX remains EDX |
| Scale-initialized slow-timer accumulator | 9964 / 7541 of 7995 | Raw-identical |
| TU-local postfix timer forwarding to Tick | 9964 / 7541 of 7995 | Identical after independently replaying 291 changed address fields |

The first control demonstrates why equal size and case scores do not establish
baseline neutrality. The timer header already spells scale plus subframe, so
the accumulator control tests a materialization boundary rather than a missing
source operand swap. The postfix control changes no class definition; it tests
a forwarding layer, not the provenance of a free versus member operator.

Two additional, distinct private source-interface hypotheses each freshly
compile all ten game-source inputs, serially at nice 10. Each uses a consistent
32-file source/header overlay with a closed quoted-include graph. All ten GL/GS
profiles are retained, including the existing scale-support Ob1 setting. Only
the two pinned harness files are reused. The first hypothesis changes two
source/header roles; the second changes three and uses existing real manager
receivers at its six qualified call sites. They are distinct hypotheses, not a
canonical replay or a same-candidate pair of acceptance cold builds.

| Source-interface hypothesis | ANM body | Complete exact direct caller |
| --- | --- | --- |
| Default static calling convention | 9948 bytes; 6770/7995; four plain RETs | 197 versus target 194: rejected |
| Ordinary member function | 9964 bytes; 7541/7995; four RET4s | 200 versus target 194: rejected |

The default-static body still receives the VM through the stack. The member
body is baseline-identical after independently replaying 287 changed address
fields. That equality does not repair its caller. The complete existing
`AnmLoadedView::SetAndExecuteScriptIndex` contract at `43E7E0`, 194 bytes/all
three fields, passes for baseline and the first three controls; it rejects
both interface hypotheses at their independently derived PDB extents. Neither
interface change is adopted.

Changed-field replay requires either the same destination offset inside the
owned function or independently identical qualified MAP symbol names. No
changed field remains unresolved. This proves only full-byte peer equality
under those bindings, with all fill retained; it is not target equality,
original source ABI or runtime validation.

Each of all five images passes the same 35 complete linked guards, 4,424
bytes/all 118 fields. Five of these are additional linked guards of existing
normal-COFF contracts; their canonical artifact kinds remain unchanged. The
194-byte caller is a separate additional check. Only the first three controls
also execute the eight bounded child-path checks.

ECL remains 7,020 bytes, 6,993 matching bytes and 27 ordinary differences, with
all 189 fields/62 local destinations and the complete 171-byte float callee
exact. Coverage is 7,019 decoded bytes plus the reviewed one-byte selector;
the generic normalization-complete flag remains false. This is preservation of
the retained nonexact baseline, not a new exact claim.

## Source interpretation and checkpoint limits

TH09 suggests a member executor; provisional TH095 suggests default static.
Both suggest a member postfix timer forwarding to Tick. These disagreeing
adjacent sources are hypothesis material only. They transfer no TH10 address,
layout, source membership, calling convention or exactness claim. Historical
TH10 source also contained a default-static form; its current measured failure
does not prove the original source form globally.

The maintained static-stdcall declaration is a boundary view that preserves
the current exact caller. RET4, a stack VM argument and even peer-identical
executor bodies do not prove original static/member membership or source
calling convention. Other caller sizes are observations only, not new matches.

All 108 maintained sources, the 32 large-source/header pins and 15 baseline
artifacts remain unchanged. Of the initial 373 source/test/config inputs, only
the deliberately qualified `functions.csv` note changes before the final CI
snapshot. Exact totals remain authored 409/74,716 bytes and overall
1,250/141,226 bytes. Source remains 613, nonexact 204 and absent 121.
Private producers pin the historical pre-note inputs. Review and adapt those
pins for a future reproduction; never restore current source or ledger notes
merely to satisfy an old experiment manifest.

Hash-checked cleanup removes 50 superseded owned files, 3,129,383 logical bytes:
the first three native carriers and two superseded locator reports. Retain
both distinct ten-source ABI hypotheses and their consistent source overlays,
the canonical baseline, compact producers/source patches/results and corrected
loop evidence. Nine independent D3D files remain untouched and unstaged.

Exact reconstruction remains active-incomplete. Whole Windows i386 product
and runtime closure are open; semantic reconstruction and portability have not
started. Original physical source/TU, production flags, data owners and source
ABI remain unknown. Rotate away from these measured forms to a distinct
operand-lifetime or linked-context hypothesis.
