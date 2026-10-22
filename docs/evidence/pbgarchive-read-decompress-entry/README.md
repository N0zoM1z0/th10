# Complete archive entry reader

`PbgArchive::ReadDecompressEntry` at `0x00434DD0..0x00434EE8` covers 281 bytes
and ten linked fields. The target is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ordinary maintained change reads the filename length before initializing
its checksum and passes the four indexed profile fields directly. There is no
signature, header, layout, global, algorithm, inline-control or target change.
The declared four-TU context consistently uses `/Gz`, retaining `/GL /GS /EHsc`
and the archive support's `/Ob1`; ResFile remains normal COFF with the same
default-convention context. This reproduces a compiler context, not original
production flags or a source-level calling-convention attribution.

The investigation distinguishes three actually executed controls:

| Control | Actual compilation | Reader result |
| --- | --- | --- |
| Consistent four-TU `/Gz` | All four TUs | 281 bytes; rejected nine-field inventory |
| Direct indexed profile arguments | Archive only; three source objects reused | 246/281 equal, all ten fields bound |
| Checksum initialization after length | Archive only; three source objects reused | 281/281 equal, all ten fields bound |

Nine existing complete guards, 956 bytes/all52 fields, pass in each control.
The declared late-buffer and combined-lifetime forms are unexecuted. The
pre-existing transform bodies are284 bytes each with caller cleanup; the `/Gz`
control produces286 each with RET20, still unequal to targets323/307. ParseHeader
is355 versus350. None of those nonexact functions gains credit from this packet.

Fresh local Ghidra reviews both transforms, all six decrypt and three encrypt
recorded call sites, ParseHeader and this reader under readOnly/noanalysis,
maxcpu1/heap768 MiB, rollback. All412289 mapped text bytes remain unchanged.
Target callers pass the seed through AL and five remaining stack arguments;
callee RET20 and subsequent caller instructions independently establish stack
cleanup. Those machine observations do not uniquely identify original source
annotations or flags. Supplementary untyped address surveys establish no owners.

The reader's complete field inventory is six REL32 calls: lookup, allocation,
two frees, decrypt and decompression. Four DIR32 fields select the existing
profile-table view at474BD8 plus8/4/1/0. All ten fields and all281 owned bytes,
including compiler alignment, are compared. Callee bodies, profile contents,
data ownership and runtime receive no duplicate credit.

Two subsequent independent canonical builds each compile all four actual
maintained inputs. Each passes the reader281/all10 and nine whole guards956/all52,
totaling1237/all62. The guards cover lookup212, memory Open46, entry size51,
FindEntry53, file destructor46, path Read214, path size190, archive Load108 and
registration36. The wrapper guard uses its actually observed `/Gz` decorated
symbol; old canonical recipes remain unchanged. The affected original-profile
path Read/GetSize also receive a separate fresh four-TU cold comparison.

Reproduce the new unit twice and the affected original-context pair once:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit pbgarchive-read-decompress-entry --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-path-read --unit filesystem-path-size --json
```

[facts.json](facts.json) records the recipe, inputs, controls and actual native
carriers. [validation-summary.json](validation-summary.json) records final checks
and cleanup. All111 CI groups pass with373 final code/test/config inputs
unchanged. Cleanup removes33 pinned superseded files/493754 logical bytes.
Private compact evidence is under
`.analysis/gpt-6.1-sol/filesystem-transform-context-20261022/`. Keep first actual
new-unit cold at its `accepted-cold-1` build path and second at the canonical
`PbgArchiveReadDecompressEntry` directory. Preserve the separately replayed old
context and its archived predecessor before removing superseded controls.

Only281 new authored bytes are credited. Original names/class/lifetime/TU/flags,
physical data owners and whole Windows i386 product/runtime remain unknown.
Semantic reconstruction and portability have not started.
