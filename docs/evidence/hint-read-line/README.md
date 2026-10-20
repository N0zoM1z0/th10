# Hint line-reader ABI and loader value shape

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent source checkpoint: `85e41f1`.

## Complete line-reader replay

HintReadLine at419F40 owns260 bytes. Both real LoadHintFile calls4199FD/419BE3
push capacity, remaining-count pointer and line buffer, carry cursor in EAX,
and do not clean arguments afterward. Both target exits use RET12. Former
cdecl source emitted256 bytes; apart from return cleanup, its instruction
stream already matched. Ordinary four-argument static stdcall source lets
LTCG select EAX and recovers the complete260-byte owner.

Two independent full cold builds reproduce all260 bytes and all4 fields:
three calls to the independently reviewed CRT strchr452730 entry and one
explicit static call to the complete raw-equal114-byte trim41A0A0 owner.
Qualified MAP/PDB ownership selects the entire static contribution, with no
retention wrapper or cropped extent. New exact credit is260 authored bytes.

The declared graph is Hint.cpp plus FileSystem.cpp, PbgArchive.cpp and Main.cpp,
all under the pinned VC7.1 SP1 build6030 /GL /GS profile with LoadHintFile as
the real entry. In the former two-TU context, OpenFile selected EBX for sizeOut;
the target takes it on the stack. The real Main/checksum/FileSystem graph
restores the entire existing360-byte OpenFile owner and all26 fields. This
is a reproduced context, not recovery of original production partitioning.

Both images also pass five complete existing guards: split75, trim114,
lookup98, clear79 and OpenFile360. The combined proof is six owners986 bytes
and all36 fields. All18 recursive project-input hashes and all six owner
machine hashes agree between colds. See [native identities](native-summary.json).

```sh
scripts/repo-python scripts/replay-exact-units.py --unit hint-read-line --json
scripts/repo-python scripts/test-hint-load.py
```

Existing guard recipes retain their original canonical contexts. The evidence
uses explicit same-image source/profile/output overrides for those five guards;
only the new line-reader recipe needs this four-TU cold. The full historical
exact cohort is not rebuilt. Private commands and source controls are under
`.analysis/gpt-6.1-sol/hint-read-line-20261020/`.

## Packed color and floating-point observations

The target entry constructor writes packed BGRA at418D18 after the self link.
The source now exposes the same four bytes as a packed unsigned value and the
existing byte components. The constructor shrinks72->51 bytes. It is still
nonexact: the compiler schedules the self-link write later, with37/51 bytes
matching. Its authored-versus-generated origin remains indeterminate; no
constructor credit is added.

At419EA5/419EAB/419EBA the target uses dword operands for the unsigned-length
correction, stored scale and7.5. Only419EC6 stores the width result to float.
The prior explicit-double source generated qword constants and preloaded
scale. Ordinary float expression source recovers the observed FPU operation
shape. In the four-TU context the whole loader shrinks1535->1533 bytes versus
1498 target; it remains nonexact.

The actual-source host parser test now requests x87 evaluation. Default host
SSE fails the existing length3/scale1.829711675643921 witness by rounding an
intermediate product. Under the host x87 model the same witness passes, and a
new explicit volatile intermediate-store mutation is rejected. A reference
width in the fixture is stored to float before comparison so excess precision
does not compare a stored value against an unrounded expression. No tolerance
or expected numeric witness is relaxed. All22 strict controls pass.

This host arithmetic model does not establish the original game's x87 control
word, actual native runtime behavior, universal floating-point equivalence,
whole-product closure or the original source types. The three large interpreter
owners remain nonexact. The loader, constructor and append helper retain their
separate open states.

All97 standard CI checks pass after the final source freeze, along with the
actual-source parser baseline and22 strict mutants, toolchain declarations,
tracking, generated progress and whitespace checks. All18 raw input hashes
were rechecked after validation. See the
[validation summary](validation-summary.json). Five inspected prototype build
directories were removed, reclaiming1456713 bytes; final colds and source/report
controls remain.
