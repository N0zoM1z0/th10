# Exact native recovery: AnmLoadedView::InitializeVm (0x00449870)

The maintained C++ body in src/AnmVmCreate.cpp now reproduces the complete
217-byte target contribution with the pinned VC7.1 compiler and LTCG.

## Verified replay

    scripts/repo-python scripts/build-match-unit.py --unit anm-loaded-initialize-vm
    scripts/repo-python scripts/compare-linked-function.py --unit anm-loaded-initialize-vm --json

The comparison report is preserved in report.json.

- Target extent: 0x00449870..0x00449948 (217 bytes).
- PDB contribution: complete 217-byte owner from source.ltcg.obj.
- Capstone decode: 217/217 bytes; normalization complete.
- Compared bytes: 217/217 exact.
- Linkage: one REL32 call at contribution offset 0xCD, bound to
  AnmLoadedView::SetAndExecuteScriptIndex at retail 0x0043E7E0.
- Unnormalized linked image bytes are not raw-equal because that call is
  relocated to the candidate's selected helper address. The comparator
  rewrites it to the retail target and then compares the complete function.

## Source-level closing observations

The earlier source had the right three vectors and script binding but did not
reproduce retail's store order. The decisive source shape is:

1. Copy the existing flag word into AnmVmFlagValue.
2. Reset positionOffset (+0x340) before position (+0x334).
3. OR the copied flag value with 0x40000000 and store it.
4. Store scriptIndex, reset alternatePosition (+0x34C), set glyph
   height/width, then bind and execute the script.

The first ordering trial differed at exactly two displacement bytes (+0x17
and +0x49). Reversing those two vector statements closed both differences;
the following full linked replay reported no byte differences.

This is a targeted function replay, not a full executable cold replay.
