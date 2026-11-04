# TH10 full 359-byte player option trail: reuse initial sampled X/Y words

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The independent `PlayerOptionTrailCallbackBody` at
**0x00427960..0x00427AC6** remains a **359-byte** physical owner; the
original includes the unreachable branch-alignment LEA sled, private ESI
option pointer, seven interpolated history samples and final option-mode
publication. Its 12-byte scheduler callback is a different already-exact
physical owner.

## The original-instruction-guided change

Target disassembly loads the initial X/Y from a selected history sample
into ECX/EDX, publishes them to the option's `replayPair0` at +0x34/+0x38,
and then in the zero-mode branch reuses those **same register values** to
subtract player position before publishing offsets at +0x44/+0x48. The
previous maintained source copied the history pair but then explicitly
read `replayPair0.x` and `.y` back from the option object in the zero-mode
branch; this selected extra loads and different register lifetimes.

`src/Player.cpp` now names the genuine initial history X/Y as
`const int initialX`/`initialY`, stores those exact values into the original
`replayPair0` members and reuses the two locals on the zero-mode branch.
The existing nonzero-mode history calculation, typed pair destination and
volatile staged coordinate writes are unchanged. No new data allocation,
forced register, embedded byte sequence, dummy call or alternate owner
is used. The history sample belongs to the player while the option record
is a separate existing typed object; the source preserves normal target
semantics without inventing an additional observable side effect.

## Full native comparison: large improvement, still NONEXACT

A fresh genuine VC7.1 SP1 `/GL` `RebuildPlayerOptions` producer compiled
primary `Player.cpp` with the real `AnmVmId.cpp` and `AnmManager.cpp` support.
The complete PDB contribution remains **359/359 bytes** against the actual
original. Five candidate address fields are present, but they are **not
accepted as all independently target-bound original fields**, and the body
still contains ordinary byte differences, so no exact-match row is added.

| Full owner source form | PDB / target | Diagnostic ordinary matching bytes |
| --- | ---: | ---: |
| Previous staged-coordinate source | 359/359 | 215/339 |
| Reload X/Y directly from history in zero mode | 359/359 | 210/339 |
| **Name and reuse both initially loaded X/Y words (selected)** | **359/359** | **245/339** |
| Copy initial pair to typed local, then reuse | 359/359 | 245/339 |
| Repeat full history indexing inside zero mode | 375/359 | 46/339 |

This is a **30-byte improvement** in the noncanonical ordinary positional
score, not 30 newly accepted exact bytes. The owner remains **NONEXACT**;
its complete target field geometry, ABI, and remaining instruction bytes
must close before a canonical match can be claimed. Old source hypotheses
and documentation were treated as falsifiable, not authoritative.

The source installed in the real working tree was separately cold-replayed
through the three accepted callback units. The existing trail scheduler
callback is **12/12 exact**, special callback **12/12**, and special callback
body **99/99**, total **123/123 accepted original bytes**. The source
SHA-256 was identical before and after this replay:
`862667c43725c7422b3c3dc1060d4ebe0dfb50145f7f52574a2c60623c7858a9`.
No all-unit cold replay was run.

Reproduce only the affected accepted units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

Private ignored actual VC7.1 results:
`.analysis/gpt-web/20261104-next/player-zero-source-reuse-results.json`,
`player-zero-reuse-selected-guards.json`, and
`player-zero-reuse-maintained-proof.json`.
Whole executable product, runtime equivalence and original source ancestry
are separate unproven gates.
