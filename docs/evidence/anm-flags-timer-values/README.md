# ANM shared flags and timer value controls

2026-10-23, starting ad6e956. Four actual-source controls preserve the complete
maintained ANM/ECL/Enemy output after independently verified address replay.
No source, ABI, profile, canonical recipe or exact ledger change is adopted.

| Source control | Complete peer result |
| --- | --- |
| Loop-scoped unsigned flag accumulator used by both flips | All three complete raw bodies equal baseline |
| Function-scoped accumulator used by both flips | All three complete raw bodies equal baseline |
| Scale-first float-by-value helper for final slow Tick addition | ANM 291 and Enemy 17 changed address fields; zero ordinary differences |
| Frame-first parameters for the same actual addition | Same field-replay outcome; zero ordinary differences |

Both accumulator controls retain the target's scale multiplication. They change
actual def/use relationships; they neither reserve an unused register nor add
padding. The two timer controls expand only ExecuteScript's final Tick and use
both float parameters in a real addition. Original speed thresholds, previous
value, fast path, conversion, restoration and return are preserved. Explicit
source-shape assertions and the adjacent patches verify what was compiled.

Every complete ExecuteScript remains 9964 bytes, with case diagnosis7541/7995,
90/92 same-sized groups and gap2. FlipX remains53 versus52, FlipY52/52 and the
common case-2 span1440 versus1441. Fresh Ghidra confirms target FlipX uses EAX's
shorter XOR encoding; skipped alignment fill accounts for the common span's
one-byte size difference. None of these controls fixes either discrepancy or
the slow timer's load order. Address replay matches the complete field inventory
and requires qualified map/static symbols or equal owned offsets. No field is
unresolved and no ordinary peer byte remains; this is not target exactness.

All four serial nice10 builds compile only ANM, reusing nine hash-verified game
objects and two harness inputs under unchanged /GL and /GS. Each passes36 whole
guards4618/all121 plus eight bounded child-return paths. ECL remains7020/6993,
all189 fields and the full171-byte float callee exact. Disk target, executing
pinned toolchain, tracking/status and direct readOnly/noanalysis Ghidra pass;
all412289 mapped bytes retain cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
Ghidra uses rollback, maxcpu1/heap768m. No full cold pair, full CI or runtime proof.

[Evidence](evidence.json) binds source/producer hashes, shape assertions, peer
replay and exact guard scope. Private inputs/full reports remain under
.analysis/gpt-6.1-sol/anm-flags-timer-values-20261023. All64 new native files were
hash-inventoried and removed, reclaiming4267615 bytes. Current source, canonical
artifacts and nine unrelated D3D inputs remain unchanged. No new exact credit;
continue large-owner work with a different compiler/context hypothesis.
