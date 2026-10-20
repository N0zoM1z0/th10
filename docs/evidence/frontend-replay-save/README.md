# Front-end replay save and replay deletion (2026-10-20)

The previously unmapped screen16 owners now have maintained source:
`FrontEndControllerView::UpdateReplaySave @ 0x433570` and
`DrawReplaySave @ 0x433B30`, together **2,415 authored bytes**. The old Result
screen name and cdecl update declaration are corrected; both target handlers
consume one stack controller and end in RET4. These source mappings do not
establish original names, translation units, native product or runtime behavior.

## Target boundaries and source

The original Japanese1.00a target is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local read-only Ghidra attests that file and the complete mapped .text hash
`c1611c9becb832a4e2ba456afc58218fb5b8fafd6c85d19e3addbebb0c946e0a`
before/after review; temporary function metadata is rolled back. Only the
update/draw dispatcher entry calls enter the respective reviewed ranges.
Decompiler calling conventions are provisional; raw pushes, receiver reads
and RET4 establish these bounded stack contracts.

The update owner is **1,460 bytes**, through0x433B23. Its unsigned0..4 guard
at433586/433589 selects the directly indexed table at433B10. All five entries
land at local instruction starts:433599,43362F,43364D,4337C9,433A33. Code ends
atRET4 through433B0F, and433B24..433B2F is12 CC bytes before the draw entry.
The earlier1,440-byte row omitted this20-byte compiler-owned table; the authored
denominator gains20, without exact credit. Draw remains955 through433EEA,
including seven internal alignment bytes; five CC bytes precede433EF0.
The focused public audit checks dense decoding, local branches, selector/table,
all direct calls, dispatcher screen16 table edges, ledger extents and neighbors.
The unrelated global401100 metadata-only boundary discrepancy remains unchanged.

Initialization opens25 numbered slots and falls through to the opening test on
the same frame. Opening waits until current>6; closing starts atcurrent>=6.
Slot cancel precedes confirm. Name entry uses13 columns, three control cells,
eight-byte space-padded names and the same cursor object as score entry.
Empty-name backspace exits before the common sound or later cancel handling.
Other name confirms run before a simultaneous cancel. The native time call
keeps the original replay owner but reloads its header afterwards. Save destroys
the existing slot first and ignores the write result. It samples the destination
slot after saving and before loading, then copies the live name to the profile.
Closing marks VM105/94 and interrupts90/91, retires the ID atcontroller+5D0,
returns to the menu, pops the cursor and deletes the captured active replay.
It restarts title music, destroys only25 numbered objects, then clears all50
pointer slots. Upper user-slot objects are not destroyed by this handler.

Drawing retains each replay header across localtime, but samples the current
ASCII manager for formatting and subsequent color/cleanup operations. The name
preview slides from the selected list row towardy240; its summary uses completion
entry8. Its timer is read again after formatting before showing name/caret/grid.
The first preview summary inherits color. List rows use15-pixel spacing; the
keyboard uses18x16 spacing, signed ordinary bytes and control glyphs81/7F/80.
Only the two active draw phases reset final color and shadow; other phases return
without those stores. These are target observations and source contracts, not
claims about actual callbacks mutating storage during game execution.

The MIT hypothesis source is `levzzz5154/th10-decomp`, pinned at
`c6791d82dc9f02e55eca45a5314f900a85481efd`; its license is retained at
[th10-decomp-MIT.txt](../../licenses/th10-decomp-MIT.txt). Its linker placement,
forced attributes, exactness claims and compiler-symbol-number tuning are not
adopted. TH10-local bytes, references and native comparisons remain authoritative.

## Native exact claim and limits

The natural, unchanged `ReplayManager::Destroy @ 0x4297B0` now closes its complete
**18 authored bytes** in the real front-end update callback graph. Earlier
standalone contexts emitted27 bytes; the recovered caller permits the target's
private ESI receiver and plain RET. Complete ownership is unchanged. Its two
REL32 fields independently bind destructor4294A0 and scalar operator delete4524A1.
No destructor syntax, ABI annotation, padding, flag sweep or target-byte source
is introduced to obtain this result.

Two independent full18-TU /GL /GS /EHs cold links freeze the same53 maintained
source/header inputs. Each strict PE/MAP/PDB comparison passes422 complete bytes
and all4 fields: new Destroy18/all2, existing cursorPush66/Pop47/Move128 and
screen setters83/80/all2. Five old units are guards only; their canonical recipes
are not migrated and their credit is not duplicated. The canonical new recipe
records the ordered real support graph and the ReplayManager PDB source owner.
[The compact cold proof metadata](native-proof.json) records both runs.

The related update callback also strictly passes7/all1 in the current graph.
The alternate draw callback is5/7 there; a separate fresh cold of its original
one-TU canonical recipe strictly passes7/all1. That recipe and its existing
credit remain unchanged. These callback checks grant no additional credit.

Current native disassembly also retains ESI across time and reloads its+18
header afterwards. After SaveReplay it reads the slot fromEBP+24 before Load,
then stores through the retained EDI index atEBP+EDI*4+59E4. These bounded native
sampling observations agree with the target and explicit source ordering; they
do not establish the remaining updater bytes or runtime behavior.

The save updater remains1364/1460 and draw961/955 in this graph. The updater
retains six repeated-input helper calls where the target expands the tests;
source-level inline or compiler-owner hypotheses remain open. Diagnostic extent
or similarity values confer no parent or partial exact credit. Other dispatcher,
replay loader, destructor and save bodies remain nonexact. Unresolved anchors
make this image non-runnable; it is not a faithful Windows product.

```sh
scripts/repo-python scripts/report-frontend-replay-save.py
scripts/repo-python scripts/build-match-unit.py --unit replay-manager-destroy
scripts/repo-python scripts/compare-linked-function.py --unit replay-manager-destroy --json
scripts/repo-python scripts/test-frontend-replay-save.py --sanitize
```

## Host validation

The suite extracts both maintained owners, the actual cursor setter/Push/Pop/
Move, repeated-input method/wrapper and replay-delete wrapper. Canonical headers
are consumed with native-layout assertions omitted, timer prefill defined and
only the native timestamp casts adapted to an explicit int32 host time role.
VM actions, state transitions, replay allocation/destruction/I/O, time/localtime,
ASCII formatting and sound/music remain explicit callback-sensitive roles.
No original/native runtime, register ABI, physical layout, x87 or product claim
follows from those seams.

The positive suite checks all phases, input priorities, 25-slot loading/failure,
name editing, time owner/header sampling, post-save/pre-load slot sampling,
ignored save/reload failures, close ordering and25-versus50 lifetime behavior,
format arguments, preview interpolation, post-format timer reload, manager
replacement, caret/grid/control positions, colors and conditional cleanup.
ASan/UBSan/leak detection pass, as do eight strict mutants with exact named
functional failures. Compilation errors, signals, sanitizer diagnostics or
unrelated assertions never count as successful mutation rejection. CI runs the
positive suite. Default host artifacts clean themselves up.

Only affected source/contracts and these complete native guards are replayed.
Full historical native cohort, product/runtime, dual-Oracle semantic and port
phases remain open. Pre-existing D3D evidence stays untouched. Repeated native
objects and temporary host builds are removed after the compact evidence and
canonical four-file carrier are retained.
