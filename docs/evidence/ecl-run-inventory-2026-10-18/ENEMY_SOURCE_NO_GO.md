# Enemy dispatcher source-only comparison

Scope: authored sources only; no builds, edits to source, target-byte/disassembly access or remote factory calls. Target dispatcher 0x40E770, parent-reported 14480 versus 14416; no claim of measured improvement.

First bounded pass: no missing spawn or beam construction semantics found. Maintained spawn request is 0x40 bytes and includes the 0x20-byte variables block by value; its aggregate assignment agrees with upstream memcpy. Beam opcodes 0x19D/0x1B0 already use inline zeroing and 8.0f at +0x2C; 0x1AD/0x1B2 already call initialization helper. Do not import upstream volatile loads, forced/noinline annotations or context stand-ins.

Second bounded pass: no missing argument-resolution behavior found in rank selection, laser setters, difficulty wait, or random movement. Getters can pop tagged scratch values and invoke external virtual getters; destination resolvers can invoke external virtual resolvers. Preserve explicit maintained sequencing. In particular laser setters 0x19E/0x19F guard both float reads behind successful lookup and read FOP(2) before FOP(1); 0x1AE reads FOP(1), then FOP(2), then resolves destination; spell start evaluates otherwise unused integer argument 2 before argument 1. Upstream compact expressions do not establish a portable replacement evaluation order. Rank selector oddities (3-way selecting argument 0 for both lower bands; 5-way lower cutoff -400) are already shared, not a fix opportunity.

Random movement: both select one RNG draw on the chosen geometry branch before argument reads, then mode IOP(1), speed FOP(2), duration IOP(0). Maintained EnemyRandomAngle is only declared in inspected manifest source set, so RNG provider implementation was not verified; do not claim identical RNG mapping or exceptional floating-point behavior.

Call/return are outside this dispatcher owner: EclVmContext::Run in EclVm.cpp owns ECL_VM_RETURN and subroutine dispatch. Do not broaden this review into that interpreter. Dispatcher wait cases retain -1 returns; normal completion returns 0.

Conclusion: exact negative for these bounded areas; no source-backed candidate for +64-byte gap. Upstream supports semantic cross-check only and is not evidence of target byte exactness. Upstream MIT license requires notice preservation for copied/substantial portions; no source imported.

## Verified maintained files
- EnemyEclDispatcher.cpp: 9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30
- Enemy.hpp: b2ef2f155861082af19c81b186f45c1bcf45496f887c96ca0d3dd5b47f8d5d4d
- EnemyLaser.hpp: b37486fc9421707645588b8b239d8c24a10f5d71d4b05fd34f6e8aa228be3743
- EnemyLaser.cpp: d3ba99945c0ef3776796cbff86cfb6f506a699b0d4cdb013becc90c6ff7d59dd
- EclVm.hpp: a01e19c6bb2659e258444cd57018947fe6f5444522b75d2b5777e13f55ffe551
- EclVm.cpp: 8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb

Manifest: /workspace/shared/anm-execute-baseline-plan/manifest.proposed.json

## Immutable references
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/LICENSE
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/game/enemy/enemy_execute.cpp#L21-L129
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/game/enemy/enemy_execute.cpp#L788-L865
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/game/enemy/enemy_execute.cpp#L867-L1001
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/include/th10/runtime/game/enemy/enemy_execute.hpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptcmd/ecl_resolve_float_operand.cpp
- https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/script/sptcmd/ecl_resolve_variable_value.cpp
