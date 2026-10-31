# ASCII high-priority draw callback — complete 10-byte exact (2026-10-31)

The reviewed, authored `AsciiManagerView::OnDrawHighPriority @ 0x00401520..0x00401529` is now **canonical linked-PE exact: 10/10 physical bytes and 1/1 independently bound REL32 field**. The pinned Japanese TH10 v1.00a target SHA-256 is `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target saves EBX, copies the ECX callback receiver into EBX, directly calls `AsciiManagerDrawGuiStrings @ 0x00401A50`, restores EBX and returns. This is one complete ten-byte executable PDB contribution, not a crop of the adjacent 570-byte GUI renderer. Target-local registration at `AsciiManagerView::Initialize @ 0x00401110` explicitly publishes the adapter at priority 0x26. Its real source is the ordinary `__fastcall OnDrawHighPriority(AsciiManagerView*)` in `src/AnmManager.cpp`, without any register keyword, altered ABI, naked function, injected padding, binary patch or artificial return.

## Why the old EDI result was wrong for this graph

The earlier complete callback in the very same 3-TU `AsciiManagerCreate` `/GL` caller graph emitted ten bytes with EDI instead of EBX: *three ordinary differences* at offsets `+0`, `+2` and `+8`, while the sole REL32 call remained correctly typed. The complete downstream `AsciiManagerDrawGuiStrings` was 562 bytes versus the original 570; its first instructions expected EDI instead of target EBX. Old StageDraw-driven seven-byte stubs with a stack receiver were another context, not evidence to waive target caller/callee register constraints.

Introduce one ordinary pointer `AnmVmView *vm = &manager->primaryVm014` after the existing GUI-string cursor declaration; use it only for the actual vector/scale setup, immediate draw path, and sprite/color state. Continue accessing other semantically distinct owner fields directly, preserving the flag-wide assignment, newline path and character-spacing publication. All pointer targets are the already-existing VM; no new state is allocated. This natural source model changes VC7.1's interprocedural register allocation to the target EBX receiver in the real factory+initialization graph. Direct independent A/B rebuilding of the *same* 3-source graph reproduces 3 ordinary callback differences with the prior source, versus **0** with the selected source.

The larger GUI renderer remains **NONEXACT**. Its new complete candidate is 576 bytes versus 570 target bytes; a structural candidate diagnostic in the *same* graph improves from 54/458 to 160/458 comparable position bytes. This is a diagnostic score, **not an accepted 570-byte match**, and the full renderer remains in the backlog. Other source-candidate controls changed alias scope and made the renderer 575-577 bytes; those yielded the same ten-byte exact callback in several variants, but the selected source has the best retained comparable score in the natural 3-TU graph. No renderer bytes are credited.

## Complete strict cold acceptance and unchanged caller guard

The new canonical `ascii-manager-on-draw-high` shares its actual producer `build/match-linked/AsciiManagerInitialize` with the existing 330-byte `ascii-manager-initialize` unit. It uses the same VC7.1 SP1 `/GL` profile, real `AsciiManagerCreate` entry, `GameErrorContext.cpp` and `Chain.cpp` support sources. Every selected cold replay strictly requires a unique PDB contribution, **10/10 complete callback bytes** and sole `AsciiManagerDrawGuiStrings` REL32 call identity. Two independent selected cold links pass both units, **340/340** bytes in the shared artifact, across distinct PDB GUIDs `7d5138bb414d0b449ab073451379b0c2` and `1fb7b3523cb48d4499184edf86ac2be7`. No other historical cold replay was run.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit ascii-manager-on-draw-high --unit ascii-manager-initialize --json
scripts/repo-python scripts/ci.py
git diff --check
```

A ten-byte codegen match and actual registration/ABI evidence do not prove that the original class/function declarations, source TU, complete 570-byte renderer, owned game data, whole product or runtime are reconstructed. The source is retained as a controlled machine-codegen improvement, not as a forced optimizer trick. Superseded diagnostic PE/MAP/PDB variants under build/gpt-web/current-turn were pruned during the 2026-10-31 storage cleanup; source experiments and full cold reports remain under .analysis/gpt-web/current-turn. Rebuild the accepted callback through the selected ascii-manager-on-draw-high / ascii-manager-initialize replay command above rather than relying on old diagnostic images.
