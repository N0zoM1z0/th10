# GUI message Run: reviewed fixed native baseline

This is a successful, source-bound code-generation observation of a behavioral,
non-exact candidate. The linked image was never executed. No new exactness,
runtime closure or acceptance credit is claimed. One parent-owned attempt was
recovered after a transport timeout; no rebuild or profile/TU sweep occurred.
See GUI_MESSAGE_RUN_NATIVE_REPRODUCTION.md for the tracked driver, input pins,
actual graph and pinned Python commands.

## Complete owner evidence

Same-start map symbols and PDB section contributions establish:

| Owner | Candidate inclusive extent | Complete bytes | Target bytes |
|---|---|---:|---:|
| Run | 4017F0–401CCB | 1,244 | 4,480 |
| UpdateStageElements | 401D40–402BFB | 3,772 | 3,836 |

Run contains 1,146 densely decoded executable bytes, two alignment bytes and a
96-byte, 24-entry opcode table at 401C6C. Every destination is a valid instruction
start inside the owner; each pointer has one HIGHLOW relocation. The next owner
401CD0 is excluded. The updater contains 3,741 executable bytes, three alignment
bytes and a 28-byte, seven-entry table at 402BE0, also fully destination/relocation
checked. Total owner relocations are 51 and 50 respectively.

The target Run extent 415E90–41700F remains all 4,480 bytes: 4,306 executable,
two alignment and 172 selector bytes across the 24/7/7/5 tables. Candidate opcodes
14/15 share one destination. Candidate stage/difficulty selectors use separately
owned static lookup arrays and shared calculations instead of three target
owner-local dispatch tables. Those .rdata arrays do not extend Run.

## Actual caller, factoring and ABI

The actual updater edge at 40259E calls Run at 4017F0 after placing the message
in EAX. Candidate Run returns with plain RET; the target passes the message on
the stack and returns RET 4. This is an observed fixed-graph LTCG calling-
convention mismatch, despite the source's stdcall spelling.

Naturally outlined source helper contributions include CreateGuiVm (85 bytes),
InterruptVm (48), DrawMessageText (63), DrawDecodedMessageText (80), SetMessageSide
(114), SetPlayerPortraitSprites (242), SetEnemyPortraitSprites (267), AddStageClearScore
(184), IncrementStageClearCount (60), and CompleteStageFromMessage (544).
Their static-map identities and PDB contributions were checked. The target Run
contains that control flow within its own owner rather than calling these source
helper boundaries. Disjoint helper sizes must not be summed into Run or credited
as matching fragments. InterruptVm's mere presence does not establish a Run call;
the candidate directly specializes those interrupt routes.

The pending WAIT path skips instruction tick/advance. Future-time exit calls the
real timer Tick. END clears the relevant state and returns -1 without tick or
advance. The updater distinguishes pending return from END, reloads its cleanup
receiver, performs VM deletion work, calls anchored operator delete and clears
the message slot. This is codegen evidence, not executable cleanup validation.

## Real dependencies and unresolved boundaries

All ten original TUs and all 43 pinned inputs match. Every required-real symbol
has a same-start executable contribution from its original TU, and none is an
unresolved anchor. Actual paths include real specialized interrupts, child search,
ANM allocation/initialization/insertion/lookup/position/deletion, sprite methods,
DrawTextLeft, timer SetCurrent/Tick/Add, score Add and supervisor publication.
Music uses the real FrontEndPlayMusic and Sound.cpp queue chain. Accepted decoder
and advance leaves remain in their original full th10::leaf TU, with 41-byte and
32-byte contributions; their raw-address/private-ABI limitations remain.

The optimized DrawDecodedMessageText owner retains its cached manager across the
real decoder even for the source reloadManager=true route. LTCG can see that
actual decoder body. Host synthetic callback observations therefore do not prove
identical native reload instructions. This is not itself a demonstrated production
semantic failure and does not justify volatile or opaque-source workarounds.

Three direct source-empty boundaries remain actual data-anchor calls: transition,
scaled audio and effect. Candidate transition pushes a cdecl receiver instead of
target EDI; effect pushes all six logical inputs instead of target EBX priority
plus five stack inputs. Scaled audio has a declared float stack input but no body.
These private-ABI/runtime obligations remain unresolved.

The full graph has 443 unresolved symbols: 204 globals, 47 imports and 192 other
function/data symbols. This is not a Run-only reachability count. CRT, graphics,
OS, operator delete and unrelated-owner anchors remain. The constructor anchor
belongs to unrelated retained ECL code, not updater Run/free flow. Synthetic global
storage and raw-address leaves cannot prove physical alias identity or runtime
layout/availability. The image must never execute.

## Artifact identities and evidence scope

Generated artifacts live under ignored build/diagnostic-gui-message-run-updater;
logs and detailed review JSON are local evidence rather than committed files.
Map timestamp/image base and PE CodeView/PDB identity were checked together.

- run-build.json: 818118b58fbfe1fb60078d21610c92f4494d9e69eef19df0db4443669e672d86
- source.exe: d216070b6425c3a2714e4092caf6bd4ca4e5b240617601e5ab7c4c89f458d02c
- source.map: 78f529966d932d21d755a9f56739ffa3d0cd37e23d8647d594c8bb148027c87a
- source.pdb: 23de9aa4e564c514c28116decfa9fb1a36824b7fa7e98c625685c13a9c994f36
- Complete candidate Run: 003648fe670d96b9f9f06b8f963acb6e1e0512f3e5b0494f8fbe66f3a57221a1
- Complete candidate updater: f348f7b88ef56692925e5b234041f88f8dc599bd4f6a1132aa8e6b6683a174bd
- Target Run owner: 13c80eaec4d7e90718b107f1e37ea8de7bd57d8ac3f86f0af4b6269644614c35

Independent read-only review report SHA256:
2149d3a2ea8f1fad53ab469cede9bc9847b6634d2b8fe1b53c7acc060bf508e0.
Durable review commands include 5b60645cff3343abbe38f43a690d35f4 (identity/pins)
and 90a9061a6dea48948070253d79cfbea3 (complete retrieved helper/Run decode).
The source ledger remains behavioral/decompiled at 0.00 match. Canonical affected-
producer replay and full CI are separate gates; this review supplies no outcome
for them. Any future control-flow reconstruction needs its own target-backed
source justification and review, not an instruction to force helper inlining.
