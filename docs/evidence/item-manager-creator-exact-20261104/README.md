# TH10 ItemManager factory @0x0041AED0: complete 185-byte exact owner

Target is the attested original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete target `GameCreateItemManager` spans **185 bytes** at
`0x0041AED0..0x0041AF88`. It allocates 0x21CEC0 bytes, constructs
**0x896 = 2,198** records at stride 0x3F0, clears and publishes the
manager, calls the independent callback-registration owner, and on an
initialization failure invokes the manager destructor and `free` before
returning NULL. It owns a genuine one-allocation EH cleanup protocol.

## Correct cause of the earlier large codegen mismatch

The maintained C++ originally called `ItemManagerRegisterCallbacks` without
an explicit out-of-line boundary. In the genuine three-source /GL /GS /EHs
producer (primary `src/ItemManager.cpp`, supports `src/Chain.cpp` and
`src/AnmManager.cpp`, real `GameCreateItemManager` link entry), the optimizer
inlined callback registration and folded away the target's real failed-result
branch because the retained callback registration returns constant zero.
This produced **225** source PDB bytes, despite the separate retained
**84/84 exact** registration function.

Natural `__declspec(noinline)` on the **actual** `ItemManagerRegisterCallbacks`
source declaration restores the independent call at original +0x79, the
actual branch after the result, and destructor/free cleanup. It is a
source-level original-boundary constraint, not an invented register ABI,
inline assembly, forced return value, or byte patch. The genuine parent now
emits the full **185/185** bytes while the complete **84-byte registration**
and **216-byte destructor** retain their previously recovered codegen.

## Complete native target-bound acceptance

Canonical unit: `item-manager-creator-0041aed0` in `config/match-units.toml`.
Every original address field is independently bound in the strict linked-PE
comparator, not merely masked by the diagnostic ordinary-byte comparison:

| Offset | Kind | Actual original target |
| --- | --- | --- |
| `+0x03` | DIR32 | `__ehhandler$GameCreateItemManager` @ `0x0046580B` |
| `+0x09/+0x11/+0x9F/+0xB1` | DIR32 | `__except_list` FS anchor 0 |
| `+0x1D` | REL32 | scalar new @ `0x00452493` |
| `+0x38` | DIR32 | `ItemRecordView` destructor @ `0x0041AD60` |
| `+0x3D` | DIR32 | `ItemRecordView` constructor @ `0x0041ACF0` |
| `+0x50` | REL32 | vector constructor iterator @ `0x0045252D` |
| `+0x64` | DIR32 | `g_ItemManager` @ `0x00477818` |
| `+0x79` | REL32 | genuine registration owner @ `0x0041AD90` |
| `+0x88` | REL32 | genuine 216-byte destructor @ `0x0041ADF0` |
| `+0x8E` | REL32 | scalar free @ `0x004524A1` |

The item allocator EH entry is not an arbitrary approved relocation: the
existing `allocation_eh_handler` exact-address selector checks the actual
unique generated MAP static, complete 21-byte PDB-owned helper, CxxFrameHandler
jump, one-state 28-byte FuncInfo/unwind metadata, correct saved-pointer frame
slot, and scalar deleting cleanup in the original and linked native candidate.
It grants *metadata/address binding only*, not executable runtime semantics or
codegen credit for the handler itself.

Two independent selected cold native PE/MAP/PDB builds pass all **185/185**
bytes and **13/13** bound fields. Their independently generated whole-image
SHA-256 digests are:

- Cold 1: `40dafdd721974e9162b980409c991e2c6d1aed6f9c3d9da02d465a86965dd2ee`
- Cold 2: `e3710ec85c9707a2694350c3f4c5b9796900bf3b30401d23dba275bccc1b992c`

Both builds simultaneously re-prove the separately accepted **216/216**
ItemManager destructor with its 21 strict fields. The changed-source selected
existing 84-byte registration, 104-byte record constructor and 34-byte record
destructor also cold-pass independently (**222/222**). No full historical cold
replay was used. Proof JSONs and actual native artifacts are retained in
ignored `.analysis/gpt-web/20261104-live/item-creator-cold{1,2}.json`
and `build/match-linked/ItemManagerDestructor/`.

For a bounded recheck:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-creator-0041aed0 \
  --unit item-manager-destructor-0041adf0 --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-register-callbacks-0041ad90 \
  --unit item-record-constructor --unit item-record-destructor --json
scripts/repo-python scripts/ci.py
```

This is exact VC7.1 source codegen, not historical TU/symbol provenance, a
complete reconstructed Windows game executable, runtime verification or
portability.
