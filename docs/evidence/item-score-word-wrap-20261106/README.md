# Exact native score/rank word arithmetic without signed overflow UB

Target: original Japanese TH10 1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Two actual TH10 `GameScoreStateView` helpers use **32-bit integer addition
followed by signed clamping**. Their original instructions were directly
reviewed:

- `AddRank @0x00405B60` loads state +`0x58`, performs `ADD EDX,ECX`, writes
  the word, then compares it *as signed* to +1024/-1024. An overflow before
  clamping must use the wrapped 32-bit signed value.
- `AddFaith @0x0041BE80` computes signed `amount/10`, loads state +`0x0C`,
  performs `ADD EDX,EAX`, writes the word and clamps signed results above
  99999. The original stores the wrapped word before taking the cap branch.

The former C++ used `rank += amount;` and `faith += amount/10;`, both having
undefined behavior on signed overflow. The reconstructed source now uses
unsigned 32-bit addition, then retains original signed bound comparisons and
storage. `AddRank` uses the well-defined signed/unsigned corresponding-type
reference alias of the same 4-byte member. `AddFaith` uses an unsigned sum
followed by the target VC7.1 i386 word conversion to signed `int`. Both
produce the exact original machine code in the pinned compiler.

Compiler feedback matters: a direct unsigned `faithWord` alias compiled to
only **42 target 46 bytes**, modifying the clamp's register schedule. That
candidate was rejected. The final natural unsigned-sum/signed-publication
form emits **46/46 original bytes**. `AddRank` emits **42/42** unmasked
original bytes. Both owners have zero external linkage fields, so every
byte is raw-compared with no masking.

## Focused native checkpoint

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-rank-add --unit item-faith-add --json
```

Result: **exact** for both, **88/88 bytes**, from a fresh pinned VC7.1 SP1
one-input `/GL` build of `src/ItemManager.cpp` with the real adjacent polar
vector helper as entry. Exact results, target masks (none), PDB identities
and compiler receipts are at
`.analysis/gpt-web/20261106-chat-th10/item-word-selected-final.json`.
Source SHA-256: `5a6456fb03757f25ad6769f43fdd627858a5f08d3bbd137aa8f5d86e11043073`.
Seven lightweight CI stages and `git diff --check` pass.

These are stronger native **source semantics** for previously accepted exact
owners. There is *no new canonical exact credit*, no whole-game image, no
runtime or cross-platform equivalence claim.
