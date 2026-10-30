# Stage animation reset interfaces — 2026-10-31

StageAnimationSlots.cpp completes the two actual GameManager dependencies at
404450 and404530. **GameEnterMenuState119/all2 is exact.** GamePrepareAnimationSlots
remains184/214, nonexact. This adds119 authored exact bytes and two maintained
source mappings; the reviewed boundaries and denominator are unchanged.

## Target-backed complete source

The reset404450..404525 receives its owner on the stack and has two RET4 exits.
It ORs2 into three callback-node flags at owner+8/+C/+2A40, assigns object flags1,
walks the signed object count and variable-size primitive headers, initializes
objectVms+17C using resource+178 and signed script+4, writes the truncated running
VM index at primitive+6, and resets instruction+4C from scriptBase+1C. Nonpositive
object count still performs that final reset. Existing StageScriptView/ChainElem
members and local offset assertions supply the observed layout. No safe-null
branches, checks or altered loop bounds are added.

The transition404530..4045A6 receives the owner in ESI and ends in plain RET.
It calls the actual StageCreateTransitionEffect43C8B0 with priority15, kind2,
frames30 and three zero values. It then sets transitionTimer+2A1C to30 and ORs2
into flags+2A18. The observed menu alias uses the same-layout stage view; original
class, storage and TU ownership remain unknown. One scoped inline_depth boundary
retains the real factory call; there are no dummy operands or register controls.

Read-only, freshly attested local Ghidra and independent complete target decoding
agree. The two owners have respectively one/two address fields, five/one local
branches and two/one return exits. GameManagerUpdateBody has one actual call to
each body; both old placeholder anchors disappear in the native graph.

## Actual dependency that closes the transition

The new reset's first complete21-input comparison gives184/214 and leaves
EnemyBeginSpell2424/2432, embedded initializer200/202 and binder197/203. Selecting
that actual reset as the link entry gives204/214 with the target stack/RET4
surface, but leaves its register/index preservation unlike. That context is not
retained. These results agree with the unresolved coupling found in the older
[private stage-pair experiment](../game-stage-pair-native-20261025/README.md),
whose source was never adopted.

The transition plus actual TransitionEffect.cpp gives118/119; its factory is
81/82. The factory receives kind in EBX and priority on the stack, unlike target.
Adding the existing actual Chain.cpp, with no source change and22 verified input
objects reused, restores priority in EBX and five stack words/RET20. Factory
becomes82/all2 and transition119/all2, both strict zero difference. The caller
then restores EBX at+1D and uses the target immediate OR2 at the tail, instead of
keeping kind2 in EBX through a register OR. TransitionEffect and Chain retain their existing canonical factory /GL profiles; other input
profiles, the real InitializeVm entry and comparator remain unchanged.

This is a real dependency correction. It does not alter the canonical fourteen-
input factory recipe or the previous twenty-input Enemy/GUI/Item recipes. The
new transition recipe uses the current23-input graph and its StageAnimationSlots
PDB owner. It does not import another game's flags or a synthetic caller.

## Complete acceptance and scoped guards

[results.json](results.json) records the target fields/locals, source/header pins,
actual caller edges, graphs, controls and independent cold identities.

Two independent23-input, zero-reuse canonical colds reproduce all119 owned bytes
and both fields: the factory REL32 at+11 and game-speed address at+47. The internal
branch and every ordinary byte also remain equal. Both same-image factory82
contracts pass; the other selected guards include EndSpell, power, score/lives,
ANM creator/sprite/lookup/ID helpers, pending interrupt, sound and stage delay.
All fourteen existing same-image contracts pass2417 bytes/all89 fields in both
frozen images. The affected existing canonical stage-delay92 also cold-passes.
No historical full-cohort replay is required or run.

GameManager's complete native owner remains1544/1544, NONEXACT. It now calls both
real definitions, but reset's private EDI/plain-RET interface differs from the
actual target stack/RET4. The full reset remains184/214 in this graph, with its
VM cursor/index preservation also unlike target. Embedded200/202, binder197/203
and EnemyBeginSpell2424/2432 remain open; unlike fixed destinations or private
interfaces are never waived. No equal-length or partial-body credit is granted.

The earlier raw E8 hints at41BE65 and4415D5 still lack a reviewed function owner.
Fresh Ghidra reports no function descriptors or incoming references at41BE40 and
4415C0; a raw E8/E9 scan finds no direct entry edges to those starts. These hints
are not source, boundary, origin or exact credits, and no artificial callers were
introduced to influence the initializer.

Native jobs run serially at CPU0/nice10 with bounded logs below
`.analysis/gpt-6.1-sol/anm-embedded-context-20261031/`. Frozen artifacts stay in
`build/gpt-6.1-sol/anm-embedded-context-20261031/`; the canonical producer is
`build/match-linked/GameStageSlots`. Reproduce only the affected selectors:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit game-enter-menu-state --unit game-stage-transition-delay --json
scripts/repo-python scripts/ci.py
git diff --check
```

Exact totals become104586/275863 reviewed authored bytes across508 owners;
canonical totals172690/1385. Mappings1213/authored695 and backlog187 distinguish
complete maintained source from exact codegen. Original ownership, product,
runtime and portability remain open.
