# TH10 StartupWorker retained nullable delete @0x0041FD70 — source-present, NONEXACT

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The standalone CC-delimited body **0x0041FD70..0x0041FD81** consists of
**18 bytes** and uses a private ESI `StartupWorkerView *` input. Its complete
native sequence checks for null, passes the nonnull object to the actual
`StartupWorkerView::~StartupWorkerView` at **0x0041FB50**, then calls scalar
`operator delete` at **0x004524A1**, and returns with plain `RET`.
Its separate 27-byte scalar-deleting-destructor at `0x0041FD50` and adjacent
69-byte factory at `0x0041FD00` are not part of this physical owner.
Original authored-versus-compiler-outline provenance, production TU and
independent direct-call identity remain unverified.

`src/StartupNullableDelete.cpp` now models this entire nullable object
destruction using the existing real, 0x3F0-byte `StartupWorkerView` C++ view.
It preserves the genuine destructor and scalar delete call symbols, with no
shadow class, fake ESI ABI, fabricated return or copied machine instructions.

## Full source compiler diagnostic, no exact credit

A fresh pinned VC7.1 SP1 `/GL` native source-only build emitted a complete
**27-byte** PDB function contribution, versus **18 original bytes**.
Its conventional stack-received parameter requires an ESI prologue/epilogue;
the native original already has its pointer in private ESI. Two candidate
`REL32` fields bind to the actual worker destructor and scalar deletion,
but their offsets are displaced versus the target. The raw preliminary
positional ordinary byte comparator reports **2/14**, a codegen/ABI artifact
and not a functional similarity percentage. **NONEXACT**: there is no new
`config/matches.csv` row, and no whole-game or original-source ancestry claim.

A selected real VC7.1 `/GL` cold replay of the established Startup graph
confirms the separate **128-byte** worker constructor, **69-byte** factory and
**63-byte** update callback remain **260/260 canonical exact** with all
existing target fields. No full cold replay was performed, and the new
standalone file does not replace any older original caller.

Repeat scoped accepted guards:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit startup-worker-create --unit startup-worker-constructor \
  --unit startup-update-callback --json
scripts/repo-python scripts/ci.py
```

Private full native receipts:
`.analysis/gpt-web/20261104-next/nullable-startup-probe.log`,
`startup-nullable-scoped-guards.json`, and the PDB/PE under
`build/gpt-web/20261104-next-nullable-startup/`.
