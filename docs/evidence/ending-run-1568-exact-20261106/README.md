# Ending script execution — complete 1568-byte exact owner

Original Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EndingTextVmView::Run` owns `0x0040BD80..0x0040C39F`, **1568 bytes**:
1488 reachable code bytes, six internal alignment bytes, two tail alignment
bytes and the complete 72-byte, 18-entry jump table at `0x0040C358`.
The stack receiver and RET 4 interface are unchanged. The existing boundary
review remains high confidence; authored-game origin remains medium confidence.
No inventory or origin review is promoted by the codegen result.

The fresh current-source baseline reproduced the historical 54 ordinary
differences, now checked by the strict full-owner comparator as **1514/1568
bytes / all 92 original-bound fields**. The original inventory was independently
redecoded from the original executable before using the older review: 45 REL32
and 47 DIR32 fields, including 19 local fields. All 53 direct local destinations
and all 18 table destinations are included. Two complete source literals match
the original NUL-terminated bytes; 71 external uses reuse reviewed canonical
symbol bindings. There are no excluded bytes or data spans.

The installed natural source restores four related operations:

- Text lookup consumes the ID value already stored in its slot, through an
  ordinary inline helper. It does not construct a second ID from an integer.
  The noninitial text line passes decode, color and lookup in one expression.
- Pending-text iteration uses the actual ID cursor and a remaining count.
  This also restores the original scaled-timer x87 operand order.
- The loading flag operation precedes background-path publication in source,
  recovering the original load/OR/path-store/flag-store instruction schedule.
- Animation opcodes 8 and 15..17 read a 16-byte command view: four header
  bytes followed by destination, resource and script dwords. The view is
  captured after deletion/release. Publication still rereads the live
  instruction after creation, preserving the original callback boundary.
  This last change closes all 28 remaining argument/result register bytes.

The updated MIT reference at
[`d1f31212514781c928fba145d8207397c8fb5650`](https://github.com/levzzz5154/th10-decomp/tree/d1f31212514781c928fba145d8207397c8fb5650),
`matching2/msvc/src/game/ending/recovery_script_executor.cpp`, supplied concrete
stored-ID, expression and cursor leads. Attribution is retained in source and
the [MIT notice](../../licenses/th10-decomp-MIT.txt). Its synthetic caller,
callee stand-ins, forced inlining, volatile macros and explicit output-buffer
arguments are not used. Local actual callees, headers, entry and compiler
profiles are preserved. A natural shared mode-animation helper outlined and
shrunk the owner to1388 bytes, so it was rejected without adding an attribute.
The original class/helper names and production TU remain unknown.

The first final-source image recompiles Ending.cpp and makes a nonincremental
LTCG link with 20 unchanged, source/header-pinned objects from this turn's
fresh 21-input baseline. It passes **1568/1568 bytes / all 92 fields**.
An independent canonical cold recompiles all 21 real inputs under the existing
checksum-root graph, including the established Ending `/GL /GS /EHs` profile:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit ending-text-run --json
```

The cold repeats the complete zero-difference comparison with different PE/PDB
identities and unchanged83 source/header pins. Canonical output is
`build/match-linked/EndingTextRun/`. In both images, explicitly selected same-image
contracts preserve constructor648, LoadScript106, decoder41, pending-ID65 and
the direct caller Tick93: **953 existing exact bytes / 24 fields**, including
the constructor's required worker-member EH address/metadata bindings.
These existing guards receive no new credit. New authored exact credit is
**one complete owner /1568 bytes**.

[Compact results](native-result.json) retain the source, graph/manifest,
image/PDB, complete binding and guard pins. Full native receipts, control
snapshots and logs remain privately under
`.analysis/gpt-web/20261106-ending-ring/`. Earlier1564/1576-byte candidates and
the 54-difference October30 source epoch are superseded by this proof.
No comparison mechanism changed. No historical cohort, host failure campaign,
runtime, whole-product closure or portability claim is made.
