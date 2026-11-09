# Complete 91-byte enemy-laser update callback exact (0x0041C480)

Original target: Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

## Source reconstruction

The original update chain callback at 0x0041C480..0x0041C4DA is installed
by EnemyLaserManagerStorageView::Initialize. Its true GameManager flags
are at global [0x00477810] plus +0x58. A native 32-bit C++ bitfield
overlay defines paused (bit0), frozen (bit1), idle (bit2) and deferred
(bit10), with compile-time word-size and offset assertions. The source
does not create another manager or shadow flag word.

The callback skips updating the enemy laser list when paused or idle
or when deferred. With the frozen bit it saves global game speed
(0x00476F78), zeroes it for the linked laser update, restores it, and
returns the updater's result in EAX. With no frozen bit, it directly
returns the updater's result.

This direct natural C++ representation is important to code generation:
the previous cached unsigned-int flags implementation emitted an 87-byte
callback using AH/AL tests. The original uses MOV EDX,[g_GameManager]
followed by MOV EAX,[EDX+0x58], and later MOV EDX,EAX / TEST DH,4 /
TEST DL,2. The new bitfield view emits the same sequence without inline
assembly, dummy instructions, hand-patched bytes or relocation waivers.

## Two complete canonical linked-PE proofs

The selected callback has a real PDB-owned **91-byte** extent, densely
decoded and ending in RET before the independent CC separator.
Two independent selected-only pinned VC7.1 SP1 /GL builds each prove
**91/91 exact**, zero ordinary mismatches, and six source-linked fields
matching the actual target instructions:

| Offset | Type | Target address | Meaning |
| --- | --- | --- | --- |
| 0x03 | DIR32 | 0x00477810 | GameManager global pointer |
| 0x2F | DIR32 | 0x00476F78 | save game speed |
| 0x39 | DIR32 | 0x00476F78 | zero game speed |
| 0x42 | REL32 | 0x0041C330 | linked-laser update |
| 0x4C | DIR32 | 0x00476F78 | restore game speed |
| 0x54 | REL32 | 0x0041C330 | normal-lane update |

All **67/67 ordinary bytes** match. The two proofs are retained in
first-link-report.json and second-cold-report.json. The target code does
not have a supported independent original TU name or runtime equivalence
proof; only the listed complete function extent is credited.

## Focused neighbor checks

The current-source Laser draw-callback source graph was rebuilt and two
exact original function boundaries were separately compared:

- EnemyLaserDrawCallback @0x0041C4E0: **46/46 exact**; one target field.
- AccumulatePointChecks @0x0041C880: **54/54 raw exact**.

The unchanged selected neighbor exactness covers **100/100** bytes. The
separate 274-byte EnemyLaserManagerStorageView::UpdateLinkedLasers is
not counted as newly exact. This evidence gives zero double credit for
any previously completed leaf.

Only the laser callback's own compilation/link graph and its two selected
neighbor checks were rebuilt. No full cold replay.

Reproduction:

    scripts/repo-python scripts/build-match-unit.py --unit enemy-laser-update-callback-bitfield
    scripts/repo-python scripts/compare-linked-function.py --unit enemy-laser-update-callback-bitfield --json
    scripts/repo-python scripts/build-match-unit.py --unit enemy-laser-draw-callback-0041c4e0
    scripts/repo-python scripts/compare-linked-function.py --unit enemy-laser-draw-callback-0041c4e0 --json
    scripts/repo-python scripts/compare-linked-function.py --unit enemy-laser-accumulate-point-checks --json
