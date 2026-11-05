# TH10 complete ECL interpreter — typed binary32 instruction operands

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Target-reviewed ECL instructions have 16 bytes of fixed header, followed by
variable-length 32-bit operand words. The maintained `EnemyEclInstructionView`
previously exposed only `int operands[1]`; the complete 14KB dispatcher
therefore repeatedly cast the address of an integer operand word to a float
pointer before passing that binary32 payload to `ReadRawFloatArgument`.
The instruction view now overlays `float floatOperands[1]` with the original
integer word array through an anonymous union. Both members are asserted at
`offsetof == 0x10`; no change is made to the encoded variable instruction
length or mask. **Thirteen actual dispatch operands** across absolute,
mirrored, global and relative enemy-spawn branches now read this typed float
word directly, preserving the target's bit interpretation and evaluation
order. This does not claim the original C++ class spelling.

The real, full **eleven-source VC7.1 `/GL` ECL graph** was rebuilt with the
modified dispatcher, plus `Enemy.cpp` and `Player.cpp` as header-affected
support sources. Eight support objects whose sources/header dependencies
were unchanged were reused only with verified SHA-256 pins. The **complete**
`EnemyRuntimeView::DispatchEclInstruction` remains a **14,352-byte** candidate
versus the **14,416-byte** original; all **716** existing linked fields
retain their offsets/types, with **zero** ordinary candidate-to-candidate
byte differences. `EnemyBeginSpell` remains a **2,432-byte** complete
candidate with 108 fields and zero ordinary candidate differences. Other
complete checked owners include the 505-byte drop core in this *different*
LTCG context, the 672-byte nonexact enemy constructor, 330-byte camera owner
and 60-byte projection leaf; none regressed. These candidate-preservation
comparisons do **not** prove original target exactness for the large owners.

Scoped, separately registered target-anchored exact replays pass
`enemy-read-int-argument` **16/16**, `enemy-read-raw-float-argument` **26/26**
and `enemy-drop-item-counts-core` **503/503**, totaling **545/545** already
credited bytes. Seven-stage lightweight repository CI and whitespace checks
passed. No full historical cold replay, game-product link, new exact credit,
or runtime claim. Detailed source pins and comparisons are in
`native-result.json`; ignored link artifact:
`build/gpt-web/20261105-float-ops/typed-float-operands/`.
