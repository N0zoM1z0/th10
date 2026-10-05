# TH10 reconstruction handoff

Updated 2026-10-05. The Factory MCP was repaired and hot-switched behind the
existing public transport without a client refresh or reconnect. The native
`th10-ghidra` `check {}` operation now passes for `target:th10-main` with
`attestation.provider_transport=factory-native-command`. Use the hash-attested
disk target, pinned compiler, full-extent and relocation Oracles. This checkpoint
does not claim whole-product or runtime completion. The detailed 2026-10-02 campaign record is in
[RE_CAMPAIGN_NOTES_2026-10-02.md](RE_CAMPAIGN_NOTES_2026-10-02.md); its older
"next" statements are history, not current instructions.

## Identity and live state

- Target: **original Japanese TH10 v1.00a only**, at the ignored, read-only
  `resources/th10.exe`. Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Never substitute another edition or commit the target.
- The pinned compiler is VC7.1 SP1 build 6030. Use `scripts/repo-python` for
  repository Python; it verifies the exact Capstone installation first.
- At this handoff: 1,732 reviewed candidate boundaries and origins, 882
  maintained source mappings, 1,109 canonical exact functions and 110,966
  canonical exact `.text` bytes. Confirmed authored ownership is 266,187 bytes;
  authored exact code is 45,253 bytes (17.00% of reviewed owned bytes). The authored source-present,
  non-exact backlog has 143 functions. These figures come from the live ledgers
  and [PROGRESS.md](PROGRESS.md), not diagnostic byte scores.
- Native Windows i386 product closure and runtime validation are open. Semantic
  reconstruction and portability have not started. Exact functions alone do
  not establish a buildable or working game.
- Current operator-requested local commits use `gpt-dots: ...`; do not push. Check `git status` on
  resume rather than assuming the handoff worktree stayed clean.

## Recovery and verification

Read [AGENTS.md](../AGENTS.md), [RE_WORKFLOW.md](RE_WORKFLOW.md),
[ORACLES.md](ORACLES.md), the relevant [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md)
rows, and the current source/ledger row before changing anything. Recover any
dirty or untracked work first. The normal preflight is:

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

The current Factory MCP repository shell, target preflight, executable
toolchain, and Factory-native Ghidra attestation all pass. The operator's
2026-10-05 accelerated scope validates only modified function bodies/interfaces,
with explicit `replay-exact-units.py --unit UNIT` cold proofs for modified exact
units and new claims. Do not automatically rerun every prior or support-context
function. Complete owner/binding and Factory acceptance proof is still required;
record partial completed checks and cancelled broader work accurately.
Finish a bounded change with `verify-toolchain.py --check`,
`validate-tracking.py`, `progress.py`, `ci.py`, and `git diff --check`.
`config/matches.csv` and the canonical match units are the exactness boundary;
`probe-ltcg-backlog.py` produces diagnostics only.

## Current open frontiers

| Owner | Last retained evidence and unresolved issue |
| --- | --- |
| `EclVmContext::Run @ 0x0044E1A0` | Source-present, non-exact. ECLVM-052 freshly reproduces the selected `/GL` (no added `/GS`) 7,020/7,020-byte owner, frame 0x108 and 94/355 differing ESP fields. Five new factoring/type/profile controls are restored. FORMAT/shared advance and x87 homes remain open. `StartSubroutine` is 551/550; `ReadInt` is 144/144 with four ordinary bytes open. Host-rooted Host::Run151 is distinct from its Enemy-rooted146 canonical context. Regenerate before comparison. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | Source-present, non-exact. ANM-089 establishes the full 9,964-byte owner. ANM-090/091 retain interpolation operand order/widths, typed timer-reset calls, six-load global tangent copies and packed flips. Two fresh four-TU `/GL /GS` cold builds give identical 9,948/9,964-byte candidates, frame 0xFC, all 92 groups ordered and 87 target-sized spans; there is no partial exact credit. All 136 affected canonical units remain exact, 23,456 bytes across 36 artifacts. ANM-092 records restored CFG/type/profile controls; shared-tail placement, helper private ABIs and aggregate lifetimes remain open. |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | Source-present, non-exact. ENEMY-086/087 correct 20 movement sentinel guards (-999999.0, including NaN fallback), two polar +2pi additions and a random-angle pi*0.5 multiplier from raw target literals. Rank storage/signed division fidelity remains. Selected seven-TU graph is 14,416/14,416 bytes, still non-exact with frame 0x2BC/0x2C4. Candidate has 20 sentinel compares versus target 19; polar CFG, private ABIs and x87 homes remain useful trials. All 14 affected canonical units pass 860/860. Normalized byte scores do not validate constant values. Regenerate from current source before comparison. |
| `EnemyBeginSpell @ 0x00409280` | Source-present, non-exact. ENEMY-083's seven-TU graph is 2,476/2,432 bytes; statistics storage and call counts are constrained, but helper private ABIs and scheduling remain open. |
| `SoundPlayerView::ProcessQueues @ 0x0043DDF0` | The 1,419-byte authored code body is now source-mapped in `src/Sound.cpp`, still non-exact. The eight-entry jump table follows a one-byte NOP and is separate from the reviewed code extent. A `/GL /GS /EHsc` Sound+ZWave diagnostic has 1,348 code bytes plus a 32-byte table. See SOUND-019. The 338-byte streaming initializer is now exact under SOUND-020; the reset/refill frontier remains open. |

These are open engineering routes for the resumed reconstruction.
The earlier, much larger handoff is archived for detailed negative probes,
commands, source-shape constraints, and artifact locations. Per-claim durable
findings live in `docs/KNOWLEDGE_BASE.md`; current status and denominators live
in `docs/PROGRESS.md` and the ledgers. Adjacent TH08/TH09/TH095 source is
hypothesis material only.

## Recent bounded checkpoint

`SoundPlayerView::ReopenBgm @ 0x0043D790` is canonical exact at 55 bytes under
the real `LoadBgm` `/GL /GS /EHsc` caller graph with ZWave support. The 651-byte
memory-stream helper and 1,419-byte queue caller are source-present but
non-exact; no credit was granted for them. The final Sound/ZWave focused cold
replay passed all 17 accepted units, 2,034/2,034 bytes across nine artifacts.
See SOUND-018 and SOUND-019 in the knowledge base and the compact reports under
`.analysis/gpt-6.1-sol/20261002-sound-stream/`.

The 2026-10-03 handoff cleanup changed command names and comments only. A fresh
Sound/ZWave cold replay again passed all 17 units and 2,034 bytes across nine
artifacts (`handoff-affected-replay.json` in that analysis directory). Target
identity, executable toolchain smoke, tracking, generated progress, CI, and
`git diff --check` also passed. The last functional source checkpoint before
this cleanup was `883528d`.

## Artifact and phase boundaries

`.analysis/` and `build/` are ignored working state. An old PE/PDB or score is
not a current Oracle result; rebuild it from current source. Do not delete
unexplained legacy artifacts, private Ghidra state, toolchains, target files,
or game data. Keep new evidence compact and retain only what is needed to
reproduce an unresolved claim. Boundary, origin, source presence, exactness,
native build closure, runtime behavior, semantics, and port status remain
independent. The intended phase order is exact reconstruction, faithful
Windows i386 product and runtime closure, semantic reconstruction with both
target and native Oracles, then portability.

## Earlier resumed checkpoint

`CWin32ResourcePbgFile::Open @ 0x004366C0` is a 168-byte canonical normal-COFF
match after natural shared failure cleanup. Two cold builds and all seven
independently reviewed link fields pass; all nine ResFile accepted units remain
exact. See RESFILE-007/008 for the positive source hypothesis and rejected Seek
controls. Factory acceptance is a separate replay boundary; ledger figures above
are local canonical results and must not be confused with accepted Factory facts.

`DecompressData @ 0x00435DC0` adds 475 canonical exact bytes after correcting
the actual callee-clean ABI and trailing macro-loop scope, then matching natural
post-allocation initialization. Two cold decoder builds and all thirteen
affected PbgArchive/PbgFile/GameScore support-context units pass. A preexisting
PbgArchive Release constructor-versus-destructor helper manifest error was
independently diagnosed with an old-header control and narrowly repaired. See
COMPRESSION-003 and ARCHIVE-004. These checks preserve the complete comparator;
no ordinary bytes were masked and no Ghidra attestation was claimed.

## Latest Factory hot-deploy and dogfood checkpoint

The Factory repair is live behind the existing public MCP transport. The green
service and worker use Factory commits `78dce53` and `c83452f`; the latter keeps
TH10 linked-artifact routing from changing TH095 behavior. Full live validation
passed for all configured repositories/providers, and the old service was
retired only after the unchanged public route answered on the green service.
No Web client refresh or MCP reconnect was needed.

Three bounded TH10 source batches were performed entirely through that public
Factory MCP:

- `bd3bbb2` recovers `GameWindowView::InitD3DInterface`; its pinned `/GL`
  diagnostic is 38/45 bytes and remains non-exact.
- `7cbd96e` recovers `FileSystem::LoadArchive`; its pinned `/GL` diagnostic is
  39/36 bytes and remains non-exact.
- `11b62e7` recovers `ReplayFile::Open` and `ReplayFile::Read`. Open remains a
  135/137-byte mismatch. Read is canonical exact across its complete 74-byte
  PDB extent and six exhaustive linked fields in unit `replay-file-read`.

Factory replay job `job:18f7dafaeff6498f9a6a26b16946408a` completed on its
first attempt with receipt
`receipt:270b3cd8476bf1867fe08be88a39e21df9ec74f14294af50f4fcadf0f9c045fb`;
the receipt verdict passed and strict-live acceptance accepted the claim bound
to commit `11b62e7`. Earlier negative probes also established that the remaining
four ordinary bytes in `UpdateAbsoluteDirectionChange` are register-coloring
only, and that natural `EclVmContext::ReadInt` cursor-specialization variants
regress codegen. Those variants were reverted and summarized in the knowledge
base.

The 2026-10-04 ANM ownership correction changes reviewed authored owned bytes
from 265,810 to 266,187; authored exact stays 38,007. This is the existing TH10
full-owned-extent metric, with 500 origins still indeterminate, and no coverage
gain. See ANM-089 and the metric note in ORACLES.md.

## Current medium-function priority and preserved large checkpoint

The current operator request prioritizes whole authored functions of several
hundred to about 2,000 bytes, then returns to large owners. Do not expand the
tiny-function queue. The already near-verified 111-byte CheckIfFileAlreadyExists
closure has two independent cold proofs and all 13 affected existing units
passing 2,387 bytes; see FILESYSTEM-009. Local canonical authored exact rises
from 38,007 to 38,118 bytes without changing the 266,187-byte denominator or
resolving any of the 500 indeterminate origins. Factory receipt acceptance is
separate and must be checked before reporting accepted credit. Current local
commits use the operator-requested gpt-dots prefix, with no push.

ANM-093 preserves an unapplied seven-TU source-order candidate that removes the
misplaced 87-byte shared block and restores the color helper private ABI. It
remains 9,948/9,964 and non-exact. The earlier canonical ANM baseline was 86a43a8; ANM-094 now adds the
texture closure. Frozen large-owner candidates and negative controls remain
in the ignored ANM directory; apply only the texture-preserving rebase. Resume the medium inventory from fresh full-owner
measurements rather than treating old diagnostic residuals as current proof.

The first medium closure is TextRenderBufferApplyAlphaBleed at 0x00436DA0,
946 complete relocation-free bytes. Two canonical normal-COFF cold builds and
a /GL linked diagnostic agree raw-equal; see TEXT-001 for the target quirks and
negative controls. No existing unit depends on the new TextRenderer TU/header.
That checkpoint raised local canonical authored exact to 39,064 / 266,187,
with the same 500 unknown origins. Its Factory acceptance is recorded below. The previous 111-byte closure was accepted at a2e371a by job
2f3b73cefa0245f98ef192fbd8c5a03d, receipt
cab880166d2e9fbf281aff8f592c1298fc5ba6cc195043b4d211336c66b2113b.
The coherent second medium target was the 1,463-byte Anm texture alpha-bleed
owner, now covered by ANM-094. It recomputes row bases and does not share the
text helper's 16-bit halving.

## Latest medium texture closure

ANM-094 closes the complete 1,463-byte texture alpha-bleed owner. Two canonical
cold links pass all six independently bound local fields, all 43 table bytes
and three alignment bytes. The full affected cohort passes 136 units and
23,456 bytes across 36 artifacts. The new local canonical authored total is
40,527 / 266,187 (15.23%); 500 origins remain indeterminate and no boundary or
denominator changed. Its Factory acceptance is recorded in the latest checkpoint below.

The 946-byte text helper was accepted at 56f825c by job
0dcf22b0af294418b5cad21fd35560de, receipt
1d6720aec0c15ef9090d41eb6c1abcf9d2cc4f33c96359259854345ef9b65bb2.
Both exact medium owners preserve the observed format-specific distinctions.

ExecuteScript and the 803-byte radial initializer retain their prior diagnostic
residuals under this source change. The old ANM-093 source-order candidate
predates the texture closure and must not overwrite it. A merged candidate is
preserved as post56-texture-rgb-order-unbuilt.cpp in the ANM analysis directory;
it has not been built or accepted. The old trial-v3 driver intentionally rejects
the new canonical source. Preserve that guard and create a new baseline-bound
driver before resuming large-owner experiments.

## Shadow-text source checkpoint

The complete 554-byte shadow-text rasterizer now has maintained source in
TextRenderer.cpp, but remains non-exact. Its normal and linked candidates have
the correct extent with 24 ordinary stack-location differences, as recorded in
TEXT-002. Thirteen bounded source/interface/profile/context controls did not
close the parameter-home reuse difference. Do not force an int/surface alias
or count partial bytes. The existing 946-byte helper cold-replays exact after
the changes; canonical authored exact remains 40,527 / 266,187.

The 1,463-byte texture closure was Factory PASS and ACCEPTED on its first
attempt at bd84d4438dd9ee0d83b7619d8ede4eaa086226be, job
291fe2aaf4814d508f59966d430715a4, receipt
9af32afc19a22719f61beb2d443c07e77bbdce60caae504aedca999b29f2d609.
The latest request remains medium authored owners of several hundred to about
2,000 bytes. Other target-reviewed text candidates are the 509-byte alternate
rasterizer and the 781-byte font initializer with its 421-byte allocation
helper. Treat the alternate path's shared stack-home risk and the allocation
helpers' private EAX/ESI contracts explicitly; no new exactness is assumed.

## Text-buffer lifecycle closure

TEXT-003 adds the complete 781-byte font initializer and its required 107-byte
cleanup dependency as canonical exact units. Two cold linked builds have distinct
PE hashes/PDB GUIDs and zero differences over all 888 bytes after all 43 linked
fields are independently verified. The existing 946-byte text bleed unit also
cold-replays exact. This raises local canonical authored exact from 40,527 to
41,415 / 266,187 (15.56%); the denominator and 500 indeterminate origins are
unchanged. Factory acceptance remains a separate receipt gate.

The initializer tries 1024x64 format 0x1A then 0x15, fills the 256-byte prefix
from the existing RNG and creates all 15 font heights. The pooled CP932 font-face
literal matches the target. Do not bind RNG generationCount references to the
neighboring diagnostic font anchor: their semantic owner is g_RngView+4.

At the TEXT-003 checkpoint the coherent 421-byte allocator was source-present
but non-exact, with 33 ordinary bytes open in its final state-store scheduling. Its natural member source and
the cleanup member recover the private EAX/ESI contracts through the real caller
graph; no explicit private ABI is fabricated. Three allocator controls were
restored. The 554-byte rasterizer retains its known stack-home/failure-path
caveat. Original class ownership, physical TU/profile and runtime behavior remain
independent. Frozen evidence is in the ignored 20261004-text-allocation directory.

## Text allocation closure

TEXT-004 closes the full 421-byte allocator using ordinary HGDIOBJ staging and
coherent successful-resource publication. Independent review binds all seven
fields from the target import table, format records and cleanup callee, then
reproduces all 421 bytes. Two canonical cold links pass the allocator and both
781/107 dependencies; the first pass also protects the normal-COFF 946-byte
helper, for 2,255/2,255 affected bytes. The local-only temporary control was
neutral. No field type, API order, signature, failure path or padding changed.
Local canonical authored exact is 41,836 / 266,187 (15.72%), with the same 500
indeterminate origins. Its accepted Factory receipt is recorded below.

The preceding initializer and cleanup are both Factory PASS and ACCEPTED at
84d7d7d32f9878edb891f1da85ddb11f9dc6f13f on their first attempts:
- initializer job 0bb5341e57a14235b50a6ded12320dea, receipt
  5f3471d73c5d2f804e0f128f6fdaf4dd966ab5640b49e7ac03ee6f13527c2dbc
- cleanup job 66d80b2ce08f4fb78ca75c3b02e5049e, receipt
  82f968e3b061c9035eda3ed919910d86f24a04d97f0735e6027c6f7919136af9

The 554-byte renderer still has its known stack-home difference; no partial
credit is assigned. Frozen allocator controls, selected inputs and two cold
PE/map/PDB triples are in the ignored 20261004-text-allocation-tail directory.

## Accepted allocator and bounded radial controls

The 421-byte allocator is Factory PASS and ACCEPTED on its first attempt at
3677e763c26a921f625acff55ebbf8136ec68f30, job
fc4d0c8e84e4482899415c04c0e34f43, receipt
bf09cbefc48424a68e902f0787bd4c1a7844dafb21e84f8ad8b439040ae36f67.
Canonical authored exact remains 41,836 / 266,187 (15.72%); the imported ledger
baseline is not a claim that all old functions have fresh accepted receipts.

ANM-095 corrects the radial initializer's old masked-score interpretation. A
fresh current-graph compile reproduces its complete 803-byte non-exact candidate;
diagnostic semantic address normalization leaves twelve differing positions,
not eight ordinary residual bytes. All nineteen floating-literal occurrences
match exact target payloads. Two bounded lifetime/aggregate controls each add
four differing displacement bytes and are restored. Source and accepted texture
closure remain unchanged, with no added exact credit. Continue the medium
inventory rather than expanding another repeated profile/context sweep here.

## Archive entry-array closure

ARCHIVE-007 closes the complete 327-byte PbgArchive::AllocEntries primary body
without changing its natural source: the target-supported /EHsc profile restores
its missing exception/vector-construction setup. Independent review covers all
ten fields, including the CRT-defined __except_list ABS0 identity and generated
handler/unwind association. Two cold canonical normal-COFF builds pass; all eleven
related existing units also pass 382/382 bytes across four artifacts. The new
object/profile is distinct and preserves Release's repaired iterator binding.
EH-support code/data exactness, original production ownership/profile and runtime
remain independent; the new primary owner is Factory accepted as recorded below.

Local canonical authored exact is 42,163 / 266,187 (15.84%), with the same 500
indeterminate origins. No source mappings, boundaries or denominator changed.
The next prepared medium candidate is the 589-byte ECL script-table loader;
its old 547-byte result must be refreshed and its missing final target tail
captured before testing validation CFG and pointer-cursor lifetime hypotheses.

## Accepted archive owner and ECL loader checkpoint

The 327-byte archive entry allocator is Factory PASS and ACCEPTED on its first
attempt at 591a1cafc6070ac8a65048f5b917cbeb7c78ea17, job
e68143fb5e414c8e8cfbca7bf4b63d45, receipt
8f93b69ab556b96d347c1cb90c1ff7fd082bc8dc5e59586a9ceafa8c6789827b.

ECLVM-053 completes the 589-byte ECL loader's raw target tail review and
freshly reproduces its 547-byte normal/linked candidate. Bounded validation,
cursor, loop, label and reference controls do not close the 0x0C/0x10 frame and
CFG differences; /Og- regresses to 944 bytes. All are restored, preserving the
complete EclVm source and every existing exact unit. No exact credit changed.
At the ECL checkpoint, the next medium owner was the 338-byte sound-buffer
initializer, whose real ProcessQueues caller and ZWave layouts are maintained. Full target review
must preserve its asymmetric failure cleanup, including live Release blocks
that the provisional decompiler omitted.

## Sound-buffer initialization closure

SOUND-020 closes the complete 338-byte sound-buffer initializer using its real
ProcessQueues caller graph and ordinary C++ member declaration. Independent
review verifies every terminal path, all six fields and the unique ZWave support
contribution. Both canonical cold links pass, and all seventeen related old
Sound/ZWave units remain exact at 2,034/2,034 bytes across nine artifacts. The
canonical recipe uses pdb_source for support ownership without changing tooling.
The target's asymmetric failure cleanup and unchecked new pointer array are
preserved, including both Release paths absent from provisional decompilation.

Local canonical authored exact is 42,501 / 266,187 (15.97%), with the same 500
indeterminate origins and no boundary/denominator change. Factory acceptance of
this new owner is recorded below. Original production ownership, full caller and
data-owner closure, and runtime remain open. At the initializer checkpoint, the
next prepared medium candidate was the 369-byte BGM preloader. Its completed
source recovery and bounded non-exact controls are recorded below.

## Accepted sound initializer and BGM preload checkpoint

The 338-byte sound initializer is Factory PASS and ACCEPTED on its first attempt
at ecc113a5fde5d21f31119017f67e086ba5ad425d, job
853383083a5a41c797f7354c1fdec16c, receipt
4e4af1506325f466367557a5ee4275bae627e6f42dbf80e2165b4cd23ec38384.

SOUND-021 retains natural source for the complete 369-byte BGM preload owner,
with no exact credit. The real ProcessQueues Sound+ZWave graph emits 369 bytes
and matches 323/329 ordinary bytes; six ReadFile argument-setup register bytes
remain different. Three read-local controls are byte-neutral; a late format-
pointer local regresses the owner to 367 bytes. All four are restored. The
source preserves global-versus-receiver filename asymmetry, unchecked reads and
requested-size publication. All seven affected exact units pass 957/957 bytes
across six canonical artifacts with the selected source.

Source mappings increase to 879 and authored source-present non-exact backlog
to 147. Canonical authored exact remains 42,501 / 266,187 (15.97%), with 500
indeterminate origins and no changed boundary, denominator or exact ledger row.
At the preload checkpoint, the next medium lane was the 637-byte streaming
refill owner and its real 189-byte worker caller. That bounded lane is recorded
below; the small worker is a dependency, not a tiny-owner queue.

## Streaming refill source and real worker closure

SOUND-022 retains the complete streaming refill source with target-supported
separate cursor lifetimes. The selected linked owner remains 634/637 and gets
no exact credit. Five bounded CFG controls are restored, including two 637-byte
layouts that still misplace the middle fill blocks. All target error/unchecked
paths and the private stack receiver are preserved without ABI tricks.

SOUND-023 closes the required real 189-byte worker caller through all seven
independently reviewed bindings and two canonical cold links. The worker uses
the existing ProcessQueues Sound+ZWave graph and shares its existing
SoundInitBuffers artifact. All 18 affected old exact units pass 2372/2372 bytes
across 10 artifacts under the selected source. No old recipe, boundary, origin
or denominator changes; the 637-byte refill remains entirely non-exact.

Local canonical authored exact is 42,690 / 266,187 (16.04%), with 500 indeterminate
origins,881 source mappings and 148 authored source-present non-exact owners.
The worker's accepted Factory receipt is recorded below; imported canonical totals
must not be described as universally fresh Factory acceptance. The next medium
candidate is the 360-byte FileSystem::OpenFile owner. Its caller count and
archive/disk mode behavior require fresh verification before source recovery.


## File loader closure and reader fidelity checkpoint

FILESYSTEM-010 closes the complete 360-byte FileSystem::OpenFile owner. Both
canonical cold links pass all 26 independently reviewed fields with distinct
PE/PDB identities. All 161 existing affected units pass 27,909/27,909 bytes
across 49 cold artifacts, including the transitive shared-header closure.
The selected real Main checksum graph uses natural three-parameter __stdcall
source and target-supported lookup outlining with a tested bare inline_depth()
reset. Original pragma spelling, production TU/profile and runtime remain open.

The distinct main archive is at 0x00497990, separate from the twenty-pack array
at 0x004923B0; entry count is main archive+4. ARCHIVE-008 restores the reader's
two-pass checksum, literal-zero seek and byte index, but its complete candidate
remains 284/281 and receives no exact credit. Preserve the frozen source/context
negative controls rather than repeating the old ABI/TU sweep.

Local canonical authored exact is 43,050 / 266,187 (16.17%), with 500 origins
still indeterminate. There are 882 source mappings, 1,104 canonical exact
functions, 108,763 exact .text bytes and 148 authored source-present non-exact
owners. The accepted OpenFile Factory receipt is recorded below; imported canonical totals
are not universally fresh accepted receipts. The prepared next medium lane is
the 433-byte ReadAnmEntries owner, starting from fresh current-source evidence.

The preceding 189-byte sound worker is Factory PASS and ACCEPTED at
57c0e9d6262fe1290909fc5c52e1fa11ca15a705, job
dddc9ea267fc4abd9646c018aeb7b890, receipt
21cbf5415a72bb3e9e51ccc42207afe167abfbe85ab3b3692dd3a24a198a9c93.
The saved acceptance packet is in the ignored 20261004-stream-refill directory.


## ANM resource-read pair closure

ANM-096 closes the complete 433-byte ReadAnmEntries owner and its required
214-byte LoadExternalTextureData dependency. Two canonical cold links reproduce
647/647 bytes and all 26 independently reviewed fields with distinct PE/PDB
identities. All 231 prior affected units pass 33,550/33,550 bytes across 81
cold artifacts. This includes the full transitive AnmManager.hpp and new
GameErrorContext.hpp closure, the accepted OpenFile context, and both existing
error-context proofs. No new claim is made for the standalone constructor16.

Natural source uses the real Fatal definition, coherent shared class declaration,
two target-supported 260-byte path arrays, the existing counter initialized
before allocation and incremented before the terminal offset test, and one real
zeroing constructor instead of separate caller memset. Original constructor
ownership and MAX_PATH spelling remain unknown. The target failure/publication
quirks remain unchanged. In this new graph Fatal200/201, OpenFile358/360 and
Preload85/81 remain non-exact; their old canonical contexts are not replaced.

The first Enemy normal replay failed on compiler-local labels. Independent
COFF definition and full table replay proved pure renumbering; exactly ten
name-only manifest updates in two resolver units were applied. Relocation
positions/types/addends/targets, extents and profiles stayed unchanged. The
full Enemy.obj cold retry passes all nine units and 204 bytes. The original
failure is retained as regression-02-before-label-review.*. See ANM-097.

Local canonical authored exact is 43,697 / 266,187 (16.42%): 1,106 exact
functions, 109,410 canonical .text bytes, 882 source mappings and 146 authored
source-present non-exact owners. All 500 indeterminate origins remain; no
boundary, origin or denominator changed. Both new claims are Factory PASS and ACCEPTED on their first attempts at
19875ae5609685952bf0282e296153ef797c0531; receipts are recorded below.

The older post56-texture-rgb-order-unbuilt.cpp candidate preserves the earlier
texture closure but predates this resource-loader source/header change. Do not copy it over
current AnmManager.cpp or relax its old hash guards. Rebase only its reviewed
intended source-order delta onto the current source/header baseline, preserving
the new constructor, Fatal calls, capacities and all prior exact closures.
The prepared next medium candidates are LoadTextureData650 and LoadSurface576;
verify their own target calls and buffer spans before transferring hypotheses.

The preceding OpenFile360 closure is Factory PASS and ACCEPTED at
a129047295043db56e391dca585775008597c182 on its first attempt, job
de160cebc24d46f6bb09657a36783fec, receipt
8d2ac1e0ab4a90b6ab9b3ce20098f035b31f4679730cfb8260efda8b416c72b6.
The complete acceptance envelope is in the ignored 20261004-filesystem-open
analysis directory. Imported canonical totals are not a claim of universally
fresh accepted Factory receipts.


## Accepted ANM resource pair

Both claims at 19875ae are terminal PASS+ACCEPTED:
- ReadAnmEntries433: job b755f9802998423684d7392fc42cefff, receipt
  b122790df83fa3d70e9223eb0d655d07fca8fef5a4e82aa0cf98f4e0ec1efd5f
- LoadExternalTextureData214: job d18c097ac5724fc3bb3b67561a79a2f8, receipt
  ccc637a19e448a022392543cd2d1a338aad6d2ff24af2459391f0654fa9896dc

Full raw envelopes are retained in the ignored ANM resource-loader directory.
The first checkpoint attempt did not execute because the automatic approval
reviewer was at capacity. After backoff and unchanged-status reconciliation,
the same request passed normal approval and committed cleanly; no review was
bypassed. Both actual Factory replay jobs then passed on their first attempts.
Authored exact remains 43,697/266,187 (16.42%), with 500 unknown origins.


## ANM texture and surface loading closure

ANM-098/099 close LoadTextureData (650 bytes) and LoadSurface (576 bytes). Two canonical cold
links reproduce all 1,226 bytes and all 38 independently reviewed bindings with
distinct PE/map/PDB identities. All 139 existing affected units pass 25,566 bytes
across 38 cold artifacts, including the 33 consumers where AnmManager is support.
The source and recipe changes are confined to AnmManager.cpp and two new units;
all old recipe dictionaries,
headers, source mappings, boundaries, origins and the denominator stay unchanged.

Texture loading uses real Fatal and the target's repeated texture-field reads
across COM calls and sprite publication. Surface loading uses its real Fatal
call, target-supported 260-byte path and two observed null-guarded frees. The first
COM failure's cleanup bypass and other unchecked/ownership behavior remain.
No private ABI annotation or synthetic caller was introduced. Postload is the
real texture caller; for Surface it is only the declared whole-TU context.
Direct incoming references were not found, and indirect reachability, original
capacity spelling, production ownership and runtime remain unknown.

Local canonical authored exact is 44,923/266,187 (16.88%), up 1,226 from 43,697,
with 1,108 exact functions, 110,636 canonical .text bytes, 882 mappings and 144
authored source-present non-exact owners. All 500 indeterminate origins remain.
Both new claims are Factory PASS and ACCEPTED on their first attempts at
b2dbb473187b564e4238227e56010487f0eff265; receipt details follow below.
Imported canonical totals are not universally fresh accepted Factory receipts.

The prior scoped 401 output read paused dependent work. After an explicit user
retry request, the identical normal-tool call recovered the final 11,380 bytes,
completing the 60,532-byte review without rerunning its command. The cause is
unknown; the denied envelope and successful retry are both retained. A separate
transient Ghidra discovery UNAVAILABLE retried normally and attestation passed.

During regression, origin/main advanced to the unchanged HEAD 64b8b8f. Its
reflog records an update by push at 03:43:20 UTC; the actor is unverified and
this lane issued no push. All 58 source/header hashes, the selected manifest
hash and the same two dirty paths were rechecked unchanged before continuing;
no completed build was repeated for the ref-only change.

The next prepared medium owner is LoadTextureRegion (320 bytes) at 0x00446D70. Inspect
its current source, full target, real caller and retained artifacts before a
bounded trial; the existing exact alpha-bleed helper is a useful dependency.
Old large-ANM whole-file candidates are still stale: rebase only intended deltas
onto this source, preserving every reviewed resource/texture/surface closure.


## Accepted ANM texture and surface pair

Both complete owners were accepted at source checkpoint b2dbb473187b564e4238227e56010487f0eff265:

- LoadTextureData, 650 bytes: job e699c1c9694a41ee94c4b99dc49c9299, receipt
  b6eec439f684eae9b2d1a4031e07a40421cf8bf40da141c22d2873106d9ccc46
- LoadSurface, 576 bytes: job 9c95845a501c46fd9b42018eff070fad, receipt
  2b1dc81773dc70dff55e5620f94a7e2231dd39240379fa892fc61fdbac2b586b

Both first attempts are terminal PASS+ACCEPTED under strict-live-v1 with forced
recompilation. This accepts the full 1,226-byte batch, bringing authored exact to
44,923/266,187 (16.88%), with the same 500 indeterminate origins. The source
checkpoint is local; this lane issued no push. Native product/runtime gates remain
open, and imported totals are not universally fresh accepted Factory receipts.

Full accepted job envelopes, the complete independent audit, both cold triples
and all 38 regression artifacts are preserved in the ignored
20261005-anm-texture-surface-final packet. The next medium lane begins with fresh
LoadTextureRegion evidence against this source; existing artifacts may be
inspected before another build when all inputs match.


## Restored medium region controls

ANM-100 corrects LoadTextureRegion's incoming ABI from complete raw bytes and
preserves four source-restoring controls. The current baseline is 321/320 with
frame 0x54; real branch-local aggregates recover frame 0x34, and delaying the
surface-pointer initialization recovers the target private input registers.
The best candidate is 319/320 with the embedded-header address/register segment
still open. Two further pointer-scope/indexed-address controls are byte-neutral.
All source is restored; authored exact stays 44,923/266,187 (16.88%) and the
500 indeterminate origins remain. No partial exact credit or new regression
is claimed. Preserve the candidates and genuinely rebase any future delta
onto the then-current source.

The next medium control is Ascii Initialize (330 bytes) through its real Create caller.
Its error path calls Log (201 bytes) at 0x44B810 with ECX=0x474F70, not Fatal. The shared
Log expression with GameErrorContext visible is independently supported; other
resource, chain-registration and SetSprite call seams are still unresolved.
Do not predict a full closure from the Log change alone.


## Latest medium closure: ASCII initialization (2026-10-05)

ANM-101 records the complete 330-byte Initialize owner at 0x00401110. Real
PreloadAnm, Chain allocation/insertion, SetSprite and Log calls replace thin
aliases. Separate file/VM lifetimes recover the target EAX receiver naturally;
both explicit post-reset anmFile stores and the CP932 error literal remain.
The shared Chain declaration preserves layout and existing attributes. Allocation
uses the observed operator new, priorities are signed, insertion methods have
standard callee cleanup and direct list-field accesses, and the depth decrement
follows LeaveCriticalSection. The five modified Chain helpers remain nonexact;
RegisterCalc/RegisterDraw retain their existing unresolved RET versus target
RET4 issue, and are not called by Initialize. No helper exact credit is added.

Both canonical cold triples independently reproduce all 330 bytes and all 28
fields. Source/interface review covers the six modified runtime definitions and
the shared layout. All 1,108 old unit dictionaries are unchanged. At the
operator's explicit acceleration request, broad regression groups 12-39 were
cancelled. Already completed groups 00-11 pass 87 units / 17,478 bytes across
12 artifacts; this is partial historical evidence, not a full 143-unit pass.
Earlier duplicate-risk approval rejections and their idle-state reconciliations
are preserved; the cancelled regression-12 action was not rerouted or retried
under the new scope. Source and both cold proofs were unchanged on resumption.

An isolated wider import-header variant also matched the 69-byte Create owner.
That variant is deferred: the selected canonical-header graph emits Create70,
and no Create credit is added. Region319/320 and the Chain helper residuals
remain open. The next medium queue prioritizes DrawMode6 (643), DrawStrings
(738) and DrawGuiStrings (570), then ProcessQueues (1,419), using current retained
artifacts before new trials; old diagnostic sizes are not fresh residuals.

The ignored evidence packet is
`.analysis/gpt-dots/20261005-ascii-initialize-final/`, with both cold triples,
selected inputs, focused review and scope/cancellation records. Local canonical
authored exact rises from 44,923 to 45,253 / 266,187 (17.00%); the denominator and
500 indeterminate origins are unchanged. The previous local-canonical baseline
was 44,923 bytes; the new 330-byte Factory receipt passed and was accepted on
2026-10-05 at 07:37 UTC, as recorded below. Imported ledger totals are not
universally fresh accepted Factory receipts. Native product, runtime, semantic and port gates remain open.
No push is performed.


### ASCII Factory acceptance (2026-10-05)

The complete Initialize330 claim passed Factory replay and was accepted at
07:37:08 UTC against clean source commit
`466804f828dc1336020fb3b7fc7aef0331f524ec`:

- Job: `job:6d6a1ee87a9b4e959b27d3512d498846`
- Receipt: `receipt:e0b410f7e8806d34908d6cb9d88511aee9732f0bf1ac7349f2985cb3e83d9af9`
- Driver: `th10-vc71sp1-linked-pe-function-v1`, forced recompile
- Result: `receipt_verdict=pass`, `acceptance_decision=accepted`

Two earlier submissions failed before execution because the service reported a
configuration mismatch; they produced no compiler result or receipt. After
backoff, public metadata exposed an explicit worker-configuration binding and
an unrelated successful replay. The third explicit submission used that normal
published binding and passed on its first executed attempt. No private
configuration was read or changed, and no rejection was bypassed. Full failed
and accepted envelopes, public binding metadata and focused proof are retained
in the ignored Ascii evidence packet. This confirms the330-byte increment to
45,253 /266,187 (17.00%) local canonical authored bytes. No Create/Chain helper
credit, broad-cohort pass, whole-product/runtime closure or push is claimed.
