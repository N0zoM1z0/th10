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
| MAIN-001 | observed / corroborated | The reviewed application entry at `0x00438AD0` spans through `0x0043903C`, has a unique CRT caller, consumes the four WinMain stack arguments, and returns with `ret 0x10`. The next three bytes are `0xCC` padding. | Attested Ghidra caller/disassembly queries; direct target disassembly; pinned VC7.1 ABI probe |
| MAIN-002 | observed | The `0x00438AD0` application entry owns the D3D9 cooperative-level/reset message loop, restart path, window teardown, configuration writeback, and process-level cleanup visible in the target. | Attested Ghidra decompile/callees; direct target disassembly and target strings |
| MAIN-003 | observed / inferred | `0x00438A30` is a reviewed 43-byte authored resource-release loop used by two target-local callers. Both callers pass the object in EAX; the function releases and clears 32 COM resource pointers. This is evidence for an interprocedural/private-ABI optimization context, but does not by itself prove which Rich-record input or TU owns it. | Attested Ghidra callers/disassembly; direct target boundary inspection |
| MAIN-004 | unknown | Code at `0x00439040`, `0x00439060`, `0x00439080`, and `0x004390A0` follows WinMain padding but is not represented as Ghidra functions and has no direct Ghidra or literal-VA xrefs. Its cleanup-funclet/compiler ownership remains unresolved, so it is not added to the function denominator yet. | Attested Ghidra function/xref queries; bounded raw target VA scan |
| CRT-001 | observed / corroborated | The target entry `0x004537DC` is VC runtime startup code rather than authored game logic: it initializes runtime state, argv/environment/heap/thread/IO support, constructs the WinMain argument set, calls `0x00438AD0`, and exits through CRT routines. | Attested Ghidra disassembly/decompile/callees plus direct target inspection |

Do not turn cross-game similarity, a decompiler type, or an attractive reused
name into a TH10 fact without target-local evidence.
