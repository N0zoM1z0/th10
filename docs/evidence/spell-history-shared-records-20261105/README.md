# TH10 shared spell-history attempts/captures and exact statistics tail

Original: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This batch corrects the **semantics and shared ownership** of the fixed
0x90-byte spell-history records in the real maintained `EnemyBeginSpell`
(complete target 2,432 bytes) and `EnemyEndSpell` (399 bytes).

Target spell-start @0x00409280 increments a counter at entry +0x84 when
beginning each spell. The separately target-exact `EnemyEndSpell`
@0x00409C00 increments **entry +0x80** only on a captured spell, while
the spell-panel reads two separate counts at owner-relative `+0x624`
and `+0x628`. The previous local ECL type called the +0x84 field
`captureCount`: **incorrect semantic name**. That claim was rechecked
against the actual end-spell increment before being corrected.

The common `GameStatisticsSpellHistoryEntryView` is now shared by
`EnemyEclDispatcher.cpp` and `GuiSpellNotice.cpp`: 0x80 bytes of
name, `captures` at +0x80, `attempts` at +0x84, eight opaque final
bytes, and compile-time size/offset assertions. `EnemyBeginSpell` now
increments `attempts`, `EnemyEndSpell` increments `captures`. The
original storage is linked to `GameStatisticsOwner::records`:
`0x5A4 = records[0] at owner +8 + record-relative 0x59C`; the overall
`0x19A8C = 8 + 6*0x437C + 0x59C` refers to record 6. No fabricated
source identity or bounds guarantee is inferred from these offsets.

The previous **dirty, unfinished tail-field change was also completed**:
`GameStatisticsTailPayloadView` overlays the exact 0x440-byte payload
without removing the raw byte array, exposing the initializer's
0x0C text and 512 successive 16-bit words starting at +0x46, with
native offset/size assertions. Real selected cold exact replay proves
`game-statistics-load` **423/423**, `game-statistics-initialize-tail`
**142/142**, and `enemy-end-spell` **399/399** = **964/964** bytes.

In the real eleven-source VC7.1 /GL graph, complete `EnemyBeginSpell`
remains a **2,424-byte candidate** versus 2,432 target and the ECL
14,416-byte dispatcher a **14,352-byte candidate**. Their respective
108 and 716 linked-field geometry is unchanged, with zero ordinary
candidate-to-candidate byte differences. Neither large function is exact;
there is **no newly admitted exact codegen**. Seven-stage lightweight
CI and whitespace checks pass. No full historical cold replay or runtime
claim. Full local artifacts under
`build/gpt-web/20261105-followup-ecl/spell-shared-attempts-and-captures/`.
