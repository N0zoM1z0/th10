# Fresh v2 independent source-only review

Status: approved for source-only recovery/review staging within the limitations below. No new build, test, native-ABI, exactness, or runtime-closure approval is implied. Historical lost-packet tests are not evidence for this revision.

Reviewed the four authoritative source files against fresh hash-pinned target decode in next-gui-owner-v2/decode.txt and fresh clean50b971f canonical headers/dependency sources. Source-only corrections requested and reread:
- Canonical Player::resource and EnemyManagerView::effectResources are void pointers; all conversion sites now explicitly cast to AnmLoadedView*.
- Canonical g_EnemyManager now has its necessary extern declaration.
- Ordinary-stage advance now uses existing g_GameStageState[] contract, not the eight-byte g_GuiScore subobject whose declared extent cannot support leaf access at+3c.
- Opcode2 selects resource only in the chosen branch, avoiding eager player access on special-stage paths and an unnecessary enemy-null prerequisite on fallback.

## Approved scope

Full Run/helper review agrees with the fresh target within valid live-owner/record domains: normal/END/WAIT controlflow; resource selection; selected-side positions with cached manager and live activeSide; character guards; portrait find/set/reload order; text lookup-before-decode and cached/reloaded manager asymmetry; music descriptor+28 and slot1; stage6 difficulty latch, priority0x31 effect and ending state; stage7 fixed bonuses and preserved ending fields; signed capped clear count; ordinary-stage cached GUI/delete/create/mode/advance order; dispatch-tail cursor reload.

Gui.hpp's byte alias and memcpy scalar accessors retain pointer identity/layout while removing fictitious record/int objects and unaligned typed reads. Preconditions remain live serialized allocation, sufficient valid header/payload/next record, correct target scalar widths/little endianness, valid text termination and bounded arithmetic. Not a malformed-buffer safety guarantee.

Profile count uses byte-copy to/from a live local int, not a reinterpret-cast int lvalue. Offset is P+(character*3+subtype)*437c+4d0+difficulty*4; adjacent unlock bytes retain target4e0/4e1+8*(stage+6*difficulty). This is sound under the explicitly live bounded byte-storage contract. Canonical GameStatisticsOwner target records start+8, placing count within record unknown4C8, but existing g_GuiPersistentData physical alias to canonical47783C remains unproved by these source declarations or fixtures. No new typed count layout is invented. A host canonical owner's widened pointer layout must not be confused with target+8.

GameStageDescriptor promotion preserves existing musicIndex+24 and adds distinct value28+28 with target offset/size assertions. GameManager normalized logical diff is descriptor header promotion only. Gui.cpp content from the updater marker onward is unchanged after newline normalization; this is not a byte-identical line-ending claim.

## Inherited and deferred limitations

Accepted leaf417010/4175E0 are direct existing symbol references, with inherited raw-global/private-ABI limitations. Transition423370, scaled audio420C30 and effect43C8B0 remain unresolved logical interfaces. Six-argument effect declaration preserves EBX priority plus five stack inputs semantically, without claiming private-ABI realization. External owner/resource global bindings remain separate integration obligations; equal layouts or host assignments cannot prove them.

Extra null guards retained outside native successful domain are not target guarantees. Invalid-character guards and nonstandard stage/difficulty combinations do not establish normal-play reachability. Callback perturbations can establish lifetime ordering, not production reentrancy. Stale text VMs/portrait parents and overflowing arithmetic are excluded. No v2 fixture or repaired execution evidence was reviewed here; profile and object lifetimes must be reviewed again when those fixtures exist. Constructor timer seams remain an explicit inherited limitation.

## Reviewed SHA256

- src/Gui.cpp: a40049671bce356ab130af52677c821c8e3dce41e706f4ceebf95a4a78751139
- src/Gui.hpp: bc6db49db4f9f3c180b1f3e89215cd2bb87bd767ea81539c77812afbfeb17361
- src/GameStageDescriptor.hpp: b9e03884d82ee00b113e821f835e5c017c7a861daf163aa858d288d33e5234de
- src/GameManager.cpp: e17146be734750f2d43fd16513be981dba3a025bcc5ea05b64ca6dcaff8c603c
