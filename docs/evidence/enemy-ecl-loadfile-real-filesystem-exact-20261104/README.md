# Exact TH10 Enemy ECL resource loader — complete 95-byte owner at 0x0040CD20

Original target is Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
This is a new complete **95/95-byte exact** function, not a standalone
similarity score. Its source remains the already maintained real C++
EnemyEclResourceView::LoadFile(const char *) in src/Enemy.cpp.

## The previous reconstruction hypothesis was incomplete

The earlier isolated Enemy source compilation emitted a 95-byte function
with ten ordinary mismatches at +0x32..+0x3B. It pushed the archive
filename on the stack for FileSystem::OpenFile. The actual target calls
that already recovered FileSystem owner at 0x0044B360 using EAX for its
filename and two stack arguments (NULL sizeOut, mode 0). The erroneous
isolated context is preserved as historical counterevidence and is not
treated as an acceptance proof. No invented EnemyLoadFileBytes API, forced
register declaration or edited target instructions are used.

The natural **five-real-translation-unit** selected VC7.1 SP1 /GL graph
anchors Enemy.cpp at the actual LoadFile entry, with FileSystem.cpp,
PbgArchive.cpp, EclVm.cpp and GameErrorContext.cpp linked as support.
The compiler now naturally lowers FileSystem::OpenFile to its real
private EAX filename ABI. The complete PDB-owned LoadFile function is
95 bytes, including the buffer reset and inline strcat, both real
callee calls, nonnegative-result normalization, epilogue and RET 4.

## Exhaustively bound original fields

All five original address fields are independently checked by the strict
complete-owner comparator, not silently normalized:

| Owner offset | Kind | Original target binding |
| --- | --- | --- |
| +0x0B | DIR32 | filename buffer 0x00497C38 |
| +0x1A | DIR32 | filename buffer 0x00497C38 |
| +0x3C | DIR32 | filename buffer 0x00497C38 |
| +0x43 | REL32 | FileSystem::OpenFile 0x0044B360 |
| +0x4B | REL32 | EclVmScriptDatabase::AddScriptData 0x00450220 |

Two **independent cold linked PE/MAP/PDB** generations with the unchanged
installed source match the *entire* target: 95/95 bytes and 5/5 original
fields, with different image SHA-256 and PDB GUIDs. See
[native-proof.json](native-proof.json) for source hash, both concrete
image identities, complete field manifests and target address resolution.
The canonical recipe is
enemy-ecl-resource-load-file-0040cd20 in config/match-units.toml.

The selected old-exact regression set also passed **633/633 bytes**:
EnemyEclResourceView::LoadPackage 239,
EnemyEclResourceView constructor 34, and the independently exact
FileSystem::OpenFile 360. No all-unit cold replay was run.

To reproduce only the new proof and directly related existing functions:

    scripts/repo-python scripts/replay-exact-units.py       --unit enemy-ecl-resource-load-file-0040cd20       --unit enemy-ecl-resource-load-package       --unit enemy-ecl-resource-constructor       --unit file-system-open-file --json
    scripts/repo-python scripts/ci.py

The source file and historical source spelling, whole Windows executable,
file I/O at runtime, and the separately nonexact 589-byte AddScriptData
callee remain independent questions. This exact credit is *only* for the
complete 95-byte function at its reviewed address.
