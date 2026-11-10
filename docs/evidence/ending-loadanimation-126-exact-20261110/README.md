# Complete 126-byte EndingTextVmView::LoadAnimation source recovered

Target: Japanese TH10 v1.00a, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040. Original address 0x0040C3C0 and complete original PDB owner size 126 bytes. The function receives its instance in EBX and ends in RET.

## Source and actual byte match

The previous maintained source called EndingRecordInt(instruction) + 29 to fetch the script number. Although that helper returned the correct 4-byte serialized word, VC7.1 selected MOV EAX,[EAX+4]; LEA ECX,[EAX+29] at +0x3F. The original instead emits MOV ECX,[EAX+4]; ADD ECX,29. A direct, natural typed read from the actual serialized instruction DWORD changes optimizer scheduling without any artificial ABI or register control. The exact source is now a PreloadAnm call with the index expression *reinterpret_cast<const int *>(instruction + 4) + 29 and the existing stage path argument. The original instruction word is a four-byte value in the pinned x86 game ABI.

The immediately preceding source improvement moved acquisition of the auxiliary VM ID until after the resource store and flag clear. It transformed an earlier 131-byte function into a 126-byte function with 123/126 matching bytes, restoring the original direct AND [EBX+0x74],-5, ESI-held auxiliary ID, and correct interrupt/counter ordering. Together the two natural C++ changes restore every ordinary byte of the complete function.

The complete result is 126/126 byte-exact with these seven original-bound fields:
- +0x07, +0x18, +0x43 DIR32 g_StageFilePath, target 0x00497C38
- +0x2A DIR32 g_AnmRenderManagerView, target 0x00491C10
- +0x4B REL32 AnmRenderManagerView::PreloadAnm, target 0x00447280
- +0x57 DIR32 g_AsciiManagerView, target 0x004776E0
- +0x6F REL32 EnemyMarkPendingInterrupt, target 0x00409E50

A fresh pinned VC7.1 SP1 21-input EndingTextRun diagnostic compiled eleven separate natural C++ call argument expressions and compared full original extents. Several retained size 126 with three or four ordinary differences, and one volatile form emitted 143 bytes. Only direct serialized DWORD access produced structural-exact; a separate manually authored seven-field strict comparison then confirmed exact 126/126. The independent canonical unit is ending-text-load-animation, with ending-text-run (1568 bytes, 92 fields) and ending-load-animation-callback (18 bytes, 2 fields) as related guards.

Trial: .analysis/gpt-web/20261110-ending-script-index-expression.py. Private receipts: build/gpt-web/20261110/ending-script-index/results.jsonl. No hand-patched bytes, inline assembly, fake calling convention, original source identity or executable runtime equivalence is asserted. The owner entered the accepted exact ledger in commit 558e58a after two independent successful 126/126, seven-field selected-unit cold replays, a separate 1568/1568 Ending Run guard, and passing CI. Successful local receipts were saved as .analysis/gpt-web/20261110-ending-loader-only.json and .analysis/gpt-web/20261110-ending-loader-only-third.json; intervening Wine LNK1104 crashes are build failures, not additional proof.
