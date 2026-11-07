# Full 133-byte original-target exact TransitionFadeIn

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete reviewed `TransitionFadeIn @0x0043C230..0x0043C2B4` body is
**133 bytes**, native scheduler callback `ECX` receiver / `RET`. It is
classified authored in the pre-existing original-origin ledger.

## Reconstructed source and exact compiler lead

The existing natural `static int __fastcall TransitionFadeIn(void*)` in
`src/TransitionEffect.cpp` already models startup abort, alpha interpolation,
the two terminal timer ticks, and the optional game-manager pause gate. The
last condition checked `(game->flags58 & 5u) == 0`, which is semantically
correct but VC7.1 optimized to `TEST BYTE PTR [EAX+0x58],5`, reducing the
compiled callback to **124 bytes**, nine short of the original.

The original `0x0043C296..0x0043C2A3` sequence instead loads the entire
32-bit flags word, copies it to ECX, shifts ECX right two bits, combines
ECX with the unshifted word, and tests bit zero:

```asm
mov eax, [eax+0x58]
mov ecx, eax
shr ecx, 2
or  ecx, eax
test cl, 1
```

The maintained C++ now expresses precisely that equivalent bit test:

```cpp
if (game == NULL || (((game->flags58 >> 2) | game->flags58) & 1u) == 0)
    effect->timer.Tick();
```

This neither changes the null short circuit, pause behavior nor the two
selected flag bits. No register forcing, padding, inline assembly, fake
callee, copied target bytes or ABI shim is introduced. A separate natural
source trial with reversed OR order was byte-identical, while an XOR
variant left one ordinary byte different; two separate flag tests did
not preserve the complete 133-byte owner.

## Strict original-bound native evidence

A changed-`TransitionEffect.cpp` probe first recompiled the single affected
source using pinned VC7.1 SP1 `/GL`, preserving source/header SHA pins of
the other thirteen actual input translation units. All 133 ordinary bytes
and all five actual code-field positions aligned to the original, before
any canonical acceptance. The original target fields are independently:

| Offset | Type | Actual native symbol | Original address |
| --- | --- | --- | --- |
| `+0x02` | DIR32 | `g_GameStartupCounter` | `0x004918A4` |
| `+0x23` | DIR32 | literal `255.0f` | `0x00470BF8` |
| `+0x2C` | REL32 | CRT `__ftol2` | `0x00463B2C` |
| `+0x5E` | DIR32 | `g_ResultGameManager` | `0x00477810` |
| `+0x79` | REL32 | `AnmVmTimerView::Tick` | `0x00404ED0` |

The actual installed source was then **independently cold built twice**, in
the real 14-TU `GuiTransitionEffectFactory` caller graph. The registered
`transition-fade-in` unit binds the complete static MAP symbol
`?TransitionFadeIn@@YIHPAX@Z` to the unique 133-byte PDB section
contribution of `src/TransitionEffect.cpp`, not a presumed public symbol.
Both cold builds strictly reproduce **133/133 original bytes and 5/5 field
identities**, including branch destinations and actual ABI, with distinct
PDB GUIDs and native image hashes:

| Cold | FadeIn | Native PDB GUID |
| --- | --- | --- |
| First | 133/133, 5 fields | `aa38b5e87cfc91439e97ce81ab0e3048` |
| Second | 133/133, 5 fields | `5ec01b819cf35d4c91eead49849fe37d` |

The first selected cold also replayed the affected prior exact functions
`transition-effect-initialize` **396/396** (37 fields) and
`transition-delete` **26/26** (2 fields): **555/555** total. Initializer
static function-address geometry for both FadeIn callback sites was updated
from candidate124 to its now-real 133-byte PDB extent; the initializer's
entire own bytes and all linkages remained strict exact.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit transition-fade-in --unit transition-delete \
  --unit transition-effect-initialize --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit transition-fade-in --json
```

Private hashes, trial images and logs are retained under
`.analysis/gpt-web/20261107-fade-in/` and
`.analysis/gpt-web/20261107-fade-in-selected-{2,3}.{json,err}`.
The final PE/MAP/PDB is under
`build/match-linked/GuiTransitionEffectFactory/`. This promotes **one new
confirmed-authored exact owner, 133 original bytes**, but does not claim
complete-game build, runtime equivalence, portability or physical original
translation-unit spelling.
