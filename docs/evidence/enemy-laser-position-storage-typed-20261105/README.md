# TH10 laser position leaf: typed narrow receiver at +0x430

Original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemySetLaserPosition` is the complete original **23-byte** target
at `0x0040CF60`. Its observed native contract takes a 32-bit laser
receiver and copies three float words into receiver +0x430. The
maintained function formerly wrote through the raw integer
`laser + 0x430` pointer cast. Its new narrow
`EnemyLaserPositionAt430View` stores exactly a 12-byte
`PlayerFloat3 position430` at `offsetof == 0x430`; the native
`sizeof == 0x43C` assertion also checks the view's total occupied
span. It does **not** guess the historical laser subtype or map
unreviewed surrounding fields. The parameter ABI and copy ordering
remain unchanged.

Two actually selected canonical normal-COFF VC7.1 replay units pass
with modified source: `enemy-set-laser-position` **23/23**, and adjacent
`enemy-find-laser` **22/22**, altogether **45/45** previously exact
original bytes. They are already accepted; no new codegen credit is
claimed. There is no new same-image full ECL boss comparison in this
isolated leaf batch; existing complete boss evidence belongs to earlier
source epochs and must not be silently transplanted. Seven-stage
lightweight repository CI and whitespace verification passed; no full
historical cold replay, product link or runtime claim.

Source pin and selected replay evidence are in `native-result.json`.
