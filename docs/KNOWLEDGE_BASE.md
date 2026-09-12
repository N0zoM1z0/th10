# TH10 verified knowledge

This file stores durable TH10-scoped facts and important negative results.
Cross-game promotion is a later Factory review, never an automatic game action.

| ID | State | Fact | Evidence |
| --- | --- | --- | --- |
| TARGET-001 | observed / corroborated | The supported executable is original Japanese TH10 v1.00a, size 487,936, SHA-256 `2f14760b...dc9040`. | Direct hash/PE inspection plus pinned thcrap version database |
| PE-001 | observed | The target is relocation-stripped PE32 i386 at image base `0x00400000`, entry `0x004537DC`, with four sections. | `scripts/verify-target.py`; `config/target.toml` |
| TOOLCHAIN-001 | observed / corroborated | Rich product IDs 95, 96, and 100 at build 6030 record 131 normal C, 15 normal C++, and 52 LTCG C++ inputs. A separately pinned VC7.1 SP1 candidate reports compiler/linker build 6030. | Direct target Rich decoding; pinned `archaic-msvc/msvc710_sp1` commit; `scripts/verify-toolchain.py --execute` |
| TOOLCHAIN-002 | observed | The pinned candidate executes normal C/C++ COFF, C++ LTCG, resource compilation, and PE32 i386 linking under a game-bound headless Wine prefix. | `scripts/verify-toolchain.py --execute`; component hashes in `config/tools.lock.toml` |
| TOOLCHAIN-003 | unknown | Per-unit normal-COFF versus LTCG ownership, flags, TU partition, libraries, resources, and link order are not established. | Explicit open fields in `config/build.toml`; empty `config/match-units.toml` |
| ANALYSIS-001 | observed | The private Ghidra project is bound to the target by SHA-256/MD5, image layout, entry function, and distributed mapped-byte samples. | `python3 scripts/ghidra.py check`; Factory `th10-ghidra` check |
| INVENTORY-001 | inferred | Ghidra supplies a provisional navigation inventory; individual boundaries and origins remain unreviewed. | `config/functions.csv`; `config/function-origins.csv` |

Do not turn cross-game similarity, a decompiler type, or an attractive reused
name into a TH10 fact without target-local evidence.
