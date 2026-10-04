# TH10 reconstruction handoff

Updated 2026-10-04. The Factory MCP was repaired and hot-switched behind the
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
- At this handoff: 1,732 reviewed candidate boundaries and origins, 871
  maintained source mappings, 1,094 canonical exact functions and 103,720
  canonical exact `.text` bytes. Confirmed authored ownership is 266,187 bytes;
  authored exact code is 38,007 bytes (14.28% of reviewed owned bytes). The authored source-present,
  non-exact backlog has 147 functions. These figures come from the live ledgers
  and [PROGRESS.md](PROGRESS.md), not diagnostic byte scores.
- Native Windows i386 product closure and runtime validation are open. Semantic
  reconstruction and portability have not started. Exact functions alone do
  not establish a buildable or working game.
- Local Web commits use `gpt-web: ...`; do not push. Check `git status` on
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
toolchain, and Factory-native Ghidra attestation all pass. For
source edits, cold-replay every affected accepted unit with
`scripts/repo-python scripts/replay-exact-units.py --source src/FILE.cpp`.
Finish a bounded change with `verify-toolchain.py --check`,
`validate-tracking.py`, `progress.py`, `ci.py`, and `git diff --check`.
`config/matches.csv` and the canonical match units are the exactness boundary;
`probe-ltcg-backlog.py` produces diagnostics only.

## Current open frontiers

| Owner | Last retained evidence and unresolved issue |
| --- | --- |
| `EclVmContext::Run @ 0x0044E1A0` | Source-present, non-exact. ECLVM-052 freshly reproduces the selected `/GL` (no added `/GS`) 7,020/7,020-byte owner, frame 0x108 and 94/355 differing ESP fields. Five new factoring/type/profile controls are restored. FORMAT/shared advance and x87 homes remain open. `StartSubroutine` is 551/550; `ReadInt` is 144/144 with four ordinary bytes open. Host-rooted Host::Run151 is distinct from its Enemy-rooted146 canonical context. Regenerate before comparison. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | Source-present, non-exact. ANM-089 corrects the reviewed full owner to 9,964 bytes: 9,587 code + one NOP + 94-slot owned table. Fresh four-TU `/GL /GS` candidate is 9,964/9,964 with all 92 physical groups ordered. Stack/interpolation homes and case spans remain open; equal size is not exactness. The +377 denominator correction adds zero exact credit. |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | Source-present, non-exact. ENEMY-086/087 correct 20 movement sentinel guards (-999999.0, including NaN fallback), two polar +2pi additions and a random-angle pi*0.5 multiplier from raw target literals. Rank storage/signed division fidelity remains. Selected seven-TU graph is 14,416/14,416 bytes, still non-exact with frame 0x2BC/0x2C4. Candidate has 20 sentinel compares versus target 19; polar CFG, private ABIs and x87 homes remain useful trials. All 14 affected canonical units pass 860/860. Normalized byte scores do not validate constant values. Regenerate from current source before comparison. |
| `EnemyBeginSpell @ 0x00409280` | Source-present, non-exact. ENEMY-083's seven-TU graph is 2,476/2,432 bytes; statistics storage and call counts are constrained, but helper private ABIs and scheduling remain open. |
| `SoundPlayerView::ProcessQueues @ 0x0043DDF0` | The 1,419-byte authored code body is now source-mapped in `src/Sound.cpp`, still non-exact. The eight-entry jump table follows a one-byte NOP and is separate from the reviewed code extent. A `/GL /GS /EHsc` Sound+ZWave diagnostic has 1,348 code bytes plus a 32-byte table. See SOUND-019. Streaming reset/initialization helpers remain source-absent. |

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
