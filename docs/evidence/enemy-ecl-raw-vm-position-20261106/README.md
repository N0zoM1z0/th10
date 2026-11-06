# Enemy ECL PLAY_ANM: preserve three native position DWORDs in VM offset transfers

Target: Japanese TH10 v1.00a executable (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).

The `PLAY_ANM`, `PLAY_ANM_HIGH` and `PLAY_ANM_ROTATED` opcode paths in the
complete Enemy ECL dispatcher transfer an exact three-DWORD world-position
triple to each created `AnmVmView::positionOffset`. Depending on the real
`flags & 0x40000` branch, the source words are either the *actual*
`PublishEclWorldAnchor` projected result, or the unmodified
`worldMotion.position` floating-point bit patterns. The original transfers
all three words with 32-bit loads/stores: conversions, sign changes, NaN
normalization or assignment to a newly computed float value are not
permitted in this path.

The maintained `EnemyPositionDwordView` is a strictly 12-byte three-word
non-owning storage overlay with compile-time native layout assertions. The
three extended opcodes now use `word[0..2]` at the existing exact original
addresses, replacing **18 repeated independent integer-pointer casts**.
The original projection call, word read/write order, VM ID lookups and
branch conditions are unchanged. This does not infer original source class
names, allocate another vector, modify the ABI, or introduce an alignment or
null guard that the original does not have.

## Fresh native whole-boss and selected exact leaf evidence

A fresh pinned VC7.1 SP1 **11-TU `/GL`** build, including the changed
`Enemy.hpp` and `EnemyEclDispatcher.cpp`, was compared with a source-pinned
preceding complete native graph. Full PDB-owned owners retain their entire
normalized candidate code and identical field geometry:

- Enemy ECL dispatcher: **14,352** candidate bytes, **716** fields, zero
  candidate ordinary differences.
- BeginSpell: **2,432** bytes / 108 fields; death finalizer **162/9**;
  viewport **330/10**; full Enemy constructor **672/5** — all zero
  candidate-code regression.

Selected *original-target-bound* cold replay under the updated source:
`enemy-read-int-argument` **16/16** and `anm-vm-id-get-vm` **21/21**,
**37/37 strict exact**. All seven lightweight CI stages and whitespace
checks passed. No historical full cold replay was performed.

**NONEXACT:** the real Enemy dispatcher is **14,416** bytes, so the
14,352-byte native candidate and its 716 *candidate* fields do not earn
new exact ledger credit. A byte-neutral native representation improvement
is not full original-target codegen acceptance, runtime or product closure.

Private evidence:
`.analysis/gpt-web/20261106-enemy-vm-position-bits/verify.py`,
`build/gpt-web/20261106-enemy-vm-position-bits/current/native-result.json`,
`.analysis/gpt-web/20261106-enemy-vm-position-bits/selected.json`.

Installed SHA-256: `Enemy.hpp` `8dadcfe889812dcf508e17d855c49d94463bf127949d834bb0d8f905e3bcb3f9`, `EnemyEclDispatcher.cpp` `187d8e5720fb9c663c811ecd09293aa0a8cd9b3fc7abf41cb188a5f507ab95df`.
