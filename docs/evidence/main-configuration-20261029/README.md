# Main configuration reader — 2026-10-29

The complete `420870..420A85` settings reader adds **534 authored exact bytes**.
Two independent canonical29-input cold links reproduce every byte and all60
semantic link fields. This adds one source mapping without changing the reviewed
authored denominator. See [results.json](results.json) for source/include pins,
per-source profiles, artifact identities, target extent hash and comparison results.

The owner resets the shared configuration at Main491C28+120, opens the requested
file, copies the complete0x34-byte settings, frees the buffer, validates six byte
settings/version/size, and publishes the nine16-bit key bindings on success.
Missing or invalid settings are logged and reset. It then logs the actual option
bits and window mode, updates the vsync field, and writes settings back. A write
error reports both target fatal messages, including the path argument, and returns
-1; success returns0. The complete comparison includes all these branches.

The original WinMain caller at438B5C pushes the Main pointer, loads the path in
EBX and calls420870. The target exits with RET4 on both paths. The old maintained
extern declaration was default cdecl; the first complete source reproduced the
body but emitted530/534 bytes with plain RETs. Correcting that declaration and
the definition to natural `__stdcall` recovers both exits through the actual
caller graph. No synthetic context argument or register-forcing ABI is added.
The linked optimizer promotes the path to EBX and retains the Main pointer on
the stack. Source-level stdcall spelling is a reconstruction model; original
identifiers and translation-unit ownership remain unknown.

Every address field is bound to its semantic base. Configuration fields belong
to `g_MainSupervisorView` at491C28 with addends120..13F; the option20 write belongs
to its disableVsync field+3B4. The binding table belongs to474E88 plus0/4/8/C/10.
Nearest diagnostic anchor labels at these offsets refer to unrelated D3D or Ending
symbols and are not used as evidence. Despite its provisional Default name, the
binding table is writable and receives validated user keys. The original table
and log data remain external; byte exactness does not establish their production
storage ownership.

The two cold images also replay seven explicit existing contracts1217 bytes:
OpenFile, WriteFile, Log, Fatal, GameManager factory, its thread forwarder and
Main worker launch. These cover the reader's direct interfaces and the callers
of the shared configuration reset whose optimizer context changed. No historical
cohort, comparator self-test or host scenario campaign is required or claimed.
The534-byte reader has selector `main-load-configuration`:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit main-load-configuration --json
scripts/repo-python scripts/ci.py
git diff --check
```

Configuration reset remains125/125 with nine ordinary byte differences. WinMain
remains1438/1389 in this graph, unchanged in size from the preceding checkpoint.
Neither gets partial exact credit. The new exact reader binds the actual reset
entry but does not prove its bytes. Main construction at421F00 still needs a
source model preserving configuration-before-worker member construction.
No product, runtime or portability claim is introduced.
