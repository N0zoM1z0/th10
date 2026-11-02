# TH10 ScriptViewer scalar deleting destructor: 27-byte exact closure

The full reviewed original Japanese 1.00a range
0x0040A410..0x0040A42A is 27 bytes. The actual ScriptViewerCreate
14-source VC7.1 /GL /GS /EHs graph emits a complete PDB-owned
scalar deleting destructor ??_GScriptViewerView@@QAEPAXI@Z
with identical ordinary bytes, all instructions decoded, and
two rigorously bound original REL32 fields:

* offset +0x02 calls ScriptViewerView::~ScriptViewerView at 0x0040A1A0;
* offset +0x0F calls scalar operator delete at 0x004524A1.

The target tests the low deleting-flag bit before requesting scalar
deallocation and returns the receiver with RET4. Its private
ESI receiver and the entire branch/return layout match the native
artifact. Identical small deleting-destructor templates for other
classes were *not* accepted as proof: callee identities are strictly
verified against this specific ScriptViewerView target.

Two independent selected cold builds protect four complete owners:

| Unit | Exact bytes |
| --- | ---: |
| Scalar deleting destructor at 0x0040A410 | 27 |
| ScriptViewerCreate | 67 |
| BulletRuntimeView destructor | 34 |
| ScriptViewerReleaseEnemies | 19 |
| Selected total per cold | **147** |

Both source-graph PE hashes and PDB GUIDs differ. The complete
target/source pins, PDB owner identity, exact field roles and two
receipts are stored in native-summary.json.

This was codegen reconstruction and evidence on an existing natural
source; no target bytes, synthetic register ABI or C++ dummy functions
were introduced. The original target origin remains **indeterminate**:
scalar deleting destructors may be source requested or generated as
a compiler contribution. This adds one canonical exact owner and
27 bytes with no authored-origin credit. There is no full product
or runtime proof, and no full historical cold replay.

Reproduce only the selected scope:

    scripts/repo-python scripts/replay-exact-units.py \
      --unit script-viewer-scalar-delete-0040a410 \
      --unit script-viewer-create \
      --unit bullet-runtime-generated-destructor-00405de0 \
      --unit script-viewer-release-enemies --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit script-viewer-scalar-delete-0040a410 --json
    scripts/repo-python scripts/ci.py
    git diff --check
