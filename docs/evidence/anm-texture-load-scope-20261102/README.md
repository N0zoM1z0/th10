> **Historical 242/243 frontier.** This document records an earlier
> compiler/source epoch. The complete 243-byte uploader was recovered
> as exact on 2026-11-09; see
> [the current exact proof](../anm-texture-load-exact-20261109/README.md).

# TH10 ANM non-region texture load: independent frame recovery (2026-11-02)

AnmRenderManagerView::LoadTexture at target 0x00446C70..0x00446D62
owns **243 bytes**, and is still **NONEXACT**. This checkpoint changes the
real maintained source, not only notes. The previous one-TU /GL graph emitted
a complete **250-byte** owner with a 0x14 allocation plus a saved EBP and
an incorrect early surface initialization schedule. The currently retained
source has a **242-byte** PDB-owned native extent, with the target 0x18
stack frame and complete corresponding prefix/branch structure.

## Distinct local lifetime correction

The one source rectangle is now declared inside the embedded-texture branch;
the surface pointer remains uninitialized until a real null assignment
immediately before the GetSurfaceLevel call. This does not discard the
branch-specific rectangle initialization, the surface release, or the
external-file and embedded-data upload logic. The original input and
function declaration remain unchanged.

Native and target prologues agree, including all instructions/stack homes
through +0x70 once the three initial real linked address fields are
normalized. At +0x71 their corresponding local branch displacements differ
by one, because the target embedded-data header computation is one byte
longer. The target at +0x72 uses MOV ECX,[ESI+0x30] and
LEA EAX,[ECX+ESI]; the native code chooses MOV EAX,[ESI+0x30]
and ADD EAX,ESI. The target MOV+LEA sequence is eight bytes against
seven native bytes. The remaining register schedules are also nonidentical.
**This is not an exact unit and no matched byte count is granted.**

Independent target whole-owner SHA-256:
d9fb43fe72a444d2309d5e6ce258d75616a287a5feaf6085eb253506bc55fcb9.

Full native owner SHA-256:
58696153c441034dd3ad74148aa0c2ce4db95eed2ff0c3e8047ff36da115fa70.

The complete target is the hash-attested original Japanese TH10 1.00a
SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

## Native experiment and regression

The real source was compiled in the single-TU AsciiManagerView::Initialize
LTCG graph. The full LoadTexture PDB contribution is 242 bytes, with
five DIR32 and three REL32 fields at native offsets
0x05, 0x13, 0x6C, 0xAC, 0xB2, 0xC8, 0xD8, 0xE3.

Three further bounded actual compilations tried an inlined typed
texture-payload accessor, an inline base/offset accessor, and an inline
base/entry accessor. Every form emitted the same 242-byte LoadTexture and
319-byte LoadTextureRegion owner. No helper implementation was retained
solely to force optimization behavior; the direct pointer expression is
clearer and remained selected.

After all experiments, a **selected cold build** of three existing canonical
resource units passes **437/437 exact bytes** in one one-source artifact:

    nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
      --unit anm-resource-create-texture-from-file \
      --unit anm-resource-create-texture-from-anm \
      --unit anm-resource-create-empty-texture --json
    scripts/repo-python scripts/ci.py
    git diff --check

There is no original TU, fully linked game, runtime, or exactness claim for
either upload owner. Neither gets a match ledger entry or partial credit.
