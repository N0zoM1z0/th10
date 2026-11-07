# ANM ExecuteScript script timer: original native 32-bit current-word increment

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete original `AnmRenderManagerView::ExecuteScript @0x0043EE30`
is **9,964 bytes** with **350 independently original-bound address/code
fields**. The maintained current-source complete native PDB extent is
also 9,964 bytes and still matches **9,962/9,964**. Two unresolved
register-selection bytes remain at opcode 91, `+0x113,+0x115`.

`TickScriptTimerScale` is inlined into the large ANM executor. Its normal
speed path previously expressed the source timer's signed current-frame
counter increment as `++*reinterpret_cast<unsigned int *>(&timer->current)`.
The target i386 increments a native four-byte word modulo 2^32. The
previous source's unrelated unsigned-int lvalue also had C++ strict
aliasing concerns. The installed natural C++ now performs an explicit
unsigned 32-bit +1 and converts the low word back to the Win32 signed
`int` counter. This retains the original previous-frame snapshot and
separate subframe float update; no additional checks, storage, forced
registers or target-byte copying have been added. This target-specific
conversion is tied to the pinned VC7.1/i386 compiler.

## Complete native boss and exact regression evidence

The **current** five-TU `anm-evaluate-rgb-interpolation` canonical graph
was first cold-compiled from real installed source and a complete target-
bound 9,964-byte `ExecuteScript` baseline obtained. Four different
natural timer-word forms were separately compiled and linked against the
same other four source-pinned support objects, checking all **350 target
fields** and every ordinary code byte. The explicit unsigned assignment,
unsigned `+=`, a scoped unsigned temporary, and the simple signed `++`
all produced full **9,962/9,964** comparisons. The simple signed `++`
was rejected because it leaves signed overflow undefined; unsigned
assignment is the installed choice.

The **installed modified source** was compiled afresh and again compared
against all 9,964 original bytes and all 350 original-bound fields.
It matches **9,962/9,964**, with precisely the same two nonfield
mismatches at `+0x113,+0x115`. Thus the source semantic correction
preserves the large whole-owner frontier, but **is not a new exact match**.

A narrowly selected original-target cold replay of the two exact units
in this **same five-TU** production graph passes **876/876** original
bytes: `anm-evaluate-rgb-interpolation` 820 and `anm-int3-scale` 56.
This is not an all-historical replay. No original source spelling,
full-game rebuild or runtime equivalence is claimed.

Private proof: `.analysis/gpt-web/20261107-turn3/anm-timer-word.py`,
`anm-timer-word-results.json`, `anm-base-target-bound.json`,
`anm-timer-installed-compare.json`, `anm-timer-guards.json`, and
`build/gpt-web/20261107-turn3-anm-timer/` linked PE/MAP/PDB per variant.
