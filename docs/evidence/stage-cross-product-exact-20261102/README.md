# TH10 StageCrossProduct: complete 77-byte target closure (2026-11-02)

The reviewed, isolated original Japanese TH10 1.00a target extent
0x00421B90..0x00421BDC owns 77 bytes. Its original machine code
is raw-identical to the full PDB contribution of the natural
StageCrossProduct helper from src/StageDraw.cpp. Its entire 77-byte
instruction stream is decoded, ends in RET, and has **zero** external
DIR32/REL32 address fields. There is no masked constant or partial
matching prefix.

The maintained source computes the alias-safe cross product of
three-component camera vectors, then copies the complete temporary
vector to the output. The real canonical StageCreate /GL /GS source graph
contains StageScript.cpp, StageLifecycle.cpp, StageDraw.cpp, Chain.cpp,
AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp, RandomMath.cpp,
FileSystem.cpp and MainViewport.cpp. PDB evidence identifies
StageDraw.cpp as the full 77-byte owner.

Two independent selected cold builds confirm the complete target and
protect a substantial existing caller in exactly the same native image:

| Selected owner | Target bytes exact | External fields |
| --- | ---: | ---: |
| StageCrossProduct at 0x00421B90 | 77 | 0 |
| StageCameraStateView::ConfigureCamera | 330 | 10 |
| Total per cold | **407** | **10** |

PE hashes and PDB GUIDs differ between the two independent builds.
The exact source/target hashes, PDB identity and cold receipts appear in
native-summary.json.

The target extent has an independently reviewed function boundary,
but no independently established entry reference or original C++
source attribution. The source function name describes a reproducible
codegen match, not a recovered original identifier, physical TU or
runtime reachability claim. Consequently this adds **77 canonical
codegen-exact bytes and one matched target owner**, with **zero new
authored-origin credit** and no claim of a whole runnable native product.
No source instruction was patched and no full cold cohort was run.

Reproduce the selected scope:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit stage-cross-product-00421b90 \
      --unit stage-configure-camera --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit stage-cross-product-00421b90 --json
    scripts/repo-python scripts/ci.py
    git diff --check
