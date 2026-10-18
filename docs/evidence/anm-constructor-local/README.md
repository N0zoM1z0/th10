# ANM constructor: ordinary local task construction

This is an unadopted source experiment and its bounded evidence, produced at
`7da027dbbefe2cfc321349d070da9b5578bf3ad3`. Production source/configuration was
unchanged. No constructor exactness, runtime equivalence or Factory acceptance
is claimed. `evidence.json` retains the immutable inputs, complete owner and
artifact hashes, separate build graphs, actual command IDs and receipt hashes.

## Source hypothesis

The corrected candidate gives a TU-local derived `AnmRenderTask` an ordinary
constructor over a passive 36-byte `AnmChainElementView`. It initializes priority,
ownership, three callback fields, self and next/previous links; ordinary scalar
`new AnmRenderTask` is followed by callback publication and two low-bit assignments.
The unchanged caller later supplies the argument/context. Reserved flags remain
unspecified and unevaluated. No extra data member/destructor, forceinline,
noinline, calling-convention tuning, custom allocator, memset, whole-word flag
read or explicit null branch is introduced.

All twenty callback casts, context assignments, priorities, registration order
and manager member construction remain unchanged. Initialize's body/linkage and
the final device call remain unchanged. Removing the public `flags` member
spelling is still a header interface change. The initial flattened draft was
abandoned during source review before any build; it is not another measured arm.

The structural reference is the authored reconstruction at
[levzzz5154/th10-decomp, 4e4ab0b](https://github.com/levzzz5154/th10-decomp/tree/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a), specifically
[sprt_ctrl_construct.cpp](https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/core/sprtlib/sprt_ctrl_construct.cpp)
and [func_ctrl.hpp](https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/include/th10/core/func_ctrl.hpp).
This is not original game source or proof of original typing. The full required
MIT notice and attribution are in `ANM_TASK_CREATION_LICENSE.txt`; the patch
alone is not the complete transferable package. Upstream forced attributes and
unrelated code-generation controls were not imported.

## Generated-code observations

The natural `WinMain@16` diagnostic graph used AnmManager.cpp plus RandomMath.cpp,
AnmVmCreate.cpp, AnmVmId.cpp and Main.cpp. Exactly one fresh baseline and one fresh
candidate cold link completed. Full same-start manager constructor contributions
measure **1284 baseline / 2219 candidate** against a **known target extent2315**.
Target bytes were not compared in this pair. The growth935, remaining size
shortfall96, and695 changed overlapping positions do not measure exactness.
Linkage/ordinary difference classification is incomplete.

Two named candidate helpers have independently attributable contributions of71
and32 bytes. Their bodies were not separately extracted, decoded or analyzed;
whole-image hash reads may include those bytes. `/opt:noref` can retain unused
helpers. Missing baseline names do not establish inlining, and helper sizes are
not added to constructor coverage.

A subsequent complete2219 linear call census observed41 supported direct calls:
20 to the identified diagnostic scalar-new anchor and21 other destinations.
There is one indirect near call, zero unsupported call forms, and zero direct
calls to either known helper entry or interior. The independently observed
complete2315 target has20 scalar-new plus23 other direct calls and one indirect
near call. These are separate inventories, not instruction correspondence.
They prove no argument values, successful allocations, object type, registration
pairing, ABI, flags, lifetime or inlining. Indirect targets remain unresolved.

The generated census initially failed a verifier gate. Follow-up metadata
localized the issue to four-byte raw containment of a diagnostic anchor; all
other address predicates passed. The reviewed correction removed only that raw
storage condition for address-only identity. Dependency, symbol/alias,
object/path, map identity, nonnegative offset, four-byte virtual containment and
nonexecuting-section checks remain. Executable owner raw bounds did not change.
No anchor contents were separately extracted or interpreted; whole-image hash
and PE-parser reads may include those bytes. No BSS or runtime storage claim
follows. Earlier
failed/diagnostic receipts are retained as provenance, not candidate failures.

## Bounded target lifetime question

The source already contains `AnmVmView vmPool[0x1000]`, a scalar `primaryVm`,
and `drawLayerSentinels[20]`, with a nontrivial VM destructor. That source shape
motivates a two-array lifetime question independently of the call-count gap.
A separate full2315 target linear census observed43 supported direct calls:
two to the independently bound canonical array-constructor iterator entry, zero
to the array-destructor iterator entry, and41 other; one indirect near call and
zero unsupported forms. Current source metadata separately revalidated the two
canonical archive entry bindings. Iterator bodies were not separately extracted,
decoded or analyzed.

These two observed calls do not identify their arguments, array dimensions,
callback types or corresponding source members. Zero direct destructor-iterator
entries here does not exclude inlined, indirect or separately owned EH cleanup.
The generated candidate counterpart was **not performed**. The observation does
not prove candidate omissions, explain the size/call-count difference, establish
cleanup/runtime equivalence or justify a source change.

## Initialize273: code evidence and operational outcome

A separate candidate guard used the original four-TU ExecuteScript graph and
canonical `anm-vm-initialize-private` unit. The worker recorded full **273/273 raw
exact** code, with all273 bytes decoded, empty linkages and data spans. A bounded
read-only recovery verified that existing report, image hash and independent
full same-start ownership. It repeated neither the build nor comparison.

The original guard nevertheless **failed overall**: ordinary client cleanup
completed, but detached shared-runtime availability remained uncertain, with
pending availability1 and incomplete sampling. Recovery exit0 is successful
evidence verification, not end-to-end guard acceptance or a cleanup repair.
Recovery rechecked current decoder disk pins without recreating the terminated
worker's loaded-module binding. No adoption authority follows.

The natural baseline/candidate pair separately completed its monitored operation,
with eight acknowledged shared Wine runtime residuals per arm. This is neither
a zero-process claim nor eight leaked compiler children. No inner-PE attestation
or shared-runtime termination is claimed. Historical claims remain consumed;
never delete/reset them or rerun their once-only packets.

## Remaining source/runtime limits

- Size/offset assertions do not prove bitfield order, field identity, callback
  compatibility or global aliasing. The mask0x2 effect is upstream-inspired;
  it was not established by native flags evidence.
- Named bitfield assignments may compile to containing-word read/modify/write.
  The model never evaluates reserved bits and proves no portable preservation
  of their physical representation.
- Ordinary scalar-new null/throw behavior, failed registration/exception cleanup,
  external destruction/deallocation pairing and lifetime remain unresolved.
- The inherited callback typedef/casts and changed public header require shared
  caller review. ASCII330 and unchanged-input Chain guards are outside this
  Initialize checkpoint; complete guard closure has not been established.

## Source-only reproduction

From a repository containing the exact baseline inputs, use the repository's
selected Python with:

```sh
python -B docs/evidence/anm-constructor-local/reproduce_source.py --repo .
python -B docs/evidence/anm-constructor-local/test_source.py --repo .
```

Both scripts accept an explicit source root as an alternative. Imports have no
file-reading or argument-parsing side effects. Reconstruction is in memory and
checks the two baseline hashes, two candidate hashes, exact diff and raw EOLs.
The tests cover source invariants and a deliberately limited defined-field model;
they do not compile C++, load a decoder, read target images or test runtime safety.

Native reproduction is a separate reviewed operation using the graph/profile,
source inventory, canonical tools and toolchain hashes in `evidence.json`, fresh
output roots and appropriate bounded cleanup controls. The natural graph is a
diagnostic graph, not a new canonical unit. This archive intentionally contains
no native artifacts or operational probe framework and authorizes no new build.
