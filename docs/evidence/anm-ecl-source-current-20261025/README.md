# Source-current shared ANM/ECL baseline, 2026-10-25

## Complete comparison result

One fresh shared ten-TU build at source checkpoint **470dca2** retains both
parked residuals. The supplemental comparison and independent same-image replay
pass and return identical results:

| Complete owner | Matched bytes | Ordinary differences | Bound fields / fixed locals |
| --- | --- | --- | --- |
| ANM ExecuteScript, 9964 bytes | 9960 | 4 | 350 / 95 |
| ECL Run, 7020 bytes | 6995 | 25 | 189 / 62 |

Neither owner is exact. ECL's full owned extent includes its trailing selector
byte at offset 7019, explicitly retained as data; the complete selector domain
and fixed table/local destinations are not cropped or normalized away.
**Zero new exact bytes, source owners or denominator bytes** are credited.

All **36 ANM guard checks** (4618 bytes / 121 fields), **16 ECL guard checks**
(1617 / 21), and the 171-byte float-value callee / all 4 fields pass. The group
counts overlap: nine guards occur in both groups, so the 52 group checks cover
**43 unique nonoverlapping physical extents**, 5098 bytes. The 171-byte callee
is already among those shared guards, not an additional physical owner.

## Actual input change and failure boundary

The shared native closure has ten translation units and 32 project files.
Only Enemy.cpp's two explicit operator-delete corrections and
GameErrorContext.hpp's nonvirtual Flush declaration changed from the retained
producer. Main.cpp, Gui.cpp and Chain.cpp are not compiled in this graph.
Profiles remain `/GL /GS`, with only AnmInt3Scale adding `/Ob1`; no source,
ABI, profile or historical comparator contract was changed for this baseline.
[Summary JSON](summary.json) retains exact graph/order, source pins and receipts.

The original **root and cold exit 1 remain failures**: after `cold_link`
successfully returned, the runner could not JSON-serialize a returned PosixPath.
It had already produced the usable native image. The frozen runner, original
failure receipt/logs and native artifacts remain unchanged. A **new ignored
supplemental observer**, not a patched runner or repeat build, reconstructs
metadata from those artifacts and runs two separate comparison processes, both
exit 0. This is independent replay of one image, not a second cold build.

Fresh image SHA-256:
`66f4f95c25b7486b6b862c0f17e6d4f8380927a055458c3bd74c2a7065e56947`.
The baseline did not force the old 4/25 result; structural failures would have
rejected it. The unchanged residuals are now source-current observations.

## Durable receipt references and next boundary

Ignored packet `.analysis/gpt-dots/anm-ecl-source-current-20261025/` retains:

- `freeze.json`, original `receipt.json`, `root.exit`, `cold.exit` and logs.
- `supplemental-serialization/receipt.json`: successful validation and identical
  independent results, SHA-256
  `7260d7308aafbf8d304f5a6a14478ec4e864f098e6e697594f18cc7ecf2b9b41`.
- `supplemental-serialization/reconstructed-completion.json`,
  `preserved-inputs.json`, and both complete comparison result directories.

These ignored artifacts are not supplied by a Git clone. No disassembly or
target payload is copied into this evidence directory. Source checkpoint
470dca2 already passed all **112 CI stages** at 2026-10-25 00:36:31 UTC; this
follow-on documentation-only checkpoint uses focused documentation checks,
**not a new full-CI claim**. No source/configuration or metric change occurs.

Keep core, HUD and WinMain parked against their previous unsupported panels.
Read-only follow-up found the exact SDK8-inspired vector storage-inheritance
experiment had already recompiled eleven TUs consistently and was byte-neutral
on October 21; that is not a new premise. EndingRun likewise has no new semantic
factor. Its latest actual 18-TU producer now includes 57 files: 22 of 52 older
files changed and five headers were added, with Ending.cpp itself unchanged.
One source-current Ending measurement is being prepared, not a source trial.
Require new evidence and a falsifiable prediction before reopening source edits. Runtime closure and original source/profile
identity remain unproved.
