# Match the executable checksum and audit ANM interfaces

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `7f45a43`. Reviewed on 2026-10-21.

CalcExecutableChecksum43A1C0/207 becomes one complete new authored exact owner.
The source uses natural signed division, a pointer countdown, an explicit outer
success path and separate walking/base pointers. The cursor receives the file
reader's result first; its snapshot is retained for free. This recovers the
target ECX cursor, EBX base, ESI checksum, EDI size and saved-register lifetimes.
No register annotations, padding, fabricated return or compiler override is added.
Original variable names, class ownership, physical source and profile remain
unknown. Main has15 exact owners out of20 source-present reviewed authored owners.

The loop runs `fileSize / 4 - 1` times when positive, summing complete DWORDs
except the last. On successful reading it frees the original base, publishes
size492384 then checksum492380, and returns the checksum. Either API/open
failure returns−1 before those writes. The previous hand-written shift/bias
and indexed loop are replaced with the target's ordinary CDQ/division and
incrementing pointer/decrementing count. Decompiler names and types alone do
not establish this shape; complete target instructions and compiler results do.

## Complete native evidence

Two independent complete five-TU cold links compile Main, GameErrorContext,
FileSystem, Midi and PbgArchive with /GL /GS through genuine WinMain. All17
recursive project-input hashes agree. Both strictly replay207 bytes/all9
independently bound fields plus ten existing owners1984/all124, totaling11
complete owners2191/all133 per image. These include the prior window/startup
helpers, error/log/file-check owners, MIDI callee and complete OpenFile360/all26.
The [native receipt](native-summary.json) pins source, all five objects,
PE/MAP/PDB identities, complete replay hashes, bindings and controls.

Real PbgArchive visibility restores the file loader's private EAX pathname,
stack sizeOut/mode and RET8. The earlier four-TU image had a297-byte file
loader with EBX sizeOut and cannot establish that caller convention. Adding
PbgArchive alone yields OpenFile360 but leaves old checksum source168, so the
dependency alone is insufficient. Changing the genuine entry from WinMain to
checksum is also insufficient. Source/dataflow corrections are necessary.
A same-size207-byte negative still has a different field inventory; it is
rejected. Swapping size/checksum target bindings also rejects (205/207).

The second cold canonically replays the new207-byte unit and the affected
original FileSystemOpen360 recipe in its separate three-TU context: two units,
two artifact groups and567 bytes total. Existing recipes retain their declared
contexts. No full historical replay is run.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit main-executable-checksum --unit file-system-open-file --json
```

Direct local Ghidra review is read-only/noanalysis with rollback, one CPU and
768 MiB. Disk identity and the inclusive mapped text401000..465A80 agree
before/after:412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The207-byte extent ends43A28E before one CC byte;438B9E is its sole direct
caller. Internal loop alignment is optimizer-generated and included in the
complete comparison. Target writes43A251/43A258 identify the two proposed
globals. The target PE import table independently identifies KERNEL32's
GetModuleFileNameA slot4660DC. Runtime/data ownership is not inferred.

## Large-owner evidence

Eight private ANM controls are rejected. The actual nine-TU baseline passes28
complete existing guards5180 bytes/all168 fields, including the complete four
variable readers and Float2/Float3/alpha/RGB evaluators. GetFloatVar's target
protocol is liveESI, one stack float and RET4. Both ordinary member source and
ordinary free stdcall source reproduce424/all44. After binding both fully
proven callees to43EAC0, the complete executor remains native-to-native equal.
This does not identify original source ownership. Free cdecl source gives368
and grows the executor to10156; forwarding wrappers grow it to10524.

A fresh nine-TU cold also tests an ordinary manager-member ExecuteScript with
an unused manager receiver. It preserves the entire9964-byte native executor
and7020-byte Run after verified fields, with all28 complete guards passing.
Enemy's native comparison retains one unbound changed REL32 field to the
source-qualified EnemyInitializeLaserType1 helper; this is not an established
ordinary instruction mismatch. No production interface changes are adopted.

G6/G7 each leave all three complete native owners equal after verified field
rewrites. One meaningful local-pointer rename is also neutral; no identifier
sweep is pursued. These controls exclude those explanations, not every profile
or source shape. All three large target comparisons remain nonexact.

Only Main.cpp changes in production. It is absent from the large graph's30
actual inputs; Main.hpp and every input/artifact hash remain unchanged. The
existing current large carrier stays valid without an unnecessary rebuild:
ANM9964/gap2/aligned7478/7995; ECL7020/all189 with27 ordinary differences in this
nine-TU graph; Enemy14468/14416 with gap652. Separate two-TU ECL remains26.

One authored owner/207 bytes is credited, with no source mapping, origin or
boundary change. Authored exact becomes381 functions/69343 bytes; canonical exact1222.
Native product/runtime, semantic and port gates remain open. Final checks and
owned-artifact cleanup are in the [validation receipt](validation-summary.json).
Private Main recovery is below `.analysis/gpt-6.1-sol/main-checksum-20261021/`;
ANM controls are below `.analysis/gpt-6.1-sol/anm-execute-flags-20261021/`.

All100 standard CI checks pass. Thirteen owned terminal discovery native
directories are pruned; ANM accounts for5712627 measured bytes. Main's seven
directory byte total was not retained after an interrupted cleanup, and is
explicitly unknown. All source snapshots and artifact identities are preserved;
both actual cold proofs and one same-size failure remain.
