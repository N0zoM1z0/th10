# Replay read-open calling convention

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `c7fddc3`. Reviewed on 2026-10-21.

`ReplayFile::Open44B6B0..44B738` becomes exact: **137 bytes/all12 fields**.
Only its declaration and definition gain ordinary `__stdcall`. No boundary,
origin, source mapping or authored denominator changes. The three original
large functions remain nonexact; product/runtime/semantic/port gates stay open.

## Target and compiler evidence

Fresh direct local Ghidra uses readOnly/noanalysis, rollback, CPU1 and768MiB.
Disk identity and the complete inclusive mapped text401000..465A80 agree
before/after:412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The137-byte owner ends in RET4, before sevenCC bytes. Its sole real caller is
LoadReplay's call42A269. The target's failure-message pointer occupies the dead
path parameter's former stack home; no separate local push is present.

The complete target read opener differs from the already exact137-byte write
opener at only three constant bytes: offsets16/18/2C. These express sequential
scan, OPEN_EXISTING and GENERIC_READ. Their twelve global/IAT fields have the
same offsets and target bindings. This is direct TH10 evidence; adjacent-game
source is not substituted for it.

The old cdecl definition produces135 bytes even when compiled with the actual
ReplayManager.cpp and real LoadReplay entry. It reserves a separate local with
PUSH ECX and uses POP ECX/plain RET exits. The ordinary stdcall source naturally
reuses the parameter home and emits both target RET4 exits. No fixed registers,
assembly, dummy parameters, padding or target changes are used. The matching
annotation is a compatible source model; original spelling/TU/profile remain
unknown.

## Complete validation

Two independent complete FileSystem.cpp + ReplayManager.cpp /GL /GS cold links
through real LoadReplay freeze the same18 recursive raw inputs. Each strictly
replays the new137/all12 plus four existing owners359/all35: Read74, write-open137,
file-check111 and close37. Thus each image passes five complete owners496/all47.
The second image is the canonical replay-file-open graph. The
[native receipt](native-summary.json) records source/artifact identities and all
field bindings.

Focused canonical replay selects12 directly affected FileSystem/ReplayManager
units,1541 bytes, retaining their declared contexts. The existing Read recipe's
entry decoration changes consistently with the new declaration; its original
single-TU /GL context stays intact. No full historical replay is run. The new
caller-context loader is458/450 bytes, versus468/450 before the annotation;
LoadReplay itself remains nonexact and receives no credit. The separately
rebuilt eighteen-TU /EHs context gives406/450; no before/after improvement
is inferred across different graphs.

Three controls reject: old cdecl source has no complete137-byte extent; the
wrong CreateFile IAT binding fails; a copied-native GENERIC_WRITE byte yields
136/137 instead of exact. The original executable is never patched. Final
checks and attributable cleanup are in the [validation receipt](validation-summary.json).

## Large owners and restored ECL controls

FileSystem.hpp changes one of the actual nine-TU graph's30 inputs. Only
AnmManager.cpp consumes it. A necessary recompilation of that support verifies
all other input closures and eight reused objects, then relinks the real graph.
The three complete native large bodies remain equal after verified field
rewrites;21 complete prior guards retain2415/all52. This is an incremental
dependency check, separate from the two complete opener colds. ANM remains9964
with90/92 target-sized groups/gap2; actual nine-TU Run7020/all189 retains27
ordinary differences; dispatcher remains14468/14416/gap652/frame2C4/19 sentinels.
None is promoted.

A complete bounded static CFG traversal also corrects the interpretation of
1B4's physical call count. Both target and current native cases reach exactly
three calls to the independently exact144-byte generic ReadInt on every static
return path. Target sites41054F/410580/4105BE all lie inside1B4. Native sites
4045E0/404625 lie inside1B4, while404A4C lies in1AA and is reached by a shared
tail. The181-selector domain, physical group order and complete pre-table
instruction coverage are checked. Calls are treated as returning and branch
predicates are unconstrained. This establishes static sharing, not argument
or runtime equivalence, and gives no exact credit. Adding a fourth source read
to repair a two-versus-three physical-region count would be unsupported.

The distinct integer-value signature `flag, context, value` is now tested in a
consistent private two-TU cold, rather than repeating the old context-first
control. Both complete124/all1 and171/all4 value callees and five other helpers
pass919/all17. Full Run7020/all189/62locals retains26 ordinary differences and
is byte-equal to the old baseline after every verified field rewrite. Moving
the null guard before the const cursor declaration retains7020/all189 but
increases differences to61. Both controls are rejected; production EclVm/Enemy
source and headers are unchanged. The preliminary mixed-header build is excluded
from this evidence. Private exact source snapshots and reports remain; terminal
rejected native products are pruned.

## Reproduction

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit replay-file-open --unit replay-file-read --json
```

The whole Windows i386 product remains open. Diagnostic anchor images are not
runnable game products; byte exactness supplies no original-runtime observation.
