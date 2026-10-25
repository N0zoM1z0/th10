# GameManager delete and real timer, 2026-10-25

## Two bounded models after a fresh baseline

The actual producer is **single-TU GameManager.cpp, /GL without /GS, OnUpdate
entry**, with no support TUs. A fresh independently reviewed baseline preceded
two separately measured models. All native stages exit 0 and retain three
strict guards, **127 bytes / all 2 fields**.

| Complete GameManagerUpdateBody | Extent versus target 1544 | Result |
| --- | --- | --- |
| Fresh current baseline | 1587 / 1544 | Nonexact |
| Explicit scalar-delete call only | 1587 / 1544 | Nonexact |
| Existing real Tick definition | 1581 / 1544 | Nonexact |

Final inventory retains **114 fields and 55 local branches**; these are not
canonical target bindings. Extent differences are not mismatch-byte counts.
**Zero new exact bytes, source owners or denominator bytes** are credited.

The delete-only model changes free(menu) to explicit global operator delete.
Independent review finds just one raw byte change within the sole release
field, now naming the actual scalar deallocator rather than remapping free.
Captured owner, explicit cleanup, null/bit-8 gates and ordinary bytes remain
unchanged. No typed delete or duplicate destructor is introduced.

The second model changes stageTimer to the same-layout AnmVmTimerView, calls its
existing Tick and removes the unused GameAdvanceTimer declaration. Five i386
fields retain offsets 0/4/8/12/16, size 20 and the manager's timer offset 0x10.
LTCG naturally outlines the real **72-byte Tick / all 4 fields exact**, using
**ESI as receiver with no caller argument cleanup**. This is existing credit,
not a newly accepted owner. Both SetCurrent source sites remain unchanged;
the native 57-byte body preserves all 53 non-field bytes and the same sole
game-speed field identity. That before/after proof is not independent target
acceptance of SetCurrent or the parent.

## Separate regression and host-test repair

The original OnDraw canonical recipe separately cold-replays **31 bytes / all
1 field exact**. Its image is distinct from the OnUpdate diagnostic image.

The timer type change exposed a stale copied host fixture declaration. Preserve
its initial compile failure. The scoped repair extracts and audits exactly the
canonical five-field timer storage; it deliberately models **no constructor or
Tick behavior** and proves no native pointer layout. The test's other logic,
50 mutation definitions, strict intended-failure classifier and sanitizer
handling are unchanged. Reordered/extra-field audit controls reject.

The staged repair passes an O2 70-case baseline and O2 ASan/UBSan **70 cases plus
all 50 compiled mutations with exact intended diagnostics**. The applied root
baseline also passes 70 cases. These focused receipts remain separate from
the subsequent complete checkpoint CI below.

## Identities, retained failures and limits

- Current GameManager.cpp:
  `4847d2f30f244f195b2240b0f9738fa66a6cfa24379b285da6e4ad616b59f4f0`
- Current OnUpdate image:
  `410c3a6ab7a735e5c416176fdb77d87f589dd40b1bba3aebf558c3bb492ad1d8`

[Summary JSON](summary.json) records all baseline/candidate identities, graph,
passed receipts and separate validation scopes. Ignored packets under
`.analysis/gpt-dots/` are `game-menu-delete-current-20261025`,
`game-menu-delete-candidate-20261025`, `game-real-tick-candidate-20261025`,
`game-teardown-timer-host-20261025` and `game-manager-checkpoint-20261025`.
The real-Tick independent receipt SHA-256 is
`f28b5c698ff1cbf088a3987330fc4a17b1075f809b53784733c02eb473b0ceb8`.

Preparation path-serialization failure occurred before a freeze/build; its
record remains. Initial independent-review identity/alignment failures also
remain beside the successful same-image review. The latter uses role-checked
before/after instruction alignment while checking all 55 branches, not target
local-address normalization. No native retry or failure overwrite occurred.
Original artifacts are preserved; no disassembly or target payload is copied
here, and ignored packets are not supplied by a Git clone.

## Completed checkpoint CI and parked leads

**All 112 target-independent CI stages pass**, verified 2026-10-25
02:16:22 UTC. In game-manager-checkpoint-20261025, root-ci.exit and
post-ci-inputs.exit are both 0; all **1489 frozen source, documentation and
protected-file inputs** were preserved. The durable root-ci.log SHA-256 is
`ef2e4c1857cb428f040d6b21c269b435c79b1c6d2caa7152c2bc9b42d266d871`;
[Summary JSON](summary.json) records the pin manifest and remaining receipt
hashes. The earlier MCP timeout was transport only, not native or CI failure.
This documentation-only result update follows the successful freeze; it does
not change source, tests, metrics or whole-owner acceptance. The prior 470dca2
pass remains historical rather than being relabeled for this checkpoint.

GUI substitution remains rejected: the actual target call is 0x417040, not
HUD UpdateStageElements at 0x414900. The completed read-only flag/control
audit cannot prove a bitfield declaration from combined versus separate flag
tests; park that lead. Existing ScriptViewerWorkerView::Stop at 0x44C150
(101 bytes) is a next audit lead only, not a completed model or new credit.
Parent exactness, original type spelling and runtime closure remain unproved.
Keep unrelated core/HUD/WinMain/Ending panels parked.
