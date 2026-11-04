# TH10 14,416-byte enemy ECL boss: real motion and bullet array addresses

Source improvement in the original Japanese TH10 v1.00a
`EnemyRuntimeView::DispatchEclInstruction` at `0x0040E770` (14,416
complete *target* bytes). The target remains **NONEXACT**. This batch
names existing typed runtime ownership in the actual large C++ function;
it does not copy target bytes or claim original class/TU provenance.

`ENEMY_ECL_ADD_OFFSET_POSITION` and `ADD_BASE_POSITION` now select
`offsetMotion.position.x`/`baseMotion.position.x` rather than hard-casting
`runtimeAddress + 0x58/0x84`. The velocity branch keeps its byte-identical
pointer arithmetic but starts from `&offsetMotion`/`&baseMotion`. The three
position words copied from the player use the real `offsetMotion.position`
float fields as 32-bit bit-copy destinations, without changing IEEE-754
semantics or the source-order stores.

Laser-request paths now copy from the correct first bullet pattern payload:
`&bulletPatterns[0].words[8]` corresponds to runtime +0x2E4. Indexed
bullet-offset zeroing/setters use the existing 12-byte `bulletPatternOffsets`
records (+0x1344) instead of decompiler-derived `i*3+0x4d1` arithmetic.
These are separate original runtime arrays, not new shadow storage.

## Selected native proof, including failed alternatives

Two successive pinned VC7.1 SP1 WPO builds use the existing 11-TU real
Enemy runtime-update carrier, with the changed dispatcher recompiled and
source hash checks on cached unchanged support inputs. The current Player
support is also recompiled because its historical source SHA changed.

| Build | Complete PDB contribution | Decoded field geometry | Normalized ordinary bytes differing from previous candidate |
| --- | ---: | ---: | ---: |
| motion/laser typed pointers | 14,352 | 716, unchanged | **0** |
| indexed offset records | 14,352 | 716, unchanged | **0** |

The comparison is against the *previous native candidate*, not the
14,416-byte original target: these results establish code-generation
preservation after substantial typed-source changes. Neither validates
full binary reconstruction nor grants any canonical exact credit.
The source shape and producer are independently evidenced by retained
PE/MAP/PDB under
`build/gpt-web/20261104-live-enemy/{motion-bullet-address-typed,indexed-bullet-offsets-typed}/`.
`motion-result.json` and `indexed-result.json` preserve source hashes,
contribution sizes and comparisons. No full-cohort replay.
