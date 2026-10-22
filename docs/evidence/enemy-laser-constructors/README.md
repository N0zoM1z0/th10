# Enemy laser member construction

Date: 2026-10-22. Starting commit `2a7c199`.
Original Japanese TH10 v1.00a only, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Natural C++ base, request and two real ANM VM member constructions reproduce
the complete type-0 constructor `41C5B0..41C679` (202 bytes) and type-1
constructor `41C680..41C75A` (219 bytes). Two independent canonical full
ten-source cold builds pass every byte and both address fields per function.
The origin ledger remains indeterminate: source-authored inline construction
versus compiler-generated lifecycle contribution is still unresolved.
Overall exact credit increases by two functions/421 bytes; authored exact
credit remains409/74716. Overall totals become1252/141647.

[Facts](facts.json), [source controls](source-controls.json) and
[validation scope](validation-summary.json) retain the compact evidence.

## Target observations

Fresh direct local Ghidra runs read-only/no-analysis, rolls back, uses one CPU
and768 MiB heap, and verifies all412289 mapped text bytes against the canonical
disk target. Three independent76-byte data windows at46DAB0/46DA60/46DA10
also agree. They establish observed nineteen-word virtual prefixes, not full
physical table extents or production data ownership. The first slot is the
RunCommands protocol, not a virtual destructor. Names and C++ declarations are
maintained construction views; callbacks gain no implementation credit.

Both target derived bodies call the complete base98 at41C030, install the
derived vptr, initialize their request and construct both embedded VMs. Type0
has request424/1DC, VMs600/9AC, totalD58. Type1 has request424/1F8, VMs61C/9C8,
totalD74 and fixed request2C=8.0f. Allocation enters construction through EBX;
the result returns in EAX. The maintained factory now uses genuine C++ `new`,
a real derived-to-base conversion and the typed ApplyRequest virtual call.
No register annotation, fake callback body or forced-inline directive is used.

The two canonical units bind REL32 at+5 to the real base constructor41C030 and
DIR32 at+B to the independently observed derived vtable addresses46DA60/46DA10.
The complete base98 and its two fields also match as an additional guard.
Its existing generic-leaf match and primary source mapping are unchanged; this
natural reproduction gives no duplicate credit and proves no original source
parameter or barrier spelling.

## Source controls and complete cold checks

Moving the unchanged VM constructor into its header compiles all ten sources
but leaves the four major bodies unchanged in their existing diagnostics.
The private header form also reproduces the complete88-byte zero-relocation
normal-COFF VM constructor. That header change is not adopted.

A genuine-constructor model using just the two already-called virtual slots
emits factory159/plain RET; caching the actual tail read brings it to153/plain
RET. Expanding to all nineteen observed slots and using a typed base virtual
call retains153/plain RET. A consistent full-ten-source standard fastcall
experiment instead emits157/RET8 and leaves the request on the stack. Standard
fastcall is rejected. These are measured source hypotheses, not proof of the
original source ABI. Only the nineteen-slot typed construction source is
adopted; the public factory signature and all shared headers stay unchanged.

Each accepted cold freshly compiles all ten game-source TUs, with GL/GS and the
existing scale Ob1 override, then regenerates the diagnostic address-anchor
harness. No source object or pinned harness is reused. Each image passes:

- New constructors202/219, two fields each, complete PE/MAP/PDB extents.
- Additional complete base98/two fields, without new ledger credit.
- All36 previous complete guards4618/all121, including the full194-byte ANM
  caller. Five guards with separate canonical COFF contracts remain extra
  linked checks; their canonical artifact kinds do not change.
- ECL7020/6993/27 ordinary differences/all189 fields/62 local destinations and
  complete171-byte float-reader callee. The7019 decoded bytes plus one reviewed
  selector still cover the owner; the generic false normalization flag remains.
- Enemy14352 versus14416, frame2C4,19 sentinels, all181 selectors/108 ordered
  groups, aligned3219/11148, gap566,42 target-sized groups.
- ANM9964, aligned7541/7995,90/92 target-sized groups, aggregate gap2.

Whole native Enemy/ANM/ECL bodies remain baseline-equal after275/60/41 changed
address fields are independently replayed. Seventeen Enemy fields require the
actual vector symbol plus4/8 or chapter-state symbol plus58. Neighboring
four-byte diagnostic anchors are not semantic identities for those member
addresses. Every ordinary byte and all remaining fields are compared; this is
baseline preservation, not target or runtime equality.

The factory is intentionally changed by real construction. It still emits153
with ordinary RET, against target153/two RET4 exits; no factory equality claim
is made. Its target fields occur at54/68/80/94, versus52/66/78/92 in this native
candidate. Matching size alone is insufficient.

## Reproduction and retained limits

```sh
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-laser-type0-constructor \
  --unit enemy-laser-type1-constructor
```

Run that command twice for independent canonical cold proofs. The group is the
existing EnemyItemDrops real-dispatcher context; each call rebuilds all ten
sources. Never treat its unresolved callback data anchors as a runnable product.
Native compile/link, data ownership and exercised Windows i386 runtime remain
open. Semantic reconstruction and portability remain unstarted.

The table survey also routes seven untracked entries, recorded in facts.json.
No new boundary, candidate, origin or exact claim is created for them; each
requires its own complete target-local review. Old boundary metadata drift is
not silently applied.

The first private comparison failed at import and the first canonical replay
preparation lacked function selectors. Both are corrected before the two actual
successful cold builds; neither failure supplies a target mismatch result.
Private recovery is
`.analysis/gpt-6.1-sol/enemy-construction-context-20261022/RECOVERY.md`.

Final111-group target-independent CI terminates0. All373 final source/test/config
inputs,108 maintained source files and19 current native files remain unchanged
throughout. Both accepted cold carriers, baseline15/32 and all9 independent D3D
files pass their retained hashes. Focused canonical comparators, toolchain,
tracking, source completeness, progress, documentation and whitespace checks
pass. No native whole-product or runtime acceptance follows from these checks.
