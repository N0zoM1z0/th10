# Sound PreloadBgm: original 0x34-byte format-index word arithmetic

Original executable: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`SoundPlayerView::PreloadBgm @0x0043D7D0` is **369 target bytes**. The
actual native target disassembly loads `bgmFormats` and executes
**`IMUL EDI,EDI,0x34`** at owner-relative `+0xDD`, then forms the relevant
format-record member addresses with native 32-bit index/address words.
`sizeof(ThBgmFormat)==0x34` is enforced by the existing layout assertion
in `src/SoundFormat.hpp`. The old maintained `bgmFormats[formatIndex]`
spelling may form C++ out-of-array/overflow pointer arithmetic for an
unchecked script-derived signed index. The original i386 behavior is the
low 32-bit multiplication and base addition, without any extra clamp.

The maintained source now uses the same unsigned word expression for **three
separate** native format reads: start offset for `SetFilePointer`, allocation
size passed to `malloc`, and request size passed to `ReadFile`:

```cpp
reinterpret_cast<ThBgmFormat *>(
    reinterpret_cast<unsigned int>(bgmFormats) +
    static_cast<unsigned int>(formatIndex) *
    static_cast<unsigned int>(sizeof(ThBgmFormat)))->preloadAllocSize
```

Each access still occurs at its original place; no `ReadFile`, allocator,
I/O flag, file-format member, memory lifetime or read-result condition is
changed. Importantly, the fourth `bgmPreloadFormats` pointer-publication
expression was **not** rewritten: combining that change with these three
makes the complete parent codegen worse (362 rather than 365 matched bytes).
This intentionally preserves the distinct target codegen and does not
promise portability or valid out-of-range file access.

## Fresh source-context complete function experiments

The real current `src/Sound.cpp`/`src/ZWave.cpp` VC7.1 SP1 `/GL` graph
was cold rebuilt after the header changes in the repository, **not**
reused from a stale support epoch. The original target-bound baseline has
a complete 369-byte PDB contribution and exactly **ten** known external
linkage obligations: the main Sound/Supervisor globals, true CRT
`free/malloc`, `GetFmtIndexByName`, `CreateFileA`, `SetFilePointer`,
`ReadFile`, and the two `CloseHandle` import addresses.

Twenty-three additional natural C/C++ `ReadFile` size/count, local scope,
`register`, data aggregate and compiler barrier variants were compiled and
fully compared against the original target. The best remain **365/369**;
several altered the PDB extent or failed to link, and an isolated Wine
link error was recorded as failure, not accepted. All experimental source
edits were restored byte-for-byte.

Nine separate native BGM-format-address controls then tested each of the
four format uses alone, the first two, the first three, the final two,
all four together, and a shared named `format` pointer. Each of the
**selected first three** individually and together retains the complete
**365/369 original ordinary bytes**, all **10/10 independently target-bound
fields**. The all-four control fell to 362/369; the shared-pointer form
changed the owner size and was rejected. The four remaining original
ordinary differences are `+0x121,+0x127,+0x12A,+0x12B`: the target uses
EDX for the ReadFile request length and EAX for the count-output pointer,
where the candidate uses the opposite registers. `PreloadBgm` is still
**NONEXACT** and contributes **zero new canonical exact codegen credit**.

The chosen three-word source was installed, with a source-only comment,
then **independently selected for an original-bound cold replay**:

| Previously exact Sound owner | Original bytes |
| --- | ---: |
| `sound-player-reopen-bgm` | 55/55 |
| `streaming-sound-init-buffers` | 338/338 |
| `sound-bgm-player-thread` | 189/189 |
| `sound-process-queues` (large owner) | 1,452/1,452 |

These four complete accepted owners pass **2,034/2,034** original bytes
and all **58** declared original linkage fields in two genuine native
build artifacts. The final installed SoundInitBuffers PE/PDB was separately
compared against the entire original PreloadBgm; its result remains
**365/369 with exactly ten original-bound fields**. No historical all-unit
cold replay or full-game runtime assertion is made.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-player-reopen-bgm \
  --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread \
  --unit sound-process-queues --json
```

Final `src/Sound.cpp` SHA-256:
`5ff49ec2afcc82da178570524ec72eafaf4f38edc9fb4d3b95d944d041503ae8`.
Full native trial images and source-pinned original-target receipts are in
ignored `.analysis/gpt-web/20261107-sound-{new-reads,format-words}/`,
`build/gpt-web/20261107-sound-{new-reads,format-words}/`, and installed
`build/match-linked/{SoundReopenBgm,SoundInitBuffers}/`.
