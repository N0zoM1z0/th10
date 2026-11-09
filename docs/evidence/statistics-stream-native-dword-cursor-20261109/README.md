# Statistics loader: native DWORD remaining-length and file cursor

Target: Japanese TH10 v1.00a, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The complete 423-byte GameStatisticsOwner::Load42B030 parses score sections from the original record stream. At each iteration it subtracts the untrusted unsigned record length from the remaining signed byte count, checks the signed result for a negative value, and advances a Win32 data cursor. Retail i386 uses low 32-bit arithmetic for both operations, including malformed lengths and wraparound. The old C++ relied on implementation-defined unsigned-to-signed conversion when storing the subtraction into int, and on potentially out-of-range C++ pointer arithmetic when advancing cursor.

The new natural C++ decrements the same signed remaining variable via its corresponding unsigned-int lvalue, then preserves the original signed negative test. It increments the cursor by casting its flat Win32 address word to unsigned int and adding record->unknown08 before reconstructing the pointer. No new range check, pointer sanitization, exception handler, memory allocation, or branch was introduced.

Six real VC7.1 SP1 six-source /GL /GS /EHsc /Gz graphs (GameStatisticsLoad plus GameStatistics, FileSystem, normal-COFF Decompress, RandomMath and GameStatisticsRecordDefaults) compared baseline, each change individually, both combined, memcpy result storage and an unsigned temporary lvalue. All generated the same complete 423-byte loader, with every original-byte comparison equal after all five independently bound REL32 fields. The paired 98-byte factory and 137-byte constructor remain nonexact and byte-neutral in the same graph. Evidence: build/gpt-web/20261109-live/stats-native-stream/results.jsonl.

Replay only game-statistics-load, game-statistics-finalize and game-statistics-initialize-tail (already exact), and inspect the resulting loader and factory PDB functions. No complete-game cold replay is required. This recovers source-level low-word behavior but adds zero new canonical exact bytes.
