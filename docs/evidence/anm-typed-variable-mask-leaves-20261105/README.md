# TH10 ANM variable-mask ownership in two exact operand lvalue helpers

Target is Japanese TH10 v1.00a with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ANM instruction header's `variableMask` at offset +0x06 is exactly
16 bits. Its bit `argumentNumber` selects whether an operand is a VM-local
variable or an immediate. The 156-byte
`AnmVmView::GetFloatVarPtr @ 0x0043ED00` and 132-byte
`AnmVmView::GetIntVarPtr @ 0x0043EDA0` both implement that selector.
The maintained code now carries a shared two-byte
`AnmVariableArgumentMaskView` with `sizeof == 2` assertion and
`SelectsVariable()` predicate. Each full leaf continues to receive its
original scalar `unsigned short variableMask` argument and invokes the
shared predicate; no new object ABI or padded source is asserted.

On the installed modified source, explicit pinned VC7.1 canonical selected
cold replays verify both complete owners with all original link fields:
`anm-get-float-var-ptr` **156/156 exact** and `anm-get-int-var-ptr`
**132/132 exact**. These were previously exact, so **no new exact credit**
is added.

A separate real five-source VC7.1 `/GL /GS` link checks the *entire*
`AnmRenderManagerView::ExecuteScript @ 0x0043EE30`. With the typed mask
used by the leaf helpers, the 9,964-byte parent interpreter retains **350
complete target-bound linkage fields** (including 95 local destinations)
and **9,962/9,964 ordinary matched original bytes**. The only remaining
original mismatches are the two previously known register-choice bytes
`+0x113` and `+0x115`. The boss is still **NONEXACT**, and no altered
bytes or near-match credit were manufactured. A different approach that
replaced the *entire interpreter's* macro bit tests with method calls
expanded the candidate to 10,268 bytes; it was rejected and restored.

Only scoped original-target replay and the full affected ANM owner were
run, plus seven-stage lightweight CI and whitespace validation. No full
historical cold replay, product build or runtime validation. Source pins
and native result appear in `native-result.json` and the ignored local
`build/gpt-web/20261105-followup-anm/typed-instruction-next/` artifact.
