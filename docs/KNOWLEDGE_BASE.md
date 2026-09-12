# TH10 verified knowledge

This file stores durable TH10-scoped facts and important negative results.
Cross-game promotion is a later Factory review, never an automatic game action.

| ID | State | Fact | Evidence |
| --- | --- | --- | --- |
| TARGET-001 | observed / corroborated | The supported executable is original Japanese TH10 v1.00a, size 487,936, SHA-256 `2f14760b...dc9040`. | Direct hash/PE inspection plus pinned thcrap version database |
| PE-001 | observed | The target is relocation-stripped PE32 i386 at image base `0x00400000`, entry `0x004537DC`, with four sections. | `scripts/verify-target.py`; `config/target.toml` |
| TOOLCHAIN-001 | inferred | Linker 7.10 and dominant Rich build-6030 records support a VC7.1 SP1-era compiler-family hypothesis. | Direct PE and Rich-header decoding |
| TOOLCHAIN-002 | unknown | Exact compiler binaries, flags, TU partition, libraries, resources, and link order are not established. | Explicit open fields in `config/build.toml` and `config/tools.lock.toml` |
| ANALYSIS-001 | observed | The private Ghidra project is bound to the target by SHA-256/MD5, image layout, entry function, and distributed mapped-byte samples. | `python3 scripts/ghidra.py check`; Factory `th10-ghidra` check |
| INVENTORY-001 | inferred | Ghidra supplies a provisional navigation inventory; individual boundaries and origins remain unreviewed. | `config/functions.csv`; `config/function-origins.csv` |

Do not turn cross-game similarity, a decompiler type, or an attractive reused
name into a TH10 fact without target-local evidence.
