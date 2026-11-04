# TH10 enemy ECL boss: typed world position and animation-state ownership

This checkpoint changes the maintained **14,416-byte target** enemy ECL
dispatcher (`EnemyRuntimeView::DispatchEclInstruction`, original
`0x0040E770`) and builds on
[typed flags](../enemy-dispatch-typed-flags-20261104/README.md).

The native full-object layout in `src/Enemy.hpp` puts
`worldMotion.position` at runtime +0x02C and contains the named state
fields `value0E8`, `value0EC`, `value0F0`, `animationBaseScript`
and `animationDirection` at +0x0E8 through +0x0F8.
This change replaces **45 more target-addressed source accesses** in
the complete dispatcher with those actual members:

- Spawn/projection XY/Z coordinate reads and the replayed movement/bullet
  position calculations now use `worldMotion.position.x/y/z`.
- Nine ANM placement transfers still explicitly copy **float bits** into
  the VM's raw position words, rather than numerically converting float
  to unsigned integer or introducing an unrelated structure copy.
- ECL selection and animation script/direction publication use the
  existing resource/script state members, not an invented parallel owner.

The original source declaration and historical translation unit are still
unknown; this is a target-layout-constrained maintained C++ representation.

## Selected source-graph result

The real pinned VC7.1 SP1 11-source graph was rebuilt twice while refining
the state accesses; the recompiled owner remains **14,352 bytes**. Against
the immediately preceding typed-flags linked image, all **716** decoded
candidate external-address fields have the same locations and types, and
**every ordinary code byte is unchanged** after field-location
normalization. The last two reads of +0x0E8 were recompiled and compared
against the preceding 14,352-byte candidate once more, also with zero
ordinary differences. The current Player source was genuinely compiled,
rather than treating an older Player object as current.

See `native-result.json`, which pins the final modified source digest,
candidate size, field inventory and linked graph directory. This is not
a direct target equality or canonical exactness claim: the retail owner
still occupies **14,416 bytes** and codegen is **NONEXACT**. No partial
matching credit, changed target bytes or full cold replay are involved.
