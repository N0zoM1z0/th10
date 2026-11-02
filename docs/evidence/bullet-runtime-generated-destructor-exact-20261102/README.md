# TH10 BulletRuntimeView destructor: complete 34-byte codegen closure

The original Japanese TH10 1.00a range 0x00405DE0..0x00405E01
is the complete reviewed 34-byte function-like destructor.
It uses a private ECX receiver, tests an optional pointer at
object+0x360, calls the original _free @0x00452422 if present,
clears the member, and returns. The complete target instruction
stream and call displacement were independently decoded.

The actual ScriptViewerCreate fourteen-source VC7.1 /GL /GS /EHs graph
generates a PDB-owned symbol ??1BulletRuntimeView@@QAE@XZ
whose complete 34-byte body is exact after strict binding of its one
REL32 _free target field. There are **zero ordinary differences**;
no fake ABI, padding, target instruction changes or cropped matches.

Two independent *selected cold builds* preserve the full owner and
two preexisting ScriptViewer exact functions:

| Owner | Exact bytes |
| --- | ---: |
| BulletRuntimeView destructor | 34 |
| ScriptViewerCreate | 67 |
| ScriptViewerReleaseEnemies | 19 |

Both builds therefore protect 120/120 common selected bytes; one build
also selects BulletResetPool45 (165/165). The independent native PE
hashes and PDB GUIDs differ. The strict field record, target and source
hashes and both selected receipts are in native-summary.json.

The original target boundary is reviewed, but original source-written
versus compiler-generated destructor ancestry is **indeterminate**.
This adds one canonical exact codegen owner and **34 bytes**, not
authored-origin credit. There is no full-product/runtime claim, and
no full historical cold replay was run.

Reproduce the focus only:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit bullet-runtime-generated-destructor-00405de0 \
      --unit script-viewer-create --unit script-viewer-release-enemies --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit bullet-runtime-generated-destructor-00405de0 --json
    scripts/repo-python scripts/ci.py
    git diff --check
