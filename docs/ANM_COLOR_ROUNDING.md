# TH10 color Hermite: conditional x87 audit

## Scope and important limitation

This is an exact rational model of observed arithmetic/store sequences, with
candidate conversion calls hypothetically bound to the actual target __ftol2.
It is NOT execution of the diagnostic probes or proof of a game-runtime bug.
The linked map verifies that candidate 0x40E148 resolves to link-anchors.obj diagnostic
external anchor `_th10_ltcg_probe_anchor_0082`, not a full CRT implementation.
An attempted 117-byte helper read exceeded its extent. Therefore the candidate
probe cannot establish real conversion behavior. No candidate-helper comparison
is needed: target-helper binding is an explicit condition of every model here.
Runtime PC/RC and reachability of the evaluated state remain unverified.

Four distinct compiled contexts are modeled:
- Target: 0x441600, 820 bytes.
- Earlier pause-linked graph: 0x409E50, 731 bytes.
- Genuine ExecuteScript-entry baseline: 0x401D90, 794 bytes.
- Natural order/call-lifetime variant: 0x401D90 in another image, 788 bytes.

Baseline and variant remain structural mismatches. Both have a separate
56-byte scale helper structurally exact against target and byte-identical to
each other, but their color evaluators inline the scaling. The experiment restored the
variant to checkpoint 0e0c15b after saving its patch. A later aggregate-return
control also failed to restore the topology and was reverted; it is not modeled.

## Conditional witnesses

Use mode 8 and zero final/both tangent vectors. Elapsed is the float value at
the evaluation point AFTER any timer step, not necessarily the call-entry value.
All arithmetic uses round-to-nearest, ties-to-even. Table entries are one
channel, repeated across RGB when all initial channels equal the stated value.

| Elapsed/duration; initial | PC | Target | Pause | Baseline | Order variant |
| --- | --- | --- | --- | --- | --- |
| 1/3; 27 | 24 | 19 | 19 | 19 | 19 |
| 1/3; 27 | 53 or 64 | 19 | 20 | 19 | 20 |
| 4/5; 125 | 24 | 12 | 12 | 12 | 12 |
| 4/5; 125 | 53 or 64 | 12 | 13 | 13 | 13 |

The baseline 4/5 witness came from a bounded scan, separately for PC53/PC64:
duration 2..32, elapsed 1..duration-1, initial channel 1..255, all ascending.
Each precision stopped at its first difference after 2,420 cases. No variant
sweep was performed, and no claim is made about unvisited cases.

## Arithmetic/store topology

Let q be the quotient rounded under x87 PC. Each operation rounds under PC;
m32 stores round to 24 bits regardless of PC. FST m32 does NOT replace ST(0).

- Target stores t=RN24(q) at 0x4417A9, doubles still-extended q at 0x4417AD,
  then stores twice to m32 at 0x4417AF. After its first scale call it reloads t
  at 0x4417CD, subtracts 1 at 0x4417D2, and stores minusOne at 0x4417E2.
  The initial basis `(twice+1)*minusOne*minusOne` is stored m32 at 0x4417F8
  before the second out-of-line scale call.
- Pause stores t at 0x409FD5 but subtracts 1 from still-extended q at
  0x409FD9, storing minusOne at 0x409FDF. It later reloads t and doubles it
  without storing twice separately; initial coefficient is stored m32 before
  its scale call. Thus the subtraction/store boundary differs from target.
- Baseline retains q and q-1 extended at 0x401EFC..0x401F13, storing only
  the completed initial coefficient at 0x401F15. Final and initial-tangent
  coefficients are also stored m32; final-tangent coefficient stays extended.
  Inlined channel scaling uses the stored initial coefficient.
- Order variant retains q/twice/final coefficient extended at
  0x401EFC..0x401F2C. After final-channel conversions it subtracts 1 from q
  at 0x401F39 and stores minusOne m32 at 0x401F3F. The initial coefficient
  is formed at 0x401F43/49/4D and NEVER stored m32 before channel FMULs
  at 0x401F53/61/6F. Initial-tangent basis stays extended; final-tangent basis
  is stored m32. Neither target's spill boundaries nor scale calls are restored.

For the 1/3 witness, target/pause t=11184811/33554432. Target minusOne is
-5592405/8388608; pause at PC53/64 gives -11184811/16777216. The target
subtraction lies exactly halfway between m32 values and ties-to-even selects
the smaller magnitude. At PC24 q already equals t, so this distinction vanishes.

At PC53/64 the 4/5 witness has:
- Target initial coefficient=6979321/67108864;
  product with 125=872415125/67108864 (~12.999998405575752), truncating to 12.
- Baseline initial coefficient=3489661/33554432;
  product with 125=436207625/33554432 (~13.000000268220901), truncating to 13.
- Order variant product with 125 at PC53=3659174806290433/281474976710656;
  at PC64=58546796900646925/4503599627370496; both ~13.000000387430195.

## Verified target helper and evidence provenance

Target 0x441E50 FILDs integer channels, multiplies by m32 coefficient and calls
0x463B2C with no intervening product store/reload. Target __ftol2 duplicates
ST(0), FSTs m32 for sign, FISTPs the duplicate to i64 under current RC,
reloads it, and uses the original extended value's residual to correct toward
zero. The m32 sign store does not replace that original value. For positive
products near 13 or 20 here, a negative residual subtracts 1 from the rounded integer.
Zero takes the zero shortcut. This full target helper has no control-word writes.
The script explicitly models this finite, range-safe path and checks truncation.

Evidence reviewed locally: color-native-evidence.txt, color-baseline-native.txt,
color-order-native.txt. Direct PE reads verified:
target 0x470AFC=0000803f=1.0; 0x470BD4=00004040=3.0;
baseline 0x40D1E4=1.0; 0x40D20C=3.0. Direct PE reads also verify order-image 0x40D1E4/0x40D20C and pause-image
0x41B840/0x41B870 are 1.0/3.0, respectively.

## Reproduction

Run `scripts/repo-python scripts/check-anm-color-rounding-model.py` or add `--scan` for the bounded search.
Only stdlib fractions and exact arithmetic are used. Explicit checks cover
both witnesses across all four contexts/three PC settings and survive python -O.
Normal and optimized runs were verified to produce identical output.
The normal-finite/range-safe model excludes exceptional x87 values and does not
model nonzero tangent output. These are conditional counterexamples, not a full
semantic-equivalence proof. Repository deliverables are this note and the script;
disassembly dumps and generated output are supporting local evidence only.

## Bounded native controls and disposition (2026-10-16)

Target SHA256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Starting checkpoint: 0e0c15ba81312c3db6f26b6d73232356028167aa.
All three fresh links use the existing four-source ExecuteScript entry:
AnmManager.cpp, RandomMath.cpp, AnmVmCreate.cpp and AnmVmId.cpp,
the pinned VC7.1 LTCG profile plus /GS. Builds were serial, nice 10,
with 180-second limits (about 66–68 seconds each). No profile sweep.

| Control | Color extent / target 820 | Raw / comparable matches | Image SHA256 |
| --- | ---: | --- | --- |
| Unmodified baseline | 794 | 34; 32/720 | cfb2378347ae61d774b9c49262157120ef21b7a3b8208c5bf9dc8a095224861e |
| Group/order only | 788 | 41; 39/720 | a8666a19e1e65380c8c6cc9b8db92e08a9291678896b642dfc21ae2da6bfdc34 |
| Value-return only | 807 | 48; 45/720 | e5961b022ff559ff8bcfa08177815ddeeb3de25a61a96dd28276d854db86bf65 |

Group/order control: introduce twiceValue=value+value; form finalWeight from
3-twiceValue; evaluate finalPart; then form minusOne, initialWeight and
initialPart; retain remaining tangent terms and sums. This did not restore
target spills or out-of-line scale calls, and was reverted.

Value-return control starts from the baseline, not the group/order variant.
Change only color Evaluate to return AnmInt3View; replace the output pointer
with a local result using the same named-field assignments; initialize both
caller-local triplets from the return. Existing arithmetic and scale order
remain unchanged. Do not use the triplet constructor: its argument order is
(x,z,y). This control still passes output in EDI, omits the target pushed result
slot, emits RET rather than RET4, and inlines scale calls. It was reverted too.
Its small diagnostic score/size change is not evidence of source recovery.

Target calls at 0x44106B and 0x44111F push a fresh local result pointer and
pass the interpolation receiver in EAX; the caller then reads stack RGB.
The baseline and value-return controls instead pass the local output in EDI.
All observed maintained callers use disjoint local results and ignore the
explicit pointer return. This makes aggregate return a reasonable hypothesis,
but does not prove it or establish equivalence for hypothetical aliasing callers.

All controls leave ExecuteScript at 9948 versus target 9964, with 1212/8564
diagnostic comparable bytes. Its contribution includes dispatch-table data;
a decoder's consumed-byte count is not a complete code/table ownership proof.
The 56-byte scale leaf is byte-identical across all three images and structurally
matches all 44 non-field bytes. Its canonical acceptance is unchanged.
No source or header change remains; no canonical manifest, metric, boundary,
receipt or exact-byte credit was changed.

Local evidence is bounded under .analysis/gpt-web/th10-resume-20261016/:
anm-color-{baseline,order,value}.json, color-order-comparison.json,
color-value-comparison.json, color-caller-context.txt, source snapshots and
the two control patches. Build artifacts are reproducible and not committed.
Do not rerun these spelling/signature controls without new evidence.
Next investigate the actual caller/LTCG graph: the pause and ExecuteScript
contexts demonstrate different inline decisions for identical source.
Do not force noinline, add dummy calls, import upstream code, or combine
unproven source shapes just to obtain a closer score.

## Real upstream root control

The maintained whole-product graph is not configured: config/build.toml has
empty source/library/resource lists, with original TU and link order unknown.
OnUpdatePrimary/OnUpdateSecondary are currently declarations and registered
callbacks, not implemented source bodies. No wrapper or arbitrary TU was added.
The actual maintained UpdatePrimaryVms calls ExecuteScript; its canonical
149-byte owner is at0x448860, with that REL32 field atoffset0x58.

A fourth serialized link changes only the real entry to
?UpdatePrimaryVms@AnmRenderManagerView@@QAEHXZ, retaining the same four TUs,
profile and unmodified source. Result image SHA256:
176f297ce386e1c4736e8622bf1c39f8779367c4414e9fa16262b35b1380443b.
Color794, scale56 and ExecuteScript9948 are all byte-identical to the baseline
contributions. The root owner's own149-byte contribution changes, as its
entry ABI is exposed, so do not generalize this to a whole-image identity.
This bounded root control does not explain the target color topology.
No further root/TU sweep is justified without additional target evidence.
Evidence: anm-color-primary-root.json and color-primary-root-comparison.json
in the same local campaign. Canonical replay/acceptance remain unchanged.
