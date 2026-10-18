# TH10 upstream alpha source provenance

Source-only review, 2026-10-18. Repository: levzzz5154/th10-decomp.
Immutable commit: `4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a`.

## Findings and limits

- The authored header models primary alpha setup as an AnmVm member with int duration and unsigned-char mode, initial, and goal. The VM owns the interpolation state: its initial, goal, controls, duration, and mode are int fields, while current alpha is unsigned char.
- Opcode 55 supplies resolved integer duration, zero mode, current byte alpha, and a raw byte goal. Opcode 58 supplies resolved integer duration, a raw byte mode, current byte alpha, and a resolved integer goal narrowed by the helper's byte formal. Raw byte reads bypass variable resolution.
- The helper model zeroes the controls, stores duration and normalized byte-valued parameters in int fields, then restarts the embedded timer.
- Crucial limitation: the helper body appears in an explicitly labeled link-context model. Its opening comment says only AnmVm::Tick is compared. This is not independent proof that helper 442300 itself is reconstructed exactly, nor that these were the original formals.
- Byte formals and int formals with explicit unsigned-byte normalization can represent the same storage semantics. This reference qualifies the hypothesis; it does not decide compiler register assignments or justify importing noinline annotations.
- whole_program_ltcg.md has no alpha-, 442300-, or 43EE30-specific discussion. Its general prose describes common optimization visibility and limits on inferring private conventions and original translation-unit organization; no upstream matching claims are adopted here.

## Immutable sources

1. Declaration, interpolation fields, VM ownership, and opcode data types:
   https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/include/th10/runtime/core/sprtanim/anm_vm_tick.hpp
   Relevant lines: 89–95, 104–114, 164–174, 223–232.
   Git blob: `93736e66a186820c92af13ea3f259ee6ed51ef1c`.
2. Helper checking-context body and provenance disclaimer:
   https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/core/sprtanim/anm_vm_tick_context.cpp
   Relevant lines: 1–3, 279–287.
   Git blob: `56a4579fef6daf510255807725e7602e331e7372`.
3. Authored interpreter callers:
   https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/core/sprtanim/anm_vm_tick.cpp
   Relevant lines: 9, 120–122, 214–216.
   Git blob: `4dcd9262ca72dc988f063bd2f53229a565e2c519`.
4. General optimization-scope prose, not an alpha-specific result:
   https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/reference/whole_program_ltcg.md
   Relevant lines: 9–22, 368–382.
   Git blob: `494c6683f9bb13f5fa798fb9744b3b3ffd92541f`.
5. License:
   https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/LICENSE
   Git blob: `0706df312064fec49fd1a57b311cb92d888acc2d`.
   MIT; copyright 2026 levzzz. Preserve copyright and permission notices for copies or substantial portions. No substantive code copied into this note.

## Review boundary

Connected GitHub read-only retrieval of source, license, prose, and repository metadata only. No assembly fallback, binary payload, decompilation, ordered native trace, repository-code execution, build, remote host action, or upstream matching metric was used. Documentation fenced code was filtered before inspection. No candidate files were edited.
