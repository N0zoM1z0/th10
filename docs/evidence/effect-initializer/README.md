# Transition initializer and static callback addresses (2026-10-20)

`TransitionEffectView::Initialize @ 0x43C970` now closes **396 authored exact
bytes** in two independent cold links. Its owner remains357 code bytes,
3 alignment bytes and36 bytes of switch table. This review also discovers five
missing callback owners, adding **1,250 authored denominator bytes** and five
source mappings, without claiming those callbacks exact.

## Source and target evidence

The only native source change is the real registrar formal order:

```cpp
RegisterCalc(void *arg, int priority, ChainCallback callback);
updateNode = Chain::RegisterCalc(this, 14, TransitionFadeOut);
```

Both calc/draw definitions and all16 actual calls are updated together.
All three real operands, their types, callee-pop annotation, node writes and
chain algorithms are preserved. The fixed existing14-TU Gui updater graph
chooses the target EAX callback/EDI priority/one-stack-owner private interface.
The complete43-byte registrars remain exact. No new ABI keyword, artificial
argument/result, padding, forced outlining, section pragma or flag is introduced.
The natural order eliminates18 ordinary instruction bytes of load scheduling
differences in the396-byte initializer; it does not prove unique original source
spelling, class/TU ownership or production compiler partition.

The MIT reference at c6791d82dc9f02e55eca45a5314f900a85481efd suggests this
real-formal order in `matching2/msvc/src/core/screen/configure.cpp`. Its separate
scheduler member/receiver model, alias context, section pragmas and noinline
controls are not adopted. TH10 target bytes and our complete native replays
establish the retained result.

Fresh local read-only Ghidra attests the target identity and full mapped `.text`
hash before/after review. Temporary metadata is rolled back. Its body ranges,
entry references, complete dense decoding, RET tails and following CC runs
independently establish these missed extents:

| Entry | Complete bytes | Maintained source | Following CC bytes |
| --- | ---: | --- | ---: |
| 0x43C230 | 133 | TransitionFadeIn | 11 |
| 0x43C310 | 175 | TransitionDim | 1 |
| 0x43C460 | 154 | TransitionPulse | 6 |
| 0x43C550 | 439 | TransitionShakeLinear | 9 |
| 0x43C710 | 349 | TransitionShakeEnvelope | 3 |

Ghidra finds initializer references only at their entries and no outside
interior references. A supplemental full-file literal scan agrees. Specific
effect/timer phases, game pause/startup state, RNG and camera writes support
authored-game origin at medium confidence, independently of native similarity.
Names are descriptive. All five implementations already existed in the source;
source presence and complete target ownership now have explicit ledger rows.
`target-facts.json` records their hashes, exact bounds, padding and references.
Their original source declarations and whole-runtime equivalence remain unknown.

The existing396-byte initializer boundary is unchanged. Its physical case order
is0,1,2,3,5,4,6,7,8; its nine table slots still represent modes0..8. Every internal
destination offset is compared exactly. The target's fixed-base image lacks
base relocations, so target fields are independently decoded from357 code bytes
and the complete nine-entry table, then bound to reviewed callees/data.

## Static address acceptance boundary

The new explicit DIR32 descriptor binds ordinary TU-static C++ function
**addresses only**. It requires source plus both complete candidate/target
sizes, the maintained target source/name, reviewed target boundary, dense code
on both sides, a unique qualified MAP/PDB owner and the exact function entry.
No addend, local mode, implicit fallback, arbitrary object override or mixed
static-CALL descriptor is allowed. `static_address_binding_only` in the receipt
makes the scope explicit. A nonexact callback may have a different declared
native size, such as124/133 or297/349, while its address remains independently
bound. The enclosing parent must still compare every byte and every field.

The direct static CALL lane keeps its existing complete raw-equal, zero-field
callee requirement. It is not weakened or implicitly reused. Fifteen synthetic
linked-context tests cover the old and new lanes, including relocated parent
equality with deliberately unequal callbacks, distinct whole sizes, wrong TU
or source name, cropped/unreviewed targets, interior entries, incomplete code,
partial/mixed descriptors and missing explicit selection. Existing qualified
owner and internal-destination controls also pass.

Both complete cold images pass **1,069 bytes/all63 fields**: initializer396/37
plus nine existing guards673/26. The37 initializer fields are15 static callback
addresses,11 registrar calls, one game-speed pointer and10 internal DIR32 fields
(table base plus nine entries). The native instruction decoder consumes395/396
bytes; the final table byte is covered by its complete authoritative DIR32 word.
All396 bytes are replayed and compared. No instruction-only truncation, table
permutation, partial-case credit or callback byte promotion is used.

`native-summary.json` pins42 identical project inputs, fixed profiles/entry and
support order, separate PE/PDB identities and all field bindings. The canonical
recipe is `transition-effect-initialize` in config/match-units.toml; it shares
the existing GuiTransitionEffectFactory carrier with the nine guards.

```sh
scripts/repo-python scripts/build-match-unit.py --unit transition-effect-initialize
scripts/repo-python scripts/compare-linked-function.py --unit transition-effect-initialize --json
```

## Validation and remaining work

Host baseline passes14 scenarios/843 assertions, with the existing kind8
rounding gap explicitly retained. Three updated selector/delete mutation
anchors compile and fail only at their required assertion/exit86. Fixture calls
change order with the real API; scenario assertions and event semantics do not
change. Only two strict LF dependency pins change after full-file reversal
proves the two registrar declarations/definitions are the entire dependency
delta. Historical raw receipts remain provenance. All19 existing EOL acceptance
and rejection controls pass unchanged.

Toolchain execution/declarations, tracking, generated progress, full public CI
and whitespace checks pass. The fresh global boundary-ledger audit still stops
at the previously documented0x401100 evidence-text difference, with identical
extent/state/confidence; no broad automatic ledger replacement is performed.
The five new extents instead have the focused target/Ghidra proof above.
`validation-summary.json` records actual checks and artifact cleanup. One
canonical PE/MAP/PDB is retained; superseded cold and host controls are removed.
The pre-existing untracked D3D evidence is untouched.

Cleanup127 still differs in four argument-move bytes. FadeIn133, Dim175,
Pulse154, LinearShake439 and Envelope349 remain nonexact despite three
target-sized native extents. Their floating-point schedules, pauses and actual
runtime remain separate work. No product/storage, target runtime, dual-Oracle
semantic or portability gate is closed by this checkpoint.
