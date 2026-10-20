# Title entry and interactive main menu

The maintained source now covers `FrontEndUpdateMainMenu @ 0x0042D300`,
`FrontEndUpdateMainMenuReturn @ 0x0042D420`, and the six-marker progress query
`FrontEndHasAnyClear @ 0x0042C850`. Names describe recovered roles; they do not
identify original symbols, translation units or the complete profile owner.

## Target observations

The canonical Japanese 1.00a target is identified by SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local headless Ghidra ran read-only, without analysis, with a 768 MiB heap and
one active processor. Executable identity, entry/mapping attestation and the
complete mapped `.text` hash were checked. Temporary metadata was rolled back
and mapped bytes stayed unchanged. Its provisional body ranges omit the tables
below; raw target instructions establish their guarded domains and ownership.

| Owner | Executable bytes | Other owned bytes | Complete extent |
| --- | ---: | --- | --- |
| Title entry `42D300` | 268 | Five-entry state table, 20 bytes | `42D300..42D41F`, 288 bytes |
| Interactive menu `42D420` | 1,195 | One NOP and three tables of 20/32/32 bytes | `42D420..42D91F`, 1,280 bytes |
| Progress query `42C850` | 69 | None; eleven following CC bytes excluded | `42C850..42C894`, 69 bytes |

The first two owners directly abut the next independent function. Every table
destination is a decoded instruction in its owner. Unsigned selectors bound
the state tables to 0..4 and the item tables to 0..7. Focused Ghidra references
show dispatcher calls at `42D065` and `42D0BD`, without an external interior
reference in either reviewed extent. The ledger correction adds **105 authored
owned bytes** to the provisional denominator, with no exact credit by itself.
The target has no PE base-relocation directory; table ownership comes from its
guards, operands, destinations and adjacent owners.

The progress query tests exactly six bytes at profile `+1D888..+1D88D` and
returns whether any is nonzero. The menu uses it to disable item 1 (Extra).
That role supports the descriptive name, without recovering a complete score
file type. `titleAnm` at controller `+18` is the resource passed to the primary
VM creator; the returned ID is stored at `+684`, now `titleAuxVmId`. Both offsets
are asserted in the maintained native view.

When backdrop slot 88 is missing, title entry creates slots 88 and 90 and
auxiliary script 0 from `titleAnm`. It resets phase 1 and cursor 0, then evaluates
the phase-1 threshold. Phase 2 consumes **pressed** mask `160B`, sends slot 90 interrupt 6,
sets phase 4, queues sound 32, then retires the auxiliary ID. At frame 30 it
switches to interactive screen 2.

The interactive menu initializes eight items, consumes practice flag `10`, and
retains a pre-existing backdrop while always recreating cursor slot 0. At frame
11 it enters phase 2, executes cursor interrupt 3 and samples its current
selection for interrupt `selection+11h`. Repeated up/down input moves the cursor;
cancel selects item 7, while cancel already on item 7 exits directly through
the quit arm, before confirm processing. Music-room selection queues sound 10
twice, including once after the phase reset. At frame 20 the eight items route
to screens `6,6,6,12,11,14,4,3`; Extra saves the old difficulty before setting
4, and ordinary/practice starts clamp a difficulty of at least 4 to 1. The
actual dispatcher sets transition flag 2 before calling the interactive menu;
that previously absent write is restored in the maintained caller.

## Native and host validation

The first dependency-aware native diagnostic recompiled the changed FrontEnd
and two header-dependent ScriptViewer inputs and reused fifteen hash-verified
objects from the prior frozen 18-TU cache. It emitted title entry **284/288**
and interactive menu **1,308/1,280**; both remain nonexact. The progress query
was raw-equal across all 69 bytes. Cached diagnostics are not cold proofs.
Each of two independent full18-TU /GL /GS /EHs colds freezes53 source/header
inputs. The complete69-byte unique PDB contribution decodes densely and is
raw-equal with zero linked fields. Each also passes1112 existing guard bytes
and all32 fields: replay Load/Destroy/FPS/control/draw callback, ASCII formatter,
sound queue, repeated input, cursor Push/Pop/Move, and both screen setters.
The existing shared ReplayManagerDestroy carrier is refreshed; the new
`frontend-has-any-clear` recipe uses that same output/build group. The anchor
image is non-runnable and establishes no production graph. Full proof/input
hashes are recorded in `native-proof.json`.

`scripts/report-frontend-main-menu.py` fails closed on target/decoder identity,
dense executable ranges, all direct calls, local branches, four exact tables,
their selector guards, dispatcher edges and current physical ledgers.
`scripts/test-frontend-main-menu.py` extracts the actual three owners, cursor
methods, repeated-input method and sound-queue implementation. It uses canonical
headers with explicit host layout/prefill adaptations. VM lookup/creation,
interrupt execution and screen/state transitions are **host roles**. Finite
checks cover all eight routes, every progress byte, input precedence, thresholds,
saved cursors/difficulty, callback-sensitive selection reads and two actual queue insertions for the
music-room request. The native sound owner is a fixed global array; the host
uses a fixed typed object for its reviewed queue view. Nine strict mutants must fail with their
named functional assertion; sanitizer errors and unrelated failures do not count.
These checks do not execute TH10, prove native private ABIs/x87 or close product
storage, runtime behavior, semantic reconstruction or portability.

The issue reference remains the MIT-licensed
[`levzzz5154/th10-decomp`](https://github.com/levzzz5154/th10-decomp), pinned at
`c6791d82dc9f02e55eca45a5314f900a85481efd` in ignored `_reference/th10-decomp`.
Its root MIT attribution is retained in
[the license file](../../licenses/th10-decomp-MIT.txt). Its menu recovery and
LTCG notes supplied hypotheses; upstream placement pragmas, forced attributes,
synthetic contexts and upstream exactness metrics were not adopted. Every
boundary and behavior stated here was reviewed against the local TH10 target.
The pre-existing D3D evidence remains untouched.
