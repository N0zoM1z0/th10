# TH10 ECL SCPT AddScriptData indexed file slots: native word-address recovery

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original `EclVmScriptDatabase::AddScriptData` is a complete **589-byte** owner.
The current VC7.1 SP1 original graph emits the same **589-byte** PDB owner
with two candidate call fields and still compares **502/581 nonfield
ordinary original bytes**, **79 mismatches**. This is **NONEXACT**.

A counted SCPT package supplies a `fileCount` used to index the database's
file-header pointer table. The original i386 computes each slot address
as `base + fileCount * 4` modulo 2^32. Before this change the C++ used
`files[fileCount]`, which can invoke undefined out-of-array pointer
arithmetic if a corrupt SCPT count is out of range. The new
`EclFileSlotAtWord` helper computes the native low 32-bit address first,
then performs the typed pointer read/write. All eight actual uses in
`AddScriptData` now use that helper, including initial publication,
header/metadata fetches, sorting counts and the final result-index
read. It does **not** add a range check or assert malformed scripts are safe.

The current pinned two-TU ECL/Enemy native compile independently established
that this change preserves the **complete 589-byte** AddScriptData candidate,
its two candidate call fields, and the existing 79 original ordinary
mismatches. The related `FindSubroutine` is still **136/136 exact**.
The full `EclVmContext::Run` retains **6,997/7,020** original bytes,
all **189 independently original-bound call/data fields**, and its exact
`ReadFloatValue` callee. Neither large owner is promoted to exact.

Four selected existing ECL exact units cold-replayed against the **actual
installed source**, passing **575/575** original bytes. No all-unit
historical cold replay or whole-game runtime proof is claimed.

Reproduction and current source-hash receipts are ignored developer
artifacts under `.analysis/gpt-web/20261107-turn4/`:
`ecl-file-words.py`, `ecl-count-results.json`,
`ecl-files-installed-proof.json`, `ecl-files-installed-run.json`,
`ecl-files-guards.json`, alongside linked PE/PDB binaries in `build/gpt-web`.
