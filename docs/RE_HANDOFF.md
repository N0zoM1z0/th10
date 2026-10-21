# TH10 reconstruction handoff

Updated2026-10-22. Exact reconstruction remains **active-incomplete**. Current
bounded work recovers both Chain frame dispatchers and their owned switch tables,
with source and actual-source host contracts. It adds no exact functions/bytes.

## Recovery and operator preferences

- Only ignored `resources/th10.exe`, original Japanese v1.00a, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
  inspect Git status/diffs and recover dirty work before editing.
- Preserve the nine independent untracked `docs/evidence/d3d-adapter-format/`
  files; never stage or clean them as part of this batch.
- Repository Python uses `scripts/repo-python`. Operator preference: direct
  local tools, no Factory MCP; native builds serial nice10, Ghidra readOnly/
  noanalysis/rollback, one CPU,768 MiB heap.
- Full mapped text401000..465A80 inclusive is412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  Older64A80-byte hashes cover a truncated prefix.
- Authorized English `gpt-6.1-sol: ...` checkpoints are committed and pushed
  to `origin/main` after validation. Replay only necessary affected units.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current source and metrics

[Generated progress](PROGRESS.md):1776 reviewed candidates,734 authored functions/
273579 owned bytes;1094 total source mappings. Authored source610/247329 bytes,
213 source-present/nonexact,124 source-absent. Authored exact **397/72387 bytes**,
26.46% of the provisional byte denominator; overall exact1238/138897 bytes.
There are524 indeterminate origins, so these are not whole-game completion rates.

[Dispatcher evidence](evidence/chain-dispatchers/README.md) recovers MainRunCalcChain
at449C00 and MainRunDrawChain at449D40. Both stack-owner/RET4 entries return their
count in EAX. Main's declarations now use int __stdcall, including drawing's
previously erased return. Original source names/annotation/method/TU remain unknown.

The old273/241-byte RET-only spans omitted owned suffixes. Calc has3 alignment
bytes and eight table words at449D14: complete308 through449D33. Draw has3 and six
words at449E34: complete268 through449E4B. Unsigned guards, local destinations,
sole owner xrefs and adjacent CC padding establish ownership. Target PE relocations
are absent. The62-byte denominator correction adds no exact credit. Function
count/origins remain unchanged; two authored source mappings are added.

Both cache next before the callback. Null callbacks are uncounted; nonnull but
disabled(mask0x2, bit1) callbacks count without executing. The callback runs unlocked
with ECX argument. Status0 unregisters only;2 retries with enable-mask recheck;
3/4/5 force1/0/-1 and break. Calc6 resets/restarts;7 calls the optional locked
delete callback without implicit unregister. Draw6/7 and out-of-range values
are defaults. No invented safe invalid-pointer paths or asynchronous lifetime
closure follow.

The prior [construction review](evidence/chain-source-closure/README.md) establishes
two0x24-byte heads in a0x48-byte owner, ownership-bit-only initialization and
untouched argument words. Constructor449AA0/59 remains origin-indeterminate:
implicit versus explicit outer construction is unresolved. Its earlier59-byte
denominator removal and this62-byte table addition are separate corrections.
The491BE4 g_MainCallbackOwner/g_Chain physical alias/data owner remains open.

## Native and host validation

Two actual-source full14-TU cold groups preserve four existing exact helpers/
282 bytes: Unregister114, RemoveAll82, RegisterCalc43 and RegisterDraw43, with
complete field replay. No all-consumer or full reconstruction replay is claimed.
The new runners remain nonexact in that graph: Calc316/308, Draw272/268.
A separate full Main/Chain/GameError3-TU diagnostic gives428/272 and inlines Calc's
unregister path. Both source visibility and flags differ between contexts, so
cause/original production partition is unknown. No masked ordinary-byte score.

Two controls each recompile one Chain object and reuse13 verified objects, not
full colds. Postfix counter spelling is raw-neutral for both whole bodies.
Case4-before3 changes two constants and two table fields per body, while size/raw
scores stay equal. The latter is not raw-neutral; neither control is adopted.

The existing [large32-input/15-artifact pins](evidence/chain-source-closure/native-summary.json)
remain current after fresh hash checks: both changed CPPs are outside that graph
and Chain.hpp is unchanged. There is no new ten-TU cold,35-guard replay or Run
measurement. Older raw-NEG/thread contexts retain their historical meanings.

Actual-source host Calc replaces the transition subset model in lifetime0/7.
The obsolete subset-only status gate is superseded by actual count assertions.
Full sanitized1375 checks cover existing effect/constructor cases plus42 new
Calc/Draw calls with literal status/flag/count/lock/restart/cache expectations.
Eight strict faulty-code controls and19 EOL guards pass. Only one active strict
LF dependency pin, Chain.cpp, changes; other six and normalization stay strict.
The new reusable runner adds one CI group. Host mutex/allocation/renderer roles
do not establish native ABI, Windows concurrency or whole-game runtime.

All108 fresh full CI groups pass; all368 frozen source/test/config raw inputs
remain unchanged through CI. Classified cleanup removes15845902 logical file
bytes from nine owned reproducible trees and one superseded dump. Current
carriers, one failed control and compact evidence remain; provider/legacy
state and nine independent D3D files are untouched. Final scope/results belong in
[validation](evidence/chain-dispatchers/validation-summary.json).
Private recovery: `.analysis/gpt-6.1-sol/chain-dispatchers-20261022/RECOVERY.md` and
manifest.json. The initial diagnostic carrier uses a prior build prefix from
its template; its actual path is recorded and no old path was overwritten.
Do not blindly rerun old producers or --existing recovery paths.

Fresh global boundary audit still fails first at401100 metadata provenance:
1116 rows (previous1114),49 state and107 confidence differences, zero geometry
differences. Two new manual table notes add two metadata differences. Unowned
indirect tables18->16. Ordinary tracking/CI success does not waive this audit.

## Large owners and next route

| Owner / target | Current retained measurement | Remaining work |
| --- | --- | --- |
|ANM43EE30 /9964|9964 bytes;7541/7995 aligned positions;90/92 target-sized groups; aggregate gap2|Child91, temporaries49/53/54, FlipX61, common-tail2 and Tick order|
|ECL44E1A0 /7020|7020/6993,27 differences;189 fields/62 locals;171-byte float callee exact|Thread argument, SIB, homes and latch|
|Enemy40E770 /14416|14352 bytes; frame2C4;181 selectors/108 groups;42 target-sized groups; aggregate gap566|Laser ABI, shared tails and packet/register/stack lifetime|

Run still uses strict60-table-word/88-selector annotations despite the generic
normalization flag at its final selector byte. Older two-TU26 is historical.
Do not repeat the11 rejected thread controls or the two new dispatcher spellings
as fresh hypotheses. Current dispatcher codegen route is the callback-adjacent
byte decrement/import scheduling: target direct DEC differs from the native
load/decrement/store and changes short/long branch layout. Investigate wrappers,
types or optimizer context with target/compiler evidence, not volatile/padding
tricks or fabricated ABI. Remaining Chain ownership/release inventory also needs
review; this batch does not establish complete production ownership.

Keep boundary, origin, source, exactness, native product and runtime separate.
Whole Windows i386 build/runtime closure remains open: globals, static init,
resources, libraries, link order and exercised paths need audit. Semantic
reconstruction and portability have not started. The broad goal remains active.
