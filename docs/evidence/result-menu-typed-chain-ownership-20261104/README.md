# TH10 ResultMenu: recover genuine ChainElem ownership at +8/+12

Original target: Japanese TH10 v1.00a SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The actual ResultMenuView at src/ResultMenu.hpp previously represented
its two native callback nodes at +0x08 and +0x0C as a separate, untyped
unsigned char unknown008[8], forcing repeated pointer reinterpretations
in the creation, registration, retirement and complete teardown owners.
The real target and canonical Chain.cpp implementations independently
establish two 4-byte th10::ChainElem* values at those exact offsets.
Those fields are now updateTask and drawTask, with explicit independent
offsetof asserts. The full owner retains sizeof 0x2C8, native timer at
+0x10 and all other field offsets.

src/ResultMenuLifecycle.cpp now registers the real callback elements
through the named typed pointers, while the independent cleanup source
src/ResultMenuCleanup.cpp uses the very same objects to unregister under
the native critical-section procedure. The nonexact smaller
ResultMenuMarkTasks422200 additionally names these fields when marking
both callback nodes, with no inserted free or removal.

## Actual selected VC7.1 complete-owner verification

Four already accepted functions were cold rebuilt from current source
using their genuine pinned VC7.1 SP1 /GL /GS /EHs graph, with the real
Chain.cpp, AnmManager.cpp, ReplayManager.cpp and ResultMenuCleanup.cpp
as support (not stale objects from before the header change):

- ResultMenuView constructor 101/101 bytes exact
- ResultMenuView Initialize 142/142 exact
- GameCreateController 67/67 exact
- GameCleanupController422220 311/311 exact

Total 621/621 exact bytes and all their original target address fields
verified. The independently reviewed 28-byte physical
ResultMenuMarkTasks422200 remains NONEXACT: the current typed source
still produces a 26-byte candidate, with ECX rather than original private
EAX receiver. This change does not register a new match or claim its
first 26 bytes are exact. The actual PDB contribution and source SHA pins
are in native-evidence.json.

This is substantive typed ownership restoration spanning the complete
constructor, update/draw task scheduling, task retirement and cleanup.
No target-byte edits, fake register ABI, partial-credit additions,
historical TU inference, complete executable or runtime claim.
No full historical cold replay was run.
