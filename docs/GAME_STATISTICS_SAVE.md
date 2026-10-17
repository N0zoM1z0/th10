# Game statistics save

## Recovered owner and evidence

`GameFinalizeStatistics42B1E0(GameStatisticsOwner*)` in `src/GameStatistics.cpp` maintains the complete 564-byte operation at `0x0042B1E0–0x0042B413`. This is a save operation, not destruction: it neither releases the statistics owner nor clears its global. The target returns `-1` for a null header or failed file open, otherwise `0`. Both direct callers ignore that result.

Evidence is pinned to `resources/th10.exe`, SHA256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. The reviewed boundary ledger and dense decode agree on all 564 bytes, ending in `RET`; the locked Capstone 5.0.6 decoder verifies 188 instructions. Adjacent `CC` padding separates the owner. Direct calls occur at `0x00417C8D` in `GameManagerCleanup` and `0x0041FC6E` in the complete caller at `0x0041FB50`. Both load pointer slot `0x0047783C` into EBX before calling. The second caller subsequently releases the owner's two buffers and allocation, then clears the slot; those actions are not part of this save body.

## Shared storage layout

`GameStatistics.hpp` contains evidence-backed partial views, without claiming original class or field names. Factory `0x0042AF20` allocates `0x1DCB4` bytes and publishes the constructed owner to `0x0047783C`. Constructor `0x0042AE60` clears that allocation, loads the score file, initializes seven records and the tail, then invokes loader `0x0042B030`. Destruction at `0x0042AEF0`, also visible inline in the second caller, separately frees nonnull pointer fields at offsets `0` and `4`.

- Owner `+0`: pointer to a file header of at least `0x18` bytes.
- Owner `+4`: separately owned buffer pointer; its exact meaning remains unresolved. Save leaves it untouched.
- Owner `+8`: seven records, each `0x437C` bytes.
- Owner `+0x1D86C`: `0x448`-byte tail, ending exactly at allocation size `0x1DCB4`.
- Record `+0`: 16-bit magic; `+4`: checksum; `+0xC`: index. Checksum payload is the `0x4374` bytes from record `+8` through its end.
- Record `+0x4C0/+0x4C4`: session-start/active-frame counters.
- Tail `+4`: checksum of its `0x440` payload bytes beginning at `+8`.
- Header `+4`: total file size; `+0x10`: compressed size; `+0x14`: uncompressed payload size. Other header fields remain neutrally named.

Production assertions check the x86 owner offsets/extent and pointer-free record/header/tail layouts. Host fixtures retain all declarations and pointer-free assertions, replacing only the three marked pointer-dependent assertions with explicit host64 layout checks.

The existing global identifier `g_GameStatisticsRows` now has the single canonical declaration `GameStatisticsOwner*`. No new storage definition is introduced. All discovered GameManager and teardown-fixture consumers are updated. The former owner-relative pseudo-stride accesses are preserved as genuine record accesses: owner `+i*0x437C+0x4C8/+0x4CC` equals record `+0x4C0/+0x4C4`. Stage accessors preserve owner-relative `+stage*0xF0+0x18/+0x1D` as record-relative `+stage*0xF0+0x10/+0x15`, using `memcpy` rather than inventing an overlapping stage array. Starting-stage selection is captured once before the two reads.

Enemy's existing byte-view alias `g_EnemySpellStatistics` remains unchanged. Its equivalence to the same physical slot is a runtime storage-identity obligation, not a second allocation. The C++ identifier of the canonical slot is unchanged, but its MSVC decoration changes with the pointer type. Repository-wide source/configuration/script references were inspected; no configuration linkage references required substitution.

## Save contract and dependencies

1. Reject a null header. Allocate `0x200000` scratch bytes and copy the current `0x18`-byte header after allocation. There is no scratch-allocation failure guard.
2. Visit seven records in index order. Only magic `0x5243` records are included. Replace their index before summing unsigned payload bytes, store their checksum, and append each complete record. Rejected records remain unchanged.
3. Compute/store the tail checksum and append its complete `0x448` bytes. Set header uncompressed size to `selectedCount*0x437C+0x448`.
4. Compress only the packed payload at scratch `+0x18`, writing compressed size through the current header. Reload the owner header after compression and set total file size to compressed size plus `0x18`. The copied scratch header is neither compressed nor used as the output header.
5. Encrypt the compressed allocation with profile `(0xAC, 0x35, 0x10, compressedSize)`; ignore the returned pointer. Open `scoreth10.dat`. Any nonzero open result immediately returns `-1`, leaving both allocations leaked as in the target. Record/header mutations remain observable.
6. Write the current header (`0x18` bytes), then the compressed allocation using the current header's compressed size. Apply the existing stateful write/close protocol, conditionally free compressed storage, always free scratch, and return `0`, including after short writes.

The genuine maintained dependencies are `CompressData` at `0x004359B0` in `Lzss.cpp`, and `FileSystem::Encrypt` at `0x0044B220` plus `FileSystem::OpenWriteFile` at `0x0044B620` in `FileSystem.cpp`. The ordinary `FileSystem::WriteToOpenFile` and `CloseWriteFile` source seams reproduce inline target protocols; they have no separate target-entry claim here. Allocator targets are `malloc` at `0x00452706` and `free` at `0x00452422`.

FileSystem retains its canonical namespace and shared state: `gReplayFileHandle` at `0x00474C38`, `gReplayFileCriticalSection` at `0x004922A4`, and unsigned-byte `gReplayFileOpenCount` at `0x0049231E`. These historical names also serve statistics saves. No statistics-private handle, lock, count or compression storage is added.

## Failure behavior and valid domain

WriteFile's BOOL result is ignored; only its returned byte count selects short-write cleanup. A mismatch reloads the shared handle, closes it, leaves the critical section and decrements the unsigned-byte count, without invalidating the handle or returning. Later writes and final cleanup can therefore operate on the unchanged closed handle and repeat close/unlock/decrement. The reconstruction deliberately does not repair this behavior or propagate a write error.

At `0x0042B39A–0x0042B3A2`, the target compares the handle with `INVALID_HANDLE_VALUE`, then unconditionally reads the current owner header and compressed size before taking the invalid-handle branch. The second source helper's argument evaluation preserves that ordering. Valid header replacement during a dependency call is covered; replacement with null is not a valid-domain test because the target also dereferences it. A null owner, failed scratch allocation, or invalid compression result is not made safe by new guards. Host tests model repeated invalid close/unlock effects through explicit mocks, never real OS locks or the user's score file.

## Host validation

The complete maintained source and actual FileSystem write/close bodies pass **229 scenarios and 44 strict compiled mutants** at O0, O2 and Python `-O` with O1 ASan/UBSan. Every mutant must compile and fail at its exact predeclared CHECK expression and generated source line; compiler errors, crashes, sanitizer reports or unrelated failures are harness errors. Independent review also passes a canonical Python `-O` O2 baseline.

Scenarios cover all 128 magic masks, unsigned checksums and index ordering, exact packing/copy bounds, the tail, header replacements after effectful boundaries, file-handle reloads, short-write/BOOL/count combinations, count wrapping, failed-open leak traces, allocation identity/order, untouched owner storage and GameManager access offsets. The fixture safely reclaims intentional leaks only after checking production effects. Compressor, encryption, open and OS calls remain explicit mocks; actual compression/encryption integration is not claimed.

On the local validation host, LSan alone required an explicitly documented ptrace-incompatibility workaround; ASan/UBSan remained enabled. The runner enables leak detection by default. Results on another host must state that host's actual sanitizer configuration rather than inherit this exclusion.

## Limits and native follow-up

The target's private live-in EBX receiver is not evidence for a source-level calling convention. Maintained source uses an ordinary typed argument, with no assembly, register pinning, fabricated owner wrappers or ABI tricks. Original declaration/TU ownership and runtime alias/storage closure remain unresolved. Constructor, loader and destructor bodies are not reconstructed by this save implementation.

Source recovery and host controls do not establish native contribution retention, whole-game execution or byte exactness. A bounded diagnostic should use the genuine GameManagerCleanup root and actual GameStatistics/Lzss/FileSystem support, inspect complete emitted ownership and bindings, and report any mismatch without codegen sweeps. Existing GameManager cleanup was already nonexact in its earlier diagnostic. Only an accepted canonical full-extent replay can grant exactness credit to this 564-byte owner; native and regression results must be recorded separately after they run.

## Native and integration observations

One ordinary LTCG diagnostic used the genuine GameManagerCleanup root and
its existing ordered Hint/Chain/AnmManager/FileSystem support, then appended
GameStatistics.cpp and Lzss.cpp. PE SHA256
`aca311b0e6dadeaf0f087249d89ebb946f560110a0a978da1902f719fd271a6c`.
The full save contribution is40FBB0/567 versus target42B1E0/564;
GameManagerCleanup is401D00/1118 versus target1105. No size ratio is a
matching score. Neither owner receives exact credit.

The genuine caller naturally passes the statistics owner in EBX, matching
the observed private target receiver without source register forcing.
Maintained compression40FDF0/1029, encryption40F990/284 and open40FAB0/137
contributions are linked. Independent full decoding identifies remaining
record-index/base register and checksum scheduling differences, a tail-loop
alignment NOP, and caller cleanup at the Encrypt boundary where the target
callee owns cleanup. No shared Encrypt declaration or ABI was altered in
this packet. Failure, write/close and free paths remain present. Diagnostic
external/global DATA anchors remain nonrunnable; static contribution
retention does not prove storage aliases or native runtime closure.

The integrated remote O1 ASan/UBSan run passed229 scenarios and44 strict
mutants with LeakSanitizer enabled, so it needed no ptrace workaround.
The updated GameManager teardown fixture also passed all70 cases and50
strict mutants. Two affected canonical cold producer groups retained31+7=38
previously tracked exact bytes; those are regression results, not new credit.
The combined regression/test request timed out in transport after work
continued remotely; complete reports and no remaining native/test processes
were verified before proceeding, without rerunning the work.

A separate read-only Encrypt contract audit checked all three target calls
(429E7D,429E91,42B335): AL seed plus five stack arguments, no caller pop,
and target RET0x14. The current linked caller/callee pair is balanced with
caller cleanup. Because this is already an optimized private ABI, it does
not establish an original ordinary six-argument __stdcall declaration.
That declaration remains a plausible hypothesis, not a proved source repair;
no ABI experiment is made merely to remove a few bytes. No semantic defect
was found in the save owner. Full target-independent CI passed2026-10-17
00:30:20 UTC.
