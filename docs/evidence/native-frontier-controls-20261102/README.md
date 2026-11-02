# TH10 current-source native controls: large ANM and bullet direction

This is a strictly **NONEXACT** experiment record from 2026-11-02. The
private source variants were actually compiled under the pinned VC7.1 SP1
`/GL` toolchain, not merely reasoned about. The accepted exact ledgers and
source are unchanged by these unsuccessful controls.

## ANM ExecuteScript: 9,964-byte complete owner

`AnmRenderManagerView::ExecuteScript` at `0x0043EE30` is 9,964 bytes. In the
five-source ANM/RandomMath/AnmVmCreate/AnmVmId/AnmInt3Scale graph with the
same `/Ob1` AnmInt3Scale support override and entry selector, the complete
PDB contribution has all 350 independently specified link fields (95 local
or table obligations). The retained source again compares **9,962/9,964**
after full binding. Only `+0x113` (native 0x4D, target 0x55) and `+0x115`
(native 0x51, target 0x52) differ, reflecting ECX versus EDX for the opcode
91 child render-layer argument.

Fresh codegen controls and full-owner results:

| Natural source control | Native comparison |
| --- | --- |
| Assign script then layer as sequential record fields | 9,962/9,964, same two mismatches |
| `const` child-argument record | 9,962/9,964, same two mismatches |
| Memory barrier immediately before child creator call | 9,962/9,964, same two mismatches |
| Call `.GetVm()` on an unnamed returned ID temporary | 9,944/9,964; regresses 18 ordinary bytes |
| Default-create ID then assign creator return | changes external-field positions; strict field-inventory rejection |

No alternative is retained. In particular the unnamed temporary is not a
credible improvement merely because its total size happens to be 9,964.
The member/stdcall three-stack-argument target contract stays untouched.
Further ANM work needs evidence that predicts **EDX** specifically instead
of cosmetic source permutations.

The independently considered texture upload owners also remain NONEXACT:
`LoadTexture` is 242 native vs 243 target bytes and `LoadTextureRegion`
is 319 native vs 320 target. Rewriting both raw ANM header calculations
from `data + offset` to `&data[offset]` in a current single-source resource
build preserved those wrong-sized extents; the spelling was reverted. The
remaining target MOV+LEA versus native MOV+ADD is not normalized away.

## Bullet absolute-direction update: 335-byte complete owner

`BulletRuntimeView::UpdateAbsoluteDirectionChange` at `0x004078E0` has a
335-byte complete target. In the **BulletHorizontalWrap** seven-source link,
the current maintained source emits a 335-byte PDB contribution. Masking
its eight *decoded candidate* external fields for a nonacceptance
codegen diagnostic yields **333/335 ordinary bytes equal**, differing at
`+0xD7` (target 0x96, native 0x86) and `+0xDD` (target 0x52, native 0x50).
The target loads the angle to EDX and pushes EDX; native chooses EAX.
These field positions were not certified as a new target-bound strict
unit, and no exact credit is claimed.

Scoped natural controls for `angleBits` (an additional float local, unsigned
integer bit carrier, a declaration at function entry and an outgoing-angle
snapshot) kept the same two differences. A volatile angle carrier changed
stack/frame codegen and yielded 352 rather than 335 bytes. Other isolated
controls also changed the owner length or kept the register blocker. Each
variant was restored; current bullet source retains the original bit-preserving
angle read. A new instruction-level register-lifetime explanation is needed
before another trial.

## Chain allocation: same-size structural control, still nonexact

The reviewed `th10::Chain::AllocElem` at `0x00449ED0` owns 74 target bytes.
In the actual four-source GameManager/Chain/AnmManager/GameErrorContext link,
retaining the previous flag bits across constructor writes with a scoped
compiler barrier recovers the target's load/stores/AND sequencing and produces
a 74-byte PDB contribution (baseline 69 bytes). It still has **27 ordinary
byte differences** after masking its sole candidate REL32 new-call field:
the optimizer selects EDX for the zero value and ECX for flags, whereas the
target uses ECX for zero and EDX for flags. Additional callback-param and
barrier controls did not recover the instruction scheduling and were restored.
This is a useful size/constructor-order probe, not an exact source checkpoint.

No full historical cold replay or product/runtime claim accompanies these
NONEXACT observations. Private, regenerable candidate logs remain in
`.analysis/gpt-web/20261102/`; sources, toolchain and original EXE remain
untouched in the committed checkpoint.
