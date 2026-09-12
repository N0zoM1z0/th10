# TH10 reconstruction handoff

Status: **active-incomplete exact reconstruction campaign**. This checkpoint
reviews and source-maps the central LZSS encoder/tree owner cohort connected to
the already reviewed decoder. It closes the target-local tree storage extent and
private helper machine boundaries, but it establishes **no canonical function
exactness**, original translation-unit ownership, whole Windows i386 product
closure, runtime validation, semantic reconstruction, or portability readiness.

## Recovery baseline

Exact selection for this session:

- repository: `th10`
- analysis provider: `th10-ghidra`
- target identity: `target:th10-main`
- branch: `main`
- starting HEAD: `cfe26df8d99bbaeb82e93275141608482a2026c2`
- starting upstream: `origin/main` at
  `2f4a3350fc61132e7eb5e340f657527965ae5500`
- starting relation: ahead 5 / behind 0
- starting worktree: 0 staged / 0 unstaged / 0 untracked / 0 conflicts
- starting `.analysis/`: **99,128 bytes**
- starting `build/`: **5,031,767 bytes** of generated/open state

The mandatory recovery review inspected recent history, branch/HEAD/upstream,
porcelain-v2, complete staged and unstaged diffs, all untracked paths, relevant
ignored state, `.analysis/`, `build/`, and this handoff. No interrupted tracked
or untracked work was recoverable.

Preserved classifications:

- `resources/th10.exe`: operator-supplied ignored private target; preserved and
  excluded from Git.
- `.tools/`: shared immutable selectors plus game-bound tool/Wine state;
  preserved.
- `ghidra-project/`: target-bound provider state; preserved.
- earlier `.analysis/` campaigns/bootstrap: checkpointed, reproducible, shared,
  or legacy state; preserved unless current-session ownership was explicit.
- `build/`: reproducible/open generated state; preserved.

No unknown/unrelated work was reset, overwritten, staged, or deleted. The
private target was never modified, replaced, moved, or committed. `/mnt` was not
searched and `TH10_TARGET_PATH` was not set.

Before editing, all prompt-named repository and Factory guidance was reread in
full through the live repository runner: `AGENTS.md`, this handoff,
`docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, `docs/TOOLS.md`, both required Factory
contracts, Factory ontology/verification/autonomy/bootstrap/recovery guidance,
`docs/semantic-reconstruction.md`, and the repository-local
`.agents/skills/th10-reconstruction/SKILL.md`. No skill injection was assumed
and no required path was missing.

## Mandatory preflight

Before target-dependent work these passed:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Target remained original Japanese TH10 v1.00a:

- size 487,936 bytes
- SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, four sections, image base `0x00400000`, entry `0x004537DC`
- Rich checksum `0x3152A748`, dominant build 6030

Pinned VC7.1 SP1 execution again passed normal C/C++ COFF, C++ LTCG, resource
compilation, and PE32 linking under the headless Wine wrapper. This proves the
candidate tool surfaces only; it does not identify a per-function profile or
physical object owner.

Native Ghidra schemas were rediscovered with
`factory_list_analysis_operations(th10-ghidra)`. Mandatory `check {}` passed
with `attestation.status=passed`, exact `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. Every useful Ghidra
result carried the same passed target/native attestation and
`exactness_credit=none`.

Entry ledger state was:

- candidates: **1,229**
- origin/boundary pending: **1,156**
- reviewed authored: **67 functions / 11,687 bytes**
- exclusions: **6**
- source-present mappings: **38**
- canonical exact: **0 functions / 0 bytes**
- canonical normal-COFF match units: **0**

## Hard packet selection

The selected hard packet followed the reviewed decoder and shared dictionary
into the connected LZSS encoder/tree owner:

- `0x004359B0-0x00435DB4` — 1,029 bytes, encoder
- already-reviewed retained `0x00435FA0-0x00435FC3` — InitTree-shaped
- already-reviewed retained `0x00435FD0-0x00435FF9` — InitEncoderState-shaped
- `0x00436000-0x00436203` — 516 bytes, match/tree insertion
- `0x00436210-0x0043625F` — 80 bytes, deletion
- `0x00436260-0x004362AE` — 79 bytes, contraction
- `0x004362B0-0x00436324` — 117 bytes, replacement
- `0x00436330-0x00436360` — 49 physical bytes, successor search

This was deliberately chosen as a central >1.8 KiB compression/data-owner
cohort rather than cherry-picking the already easy structural match at
`0x00435FD0`.

Observable goals were to recover complete physical extents and padding, call
relationships, target machine ABIs, the shared tree storage owner, natural
source shape, compiler feedback, and source presence without inventing original
TU/profile/exactness.

## Target-local owner and call graph

The target call graph is coherent:

- encoder `0x004359B0` has two retail callers: `0x00429B60` and `0x0042B1E0`;
- encoder directly calls the `0x00436000` match helper and `0x00436210`
  deletion helper plus `malloc`;
- `0x00436000` is called only from the encoder;
- `0x00436210` is called from the encoder and recursively from itself;
- deletion dispatches to `0x00436260`, `0x004362B0`, and `0x00436330`;
- those latter three have no other reviewed caller.

The two retained init helpers have no modeled entry xref. Their exact behavior
is independently present inline inside the encoder, so they remain source-shaped
retained authored bodies rather than padding or compiler helpers.

## Physical boundaries

Target bytes establish these complete extents:

- `CompressData`: `0x004359B0-0x00435DB4`; final `ret 0x0C` at
  `0x00435DB2-0x00435DB4`; `0x00435DB5-0x00435DBF` is `CC` padding.
- `Lzss::InitTree`: `0x00435FA0-0x00435FC3`; `0x00435FC4-0x00435FCF` is
  `CC` padding.
- `Lzss::InitEncoderState`: `0x00435FD0-0x00435FF9`;
  `0x00435FFA-0x00435FFF` is `CC` padding.
- `Lzss::AddString`: `0x00436000-0x00436203`; final `ret 4` at
  `0x00436201-0x00436203`; `0x00436204-0x0043620F` is `CC` padding.
- `Lzss::DeleteString`: `0x00436210-0x0043625F`; its two-child path ends in a
  tail jump to `0x004362B0`.
- `Lzss::ContractNode`: `0x00436260-0x004362AE`; `0x004362AF` is `CC`.
- `Lzss::ReplaceNode`: `0x004362B0-0x00436324`;
  `0x00436325-0x0043632F` is `CC` padding.
- `Lzss::FindNextNode`: physical `0x00436330-0x00436360` (49 bytes), followed
  by `CC` at `0x00436361-0x0043636F`.

Ghidra reports only 43 body addresses for `0x00436330`; direct target bytes show
that `0x0043634A-0x0043634F` is a six-byte unreachable alignment
`lea ebx,[ebx+0]` inside the physical function extent. The ledger's 49-byte
extent is therefore retained; the 43-byte Ghidra CFG count is not a corrected
boundary.

## Machine ABI observations

Machine boundaries are recorded independently from natural source declarations:

- `CompressData`: three stack arguments / `ret 0x0C` — input, input size,
  output-size pointer; returns allocated output in EAX. Both retail callers use
  this exact shape.
- `InitTree`: live-in EAX root / plain `ret`.
- `InitEncoderState`: no explicit input / plain `ret`.
- `AddString`: live-in EDX `newNode` plus one stack `matchPosition` pointer /
  `ret 4`.
- `DeleteString`: ECX node / plain `ret`.
- `ContractNode`: live-in ESI old node + EDX new node / plain `ret`.
- `ReplaceNode`: live-in EAX old node + ECX new node / plain `ret`.
- `FindNextNode`: live-in EAX node / plain `ret`, return in EAX.

These private register inputs are optimizer/interprocedural evidence. Maintained
source deliberately uses ordinary C++ method/function signatures and does not
encode these machine conventions as source-level `__fastcall` claims.

## Encoder behavior

`0x004359B0` is a source-shaped LZSS encoder paired with the already reviewed
decoder:

1. allocate `inputSize * 2` bytes with `malloc` and return null on failure;
2. initialize output bit mask to `0x80`, output bits to zero, and output size to
   zero;
3. clear the complete dictionary and tree state inline;
4. set dictionary head to 1 and preload up to 18 input bytes;
5. initialize the tree sentinel/root state inline;
6. while input remains, clamp match length to available lookahead;
7. emit a literal token when match length is below 3, otherwise emit a 13-bit
   offset and 4-bit `length - 3` token;
8. delete outgoing tree nodes, load/retire input bytes, wrap dictionary positions
   with mask `0x1FFF`, and find the next longest tree match;
9. emit the zero-offset terminator;
10. store `outputCursor - output` through the output-size pointer and return the
    allocated buffer.

The two target callers consume the returned compressed buffer and immediately
feed it to the related transform at `0x0044B220`, which is the next hard packet.

## Tree helper behavior

`Lzss::AddString` searches the tree rooted at sentinel `tree[0x2000].right`,
compares up to 18 wrapped dictionary bytes, records the longest match and test
node, replaces a full-length match, or inserts the new node into the selected
left/right slot.

`DeleteString` returns for an unlinked node; contracts a one-child node; for two
children it finds the replacement node, recursively removes it, then tail-
dispatches the replacement operation.

`ContractNode` transfers the old parent to the replacement, redirects the
parent's old left/right link, and clears the old parent field.

`ReplaceNode` redirects the parent, copies all three tree-node fields, rewrites
both child parent links, and clears the old parent field.

`FindNextNode` starts from `node.left` and follows right-child links until zero,
returning the rightmost descendant.

These behaviors are established from TH10 target reads/writes/control flow;
adjacent games were used only later for maintained naming/source shape.

## LZSS tree and dictionary storage

The tree is physically bounded to exactly:

`[0x00477858, 0x0048F864)` = 0x1800C bytes

That is exactly 0x2001 `0x0C`-byte nodes with fields parent/left/right.
`tree[0x2000]` is the sentinel/root node and its right field is at
`0x0048F860`, which the target encoder writes and AddString reads.

Boundary evidence:

- `0x00477850` is a separately reviewed archive counter;
- `0x00477854` has no observed reference;
- dense tree references begin at `0x00477858`;
- tree arithmetic ends exactly at `0x0048F864`;
- `0x0048F864-0x0048F867` is an observed four-byte unreferenced gap;
- the separately reviewed 0x2000-byte shared dictionary begins at
  `0x0048F868`.

Maintained source therefore defines `g_LzssTree[0x2001]` with a 0x0C node size
assertion, while `g_DecompressionRing[0x2000]` remains the one shared dictionary
owner introduced in the previous packet. Original source/data TU ownership is
still unknown.

## TH10 source-path evidence

Attested TH10 string search found:

- `.\\src\\pack\\LzssUtil.cpp:201 BYTE`
- `.\\src\\pack\\LzssUtil.cpp:56 BYTE`

Neither string start has a modeled code xref. These are target-local proof that
`src/pack/LzssUtil.cpp` participated in the original build, but they do **not**
bind any reviewed function or data object to that original physical TU.
Maintained names `CompressData` and `Lzss::*` are therefore source-shape/naming
choices, not recovered original-symbol or TU claims.

## Maintained source

Added:

- `src/Lzss.hpp`
- `src/Lzss.cpp`

The source defines natural maintainable:

- `LzssTreeNode` (0x0C bytes)
- `g_LzssTree[0x2001]`
- `CompressData`
- `Lzss::InitTree`
- `Lzss::InitEncoderState`
- `Lzss::AddString`
- `Lzss::DeleteString`
- `Lzss::ContractNode`
- `Lzss::ReplaceNode`
- `Lzss::FindNextNode`

All eight reviewed authored bodies are now source-present together. The two
small retained init helpers were deliberately not source-mapped separately in
the previous packet; this packet maps them only after closing the wider
encoder/tree owner.

No original source ownership was added to `config/build.toml`.

## Compiler feedback

The same natural `src/Lzss.cpp` compiled successfully with pinned VC7.1 SP1
build 6030 as:

```text
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL
```

A first compile attempt stopped before code generation because the natural
source initially scoped `bitfieldMask` inside the token loop while the terminator
macro also uses it after the loop. Repository status was re-read; no unrelated
state changed. The declaration was moved to the enclosing function scope with no
algorithm or compiler-flag change, after which both normal and `/GL` compiles
passed.

Fixed-normal target-bound diagnostics:

| Maintained body | target/object bytes | result | comparable match |
| --- | ---: | --- | ---: |
| `CompressData` | 1029 / 1029 | mismatch | 23 / 913 |
| `Lzss::InitTree` | 36 / 36 | mismatch | 0 / 23 |
| `Lzss::InitEncoderState` | 42 / 42 | structural-exact | 30 / 30 |
| `Lzss::AddString` | 516 / 516 | mismatch | 251 / 440 |
| `Lzss::DeleteString` | 80 / 80 | mismatch | 2 / 52 |
| `Lzss::ContractNode` | 79 / 79 | mismatch | 1 / 51 |
| `Lzss::ReplaceNode` | 117 / 117 | mismatch | 12 / 81 |
| `Lzss::FindNextNode` | 49 / 49 | mismatch | 14 / 37 |

Every natural normal-COFF body has exactly the same physical byte extent as its
target body. Standalone normal code already inlines `InitEncoderState`,
`InitTree`, and substantial tree work inside `CompressData`, but retains ordinary
source-ABI helper calls where the target uses stronger private-register lowering
or further inlining. The same tracked source compiles with `/GL`. This is strong
source-shape/profile-context evidence but it does not establish which target
bodies belong to normal COFF versus LTCG.

No source-calling-convention tricks, padding, target bytes, fake returns, or
iterative byte fishing were introduced. No canonical match unit or exact ledger
row was created. All compiler diagnostics have `acceptance_authority=none`.

Compact retained report:

`.analysis/gpt-web/20260912-lzss-encoder/compiler-shape-report.txt`

## Adjacent-game discipline

Only committed adjacent content was consulted, after TH10 target behavior and
data ownership were established.

Observed live adjacent states during this packet:

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean.
- TH09: HEAD `e4c7a78cdefd4565a7f2cd7c4fd609f2e27f83af`, branch `main`, ahead 5,
  clean at consultation.
- TH095: HEAD `9674fb70fe18d6fddd51a979f1d741a40a57fe0b`, branch `main`, ahead 3,
  with unrelated untracked `EnemyManagerUpdate.i`,
  `config/runtime-scenarios.json`, `droid.resume.txt`, and
  `scripts/runtime-diff.py`; none of that uncommitted content was consulted.

Committed TH09 `LzssEncode.cpp`, `LzssTree.cpp`, `LzssInit.cpp` and TH095 LZSS
source corroborate the recovered algorithm/naming. Addresses, physical/data
ownership, private ABI, profile, exactness, and completion claims were not
transferred. TH10 evidence wins on every discrepancy.

## Ledger and verification planes after this packet

Generated progress now reports:

- tracked candidates: **1,229**
- origin/boundary pending: **1,150**
- reviewed authored: **73 functions / 13,557 bytes**
- exclusions: **6**
- source-present authored mappings: **46**
- canonical exact: **0 functions / 0 bytes**
- canonical normal-COFF match units: **0**

Relative to session entry:

- candidates: +0
- reviewed authored: +6 functions / +1,870 bytes
- exclusions: +0
- source mappings: +8
- canonical exact: +0

The two init helpers were already reviewed authored at entry, so mapping all
eight functions adds eight source mappings while only six functions move out of
origin/boundary pending.

Verification planes remain independent:

- **source presence:** 46 maintained authored mappings; eight new mappings here.
- **exactness:** still 0 canonical exact functions/bytes.
- **whole Windows i386 build:** open; `config/build.toml` remains unchanged.
- **runtime validation:** not started.
- **semantic/port phases:** not started.
- **Factory acceptance:** the pre-checkpoint accepted-snapshot query was
  unavailable due a Factory transport failure. No current accepted/rejected
  packet fact is inferred.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` remain
unchanged.

## Artifact lifecycle

Session entry `.analysis/`: **99,128 bytes**.

Current campaign:

`.analysis/gpt-web/20260912-lzss-encoder/`

Peak observed `.analysis/`: **162,664 bytes**. No file approached 64 MiB and the
campaign was far below 256 MiB.

The successful normal/LTCG objects and detailed comparator JSON were explicitly
current-session reproducible artifacts. After their results were compacted, they
were removed. The campaign retains only `compiler-shape-report.txt` and
`manifest.json`. Current `.analysis/` after scratch cleanup and manifest update
is approximately **103 KiB**; final checkpoint-bound size is remeasured at
closeout. No prior campaign, target, provider database, toolchain, Wine prefix,
or unknown/shared artifact was removed.

## Engineering/recovery events

Several Factory transport failures occurred during read-only recovery, target
query pagination, the first compile invocation, artifact cleanup, and Truth
snapshot access. After each failure the live repository status was re-read
before retry or continuation. No unexpected tracked/untracked state appeared.
One failed compile transport had generated no object files; this was verified
before rerunning. One artifact-cleanup transport failure likewise had made no
artifact changes; the campaign inventory was checked before cleanup was retried.

These are workflow events, not target evidence.

During pre-checkpoint audit the upstream relation changed without any local HEAD
movement: `origin/main` advanced externally from
`2f4a3350fc61132e7eb5e340f657527965ae5500` to the session starting HEAD
`cfe26df8d99bbaeb82e93275141608482a2026c2`. The remote-tracking reflog records
`2026-09-12T22:48:10+08:00 update by push`. This conversation did not run or
request a push and does not attribute that external publication to itself. After
the update, before this packet commit, `main` was ahead 0 / behind 0; the packet
worktree itself remained exactly the reviewed eight tracked edits plus two new
LZSS source files.

## Validation performed so far

Entry/preflight passed:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Target analysis used attested `function`, `decompile`, `callers`, `callees`,
`xrefs_to`, `search_strings`, and bounded raw target disassembly. Compiler
feedback used fixed-profile `scripts/compile-probe.sh` for normal and `/GL`, plus
probe-mode `scripts/compare-coff-function.py`. Ghidra/compile diagnostics do not
carry exactness authority.

After the final source/ledger/handoff state, `src/Lzss.cpp` was compiled again
from a command-local temporary directory as both fixed-profile normal COFF and
the same source plus `/GL`; both passed. The checkpoint surface then passed
`verify-target.py`, `verify-toolchain.py --check`, tracking, generated progress,
the zero-unit normal-COFF match graph, the explicitly open build graph,
reconstruction status, public CI, and `git diff --check`. The honest
`python3 scripts/build.py` returned expected RC=2/open because compile flags, TU
partition, libraries, resources, and link order remain unknown. The three
control-plane files `config/matches.csv`, `config/match-units.toml`, and
`config/build.toml` remained unchanged. A dirty-state post-edit Ghidra `check {}`
is run immediately before staging; the final committed source state remains
authoritative.

## Remaining unknowns/blockers

- Original function identifiers for the maintained `CompressData` / `Lzss::*`
  names.
- Whether any reviewed encoder/tree target body is physically normal-COFF or
  LTCG-owned; `/GL` feasibility is not an ownership Oracle.
- Original source/data TU ownership of `g_LzssTree` and
  `g_DecompressionRing`, despite their exact target physical extents.
- Whether target `src/pack/LzssUtil.cpp` physically owns this entire cohort; the
  retained strings have no code xref.
- Production compile profiles, TU partition, libraries, resources, link order,
  whole-product closure, and runtime behavior.
- Broader compression/replay/score caller ownership remains mostly pending.

## Next evidence-connected hard packet

Prefer the directly connected **FileSystem encrypt-transform sibling**:

`0x0044B220-0x0044B352` — 307 bytes, currently `unknown/review`.

Target-local routing already established:

- `0x0044B213-0x0044B21F` is `CC` padding after reviewed `Decrypt`;
- function starts at `0x0044B220` and ends with `ret 0x14` at
  `0x0044B350-0x0044B352`;
- `0x0044B353-0x0044B35F` is `CC` padding before reviewed OpenFile at
  `0x0044B360`;
- Ghidra body is 307 bytes;
- its two callers are exactly the same two functions that call the newly
  reviewed `CompressData`: `0x00429B60` and `0x0042B1E0`;
- raw call sites load initial XOR seed through AL and push five stack arguments,
  matching the private boundary family of reviewed `FileSystem::Decrypt`;
- decompilation shows the inverse data motion: copy bounded source to scratch,
  consume alternating bytes backward from each scratch chunk, XOR with evolving
  key, and write sequential output bytes.

The next packet should close the exact ABI at both caller families, prove the
inverse/tail behavior against target bytes, compare with committed adjacent
`FileSystem::Encrypt`, and add natural maintained source only if TH10 evidence
supports it. This packet is not an easy leaf: it closes the write-side transform
paired with the central LZSS encoder and reviewed decrypt/decompress read-side
pipeline.

An alternate route if this candidate is blocked is its two common owner callers
`0x00429B60` and `0x0042B1E0`, which now have both compression and transform
edges reviewed.

## Checkpoint closeout

The first local checkpoint for this packet was
`759be29de77aa7dc8bc919750ca6ba4e9464c59d`, subject
`gpt-web: reconstruct TH10 LZSS encoder tree`. Immediately after commit the
tracked/untracked worktree was clean: zero staged, zero unstaged, zero untracked,
zero conflicts. Because `origin/main` had already been externally updated to the
session starting HEAD, the new local packet commit made `main` ahead 1 / behind
0. This conversation did not push.

The ignored campaign manifest was marked `checkpointed`, bound to that commit
with `pushed=false`, and the campaign retained only the 2,537-byte compact
compiler/target report plus its manifest. `.analysis/` measured **103,413
bytes** after the first checkpoint binding. Current-session normal/LTCG objects
and detailed comparator JSON had already been removed as reproducible scratch.

This handoff-only closeout update is amended into the same logical checkpoint.
The final live Git HEAD after that amend is authoritative; the ignored manifest
is rebound to the final hash. Final cold repository checks, post-commit Ghidra
attestation, one Truth Kernel snapshot refresh, final `.analysis/`, and final
branch/upstream/dirty state are read again without changing reconstruction
conclusions.
