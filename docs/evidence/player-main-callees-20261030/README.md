# Player fallback and complete result openings — 2026-10-30

**Two complete411-byte result owners are exact in two independent canonical
cold links:822 newly exact authored bytes/all56 fields.** The3112-byte Player
main remains nonexact; its complete native candidate is now3080 rather than3064.
The [results](results.json) retain both cold reports, selected old replays,
independent target review, source/object pins and complete native preservation.

## Complete result owners

| Target | Maintained source | Complete bytes | Fields | Fixed locals |
| --- | --- | ---: | ---: | ---: |
| 4231D0..42336A | ResultMenuOpenGameOver | 411 | 28 | 3 |
| 423370..42350A | GameCompleteStageTransition | 411 | 28 | 3 |

Fresh pinned Capstone decode consumes102 instructions for game-over and103 for
completion, including each final RET. Five CC bytes precede the next reviewed
entry. The old isolated pilot's108-instruction game-over count is superseded;
its411-byte boundary and authored-origin decision are retained. All18 DIR32
and10 REL32 fields per owner are independently checked from encoded target
operands, then bound to exact source publics or unique unresolved aliases.
No local branch or ordinary operand is normalized. Each complete canonical
comparison has zero differences, independently reproduced after recompiling
all15 inputs twice. Native EDI receiver/plainRET emerges from ordinary pointer
source and the actual Player caller.

Both paths implement the mode1/configuration1000 early transition2/4, state6,
timer0, flags58|10, capture script0/layer15, rectangle32/16/384/448, GUI front
resource, music17, conditional dummy4 then unconditional dummy2, profile mark
1D8A3, speed save/reset and their distinct final state. Game-over uses script128
and completed0; completion uses129 and1. Every operation belongs to the complete
comparison. Legacy FrontEndProfileData is bound to the already reviewed47783C
alias; this does not establish production storage/alias legality.

The important source corrections are real `CreateVmVariant1` calls and a typed
`AnmVmIdView` parameter for RequestCaptureRectangle. The creation methods inline
the observed allocation/initialization/publication sequence and reuse one
four-byte result slot. Typed capture passes the existing ID directly to FindVm,
avoiding the former int-to-ID construction and restoring the target ESI receiver,
five stack values and RET20. The capture owner itself is162/162 **nonexact**:
remaining ordinary store scheduling differs. No capture-owner credit follows.

The graph adds the actual Sound.cpp command wrapper/QueueCommand definition to
the preceding fourteen-input Item/Main/Player context. It uses the real
InitializeVm source as the diagnostic entry to preserve its observed ECX
resource/two-stack-value/RET8 boundary. All pinned per-source /GL profiles remain
unchanged; no new compiler options, normal-support partition or fake ABI is
introduced. AddVmVariant1's complete118 raw bytes and the old FindVm73 remain
exact. Other callee bodies, including capture and initializer, need not be exact
for the separately bounded callers to be exact. All caller fields nevertheless
bind their actual reviewed destinations and the observed call interfaces.

## Main owner and rejected controls

The [preceding ownership review](../player-update-owner-20261030/README.md)
remains authoritative: main425730..426357 owns3112 bytes, including alignment
and its five-entry table. Binding its real fallback to owner477830 restores
EBP Player, the target1C frame/save order and the missing owner-load field.
The guarded power quotient is explicitly short, item angle is evaluated before
kind selection, the expired timer tests zero, and fallback/main-mode branch
order follows the target. These recover the target16-bit tests, angle lifetime,
saved-position copy and branch topology. The native owner now has110 fields;
only the two scaled-decrement FMUL-one fields are absent from the target112-field
count. Other ordinary history/motion/vector/alignment differences remain.
Size and field counts do not establish partial exactness.

The first canonical15-input cold also reproduces the entire native main3080,
capture162 and power-transition397 with identical ordinary bytes, field offsets,
semantic bindings and fixed internal destinations versus the final warm graph.
Nine preexisting native nonexact candidates, including destructor262, preserve
their complete bytes/field identities versus entry. Twenty-one selected existing
exact bodies pass2508 bytes/all60 fields in this same cold image. New-owner
credit is kept separate from these preservation checks.

The following specific controls are parked:

- Moving float Subtract to a separate /GL TU, const-reference and integer
  parameters, and reversing multiplication operands leave main unchanged.
  None restores the two FMUL-one fields; all are reverted.
- Adding actual Gui.cpp callers is byte-neutral for this question and is omitted.
- Capturing a named VM ID local leaves two result slots and emits415; reverted.
- Expanding creation manually emits414 with the initializer entry; the real
  creation methods plus typed capture produce the accepted411 owners.

A transient Wine path conversion failed before compilation. The retained failure
is not a comparison result; the required toolchain execution smoke passed and
only that unfinished probe was rerun. Native comparison jobs are serial,
CPU0/nice10, with700-second probe or600-second canonical-batch limits and exit
logs. No Ghidra call, host failure simulation or comparator change was needed.

## Reproduction and scope

With the canonical read-only target available, use explicit selectors:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --check
    timeout 600 taskset -c 0 nice -n 10 scripts/repo-python \
      scripts/replay-exact-units.py \
      --unit result-menu-open-game-over \
      --unit game-complete-stage-transition --json

Repeat the selected pair for a second independent cold witness. The canonical
manifest declares the full15-input graph, pinned profiles, real initializer
entry, FrontEnd PDB contribution and all56 field bindings. The retained read-only
[review script](review.py) checks complete target operands/local branches and
replays that contract against any saved cold image directory:

    scripts/repo-python docs/evidence/player-main-callees-20261030/review.py \
      --image-dir build/match-linked/ResultTransitions \
      --output .analysis/player-result-review.json

Affected old canonical selections are explicit: SpawnShot717, input174,
bonus81, shared clear44; then rows105, callback7, angle67, AddVmVariant1 118 and
FindVm73. They pass1386 bytes/all30 fields. Together with the new pair, this is
11 selected owners/2208 bytes/all86 fields, without a historical-cohort replay.
Default seven-stage CI and whitespace checks pass. No original declaration/TU,
production storage, whole-product, runtime or portability claim follows.
