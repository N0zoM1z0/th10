# Complete archive load

The canonical `pbgarchive-load` contract covers the complete 108-byte owner at
`0x00434C30..0x00434C9B` and all seven linked fields. It uses the original
Japanese TH10 v1.00a target, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained `PbgArchive::Load` source is unchanged. The existing four-TU
path graph already emits its complete target shape, including the private
ECX archive/EAX filename boundary. `pdb_source = "src/PbgArchive.cpp"` binds the
parent to the real archive support contribution. FileSystem remains the primary
input with the real Read entry; PbgArchive and PbgFile are LTCG supports and
ResFile is the reviewed normal-COFF support. The recipe retains `/GL /GS /EHsc`
and the archive support's `/GL /Ob1`. No forced ABI, dummy call, source, header,
layout or compiler-profile change is used to obtain equality.

Target observations independently establish these operations: release prior
state; allocate a 12-byte CPbgFile backend; return false for a null backend;
parse the header; copy/publish the filename; reopen through the backend using
the existing mode-0 pointer; and release on parse/copy failure. The reopen
result is ignored. The two releases remain distinct calls. Source names and
the physical call view do not establish original class, TU or ownership.

Fresh local read-only/noanalysis Ghidra reviews Load and the release, parser,
filename-copy and constructor contexts. Startup's recorded call at `0x0042011F`
sets ECX to `0x00497990` and EAX to the `th10.dat` string. The separately decoded
call at `0x004357DF` lies in LoadArchive, which has no Ghidra function record;
the missing Ghidra xref does not erase that target call. All 412,289 mapped text
bytes are verified unchanged before and after rollback, maxcpu1/heap768 MiB.

The complete field inventory is five REL32 fields (two releases, allocation,
parse and filename copy) and two DIR32 fields (vtable and mode array). Both
parent bytes and every field must pass. Callee code, vtables, data ownership
and native runtime receive no separate credit from this parent comparison.

Two pinned Read images first supplied cached diagnostics, not a new cold in
that earlier checkpoint. Acceptance for this packet uses two subsequent
independent canonical builds, each compiling all four actual source inputs.
Each checks the new Load108/all7 and seven existing whole guards812/all41:
Read214, GetSize190, FindArchive212, memory Open46, entry size51, FindEntry53
and CPbgFile destructor46. Total checked per image is 920 bytes/all48 fields;
only one function/108 bytes is new credit. No broad old-unit replay is needed
because maintained source and headers did not change.

An initial proposed separate output directory was rejected by build-graph
validation before compilation: identical recipes must share one output. The
unit therefore shares `build/match-linked/FileSystemPathSize`. The preceding
Read second cold was archived and hash-checked before overwriting that output;
its two historical carriers remain retained. This changes no infrastructure.

Reproduce the focused native acceptance twice with:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit pbgarchive-load --json
```

[facts.json](facts.json) records the contract, inputs and actual artifact
hashes; [validation-summary.json](validation-summary.json) records final checks.
All 111 CI groups pass. Subsequent restoration of unrelated optional CSV
quoting changes no parsed row or source/test/tool input; the summary records
both raw snapshots and the row-equivalence check.
Private compact producers/results are under
`.analysis/gpt-6.1-sol/archive-load-exact-20261022/`. Keep the first actual cold
at its matching build path and the second in the shared canonical directory.

Whole Windows i386 product/runtime closure, original production flags and TU,
data ownership, semantic reconstruction and portability remain open.
