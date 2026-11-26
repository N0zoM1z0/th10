# TH10 Semantic Reconstruction

Semantic reconstruction replaces layout-shaped source—raw object offsets,
anonymous storage, magic protocol numbers, and provisional identifiers—with
evidence-backed C++ while preserving both the accepted VC7.1 SP1 exact bytes
(751/751 confirmed authored functions) and the playable, position-independent
Windows i386 PE32 build (`build/windows-i386/th10.exe`).

Subsystem navigation lives in [SEMANTIC_INDEX.md](SEMANTIC_INDEX.md).

## Evidence boundary

- **Observed:** TH10 v1.00a access offsets, widths, signedness, bit operations,
  callers/callees, strings, relocations, exact object code, and verified runtime
  behavior.
- **Corroborated:** independent TH10 producers/consumers agree, or adjacent
  Touhou titles (TH08/TH09/TH095) match TH10 evidence as a secondary lead.
- **Inferred:** dataflow supports a neutral role, even if TH10 does not name it
  explicitly. State uncertainty once when it matters.
- **Unknown:** only storage width or alignment is known. Keep it opaque.

## Bounded batch workflow

1. Pick one coherent protocol family or struct/subsystem (e.g., configuration
   flags, ANM bank slot IDs, supervisor game modes, sound cue IDs, or one
   subsystem's state machine).
2. Verify every read/write, caller, string, and consumer in TH10 `src/` before
   naming an enum constant or member.
3. Make the smallest natural C++ change (prefer typed `enum` constants, named
   flags, and clear comments over invasive control-flow rewrites).
4. Validate with both oracles:
   - **VC7.1 exact oracle:** replay every affected accepted exact unit via
     `scripts/build-match-unit.py --unit <unit>` and
     `scripts/compare-linked-function.py --unit <unit>` (must remain 0 diffs).
   - **Product build + PI oracle:** run `scripts/build.py --fresh` (required
     after editing shared `.hpp` headers, since incremental `scripts/build.py`
     tracks `.cpp` mtimes only), `scripts/build.py --reverse-link`,
     `scripts/verify-pi.py`, `scripts/check-semantic-protocols.py`,
     `scripts/ci.py`, and `git diff --check`.
5. Commit each coherent semantic batch on `semantic/readability-protocols` with
   a concise evidence summary.

## Source rules

- Preserve VC7.1 x86 ABI, field offsets, widths, packing, bitfield behavior,
  class size, construction order, and target-shaped expressions where codegen
  depends on them.
- Keep `sizeof`/`offsetof` compile-time layout assertions intact.
- Keep serialization, instruction streams (ECL/ANM/STD/MSG), and platform ABI
  glue byte-oriented where byte addressing is their true representation.
- Never invent TH10 names from speculation; derive names from TH10 strings,
  producers, consumers, and state transitions.
