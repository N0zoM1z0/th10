# TH10 full Enemy ECL dispatcher: typed initialization of bullet pattern records

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This change edits the **actual 14,416-byte original-owner reconstruction**
`EnemyRuntimeView::DispatchEclInstruction` (`0x0040E770`), not a helper
stub or documentation-only version. The full function is still **NONEXACT**.

The `INITIALIZE_BULLET_PATTERN` opcode clears one native 0x210-byte
pattern record (132 dwords), then initializes seven independent slots
previously accessed via untyped `iVar11 + constant` addresses. All seven
now use the verified native `bulletPatterns[iVar26].fields` members:

| Original slot | Recovered source field | Initial value |
| --- | --- | --- |
| pattern +0x1F4 | `countA` | 1, 16-bit |
| pattern +0x1F6 | `countB` | 1, 16-bit |
| pattern +0x010 | `angleA` | 0, 32-bit bit store |
| pattern +0x018 | `speedA` | `0x40000000`, 32-bit float bit store |
| pattern +0x200 | `soundId` | 7 |
| pattern +0x204 | `soundVariant` | 0x18 |
| pattern +0x1FC | `flags1FC` | 0x203 |

The 0x210 record extent, typed member offsets and byte widths are
asserted in `Enemy.hpp`. The 132-word clearing loop **keeps the original
`iVar11`-based pointer and loop schedule**; its typed-field writes follow
that clearing. This avoids invented bounds checks, new storage and
historical-source-name claims. It does not alter the two separately typed
bullet offset resets at runtime +0x1344/+0x1348.

## Real VC7.1 WPO experiments, including regressions

The actual 11-source Enemy update graph was rebuilt with the pinned
MSVC 7.1 SP1 `/GL` compiler, recompiling the modified dispatcher. The
old `.analysis/gpt-web/20261103` source notes were **not** assumed to be
right: the compiled PDB owner and exhaustive linked field inventory were
observed afresh in four source-shape variants.

- Baseline after earlier real template recovery: **14,352** candidate
  code bytes, **716** decoded address fields. It is not the 14,416-byte
  physical target.
- Typed pointer plus seven typed stores: **14,348** candidate bytes,
  716 fields with changed field geometry.
- Typed pointer derived directly from `iVar11`, plus the seven typed
  stores: also 14,348, 716 fields with changed geometry.
- Seven typed fields **while retaining the original clearing pointer**:
  **14,348**, 716 fields, changed geometry — the selected source shape.
- Only two typed count fields: 14,352 candidate bytes, but **field
  geometry still changes**. Equal size alone is not a strict match.

The original initialization block continues to emit the exact observed
store widths and values, but the whole WPO optimizer changes some
register/address scheduling when type information becomes available.
Accordingly there is **no attempt to normalize away the changed field
positions**, and no claimed full-target or full-candidate raw equality.
The current source is the selected seven-field type recovery and remains
NONEXACT, **zero new exact bytes** credited. See
[native-results.json](native-results.json) for concrete hashes, PDB owner
sizes, field geometry and retained PE/MAP/PDB artifact locations.

After installing that exact source epoch, three selected already-exact
functions were cold-replayed: `enemy-runtime-constructor` 145,
`enemy-drop-item-counts-core` 503 and `enemy-read-raw-float-argument` 26,
**674/674 bytes exact**. No full all-unit cold replay was run.

This is real structured source recovery within the giant opcode switch,
not original historical C++ code attribution, complete program linking,
runtime correctness or portability.
