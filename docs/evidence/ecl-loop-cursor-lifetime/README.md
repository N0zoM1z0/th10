# ECL loop-condition and body instruction snapshots

2026-10-22, startingebe947c. Four current-context source controls leave maintained
Run at7020 bytes and27 ordinary differences. No source, compiler profile, ABI,
recipe or exact ledger change is adopted.

## Observed target and compiler behavior

Fresh target review shows the null/time-check path loading instruction into EAX
at Run+B, followed by FILD[EAX] at+26. Only afterward is EDI saved. The loop body
begins at+40 with a separate MOV EDI,[EBX]. The maintained baseline reproduces
this prefix, including the intervening alignment. The remaining late loop-latch
register differences do not establish that these two observations should become
one source-level snapshot.

| Source control | Complete Run extent | Strict Run result |
| --- | ---: | --- |
| Carry current in for initializer/update; refresh on continue |7004|Complete extent rejected|
| Assign current in while condition; use it in body |7004|Complete extent rejected|
| Read current at top of infinite loop, then test/break |7004|Complete extent rejected|
| Name conditionInstruction, retaining separate body current read |7020|Same27 differences; all three large raw bodies unchanged|

The for update executes after ordinary advancement and after StartSubroutine's
continue, so the trial does not accidentally retain the previous subroutine
instruction. The top-test form uses !(time <= currentTime), preserving unordered
comparison behavior. None introduces a new null guard, padding, target bytes or
forced register/ABI annotation.

The three unified-snapshot controls all save EDI earlier, load instruction into
EDI before the null/time tests, and omit the separate body reload. Their body
begins at+30 rather than+40, with a changed early-return arrangement. These
changes extend beyond the desired latch register choice. The7020-byte Oracle
rejects each7004-byte contribution; no cropped or shifted target score is used.

The condition-assignment and top-test Run bodies are raw-identical. The for
variant differs from them at exactly one byte,+9D: MOV EAX,[EAX+EDI] versus
MOV EAX,[EDI+EAX] at+9B. This is a peer observation on rejected7004-byte bodies,
not a repaired target byte. ANM and Enemy have identical complete raw bodies
across these three controls; this does not assert equality to their baseline
relocated bytes.

The final isolation control preserves both source observations and merely names
the condition pointer. Complete Run7020, ANM9964 and Enemy14336 are each
raw-identical to the current canonical carrier. Run's27 target-different offsets
and both byte values agree with the fresh baseline; all189 fields/62 local
bindings are checked and the complete171-byte float callee is exact. The tested
condition-pointer declaration therefore does not explain the residual register
choices. These results are context-specific, not proof of original declarations.

## Validation and reproduction

Private producers, manifest, source copies, target review, full reports and
peer-byte review are under
`.analysis/gpt-6-astra-medium/ecl-loop-cursor-lifetime-20261022/`.
Patches here apply to maintained source at the starting commit.
[evidence.json](evidence.json) records current owner hashes, source/producer pins,
actual reused inputs, strict rejection reasons and inventories.

Fresh disk/toolchain execution/tracking and direct Ghidra attestations pass.
Target original Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
All412289 mapped bytes retain
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra uses rollback, readOnly/noanalysis, one CPU and768 MiB heap. Builds are
serial nice10, compiling only ECL and reusing nine hash-verified game objects
and two harness inputs. GL/GS profiles are unchanged. Guard descriptors are
checked against current canonical entries before private-path substitution.

All four producers finish exit0 and pass nine complete ECL guards1137/all18,
including the float-value owner. Their successful completion does not turn the
three recorded Run extent rejections into passes. Maintained source, canonical
artifacts/recipes and nine unrelated D3D files remain hash-unchanged. All four
new carriers were verified and removed:64 files,4266176 bytes. Keep compact
reproduction inputs and the current canonical baseline. Focused checks replace
full cold replay/full CI under operator instructions. Whole-product/runtime,
semantic and portability closure remain unproven; exact counts do not change.
