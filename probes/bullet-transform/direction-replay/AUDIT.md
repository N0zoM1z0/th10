# Angle NaN audit: conditional findings and remaining obligations

A source-relevant conditional difference is established for a concrete valid entry domain. The host source oracle sends payload.float0 NaN to player aim; the bounded target proof instead stores a NaN classification to direction value1 without calling player aim along the proved path. This is not an all-state or compiled-equivalence claim, and no production edit is included.

## Evidence already established

- `repository-command:94d9c15e76264469bed8cd8d9aaff698`: both identified angle-call predecessor branches select the other immediate successor when their FCOMP result is unordered. Neither selects its two-instruction direct angle-call preparation path. Reviewed verifier SHA256 `5b013673bb97fc615169dfd67777ba1f02bfd6bce73fbb4a7b81717fa0165dec`.
- `repository-command:139f880b5b8a432b9e0cdf6d5b8b0a47`: one direction-result site passes a path-conditional NaN reload/store proof. The selected path reloads the same stable effective address as the FLD immediately preceding its comparison, then stores a NaN classification to the conditional receiver's direction value1 field without an intervening angle call. No modeled arithmetic step was needed. Reviewed verifier SHA256 `9e998577520ebbef9e25282f0d3d313b25f65cdb1ac554703e281d5a3bf83cf6`.
- `repository-command:40a76b7c4e6d4522870bfeeed9c3ae91`: the immediately earlier direction guard permits the later comparison on unordered input; the shared source matches the earlier FLD address. Reviewed verifier SHA256 `25bb8f4ac54e8c7bd09bb23b679fa5e7fc2a0fd3dae4e5acb75e5cbde4c01c6d`. This still assumes execution reaches that earlier guard and does not identify the current transform record.
- `repository-command:7d1081c2028f47c68c659e6e6b3d1ab9`: the same stable source composes across the earlier guard, later comparison, and final reload/store.
- `repository-command:1851bef677054be58c50ee04106b82e4`: the source address is receiver+0x464+24×cursor modulo 32 bits, and cursor memory is unchanged from its identified load to the earlier guard. All affine/freshness/current-record predicates pass, with 29 checked instruction occurrences. Reviewed verifier SHA256 `17d905b69300cc4c521af6112e0de18c65a4670d3c24dab3743e8b48d27ec3e2`. Under the valid-object/index domain, this establishes payload.float0 identity and composes its NaN to the direction value1 store when that guard is reached.
- `repository-command:60bd48b13ede4900971ac1f79441ba59`: entry witnesses for kinds 0x40, 0x80, and 0x100 reach the already verified earlier direction guard in 40, 40, and 35 modeled instructions respectively. The combined concrete-domain NaN-store predicate passes. Executed verifier SHA256 `581eb1fdb1e4e2068c901bfea6d402c9aca1bde5f27d48a2192596e5e90da671`.
- The vector site remains unresolved at an unsupported floating-operation shape. It is not evidence of a different source outcome.

The status proof uses the exact x87 unordered C3/C2/C0 pattern, preserves all unknown status bits, and checks the status-word-to-AX, TEST AH, and conditional-branch effects. Exact predecessor requirements prevent a bypass. The path-specific store theorem does not require other incoming merge paths to carry equal values.

## Premises

Pinned immutable owner code/tables; ordinary execution; valid x87 stack and readable/writable flat32 memory; floating-point exceptions masked so the comparisons and stores complete. Shared source memory must remain stable across the entire composed earlier-guard-to-final-store path, with no asynchronous or volatile/device-memory mutation. Conditional receiver roles assume ordinary nonvolatile-register preservation by opaque calls, not memory preservation or verified private-helper ABI contracts.

NaN classification is proved only where a gate passes. Signaling status and bitwise NaN payload identity are not preserved claims.

## Concrete domain and limits

Entry ECX is the valid receiver; cursor is zero; active flags and record[0].allowWhileActive are zero; record[0].kind is separately 0x40, 0x80, or 0x100; payload.float0 is a stable NaN. The private valid stack has an 8-byte-aligned entry pointer and is disjoint from the receiver, code, and tables. Its return cell stays opaque. The verified stack-alignment operation preserves this rooted address model; address-dependent flags are not guessed.

The entry witness begins at the actual owner entry, rejects calls and repeated instruction addresses, and reaches the exact earlier guard used by the independently composed NaN proof. Unknown control inputs, unsupported operations, and unmodeled aliasing writes fail closed. Thus the earlier reachability obligation is closed for this fixed index-zero domain. The existing output labels saying dispatch/action-domain reachability is unproved or complete behavior is false are inherited broad limitations: the added concrete-domain witness predicate is true; universal reachability is still unproved.

Other indices, active flags, allowWhileActive states, vector behavior, and unmasked floating exceptions are outside this result. No threshold constant values were read. The source's unordered comparison behavior is distinct from this target path, but a proposed source correction and its wider semantic tests are a separate deliverable.

The address proof uses exact MOV/LEA reaching definitions with receiver origins separately conditional on ordinary nonvolatile-register preservation. Relevant dependency paths are acyclic after cutting at one cursor-load event; saved prior-iteration values cannot be merged solely by static load-site identity. Freshness rejects unmodeled writes and calls. The only accepted non-stack field write on the observed slice is an exact proven-receiver flags+0x43C write, disjoint from cursor+0x45C under the valid-object premise.

Source layout is pinned: payload.float0 is record+0; record+0x10 is kind. No threshold constant values have been read. The concrete entry witness connects this address and NaN-store theorem for the stated three direction kinds; it does not authorize a broad source rewrite.

## Separate rejected native experiment

Expanding the authored angle helper inline at its two source sites produced a 1596-byte candidate owner versus the unchanged-source 1476-byte baseline and 1454-byte target. The exact-885 support guard still passed. The overlay was not adopted. This was a source-ownership/code-generation experiment, not a target behavior correction or exactness result.
