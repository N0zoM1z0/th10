# TH10 EnemyManagerUpdate: native signed timer word increment

Target: Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original `EnemyManagerUpdate @0x0040D750` is a complete **166-byte**
function, reached through an independent **10-byte** callback at
`0x0040D810`. Their source owner is `src/Enemy.cpp`, with the real
VC7.1 SP1 `/GL` callback entry used for canonical verification.

The manager timer's `current` field is a signed Win32 32-bit integer. At
original offset `+0x7D` the i386 code emits `INC EDX` and stores EDX
back to the same timer word; this wraps at the signed boundary. The prior
source `++manager->timer.current` invokes signed-overflow undefined
behavior at `INT_MAX`. The maintained source now explicitly adds 1u to
the unsigned representation and publishes that low 32-bit word back to
`int`, preserving the original signed timer comparisons, previous-value
publication, floating subframe addition, and interpolated-timer fallback.
No new range check or malformed-state safety policy is introduced.

## Complete native compiler evidence

Three natural source forms were compiled and linked in the actual full
single-TU VC7.1 SP1 `/GL` `EnemyManagerUpdateCallback` graph: unsigned
word add followed by signed publication, unsigned lvalue increment,
and unsigned aliased `+= 1u`. All three reproduced the **entire
166/166-byte original manager update** with all **5 target-bound fields**
and **10/10-byte callback** with its original field. The simplest
unsigned-word-add source was installed; a separate selected original-bound
cold replay from installed source passed **176/176 bytes and all 6/6
original fields**.

This is a Win32 source semantic repair of already accepted exact owners,
**not** a new exact function or byte-credit increase. It does not establish
whole-game runtime equivalence or an original human source spelling.

Selected reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-manager-update --unit enemy-manager-update-callback --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Private trial script/complete PE-PDB-MAP artifacts and receipts:
`.analysis/gpt-web/20261108-enemy-manager-timer-increment.py`,
`.analysis/gpt-web/20261107-active/enemy-update-timer-results.json`,
`.analysis/gpt-web/20261108-enemy-update-timer-installed.json`,
`build/gpt-web/20261108-enemy-manager-timer/`.
