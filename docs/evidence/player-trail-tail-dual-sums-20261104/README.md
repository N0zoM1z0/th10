# TH10 359-byte Player option trail: dual-sum final X/Y publication

Original pinned Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This checkpoint modifies the *complete* nonexact player option-trail owner
at **0x00427960..0x00427AC6** (359 bytes), not its separately exact 12-byte
scheduler adapter at `0x00427950`.

## New maintained source behavior

The original body handles mode transitions and seven intermediate history
samples, then publishes the current X and Y positions to
`option->replayPair0` (offsets +0x34/+0x38), and saves the previous mode
at +0x84. The original machine-code tail loads both player coordinates
and both replay offsets before completing both coordinate stores. The
older source directly assigned `replayPair0.x` followed by `replayPair0.y`,
letting VC7.1 write the first coordinate before it had computed the second.

The maintained natural C++ source now uses two ordinary `const int` result
locals, `sumX` and `sumY`, then performs the two original field assignments
in the same order as before. This preserves the real source value types,
member addresses, preexisting branch conditions and return behavior. The
real player position and saved option offsets are independent fields;
this is **not** an assembly/register shim, synthetic data owner or copied
original bytes.

## Actual full native PDB diagnostic (not an exact claim)

In the genuine `RebuildPlayerOptions` VC7.1 SP1 `/GL` three-source graph
(`Player.cpp`, `AnmVmId.cpp`, `AnmManager.cpp`), both before and after
candidates have a **359-byte PDB-owned function against the 359-byte target**.
After normalizing the five *candidate* external address fields,
preliminary positional agreement increases from **205/339** to **209/339**.
Both results are **NONEXACT**; the five candidate fields alone do not prove
all original target field locations or the original private ESI transport.
The completed native owner, nonidentical byte values, and remaining call
geometry are retained as unresolved work, with **no** `config/matches.csv`
credit for the 359-byte target.

The following natural, value-preserving alternatives were each actually
compiled against the *complete* owner, then restored when not selected:

| Candidate | PDB bytes / target | Ordinary positional equal |
| --- | ---: | ---: |
| Four separately named input loads | 359 / 359 | 204 / 339 |
| Compute X/Y results before writes (**retained**) | 359 / 359 | **209 / 339** |
| Compute Y result before X | 359 / 359 | 209 / 339 |
| Create then assign a two-int pair (X before Y) | 359 / 359 | 207 / 339 |
| Create then assign a pair (Y before X) | 359 / 359 | 181 / 339 |
| Pair memcpy into destination | 359 / 359 | 207 / 339 |
| Save previousMode before coordinates | 359 / 359 | 202 / 339 |

The selected current `src/Player.cpp` source SHA-256 was verified unchanged
before and after a fresh **selected cold** canonical callback replay:
`37958e3a73e3c4a1d6f175642972098352fba1fec4583cbaa562fab05d51d036`.
The three existing accepted callback units remain exact:
trail wrapper **12/12**, special wrapper **12/12**, special body **99/99**;
**123/123 total**. These were regression guards, not new 359-byte credit.

Private native receipts are ignored under
`.analysis/gpt-web/20261104-next/player-tail-lifetimes-results.json`,
`player-tail-pair-results.json`,
`player-tail-retained-cold.json`, and
`player-tail-retained-proof.json`.
Reproduce **only affected canonical units**:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

Historical source attribution, original game product/runtime behavior and
portability are not established by this codegen improvement.
