# Complete Hint frame update exact — 2026-10-31

`HintOwnerView::Update @419650..41988A` is newly exact across its complete
571 bytes and all17 link-resolved fields. This adds one authored exact owner
and571 authored/codegen bytes. The existing10-byte update callback remains
exact, with its origin still indeterminate.

The [previous checkpoint](../hint-frame-values-20261031/README.md) reproduced
571/571 NONEXACT. Its loop selected the ID-array baseDC instead of the target's
width-array base17C. That changed two LEAs, ID/width displacements, the manager
field position and internal alignment, despite already matching arithmetic.

The retained source removes the named half-width local and writes the same
width-array expression at its three real uses. The pinned compiler chooses
the target width cursor and eliminates repeated arithmetic naturally. This
restores the target's half-width store/reloads, manager field72, loop alignment
and every ordinary byte. No independent pointer, forced register declaration,
padding, shared header, build flag or source graph change is introduced.
The source model does not establish original variable names or expressions.

## Current-source refresh after the HintLoad split

The original results.json is a retained 36-pin witness from before the later
AnmManager.cpp and Hint.cpp changes; only 34/36 of those source/header pins
match today's tree. Its image is historical and must not be reused as a
current-source cache. The canonical selected replay was repeated twice after
the Hint.cpp / HintLoad.cpp partition. Both fresh cold runs strictly reproduce
HintOwnerView::Update 571/571 and its update callback 10/10 (581/581 total),
with distinct PDB GUIDs 1121e78937b9ca49814888c78c86888c and
7a82b94de2986a41833ad56d687e784e. Receipts are retained in
.analysis/gpt-web/current-turn/hint-frame-post-split.json and
hint-frame-post-split-2.json.

## Complete validation

Fresh target decoding and both final images agree on169 instructions,17 fields,
23 fixed local branches, the private EBX owner and RET at23A. No local branch
operand is normalized. All10 literal references bind their actual DWORD float
payloads; all global/call identities bind through the real map. ConsumeEntries
has an actual1308-byte PDB-owned body with the caller-observed private ABI;
it remains NONEXACT against1317, and passes its previous full evidence check
in both images. The two conversion calls retain the ordinary CRT harness
identity. Exactness does not close the product, runtime or portability phases.

Warm and zero-reuse eight-input cold pass the unchanged complete comparator.
Both images also preserve the real callback10/all1 and factory/draw73/all5:
654 bytes/all23 as same-image checks. Independent canonical cold selects only
the new frame571/all17 and its affected callback10/all1, totaling581/all18 in
one existing factory-rooted producer. No other producer or historical cohort
is replayed. Native jobs are serial, CPU0/nice10, bounded and exit-logged.

```sh
scripts/repo-python docs/evidence/hint-frame-exact-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/hint-frame-loop-20261031/repeated-width-expression
scripts/repo-python docs/evidence/hint-frame-exact-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/hint-frame-loop-20261031/final-cold
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit hint-frame-update --unit hint-update-callback --json
```

[results.json](results.json) retains all36 source/header pins, the unchanged
eight-input graph and profiles, whole-owner geometry and normalized hash,
all17 field roles/payloads, actual callee presence and complete cold receipts.
There is one source control, compared as a complete owner before acceptance.
The earlier neutral pointer/parameter models remain parked. Boundaries, source
mappings, origin classifications and the authored denominator do not change.
