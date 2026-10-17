# Fresh v2 dependency boundaries

Complete source owner under review:415E90..41700F,4480 bytes including4306 executable bytes,2 alignment bytes and172 table bytes. All24 opcode arms stay in one maintained Run owner.

Retained real dependencies: AnmVmIdView SetInterrupt/SetSprite/SetSpriteWithAnm; ANM FindVm/SetVmPosition/MarkVmForDeletion/CreateVmVariant0/DrawTextLeft; timer SetCurrent/Tick/Add; GuiScoreView Add; MainSupervisorView SetNextGameMode; EnemySoundQueueView QueueSoundSample; PlayerSetShotVmInterrupt2; EnemyMarkPendingInterrupt; ResultMenuPendingThree/ResultMenuFindScript; FrontEndPlayMusic. Definitions and declarations must be verified as externally linked in their original TUs before native use. No wrapper/copied body gains source credit.

Accepted leaf owners th10::leaf::Fn00417010(unsigned char*) and Fn004175E0(unsigned char*) remain in DecomphelpLeafAccessors.cpp and retain their prior bindings, raw-address and private-ABI limitations. Decoder and stage-advance host outcalls can establish call inputs/order only. They cannot establish the real decoder algorithm, stage increment/cap or descriptor publication.

Three genuinely source-empty interfaces remain declared, without fake implementations:
- GameCompleteStageTransition(GameChainOwnerView*),423370; Run supplies g_GameOwner477830, native EDI receiver, no stack argument.
- StageCreateTransitionEffect(priority,kind,frames,u32,u32,u32),43C8B0; existing StageDraw logical interface. Run inputs0x31,5,120,0,0,0. Native priority is EBX0x31; no forced ABI is introduced.
- int __stdcall GameQueueScaledSoundCommand(float),420C30; native float input/ret4, scales/truncates and queues command5, returns0.

GameStageUpdateView is promoted from GameManager.cpp to GameStageDescriptor.hpp, retaining musicIndex+24 and exposing neutral value28+28 plus final4bytes to known30hex stride. Opcode18 uses value28. No semantic identification as musicIndex is claimed for+28.

Canonical Player resource and EnemyManager effectResources[2] are used in Run; legacy updater shadow views remain unchanged below the updater marker. Live owner/cursor reloads and cached managers follow target call order. Synthetic callback tests, if provided, establish fidelity under their explicit contracts, not production reentrancy or normal-play reachability of unusual scalar cases.

Host methodology must inventory all constructor/destructor/resource/render/decode/advance/transition/SDK seams. Full canonical header use and sanitizer passes do not establish native layout, physical data aliases, raw-integer constructor correctness, private ABI, runtime closure, binary exactness or leak freedom.
