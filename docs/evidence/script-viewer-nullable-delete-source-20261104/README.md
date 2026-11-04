# TH10 ScriptViewer retained nullable delete @0x0040A430 — source-present, NONEXACT

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original separate CC-delimited **18-byte** body `0x0040A430..0x0040A441`
uses a private ESI `ScriptViewerView *` argument and plain `RET`:
null check; nonnull pointer pushed for actual `ScriptViewerView::~ScriptViewerView`
at **0x0040A1A0**; then the same pointer passed to scalar
`operator delete` at **0x004524A1**, with caller-side final cleanup.
This body is *physically distinct* from the already accepted 27-byte
`ScriptViewerView::ScalarDeletingDestructor` at `0x0040A410` and 67-byte
`ScriptViewerCreate` at `0x0040A3C0`. Original source TU, authored-versus-
compiler outline ancestry and any independent caller remain unresolved.

`src/ScriptViewerNullableDelete.cpp` adds a natural whole-object
`ScriptViewerNullableDelete(ScriptViewerView *)` implementation. It uses the
existing exact-layout 0x688-byte `ScriptViewerView` declaration, normal
`if (viewer != NULL) delete viewer`, and emits the genuine nonvirtual destructor
and scalar `operator delete` call *symbol identities*. No fake ESI ABI,
shadow owner, target-byte patch, shortened owner or synthetic return is used.

## Actual VC7.1 results and scope

The pinned MSVC 7.1 SP1 `/GL` native link emits a complete **27-byte** PDB
owner. Its ordinary explicit-stack calling convention must load the pointer
from the stack and preserve ESI; its two candidate `REL32` fields are the
actual destructor and scalar-deleter identities, but their offsets differ
from the target. The native 18-byte function already receives its pointer
privately in ESI. The preliminary positional ordinary comparison yields
**2/14**, entirely nonexact. It is a *complete semantic source mapping*,
not an exact byte proof; `config/matches.csv` is unchanged and original
provenance remains indeterminate.

A fresh selected cold VC7.1 `/GL` real ScriptViewer graph replayed only the
adjacent already-accepted functions `script-viewer-create` **67/67** and
`script-viewer-scalar-delete-0040a410` **27/27**, **94/94 total exact**.
Neither existing source nor exactness credit was changed. Reproduce:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-create --unit script-viewer-scalar-delete-0040a410 --json
scripts/repo-python scripts/ci.py
```

Private independent native receipts:
`.analysis/gpt-web/20261104-next/nullable-viewer-probe.log`,
`viewer-nullable-selected-guards.json`,
`build/gpt-web/20261104-next-nullable-viewer/source.pdb`.
No full-game product/runtime/portability claim is made.
