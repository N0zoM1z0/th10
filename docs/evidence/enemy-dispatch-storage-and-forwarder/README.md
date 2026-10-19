# Enemy dispatcher storage and raw-float forwarding review (2026-10-20)

The complete dispatcher remains nonexact at **14524/14416 bytes**, with the
target-sized0x2C4 frame. All26 source/header inputs are restored to b3dc30b.
No exact bytes or authored denominator bytes are added. The existing26-byte
origin-indeterminate entry0x412AA0 gains a maintained source mapping to
`EnemyRuntimeView::ReadRawFloatArgument`; this is source presence only.

## Independently observed target facts

Local read-only Ghidra attests the executable identity and complete mapped
`.text` hash before/after a rolled-back raw-instruction review. Its dispatcher
body and raw-float adapter agree with independently decoded target bytes.
`target-facts.json` records the selected addresses and all13 adapter callsites.

The dispatcher clears64 bytes for a spawn request at ESP+0x248, and also
decodes spell-name bytes at ESP+0x248. Its cookie is at final ESP+0x2CC.
This proves physical reuse, not an original authored union. The maintained
union's0x84 byte capacity remains provisional: distance to a cookie alone does
not prove an array declaration or capacity. Neither that value nor the
0x1F8 laser scratch layout is changed or size-swept.

The MIT reference's `matching2/msvc/src/game/enemy/enemy_execute.cpp`, at
c6791d82dc9f02e55eca45a5314f900a85481efd, suggests separate spawn and spell
locals. It describes this owner as equivalent recovery with assembly exact
fallback. Its128-byte array, volatile state, force-inline helpers and assembly
are not TH10 evidence and are not adopted.

Target0x412AA0..0x412AB9 is a complete26-byte forwarder: EAX supplies the
runtime, its+0x14D8 field supplies the full owner, and owner+4 supplies the
active ECL context. The first stack word is the operand index, forwarded in
ECX; the second is the raw float, forwarded on the stack to0x44FF80. The
wrapper returns through RET8. All13 direct calls are in the dispatcher.
This justifies the descriptive source mapping. Original source declaration,
class/TU, authored-versus-outlined origin and exact codegen remain unknown.

The current integer adapter source is a direct return. Its ledger previously
described an older assignment-to-index experiment as current; that sentence
is corrected without changing its target boundary, ABI or origin.

## Falsified source models

| Control | Complete dispatcher | Frame | Result |
| --- | ---: | ---: | --- |
| Fresh baseline | 14524/14416 | 2C4/2C4 | Retained source |
| Separate function-scope spawn request and block-local spell buffer | 14552/14416 | 30C/2C4 | Request and spell bases become ESP+48/+288; physical reuse is lost; restored |
| Scope the request to the spawn cases and move its real shared tail into that block | 14552/14416 | 30C/2C4 | Same storage failure; restored |
| Direct receiver expression in the raw-float forwarder | 14524/14416 | 2C4/2C4 | Wrapper receiver changes to ECX instead of target EAX; restored |
| Snapshot the real raw-float argument before resolving the context | 14524/14416 | 2C4/2C4 | Entire raw parent and wrapper unchanged; restored |
| Match the wrapper index's unsigned type to the real generic consumer | 14524/14416 | 2C4/2C4 | Entire raw parent and wrapper unchanged; restored |

The direct-expression wrapper improves ordinary positional agreement from7/22
to13/22, but changes the actual private input ABI. That score cannot justify
adoption. The other wrapper controls preserve7/22. No dummy argument, ABI
attribute, volatile access, artificial return, constructor, padding or compiler
profile change is used. Both separated-storage controls keep the existing
0x84 spell capacity,64-byte request clear, actual fields and real calls.

The baseline and unsigned-index full cold links emit the identical complete
parent hash. Both retain all181 selector bytes,108 ordered physical groups,
19 sentinel comparisons and35 target-sized spans. Whole-owner diagnostic
agreement is613/11580 positions; aligned agreement is2447/11143, with aggregate
absolute span gap792. These counts do not grant partial-case exact credit.
The separated controls have32 equal-sized spans and absolute gap972.

## Reproduction and validation scope

`source-overlays.json` contains ordered byte-preserving source replacements,
counts and verified input/output hashes for all five rejected variants.
`native-review.json` freezes each complete parent/image hash, all26 project
inputs, the fixed compiler profile and actual eight-TU graph. Use
`ltcg_link.cold_link` with those inputs, primary EnemyEclDispatcher.cpp, real
DispatchEclInstruction entry and the listed support order to reproduce a cold
diagnostic. The retained baseline inputs equal the maintained source at b3dc30b.

The baseline and shared-header unsigned-index control are full cold builds.
The four primary-only controls compile that TU anew and relink with seven
hash-checked baseline /GL objects after verifying every other source/header
input unchanged. Their reuse receipts are included; they are diagnostics,
not independent cold or exact proofs. No native job is restarted to recover
lost observation. No historical whole-cohort replay is performed.

The baseline strictly replays six existing complete target recipes in its real
carrier: ResolveIntArgument75, ReadInt144, ReadFloat183, WrapAngle94,
ReadIntValue124 and ReadFloatValue171: **791 bytes/all16 linked fields**.
The two value consumers therefore do not have a byte mismatch in this graph.
`existing-guards.json` records their scoped graph overrides and results. The
normal-COFF WrapAngle recipe is unchanged; its linked replay is an extra guard.
Canonical recipes and matches.csv are unchanged.

Toolchain check, tracking, generated progress, full public CI and whitespace
checks pass. Only one fresh baseline is retained; five completed diagnostic
builds are removed. The pre-existing untracked D3D experiment is untouched.
Original optimizer partition/lifetimes, complete dispatcher codegen, native
product/runtime, dual-Oracle semantics and portability remain open. Revisit
this owner with new TH10-backed storage or partition evidence; do not repeat
the rejected capacity-preserving storage, receiver, snapshot or index controls.
