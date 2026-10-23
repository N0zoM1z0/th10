# Stage registration, Update and timer closure (2026-10-19)

Context update2026-10-23: the [Stage color checkpoint](../stage-color-storage/README.md)
refreshes the current nine-TU graph with21 source/header inputs and all11 whole
guards987/all36 passing. Only opcode8's color view changes maintained source;
parent stays1868 and its structural positions improve1236->1241/1724. The pins
and measurements below remain historical, not current-source input hashes.

The genuine StageCreate/registration graph now closes **376 authored exact
bytes** and **21 origin-indeterminate exact bytes**. Two independent cold
CL13.10.6030/LINK7.10.6030 links, with the same19 project input hashes, pass
every complete owner and all18 independently target-bound fields. Existing
Fog/object-VM and SetCurrent guards retain590 bytes/all18 fields in both images.
The canonical default paths also pass without another native build.

| Target | Maintained owner | Bytes | Fields | Origin disposition |
| --- | --- | ---: | ---: | --- |
| 0x402720 | StageScriptView::Update | 304 | 11 | authored, unchanged |
| 0x404ED0 | AnmVmTimerView::Tick | 72 | 4 | authored, unchanged |
| 0x403050 | StageUpdateCallback | 7 | 1 | indeterminate |
| 0x403060 | StageDrawEarlyCallback | 7 | 1 | indeterminate |
| 0x403070 | StageDrawLateCallback | 7 | 1 | indeterminate |

## Target observations and source corrections

Read-only local Ghidra reviews attest both the original disk target and full
mapped `.text` before and after analysis; all temporary changes roll back.
Pinned Capstone and independent target operand decoding agree. The reports
in `target-fields.json` and `target-ownership.json` distinguish these observations
from native compiler results and unknown original declarations.

Update takes the stage in EAX and returns with plain RET. Three explicit real
auxiliary ExecuteScript calls replace the three-iteration loop. This naturally
recovers the target camera-publication/store scheduling and all304 bytes in
the real caller graph. No memcpy, volatile, padding or artificial callback
root is needed. The four ANM calls, normalization call, object updater,
dispatcher, speed global and published-camera global are independently bound.

Tick takes its timer in ESI. The target's72-byte body lacks the fast-path
`MOV EAX,EDX` added by the old74-byte `int Tick()` candidate. All10 incoming
target call sites were reviewed: their postcall paths overwrite EAX before use.
The maintained method now returns void; every mutating statement is unchanged.
This models an unconsumed result and does not prove the original C++ return
spelling. Three float constants and the conversion call are exhaustively bound.
The inline definition remains in AnmManager.hpp; the selected PDB contribution
is materialized in primary StageScript.cpp, which is the ledger's native source
mapping. Original class names and production TU ownership remain unknown.

`tick-header-review.json` proves that substituting the old Tick block restores
the whole prior normalized AnmManager.hpp hash. The strict transition fixture
pin is updated only for this reviewed change. Other timer, viewport and RNG
dependencies and rejection policies remain unchanged.

The real initializer takes the addresses of the adapters at0x402355,
0x402388 and0x4023AD. Update's adapter moves ECX to EAX then jumps to0x402720;
the draw adapters push ECX, call their existing stack-receiver cores and return.
Each seven-byte extent has a separate entry and nine following CC bytes.
Natural source wrappers reproduce these bodies, but a source-written wrapper
and compiler/LTCG-generated adapter remain plausible. Their origin therefore
stays unknown/indeterminate. The draw cores remain nonexact.

## Complete dispatcher ownership and remaining mismatch

StageExecuteScript owns0x403C80..0x4043CB, **1868 bytes**:1806 code, two-byte
MOV EDI,EDI alignment, and15 four-byte destinations at0x404390. The unsigned
selector bound is14, with EAX*4 indexing; every table destination is an
instruction start inside the code. No other ledger owner overlaps the tail.
The target PE has no base relocation directory. A whole-file literal scan
finds the actual indexed-jump operand and one incidental0x4043C7 byte pattern
at0x41522F. The latter crosses opcode/modrm/displacement/immediate bytes of
`MOV dword ptr[EBX+0x40],0`, rather than an address operand. Computed references
are not excluded by this literal scan. Ghidra finds no independent table owner.
This corrects the code-only ledger, adding62 authored denominator bytes and
zero dispatcher exact bytes.

The maintained rocking logic now uses an outer nonzero test and switch case1.
It naturally outlines the actual Tick and SetCurrent calls; the compiler emits
FSIN for the existing sine expression. Target-backed corrections also re-read
the case1 jump operand after SetCurrent, set mode8 before the final tangent in
cases10/11, and put VM initialization first in case14. No ABI keywords change.

The selected native dispatcher has the target1868-byte contribution and0x8C
frame, but remains nonexact: the dated diagnostic is1236/1724 ordinary
comparable positions. Opcode8 byte-to-float scheduling,10/11 vector temporary
registers,12 byte registers,13 global-store register, and14 VM initializer
receiver still differ. Target-sized output earns no whole-owner or case credit.
The former current-frontier claims of an EBX/plain-RET dispatcher and ECX
Update are obsolete for this genuine nine-TU graph.

Controls are pinned in `native-summary.json`. Four color-channel snapshots
expanded the frame to0x98 and owner to1892; they are removed. A same-layout
argument-slot union was byte-neutral and is restored. Adding the real bullet
caller of VM initialization was also byte-neutral and is not retained. Fresh
target xrefs find23 VM initializer calls across eight callers; EnemyBeginSpell
has15 of them. A subsequent ten-TU control adds its genuine EnemyEclDispatcher.cpp
source, pinning27 inputs. The normalized dispatcher instruction/size sequence
is identical to the selected graph; all987 bytes/all36 fields still pass.
That caller-context hypothesis is closed without changing the canonical graph.
EnemyBeginSpell itself is2476/2432 with771/2032 ordinary comparable positions
in this dated context, a separate future owner rather than exact credit.
Repeated caller/flag/ABI sweeps are unwarranted.

## Reproduction and limits

Primary StageScript.cpp links with these supports in order: StageLifecycle.cpp,
StageDraw.cpp, Chain.cpp, AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp,
RandomMath.cpp, FileSystem.cpp. All nine use
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`; the genuine entry is StageCreate.
`native-summary.json` pins the graph,19 input hashes, both PE hashes/PDB identities,
complete strict comparisons, tool hashes and scoped control outcomes. Tick's
SetCurrent guard is existing credit and is not added as another canonical unit.

```sh
scripts/repo-python scripts/build-match-unit.py --unit stage-update
for unit in stage-update anm-timer-tick stage-update-callback stage-draw-early-callback stage-draw-late-callback; do
    scripts/repo-python scripts/compare-linked-function.py --unit "$unit" --json || exit 1
done
```

Reference source at `_reference/th10-decomp` commit
`c6791d82dc9f02e55eca45a5314f900a85481efd` supplied three-call, rocking-switch and
void-timer hypotheses under its retained MIT license. Every adopted conclusion
was checked against TH10 target and native evidence. Its assembly, volatile,
fake-slot and other exactness workarounds are not adopted.

Factory81/151, constructor291/188, initializer474/522, loader319/308,
destructor451/499, Fog691/688 and easing612/576 remain nonexact in this graph.
Unresolved external symbols are diagnostic data anchors; the linked image is
not executed. Published-camera aliasing, whole-product build/runtime, semantic
and portability closure remain open. Native links run serially at nice10;
Ghidra is limited to768MiB and one active processor. Duplicate cold/control
images and superseded source snapshots are removed after verification. The
canonical image and only the PE/MAP/PDB for unresolved EnemyBeginSpell remain.

Final canonical replay, toolchain declaration check, tracking, generated
progress, complete public CI and whitespace checks pass. CI includes843
transition assertions and19 EOL/pin controls with the reviewed Tick header.
About6.2MB of duplicate intermediate artifacts and source snapshots are removed;
the unrelated pre-existing D3D experiment is untouched.
