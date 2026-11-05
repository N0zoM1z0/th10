# TH10 157-byte text alpha inversion: recover target sign branch

The private original is Japanese TH10 1.00a with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete owner at `0x00437160` is 157 bytes. The A1R5G5B5 format
inverts the high alpha bit, then clears the 15-bit color when that high
alpha bit is **zero**. This is a bit-of-stored-format test, not a signed
floating-point comparison or a new alpha conversion.

The retained source formerly checked whether the high byte, interpreted as
`signed char`, was nonnegative. After `TEST AH,AH`, MSVC 7.1 used `JL` at
owner-relative `+0x69`; the target uses `JS` there. Both are equivalent after
`TEST` (which clears overflow), but they are not the same native bytes. The
source now tests the actual high-byte `0x80` bit explicitly using
`(static_cast<unsigned char>(newValue >> 8) & 0x80u) == 0`. The existing
per-format loops, RGB clear, original branch ordering and API surface stay
unchanged. No forced register, inline assembly or target byte patch is used.

## Full owner and scoped exact guards

Two **actual** normal-COFF VC7.1 SP1 `/O2 /Gy /GF /Oi /MT` compilations of
`src/TextRenderer.cpp` emitted complete 157-byte `InvertAlpha` contributions,
with **zero relocations**. The **only change** across the two candidate bodies
is `+0x69` `0x7C` (JL) to `0x78` (JS), the target byte. Raw matching bytes
in this specific normal-COFF context improve from **111/157 to 112/157**;
all other 45 normal-COFF mismatches remain. The complete function is
**NONEXACT**; this is not an exact ledger admission. Earlier notes describing
other compiler contexts do not override this current full-owner comparison.

Three selected original-bound complete-function cold replays on the installed
modified source passed: `text-buffer-apply-alpha-bleed` 946/946,
`text-create-buffer` 781/781 and `text-fill-color` 72/72. **1,799/1,799
previously accepted exact bytes** remain verified. No new exact credit, full
historical replay or native game product/runtime claim.

Source pins, first/last raw bytes and selected unit results are in
`native-result.json`; ignored native COFF and selected linked artifacts
remain under `build/gpt-web/20261105-text-leaf/` and `build/match-linked/`.
