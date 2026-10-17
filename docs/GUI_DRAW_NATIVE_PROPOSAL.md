# One fixed native diagnostic proposal

Prepared 2026-10-17 UTC. This command was not executed during packet preparation; later integration/build results must be recorded separately.

Prerequisites: parent finishes and checkpoints the lifecycle packet, reviews/applies this raw-byte patch, reruns the local contracts, and confirms pinned tools/target as usual. Parent remains sole remote builder. This is a diagnostic, not a new canonical match unit or a request to run the executable. Existing canonical profiles stay unchanged.

## Genuine context and fixed source order

Entry is the target-observed 415AF0 callback, naturally represented as GuiDrawCallback(void*), expected C++ fastcall symbol ?GuiDrawCallback@@YIHPAX@Z. Its actual registration in 413980 selects this address at draw priority 43; no synthetic initializer is needed. The callback calls the complete GUI draw 724-byte owner, which genuinely calls the 483-byte DrawColoredRectangle and existing the 216-byte ANM Draw. Rectangle calls existing the 133-byte FlushVertexBuffer. No new tiny-owner exactness claim.

Compile source order: GuiDraw.cpp, AnmRectangle.cpp, AnmManager.cpp. This is the chosen responsibility order, not a search parameter. The existing canonical anm-render-draw producer uses AnmManager.cpp alone with no support_sources, /GL and the standard anchored renderer link profile. That proves the maintained dispatch/draw family already has an established single-TU diagnostic context; the new graph only adds its genuine GUI caller and rectangle sibling. No speculative RandomMath/ECL/GUI initialization source is necessary for this initial diagnostic. All three use the same copied canonical compile profile, without per-TU exceptions.

## Proposed command

From the repository root, once authorized to build:

```sh
PYTHONDONTWRITEBYTECODE=1 PYTHONPATH=scripts scripts/repo-python - <<'PY'
from pathlib import Path
import json
import tomllib
from ltcg_link import cold_link, tool_environment, LINK_PROFILE, HARNESS_KIND

root = Path.cwd()
with (root / 'config/match-units.toml').open('rb') as f:
    unit = tomllib.load(f)['units']['anm-render-draw']
if unit['source'] != 'src/AnmManager.cpp' or unit.get('support_sources', []):
    raise RuntimeError('Canonical ANM renderer context changed; re-audit first')
if unit['harness'] != HARNESS_KIND or unit['link_profile'] != LINK_PROFILE:
    raise RuntimeError('Canonical harness/link profile changed; do not substitute')
linker, environment = tool_environment()
directory = root / 'build/diagnostic-gui-draw-callback'
result = cold_link(
    root / 'src/GuiDraw.cpp', directory,
    '?GuiDrawCallback@@YIHPAX@Z', linker, environment,
    profile=list(unit['profile']),
    support_sources=[root / 'src/AnmRectangle.cpp', root / 'src/AnmManager.cpp'],
)
required = ('DrawStageElements', 'DrawColoredRectangle',
            '?Draw@AnmRenderManagerView', 'FlushVertexBuffer')
bad = [s for s in result['unresolved_symbols'] if any(x in s for x in required)]
if bad:
    raise RuntimeError('Real owner/dependency unexpectedly anchored: ' + repr(bad))
(directory / 'diagnostic.json').write_text(json.dumps(result, default=str, indent=2))
print(json.dumps(result, default=str, indent=2))
PY
```

The symbol spelling is the natural MSVC fastcall expectation, not newly compiled evidence. cold_link rejects any unresolved entry and verifies PE entry/map/PDB executable contribution identity; it must fail rather than anchor a guessed/missing callback. If this command fails or owners are absent/inlined, report that result and inspect actual compiler evidence. Do not force noinline, alter entry, move TUs or sweep flags to obtain desired sizes.

## Required inspection after the single build

Use returned image/map/PDB paths with existing inspect-linked-functions.py. Confirm callback identity and the complete PDB-owned DrawStageElements and DrawColoredRectangle contributions; do not compare a 724/483-byte arbitrary window through neighbors. Inspect direct real callee contributions and their relative-call relocations, all global/constant references, callback stack cleanup and rectangle EDI/EBX ABI. Compare full target-owned extents only after exhaustive link-field coverage. Preserve FILD and FSTP locations; look for extra early float stores rather than interpreting host witness agreement as proof.

Existing cold_link writes diagnostic-only 4-byte data anchors for unresolved symbols. They are not callable function implementations or valid runtime global storage. Imported D3D, all dynamically dispatched device methods, CRT/import stubs and global initialization remain nonexecuting boundaries. The four real owners/dependencies above must not resolve to anchors. No still-source-empty direct callee remains in the proposed 724/483-byte source bodies, but transitive ANM runtime/graphics/global closure is not claimed. Never run this linked diagnostic image.

No config/match-units.toml changes, canonical receipts, build outcomes or exact-byte credit are included in this proposal.
