# ECL table merge: signed interpretation and predecrement of native words

Target: Japanese TH10 1.00a, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

In EclVmScriptDatabase::AddScriptData at 0x00450220, the old-table count is the low 32 bits of subroutineCount - addedCount, but the existing C++ converted that potentially out-of-range unsigned result to a signed integer. The revised code writes the result through the unsigned int view of the actual int sortedCount, then uses the original signed comparisons unchanged. The backward insertion loop similarly decrements the existing signed index through its unsigned counterpart and returns the same signed value from an inline helper for the loop condition. This preserves i386 modulo-32-bit behavior without implementation-defined signed narrowing.

Genuine VC7.1 SP1 /GL EclVm.cpp + Enemy.cpp experiments compared baseline, each change individually, memcpy storage, and their combination. Both the direct and helper-based formulations emitted the complete original-sized 589-byte AddScriptData; its two original external CRT call offsets remain +0x6C (malloc) and +0x101 (free). The existing candidate still has 510/589 target-matching bytes after accounting for those fields; no exact credit is added. The same graph preserves the original 136/136 FindSubroutine and the complete 7020-byte Run, all 189 original-bound fields and 6997/7020 matching bytes, with the existing 23 mismatches unchanged.

Local trials: .analysis/gpt-web/20261109/ecl-addscript-native-negative-index.py and build/gpt-web/20261109-live/ecl-native-negative-index/results.jsonl. Verify the installed source with selected exact ECL units and the two complete original-bound nonexact functions. No whole-game replay is needed.
