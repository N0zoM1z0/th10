# Complete archive registration wrapper

`FileSystem::LoadArchive` at `0x004357D0..0x004357F3` owns 36 bytes and four
linked fields. The contract targets only original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target selects archive row `g_PbgArchives[g_PbgArchiveCount]`, calls Load,
increments the count only for a nonzero AL result and returns AL=1/0. The row
stride is 16, array base `0x004923B0` and count address `0x00477850`. Filename
arrives through EAX and the selected row through ECX. No stack parameter,
capacity check or original source-level ABI is inferred from that machine view.

The maintained change is an ordinary positive-if branch: load successfully,
increment and return true; otherwise return false. The previous early failure
return produced 34 bytes in this graph. The success-first source recovers the
target branch ordering and explicit false-result clear, reaching all36 bytes.
Signatures, headers, globals, layouts and compiler profiles stay unchanged.
No register forcing, fake return, target patch or padding is involved.

One private control compiles only FileSystem and reuses three attested source
objects. It passes36/all4 and eight existing whole guards920/all48. Two other
declared forms are intentionally unexecuted after that first match. This is
routing evidence, separate from the subsequent two full four-TU canonical
colds. Each actual cold compiles current FileSystem, PbgArchive, PbgFile and
normal-COFF ResFile. The existing GL/GS/EHsc and GL/Ob1 archive profiles remain.
Each checks new36/all4 plus eight guards920/all48, totaling956/all52. Only36
bytes and one function are new credit; all1246 previous unit dictionaries remain.

Independent target decoding covers all11 instructions/36 bytes and the complete
three-DIR32/one-REL32 inventory: count read, array base, Load call and count
increment. The existing boundary/origin rows remain unchanged. The fresh local
archive review attests all412289 mapped text bytes under readOnly/noanalysis
rollback; it has no Ghidra function record at4357D0. Raw target bytes and the
whole native comparison supply coverage without inventing a Ghidra boundary.

The unit shares the identical four-TU build/output group. Before replacing the
canonical carrier, the prior archive108 second cold is archived and hash-checked;
both historical archive108 and Read cold pairs remain retained. The private
one-input experimental carrier becomes disposable after actual cold acceptance.

Reproduce twice:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit filesystem-load-archive --json
```

[facts.json](facts.json) records the full contract, input hashes and both actual
carriers. [validation-summary.json](validation-summary.json) records final checks
and cleanup. All111 CI groups pass with373 final source/test/config inputs
unchanged;12 pinned superseded files/1642902 logical bytes are removed.
Private compact sources/producers/results live under
`.analysis/gpt-6.1-sol/filesystem-load-archive-20261022/`, with the preceding
private positive-if probe under `archive-load-exact-20261022/`. Keep the first
actual cold at its matching build path and the second canonical carrier.

Original identifiers, source/TU and production flags remain unknown. Complete
FileSystem source presence is13/13, exact11/13; transforms323/307 remain
nonexact. Whole product/runtime, semantic reconstruction and portability remain
open; the diagnostic link is not a runnable game.
