# ECL SCPT file-slot counter: original i386 word wrap

Original reference: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmScriptDatabase::AddScriptData @0x00450220` registers a SCPT file,
merges the named-script table and advances its database `fileCount`. The
original uses a native 32-bit `LEA index+1` followed by one 32-bit store to
`[this+4]`. The previous C++ `fileCount++` had signed-overflow undefined
behavior at `INT_MAX`, unlike the original i386 wrap. The maintained source
retains the original index for both `files[result]` and the returned result,
then explicitly advances the `unsigned int` representation by one and stores
that same 32-bit result back to the signed member. No range validation,
array-bound guarantee, or additional error path is inferred.

## Fresh current-source native comparison

A new original-bound two-TU `/GL` VC7.1 SP1 source/link context compiles
`EclVm.cpp` and `Enemy.cpp` with the actual `EnemyRuntimeUpdate` entry. The
complete current `AddScriptData` contribution stays **589/589 bytes** in
extent, **510/589** ordinary original bytes match, and both independent
external call-field positions retain their target bindings:
`malloc` at `+0x6C` and `free` at `+0x101`. The function is **NONEXACT**,
not newly accepted. A separately original-bound comparison of the same
current image's **7,020-byte `EclVmContext::Run`** retains **6,997/7,020**
ordinary matching bytes and **189** bound fields, including its dispatch
jump-table destinations. It too remains **NONEXACT**.

Four tested natural file-slot initialization alternatives did not improve
`AddScriptData`: slot-first and nonvolatile slot moved the `malloc` call;
slot-order kept the baseline 510-byte ordinary agreement; collapsing the two
header-validation arms increased the function to 605 bytes. All probe variants
were restored, not adopted. An independent typed-score-record exploration
at the same checkpoint likewise rejected its three regressing variants.

Selected affected canonical cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-find-subroutine \
  --unit ecl-vm-read-int-value \
  --unit ecl-vm-read-float-value --json
```

The selected existing exact functions passed **431/431 original bytes**;
seven lightweight CI checks and whitespace checks passed. There is **no new
exactness credit**, no whole-game cold build, and no runtime or provenance
claim. The original target identity, current source digest, PE/MAP/PDB and
comparison are preserved in ignored artifacts under
`.analysis/gpt-web/20261107-ecl-slot/` and
`build/gpt-web/20261106-ecl-count-reference/gptweb-20261107-filecount-wrap/`.
