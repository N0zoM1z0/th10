# Script browser Update and native lifecycle closure

Follow-up: [the complete Draw checkpoint](../script-viewer-draw/README.md)
closes Draw 664 and its ten-byte callback, including the previously omitted
owned switch table. Nonexact sizes below describe this lifecycle source context.

This checkpoint adds **1,419 authored exact bytes** and 140 exact bytes whose
origin remains indeterminate. Two independent frozen-source VC7.1 SP1
CL13.10.6030 / LINK7.10.6030 cold links pass all 11 new complete owners,
all 114 new fields and 17 existing complete-owner guards. No partial region
of a larger nonexact owner is credited.

| Target | Maintained function | Bytes | Fields | Origin |
| --- | --- | ---: | ---: | --- |
| 0x40A3C0 | ScriptViewerCreate | 67 | 5 | authored |
| 0x40A350 | ScriptViewerView::InitializeManagers | 101 | 10 | authored |
| 0x40A340 | ScriptViewerLoadThread | 16 | 2 | indeterminate |
| 0x40A450 | ScriptViewerView::Update | 1251 | 83 | authored |
| 0x44C130 | ScriptViewerWorkerView destructor | 16 | 2 | indeterminate |
| 0x4148E0 | ScriptViewerReleaseGui | 18 | 2 | indeterminate |
| 0x425090 | ScriptViewerReleasePlayer | 18 | 2 | indeterminate |
| 0x406140 | ScriptViewerReleaseBullets | 18 | 2 | indeterminate |
| 0x405730 | ScriptViewerReleaseBomb | 18 | 2 | indeterminate |
| 0x41AFB0 | ScriptViewerReleaseItems | 18 | 2 | indeterminate |
| 0x42B6D0 | ScriptViewerReleasePopups | 18 | 2 | indeterminate |

See [the native proof and fixed profiles](native-summary.json),
[the target facts](target-facts.json), and
[the field bindings](field-bindings.json). Names and views are descriptive;
original class declarations, complete class sizes and production TU ownership
remain unknown. The six newly reviewed release boundaries add 109 indeterminate
bytes and no authored denominator bytes. Natural source is also present for
the browser, Bullet, Bomb and Popup destructors; their larger bodies remain
nonexact.

Reporting correction: the loader's 16 bytes have always retained indeterminate
origin in the ledgers. Earlier prose counted them as authored; the generated
progress totals were correct. This correction changes no source, exact proof,
origin classification or overall exact-byte total.

## Coupled compiler context

The real ScriptViewerCreate entry and source/support order are fixed in the
canonical manifest. Primary and supporting inputs use `/GL /GS /EHs`, except
Enemy.cpp retains its earlier canonical `/GL` profile. Every input stays in
the LTCG lane. This is a reproducible bounded context, not proof of original
production flags or partitioning.

Uniform `/GL /GS /EHs` emits a target-sized Update with one unlike ordinary
byte at +0x273: LEA ECX for the spawn request, versus target LEA EAX. The earlier
complete EnemySpawn 577 recipe preserves that EAX input. Applying its explicit
per-TU profile reproduces both **Update 1251 and EnemySpawn 577 in the same
image**, including the latter's reviewed alignment, jump table and selector
tail. Its existing exact credit is preserved rather than added again.

The tool driver now records explicit `support_profiles`, requires each override
to select an existing LTCG support, rejects `/GL` removal and normal-COFF
conflicts, and includes the full mapping in build/replay grouping. Defaults and
the separate normal-support lane retain their behavior.

Two Update calls enter the existing 28-byte retirement primitive 0x409E20. The
actual static SuspendScriptViewerOwner has no public MAP entry. Its two fields
use explicit `static_source`/`static_size` selectors. The restricted comparator
requires a direct REL32 CALL to the entry, one qualified MAP static and unique
nonoverlapping same-start PDB contribution in the producer output directory,
a complete reviewed target extent, dense zero-field decoding, and raw equality
of the entire callee. It never falls back from an unresolved public name to a
static. All 28 helper bytes are checked, with no duplicate ledger credit for
the already exact retirement primitive.

```sh
scripts/repo-python scripts/build-match-unit.py --unit script-viewer-update
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-update --json
scripts/repo-python scripts/compare-linked-function.py --unit enemy-spawn --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-worker-destructor --json
```

## Source and target observations

PlayerCreate is kept out of line at the observed initialization call 0x40A351.
This removes the 52-byte inlined allocation/constructor/registration/failure
expansion and restores initializer 101 and loader 16. The annotation preserves
an observed boundary; it does not identify an original source keyword.

Worker table 0x4703E4 contains one entry to the standard flag-controlled scalar
cleanup at 0x44C100. The +0x10 browser unwind funclet calls the 16-byte worker
destructor. Modeling a real virtual destructor makes the compiler emit the
vtable reset and scalar cleanup; the old manual vtable store differed in its
base register. Constructor 234 remains exact with the actual compiler-owned
vtable binding. This establishes the bounded virtual model, not an original
worker class name, inheritance tree or allocation size.

The Player unwind funclet 0x465280 adds 0x14 and calls the existing ANM VM
destructor 0x401FF0. PlayerDrawVmView now has its own destructor and the parent
no longer manually frees that buffer at its tail. Normal and exceptional
member cleanup follows C++ lifetime rules. The buffer offset remains 0x358
within the VM. Player destruction itself remains nonexact.

The owner-delete helper saves a pointer by value before calling its destructor
and scalar deallocation. This preserves the captured allocation when a
callee clears its global slot. A reference-template hypothesis from the MIT
reference exposed the global pointer addresses and produced extra reloads:
Update 1296 versus 1251. The value parameter restores 1251. The observations are
consistent with address-exposure handling in the old optimizer; they do not
prove the original template or macro spelling. If destruction does not return,
the explicit protocol does not reach deallocation.

The observed unregister interface is ECX task / EDX chain / RET. Its maintained
fastcall spelling preserves all 114 bytes; RemoveAll 82 also passes. It does not
resolve the remaining destructor argument-move scheduling differences. Explicit
stdcall annotations on C++ destructors were rejected as a useful representation:
three isolated declaration/definition/default controls all emit QAE symbols.
Those ineffective annotations are removed.

Item access now uses the existing g_ItemManager and its typed array rather than
a second opaque pointer alias. The clear covers exactly 0x21CEA0 bytes from +0x14:
**0x896 (2198) records** of 0x3F0 bytes. Earlier decimal-896 wording is corrected.
Bullet destruction marks the loaded ANM before clearing its global and then
destroys 2001 records in reverse. Popup's +0x18 VM is corroborated by its unwind
funclet; its owned buffer is at +0x370. Browser deletion order is GUI, Player,
Bullets, Enemies, Bomb, Items, Popups, then global clear, VM, worker.

## Verification and remaining work

The existing 11 browser/dependency units, unregister 114, remove-all 82,
EnemySpawn 577, cursor 128, input 33 and mode-switch 37 pass both cold images.
Host fixtures check snapshot-before-lock, callback replacement during unlink,
byte-depth wrapping, destruction/deallocation order, all nullable callbacks,
reverse 2001-record cleanup, global-clear-before-member-cleanup, buffer release
on normal/exceptional exit, and skipped deallocation when destruction throws.
Existing HUD/message fixtures explicitly stub the new draw-VM cleanup
outside their observation scopes. GUI construction/lifecycle fixtures similarly
stub the new Bullet destructor dependency. The GameManager teardown fixture
keeps its existing opaque Player boundary. The transition-effect dependency pins
are refreshed only for the reviewed Chain fastcall declaration/definition;
843 host assertions and all 19 strict pin controls pass. Static-owner controls
and new adversarial profile/static-call controls pass. Standard repository CI passes.

Browser destructor 431/412, Player 341/325, GUI 579/574, Popup 240/232,
Bullet 236/236 and Bomb 137/137 remain nonexact. The last two retain four ordinary
argument-move ordering differences. Item destruction 232/216, Enemy 299/300,
PlayerCreate 67/69 and Rebuild 2408/2520 also differ; Draw 647 still lacks source.
Uniform `/EHa` regresses constructor and creator and is not adopted. Changing object
order or the real entry did not solve the spawn-request edge. These diagnostics
have no acceptance authority and no partial credit.

Synthetic tests do not establish Windows runtime, original exception behavior,
production storage ownership or whole-product closure. The anchor images are
never executed. Semantic reconstruction, portability and Factory acceptance
remain unopened.
