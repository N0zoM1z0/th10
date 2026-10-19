# GameManager callback retirement: complete static owner

The existing `MarkGameChains` source in `src/GameManager.cpp` reproduces
the complete 28-byte target owner at `0x00409E20` without a C++ source edit.
Two independent cold VC7.1 SP1 `/GL` links pass the existing canonical
comparator: 28 bytes decoded, raw equality, zero linkage fields, zero data
spans. The source mapping and `game-manager-mark-chains` unit close an
omission in the maintained ledgers. The existing authored-origin decision
and 266,187-byte authored denominator are unchanged.

Ghidra confirms the target caller at `0x00417C53`, in
`GameManagerInitialize`. The helper reads callback pointers at owner `+8`
and `+12`, tests each for null, and ORs bit 1 into each nonnull node's flags
at `+4`. Existing partial source views describe precisely those fields.

The xref result is not an exhaustive caller inventory. A full target `.text`
REL32 byte scan also finds possible calls at `0x0040A655` and `0x0040A685`,
inside the untracked interval between the reviewed owner ending at
`0x0040A005` and the next candidate at `0x0040A940`. Independent short code
begins at `0x0040A010` and ends in RET at `0x0040A033`, followed by CC padding
and another code start at `0x0040A040`. The two additional call candidates
and larger gap require fresh inventory/body-range review; they are not new
authored or exact credit in this checkpoint. The reference's `game/eclview/`
sources supply hypotheses for this next major-owner route.

The emitted helper appears in the MAP's **Static symbols** section rather
than its public-symbol list. The existing `owner_visibility = "static"`
lane independently requires the exact decorated symbol and producer object,
a unique same-start executable PDB contribution, no colliding MAP start,
no overlapping contribution, and matching PE/MAP/PDB identities. It does
not crop a larger function. No comparator, decoder, compiler flags, ABI
annotation, or normalization rule was changed.

## Reproduce

```sh
scripts/repo-python scripts/build-match-unit.py --unit game-manager-mark-chains
scripts/repo-python scripts/compare-linked-function.py --unit game-manager-mark-chains --json
scripts/repo-python scripts/compare-linked-function.py --unit game-manager-update-callback --json
```

The new owner shares the existing GameManager update callback's source,
profile, real entry and output graph. The same-artifact callback guard passes
its complete seven bytes. Full source and artifact bindings are recorded in
[`summary.json`](summary.json).

## Scope and limits

This is a bounded local codegen result. The diagnostic's `/OPT:NOREF`
retains the out-of-line static copy even though its callers inline the
operation. In particular, the diagnostic initializer does **not** establish
the target's incoming call edge to this helper. Its complete owner remains
1,039 candidate bytes against 1,009 target bytes. No parent exactness,
original declaration, conceptual owner, production TU, product link, runtime
behavior or Factory acceptance is inferred.

An isolated member-function alternative also produced 28 raw-equal helper
bytes, but changed the initializer to 1,021 bytes and did not close it.
That alternative was discarded; the registered unit uses unchanged source.

The source-shape investigation consulted
[the issue's reference repository](https://github.com/N0zoM1z0/th10/issues/1),
fixed at
[`c6791d82`](https://github.com/levzzz5154/th10-decomp/tree/c6791d82dc9f02e55eca45a5314f900a85481efd).
Its source is MIT licensed. No upstream implementation was imported; the
matching helper already existed locally. Upstream status, synthetic contexts,
assembly fallbacks and equivalent-region allowances provide no local credit.

## Other bounded controls in this checkpoint

[`frontier-controls.json`](frontier-controls.json) records fresh ECL/Bullet
diagnostics and their source/graph hashes. These are negative experiments,
not new exact owners:

- ECL Run stays 7,020 bytes with all 60 table destinations and 88 selector
  bytes aligned. Five integer arithmetic cases using explicit operand
  snapshots reduce ordinary comparable agreement from 6,125/6,264 to
  6,119/6,264. Both same-image existing exact controls pass. The candidate
  was discarded; the 88-byte undecoded selector suffix and other differences
  remain open.
- VerticalWrap stays nonexact. Reassociating its upper-wrap expression
  produces identical candidate bytes. Lexical duplicated tails and delaying
  its reference lifetimes each produce 166 bytes against the target's 172,
  and regress agreement. All preserve the existing 148-byte horizontal-wrap
  control. No candidate was adopted.

These findings bind the recorded source/tool context; they are not a claim
that other natural implementations or future contexts cannot work.
