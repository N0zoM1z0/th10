# TH10 reconstruction handoff

Updated2026-10-22. Exact reconstruction remains **active-incomplete**. Current
batch closes both Chain frame dispatchers in the canonical linked-PE Oracle:
**308/268 complete owned bytes,44 fields, two independent actual-source colds**.
New credit is two authored functions/576 bytes; product/runtime remains open.

## Resume safely

- Original Japanese v1.00a only, ignored `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
  inspect status/diffs and recover dirty work before editing.
- Preserve nine independent untracked `docs/evidence/d3d-adapter-format/` files.
  Their bytes are checked; never stage or clean them with this batch.
- Use `scripts/repo-python`. Operator preference: direct local tools, no Factory
  MCP; native builds serial nice10; Ghidra readOnly/noanalysis/rollback, one CPU,
  768 MiB heap. No subagents without explicit authorization.
- Full mapped text401000..465A80 inclusive is412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  Older64A80-byte hashes cover only a truncated prefix.
- Authorized English `gpt-6.1-sol: ...` checkpoints are committed and pushed to
  `origin/main` after validation. Replay only necessary affected units.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current source and exact progress

[Generated progress](PROGRESS.md):1776 reviewed candidates,734 authored functions/
273579 owned bytes;1094 source mappings. Authored source610/247329 bytes,
211 source-present/nonexact,124 source-absent. Authored exact **399/72963 bytes**,
26.67% of provisional authored bytes; overall exact1240/139473 bytes. Unknown
origins524 keep the whole-game denominator unresolved.

The [native dispatcher proof](evidence/chain-dispatcher-native/README.md) closes
Calc449C00..449D33/308 and Draw449D40..449E4B/268, including all internal/post-RET
alignment and eight/six guarded local table words. The earlier62-byte table
ownership correction is separate from this576-byte exact numerator gain.
Physical stack-owner/RET4 and EAX count remain; original source annotation,
free-function/method spelling, names, classes, physical TU and flags are unknown.

Natural source reuses existing MainSupervisor CS[0]/unsigned lockCounts[0] at
base491C28+64C/+6F4, the known492274/49231C pair, and places case4 before3.
No new global definition, counter type, volatile, target-byte copy, fake return,
ABI or inert padding is introduced. Other Chain helpers retain prior scalar
aliases. This exact codegen does not close their source storage/data ownership.

Both runners cache next before callbacks; null callbacks are uncounted, disabled
nonnull callbacks(mask0x2, bit1) counted. Callbacks run unlocked with ECX argument.
0 unregisters only;2 retries with enabled-mask recheck;3/4/5 force1/0/-1 and break.
Calc6 resets/restarts;7 invokes an optional locked delete callback without implicit
unregister. Draw6/7 and out-of-range values are defaults. No arbitrary callback
mutation/concurrency or invalid-pointer success guarantee follows.

The [construction review](evidence/chain-source-closure/README.md) retains two
0x24-byte heads in the0x48-byte owner, partial ownership-bit initialization and
untouched argument words. Constructor449AA0/59 stays origin-indeterminate and
nonexact; implicit versus explicit outer construction is unknown. The491BE4
physical g_MainCallbackOwner/g_Chain alias and production data owner remain open.

## Current validation and recovery

Two independent full14-TU actual-source colds pass the new pair576/all44. First
cold also passes existing Unregister114/RemoveAll82 (four units772 total); its
full outputs are snapshotted before the second cold. Additional registration
cold passes43+43. Six unique units858 bytes are checked, with only576 new credit.
Canonical chain-run-calc/draw share the existing identical build-group output
with the lifecycle helpers. Profiles/support overrides remain declared, with
`/GL` retained. A dedicated duplicate output was rejected before compilation;
no build-graph invariant was weakened. Full source visibility/original flags are
not inferred from this reproduced14-TU context.

Private one-Chain-object/13-cache controls distinguish scalar wrappers/header-only
(316/272) from direct/member wrappers(308/268). Correct4/3 ordering then passes
all bytes. Equal-sized wrong table destinations fail the unchanged local-offset
gate at Calc+120h and Draw+100h. Signed-byte source compiled but its cached link
lacked the new decorated symbol: no linked comparison or signedness conclusion.
Do not repeat these forms as unexecuted hypotheses. They are not full colds.

Source host1375 ASan/UBSan assertions, eight strict faulty-code controls and19
EOL guards pass. Two new member-view projections map onto the same existing host
mutex/counter roles, exactly eight occurrences each, recorded in the manifest.
Only the active Chain.cpp LF pin changes; six other dependencies and strict
CRLF-to-LF-only rejection remain unchanged. No native layout/Windows concurrency/
renderer/game-runtime proof is implied by those host roles.

All1238 preexisting canonical unit definitions and existing
[large32-input/15-artifact pins](evidence/chain-source-closure/native-summary.json)
are freshly checked unchanged. The whole canonical manifest fingerprint changes
only by adding two units, so the old file hash is historical. Chain.cpp is outside
the large graph; Main.hpp is unchanged. No new large cold,35-guard replay or Run
measurement is claimed. Old scalar dispatcher receipts retain historical scope.

All108 fresh full CI groups pass; all368 frozen source/test/config inputs
remain byte-identical throughout CI. Classified cleanup removes20432288 logical
file bytes from12 owned reproducible trees. Both actual cold carriers, one
equal-sized rejected model and compact facts remain. Provider/legacy state and
nine independent D3D files are preserved. Final results:
[validation](evidence/chain-dispatcher-native/validation-summary.json).
Private recovery: `.analysis/gpt-6.1-sol/chain-counter-codegen-20261022/RECOVERY.md`
and manifest.json. Retain actual first-cold snapshot and current second-cold
carrier, one equal-sized failing local-table model, and compact producers/facts.
Do not blindly rerun old producers that restore source or reuse historical pins.

Global manual/automatic boundary metadata drift remains open:1116 rows,49 state
and107 confidence differences, zero geometry differences. First failure401100
provenance. Unowned indirect tables16. Ordinary tracking/CI does not waive it.

## Large owners and next route

| Owner / target | Current retained measurement | Remaining work |
| --- | --- | --- |
|ANM43EE30 /9964|9964 bytes;7541/7995 aligned positions;90/92 target-sized groups; aggregate gap2|Child91, temporaries49/53/54, FlipX61, common-tail2 and Tick order|
|ECL44E1A0 /7020|7020/6993,27 differences;189 fields/62 locals;171-byte float callee exact|Thread argument, SIB, homes and latch|
|Enemy40E770 /14416|14352 bytes; frame2C4;181 selectors/108 groups;42 target-sized groups; aggregate gap566|Laser ABI, shared tails and packet/register/stack lifetime|

Run still uses strict60-table-word/88-selector coverage despite the generic
normalization flag at its final selector byte. Older two-TU26 is historical.
The next bounded route can test whether the same known supervisor member view
recovers Calc/Draw insertion scheduling or audit remaining Chain source/ownership.
Do not transfer exactness across functions or change metadata from adjacency.
Original signedness/class/TU remain unknown even when codegen matches.

Whole Windows i386 product/runtime closure is open: globals, static init,
resources, libraries, link order and exercised paths still need audit. Semantic
reconstruction and portability have not started. The broad goal remains active.
