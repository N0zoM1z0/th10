# TH10 Bomb Update: recovered 223-byte owner source; 10-byte callback exact

Target is the Japanese v1.00a executable with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `0x00405750..0x0040582E` 223-byte per-frame Bomb update is now
source-present in the maintained `ScriptViewerBombOwnerView::Update` body.
The independently reviewed callback `0x00405840..0x00405849` was missing
from the target-function inventory; it is now a complete **10/10 byte EXACT**
source owner with an independent registered boundary and exact target call.

## Real target behavior, not copied binary bytes

The original 0x48-byte Bomb controller stores mode at +0x28, ANM VM ID at
+0x2C, world position XYZ at +0x30..+0x38, a VM-derived value at +0x3C,
and an ANM timer at +0x14. `Update` returns one for inactive mode. If the
mode is one, it resolves its VM through the real ANM manager at 0x00491C10,
clears the state/ID and returns one if that VM is absent. When present, it
moves the position Y by **0.5f** in the reviewed spell-ID special cases,
by **1.0f** in one character/statistics mode and by the actual original
**1.3f** constant (0x00470D34) otherwise. The source then adds the target
playfield origin **224.0f/16.0f**, publishes the screen position using the
real `AnmRenderManagerView::SetVmPosition`, copies the VM +0x40 field to
owner +0x3C, and enters the separately retained Bomb effect helper at
`0x00405AC0`. The frame timer advances through the real separately outlined
`AnmVmTimerView::Tick` at `0x00404ED0`. This path preserves the original
null, state and gameplay branches, without injected host failure scenarios.

The selected full parent is **NONEXACT**: after the genuine compiler profile
and preserving the out-of-line timer call, the new VC7.1 SP1 `/GL /GS /EHs`
PDB section contribution is **220 native bytes versus 223 target bytes**,
with 12 native external field slots. Earlier trial forms were 317 bytes when
the timer was inlined, 242 when it was outlined but used an untyped external
Bomb emission, and 222 with separate member emission and alternate mode
selection. The remaining register allocation, early zero test and local
branch scheduling are not yet exact. **Zero bytes from this 223-byte parent
are credited as exact**. The `0x405AC0` helper remains source-absent at this
checkpoint. This is a genuine maintained-source model, not original class/TU
identification.

## Complete 10-byte callback proof

`ScriptViewerBombUpdateCallback405840` takes its game-chain receiver in ECX.
The complete original instructions are `PUSH EDI; MOV EDI,ECX; CALL
0x00405750; POP EDI; RET`. They decode densely for all **10 bytes**, then
end before the CC alignment and separately accepted 6-byte `0x00405850`
no-argument draw callback. The candidate PDB boundary matches exactly, with
its one REL32 **+0x04** independently bound to the original `Update` entry
`0x00405750`. The body is not awarded authorship: the adapter could be
explicitly authored or compiler-generated.

The two independently cold-compiled 4-source contexts use
`ScriptViewerBombCreate.cpp`, `Chain.cpp`, `ScriptViewerLifecycle.cpp`, and
`AnmManager.cpp`, with pinned VC7.1 SP1 `/GL /GS /EHs`. They reproduce
**10/10 exact**, with different PE hashes and PDB GUIDs. The existing exact
Bomb registration **84/84**, factory **85/85** and destructor **137/137**
are separately selected-cold guarded for **306/306** with their 28 external
field bindings preserved. Their natural source is unchanged; no historical
full cold replay was run. [native-summary.json](native-summary.json) retains
the source, target and accepted cold identities.

Reproduce with selected scope only:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-bomb-update-callback --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-bomb-create --unit script-viewer-bomb-register-tasks \
  --unit script-viewer-bomb-destructor --json
scripts/repo-python scripts/ci.py
git diff --check
```
