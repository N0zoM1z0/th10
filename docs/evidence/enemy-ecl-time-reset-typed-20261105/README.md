# TH10 Enemy ECL active-context binary32 time reset across lifecycle

Target: pinned Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The verified full Enemy layout puts `activeEclContext` at full-object +4;
its current ECL context starts with a binary32 `currentTime` at offset +0,
as independently established by the generic ECL VM interpreter. The
maintained `EnemyEclContextView` already contains an exact-offset union of
`int value00` and `float currentTime`. Five original lifecycle operations
still wrote the anonymous integer word with `= 0` rather than the semantic
frame time. `src/Enemy.cpp` now writes **`currentTime = 0.0f`** in:

- Named-subroutine installation (`EnemyEclHostBaseView::SetSubroutine`).
- Full ECL host reset (`ResetEclState`).
- Embedded-context initialization and active script installation in the
  actual 672-byte generated Enemy full-object constructor.
- Database host creation (`EclVmScriptDatabase::CreateHost`).

This is exactly the same four zero bits, with no changed field location,
new synchronization or guessed original variable naming; the accepted
original is still the only oracle. Actual VC7.1 MSVC `/GL` compile and PDB
owner comparison against the current `Enemy.cpp`/`EclVm.cpp` source epoch
preserves the entire `SetSubroutine` 30-byte candidate, `ResetEclState`
71-byte candidate, Enemy full constructor 672-byte candidate, `CreateHost`
89-byte candidate and ECL VM `Run` 7,020-byte candidate, with **zero
ordinary candidate changes and identical linked field geometry**.

A separate genuinely **fresh all-11-TU** selected `/GL` ECL graph proves the
current header/source can be linked with these changes: full Enemy ECL
`DispatchEclInstruction` PDB size 14,352 bytes/716 candidate fields,
`EnemyBeginSpell` 2,432 bytes/108 fields, Enemy constructor 672 bytes/5
fields and generic `Run` 7,020 bytes/189 fields. **The original dispatcher
is 14,416 bytes and the original constructor is 673 bytes; these are still
NONEXACT**. Candidate-to-candidate agreement does not grant original-target
exactness.

Six explicitly selected original-target canonical exact unit cold replays
passed on the changed source: `enemy-ecl-reset-state` 71,
`enemy-ecl-host-constructor` 21, `enemy-ecl-context-constructor` 15,
`enemy-runtime-constructor` 145, `enemy-ecl-resource-constructor` 34,
`ecl-vm-host-run` 146, totaling **432/432 previously credited bytes**.
There is **zero new exact credit**. A separate 30-byte private ESI/ECX
`SetSubroutine` original has different register placement from the
candidate and is not misreported as exact.

Complete owner/field and source-hash receipts are in `native-result.json`;
private native PE/MAP/PDB under
`build/gpt-web/20261105-enemy-time-lifecycle/`. No historical all-unit cold
replay, full reconstructed game link or runtime-equivalence claim.
