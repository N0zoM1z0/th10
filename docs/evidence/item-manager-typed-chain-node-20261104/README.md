# TH10 ItemManager: unify registered callbacks with actual ChainElem ownership

Original Japanese TH10 1.00a executable SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The maintained ItemManager callback and lifecycle source now uses the
**real** `th10::ChainElem` object exposed by `Chain.hpp`, instead of a
second decompiler-shaped `ItemChainElementView` with an independently
invented layout.

The original Item registration routine at `0x0041AD90` allocates a
0x24-byte Chain node, clears bit 1 of node flags at +0x04, stores the
ItemManager receiver at node +0x20, inserts it at priorities 21/25 and
publishes the links at manager +0x08/+0x0C. Its destructor at `0x0041ADF0`
removes these exact objects using `Chain::UnregisterElem`. The existing
qualified Chain ownership and its `arg` member were already source-recovered.

The prior separate Item-only structure duplicated both the Chain flags and
argument layout and required casts on allocation, insertion and removal.
Now the two native callbacks in `ItemManager.hpp` are `th10::ChainElem*`,
the local nodes in `ItemManager.cpp` are the same type, `node->arg` stores
the real manager pointer and `Chain::AddToCalcChain`, `AddToDrawChain` and
`UnregisterElem` all receive the actual node pointer. The source retains
independent compile-time checks for `flags` +0x04 and `arg` +0x20, along
with the previously established exact overall ChainElem size +0x24.
No new shadow structure, pointer alias ABI or target-byte injection.

## Real source/target validation

Scoped pinned VC7.1 SP1 cold replays on the **installed modified source**:

- `item-manager-register-callbacks-0041ad90` — 84/84 exact, 8 target fields
- `item-manager-destructor-0041adf0` — 216/216 exact, 21 target fields and one-state vector EH proof
- `item-manager-creator-0041aed0` — 185/185 exact, 13 original fields and allocation EH proof
- `item-manager-update-callback` 48, draw callback 29, power display 79,
  item-record constructor 104, record destructor 34 — all exact

In total **8 individually selected complete functions, 779/779 exact bytes**.
The source is recovered; **no new exact unit** is claimed, since all these
physical functions were already independently credited. The existing full
281-byte ItemManager draw remains NONEXACT. Unmodified source inputs are
not inferred from historical notes: current hashes and selected actual
unit reports are in [native-evidence.json](native-evidence.json).

No all-cohort cold replay, full Windows product build, runtime equivalence
or original historical C++ symbol spelling is asserted.
