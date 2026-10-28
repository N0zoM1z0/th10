# Input polling and gameplay repeat

Three existing authored owners now have complete source in
[InputPolling.cpp](../../../src/InputPolling.cpp): joystick merge44A190/833,
controller button query44A4E0/264 and frame poll44A5F0/850, totaling1947 bytes.
The query closes **264 new authored exact bytes**. Two previously untracked
button helpers44A130/37 and44A160/43 now have reviewed boundaries and maintained
source, but their source-versus-optimizer origin stays indeterminate. They add
no authored denominator or exact credit.

## Target and layout

Fresh local Ghidra readOnly/noanalysis reviews attest all412289 mapped text
bytes401000..465A80, SHA256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
Temporary helper bodies are rolled back and mapped bytes stay unchanged.
Full raw decode, every return, local branch, neighbor padding, actual callers
and selected operands are independently checked by
[the target audit](../../../scripts/report-input-polling.py). Joystick merge
owns833 bytes, including the3-byte nonexecuted LEA alignment44A35D that lies
between Ghidra's830-byte body ranges; no denominator correction is needed.

Both merge and frame poll independently multiply the binding index by6A and
use the existing root474E30. The complete observed record is naturally aligned
as words in [InputPolling.hpp](../../../src/InputPolling.hpp):

| Offset | Observed role |
| --- | --- |
| 0/2 | current/previous gameplay word |
| 4/6/8 | repeated/pressed/released gameplay masks |
| A..29 | sixteen gameplay hold counters |
| 2A | unknown word |
| 2C..57 | independently established debug input/counters |
| 58..69 | nine signed controller bindings |

This complete word view reuses the established global root and is distinct
from the older partial FrontEnd view with its32-bit aggregate alias. The
initial attempted extension of that partial view required pack2 solely because
of the aggregate alias; it is superseded by the naturally word-aligned record.
The original array count, defining storage, common original C++ type and alias/
object-lifetime closure are still unknown. No existing source/header or
canonical primary/support input changes; no historical replay is required.

The imported state layouts are52-byte WinMM info,404-byte capabilities and
272-byte DirectInput joystick state with128 buttons at30. COM Acquire,
GetDeviceState and Poll occupy1C/24/64. Captured globals include capabilities
at4918B8, pad pointers491C3C, keyboard pointer491C38, signed thresholds491D5E/
491D60, backend flags491FF4 and shared buttons497BB0. These are observed views,
not defining data owners.

## Input contracts

The joystick merger receives the existing action mask in private ECX, two
stack indices and RET8, returning the merged word in AX. WinMM calls device
ID`padIndex!=0` but indexes capabilities using the **original** pad index.
Negative mappings are disabled; shot/bomb/menu/focus/skip use entries0/1/3/2/8.
Direction mappings4..7 are preserved but not consumed. Unsigned midpoint and
quarter-range arithmetic gives strict X/Y thresholds. DirectInput uses signed
axes and signed-short thresholds, fresh device reads after Poll/Acquire,
zeroed state and rejects negative GetDeviceState HRESULTs.

A failed Poll attempts Acquire once, then up to400 additional retries while
the result equals8007001E. It returns without a state read even when Acquire
succeeds. The raw button query has this same failure protocol, but WinMM always
uses ID0. It clears the128-byte output and expands32 WinMM bits. After a
successful DirectInput Poll it **does not initialize its local state or inspect
GetDeviceState's result** before copying128 bytes. Preserve that native hazard;
the host fixture supplies written bytes even for its failure-result control.

The two adjacent helpers prove a real return contract: after ORing the selected
action into ushort output, they return the zero-extended action or zero. The
DirectInput helper rereads its button after the write. Their empty entry-xref
sets do not establish source authorship or prove source-versus-optimizer origin.
This raw evidence supersedes the initial void-helper hypothesis. The natural
WinMM C++ shift has defined domain0..31; x86's wider masked-CL behavior does not
establish portable C++ behavior for invalid persisted mappings.

Enabled4924FC gates **keyboard only**; joystick merge always executes. Flag200
chooses GetKeyboardState versus DirectInput keyboard, independently of flag400
for joystick. GetKeyboardState's BOOL is ignored. Every nonzero DirectInput
keyboard result, including a positive HRESULT, acquires once and contributes
no keyboard bits. A successful read maps Z/X/shift/escape, arrow/numpad axes,
control/Q/S/P-or-home/return/D/R, and four diagonals to the15-bit game word.

After all hardware calls, the frame owner publishes previous/current, clears
repeat, and updates all16 counters: held counters increment modulo16 bits;
values26 or above repeat their **own bit** and subtract8; released counters
reset. It finally derives pressed/released edges from the published pair.
This is separate from the old debug routine at40AC20, whose counter region and
bit-zero accumulation quirk remain unchanged.

## Compiler evidence and limits

Two independent complete single-TU canonical /GL /GS cold builds freeze all
three project inputs and reproduce every byte of the264-byte query plus all12
independently target-bound fields. [The receipt](native-proof.json) records
complete comparisons, artifact identities, source hashes and diagnostics.
The real frame owner is the link entry; there is no synthetic caller, code
placement, fake register parameter, forced inlining, volatile or copied bytes.

The final merger865/833 and frame owner854/850 remain nonexact. Controlled
formal-order and stdcall/fastcall hypotheses do not recover the target's
private parameter allocation. Word storage and helper returns follow target
evidence; the diagonal lexical order remains a reference hypothesis. The
actual mapped bits are independently target-established. The
remaining register/scheduling/context differences stay open. No fixed-profile
sweep or normal-COFF substitution is used. Unresolved diagnostic anchors do
not make a runnable product. The new helper origins remain indeterminate.
The current static helpers have no separate contribution in the final PDB.
Original linkage/outlining is therefore an open hypothesis for a later bounded
experiment, rather than an established source fact. The retained canonical
carrier also reproduces the two open owners; superseded controls and the
completed sanitizer carrier are pruned after recording receipts, freeing937405
bytes. Historical diagnostic artifact hashes are not reusable current inputs.
All94 public CI checks pass in one fresh complete run. Current target-only
audit, toolchain, tracking, generated progress, documentation and whitespace
checks also pass; the predecessor helper-padding audit is rerun after CI.

## Host evidence

The test (`e1c4075:scripts/test-input-polling.py`, retired host tooling) runs all three complete
owners and both actual helper bodies. Scripted OS/COM roles exercise both
keyboard mappings for every key/high-bit/toggle case, unsigned/signed strict
axis boundaries, binding stride, 26/8 counters and wrap, edge publication,
failed/positive HRESULT differences,401-call retry caps, fresh device/global
reads, disabled-keyboard joystick behavior, and query output rules. The fixture
uses typed live arrays and keeps the6A layout assertions active; only native
calling-convention/import macros and pointer-size assertions are adapted.

The positive suite and32 strict named-CHECK mutants pass ASan/UBSan/leak
detection. Compile errors, sanitizer failures and unrelated CHECKs do not count
as mutation success. Invalid indexes, WinMM shifts outside0..31, unwritten
failed state data, actual OS devices, native storage/lifetimes and gameplay
reachability remain outside this host evidence. Semantic/product/port gates
are not opened by these tests.

The ignored MIT [reference](https://github.com/levzzz5154/th10-decomp) is pinned
at c6791d82dc9f02e55eca45a5314f900a85481efd. Its input source supplied hypotheses,
with [MIT attribution](../../licenses/th10-decomp-MIT.txt). Forced ABI casts,
packed64-bit argument adapters, synthetic callers and code placement are not
adopted. TH09 ControllerInput.cpp supplied adjacent signature/helper hypotheses;
all accepted TH10 observations and credit above are TH10-local. Pre-existing
D3D evidence is untouched.

```sh
scripts/repo-python scripts/report-input-polling.py
scripts/repo-python scripts/test-input-polling.py --sanitize
scripts/repo-python scripts/replay-exact-units.py --unit input-controller-buttons --json
```
