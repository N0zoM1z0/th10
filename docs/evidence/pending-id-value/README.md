# Explicit VM-ID values close pending3/pending6 (2026-10-21)

Two complete authored functions are now canonical exact: ResultMenuPendingThree
at40C4D0/65 and ResultMenuPendingSix at4243F0/65. This adds **130 exact bytes**.
The authored bar advances to396 functions,72283/273576 bytes, **26.42% provisional**.
All three original large functions remain nonexact.

Both helpers already received their input pointers in EAX. The prior source
copied the existing ID object into FindVm; its loaded value occupied ECX.
The target uses MOV EAX,[EAX] then PUSH EAX. Explicit construction through the
existing `AnmVmIdView(id->value)` constructor recovers those two bytes, while
preserving the complete65-byte body, branch layout and read-only ID behavior.
Only two expressions in FrontEnd.cpp change. Signatures, headers, calling
conventions, entry and compiler/linker profiles are unchanged. This observation
does not identify original source spelling, class/TU ownership or annotations.

Fresh direct Ghidra checks the original Japanese executable and complete mapped
text before/after read-only/noanalysis review, with rollback, CPU1 and768 MiB.
The target65-byte pending2/3/6 bodies differ only in their constants after their
FindVm calls are independently accounted for. Both new units bind DIR32+04 to
manager491C10 and REL32+0A to FindVm4491C0. See [target facts](target-facts.json).

One private one-TU probe first reproduces both complete owners with all fields.
Then two independent full two-TU cold builds each recompile FrontEnd.cpp and
AnmManager.cpp from19 recursively frozen project inputs. The existing menu-row
callback entry and `/GL /GS /EHs` profile are retained. Each image strictly
replays both new65-byte helpers and the existing404-byte name-entry row guard:
**three complete owners534/all20**. All three native body hashes reproduce
across the two colds. No cropped extent, masked ordinary byte, ABI annotation,
assembly or source padding is introduced.

An additional fresh five-TU link reuses the new canonical FrontEnd object and
four unchanged hash-verified support objects from the prior actual caller graph.
It compiles no further game source. All four complete owners599/all22 agree,
including the existing65-byte Player pending2 helper. The Player direct call,
biased shot iterator and ID+44 identity are rechecked; a copied native CALL to
the same-sized pending6 body is rejected. The full Player caller remains
738/876 and receives no equality claim. Diagnostic anchors still prevent these
images from being production executables.

Only the shared artifact's three canonical units are replayed. All1235 old
recipes remain unchanged; two recipes are added. The current large graph's33
inputs do not include FrontEnd.cpp, and all33 inputs/16 native artifacts plus
the canonical large carrier stay unchanged. No new large compile or new
ordinary-byte comparison is claimed. Earlier ANM9964, Run7020/27 and
Enemy14464/14416 measurements retain their original scopes.

Two other prepared value-construction variants were never compiled and are
removed. The exploratory positive native directory is superseded by the two
canonical colds and removed. Total measured cleanup is1,137,143 bytes. The first
cold, final canonical artifact, additional five-TU image and compact producer/
source/result receipts remain. Independent D3D work is untouched.

[Native proof](native-summary.json) pins the inputs, recipes, complete cold
results, additional link and unchanged large scope.
[Validation](validation-summary.json) records final gates and actual CI scope.
Exactness proves bounded compiler reproduction, not product/runtime, semantic
or portability closure.

```sh
nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit result-menu-pending-three
scripts/repo-python scripts/compare-linked-function.py --unit result-menu-pending-three --json
scripts/repo-python scripts/compare-linked-function.py --unit result-menu-pending-six --json
scripts/repo-python scripts/compare-linked-function.py --unit result-menu-draw-name-entry --json
```

Private recovery: `.analysis/gpt-6.1-sol/pending-id-value-20261021/RECOVERY.md`.
