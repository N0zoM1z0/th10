# Chain allocation owner: restore 74-byte shape, retain nonexact status

The original Japanese TH10 1.00a function `th10::Chain::AllocElem`,
`0x00449ED0..0x00449F19`, is a complete 74-byte function. The preceding
natural source in `src/Chain.cpp` produced a **69-byte** function in the
GameManager/Chain/ANM/ErrorContext four-source `/GL /GS /EHs` graph. Its
constructor read/modify/write of `ChainElem::flags` was incorrectly scheduled
before some callback clear stores. The original target loads the existing
flags, clears the callback/priority fields, then clears only the ownership
bit and finishes the remaining pointer stores.

The maintained source now expresses the previous-flag read explicitly and
places a scoped compiler memory barrier before constructor callback stores
and another before publishing the updated flags. This preserves all other
flag bits and the actual allocation/null branch and callback registration;
no copied target bytes, synthetic ABI, padding, assembly or manufactured
return path is used. Qualifier and barrier spelling is a **compiler ordering
model**, not a claim about original source or thread synchronization.

The fresh selected actual source link has a complete **74-byte** PDB
contribution. The strict native comparison has the one real operator-new
REL32 at original `+0x03`, bound to `0x00452493`; it reports **52/74**
matching ordinary bytes with 22 different bytes, including ECX/EDX zero
register allocation and postconstructor callback/flag scheduling. This is
**NONEXACT**: no match ledger entry and **zero new exact bytes**.
A single barrier before the callback clears, with the same volatile flag
load, also emitted 74 bytes but regressed to 46/74; an absent early barrier
was byte-neutral with the 52/74 candidate. Earlier callback-lifetime
variants were either unchanged or emitted a 68/69-byte extent. The retained
version is selected because it maintains the target's full extent and
better register-independent write sequence, without claiming the unresolved
register assignments.

This does not alter the already exact GameOwner creator/initializer. Scope
validation to their directly affected four-source graph; no full historical
cold replay, full GUI consumer graph or runtime tests are implied:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit game-owner-40af90-create --unit game-owner-40ae70-initialize --json
scripts/repo-python docs/evidence/chain-alloc-74-20261102/verify-native.py
scripts/repo-python scripts/ci.py
git diff --check
```

Original physical source, complete product closure and runtime/portability
are not established. The strict comparison retains all ordinary bytes and
all original branch targets: equal body length is not acceptance.
