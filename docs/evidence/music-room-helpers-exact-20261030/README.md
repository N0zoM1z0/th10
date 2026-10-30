# Complete music parser and interpolation helpers — 2026-10-30

Three complete authored owners pass two independent canonical20-input colds:
**357 bytes/all7 resolved fields/all20 fixed local destinations**, zero differences.
Game C++ source is unchanged. This closes helper exactness, not the2796-byte
MusicRoom parent's still-NONEXACT result.

| Owner | Target span | Bytes | Instructions | Fields | Locals |
| --- | --- | ---: | ---: | ---: | ---: |
| FrontEndMusicSkipLine | 4349E0..434A1A | 59 | 29 | 0 | 9 |
| FrontEndMusicCopyLine | 434A20..434A7B | 92 | 43 | 0 | 10 |
| AnmVmView::StartPositionInterpolation | 434A80..434B4D | 206 | 50 | 7 | 1 |

## Complete target and static qualification

Fresh dense decoding covers all122 instructions, all exits and20 fixed local
branches. SkipLine has EAX cursor/EDX remaining and plain RET. CopyLine has ECX
cursor/EDI remaining, one stack destination and RET4. Position interpolation has
EAX VM/EDX initial/ECX final, stack duration/mode and RET8. The actual MusicRoom
caller supplies these interfaces. All parser read-before-count, destructive
copy, delimiter/count paths and the full interpolation timer branch are included.

The two parsers remain static in their original maintained source. Canonical
selection explicitly joins the real MAP Static symbols section to the complete
FrontEnd PDB contribution, qualified by source/object and producer directory.
It introduces no visibility, retention or ABI annotation. Zero-field parser
bodies are raw-equal. One SkipLine, three CopyLine and two interpolation entry
refs decode from the reviewed MusicRoom caller; no encoded interior ref or
unclassified encoded helper ref occurs. CC counts before/after are4/5,5/4,4/2.
Existing reviewed boundaries and origin dispositions are unchanged.

## Resolve the vector accesses without inventing data ownership

The old206-byte structural comparison already matched178 ordinary bytes but
left six tangent-source accesses without a correct target role. The unresolved
harness reserves four bytes per generic anchor: vector+4/+8 happen to coincide
with native Chain/Device anchor addresses. Those native lookup labels are not
target identities. Existing canonical bindings place Chain at491BE4 and Device
at491C30; the interpolation reads491C14/18/1C twice.

The existing `g_AnmZeroVector491C14` declaration is a12-byte AnmFloat3View.
Bind all six observed component operands through that same base with addends
0/4/8. The seventh field is the existing game-speed address476F78 written to the
timer scale pointer. The strict comparator checks each native base+addend,
replays every field to its target address and compares the entire206 bytes.
No target byte, local destination, actual ABI or component access is waived.
No new data definition, alias, literal-zero substitution or game-source edit is
needed. Original aggregate allocation/type/ownership remains unproved.

An initial raw-file read of491C14 fails correctly: the whole12-byte window is
in the virtual tail of PE .data beyond its file-backed extent. Section geometry
and all actual instruction operands establish the readable image view; the
verifier does not manufacture raw bytes. Initial PE zero-fill, later contents,
writes and data ownership remain separate. Source copies the mutable view; this
proof does not assert immutable runtime zeros or product behavior.

## Canonical reproduction

All three recipes retain the established genuine20-input responsibility order,
InitializeVm root, support order and pinned profiles. Both colds recompile every
input without reuse, serial CPU0/nice10 with bounded jobs and retained exits.
Frozen source/header/manifest hashes agree. Full MAP/PDB extents and all fields,
literals, fixed destinations and actual interfaces pass complete comparisons.

```sh
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit front-end-music-skip-line --unit front-end-music-copy-line --unit anm-start-position-interpolation --json
scripts/repo-python docs/evidence/music-room-helpers-exact-20261030/verify-proof.py --artifact-dir build/match-linked/MusicRoomHelpers
```

Static PDB qualification requires the real producer directory. Both independent
canonical comparisons run there before snapshots are copied. Private snapshots
preserve hashes/reports; a copied directory cannot itself qualify as the
producer. The evidence command uses the actual declared directory, retaining
the existing qualification checks. Restoring an older snapshot for independent
replay requires restoring its original producer location, not bypassing checks.

The complete MusicRoom native receipt2788/all104 reproduces in the final helper
image and stays NONEXACT. No full historical cohort, host failure/lifecycle or
mutation campaign is required. Current helper context is
`.analysis/gpt-6.1-sol/music-room-helpers-context-20261030/cold-b-context.json`;
its active artifacts point to the actual producer and its retained snapshots are
recorded separately. The previous20-input MusicRoom source epoch remains current:
this batch changes metadata/evidence only, not game C++ or compiler profiles.

Authored exact increases357 to102270/275835 across502 owners; total codegen
170374/1379. Source mappings1207/authored689/backlog187. Original class/helper/TU,
physical global ownership, product/runtime closure and portability remain open.
