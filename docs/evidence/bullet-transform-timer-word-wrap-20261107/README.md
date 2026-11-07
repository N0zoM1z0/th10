# Bullet transforms: seven frame-timer increments as native 32-bit words

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Seven distinct transform timer fast paths in the actual maintained
`src/BulletManager.cpp` increment the signed `state.timer.current` word
when the timescale is approximately one. Previously `++state.timer.current`
used signed C++ increment, which can be undefined at `INT_MAX`, while the
original i386 instruction preserves the low 32-bit word across overflow.
The natural maintained C++ now increments the same word through its matching
unsigned 32-bit representation:

```cpp
++*reinterpret_cast<unsigned int *>(&state.timer.current);
state.timer.subframe += 1.0f;
```

No timer allocation, data layout, new guard, control transfer, ABI annotation,
extra field or floating arithmetic changes. The existing `previous` copy,
strict scale interval, signed timer comparisons, fractional-frame and
external transform calls stay in their original source positions. This is
a codegen/behavior claim under the pinned Win32 compiler, not an arbitrary
portable C++ pointer safety claim or original declaration attribution.

## Actual seven-TU whole owner/native comparison

With the current `BulletHorizontalWrap` 7-TU `/GL /GS` graph from pinned
VC7.1 SP1 compiler and source/hash-verified support inputs, three distinct
changes were compiled and linked:

1. All seven timer increments through the unsigned field word (installed).
2. Only the relative/absolute direction-change pair.
3. Only the polar plus relative/absolute direction-change trio.

Every real source variant retained identical complete PDB contributions,
zero ordinary candidate byte changes and the same actual candidate field
positions/types/widths for seven reviewed owner functions:

| Complete native owner | Candidate bytes | Real linked fields | Ordinary candidate byte differences |
| --- | ---: | ---: | ---: |
| `UpdateRelativeDirectionChange` | 338 | 8 | 0 |
| `UpdateAbsoluteDirectionChange` | 335 | 8 | 0 |
| `UpdateAimedDirectionChange` | 431 | 12 | 0 |
| `UpdatePolarAcceleration` | 209 | 8 | 0 |
| `UpdateDeceleration` | 164 | 7 | 0 |
| `UpdateHorizontalWrap` | 148 | 8 | 0 |
| `UpdateVerticalWrap` | 172 | 8 | 0 |

The original `UpdateAbsoluteDirectionChange @0x004078E0` is **335 bytes**.
Both the preedit baseline and all tested native variants remain strict
original-bound **333/335** with **8/8 independently original-bound call/
data fields**. Two register operand bytes `+0xD7`/`+0xDD` (EAX/EDX angle
forwarding) are unresolved. This nonexact boss is **not** promoted merely
because its dimensions are correct or its changes are candidate-byte-neutral.

## Final installed-source focused exact cold

A new pinned selected cold from the actually installed and commented source
checks the affected previous exact owners:

- `bullet-update-deceleration` **164/164**, seven target linkage fields;
- `bullet-update-horizontal-wrap` **148/148**, eight fields;
- `bullet-update-vertical-wrap` **172/172**, eight fields.

All three pass **484/484** original target code bytes in two native build
artifacts. The same current output was independently checked against the
335-byte absolute direction original: still **333/335** and all eight fields.
No additional exact owner or partial byte credit is claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-update-horizontal-wrap \
  --unit bullet-update-deceleration \
  --unit bullet-update-vertical-wrap --json
scripts/repo-python scripts/ci.py
```

Final installed source SHA-256:
`4e2358af726818fa555e5c2ca9524dd4d4eb2c709ba8eb0fc52bf8014106f424`.
Full pinned variant source/image/PDB and original-bound comparisons:
`.analysis/gpt-web/20261107-bullet-timer-wrap/probe.py` and `results.json`;
selected cold `.analysis/gpt-web/20261107-bullet-timer-wrap/installed-selected.json`,
with native PE/MAP/PDB in `build/gpt-web/20261107-bullet-timer-wrap/` and
`build/match-linked/BulletHorizontalWrap/`.
No all-unit cold replay, original source-TU identification, whole-game build
or runtime guarantee.
