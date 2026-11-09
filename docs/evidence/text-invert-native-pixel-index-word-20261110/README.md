# Text bitmap alpha inversion: native DWORD loop indexes

Japanese TH10 v1.00a original target SHA-256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

TextRenderBufferView::InvertAlpha at 0x00437160 processes GDI bitmap alpha in three pixel formats. The A8R8G8B8 byte loop advances index by four; the A4R4G4B4 byte loop advances index by two. At the edge of a large positive row length, the final increment can set the high bit of the 32-bit index. The retail x86 ADD always produces the low DWORD bits, after which a signed comparison controls the following iteration.

Previous C++ computed each increment in unsigned arithmetic and then applied an out-of-range signed narrowing conversion. The maintained source now adds directly through the existing signed int index's corresponding unsigned int lvalue, preserving the original low DWORD and signed loop comparison. No additional bounds check, altered pixel data access, code patch, artificial register assignment or operand evaluation order is involved.

A focused pinned VC7.1 SP1 normal COFF sweep compiled baseline, two individual DWORD updates, both combined and the native subtraction expression for the A1R5G5B5 count. All compilable ordinary variants produced the same complete 157-byte InvertAlpha and zero relocations with the previous 112/157 byte comparison. The 45 differing bytes remain in the pixel-pitch/receiver register allocation, not in the loop-index addition. Two unrelated statement-body variants did not compile and were excluded. The 554-byte exact TextHelperView::RenderTextToTexture owner is checked separately in its canonical hybrid LTCG/COFF graph.

Trial and evidence: .analysis/gpt-web/20261110-text-index-native-word.py and build/gpt-web/20261110/text-invert-index-word/results.jsonl. The nonexact InvertAlpha must not be admitted to the exact ledger on this evidence. Scope validation only to the affected text owner and exact text-render-main, not the full cold replay.
