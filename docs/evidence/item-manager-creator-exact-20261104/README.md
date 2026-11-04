# Exact ItemManager factory (0x0041AED0; 185-byte complete owner)

The original Japanese TH10 v1.00a target SHA-256 is
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Its complete `GameCreateItemManager` at `0x0041AED0..0x0041AF88`
allocates the Item manager, constructs 2,198 0x3F0-byte records, installs
registration callbacks even for a NULL allocation, and conditionally
invokes the destructor and free on initialization failure. These original
boundaries were independently reviewed; the historical source spelling
and Windows product/runtime still are not established.

## Actual source/optimizer recovery

The existing real C++ source in `src/ItemManager.cpp` uses the real
`ItemManagerView` constructor and destructor and the real registered
callbacks from `Chain.cpp`. In the pinned VC7.1 SP1 three-input
`ItemManager.cpp + Chain.cpp + AnmManager.cpp` `/GL /GS /EHs` graph, an
explicit **noinline** boundary on `ItemManagerRegisterCallbacks` preserves
the original independent 84-byte registration owner, rather than allowing
whole-program optimization to fold it into the caller. This produces the
entire natural **185-byte** factory, not only a similar-size helper.

The canonical selector `item-manager-creator-0041aed0` strictly binds
**13 original target fields**: the generated allocation EH entry, FS
registration fields, `operator new`, the actual element constructor and
destructor, the MSVC vector constructor iterator, the ItemManager global,
the real 84-byte registration entry, the actual 216-byte destructor and
`free`. The EH selector separately checks generated one-state C++
unwind/handler metadata; it is not treated as a generic unverified address.
The original factory does **not** receive credited bytes for unknown
runtime behavior outside this physical owner.

## Independent cold proofs and regression guards

Two independently regenerated linked PE/MAP/PDB images both pass **185/185
bytes exact** after all 13 address fields are bound, and simultaneously
pass the 216-byte destructor with all 21 fields. Their native image hashes
and PDB GUIDs differ. See [native-proof.json](native-proof.json), which
records both results and the installed source hash. The real source
compiled in these images is pinned and maintained; no target-code patching
or source-independent byte normalization is used.

A separate selected replay on the installed noinline change passed
`item-manager-register-callbacks-0041ad90`, `item-manager-update-callback`,
`item-record-constructor` and `item-record-destructor`, **270/270 bytes**
exact. No full repository cold replay was performed.

Reproduce the relevant group only:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-creator-0041aed0 \
  --unit item-manager-destructor-0041adf0 \
  --unit item-manager-register-callbacks-0041ad90 --json
scripts/repo-python scripts/ci.py
```

This is full target-bound owner codegen exactness, **not** proof of the
original type/TU names, assembled runnable executable, or runtime behavior.
