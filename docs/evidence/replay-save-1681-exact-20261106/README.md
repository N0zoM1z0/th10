# Replay save — complete 1681-byte exact owner

Original Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`ReplayManager::SaveReplay` owns `0x00429B60..0x0042A1F0`, **1681 bytes**.
The reviewed private interface is ECX owner, EDX replay path, one stack
replay name and RET 4. Existing high-confidence boundary and authored-game
origin reviews are unchanged.

The updated MIT reference at
[`d1f31212514781c928fba145d8207397c8fb5650`](https://github.com/levzzz5154/th10-decomp/tree/d1f31212514781c928fba145d8207397c8fb5650)
provides useful source leads in `matching2/msvc/src/game/replay/replay_save.cpp`.
Its variable lifetimes and repeated frame measurements informed this local
source model; attribution is in `src/ReplayManager.cpp`, with the retained
[MIT notice](../../licenses/th10-decomp-MIT.txt). Acceptance comes from the
original TH10 image and pinned local VC7.1 comparison.

The installed source restores the entry NUL-copy loop and early stage state,
uses ordinary frame-size/count helpers at each actual read, and obtains the
score from shared `g_GameScoreState.score`. The latter is original address
`0x00474C40 + 4`, not a separate replay-only score object. The compressor's
output-length variable has its own scope. The header's compressed-size field
is reread to compute the USER offset; USER padding and writes consume the
actual cursor difference and header length. Reusing the allocation byte count
as the packed byte count also restores the last indexed-address encoding.
No forced inlining, fake ABI, inert padding or compiler-profile change is used.
The original spelling, helper ownership and production TU remain unknown.

Both complete images pass **1681/1681 bytes, all 83 original-bound fields**:
27 REL32 and 56 DIR32. Every instruction, local destination and return is
included, with no data-span exclusion. Field destinations were decoded from
the original independently: 59 uses bind existing canonical symbols, 15 bind
complete matching NUL-terminated source literals, and nine uses bind reviewed
storage, data or callee roles. The latter include the shared score addend,
100.0f bits, original USER format bytes, character/difficulty tables and the
actual Encrypt interface. Adjacent four-byte harness-anchor names do not
override the genuine shared score base plus addend.

The first final-source image recompiles the changed ReplayManager TU and
performs a fresh nonincremental LTCG link, reusing 20 unchanged source/header
pinned objects from the fresh baseline. The independent canonical cold
recompiles **all 21 actual inputs** with their established per-source profiles,
including ReplayManager `/GL /GS /EHs`. Its real InitializeVm-root graph is
the existing `enemy-read-int-operand` graph, so both units share canonical
output `build/match-linked/EnemyReadIntOperand/`:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit replay-manager-save --json
```

The PE and PDB identities differ between the two images; all 83 source/header
pins remain unchanged. Both images additionally pass complete Destroy18,
Load181, OpenWriteFile137 and WriteToOpenFile76 comparisons: **412 existing
exact bytes / 33 fields**, including Load's required EH bindings. The cold
image also preserves the shared artifact's ReadIntOperand820 exact owner.
These guards earn no new credit. New authored exact credit is **one owner /
1681 bytes**. No historical cohort is rebuilt or replayed.

[Compact results](native-result.json) retain source, image, PDB, original-field
and comparison pins; the canonical manifest holds the reproducible recipe and
all field bindings. Full receipts and private analysis remain under
`.analysis/gpt-web/20261106-replay-save-scope/`. The historical October 30
1664-byte candidate and this turn's fresh 1669-byte baseline are superseded
source epochs. Encrypt's complete native body remains nonexact (286 bytes
versus original 307), with its actual AL seed and RET 20 interface preserved;
this checkpoint grants no Encrypt credit. No host failure campaign, runtime,
product closure or portability claim is made.
