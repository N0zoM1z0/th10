# TH10 bullet-cancel 199-byte leaf: native BulletRuntimeView storage

Target is hash-attested Japanese TH10 v1.00a,
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete 199-byte `EnemyCancelBulletRecord` at `0x00408030`
is called from the complete 260-byte `EnemyCancelManagerView::CancelBulletPattern`
at `0x00408100`. These are separate target-owned bodies, not snippets.

Previously the 199-byte source read/write paths used dozens of raw byte
base+offset casts. It now views its existing `unsigned char *` ABI
parameter through the real `BulletRuntimeView` whose independently
checked fields are `state` at +0x446, `position` at +0x3B4,
`effectScript` at +0x438, `flags` at +0, the VM pending interrupt at
`+0x008+0x304=0x30C`, and the state timer at +0x3F8 (with flags
at +0x408). The timer's previous/current, subframe, scale and flags
initialization retains its exact order and direct flag-pointer lifetime;
the named view does not invoke an extra constructor or alter the record.
The out-of-playfield / in-playfield effects, state switches, bullet-flag
OR and original checked frame count are unchanged. Some unrelated
anonymous sprite/color accesses remain deliberately untyped.

The actual pinned VC7.1 `/GL /GS` selected canonical cold replays,
using the existing real dispatcher/cancel caller, prove the complete
`enemy-cancel-bullet-record` **199/199** and
`enemy-cancel-bullet-pattern` **260/260** exact, including original
linked fields. These owners were already exact: **0 new codegen credit**.

A separate real **11-source /GL** ECL graph verifies the companion full
`EnemyBeginSpell` and opcode interpreter against the previous candidate:
2,424/2,424 and 14,352/14,352 candidate bytes, respectively, with
unchanged 108 and 716 linked field positions/types and **zero** ordinary
candidate differences. Original targets remain 2,432 and 14,416 bytes,
so both big owners remain **NONEXACT**.

Only the two directly affected canonical exact units and two complete
large linked owners were checked, plus the seven-stage lightweight CI;
no historical full cold replay, runtime, portability or product claim.
See `native-result.json` and ignored
`build/gpt-web/20261105-bullet-cancel/typed-bullet-cancel-record/`.
