# Compact opcode-82 investigation archive

Read FINAL_RESULT.md first. No candidate was built. No archived invocation grants execution authority.

- verifier/: exact executed target local-order and baseline local-order scripts, their actual receipts and scoped interpretations. op82_metadata.py is retained because the portable regression chain imports it; redundant first-pass metadata receipt/plan are omitted.
- baseline/: successful unchanged-source baseline runner, locked manifest, actual receipt and necessary synthetic owner fixtures. INVOCATION.md is a historical executed instruction, not a request to rerun a build. V2_CORRECTION.md records the earlier failed preclaim without retaining failed runners.
- dependencies/: three pinned authored Python modules required for import-registration regression; no native payloads or full reconstructed source trees.
- destination-first-hypothesis/: frozen isolated patch and host evidence, now parked. Host scripts deliberately retain exact original source pin gates and require the externally maintained pinned snapshot at their documented path. They cannot be reproduced from this archive alone without that source. No native VC7.1 code-generation claim follows from the host tests.

Portable native-free regressions (Python 3.11+):

    PYTHONDONTWRITEBYTECODE=1 python verifier/test_op82_baseline_order.py
    cd baseline && PYTHONDONTWRITEBYTECODE=1 python -m unittest test_runner test_owner

Only two archived test path expressions were changed for portability: the pinned import module root and the baseline owner fixture path. Executed verifier scripts, baseline runner and frozen host scripts are byte-identical. MANIFEST.json records original and packaged hashes so these changes are explicit. Tests use synthetic decoder/PE fixtures and never open target or actual baseline artifacts. No decoder installation is needed for these tests.

Remote reproduction requires the exact pinned clean repository, original target, generated baseline artifacts, locked tools and an independently authorized bounded invocation. This packet intentionally contains none of the native image/object/PDB/MAP/anchor payloads. Receipts are hash/count/Boolean evidence only.
