# TH10 full 2,432-byte EnemyBeginSpell — first child node/owner reload

Target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The first child-script search for `0x19F` inside `EnemyBeginSpell` was
previously represented by a cached `AnmVmView *child` whose value was
set inside the loop. The original target disassembly from
`0x004097C8...` instead retains the ANM layer-list node, branches
when its owner's `scriptIndex` matches, then reads `node->owner`
**after** the loop at original owner-relative `+0x57D`. The new
maintained source captures that node/owner lifetime and preserves the
original null-result behavior before the unconditional `intVar2` write.
This is a correction to the actual 2.4 KB function, not a toy helper.

Full native **eleven-source VC7.1 /GL** linked contribution remains
**2,424 bytes**, compared to a **2,432-byte original target**, with
**108** candidate linked address fields in identical positions/types.
An explicit ordinary-byte *diagnostic* masking only those candidate
field offsets shows 1,185 -> **1,235** equal bytes against the target;
in the first lookup block `+0x53F..+0x58A`, equality improves **18/76
-> 67/76**. The improvement is an independently observed, real
compiler output effect; these are **not** strict target-bound field
bindings, and neither candidate-to-target equality nor new canonical
exact credit is claimed. The 8-byte owner extent gap remains.

Retained complete linked candidate artifact and per-variant shape:
`build/gpt-web/20261105-boss-loop/node-resolve-first/`.
Selected independently replayed exact neighbors
`enemy-ecl-resource-load-package` 239/239,
`enemy-configure-interrupt` 48/48 and `enemy-cancel-bullet-record`
199/199 remain exact. Seven-stage lightweight CI and `git diff --check`
pass. No all-unit replay or game runtime claim.
