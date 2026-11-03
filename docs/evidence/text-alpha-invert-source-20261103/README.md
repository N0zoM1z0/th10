# TH10 text-buffer alpha inversion at 0x00437160

The previously source-absent, target-reviewed **157-byte** owner is now
maintained as `TextRenderBufferView::InvertAlpha(int)` in `src/TextRenderer.cpp`,
under the existing bitmap view in `src/TextRenderer.hpp`.

## Direct original executable decode

The native receiver is ECX; pitch is at +0x110, raw pixels at +0x120 and the
pixel-format tag at +0x100. The stack argument is the number of rows to
process, signed-multiplied by pitch; the function returns with `RET 4` and
sets AL 1 for supported formats, AL 0 for unsupported values.

The three format-specific branches were checked against original PE
instructions and target-hash-attested Ghidra decompilation:

- **0x15, 32-bit A8R8G8B8:** invert the fourth byte of every 4-byte pixel
  from byte offset 3 up to the signed row-byte count. RGB is untouched.
- **0x19, 16-bit A1R5G5B5:** for `(rowBytes - 1U) / 2 + 1` words when the
  row count is positive, invert the high alpha bit, retaining RGB if the
  new high bit is set, but clearing the **entire pixel** otherwise. The
  branch uses the sign of the high byte, not an approximate alpha fraction.
- **0x1A, 16-bit A4R4G4B4:** XOR `0xF0` into bytes at offsets 1,3,5,...
  below the row count, preserving the low nibble and other byte.

There are no pixel-buffer allocation or null checks in the original body;
none are invented in this implementation. Both ordinary and alternate text
rasterizers already invoke this method as a real dependency.

## Scoped compiler exploration

The pinned VC7.1 `/GL` isolated-source comparator was run on successive
maintained source shapes; all outputs are **nonexact**:

| Variant | Native contribution vs target | Preliminary ordinary bytes |
| --- | --- | --- |
| Initial typed three-branch loops | 165 vs 157 | 46/157 |
| Unsigned A1 word count and masked fallback | 157 vs 157 | 82/157 |
| A1 `do/while`, sign-byte predicate, word-oriented pointer | 157 vs 157 | 111/157 |
| Shared source `index` among branches (selected) | 157 vs 157 | 111/157 |

The final 157-byte diagnostic matches the three original selector offsets
and main A1 bitwise instruction sequence, but persists with a different
register assignment (candidate ESI pixel base and EDX byte count versus
native EDX pixel base and EAX byte count). Physical size agreement is **not**
codegen exactness, and zero independently target-bound field proofs are
asserted for this owner. No change to canonical match units or exact ledger.

Logs are retained privately at
`.analysis/gpt-web/text-alpha-invert-{first,second,third,fourth}-probe-20261103.log`.
Original source spelling, entire TU and runtime equivalence remain unproven.
