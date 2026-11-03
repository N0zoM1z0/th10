# TH10 585-byte rectangular bullet-cancellation owner @ 0x00408210

This checkpoint introduces `src/BulletCancelArea.cpp`, the complete
*semantic source surface* for previously source-absent, reviewed/authored
`0x00408210..0x00408458` (585 target bytes). Its owner is the existing
`BulletManagerView` because direct target operand offsets resolve to that
class's pool and the adjacent authored bullet-cancellation routines. The
original method name, exact TU identity and private ABI source spelling are
not established.

## Fresh independent target inspection

Direct `objdump -Mintel` against original TH10 1.00a was compared with a
hash-attested, read-only Ghidra decompile. The latter omits some temporary Z
arithmetic, so use original disassembly as primary evidence:

- Private EBX receives the manager; the single stack argument selects item
  emission. The function returns `0` with `RET 4` after exactly 2000 records,
  starting at manager `+0x60` and advancing `+0x7F0` per record.
- Six manager floats at +0x44..+0x58 describe the cancellation rectangle's
  center and full extents. The native code computes each of the three
  center ± half-extents, including Z; actual overlap tests use X/Y only.
- State 0 and state 3 skip. The intersection compares the bullet's center
  ± collisionWidth/Height * 0.5 with the region edges, **including tangency**.
  The original FCOMP/FNSTSW predicates also preserve an unordered-input
  acceptance path; tests are expressed as explicit rejection branches.
- Matched bullets set flag bit 8. With the caller's optional flag, the real
  `ItemManagerView::Spawn` at 0x0041BB00 receives type 8, color -1, angle
  `-1.5707964f`, magnitude `0.6f`, bullet position.
- The real `AnmLoadedView::CreateVmAtWorldVariant0` at `0x00448DB0` receives
  script `0x173` and that position. The created id is resolved through the
  maintained `AnmVmIdView::GetVm` / native FindVm at 0x004491C0.
- If a source sprite is loaded, its width (at sprite +0x34) selects a
  16-color table at 0x00474350, 8-color table at 0x00474390 or 4-color
  table at 0x004743B0 at thresholds 16.0/32.0. The palette is selected
  by the **signed** bullet color index, then written to effect VM primary
  color at +0x2FC. Width comparisons use negated greater-than to match the
  native unordered branch. Finally the bullet +0x450 field is zeroed.

The reconstructed source uses real, existing typed bullet, sprite, ANM and
item owners; it does not invent null/malloc guards or replace target bytes.

## Native diagnostics and remaining work

The pinned VC7.1 `/GL /GS` single-source probe, entered at
`BulletManagerView::CancelBulletsInArea`, compiled and linked successfully,
but its isolated compiler context produces **437** PDB owner bytes compared
with **585** target bytes, only 19/533 preliminary comparable ordinary bytes,
13 candidate address fields. Repeating with the target's unordered-aware
sprite-width predicates remains 437/585. This is a **NONEXACT** source map;
no complete linked-address binding exists for this owner and no exact-credit
ledger change is justified. The native EBX ABI/register scheduling and Z
aggregate materialization must be re-established through a genuine caller
context before attempting canonical acceptance.

Private build diagnostics are retained under
`.analysis/gpt-web/bullet-cancel-area-*-20261103.log`.
