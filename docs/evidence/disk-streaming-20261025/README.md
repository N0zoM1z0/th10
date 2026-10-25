# Disk streaming: first complete owner and constructor visibility

2026-10-25, source HEAD 7e60288. **Diagnostic proposals only.** No maintained
source mapping, canonical unit, authored exact/source credit, origin row or
denominator changes. Native product/runtime/semantic/port gates remain open.

## Scope and first comparison

Raw startup call 41FA7A (within 41F990) enters actual disk caller43D690, which
calls owner44C8F0. This is not the existing memory LoadBgm caller.
Natural Sound+ZWave C++ uses the existing /GL /GS /EHsc graph and disk caller
as diagnostic entry. No register ABI declarations, new noinline, padding,
inline assembly, downloaded SDK source, production source edits or flag sweep.

| Complete target | First model | Constructor-visible model | Result |
| --- | ---: | ---: | --- |
| disk CreateStreaming44C8F0,702 | 681 | 681 | structural rejection |
| actual disk caller43D690,241 | 249 | 249 | structural rejection |
| CWaveFile::Open44DBF0,173 | 173/all3 | 173/all3 | diagnostic code exact |
| CSound constructor44CE80,118 | declaration absent | 118/all3 | diagnostic code exact |
| CStreamingSound constructor44D6A0,109 | unresolved declaration | 109/all4 | diagnostic code exact |

The first complete parent includes all702 target bytes, including eleven
internal alignment bytes44CAC5..44CACF. Its raw inventory is13 link fields,
21 local branches and15 calls; actual caller has10 fields/3 locals/4 calls.
Target PE lacks base relocation entries: raw operand inventory supplements
the generic linked-image reader. Do not interpret its target field counts
of9/2/0 as complete parent/caller/Open inventories.

The first image proves Open173 against all three bound fields and six local
branches. It preserves Open's private ESI receiver/EAX flags/ECX filename/EDX
format naturally. Parent naturally recovers ECX output/EDX flags and ten stack
dwords, RET40. Disk entry remains ordinary ECX rather than target private EAX:
startup context was not expanded. Parent's constructor call initially uses
ECX because the definition is unresolved, not a forced ESI declaration.

This concrete missing definition justified exactly one separately reviewed
constructor-visibility model. Ordinary base chaining recovers full base118
and derived109, including ESI derived receiver/four stack arguments/RET16.
The parent changes only one non-link byte (offset266h, mov ECX,EAX becomes
mov ESI,EAX) relative to the first model; caller and Open are normalized-byte
unchanged. Parent's early epilogue/tail merging/alignment still reject.
Neither reviewer found a grounded next semantic correction. Park the parent
and caller; do not reopen with register/spelling/profile/context panels.

## Semantic facts retained

- Disk caller copies path before manager/directSound validation, calls StopBgm,
  then captures the first format row. DWORD multiplication/division/remainder
  intentionally wraps. CreateEvent/CreateThread results are unchecked.
  Parent receives original path, not copied storage. No indexed-slot publish.
- Open stores flags, clears memory-mode, handles only flags==1, checks null
  filename, opens sequential read/share-read, preserves ResetFile(false)'s
  post-call alias rereads, and returns S_OK for other flags.
- Parent treats **any non-S_OK Open result** as failure; deletes the wave only
  there. CreateSoundBuffer/QueryInterface failures leak it as observed.
  Notification allocation failure does not release the notification interface.
  SetNotificationPositions failure releases notification and deletes its array.
  No RAII safety repair was introduced. GUID locals, DWORD overflow, memcpy
  description and separate output-pointer rereads are preserved.
- Base allocates/copies buffer pointers before size/count/wave stores, fills
  the first buffer unconditionally (even count0), rewinds each buffer using
  the incoming count and repeated member-array reads, then clears playing.
  Derived specializes one buffer, initializes5C/60/64/68 and notifySize6C;
  it does not fabricate initialization of fade/manager/description/event/lock.

## Code, EH and data are separate

All400 dependency code bytes are complete/all-field exact-shaped diagnostics,
not three admitted authored owners. Base has four fixed locals; derived none.
Independent reviewer reproduced normalized raw equality, not merely sizes.

Parent first/second EH retain one unwind action for the streaming allocation
only, reading [EBP-48], calling operator delete; no wave RAII or try blocks.
Target handler46533B loads FuncInfo471398:
[19930520,1,471390,0,0,0,0], unwind[-1,465330].
First native handler4039BB/FuncInfo404344/unwind40433C/action4039B0;
second403AAB/40434C/404344/403AA0. Handler tail binds CxxFrameHandler.
Only the seven FuncInfo words are claimed; following data are not part of it.

Vtable code references bind base4705D8/derived4705D4. Separately observed target
slots point44CF00/44D710; second native404284/404280 point402ED0/4033D0.
That observation does not prove vtable data ownership, destructor closure,
original TU layout or full product linkage. No vtable/data exact claim.

## Provenance and acceptance

Existing authored_game/medium ledger rows derive from behavior/direct-call
anchors, not an independent source-origin proof. DSUtil/DirectSound sample
ancestry is plausible but unproved. Bounded permitted local SDK search found
headers/libs, not DSUtil source. No raw SDK acquisition or download occurred.
Keep origin uncertainty separate from complete diagnostic byteproof and leave
classification/denominator unchanged. Admission requires a separate origin
decision and canonical reproducible source/current proof; this pilot does not
make that decision.

## Validation and retained evidence

Both cold native models exit0 under oneCPU/nice10/180s bounds. Same-image
StopBgm176/all4, ReopenBgm55/all2 and Thread189/all7 pass in both images.
Separately, all three original canonical contexts cold-pass420/all13.
The canonical aggregate's transport returned no result; terminal exit0/logs
and three exact JSONs were recovered without rerunning or duplicate jobs.
No new full-CI claim for unadopted source hypotheses.

- Ignored campaign: .analysis/gpt-dots/disk-streaming-20261025/
- Images: build/gpt-dots/disk-streaming-{first,constructor}/
- First image SHA360f125b647c3ff4ba891db0193e4fd1b060879884cc6e05ed04f2c63e7cb727.
- Second SHA6c8787c03c73441ef116490694a6d7fbf936aa59f71b949b8fe606f95c492de1.
- First85 artifacts frozen before second model; tracked/protected input hashes
  preserved. HEAD unchanged; upstream ahead3 changed externally to0 during
  second build. This task did not fetch or push.
- Raw owner/startup audit: repository-command:456a22c1b600488282838b2d7ae36af8.
- Raw constructor audit: repository-command:8b412355c14f4b0ab69dc68555ca39f8.
- First native: repository-command:c40e8e4d8e654f039a3b7e97d3194a50.
- Second native: repository-command:b47f56eb3b494f998d2136c9f8f53598.
- Independent ctor source review: repository-command:920602ca520a484dabb2d9b1f600499f.
- Independent code equality: repository-command:5f4ab92475bf48de85cd916a722f8895.
- Independent EH: repository-command:e3743699c78649acabec64ca98f2b436 and
  repository-command:ad1e2c152ddd403d828687d6785aafcb.
- Canonical terminal recovery: repository-command:9645bf0bde0b46c88b29db469738be95.

diagnostics.json retains full field/local inventories and bounded code
comparisons without target byte dumps. The compact patches reproduce the
original model source byte-identically (verified for both models). They are
hypotheses, not production sources.

Reproduction (choose unique ignored model/build directories; run serially
with taskset, nice10 and timeout180s):
scripts/repo-python docs/evidence/disk-streaming-20261025/reproduce.py
--trial first|constructor --model-dir .analysis/UNIQUE
--output-dir build/UNIQUE
Use --prepare-only to verify source without compiling. It pins maintained
input hashes and target identity; a successful link alone grants no exactness.
