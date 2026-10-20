# Hint split control flow and static tail transfers

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent source checkpoint: `c674189`.

## Complete source and compiler result

HintSplitLine at0x0041A050 owns75 bytes. The target copies the key, finds the
first colon, and takes the colonless trim tail before the colon-present body.
The former source's if/else emitted71 bytes and placed that tail last. An
explicit `colon == 0` early return recovers the complete75-byte owner, including
compiler alignment. A missing colon still leaves the value buffer unchanged.

Both independent full cold builds use the real LoadHintFile entry, Hint.cpp
plus FileSystem.cpp and the pinned VC7.1 SP1 build6030 /GL profile. Qualified
static MAP/PDB ownership selects the whole helper; all4 fields replay exactly:

| Offset | Transfer | Reviewed target |
| --- | --- | --- |
| 12h | CALL | CRT strchr452730 |
| 20h | JMP | HintTrimWhitespace41A0A0 |
| 3Fh | CALL | HintTrimWhitespace41A0A0 |
| 47h | JMP | HintTrimWhitespace41A0A0 |

The static trim callee is independently complete114-byte, zero-field and
raw-equal. The two tail exits pass the key or value pointer in EAX; the
colon-present exit restores ESI before the final JMP. Existing trim114,
lookup98 and cleanup79 also replay in both images. The total is four complete
owners,366 bytes and all6 fields; only the split helper adds75 exact bytes.
Source/owner/artifact identities are in [native summary](native-summary.json).

## Explicit static JMP binding

The comparator retains its original static CALL default. A separate literal
`static_tail_jump = true` opts a REL32 field into unconditional JMP entry
binding. It requires the selected source/object, complete reviewed target
boundary, maintained target source/name mapping, and complete raw-equal
zero-field callee. The full parent still has to match after every field binds.

The22 context tests include the old implicit-JMP rejection and new controls
for explicit JMP success, CALL/conditional-branch rejection, invalid flag
values, incomplete selectors, interior destinations, wrong target mappings,
callee byte/field mismatches, cropped/provisional boundaries and a mixed
CALL/JMP cache control that prevents bypassing the stricter JMP mapping check. The50 static
owner tests retain the qualified MAP/PDB checks. No callee credit is duplicated.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit hint-split-line --unit hint-trim-whitespace \
  --unit hint-lookup-value --unit hint-clear-lists --json
scripts/repo-python scripts/test-linked-context.py
scripts/repo-python scripts/test-static-owner-selection.py
scripts/repo-python scripts/test-hint-load.py
```

The declaration, original translation-unit/profile identity and native runtime
remain unknown. LoadHintFile is still nonexact (1546/1498 bytes in this graph).
A pointer-to-link AppendLink control regresses40->39 bytes versus target52 and
is restored. An ordinary ECL two-float value control regresses Run7020->7024
and frame108->110; it changes the second spill to10C instead of target14 and
is also restored. These negative controls do not rule out other value shapes.
Private source/report recovery is under
`.analysis/gpt-6.1-sol/hint-control-flow-20261020/` and
`.analysis/gpt-6.1-sol/ecl-value-shape-20261020/`.

The actual-source parser baseline and22 strict mutation controls pass under
UBSan. All97 standard CI checks, toolchain declaration checks, tracking,
generated progress and whitespace checks pass; source/comparator hashes were
rechecked afterward. The [validation summary](validation-summary.json) records
the exact scope. Three inspected negative/superseded build directories were
removed, reclaiming671588 bytes; full source/report controls remain.
