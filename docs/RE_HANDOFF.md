# TH10 reconstruction handoff

Updated2026-10-22. Exact reconstruction remains **active-incomplete** under the
operator's instruction to checkpoint and push bounded work. Current work recovers
Chain construction and four missing source mappings. It adds no exact bytes.
Historical reports retain their stated source/compiler context.

## Resume safely

- Original Japanese TH10 v1.00a only, ignored `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md),
  review Git status/diffs and recover dirty work before editing.
- Preserve the nine unrelated untracked `docs/evidence/d3d-adapter-format/`
  files. They are independent work, neither staged nor cleaned here.
- Use `scripts/repo-python` for repository Python. Operator preference is direct
  local tools, no Factory MCP. Local Ghidra review is read-only/noanalysis with
  rollback, one CPU and768 MiB heap; native builds serialized at nice10.
- Full mapped text401000..465A80 inclusive is412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  The older64A80-byte digest covers only a truncated prefix.
- Authorized checkpoints use English `gpt-6.1-sol: ...` commits and push to
  `origin/main` after validation. Replay only necessary affected units.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current source and progress

[Generated progress](PROGRESS.md):1776 reviewed candidates,734 confirmed authored
functions/273517 bytes,1092 source mappings and1238 exact functions overall.
Authored source is608 functions/246753 bytes;211 are source-present/nonexact
and126 remain source-absent. Authored exact remains **397 functions/72387 bytes**,
26.47% of the provisional authored byte denominator. Overall exact138897 bytes.
There are524 indeterminate origins, so these are not whole-game completion rates.

The [Chain review](evidence/chain-source-closure/README.md) maps existing complete
AddToCalc449AE0/131, AddToDraw449B70/131 and AllocElem449ED0/74. New actual source
constructs both0x24-byte heads in the0x48-byte owner and implements WinMain's
existing creation helper. The previous0x44-byte declaration omitted the final
argument word. Partial ownership-bit initialization preserves other flag bits;
both argument words remain untouched. New equal-priority entries precede older
entries.

Constructor449AA0/59 is now **origin indeterminate**: member construction and
WinMain allocation do not distinguish implicit synthesis from an explicitly
specified outer constructor. This removes one59-byte owner from the authored
count; it is not an excluded/compiler-origin assertion. Four total mappings
add three authored-source owners. The exact numerator does not change.

Actual Main/Chain/GameErrorContext cold emits59-byte constructor43/59 raw,
69/74 allocator and131-byte insertion bodies with shifted linked fields. Strict
comparison rejects allocation extent and insertion field inventories; masked
structural totals are not ordinary-byte scores. Three constructor order controls
are rejected; ownership publication after callback setters is adopted without
exact credit. Original spelling, class/TU ownership and compiler partition remain
unknown.

`g_MainCallbackOwner` and `th10::g_Chain` both bind target491BE4; physical source
alias/data ownership remains unresolved. No duplicate definition was added.
RunCalc449C00/273, RunDraw449D40/241 and other unmapped Chain behavior remain
open; this batch does not make the module source-complete.

## Current validation and recovery

Four full affected canonical cold groups pass10 selected units/1784 bytes:
lifecycle helpers196, registration wrappers86, Enemy helpers1172, ASCII330.
Each group rebuilds its complete declared graph, respectively14/14/10/3 TUs.
The separate final Main graph is a full three-TU diagnostic. No all-consumer
or full-reconstruction replay is claimed.

The Enemy image passes35 complete guards4424/all118 fields. All three complete
large bodies remain raw-identical to the preceding context. The unmodified Run
diagnostic retains7020/6993/27, all189 fields/62 locals and the complete171-byte
float callee exact. New **current32-input/15-artifact pins** are in
[Chain native evidence](evidence/chain-source-closure/native-summary.json).
Only Chain.hpp differs among those source inputs. Raw-NEG/thread and older
radial receipts are historical source/artifact contexts after this header edit.

The strict transition host refreshes exactly two of seven active LF pins and
compiles actual Chain constructors. Its860 assertions retain all843 old checks,
with16 prefilled-head properties and one post-scenario lock check. Sanitized
baseline and19 EOL guards pass. Hint and GameManager teardown compile the actual
source-extracted constructors; sanitized baseline and70 teardown cases pass.
Full CI first stopped at group82 because Ending's copied Chain declaration lacked
constructor definitions. Its builder now extracts both actual constructors and
records Chain.cpp as a dependency; repaired sanitized baseline passes. This is
a host-build dependency repair with unchanged game source and native pins.
Host success does not prove native layout/private ABI or game runtime.

All107 fresh full CI groups pass; all367 maintained source/test/config inputs
remain unchanged through CI. Classified cleanup removes7751908 logical file
bytes from four superseded native trees, three host review trees and four CI
trees. Current carriers, producer dependencies and compact source controls stay;
shared/provider/unknown state and nine independent D3D files are untouched. See
[checkpoint validation](evidence/chain-source-closure/validation-summary.json).
Private work: `.analysis/gpt-6.1-sol/chain-source-closure-20261022/`; begin with
RECOVERY.md and manifest.json. Avoid blindly rerunning historical producers:
some restore old source or modify documentation.

Global boundary metadata drift remains open:1114 metadata rows,49 state and107
confidence differences, zero geometry differences; first failure401100 provenance
text. Ordinary tracking/CI success does not waive this separate audit.

## Three large owners remain nonexact

| Owner / target | Current measurement | Remaining work |
| --- | --- | --- |
|ANM ExecuteScript43EE30 /9964|9964 bytes;7541/7995 aligned positions;90/92 target-sized groups; aggregate gap2|Child group91, float-index temporaries49/53/54, FlipX61, common-tail2 alignment and slow Tick operand order|
|ECL Run44E1A0 /7020|7020 bytes;6993 matched; all189 fields/62 locals; complete171-byte float callee exact|27 ordinary differences: thread argument, SIB, stack homes and common latch|
|Enemy Dispatch40E770 /14416|14352 bytes; frame2C4;19 sentinels;181 selectors/108 ordered groups;42 target-sized groups; aggregate gap566|Laser private ABI, tail sharing, packet lifetime and register/stack scheduling|

Run's generic decoder-normalization flag is false at the final selector byte;
strict annotations still cover all7020 bytes,60 table words and88 selectors.
The older separate two-TU26-difference result is historical, not current.
Positional counts are diagnostics and grant no partial exact credit.

Recent [thread/register controls](evidence/ecl-thread-register-controls/README.md)
reject11 private source models. Five repair SIB+146E while adding RETURN+AF,
keeping27 with a changed whole body. Earlier standalone StopThread locals were
already executed in older contexts; avoid repeating them as new work.
The [raw-word NEG correction](evidence/ecl-raw-negation/README.md) preserves the
target's modulo32 operation with0u-bits and removes signed-overflow UB; all three
large bodies remain unchanged. Its source/host evidence still applies, but the
Chain pack owns current ten-TU pins.

## Next bounded work and gates

Continue Chain full-body recovery at RunCalc/RunDraw, or isolate constructor
first-head store scheduling and the allocator's missing five bytes in a pinned
small native context. Review target exits, callback statuses and lock protocol
before adopting dispatcher source. Do not reuse ambiguous structural normalization
as exact proof. Adjacent TH08/TH09/TH095 source remains hypothesis material only.

Keep source presence, boundary, origin, exactness, product closure and runtime
independent. Whole Windows i386 build/runtime closure is open: data owners,
static initialization, resources, library objects, link order and exercised
runtime paths still need audit. Semantic reconstruction and portability have
not started. The broad reconstruction goal remains active.
