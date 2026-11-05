# TH10 spell-history ownership: typed accessor shared by beginning and ending spells

Target: hash-attested Japanese TH10 1.00a,
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The fixed `GameStatisticsOwner` layout places records at +0x08 with a
0x437C stride. Each row has 110 consecutive 0x90-byte spell-history
entries starting at row +0x59C, ending exactly at row +0x437C.
The independently established `captures` at record +0x80 and
`attempts` at record +0x84 are not interchangeable.

A real `GameStatisticsRecord::SpellHistoryAt(int)` member now expresses
that row-relative address. The full 2,432-byte target `EnemyBeginSpell`
accesses its statistics rows through the genuine manager layout, including
row six (`8 + 6*0x437C + 0x59C == 0x19A8C`), without separately multiplying
opaque global offsets. The target-exact `EnemyEndSpell` likewise obtains
its two capture records through the same member. The old explicit second
spell-start group-index read is retained, not cached or optimized away by
source rearrangement. Original class names/ownership remain provisional.

An actual pinned 11-source VC7.1 /GL native graph compiled the **entire**
changed ECL source with only previously hash-pinned support objects whose
transitive quoted-header dependencies do not contain the modified header.
It reproduces the **2,424-byte** `EnemyBeginSpell` candidate with **108**
unchanged linked fields, and the **14,352-byte** ECL dispatch candidate
with **716** unchanged fields. Masking only those decoded field locations,
there are **zero ordinary candidate-to-candidate differences**. Original
complete targets are **2,432** and **14,416** bytes, respectively: neither
large function is target-exact and no new exact credit is claimed.

Selected cold canonical replays from the changed sources also confirm
`enemy-end-spell` **399/399**, `game-statistics-load` **423/423**, and
`enemy-spell-panel-update` **1,057/1,057** exact: **1,879/1,879 bytes**
previously accepted. No full cohort, product closure or runtime test.
The `native-result.json` pins the installed source/record layout; ignored
compiled artifacts are under `build/gpt-web/20261105-spell-boss/`.
