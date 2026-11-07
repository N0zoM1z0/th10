# TH10 ECL AddScriptData: native sorted-record mover addresses

Target original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target's complete 589-byte `EclVmScriptDatabase::AddScriptData
@0x00450220` merges serialized SCPT named subroutine records into a
sorted eight-byte-entry table. During insertion, the existing source
moved entries using `subroutines[index] = subroutines[index - 1]`.
The indices derive from on-disk untrusted record counts; those C++
array expressions can leave the abstract array for an oversized index.
Retail Win32 instead forms its addresses as 32-bit words multiplied by
the eight-byte stride. The maintained source now obtains both the
moving destination and preceding entry through the existing typed
`EclMutableEntryAtWord`, with a native 32-bit `index - 1` word, then
copies the complete entry through natural C++ struct assignment. There
is no new clamp, altered record ordering, or claim that invalid records
are safe; this is a Win32 target source model.

## Actual two-input current-source original-target trials

The real VC7.1 SP1 `/GL` EclVm.cpp + Enemy.cpp graph was cold built
from current source before editing. Four natural typed pointer-copy
variants were linked and compared as **complete functions** with the
original TH10 target: direct helper references, base-word preceding
pointer, typed `memcpy`, and named destination/previous locals.

The first three remained 589-byte PDB owners with the two original CRT
call-field positions, but regressed `AddScriptData` ordinary target
matches from **502/581 to 439/581**. They were rejected. The named
local pointer form preserves **502/581** ordinary bytes and the two
original `REL32` field positions at `+0x6C,+0x101`. This is the installed
version; the 79 other original mismatches remain and the function stays
**NONEXACT**. None of these variations wins new codegen exact credit.

Each experiment also strictly checked the complete original-bound
7,020-byte `EclVmContext::Run` through the real ECL/Enemy graph. The
installed, neatly formatted source independently preserved **6,997/
7,020** original bytes and all **189 target-bound fields** (same 23
ordinary mismatches); its 171-byte ReadFloatValue callee was exact.
The diagnostic tool returns a nonzero result for the expected NONEXACT
runner, not because the native compiler or linker failed.

After installing the final source, a separately selected original-bound
cold replay of **five previously exact owners** passed:

| Existing exact owner | Target bytes |
| --- | ---: |
| EclVmHost::Run | 146/146 |
| EclVmScriptDatabase::FindSubroutine | 136/136 |
| EclVmContext::ReadFloatValue | 171/171 |
| EclVmStackView::Push | 96/96 |
| EclVmStackView::Pop | 122/122 |
| **Total** | **671/671** |

Reproduce only these relevant exact units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-find-subroutine \
  --unit ecl-vm-read-float-value --unit ecl-vm-stack-push \
  --unit ecl-vm-stack-pop --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Private trial sources/whole-PE/PDB/MAP and full original-bound receipts:
`.analysis/gpt-web/20261107-active/ecl-sorted-entry-move-words.py`,
`ecl-sorted-entry-move-results.json`,
`ecl-native-sorted-copy-final-addscript.json`,
`ecl-native-sorted-copy-final-run.json`,
`ecl-native-sorted-copy-final-replay.json`, and
`build/gpt-web/20261107-active-ecl-sort-move/`.
The installed `src/EclVm.cpp` SHA-256 is
`0e94c3f1b23ae662bead5c35dbf24725f31799a9eed4e6d02813e9b514a0226b`.
No full historical replay, complete executable, or runtime equivalence.
