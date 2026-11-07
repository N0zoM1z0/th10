# TH10 archive discovery/registration: native 0x10-byte object address words

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The reviewed `ArchiveOrDiskPathView::FindArchive @0x004356F0` is a
**212-byte** previously accepted exact owner. `FileSystem::LoadArchive
@0x004357D0` is a **36-byte** previously accepted exact owner. They
share the global PbgArchive array (0x10 bytes per record) and a signed
32-bit active archive count.

The original i386 archive search uses a pointer stride `ADD EDI,0x10`
and increments the loop counter with native `INC`. Source ordinary
`++archive`, `++index` may form pointers outside a C++ array and
invoke signed-overflow undefined behavior when the file-controlled
active count is invalid. The maintained C++ now advances the typed
PbgArchive cursor through its low-32-bit address plus `sizeof(PbgArchive)`
and publishes an unsigned 32-bit counter increment back to `int`, while
keeping the original signed search bound and lookup order unchanged.

The original loader addresses a slot using `SHL ECX,4` and adds the
array base, then increments the stored count through `INC dword ptr` if
Load succeeds. Its old `g_PbgArchives[g_PbgArchiveCount]` and signed
`++g_PbgArchiveCount` are now explicit native modulo-32-bit typed
record addressing and increment. No new max-archive check, error
handling, or safe behavior on malformed counts was invented; this
models the original native target's unchecked arithmetic.

## Whole-function native controls

Seven different source variants were actually compiled against the
original bytes: find-index, find-cursor, find-both, load-slot,
load-count, load-both and the installed all-four combined form. Each
variant was built in **both** correct VC7.1 SP1 four-TU contexts:

- `filesystem-find-archive`: `/GL /GS /EHs`, real
  `ArchiveOrDiskPathView::Read` entry, FileSystem + PbgArchive + PbgFile
  plus normal COFF ResFile support
- `filesystem-load-archive`: `/GL /GS /EHsc`, same real entry with
  separate `/Ob1` PbgArchive support as the canonical recipe declares

For **all fourteen** full native codegen comparisons, `FindArchive`
reproduces **212/212 original bytes and 7/7 original fields** and
`LoadArchive` reproduces **36/36 and 4/4**. No PDB extent, relocation,
local address or branch is excluded from these strict results.

The **installed combined source** was independently cold rebuilt and
verified with three additional directly affected existing exact owners:

| Existing exact owner | Original bytes | Original-bound fields |
| --- | ---: | ---: |
| FindArchive | 212/212 | 7 |
| LoadArchive | 36/36 | 4 |
| ArchiveOrDiskPathView::Read | 214/214 | 14 |
| ArchiveOrDiskPathView::GetSize | 190/190 | 13 |
| PbgArchive::Load | 108/108 | 7 |
| **Total** | **760/760** | **45/45** |

This preserves already accepted exact owners. **No new codegen exact
function or byte credit**, full historical replay, whole executable or
runtime-equivalence claim is added.

Reproduction, scoped to these selected canonical units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-find-archive --unit filesystem-load-archive \
  --unit filesystem-path-read --unit filesystem-path-size \
  --unit pbgarchive-load --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial runner/receipts and PDB/MAP/PE artifacts:
`.analysis/gpt-web/20261107-active/filesystem-archive-native-word-probes.py`,
`filesystem-archive-native-words-results.json`,
`filesystem-native-words-final-replay.json`, and
`build/gpt-web/20261107-filesystem-archive-words/`.
Installed `src/FileSystem.cpp` SHA-256:
`075bd45019de1795ba39547412ce570c9d61bac489da12105f6e1f3bad2e9711`.
