# GUI message teardown and parent controls — 2026-10-31

The complete GuiMessageVmView destructor at `4136C0..413720` reproduces all
**97 bytes and seven fields** in two independent 34-input canonical cold links.
Its ESI receiver, six real MarkVmForDeletion calls, sole plain return and every
instruction agree. This adds one authored exact owner and97 bytes; mappings,
boundaries, origins and the authored denominator stay fixed.

## Mark/clear order

Target decoding observes six ordered triples: read the id at40/44/48/4C/50/54,
call the actual manager marking operation4492A0, then clear that same id before
reading the next one. The manager is captured once in EDI. The final clear stays
between the two final register pops; there are no conditional exits or EH fields.

The previous97-byte native body hoisted each next id read ahead of the previous
clear. Scoped volatile reads/writes preserve the order but choose EAX for every
id, leaving eight opcode bytes wrong. Volatile writes alone retain both wrong
scheduling and different bytes. Neither candidate is accepted.

The retained source adds a compiler memory barrier between each adjacent
mark/clear pair. These five intrinsics emit no instructions; the complete target
order and original typed-id call operands then reproduce naturally. No registers,
ABI arguments, returns, bytes or padding are manufactured. This is a maintained
source model of observed memory order. **Original qualifiers or barriers remain
unknown**; exact codegen does not establish an original spelling or threading
contract. The final pair needs no inter-pair ordering operation and retains the
observed pop/clear/pop epilogue.

## Complete parent controls

The batch first investigated full GameManager cleanup1105. All attempts compare
the complete owner, never a prefix or equal-length crop:

| Control | Native/target | Result |
| --- | --- | --- |
| Whole mode switch | 1120/1105 | Wrong jump table; some common flag stores recover |
| Nested tail switch | 1091/1105 | Wrong destructive subtraction dispatch and block placement |
| Actual shared score-owner address | 1098/1105 | No complete equality |
| Separate flag assignments | 1098/1105 | No complete equality |
| Branch-local publication value | 1118/1105 | Extra instructions; common target writeback not recovered |

All five edits are restored. Full update1538/1544 was independently inspected:
its differences include the stage helper's EDI versus target stack interface,
manager register allocation, statistics member addressing and a redundant target
null check. Its six-byte size gap is not a six-byte equality claim. No update
source change is retained.

A nonthrowing PlayerDrawVmView destructor declaration/definition also leaves
Player teardown333/325, including its extra EH-state write. That control is
restored. Existing /GL and all effective compiler/linker profiles stay fixed.
Cleanup1098/1105, update1538/1544, Player333/325 and GUI579/574 remain nonexact.
These controls are parked until new evidence distinguishes another hypothesis.

## Scoped acceptance

The first zero-reuse canonical run selects the new message destructor and its
direct GUI reuse caller:218 bytes. The second selects only the new97-byte owner.
Both use the existing GameCleanupContext producer and unchanged34-input graph.
Each produced image also passes46 selected existing contracts6261/all275 and the
protected startup1009/all102 with its qualified complete28-byte static callee.
The actual marking callee also matches its complete66/all1 contract in each
produced image; its existing normal-COFF recipe is unchanged. These comparisons
read the already produced image and add no build.
No prior rejection is waived and no historical full cohort is replayed.

Independent complete target/native inventories verify all seven external fields,
all six calls, zero local branches and the return. Frozen PE/MAP/PDB identities
agree with their original producer proofs; cold image hashes and PDB GUIDs are
independent. [results.json](results.json) records the source/header84 pins, full
contract, complete inventories, ordered memory events, ten controls and cold
identities. Complete read-only diagnosis of the first frozen cold image again
rejects all four open parents at the reported extents. Private logs and retained
cold/warm artifacts are under the
`cleanup-switch-20261031` campaign. Jobs are serial, CPU0/nice10 and bounded.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit gui-message-destructor --unit gui-prepare-reuse --json
scripts/repo-python scripts/replay-exact-units.py --unit gui-message-destructor --json
scripts/repo-python scripts/ci.py
git diff --check
```

There are no comparator changes or host failure/lifecycle tests. Authored exact
becomes107618/275863 across520 owners; canonical175732/1398, mappings1221/
authored703/backlog183. Product, runtime, original ownership and portability
remain separate.
