# Replay-controller cleanup pilot, 2026-10-25

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Started at 5c9241c with maintained files clean and protected nine files intact.
Fresh target/toolchain/tracking/status and Factory-native Ghidra checks passed.

## Complete owner, not an exact match

Reviewed authored owner 422220..422356 is 311 bytes, including its three-byte
internal LEA alignment at +269. Existing actual callers are GameManagerCleanup
at 417E21 and controller factory at 42238D. Authored source was absent; the
existing GameManager.cpp declaration/call supplies the natural stdcall interface.

Target and first candidate each have a complete 311-byte extent, but the
candidate structurally rejects: its actual
ReplayManager destructor receives EAX instead of the target stack receiver.
That extra instruction byte shifts three fields and several local destinations;
a two-byte alignment instruction happens to restore the final extent. Equal
size alone proves nothing.

One target-grounded dependency correction restores the real stack call,
all 18 fields, all 16 fixed locals and the original three-byte alignment. The
complete 311-byte owner still has **four ordinary byte differences**, at
+2F/+31/+63/+65: the compiler reverses two equivalent ECX/EDX setup instructions
before each UnregisterElem call. This is **nonexact**, not 307 credited bytes.
No maintained source, canonical recipe, exact ledger or metric changed.
Totals remain 418 authored exact owners, 76347/273658 owned bytes.

Park this proposal. Reopen only with independent source/dataflow or actual
context evidence that predicts those remaining call-setup differences; do not
permute registers, declarations, spelling, padding or compiler flags.

## Source and dependency evidence

The partial controller view uses existing GameChainOwnerView, ReplayManager,
AnmVmIdView and ANM node/VM types. It asserts VM ID+1DC and 25 pointer slots+1EC.
It does not claim the original class, full allocation layout or runtime closure.

Both chains and their current global chain receiver are captured before their
null-gated Enter/increment/Unregister/Leave/decrement sequences. The second pair
is freshly loaded after the first operation. The 25 replay pointers are captured,
null-tested and naturally scalar-deleted without slot clears. The controller
itself is not released here. Existing FindVm inlines its primary/secondary
lookup; the lexical tree-mark block applies 04000000 to the found VM and, only
for a root, every following child. Owner VM ID and global 477830 always clear.
Existing MarkVmForDeletion attributes/inline-depth settings are untouched.

The graph is the existing GameManagerCleanup diagnostic entry plus
Hint.cpp, Chain.cpp, AnmManager.cpp, FileSystem.cpp, GameStatistics.cpp,
Lzss.cpp and the actual ReplayManager.cpp. This is diagnostic context, not a
claim about the original production TU partition. All primary/other-support
flags remain the existing LTCG_PROFILE.

Raw target destructor 367 has an FS:[0] exception frame, saved stack receiver,
array-destruction tail 4525FF and RET4. The preserved canonical ReplayDestroy
graph's actual destructor 237 also has an exception frame and RET4; the first
pilot's destructor 199 has neither the frame nor stack receiver. Therefore the
second model changes **only ReplayManager's support profile** to its existing
canonical /GL /GS /EHs. It naturally recovers stack passing without an ABI
annotation. Destructor237 remains nonexact; no destructor/runtime credit
follows. This targeted EH-context correction does not establish original flags.

## Exhaustive comparison and verification

Target manifest: 14 DIR32 and 4 REL32 fields. Supervisor fields bind real base
491C28 with addends 64C and 6F4, not nearest-address aliases. Candidate fields
independently resolve exact MAP symbol bases plus these addends; all 18 unique
offsets are consumed. The stripped target PE relocation list is not a zero-field
claim. Both full bodies and 16 local destinations are retained in ignored
inventories; tracked inventories omit raw instruction dumps.

Independent source/raw-byte/field/local review confirmed the complete semantics,
four remaining ModRM bytes and correct proposal-only scope. First-image guards
FindVm 73, MarkVm 66 and Unregister 114 pass 253/all2; Destroy 19/18 rejects.
EH-image guards add exact Destroy 18, totaling 271/all4. These are existing
same-image guard checks, not new exact credit.

Both native jobs used one CPU, nice 10, 240-second bounds and frozen inputs.
First run exits 0. The EH run's original exit 1 is retained: linking succeeded,
then JSON serialization rejected PosixPath support-profile dictionary keys.
Retained PE/MAP/PDB were recovered and compared without rebuilding or rewriting
that failure. Initial diagnostic guard setup errors (COFF linkage conversion
and a stale support-profile override) are separately retained; corrected
same-image supplements provide the guard results above.

Both 124-input freezes pass, including maintained source and protected 9.
All 150 prior statistics-initializer artifacts and 56 loader artifacts match
their existing retained hashes. No original native proof was overwritten.
Focused toolchain/tracking/progress/whitespace checks cover this documentation
checkpoint. No new full-CI, canonical cold, target-runtime or product claim.

## Reproduce or inspect

`prepare.py` checks the maintained GameManager.cpp hash and constructs the
ignored proposal without replacing a different candidate. `first.py` and
`replay-eh.py` retain the actual distinct build recipes; existing output
directories fail closed. The latter deliberately retains its historical
post-link metadata serialization failure. `recover.py` inspects those retained
artifacts without native execution. `compare-v2.py first` and
`compare-v2.py replay-eh` reproduce the full comparison and three peer checks.
Run scripts via `scripts/repo-python`; native runs require serial one-CPU
nice10 execution and bounded logs. Chain guard manifests/results remain in
the 71-file retained artifact inventory; original and corrected failures stay
separate. All proposals are evidence only, not source-present ledger entries.
