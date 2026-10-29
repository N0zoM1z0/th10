# Complete primary-buffer format exact, 2026-10-29

`CSoundManager::SetPrimaryBufferFormat`, **44C7E0..44C8E3 / 260 bytes**,
is admitted as `sound-manager-set-primary-buffer-format`. The maintained
implementation was already present in ZWave.cpp; this batch establishes its
complete exactness rather than claiming newly recovered source.

The original Japanese TH10 v1.00a target has SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local read-only Ghidra attestation passes. The existing reviewed boundary agrees
with a fresh complete raw decode: 260 instruction bytes, no trailing table or
undecoded suffix, ending in RET4. The real initializer's call at43CE0A supplies
stack owner, EBX channels, EDI sample rate and ECX bits. The ordinary C++ member
declaration and pinned LTCG context reproduce this private interface naturally.

All three link fields are independently bound: security cookie473660 at+4,
and security-check458EA5 calls at+30/+FA. Existing canonical declarations agree
on both target symbol addresses. The unchanged strict comparator verifies the
qualified ZWave PDB contribution, PE/MAP/PDB identity, exhaustive field inventory,
local branches and every ordinary byte. No new mask or comparator exception is
introduced. CreateSoundBuffer, SetFormat and Release remain actual COM calls.
The SetFormat failure path returns without releasing the primary buffer, as in
the target; it is included in the byte proof.

Two independent final-source canonical Sound+ZWave cold builds pass the whole
260-byte owner/all3 fields under the existing `/GL /GS /EHsc` graph. The first
also replays four existing Sound/streaming guards and the affected setup-thread
interface in its separate real-Main graph: **2509 bytes/all70 fields/six owners**.
The second selects only the new owner for its independent reproduction. Initial
strict comparisons also pass in the retained two-input and real15-input Main
images. These are bounded exactness claims, not whole-product/runtime closure
or proof of original source spelling, TU, class names or SDK authorship. The
existing origin attribution is unchanged and remains separate from byte proof.

Reproduce the new owner alone:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit sound-manager-set-primary-buffer-format
```

[results.json](results.json) retains compact proof scope, source pins and
experiment outcomes. Private logs, images and listings stay under
`.analysis/gpt-6.1-sol/sound-primary-format-20261029/`,
`.analysis/gpt-6.1-sol/sound-initialize-20261028/` and
`build/gpt-6.1-sol/sound-primary-format-20261029/cold-a/`.

The parent Initialize43CD30 remains **755/786 bytes, nonexact**. Its target
zeros WAVEFORMATEX::cbSize twice: once as the final short of ZeroMemory at43CE4D,
then explicitly at43CE92. The explicit assignment is retained; source spelling
is provisional. The old pending producer now completes successfully. Success-first
branches, descriptor ordering and separate/common failure-exit models do not
reproduce the parent. In particular, one shared failure exit saves EBX at entry,
whereas the target saves it only after manager initialization succeeds. All
speculative CFG/order rewrites are restored; no parent partial credit is taken.
