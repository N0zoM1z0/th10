# Conditional local-order implementation

The approved metadata-only script and actual receipt remain unchanged. `op82_local_order.py` is a separately reviewable derivative, prepared with parent approval after receipt 4f7693c270c04d2aa111b29d0ae7599a. It has not run on the target.

Exact theorem scope: **if execution starts at the entry obtained from the verified selector table for 0x52, code/table bytes remain immutable throughout modeled execution (including during opaque helpers), all three known helper calls return normally, and non-call instructions follow ordinary normal completion with no exception or nonlocal transfer**, determine call ordering along every modeled path until its first direct outbound edge from the same previously authorized lexical interval. All syntactic CFG branches, including potentially infeasible branches, are conservatively included. This is a conditional entry-to-escape statement, not a proof that runtime opcode 0x52 dispatches there, that an escape reaches common instruction advancement, or that complete opcode behavior is equivalent.

No continuation or dispatcher-provenance expansion occurs. The same whole-owner identity/mapping, exact helper identities, source/config/decoder pins, clean HEAD pre/post checks, resource limits, prefix/case decode bounds and fixed output ceiling apply. Table and owner addresses stay internal. The previously executed metadata script is preserved byte-for-byte.

The case decoder now returns its internal instruction map, successor graph and unordered direct/fallthrough escape facts in memory only. Conditional order analysis runs only after every repository/source/tool/target postcheck has succeeded. It requires complete metadata traversal, no indirect/return/fallthrough exits, at least one direct escape, and exact five-byte near direct calls with the E8 encoding, CALL identity, one four-byte immediate operand and one of the three pinned helper targets. Unexpected calls or control forms reject proof.

Finite call-state traversal uses states `(instruction, sequence of known helper identities)` internally, capped at 256 distinct processed states. It propagates both conditional successors, checks every direct escape, rejects repeated/missing calls, and requires exactly one each of ResolveFloat, ReadFloat and EnemyWrapAngle on every exit. Decoder instruction/edge/state bounds remain case128/256/256 and prefix256/512/512. Loops and missing/overlapping decode reject metadata first. No lexical address sort is substituted for path analysis.

Only fixed Boolean local results and bounded state/exit counts are added to the existing Boolean/count/hash schema:

- local_order_resolved
- local_destination_before_read / local_read_before_destination
- local_read_before_wrap / local_wrap_before_read
- local_reference_consistent (all paths match upstream's destination-before-read-before-wrap statements)
- conditional_entry_assumption = true
- immutable_code_and_tables_assumed = true
- ordinary_noncall_completion_assumed = true
- no_exception_or_nonlocal_transfer_assumed = true
- infeasible_branches_conservatively_included = true
- full_opcode_semantics_proved = false
- local_state_budget_exhausted, local_control_valid, local_one_each_every_exit, local_nonvacuous_exits
- local_state_count, local_exit_state_count

Mixed path order leaves local_order_resolved false. Both directions are computed without assuming upstream is correct. A failure clears directional/resolution/reference proof flags. The original global order_resolved, dispatch_association_valid, continuation_valid and both global ordering flags remain false, and unresolved remains true. The result cannot authorize a source edit, build, or exactness claim. Actual maintained Enemy destination resolution remains pure for valid float destinations; no actual behavior bug is asserted.

Synthetic tests rerun all 38 predecessor tests against the derivative, plus all six helper permutations, missing/duplicate/far/wrong-opcode/wrong-width calls, vacuous/return/fallthrough exits, uniform and mixed branches, bypass, loop, unknown call, decode failure and local state-budget mutations. No target data, native trace, private byte fixture or target execution is used in those tests.
