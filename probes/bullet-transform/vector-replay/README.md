# Bounded vector-NaN replay

This package contains one unchanged, reviewed, executed standalone collector plus compact authored synthetic controls. No target instruction bytes, disassembly, ordered trace, intermediate collectors, or production edits are included.

Parent-authorized target replay at the pinned historical checkpoint:

    python3 verify_vector_nan.py --root REPOSITORY --target REPOSITORY/resources/th10.exe

The collector checks clean HEAD, whole-image/source/ledger/tool hashes, bounded executable owner/table spans and complete owner CFG. It uses the repository's pinned Capstone helper. Unsupported semantics fail closed. Python optimization is deliberately rejected. Output contains bounded fixed predicates, counts, hash pins, premises and unresolved-stage categories.

For local synthetic tests, install `requirements.txt` in a test environment and run `python test_synthetic.py`. Capstone must be exactly 5.0.6. The fixtures are authored test instructions, not extracted target bytes. The compact suite covers full vector composition with protected PUSH, wrong input/result roles, extra PUSH rejection, and final exact-field/flag behavior. Earlier CFG/x87/origin/freshness suites were independently reviewed and tested during construction; this package does not duplicate their full history or claim exhaustive coverage.

Read AUDIT.md for the concrete input domain and first-result-store boundary. Inherited broader proof labels must not be interpreted as full return-state equivalence. The collector is preserved byte-for-byte for its execution receipt; do not silently update historical pins.
