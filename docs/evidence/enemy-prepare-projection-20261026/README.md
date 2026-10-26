# Enemy projection helper — full 60-byte exact owner

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Verified 2026-10-26 in the real ten-TU Enemy dispatcher /GL /GS native
context, with the AnmInt3Scale `/Ob1` support override retained.

`EnemyPrepareProjection` is an already-reviewed authored function at
`0x00401D20`, 60 complete bytes. The real Enemy dispatcher calls this helper.
The maintained source sets the active viewport, calls
`EnemyConfigureViewportOwner`, sets the D3D viewport and publishes the index.
The two helpers' formerly `static` declarations are now ordinary
`__declspec(noinline)` functions. This lets the linker expose their genuine
symbols and naturally select the helper's internal ESI-supervisor / EBX-index
ABI, without source-level register forcing or target-byte patching.

Two independent pinned VC7.1 SP1 cold builds each produce a uniquely PDB-owned
60-byte contribution. Both pass the **complete** linked-PE comparison: all 60
bytes, the one `REL32` field at offset `+0x17`, the target-independent symbol
binding, and the full extent/ABI/callsite checks. First and second linked-image
SHA-256 values, respectively:

- `327205224ccce9cc3d3ae30bd0b6671873483473d787125ff6f520e48f6b679e`
- `95a546f62ffeb8adf890a214149434b13582cc29988330b298ff15b4365616fd`

Their native code is **not raw byte-equal** to the target until the declared
call's link-resolved field is normalized. The field names the native
`EnemyConfigureViewportOwner` and independently reviewed target callee
`0x004215A0`. That full target callee is **not** exact: the reviewed role and
single call target do not prove the callee's own 330 bytes, production TU, or
runtime identity. No further function extent, static data, or original ABI
assertion is promoted by this checkpoint.

In the second native image, independently compared complete same-image units
also pass: 36 ANM guards / 4,618 bytes / 121 fields; 16 ECL guards / 1,617
bytes / 21 fields; and the full EnemyDropItemCounts (48 bytes) and
EnemyDropItemCountsCore (503 bytes). The ANM/ECL sets overlap and **must not**
be summed as unique owners. These checks guard against regressions but do not
establish a reconstructed Windows product or runtime test.

The live diagnostics and both terminal-cold receipts remain in ignored
`.analysis/gpt-web-th10-ecl-20261026/`, including
`projection-cold-1.json`, `projection-cold-2.json` and
`all-guards-result.json`. To reproduce the declared exact unit from source:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/replay-exact-units.py --unit enemy-prepare-projection --json
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/progress.py --check
```

Only this complete helper is newly credited: **+60 authored exact bytes and
one source/exact owner**; the authored denominator is unchanged. The ongoing
14,416-byte Enemy dispatcher remains nonexact, as do ECL Run and ANM
ExecuteScript. No native-product, target-runtime or portability gate is opened.
