# Exact RGB evaluator and handoff checkpoint

RGB Evaluate at **0x00441600/820 bytes** now reproduces the complete target
in two independent canonical cold links, including all **17 address fields**.
This adds820 authored exact bytes. The existing56-byte Int3 scale operator
migrates to its maintained support TU without adding duplicate credit.

See [target bindings](target-bindings.json), [native proof](native-summary.json),
[nine discovery controls](controls.json) and [validation](validation-summary.json).
This supersedes the RGB outlining frontier and current-cache statements in
the earlier integer/vector/Float2 checkpoint packs; their pinned historical
results remain valid for their own inputs.

## Source and compiler context

The source returns an ordinary Int3 value. Constructor-return tuple arithmetic,
initial-value snapshots in add/acceleration modes, shared t+t, named Hermite
weights and named result values recover the target value graph. The existing
X/Z/Y constructor slots preserve the separately defined operator's observed
Y/Z/X conversion order. Hermite retains four real scale calls, each truncating
components before integer summation. Linear interpolation uses a normal
in-class Scale method and truncates the product before adding the initial
integer component:255 to0 halfway gives128, whereas scalar alpha gives127.

AnmManager.cpp, RandomMath.cpp, AnmVmCreate.cpp, AnmVmId.cpp and the new
AnmInt3Scale.cpp form the complete candidate link with the real ExecuteScript
entry. All retain /GL /GS; only the scale support uses /Ob1. Global /Ob1 and
same-TU/by-value-only controls had not recovered this complete call graph.
Natural named results close the final two byte differences. There are no new
forced-inline/noinline attributes, fabricated ABI, padding or target bytes.
This reproduces a bounded source/profile hypothesis; original production
class names, return spelling, physical TU and flags remain unknown.

The complete target is independently decoded:8 absolute fields and9 relative
calls. Literal bits, speed, curve, conversion helper and four scale calls are
bound to TH10-local observations. Each canonical comparison covers the full
PDB-owned extent and rejects missing/extra fields. Local Ghidra is read-only,
noanalysis, one CPU/768MiB, with temporary metadata rolled back and full mapped
text verified before/after (401000..465A80 inclusive,412289 bytes,
SHA256 cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f).

## Reproduction and regression scope

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-evaluate-rgb-interpolation --unit anm-int3-scale --json
scripts/repo-python scripts/test-anm-rgb-interpolation.py
```

Both colds freeze12 recursive raw project inputs. Distinct PDB identities and
artifact hashes are recorded. Canonical RGB820/17 and scale56/3 pass in each
image. Twelve unchanged complete guard owners pass4579 bytes/all183 fields
under explicit same-image context/output overrides. Together each cold passes
14 complete owners5455 bytes/all203 fields. Existing guard canonical recipes
are unchanged; scale's recipe is explicitly migrated. No full historical
cohort is replayed and no parent exact credit is granted.

The host test extracts the actual owner/vector declarations, RGB body and new
scale definition. It passes13 scenarios and9 strict mutants: each mutant must
compile and fail its named assertion with exit17. Positive ASan/UBSan with
leak detection also passes. Timer and linear curve are bounded roles; these
checks establish neither target ABI/layout/x87 state nor original/native
product runtime. Transition-host RNG/timer/viewport extractions are unchanged;
reviewed whole-file pins advance without weakening normalization or rejection.

## Remaining work and cleanup

ExecuteScript remains nonexact: current9948/target9964, frameFC,89 target-sized
groups, absolute gap18, pre-table delta−16, aligned diagnostic7400/7995.
Unequal intervals61(+1),60(−7),2(−10) include common updates/alignment. Current
RGB call windows follow target R/G/B loads/stores. These observations add no
partial parent credit. Earlier9964/gap14 diagnostics used a different graph.

Authored exact is67505/273274 bytes (24.70%),370 functions. Whole Windows i386
product closure remains open; semantic reconstruction and portability have not
started. The operator ended the95% campaign and requested handoff. Old goal
instructions and superseded current-state logs are removed from the resume
brief. Source experiments are retained as verified deltas; redundant native
controls are pruned after recording their hashes. Protected pre-existing D3D
work remains independent and untouched.

All97 standard CI checks pass across89 completed frozen-input checks and
8 resumed documentation/tracking/build checks. The summary link was missing
at the first documentation check; after writing it, the unchanged public
commands90..97 pass. Raw project inputs and current carrier hashes remain
unchanged. Cleanup removes977135845 bytes:590 inactive historical host ELFs,
20 raw snapshots replaced by verified deltas, and10 superseded native directories.
All historical host sources/reports and the current native carrier are retained.
