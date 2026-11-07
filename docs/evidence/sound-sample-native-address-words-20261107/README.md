# TH10 LoadSoundSample: native slot, RIFF and PCM byte addresses

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original `SoundPlayerView::LoadSoundSample @0x0043D390` is a complete
**684-byte** nonexact owner. The actual five-input VC7.1 SP1 `/GL /GS`
producer is Main.cpp, FileSystem.cpp, PbgArchive.cpp, GameErrorContext.cpp,
and Sound.cpp (the Sound TU also uses `/EHsc`). It owns a 71-byte
`FindWaveChunk` callee, already original-bound exact.

The original i386 addresses sound-buffer slots and pending-file-data slots
by adding a signed ID multiplied by four to the low 32-bit base. The old
C++ used `&soundBuffers[index]` and `soundFileData[index]` directly, which
can form an invalid C++ array reference before or beyond these fixed
arrays when index is not checked. The loader does **not** perform a
range check. The source now uses a typed `SoundWordSlot` forced-inline
helper for these accesses, with a native unsigned 32-bit base/index/stride
calculation. RIFF's `allocation+4`, successive two four-byte tag advances,
and the second wrapped lock-buffer copy's `samples+firstSize` now use a
matching `SoundByteWord` helper. They preserve byte orders, allocated
buffer ownership, error paths, stale slot cancellation and all DirectSound
calls; no range validation, new behavior on malformed input, or portable
pointer guarantee is claimed.

## Independent complete-source native trials

Seven C++ source forms were actually compiled and fully linked in the above
five-input graph against the original target: only the buffer slot,
pending-data slots with repeated or local slot addressing, only RIFF byte advances, only second PCM copy,
both slots, and the installed all-address combination. Every result
preserved the entire 684-byte PDB contribution, all **25 candidate**
link-field positions/type/width, and the remaining **six original ordinary
byte differences at `+0x122..+0x127`**, namely the `MOV EAX,EBP` versus
`LEA EDI,[ESP+30]` scheduling. Ordinary exactness remains **578/584**;
these 25 candidate fields are *not* upgraded to independent original
bindings. All builds preserved the full 71-byte `FindWaveChunk` leaf,
with its original CRT strncmp linkage.

A first selected cold replay checked five already accepted units
(1,874/1,874); a separate extended current-source cold replay also
checked the original-bound 338-byte streaming-buffer owner. All six
checks pass in the four affected graphs:

| Existing exact unit | Original bytes |
| --- | ---: |
| `sound-find-wave-chunk` | 71/71 |
| `sound-data-loader-thread` | 107/107 |
| `sound-process-queues` | 1,452/1,452 |
| `sound-bgm-player-thread` | 189/189 |
| `sound-player-reopen-bgm` | 55/55 |
| `streaming-sound-init-buffers` | 338/338 |
| **Total** | **2,212/2,212** |

No new exact owner/codegen credit, whole-game build, or runtime correctness
claim. The five-input retained parent from the installed source was also
independently inspected after cold replay: 684 bytes / six remaining
ordinary mismatches. Reproduction (selected units only):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-find-wave-chunk --unit sound-data-loader-thread \
  --unit sound-process-queues --unit sound-bgm-player-thread \
  --unit sound-player-reopen-bgm \
  --unit streaming-sound-init-buffers --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored source/PE/PDB/MAP trial artifacts are under
`.analysis/gpt-web/20261107-active/sound-sample-native-word-variants.py`,
`.../sound-sample-native-word-results.json`,
`.../sound-sample-native-installed-replay.json`, and
`build/gpt-web/20261107-active-sound-sample-words/`.

## Additional current-source field/extent witness

After the initial commit, the seven isolated word-index alternatives and
the helper-based installed source were compared again from the real current
five-TU PE/MAP/PDB against the complete original 684-byte function: the
source retains **25 candidate** field locations/types and precisely six
ordinary differing bytes at `+0x122..+0x127` (**578/584**). The sibling
`FindWaveChunk` remains complete 71 bytes with one original-bound strncmp
REL32 at `+0x1A`. The additional selected cold replay passes six existing
exact units **2,212/2,212**, not a new match claim.

Retained private receipts: `.analysis/gpt-web/20261107-active/`
`sound-sample-native-index-probe.py`, `sound-sample-native-results.json`,
`sound-sample-current-owners.json`, `sound-sample-words-selected-replay.json`;
full independent candidate PE/MAP/PDB images are under
`build/gpt-web/20261107-active-sound-sample-native/`.
