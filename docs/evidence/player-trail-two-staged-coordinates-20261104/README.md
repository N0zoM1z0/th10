# TH10 Player option trail: two staged coordinates, complete 359-byte source frontier

Original pinned Japanese TH10 v1.00a:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The physical `0x00427960..0x00427AC6` owner is 359 bytes, including its
original unreachable alignment sled. It has a separate 12-byte scheduler
callback at `0x00427950` and an internal private ESI option receiver.
This checkpoint changes the full **359-byte** natural
`PlayerOptionTrailCallbackBody` in `src/Player.cpp`, **not** the accepted
12-byte wrapper. Original C++ spelling/production TU remain unverified.

## Original-guided C++ change

The original nonzero-mode arm reads both player coordinates and both saved
option deltas, calculates both resulting coordinates, constructs the
8-byte replay-history destination, and stores X then Y as adjacent dwords.
The prior maintained source assigned history X, then loaded and assigned
history Y in a separate indexed expression. That reproduced the game logic
but scheduled the two coordinate stores differently in VC7.1.

The selected code now uses separate `volatile int nextX` and
`volatile int nextY` values, preserving both natural coordinate lifetimes,
followed by one typed `PlayerPositionPair *const destination` to the *real*
8-byte history entry, then `destination->x = nextX; destination->y = nextY;`.
This is type-safe normal C++ with the same actual source members and
preexisting option index. It is **not** an instruction patch, arbitrary
stack padding, synthetic register ABI, changed game branch, or new data
owner. Volatile is a *codegen-lifetime hypothesis*, not an assertion about
the original C++ declaration. Unlike a `int*` taken from the X member and
indexed beyond that individual member, the retained typed pointer stays
inside the actual `PlayerPositionPair` object.

## Native proof and negative controls

The same original-target-attested VC7.1 SP1 `/GL` graph uses primary
`Player.cpp` and real `AnmVmId.cpp`/`AnmManager.cpp` support with entry
`RebuildPlayerOptions`. The entire target 359-byte body and the entire
candidate PDB contribution are compared. The source's normal external
address fields (five in this candidate graph) are normalized *diagnostically*;
all original field offsets and linked targets still need canonical proof.

| Source form | Full PDB / target | Equal ordinary positional bytes |
| --- | ---: | ---: |
| Prior maintained final-two-sums shape | 359 / 359 | 209 / 339 |
| Separate ordinary nextX/nextY and typed object pointer | 327 / 359 | 31 / 339 |
| Two staged volatile coordinates with int-member pointer | 359 / 359 | 215 / 339 |
| **Two staged volatile coordinates with typed pair pointer (retained)** | **359 / 359** | **215 / 339** |
| Only nextX or nextY volatile | 343 / 359 | 32 or 56 / 339 |

**All 359 bytes and five candidate fields remain nonexact**: agreement
rises **209/339 → 215/339**, a six-byte diagnostic gain, but this does
**not** satisfy original target-bound linkage geometry, private ABI or
complete byte identity. No `config/matches.csv` row or canonical exact
byte credit has been added for this body.

With the installed source, a new **selected cold** canonical replay of
the existing three neighboring owners passes `player-option-trail-callback`
**12/12**, `player-option-special-callback` **12/12**, and
`player-option-special-body` **99/99**, totaling **123/123 accepted bytes**.
The installed source SHA-256 was the same before/after that replay:
`abcf7c2f7063b8de2b822ace9f3971843e96df5f0617421fa962e140582a6e78`.
No all-unit cold replay was requested.

Reproduce only the affected already-accepted guards:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

Private ignored reproducible receipts:
`.analysis/gpt-web/20261104-next/player-nonzero-pair-final-results.json`,
`player-typed-volatile-results.json`,
`player-typed-volatile-selected-guards.json`, and
`player-typed-volatile-current-proof.json`. This is complete-owner source
codegen improvement, not proof of original authorship, a runnable executable,
game runtime equivalence, or portability.
