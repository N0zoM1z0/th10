# TH10 reconstruction handoff

Updated 2026-10-22. Exact reconstruction remains **active-incomplete**. Current
batch additionally closes both Chain insertion owners: **131 bytes each, seven
fields each, two independent actual-source colds**. New credit is two authored
functions/262 bytes; product/runtime remains open.

## Resume safely

- Original Japanese v1.00a only, ignored `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
  inspect status/diffs and recover dirty work before editing.
- Preserve nine independent untracked `docs/evidence/d3d-adapter-format/` files.
  Their hashes are checked; never stage or clean them with this batch.
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
209 source-present/nonexact,124 source-absent. Authored exact **401/73225 bytes**;
overall exact1242/139735 bytes. Unknown origins524 keep the whole-game denominator
unresolved. Boundary/origin/source/exact/product/runtime remain separate claims.

The [insertion proof](evidence/chain-insertion-native/README.md) closes complete
Calc449AE0..449B62 and Draw449B70..449BF2 owners131 each, densely decoded through
RET4. The source reuses existing supervisor CS[0]/unsigned-byte counter[0] views;
no new global, type, signature, algorithm, volatile or padding. Physical ESI element,
EDI signed priority and stack owner remain. Added callbacks use ECX argument before
locking, retain their result and are cleared; new equal-priority elements precede
old ones. Original source class/TU/profile and physical storage aliases are unknown.

The earlier [runner proof](evidence/chain-dispatcher-native/README.md), committed
and pushed as10798f4, closes Calc449C00..449D33/308 and Draw449D40..449E4B/268,
including alignment and14 local table words. That576-byte gain is separate from
the prior62-byte table ownership correction and current262-byte insertion gain.

Both runners cache next before callbacks; null callbacks are uncounted, disabled
nonnull callbacks(mask0x2, bit1) counted. Callbacks run unlocked with ECX argument.
0 unregisters only;2 retries with enabled-mask recheck;3/4/5 force1/0/-1 and break.
Calc6 resets/restarts;7 invokes an optional locked delete callback without implicit
unregister. Draw6/7 and out-of-range values are defaults. Arbitrary mutation,
invalid-pointer success and Windows concurrency are outside the established scope.

The [construction review](evidence/chain-source-closure/README.md) retains two
0x24-byte heads in the0x48-byte owner, partial ownership-bit initialization and
untouched argument words. Constructor449AA0/59 stays origin-indeterminate and
nonexact; implicit versus explicit outer construction is unknown. AllocElem74
still emits69 in its reviewed context. The491BE4 physical g_MainCallbackOwner/
g_Chain alias and production data owner remain open.

## Current validation and recovery

Two independent full14-TU actual-source colds pass the insertion pair262/all14
fields. First cold also freshly preserves both runners576/all44: four units838
bytes. Its complete output is snapshotted and hashed before the second cold.
Second cold recompiles all14 TUs for the two new owners. No other old helper or
large-owner replay is claimed for this batch. Canonical new units share the
existing ScriptViewerLifecycle-cold-2 output and preserve all `/GL` profiles and
support overrides. No original production flags or TU partition is inferred.

The private one-object/13-cache member model passes262/14. Old scalar bodies are
already131 bytes but fail exhaustive field positions: target counter25h/31h,
candidate29h/3Bh. These controls are not full colds. Earlier dispatcher wrapper/
header-only/member/order trials and the signed-symbol link failure retain their
historical source scope; do not rerun old producers that restore source or use
historical pins.

Fresh host1375 ASan/UBSan assertions, eight strict faulty-code controls and19 EOL
checks pass. The two existing supervisor-to-host storage projections now require
12 occurrences each, from8. Only the active Chain.cpp LF pin changes; six other
dependencies and CRLF-to-LF-only rejection remain unchanged. Host roles do not
prove native layout, Windows concurrency, renderer or game runtime.

All1240 preexisting canonical unit definitions and existing
[large32-input/15-artifact pins](evidence/chain-source-closure/native-summary.json)
are freshly checked unchanged. Whole canonical manifest fingerprints change only
by adding two units and are historical outside their recorded source snapshots.
Chain.cpp is outside the large graph; Main.hpp is unchanged. No new large cold,
35-guard replay or Run measurement is claimed. Current insertion colds supersede
the earlier runner proof's shared second carrier; its first snapshot and receipts
remain historical, with current runners freshly preserved in first insertion cold.

All108 fresh full CI groups pass with all368 frozen source/test/config inputs
unchanged. Classified cleanup removes14120146 logical file bytes from eight
owned reproducible trees and one duplicate disassembly. Both actual cold
carriers, compact facts and the baseline negative remain. Independent D3D work
and provider/legacy state are preserved. Results:
[validation](evidence/chain-insertion-native/validation-summary.json).
Private recovery: `.analysis/gpt-6.1-sol/chain-insertion-codegen-20261022/RECOVERY.md`
and manifest.json. Retain current first-cold snapshot and second canonical carrier,
compact source model/negative inventory reports and producers. The previous batch
already removed20,432,288 logical file bytes and preserved independent D3D work.

Global boundary metadata drift remains open:1116 rows,49 state and107 confidence
differences, zero geometry differences. First failure401100 provenance. Unowned
indirect tables16. Ordinary tracking/CI does not waive this separate gate.

## Large owners and next route

| Owner / target | Current retained measurement | Remaining work |
| --- | --- | --- |
|ANM43EE30 /9964|9964 bytes;7541/7995 aligned positions;90/92 target-sized groups; aggregate gap2|Child91, temporaries49/53/54, FlipX61, common-tail2 and Tick order|
|ECL44E1A0 /7020|7020/6993,27 differences;189 fields/62 locals;171-byte float callee exact|Thread argument, SIB, homes and latch|
|Enemy40E770 /14416|14352 bytes; frame2C4;181 selectors/108 groups;42 target-sized groups; aggregate gap566|Laser ABI, shared tails and packet/register/stack lifetime|

The Chain insertion route is now closed in the declared context. A next bounded
route can resume ECL Run's thread argument/home/latch controls against the pinned
ten-TU context. Its strict60-table-word/88-selector coverage remains required despite
the generic normalization flag at the final selector byte. Older two-TU26 is
historical. Do not transfer exactness across functions or infer original layouts,
class/TU/profile, signedness or storage ownership from matching neighbors.

Whole Windows i386 product/runtime closure is open: globals, static init,
resources, libraries, link order and exercised paths still need audit. Semantic
reconstruction and portability have not started. The broad goal remains active.
