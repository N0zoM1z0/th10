# Script browser: missing functions and native reconstruction

Local Ghidra 12.1.3 and the hash-attested Japanese TH10 v1.00a target establish
13 previously untracked functions, totalling 2,371 bytes. Eight functions add
2,293 reviewed authored bytes; five short callback/delete adapters remain
origin-indeterminate. Every new boundary has a complete dense target decode,
matching temporary Ghidra body, instruction-aligned internal branches, no
tracked overlap, and a RET followed by CC padding or the next reviewed entry.
The individual extents, hashes and direct control transfers are recorded in
[`target-boundaries.json`](target-boundaries.json).

The initial architecture export had no functions in `0x0040A006..0x0040A93F`.
The local review creates missing instruction/function metadata temporarily,
decompiles it, rolls the transaction back, and runs headless with `-readOnly`.
Mapped bytes before and after agree. No executable or persistent database
bytes are patched. The global `report-boundary-inventory.py --check-ledger`
already fails on differing evidence text at `0x00401100`; this batch preserves
the older rows and independently reviews the added entries.

## Complete exact owners

| Target | Natural source | Bytes | Linked fields |
| --- | --- | ---: | ---: |
| `0x00409EB0` | `ScriptViewerView::ClearScriptNames` | 86 | 2 |
| `0x0040A040` | `ScriptViewerView::ScriptViewerView` | 234 | 2 |
| `0x0040A130` | `ScriptViewerView::RegisterTasks` | 106 | 12 |
| `0x0040ABE0` | `ScriptViewerUpdateCallback` | 7 | 1 |

Two independent cold VC7.1 SP1 13.10.6030 / LINK 7.10.6030 `/GL /GS` links
replay all 433 bytes and all 17 fields using the existing canonical comparator.
The three authored owners contribute **426 authored exact bytes**. The
seven-byte callback's origin remains indeterminate. Full PDB contributions
establish extents; no larger owner is cropped. Source/artifact hashes and
comparison results are bound in [`native-summary.json`](native-summary.json).

All four units share `src/ScriptViewer.cpp` as primary and the real
`ScriptViewerCreate` source entry, with AnmManager, Chain, FrontEnd, Player and
Gui support. Unresolved external symbols use the existing non-runnable data
anchor harness. Reproduce the shared image once, then compare each owner:

```sh
scripts/repo-python scripts/build-match-unit.py --unit script-viewer-constructor
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-constructor --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-register-tasks --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-clear-names --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-update-callback --json
```

## Target facts and rejected shortcuts

The constructor runs the worker, three cursor and ANM VM subconstructors before
clearing the entire `0x688`-byte object, setting the first word's bit 1 and
publishing `0x4776F8`. The final word at `+0x684` is a separate flag field.
Worker constructor stores at `+4/+8/+12/+16` and the target's Start/Stop
consumers distinguish the thread handle, thread ID, stop request and running
flag. Worker Start also accesses `+0x18`; the view reserves the whole region
before browser state `+0x30`, without inventing initialization of unused words.

Registration sets each task's **`flags |= 2`**, stores the browser at task
`+0x20`, and registers priorities 5 and 39. The reference's `flags |= 1`
candidate is inconsistent with the target. The first local chained stage-global
assignment also reversed `0x474C7C/0x474C80`: it was structurally identical
after masking addresses, but failed strict symbol-to-target binding. Explicit
ordered assignments resolve that difference. This negative control is retained
in the summary; similarity alone grants no exact credit.

Fresh temporary Ghidra disassembly confirms calls to `MarkGameChains` at
`0x0040A655` and `0x0040A685`, in addition to the previously recorded
`0x00417C53`. It also confirms the direct call at `0x0040A749` to the existing
Player release candidate `0x00425090`, correcting the earlier no-caller claim.
The Player release candidate's authored-versus-compiler origin remains unknown.

## Large owner and open work

`ScriptViewerView::Update` models the complete **1,251-byte** target owner:
state 0 enumerates `../../data/*.ecl` twice and initializes the menus; state 1
selects scripts, launches manager loading, spawns an enemy from an entry, or
requests mode 3; state 4 polls input and resumes the managers on bit 8. It
preserves the unconditional FindClose, unchecked allocation paths, explicit
spawn-request clearing and repeated manager loads observed in the target.

The complete linked candidate is 1,280 bytes and remains nonexact. The
manager initializer is 153/101 bytes; loader callback 16/16 differs; creator
68/67 differs; player suspension 39/36 differs because the current maintained
RebuildPlayerOptions interface has caller cleanup where this target edge has
callee cleanup. No ABI declaration is falsified to hide these differences.
The destructor, scalar-delete candidate and conditional-delete candidate have
reviewed boundaries but no maintained implementation in this batch. Draw's
647-byte owner remains a separate open source recovery. Other calls still
need complete implementation and linkage ownership; new storage aliases are
target-address obligations, not proven production data owners.

These results do not establish an exact parent, complete native product,
runtime validation, semantics gate, portability or Factory acceptance.

`scripts/test-script-viewer-update.py` compiles the actual update, clear-names,
callback and task-flag helper bodies against synthetic host sinks. Fixtures
check manager event order and receiver identity, input-edge distinctions,
spawn-name/packet selection, full item clearing, two-pass directory enumeration,
empty-search FindClose, null manager and null callback paths. This verifies
bounded source behavior; host fixtures do not establish Windows runtime or ABI.
The same diagnostic image also passes the existing 128-byte cursor Move and
33-byte input-mask helper through strict complete-owner comparisons.

The source shape was studied from the issue's reference,
[`levzzz5154/th10-decomp` at c6791d82](https://github.com/levzzz5154/th10-decomp/tree/c6791d82dc9f02e55eca45a5314f900a85481efd),
under its [MIT notice](../../licenses/th10-decomp-MIT.txt). Local TH10 evidence
determines adopted behavior, layouts, fields and exactness. Reference assembly
fallbacks, synthetic contexts and equivalent-region allowances are not used.
