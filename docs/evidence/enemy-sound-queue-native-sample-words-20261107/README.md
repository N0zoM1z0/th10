# Enemy ECL sound queue: unchecked sample-count address words

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Both `EnemySoundQueueView::QueueSoundCue` and
`EnemySoundQueueView::QueueSoundSample` reuse an existing active sound
slot. They reject stored sample counts `>= 128`, but do not reject negative
counts. Consequently the original `samples[slot][count]` C++ indexing
was undefined for counts before the start of the nominal 128-element row,
while retail x86 adds a signed count scaled by four modulo 2^32.

The stored sample slot is now formed by
`EnemyNativeWordIndex(&samples[slot][0], count)` in each function. The
12-slot search, `>= 128` rejection, sample-count increment, and duplicate
sound-ID handling are unchanged. Neither a bounds fix nor an assumption
of valid negative indices is implied; this is native Win32 behavior only.

Current-source focused native validation:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-sound-queue-cue --unit enemy-sound-queue-sample --json
scripts/repo-python scripts/ci.py
git diff --check
```

The selected cold VC7.1 SP1 `/GL /GS` four-source linked graph passes
**123/123 `QueueSoundSample`** and **149/149 `QueueSoundCue`** original
code bytes, all four original-bound fields. These are *existing* exact
owners, not new ones. The encompassing Enemy ECL dispatcher still needs
separate full-owner reconstruction. Selected ignored receipt:
`.analysis/gpt-web/20261107-active/queue-sample-native-replay.json`.
