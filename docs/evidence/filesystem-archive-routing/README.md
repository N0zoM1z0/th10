# Archive-or-disk path helpers

2026-10-22, starting38480c3, original Japanese TH10 v1.00a only. This adds
maintained source for three reviewed authored owners, **616 bytes and no exact
credit**. [Facts](facts.json) retain target, source, artifact and validation pins.

| Target | Owned bytes | Maintained source name |
| --- | ---: | --- |
|4356F0..4357C3|212|ArchiveOrDiskPathView::FindArchive|
|435800..4358D5|214|ArchiveOrDiskPathView::Read|
|4358E0..43599D|190|ArchiveOrDiskPathView::GetSize|

## Target behavior recovered

Lookup copies the complete path into a260-byte local buffer. With a first `/`,
it overwrites that slash and the suffix with `.dat`; without a slash it compares
the path unchanged. It scans the active16-byte PbgArchive rows from4923B0 using
the signed count at477850, comparing filename member+8 with case-sensitive
strcmp. It returns the first match or NULL. Existing archive array/count
declarations are reused; no new globals or physical storage owner is introduced.

A matching archive selects its entry reader or decompressed-size helper.
The entry name follows the first slash, with the complete original path used
when no slash exists. Archive read NULL or size zero does not select disk.
The optional output buffer is forwarded only to the archive reader.

Without an archive, both helpers construct a real CPbgFile, call Open with
g_PbgFileOpenModes[0], and ignore the boolean result. The mode pointer at474908
names `r` at46F2C0. Read calls GetSize and ReadWholeFile with that maximum;
GetSize returns the file size. Automatic destruction runs on normal exit,
including a failed Open. There is no new early Open-failure guard or disk use
of the optional archive buffer. Buffer overflow, invalid archive state and
Windows/engine runtime behavior remain outside the host validation claim.

The existing ArchiveOrDiskPathView declaration moves from ResFile.cpp into a
shared header. It describes the physical ECX path and Read's stack buffer;
it does not establish an original class or object lifetime. A nonvirtual
read-only PbgArchive filename accessor exposes the already recovered member
without changing fields, size or virtuals. Original class names, source-file
partition and compiler flags remain unknown. The target File.cpp string has
no code xref and is not used as original-TU evidence. Adjacent TH09/TH095 pack
source was read as hypotheses; every conclusion above uses TH10-local evidence.

## Evidence and completed checks

Fresh local Ghidra attests all412289 mapped text bytes before/after readOnly,
noanalysis and rollback with one CPU/768MiB. All three helpers decompile and
decode over their complete reviewed extents. Ghidra lacks a function record
at436400; the initial strict caller query failed, then the final query retained
raw caller context. No database metadata was saved. Its46-byte caller is
independently exact under the normal-COFF Oracle; Ghidra names/types are provisional.

Two actual four-TU VC7.1 `/GL /GS /EHs` diagnostic builds succeed: all-GL, then
normal-COFF ResFile with GL FileSystem/PbgArchive/PbgFile. The latter partition
uses the already exact normal-COFF caller. Both produce helper extents220/236/242
versus212/214/190. These remain candidates. Positional diagnostic scores are not
semantic field bindings and contribute no exact or partial credit.

Three selected canonical units independently cold-replay exactly: memory Open46,
PbgArchive constructor16 and entry size51, **113 bytes/all3 fields**. An additional
same-image caller guard in the new normal-support graph passes its complete46
bytes/all2 call fields. This guard creates no new exact claim or original GL/TU
ownership inference. No broad historical replay is claimed for shared headers.

The actual routing source passes112 ASan/UBSan host assertions and eight compiled
strict functional controls: slash selection, filename comparison, correct array
element, buffer forwarding, no archive-error fallback, unguarded Open flow,
disk size and entry suffix. The fixtures explicitly model archive/disk dependencies;
they prove no target ABI, class lifetime, Windows API or engine runtime behavior.
The existing statistics-save baseline also passes229 scenarios/all128 masks.

All110 CI groups pass with all371 frozen code/test/config inputs unchanged.
Tracking and the FileSystem source-presence gate pass: all13 authored owners now
have source. Authored source is613/247945 bytes, nonexact212, absent121; exact
remains401/73225 and overall1242/139735. Unknown origins524 keep the whole-game
denominator unresolved. [Validation summary](validation-summary.json).

## Native follow-up and recovery

Actual native calls distinguish the next codegen routes. Read has two archive
reader call sites versus target's one and retains an out-of-line CPbgFile
destructor. GetSize directly calls stricmp inside its owner, consistent with
entry-size/lookup inlining, while target calls434EF0; its destructor also stays
out of line. FindArchive has branch/copy/alignment differences. Investigate
these target-supported call and lifetime shapes without forced private registers,
padding, fabricated returns or a normal-COFF substitution for the GL input.
Every future exact claim still needs complete field/EH binding and cold replay.

Private recovery is `.analysis/gpt-6.1-sol/filesystem-archive-routing-20261022/`.
Retain the normal-caller carrier, producers, compact results and current input
pins; superseded all-GL diagnostic output can be cleaned after its facts are
retained. The ten-TU large-owner32 source/header and15 artifact pins freshly
remain unchanged; no new ANM/ECL/Enemy build or comparison is claimed. The old
broad107-native-input inventories are historical after this source change;
current source has108 files. Independent D3D files and provider/legacy state are
preserved. Windows i386 product/runtime closure, semantic reconstruction and
portability remain open or not started as before.
