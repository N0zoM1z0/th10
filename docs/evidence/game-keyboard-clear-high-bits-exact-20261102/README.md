# Complete TH10 keyboard high-bit reset — exact 75-byte owner (2026-11-02)

Target: **Japanese TH10 v1.00a**, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The reviewed game-authored instruction body `0x0044B080..0x0044B0CA` is
**75/75 exact** under the pinned VC7.1 SP1 `/GL /GS` compiler and linker.
The complete PDB-owned contribution is 75 bytes; Capstone consumes the full
instruction extent, no unannotated bytes remain, and **all four** original
address fields are separately bound. No partial match or normalized omission
is accepted. The directly calling 0x00438AD0 WinMain is itself authored
and independently accepted; the source name `ClearKeyboardHighBits` and
translation-unit placement are descriptive, not provenance claims.

The natural `src/GameKeyboardReset.cpp` source uses a 256-byte stack array,
`GetKeyboardState`, a signed-index loop applying `& 0x7F` to each byte, and
`SetKeyboardState`. This generates the retail `SUB ESP,0x104` frame,
256-element loop with index EAX and `CMP EAX,0x100`, both imported calls,
and complete `/GS` canary verification and return. No target code byte,
synthetic padding, fake ABI, forced register or inlined machine-code constant
is involved.

| Original offset | Field | Original target |
| --- | --- | --- |
| `+0x07` | `___security_cookie`, DIR32 | `0x00473660` |
| `+0x1A` | `__imp__GetKeyboardState@4`, DIR32 | `0x00466208` |
| `+0x32` | `__imp__SetKeyboardState@4`, DIR32 | `0x00466204` |
| `+0x40` | `@__security_check_cookie@4`, REL32 | `0x00458EA5` |

Two **independent targeted cold builds** each emit a 75-byte exact result,
with distinct PE and PDB identities (not reused intermediate artifacts):

- First PE SHA-256 `6fb856c5ddd68c9fc3c496840c3a6acc1d92bcaef4a6aa8e547e7522b916da03`, PDB GUID `60fdeec354720348bf9cb8f2dc36fa5d`.
- Second PE SHA-256 `ba91d4e8fb4630d69e025c85010999a790ba7868c95d49a1b535d683de036462`, PDB GUID `7834051f07aade47816ca534bbeb600a`.

Scoped reproduction:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit game-keyboard-clear-high-bits --json
scripts/repo-python scripts/ci.py
git diff --check
```

The unit establishes only complete codegen exactness for this owner. It does
not establish the original physical source text, product/image closure,
runtime equivalence or portability. Other ANM/ECL/Sound/Chain candidates
remain nonexact; their unrelated working variants were not discarded or
included in this canonical exact unit.
