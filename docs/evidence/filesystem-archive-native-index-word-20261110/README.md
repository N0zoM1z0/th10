# Archive search: native DWORD array index advancement

Target is the Japanese TH10 v1.00a executable with SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

ArchiveOrDiskPathView::FindArchive, original target 0x004356F0 (212 bytes), scans g_PbgArchives using a signed index bounded by g_PbgArchiveCount. Retail uses a native 32-bit increment before the signed loop test. The earlier C++ computed an unsigned increment but converted the result to signed int, which is implementation-defined when the high bit becomes set. The revised C++ increments the same signed int index through its corresponding unsigned int lvalue. This preserves the native low DWORD, the signed comparison, the existing pointer scan and the exact archive name construction; it adds no range check or artificial calling convention.

Two genuine MSVC 7.1 SP1 four-TU /GL /GS /EHs archive/disk caller links compiled baseline and the native-index source. Both reproduced the complete 212-byte FindArchive contribution with 184/184 original non-address bytes and all seven independently bound original code/data fields. The same diagnostic link is not used to certify the 214-byte Read or 190-byte GetSize because those owners require their own /EHsc and PbgArchive /Ob1 profiles. Their canonical source-specific exact replays are the proper controls.

Local source trial: .analysis/gpt-web/20261110-filesystem-archive-index-words.py and build/gpt-web/20261110/filesystem-archive-index/results.jsonl. No new exact-ledger codegen credit, no modified target bytes, and no historical full cold replay.
