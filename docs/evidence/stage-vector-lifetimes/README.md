# Stage dispatcher vector lifetimes and initializer visibility

2026-10-23, starting fd366f2. Six actual source controls test the complete
1868-byte StageExecuteScript owner. All are neutral against the maintained
parent; no source, profile, recipe or ledger changes are adopted.

## Observed results

Fresh readOnly/noanalysis Ghidra review covers spline 10/11,12/13,14 and the
common tail. The complete mapped 412289 bytes retain SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f,
with rollback, maxcpu1 and heap768m. Target and native spline stack homes agree,
but several EAX/ECX/EDX choices differ. These observations motivate the source
controls; they do not establish original source types or translation units.

| Actual source control | Complete maintained-parent comparison |
| --- | --- |
| One shared spline vector triple inside the script loop | All 1868 raw bytes equal |
| The same triple outside the loop | All 1868 raw bytes equal |
| One final vector shared by linear 3/5 and spline 10/11 | All 1868 raw bytes equal |
| Explicit opcode 14 script/slot/resource/VM receiver locals | All 1868 raw bytes equal |
| Move the existing VM initializer definition into Stage's TU | Equal after 3 independently bound address fields |
| Initialize the six spline vectors with explicit x/y/z assignments | All 1868 raw bytes equal |

Each image retains frame 8C, case 1443/1619,13/15 target-sized groups and gap 2.
Complete 15-entry table/domain/order checks and all 11 whole guard contracts
987 bytes/all 36 fields pass in every image. Peer comparisons require equal
complete extents and exhaustive field inventories; only matching semantic
symbols or identical local offsets authorize address replay. No ordinary byte
is masked. Baseline equality is not target equality.

The five primary-only controls compile one actual StageScript object and reuse
eight pinned game objects. The definition move compiles both StageScript and
AnmManager, reusing seven. The method body is preserved verbatim after newline
normalization; shared declarations and /GL /GS profiles stay unchanged. These
are seven actual game compilations and six links, not a same-candidate cold pair.
All native jobs terminate0 and run serially at nice10.

## Remaining constraint and closure

The moved initializer remains the complete 200-byte native body versus target 202.
Its two changed CALL fields independently bind the same callees; replay leaves
all 200 bytes equal to baseline. Opcode14 still supplies native EDX versus target
ESI for the VM receiver. Changing definition visibility did not repair that ABI
or the Stage registers; no callee exact claim is made.

The unchanged canonical Stage image freshly passes 11 complete guards 987/all 36
and retains parent SHA-256
c1984eb7ab1703cc428d6659507d841f3ae7e7ebe04cfa094e8f07bd18685b26.
The [evidence](evidence.json) binds actual source shapes, profiles, object hashes,
complete parent/child peer comparisons, sessions and final canonical replay.
Adjacent patches preserve the transformations. All 90 neutral native files,
4058006 bytes, are hash-verified then removed; see the
[inventory](cleanup-inventory.json). Private source copies, producers, small
reports and target review remain expanded below
`.analysis/gpt-6.1-sol/stage-vector-lifetimes-20261023`.

Do not repeat these six forms or the preceding integer controls. One untested
route is consistent formal-argument ordering of the existing VM initializer,
preserving its method body and every compiled declaration/call. This is a
source hypothesis, not evidence of the original signature. Preserve the full
Stage parent and guards; alternatively rotate to another unresolved large owner.
Original ownership, Windows product/runtime and semantic/port gates stay open.
Focused validation only; no full CI or exact-count gain. See
[validation](validation-summary.json).
