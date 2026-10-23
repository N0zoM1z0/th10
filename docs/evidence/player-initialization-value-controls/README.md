# Player initialization: value forms and current caller contexts

2026-10-23, starting370637d. This batch makes no maintained game-source, header,
ABI, profile, recipe or ledger change. The complete1435-byte PlayerInitialize
remains **1380 matching/55 different bytes**, after49 independent bindings.
No small-function mapping or exact credit is added. The campaign is active-incomplete.

## Completed controls

[Controls and hashes](controls.json) record16 complete linked images/15 new
object compilations. Twelve parents are1435 bytes and replay all49 fields;
four1443-byte parents fail the strict size check. No candidate is cropped.
All16 images pass eight complete shared guards1753 bytes/all99 fields.

| Source/control | Full parent outcome |
| --- | --- |
| Flags pre-masked before owner capture | Whole1380 replay unchanged |
| Flags read through actual memcpy value |1207/1435;regressive |
| Unsigned character / shot / both | Whole1380 replay unchanged,three images |
| Two-int selection record |1344/1435;regressive |
| Direct / staged scalar word copies of two real vectors |1443 each;strict size rejection |
|32-bit character/shot enum views | Whole1380 replay unchanged |
| Real flags getter / real ordered-owner getter / both | Whole1380 replay unchanged,three images |
| Real const-reference three-word copy helper, POD / ANM destination |1443 each;strict size rejection |
| Current source in27-input real-caller graph | Whole1380 replay unchanged |
| Same27 inputs,actual maintained WinMain entry | Whole1380 replay unchanged |

Ten complete images reproduce every baseline byte; two target-sized images
regress and four sizes are rejected. Width, enum and getter neutrality does not
establish original signedness or source types. No fake return, forced register
ABI, source padding or target-byte patch is used. Original classes/TUs and
runtime storage ownership remain open; source-value shuffling alone is insufficient.

## Current graph and entry proof

The [reuse proof](caller-reuse-proof.json) checks all26 unchanged reused source
hashes,50 historical header hashes and tracked source history since the older
caller graph. Only Player differs from that graph, and it is freshly compiled
from current1380 source. The same27-input actual-entry experiment reuses all27
objects and adds no compile. Both compare the complete1435-byte parent/all49
fields and all eight guards; neither is an independent cold pair.

The linker selector WinMain@16 resolves the actual PDB-owned _WinMain@16 in
Main's object. A preliminary request using the public spelling _WinMain@16
was refused as unresolved before any image. Its partial output is archived;
no data anchor supplies the entry. The corrected label uses a fresh directory.
The console diagnostic harness and unresolved non-entry anchors remain;
this is not a reconstructed product or validated startup/runtime.

These controls extend the earlier1195-context caller result to the current1380
source rather than assuming it transfers. Target call-site coverage, original
ABI and the200-byte nativeEDX/202-byte targetESI wrapper remain unresolved.
The maintained17-input graph remains the current source reference.

## Reproduction and cleanup

Producers: `.analysis/gpt-6.1-sol/player-value-lifetimes-20261023`.
Its manifest.json/common.py bind unchanged1380 source; historical pins are immutable.
Both expanded27-input carriers can be checked without rebuilding:

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-value-lifetimes-20261023/bound-parent.py current-callers-27
scripts/repo-python .analysis/gpt-6.1-sol/player-value-lifetimes-20261023/bound-parent.py current-winmain-entry-27
```

Expected complete mismatch1380/1435,49 fields. Do not restore historical source
into src/ or weaken pins when later source changes.
[Cleanup](cleanup-summary.json) archives14 closed images/14 private source folders
and one refused-entry partial tree:406 files/29.93MB become0.48MB,reclaiming29.45MB.
Every member is length/hash-verified before removal; all25332 protected hashes
remain unchanged during compaction. Current17/27-input carriers, compact reports,
producers, source witnesses, target, tools, Wine, database and all nine unrelated
D3D files remain. Restore required ignored paths before historical consumers:

```sh
tar -xJf .analysis/player-value-controls-cleanup-20261023/completed-controls.tar.xz -C .
```

[Validation](validation-summary.json) records focused Oracles and repository
checks. Full CI and broad cold replay are omitted under the operator preference.
No exact, product, runtime, semantic or portability credit changes. Next seek
new large-owner/ABI evidence rather than repeat these source-value variants.
