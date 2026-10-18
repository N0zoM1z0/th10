# Vector-angle NaN: bounded entry-to-result-store evidence

The target has a proved path prefix for a concrete valid vector-action input: it stores a NaN classification to the receiver's vector value1 field without an intervening player-aim call. The maintained source at the pinned checkpoint instead selects player aim for a NaN requested vector angle. This supports a narrowly scoped source-comparison correction proposal. It does not establish full-owner return-state equivalence or authorize a compiler-layout change.

## Executed evidence

Final receipt: `repository-command:6e7a037fdda14bc9914a41683e8dc1ec`.
Executed standalone verifier SHA256: `4ae35880fd3de203cff1af4bd1d93841c7c1c0b89f4390495335633e354519c3`.

All vector prefix stages passed:

- Earlier unordered guard permits the later comparison, and the same stable NaN source composes through its alternate reload to the result store.
- Source address is receiver + 0x464 + 24 × cursor + 4 modulo 32 bits. Under the valid-object/index domain, this identifies current-record payload.float1.
- Cursor freshness passes 28 checked instruction occurrences. One exact full-dword MOV targets independently proven receiver + 0x65C, the separate vector value0 field.
- A concrete owner-entry witness reaches the earlier guard in 40 modeled instructions.
- The selected path performs one protected register PUSH, then FSTP m32 to independently proven receiver + 0x660. It needs no modeled floating arithmetic step and makes no player-aim call before this result-field store.

The earlier first-store extension allowed either FST or FSTP. Actual evidence selected FSTP; no FST occurrence is claimed. The result is the first result-field store, not the first memory write: the protected PUSH writes a private stack cell first.

## Explicit domain and premises

Entry ECX names the valid receiver; cursor is zero; active flags and record[0].allowWhileActive are zero; record[0].kind is 0x10. Payload.float0 has finite float32 value 1.0 (raw bits 0x3F800000), and payload.float1 is a stable NaN. The private valid stack has an 8-byte-aligned entry pointer and an opaque return cell. Address arithmetic does not wrap outside valid storage.

Owner code and the two authorized dispatch tables are immutable. Execution uses ordinary flat32 memory, a valid x87 stack, and masked floating exceptions. Shared source memory stays stable across the entire composed earlier-guard-to-result-store path. No asynchronous, volatile, or device-memory mutation is assumed. The protected PUSH cell is disjoint from the receiver, matched float source, and code/tables. Conditional receiver origins use ordinary nonvolatile-register preservation by opaque calls; no private-helper memory-preservation contract is inferred.

The source layout is independently pinned: record stride 0x18, payload.float1 at +4, kind at +0x10, exStates at receiver +0x614, state stride 0x34, and value0 at state +0x14, deriving vector value0 +0x65C. Field +0x65C is disjoint from cursor +0x45C and the selected record input. Unknown aliases or writes do not receive a blanket exemption.

## Limits

This is one fixed index-zero, inactive-domain entry witness and a path-prefix/first-result-store theorem. General all-index/all-state dispatch, subsequent vector continuation, field retention through return, and full function equivalence remain unproved. NaN classification is established, not signaling state or bitwise NaN payload identity. Unmasked exceptions are outside the proof.

No target floating threshold constants were read. Ordered threshold identity remains a maintained source assumption. A source comparator correction can preserve all ordered source inputs while changing NaN handling; its implementation, host tests, and any native experiment are separate work. No production edit or native build is included in this package.

## Replay identity

The collector intentionally retains the exact executed checkpoint pins: clean HEAD `1ace9bb5c34811a6d5911864cbe75599b47966f2`, BulletTransform.cpp SHA256 `b635d17c1f7bc76d3af8fc7b9fd4453f6c32b0e1f90fd6f718a4e6a7460516c0`, unchanged header and target/tool pins. Later commits must not silently repin this historical receipt-bound artifact.
