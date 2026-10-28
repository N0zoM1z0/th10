# Ending ID storage and complete text construction, 2026-10-28

This checkpoint admits **648 authored exact bytes / one complete owner**,
`EndingTextVmView::EndingTextVmView @ 0x40BA90`. Two independently cold-compiled
18-TU images reproduce all648 bytes and all12 semantic fields, including a
strictly bound worker-member EH handler. The complete1568-byte Run interpreter
also receives substantive source/dataflow work but remains nonexact at1564;
no fragment of it is credited.

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting HEAD `7c1debd` is clean. Disk verification, pinned VC7.1 execution,
tracking and direct-local read-only Ghidra six-sample identity checks pass.
No Factory operation, target patch, push or full prior-cohort replay is used.
The previous goal turn produced a committed source/evidence checkpoint.

## Fresh review and first complete comparison

The first fresh18-TU checksum-entry baseline compares the complete interpreter
before source changes. It retains **1576/1568 bytes**, all18 owned table words
and a fully decoded code/alignment interval; the old parked size is reproduced.
Text construction is **772/648**, bank loading131/126. Existing exact Ending
contracts pass. The observer initially routes two guards to the wrong PDB source;
that error is preserved, and a separate same-image correction passes all ten
contracts943 bytes/all57 fields without recompiling or relabeling the original
observer result.

Fresh target decode provides two useful discriminators:

- Both pressed-key tests read DWORD at input+6. The current WORD declaration
  already compiles to those DWORD tests. No input type/width change is warranted.
- The constructor executes three timer flag initializations and worker setup,
  followed by the whole0xEC-byte clear. It has **no21 ID zero-initialization
  stores** before that clear. The old five text IDs and sixteen descriptive
  animation slots were typed as the zero-constructing AnmVmIdView, generating
  those extra stores. Passive integer word storage removes that unobserved
  construction while keeping all offsets and the allocation size unchanged.

Ending.hpp is included only by Ending.cpp in this native graph. All17 support
source/header closures are verified to exclude it, so intermediate models can
recompile the one changed TU and reuse hash-checked support objects. Final
acceptance uses two independent full18-TU source compilations.

## Coherent source models and outcomes

| Complete model | Text constructor /648 | Run /1568 | Existing ten guards |
| --- | ---: | ---: | --- |
| Fresh maintained baseline | 772 | 1576 | 943/all57 after observer correction |
| Passive five/sixteen ID word intervals | 660 | 1576 | Pass |
| Handwritten lookup with early returns | 660 | 1576 | Pass; not retained |
| Explicit returned-ID lifetime/publication | 660 | 1564 | Pass |
| Actual shared FindVm dependency | 648 | 1564 | Pass |
| Target-observed unsigned constructor loop | 648, ordinary bytes equal | 1564 | Pass |

These are complete contribution sizes, not ordinary-byte mismatch scores.
The passive slots are converted to the existing handle value/pointer views at
existing ID APIs; shared AnmVmIdView constructors and APIs are unchanged. The
compiler-facing distinction is local to Ending. Original C++ field spelling,
physical types, capacity and lifetime remain unknown; sixteen is still a
consumed interval before the worker, not proof of original array capacity.

The constructor now names its returned ID temporary before copying the word,
matching the target's stack-temporary consumption. Run uses an ordinary inline
publication helper receiving the returned handle by const reference; creation
completes before the helper reads the live cursor/destination and copies its
word. No hidden-output parameter, ABI annotation, forced inlining, register
hint, volatile, padding or copied target bytes are introduced. The prior helper
control over active handle-array members is historical; this new model is
motivated by the independently observed passive storage and actual return-word
consumption. Native audits separately confirm all four primary-list creators
and post-call destination reads, plus the two initializer sampling contracts.

Replacing the constructor's duplicated lookup loops with the actual FindVm
lets the existing LTCG context reproduce the three inlined target lookups.
The constructor's last ordinary difference is its signed JL loop branch at
+0x1DD; target uses JB. An ordinary unsigned index reproduces that branch and
the complete body. Source-facing C++ construction naturally lowers to the
observed stack this+command /RET8 interface; original declaration is unknown.

Current other complete source-only owners are text destructor253/192
(baseline275/192), controller Initialize584/584, controller destructor361/362,
and bank loader131/126. None gets exact credit. The whole Run remains1564/1568;
its table and local destinations remain owned and untrimmed.

## Strict worker-member EH address binding

The new opt-in `worker_member_eh_handler = true` supports only the named Ending
text constructor and a separate DIR32 handler field at+3. It cannot combine
with allocation, stack-file or static selectors. The ordinary lanes are intact.
The parent must still pass its full648-byte extent and every linked field.

The handler is the parent's unique unaliased MAP static symbol, inside one
complete **24-byte generated PDB contribution in the same parent object/module**:
14-byte cleanup action followed by10-byte dispatch. In both images the action
loads saved this from EBP+4, adds member offset0xD0 and tail-jumps to the real
worker destructor. The dispatcher selects immutable **28-byte one-state
FuncInfo** and its eight-byte unwind map, then jumps to CxxFrameHandler. Wrong
state, storage/member offset, dispatch, mutable data/code and unrelated handlers
fail closed. An adjacent eighth metadata word is not consumed.

The cleanup requires the real qualified16-byte worker destructor and101-byte
Stop contributions from ScriptViewerWorker.cpp. Diagnostic anchors cannot
replace the destructor, Stop or vtable. The complete destructor is compared
against target with exactly its two known fields (vtable4703E4 and Stop44C150),
including authoritative field coverage and every ordinary byte. The same-image
canonical destructor/Stop guards independently pass117 bytes/all5 fields.
This supplies **handler address identity only**: generated24-byte code, metadata,
vtable storage and runtime unwinding receive no new ownership/exact credit.

Constructor fields also bind the real allocator, InitializeVm, AddVmVariant0,
ASCII/ANM globals, game speed and worker vtable. Three FS offsets bind the known
absolute-zero `__except_list` symbol, supported by the prior pinned exsup.obj
review. The diagnostic graph itself still uses unresolved-symbol data anchors
and is not a runnable product.

Four synthetic metadata positives and39 rejection controls protect this narrow
lane. The existing allocation37 and stack-file45 rejection controls also pass.
Synthetic fixtures use unrelated addresses and contain no copied TH10 fixture.

## Corrected complete Run inventory

The 2026-10-25 report's target90/native92 count was wrong. Its immediate-address
heuristic attempted to read one file byte at each address, so it dropped the
two real sound-owner immediates at target offsets **+0x204/+0x284**, both
0x492590. The symbol lies in the PE section's virtual BSS beyond its raw bytes.
This was an inventory omission, not two extra native references.

`audit-ending-run-native.py --inventory` now recognizes section addresses from
declared virtual intervals, without manufacturing BSS contents or changing the
raw-byte reader. Correct target/native counts are **92/92**. Each complete
inventory decodes all code/alignment, verifies18 table words and every local
destination against instruction starts, and cross-checks the native field set
against authoritative linked fields. File-backed/BSS positives and section-end,
hole and raw-padding controls guard recognition. This remains **syntactic
inventory**, not semantic binding or full Run equality. Historical receipts
stay immutable; their false count is explicitly superseded here.

## Reproduction and validation

Canonical `ending-text-constructor` retains the actual checksum-entry graph
from `ending-load-script`: Ending.cpp plus seventeen ordered actual supports,
all `/GL /GS /EHs`. Both final cold images preserve identical full source/header
pins. Constructor648/all12 and twelve existing Ending/file/creator/release/worker
contracts **1060/all62** pass per image. Combined proof is1708 bytes/all74 fields,
with only648 bytes newly credited. Earlier guard recipes are not migrated.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit ending-text-constructor --json
scripts/repo-python scripts/compare-linked-function.py --unit ending-text-constructor --json
scripts/repo-python scripts/audit-ending-run-native.py \
  --directory build/match-linked/EndingLoadScript --include-initialize --inventory
scripts/repo-python scripts/test-ending-state.py --sanitize
scripts/repo-python scripts/test-linked-worker-member-eh.py
scripts/repo-python scripts/test-ending-address-inventory.py
```

Actual-source ASan/UBSan/leak host checks and all seven deliberate mutants pass.
The stale-destination mutant supplies the pre-creation cursor to a faulty copy
of the publication helper; production reads the live cursor inside its body.
Host clear preserves the worker vptr; allocation/thread/SDK/asset roles and
omitted native-layout assertions retain their documented limitations. These are
source contracts, not original/i386 ABI or worker/runtime validation.

[Results](results.json) record selected CI and receipt hashes. Source coverage
and authored denominator are unchanged. New totals are **78,471/275,434 authored
exact bytes,435 owners**. Whole Windows product, runtime, semantic and portability
gates remain open. Keep compiler jobs serial, one CPU, nice10 and bounded.
All16 selected CI stages pass, including all three EH selectors, complete
address inventory, existing field/local/table rules, semantic bindings, ledger,
graph, progress, documentation and whitespace checks. The two new stages bring
default collection to123; no full123-stage execution is claimed. Superseded
intermediate build directories are removed after retaining their source and
structured receipts; baseline and both final independent images remain.
Working producers/logs/snapshots are below
`.analysis/gpt-6.1-sol/ending-run-20261028/`; the first final image is under
`build/gpt-6.1-sol/ending-run-20261028/cold-a/`, the second producer under
`build/match-linked/EndingTextConstructor/`. Its byte-identical artifacts are
adopted at the existing `build/match-linked/EndingLoadScript/` canonical path
so the two identical build recipes share one output; previous loader artifacts
are preserved privately. No source/compiler change or build rerun follows this
output-group correction.
