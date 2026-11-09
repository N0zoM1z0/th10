# ECL script database: native low-DWORD counters

Original Japanese TH10 v1.00a, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The complete EclVmScriptDatabase::AddScriptData at 0x00450220 is 589 bytes. It merges a serialized subroutine name table and updates six integer count/index words. The previous C++ computed the right unsigned low-32-bit result but narrowed it back to int, which is implementation-defined for words above INT_MAX. This revision uses the corresponding unsigned int lvalue of each signed int, preserving the original modulo-32-bit update and the original signed comparisons without that conversion. It does not change read order, sorting, allocation, cursor behavior, bounds checks, ABI or target machine code.

The real VC7.1 SP1 /GL EclVm/Enemy graph was used for baseline, six single replacements, counter-only, index-only and all six together. Each retained the complete 589-byte AddScriptData with 502/581 ordinary bytes and two original-bound CRT REL32 calls: malloc +0x6C targeting 0x00452706 and free +0x101 targeting 0x00452422. This owner stays NONEXACT with no exact credit. The same graph keeps the complete 7,020-byte Run with 6,997/7,020 matching bytes and all 189 original-bound fields, and previously exact 136-byte FindSubroutine.

Source probe: .analysis/gpt-web/20261109/ecl-script-table-counter-native-words.py
Candidate receipts: build/gpt-web/20261109-live/ecl-script-count-words/results.jsonl
Reproduce with scoped ecl-vm-find-subroutine, ecl-vm-read-float-value and ecl-vm-host-run exact-unit replays, plus strict complete 589-byte and 7,020-byte comparisons. No full historical replay or runnable game claim.
