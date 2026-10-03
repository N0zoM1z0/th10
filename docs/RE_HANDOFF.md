# TH10 reconstruction handoff

Updated 2026-10-03. The operator explicitly resumed TH10 through Factory and
authorized proceeding without Ghidra. Use the hash-attested disk target, pinned
compiler, full extent and relocation Oracles. Do not bypass or alter the broken
Ghidra provider binding, and do not claim a new Ghidra attestation. This checkpoint
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
- At this handoff: 1,732 reviewed candidate boundaries and origins, 867
  maintained source mappings, 1,093 canonical exact functions and 103,646
  canonical exact `.text` bytes. Confirmed authored ownership is 265,810 bytes;
  authored exact code is 37,933 bytes (14.3%). The authored source-present,
  non-exact backlog has 144 functions. These figures come from the live ledgers
  and [PROGRESS.md](PROGRESS.md), not diagnostic byte scores.
- Native Windows i386 product closure and runtime validation are open. Semantic
  reconstruction and portability have not started. Exact functions alone do
  not establish a buildable or working game.
- Local commits use `gpt-dots: ...`; do not push. Check `git status` on
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

The current operator selected Factory MCP repository shell and explicitly waived
Ghidra for this session. Target and executable toolchain preflight passed. For
source edits, cold-replay every affected accepted unit with
`scripts/repo-python scripts/replay-exact-units.py --source src/FILE.cpp`.
Finish a bounded change with `verify-toolchain.py --check`,
`validate-tracking.py`, `progress.py`, `ci.py`, and `git diff --check`.
`config/matches.csv` and the canonical match units are the exactness boundary;
`probe-ltcg-backlog.py` produces diagnostics only.

## Current open frontiers

| Owner | Last retained evidence and unresolved issue |
| --- | --- |
| `EclVmContext::Run @ 0x0044E1A0` | Source-present, non-exact. ECLVM-049's selected `/GL` graph has a 7,020/7,020-byte owner and 5,767/6,264 normalized comparable bytes. FORMAT and shared advance, x87 stack homes, and private register allocation remain open. `StartSubroutine` is 551/550; `ReadInt` is 144/144 but has four ordinary bytes open. Regenerate the graph before trusting candidate addresses or scores. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | Source-present, non-exact. ANM-083 retains 92 physical groups in target order and 85 EDI restores; the selected PDB contribution is 9,964 bytes, with 5,324/8,069 case-aligned diagnostic agreement. Float-local lifetimes and interpolation homes remain open. |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | Source-present, non-exact. The prior selected graph is 14,384/14,416 bytes with a 0x2BC/0x2C4 frame difference. It depends on ECL readers, ANM/Player helper ABIs, laser construction, and whole-function allocation. Regenerate from current source before comparison. |
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

## Latest resumed checkpoint

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
