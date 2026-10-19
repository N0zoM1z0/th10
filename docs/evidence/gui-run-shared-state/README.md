# GUI Run replay exit and shared-state controls (2026-10-20)

This is a complete-owner negative result at source base `37e94db`. No function,
partial case or byte gains exact credit. All experimental production changes
are restored; the existing canonical child-search carrier remains unchanged.

## Target observations

The input is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh local read-only Ghidra verifies identity, the complete mapped text hash,
and unchanged bytes. The mapped `.text` SHA-256 is
`c1611c9becb832a4e2ba456afc58218fb5b8fafd6c85d19e3addbebb0c946e0a`.
Run starts at `0x415E90`; code ends at `0x416F61` inclusive.
Its reviewed physical owner remains 4480 bytes: 4306 code, 2 alignment and 172
bytes of four 24/7/7/5-entry tables. Frame `0xD8` and 150 CALLs are target facts.

Both opcode20 replay checks use JE to the same `0x416E8A` mode4 tail. After its
first profile unlock-byte store, the target reloads character `0x474C68`,
shot `0x474C6C`, difficulty `0x474C74` and stage `0x474C7C` for the second store. The stage6
branch writes `guiFlags |= 0x20` before reading faith `0x474C4C`. These sampling and
ordering observations do not establish the original C++ class or declaration.

## Compiler controls

All variants use the existing 14-TU real-Gui-updater graph, existing support
order, and VC7.1 SP1 build6030 `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`.
There is no `/GS`, forced register/inline control, target patch, synthetic
escape helper or copied target code. Reports below are structural diagnostics;
ordinary byte fractions use different address masks and are not exact scores.

The starting context recovers the six rejected controls documented in
[gui-child-search](../gui-child-search/README.md): physical case order,
character split, direct IDs, sprite full expressions, ordinary predicates and
packed-bit assignment. These are context, not six newly accepted changes.

| Cumulative control | Full Run bytes /4480 | Frame | CALLs | Ordinary diagnostic |
| --- | ---: | --- | ---: | --- |
| Restored source base | 4480 | D8 | 148 | 253/3160 |
| Replay work in `if (mode != 1)`, mode4 in `else` | 4444 | D8 | 150 | 1166/3152 |
| Four configuration reads through existing stage byte storage | 4484 | D8 | 150 | 1167/3136 |
| Four typed fields in existing GameScoreStateView | 4464 | D8 | 150 | 1163/3132 |
| Complement stage7 predicate and swap complete arms | 4464 | D8 | 150 | 1163/3132 |
| Integer getter local POD copy | 4464 | D8 | 150 | 1163/3132 |
| Repeat profile-count address expression | 4464 | D8 | 150 | 1163/3132 |
| Extend state declaration only | 4464 | D8 | 150 | 1163/3132 |
| Use shared state for six remaining scalar families | 4456 | D8 | 150 | 1162/3132 |
| Opaque cursor tag; byte-safe accessors retained | 4456 | D8 | 150 | 1162/3132 |

Replay inversion recovers the single shared mode4 call. The common-storage
models recover configuration reloads; the typed model's first 106 bytes of
opcode20 have the target instruction sequence before linked-field binding.
Using the common state for faith/power/value30/message40/message44/flags60
also recovers the flag-OR-before-faith-load sequence. It shortens opcode20
from 796 to 788 bytes, so this remains a source hypothesis, not closure.
The prefix was extended from `0x5C` to `0x64` only as a diagnostic; original class,
full object size and the semantic meaning of value30 remain unknown.

The typed-field and declaration-extension baselines each use a full fresh
14-TU cold link. Other controls reuse hash-checked support objects; the POD
and opaque-cursor controls rebuild the three header-dependent TUs. Every
run freezes all 42 quoted project inputs before and after the build. These
reuses are diagnostics, not independent cold proofs. HUD remains 3772/3836;
child search remains 70 bytes with 62/62 ordinary bytes, without a new exact claim.
Four-byte non-runnable anchor allocations can collide after field offsets are
added. Candidate-address coincidence does not establish a semantic binding.

## Opcode13 register evidence

The typed-state model has 1520 bytes against target 1535 in this physical span.
Both contain 414 instructions with matching opcode counts apart from the
stage7 conditional branch polarity. In physical order, all 32 hidden-return
LEA displacements and instruction lengths agree. Return-slot length accounts
for zero of the 15-byte gap. Instead the target has 24 six-byte absolute loads
into ECX; the candidate has 9 into ECX and 15 five-byte loads into EAX. This
accounts for the entire size gap. Load scheduling and registers also differ;
matching instruction counts do not prove semantic or byte equality.

The stage7 complement is *not* byte-neutral: it changes the complete raw owner
hash despite identical diagnostic totals. The getter POD and duplicate count
address controls are raw-neutral relative to their immediate stage7 context.
Only the address expression was duplicated; no second memory read was tested.
The opaque cursor is also raw-neutral relative to the shared-all model. A
record-pointer tag alone does not recover opcode13 load scheduling.

The pinned MIT reference c6791d82dc9f02e55eca45a5314f900a85481efd suggests
branch shape and a typed command/member-helper model. Its packed command,
forced directives and exactness claims are not adopted as TH10 evidence.
The opaque cursor retains byte-buffer reads and does not create or dereference
record objects. No packed integer lvalue or fixed eight-byte record is used.

## Checkpoint and limits

Private `.analysis/gpt-6.1-sol/th10-gui-replay-20261020` retains bounded
read-only target observations, frozen input/artifact hashes, diagnostic
reports and reproducible source overlays. Superseded native images and
full source copies are pruned after the checkpoint. Canonical recipes,
boundaries, origin and exact ledgers are unchanged. Product/runtime,
semantic and port gates remain open. See `validation-summary.json` for
completed checks and cleanup; pre-existing D3D evidence is untouched.
