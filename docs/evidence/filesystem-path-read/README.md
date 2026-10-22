# Complete archive-or-disk read

`ArchiveOrDiskPathView::Read` at `0x00435800..0x004358D5` is now exact over
its complete 214-byte target extent and all 14 linked fields. This adds one
function and 214 authored bytes. The target is original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained source uses one result variable across the archive and disk
branches, with the automatic `CPbgFile` confined to the disk branch. Compared
with the preceding early-return form, this recovers the target's register and
return ordering. It preserves archive failure handling, archive-only optional
buffer forwarding, ignored disk Open result and real file destruction. No
layout, signature, virtual, global, profile or target byte changes are involved.
This establishes a reproduced source form, not the original spelling or class.

Five private controls were actually executed. G7, a direct suffix expression,
archive-first declarations and a real normal-caller link entry retain 200/214
equal bytes with all 14 fields bound. The shared-result form reaches 214/214.
Each retains six existing complete guards, 598 bytes/all 27 fields. The first
four compile selected inputs and reuse attested support objects; the caller
entry control only relinks. These experiments are not full cold builds.

Two subsequent independent canonical builds each compile all four actual TUs:
FileSystem, PbgArchive, PbgFile and normal-COFF ResFile. The declared recipe
retains `/GL /GS /EHsc`, with `/GL /Ob1` PbgArchive support. In each image:

| Complete contribution | Bytes | Fields |
| --- | ---: | ---: |
| New Read | 214 | 14 |
| Existing GetSize | 190 | 13 |
| Existing FindArchive | 212 | 7 |
| CMemoryPbgFile::Open | 46 | 2 |
| PbgArchive entry size | 51 | 1 |
| PbgArchive::FindEntry | 53 | 1 |
| CPbgFile destructor | 46 | 3 |
| Total checked per image | 812 | 41 |

The two parents share one build recipe and artifact; this is two full four-TU
builds, not four. FindArchive additionally passes a fresh cold under its own
unchanged original canonical recipe, 212 bytes/all seven fields. All 1,244
previous canonical unit dictionaries remain unchanged. No broad historical
replay or large-owner cold is claimed.

The 14 Read fields are exhaustive: nine DIR32 and five REL32 fields. The
existing explicit stack-file EH binder validates the same-parent generated
18-byte region, the real PbgFile destructor and immutable one-state 28-byte
FuncInfo protocol. It binds an address only; generated EH code and metadata
receive no separate exact or ownership credit.

Fresh local read-only/noanalysis Ghidra inspection attests all 412,289 mapped
text bytes unchanged before and after rollback. It reviews the path/caller,
callee and EH contexts without inventing missing function records. The source
host contract passes 112 assertions and eight compiled strict controls through
explicit archive/disk seams; this is no Windows ABI, lifetime or runtime proof.

Reproduce the focused native acceptance with:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-path-read --unit filesystem-path-size --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-find-archive --json
```

Run the first replay twice for independent colds. [facts.json](facts.json)
records the recipe, input hashes, field contract and both actual carrier hashes;
[validation-summary.json](validation-summary.json) records the passed 111-group CI with 373 unchanged final code/test/config
inputs and final checks. Private
compact producers/results are under
`.analysis/gpt-6.1-sol/filesystem-read-registers-20261022/`. Keep its first
actual cold and the second canonical carrier. The five superseded experimental
carriers may be removed after their hashes and compact results are retained.

Original names, class/lifetime, source/TU ownership, production flags, data
owners and native product/runtime remain unknown. Source presence remains
613/734; authored exact becomes 404/73,841 bytes. Whole-product, semantic and
portability gates remain open.
