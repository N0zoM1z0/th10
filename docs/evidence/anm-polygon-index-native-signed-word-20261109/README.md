# ANM ExecuteScript: 32-bit generated-polygon loop index

Original Japanese TH10 v1.00a target SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The complete 9,964-byte AnmRenderManagerView::ExecuteScript at 0x0043EE30 contains a generated-polygon loop in render mode 9. Its script-controlled integer vertex index increments as a native low-32-bit DWORD. Earlier maintained C++ computed the correct unsigned increment but converted the result to signed int using static_cast<int>, whose out-of-range result is implementation-defined.

The selected source passes the unsigned increment to the small inlined AnmSignedWordBits helper, copying four bytes through memcpy into the signed local. This preserves the original signed bit pattern without signed overflow and leaves the loop's signed comparison, target pointer increments, x87 operations and existing distinct vertexCount - 1 evaluations intact. No target bytes, compiler profile, register assignment or artificial ABI are modified.

A pinned VC7.1 SP1 /GL /GS five-source ANM link compared baseline, all three vertex-count decrement uses, index increment only, and their combination. Index-only retained the entire PDB contribution (9,964 bytes), all 350 independently original-bound fields including 95 local destinations, and 9,962/9,964 original bytes with the same opcode91 differences at +0x113,+0x115. Vertex-count replacements changed function extent or linked field geometry and were rejected. Occurrence-specific angle, texture and bound experiments confirmed those expressions are not currently codegen-neutral. The 73-byte CreateVmVariant2 and accepted ANM variable helpers are replayed from installed source.

Evidence: build/gpt-web/20261109-live/anm-polygon-signed-word/results.jsonl and original-bound link under index-bitcopy. The boss remains NONEXACT; no new exact-ledger credit or whole-game cold replay.
