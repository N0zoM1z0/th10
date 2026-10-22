# Enemy rank selection and shared float result

2026-10-22, starting47365cb. Four private-source controls exercise the complete
large Enemy dispatcher. None is adopted. Maintained extent remains14336 versus
14416; case-aligned diagnostic3245/11128,45 target-sized groups, absolute gap328.
No source, ABI, compiler profile, ledger or exact credit changes.

## Target observation

Fresh read-only Ghidra review places rank-five selection at4118E2 (169 bytes)
and aiming at411C5A (76 bytes). Aim ends with a jump to411956, a49-byte result
store/return tail inside rank-five selection. The native baseline instead keeps
that tail inside its117-byte aim region; rank-five occupies125 bytes. These
physical case-size differences do not establish missing behavior.

Target also forms the second player draw-position pointer with ADD EDI,3C0
before its second float-argument read, then accesses [EDI+4]. Baseline leaves
EDI at the player base and accesses [EDI+3C4]. The target pointer computation
costs three more bytes, while replacing an in-place49-byte tail with a five-byte
jump saves44. This explains the baseline aim region's net41-byte excess.

## Compiler controls

| Control | Enemy bytes | Case matched / comparable | Absolute gap | Observation |
| --- | ---: | --- | ---: | --- |
| Baseline |14336|3245/11128|328|Aim117, rank-five125|
| Literal low-rank reads, preserving two result tails |14344|3252/11134|338|Aim117, rank-five128; other groups regress|
| Separate rank index local |14336|3245/11128|328|All four complete raw bodies unchanged|
| ResolveFloatArgument wrapper at result tail |14336|3245/11128|328|All four complete raw bodies unchanged|
| Second aim pointer uses drawPosition field |14340|3261/11128|332|Aim120; desired ADD and short field access recovered|

The low-rank control keeps the existing high3/high4 difficulty-label sharing;
it does not repeat the earlier all-five duplicated-tail or single-result tests.
The index and wrapper controls establish neutrality only in this exact compiler
context. In particular, the manual owner/context spelling is not necessary to
preserve current bytes; this says nothing about original source authorship.

The pointer control retains the first pointer, both existing float temporaries
and argument-read order. Unlike the older aim-block experiment it introduces no
new x/y float locals. Aim improves from23/60 to39/60 comparable positions, but
still ends in RET with its shared tail in place. Its three extra instruction
bytes plus alignment grow the parent by four. Preserve this promising local
expression for a follow-up combined tail-placement experiment; do not mistake
the score gain for complete target equality or claim other owners are raw
unchanged merely because their sizes match. No candidate repairs tail placement.

## Reproduction and validation

Private producers, frozen manifest, source overlays, Ghidra observations and
full reports remain under
`.analysis/gpt-6-astra-medium/enemy-rank-result-routing-20261022/`.
The four patches here are relative to the starting maintained source.
[evidence.json](evidence.json) records full-body hashes, all108 case rows,
artifact inventories and complete guard summaries for baseline and controls.

Each control compiles one Enemy object and reuses nine hash-verified support
objects and two harness inputs, with unchanged GL/GS profiles and serial nice10
execution. All pass36 complete guards4618/all121, including the194-byte direct
ANM caller. ECL remains7020/6993 with its171-byte float callee exact; ANM retains
9964 and7541/7995 case diagnostics. No whole-product or runtime claim follows.
Disk/toolchain/tracking preflight and fresh read-only Ghidra attestation passed;
all412289 mapped target bytes retain SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

All maintained source inputs, canonical artifacts/recipes and nine unrelated
D3D files were hash-verified unchanged before documentation edits. Retain the
pointer carrier for follow-up inspection; three other carriers were individually
hash-verified and removed:48 files,3199750 bytes. Compact reproduction inputs
remain. Full CI was omitted under the operator's focused-replay preference.
