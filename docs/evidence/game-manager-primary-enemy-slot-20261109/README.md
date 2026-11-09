# GameManager frame update: recover primary enemy-slot gate

Target: Japanese TH10 v1.00a `0x00418190..0x00418797` (1,544 bytes).
The active source function is `GameManagerUpdateBody` in
`src/GameManager.cpp`. Its actual owner source layout, private LTCG ABI and
full 1,544-byte code generation remain unresolved.

## Target-grounded field

At `0x00418740` the target loads the enemy-manager pointer from
`[0x00477704]`, then at `0x00418746` compares the DWORD at
`manager+0x10` to zero. A null value is followed by two independent gates:
the GUI message pointer at `g_GuiView+0x9EB8` must be null, and the
game stage timer at `GameManager+0x14` must be at least 90.

The existing typed `EnemyManagerView` layout identifies offset `+0x10`
as `specialEnemySlots[0]`. The separately measured active-enemy count is
at `+0x60`, not `+0x10`. Consequently, the previous source expression
`*reinterpret_cast<int *>(...+0x10)==0` concealed a real pointer-null
test. The source now names `g_EnemyManager->specialEnemySlots[0] == NULL`
and asserts `offsetof(EnemyManagerView, specialEnemySlots)==0x10`.
No new branch, null repair or alternate game behavior is added.

## Selected native verification

After editing the real source, pinned VC7.1 SP1 `/GL /GS` rebuilt the
affected `game-manager-update-callback` graph and the **entire**
`GameManagerUpdateBody` PDB contribution. The update body is still
**1,538 vs 1,544 target bytes (NONEXACT)**, with 114 candidate relocation
fields. No byte credit is taken for this still-open large owner.

The two selected existing exact guards remain original-bound exact:

- `game-manager-update-callback`: **7/7 bytes**, one bound field;
- `game-clear-linked-objects`: **66/66 bytes**, one bound field.

Together the focused original-bound guards cover 73/73 bytes. This is
a typed field/condition reconstruction in the large function and not
whole-function or runtime equivalence.

Reproduce with:

    scripts/repo-python scripts/build-match-unit.py --unit game-manager-update-callback
    scripts/repo-python scripts/compare-linked-function.py --unit game-manager-update-callback --json
    scripts/repo-python scripts/compare-linked-function.py --unit game-clear-linked-objects --json
