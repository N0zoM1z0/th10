# CSound::GetBuffer: exact complete 19-byte DirectSound leaf (2026-11-02)

The original Japanese TH10 v1.00a function at 0x0044D3E0..0x0044D3F2
owns exactly **19 physical bytes**. Its maintained ordinary member source
now reproduces all of them, **raw-equal with zero external fields and
complete instruction decoding**. This adds one canonical codegen-exact
owner and 19 bytes, not a fragment of the adjacent Get3DBuffer.

## Independent target behavior and source recovery

The target receives CSound in ECX and an unsigned index in EDX.
It reads the buffer array from +4; NULL returns zero. It compares
the index against the unsigned count at +0x10; an index at/above the count
returns zero. Otherwise it returns the selected DirectSound interface
pointer. It emits a distinct immediate zero/RET block before its successful
indexing path. The original slot at 0x0044D3F3 is following CC padding,
not part of the owner.

The previous combined condition in the pending maintained source:

    if (m_apDSBuffer == NULL || index >= m_dwNumBuffers)
        return NULL;
    return m_apDSBuffer[index];

compiled to the same 19-byte length but ordered success before the
shared failure return (JE/JAe versus target JE/JB). Separating the
two real rejection conditions:

    if (m_apDSBuffer == NULL)
        return NULL;
    if (index >= m_dwNumBuffers)
        return NULL;
    return m_apDSBuffer[index];

naturally selects the target JE/JB/RET schedule, reproducing **all 19 bytes**.
No target bytes, inlined register asm, fabricated parameter, no-op instruction,
or unrelated caller behavior is changed. Full controls with positive/nested
conditions and explicit failure labels were rejected and restored.

## Genuine source/PDB owner and focused cold replays

The source is the actual CSound member implementation in src/ZWave.cpp.
The diagnostic compiler context is the existing source-facing
ProbePlayRoot entry in the /GL /GS ZWavePlayRoot graph, using the pinned
VC7.1 SP1 toolchain and the original target's hash/PE identity.

Two independent **selected cold** compiler/linker builds reproduce complete
19/19, along with the already accepted original complete bodies:

| Unit | Exact bytes | Address fields |
| --- | ---: | ---: |
| CSound::GetBuffer | 19 | 0 |
| CSound::Get3DBuffer | 52 | 1 (IID DirectSound 3D) |
| CSound::Play | 145 | 5 |
| CSound::FillBufferWithSound | 483 | 6 |
| **Per cold producer** | **699** | **12** |

Both cold PE images and PDB GUIDs are distinct, all selected functions
have PDB-owned whole extents and exact target address bindings, and
GetBuffer itself is raw-equal with no normalization required.

[Native summary](native-summary.json) records the target/source hash,
exact target function body fingerprint, normalized field counts, PDB owner,
per-cold PE/PDB identities, and the strict zero-difference outcome.

Reproduce just this scope:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit zwave-csound-get-buffer --unit zwave-csound-get3d-buffer \
      --unit zwave-csound-play --unit zwave-csound-fill --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit zwave-csound-get-buffer --json
    scripts/repo-python scripts/ci.py
    git diff --check

Original SDK/game source authorship and isolated callable entry provenance
remain **indeterminate**; neither original source ownership nor a runnable
complete game build is asserted. No authored-origin exact credit is added.
