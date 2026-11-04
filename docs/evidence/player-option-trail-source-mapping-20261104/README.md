# TH10 Player option trail: correct full 359-byte source-presence mapping

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The physical 359-byte owner at **0x00427960..0x00427AC6** has a complete,
existing natural source body in `src/Player.cpp`:
`PlayerOptionTrailCallbackBody`. It was present before this checkpoint, but
`config/functions.csv` had an **empty** `source_file` while the handoff and
knowledge base already described the function as source-present. This
checkpoint resolves the contradictory tracking without asserting source-era
identity or any new exact byte credit.

## Original owner and current source behavior

The observed `0x00427950` 12-byte wrapper is a separate exact function;
it adapts its ECX option pointer into the native body register ESI, invokes
`0x00427960`, then returns zero. The original body reads the real global
`g_Player` at `0x00477834` and the current option mode. It accesses a
0x98-byte `PlayerOptionRuntime` element and index at **+0x88**, stores
its current replay sample at **+0x34**, maintains the difference pair at
**+0x44** for the zero mode, and fills seven intermediate replay history
samples with the original `0.125f` fraction for the other mode. Both paths
refresh the pair at **+0x34** and save the mode at **+0x84**. The native
physical body has an unreachable six-byte LEA alignment sled, which must
remain part of the complete owner in any future exact comparison.

## Current source graph (no codegen acceptance)

A **new**, selected cold VC7.1 SP1 `/GL` build of the actual
`Player.cpp` + `AnmVmId.cpp` + `AnmManager.cpp` graph was executed through
entry `RebuildPlayerOptions`. Its complete PDB contribution for the trail
body is **355 bytes**, against **359 native**; the diagnostic reports
**92/339** ordinary comparable bytes and five candidate fields, which are
not an exact complete-owner alignment. This is **NONEXACT**, and no
`config/matches.csv` row has been added for `0x00427960`.

The scoped canonical replay simultaneously passed the separately accepted
12-byte trail wrapper, 12-byte special wrapper and 99-byte special body:
**123/123 exact existing bytes**. The source implementation and these
previous exact units were not changed; the only correction is recording the
previously omitted body source mapping and its factual compiler limitation.

Targeted reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback \
  --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

Ignored current native receipts under `.analysis/gpt-web/20261104/`:
`player-option-scoped-replay.json` and `player-option-trail-current.json`.
This does not establish original authored/compiler-generated origin,
production TU, whole-product closure, runtime correctness or portability.
