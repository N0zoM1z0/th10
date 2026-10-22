# Complete resource archive loader

The `pbgarchive-load-from-resource` contract covers the complete100-byte owner
at `0x00434CA0..0x00434D03` in original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained source is byte-identical to checkpoint73fbf4c. The previous
normal-COFF ResFile support context emits161 bytes with a base-constructor call
and exception handling; the separate Ob2/Gz context emits280. Compiling the
same ResFile support with `/GL` makes the existing memory-backend constructor
visible to the optimizer and reproduces all100 target bytes. All four actual
inputs retain `/GL /GS /EHsc`, with archive support `/Ob1`. This is a reproduced
context, not evidence of original translation-unit ownership or production flags.

Fresh local Ghidra inspection uses readOnly/noanalysis/rollback, maxcpu1 and a
768 MiB heap. All412289 mapped text bytes remain unchanged. Ghidra records the
existing100-byte body and its resource-backend vtable reference, and reports no
incoming reference to this retained body. Original callers remain unknown.
The decompiler's signature remains provisional: the machine body consumes
live-in ECX archive and EAX resource name, and returns without stack arguments.

Independent complete target decoding covers36 instructions and five local
branches. The exhaustive linked-field contract contains five REL32 calls and
one DIR32 vtable address: Release twice, operator new, ParseHeader, CopyFileName
and the resource-backend vtable. The original PE has no base relocations;
target fields come from decoded operands and Ghidra callee/vtable observations.
Candidate fields are independently checked against its PE relocations and
complete instruction inventory. Every branch, return and initialization byte
remains part of the comparison.

The initial private control compiles only ResFile and reuses three pinned source
objects. It is followed by two independent actual full four-TU canonical colds.
Each reproduces new100/all6 plus nine complete existing guards956/all52, totaling
1056/all58: FindArchive212, memory Open46, entry size51, FindEntry53, CPbgFile
destructor46, path Read214, path GetSize190, archive Load108 and registration36.
All1249 prior canonical recipes remain unchanged. Only100 new authored bytes
are credited; callees, vtable storage and constructor ownership gain no credit.

Reproduce the accepted context twice:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit pbgarchive-load-from-resource --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit pbgarchive-load-from-resource --json
```

[facts.json](facts.json) records the actual cold pair, complete field inventory
and source/profile scope. [validation-summary.json](validation-summary.json)
records checks and cleanup. Compact private producers and results live below
`.analysis/gpt-6.1-sol/archive-resource-load-20261022/`. Keep the first actual cold
at its matching `accepted-cold-1` build path and the second at
`build/match-linked/PbgArchiveResourceLoad`.

All111 final CI groups pass with exit0 and all373 final source/test/config
inputs unchanged. Focused comparison, toolchain/tracking, Archive source gate,
generated progress, documentation links and whitespace pass. Hash-checked cleanup
removes11 superseded files/187410 logical bytes while preserving both actual
colds, old-context artifacts and nine independent D3D files.

Archive now has15/15 reviewed authored functions source-present and exact.
Different accepted compiler contexts do not establish one production build.
Whole Windows i386 compile/link/runtime closure, physical data owners, original
identifiers and production partition remain open. Semantic reconstruction and
portability have not started.
