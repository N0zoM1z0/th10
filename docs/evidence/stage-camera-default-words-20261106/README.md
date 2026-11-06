# TH10 camera reset: recover both +0xE4 word stores

The complete reviewed original owner is `StageResetPublishedCameras` at
`0x004216F0..0x004218C0`, **465 bytes**, in Japanese TH10 v1.00a with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Direct original decoding reveals two stores omitted by the maintained source:

| Original instruction | Destination | Value |
| --- | --- | --- |
| `0x0042183F` | secondary camera `0x00491E94 + 0xE4` | dword 1 |
| `0x0042189C` | published camera `0x00491D7C + 0xE4` | dword 0, from zeroed EAX |

The source now represents the existing four bytes as `unsigned int unknown0E4`
and restores both assignments after each camera's viewport setup. Compile-time
checks preserve the **0x118-byte** camera extent and **+0xE4** member offset.
The word's higher-level purpose remains unknown; no camera storage definition
or alias ownership is inferred.

Fresh pinned VC7.1 SP1 native comparisons of the complete installed source
produce **533 bytes** under both the single-TU `/GL` profile and the repository's
normal-COFF `/O2` profile. The normal object contains **40 DIR32 relocations**;
its named camera relocations and +0xE4 addends independently confirm the two
new stores. See [the bounded native record](native-result.json). The original
465-byte extent still differs: vector temporary reuse and store scheduling
remain unresolved. **Zero new exact bytes**, no match-ledger admission.
The earlier 518-byte result predates these assignments and remains historical.

Private reproduction:

- `.analysis/gpt-web/20261106-stage-camera-mode/native.py mode-writes`
- `.analysis/gpt-web/20261106-stage-camera-mode/verify-fields.py`
- `build/gpt-web/20261106-stage-camera-mode/mode-writes/native-result.json`
- `.analysis/gpt-web/20261106-stage-camera-mode/normal-compare.json`

Installed source SHA-256:
`src/StageCameraDefaults.cpp` =
`4ac0d9c1eeb14e1763f5ed642e50895e5081fdf2f41dcda819a6ca7ee7255fb8`;
`src/StageScript.hpp` =
`3a1a5402fdd969c150844102680d007f1080f6e38e44f429e70f0594b537bf5b`.
This establishes source recovery, not product/runtime or original-TU closure.
