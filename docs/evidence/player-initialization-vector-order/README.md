# Player initialization: ordered vector-value construction

2026-10-23, starting ced216b. The complete1435-byte PlayerInitialize4247F0
improves from1355 matching/80 different bytes to **1359 matching/76 different**,
after all49 independent address bindings. Four ordinary bytes are repaired with
no target-equal losses. No small-function target mapping or exact credit is added.

The [timer-commit successor](../player-initialization-timer-commit/README.md)
now reaches1370/65. This packet's1359 source pins and commands are historical.

## Source and full comparison

[The patch](maintained.patch) constructs the actual extent-plus-position value
in component order through an ordinary internal helper. Its real Y-operand read
uses a scoped volatile view; the returned vector's three components are all used.
Automatic inlining gives the target extent-Y load followed by position-Y addition.
The previous scalar expressions commute those two operands. No extra call appears
inside the compared parent, and no fake return, ABI coercion, dummy store or
source padding is introduced. Original qualifiers/helper structure remain unknown.
The zero-context source patch applies only at the pinned starting source; use
`git apply --unidiff-zero` when replaying it.
Headers, layouts, declared interfaces, canonical recipes and exact ledgers stay
unchanged. Four compiler barriers and the earlier owner capture are retained.

[The residual](parent-differences.json) retains every remaining ordinary/local
byte difference and all49 bindings. Nothing is cropped or masked. Prefix37,
early-timer21, vector-region7 and tail11 differences remain. The wrapper still
uses the unlike private receiver; a source helper match is not the objective.

Eight complete shared guards pass1753 bytes/all99 fields. Private and separately
recompiled maintained Player objects reproduce all519 complete public PDB owners
raw,140370 bytes. Only one Player object is rebuilt for maintained reproduction;
16 hash-pinned inputs are reused. No independent whole-graph cold pair, new exact
acceptance, native product or runtime validation is claimed.

## Address movement is distinct from ordinary code changes

[The public-owner comparison](public-owner-reproduction.json) records a useful
scope correction. The /OPT:NOREF diagnostic image also retains an out-of-line
copy of the ordinary static helper at418D00; later code moves by40h. This copy
has no target mapping or exactness credit. Old-versus-new raw bytes differ in160
public owners. For159, every difference lies within authoritative PE DIR32 or
decoded external REL32 field extents in BOTH images; only PlayerInitialize has
ordinary differences. The initial stronger raw-equality assertion is rejected,
not silently treated as a pass. This classification does not bind every semantic
destination and is not a target-wide exactness proof. The eight independent
full target guard contracts and complete parent replay remain the exact checks.
Static/unowned fragments and the non-product diagnostic layout stay out of scope.

## Executed controls

[Evidence](evidence.json) binds18 complete images/18 new object compilations.
Every parent remains1435 bytes and has all49 independent fields replayed;
every image passes all eight full shared guards.

| Control | Complete match /1435 | Outcome |
| --- | ---: | --- |
| Ordered first calc flags read / read-write view |1182 /1172|Regressive; do not combine by assuming independent gains |
| Finish flags snapshot before captured owner with a compiler barrier |1194|Regressive; short-load allocation is lost |
| Five alternate first-timer store orders |1351,1352,1349,1349,1354|Changes stay in the timer region; none improves the21-byte residual |
| Canonical timer-scale global identity / actual Player G5 profile |1355 each|Entire replay neutral; CPU flag is observed in the compile profile |
| Ordered Y in a direct constructor / returned constructor value |1338 /1341|The read moves too early; regressive |
| Component-order real return value with ordered Y |1359|Selected; four repairs/no losses |
| Ordinary inline declaration; named ANM/POD subtraction; timer-value helper |1359 each|Whole-parent equal to selected; not adopted as additional changes |

The ordinary inline declaration does not eliminate the retained diagnostic copy.
Two refused preliminary invocations neither compile nor overwrite an image:
one finds an occupied old build directory, the other rejects historical source
pins after maintained code changes. Current producers and artifact paths are
explicit; historical receipts retain their original paths and hashes.

## Recovery, cleanup and validation

Current producers: `.analysis/gpt-6.1-sol/player-residual-publication-20261023`.
Use manifest-final.json, common-final.py, probe-maintained.py and bound-parent-final.py.
The maintained carrier is under the corresponding new build campaign. The
selected max0-ordered-named-result private carrier is under the older
player-stage-caller build campaign; its actual context is recorded, not inferred
from directory names. The earlier1355 carrier remains an unchanged support cache.

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-residual-publication-20261023/bound-parent-final.py maintained-17
```

Expected mismatch1359/1435,49 fields. Never restore old source over src/ or weaken
historical input pins. The exact prechange Player source is retained privately.
[Cleanup](cleanup-summary.json) losslessly archives closed native controls and
private source proposals after verifying every member's length/SHA-256. Current
selected/maintained carriers, required old caches, target, tools, Wine, database
and all nine unrelated D3D files remain. Restore archived ignored paths as needed:

```sh
tar -xJf .analysis/player-vector-order-cleanup-20261023/completed-controls.tar.xz -C .
```

[Validation](validation-summary.json) records the focused scope. Full CI and broad
cold replay are omitted under the operator preference. Authored409/74716 and
overall1252/141647 exact counts do not change; product/runtime and later gates
remain open. The campaign is active-incomplete.
