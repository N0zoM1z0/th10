# TH10 bounded sound-caller audit

## Scope and checkpoint

Owner: BulletRuntimeView::AdvanceTransformProgram, 0x406D90, 1454 bytes.
HEAD: 75536f63312688610002392067ab8604cd33844b.
BulletTransform.cpp SHA256: 72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1.
The verifier independently pins the PE, owner, maintained ledger, header, decoder
helper and exact 20-byte pointer/128-byte remap tables. No sound-callee body or
new floating constants are read. Output is fixed predicates/counts/hashes only.

## Established caller evidence

- Actual 22cfbb5add9248e7947d29974b0783e6: two direct QueueSoundSample calls
  (0x43DC90), one direct QueueSoundCue call (0x43DD10); maintained names/extents
  checked against the current pinned ledger.
- Actual 81bc482182c642a7a55caa61c8d2f9b1: both Sample preparation corridors set
  ECX to direct sound storage 0x492590, push one integer-zero word, and retain EDI.
  Cue sets ESI to that storage, carries an actual record-offset8 dword read in EBX,
  and pushes a word read from bullet X at +0x3B4. Bullet receiver provenance is
  conditional on ordinary nonvolatile-register preservation across opaque calls.
- Actual 0331816d58e84464b04f13c4772f43b2: both Sample EDI values come from live
  receiver+0x458, with signed sound>=0 and live cursor!=0 gates on the bounded
  reaching predecessor chains. Cue's int0 address is receiver+0x464+24*cursor+8;
  live-current-record freshness passes across 23 checked instruction occurrences.
- Actual c7bd98743df34eb1b5980b3a22be4da3: the concrete Cue witness reaches its
  actual call entry in 38 modeled instructions. Each Sample witness reaches its
  exact flags-OR initializer instruction in 37 instructions. Each initializer is
  required by all bounded predecessor paths of one Sample call site.

The other cross-site query stops at the unchanged proof budget for kind0x10 and
at a cycle for kind0x20. These are unresolved queries, not proofs of absence.
Actual f2a8ea6629ab4a28a994ce72734c0b7e (final packaged collector) proves that
each kind has exactly one successful site, the successful sets are disjoint, and
their union of two sites covers both Sample calls. The Cue38/Sample37 entry
counts and prior field predicates remain positive. Cross-query limits remain
unresolved and are not enlarged or discarded.

## Domains and interpretation

Cue witness: entry ECX is a valid bullet receiver, cursor0, active flags0,
record0 kind0x20000, allow0, int0=7, float0=1.0, sound7, X=-123.75.
Sample initializer witnesses: cursor1 and record1 kind0x10 or0x20, otherwise the
same chosen inputs. All use a valid private eight-byte-aligned entry stack,
opaque return cell, and private stack/return-cell storage disjoint from the
receiver (including its records), code and tables; required object fields have
the stated nonoverlapping extents. Memory is
readable, ordinary flat32 and stable within the explicitly modeled intervals;
there is no asynchronous or device-memory mutation. Valid record indices lie
in [0,17], and address arithmetic must stay within the object.

Opaque calls preserve ordinary nonvolatile GPR values only where explicitly
assumed for receiver provenance. No helper memory preservation is assumed.
Sample sound/guard snapshots are established after the nearest opaque-call
boundary. Protected PUSH and CALL return cells cannot alias observed inputs.

Sample assignment means a particular initializer instruction is required before
a call site. Its rooted receiver role is established on the explicit entry
witness, not universally for every dynamic execution. This does not prove the
call occurs, all initialization statements execute, or helpers leave fields
unchanged. Cue stops before CALL. General all-index/all-state reachability,
callee consumption/private ABI, RET4, actual clobbers and continuation remain
separate obligations. Historical maintained ABI notes are not fresh callee replay.

These results support source-interface hypotheses; they confer no byte-exactness
credit, production adoption, compiler-boundary change or full-owner equivalence.
