# TH10 BulletManager draw callback — 44-byte newly inventoried owner exact

Original target TH10 Japanese v1.00a: the registered callback starts at
`0x004067A0` and ends with RET at `0x004067CB`, yielding **44 dense bytes**;
`0x004067CC..` is CC padding before the neighboring bullet spawning code.
The prior initial Ghidra inventory did not list this *whole function*, so a
fresh reviewed row has been inserted into `config/functions.csv`. A real
original caller in the already-exact `BulletManagerView::Initialize` at
`0x00405E8F` pushes the address `0x004067A0` into the draw-chain task. It is
not a substring/optimizer interior entry, so a distinct boundary is justified.

The callback accepts the manager in `ECX`, returns 1, reads the optional
update/draw gate pointer from original data global **0x00477810**, and
checks flag bit 2 at gate +0x58. With no gate, or when that bit is unset,
it iterates six buckets numbered 0..5, passing its manager in native `EAX`
and each bucket index in `ECX` to the real target 0x004066E0 `DrawBucket`.
The maintained source `src/BulletDraw.cpp` implements this real callback,
reuses the known `BulletManagerView` record layout, and keeps its separate
134-byte bucket-rendering callee in the same file.

## Codegen proof and denominator change

The pinned VC7.1 SP1 `/GL` single-source linked-PE graph selects
`BulletManagerDrawCallback` itself as the real entry. The compiler produces
one uniquely PDB-owned **44-byte** contribution matching **36/36** ordinary
source/target bytes and exactly two genuinely target-bound external fields:

- `+0x01` DIR32 `g_BulletUpdateGate` = **0x00477810**;
- `+0x1A` REL32 `BulletManagerView::DrawBucket` = **0x004066E0**.

Two different zero-reuse cold PEs with distinct image SHA-256s and PDB GUIDs
reproduce the entire **44/44** owner in the strict canonical comparator,
with both fields matched and no skipped or patched bytes. Reports and logs:
`.analysis/gpt-web/20261103/bullet-draw-callback-cold{1,2}.json/.log`.
The ledger gains one target-reviewed authored function boundary and one
new canonical exact 44-byte function; the new boundary is not disguised as
an existing denominator entry. Neither the original source filename nor
runtime behavior of the reconstructed executable is established.

## The 134-byte draw-bucket owner is separate and nonexact

The adjacent `0x004066E0..0x00406765` target body reads a bucket head at
manager +0x14 + index*4, walks `BulletRuntimeView::nextInDrawBucket` at
bullet +0x44C, and copies the bullet's world position into embedded
`AnmVmView` +0x334, adding native floating offsets +224 (0x00470B4C)
and +16 (0x00470B48) to X/Y. For bullets with `vm.flags35C` bit
0x08000000, it normalizes angle +1.5707964f through real 0x0044BC10,
updates `vm.rotation.z` and sets flag bit 4; each bullet is rendered
via actual `AnmRenderManagerView::Draw` at 0x004451C0.

`BulletManagerView::DrawBucket` is newly implemented as full typed C++ source
but remains **NONEXACT**: isolated VC7.1 links vary between 136 and 144
bytes against target 134, while actual ANM LTCG support produces
139/134 with 30/114 initial preliminary ordinary bytes. Preserving the real
world-position reads as const volatile float references improves that same
139-byte graph to **35/114** preliminary ordinary bytes without changing the
callback. This qualifier models ordering, not original threading semantics. Original private EAX/ECX
transport and loop scheduling remain unresolved. The callback's exact REL32
call **does not** make the draw-bucket function itself exact. No target bytes,
synthetic register stubs or match ledger credit were added for that owner.

Only scoped cold replay of the callback, related bullet update/initialization
owners and repository checks was performed, never the full game cold replay.

## Further bucket-renderer controls — no false exactness

Four more pinned VC7.1 `/GL` links against the real `AnmManager.cpp`
explored the exact x87 read/add order and original bit-flag store in
`BulletManagerView::DrawBucket` (134 target bytes): reversing X/Y add
terms held **139/134**, **35/114** comparable bytes; direct nonvolatile
member reads fell to 30/114. A volatile in-memory OR emitted 143 bytes
and 25-29/114 comparable bytes. Both C++ addition order and the volatile
OR qualifier were restored after these neutral/regressive trials, leaving
the previous strongest typed source and the **44/44** exact callback
unchanged. Full candidate receipts are retained under
`.analysis/gpt-web/20261103/bullet-draw-order-results.json`.
