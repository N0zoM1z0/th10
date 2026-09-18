# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`, target `target:th10-main`, analysis provider `th10-ghidra`.
- Current packet base: `b3f8fc7 gpt-web: isolate ECL VM allocator gap`, branch `main`.
- Completed session checkpoint: `bd9b2e3 gpt-5.6-sol: promote exact PbgFile accessors`.
- Completed session checkpoint: `9b5e7eb gpt-5.6-sol: add exact replay workflow`.
- Completed session checkpoint: `4ed34ee gpt-5.6-sol: promote exact PbgArchive lifecycles`.
- Completed session checkpoint: `621add9 gpt-5.6-sol: promote exact LZSS state reset`.
- Completed session checkpoint: `88800b6 gpt-5.6-sol: report exact reconstruction backlog`.
- Completed session checkpoint: `96f17c6 gpt-5.6-sol: harden COFF extent diagnostics`.
- Completed session checkpoint: `d59f975 gpt-5.6-sol: batch probe exact backlog`.
- Completed session checkpoint: `26af714 gpt-5.6-sol: add LTCG linked-image diagnostics`.
- Completed session checkpoint: `f5a0c4f gpt-5.6-sol: add canonical LTCG replay`.
- Completed session checkpoint: `40c96ec gpt-5.6-sol: promote exact PbgArchive LTCG helpers`.
- Completed session checkpoint: `e4c9077 gpt-5.6-sol: promote exact LZSS LTCG helpers`.
- Completed session checkpoint: `37ebb83 gpt-5.6-sol: promote remaining exact LTCG helpers`.
- Completed session checkpoint: `ead21f9 gpt-5.6-sol: rank exact reconstruction backlog`.
- Completed session checkpoint: `d4af1a5 gpt-5.6-sol: recover ECL core lifecycles`.
- Completed session checkpoint: `e2fecf3 gpt-5.6-sol: recover ECL host hierarchy`.
- Completed session checkpoint: `2dbce7d gpt-5.6-sol: recover ANM VM lifecycle`.
- Completed session checkpoint: `260ac75 gpt-5.6-sol: recover ANM manager core`.
- Completed session checkpoint: `3e6de60 gpt-5.6-sol: recover ASCII text pipeline`.
- Completed session checkpoint: `e239b4b gpt-5.6-sol: recover ANM render buffer core`.
- Completed session checkpoint: `591f59b gpt-5.6-sol: recover ANM draw core`.
- Completed session checkpoint: `3616aeb gpt-5.6-sol: recover ANM camera projection`.
- Completed session checkpoint: `1923623 gpt-5.6-sol: recover ANM rotated draw modes`.
- Completed session checkpoint: `56ac2e8 gpt-5.6-sol: recover ANM projected 3D quad`.
- Completed session checkpoint: `9ad868a gpt-5.6-sol: recover ANM mode dispatch`.
- Completed session checkpoint: `fc3dce0 gpt-5.6-sol: recover ANM projected photo blend`.
- Completed session checkpoint: `f6d88dd gpt-5.6-sol: recover ANM direct 3D`.
- Completed session checkpoint: `09e4da8 gpt-5.6-sol: recover ANM generated geometry`.
- Completed session checkpoint: `2d8c48b gpt-5.6-sol: recover ANM radial trail and RNG`.
- Completed session checkpoint: `92db5f4 gpt-5.6-sol: recover ANM script variables`.
- Completed session checkpoint: `d994538 gpt-5.6-sol: reconstruct ANM script executor`.
- Completed session checkpoint: `94141ac gpt-5.6-sol: reconstruct ANM child VM lifecycle`.
- Completed session checkpoint: `8c254ad gpt-5.6-sol: reconstruct ANM manager update core`.
- Completed session checkpoint: `bdf8f14 gpt-5.6-sol: reconstruct ANM manager setup core`.
- Completed session checkpoint: `acfcf07 gpt-5.6-sol: reconstruct ANM resource and surface core`.
- Completed session checkpoint: `adfd172 gpt-5.6-sol: reconstruct Enemy ECL dispatcher`.
- Completed session checkpoint: `ff78689 gpt-5.6-sol: reconstruct Enemy callback core`.
- Completed session checkpoint: `d4352b5 gpt-5.6-sol: reconstruct generic ECL VM core`.
- Completed session checkpoint: `623cd37 gpt-5.6-sol: reconstruct GUI update and message cores`.
- Completed session checkpoint: `979522c gpt-5.6-sol: reconstruct Main execution corridor`.
- Completed session checkpoint: `048a3c8 gpt-5.6-sol: separate boundary inventory`.
- Completed session checkpoint: `e7615ce gpt-5.6-sol: reconstruct front-end options core`.
- Completed session checkpoint: `4142a89 gpt-5.6-sol: reconstruct front-end key config core`.
- Completed session checkpoint: `5b6a544 gpt-5.6-sol: reconstruct front-end selection core`.
- Completed session checkpoint: `41c0f09 gpt-5.6-sol: reconstruct front-end stage core`.
- Completed session checkpoint: `8edf352 gpt-5.6-sol: reconstruct front-end replay core`.
- Completed session checkpoint: `9cbe831 gpt-5.6-sol: reconstruct front-end practice core`.
- Completed session checkpoint: `5f719b6 gpt-5.6-sol: complete front-end practice draw`.
- Completed session checkpoint: `27b272a gpt-5.6-sol: reconstruct front-end score entry`.
- Completed session checkpoint: `3fd501c gpt-5.6-sol: fix ANM exact owner boundaries`.
- Completed session checkpoint: `7a2779f gpt-5.6-sol: recover exact ANM Draw3D`.
- Completed session checkpoint: `ac9541d gpt-5.6-sol: recover exact ANM Draw2D pair`.
- Completed session checkpoint: `ee13d3a gpt-5.6-sol: recover exact ANM 3D render state`.
- Completed session checkpoint: `8b0f19e gpt-5.6-sol: recover exact ANM generated draws`.
- Completed session checkpoint: `91c8d5f gpt-5.6-sol: recover exact ANM float variable resolver`.
- Completed session checkpoint: `52bd0ff gpt-5.6-sol: recover exact ANM embedded texture upload`.
- Completed session checkpoint: `bdc2a51 gpt-5.6-sol: recover exact ANM surface capture`.
- Completed session checkpoint: `d955eba gpt-5.6-sol: recover exact ANM projected photo blend`.
- Completed session checkpoint: `209595d gpt-5.6-sol: recover exact ANM draw dispatcher`.
- Completed session checkpoint: `e17e4ff gpt-5.6-sol: recover exact ECL VM stack core`.
- Completed session checkpoint: `358daf4 gpt-5.6-sol: recover exact ECL VM typed pop`.
- Completed session checkpoint: `85b7088 gpt-5.6-sol: recover ECL VM operand helpers`.
- Completed session checkpoint: `c615a72 gpt-5.6-sol: recover ECL VM thread control`.
- Completed session checkpoint: `82003dd gpt-5.6-sol: recover ECL script database loader`.
- Completed session checkpoint: `b3eaac1 gpt-5.6-sol: recover exact ECL host runner`.
- Completed session checkpoint: `afa2277 gpt-5.6-sol: align ECL dispatcher code shape`.
- Completed session checkpoint: `581d75d gpt-5.6-sol: recover ANM executor physical order`.
- Completed session checkpoint: `7248af7 gpt-5.6-sol: recover ANM interpolation value flow`.
- Completed session checkpoint: `2e1cd95 gpt-5.6-sol: recover ANM Float2 evaluator core`.
- Completed session checkpoint: `dfc22c4 gpt-5.6-sol: recover ECL typed arithmetic flow`.
- Completed session checkpoint: `0d85d50 gpt-5.6-sol: gate ANM ECL Web handoff`.
- Completed session checkpoint: `34e55b8 gpt-5.6-sol: repair Web Python toolchain selection`.
- Completed session checkpoint: `b3f8fc7 gpt-web: isolate ECL VM allocator gap`.
- Planned current checkpoint subject: `gpt-web: recover exact PbgFile write leaf`. Final commit hash is intentionally not self-recorded before the commit exists; recover it from live Git after checkpoint.
- Recovery continued from the clean boundary-inventory checkpoint. The ignored private target, existing `.analysis/`, toolchain, Wine prefix, Ghidra project, and build caches were preserved.
- Current campaign: small-leaf exact recovery, with the ECL linked-image corridor locally closed and a Factory-replayable normal-COFF `CPbgFile::Write` leaf prepared next. The retained pinned-decoder diagnostics live under `.analysis/gpt-5.6-sol/20260916-ecl-vm-read-resolve/`; current cold linked evidence is rebuilt from tracked source and canonical match-unit definitions. Factory acceptance still requires the post-commit canonical replay receipt.
- This session has not pushed. The exact-reconstruction campaign remains active/incomplete.

## Recovery and authority

The session inspected branch/HEAD/history, complete tracked/untracked state, and the prior handoff. Committed base `b3f8fc7` is the current live authority.

All requested repository and Factory guidance was re-read from the live repository shell before tracked reconstruction work. No requested path was missing.

Fresh repository preflight passed at the start of the current front-end session:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`

The execute toolchain path passed pinned VC7.1 SP1 build6030 normal COFF, C++ `/GL`, resource and PE32-i386 link smoke through headless Wine. Native `th10-ghidra` `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`. This packet combines that provider's read-only address-set ranges, decompilation, disassembly, calls and xrefs with direct hash-attested target disassembly.

Target remains the ignored operator file `resources/th10.exe`: size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`, dominant Rich build6030. It was not modified, moved, staged or committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Completed packet: independent boundary inventory

Boundary state is now separate from origin state. `config/function-boundaries.csv`
contains one target-bound row for each of the 1,312 candidates, and
`scripts/validate-tracking.py` enforces its address order, one-to-one coverage,
span equality, state/confidence combinations, evidence and notes. Exact rows
must have a reviewed boundary.

`scripts/report-boundary-inventory.py --check-ledger` reproduces the automatic
review against the exact target with the pinned Capstone decoder. It densely
decodes each tracked extent, checks terminal control flow, independent code and
aligned non-text pointer references, overlapping extents, external branches into
another body, Ghidra sparse/remote ranges, `.text` union coverage and gaps. The
current ledger has **1,173 reviewed, 88 provisional and 51 needs-review**
boundaries. Tracked extents cover 340,424 union bytes. The remaining 71,865
`.text` bytes contain 519 pure alignment gaps / 4,186 bytes and 430 unresolved
gaps / 67,679 bytes. The report retains 26 overlapping extent pairs involving
37 candidates, four direct targets outside tracked extents and 97 aligned data
pointers into untracked `.text` as triage evidence.

The audit now also extracts absolute `.text` bases from indexed indirect jumps.
It finds 100 target references to such control tables: 61 table references are
inside their source owner's recorded span and 39 point beyond it. This is a
boundary-review queue, not automatic permission to extend those owners; table
length, selector domain, neighboring code/data and padding still require direct
target review.

The former inventory export treated each Ghidra function as one dense interval.
`python3 scripts/ghidra.py architecture` now exports every address-set range.
The current database has 1,419 ranges for 1,195 functions, including 122 sparse
bodies and two functions with ranges below their entry. `0x004451C0` owns remote
ranges at `0x004436C0-0x0044390E` and `0x00443910-0x00443B5E` in addition to
its entry range; `0x0046227B` owns the shared return range
`0x00452720-0x00452724`. These cases demonstrate why boundary review cannot use
`entry..maximum-address` as the physical owner.

Direct review added three Ghidra-missed front-end candidates. The reviewed
`0x0042D260-0x0042D2D3` body owns code through `0x0042D2B3` and an eight-entry
absolute switch table. The seven-byte adapters at `0x0042D2E0` and
`0x0042D2F0` move ECX to EAX and jump to `0x0042CDF0` and `0x0042D260`
respectively; target data references and following CC padding close both.

Changing the registered Ghidra wrapper/exporter caused the earlier provider
failure. The Factory compares an aggregate SHA-256 of native implementation
files with its private operator binding, so the message “native Ghidra
implementation differs from its operator binding” was a deliberate stale-binding
failure. The new implementation was reviewed, the ignored private binding was
refreshed to `c001f8ef3d9a64e96664d999c14891d9c92fa12f393bca64cc77bb087c0e2143`,
the Factory validator passed, and native `check {}` passed again. No tracked
Factory file changed.

`docs/PROGRESS.md` and `scripts/report-reconstruction-status.py` retain the
separate origin and boundary ledger counts. The generated SVG follows the TH09
status-card convention and shows one combined `Origin/boundary reviewed` bar;
its count is the conservative intersection of candidates whose origin and
boundary reviews are both complete. `scripts/rank-core-backlog.py` orders
source-absent, boundary-reviewed candidates by target size and optional Ghidra
connectivity while explicitly granting no ownership or semantic credit.

## Completed packet: exact ANM draw dispatcher

`AnmRenderManagerView::Draw @ 0x004451C0-0x00445297` is now canonical exact
across its complete 216-byte linked `/GL` PDB contribution. The contribution
contains 173 bytes of executable code, the three-byte alignment instruction at
`0x0044526D`, and the ten-entry absolute jump table at `0x00445270`.

TH10 physically orders its case blocks as modes `0, 1, 4, 5, 6, 7, 8, 9, 2,
3`, although the table continues to map numeric modes 0-9 normally. Preserving
that source case order closes the dispatcher after offset `+0x20`. The final
entry mismatch was confined to the three VM guards: separate early returns
made VC7.1 place the shared failure block after the first visibility test,
whereas the target branches both flag failures into the alpha-zero rejection
block immediately before the switch. Expressing that real common return with a
single label inside the alpha branch reproduces the target control flow without
adding state, padding, assembly, or copied bytes.

Two independent focused cold replays close **216/216 bytes and all 21 linkage
fields**. The exhaustive fields cover the table base, ten renderer calls, and
all ten table destinations as offsets within the same PDB owner. The full
`src/AnmManager.cpp` cold gate closes **76 units / 10 artifacts / 15,739 target
bytes** with no regression. Repository totals are now **126 exact functions /
17,730 bytes**. ANM has **66 exact owners / 14,903 bytes** and **57
source-present authored owners / 26,804 bytes** remaining.

## Completed packet: exact ECL VM stack core

Direct target review separates `EclVmContext + 0x08` into an embedded
`EclVmStackView` of exactly `0x1008` bytes: `data[0x1000]`, `stackTop` at
stack `+0x1000`, and `frameBase` at stack `+0x1004`. The enclosing context
therefore retains its proven `0x1024` size and the absolute member offsets
`+0x1008/+0x100C`, while the source now expresses the original nested owner
used by the helper corridor.

`EclVmStackView::Push @ 0x004505B0-0x0045060F` and
`EclVmStackView::EnterFrame @ 0x00450690-0x004506CF` are canonical exact over
their complete relocation-free linked `/GL` PDB contributions. Push is 96
bytes and takes its private receiver in EAX, type in DL, and size/value on the
stack with `RET 8`; EnterFrame is 64 bytes and takes the receiver in EAX and
local byte count in ECX. Two independent focused cold replays and the complete
`src/EclVm.cpp` source gate reproduce **2 units / 1 artifact / 160 bytes**.

The adjacent `EclVmStackView::LeaveFrame @ 0x004506D0-0x004506F8` is source
present over its reviewed 41-byte owner. Natural VC7.1 `/GL` output matches
39/41 bytes; its only difference is `SUB ECX,4` where the target uses the
equivalent `ADD ECX,-4` encoding. It remains a near match without a canonical
unit or exact credit. Repository totals become **1,312 candidates, 285 source
mappings, and 128 exact functions / 17,890 bytes**. The authored source
backlog is **153**.

Recompiling the generic `EclVmContext::Run` after restoring this owner shape
still produced a selected `/GL` contribution of about 2,756 bytes against the
7,020-byte target owner. That result selected the Ghidra-missed typed-pop body
beginning at `0x00450610` as the next target-local owner seam; the following
packet records its recovery.

## Completed packet: exact ECL VM typed pop

Direct target review adds the Ghidra-missed
`EclVmStackView::Pop @ 0x00450610-0x00450689` as a new 122-byte candidate.
The final `RET 4` occupies `0x00450687-0x00450689`; six `CC` bytes delimit the
independent `EnterFrame` owner at `0x00450690`. The target-private ABI carries
the stack receiver in EAX, requested byte count in ECX, requested scalar type
in BL and the output pointer as its sole stack argument.

The owner copies an arbitrary-sized value, optionally consumes the preceding
four-byte type slot, and converts stored integer/float scalars when the caller
requests the opposite representation. Natural source using the nested stack
owner reproduces the target's physical integer-conversion fallthrough and
float-conversion forward branch. Two independent cold replays close all
**122 bytes and the sole `__ftol2` REL32 field**. The full current
`src/EclVm.cpp` gate closes **3 units / 1 artifact / 282 bytes**.

Repository totals become **1,313 candidates, 286 source mappings, and 129
exact functions / 18,012 bytes**. Replacing the provisional local pop logic
raises the selected `/GL` `Run` contribution from about 2,756 to 2,776 bytes,
still far below the 7,020-byte target owner. The remaining core gap is not a
small inlining effect; the next evidence-bearing batch is the target read and
resolve corridor at `0x0044FDB0-0x004500CC`.

## Completed packet: ECL VM operand helpers

The six reviewed owners from `0x0044FDB0` through `0x004500CC` now have
maintained member source: `ReadInt`, `ReadFloat`, `ReadIntValue`,
`ReadFloatValue`, `ResolveInt`, and `ResolveFloat`. They implement the complete
generic operand path for immediate values, frame locals, typed-stack pops, and
host-owned integer/float variables. Target code establishes their distinct
private register conventions independently from the logical C++ member
signatures.

`EclVmContext::ResolveFloat @ 0x00450070-0x004500CC` is canonical exact over
its complete 93-byte linked `/GL` PDB contribution and all three fields: the
zero constant and two `__ftol2` calls. Two independent cold replays produce
different PE hashes while closing 93/93 bytes. The full `src/EclVm.cpp` gate
closes **4 units / 1 artifact / 375 bytes**.

Three adjacent natural-source helpers reach the exact target size and differ
only in the stack-pop decrement encoding: `ReadFloat` is 183 bytes with
181/183 bytes equal, `ReadIntValue` is 124 bytes with 122/124 equal, and
`ReadFloatValue` is 171 bytes with 169/171 equal. The compiler emits `SUB
register,4`; the target emits the equivalent `ADD register,-4`. They remain
near matches with no exact credit. Bounded natural forms did not alter this
choice.

`ReadInt` remains 120 candidate bytes against 144 target bytes, and
`ResolveInt` remains 59 against 58. Their target-private receiver registers
depend on production callers outside the limited diagnostic link graph; the
selected `Run` contribution is 2,772 bytes against the 7,020-byte target. No
dummy parameters, artificial dependencies, or ABI claims were introduced to
force the output. Repository totals become **1,313 candidates, 292 source
mappings, and 130 exact functions / 18,105 bytes**. The authored source backlog
is **158**. The next ECL exact frontier should restore more of the real
whole-program owner/caller context around `Run` before revisiting these private
ABI-dependent helpers.

## Completed packet: ECL VM thread control

The target-local host is now represented through its complete `0x103C` prefix:
a virtual interface, active context at `+0x4`, embedded `0x1024` context at
`+0x8`, script database at `+0x102C`, and an embedded `0x0C` thread-list
sentinel at `+0x1030`. Each secondary node stores its context, next node and
previous node. This corrects the former source assumption that
`EclVmFindThread` returned a context directly; target callers first receive the
node and then dereference its context.

`EclVmHost::FindThread @ 0x00450160`, `StopAllThreads @ 0x00450190`, and
`EclVmScriptDatabase::FindSubroutine @ 0x00450470` are canonical exact over
complete relocation-free linked `/GL` contributions of 36, 31 and 136 bytes.
Two independent cold replays close **3 units / 1 artifact / 203 bytes** with
different PE hashes. The full current `src/EclVm.cpp` gate closes **7 units /
1 artifact / 578 bytes**.

The maintained source also restores the two larger control-flow owners.
`EclVmHost::SpawnThread @ 0x004500D0` allocates a context and node, inherits the
active difficulty mask, links the node, and invokes the new subroutine; its
current selected-entry contribution is 142/142 bytes with 120/130 comparable
bytes equal. `EclVmStartSubroutine @ 0x0044DF70` converts call arguments into
the destination stack, preserves return state, changes the active context and
looks up the named subroutine; it is 487 candidate bytes against the 550-byte
target. Both remain non-exact while their private LTCG parameter allocation is
calibrated.

Repository totals become **1,313 candidates, 297 source mappings, and 133
exact functions / 18,308 bytes**. The authored source backlog is **160**. The
next high-value ECL owner is the 589-byte script database loader at
`0x00450220`; restoring it will complete the database side of the same control
flow before another `StartSubroutine` exact pass.

## Completed packet: ECL script database loader

`EclVmScriptDatabase::AddScriptData @ 0x00450220-0x0045046C` now has complete
maintained source for its 589-byte reviewed owner. The virtual method retains
one caller-owned SCPT buffer in the next of 32 file slots, rejects any buffer
whose magic is not `SCPT` or whose version is not one, and leaves a rejected
slot null without advancing the file count.

The target establishes a 0x24-byte fixed header, include-data length at
`+0x06`, and the low 16-bit subroutine count at `+0x10`. The loader finds the
subroutine-offset table immediately after the include block and the packed
name strings immediately after that table. A first file initializes every
eight-byte `{name, header}` entry. Later files copy the old table, free it,
insert each new entry before the first lexically greater-or-equal name, and
shift the remaining entries right. The resulting table is the ordered owner
consumed by the already exact binary-search lookup at `0x00450470`.

The resource database is now modeled as the target-proven polymorphic
0x1098-byte object rather than only its 0x90-byte used prefix. Virtual slot zero
is `AddScriptData`; when the include length is nonzero, it invokes slot one on
the block at file `+0x24`. The Enemy-derived vtable at `0x0046D0B4` binds that
slot to the already maintained ANIM/ECLI package loader.

Pinned VC7.1 normal and linked `/GL` diagnostics both emit a complete natural
547-byte contribution under `/O2`; `/Ot` is identical, while `/O1` and `/Os`
shrink it to 522 bytes and move farther from the target. The `/O2` candidate
has exactly two linked fields, `malloc` and `free`, but remains a real mismatch
with `acceptance_authority=none`. No exact row is added. The full current
`src/EclVm.cpp` exact gate remains **7 units / 1 artifact / 578 bytes**.

Repository totals become **1,313 candidates, 298 source mappings, and 133
exact functions / 18,308 bytes**. Combined origin/boundary review rises to 329
and the authored source backlog to 161. The next ECL exact pass should use the
now complete database/caller context to revisit `EclVmStartSubroutine`, then
the generic `Run` owner and its private helper ABIs.

## Completed packet: exact ECL host runner

`EclVmHost::Run @ 0x0044FD10-0x0044FDA1` now has complete maintained source
for its 146-byte reviewed owner. It walks the embedded thread-list sentinel at
host `+0x1030` and all secondary nodes, publishes each node's context through
host `+0x4`, and invokes `EclVmContext::Run` with the supplied frame delta. A
failure from the embedded context aborts with `-1`; a failed secondary context
is deleted, unlinked in both directions and followed by deletion of its node.
After the list is exhausted, the active context is restored to the embedded
context at host `+0x8`.

The two target callers at `0x0040E0FE` and `0x0040E205` are both inside the
maintained `EnemyRuntimeUpdate` owner and keep the ECL host in EDI. Connecting
that existing logical call to the generic host runner gives the linker the
real cross-TU graph. With `EnemyRuntimeUpdate` as the entry and `Enemy.cpp` as
support context, VC7.1 naturally emits the target's private EDI host ABI and
also promotes both calls into `EclVmContext::Run` to its target EAX context
ABI. Two independent cold canonical links reproduce the complete 146-byte
PDB-owned ECL contribution and all four REL32 fields: two calls to context
`Run` and two operator-delete calls.

The adjacent `EclVmStartSubroutine @ 0x0044DF70` source now re-reads the caller
instruction across virtual typed-operand resolution, uses the target's signed
argument-index comparison, and writes the new instruction/time through
`host->activeContext`. These target-observed changes raise its selected `/GL`
candidate from 487 to 518 bytes and from 22 to 73 of 534 comparable bytes. It
remains non-exact because the incomplete 7,020-byte context `Run` owner still
does not reproduce the production private ABI passed to this deeper helper.

The full current `src/EclVm.cpp` gate closes **8 units / 2 artifacts / 724
bytes**; the independently rerun `src/Enemy.cpp` gate remains **20 units / 2
artifacts / 511 bytes**. Repository totals become **1,313 candidates, 299
source mappings, and 134 exact functions / 18,454 bytes**. The authored source
backlog is **161**. The next core exact frontier is the generic
`EclVmContext::Run` owner, whose missing source shape controls the remaining
private ABI of `StartSubroutine` and `SpawnThread`.

## Completed packet: ECL dispatcher code shape

The generic `EclVmContext::Run @ 0x0044E1A0-0x0044FD0B` now reproduces the
target's dominant LTCG structure instead of compiling as a compact
helper-driven switch. Target disassembly proves that typed stack pops are
expanded inside every opcode body, formatted-operand evaluation is part of the
same owner, and typed pushes retain calls to the independently exact
`EclVmStackView::Push`. The source records that asymmetric compiler shape with
forced inline pop/format helpers and VC7.1 `inline_depth` around the push
wrappers. This keeps the natural stack operations and does not add padding,
copied bytes or artificial control flow.

In the real `EclVmHost::Run` caller context, the linked `/GL` PDB contribution
grows from **2,756 to 6,748 bytes** against the reviewed 7,020-byte owner. It
retains the target's private EAX context ABI and improves the structural
diagnostic from 82 to **402 matched bytes**. The source also restores three
direct target observations: the formatted-operand flag index is one byte, the
spawn-thread operand index uses an unsigned right shift, and a failed raw
return-stack pop leaves its destination untouched rather than receiving a
source-only zero initializer. The candidate remains 272 bytes short and is a
real mismatch with `acceptance_authority=none`; no exact row is added.

`scripts/report-ecl-vm-table.py` now accepts an optional linked candidate and
function address. It locates the candidate's unique copy of the target
88-byte selector table, validates the preceding 60 unique in-owner jump-table
destinations, and reports target/candidate relative destinations plus the gap
to each following destination. This turns the remaining work into bounded
opcode-family comparisons. Current reports show most leaf/vector/thread cases
within a few bytes, while integer multiply/divide/comparison, float arithmetic
and bitwise families account for the main residual code-shape deficit.

A cold canonical replay after these changes keeps `EclVmHost::Run` exact across
all **146 bytes and four REL32 fields**. Repository exact totals therefore stay
at **134 functions / 18,454 bytes** while the central non-exact dispatcher is
substantially closer and has a reproducible per-case triage surface.

## Completed packet: ECL dispatcher control flow

The next target/candidate comparison showed that opcode-number order was the
wrong source model. The target groups integer arithmetic before float
arithmetic, integer comparisons before float comparisons, and uses separate
target-specific thread, jump, vector and format groups. Reordering the clauses
to those target destinations makes the linked candidate reproduce the complete
59-case physical order. The report now records both physical-order lists and
fails the comparison visibly when they differ.

Two source corrections also recover target control flow. RETURN now pops the
restored instruction pointer and time directly into the context fields instead
of routing both through one scalar scratch. This restores the target `0x108`
frame and saved-register set and reduces the RETURN block from 109 candidate
bytes to 81 against target 83. TRUE and FALSE conditional jumps now enter one
shared JUMP block; that block and ordinary instruction advancement join at one
offset-based writeback tail, matching the target's table-visible block order.
The source uses only real VM state and shared control-flow labels already
present in the target.

The final selected `/GL` diagnostic is **6,648 / 7,020 bytes** with **431 / 6,316
comparable bytes** equal. Its pre-table span is **6,320 / 6,692 bytes**,
the complete case order matches, the frame is `0x108`, and the EAX receiver ABI
is unchanged. This contribution is smaller than the previous 6,748-byte
candidate because the recovered jump and advance tails remove duplicated work;
the remaining 372-byte owner gap is therefore a more meaningful mismatch.
Target/candidate disassembly localizes it mainly to typed arithmetic conversion
branches plus an EBX/EBP live-range exchange. Exactness remains unknown and no
match row is added.

`scripts/report-ecl-vm-table.py` now includes the 6,690-byte target code size,
target/candidate pre-table spans, signed span delta, both physical order lists and `physical_order_matches` in
candidate reports. The full current `src/EclVm.cpp` canonical gate remains
exact at **8 units / 2 artifacts / 724 bytes**, including the 146-byte host
runner and all four fields. Repository exact totals remain **134 functions /
18,454 bytes**.

## Completed packet: exact ANM projected photo blend

`AnmRenderManagerView::DrawMode7 @ 0x004445C0` was already semantically closed
but remained at 404 candidate bytes against the 402-byte target. The explicit
three-pointer loop let LTCG rebase its color induction variable to the dword
color address, while the target keeps the alpha address and accesses BGRA at
offsets `-3..0`. Eight independently calibrated VC7.1 identifier buckets
produced byte-identical 404-byte candidates, ruling out identifier hashing for
this register-allocation decision.

Restoring the natural `for (i = 0; i < 4; ++i)` source form lets VC7.1 derive
the target alpha/source/transformed-vertex induction variables on its own. A
fully live 16-bit snapshot of the selected red and alpha pair across the final
float-to-byte conversion then emits the target packed word reload instead of
two separate byte loads. No padding, inert local, assembly, or copied target
bytes are involved.

Two focused cold replays close the complete **402/402-byte PDB contribution and
24/24 linkage fields** in the established real two-VM draw-callback `/GL`
context. The retained negative and convergence reports are below
`.analysis/gpt-5.6-sol/20260916-anm-mode7-buckets/`. The final full-source cold
gate closes all **75 configured `src/AnmManager.cpp` units / 10 artifacts /
15,523 target bytes**. Repository totals are now **125 exact functions / 17,514
bytes**. ANM has 65 exact owners / 14,687 bytes and 58 source-present authored
owners / 27,020 bytes remaining.

## Completed packet: exact ANM surface capture

`AnmRenderManagerView::CaptureToSurface @ 0x00448450` previously produced a
378-byte linked contribution against the complete 379-byte target owner and
matched 204/335 non-linkage bytes. The existing boundary note incorrectly
described the last two bytes as padding. Direct decode establishes that
`0x004485C8-0x004485CA` is the complete three-byte `RET 36`; the canonical PDB
contribution now covers the full instruction.

TH10 Main layout places the D3D9 device at `MainSupervisorView +0x8` and the
present-parameter backbuffer format at `+0xEC`. Expressing both fields through
that common source owner restores the target's render-target and fallback-call
schedule. Retaining a natural pointer to the selected secondary surface slot
and a local `secondaryDevice` then restores VC7.1's register choice for the
second surface creation and both following D3DX copies.

Two focused cold replays close the complete **379/379-byte PDB contribution and
11/11 linkage fields** in the established `AnmResourcePipeline` `/GL` context.
The final full-source cold gate closes all **74 configured
`src/AnmManager.cpp` units / 10 artifacts / 15,121 target bytes**; the resource
artifact itself closes seven owners / 1,183 bytes. Repository totals are now
**124 exact functions / 17,112 bytes**. ANM has 64 exact owners / 14,285 bytes
and 59 source-present authored owners / 27,422 bytes remaining.

## Completed packet: exact ANM embedded texture upload

`AnmRenderManagerView::CreateTextureFromAnm @ 0x00446F40` previously produced a
261-byte linked contribution against the 264-byte target, with every difference
inside the `D3DXLoadSurfaceFromMemory` argument setup. Stack reconstruction of
the target's two `LEA` instructions shows that both resolve to the same local
rectangle built from the embedded texture header. The maintained source passed
that rectangle only as the source region and used `NULL` for the destination.
Passing `&sourceRect` for both regions restores the target call schedule and
correctly limits the destination upload to the serialized header dimensions.

Two focused cold replays close the complete **264/264-byte PDB contribution and
9/9 linkage fields** in the established `AnmResourcePipeline` `/GL` context.
The final full-source cold gate closes all **73 configured
`src/AnmManager.cpp` units / 10 artifacts / 14,742 target bytes**; the resource
artifact itself closes six owners / 804 bytes. Repository totals are now **123
exact functions / 16,733 bytes**. ANM has 63 exact owners / 13,906 bytes and 60
source-present authored owners / 27,801 bytes remaining.

## Completed packet: exact ANM float variable resolver

`AnmVmView::GetFloatVar @ 0x0043EAC0` already had the exact 424-byte linked
extent, including its two-byte alignment and 22-entry absolute jump table. The
49 remaining comparable differences occupied only the three RNG case bodies.
The maintained switch listed angle, unsigned random and signed random, while
the target physically emits unsigned random, signed random and angle. Reordering
those three case clauses restores the target body order without changing any
case value, jump-table mapping or returned value.

The canonical unit runs in the established `AnmExecutor` `/GL` context and
declares all **44 linkage fields**: `__ftol2`, six RNG calls, six RNG-owner
globals, two pi constants, both three-component global owners with explicit
addends, the switch-table base and all 22 case destinations. Two independent
focused cold replays close **424/424 bytes and 44/44 fields**. The final
full-source cold gate closes all **72 configured `src/AnmManager.cpp` units /
10 artifacts / 14,478 target bytes**. Repository totals are now **122 exact
functions / 16,469 bytes**. ANM has 62 exact owners / 13,642 bytes and 61
source-present authored owners / 28,065 bytes remaining.

The preceding `SetAndExecuteScriptIdx @ 0x0043E8B0` triage confirms its control
flow, 287-byte extent and four linkage fields. Its 22 non-field differences are
only an `ESI`/`EDI` live-range exchange. Parameter naming, top-scoped locals,
split and combined guards, direct flag access, `register`/`const`, reversed
subscript syntax, explicit link entry and `/G7` diagnostics did not produce an
evidence-backed exact form. Natural source was restored; the owner remains a
near match rather than retaining an artificial code-shaping variant.

## Completed packet: exact ANM generated draws

The refreshed dual-lane rank placed `DrawGeneratedVertices @ 0x00444CE0` and
`DrawTexturedTriangleFan @ 0x004450E0` immediately after the retained
two-byte radial-trail near match. Their linked `/GL` contributions already had
the exact 224- and 211-byte target sizes; each remaining 13-byte region was the
same texture-bind scheduling pattern recovered in Draw3D.

Both target paths push the new texture and update renderer cache `+0x3ADA64`
before loading the D3D9 device. Expressing only these texture binds through the
TH10-proved `MainSupervisorView +0x8` device field restores that order. The
other D3D9 calls remain expressed through the standalone device global. No
volatile qualifier, padding, assembly, or copied bytes is used.

Two focused cold replays close `DrawGeneratedVertices` at **224/224 bytes and
7/7 fields** and `DrawTexturedTriangleFan` at **211/211 bytes and 10/10
fields**. The final full-source cold gate closes all **71 configured
`src/AnmManager.cpp` units / 10 artifacts / 14,054 target bytes**. Repository
totals are now **121 exact functions / 16,045 bytes**. The ANM module has 61
exact owners / 13,218 bytes and 62 source-present authored owners / 28,489
bytes remaining.

## Completed packet: exact ANM 3D render state

`AnmRenderManagerView::SetRenderStateForVm3D @ 0x004423E0` previously matched
396/408 comparable bytes across its complete 440-byte linked `/GL` PDB
contribution. Its only difference was the texture-factor state call: the
candidate loaded the standalone device global before pushing/storing the new
cache value, while the target performs the push and store first.

The already established Main layout proves that target address `0x00491C30`
is `MainSupervisorView +0x8`. Expressing this one call through the same
source-local supervisor prefix used by exact Draw3D restores the target
instruction schedule. The canonical manifest binds that field to supervisor
base `0x00491C28` with addend eight; the other device uses remain tied to the
standalone global. No volatile qualifier, padding, assembly, or copied bytes is
used.

Two focused cold replays close the complete **440/440 bytes and 8/8 fields**.
The final full-source cold gate closes all **69 configured
`src/AnmManager.cpp` units / 10 artifacts / 13,619 target bytes**. Repository
totals are now **119 exact functions / 15,610 bytes**. The ANM module has 59
exact owners / 12,783 bytes and 64 source-present authored owners / 28,924
bytes remaining.

## Completed packet: exact ANM Draw2D pair

The two 591-byte rotated two-dimensional draw owners at `0x004436C0` and
`0x00443910` previously reproduced every target field and all but one
equal-length scheduling region. In the near match, VC7.1 began loading the Y
coordinate before completing the horizontal-anchor `AND/SUB`; the target
completes that integer normalization first.

The maintained source had split each three-term coordinate sum across one
assignment and two compound assignments. Restoring each to one naturally
left-associated C++ expression retains the same operand order and arithmetic,
while giving VC7.1 the target instruction schedule. Both complete linked `/GL`
PDB contributions now replay exactly: **591/591 bytes and 21/21 fields each**.
No padding, volatile dependency, copied bytes, or new assembly was introduced.

Two focused cold replays close both owners. A final full-source cold gate closes
all **68 configured `src/AnmManager.cpp` units / 10 artifacts / 13,179 target
bytes**. Repository totals are now **118 exact functions / 15,170 bytes**. The
ANM module has 58 exact owners / 12,343 bytes and 65 source-present authored
owners / 29,364 bytes remaining.

## Completed packet: exact ANM Draw3D

Fresh ANM/ECL dual-lane ranking after `3fd501c` leaves the two-byte X87
`UpdatePulsingRadialTrail` experiment first and places the 929-byte
`AnmRenderManagerView::Draw3D @ 0x00444760` second. Its diagnostic linked `/GL`
contribution already had exact size, all 25 fields visible and only thirteen
non-field bytes different. Those bytes form one contiguous instruction-order
change around the texture-cache update.

Three ordinary source spellings through the standalone `g_Direct3DDevice`
pointer all emitted the same device-load/push/store order. Target-local Main
layout establishes that address `0x00491C30` is also the `d3dDevice` field at
`MainSupervisorView +0x8`, based at `0x00491C28`. Expressing the texture call
through a source-local supervisor prefix changes VC7.1's alias scheduling to
the exact target push/cache-store/device-load sequence. The source continues to
perform the same cache update and COM call; no volatile qualifier, inline
assembly, padding or copied bytes are used.

The canonical linked unit covers the complete 929-byte PDB contribution and
all 25 fields. Its supervisor field manifest records base `0x00491C28` with
addend eight, which disambiguates the address from a neighboring four-byte
diagnostic anchor. Two cold replays close Draw3D; the second also closes the
four adjacent renderer owners in the same artifact for **1,756/1,756 bytes**.
The final full-source regression gate closes all **66 configured units / 10
cold artifacts / 11,997 target bytes** with no failure.
Repository totals are now **116 exact functions / 13,988 bytes**. The ANM module
has 56 exact owners / 11,161 bytes and 67 source-present authored owners /
30,546 bytes remaining.

## Completed packet: ANM exact boundary repair

Dual normal-COFF and linked `/GL` backlog ranking exposed a systematic boundary
error: eight reviewed spans ended on the `C2` opcode of `RET imm16` but omitted
its two-byte immediate. The boundary audit had reported incomplete dense decode,
yet its prior-review path retained `reviewed` state. The corrected extents are:

- compiler-owned deleting helpers `0x00425070-0x0042508A` and
  `0x004296D0-0x004296EA`;
- `TranslateRotation @ 0x00443680-0x004436B3`;
- `QueueSpriteQuad @ 0x00444DC0-0x00444E50`;
- untextured strip/fan `0x00444E60-0x00444F9A` and
  `0x00444FA0-0x004450DA`;
- `LoadSurface @ 0x00447C80-0x00447EBF`;
- `CopyTextureRect @ 0x00448360-0x00448442`.

`scripts/report-boundary-inventory.py` now recognizes a one- or two-byte
trailing `C2/CA` suffix as a truncated `RET imm16` and refuses to preserve a
reviewed result for it. All eight corrected owners densely decode through a
complete return. The correction adds sixteen tracked `.text` bytes without
changing the candidate or review-state counts.

Four corrected ANM owners were already exact linked `/GL` contributions once
the missing immediates were admitted: 52-byte `TranslateRotation`, 145-byte
`QueueSpriteQuad`, and the two 315-byte untextured strip/fan methods. Their
complete canonical manifests cover 827 bytes and 37 linked fields. Normal COFF
also closes the adjacent 66-byte `MarkVmForDeletion` plus its sole `FindVm`
REL32 relocation. Two independent cold aggregate replay passes return exact
for all five units, **893/893 bytes**. Repository totals are now **115 exact
functions / 13,059 bytes**; ANM accounts for 55 exact owners / 10,232 bytes,
with 68 source-present authored owners / 31,475 bytes remaining.

The same ranking places `UpdatePulsingRadialTrail @ 0x00445620` at 598/598
bytes with all eighteen fields resolved and 524/526 comparable bytes equal.
The two differences swap commutative X87 operands for only the Y component of
`position + positionOffset`. Bounded natural-source variants did not reproduce
that scheduling, so this owner remains honestly non-exact.

The final full-source regression gate also caught a stale source-shape
interaction in an older exact owner. Typing ASCII manager field `+0x89A4` as
the non-trivial `AnmVmIdView` had inserted a target-absent pre-clear before the
constructor's full-object `memset`, growing its linked contribution from the
canonical 251 bytes to 255. Removing that constructor globally repaired ASCII
but changed exact `AddVmVariant0` from 118 to 119 bytes. The maintained layout
now expresses only this ASCII field as its target-used scalar integer storage;
general VM-id return values retain their established wrapper semantics. A
focused cold replay closes the 251-byte ASCII constructor and both 118-byte
AddVm tails. The final full `src/AnmManager.cpp` gate closes all **65 configured
units / 10 artifacts / 11,068 compared bytes** with no failure.

## Completed packet: front-end score entry

The score-entry screen discriminator 15 now routes to maintained update and draw
owners instead of unresolved external stubs:

- `FrontEndControllerView::UpdateScoreEntry @ 0x00432CB0-0x00433227`
  (1,400 bytes)
- `FrontEndControllerView::DrawScoreEntry @ 0x00433230-0x00433569`
  (826 bytes)

The four-state updater inserts the current score into the selected
character/shot/difficulty table, loads the profile's last eight-character name,
and drives a 91-entry wrapping keyboard laid out in thirteen columns. The final
three entries are space, backspace and accept. Accept copies the name into both
the inserted 0x18-byte score record and profile offset `+0x1D878`; cancellation
erases one character, while an insertion failure skips directly to the result
screen after confirmation. The owner creates and retires the target
`0x68/0x98+/0x9A+/0xA0+` ANM scripts and preserves the target opening/closing
timer thresholds.

The draw owner emits all ten score rows using the same target record layout as
the practice screen, highlights the inserted row, overlays the editable name and
caret on that row, and draws all 91 glyphs at `(212,360)` with 18-pixel columns
and 16-pixel rows. Controller layout review binds the name at `+0x58DC`, name
cursor at `+0x58E8`, insertion-failure flag at `+0x58EC`, and a complete second
`FrontEndCursorView` at `+0x58F4`; its target-written wrap field is therefore
the existing cursor member at `+0x59C4`.

Boundary review extends the update owner beyond Ghidra's executable end. Code
ends with `RET 4` at `0x00433213-0x00433215`, `MOV EDI,EDI` occupies
`0x00433216-0x00433217`, and the directly indexed four-entry table at
`0x00433218-0x00433227` targets `0x00432CCC`, `0x00432EDA`, `0x00432F00`, and
`0x004331AC`. Eight `CC` bytes separate the draw entry; its `RET 4` ends at
`0x00433569`, followed by six `CC` bytes. The focused
`scripts/report-frontend-score-entry.py --check` binds these extents, both
dispatcher call sites, direct-call multisets, alphabet/table bytes, source
markers and controller offsets to the canonical target.

Pinned VC7.1 SP1 build6030 compiles the expanded `src/FrontEnd.cpp` under normal,
`/GL`, and `/W4` profiles; `/W4` adds only the established anonymous-union
extension warnings. Normal COMDATs are 1,384/781 bytes and mismatch at 72/1,096
and 22/718 comparable bytes. With `UpdateScoreEntry` selected as the linked
`/GL` entry, PDB contributions are 1,392/781 bytes and mismatch at 88/1,100 and
22/718 comparable bytes. Every diagnostic has `acceptance_authority=none`; no
exact row is added. Repository totals become 282 source mappings and remain 110
exact functions / 12,166 bytes.

## Completed packet: front-end/options core

The selected title/options chain now has maintained source across three authored
owners and 8,315 reviewed target bytes:

- `FrontEndControllerView::Update @ 0x0042CDF0-0x0042D257` (1,128 bytes)
- `FrontEndControllerView::UpdateOptions @ 0x0042D920-0x0042E593` (3,188 bytes)
- `FrontEndControllerView::RefreshOptionsDisplay @ 0x0042E5A0-0x0042F53E`
  (3,999 bytes)

Boundary review corrected the first two imported extents. The title dispatcher
ends executable code at `0x0042D212`, has one NOP, and owns its directly
referenced 17-entry screen table through `0x0042D257`. The options controller
ends with `RET 4` at `0x0042E57D-0x0042E57F` and owns its directly referenced
five-entry state table through `0x0042E593`. Both tables were previously left
in untracked gaps by the terminal-instruction heuristic.

The source covers the complete 17-screen title dispatcher, 900-frame demo replay
rotation, front-end ANM cleanup and re-entry, the five-state/six-row options
controller, color-depth cycling, BGM/SFX adjustment and preview, defaults,
key-configuration transition and return. The display owner reproduces the
observed sound-state update and quartic BGM attenuation curve, queues `SetVol`,
updates twelve digit sprites and toggles the four normal/selected leading-digit
pairs.

Repeated child traversal in both this owner and the earlier GUI source compares
ANM VM `scriptIndex` at `+0x38A`. The maintained GUI helper had instead compared
`activeSpriteIndex` at `+0x384`; this packet corrects that semantic field error.
`FrontEndCursorView` is closed at `0xD8`, and the maintained partial controller
binds screen/state/cursor at `+0x1C/+0x20/+0x24`, timer at `+0x2B0`, and VM ids
from `+0x2C4` through slot `0x5B` at `+0x430`.

`scripts/report-frontend-core.py --check` fails closed on the target and verifies
the two tables, tails/padding, selected direct-call multiplicities, source marker
coverage, state enums, layout assertions and the `+0x38A` lookup invariant.
Pinned VC7.1 SP1 build6030 compiles `src/FrontEnd.cpp` under fixed normal,
`/GL`, and `/W4` profiles. All three normal and linked diagnostics are real
mismatches with `acceptance_authority=none`: normal object sizes are 324, 928
and 203 bytes; selected linked `/GL` contributions are 368, 884 and 199 bytes.
The target owners are dominated by optimizer-inlined helpers and repeated VM
lookup paths, so no match row or exactness promotion is made.

Repository totals at that checkpoint became **1,312 candidates, 266 source
mappings and 110 exact functions / 12,166 bytes**.

## Completed packet: front-end key-configuration core

The connected continuation now has maintained source across three more authored
owners and 3,532 reviewed target bytes:

- `FrontEndControllerView::UpdateKeyConfig @ 0x0042F540-0x0042F8AF`
  (880 bytes)
- `FrontEndControllerView::RefreshKeyConfigDisplay @ 0x0042F8B0-0x00430243`
  (2,452 bytes)
- `FrontEndControllerView::AssignKeyConfigBinding @ 0x00430250-0x00430317`
  (200 bytes)

The first owner ends code at `0x0042F89A`, has one NOP, and owns the five-entry
state table at `0x0042F89C-0x0042F8AF`. This closes the table that the boundary
inventory had previously reported outside its owner. The display and assignment
owners are separated from their successors by twelve and eight `CC` bytes.

The five-state/seven-row controller creates key-config root VM slot 2, scans 31
signed controller-button transition bytes, handles cursor interrupts, edits the
five configurable actions, resets from live mappings, commits the complete
mapping and returns to options. Duplicate button assignments are resolved by
swapping the displaced binding. The display path updates twenty digit VMs: two
digits in each of the normal and selected rows for each editable action.

TH10-local input and configuration consumers establish the runtime mapping at
`0x00474E88` as nine adjacent signed shorts with target defaults
`0,1,2,3,-1,-1,-1,-1,4`. Target button masks identify the first four as
shot/bomb/focus/menu and the last as skip. The maintained up/down/left/right
names for the preserved middle four entries are adjacent-supported and remain
provisional because the reviewed TH10 controller path derives direction bits
from axes. The key-config scratch copy contains the first four actions plus skip
at controller `+0x59CC`; commit preserves the middle entries and copies the
complete 18-byte mapping to persisted storage at `0x00491D4C`.

`scripts/report-frontend-key-config.py --check` fails closed on the exact target
and validates all three extents, the five-entry table, tails/padding, complete
direct-call multisets, the display owner's 40 FindVm transfers and 19 SetSprite
calls plus one tail jump, the nine-short defaults, source markers/state coverage
and layout assertions.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass. The new normal
COMDATs are 624/113/171 bytes versus target 880/2,452/200 and remain real
mismatches at 15/700, 9/2,432 and 19/192 comparable bytes, each with
`acceptance_authority=none`. No match row or exactness promotion is made.

Repository totals become **1,312 candidates, 269 source mappings, 285 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,260 `.text` bytes; the indexed-table queue is now 54 references inside
their recorded owner and 46 outside.

## Completed packet: front-end selection core

The next contiguous front-end corridor now has maintained source across three
authored owners and 3,268 reviewed target bytes:

- `FrontEndControllerView::UpdateDifficulty @ 0x00430320-0x00430697`
  (888 bytes)
- `FrontEndControllerView::UpdateCharacter @ 0x004306A0-0x00430A5B`
  (956 bytes)
- `FrontEndControllerView::UpdateShotType @ 0x00430A60-0x00430FEF`
  (1,424 bytes)

All three owners use a five-state selection protocol. Difficulty distinguishes
the four normal difficulties from Extra, selects VM root slot `0x77/0x78`,
manages screen VM `0x5E`, auxiliary VM `+0x5D0` and transition VM `0x62`, then
advances to character selection or restores the title-menu state. Character
filters both entries from the six Extra unlock bytes at profile
`+0x1D888..+0x1D88D`, drives the horizontal ANM cursor and sends four
complementary child interrupts on confirmation. Shot type filters three
per-character unlocks, suppresses three unavailable profile-record child rows,
drives the vertical ANM cursor and commits normal-game or practice stage state.
The normal path creates the observed ASCII VM at `(480,392)`, starts the
32-frame transition and publishes stage record zero or six, stage one or seven,
and game mode seven.

Boundary review closes three tables previously reported outside their owners.
Difficulty code ends at `0x00430680`, has a three-byte alignment instruction,
and owns its five-entry table at `0x00430684-0x00430697`. Character code ends at
`0x00430A47` and its table immediately follows through `0x00430A5B`. Shot-type
code ends at `0x00430FDA`, has one NOP, and owns its table at
`0x00430FDC-0x00430FEF`. The controller layout now covers VM slots through
`0x97`, the auxiliary difficulty VM id at `+0x5D0`, saved difficulty at
`+0x58F0`, and the existing key-config scratch at `+0x59CC`. The ASCII manager
view names its separately observed auxiliary VM id at `+0x89A4`.

`scripts/report-frontend-selection.py --check` fails closed on the canonical
target and validates the three complete owners, table destinations,
alignment/tails, complete direct-call multisets, five state values, source
markers and the new layout offsets. The existing front-end/options and
key-config reports still pass after the shared layout expansion.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass. Normal COMDATs are
876/872/1,196 bytes against target owners 888/956/1,424. Strict normal
diagnostics are real mismatches at 40/640, 49/740 and 49/1,120 matched
comparable bytes, with 62/54/76 relocations and `acceptance_authority=none`.
No match row or exactness promotion is made.

Repository totals become **1,312 candidates, 272 source mappings, 288 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,324 `.text` bytes; the indexed-table queue is now 57 references inside
their recorded owner and 43 outside.

## Completed packet: front-end stage core

The stage-selection continuation now has maintained source across two authored
owners and 1,471 reviewed target bytes:

- `FrontEndControllerView::UpdateStage @ 0x00430FF0-0x0043140B`
  (1,052 bytes)
- `FrontEndControllerView::DrawStageScores @ 0x00431410-0x004315B2`
  (419 bytes)

The five-state update owner restores a six-entry cursor, creates the stage and
character-specific ANM VMs, handles movement/cancel/confirm, rejects unavailable
profile stages, records numeric-key stage shortcuts, and completes the
stage-record/game-mode transition. Direct target review identifies one shared
256-byte keyboard buffer: the helper at `0x0044B010` returns nonzero after the
DirectInput path, selecting scan codes `2..10`, and zero after the Win32
`GetKeyboardState` path, selecting virtual-key codes `0x31..0x39`.

The display owner is called at `0x0042D2A8` with the controller retained in
private EDI. It draws the six stage rows at x=168 or 296 and y=216 with 18-pixel
spacing, prints the stored score or an unavailable marker, distinguishes the
current row by color, blinks it during confirmation, and restores ASCII color
and shadow state. Profile entries are eight bytes: score at `+0`, an unknown
byte at `+4`, availability at `+5`, then two unknown bytes. The selected record
is indexed by stage `1..6` plus difficulty times six within the
character/shot-group block.

Boundary review extends the update owner past its final `RET 4` at
`0x004313F4-0x004313F6`: one NOP follows and its directly indexed five-entry
state table occupies `0x004313F8-0x0043140B`. Four `CC` bytes separate the
display owner, whose RET at `0x004315B2` is followed by thirteen `CC` bytes.
This adds 21 bytes to tracked `.text` union coverage and moves one indexed table
inside its owner.

`scripts/report-frontend-stage.py --check` fails closed on the canonical target
and validates both extents, the state table, tails/padding, complete direct-call
multisets, draw-callback call site, stage-name pointers/strings, 18.0 row
spacing, source markers, profile layout tokens and dispatcher integration.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass; `/W4` reports only
the existing anonymous-union extension warnings. Normal COMDATs are 660 and 378
bytes versus target owners of 1,052 and 419 bytes. Strict diagnostics are real
mismatches at 26/876 and 5/371 matched comparable bytes, with 44 and 12
relocations and `acceptance_authority=none`. No match row or exactness promotion
is made.

Repository totals become **1,312 candidates, 274 source mappings, 290 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,345 `.text` bytes; the indexed-table queue is now 58 references inside
their recorded owner and 42 outside.

## Completed packet: front-end replay core

Replay selection now has maintained source across two authored owners and 2,317
reviewed target bytes:

- `FrontEndControllerView::UpdateReplay @ 0x004315C0-0x00431B93`
  (1,492 bytes)
- `FrontEndControllerView::DrawReplay @ 0x00431BA0-0x00431ED8`
  (825 bytes)

The six-state update owner loads the twenty-five numbered replay slots, scans up
to twenty-five additional `th10_ud????.rpy` files, preserves the target's
unconditional `FindClose` error path, and constructs the seven-stage cursor from
the selected replay's stage headers. Confirmation publishes the selected stage,
stage record, replay path, character, shot type and difficulty before entering
game mode 12. Cancellation releases all fifty replay pointers and the replay
screen's ANM VM tree before restoring the outer cursor.

The display owner draws twenty-five fixed replay rows with replay name, date,
character/shot, difficulty, completion marker and slowdown percentage. After a
file is chosen, its summary moves from the selected list row to y=80 over ten
frames, then seven stage rows appear at x=220/y=128 with 18-pixel spacing. For
stages before the last available stream, the displayed score and trailing digit
come from the next stage header at `+0x0C/+0x1B4`; otherwise they come from replay
header `+0x10/+0x60`. Controller layout assertions bind the replay fields at
`+0x59D8/+0x59DC/+0x59E0` and the fifty-pointer array at `+0x59E4..+0x5AAB`.

Boundary review corrects the prior code-only extent for the update owner. Its
`RET 4` ends at `0x00431B79`, `MOV EDI,EDI` occupies `0x00431B7A-0x00431B7B`,
and the directly indexed six-entry table spans `0x00431B7C-0x00431B93` with
destinations `0x004315F2`, `0x0043174D`, `0x004317B1`, `0x004319B4`,
`0x004318FD` and `0x00431A9E`. Twelve `CC` bytes precede the display owner; its
`RET 4` is followed by seven `CC` bytes before `0x00431EE0`. This adds 26 bytes
to tracked `.text` union coverage and moves one indexed table inside its owner.

`scripts/report-frontend-replay.py --check` fails closed on the canonical target
and validates the two extents, table/tails, complete direct-call multisets,
update/draw callback call sites, name tables, display constants, source markers,
state domain and controller offsets.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass; `/W4` reports only
the existing anonymous-union warnings. The normal COMDATs are 1,588 and 554
bytes versus target owners 1,492 and 825, with 89/1,184 and 33/753 comparable
bytes matching and 88/18 relocations. In the selected replay-update linked
`/GL` context the contributions are 1,672 and 554 bytes with 102/1,196 and
33/753 comparable bytes matching. Every diagnostic is a real mismatch with
`acceptance_authority=none`; no match row or exactness promotion is made.

Repository totals become **1,312 candidates, 276 source mappings, 292 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,371 `.text` bytes; the indexed-table queue is now 59 references inside
their recorded owner and 41 outside.

## Completed packet: front-end practice draw and discriminator correction

The practice-record screen is now complete through its draw path, and the
front-end draw dispatcher itself has maintained source:

- `FrontEndControllerView::Draw @ 0x0042D260-0x0042D2D3` (116 bytes)
- `FrontEndControllerView::DrawPractice @ 0x004329F0-0x00432CA0`
  (689 bytes)

Reviewing the target draw table exposed a semantic error in the maintained
screen enum. Target discriminator 11 dispatches to `UpdatePractice @
0x00431EE0` and `DrawPractice @ 0x004329F0`; discriminator 12 dispatches to
`UpdateReplay @ 0x004315C0` and `DrawReplay @ 0x00431BA0`. The enum is corrected
accordingly, so every maintained `SetScreen` and both central dispatchers now
use the target-proven values.

The practice draw owner renders ten 0x18-byte score records on summary page
zero. Each record supplies a score, stage-name selector, suffix digit,
ten-byte name, timestamp and slowdown percentage. Rows fade from white by
subtracting `0x10` from the red and green channels for each successive entry.
The footer
always prints the selected shot's aggregate value at profile `+0x4C8`, converts
the frame total at `+0x4CC` to `hours:minutes:seconds`, and prints the selected
difficulty value at `+0x4D0 + difficulty*4`.

The draw dispatcher code ends at `0x0042D2B3` and owns its eight-entry table at
`0x0042D2B4-0x0042D2D3`. Its `0x0042D2F0` callback adapter remains a separate
seven-byte candidate. `DrawPractice` ends in `RET` at `0x00432CA0`, followed by
fifteen `CC` bytes. The expanded `scripts/report-frontend-practice.py --check`
validates these extents, both dispatch tables, complete direct-call multisets,
formats, screen discriminators, profile-access source markers and the earlier
practice update/refresh evidence.

Pinned VC7.1 normal, selected-entry `/GL`, and `/W4` probes pass. Normal `Draw`
and `DrawPractice` COMDATs are 116/585 bytes versus target owners 116/689 and
mismatch at 10/60 and 51/625 comparable bytes with 14/16 relocations. Selected
`/GL` contributions are 120/585 and mismatch at 15/64 and 54/625 comparable
bytes. The equal normal size of `Draw` is diagnostic only; no exactness is
claimed.

Repository totals become **1,312 candidates, 280 source mappings, 296 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage
remains 340,390 `.text` bytes and the indexed-table queue remains 60 references
inside their recorded owner and 40 outside.

## Completed packet: front-end practice-record core

The next front-end owner pair now has maintained source across 2,818 reviewed
target bytes:

- `FrontEndControllerView::UpdatePractice @ 0x00431EE0-0x0043268F`
  (1,968 bytes)
- `FrontEndControllerView::RefreshPracticeRecords @ 0x00432690-0x004329E1`
  (850 bytes)

The four-state updater maintains three independent cursors: six character/shot
combinations at controller `+0x24`, five difficulties at `+0x0FC`, and record
pages at `+0x1D4`. It creates the character, shot, difficulty, navigation and
ten ASCII-row ANM VMs, refreshes the displayed page after selection changes,
cycles back to the summary page, and retires the complete VM set when returning
to the title menu. The ten row ids occupy controller `+0x5D4..+0x5FB`; the
number of populated rows is stored at `+0x2AC`.

The refresh owner filters the target's 110-entry spell-card catalog by its
difficulty byte table, skips ten matching records per preceding page, and emits
up to ten centered ANM text rows. Unlocked catalog names at profile
`+0x19A8C + index*0x90` are padded to 42 bytes; the two displayed counters come
from profile `+0x624 + shot*0x437C + index*0x90`. Locked entries use the target
placeholder format, and unused row VMs receive a single space.

The Extra/third-shot selection also enables the target's hidden keyboard path.
It snapshots all 256 keys, derives rising edges, accepts the 22-entry scan-code
sequence at `0x0046EF10`, resets on another press among codes `0..56`, and
expires after 300 frames. Completion invokes the profile-record unlock helper
and queues sound 44. This behavior is source-present; the helper at
`0x0042C8C0` remains a separate source-absent leaf.

Boundary review extends the updater past its prior code-only end. `RET 4` ends
at `0x0043267C`, `LEA ECX,[ECX]` occupies `0x0043267D-0x0043267F`, and the
directly indexed four-entry table spans `0x00432680-0x0043268F` with state
destinations `0x00431F06`, `0x00432095`, `0x004320BA`, and `0x00432615`. The
refresh helper ends in `RET` at `0x004329E1`; fourteen `CC` bytes separate it
from `0x004329F0`. This adds 19 tracked bytes and moves one indexed table inside
its owner.

`scripts/report-frontend-practice.py --check` fails closed on the canonical
target and validates both extents, the state table, tails/padding, complete
direct-call multisets, parent dispatch call, difficulty and secret-key tables,
row formats, source markers, state domain, and controller offsets. Existing
front-end focused reports remain regression gates.

Pinned VC7.1 build6030 normal, selected-entry `/GL`, and `/W4` builds pass. The
normal COMDATs are 1,692/535 bytes against target owners 1,968/850, with
70/1,592 and 28/786 matched comparable bytes and 94/16 relocations. The `/GL`
contributions are 1,764/543 bytes and mismatch at 65/1,596 and 23/786
comparable bytes. Every diagnostic has `acceptance_authority=none`; no match row
or exactness promotion is made.

Repository totals become **1,312 candidates, 278 source mappings, 294 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,390 `.text` bytes; the indexed-table queue is now 60 references inside
their recorded owner and 40 outside.

## Completed packet: Main execution corridor

Ten central application/window/D3D owners now have maintained source in
`src/Main.cpp` across 5,456 reviewed target bytes:

- `WinMain @ 0x00438AD0`
- `GameWindowView::WindowProc @ 0x004390E0`
- `GameWindowView::Present @ 0x004391F0`
- `GameWindowView::Render @ 0x00439390`
- `GameWindowView::GetTimestamp @ 0x00439540`
- `GameWindowView::CreateGameWindow @ 0x00439730`
- `GameWindowView::InitD3DRendering @ 0x00439890`
- `GameWindowView::ResetRenderState @ 0x00439D20`
- `GameWindowView::CheckForRunningGameInstance @ 0x00439FF0`
- `GameWindowView::CalcExecutableChecksum @ 0x0043A1C0`

The source covers startup and configuration, the Win32 message/device-loss
loop, restart and teardown, window activation/close handling, frame timing,
gameplay/draw callback chains, ANM buffer servicing, presentation and snapshot
capture, window/fullscreen setup, the D3D9 HAL/software/REF fallback ladder,
view/projection/viewport setup, single-instance and launch-path handling, and
executable checksum calculation. `ResetRenderState` retains the complete target
order of 19 render-state, eight texture-stage and six sampler-state calls plus
five ANM manager cache writes.

`src/Main.hpp` provides target-bounded Win32/D3D9 views without changing the
existing exact-sensitive D3D declarations. Compile-time assertions close
`GameWindowView` at `0x58` bytes and bind its QPC/path/timestamp fields. They
also bind the Main supervisor's D3D objects, matrices, viewport and
presentation parameters, serialized configuration, gameplay and active
viewports, transition/render flags, music owner, caps, seven critical sections,
render-state cache and last-frame duration. The allocation registry is exactly
`0xA004` bytes.

`scripts/report-main-corridor.py --check` fails closed on the wrong executable
and validates all ten extents, epilogues and separating padding, the registered
window-procedure pointer, source marker order, layout assertions and all 33 D3D
state calls. It reports ten owners / 5,456 bytes.

Pinned VC7.1 SP1 build6030 compiles `src/Main.cpp` under fixed normal `/GS`, the
same profile plus `/GL`, and `/W4`. Nine owners remain diagnostic mismatches.
`GetTimestamp` is 282/282 bytes in both lanes and matches 159/162 comparable
bytes; the only differences are three instances of the stack-frame constant
`0x10` versus target `0x14`. In the selected `Render` LTCG context,
`CreateGameWindow` is also 341/341 bytes but differs in 11 of 261 comparable
bytes.

`ResetRenderState` is the strongest result: normal and selected `/GL` lanes both
produce the complete 710-byte extent, account for all 34 linkage fields and
match all 574 comparable bytes. Both diagnostics are `structural-exact` with
`acceptance_authority=none`. Physical normal-COFF versus LTCG ownership and the
original TU identity are still unresolved, so no canonical unit or exact ledger
row is added.

Repository totals become **1,309 candidates, 263 source mappings and 110 exact
functions / 12,166 bytes**. The remaining nearby Main owners are bounded leaf
or medium seams at `0x00438A30/60`, `0x004392E0`, `0x00439350`, `0x00439660`,
`0x00439700`, `0x0043A1B0`, `0x0043A290`, and `0x0043A3A0`; they remain in the
Web queue. The central Main execution skeleton is now source-present.

## Completed packet: generic typed-stack ECL VM

`EclVmContext::Run @ 0x0044E1A0` is now maintained in `src/EclVm.cpp`.
This is the generic TH10 VM below the already reconstructed Enemy-specific
`0x100-0x1B4` dispatcher, rather than another part of that high-opcode switch.
The target's 88-byte selector table contains 59 active generic opcodes and
routes 29 values to the host's virtual extension dispatcher.

The source recovers time and difficulty gating, ordinary advancement, call,
return and conditional/unconditional jumps, child-thread control, frame entry
and leave, the typed operand stack, local and host-backed int/float operands,
assignment, arithmetic, comparison, logical and bitwise operations, sine,
cosine, polar conversion, angle normalization, vector length squared and point
angle. Opcode `0x1E` retains the target-observed format scan and operand
evaluation even though the target makes no output call. Opcode `0x55` retains
the target's unusual integer `NEG` of the raw float dword before pushing it as
type `f`; it is deliberately not rewritten as ordinary floating negation.

Direct target review expands the physical owner from Ghidra's reachable body
to `0x0044E1A0-0x0044FD0B` = 7,020 bytes. Executable code ends at
`0x0044FBC1`, two alignment bytes follow, the 60-entry destination table spans
`0x0044FBC4-0x0044FCB3`, and the 88-byte selector table spans
`0x0044FCB4-0x0044FD0B`; four `CC` bytes precede independent
`0x0044FD10`. `scripts/report-ecl-vm-table.py --check` verifies the target
identity, this layout, the sole default slot, and exact target/source opcode
coverage.

The context is a target-observed `0x1024`-byte extended view: float time at
`+0x0000`, instruction at `+0x0004`, `0x1000` stack bytes at `+0x0008`, stack
top/frame base at `+0x1008/+0x100C`, thread id and host at
`+0x1010/+0x1014`, thread control and difficulty mask at
`+0x1018/+0x101C`, and flags at `+0x1020`. For the embedded Enemy context this
view overlaps the separately known host fields immediately after its former
`0x1018` prefix; dynamically spawned contexts allocate the full `0x1024`
bytes. Integer operands use negative host ids and `-1` as stack-pop; float
operands use values below zero as host ids, `-1.0f` as stack-pop, and
nonnegative flagged values as frame offsets.

Raw entry/caller/return evidence establishes a private EAX context plus one
stack float and `RET 4`. Natural source expresses the logical member method;
fixed normal VC7.1 emits a 2,760-byte COMDAT, and the `/GL` PDB contribution is
also 2,760 bytes, versus the 7,020-byte source-owned target extent. Both
diagnostics are mismatches with `acceptance_authority=none`; normal, `/GL` and
`/W4` compilation pass and no exact claim is added. Repository totals become
**1,309 candidates, 251 source mappings and 110 exact functions / 12,166
bytes**.

The adjacent `0x0044DF70` subroutine setup and
`0x0044FD10-0x004506D0` runner/operand/thread/stack helpers remain bounded leaf
seams. The next user-selected core batch is GUI `0x00414900 + 0x00415E90`,
followed by the Main corridor.

## Completed packet: Player update callback/core split

The largest confirmed authored source gap was
`PlayerUpdateCallbackBody @ 0x00425730-0x00426340`, even though its full semantic
implementation already existed under the name `PlayerUpdateCallback`. The source
mapping had been attached only to the seven-byte registered entry at `0x00426500`.
Fresh target review confirms two distinct physical boundaries: the registered ECX entry is
exactly `push ecx; call 0x00425730; ret`, while the 3,089-byte body loads one
stack Player pointer and ends in `ret 4`.

Maintained source now represents those boundaries directly.
`PlayerUpdateCallbackBody` is a `__stdcall` body in `src/Player.cpp`; the
separate `src/PlayerUpdateCallback.cpp` defines the `__fastcall` chain entry.
The core body retains the complete five-state update machine, power and death
transitions, movement/options, 32 active effect rows, highlight and update
timers, collision-derived vectors, input owner and 128-shot update.

Ghidra still proposes a two-parameter `__thiscall` signature for the body, but
the raw entry, sole incoming call, stack load and `ret 4` establish the actual
machine ABI. Its reachable-body count remains 3,060 because the reviewed
physical extent contains five alignment sleds totaling 29 bytes; its separate
five-entry state table remains at `0x00426344-0x00426357`.

Pinned VC7.1 normal compilation emits a 2,800-byte body COMDAT including its
compiler-owned table versus the target's 3,089-byte body plus separate table;
the target diagnostic remains non-exact at 139/2,693 comparable bytes with 99
relocations. Normal, `/GL`, and `/W4` compilation all succeed. The isolated
adapter naturally emits all seven target bytes and its sole REL32 resolves to
`0x00425730`, but its source-written-versus-LTCG-generated origin remains
unknown. It therefore stays a structural diagnostic without a canonical exact
claim. Repository totals become **1,309 candidates, 250 source mappings and 110
exact functions / 12,166 bytes**.

## Completed packet: Enemy runtime callback resolver

`EnemyResolveRuntimeCallback @ 0x004127A0-0x004129F0` is now maintained in
`src/EnemyCallback.cpp` with its target-observed ECX-bound fastcall ABI. Both
direct callers in `EnemyRuntimeUpdate` supply the full `0x2518`-byte Enemy
owner. The function scans eight `0x10`-byte callback records beginning at full
Enemy `+0x2494`; each record contains a life threshold, timer threshold, life
callback name and timer callback name.

The first active life record stores `life - threshold` at full `+0x2404`. When
life reaches the threshold, it clamps life, disables that record, resets the
update timer, clears runtime flag `0x10000` and returns the life callback name.
The first active timer record publishes a capped `0..99` seconds countdown at
visual owner `+0x9EC0`. At expiry it performs the same life/timer transition,
sets flag `0x10000`, subtracts 3000 from the global timeout value with a floor
of 5000, and conditionally clears bit one from eight Player state words before
returning the timer callback name.

Direct disassembly corrected the timer reset source: the target initializes
`current=0`, `previous=-999999`, `subframe=0`, installs the shared scale pointer
and flag bit zero when needed, then always finishes with `current=0`,
`subframe=0`, and `previous=-1`. Callback-record and full-runtime fields now
carry descriptive or offset-neutral names instead of the former misleading
`threshold/callbackId/state` and `ageCounter` names.

The isolated source compiles with pinned VC7.1 SP1 build6030 under fixed normal,
`/GL`, and `/W4` profiles. Its final normal fastcall COMDAT is 598 bytes versus
the target's 593 bytes and remains a diagnostic mismatch at 48/565 comparable
bytes, so no exactness is claimed. Keeping it outside `Enemy.cpp` prevents the
new control flow from renumbering VC7 compiler-local switch labels; a cold
replay still reproduces all **20 existing Enemy units / 511 bytes** exactly.
Repository totals become **1,309 candidates, 249 source mappings and 110 exact
functions / 12,166 bytes**.

The adjacent `0x00412720` callback-record setter and the Ghidra-missed retained
bodies at `0x00412750/0x00412790` remain in the Web leaf queue. The next useful
core frontier should stay inside Enemy/ECL or move to another high-fanout core
owner instead of consuming those leaves.

## Completed packet: complete Enemy ECL dispatcher

`EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` now has a complete
maintained body in `src/EnemyEclDispatcher.cpp`. Direct decoding of the
hash-attested target's 181-byte selector table and 108-entry destination table
finds 124 active opcode values and 57 values routed to the common default. The
maintained enum has the same 124 unique values and the switch has exactly one
case label for every enum member. The full family spans enemy creation and ANM,
motion/interpolation, hitboxes and lifecycle flags, items, dialog/spells,
rank/difficulty selection, bullet patterns and lasers.

The source preserves the separately reviewed physical owner
`0x0040E770-0x00411FBF` = 14,416 bytes: executable code through
`0x00411D2E`, alignment, the two top-level tables and both five-entry nested
difficulty tables. The 108 number describes jump destinations rather than
semantic opcode coverage. `scripts/report-ecl-dispatch-table.py --check`
replays this distinction directly against the canonical target and fails if a
target-active opcode is missing, a source opcode reaches the target default, or
enum/case values are duplicated.

Decompiler stack-overlap artifacts were removed from the source representation.
Enemy creation uses the existing typed 0x40-byte `EnemySpawnRequestView`; laser
instructions use a 0x1F8 union-backed request scratch that contains both the
0x1DC curved-laser prefix and the complete straight-laser request. This avoids
array overruns and preserves the target's packed short and float bit fields.
Opcode and helper identifiers remain descriptive where original symbols cannot
be recovered, and the six private argument helpers remain declaration-only leaf
seams for the Web queue.

Pinned VC7.1 SP1 build6030 compiles the complete dispatcher under both fixed
normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus
`/GL`. No target-byte or bounded linked-image comparison is accepted for this
LTCG-sized owner, so exactness remains unknown. Repository totals are **1,309
candidates, 248 source mappings and 110 exact functions / 12,166 bytes**.

The first `Enemy.cpp` canonical replay failed closed because the six new member
declarations advanced the compiler's internal resolver `$L...` labels by
exactly 14. Object inspection found the same five DIR32 offsets and target VAs
in each resolver; only the generated names changed. After refreshing those ten
manifest symbol names, two independent cold builds replay all **20 existing
Enemy units / 511 bytes** exactly.

## Completed packet: ANM resource, text and surface core

The manager setup corridor is now source-present from `0x00445900` through the
end of `CaptureToSurface @ 0x004485CA`. Its texture/resource half covers direct
and sub-rectangle texture loads, texture creation from files and ANM payloads,
empty-texture creation, synchronous/preloaded ANM parsing, external texture
data, postload relocation, texture upload, preload service, resource release,
and sprite materialization. The target manager owns 33 loaded-resource views at
`+0x3AD06C`; each complete view is `0x130` bytes and retains the file buffer,
entry pointers, 32 script pointers and 32 sprite pointers.

The text half recovers the shared rasterizer and left/right/centered formatters.
VM byte fields `+0x3A0/+0x3A1` are glyph width and height, while flag bit one at
`+0x360` selects the alternate text helper. The formatter family uses guarded
stack buffers and therefore requires the target's `/GS` profile. The same fact
also closes three previously authored ASCII-manager formatters.

The surface half covers load/preload/release, two surface-to-backbuffer copies,
texture capture, rectangle copy and surface capture. Manager storage consists
of two 32-pointer surface arrays, 32 retained file buffers and sizes, and 32
`0x1C` image-info records at `+0x3AD4E0..+0x3ADA5F`. Direct D3D9 vtable slots and
target branches establish render-target acquisition, offscreen backup creation,
filter selection and cleanup. The apparent null write in the preload service
and the redundant second `surfaceData[index] = NULL` in release are retained
because both occur in the target.

Canonical linked replay adds five resource units (540 bytes) and eight `/GS`
text/surface units (1,032 bytes). The latter include three ASCII formatters,
four ANM text functions and `ReleaseSurface`. Two focused cold replays pass each
new group independently; two full-source cold replays pass all **60
`AnmManager.cpp` units / 10,175 bytes** across ten artifacts. Repository totals
are **1,309 candidates, 247 source mappings and 110 exact functions / 12,166
bytes**. Four Ghidra-missed authored candidates were added at `0x00447080`,
`0x00447C80`, `0x00447EC0` and `0x00448360`.

The tracked core corridor contains 31/31 source-present functions and 10,628 /
10,628 source-present bytes. Across the full `AnmManager` module, 123/125
candidates are source-present (98.4% by function count); only the 10-byte
`0x004452E0` and 7-byte `0x004458A0` leaf bodies remain unmapped. Small retained
helpers and the `0x004485D0-0x00448850` wrapper/id-propagation family remain in
the Web leaf queue. The next core frontier is
`EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770`, the 14,416-byte ECL
dispatcher.

## Completed packet: ANM manager update and removal core

The former `th10-ghidra` discovery refusal was traced to the intentional
implementation-binding guard. Commit `f5a0c4f` added pinned Capstone identity to
the registered `config/tools.lock.toml`, while the ignored private Factory
configuration retained the previous aggregate hash. Per-file history proved
that this was the sole registered-file delta. The private binding was refreshed
to aggregate `406ab2a25af49916607390d2d2eaad09d31fe3df6291b93b07919d162926679d`;
Factory validator, live MCP discovery and live MCP `check {}` then passed against
the canonical target with `provider_transport=factory-native-command`.

Maintained source now covers the four central manager-update bodies.
`UpdatePrimaryVms @ 0x00448860` rebuilds ordinary layers 0–18 while updating the
primary manager list. `UpdateSecondaryVms @ 0x00448900` resets the shared
processed count and rebuilds reserved layer 19. Both save the next manager node
before callbacks, remove marked or completed VMs, run the optional position
callback before `ExecuteScript`, append survivors through `nextInDrawLayer`, and
count each visited VM. `DrawLayer @ 0x00448980` skips removal-marked VMs, invokes
the optional draw callback and reaches the common draw dispatcher.

`RemoveVm @ 0x00448BB0` is the shared 268-byte lifecycle owner. It repairs
either manager-list head/tail pair, unlinks both the manager and parent/child
nodes, and distinguishes the 4096-entry inline pool from heap fallbacks. Pool
VMs clear their occupancy byte, release generated storage and run `Initialize`
in place; heap VMs are destroyed and deleted. Direct-member expressions for the
four head/tail transfers reproduce the target's VM-relative encodings without
artificial constraints.

All four functions are canonical exact under pinned VC7.1 SP1 build6030 `/GL`
in the complete ANM executor context: primary update 149 bytes, secondary update
123, layer draw 67 and removal 268. Two independent final cold replays cover
**607/607 bytes** and all eleven declared linkage fields with zero differences.
The full `AnmManager.cpp` canonical set is now **47 functions / 8,603 bytes**;
repository tracking contains **1,305** candidates, **217** source mappings and
**97 exact functions / 10,594 bytes**.

The 206-byte manager destructor at `0x00446220` is also source-present. It drains
both lists through `RemoveVm`; target SEH states establish the subsequent member
order as twenty sentinel VM destructors, the embedded primary VM whose
generated-vertex pointer is at `+0x3AD488`, then 4096 pool VM destructors. Its current 197-byte linked candidate remains
non-exact because the target retains a different SEH/vector-destructor context.

Small update/draw wrappers and the Ghidra-missed VM scalar deleting helper are
left for the Web leaf-function queue. The next core frontier is the 2,315-byte
manager constructor at `0x00445900`, followed by the 472-byte initialization
owner at `0x004462F0` and other large bodies through the manager setup corridor.

## Completed packet: ANM child-VM and manager lifecycle

The ANM executor's former child-VM declarations are now connected to maintained
source. Twelve creator bodies cover the four manager-order variants and their
screen/world-positioned forms. Each obtains a VM from the manager's 4096-entry
inline pool or heap fallback, initializes the selected script, applies a render
layer or position, and inserts the VM at the front or back of the primary or
secondary manager list. World-positioned forms apply the TH10-local playfield
origin `(+224,+16)`; no adjacent-game offset was transferred.

The manager view now exposes the target-proved pool at `+0x68`, occupancy bytes
at `+0x3AC068`, list heads/tails at `+0x72DAD4..+0x72DAE0`, twenty draw-layer
sentinel VMs at `+0x72DAE4`, and the next VM id at `+0x732454`. Maintained bodies
cover allocation, all four insertion variants, two-list id lookup, stale-id
clearing, interrupt and immediate-interrupt propagation, tree deletion,
screen/world position propagation, position lookup, and resource-wide deletion.
The VM's `+0x04` manager node and `+0x10` parent/child layer node are distinct;
root operations walk only the latter child chain.

Three script-transition bodies close the resource side.
`SetAndExecuteScriptIndex @ 0x0043E7E0` binds and executes frame zero without a
full VM initializer, while `InitializeAndExecuteScriptIndex @ 0x0043E710`
initializes a newly allocated VM first. `SetAndExecuteScriptIdx @ 0x0043E8B0`
restarts an existing VM, rebuilding color, timer and interpolation state and
undoing a pending horizontal flip by
negating scale X. The restart body has the exact 287-byte target extent and only
22 ordinary differing bytes, all caused by an ESI/EDI register exchange; it is
retained as a strong near match without exact credit.

Pinned VC7.1 `/GL` in the complete executor context canonically reproduces three
new units: `SetAndExecuteScriptIndex` (194 bytes), primary-tail insertion (118),
and secondary-tail insertion (118). Two final independent cold replays rebuild
all **43 AnmManager.cpp units / 7,996 bytes** across eight artifacts with zero
differences. Current repository tracking contains **1,305** candidates, **240**
authored functions, **212** source mappings, and **93 canonical exact functions /
9,987 bytes**. The authored source-present exact backlog is **115**.

The remaining lifecycle bodies are source-present and non-exact. Representative
candidate/target extents are 197/203 for initialize-and-bind, 201/217 for
`InitializeVm`, 153/161 for allocation, 65/73 for two-list lookup, 19/21 for
stale-id lookup, and 74/73, 93/95, 116/118 for the three creator shapes. The two
front insertion variants are 94/112 because the current complete-program
context eliminates a generic list branch that the target retained. These gaps
are compiler/caller-context diagnostics and have no acceptance authority.

`scripts/progress.py` now keys cross-ledger origin lookups by parsed integer
addresses. This fixes a real report failure when equally valid uppercase and
lowercase hex spellings occur in different CSV files, while tracking validation
already treats addresses numerically.

At that checkpoint, a later provider discovery attempt refused operation because
the native Ghidra implementation no longer matched its operator binding. The
current manager-update packet reviewed the registered-file delta, refreshed the
private binding and re-attested the provider before making new Ghidra reads.

The 268-byte VM removal frontier is completed by the current packet. Remaining
id/child wrappers stay in the leaf queue, while core work proceeds to the large
manager constructor and initialization owners.

## Completed packet: ANM radial trail and RNG

The pulsing radial-trail family at `0x004452F0-0x00445898` is now
source-present. Its 0x4B0-byte VM-owned payload contains 33 textured vertices,
33 radii, 33 radial velocities, a two-float UV velocity and a trailing dword.
The initializer frees/replaces VM `generatedVertices`, installs callbacks at
VM `+0x398/+0x39C`, seeds a center plus 31 radial vertices, and uses RNG owner
`0x004918B0`. The updater scrolls both U and V with the X velocity, applies the
target's 33-vertex wrap loops, advances radii, rebuilds the ring with the
angle/magnitude helper, clears ring alpha and copies vertex 1 into closing
vertex 32. The draw callback submits the 33-vertex textured fan.

`UpdatePulsingRadialTrail` has the exact 598-byte target extent and matches
524/526 comparable bytes under normal and linked compiler probes; only two
commutative X/Y position-sum displacement bytes differ. In the single-source
LTCG diagnostic the initializer was only 572 bytes because unresolved RNG
methods could not be inlined. New multi-source `/GL` support lets the probe add
`src/RandomMath.cpp`; the initializer then becomes 804 bytes versus target 803,
but remains non-exact because its real caller is the source-absent 9,587-byte
ANM script executor at `0x0043EE30`, which supplies the target's private EDI VM
receiver.

Two trail units are canonical exact in that multi-source context:
`DrawPulsingRadialTrail` (25 bytes and two linked fields) and
`AnmFloat3View::FromAngleMagnitude` (30 relocation-free bytes). The recovered
RNG recurrence independently produces four normal-COFF exact units:
`GetRandomU16` (39), `GetRandomU32` (78), `GetRandomF32` (109), and
`GetRandomF32Signed` (115), totaling 341 bytes and five constant relocations.
Boundary review also added Ghidra-missed, unreferenced bodies at `0x004452E0`,
`0x004458A0`, `0x0044B9B0`, and `0x0044BA30`; only `0x0044B9B0` has sufficient
source/compiler evidence for authored exact promotion.

Current tracking contains **1,298** candidates, **190** authored functions,
**162** source mappings, and **79 canonical exact functions / 7,896 bytes**.
The authored source backlog is **79**. The full `src/AnmManager.cpp` canonical
set is **29 functions / 5,905 bytes** across six artifact contexts; its cold
replay passes with the trail artifact compiling both ANM and RNG inputs.

The next high-value ANM frontier is the script executor at `0x0043EE30`, which
owns the private trail-initializer call context, or the following generated
effect families beginning at `0x00445900`.

## Completed packet: ANM generated geometry

The full `0x00444B10-0x004451B2` generated-geometry corridor is now
source-present as eight distinct functions. Horizontal and vertical strip
initializers fill alternating UV rows or columns with the VM primary color and
unit RHW; the adjacent color helper updates a caller-supplied 0x1C-stride
range. Dispatcher mode 9 reaches the visible/alpha-gated textured strip
submitter with `generatedVertices` and twice `generatedVertexCount`.

The corridor also contains a batched supplied-quad path, two 0x14-stride
untextured diffuse submitters for triangle strips and fans, and a textured fan
submitter used by the 33-vertex callback at `0x00445880`. The untextured paths
select diffuse-only texture operations, disable Z writes for submission, then
invalidate the renderer's shader/color/blend/Z caches and restore normal
modulate/texture arguments. D3D9 vtable slots and all state/FVF/primitive
values are represented explicitly in the target-bound device view.

Ghidra originally omitted the independently CC-delimited bodies at
`0x00444CB0`, `0x00444DC0`, `0x00444E60`, and `0x00444FA0`; all four are now
included in the function denominator. Ghidra and raw scans find target callers
only for the dispatcher strip path and the 33-vertex textured fan. Names for
the retained no-xref bodies are descriptive or adjacent-supported rather than
original-symbol claims.

Three units are canonical exact after two cold linked-image replays:
`InitializeHorizontalTextureStrip` (201 bytes),
`InitializeVerticalTextureStrip` (201), and `SetGeneratedVertexColor` (34).
They add 436 exact bytes and have no linked fields. `DrawGeneratedVertices`
and `DrawTexturedTriangleFan` reproduce their complete 224/211-byte extents
apart from the same 13-byte texture-bind scheduling block seen in Draw3D.
`QueueSpriteQuad` and both 313-byte untextured submitters reproduce every
instruction and linked field, while the candidate PDB contributions own two
extra trailing `CC` bytes; strict extent policy keeps these five non-exact.

At checkpoint `09e4da8`, tracking contained 1,293 candidates, 154 source
mappings and 73 canonical exact functions / 7,500 bytes. The next frontier at
that checkpoint was the generated radial-trail family now covered above.

## Completed packet: ANM direct 3D

`SetRenderStateForVm3D @ 0x004423E0-0x00442597` is reconstructed from
TH10-local target evidence. It is the direct-3D sibling of the batched 2D
state helper: blend changes select inverse-source-alpha for mode zero and one
for modes one and two; the selected primary/secondary VM color is optionally
multiplied by the renderer color and published as D3D9 texture factor; and the
same point/linear sampler cache is maintained. Every incompatible state change
flushes pending 2D sprites, and the helper increments the per-frame state count.

`Draw3D @ 0x00444760-0x00444B00` rejects hidden, disabled and zero-alpha VMs,
flushes the 2D batch, updates scale and X/Y/Z rotations, and applies anchor
translation from `spriteOffset + preservedPosition + position`. It installs
the world transform, binds the sprite texture, optionally refreshes the
texture transform from UV scroll, and submits the renderer's four 0x14-byte
vertices as a two-primitive triangle strip. TH10 binds the texture before
refreshing the sprite/UV cache, repeats the `uvScrollX != 0` test in the
texture-transform condition, and writes world Z first without then with
`spriteOffset.z`; source retains those target-observed details rather than
normalizing them from adjacent code.

Pinned VC7.1 `/GL` reproduces both complete target contribution extents: 440
bytes for the state helper and 929 for the draw owner. The state helper matches
396/408 comparable non-linkage bytes and Draw3D matches 816/829. Each remaining
gap is one equal-length instruction-scheduling region around a D3D9 COM call:
12 bytes for texture-factor state and 13 bytes for texture binding. Natural
assignment-expression, explicit-local and inline member-wrapper probes do not
change those regions. They remain source-present and non-exact; no artificial
dependency or assembly constraint was added.

Current tracking contains **1,289** candidates, **174** authored functions,
**146** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **72**. The full `src/AnmManager.cpp` canonical
set remains **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is the generated-vertex initialization/submission
corridor at `0x00444B10`, `0x00444BE0`, `0x00444CE0`, and `0x004450E0`.

## Completed packet: ANM projected photo blend

`DrawMode7 @ 0x004445C0-0x00444751` is reconstructed from TH10-local target
evidence. After `Project3DQuad`, it transforms the four renderer-local
0x14-byte source vertices beginning at renderer `+0x3ADA78` through cached
world matrix `+0x3AD0F0`. Each resulting XYZ is measured independently from
camera global `0x00491D7C`.

Within the near distance at `0x00491E78`, a vertex keeps the VM's selected
primary or secondary color. Across the near/far interval it interpolates B/G/R
toward float channels at `0x00491E80/84/88` and preserves VM alpha. At or
beyond the far endpoint, it takes dword color `0x00491E90` and then restores VM
alpha. Unlike mode 6, this path neither applies renderer mix color nor rejects
a fully blended quad. It submits through `DrawInner(vm, 2)` and restores all
four shared RHW fields to one.

The dispatcher pushes VM then renderer for mode 7; the callee owns both stack
arguments and returns with `RET 8`. Its source view now names the four source
vertices and extends the photo-blend state through the far color, with compile-
time offset and size checks. TH095 supplied a source-family hypothesis only;
all accepted offsets, arithmetic and ABI facts are TH10-local.

Pinned VC7.1 `/GL` in the real two-VM caller context produces a 404-byte PDB
contribution versus the 402-byte target. Refining the initial 479-byte adjacent-
shaped candidate to TH10 float-channel arithmetic, pointer induction and a
12-byte live color/range/conversion aggregate closes the stack frame exactly at
`0x4C` and aligns the full transform/distance/control-flow core. The remaining
two-byte extent difference comes from the candidate retaining the shared-color
base with positive channel offsets and an adjusted loop comparison, while the
target retains an alpha pointer and offsets `-3..0`. It remains source-present
and non-exact without artificial codegen constraints.

Current tracking contains **1,289** candidates, **172** authored functions,
**144** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **70**. The full `src/AnmManager.cpp` canonical
set remains **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is `Draw3D @ 0x00444760`, followed by the generated-
vertex initialization/submission corridor at `0x00444B10-0x004450E0`.

## Completed packet: ANM mode dispatch

`DrawMode6 @ 0x00443FB0-0x00444232` is now reconstructed from TH10-local
evidence. It first uses the camera-facing projector, computes the distance from
`position + preservedPosition + spriteOffset` to background-camera global
`0x00491D7C`, and optionally applies the renderer color multiplier. Beyond the
near distance at `0x00491E78`, it interpolates color toward float B/G/R globals
at `0x00491E80/84/88` and alpha toward zero across the configured near/far
interval. A completed fade returns `-1`; otherwise all four shared vertices
receive the computed color and `DrawInner(vm, 2)` submits the quad.

The total `Draw @ 0x004451C0` owner is also source-present. Direct target and
PDB review correct its physical extent from Ghidra's 173-byte code-only body to
216 bytes through `0x00445297`: code ends at `0x0044526C`, a three-byte
alignment instruction follows, and `0x00445270-0x00445297` is the owned
ten-entry absolute jump table. It checks visible, draw-enabled and primary-alpha
state separately, extracts render mode from VM flag bits 22-25, and dispatches
modes 0-9 across the recovered draw family. Mode 9 exposes VM `+0x30C` as a
generated-vertex count and passes twice that count with the VM vertex pointer.

A real target caller at `0x00409230` draws two adjacent 0x3AC-byte VMs through
the global renderer when owner flag bit zero is set. Using this callback as the
pinned VC7.1 `/GL` entry reproduces both its complete 50-byte contribution and
the private 60-byte `DrawProjected3DQuad` contribution exactly, including all
ten declared linked fields. `DrawMode6` remains a close natural-source
candidate at 645 bytes versus target 643: its EDI/ESI private ABI is correct,
but the compiler candidate uses a `0x34` stack frame where the target uses
`0x20`. `Draw` remains non-exact at 208 versus the 216-byte physical owner
until its unrecovered mode-7, direct-3D, and generated-vertex callees are
restored in the same LTCG context.

This packet also fixes the linked-image Oracle for code contributions with
inline absolute tables. PE base relocations now override only overlapping
Capstone-derived false relative fields, while every other overlap still fails
closed. This lets switch-heavy ANM and ECL contributions be probed without
misclassifying jump-table data as code.

Current tracking contains **1,289** candidates, **171** authored functions,
**143** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **69**. The full current `src/AnmManager.cpp`
exact set is **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is mode 7 at `0x004445C0`, followed by the direct-3D and
generated-vertex owners reached by dispatcher modes 8 and 9.

## Completed packet: ANM rotated draw modes

Direct target boundary review splits the former `0x004436C0-0x00443B5E`
candidate at the `CC` byte at `0x0044390F`. The two resulting 591-byte bodies
are `0x004436C0-0x0044390E` and `0x00443910-0x00443B5E`. Ghidra xrefs and raw
control flow prove that `Draw @ 0x004451C0` render-mode cases 1 and 3 tail-jump
to the two entries at `0x00445203` and `0x00445263`. The dispatcher extracts
the four-bit mode from VM flags 22-25 after checking visible, draw-enabled and
primary-alpha state.

Both bodies are instruction-identical apart from their two address-dependent
REL32 displacements. Each handles zero Z rotation through
`DrawNoRotationNoRound`, uses one x87 `FSINCOS`, accumulates all three VM
position vectors, scales sprite width/height, applies the anchor modes in flag
bits 18-21, writes four rotated XY pairs plus common accumulated Z, and enters
`DrawInner(vm, 0)`. Maintained source therefore supplies `Draw2D` for mode 1
and a distinct `Draw2DRotatedOrAxisAligned` method for mode 3. The latter name
is corroborated by TH08's same mode table and remains a descriptive hypothesis;
all behavior, boundaries, ABIs and duplicate-body facts come from TH10.

Pinned VC7.1 SP1 build6030 `/GL` produces complete 591-byte PDB contributions
for both methods. The initial form matched 497/507 comparable bytes; its only
ten non-linkage differences were one equal-length scheduling choice at offsets
`+0x54..+0x5E`, where the target completes the horizontal anchor mask before
loading the first Y-offset operand. A later exact-source pass recovered the
original expression family: keeping each three-term coordinate sum as one
left-associated expression reproduces all 507 comparable bytes and all 21
fields in both owners. The canonical promotions are recorded in the current
exact Draw2D packet.

Current tracking contains **1,289** candidates, **166** authored functions,
**140** source mappings, and **68 canonical exact functions / 6,954 bytes**.
The authored source backlog is **68**. The full current `src/AnmManager.cpp`
exact set is **22 functions / 5,304 bytes** across four artifact contexts.

The photo-color owner and real dispatcher context identified here are completed
by the current packet above. Mode 7 and the direct-3D/generated-vertex paths
remain the open continuation.

## Completed packet: ANM projected 3D quad

`Project3DQuad @ 0x00444240-0x00444571` is recovered from TH10-local target
evidence. It establishes VM matrices at `+0x23C/+0x27C`, flag bit 14 as the
matrix-recalculation suppressor, and the renderer's cached world matrix at
`+0x3AD0F0`. Dirty transforms copy the base matrix, apply scale to `_11/_22`,
then compose nonzero X/Y/Z rotations through D3DX. Anchor bits 18-21 select
the local `-256/-128/0/128/256` corner coordinates before four projections
through the active viewport owner.

TH10 adds all three VM position vectors to existing world X/Y translation but
replaces world Z with their sum. This differs from the first adjacent-game
source-shape hypothesis and accounts for the only four bytes in the initial
822-byte candidate beyond the 818-byte target. With the corrected assignment,
the canonical `/GL` unit reproduces the complete PDB contribution and all 21
link fields exactly.

`DrawProjected3DQuad @ 0x00444580-0x004445BB` is source-present. It projects,
calls `DrawInner(vm, 0)`, restores the four shared RHW values to one, and
returns the draw result. Its standalone compiler entry emitted a conventional
72-byte wrapper. The subsequently recovered real two-VM draw callback supplies
the private ESI/EDI context and reproduces the complete 60-byte target form;
the current packet promotes it to canonical exact.

## Completed packet: ANM camera-facing projection

The camera-facing placement owner at `0x00443B60-0x00443F76` is now recovered
from TH10-local target evidence. It reads VM Z rotation at `+0x2C`, accumulates
`position + preservedPosition + spriteOffset`, builds a translated identity
world matrix, and calls the imported `D3DXVec3Project` thunk at `0x0045218E`
twice through the active viewport owner at `0x00491FAC`. The owner layout is
camera-right `+0x30`, view matrix `+0x4C`, projection matrix `+0x8C`, and D3D9
viewport `+0xCC`; all four offsets have compile-time checks.

The first projection supplies a zero vector and rejects projected Z outside
`[0,1]`. The second projects camera-right, and the screen-space distance between
the results supplies half-scale for sprite width/height. The same VM flag bits
18-21 select horizontal and vertical anchor modes before the two-dimensional
rotation is applied. TH10 writes `projectedPosition.z` to all four shared
vertices; this target-local behavior differs from the adjacent TH095 source
shape and was retained from TH10 evidence.

Pinned VC7.1 SP1 build6030 `/GL` in the regular-text entry context reproduces
the complete 1,047-byte PDB contribution and all 25 viewport, import, constant,
and shared-quad fields exactly. The 33-byte `DrawCameraFacingQuad` wrapper is
also exact with both REL32 calls replayed to `ProjectCameraFacingQuad` and
`DrawInner`. The compiler's local symbol ordering affects LTCG stack coloring:
renaming the two 12-byte projection temporaries alone produced eight stack-
displacement differences, while the retained source form reproduces all 1,047
bytes. This is compiler evidence, not an inferred target layout.

Direct target review also recovered a Ghidra-missed 50-byte
`TranslateRotation` body at `0x00443680-0x004436B1`. Its 50 instruction bytes
are raw-equal to current source, but the candidate PDB contribution includes
two trailing `CC` bytes and is therefore conservatively source-present/non-
exact under the current complete-contribution Oracle. The following rotated
draw range was subsequently split and reconstructed in the current packet.

## Completed packet: ANM shared draw core

The common renderer path at `0x00442670` is now reconstructed together with
its 25-byte color mixer at `0x004423C0`, 201-byte render-state owner at
`0x004425A0`, and axis-aligned callers at `0x00443080/0x00443290`. Direct TH10
evidence establishes screen shake at renderer `+0x5C/+0x60`, the texture/blend/
shader/filter cache at `+0x3ADA64..+0x3ADA6E`, mix color at `+0x732458`, VM UV
scroll at `+0x54/+0x58`, primary/secondary colors at `+0x2FC/+0x300`, and
sprite texture/UV/dimensions at `+0x04/+0x20..+0x34`. Every named layout field
has a compile-time offset check or belongs to an already size-checked view.

`DrawInner` adds screen shake to four shared vertices, optionally performs the
D3D9 half-pixel `FRNDINT - 0.5f` sequence, assigns sprite UVs, computes four
bounds, culls against the unsigned active viewport at global `0x00491FAC`,
flushes on texture/shader changes, applies primary or secondary VM color and
optional per-channel renderer mixing, updates render state, and expands the
quad into the packed buffer. `SetRenderStateForVm` handles destination blend
and point/linear min/mag filtering through D3D9 vtable slots `+0xE4/+0x114`;
the texture change uses `SetTexture` at `+0x104`.

Both axis-aligned draw owners implement the three horizontal and vertical
anchor modes from VM flag bits 18-21. The rounded path floors centered axes and
enters `DrawInner(vm, 1)`; the no-round path preserves sub-pixel coordinates
and enters with zero. TH10 has one target-local quirk: no-round writes vertex Z
from `spriteOffset.y (+0x350) + preservedPosition.z + position.z`, while the
rounded sibling reads `spriteOffset.z (+0x354)`. Maintained source preserves
the observed difference rather than normalizing it from adjacent games.

Pinned VC7.1 `/GL` in the real regular-text draw entry context reproduces all
five complete PDB contributions exactly: color mix 25 bytes/no fields, state
owner 201 bytes/six fields, `DrawInner` 1,111 bytes/91 fields, rounded axis draw
515 bytes/29 fields, and no-round axis draw 486 bytes/29 fields. Two cold
canonical passes replay all **2,338 bytes and 155 linked fields** with zero
differences. The full current `src/AnmManager.cpp` exact set is **19 functions /
3,406 bytes** across four artifact contexts.

Current tracking contains **1,286** candidates, **161** authored functions,
**133** source mappings, and **65 canonical exact functions / 5,056 bytes**.
The authored source backlog is **64**. This packet adds five source mappings
and five exact functions without adding speculative denominator entries.

The next ANM frontier is the neighboring transformed-quad corridor beginning at
`0x00443B60`; keep its rotation/projection semantics provisional until its own
TH10 matrix, caller, and compiler evidence is complete.

## Completed packet: ANM render buffer core

The actual ANM renderer reached through pointer `0x00491C10` owns a shared
`0x20000`-entry vertex buffer. TH10 directly establishes its `0x1C`-byte packed
vertex size, sprite count `+0x3ADAC8`, buffer base `+0x3ADACC`, end/start cursors
`+0x72DACC/+0x72DAD0`, and flush count `+0x58`. The complete buffer span is
therefore the observed `0x380000` bytes rather than an adjacent-game layout.

Maintained source covers the 29-byte clear owner at `0x00442F30`, 133-byte
flush at `0x00442F50`, and 157-byte quad-to-triangle expansion at `0x00442FE0`.
The clear resets count and cursors; the add path copies corners `0/1/2/1/2/3`,
advances by six vertices, and increments the count; flush configures D3D9
diffuse arguments and FVF, submits two triangle-list primitives per sprite,
advances the submitted range, clears the count, and increments the frame tally.

All three are canonical exact linked-PE units in the real regular-text draw
context: clear and add are complete raw-equal PDB contributions with no linked
fields; flush contains exactly four DIR32 fields, all replayed to D3D9 device
global `0x00491C30`. Two independent cold builds reproduce all **319 bytes**.
The pinned VC7.1 SDK lacks D3D9 headers, so `src/D3d9View.hpp` supplies the
minimal target-used COM prefix with compile-time checks for vtable slots
`+0x10C`, `+0x14C`, and `+0x164`.

Current tracking contains **1,286** candidates, **156** authored functions,
**128** source mappings, and **60 canonical exact functions / 2,718 bytes**.
The authored source backlog remains **64**. The current `AnmManager.cpp` source
replays fourteen exact units across **1,068 bytes** in four artifact contexts.

The next draw-core seam is the paired axis-aligned placement owners at
`0x00443080` and `0x00443290`. Their existing TH10 evidence already establishes
VM position/offset/scale fields and the horizontal/vertical anchor bits; their
final call reaches the common render-state/draw owner at `0x00442670`.

## Completed packet: ASCII text pipeline and ANM-owner correction

TH10-local queue append, formatting, glyph lookup, viewport, callback, and draw
evidence corrects the preceding checkpoint's owner hypothesis. The `0x89AC`-
byte polymorphic object at global `0x004776E0` is the ASCII text manager. It
contains 256 regular and 64 GUI records of `0x68` bytes, queue counts at
`+0x896C/+0x8970`, text color/scale/viewport/shadow/spacing state, an ASCII ANM
resource at `+0x8994`, and two embedded `0x3AC` ANM VMs. The true ANM renderer is
a distinct object published through the pointer at `0x00491C10`.

Maintained source now expresses the complete regular and GUI record layouts,
queue reset, both append paths, three variadic formatters, and the two large text
renderers at `0x00401760` and `0x00401A50`. The renderers cover both `0x62`-glyph
banks, `0x44`-byte sprite records, line and space advance, regular-text shadow,
viewport transitions, vertex-buffer flushes, and rounded versus non-rounded ANM
draw calls. Each named field and helper target is directly visible in TH10. The
TH095 ASCII implementation supplied only a source-shape hypothesis.

The retained reset at `0x004014D0`, regular append at `0x00401530`, and GUI
append at `0x004015C0` are canonical exact linked-PE units across **267 bytes**.
They are complete, raw-equal PDB-owned contributions with no linked fields in
the real `AsciiManagerCreate` LTCG context. Together with the four renamed exact
units from `260ac75`, the current source file replays eleven exact units across
**749 bytes**. The three formatters and two renderers remain source-present
non-exact work; no semantic recovery claim has been promoted to exactness.

Current tracking contains **1,285** candidates, **153** authored functions,
**125** source mappings, and **57 canonical exact functions / 2,399 bytes**.
The authored source backlog is **64**. Relative to `260ac75`, this packet adds
eight authored functions, eight source mappings, three denominator candidates,
and three exact functions / 267 bytes while correcting the manager owner across
source, ledgers, exact manifests, and documentation.

The following renderer-buffer packet takes up the real owner at `0x00491C10`
and closes its clear, flush, and quad-expansion seam before the neighboring draw
helpers at `0x00443080` and `0x00443290`.

## Completed packet: ANM VM lifecycle

Direct TH10 constructor, destructor and reset evidence establishes a `0x3AC`-
byte ANM VM. Its constructor runs nine `0x14` timer-like member constructors,
clears exactly `0xEB` dwords, and marks the active sprite index at `+0x384` as
invalid. Its destructor owns and clears the optional generated-vertex pointer at
`+0x358`. The reusable reset at `0x00401DE0` preserves `+0x20` and
`+0x340..+0x348`, clears the VM, restores list sentinels, scale, matrix, timer,
flags and seven observed state fields, and writes `-1` at `+0x2FC`.

Maintained `AnmVmView`, `AnmVmTimerView`, and `AnmMatrixView` source expresses
these facts with compile-time size and offset checks. Denominator review also
adds two CC-delimited helpers missed by Ghidra: the 55-byte matrix identity body
at `0x00401F00` and the 15-byte timer initializer at `0x00401F40`. Their logic
is independently visible inline in the reusable reset.

Two cold canonical passes reproduce four complete functions across **192
bytes**: the 34-byte destructor and 88-byte constructor as normal COFF, and the
55-byte matrix and 15-byte timer helpers as raw-equal linked-PE PDB
contributions with no linked fields. The 273-byte reset compiles to the same
extent under normal COFF but remains non-exact because the bounded harnesses do
not recover the target's private EDX receiver and register/save ordering. In the
new explicit manager-factory link context it is 271 bytes.
The emitted seven-byte timer constructor shape also occurs at two target
locations already associated with other or unresolved owners; it receives no
ANM mapping or exactness credit.

Current tracking contains **1,279** candidates, **137** authored functions,
**109** source mappings, and **50 canonical exact functions / 1,842 bytes**.
The authored source backlog is **55**. Relative to the preceding checkpoint,
this packet adds five authored functions / 465 bytes, five source mappings, two
denominator candidates, and four exact functions / 192 bytes.

## Completed packet: ECL host hierarchy

Direct TH10 evidence closes the previously unresolved class boundary. The
six-slot vtable at `0x0046D0D8` contains neutral dispatch/int/int-pointer/float/
float-pointer methods followed by scalar deleting destructor `0x0040C7B0`.
The derived table at `0x0046D0C0` replaces those five methods and ends in the
Enemy deleting destructor at `0x0040CC50`. Construction at `0x0040C710` and
`0x0040D830`, destruction at `0x0040DAE0`, and the inline base tail through
`0x0040DC5F` independently show a `0x103C` polymorphic ECL host followed by the
Enemy runtime tail. The manager's indirect call through slot `+0x14` with flag
one is virtual deletion.

Maintained source now expresses that evidence as an actual
`EnemyEclHostBaseView` base and `EnemyFullObjectView` derived class, including
real constructors/destructors, `new`, virtual `delete`, and a base-typed operand
resolver in the embedded context. Compile-time layout assertions preserve base
size `0x103C`, full size `0x2518`, active context `+0x004`, embedded context
`+0x008`, allocation list `+0x1034`, and runtime tail `+0x103C`.

Five neutral virtuals are canonical exact normal-COFF units across 27 bytes.
The 21-byte base constructor and 47-byte base destructor are canonical exact
linked-PE units; their complete PDB-owned extents declare the base-vtable field,
the constructor's one DIR32 field, and the destructor's two `free` REL32 fields.
Two independent cold builds replay all seven new units across **95 bytes**.
All 20 Enemy exact units replay together across **511 bytes** after refreshing
only VC7 compiler-local resolver label names. Base reset and the large derived
constructor/destructor remain non-exact.

Repository exact coverage is now **46 functions / 1,650 bytes**. This bounded
evidence establishes source-level class shape and the declared function bodies;
it does not establish production TU partition or whole-product link closure.

## Completed packet: ECL core lifecycle and resource base

Direct TH10 review establishes a reusable ECL host around the full object's
embedded `0x1018` context. The two callback-subroutine transition paths call a
45-byte allocation-list release at `0x0040C6E0` and then the 71-byte execution
reset at `0x0040C730`. Maintained `EnemyRuntimeUpdate` now expresses both calls.
The release helper is canonical exact; reset is source-present and remains
non-exact at 69 linked bytes versus 71 target bytes.

The neighboring ECL resource base is now source-complete for its reviewed
lifecycle seam. Base vtable slot one is a five-byte default `LoadPackage`
returning zero; the five-byte indexed getter returns one of 32 script pointers
from `+0x0C`; the 34-byte constructor clears the complete `0x1098` object and
installs vtable `0x0046D0F0`; and the 36-byte destructor restores that vtable,
frees lookup owner `+0x8C`, and clears it. The embedded-context constructor
independently clears the two storage cursors at context `+0x1008/+0x100C`.

Two cold linked builds replay all six newly exact functions across **140
bytes**. The three nonzero-field units declare both release calls, the base
vtable fields, and the direct free call exhaustively; omitted-field negative
manifests are rejected. Repository exact coverage is now **39 functions / 1,555
bytes**. Production TU partition and linked-image ownership remain unknown.

## Completed packet: dual-codegen exact-backlog ranking

`scripts/rank-exact-backlog.py` now turns the two existing batch probes into one
fresh selection tool. It obtains the current authored backlog once, runs both
normal COFF and LTCG diagnostics, requires both reports to match every current
address/name/source/size key exactly, and rejects a run if the target, ledgers,
tools, or maintained source tree changes between the two probes. Its explicit
distance combines relative complete-extent size delta with the mismatch ratio
over non-link-field bytes. Profile recommendation and `ltcg_gain` remain triage
signals with `acceptance_authority=none`.

A full target-bound run covers all **53** current authored backlog functions.
Both lanes report **53 mismatches**, zero structural-exact candidates, and zero
unresolved keys. The leading bounded experiments are:

1. `Lzss::AddString`, LTCG: candidate/target **512/516 bytes**, **51/408**
   comparable mismatches, 27 linked fields, distance **0.133**;
2. `CPbgFile::Write`, normal COFF: **67/63**, **10/59**, one relocation,
   distance **0.233**;
3. `EnemyManagerCreate`, LTCG: **94/95**, **22/75**, five linked fields,
   distance **0.304**.

The ranking does not make any function exact or determine its production
profile/physical owner. It gives later sessions a reproducible reason to choose
a source/codegen experiment and preserves both lanes' raw metrics in JSON.

## Completed packet: final structural-exact LTCG promotions

The final seven unreviewed fields from the original linked diagnostic are now
bound to TH10-local targets. The Enemy link uses `g_EnemyManager` at
`0x00477704`, derived/base ECL-resource vtables at `0x0046D0B4` and
`0x0046D0F0`, and the already reviewed `free` entry at `0x00452422`. The
PbgFile link uses the reviewed `strchr` entry at `0x00452730`, complete
45-byte `strrchr` entry at `0x00452930`, and the `GetModuleFileNameA` IAT slot
at `0x004660DC`.

Two independent cold linked builds reproduce:

- `EnemyManagerView::EnemyManagerView`: **29/29 bytes**, one DIR32 field;
- `EnemyEclResourceView::EnemyEclResourceView`: **34/34 bytes**, one DIR32;
- `EnemyEclResourceView::~EnemyEclResourceView`: **36/36 bytes**, one DIR32
  and one REL32;
- `CPbgFile::GetFullFilePath`: **121/121 bytes**, one DIR32 and two REL32.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared fields. Replaying all seven fields leaves zero differences
across all **220 bytes**. Negative manifests omitting the Enemy global DIR32 or
the PbgFile `strrchr` REL32 are rejected. The `strrchr` boundary is independently
reviewed and classified library/CRT; `strchr` keeps origin/boundary state open
because its optimized body shares a backward return tail outside the current
Ghidra candidate. A fresh Enemy/PbgFile backlog probe reports 18 mismatches and
zero structural-exact candidates. All 13 structural candidates / 715 bytes from
the original full LTCG diagnostic have now been promoted through canonical
replay; none remains a diagnostic-only exact candidate.

## Completed packet: canonical Lzss linked fields

All 24 linked fields across the four structural-exact Lzss candidates resolve
to the already target-proven `g_LzssTree` owner at `0x00477858`. Their only
addends are `+0` (`parent`), `+4` (`left`), `+8` (`right`), and `+0x18008`
(`tree[0x2000].right`). The last address is four bytes inside the final
0x0C-byte node of the independently bounded `0x2001`-node / `0x1800C`-byte
array; it is not promoted as a separate global.

Two independent cold linked builds reproduce:

- `Lzss::InitTree`: **36/36 bytes**, four DIR32 fields;
- `Lzss::ContractNode`: **79/79 bytes**, eight DIR32 fields;
- `Lzss::ReplaceNode`: **117/117 bytes**, nine DIR32 fields;
- `Lzss::FindNextNode`: **49/49 bytes**, three DIR32 fields.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared field offsets, types and widths. Replaying the reviewed
tree base/addends leaves zero differences across all **281 bytes**. Together
with the normal-COFF exact `InitEncoderState`, this seam has five exact helpers
/ 323 bytes in two explicitly separate artifact contexts. The result does not
establish a production source/data TU, physical compiler owner, or product
closure. A fresh Lzss backlog probe after the promotions reports only three
mismatches and no remaining structural-exact candidate for this source.

## Completed packet: canonical PbgArchive linked fields

The remaining three structural-exact PbgArchive helpers each have one external
REL32 field. Direct hash-attested target review closes `0x00452706-0x00452717`
as an 18-byte CRT `malloc` wrapper with 94 direct call sites, and
`0x0046054E-0x004605B6` as a 105-byte CRT `_stricmp` wrapper with 11 direct
call sites. Both boundaries end at RET and meet either CC padding or the next
independent CRT function. Their behavior and surrounding CRT owners justify
library/CRT exclusion; the exact original CRT library members remain unknown.

Two independent cold canonical linked builds reproduce all three functions:

- `PbgArchive::GetEntryDecompressedSize`: **51/51 bytes**, one `__stricmp`
  REL32 at `+0x15` replayed to `0x0046054E`;
- `PbgArchive::FindEntry`: **53/53 bytes**, one `__stricmp` REL32 at `+0x18`
  replayed to `0x0046054E`;
- `PbgArchive::CopyFileName`: **47/47 bytes**, one `_malloc` REL32 at `+0x11`
  replayed to `0x00452706`.

All three complete `source.ltcg.obj` PDB contributions decode without gaps and
contain exactly their declared fields. The 151 replayed bytes have zero
differences. Together with the two zero-field pointer helpers, this shared
PbgArchive `/GL` context now has five linked-PE exact functions / 214 bytes.
The diagnostic anchor image remains non-runnable and receives no product or
physical-owner credit. A fresh Factory Ghidra operation-discovery attempt was
unavailable because the native implementation differed from its operator
binding; it returned no semantic evidence, so this packet relies on direct
target and compiler/linker evidence only. A fresh PbgArchive backlog probe after
the promotions reports only seven mismatches and no remaining structural-exact
candidate for this source.

## Completed packet: canonical LTCG exact replay

The exact-unit graph now supports `artifact_kind="linked-pe"` alongside normal
COFF. Shared `scripts/ltcg_link.py` owns the fixed cold `/GL` compile and link
context. `build-match-unit.py` validates the artifact-specific manifest and
builds one image/map/PDB set per shared context. `compare-linked-function.py`
then rebinds PE/map/PDB identity, selects one unique `source.ltcg.obj` PDB
contribution, requires its full manifest size, verifies the pinned Capstone
5.0.6 component hashes, decodes the entire extent, rejects missing or extra
linked fields, validates declared semantic symbols through map publics or the
generated unique anchor aliases, replays target addresses, and compares every
byte. `replay-exact-units.py` cold-builds and checks both artifact kinds.

Two independent cold canonical builds reproduce
`PbgArchive::SeekPastInt` at `0x00435270` across **12/12 bytes** and
`PbgArchive::SeekPastString` at `0x00435280` across **51/51 bytes**. Their
complete PDB-owned extents decode without gaps, contain zero linked fields, and
are raw-equal to all **63 target bytes**. A separate focused `/OPT:REF` test
emitted a 16-byte ordinary stack-ABI `SeekPastInt`; the declared `/OPT:NOREF`
source context is required to recover the target's 12-byte private-register
form. These two accepted claims bind only the declared bounded LTCG context.
The non-runnable anchor image is not a reconstructed product, and production
TU partition, physical owner, link order, data owners, and whole-build closure
remain unknown.

## Completed packet: linked LTCG backlog diagnostics

The repository now parses MSF 7/PDB DBI section contributions directly and joins them to VC7.1 linker-map publics and PE raw bytes. It fails closed unless the PDB RSDS GUID/age matches the PE CodeView record and map timestamp, image base, public start, and section layout all agree. `verify-toolchain.py --execute` exercises this PE/PDB extent path in addition to the existing compiler, resource, and link smoke.

`scripts/probe-ltcg-backlog.py` cold-compiles one source at a time with fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, then performs an iterative diagnostic link. Missing externals receive distinct non-runnable data anchors, allowing every linked field to retain a unique candidate-symbol identity without manufacturing executable stubs. PE HIGHLOW fields and decoded external control-flow operands are masked only for structural comparison. The harness, every result, and the report itself explicitly carry no acceptance authority.

The complete authored backlog scan covers all **66 functions across nine sources**. It found **13 structural-exact diagnostics / 715 bytes** and 53 mismatches: three Enemy lifecycle bodies, four Lzss tree helpers, five PbgArchive helpers, and `CPbgFile::GetFullFilePath`. The two PbgArchive pointer-advance helpers have no linked fields and reproduce all **63 raw bytes**. The other 11 candidates contain 34 resolved fields requiring explicit symbol/target review before any canonical linked unit can exist. Two independent cold PbgArchive passes reproduced the same result/extent/field signature. At checkpoint `26af714` all 13 remained diagnostics; later canonical packets have promoted the five PbgArchive and four Lzss candidates, leaving four structural candidates under review.

## Completed packet: batch normal-COFF backlog diagnostics

Checkpoint `d59f975` added `scripts/probe-exact-backlog.py`. It cold-compiles each selected source once under fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`, enumerates strict COFF functions, joins conservative source-name hints only when one external symbol matches, and runs every authored source backlog entry through target-bound probe comparison. Unresolved mappings, true size/byte mismatches and structural-exact results are separate states; the entire report has `acceptance_authority=none`.

The complete current run covers all **66 authored, source-present, non-exact functions across nine sources**. All 66 map uniquely to a COFF symbol and all 66 are real mismatches; there are zero unresolved and zero structural-exact results. Thus the fixed normal profile has no additional exact unit waiting for ledger-only promotion. This does not reject alternate normal profiles or LTCG ownership.

Probe relocation reporting now also handles a candidate relocation that crosses a shorter target extent: it retains the object addend, marks target encoding/candidate unavailable with `target_extent_complete=false`, and reports a normal size mismatch instead of aborting the batch. Canonical comparison remains strict. No source behavior, exact ledger row, or whole-build state changed.

## Completed packet: strict COFF extent diagnostics

Checkpoint `96f17c6` added decorated-symbol/extent/relocation enumeration and removed the unsafe probe fallback that could slice a caller-requested target length beyond the candidate COMDAT. Missing candidate bytes are now explicit differences. The corrected diagnostic reports `PbgArchive::GetEntryDecompressedSize` as a true 25-byte object against the 51-byte target, and all **20 canonical units / 700 bytes** still cold-replay exact under the strict extent rule.

## Completed packet: exact-work backlog reporting

Checkpoint `88800b6` added `scripts/report-exact-backlog.py`. It joins and validates the function, origin, source-mapping and exact ledgers, with source/module/state filters and JSON output. The 90 source mappings split into **20 canonical exact**, **66 authored non-exact functions / 26,540 bytes**, and **four origin-review Player callbacks / 38 bytes**. Progress and live status now expose the authored/origin-pending split.

## Completed packet: canonical LZSS state-reset backlog

Checkpoint `621add9` promoted the reviewed `Lzss::InitEncoderState` body at `0x00435FD0-0x00435FF9`. Two cold builds reproduced **42/42 bytes and three DIR32 relocations**: dictionary base `0x0048F868`, plus `g_LzssTree` base `0x00477858` with object addends `+4` and `+0x18010`. Ghidra still misses the standalone helper; raw target code and the encoder's inline reset establish its extent and behavior.

## Completed packet: canonical PbgArchive lifecycle backlog

Checkpoint `4ed34ee` promoted `PbgArchive::PbgArchive`, `PbgArchiveEntry::~PbgArchiveEntry`, and `PbgArchiveEntry::PbgArchiveEntry`. Two cold shared-object passes reproduced **51/51 bytes and one REL32 `free` relocation**. Fresh Ghidra review modeled only the entry destructor; raw boundaries and reviewed vector-constructor references remain the evidence for the two inventory-missed constructors.

## Completed packet: exact replay workflow and ResFile backlog

Checkpoint `9b5e7eb` promoted six `CMemoryPbgFile` methods across **210/210 bytes and four relocations** and added `scripts/replay-exact-units.py`. The tool cold-builds one shared object per source/profile and strictly compares every selected unit. The then-current 16 canonical units used three shared outputs instead of compiling the same three translation units 16 times. Manifest validation rejects malformed extents/relocations, overlapping target ranges and inconsistent output sharing; ledger validation joins canonical units to exact, source, function and origin records and requires both exact-unit sets to agree.

## Completed packet: canonical PbgFile exact backlog

Checkpoint `bd9b2e3` promoted six already reviewed PbgFile methods. Current `src/PbgFile.cpp` compiled from absent output paths twice and replayed exact across **220/220 bytes and nine DIR32 relocations**: destructor 46 bytes/three relocations, `Close` 34/one, `Read` 57/one, `Tell` 25/one, `GetSize` 21/one, and `Seek` 37/one. It also extended diagnostic probe output with target-derived relocation candidates; these remain non-authoritative until a reviewed canonical unit passes.

## Completed previous hard packet: Enemy ECL dispatcher context seam

This packet deliberately stayed on the central 14,416-byte `EnemyRuntimeView::DispatchEclInstruction` owner at `0x0040E770-0x00411FBF` instead of pivoting to easier leaf functions. It bounded opcodes `0x15D-0x164`, the nested difficulty tables, and the adjacent integer argument seam. The observable outcome was structural: recover context ownership/ABI and denominator gaps needed for a natural future dispatcher, without writing a partial `default`-return switch and falsely promoting the dispatcher to source-present.

Direct TH10 table decoding maps the eight selected opcodes to case entries `0x004118BB`, `0x004118E2`, `0x0041198B`, `0x004119FC`, `0x00411A3E`, `0x00411B23`, `0x0041009B`, and `0x004100EA`. `0x15F` uses rank interpolation with target constants `1024.0f` and `1/2048.0f`; `0x162` performs the integer analogue `(high-low)*(rank+0x400)/0x800 + low` with the target signed truncation-toward-zero correction. Difficulty cases reach five-entry tables at `0x00411F98` and `0x00411FAC`. These observations are provisional semantic/target evidence, not exactness credit.

The strongest ownership result is the ECL execution context embedded in full Enemy `+0x008..+0x101F`, exactly `0x1018` bytes, with the active-context pointer at full `+0x004`. Context `+0x004` is the current ECL instruction; its target-observed header has opcode at `+0x004`, variable-operand mask at `+0x008`, and dword operands at `+0x010`. Generic typed-operand helpers use context `+0x1008` as operand-stack state, `+0x100C` in local-storage addressing, and context `+0x1014` as the operand-resolver owner. Constructor machine code stores the full Enemy itself at full `+0x101C` = context `+0x1014`, directly binding the generic context to the already-reviewed Enemy operand vtable. Maintained `src/Enemy.hpp/.cpp` now express this exact-size context/instruction view while preserving all full-object offsets.

`0x00412A00-0x00412A0F` and `0x00412A10-0x00412A5A` are now boundary/ABI-reviewed integer argument wrappers. Both take `EnemyRuntimeView*` in EAX and argument index in ECX; the first forwards to the generic int reader and the second resolves a writable int argument address. Complete target scans find all 30 and 9 direct calls respectively inside the central dispatcher, but whether these are source-written helpers or compiler/LTCG outlines remains unknown, so origin stays `unknown/review` and neither is source-present.

Raw corridor review also found three initial-inventory misses: `0x00412A60-0x00412A6D` (14B), `0x00412A70-0x00412A7D` (14B), and `0x00412A80-0x00412A90` (17B). They are CC-delimited helper-shaped bodies adjacent to the seam, but fresh Ghidra xrefs plus complete direct-REL32 and absolute-pointer scans find no target references. They are therefore added only as new `unknown/review` candidates; authored/source/exact denominators do not increase.

Adjacent-game hypothesis provenance for this packet: clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`; clean committed TH09 HEAD `f1d4c1e332828a0cb8b9308b61b79ce77f86e4d0`; TH095 live tree was dirty (4 paths), so only committed HEAD `dee282c33dbb75ca0545d3115cd421a6e4f6a7bd` was consulted. Adjacent source supplied only ECL operand source-family hypotheses after TH10-local recovery; no adjacent addresses, opcodes, layouts, owners, or exactness were transferred.

Pinned VC7.1 SP1 build6030 compiles the refined Enemy source under fixed normal and `/GL`. A separately compiled clean-HEAD baseline and the refined normal object emit byte-identical 586-byte `EnemyConstruct` COMDAT contents with the same four relocation sites; both fresh target diagnostics are non-exact at `63/657` comparable bytes. The old `64/657` note is superseded. This is structural source feedback only, not constructor or dispatcher exactness.

The first cold canonical replay after adding the header declarations correctly failed on the int lvalue resolver because VC7.1 renumbered compiler-internal `$L...` symbols by exactly +37. Clean-HEAD versus revised COFF symbol review proved every int/float resolver jump-table/case label kept the same section offset, and the canonical relocation offsets, types and target VAs are unchanged. `config/match-units.toml` therefore refreshes only those generated object-symbol names. A second cold deletion/rebuild/replay then returns exact 6/6, 11/11, 80/80 and 80/80 for all four configured units. The failed receipt is retained as recovery evidence; exactness was not assumed across the manifest update.

## Completed hard packet: Enemy ECL operands

The packet reconstructed the four vtable-owned operand methods immediately following the previously reviewed 14,416-byte Enemy ECL dispatcher physical owner. Their boundaries/origin were already reviewed authored-game denominator entries at packet entry; this packet adds maintained source and exactness evidence rather than inflating the authored denominator.

| Address | Maintained source | Target bytes | Final state |
| --- | --- | ---: | --- |
| `0x00411FC0-0x004122F3` | `EnemyFullObjectView::ReadIntOperand` | 820 | source-present, non-exact |
| `0x00412300-0x0041234F` | `EnemyFullObjectView::ResolveIntOperand` | 80 | source-present, canonical exact |
| `0x00412350-0x004126C3` | `EnemyFullObjectView::ReadFloatOperand` | 884 | source-present, non-exact |
| `0x004126D0-0x0041271F` | `EnemyFullObjectView::ResolveFloatOperand` | 80 | source-present, canonical exact |

The reviewed full Enemy vtable at `0x0046D0C0` owns these entries in slots 1-4. Raw target review closes each `RET 4`, adjacent padding and each resolver jump table. Ghidra does not model the four vtable entries as standalone functions; its semantic view remains provisional and carries no exactness credit.

## Operand protocol and layout refinement

The two lvalue resolvers and the target spawn copy independently close the 0x20-byte block at full Enemy `+0x1138..+0x1157` / runtime `+0x0FC..+0x11B` as four writable integer ECL variables followed by four writable float ECL variables. Spawn executes `REP MOVSD` for exactly eight dwords from request `+0x20` to full Enemy `+0x1138`, so maintained `EnemyEclVariableView { int integers[4]; float floats[4]; }` is shared by `EnemyRuntimeView` and `EnemySpawnRequestView`. This refines the prior neutral eight-dword transport block without changing the reviewed object extents.

Both readers preserve the target's exact 51-selector domain `-10000..-9950`. TH10-local cases cover RNG U32/unit/signed-unit values, signed random angle scaling, current/offset/base motion values, Player position and angle-to-point helpers, update timer, runtime flag bit `0x10000`, the four int/four float variables, special-enemy coordinates, managed-VM id resolution followed by signed VM word `+0x38A`, rank, difficulty, velocity angle, life and difficulty equality tests.

The integer and float readers deliberately remain separate because TH10 contains type-dependent behavior elsewhere. Direct table entry sends integer selectors `-9996` and `-9976` to target `0x0041219A` while `ECX` still names this Enemy, so both readers expose this Enemy's world-Y and the integer form applies the target float-to-int helper. Only selector `-9962` enters through `0x00412192`, replaces `ECX` with the manager primary special-enemy pointer, and then falls through to `0x0041219A`; selector `-9963` resolves the corresponding special-enemy X directly. Maintained source preserves this target control flow rather than normalizing it from adjacent games.

TH10-local writers establish `0x00474C74` as a `0..4` difficulty index and `0x00474C98` as a signed rank-like value restored from replay and adjusted/clamped to `[-0x400,+0x400]`; maintained source uses descriptive names `g_EnemyDifficulty` and `g_EnemyRank`, not original-identifier claims. Helper evidence independently identifies RNG helpers at `0x0044B9E0/0x0044BB20/0x0044BB90`, Player angle-to-point helper `0x00426660`, float-to-int helper `0x00463B2C`, and VM id resolve-or-clear helper `0x00449450`. Source does not add a target-absent null guard around the VM lookup.

Committed adjacent source was consulted only after TH10 switch tables/storage were independently decoded. TH08 observed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean; adjacent material only corroborated the broad typed int/float/lvalue ECL source family. No selector, address, layout, ABI, owner or exactness fact was transferred. Dirty adjacent untracked content was not used.

## Compiler and exactness feedback

Final maintained `src/Enemy.cpp` compiles with pinned VC7.1 SP1 build6030 under fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.

True final normal `/Gy` extents versus target are:

- `ReadIntOperand`: **800 / 820 bytes**; target-window diagnostic `mismatch`, **115/452 comparable bytes**, 92 object relocations, `acceptance_authority=none`.
- `ResolveIntOperand`: **80 / 80 bytes**.
- `ReadFloatOperand`: **800 / 884 bytes**; target-window diagnostic `mismatch`, **95/540 comparable bytes**, 86 object relocations, `acceptance_authority=none`.
- `ResolveFloatOperand`: **80 / 80 bytes**.

The two 80-byte lvalue resolvers each have an explicit canonical `artifact_kind="coff"` unit. Each complete owned extent carries five DIR32 relocations: one jump-table relocation plus four local case-target relocations. Current-source clean builds and repeated target-bound comparison return `result="exact"`, 80/80 bytes, with all relocation targets matching the target. These are the only new exact promotions in this packet.

The two readers remain source-present/non-exact. Successful `/GL` compilation preserves a possible production LTCG/interprocedural context but gives no standalone exactness credit.

Because all current Enemy exact units share `src/Enemy.cpp`, the final integration gate deletes/rebuilds and compares every configured exact unit after the operand edits:

- `enemy-manager-draw-callback`: exact 6/6, zero relocations;
- `enemy-ecl-dispatch-forwarder`: exact 11/11, one relocation;
- `enemy-ecl-int-lvalue-resolver`: exact 80/80, five relocations;
- `enemy-ecl-float-lvalue-resolver`: exact 80/80, five relocations.

Total repository canonical exact coverage after the Enemy packet was **4 functions / 177 bytes**.

## Ledger and verification planes

Current ledger:

- candidates: **1277**
- origin/boundary pending: **1130**
- authored: **132 / 53,233 bytes**
- excluded: **15**
- source-present: **104 / 27,584 bytes**
- canonical exact: **46 / 1,650 bytes**

Current hierarchy packet delta from `d4af1a5`: **+7 functions / +95 exact bytes**, seven source mappings, and one compiler-generated deleting-destructor denominator entry. The authored source backlog remains **54 functions**. Session exact delta from `444eb1a...` is **+42 functions / +1,473 bytes**.

**Source presence:** seven ECL-host class functions became source-present.

**Exactness:** repository canonical exact coverage is 46 functions / 1,650 bytes. The complete mixed set cold-replays through the configured shared artifacts across five source files. This exact lane does not establish production object ownership or whole-build closure.

**Whole build:** actual final `python3 scripts/build.py` returned **RC2 / explicitly open**. Production compiler flags, TU partition, libraries, resources and link order remain unknown. `build.py --check`, tracking, toolchain execution and public CI pass this honest open state.

**Runtime:** not performed. There is no closed faithful reconstructed Windows-i386 product to execute.

**Factory Truth:** no Factory replay or acceptance submission was performed by this session. Repository canonical exact results and Factory accepted facts remain separate states.

## Scratch and recovery artifacts

The overall recovery/resume entry inventory earlier in this conversation was **365 regular files / 4,218,280 logical bytes / 0 files >64 MiB**. At the clean `2053a39e...` hard-packet boundary, `.analysis/` contained exactly **319 regular files / 3,709,138 logical bytes / 4,464 KiB allocated / 0 files >64 MiB**. Legacy/unknown analysis, other campaigns, Ghidra state, toolchains, Wine state and the private target were preserved.

The hard campaign `.analysis/gpt-web/20260914-enemy-dispatcher-hard/` exits with **28 files / 257,221 bytes**. It retains the manifest, bounded dispatch/table/helper disassemblies, top-level and nested table maps, clean-HEAD baseline source/object evidence, final normal and `/GL` objects, constructor/read-int/read-float diagnostics, local-label shift receipt, exact replay receipts (including the intentional failed pre-refresh receipt), and whole-build logs. Superseded current-packet `Enemy.context.*` scratch and reproducible `build/match/*.obj` outputs were removed only after final receipts existed.

The previous Web checkpoint's final `.analysis/` inventory was **347 regular files / 3,966,359 logical bytes / 4,796 KiB allocated / 0 files >64 MiB**. No legacy/unknown artifact was bulk-deleted, and the ignored private `resources/th10.exe` was not modified, moved, staged or committed.

The linked-diagnostic, canonical replay, PbgArchive, Lzss, final-structural, backlog-ranking, ECL lifecycle, and current host-hierarchy campaigns each retain one compact manifest; full JSON reports, objects, linked images, maps and PDBs are reproducible below ignored `build/`. No copied target was created.

## Completed packet: ANM executor physical order and child control flow

`scripts/report-anm-execute-table.py` is the target-bound ANM counterpart to the
generic ECL table report. It verifies the 9,587-byte code body, the NOP at
`0x004413A3`, the 94-entry absolute table at `0x004413A4`, all 94 enum values
from `-1` through `92`, and optional candidate destinations and physical order.

The maintained executor switch now follows the target-observed physical order
instead of numeric opcode order. The four early child cases are independent and
map 88/91/90/92 to Variant0/Variant2/Variant1/Variant3, correcting the former
90/91 reversal. `STOP_HIDE` falls through `STOP`, NOP and interrupt labels share
the instruction-advance path, and the target-absent executor-side position
callback was removed; manager update owners already call it before execution.
`AnmVmIdView::GetVm` retains an out-of-line target call through the same guarded
MSVC no-inline mechanism used by other target-proven ANM call seams.

The final linked diagnostic has a 9,488-byte candidate pre-table span against
the target's 9,588 bytes including alignment. UNIT_SPEED and child 88/91/90 are
within one byte of their target relative entries, child 92 is within four,
sprite is within eight, and END/STATIC are within one. All 92 physical groups
are in target order except the shared NOP/interrupt advance block, which remains
placed between conditional cases 28 and 29. The candidate is non-exact at
379/8,516 comparable bytes and has no acceptance authority. The full ANM source
gate remains exact for **76 units / 10 artifacts / 15,739 bytes**. Durable
reports and the stable candidate image are below
`.analysis/gpt-5.6-sol/20260916-anm-executor-exact/final/`.

## Completed packet: ANM interpolation value flow

The Float3 evaluator and the executor's post-loop interpolation calls now retain
the target-observed value flow. Float3 modes 7 and 17 assign vector sums instead
of mutating the operands through `operator+=`, and the completed-duration path
keeps separate initial/final returns. In the executor, position branches before
calling `Evaluate` and consumes its returned pointer on either path; rotation
evaluates through a shared local and copies the returned vector; scale also
uses the returned pointer. These are natural source changes tied to target data
flow rather than stack padding or artificial dependencies.

In the selected `ExecuteScript` `/GL` context, the Float3 evaluator moves from
739 candidate bytes and 29/847 comparable matches to **854/887 bytes and
201/847 matches**. The executor's position block now has the target's two-call
branch shape, and its rotation block has the target local/copy shape apart from
the outer register context. The executor candidate remains non-exact at a
**9,472 / 9,588-byte pre-table span**, delta **-116**. Its Float2 evaluator
still uses a different private receiver register and temporary slot. A full
cold replay preserves all **76 ANM exact units / 10 artifacts / 15,739 bytes**.
Reports and the stable candidate image are below
`.analysis/gpt-5.6-sol/20260916-anm-interpolation-shape/final/`.

## Completed packet: ANM Float2 evaluator core

The scale evaluator now preserves the target's value flow rather than using
component-wise compound assignments. Its completed-duration path has distinct
mode-7 initial and ordinary final returns, modes 7 and 17 materialize their
two-component sums, and Hermite materializes four weighted vectors before
summing x and y. In the real maintained executor context this restores the
target private `ESI` receiver and `EDI` output ABI. The selected candidate is
**656 bytes against the 652-byte target**, with **190/612 comparable bytes**
matching; the remaining four-byte contribution difference is not promoted to
exactness. The executor table report is unchanged at a **9,472 / 9,588-byte
pre-table span**. A fresh full-source cold replay closes all **76 ANM exact
units / 10 artifacts / 15,739 bytes**. Durable evidence is below
`.analysis/gpt-5.6-sol/20260916-anm-float2-core/vector-products/`.

The local analysis store was pruned after this packet. Superseded intermediate
variants and duplicate whole-source diagnostic snapshots were removed while
canonical replay receipts, final owner reports, explicitly cited evidence and
the current unresolved candidates were retained. `.analysis/` decreased from
154,965,595 logical bytes to about 43 MiB; the local audit is
`.analysis/gpt-5.6-sol/20260916-artifact-cleanup.txt`.

## Completed packet: ECL VM typed arithmetic core

The generic `EclVmContext::Run @ 0x0044E1A0` now expresses all nine integer
and floating arithmetic opcodes through the target-observed typed stack owner.
Each case pops the right operand and then the left operand directly into typed
locals, mutates the left local in place, and pushes that same local. The two
negation cases likewise reuse their pop local; opcode `0x55` continues to apply
integer `NEG` to the raw float dword. Post-decrement preserves its original
value in the pushed local while writing the decremented value through the
resolved destination.

In the selected `EclVmHost::Run` `/GL` context these natural source forms move
the complete `Run` contribution from **6,648 to 6,916 bytes** against the
7,020-byte physical target owner. The code before its two tables moves from
**6,320 to 6,588 bytes** against the 6,692-byte target span, reducing the gap
from 372 to **104 bytes**. The candidate retains the target's `0x108` stack
frame and exact physical order for all 59 active opcode bodies. Formerly
exceptional comparison and negation blocks now differ by the same two-to-four
byte class as neighboring typed-pop cases.

The remaining systematic difference is code generation rather than missing VM
semantics: the target keeps the instruction cursor in `EBX` and zero in `EBP`,
while the selected candidate exchanges those live ranges and emits `SUB reg,4;
JS` where the target emits `ADD reg,-4; CMP reg,EBP; JL`. The three-byte
post-decrement excess is also isolated to LTCG's internal call convention: the
target passes zero with `PUSH EBP` and preserves the context register between
the read and resolve calls, while the candidate clears and reloads registers.
No artificial dependency was retained to force either allocation.

The final diagnostic packet is
`.analysis/gpt-5.6-sol/20260916-ecl-typed-arithmetic/final/`. A cold full-source
gate preserves all **8 canonical ECL units / 2 artifacts / 724 bytes** exactly.
The generic VM's opcode semantics, case topology and typed arithmetic data flow
are therefore closed for core reconstruction; its 104-byte aggregate codegen
gap belongs in the Web exact queue.

## Completed packet: ANM/ECL Web handoff gate

`scripts/report-source-completeness.py` now audits reviewed authored owners
independently from exactness and can fail closed for selected modules. The
focused gate for `AnmManager`, `EclVm`, and `Enemy` covers **178 authored owners
/ 74,197 bytes** and reports **178 source-present, zero source-absent**. Of
those, 94 owners are canonical exact and **84 owners / 58,059 bytes** are
source-present/non-exact. The matching exact-backlog and completeness reports
are retained below
`.analysis/gpt-5.6-sol/20260916-anm-ecl-web-handoff/`.

The three target-bound topology gates also pass together: ANM `ExecuteScript`
has all 94 enum values and explicit cases, generic ECL `Run` has all 59 active
opcodes in target physical order, and the Enemy dispatcher has all 124 active
opcodes with its complete top-level and nested tables. This establishes the
requested semantic handoff condition without converting source presence into
an exactness claim. Large non-exact owners remain in the queue, including the
14,416-byte Enemy dispatcher, 9,587-byte ANM executor and 7,020-byte generic ECL
VM, but their opcode/business coverage is maintained; their remaining task is
compiler/context matching and complete owned-extent replay.

## Completed packet: Web Python decoder selection

The screenshot's Capstone preflight failure is an interpreter-path issue. On
this host `/usr/bin/python3` imports system Capstone 4.0.2, while the Conda
Python imports the hash-pinned Capstone 5.0.6. A direct system-Python
`verify-toolchain.py --execute` correctly fails closed; it now reports both the
observed package path and the required `scripts/repo-python` entry point.

The new executable `scripts/repo-python` tries the current Python and available
Conda candidates, selecting one only after `verify_capstone` checks all four
unchanged lock hashes. `AGENTS.md` and the README now use it for repository
commands. With `PATH=/usr/bin:/bin` to reproduce the Web shell, the wrapper
selects the pinned interpreter and passes the complete VC7.1/Wine normal COFF,
LTCG, resource, PE32 and PDB extent smoke. The generic ECL table gate also
passes in that environment. The receipt is
`.analysis/gpt-5.6-sol/20260916-python-selection-toolchain.json`.

Factory-native `th10-ghidra` discovery and `check {}` independently passed for
`target:th10-main` with `provider_transport=factory-native-command`. The target
hash and exact ledger remain unchanged; this packet repairs only environment
routing and error diagnosis.

## Completed packet: ECL VM allocator hard frontier

This packet deliberately stayed on the 7,020-byte generic
`EclVmContext::Run @ 0x0044E1A0-0x0044FD0B` instead of consuming easier
near-match leaves. Fresh Factory-native Ghidra attestation remained passed for
`target:th10-main` with `provider_transport=factory-native-command`, and raw
target review reconfirmed the complete code/table owner and its one real caller
`EclVmHost::Run`.

A fresh current-source `/GL` link reproduces the same **6,916-byte** PDB-owned
candidate and **6,588 / 6,692-byte pre-table span** recorded by the typed
arithmetic packet. Repeating the link in the already canonical
`EnemyRuntimeUpdate -> EclVmHost::Run -> EclVmContext::Run` context, with
`src/Enemy.cpp` as a support source and the real Enemy runtime entry, leaves the
Run contribution at 6,916 bytes and preserves the same entry register shape.
Thus the remaining gap is not caused by omitting that proven outer caller
context.

Direct target/candidate comparison isolates the dominant systematic difference.
The target keeps `this` in ESI, `&instruction` in EBX, zero in EBP, and the
current instruction in EDI; the candidate keeps the same ESI/EDI roles but
exchanges the EBX/EBP live ranges. Every inlined typed pop therefore uses target
`ADD reg,-4; CMP reg,EBP; JL` instead of candidate `SUB reg,4; JS`, adding two
target bytes per pop. The target-minus-candidate destination-gap sum across all
59 physical case groups is **93 bytes**; the complete pre-table difference is
**104 bytes**, leaving only eleven bytes in shared/prologue-tail shape. Binary
typed arithmetic/logical cases are normally four bytes short and unary cases
two bytes short, exactly matching their pop counts.

The large format case supplies an independent lifetime check. Target opcode
`0x1E` uses the outer EDI instruction directly, then temporarily repurposes EBX
as the one-byte operand-flag index and EBP as value-word index six. Before the
common advance tail it restores zero with `XOR EBP,EBP`; the tail reloads the
instruction from `[ESI+4]` and reconstructs `&instruction` in EBX. This proves
that the allocator intentionally recolors those registers across the case and
that no separate persistent owner is missing.

Bounded natural-source probes were deliberately kept below ignored scratch and
were not promoted. Swapping declarations, `register`/`const` hints, pointer
versus reference aliases, moving `current` to function scope, and using the full
Enemy caller graph either emitted the exact same 6,916-byte contribution or did
not fix the EBX/EBP exchange. Direct-member variants were 6,900 bytes; a direct
loop form fell to 6,800; a direct entry check reached 6,932 only by introducing
a second null test absent from the target; direct-context and lexical format
variants were 6,904 and 6,900 bytes and changed register coloring in the wrong
direction. Natural maintained source was restored unchanged. The compact probe
summary is `.analysis/gpt-web/20260916-ecl-run-hard/findings.json`; reproducible
variant sources and diagnostic build trees were removed at checkpoint.

No exact ledger row changed: repository state remains **1,313 candidates, 299
source mappings, 134 canonical exact functions / 18,454 bytes**. The honest
product check remains separate: `python3 scripts/build.py --check` passes the
explicitly open graph, while `python3 scripts/build.py` returns **RC2** because
production compiler flags, TU partition, libraries, resources, and link order
remain unknown. Runtime was not attempted.

The preceding `34e55b8 gpt-5.6-sol: repair Web Python toolchain selection`
checkpoint is valid local repository infrastructure work, but this Factory
repository runner exposes `HOME=/tmp/home` and cannot see the operator Conda
interpreter. `scripts/repo-python` therefore correctly fails closed in this
runner even though the retained local receipt proves the pinned Capstone 5.0.6
path and full toolchain smoke in the operator Web shell. This packet grants no
new linked exactness on that basis; canonical replay must be rerun from a
Factory-visible pinned interpreter before any future promotion.

## Completed packet: exact ECL stack/read leaves

The small-leaf pass revisited the four ECL near matches whose only ordinary-code
difference was the first four-byte stack-pop decrement. The key source-shape
recovery is semantic rather than byte-directed: `EclVmStackView::Pop` now tests
`stackTop - size`, then updates the member directly with `stackTop -= size` and
copies from the updated top. Its standalone 122-byte canonical body remains
unchanged, while VC7.1 LTCG now lowers constant-size inlined pops to the target
`ADD reg,-4` encoding. `LeaveFrame` uses the same direct state-update form for
its saved-frame pop.

That single owner-shape correction closes four reviewed leaves:
`EclVmContext::ReadFloat @ 0x0044FE40` (183 bytes),
`ReadIntValue @ 0x0044FF00` (124), `ReadFloatValue @ 0x0044FF80` (171), and
`EclVmStackView::LeaveFrame @ 0x004506D0` (41). Two independent cold builds of
the maintained `EclVmContext::Run` linked context reproduce each complete
PDB-owned extent with zero ordinary-byte differences after replaying the
exhaustive declared DIR32/REL32 fields; `LeaveFrame` is raw 41/41 byte-identical.
The same-context accepted units `ResolveFloat`, `FindThread`, `StopAllThreads`,
`FindSubroutine`, `Push`, `EnterFrame`, and `Pop` remain zero-difference. The
separate `EclVmHost::Run` canonical context was rebuilt with `src/Enemy.cpp`
support and also remains 146/146 normalized zero-difference.

The tracked graph now contains **138 exact match units** and the generated status
reports **138 source-present exact functions / 157 authored source backlog**.
These four local exact claims are ready for the required post-commit Factory
canonical replay; they are not counted as a new accepted snapshot until that
receipt is accepted. The generic 7,020-byte `EclVmContext::Run` allocator gap
remains open and was not forced with register hints, dummy dependencies, inline
assembly, or copied target bytes.

## Completed packet: exact normal-COFF `CPbgFile::Write` leaf

After the linked-image ECL packet, the pass deliberately rotated to a Factory-
replayable normal-COFF leaf rather than forcing a linked unit through the current
normal-COFF-only Factory acceptance driver. `CPbgFile::Write @ 0x004354D0` was
already close: the maintained normal-COFF body was 67 bytes versus the 63-byte
target and matched through the `WriteFile` call. Rewriting only the final bool
expression as an explicit true/false branch makes VC7.1 naturally retain the
requested length in ESI and compare it directly against the stack local, yielding
the target `cmp esi,[esp+4]; sete al` tail.

The new `pbgfile-write` canonical unit covers the complete 63-byte COMDAT and
its sole DIR32 relocation to `__imp__WriteFile@20 -> 0x004660D4`. Two independent
cold `replay-exact-units.py` runs both report 63/63 exact. Rebuilding the shared
`PbgFile.obj` also leaves the six pre-existing normal-COFF units (`destructor`,
`Close`, `Read`, `Tell`, `GetSize`, `Seek`) exact with complete reviewed
relocation coverage. No private calling-convention annotation, inline assembly,
padding, copied bytes, or inert dependency was introduced.

The tracked repository graph therefore advances to **139 canonical exact
functions / 19,036 authored exact bytes**, with **156 authored source backlog**.
The preceding four ECL linked-PE claims remain locally reproducible exact claims
but cannot yet receive a Factory acceptance receipt because the currently
exposed TH10 Factory replay driver accepts only declared normal-COFF units. This
`CPbgFile::Write` unit is intentionally in that supported lane and should be
submitted after the checkpoint commit.

## Next hard frontier

Continue the user-selected ANM/ECL exact campaign owner by owner. The refreshed
rank leaves two thoroughly triaged near matches first: `UpdatePulsingRadialTrail
@ 0x00445620` is 598/598 bytes and 524/526 comparable bytes, while
`SetAndExecuteScriptIdx @ 0x0043E8B0` is 287/287 and 249/271. Their remaining
differences are respectively one commutative X87 operand order and one complete
`ESI`/`EDI` live-range exchange. Retain their natural source until stronger
production-context evidence exists. The four 93/95-byte
`CreateVmAtScreenVariant*` siblings can remain in the Web leaf queue unless
their shared two-byte issue reveals useful owner context. `CaptureToSurface @
0x00448450`, `DrawMode7 @ 0x004445C0`, and the complete `Draw @ 0x004451C0`
dispatcher are now canonical exact.

The two largest ANM/ECL execution owners now have their core semantics and
table topology represented. ANM `ExecuteScript @ 0x0043EE30` retains a
9,472/9,588-byte pre-table span; its remaining shared-advance placement,
Float2 four-byte extent difference, Float3 33-byte extent difference and outer
register schedule are suitable for the Web exact queue. Generic ECL `Run @
0x0044E1A0` retains a 6,588/6,692-byte pre-table span with exact active-opcode
order; its typed-pop short-branch encoding, `EBX`/`EBP` live-range exchange and
small internal-call ABI differences are likewise exact-codegen work. Retain
the exact host runner and the complete ANM/ECL cold replays as regression gates.
Keep exactness tied to complete owned extents; linked diagnostic proximity alone
does not promote a unit.

Do not reopen a supposed second Enemy opcode cohort: the 14,416-byte maintained
dispatcher already covers every target-active selector. Web work should not
repeat the exhausted ECL alias/declaration/register probes. The next hard packet
should rotate to `AnmVmView::ExecuteScript @ 0x0043EE30`
(the 9,588-byte central ANM executor) or return to ECL only with new production
optimizer-context evidence; the four small ANM creator siblings remain a leaf
queue rather than the default choice. The real production link graph
remains a major infrastructure gap; canonical bounded linked-image units do not
close it.

The exact-reconstruction campaign remains active/incomplete. Faithful Windows-i386 whole-build closure, runtime validation, Factory acceptance, semantic reconstruction and portability remain open.

## Completed packet: normal-COFF leaf sweep and Main exact promotion

This pass re-ranked the complete authored normal-COFF backlog by ordinary-byte
difference count and explicitly rejected several tempting private-ABI leaves
rather than reshaping source around optimizer registers. `AsciiManagerView::
OnDrawHighPriority`, `ReplayManager::Destroy`, `AsciiManagerCreate`,
`CMemoryPbgFile::Open`, `EnemyEclResourceView::LoadFile`, `PlayerCreate`, and
the pending-interrupt wrappers all expose target-only live-register or
devirtualized LTCG contracts and remain non-exact in standalone COFF.
`UpdatePulsingRadialTrail @ 0x00445620` remains 598/598 with only the Y-component
X87 load/add operands exchanged; reversing the whole vector expression worsened
the comparison and explicit component expansion changed the body size, so the
natural operator source was retained.

One genuine leaf source-shape correction closed `AnmRenderManagerView::
GetVmPosition @ 0x004493C0`. Replacing the ternary null return with an explicit
non-null branch yields the retail 27-byte control flow naturally. The new
`anm-manager-get-vm-position` normal-COFF unit replays all 27 bytes and its sole
`FindVm` REL32 field exactly on two cold builds; the six older normal-COFF units
sharing `AnmManager.obj` remain exact in both passes.

The sweep also exposed a canonicalization gap: `GameWindowView::ResetRenderState
@ 0x00439D20` was already 710/710 structural-exact in the maintained source but
had never been admitted because physical normal-COFF versus LTCG ownership is
still unknown. Exact codegen and production ownership are independent. The new
`main-reset-render-state` unit therefore records the bounded normal-COFF fact
without changing the ownership claim: two cold replays reproduce all 710 bytes,
574 ordinary bytes, and all 34 DIR32 fields.

The tracked graph is now **141 canonical exact functions / 19,773 authored exact
bytes**, with **154 authored source backlog**. `GameWindowView::GetTimestamp @
0x00439540` remains a useful near-exact profile frontier: normal VC7.1 emits the
correct 282-byte body and every relocation, with only the prologue and two
epilogue stack-allocation immediates at `0x10` versus target `0x14`. Named-local,
local-count/order, static-versus-member, `/Zi`, `/Z7`, `/GS+/-`, `/EHsc`, `/G6`,
`/Ob1/2`, `/O1 /Ot`, `/Ox`, and `/Og` experiments do not change those three
bytes; `/G7`, `/Os`, and `/Oy-` change the body substantially. TH10 call sites
make the current static source spelling the best local evidence, so no ABI change
was retained.

## Next leaf frontier

Continue with standard-ABI leaves first. Avoid the already classified
private-register wrappers unless a real linked production context becomes
available. The best current near-exact owner remains `UpdatePulsingRadialTrail`
(two ordinary bytes), followed by `GetTimestamp` (three frame-size bytes), but
both now have bounded exhausted natural-source/profile probes. Rotate to another
owner rather than using volatile qualifiers, fake dependencies, inline assembly,
or calling-convention lies.

## Completed packet: FPS caller context closes GetTimestamp and CalculateFps

The leaf pass found that `GameWindowView::GetTimestamp @ 0x00439540` was not
missing a local variable or source-level pragma. Normal COFF and incomplete
linked contexts already reproduced the complete 282-byte body and every one of
its 30 address/import/constant fields; only the prologue and two epilogue frame
immediates used `0x10` instead of target `0x14`. `LARGE_INTEGER` is size eight
and alignment eight under the pinned VC7.1 SDK/compiler, and all declaration,
`#pragma var_order`, `/Zi`/`/Z7`, `/GS`, optimization, and member/static probes
left those three bytes unchanged.

The missing evidence was a real cross-owner caller. Target owner
`0x004134B0-0x004135C7` begins `push ebp; mov ebp,esp; and esp,-8` and calls
GetTimestamp three times while maintaining the half-second FPS sample state.
The adjacent `0x004135D0-0x00413683` display owner calls that sampler, emits the
literal `%2.1ffps`, and increments its frame count by `frameskip + 1`; callback
bridge `0x00413690` directly calls the display owner at `0x00413693`. TH095's
`CalculateFps`/`DrawFpsCounter` family corroborates only the descriptive family
name. TH10 itself proves every maintained offset, branch, threshold and call.

`src/Main.cpp` now contains a bounded `FpsCounterView` of size `0x8C` and a
minimal `FpsSampleGateView`. `FpsCounterView::CalculateFps` binds the target
fields at `+0x14/+0x1C/+0x20/+0x24/+0x2C/+0x34`, the 0.5-second sample window,
65-FPS clock-anomaly handling, 57/60-FPS lag accounting, and the QPC reset path.
`DrawFpsCounter` is retained as source-present support and as the real optimizer
entry context; its own exactness remains open.

Two independent cold VC7.1 SP1 `/GL` links using
`?DrawFpsCounter@FpsCounterView@@QAEHXZ` as entry reproduce:

- `FpsCounterView::CalculateFps @ 0x004134B0`: 280/280 bytes with all 20 declared
  REL32/DIR32 fields replayed and zero ordinary-byte differences;
- `GameWindowView::GetTimestamp @ 0x00439540`: 282/282 bytes with all 30 declared
  critical-section, timing-import, GameWindow, supervisor and floating-constant
  fields replayed and zero ordinary-byte differences.

The key codegen seam is physical rather than cosmetic: once the real FPS caller
supplies an 8-byte-aligned stack phase, VC7.1 naturally changes GetTimestamp's
local allocation from `sub esp,0x10` to the target `sub esp,0x14`; no source
padding, volatile qualifier, dummy local, assembly, or copied target bytes are
used. `config/match-units.toml` records both linked units in the shared
`MainFps` context.

The repository now tracks **143 canonical exact functions / 20,335 exact
bytes** and **301 source mappings**. The authored source-present exact backlog
remains 154 because GetTimestamp leaves the backlog while the newly recovered,
non-exact DrawFpsCounter enters it. The live repository shell currently cannot
import the hash-pinned Capstone 5.0.6 package, so `scripts/repo-python` and the
formal `replay-exact-units.py` wrapper fail closed. No system Capstone fallback
was used; the two cold links were instead checked directly across complete
owned extents with exhaustive declared fields. Factory service acceptance for
linked-PE claims remains a separate platform capability/receipt question.

## Current leaf frontier

The remaining five same-size historical near matches at five ordinary bytes or
fewer are now well classified. `UpdatePulsingRadialTrail @ 0x00445620` remains
598/598 with only the Y-component X87 load/add operands exchanged after natural
operator/local/profile experiments. `EnemyManagerUpdateCallback @ 0x0040D810`,
`AsciiManagerView::OnDrawHighPriority @ 0x00401520`, and
`PbgArchive::~PbgArchive @ 0x00434C20` are linked private-register bridge
frontiers whose owner contracts choose the wrong nonvolatile register in the
available graph; member/declaration/loop-shape probes did not move them.
`EclVmHost::SpawnThread @ 0x004500D0` is controlled by the still-open private
EAX receiver contract of `EclVmStartSubroutine` inside the larger generic ECL
optimizer graph. Do not revisit these with ABI lies or inert code shaping; rotate
to a new standard-ABI leaf unless stronger production-link evidence appears.

### Follow-up: exact FPS display owner

The first FPS checkpoint deliberately left `FpsCounterView::DrawFpsCounter @
0x004135D0` source-present but non-exact. Its Main-only `/GL` candidate already
had the correct 180-byte physical extent, but could not reproduce the target's
private call into `AsciiManagerView::AddSmallFormatText`: without the formatter
TU visible, VC7.1 kept manager/position as ordinary call arguments and reloaded
`currentFps` around both comparisons.

Adding the real `src/AnmManager.cpp` support contribution resolves that seam.
VC7.1 then naturally selects the target `EDI` FPS owner, `ESI` ASCII manager and
`EBX` position contract. A source-local `float fps = currentFps` preserves the
same x87 value across the `<30` and `<40` tests and the final variadic conversion,
yielding the target `FCOM`, second `FCOM`, then `FSTP QWORD [esp]` sequence.
No register annotation, artificial dependency or assembly is used.

Canonical unit `main-fps-draw` uses the retained FPS draw adapter as the linked
entry and `src/AnmManager.cpp` as support. Two independent cold VC7.1 SP1 `/GL`
links reproduce all **180/180** target bytes after replaying nine declared fields.
The same links emit adapter `0x00413690` as the target ten bytes exactly, including
the relative call displacement; the adapter remains outside canonical exactness
because source-written versus compiler/LTCG-generated provenance is still
unresolved. It is maintained only as optimizer-context glue.

The Draw change was also replayed twice in the original Main-only FPS context:
`CalculateFps` remains 280/280 normalized zero-difference and `GetTimestamp`
remains 282/282 normalized zero-difference. The FPS family therefore closes
three canonical owners / 742 bytes in total with no exact regression. Repository
totals are now **144 canonical exact functions / 20,515 exact bytes**, with
**153 authored source-present exact backlog**.

## Completed packet: exact `AnmInt3View::operator*` scale leaf

The former descriptive `ScaleAnmInt3 @ 0x00441E50` helper is now represented by
the source abstraction that explains its complete target ABI and instruction
schedule: `AnmInt3View::operator*(float) const`. The unique color-Hermite caller
physically calls this owner four times. Target call order is final, initial,
initial tangent, final tangent; each call supplies the triplet in EDI, hidden
result storage in ESI and one float stack argument. The target callee returns
with `RET 4`.

VC7.1's value-return path is sensitive to constructor argument evaluation. The
maintained three-argument `AnmInt3View` therefore keeps X/Z/Y argument slots
while storing members in the normal X/Y/Z field order. `operator*` returns
`AnmInt3View(x*scale, z*scale, y*scale)`: VC7.1 evaluates those arguments from
right to left as Y/Z/X, exactly matching the target's three FILD/FMUL/__ftol2
sequences, while the hidden-result stores naturally become Z/X/Y at the target
addresses. No explicit private calling convention, noinline annotation, asm,
volatile qualifier, padding, copied bytes or fake dependency is retained.

Two independent cold `ExecuteScript`-context links from tracked source emit the
same complete 56-byte PDB contribution. After replaying the three __ftol2 REL32
fields at offsets 0x09, 0x17 and 0x25, all ordinary bytes are zero-difference.
All 16 previously canonical exact units sharing this exact entry/profile remain
zero-difference in the final tracked-source regression. Clean metadata validation
advances the repository to **147 canonical exact functions / 20,729 exact bytes**
with **150 authored source-present functions** remaining in the exact backlog.

`UpdatePulsingRadialTrail @ 0x00445620` remains a separate 598/598 two-byte X87
operand-scheduling frontier. Its natural caller/type/profile/link-root variants
have been exhausted and should not be forced with volatile/asm/inert shaping.

## Completed packet: IDA recovery, angle and Main timing leaves

This session recovered ten pre-existing dirty tracked files before selecting new
work. The dirty state contained three new source-present names without mapping
rows, two declared Enemy match units without exact ledger rows, and a missing
`MainInitializeSystemParameters` forward declaration that made `Main.cpp` fail
VC7.1 compilation. Those gaps were repaired in place, without resetting or
silently staging unknown work. The source for GUI life display, Player call-site
routing, Main system/timing helpers and the `Lzss::DeleteString` ECX ABI was
retained after target review and focused compiler checks. No target bytes or
private game data changed.

Per the current operator request, live analysis used IDA Pro MCP instead of the
Factory Ghidra provider. IDA metadata reports SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, image base `0x00400000`, and
file size `0x77200`; the independent canonical `resources/th10.exe` verification
passed. IDA decompilation was treated as provisional. It does not identify a
function at `0x00439350`; direct pinned-Capstone decode covers that complete
62-byte reviewed extent through RET. The reference
`saekaze/th10-decomphelp-forN0` was shallow-cloned under
`.analysis/gpt-web/20260917-ida-progress/reference/` for hypotheses only. Its
port/progress numbers and source did not establish any TH10 exact claim here.

`EnemySetMotionAngle @ 0x00413170` and `EnemyWrapAngle @ 0x0044BC70`
now replay as two canonical normal-COFF exact units: 20 + 94 bytes, one REL32
call and six DIR32 constants. Two independent cold replays pass all 114 bytes.
The source and the `Enemy.cpp` declaration use the target `__stdcall` RET 8 /
RET 4 contracts. This shifts VC7.1 local label names in two pre-existing Enemy
COFF units; their manifests were updated to the actual compiler labels while
keeping every target destination fixed. A full affected-source regression then
passed all 23 Enemy units.

`MainInitializeSystemParameters @ 0x004392E0`,
`MainRestoreSystemParameters @ 0x00439350`, and
`MainGetTimestampUnlocked @ 0x00439660` are also canonical normal-COFF exact
in the declared profile. Strict diagnostics first reported complete sizes
100/62/152 and 68/42/96 matching non-relocation bytes; two independent cold
replays then reproduced all 314 bytes and all 8/5/14 declared fields. The
production COFF/LTCG physical owner and original identifiers remain unknown.
The complete affected-source regression passed 34 units / 2,647 bytes across
Enemy, Enemy ECL dispatcher, LZSS and Main; GUI and Player `/GL` smoke builds
also passed. The only changed-source support edge, `src/Enemy.cpp` into
`ecl-vm-host-run`, was separately replayed with zero differences across its 146-byte linked extent.
`GuiSetLivesDisplayCount` and `Lzss::DeleteString` remain source-present but
non-exact.

The live ledger now has 1,313 candidates, 1,175 reviewed boundaries, 320
reviewed authored functions, 307 source mappings, and 161 canonical exact
functions / 22,413 exact authored bytes. Seventeen reviewed authored functions
still lack source; 142 reviewed authored mapped functions remain non-exact;
978 origin reviews and 138 boundary reviews remain open. The Windows i386
product graph is still a skeleton with unknown TU partition, flags, resources,
libraries, static/data owners and link order; runtime, semantic dual-Oracle and
portability gates remain unopened. The largest mapped exact backlogs remain the
14,416-byte Enemy ECL dispatcher, 9,587-byte ANM script executor, and
7,020-byte generic ECL VM runner. Their codegen/context issues are documented
above and should not be forced with target-byte copies or fake dependencies.

## Active packet: IDA boundary and runtime-origin review

The operator asked to finish authored/origin and boundary review before
revisiting the three large dispatchers, to use IDA Pro MCP instead of Factory
Ghidra, and to favor focused checks over unrelated cold builds. This packet
uses only the hash-attested original Japanese target. The locked VC7.1 SP1
`libcmt.lib` and `libcpmt.lib` are checked by SHA-256 before analysis.

`scripts/review-crt-origins.py --apply` classified 242 uniquely matched
`libcmt.lib` function extents (49,952 target bytes) as library/excluded. It
requires a complete matching COFF function extent, at least 32 bytes of body,
at least 24 exact non-relocation bytes, and a unique archive match. All 635
resolvable intra-runtime relocation targets agree with the target addresses.
The detailed candidate and ambiguity report is ignored under
`.analysis/gpt-5.6-sol/20260917-origin-review/`. No candidate was added to
`config/matches.csv`. The same member extents promoted 41 non-overlapping
boundary rows. Overlapping function candidates were left open.

The IDA function inventory and target xrefs identified 28 more previously
provisional entries. `scripts/review-boundary-entry-refs.py --apply` replays a
target-bound Capstone check that each recorded reference is an external direct
call or jump into its reviewed entry. The prior dense-decode audit and the
independent IDA extent agree. These 28 boundaries were promoted without using
IDA decompilation as an exactness Oracle.

Further boundary review corrected `0x00463C0D` from a false 5,022-byte
contiguous span to its 174-byte main fragment, revealing the independent
interleaved math CRT entries. Ten runtime functions contain a small directly
called RET helper inside their full function extent; the two corresponding
ledger rows now document this intentional overlap. Six independent math helper
entries, a seven-byte shared prefix, nine focused termination/exception
entries, and three target-direct entries omitted by IDA were also replayed.
The final fifteen no-reference code islands were reviewed at medium confidence
from eleven matching IDA extents or four target-local RET/CC separators. The
two 829-byte memory-copy bodies have embedded dispatch data; both match
complete pinned CRT object extents and all 46 local relocation targets stay
inside their physical range. Thirteen additional ambiguous-symbol CRT-family
bodies were classified as library origin without choosing a symbol name.

Current ledger: 1,313 candidates and **all 1,313 boundary rows reviewed**;
320 reviewed authored, 270 excluded, and 723 origin reviews pending; 307
source mappings and 161 canonical exact functions / 22,413 exact bytes.
The 490 pending origins before `0x00452000` need owner and
authored-versus-compiler evidence; an address range alone is insufficient.
The remaining library-region origins include short, ambiguous, and unmatched
bodies. Origin review is therefore still open, and the three large dispatchers
have not been resumed in this packet.

## Completed checkpoint: target import, EH, and CRT-origin expansion

The next origin pass classified 21 import thunks by decoding their six-byte
`FF 25` body and parsing the target's own PE import descriptor/IAT slot. The
target's `.rdata` also contains 41 valid VC7.1 C++ `FuncInfo` records with
magic `0x19930520`; their unwind maps identify all 62 current `Unwind@`
cleanup actions. These were classified compiler generated after complete
target code decoding. `scripts/review-import-origins.py` and
`scripts/review-eh-origins.py` replay the evidence without IDA write access.

The pinned CRT scan additionally accepted 39 unique short function bodies at
medium confidence, 30 complete COFF bodies whose physical object extent is
longer than the ledger's local entry fragment, 30 code fragments contained
inside those complete runtime member extents, and 11 lower-information
same-size CRT matches supported by the candidate's provisional name. The
complete extended bodies match all non-relocation bytes, and all 961
uniquely resolvable intra-runtime reference fields reach their expected
target addresses. These are origin observations only; no source/exact ledger
was changed. The `.analysis/gpt-5.6-sol/20260917-origin-review/` report keeps
the complete accepted and ambiguous match lists.

Latest ledger: 1,313/1,313 boundaries reviewed; 783 origin rows reviewed
(320 authored, 463 excluded), 530 origin rows pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Of the pending origins,
489 lie before `0x00452000` and 41 after. The remaining authored and
compiler-versus-library provenance requires target-local evidence; the
three large exact dispatchers remain queued behind that origin review.

## Completed checkpoint: target call and shared-global authored origins

`scripts/review-game-global-origins.py --apply` classified 46 more game-region
entries as authored at medium confidence, covering 31,234 target bytes. Every
accepted entry completely decodes within its reviewed physical extent, directly
calls a previously reviewed authored function, and references at least two
specific game-global addresses that each occur in two or more previously
reviewed authored bodies. No reviewed excluded body uses those selected
globals. The classifier excludes its own 46 results from the seed set, so the
evidence does not amplify itself on replay. IDA spot checks at five entries
support the game-code interpretation, but do not establish source identity or
exact codegen. The functions keep unknown subsystem and source ownership.

Current ledger: 1,313/1,313 boundaries reviewed; 829 origin rows reviewed
(366 authored, 463 excluded), 484 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. The pending origins
include 443 game-region entries and 41 later runtime-region entries. Continue
conservative origin review before the three requested large exact owners.

## Completed checkpoint: direct authored neighbors with shared globals

The same target-bound game-origin reviewer now identifies a second fixed cohort:
77 more complete bodies, totaling 30,505 bytes. Each references a game-global
address used by at least two of the original 320 reviewed authored functions
and has a direct call edge to or from an original reviewed authored function.
Selected globals are absent from reviewed excluded bodies. IDA spot checks
support game-specific behavior across several code regions. Both cohorts keep
their own evidence IDs and exclude one another from the seed set on replay.
The second cohort is medium-confidence authored origin only; source, subsystem,
production compiler mode and exactness remain open.

Current ledger: 1,313/1,313 boundaries reviewed; 906 origin rows reviewed
(443 authored, 463 excluded), 407 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. The pending origins
include 366 game-region entries and 41 later runtime-region entries.

## Completed checkpoint: call/data owners, catch handlers and CRT wrappers

`scripts/review-game-edge-origins.py --apply` classified 11 complete game bodies
between direct calls from and to the original 320 reviewed authored functions,
plus 18 more complete bodies with an original authored call edge and a shared,
non-excluded `.data` address. The generic `1.0f` data constant is explicitly
excluded. These 29 medium-confidence origins cover 10,641 target bytes and do
not grant source presence or exactness.

The target's VC7.1 `TryBlockMap` and `HandlerType` records directly point to
the two tracked `Catch@` bodies; `scripts/review-eh-origins.py --apply` now
classifies them compiler generated while replaying all 62 existing unwind
actions. Eleven individually reviewed 5–15-byte CRT wrappers directly enter
an already reviewed runtime body and were classified library origin at medium
confidence by `scripts/review-crt-wrapper-origins.py --apply`. Their exact CRT
object identities remain open.

Current ledger: 1,313/1,313 boundaries reviewed; 948 origin rows reviewed
(472 authored, 476 excluded), 365 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Four source-present
player callback adapters remain origin-unknown by prior review. Three large
exact dispatchers remain queued behind origin review.

## Completed checkpoint: individually reviewed game behaviors

Twenty more large, complete bodies totaling 14,404 bytes were individually
inspected with IDA and replayed from target instructions in
`scripts/review-game-behavior-origins.py --apply`. They show ANM VM commands,
stage/score state, player options, the target `TH10` replay header, game sound
cues and input transitions. Every row has a direct call edge to or from an
already reviewed authored function. Two reviewed authored callers include
switch data that blocks a whole-span linear decode; their actual `call`
instructions at `0x00432DB9` and `0x0042F6B6` are independently decoded and
bound to the caller span. The origin confidence is medium; source units and
exact codegen remain unknown.

Current ledger: 1,313/1,313 boundaries reviewed; 968 origin rows reviewed
(492 authored, 476 excluded), 345 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Origin review still
precedes the three requested large exact owners.

## Completed checkpoint: five additional runtime entries

The runtime reviewer now checks five further complete entries: SSE `floor`
reaching two reviewed CRT math bodies, CRT floating-conversion pointer-table
initialization, `__NLG_Notify1` and `strcpy` prefixes jumping into reviewed
library bodies, and an x87 status-word helper. Each had an IDA behavior review;
the replay script checks the concrete target transfer or instruction/data shape.
They are medium-confidence library-origin observations without source-exact or
production object ownership claims.

Current ledger: 1,313/1,313 boundaries reviewed; 973 origin rows reviewed
(492 authored, 481 excluded), 340 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. The remaining 25
runtime-region entries include ambiguous C++ standard-library bodies and
unresolved short helpers. The authored-origin queue remains open.

## Completed checkpoint: standard C++ exception and EH runtime cluster

`scripts/review-cxx-runtime-origins.py --apply` classified 19 complete
`std::string`, standard exception, type-info and throw-helper bodies as
medium-confidence C++ runtime library origin. IDA behavior and target vtable,
call and exception-string observations support the family interpretation;
duplicate template code still leaves exact CRT object membership unknown.
Three identical seven-byte `_CallMemberFunction` adapters are directly called
from already reviewed CRT EH bodies, with their exact target call sites replayed.
Those are also excluded as runtime library origin at medium confidence.

Current ledger: 1,313/1,313 boundaries reviewed; 995 origin rows reviewed
(492 authored, 503 excluded), 318 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Only three short
runtime-region entries remain pending; 315 game-region entries, including four
source-present player adapters, still require authored-versus-library or
generated-origin evidence.

## Completed checkpoint: remaining tracked CRT callback helpers

Three short runtime entries now have independent target callback ownership:
`0x0045631E` is the `TlsAlloc` import wrapper installed by reviewed CRT
`__mtinit`; `0x00456A8B` is a one-byte RET stored in two CRT thread callback
slots; `0x0045E3A0` is a zero-result math callback stored in a CRT data slot
and directly called by two reviewed math bodies. The callback reviewer checks
all target instruction, import and data references before classifying their
origins library at medium confidence. Exact CRT object membership stays open.

Current ledger: 1,313/1,313 boundaries reviewed; 998 origin rows reviewed
(492 authored, 506 excluded), 315 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. All current pending
origins are before `0x00452000`; four have maintained Player callback sources
but unresolved authored-versus-generated provenance.

## Completed checkpoint: early game behavior entries

IDA behavior review plus target-decoded direct call edges classified 32 more
complete entries in the first `0x0040xxxx` code region as medium-confidence
authored game code. The 6,650-byte cohort covers game/ANM managers,
projectiles, playfield bounds, scheduling and resource access. The reviewer
keeps the original 20 manually checked game entries out of this cohort's
authored seed set, so the new result does not rely on itself. Nearby pure
math/constructor fragments with no independent reviewed call edge stay open.

Current ledger: 1,313/1,313 boundaries reviewed; 1,030 origin rows reviewed
(524 authored, 506 excluded), 283 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Origin review remains
the gate before the three large exact tasks.

## Completed checkpoint: middle game behavior entries

Twenty-two complete `0x0041xxxx` entries totaling 2,732 bytes were reviewed
for stage objects, scheduling, projectile pooling and
playfield behavior. Each has a direct target call edge to or from an authored
owner outside the current manual behavior cohorts; the reviewer excludes all
three of its own cohorts from the seed set on replay. The medium-confidence
origin classification leaves source and exactness open. Generic string/math
helpers and constructor fragments without independent authored edges remain
pending. The 75-byte ECL integer-lvalue helper at `0x00412A10` also remains
unknown: a direct dispatcher call proves its use, but prior target review left
source-written versus compiler-outlined provenance unresolved. It is outside
the accepted cohort and does not need a fabricated source mapping.

Current ledger: 1,313/1,313 boundaries reviewed; 1,052 origin rows reviewed
(546 authored, 506 excluded), 261 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Origin review is still
the prerequisite for the requested large exact work.

## Completed checkpoint: late game behavior entries

Twenty-five complete `0x0042xxxx` entries totaling 3,316 bytes now have
target-decoded call edges to previously reviewed authored functions plus
individual IDA behavior review. They cover resource handling, stage state,
player option/position logic, playfield tests and game controllers. The
reviewer excludes all four of its manual cohorts from its seed set on replay.
Player callback bodies at `0x00427960` and `0x00427AE0` retain their prior
origin-unknown state while their wrapper ownership remains unresolved.

Current ledger: 1,313/1,313 boundaries reviewed; 1,077 origin rows reviewed
(571 authored, 506 excluded), 236 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. Source mapping and
exactness were not changed in this cohort.

## Completed checkpoint: standard string and audio/render origin review

Nine complete `0x004381F0-0x004387C1` candidates are now excluded as VC7.1
`std::basic_string<char>` library code. Target small-string storage, calls to
reviewed standard string throw helpers, and IDA body/EH review support the
classification; `0x0043874C` is a post-catch continuation, not a distinct
source routine. Exact library object membership is still unknown.

Twenty-one more audio/render/game-window candidates totaling 3,690 bytes are
authored at medium confidence. Each has an independently decoded direct call
edge to or from a previously reviewed authored function outside all five
manual behavior cohorts, plus individual IDA behavior review. The reviewed
roles include GDI font/texture work, MIDI output cleanup, D3D drawing, ANM
random state and TH10 sound cue/stream management. Source unit, original
identifier, production build mode and exact codegen remain open.

Current ledger: 1,313/1,313 boundaries reviewed; 1,107 origin rows reviewed
(592 authored, 515 excluded), 206 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. No source or exact
mapping changed in this checkpoint. Continue the remaining origin review
before the requested three large exact owners.

## Completed checkpoint: connected game component origins

Eighteen additional entries totaling 5,902 bytes are authored at medium
confidence. The target-bound reviewer requires a complete decode and a fixed
direct call to or from an independently reviewed authored anchor. Its explicit
cohort order is acyclic, so later entries can use earlier reviewed GDI and
MIDI entries without self-supporting cycles. IDA review covered game text
parsing, render object state, GDI glyph bitmap processing, MIDI tracks and
events, sprite state and sound filename lookup. No original source unit or
exactness is inferred. The DirectInput initializer `0x0043B8D0` was excluded
from this cohort because its observed callee `0x00421D20` is still pending.

Current ledger: 1,313/1,313 boundaries reviewed; 1,125 origin rows reviewed
(610 authored, 515 excluded), 188 origins pending; 307 source mappings,
161 canonical exact functions and 22,413 exact bytes. The next origin queue
still includes source-present Player adapters and ambiguous constructor or
compiler-outlined fragments. Three large source-present exact owners remain
queued behind the user's requested review order.

## Completed checkpoint: game utilities and sound stream origins

Forty more `0x0044xxxx` candidates totaling 7,014 bytes now have
medium-confidence authored origin. The fixed call-anchor replay requires a
complete target decode and a direct edge to a previously reviewed authored
owner; IDA inspection confirmed scheduler/resource nodes, input and logging,
game timer/movement, and sound stream object/file/playback behavior. The
short `0x0044C100/0x0044C130` wrappers and several object lifecycle bodies
remain pending because a game call or address neighborhood does not by itself
settle compiler-generated versus source-written ownership.

Current tracked ledger: 1,313/1,313 boundaries reviewed; 1,165 origin rows
reviewed (650 authored, 515 excluded), 148 origins pending; 307 source
mappings, 161 canonical exact functions and 22,413 exact bytes. IDA's full
function inventory also contains independent-looking starts outside this
tracked candidate set, including `0x0043BA90`, `0x0043BB30` and
`0x0043CCF0`; the 1,313 boundary count applies only to tracked candidates.
These untracked IDA entries need separate target entry/extent triage before
any inventory expansion or exhaustive `.text` claim.

## Completed checkpoint: four pointer-backed boundaries and input/sound origins

IDA's previously untracked `0x004201B0`, `0x0043BA90`, `0x0043BB30` and
`0x0043CCF0` entries now have reviewed target-bound ledger extents. Each has
an independent immediate function-pointer reference, dense decode through
RET, an agreeing contiguous IDA extent and no overlap with tracked entries.
The four add 424 candidate bytes. Of the 892 IDA function starts outside the
old denominator, 888 remain provisional; IDA starts alone do not prove
independent function boundaries.

The new input initializer, polling thread, DirectInput setup and device
callback form a target-replayed acyclic source-behavior chain. Two sound
worker callbacks are pointer-linked to existing authored callers; the sound
setup, WAV format and chunk helpers are linked by direct calls to those
reviewed workers. These nine entries, 2,593 bytes, are authored at medium
confidence without source-unit or exact-codegen claims.

Current tracked ledger: 1,317/1,317 boundaries reviewed; 1,174 origin rows
reviewed (659 authored, 515 excluded), 143 origins pending; 307 source
mappings, 161 canonical exact functions and 22,413 exact bytes. The boundary
statistic remains limited to tracked candidates. The original-executable
target hash and target pointer sites are replayed by the new scripts.

The four focused boundary/origin reviewers, target-required ledger validation,
toolchain check, generated progress check and `scripts/ci.py` pass. The older
`report-boundary-inventory.py --check-ledger` compares against the original
automatic audit and still fails first at `0x00402720`, whose boundary was
manually promoted in an earlier IDA-backed campaign; it does not replay those
manual promotions. This failure predates the four new entries and is not a
target mismatch.

## Completed checkpoint: additional game components and deleting destructors

Forty-six more complete entries totaling 3,997 bytes have authored origin at
medium confidence from individual IDA behavior review and fixed direct target
call edges. They cover ANM and game timers, stage/hint-file helpers, replay
data initialization, input/worker setup, DirectSound buffer creation and
sprite task cleanup. Three separate VC7.1 scalar deleting-destructor wrappers
at `0x0044C100`, `0x0044CF00` and `0x0044D710` are compiler-generated and
excluded after target replay of their destructor call, deletion-flag test,
conditional free and `RET 4`.

Current tracked ledger: 1,317/1,317 boundaries reviewed; 1,223 origin rows
reviewed (705 authored, 518 excluded), 94 origins pending; 307 source
mappings, 161 canonical exact functions and 22,413 exact bytes. The remaining
tracked queue includes constructor/destructor ownership, short adapters,
Player callbacks and ECL/LTCG outlined-helper questions. The larger
untracked IDA-entry queue remains a separate provisional boundary inventory.

## Completed checkpoint: tracked origin review with explicit unknowns

Four more call-backed game helpers are authored at medium confidence. The
remaining 90 tracked entries were individually inspected in target/IDA and
recorded as `indeterminate`, with a per-address observation and reason rather
than being silently counted as authored or excluded. These include C++ object
lifecycle bodies, private adapters, source-present Player callbacks and game
behavior with unresolved original source/linker attribution. The author/exact
denominator is still unknown; `indeterminate` is a completed review outcome,
not a positive origin claim. `report-exact-backlog.py` reports the four mapped
Player entries separately as `origin-indeterminate`.

Current tracked ledger: 1,317/1,317 boundary reviews; 1,317/1,317 origin
reviews (709 authored, 518 excluded, 90 indeterminate), zero unreviewed.
There are 307 source mappings and 161 canonical exact functions / 22,413
bytes. IDA's 888 additional function starts remain provisional and outside
the tracked boundary denominator; exhaustive `.text` inventory is still open.
Next investigate the three requested large exact owners using focused
compilation/linked-image diagnostics, starting with Enemy ECL dispatcher
`0x0040E770` (14,416 bytes). Its small ECL helpers include reviewed but
origin-indeterminate entries such as `0x00412A10`; do not use their apparent
source shape as an authorship or exactness assumption.

## Current bounded exact packet: Enemy ECL dispatcher

The maintained `EnemyEclDispatcher.cpp` had an explicit opcode range guard
before its `switch`. The target has one guard; VC7.1 emitted two for that
source shape. Removing the redundant guard preserves default dispatch and
improves the focused `/GL /GS` candidate from 13,128 bytes / 421 of 11,692
comparable bytes equal to 13,112 bytes / 486 equal. Target extent is 14,416
bytes. The candidate remains non-exact, with incomplete normalization; no
match row or production compiler-profile claim follows.

The new optional candidate mode of `report-ecl-dispatch-table.py` finds the
linked byte selector and jump table from the entry instructions, then checks
adjacency, selector range and pre-table destinations. The candidate selector
matches the target in all 181 bytes; both have 108 distinct jump destinations
and 43 bytes after the selector within their contributions. All 1,304 missing
bytes lie before the table. Physical case order differs from the first group,
so the next source/codegen work should compare case order and per-case size.
The target prologue contains a VC7.1 security cookie and allocates `0x2C4`
bytes; `/GS` reproduces the cookie form but current source allocates `0x2BC`.
The default profile omits the cookie. A separate spell-name buffer made the
frame `0x304` and was reverted. Retained ignored evidence is under
`.analysis/gpt-5.6-sol/20260918-enemy-exact/`.

The other two requested frontiers remain source-present and non-exact. Prior
target-bound linked diagnostics put ANM `ExecuteScript @ 0x0043EE30` at a
9,488-byte pre-table span versus 9,588 target (`-100`) with a misplaced shared
NOP/interrupt advance block. Generic `EclVmContext::Run @ 0x0044E1A0`
remains 6,916 versus 7,020 bytes (`-104`), with 93 bytes of the code gap
attributed to typed-pop case shapes and EBX/EBP register allocation. Those
figures are earlier local observations, not fresh exact claims. Keep using
focused compiler probes; skip unneeded full cold replay while no canonical
unit or shared source is being promoted.

## Current bounded exact packet: ANM and generic ECL dispatch

Fresh focused `/GL` diagnostics supersede the prior ANM/ECL size figures
above. Current ANM `ExecuteScript` under its own selected entry has a 9,408
byte pre-table span against 9,588 target (`-180`), while the 9,784-byte PDB
contribution includes a separate 376-byte jump table. Splitting the shared
NOP/interrupt advance from switch-exit advance puts **all 92 physical case
groups** in target order; NOP/interrupt moves from candidate index 77 to
target index 21. The code span is farther from target than before (`-116`),
so the owner is still non-exact. All 17 exact units sharing this `AnmExecutor`
artifact replay exactly after the change (2,746 bytes); other ANM contexts
were not cold replayed after it.

The ANM replay also surfaced a pre-existing manifest error for
`anm-int3-scale`: two `__ftol2` REL32 offsets described CALL opcodes rather
than displacement fields. Target/candidate disassembly proves fields at
`+0x0A`, `+0x18`, `+0x25`; fixing the first two manifest offsets restores the
56-byte exact unit. Before the ANM source change, full cold source replay
passed 79 units / 16,420 bytes over ten artifacts. The false attribution of
the initial replay error to the NOP source experiment was corrected by
retesting after reverting that experiment.

Current generic `EclVmContext::Run` in the selected `EclVmHost::Run` context
has a 6,712-byte pre-table span versus target 6,692 (`+20`) and a 7,040-byte
PDB contribution versus target 7,020. Its 59 physical opcode groups remain
in target order. A natural switch over the format conversion character
shortened the candidate by four bytes; the largest remaining local format
case gap is 15 bytes. It remains a mismatch with incomplete normalization.
All 13 exact `EclVm.cpp` units / 1,301 bytes across two artifacts replay after
the source change. Evidence is retained below `.analysis/gpt-5.6-sol/20260918-anm-exact/`
and `.analysis/gpt-5.6-sol/20260918-ecl-vm-exact/`.

## Completed packet: exact FileSystem close leaf and ECL ABI check

IDA attests the canonical target SHA-256 and confirms that
`FileSystem::CloseWriteFile @ 0x0044B7E0-0x0044B804` is a complete 37-byte
reviewed authored body with two direct replay-loader call sites. Natural source
in `src/FileSystem.cpp` reproduces every byte in a normal VC7.1 SP1 COFF
object. The canonical `file-system-close-write-file` unit declares all five
DIR32 fields: shared handle `0x00474C38`, `CloseHandle` IAT `0x004660CC`,
critical section `0x004922A4`, `LeaveCriticalSection` IAT `0x004660B4`, and
active-file count `0x0049231E`. Two independent cold builds and the focused
source replay pass with zero differences. Tracking is now 308 source mappings
and 162 exact functions / 22,450 bytes; all 1,317 tracked boundary and origin
reviews remain complete. Production TU/profile and global data ownership stay
unknown.

The requested generic ECL runner remains non-exact. A source experiment
removing its per-iteration null check changed the selected pre-table span from
6,712 to 6,736 bytes versus 6,692 target, so it was reverted. Giving
`EclVmStartSubroutine` internal linkage left its caller unchanged and made the
helper invisible to the current symbol diagnostic. A natural `__stdcall`
declaration removed caller cleanup but allocated the private destination in
ESI rather than target EAX, shortened `SpawnThread` to 140 versus 142 target,
and worsened the generic runner. That experiment was also reverted. Retained
focused diagnostics are under `.analysis/gpt-5.6-sol/20260918-ecl-vm-exact/`;
no exact ECL VM claim follows.

## Current bounded packet: three large exact frontiers

Enemy `DispatchEclInstruction @ 0x0040E770` now has candidate definitions for
the active-context argument wrappers observed in IDA. Replacing five natural
16-word zero loops with `memset` makes VC7.1 emit the target's `rep stosd`
form. A sequence of target-directed source case moves makes the linked
candidate's 108 physical jump destinations agree with the target in **all 108
positions**. The 181-byte selector is equal, and both contributions have the
same 43-byte suffix after it. With `src/EclVm.cpp` as `/GL` support and `/GS`,
the selected candidate is 14,532 bytes versus the 14,416-byte target, with
692/11,684 comparable bytes equal and complete normalization. The target
frame is still `0x2C4` versus candidate `0x2BC`; helper receiver registers
and individual case bodies remain different. No exact row is added. The next
bounded comparison is the first spawn case and its shared absolute-position
tail: physical order is now controlled, while those adjacent case spans
redistribute 133/139 bytes between target and candidate.

ANM `ExecuteScript @ 0x0043EE30` now has explicit common child tails for
88/90 and 91/92. Their first two candidate physical gaps are 44 and 47
bytes, exactly the target gaps, and all 92 physical groups stay in target
order. The focused selected-entry `/GL` contribution with `RandomMath.cpp`
support remains 9,784 bytes including its 376-byte table, and the pre-table
span remains 9,408 versus target 9,588. The target reserves `0xFC` stack
bytes versus candidate `0xD4`; investigate the missing 40 bytes of live
locals and later per-case gaps before making an exact claim.

Generic `EclVmContext::Run @ 0x0044E1A0` still gives a 7,040-byte candidate
against 7,020 target when selected as its own `/GL` link entry. The target
receives the context in EAX, while this candidate receives it in ECX; both
reserve `0x108` stack bytes. A 32-bit format-index type experiment did not
change the 20-byte span excess and was reverted. Use the host entry for
subsequent source probes so the private receiver convention is preserved.
IDA confirms both direct target callers are in `EclVmHost::Run @ 0x0044FD10`.
Selecting that real host as the link entry makes the candidate runner receive
its context in EAX too, while the contribution remains 7,040 bytes; this
isolates the remaining register problem to the runner's EBX/EBP coloring and
later body shape, rather than the private receiver convention itself.
The three owners remain non-exact. Focused diagnostics and intermediate
negative probes are below `.analysis/gpt-5.6-sol/20260918-enemy-order/`.
A focused cold replay of the two edited source files passed all 81 configured
exact units across 11 artifacts (16,534/16,534 bytes plus every declared
linkage field); it does not establish exactness for either large owner.
Target-independent CI and the required tracking/progress/toolchain checks pass.
After this checkpoint, moving the absolute-spawn source label next to its
switch cases left the Enemy candidate byte-identical at 14,532 bytes; that
experiment was reverted. In the real ECL host entry, swapping the runner's
two local declarations likewise left the 7,040-byte candidate unchanged.
Changing only its initial null guard from `*instructionCursor` to
`instruction` grew it to 7,056 bytes and reduced comparable byte agreement
from 2,281 to 460; it was reverted. Retained negative diagnostics are
`absolute-inline-label-probe.json`, `ecl-local-declaration-order-probe.json`,
and `ecl-direct-initial-guard-probe.json` in the same ignored analysis folder.

## Current continuation: projection ABI and ANM vector construction

IDA-attested target bytes show the Enemy dispatcher calls the imported
`D3DXVec3Project` thunk with six stack arguments and no caller cleanup. The
manual source prototype incorrectly used caller cleanup; it now declares
`__stdcall`. The target also writes all three projection-input coordinates
before `EnemyPrepareProjection`, so the source builds that vector before the
call. In the focused selected `/GL /GS` diagnostic with `EclVm.cpp` support,
these two corrections reduce the candidate from 14,532 to 14,508 bytes versus
14,416 target. All 108 jump destinations retain target physical order. The
candidate frame remains `0x2BC` versus `0x2C4` target, and the current
diagnostic cannot normalize all linked fields. Reordering the two large local
declarations and widening the temporary opcode type produced byte-identical
results, so those experiments were reverted.

The target ANM `POSITION` case evaluates Z, Y, X before copying a completed
three-word vector into the chosen VM position. The maintained field-by-field
X, Y, Z source has been replaced with the existing `AnmFloat3View` constructor
in each branch. This reproduces the local operand order and aggregate-copy
shape. The candidate's executable pre-table span becomes **9,584 versus 9,588
target** (`-4`, formerly `-180`), with all 92 physical case groups still in
target order. The selected PDB contribution is 9,960 bytes because it also
owns the 376-byte jump table; this should not be compared directly to the
9,587-byte target code-only ledger extent. The constructor also changes the
candidate frame from `0xD4` to the target's `0xFC`; the saved game-speed local
still has a different stack slot (`+0x60` candidate, `+0x98` target), and the
alternate-position block is placed before the interrupt-label block instead
of after it. Explicit shared-tail, label, and early-break variants did not
recover that local placement and were reverted. The early-break form increased
comparable byte agreement but widened the executable span deficit to 84 bytes.

For the generic ECL runner, a carried current-instruction local shortened the
host-entry candidate to 7,028 versus 7,020 target but changed the frame to
`0x10C` versus target `0x108` and left one undecoded suffix byte. An explicit
percent-case `continue` produced 7,048 bytes. Both experiments were reverted,
leaving the current 7,040-byte host-entry candidate. All three large owners
remain non-exact. Focused probe JSON and case layouts are retained under
`.analysis/gpt-5.6-sol/20260918-enemy-order/`.

A focused cold replay after the two source changes passes all 81 configured
exact units across 11 artifacts, 16,534/16,534 bytes and all declared fields.

## Current continuation: generic ECL format operand ordering

IDA target disassembly of `EclVmContext::Run` shows the format opcode compares
the operand metadata type before loading the raw operand, and reloads the
instruction through its context. The maintained inline helper now performs
those reads in the same order and accepts only the initial format cursor from
the outer `current` pointer. `flagIndex` is a 32-bit counter. In the real
`EclVmHost::Run` selected `/GL` diagnostic, this source gives a **7,020-byte
contribution and 6,692-byte pre-table span**, both exactly the target lengths;
all 59 opcode groups retain target physical order. The format case's local
span is 250 versus target 247, improved from 262. The candidate now assigns
EBX to the instruction cursor and EBP to zero at the prologue, as the target
does. The remaining frame differs (`0x104` candidate, `0x108` target), and
the full byte comparison and linked-field normalization fail. This is still
a non-exact owner. An explicit `continue` in the percent case grew the
candidate to 7,028 bytes and was reverted. Focused evidence is
`ecl-format-cursor-argument-{probe,layout}.json` and
`ecl-format-final-percent-continue-probe.json` below the same analysis folder.
A focused cold replay of `EclVm.cpp` passes all 13 configured exact units in
two artifacts, 1,301/1,301 bytes and all declared linked fields.

The equal-length ECL runner still has opposite 12-byte local gaps in float
comparison opcodes 0x3E and 0x44. In 0x3E the target's first float temporary
is at `[esp+0x100]`, causing long displacement encodings, while the candidate
uses `[esp+0x4C]`. In 0x44 the target uses short-offset slots `+0x7C/+0x74`
and the candidate uses `+0x80/+0x78`. Function-scope temporary and commutative
operand-name experiments compiled byte-identically and were reverted. The
stack-slot allocation remains an exact-codegen blocker.

## Current continuation: ECL loop entry and ANM advance placement

The IDA-attested target `EclVmContext::Run @ 0x0044E1A0` checks for a null
instruction only on entry. The maintained runner now omits the extra null
condition on each loop iteration. Its selected `EclVmHost::Run` `/GL` candidate
matches the target's entry and first selector instruction sequence apart from
the frame constant (`0x104` candidate, `0x108` target) and linked addresses.
The return-to-loop path now uses the target's `FILD/FCOMP` comparison. The
candidate is **7,036 versus 7,020 target bytes** and has 694/6,268 raw
comparable bytes equal, up from 591; the sum of physical case-gap differences
falls from 68 to 60. This is a better local codegen hypothesis, not an exact
match. The shared instruction advance still differs: target adds an ECX
offset to an EAX pointer before storing; candidate adds directly in memory
and reloads. Direct-member and explicit-local advance spellings and an
unsigned offset compiled identically; cursor-reload variants shortened the
whole candidate to 6,972 bytes and were reverted. Selected evidence is
`ecl-loop-final-{probe,layout}.json` and `ecl-loop-final.exe` under the
existing `.analysis/gpt-5.6-sol/20260918-enemy-order/` folder.

ANM POSITION/NOP advance placement remains open. Target puts the NOP advance
at `0x0043F532` between primary and alternate POSITION branches; the current
candidate puts its separate NOP advance after alternate POSITION. Two explicit
shared-label variants kept case order but reduced the candidate contribution
to 9,848 bytes and still did not move NOP before alternate; both were
reverted. Removing a redundant switch-exit `continue` changed no case
destinations. The selected ANM candidate remains 9,960 bytes including its
separate 376-byte table, with 9,584 versus 9,588 pre-table bytes, and is
non-exact. Enemy remains 14,508 versus 14,416 bytes and non-exact.

## Current continuation: ECL direct STORE operands

IDA and canonical disassembly show that runner opcodes 0x2B and 0x2D resolve
the destination pointer first, then inline `stack.Pop` to copy directly into
it. The old source popped into a temporary before resolving the destination.
Both cases now call `stack.Pop` with `ResolveInt(0)` or `ResolveFloat(0)` as the
output pointer. Their candidate physical spans match the target exactly:
80/80 and 86/86 bytes, with matching local instruction shapes apart from
linked addresses. All 59 opcode groups remain in target order. The selected
runner contribution becomes **7,032 versus 7,020 target bytes** and the sum
of absolute case-gap differences improves from 60 to 52. Its frame becomes
`0xFC` versus target `0x108`; other float case slots move and the complete
owner remains non-exact. Focused evidence is `ecl-direct-store-{probe,layout}.json`
and `ecl-direct-store.exe` in the same analysis folder.

The target `ReadInt @ 0x0044FDB0` uses EDX for its receiver and a stack
argument. The current linked probe specializes its candidate helper to EDX
plus ECX, which contributes to the opcode-0x14 call-shape gap. Recover this
helper/link context before treating nearby case lengths as exact evidence.

## Current continuation: ECL polar helper and angle calls

IDA establishes that the generic runner's 0x51 polar opcode calls a separate
30-byte helper at `0x004501B0`, and both polar and angle-normalization opcodes
call `EnemyWrapAngle @ 0x0044BC70`. The maintained source now names that exact
angle helper and implements `EclVmPolarVectorView::FromAngleMagnitude` with
the target's `FSINCOS` operation. Two independent cold linked PE builds in the
real `EclVmHost::Run` entry context reproduce all 30 bytes of the complete
PDB-owned helper, with no relocation fields. Its exact unit is
`ecl-vm-polar-from-angle-magnitude`; the replay receipts are
`ecl-polar-canonical-{1,2}.json` in the current analysis folder.

Nested argument evaluation in 0x51 restores target-sized physical opcode
spans of 97/97 for polar conversion and 36/36 for angle normalization. All 59
runner opcode groups stay in target physical order, but the whole contribution
is 7,032 candidate versus 7,020 target bytes, with a `0x104` versus `0x108`
frame. The generic runner is therefore still non-exact. The selected probe
and layout are `ecl-polar-nested-call-{probe,layout}.json`; remaining local
gaps include the paired integer/float arithmetic cases, 0x34 and float
comparison slots. Enemy dispatcher remains 14,508 versus 14,416 bytes, and
the ANM script executor remains a 9,960-byte contribution including its
376-byte table versus the target's 9,964-byte owner. Neither is exact.

The focused `src/EclVm.cpp` cold replay passes all 14 exact units across
three artifacts, 1,331/1,331 bytes and every declared linkage field. Its
receipt is `ecl-polar-focused-exact-replay.json` in the same analysis folder.

## Current continuation: ECL polar local width

The target 0x51 call stores its polar result in adjacent stack floats, and
the byte-identical ANM helper operates on a three-float vector. The ECL view
now also has three floats, asserted at 12 bytes. In a focused real-host `/GL`
diagnostic, this changes the runner's frame from `0x104` to the target's
`0x108` and moves the first raw byte difference from offset `0x2` to `0x32`.
Its complete contribution remains **7,032 versus 7,020 target bytes**. The
polar case remains 97/97 bytes, all 59 opcode groups stay in target order,
and a cold exact replay of the 30-byte helper still passes. The third float's
runtime meaning is not yet established. Evidence is
`ecl-polar-three-component-{probe,layout}.json` and
`ecl-polar-three-component-helper-replay.json` in the same analysis folder.
The full focused `src/EclVm.cpp` cold replay then passes all 14 exact units
across three artifacts, 1,331/1,331 bytes and every declared field; receipt
`ecl-polar-three-component-focused-replay.json`.

The next runner gaps remain the paired arithmetic temporary placements and
the private `ReadInt` call ABI. Target `ReadInt @ 0x0044FDB0` is 144 bytes with
an EDX receiver and stack index; the selected candidate is 122 bytes with an
EDX receiver and ECX index. Prologue and return paths differ, so neither this
helper nor the caller should be marked exact by case length alone.

Target `ReadInt` keeps its input index in EAX across an empty-stack typed pop,
returning that index on underflow. The maintained pop destination is now
initialized from the index. In the real-host linked probe this corrects the
return-path semantics and changes the candidate helper from 122 to 117 bytes;
the target is 144 bytes. The private EDX-plus-ECX versus EDX-plus-stack index
ABI difference remains, as does the 7,032 versus 7,020-byte runner gap. The
focused diagnostic and opcode layout are
`ecl-readint-init-index-{probe,layout}.json` in the analysis folder.
The subsequent focused `src/EclVm.cpp` cold replay passes all 14 exact units,
1,331/1,331 bytes and every declared linkage field; receipt
`ecl-readint-init-index-focused-replay.json`.

## Current continuation: linked context and ANM tail audit

The repository remains source-clean after this bounded probe batch. IDA still
attests the original SHA-256 target. For the ECL runner, removing Enemy support
from the host-entry `/GL` probe or selecting Enemy as the entry with ECL support
leaves `Run` at 7,032 bytes and `ReadInt` at 117 bytes; target sizes are 7,020
and 144. Target opcode 0x14 calls `ReadInt` with a stack index and occupies 51
bytes; the candidate passes an ECX index and occupies 57. The full private ABI
and caller register allocation remain open.

For Enemy, the target entry has an EBP-aligned `0x2C4` frame and security
cookie. The matching compiler profile is `/GS`: selected-entry, ECL-supported
codegen reproduces the earlier 14,508-byte candidate, physical case order and
181-byte selector against the 14,416-byte target. Without `/GS`, the candidate
shrinks to 12,988 bytes with ECL support or 12,692 without it; those are
profile diagnostics only. The `/GS` candidate frame is `0x2BC`. No-inline
`ReadInt` changes neither candidate, and a 32-bit opcode local grows the Enemy
candidate to 14,516 bytes. Both source experiments were reverted.

For ANM `ExecuteScript`, the saved-speed declaration-order experiment compiled
byte-identically to the current 9,960-byte candidate. Inverting POSITION's
condition changed only three bytes and changed the branch sense away from the
target, so it was reverted. The target's float-jump opcode 39 body ends at
`0x00440DFC`, followed by END return and then the stop-update block at
`0x00440E2B`. The candidate puts the first stop-update block at `0x00407EA6`
before END at `0x00407EFD`, widening that physical interval from 105 to 192
bytes. Its complete contribution remains 9,960 bytes, with a 9,584-byte
pre-table span versus target 9,588. `FindVm`/`GetVm` and child Variant3 retain
the private ABI mismatches recorded in `ANM-041`; their exactness is unknown.

Probe receipts are under `.analysis/gpt-5.6-sol/20260918-ecl-abi/`, with the
prior selected ANM candidate and layout in the neighboring
`20260918-enemy-order/` folder. The next actionable source work is to recover
the target register and local-slot live ranges that select the private helper
ABIs and stop-tail placement. Do not promote any of the three large owners by
size, table order, or source coverage alone.

The ANM float-jump operand slots further isolate the allocator issue: for
opcodes 33/35/37, target offsets `+0x84/+0x8C/+0x94` each exceed candidate
`+0x4C/+0x54/+0x5C` by `0x38`, matching the saved-speed slot difference;
opcode 39 instead uses `+0x2C` versus `+0x24`. A single shared float
temporary would erase target-observed distinct slots, so the next source
experiment should preserve each case's independent operand lifetime.
