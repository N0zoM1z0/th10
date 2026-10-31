# Complete SoundPlayerView::PreloadBgm read setup — 2026-10-31

The original Japanese TH10 1.00a owner `0x0043D7D0..0x0043D940`
(369 bytes, end inclusive) is still **NONEXACT**. This changes an ordinary
source expression and measures the **entire** PDB-owned native contribution
in the original `ProcessQueues` compiler context. No canonical match or
partial credit is claimed. The verified target SHA-256 is
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Target instructions and complete candidate

The original function receives its owner in ESI, path in EAX and index on the
stack. It calls `GetFmtIndexByName` with path in ECX, then seeks/allocates,
reads the selected preload size and publishes the successful allocation and
format. The old notes mistakenly claimed EAX as the callee's path input;
that was a different, unrelated no-/GS compiler context. See
[the independent caller/callee audit](../sound-preload-reg-contract-20261031/README.md).

The read setup at `+0x11A..+0x12E` uses ECX for the format-table base,
EDX for the DWORD requested size, and EAX for the output DWORD address.
The current plain source selected EDX/EAX/ECX instead, giving a complete
369-byte candidate with **6 ordinary byte differences** (all register
choices). Preserving the loaded size as an explicit local `const DWORD`
and placing a compiler memory barrier between that load and `ReadFile`
changes the register schedule to ECX/EAX/EDX. It is now **365/369 exact**
after binding the complete 10-field manifest, 121 dense instructions and
all fixed local destinations. Remaining normal differences are exclusively
`+0x121`, `+0x127`, `+0x12A`, `+0x12B`, the EAX/EDX exchange across the size
load, output address and two pushes. The original qualifier/barrier source
spelling is unknown; the retained barrier is a supported compiler ordering
model, not evidence of synchronization or an original threading requirement.

The ten bound fields are: `g_MainSoundOwner` base+`0x4108`,
`g_MainSupervisorView` base+`0x150`, CRT `free`, `CreateFileA`, the **actual
owned** `GetFmtIndexByName`, `SetFilePointer`, CRT `malloc`, two
`CloseHandle` references and `ReadFile`. The full comparator checks the
original address/relocation roles, the body extent and the actual callee
binding, not just a masked or cropped byte count. All 10 roles are bound in
`verify-native.py`. A status of mismatch, **not exact**, is required.

## Scoped controls and acceptance of existing dependencies

Several attempted source spellings were rejected on **complete owner**
comparison. Named size/output pointers, a format reference, an inline
read wrapper and differently ordered helper arguments were byte-neutral at
6 differences. Volatile size and table pointers widened the owner to
383 and 384 bytes. The basic read barrier without a captured size was
byte-neutral; the captured-size barrier first improved 6 to 4 differences.
Pointer-first/last, reference and two-barrier variants retained the same
four differences. All unselected forms were restored, with their private
comparison images and logs under `build/gpt-web/sound/` and
`.analysis/gpt-web/sound/`. These controls do not justify accepting a
nonexact owner.

The real producer is Sound.cpp primary plus ZWave.cpp support, pinned
`/GL /GS /EHsc` under the actual `ProcessQueues` entry. Only the affected
canonical `sound-process-queues` and `sound-bgm-player-thread` units were
cold-replayed. Both pass, **1452+189=1641 owned bytes exact**, in the same
recompiled graph. The complete 369-byte nonexact candidate was independently
strict-compared both in that canonical producer and in a separate local
fresh two-input build. Before the successful local build, one Wine linker
attempt failed with a page fault; the toolchain `--execute` smoke subsequently
passed and the successful retry produced a complete PE/MAP/PDB. No failed
attempt is relabeled successful.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python docs/evidence/sound-preload-barrier-20261031/verify-native.py \
  --artifact-dir build/match-linked/SoundInitBuffers
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-process-queues --unit sound-bgm-player-thread --json
scripts/repo-python scripts/ci.py
git diff --check
```

No manifest or exact ledger entry is added for PreloadBgm. The record does not
establish original code ownership, whole-game build, runtime semantics or
portability. Recover the EAX/EDX lifetime before another exact claim.
