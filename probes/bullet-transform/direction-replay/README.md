# Bounded direction-NaN replay

`verify_direction_nan.py` is the exact reviewed and executed standalone collector, SHA256 581eb1fdb1e4e2068c901bfea6d402c9aca1bde5f27d48a2192596e5e90da671. It reads only the pinned 1454-byte owner and the two explicitly authorized immutable table ranges after whole-image, repository, source-layout, ledger, and tool identity checks. No native payload or translated trace is included in this package.

Target replay is parent-authorized remote execution only:

    python3 verify_direction_nan.py --root REPOSITORY --target REPOSITORY/resources/th10.exe

It requires the pinned clean HEAD and repository Capstone helper. Do not run with Python optimization; the verifier deliberately rejects it. Normal output is bounded fixed predicates, counts, hashes, and explicit premises. Unsupported cases fail closed. `AUDIT.md` explains the concrete-domain result and broader limits, including inherited conservative output labels.

Local synthetic controls use only authored instruction fixtures:

    python3 -m venv /tmp/direction-nan-tests
    /tmp/direction-nan-tests/bin/pip install -r requirements.txt
    /tmp/direction-nan-tests/bin/python test_synthetic.py

The compact test file covers the final entry-witness and alignment layers: three fixed kinds, table selection, integer flag behavior, unknown inputs, wrong guard/kind, stack aliasing, and unsupported alignment cases. Earlier constituent CFG, receiver, affine, freshness, and x87 gates were independently reviewed/tested during construction; their full historical test bundles are not duplicated here. This compact suite is not a claim of exhaustive coverage. The collector is frozen unchanged; no production source edit is present.
