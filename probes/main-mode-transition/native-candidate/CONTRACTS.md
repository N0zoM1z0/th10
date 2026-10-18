# Conditional natural native integration proposal

Status: the one conditional native compiler experiment compiled successfully. Corrected read-only attribution of the same artifacts established all four owner extents; none matches its target size. No production application has occurred. The helper interfaces remain explicitly conditional source hypotheses.

## Source ownership

Define `int MainOpaqueUpdate4218D0(MainSupervisorView *supervisor)` in existing Main.cpp, reached through the genuine MainInputCalcCallback already present in the a003 overlay. The definition is ordinary C++ with no explicit register calling convention, adapter, virtual hook, dummy parameter, or artificial retention wrapper. It layers onto Main.cpp SHA 9035190b5eff8665fec60ef1cbb40ad4850a13826457c5025579878819ea1a89 and Main.hpp SHA 973c294a3ae9142a701a1e591b1055650039e4881ee7ef3f72047892453a1fad from the reconstructed input checkpoint. Input.cpp and the canonical build graph remain separate unchanged inputs.

## Existing and refined data views

Keep existing `nextGameMode` at +390, despite the public reference's different terminology. Add neutral dwords `transitionState38c`, `transitionPrevious394`, `transitionVariant39c`, `transitionResult768`, and `transitionColor780`. The result field is raw unsigned 32-bit storage; a pointer type is not proved. Split only opaque spans and extend this partial view through +784, with exact field-width and offset assertions. The existing double at +778 is preserved; no claim about the total allocated object extent follows from the partial view's C++ size.

Use existing `criticalSections[5]` and `criticalSectionLockCounts[5]`, with guards confirming +6C4 and +6F9. Enter/Leave call the ordinary SDK imports on that field. Runtime initialization, ownership, and valid private-stack separation remain premises.

Use maintained declarations `g_GameStartupMode`, `g_GameComparison474C80`, and `g_GameStageUpdate`. The proposed `GameStageUpdateView g_MainStageRecords474788[]` is a typed source hypothesis from the proved base/0x30 stride and existing destination pointer type. Its extent and valid index domain are not recovered. Ordinary array indexing requires a valid-index runtime premise; the target's wider raw32 modular theorem does not alone establish that premise. The global at 491C00 is neutral `g_MainTransitionState491C00`.

## Helper interface hypotheses

- 41FD00: `unsigned int MainOpaque41FD00()` models the observed raw EAX result. No pointer result or no-argument ABI is yet proved.
- 4203F0, 421C50, 42CD50, 418150, 40B9D0, 42CDB0, 40B940: ordinary `void MainOpaqueADDRESS()` hypotheses. Their results are not used by the candidate; that does not prove native return type or lack of consumed arguments.

- 4180E0: `void __stdcall MainOpaque4180E0(unsigned int initializationMode)` is a meaningful one-dword interface hypothesis. Actual 107-byte helper decoding establishes RET 4, while consumed-entry-role recovery remains incomplete. The caller proves values 0/0/snapshot/1/0 for modes 7/10/11/12/13. Mode 11 snapshots `g_GameManager->initializationMode` before optional cleanup and retains that value across the helper. This cannot be replaced with a constant or a post-cleanup reread.

The pinned public semantic header uses zero-argument abstract hooks (and a pointer-returning creation hook), but it is not native type authority. Maintained ordinary source has no matching named helper declarations. Consequently these declarations are a concrete draft for review, not accepted signatures. No unchanged ambient ECX/EAX value is treated as an argument.

Bounded caller and 69/107 owner checks are complete for this experiment. The 69-byte owner returns with RET 0 but does not prove an empty signature. Its raw uint32 no-argument declaration remains the minimal baseline hypothesis. No further helper-body recursion is required for the authorized conditional measurement.

## Shared GameManager declaration

Extract the exact existing GameManager declaration and its aliases into GameManager.hpp, including Player.hpp for PlayerTimerView. GameManager.cpp includes that header and drops only the duplicate declaration, aliases and extern. Main.cpp reads the existing g_GameManager symbol. No second pointer alias or accessor body is added. The maintained declaration binds g_GameManager to 477810 and initializationMode to +5C; the independently verified caller load now matches those roles.

`check_shared_layout.py --baseline-root PATH --overlay-tree RECONSTRUCTED` verifies the raw extraction against pinned GameManager.cpp/Player.hpp files, unchanged struct bytes, CRLF transport for header/GameManager.cpp and LF for Main.cpp. It compiles both old and shared declarations with g++ -m32 syntax-only, verifies all field offsets/size and rejects two layout mutations. Calling-convention keywords are stubbed solely for this host layout check; it proves no MSVC ABI. GameManager.cpp is an overlay for coherent declaration ownership and is not an extra /GL support TU in this experiment.

## Semantic evidence and remaining gates

The body follows independently recovered mode partitions, fresh post-Enter snapshots, 42 fixed event-precedence pairs, and five prefix-load ordering facts. It follows the corrected mode 4 helper condition, preserves opaque mutation across helpers and Leave, and retains failure return 4 without an invented unlock. The host candidate passed 666 scenarios and rejected 15 compiled mutants before this natural-source translation.

The mode 10 remaining instruction category is a fixed register/EFLAGS shift. This does not prove flags dead or complete native source equivalence. Full target effects, frame restoration, helper argument/ABI closure, actual runtime object/table domains, and exact native comparison remain open. A reviewed compiler experiment may measure this candidate honestly without accepting those claims.

Actual argument evidence: a50c20a783de48b08159cc2f4c6be82d established four constant arguments; a5c977ce9d6e4c01bf72e7923ded5802 closed the mode-11 field snapshot and its preservation across optional cleanup. The mode-11 snapshot is placed before cleanup and independent supervisor stores under the normal distinct-object/ordinary-field source domain. No cross-object aliasing or asynchronous mutation is modeled. All five parameter values are meaningful source data rather than register-retention devices.

Reproduction: run the shared-layout check above and `python ../host-candidate/run_tests.py`. The separate host candidate passes 666 cases and 15 compiled source mutations. The native packet must retain the a003 Main.cpp + Input.cpp /GL graph and /GS policy; production integration and collateral native validation remain deferred.
