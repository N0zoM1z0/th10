# Script browser: real manager and input dependencies

The browser now calls maintained Bullet pool reset, Enemy clear/suspension,
the shared FrontEnd input helper, and MainSupervisor mode switching. These
replace unresolved aliases and a redundant input adapter. The input view
unifies the existing first-eight-byte FrontEnd prefix with the later fields
observed by the browser's repeat updater. It establishes no allocation size,
record stride, array count, keyboard mapping integration or production owner.
The separately archived `probes/input-native` experiment is unchanged.

## Complete exact functions

| Target | Source | Bytes | Linked fields | Origin |
| --- | --- | ---: | ---: | --- |
| `0x405ED0` | `BulletManagerView::ResetPool` | 45 | 2 | authored |
| `0x409F90` | `EnemyManagerClear` | 118 | 2 | authored |
| `0x40D510` | `EnemyManagerView::SuspendCallbacks` | 28 | 0 | indeterminate |
| `0x40AC20` | `FrontEndInputView::UpdateDebugRepeat` | 109 | 0 | authored |

All 300 bytes and four independently bound fields pass two cold VC7.1 SP1
13.10.6030 / LINK 7.10.6030 `/GL /GS` builds. This adds **272 authored exact
bytes** and 28 origin-indeterminate exact bytes. Complete unique PE/MAP/PDB
contributions establish every extent; the zero-field functions are raw equal.
See [the native bindings](native-summary.json) and
[the target facts](target-facts.json).

The 28-byte suspension entry was missing from both the ledger and persistent
Ghidra function inventory. A fresh architecture export precedes review.
Temporary Ghidra disassembly identifies the full `0x40D510..0x40D52B` body
and caller `0x40A667`; its region is hash-compared with the canonical target
before analysis and unchanged afterward. Headless runs readOnly, and the
metadata transaction rolls back. Source-written method versus shared/compiler
outlining remains unresolved. The old global boundary-check evidence-text
difference at `0x401100` remains documented; unrelated rows are preserved.

All eight browser units share one cold image and the real ScriptViewerCreate
entry. The four prior browser owners also pass in this expanded graph.
Existing cursor Move128, input-mask33 and mode-switch37 pass same-image
complete-owner guards. No broader historical native cohort is replayed.

```sh
scripts/repo-python scripts/build-match-unit.py --unit bullet-reset-pool
scripts/repo-python scripts/compare-linked-function.py --unit bullet-reset-pool --json
scripts/repo-python scripts/compare-linked-function.py --unit enemy-manager-clear --json
scripts/repo-python scripts/compare-linked-function.py --unit enemy-manager-suspend-callbacks --json
scripts/repo-python scripts/compare-linked-function.py --unit input-debug-repeat --json
```

## Target behavior and falsified source hypotheses

Bullet reset marks the loaded ANM's VMs before clearing all 2,001 records,
points the cursor at the first record, and assigns state5 to the last record.
It leaves the draw buckets, active count and loaded-resource pointer outside
the clear. The full `0xF82BC`-dword target clear agrees with the asserted
record layout; no guessed sentinel padding is introduced.

Enemy clear marks loaded ANMs9..12, then saves each node's next pointer before
virtual deletion. The destructor can unlink that node. It clears manager+0x64
and the first special slot+0x10, then clears callback flag2 independently for
each nonnull task. It does not directly zero activeEnemyCount+0x60. A local
renderer snapshot produced a target-sized 118-byte function with twelve
ordinary differences. Using the actual shared `loadedAnms[9..12]` expression
recovers the target global-load, register-save and loop schedule without a
register annotation.

The debug input updater clears word+4, shifts a 16-bit local through sixteen
counters, increments held-key counters modulo65536, and on reaching26
subtracts8 and ORs **bit zero** into+0x30. It never clears+0x30 and never
shifts that accumulated flag according to key index. The two edge stores are
at+0x32/+0x34. These unusual details are target observations, not inferred
input fixes. The meaning of the browser's separate+8 mask stays neutral.

A 16-bit explicit changed-bits local emitted five unlike bytes in the edge
tail. Reversing one AND's operands was byte-neutral; widening the temporary
regressed ordinary agreement. Writing the two edge expressions directly lets
the pinned compiler share their XOR and reproduces all109 bytes. Six private
trial bodies were diagnostic only; the unchanged real-source baseline served
as entry and a duplicate-body control agreed. Only the final maintained
function's complete owned extent receives credit.

## Large owner and validation limits

The browser Update remains **1,267 / 1,251 bytes**, nonexact. The earlier
1,280-byte candidate's 141/919 ordinary agreement improves through real
dependencies, and pointer-based cursor remembrance yields539/923 in the
final graph. These scores are structural diagnostics with no acceptance
authority. Removal of the input adapter fixes callee identity; it does not
by itself change the owned byte count. No matching region of Update is
credited separately.

Initializer153/101, creator68/67, loader16/16 and player suspension39/36
still differ. Enemy manager destruction remains298/300. The original
RebuildPlayerOptions cleanup ABI, worker launch, manager release helpers,
browser destructor/Draw and production storage/link/runtime closure stay open.

The dated [worker follow-up](../script-viewer-worker/README.md) closes Start/Stop
and player suspension, superseding those open-ABI/worker observations. It keeps
the larger browser and Player owners nonexact.

Host fixtures compile the actual maintained reset, repeat and browser bodies.
They check destruction-mutated lists, null callbacks, exact clear range and
untouched prefix, null resource forwarding, counter wrap/threshold behavior,
preserved repeat flags, input edge stores, receiver/event order and cursor
snapshot-before-move. The standard repository CI also passes. These are
source fixtures, not Windows runtime validation, an original/native dual
Oracle, semantic gate, port proof or Factory acceptance.
