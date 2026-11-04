# TH10 Chain::AllocElem leaf: restore actual callback member ownership

Original target: Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The native th10::Chain::AllocElem at 0x00449ED0 has a complete 74-byte
function and one operator-new call field.

## Substantive C++ recovery

This source-backed allocator has a real 0x24-byte ChainElem class, but
previously still wrote its three registered callbacks through raw
byte-address additions of +0x0C, +0x10 and +0x08 to the allocated element.
Those are exactly the existing ChainElem::addedCallback,
deletedCallback and callback members. The maintained Chain.cpp now
sets these members directly, retaining the original write order, null
values and pointer width. It also includes compile-time native-offset
assertions for all three callback fields.

This is not a new shadow type or fabricated function ABI. It reuses the
very same source model that ItemManager and ResultMenu now use to register
and tear down frame callbacks. It deliberately retains the original
null-allocation behavior, the existing flags bit update and the
compiler-visible constructor ordering, instead of introducing unrelated
safety changes while matching original code.

## Actual whole-function validation, including negative exact result

A fresh pinned MSVC 7.1 SP1 /GL /GS /EHs native image was compiled
from the edited actual Chain.cpp. Its Chain::AllocElem PDB owner has
the complete 74-byte target extent, exactly one native linked
REL32 operator-new field at +0x03, and the same linked-address geometry
as the previous true ItemManagerDestructor producer image.

After normalizing only that matched candidate field, every one of the
74 ordinary candidate positions is identical to the prior compiled
source epoch. The physical original target still differs at 22
ordinary bytes. Those 22 include the EDX/ECX zero/flag register
allocation and post-allocation callback-store order; this remains
NONEXACT, not a codegen-accepted allocation function.
See native-result.json for the source SHA, both actual image origins
and complete 74-byte comparison evidence.

Two batches of specific existing exact codegen units were cold-replayed
from the installed source:

- ItemManager callback registration 84/84
- ResultMenu initialization 142/142
- Chain calc registration 43/43
- Popup owner constructor 134/134

All 403/403 bytes match independently bound original target fields.
See selected-exact-guards.json. This was not a full historical cold
replay. No new exact-unit credit, original TU/spelling, game-product
build or runtime behavior claim is made.

Controlled experiments also tested six natural constructor-barrier and
callback-store-order variants. They preserved the 74-byte owner with
22 differences or shrunk it to 69 bytes and changed the independent
59-byte Chain sentinel constructor; none was accepted. Those
experimental source changes were restored before this checkpoint.
