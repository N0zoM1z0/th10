# CSound::Get3DBuffer: complete 52-byte target-bound native exactness

Target: original Japanese TH10 1.00a, CSound::Get3DBuffer at
0x0044D400..0x0044D433, complete 52-byte reviewed PDB contribution.
This checkpoint adds a **new canonical exact owner**. Its original
source authorship and game-versus-SDK provenance remain **indeterminate**.

## Recovered source behavior

The former target-only gap between CSound::GetFreeBuffer and CSound::Play
has a maintained definition in src/ZWave.cpp. It checks the actual
CSound buffer-pointer array at +0x04 and returns CO_E_NOTINITIALIZED
if that array is absent. For an unsigned index outside the count at
+0x10, it returns E_INVALIDARG. Only after both checks does it
clear the caller's output pointer and QueryInterface the selected
DirectSound buffer for IID_IDirectSound3DBuffer. The caller-visible
null/out-of-range ordering and all returns match the target.

The target's single external address field is a DIR32 immediate at
owner offset +0x2B: the DirectSound 3D buffer IID at 0x0046747C.
The original target stores the 16-byte GUID there as
86fa9a278149ce11a5210020af0be560. The native diagnostic
uses a linker anchor for that external source symbol; exact
codegen binds that precise original target destination rather
than copying target bytes into the reconstruction.

## Two independent selected cold builds

The genuine ZWavePlayRoot compiler context, with
ProbePlayRoot as a natural code entry and pinned VC7.1 SP1
/GL /GS, generates a full 52-byte PDB member for
CSound::Get3DBuffer. Its source symbol is
?Get3DBuffer@CSound@@QAEJKPAPAUIDirectSound3DBuffer@@@Z.

One selected canonical cold rebuild proves the new 52-byte
function together with CSound::Play 145 and CSound::FillBufferWithSound
483, **680/680 bytes** in a single real source graph. A second,
independent cold rebuild selects **only** the new 52-byte unit and
reproduces the full body/field again. The two resulting PE/PDB
identities and GUIDs are different and included in
[native-summary.json](native-summary.json).

The strict comparator checks complete owner length, PDB module,
ordinary code bytes, all fixed internal branch destinations
and the only original IID linkage. It reports zero differences.
The source uses the ordinary nonvirtual member declaration
and no forced register ABI, copied target bytes, inert filler,
binary patch or fabricated callable entry.

Reproduce only the affected scope:

    nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
      --unit zwave-csound-get3d-buffer \
      --unit zwave-csound-play --unit zwave-csound-fill --json
    nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
      --unit zwave-csound-get3d-buffer --json
    scripts/repo-python scripts/ci.py
    git diff --check

This adds **52 canonical codegen-exact bytes** and one exact owner.
No authored-origin numerator is promoted; complete link/product
closure, data-owner reconstruction, runtime equivalence and
portability remain separate open questions.
