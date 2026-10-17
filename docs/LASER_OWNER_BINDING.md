# Laser owner binding repair

Target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The laser owner at `0x47781C` and bullet owner at `0x4776F0` are distinct.
The former allocates `0x45C` bytes. Its common sentinel occupies `+10..433`;
first live node `+18` is sentinel.next. `+434` is the append tail, followed by
count `+438`, next ID `+43C`, cancellation position `+440`, and ANM resource
`+458`. Constructor41C290, initializer41C120, insertion41C510 and cleanup41C1C0
jointly establish this identity.

Nine dispatcher sites incorrectly used the bullet pointer: four laser creation,
two clear, two pattern cancel and one wait. All ten such sites in EnemyLaser.cpp
were wrong: six split/reflection creation and four effect-resource reads.
Those nineteen operands now use the existing canonical g_EnemyLaserManager.
Six already-correct lookup calls and all genuine bullet uses remain unchanged.
Three GameManager chain-prefix users now explicitly cast this same pointer;
the independent g_GameOwner47781C declaration is removed.

EnemyLaser.hpp shares the existing lookup and known-virtual-prefix declarations
unchanged. The common storage and pattern declarations also move unchanged.
The full storage view embeds that genuine common sentinel and names the tail
correctly. Narrow lookup, resource, virtual-call and chain views remain explicit
boundaries over one allocation, not separate owners. Factory parameter type,
helper signatures and bodies, real type0/type1 extents, and all three external
19-entry vtables are preserved. No new production function is introduced.
Unknown virtual contracts and unknown storage bytes remain unknown.

## Verification boundaries

`test-laser-owner-binding.py` extracts actual corrected owner operands/casts and
real wait/clear/pattern bodies. Distinct live host owners/resources distinguish
bullet from laser routing. These are bounded host contracts, not execution of
complete dispatcher/collision owners or native virtual tables. Factory allocation/
append behavior and pointer-returning FindLaser are not host-executed here;
the factory body differs only in the head-to-tail field name, while FindLaser
is unchanged. Mutations must
compile and fail precisely their intended CHECK; arbitrary nonzero exits do not
count. Existing teardown tests cover the updated canonical chain identity.

The shared header's i386 assertions cover full size and offsets. Native compiler
verification and canonical replay remain separate requirements. Unchanged exact
lookup/wait/clear bodies never proved their callers used the right global.
Only the real bullet-runtime-update canonical unit has bullet-global DIR32
bindings (two, both4776F0); no normalization record needs remapping.

EnemyLaser.hpp has no header includers. Its two original source consumers plus
the new GameManager include affect38 manifest units across16 linked producer
groups and one normal ECL compile group. Include support-source closures for
EclVm, AnmVmId, bullet, frontend and hint units; preserve all existing profiles.
No bytes-exact, target-runtime or product-closure gain is claimed by this repair.
The message constructor and its missing enemy helper are deferred.

## Integrated checks

Applied after checkpoint 878e399 on 2026-10-17. Repository .gitattributes restored CRLF for four C++/header files; all line-ending-normalized hashes match the independently reviewed proposal. The first native compile exposed an include-order conflict: EnemyLaser.hpp now reaches AnmManager.hpp before the old Main.hpp position. Moving the existing Main.hpp include first restores its established Windows SDK import shielding, with no body, compiler-profile or source-graph changes. The failed compile log was retained; it is not a validation pass.

Remote O2 and ASan/UBSan checks passed at 03:28 UTC: 22 strictly classified binding/traversal mutations, complete real-header GNU i386 layout diagnostics, and the updated caller fixture with 70 cases/50 strict mutations. These host models do not execute factory allocation or native virtual tables.

One fixed native retry completed at 03:31 UTC: PE SHA256 0814a44537ced060981af33f7cc4efd94c8a9fe2375c9950e0241db4cca1bc6d. Reproduce on a fresh dedicated output with scripts/repo-python scripts/diagnose-laser-owner-binding.py; --check-only verifies the pinned canonical graph/profile without building. It uses the real ECL dispatcher plus established EclVm/AnmManager/AnmVmCreate supports and real EnemyLaser.cpp. Do not execute the resulting anchored image. Independent per-site native dataflow review is complete as detailed below; grouped existing-unit regressions in LASER_OWNER_BINDING_UNITS.json subsequently passed. No new exact-byte credit or runtime binding is implied.

### Compiled laser-owner routing diagnostic

The fixed dispatcher-plus-real-laser graph was built after moving the dispatcher's existing `Main.hpp` include before the new `EnemyLaser.hpp` include. This restores `Main.hpp`'s existing ANM/Windows import shielding; no function body, compiler profile or support graph was changed for that correction. Reproduction is defined by `scripts/diagnose-laser-owner-binding.py`: the established dispatcher entry, EclVm/AnmManager/AnmVmCreate supports, real EnemyLaser support, `/GL /GS`, and the existing unresolved-symbol data-anchor harness. The diagnostic image must not be executed.

The inspected PE SHA-256 is `0814a44537ced060981af33f7cc4efd94c8a9fe2375c9950e0241db4cca1bc6d`. Map/PDB identity agrees with the image's timestamp, base and CodeView GUID/age. All five recorded source hashes match the inputs. Six required caller contributions are retained with unique map/PDB ownership:

| Caller | Diagnostic address | Extent | Producer |
|---|---:|---:|---|
| Dispatcher | `402760` | 14480 bytes | `source.ltcg.obj` |
| Laser boundary | `413C00` | 486 bytes | `support-03-EnemyLaser.ltcg.obj` |
| Type-0 box | `413DF0` | 1161 bytes | Same laser object |
| Type-0 circle | `414280` | 1366 bytes | Same laser object |
| Type-1 box | `4147E0` | 1029 bytes | Same laser object |
| Type-1 circle | `414BF0` | 1237 bytes | Same laser object |

The image has one canonical `g_EnemyLaserManager` symbol at diagnostic cell `417190`, a distinct `g_EnemyBulletManager` at `41714C`, and no old `g_GameOwner47781C` symbol. Each cell is separate anchor storage. All nineteen corrected source routes were traced to the laser cell: four ECL factory routes, two clears, two pattern cancels, one wait, six boundary/collision factory routes and four resource reads. Six existing lookup routes also use that cell. The genuine dispatcher bullet-spawn receiver loads the separate bullet cell; its callee remains an intentional anchor.

Source-site counting differs from relocation counting. The two ECL type-0 groups merge into the same tail: the first group's OR-one path branches from `405126` to `40544C`; the second group's AND-not-one path falls through from `405449`. Both load the laser cell at `405454` and call the real factory at `405461`. Six other source factory paths inline recognizable count/ID/tail insertion operations using the loaded laser owner. These transformations do not obscure the receiver binding. The five laser caller extents contain ten laser-global fields and no bullet-global fields; the dispatcher contains fourteen laser fields and one bullet field.

All five laser callers decode completely. The dispatcher's first 13824 bytes decode through `405D5F`. Its remaining 656 bytes are proven switch data: 108 DWORD targets at `405D60`, 181 byte indices at `405F10`, three alignment bytes, and two five-DWORD tables at `405FC8`/`405FDC`. The dispatch instructions reference these exact ranges; treating the tables as instructions caused the initial linear-decoder shortfall.

FindLaser `401790` (22 bytes), wait `4018C0` (44), clear `401950` (44), pattern cancel `4018F0` (85), factory `415210` (153), both constructors and the resource callees have real matching contributions. Static map symbols explain traversal helpers omitted by a public-only inventory.

**Bounded result:** compiled owner/resource routing is verified for nineteen corrected routes, six lookup routes and one bullet receiver. The stronger indirect-callgraph gate remains **INCONCLUSIVE**: anchored vtables do not establish concrete slot-1/5/7 destinations. This establishes neither target `47781C`/`4776F0` placement, executable storage/lifetimes, runtime behavior nor byte exactness. GameManager is outside this graph. The separate 38-unit regression run subsequently passed, as recorded below.

## Final regression and additional caller check

On 2026-10-17 at 03:55 UTC, all 38 existing units across 17 serialized cold producers passed: 11,331 bytes remain exact, with unchanged canonical profiles. These are preservation checks, not fresh Factory acceptance or new-byte credit.

Read-only inspection of the already-built GameManager regression artifacts also verifies all three canonical laser-prefix routes. Both retain complete ResetGameStage (4010C0, 456 bytes) and GameManagerCleanup (401D00, 1138 bytes). Loads at 4010FD, 4011EB and 401F1C resolve the single laser cell 404124, feeding linked-object clearing, chain flags and saved-pointer cleanup/release. No old g_GameOwner47781C symbol remains. GameManagerOnDraw PE SHA256 is 3d4f7789035157964cb6641491992f26ecb3f5b7c8acedc5492eb73b3e8a1d2f; GameManagerUpdateCallback-cold-2 PE is 88c932d0b89b583c6001f3129de37d401d1ec2810d0773c6083079790e0f1e07. Their map/PDB identities and full owned extents were independently checked. This extends source/host checks to compiled routing in these separately identified artifacts; anchored callees, runtime storage, physical target placement and whole-owner exactness remain unproved.

Final target-independent CI passed on 2026-10-17 at 03:56 UTC, including the new binding baseline hook and unchanged canonical graph checks. No match-unit bindings or accepted exact claims were changed by this correction.
