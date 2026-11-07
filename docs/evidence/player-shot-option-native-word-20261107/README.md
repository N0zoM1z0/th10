# TH10 Player shot option origin: native 32-bit option-record address

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`PlayerShotSetOptionOrigin @0x00428C20` consumes the signed 8-bit
`PlayerShotDescriptorView::sourceIndex` at descriptor +0x1c and addresses a
`0x98`-byte Player option record before copying its fixed-point coordinate
pair to a shot's float position. The original i386 forms the native low
32-bit scaled index; it does not clamp the signed source index. Previously
`&player->options[descriptor->sourceIndex - 1].replayPair1` formed a C++
array pointer outside the declared array for unchecked index values. The
maintained source now forms the typed record location with modulo-32-bit
unsigned address addition and multiplication by `sizeof(PlayerOptionRuntime)`.
It preserves the signed 8-bit promotion and subtract-one, the original
float conversions/offset subtraction, and all later writes. This is not
new input validation, portable pointer safety, or a runtime-correctness
claim for malformed shot descriptors.

The encountered `Enemy.hpp` include dependency was also repaired: this
header uses the complete `AnmFloat3View` size but previously did not
include its definition; `AnmManager.hpp` is now explicit. The worktree's
existing `PlayerShotTracking.cpp` explicit ANM include was retained, not
reset or discarded. This allows standalone COFF probing of the player
shot owners instead of failing on an undeclared vector type.

## Current native results and limits

Seven separately compiled natural VC7.1 source variants, including typed
record-word and byte-position-word formulas, produced the same complete
normal-COFF `PlayerShotSetOptionOrigin` candidate: **122 bytes**, two
candidate relocations, **80/117** ordinary bytes at original offsets. Its
original target is **125 bytes**; the EAX/ESI register lifetime and three
byte extent gap remain nonexact. `PlayerShotTrackEnemy` remains a
**266-byte original / 288-byte normal-COFF candidate**, with no new
exact claim.

The actual installed source and changed header were cold replayed against
three already-accepted original-bound units (selected, not full replay):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-shot-acquire-target-00428ad0 \
  --unit player-shot-angle-difference-00428ce0 \
  --unit ecl-vm-host-run --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
git diff --check
```

`PlayerShotAcquireTarget` **53/53**, `PlayerShotAngleDifference`
**80/80**, and `EclVmHost::Run` **146/146** — **279/279** already exact
original bytes across three actual native artifacts. No new codegen exact
owner, complete TH10 executable or semantic/runtime proof is asserted.
Trial scripts, source checks and receipts are in ignored
`.analysis/gpt-web/20261107-active/{player-shot-option-variants.py,
player-shot-option-results.json,player-shot-native-exact-replay.json,
player-shot-native-coff.json}`.
