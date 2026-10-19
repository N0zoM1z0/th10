# Script browser worker and Player option rebuild interface

This checkpoint connects the maintained browser to real worker Start/Stop
bodies and corrects the observed Player option-rebuild calling interface.
Existing boundary and origin classifications, including the workers' medium
origin confidence, are preserved. Source names describe the reviewed prefix;
they do not identify the original class, annotation or production TU.

## Complete native owners

| Target | Maintained function | Bytes | Bound fields |
| --- | --- | ---: | ---: |
| `0x40A010` | `ScriptViewerSuspendPlayer` | 36 | 1 |
| `0x44C150` | `ScriptViewerWorkerView::Stop` | 101 | 3 |
| `0x44C1C0` | `ScriptViewerWorkerView::Start` | 56 | 2 |

Two independent frozen-source VC7.1 SP1 CL13.10.6030 / LINK7.10.6030
cold `/GL /GS` links reproduce **193 authored bytes** and all six fields.
Unique complete PE/MAP/PDB contributions and complete decoding establish each
extent. No matching region is sliced out of a larger nonexact owner.
See [the compact native proof](native-summary.json) and
[the target observations](target-facts.json).

Both links use actual `ScriptViewerCreate` as entry, primary
`ScriptViewer.cpp`, and the ten support TUs in canonical manifest order.
The new worker support follows Main.cpp. The diagnostic unresolved-symbol
address anchors are never executed. No artificial caller, register annotation,
code segment, inline assembly, byte array or altered compiler profile is used.
The MIT-licensed reference suggests the source shape; TH10 disk bytes, fresh
local Ghidra, real calling edges and strict native replay establish this claim.

```sh
scripts/repo-python scripts/build-match-unit.py --unit script-viewer-worker-stop
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-worker-stop --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-worker-start --json
scripts/repo-python scripts/compare-linked-function.py --unit script-viewer-suspend-player --json
```

## Target behavior and ABI review

Stop leaves every field untouched when handle+4 is null. Otherwise it writes
stopRequested+0xC=1 and running+0x10=0 before waiting200ms. On each WAIT_TIMEOUT
it repeats both writes, sleeps1ms and waits again using the current handle.
Any other status, including WAIT_FAILED, proceeds to CloseHandle, clears the
handle and entry+0x18, and preserves thread ID+8 and the unobserved fields.
The CloseHandle result is ignored.

Start stops an existing thread before storing the new entry, running=1 and
stopRequested=0. It calls the actual `_beginthreadex` API with null security,
stack size0, the unchanged callback address, supplied context, flags0 and
`&threadId`. It stores the returned handle even when null; it does not clear
running on creation failure. Native LTCG selects Stop's ESI receiver and
Start's EAX receiver, EDI callback and stack context/RET4 from these real
bodies and their browser caller.

The browser's load callback has no argument reads and a plain RET. Its address
is passed unchanged via an explicit cast to the CRT API's declared callback
type. That preserves the observed interface; original callback source typing
and behavior under a real Windows thread remain unvalidated.

RebuildPlayerOptions reads its sole Player pointer from entryESP+4 and has one
reachable exit, RET4 at0x42792B. Fresh local Ghidra agrees with a single stack
parameter; the maintained declaration and definition now use `__stdcall` to
express that measured cleanup contract, with no algorithm change. This is an
ABI reconstruction, not a claim that the original source wrote that keyword.
All ten direct calls push the Player pointer. The persistent database reports
nine; the separately reviewed browser suspension call0x40A02D is the tenth.
The previous nine-caller/plain-RET diagnostics remain historical observations.
The old cdecl source emitted an extra ADD ESP,4 in the suspension helper;
the complete helper is now exact without changing its body.

## Coupled functions and limits

All eight earlier browser dependency units remain exact in both expanded cold
images. Existing cursor Move128, input IsRepeated33 and mode-switch37 also
pass complete-owner same-image guards. The three Player option callback units
use RebuildPlayerOptions as their canonical entry; their entry symbol changes
to the measured callee-clean interface, and one focused cold replay of that
original three-TU graph preserves all three exact functions. This is fourteen
existing guards plus three new owners, not a historical full-cohort replay.

Update changes1267 to1264 bytes against target1251; its ordinary structural
agreement changes539/923 to522/923. RebuildPlayerOptions changes2404 to2408
owned bytes against2520 and now returns RET4; its body and six-entry table
remain nonexact. Neither larger owner receives partial exact credit.
Creator68/67 differs at the unresolved destructor call: native moves the
receiver into ECX, while target pushes EBX. Restoring the real412-byte browser
destructor is a concrete next dependency. Initializer153/101, loader16/16,
manager release helpers and Draw647 also remain open.

The initializer's extra52 bytes are localized to inlined PlayerCreate: target
calls it at0x40A351, while native includes its allocation, construction,
registration and failure cleanup before the next creator. That next call lies
at native+0x3A versus target+0x06, exactly52 bytes apart. Recovering the observed
outlining/caller context is a concrete next experiment; an original noinline
annotation is not established.

Host fixtures compile the actual maintained worker bodies and check null
handles, repeated timeouts, changed flags/handles, non-timeout failures,
ignored CloseHandle failure, stop-before-start ordering, all six CRT API
arguments and creation failure. The browser state/event fixture and standard
repository CI pass. These are source-level fixtures, not Windows runtime or
dual-Oracle semantic validation. Native product closure and later phase gates
remain open. No new boundary, origin promotion or Factory acceptance is claimed.
