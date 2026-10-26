# Current TH10 ECL and ANM parent-residual controls (2026-10-26)

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
These are **complete-owner diagnostic replays**, not a new exact match or a
portability/runtime claim. See `controls.json` for individual source hashes,
actual attempted controls, compiler/link receipt hashes, and CI limits.

## Independent source-current baseline

A serial `nice 10` ten-source `/GL /GS` cold link through the real Enemy
dispatcher entry first reproduced ECL Run at 6,995 of **7,020 owned bytes**;
all 189 link fields, 62 fixed local destinations, both jump-table/selector data
regions, and the 171-byte ReadFloatValue callee were checked. The subsequent
Enemy viewport-helper export changed the shared source. Therefore this
checkpoint **also independently reruns the ECL diagnostic and full ANM bound
comparator against the later actual EnemyItemDrops image**, SHA-256
`95a546f62ffeb8adf890a214149434b13582cc29988330b298ff15b4365616fd`.
ECL remains 6,995/7,020, all 189 fields, callee 171 exact; ANM ExecuteScript
remains **9,960/9,964**, with all 350 fields. No source/target/ledger byte
normalization has been weakened. These numbers measure owned *matching bytes*;
7,020/7,020 function *extent* is not 7,020 bytes exact.

ECL's remaining 25 ordinary differences include early thread-register and
addressing choices, a pair of final loop-latch registers, and sixteen math /
FORMAT stack-home bytes. Complete target/native Capstone decoding identifies
math operand stores in `esp+0x14` versus `esp+0x18` and FORMAT's scratch/counter
lifetime occupying the opposite homes. The source-current 10-TU cold replay
preserves that topology. ECL is **still nonexact**.

ANM's four remaining differences are precisely `+0x113/+0x115` (Child91 uses
ECX for renderLayer while the target uses EDX) and `+0x249A/+0x24A0`
(the second vertex's Z sum loads positionOffset.z/position.z in the wrong order).
They occur inside the same 9,964-byte source-current owner; neither is an
unreviewed instruction-table hole. The two nearby X/Y operations and the first
vertex are already equal. The 92 physical dispatch groups are not reordered.

## Bounded attempts and outcome

The ignored experiment runner under `.analysis/gpt-web-ecl20261026/` compiled
private source copies only. ECL probes compiled only EclVm and reused nine
hash-checked game objects. ANM probes compiled only AnmManager and reused nine
hash-checked objects. Each comparison required the full target extent and
strict link-field contract; all compiler/profile inputs retained `/GL /GS`.
No target EXE was written or patched.

- ECL: alternative float declaration order in LENGTH_SQUARED and POINT_ANGLE,
  FORMAT declaration placement, and inlining the final advance expression all
  yielded the same complete 6,995/7,020. An explicit thread-ID intermediate
  changed an externally bound call location: the strict comparator rejected
  its field inventory; **no partial score** was assigned.
- ANM: reversing only the second radial vertex's whole vector addition fixed
  the two Z operand bytes **but lost four previously matching X/Y bytes**,
  yielding 9,958/9,964. Explicit three-component constructors and tested
  per-component operand reversals likewise yielded 9,958/9,964. Thus surface
  operator reversal is falsified as a whole-owner fix, even where Z improves.
- ANM Child91: separate assignment to the two-member argument record preserved
  9,960/9,964 unchanged; named loaded-file capture yielded 9,958/9,964. A
  reversed record field layout displaced link fields and was rejected before
  scoring, not promoted as 9,964-byte evidence.

The source copies, private compiler/link logs, result JSON and exact diagnostic
images remain ignored; the public JSON records hashes and results for each
executed control. No trial source change was adopted. **No gain to the ECL/ANM
exact ledgers** follows. The separate newly exact EnemyPrepareProjection helper
(complete 60-byte owner / one REL32 field) was correctly committed earlier as
`2c5a96f`, with supplemental proof at `3b7daa8`.

## Validation and limits

A full-CI invocation previously encountered an `AddressSanitizer:DEADLYSIGNAL`
output storm at **Validate Spt Test helper contracts** after passing earlier
stages. The ignored log grew to 28,837,623,836 bytes; its original size,
first 131 KiB, last 32 KiB and truncation scope were recorded before deleting
the redundant 28.8-GB scratch log. The isolated Spt Test helper then passed
1/1 in a separate normal (non-capped-compiler) sanitizer invocation. That does
**not** retroactively validate the failed full-CI attempt. A second bounded
full-CI run progressed through 37 stages before its **240-second safety limit**;
it is **incomplete**, not a pass. Both exit conditions and bounded logs are
retained in ignored `.analysis/gpt-web-ecl20261026/`.

Focused ECL FORMAT passes 10 scenarios and eight functional mutants; ANM child
routing passes 24 layouts, 192 static paths and 16 strict negative fixtures.
The fresh Enemy projection linked comparator passes 60/60 with its sole field;
toolchain, tracking, progress and whitespace gates are independently rechecked.
Native whole-product build, production ABI, executable runtime and portability
remain open. Do not infer success from a matching length, a subset of aligned
instructions, or a host sanitizer test.
