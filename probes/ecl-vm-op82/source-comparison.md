# ECL VM source-only reference review

Scope: `EclVmContext::Run`, parent-reported target 0x0044E1A0 / 7020 bytes. Authored C++ and source hashes only. No Factory calls, production edits, builds, native bytes, disassembly, export, or runtime tests. Historical extent/case alignment is not a fresh measurement or byte-exact result.

## Result

No missing call/return, thread lifecycle, or typed stack contract found in the bounded comparison. One narrow source-sequencing hypothesis remains, requiring maintained target-contract evidence before promotion: opcode 82 explicitly resolves the destination before fetching the float in upstream authored Run, whereas maintained Run combines both calls in one assignment expression.

Maintained `EclVm.cpp:1135-1137`: `*ResolveFloat(0) = EnemyWrapAngle(ReadFloat(0));`. Upstream `ecl_core_interpreter.cpp:357-360`: a separate `float* d = FloatDest(0);` statement precedes the operand read and normalization. The maintained historical C++ expression does not itself guarantee that same sequencing. These functions can invoke separate host virtual callbacks; getters can also consume tagged stack entries. A source-level candidate would capture the destination pointer in a statement before the read, but this report does not establish which ordering the target requires or claim that any current compiled ordering differs. No patch or probe is justified solely by the reference.

## Negative findings

- Return leaves the local frame, terminates on zero stack top, otherwise restores instruction and time in that order, tests null, and resumes by advancing the restored call instruction. Call transfers with continue and does not apply ordinary instruction-size advancement.
- Jump uses the immediate time and relative byte displacement. Difficulty-skipped instructions still advance. Extended-dispatch -1 exits Run with 0 before time accumulation; normal loop exit adds delta.
- StartSubroutine reserves a zero sentinel for fresh stacks, writes argument values above the saved-frame area, resolves each value in the caller context, recognizes both f/g source types, converts by destination tag, preserves the frame word after argument pops, and saves time then instruction. Lookup installs the destination as active; failure nulls caller instruction and does not restore the previous active context. Both sources share that failure behavior; it is not a missing restoration to repair speculatively.
- Thread creation inherits difficulty, links immediately after the anchor before starting, and initializes only the listed fields. Neither source initializes flags/thread-control here. Caller Run ignores the helper result in upstream, so maintained void SpawnThread does not omit a tested result at this call site.
- Owner iteration captures next before running each context, never frees the root, unlinks/frees finished children, and restores the embedded active context on normal completion. Upstream free versus maintained delete is not evidence of missing lifetime semantics: maintained allocation uses new and the inspected classes have no explicit destructor.
- Stop-all nulls child instructions, excluding root. Find scans from the root. Thread-control reads operand 1 only after successful lookup, with the child pointer captured first.
- Typed pop checks raw-value availability before copying, consumes the tag only after success, and converts i/f as requested. Frame push/pop and their boundary inequalities agree. Do not add defensive checks that change these contracts.
- Arithmetic consumes right then left, uses integer-tagged comparison outputs, and opcode 85 negates the raw integer storage of a float-tagged value in both sources. Formatting opcode 30 retains argument-resolution side effects even though logging is absent. No missing generic case was identified.
- Upstream opcode 86 spells the two squared terms in opposite addition order. Both explicitly read operand 1 then 2 before destination resolution. This spelling difference alone is not a defensible target-behavior repair and should not become a floating-point/code-shape sweep.
- Upstream compact expressions for polar and point-angle operands do not establish portable evaluation order. Do not import its volatile, forced-inline/noinline, private ABI, exact assembly, or synthetic context mechanisms.

## Verified local pins

Manifest: `/workspace/shared/anm-execute-baseline-plan/manifest.proposed.json`, base commit `e534a6f6c77115017e613721353308a41cde9b6a`.

Files read under `/workspace/shared/gui-opcode20-isolated/candidate/` and verified equal to manifest source_files:

- `src/EclVm.cpp`: `8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb`
- `src/EclVm.hpp`: `a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551`

Current config `/workspace/shared/effect-canonical-bookkeeping/tree/config/match-units.toml` verified SHA-256 `8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88`.

## Immutable reference sources

All references inspected at upstream commit `4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a`; source inspiration only. Upstream labels Run an equivalent recovery, not a portable proof of exact target semantics. MIT status was supplied by parent; no substantial source was imported.

- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptcmd/ecl_core_interpreter.cpp#L357-L360
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptcmd/ecl_core_interpreter_context.cpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/include/th10/runtime/script/sptcmd/ecl_core_interpreter.hpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptcmd/ecl_start_child.cpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptcmd/ecl_vm_children.cpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptlib/ecl_pop_scratch_value.cpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptlib/ecl_push_scratch_value.cpp

## Host-side counterexample and evidence gate

This is a source-level thought experiment, not a target/runtime test or a claim about the real Enemy host. Use indirect operand 0 with value -2.0f and a host with a float cell initially 2.0f. Its ResolveEclFloat(-2) sets that cell to 1.0f and returns its address; its ReadEclFloat(-2) returns the cell. Both callbacks conform to the inspected abstract interface, which imposes no purity restriction. Both 1 and 2 radians are already normalized.

- Destination-first: resolver sets cell to 1; getter reads 1; normalizer returns 1; assignment stores 1.
- Value-first: getter reads 2; normalizer returns 2; resolver sets cell to 1; assignment stores 2.

Thus explicit destination capture preserves a meaningful possible callback contract, rather than being inherently only register-allocation shaping. It still does not prove the actual host has these side effects, the historical compiler selected value-first, or the target requires destination-first. The opcode's callback trace would be sufficient to distinguish the two orders under this host. The maintained generic interface declares both callbacks virtual. Follow-on inspection located the actual Enemy implementations under different view names: EnemyFullObjectView::ReadFloatOperand and ResolveFloatOperand. The latter is pure address selection for four float-variable fields; the matching getter branches merely read those fields. The abstract counterexample therefore does not demonstrate an actual Enemy behavior bug. Full detail and additional manifest-verified pins are in op82-verifier-plan.md.

Parent supplied authored source ledger capture `/workspace/shared/ecl-vm-source-notes.json`, command `342c9b72548a4cfb8897d97858ebb58c`, complete reassembled pages. ECLVM-021 documents angle normalization and historical nested opcode 0x51 extent, not an explicit opcode 0x52 destination-before-value contract. Later ECLVM-061/062 retain thread-argument scheduling and format-store sequencing; 063/064 are parked controls, not proof for opcode 82. Parent confirms no fresh target-order evidence.

Stopping condition: bounded source comparison complete. No-go for production adoption/build from upstream alone. A later authorized, narrowly target-bound ordering proof is required before making this one-statement sequencing change or measuring it; this audit accessed none of that target evidence.
