# ECL VM stack scalar payload: one typed 32-bit word across all access paths

Target is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The existing ECL implementation already models the 4-byte stack *type slot*
and the independent saved frame word. However, variable reads and writable
operand resolution still cast the underlying stack byte array independently
as `int*`, `unsigned int*`, and `float*`. The 32-bit `EclVmScalar` union was
already used by the ECL interpreter for float/int bit-preserving dispatch.
The maintained source now uses this same scalar view for **seven** stack
payload read/address sites in ReadIntValue, ReadInt, ReadFloatValue,
ReadFloat, ResolveInt and ResolveFloat, with an explicit `sizeof == 4`
native-layout assertion. This changes neither serialized type tags nor the
actual offset calculation and does not assert an original C++ union name.

## Whole boss and selected exact leaf checks

- Fresh pinned VC7.1 SP1 two-source `/GL` `EclVm.cpp` + `Enemy.cpp`
  native link: complete `EclVmContext::Run` PDB extent **7,020/7,020**,
  **189** independently original-bound fields (including local table
  destinations), **6,997/7,020** ordinary bytes matching the original.
  The same 23 ordinary-byte differences remain; **NONEXACT**.
- Selected native original-target replay for **all six affected value/lvalue
  accessors** (`ecl-vm-read-int`, `read-float`, `read-int-value`,
  `read-float-value`, `resolve-int`, `resolve-float`): **773/773 exact**.
  No full historical replay was performed.
- Repo's seven lightweight CI checks and `git diff --check` pass.

Private evidence and reproducible script:
`build/gpt-web/20261105-ecl-stack-scalar/current/native-result.json`,
`.analysis/gpt-web/20261105-ecl-stack-scalar/verify.py`, and
`.analysis/gpt-web/20261105-ecl-stack-scalar/selected-leaves.json`.

Separate speculative FORMAT stack-home trials were attempted under
`.analysis/gpt-web/20261105-ecl-format-stack/` and **rejected**, with
production ECL source restored after each. Two declaration-order trials kept
6,997/7,020 without improvement. Two aggregate-state trials changed the
complete owner to 6,928 or 7,028 bytes and failed the original-bound
contract. None grants codegen credit or justifies adopting a guessed
original local struct. No native product, runtime or portability claim.
