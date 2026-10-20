# Enemy operand adapters through the generic host view

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `8cf6146`. Reviewed on 2026-10-21.

Five complete operand adapters now reproduce their reviewed target extents:

| Maintained EnemyRuntimeView method | Target | Bytes | Complete linkage binding |
| --- | --- | ---: | --- |
| ReadIntArgument | 412A00 | 16 | REL32 at +0x0B to ReadInt, 44FDB0 |
| ReadFloatArgument | 412A60 | 14 | REL32 at +0x0A to ReadFloat, 44FE40 |
| ResolveFloatArgument | 412A70 | 14 | REL32 at +0x0A to ResolveFloat, 450070 |
| ReadRawIntArgument | 412A80 | 17 | REL32 at +0x0C to ReadIntValue, 44FF00 |
| ReadRawFloatArgument | 412AA0 | 26 | REL32 at +0x13 to ReadFloatValue, 44FF80 |

Each method first loads the full owner from runtime +14D8, treats that loaded
pointer as the existing EclVmHost view, and reads activeContext at owner +4.
The previous source accessed a partial Enemy context member before casting the
context pointer. The generic host view recovers the intermediate EAX loads,
private entry registers and argument scheduling naturally under VC7.1 LTCG.
The raw-float adapter needs the ordinary local host variable; a direct nested
expression changes its receiver to ECX and fails the complete comparison.
ResolveIntArgument's already-exact 75-byte source is unchanged.

The source retains both target-proven dereferences. Runtime itself is not an
EclVmHost. Existing native layout assertions check runtime owner +14D8 and host
activeContext +4. These partial views do not establish allocated-data aliases,
the original hierarchy, declaration, source spelling or production TU.

## Target and compiler evidence

Fresh local Ghidra review used read-only/noanalysis, a rolled-back metadata
transaction, one CPU and a 768 MiB heap. The executable identity and all 412289
mapped text bytes at 401000..465A80 were verified before and after review, with
SHA-256 `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
All five bodies decode completely. The two 14-byte entries end in tail jumps;
the raw-float entry ends in RET 8. Neighboring padding is outside each extent.

Ghidra references and an independent complete raw-text REL32 scan find 30
calls to 412A00 and 13 to 412AA0, all within the reviewed dispatcher. Neither
scan finds direct references to the other three entries. A complete executable
file scan finds no absolute pointers to any of these five entries. Absence
of these static references does not establish original authorship or exclude
unobserved runtime address construction. All five origins remain indeterminate.

Two independent full cold links compile EnemyEclDispatcher.cpp followed by
EclVm.cpp, AnmManager.cpp, AnmVmCreate.cpp, EnemyLaser.cpp, Enemy.cpp, AnmVmId.cpp
and RandomMath.cpp, all with `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`.
Both select the genuine DispatchEclInstruction entry. The second cold uses the
canonical graph already shared by anm-release-slot, in
`build/match-linked/AnmReleaseSlot`; unrelated canonical contexts are unchanged.
The anchor image is a diagnostic artifact, not a runnable product.

In each cold, the comparator binds every declared field through the map and
checks all 87 owned bytes against the target using complete PE/PDB extents.
Fourteen existing complete same-image guards retain 1452 bytes and all 27
fields. Together, each image passes 19 complete owners, 1539 bytes and all 32
fields. Recursive input pins and individual owner identities are recorded in
[the compact proof](native-summary.json).

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --unit anm-release-slot --json
```

## Large owners and rejected hypotheses

In the current eight-TU graph, the dispatcher changes from 14524 to 14404 bytes
against the target's 14416. Its complete selector mapping and physical case
order remain correct; 40/108 cases have target sizes, versus 35/108 initially.
The sum of absolute case-size gaps falls from 792 to 690. Both frames allocate
2C4 bytes and both contain 19 comparisons with the double -999999 sentinel.
These are diagnostics only: the dispatcher is still nonexact, including
register assignment, instruction ordering, lifecycle and stack-use differences.
The polar-store case remains 399/405 bytes. Total size proximity grants no
partial exact credit.

Run remains 7020 bytes with all 189 fields bound, including 62 local targets,
and 27 ordinary byte differences in this graph. The separate two-TU context's
26 differences remain a different measurement. ANM ExecuteScript source was
not edited; its bytes are not replayed here. All three large owners remain open.

Moving the unchanged integer adapter into a separate ninth `/GL /GS` TU, or
placing its definition after the dispatcher, leaves its original three byte
differences. Complete native-to-native comparisons of the integer adapter and
dispatcher are equal after verified field rewrites in those controls. They
reject these particular partition/order hypotheses, without proving original
TU ownership or runtime semantics. Both controls are restored. Private source
overlays, commands and unresolved diagnostics are retained under
`.analysis/gpt-6.1-sol/enemy-adapter-partition-20261021/`.

The ledger adds five exact functions and three source mappings. Authored exact
counts remain unchanged because all five origins are indeterminate. Product,
runtime, semantic and portability gates remain open.

All 100 standard CI checks, toolchain declarations, tracking, progress and
whitespace checks pass. Final source pins still match both cold proofs. Cleanup
removes seven superseded native directories and 29 intermediate analysis files,
reclaiming 8639051 bytes. See [validation scope](validation-summary.json).
