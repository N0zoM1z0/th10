# TextRenderBufferView::InvertAlpha: native signed bitmap row-byte count

Target: original Japanese TH10 v1.00a, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The 157-byte native InvertAlpha owner at 0x00437160 multiplies an integer bitmap pitch by the processed row count using the low 32 bits of an i386 IMUL, then makes signed comparisons against pixel byte offsets. The existing maintained source already used unsigned multiplication to avoid signed overflow but converted the result back to signed int using static_cast<int>, which is implementation-defined for words above INT_MAX.

The new code computes the unsigned native row-byte word and copies its 4-byte representation into the signed totalBytes with standard C++ memcpy. Thus signed comparisons and the original unchecked malformed-count behavior remain exactly as in the target model. It does not sanitize the bitmap extent, introduce overflow checks or modify the A8R8G8B8, A1R5G5B5 or A4R4G4B4 channel operations. This byte copying is a source representation clarification in the pinned 32-bit/two's-complement execution model, not a general portable bitmap format.

Six independent real pinned VC7.1 SP1 normal-COFF source forms compared the original cast, memcpy, unsigned-alias pointer, unsigned-alias reference, union, and reversed multiplication order. Every full InvertAlpha owner was exactly 157 bytes, had zero relocation fields, and retained the same 112/157 raw matching bytes and same 45 mismatch locations. The outstanding register allocation differs (target EAX row-byte count and EDX pixel base, candidate EDX and ESI), so InvertAlpha remains NONEXACT and has zero new codegen ledger credit.

Only affected previously accepted text buffer owners are replayed: text-buffer-apply-alpha-bleed, text-create-buffer, text-try-allocate-buffer, text-release-buffer and text-fill-color. Combined they account for 2,327 already exact bytes. The trial sources/receipts are private at build/gpt-web/20261109-live/text-invert-row-word/results.jsonl. Target EXE bytes, linker profiles and machine code were not modified; neither full historical cold replay nor runnable game test is asserted.
