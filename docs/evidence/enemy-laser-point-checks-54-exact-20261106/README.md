# EnemyLaserManagerStorageView::AccumulatePointChecks — full 54-byte exact owner

Original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
complete original owner `0x0041C880..0x0041C8B5` (54 bytes), followed by
unrelated `CC` fill through the next boundary. The original's source/
compiler-author provenance is **indeterminate**, not automatically authored:
there is no independently established original direct caller or literal VA
reference, and an outlined helper or library provenance cannot be excluded.
The existing origin-review note in `config/functions.csv` is preserved.

The maintained source in `src/EnemyLaserUpdateList.cpp` naturally models an
EAX-live-in manager, two stack argument words (`EnemyLaserVectorView *point`,
`float radius`) and a linked-list walk starting at manager +0x18. Each
node's next link is prefetched from +0x08; nodes in state 1 at +0x0C are
skipped, and other nodes invoke the actual virtual CheckPoint at vtable
+0x20. Returns the accumulated integer in EAX with `RET 8`. This exact
native codegen was recovered without fake register-binding declarations,
inline assembly, dummy padding, altered source flow or target-byte copying.

## Two separately generated original-bound complete-owner witnesses

An older isolated source-epoch probe emitted 54 bytes but only **31/54**
raw bytes matched the original, because its private receiver/prologue
register schedule differed. That is **superseded** by the registered
one-TU VC7.1 SP1 `/GL` **genuine draw-callback entry graph**. The *full*
PDB-owned 54-byte `AccumulatePointChecks` in this existing native graph
matches all **54/54 original bytes**, including every conditional local
branch, the complete virtual call and the `RET 8`. There are **zero
external relocation or global-target fields** in the complete body: the
comparison masks nothing. First native image SHA-256:
`6a9ddce4ba8b2156d961ab89a2428c5ed980f8e6eebc155d12260a866c579702`;
first PE/MAP/PDB are preserved under private
`.analysis/gpt-web/20261106-laser-point-54/first-artifact/`.

The independent canonical source reconstruction
`[units.enemy-laser-accumulate-point-checks]` then rebuilt its selected
one-TU graph from the *installed source* (not the first cached PE).
`nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit
enemy-laser-accumulate-point-checks --json` passed **54/54 exact,
0 fields** again. Second image SHA-256:
`5ef0d4a57c455c89b3c39ea3433753741994e21cd7a10fda852ccf27029adc6e`.
The canonical artifact is `build/match-linked/EnemyLaserDrawCallback/`;
private receipt is `.analysis/gpt-web/20261106-laser-point-54/canonical-replay.json`.
No full-cohort cold replay, test padding or runtime injection was performed.

**New canonical credit: one original-bound complete function / 54 bytes,
but zero additional *confirmed-authored* functions/bytes.** This also does
not establish original source identity, complete product build, runtime
correctness or portability.

Installed `src/EnemyLaserUpdateList.cpp` SHA-256: `e57daeba7a702a1ef0aec578b398a42d3fe40747e40a0d8302ed8c675d98c3cf`.
