# TH10 player option shot-origin callback @0x00428C20 — 125-byte source owner

Reviewed original TH10 Japanese 1.00a: `0x00428C20..0x00428C9C`, a
125-byte independent, CC-delimited function with **private ECX Player, EDX
PlayerShotRuntimeView, plain RET**. Its physical C++ TU, callback registration
and original handwritten/generated ancestry are not established. The method
implemented here is a *descriptive* source mapping, not a proof of the
historical name or an exact match.

The native reads the shot's 0x34-byte descriptor at shot +0x58, promotes
its **signed** sourceIndex at descriptor +0x1C, scales it by the genuine
0x98-byte `PlayerOptionRuntime` stride and reads the *preceding* indexed
option's `replayPair1` fixed-point X/Y, which resolve to actual player
`+0x3244/+0x3248` at index zero. It applies the original **0.01f** scale
from 0x00470B00, builds a float3 origin with zero Z and copies it into
shot motion +0x14/+0x18/+0x1C. It then adds descriptor spawn offset X/Y
(+4/+8) minus motion velocity X/Y (+0x20/+0x24) to the two new coordinates;
no Z velocity is touched, and it returns zero.

A fresh full native decode and actual typed source comparison revealed a
wrong early guess: `options[sourceIndex].replayPair0` was **0x90 bytes too
far into the player**, even though it built and visually resembled the
native code. That hypothesis has been rejected. The maintained source now
uses `options[sourceIndex-1].replayPair1`, with a named pointer to the real
existing `PlayerPositionPair`, and no invented shadow class or dummy data.
The source is in `src/PlayerShotTracking.cpp` next to the separately exact
53-byte target acquisition and 80-byte signed-angle helper.

Three bounded VC7.1 `/GL` source experiments tested the initial fixed
option layout, a named `PlayerPositionPair` pointer, delayed descriptor
reload and shot-position pointer assignment. All produce a valid native
function surface; the selected pointer model yields **122 vs 125 target
bytes, 80/117 preliminary ordinary instruction bytes and two candidate
literal linkage fields**. Compared with the initial wrong-layout source,
this fixes the actual native data operands and slightly improves matching;
`config/matches.csv` remains unchanged. The remaining nonexact differences
are mostly VC7.1 allocation/register scheduling (descriptor ESI vs EAX,
index EAX vs ESI, and aggregate-copy temporary aliasing). Length or
partial ordinary agreement must not be misreported as canonical exactness.
Trial logs: `.analysis/gpt-web/20261103/player-option-125-*` (ignored).

Only the two existing exact player-shot functions in the same TU and relevant
Player/angle math references are scoped cold-replayed; no full product or
historical cold replay is invoked. Further work must use the complete
125-byte PDB contribution and actual source/caller ABI for admission.
