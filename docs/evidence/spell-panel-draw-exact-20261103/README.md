# TH10 spell-panel draw chain @ 0x004091C0 + 0x00409270

**Current result:** both 95-byte draw parent and 10-byte callback are
canonical exact. The older one-source nonexact diagnostic below is retained
as a controlled explanation of why real ANM LTCG context is required.

Independent target disassembly and read-only Ghidra review confirm that the
previously source-absent `0x004091C0..0x0040921E` is a complete **95-byte**
draw routine over the existing `EnemyGameStateView`. It checks flag `+0x378C`
bit 0, traverses eight 0x3AC-byte `frontVms` starting `+0x778`, followed by
five 0x3AC-byte `secondaryFrontVms` starting `+0x24D8`, and invokes real
`AnmRenderManagerView::Draw` at 0x004451C0 for every VM. It returns 1.
This is a complete, maintained but **nonexact** source owner; the first
isolated /GL source link emitted 93 bytes versus 95 target bytes, with four
candidate address fields and 38/79 comparable ordinary bytes. It receives
the panel owner in private EBX; the C++ method name is descriptive.

Native 0x00409270 is the ten-byte callback adapter, holding the true owner
in ECX and calling 0x004091C0. Source lives in `src/SpellPanelDraw.cpp`, and
its 10-byte VC7.1 codegen, including the sole actual callee identity, is
**canonical exact**. The independently verified code is:

```
push ebx
mov ebx, ecx
call 0x004091C0
pop ebx
ret
```

Two independent, zero-reuse pinned VC7.1 SP1 `/GL`/`/MT /O2 /Gy /GF /Oi`
linked-PE builds using `EnemySpellPanelDrawCallback` as the *real entry*
produced different image SHA-256s and PDB GUIDs. Both compare **10/10**
bytes after normalizing the one reviewed `REL32` field at offset `+4` to
0x004091C0; all remaining bytes are raw equal. They were built serially
with `scripts/build-match-unit.py --unit enemy-spell-panel-draw-callback` and
compared via `scripts/compare-linked-function.py --unit ... --json`.
Evidence logs/complete JSON receipts are retained privately under
`.analysis/gpt-web/20261103/spell-panel-draw-cold{1,2}.{log,json}`.

This one exact adapter is admitted to `config/matches.csv` and the complete
manifest. Its actual registration at 0x00408A11 takes the callback's VA;
its nominal callback-versus-optimizer-generated origin remains indeterminate.
No exactness or original-TU/runtime claim is made for the 95-byte parent or
the 402-byte owner destructor. Product and runtime validation remain open.

## 95-byte parent exactness — superseding the isolated probe

**This section supersedes the nonexact isolated-graph diagnosis above.**
A fresh, deliberately small two-source compile passes `AnmManager.cpp` as a
real `/GL` support input. The actual implementation of
`AnmRenderManagerView::Draw` becomes visible to VC7.1 LTCG, which promotes
the VM argument from the ordinary source stack to the target's private
**EAX** register. The genuine native entry retains the panel owner in EBX.
No register-forcing annotation or ABI shim was introduced, and the source
body is unchanged.

The function immediately expands from isolated **93/95** to **95/95** bytes,
with **79/79 comparable ordinary instruction bytes** and four real fields.
A new canonical `enemy-spell-panel-draw-vms` unit uses
`SpellPanelDraw.cpp` + `AnmManager.cpp`, and
`EnemySpellPanelDrawCallback` as the actual entry, with independently
verified full field locations:

| Complete owner offset | Field | Target |
| --- | --- | --- |
| +0x22 | DIR32 `g_AnmRenderManagerView` | 0x00491C10 |
| +0x29 | REL32 `AnmRenderManagerView::Draw` | 0x004451C0 |
| +0x43 | DIR32 same global | 0x00491C10 |
| +0x4A | REL32 same callee | 0x004451C0 |

Two serial **zero-reuse** cold builds through `build-match-unit.py` produce
independent image/PDB identities. Both compare complete owner **95/95**
exact with all four fields equal using `compare-linked-function.py`.
The full strict reports are retained at
`.analysis/gpt-web/20261103/spell-panel-draw-vms-cold{1,2}.json`.
The 10-byte adapter remains separately exact; it does not need to hide
or replace the actual parent implementation. This is real **additional
95 authored exact bytes** credited in `config/matches.csv`, unlike the
earlier nonexact candidate diagnostics. Original source identity,
full product linkage and runtime behavior are still not proven.
