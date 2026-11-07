# Enemy ECL sound queues: native record and cue-index words

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The Enemy ECL sound helpers `EnemySoundQueueView::QueueSoundSample`
(`0x0043DC90`, complete 123-byte owner) and
`EnemySoundQueueView::QueueSoundCue` (`0x0043DD10`, complete 149-byte
owner) receive sound IDs without an index-range check. The source had
formed `g_EnemySoundCueMetadata[soundId]` and `cueValues[soundId]` C++
array references, undefined outside those abstract arrays. The original
Win32 executable instead forms flat modulo-2^32 indexed addresses. Both
helpers now reuse the typed `EnemyNativeWordIndex` with the actual 8-byte
metadata-record stride and 4-byte cue-value stride, and a compile-time
wire check establishes the metadata cue halfword at +6. The existing
12-slot queue search, sample capacity guard, sound conversion and writes
are retained. This is not a new bounds check, and the game can still
access invalid memory for bad IDs.

Reproduction from this installed source (pinned VC7.1 SP1 `/GL /GS`,
real four-source linked diagnostic graph):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-sound-queue-cue --unit enemy-sound-queue-sample --json
scripts/repo-python scripts/ci.py
git diff --check
```

The selected cold artifact passes **272/272 original target bytes**:
`QueueSoundSample` 123/123 and `QueueSoundCue` 149/149, all four
original-bound fields (metadata DIR32 including its +6 addend,
floating-point constant DIR32 and conversion-call REL32). Seven default
CI stages also passed. This preserves two **previously accepted** exact
owners, without new exact credit or a whole-image/runtime proof. The
14,416-byte `DispatchEclInstruction` parent is still nonexact and not
certified by the leaf comparison.

The ignored selected replay receipt is
`.analysis/gpt-web/20261107-active/queue-exact-replay.json`.
