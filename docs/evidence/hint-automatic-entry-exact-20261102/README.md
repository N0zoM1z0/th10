# TH10 Hint automatic-entry allocator: 220-byte authored exact owner

The reviewed complete TH10 Japanese 1.00a function at
0x0041A120..0x0041A1FB now cold-replays **all 220 bytes and all seven
independent DIR32/REL32 fields**, including original internal control
transfers and the RET8 ending. Source mapping remains
HintOwnerView::AddAutomaticEntry in Hint.cpp. The origin ledger already
classifies this game owner as authored; its exactness adds 220 authored
and canonical codegen bytes, rather than partially crediting a prefix.

## Source recovery

The target allocates/constructs the 0x88-byte HintEntryView, appends the
node to the stage export list, sets remaining=30, copies 12 position
bytes, copies at most 64 text bytes, sets alignment zero, writes the
stage/base key, and selects the frame counter. Both reads of
g_GameFrameRestorePending at offsets +0xB6 and +0xBE are present
as separate actual global loads in the original target. The ordinary
direct repeated variable expression let VC7.1 coalesce those reads and
reorder alignment/counter operations, giving a prior 212/220 candidate.

The maintained source now uses a C++ const volatile reference scoped
locally to the actual existing shared stage flag, preserving the two
physical observations while leaving the two decisions, global storage
and outward API intact:

    const volatile int &stageFlag = g_GameFrameRestorePending;
    entry->base = stageFlag;
    entry->count = stageFlag != 0
        ? g_GameFrameCounter1 : g_GameFrameCounter0;

The generated body returns to target **220 bytes**, with every byte of the complete owner validated after strict binding
of the seven independently observed external fields. This is a codegen constraint
and does **not** establish the original source's use of volatile,
thread synchronization or the original physical translation unit.

## Native exact proof and regression scope

The canonical nine-source HintCreate graph, including one real
normal-compiled HintLoad support and the real CreateHintOwner entry,
yields a PDB-owned 220-byte AddAutomaticEntry in Hint.cpp.
Two independent selected /GL cold builds pass all three units:

| Unit | Owned bytes | External fields |
| --- | ---: | ---: |
| HintOwnerView::AddAutomaticEntry | 220 | 7 |
| CreateHintOwner | 67 | 5 |
| HintOwnerView::OnDraw | 6 | 0 |
| Selected total per cold | 293 | 12 |

The two PE hashes and PDB GUIDs differ. Both complete owners, all
semantic roles and local destinations, the natural constructor/copy
schedule and the actual outer API remain checked through the
repository's unmodified strict comparator. Independently selected
consumer guards also preserve AddCaution104 and the other three
constructor/ReadLine/Append51+260+52, **467/467 bytes** across
the affected source graphs. No full historical cold replay was run.

[Native evidence](native-summary.json) retains exact target/source
hashes, all seven address identities and field locations, native
owner identity, two cold receipts and the selected consumer guards.

Source controls with memory read/write barriers around alignment or
base stores stayed at 212 bytes; combined local volatile/reference
plus barriers changed other instructions. The unqualified source's
byte count was not admitted. No original game bytes were patched
or copied, no artificial parameters/return paths, and no fake ABI
or inert padding were introduced.

The larger 1317-byte ConsumeEntries, main Hint loader and 571-byte
Hint update are **not** promoted: their own complete comparisons remain
nonexact. Product closure, source authorship, runtime equivalence and
portability are separate open questions.

Reproduce scoped acceptance:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit hint-add-automatic-entry --unit hint-create-owner \
      --unit hint-draw-callback --json
    scripts/repo-python scripts/replay-exact-units.py \
      --unit hint-add-caution --unit hint-entry-constructor \
      --unit hint-read-line --unit hint-append-link --json
    scripts/repo-python scripts/ci.py
    git diff --check
