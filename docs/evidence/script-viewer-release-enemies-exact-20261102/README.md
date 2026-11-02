# TH10 ScriptViewerReleaseEnemies: complete nullable-delete leaf exact

The original Japanese 1.00a owner at 0x0040D730..0x0040D742 is a
complete, reviewed **19-byte** nullable delete adapter. This checkpoint
admits the complete 19-byte PDB-owned body as **canonical exact** after
strict comparison with all external field identities, but it does
**not** reclassify the original source-authored versus compiler-generated
origin, which remains indeterminate.

The real target checks ESI for NULL, passes that pointer in EAX to
EnemyManagerView::~EnemyManagerView at 0x0040D530, then passes ESI to
scalar operator delete at 0x004524A1. Both calls are REL32 fields at
owner offsets +0x07 and +0x0D. The full branch and ordinary return
are identical to natural source:

    __declspec(noinline) void ScriptViewerReleaseEnemies(
        EnemyManagerView *owner)
    {
        ScriptViewerDeleteOwned(owner);
    }

The compiler emits the 19-byte contribution through the actual
ScriptViewerCreate root with ScriptViewerLifecycle.cpp as its support
source; there is no forged ABI or manufactured register argument.
The target body is fully decoded, the PDB extent is complete, all
ordinary bytes compare with no differences, and both call targets
bind to the reviewed target addresses. Source code was already
present; this commits the missing *proof and conservative credit*,
not a claim that the source was newly reconstructed.

Two independent selected VC7.1 SP1 /GL /GS /EHs cold links (the real
14-source ScriptViewer creation graph) preserve all selected owners:

| Unit | Exact bytes | External fields |
| --- | ---: | ---: |
| ScriptViewerReleaseEnemies | 19 | 2 |
| ScriptViewerCreate | 67 | 5 |
| EnemyManagerClear | 118 | 2 |
| Total per cold | **204** | 9 |

The independent PE hashes and PDB GUIDs differ. See
[native-summary.json](native-summary.json) for original EXE/source
hashes, field roles, complete owner and two separate receipts.
There is no broad cold replay, full game binary reproduction,
runtime certification or original source provenance claim.

Reproduce only the selected scope:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit script-viewer-release-enemies --unit script-viewer-create \
      --unit enemy-manager-clear --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit script-viewer-release-enemies --json
    scripts/repo-python scripts/ci.py
    git diff --check
