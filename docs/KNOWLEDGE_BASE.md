# TH10 durable reconstruction knowledge

This index retains target-observed boundaries, structure layouts, private ABI
constraints, canonical exact findings, and useful negative constraints. It is
**not** an experiment journal or a live similarity scoreboard. Older transient
trials were pruned from this document; their chronology is in Git history.

For current function counts use [PROGRESS.md](PROGRESS.md), generated from the
ledgers. For current non-exact owner diagnostics and next steps, use
[RE_HANDOFF.md](RE_HANDOFF.md). A matching size, old score, or commit does not
establish codegen exactness: only canonical replay can do that.

The ignored `build/` and `.analysis/` directories are disposable. Removed
historical scratch paths are not expected to exist; recheck a claim against the
target and maintained source before using it as an implementation constraint.

## Target

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| TARGET-001 | observed / corroborated | The supported executable is original Japanese TH10 v1.00a, size 487,936, SHA-256 `2f14760b...dc9040`. | Direct hash/PE inspection plus pinned thcrap version database |

## PE identity

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| PE-001 | observed | The target is relocation-stripped PE32 i386 at image base `0x00400000`, entry `0x004537DC`, with four sections. | `scripts/verify-target.py`; `config/target.toml` |

## Analysis

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ANALYSIS-001 | observed | The private Ghidra project is bound to the target by SHA-256/MD5, image layout, entry function, and distributed mapped-byte samples. | `scripts/repo-python scripts/ghidra.py check`; Factory `th10-ghidra` check |
| ANALYSIS-002 | observed | The Factory-native `th10-ghidra` provider binds the registered implementation files by aggregate SHA-256. Editing `scripts/ghidra.py` or a registered Java script deliberately makes provider discovery/check fail with “native Ghidra implementation differs from its operator binding” until the private operator binding is reviewed and refreshed. After adding address-set range export, the reviewed private binding became `c001f8ef3d9a64e96664d999c14891d9c92fa12f393bca64cc77bb087c0e2143`, and the Factory-native `check {}` again passed with `provider_transport=factory-native-command`. | Factory provider renderer and validator; private ignored `service.toml`; Factory-native `th10-ghidra` check |

## Inventory

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| INVENTORY-001 | observed / inferred | Ghidra supplied the initial navigation inventory, which has since been augmented by target review. Candidate entry, physical boundary, origin, source presence and exactness are independent claims recorded in separate ledgers; no Ghidra name or dense span alone establishes any of them. | `config/functions.csv`; `config/function-boundaries.csv`; `config/function-origins.csv`; `config/reccmp-functions.csv`; `config/matches.csv` |
| INVENTORY-005 | observed / corrected | Eight reviewed owners were recorded one or two bytes short because their ledger spans ended on the `C2` opcode of `RET imm16` and omitted its two-byte immediate. Their corrected complete extents are `0x00425070-0x0042508A`, `0x004296D0-0x004296EA`, `0x00443680-0x004436B3`, `0x00444DC0-0x00444E50`, `0x00444E60-0x00444F9A`, `0x00444FA0-0x004450DA`, `0x00447C80-0x00447EBF`, and `0x00448360-0x00448442`. The boundary audit now classifies a one- or two-byte trailing `C2/CA` suffix as a truncated `RET imm16` and refuses to retain a reviewed state for that malformed span. Corrected tracking covers 340,424 target `.text` bytes; boundary-state counts remain 1,173 reviewed, 88 provisional and 51 needs-review. | Canonical target tail bytes; pinned Capstone dense decode; `scripts/report-boundary-inventory.py --check-ledger`; corrected function/boundary ledgers |

## Boundaries

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| BOUNDARY-001 | observed / independent-entry-review | The pinned runtime member extents establish 41 previously provisional or focused, non-overlapping function boundaries. Separately, IDA MCP identified 28 prior provisional entries with matching function extents and entry references. Re-decoding the hash-attested target at every referenced address proved a direct `call` or `jmp` to the candidate entry, and the prior boundary audit had already established complete dense decode and no overlap. These first two campaigns moved the boundary ledger from 1,175 to 1,244 reviewed entries. | `scripts/review-crt-origins.py`, `scripts/review-boundary-entry-refs.py`; IDA MCP target hash and function inventory; pinned Capstone direct-branch replay; existing target boundary audit |

## Origin

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ORIGIN-001 | observed / runtime-library-origin | A complete-COFF-symbol scan of the pinned VC7.1 SP1 `libcmt.lib` and `libcpmt.lib` against the hash-attested TH10 image found 242 unique `libcmt.lib` function-body matches covering 49,952 target bytes. Each accepted symbol has a matching complete COFF extent of at least 32 bytes and at least 24 matching non-relocation bytes. All 635 relocation references whose symbols resolve uniquely within the accepted set reach the expected target entry. These 242 entries are classified `library` and excluded from the authored denominator; this is provenance evidence, not a source-exact or build-closure claim. The largest is the 3,627-byte `__input @ 0x00457A7C`. Short or ambiguous matches remain under review. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ORIGIN-002 | observed / runtime-family-origin | Thirteen more candidate bodies match multiple pinned `libcmt.lib` COFF symbols with the same full extent and at least 24 identical non-relocation bytes. Their library origin is recorded with medium confidence; their individual source symbol remains ambiguous. This includes two 829-byte memory-copy bodies: both match `_memcpy` and `_memmove` members across 645 ordinary bytes, and each contains 46 relocated local control targets that all resolve inside its own extent. Repeated C++ template matches from `libcpmt.lib` were left pending because an identical inline instantiation does not prove library ownership. | `scripts/review-crt-origins.py`; pinned target and VC7.1 library hashes; full COFF function bodies and target-local relocation destinations |
| ORIGIN-003 | observed / PE-import-and-EH-provenance | Twenty-one six-byte `FF 25` jump thunks resolve to named or ordinal entries in the target's own PE import table and are classified `import_thunk`. Forty-one VC7.1 C++ `FuncInfo` records with magic `0x19930520` point through valid unwind maps to all 62 current `Unwind@` cleanup entries; each action has a complete target RET/JMP body and is classified `compiler_generated`. These conclusions come from target metadata and target instructions, not from IDA's provisional labels alone. | `scripts/review-import-origins.py`, `scripts/review-eh-origins.py`; pinned target PE import descriptors and C++ exception tables; IDA xref spot checks; pinned Capstone |
| ORIGIN-004 | observed / CRT-object-family-provenance | The runtime scan now classifies 39 unique short `libcmt.lib` bodies with medium confidence, 30 candidate entries whose complete COFF body extends beyond the ledger's contiguous local fragment, 30 nested candidate fragments fully inside a matched CRT object's function extent, and 11 same-size lower-information bodies whose provisional inventory name agrees with a matching CRT symbol. Every accepted extended body matches all non-relocation bytes across the complete original COFF extent, including locally split suffix/helper code; 961 uniquely resolvable runtime relocation references agree with target destinations. Source-exact matching and authored ownership are not inferred from these provenance observations. | `scripts/review-crt-origins.py`; pinned `libcmt.lib` hash; complete COFF symbol extents, relocation masks, target-local destination replay; `config/function-origins.csv` |
| ORIGIN-005 | observed / authored-game-provenance | Forty-six previously unclassified candidate bodies totaling 31,234 target bytes are completely decoded and independently show a direct call to an already reviewed authored function plus references to at least two exact game-global addresses each used by at least two previously reviewed authored functions. None of those globals appears in reviewed excluded bodies. IDA spot checks show game display, flow and state operations in representative entries. These bodies are classified authored at medium confidence; their subsystem, source unit, production compiler mode and exactness remain unknown. The evidence script excludes its own rows from the seed set on replay. | `scripts/review-game-global-origins.py`; hash-attested target instructions, direct calls and global references; reviewed origin ledger; IDA MCP decompilation spot checks at `0x00402850`, `0x00409280`, `0x004236F0`, `0x00433EF0`, `0x00422C30` |
| ORIGIN-006 | observed / authored-game-neighbor-provenance | Seventy-seven further complete code bodies totaling 30,505 bytes reference an exact game-global address also used by at least two earlier reviewed authored functions and have an independently decoded direct call to or from one of those earlier authored functions. Reviewed excluded bodies do not reference the selected global. Representative IDA decompilations show game state, callback, rendering and file operations. The 77 entries are authored-origin observations at medium confidence; no subsystem, source unit, normal-COFF/LTCG ownership or exactness follows. Both authored-origin campaigns use the earlier 320 reviewed authored entries as fixed seeds. | `scripts/review-game-global-origins.py`; hash-attested target decode/call/global replay; existing origin ledger; IDA MCP spot checks at `0x00405ED0`, `0x00420470`, `0x0043DDF0`, `0x00449470`, `0x0044B620` |
| ORIGIN-007 | observed / authored-call-and-data-provenance | Eleven more game-region bodies totaling 2,789 bytes are directly called by an earlier reviewed authored function and themselves directly call another earlier reviewed authored function. Eighteen further bodies totaling 7,852 bytes reference a target `.data` address used by at least two earlier authored bodies and no reviewed excluded body, and also have a direct call edge to or from an earlier authored body. The ubiquitous `1.0f` constant at `0x00476F78` is excluded from the data evidence. Each selected body completely decodes, and representative IDA decompilation shows game behavior. These are medium-confidence authored-origin classifications with source, subsystem and exactness still open. | `scripts/review-game-edge-origins.py`; target PE `.data` range, target call instructions, fixed earlier authored origins; IDA MCP spot checks at `0x00404F30`, `0x00412AC0`, `0x00426520`, `0x00438A60` |
| ORIGIN-008 | observed / compiler-EH-handler-provenance | The target's VC7.1 C++ `FuncInfo` records include valid `TryBlockMap` entries whose 16-byte `HandlerType` records directly name the two tracked `Catch@` bodies at `0x00438723` and `0x004387C2`. Both completely decode and are classified compiler generated. The prior 62 `Unwind@` classifications still replay independently from unwind maps. | `scripts/review-eh-origins.py`; pinned target `.rdata` `FuncInfo`/`TryBlockMap`/`HandlerType` fields; IDA MCP data xrefs at `0x00471410` and `0x00471400` |
| ORIGIN-009 | observed / thin-CRT-wrapper-provenance | Eleven individually identified 5–15-byte runtime wrappers completely decode and each transfers to exactly one reviewed CRT library body outside its own extent. The target call destination plus wrapper behavior identifies allocator/free/conversion, thread error, exit-lock/cleanup, security-cookie, floating-point trap or math-dispatch support. They are excluded as library origin at medium confidence; the narrow structural evidence does not prove an exact CRT object symbol or source form. | `scripts/review-crt-wrapper-origins.py`; hash-attested target code and previously reviewed CRT callee origins; IDA MCP semantic spot checks for `_errno`, `__doserrno`, `_lockexit`, `_unlockexit`, `_fptrap`, `_CIfmod` |
| ORIGIN-010 | observed / individually-reviewed-game-behavior | Twenty further complete game-region bodies totaling 14,404 bytes were individually inspected in IDA. Their visible behaviors include ANM command/VM state, stage and score transitions, player option state, the TH10 replay header, game sound cues and DirectInput/keyboard transitions. Each has a direct target call edge to or from an already reviewed authored body. Two callers with embedded switch tables have their exact direct `call` sites separately decoded at `0x00432DB9` and `0x0042F6B6`. The 20 entries are classified authored at medium confidence; IDA behavior descriptions are provisional and no source unit, compiler mode or exactness is inferred. | `scripts/review-game-behavior-origins.py`; hash-attested target code, direct call-site replay, existing origin ledger; IDA MCP decompilation of each listed entry |
| ORIGIN-011 | observed / runtime-special-provenance | Five more complete runtime-region entries have individually reviewed CRT behavior: SSE `floor` with calls/jumps to reviewed CRT math bodies, a six-slot CRT floating-conversion pointer initializer, the `__NLG_Notify1` and `strcpy` prefixes that jump into reviewed library bodies, and an x87 status-word helper. They are classified library at medium confidence from target instructions plus IDA semantic inspection; their exact object member/source identity remains unknown. | `scripts/review-crt-wrapper-origins.py`; hash-attested target instructions and previously reviewed CRT bodies; IDA MCP decompilation of the five entries |
| ORIGIN-012 | observed / C++-runtime-family-provenance | Nineteen complete bodies in the late runtime cluster implement `std::string`, standard exception class constructors/destructors, type-info deletion or standard string throw helpers. Target vtable writes, calls and exception strings agree with IDA's type interpretation; several bodies also have matching candidate code in pinned VC7.1 C++ runtime archives, while duplicate template bodies leave exact object membership ambiguous. Three identical seven-byte member-call adapters use `pop eax; pop ecx; xchg [esp],eax; jmp eax` and are directly called from already reviewed CRT EH bodies at three independently decoded call sites. These 22 entries are excluded as library origin at medium confidence without claiming exact CRT symbols or TU ownership. | `scripts/review-cxx-runtime-origins.py`; target instructions, vtable/call/string behavior, pinned CRT candidate report, IDA MCP C++ type and xref review |
| ORIGIN-013 | observed / CRT-callback-helper-provenance | Three final short runtime-region entries are target-bound to reviewed CRT owners: a nine-byte wrapper calls the target `KERNEL32!TlsAlloc` IAT slot and is installed by CRT `__mtinit`; a one-byte RET is the initial value of two CRT thread callbacks consumed by reviewed thread-start/end bodies; and a three-byte zero-result function is the initial CRT math-error callback and is called directly by two reviewed math bodies. Each complete body is classified library origin at medium confidence, leaving exact object provenance unknown. No currently tracked runtime-region origin remains pending. | `scripts/review-crt-wrapper-origins.py`; target PE import table, target `.data` callback slots, target CRT caller instructions and reviewed origin ledger |
| ORIGIN-014 | observed / early-game-behavior-provenance | Thirty-two further complete entries from `0x00401000-0x0040FFFF`, totaling 6,650 bytes, were individually reviewed in IDA for game manager, ANM VM, projectile pool, playfield, scheduler and resource-path behavior. Each has a target-decoded direct call edge to or from an earlier reviewed authored body. They are classified authored at medium confidence; several nearby math helpers and constructors without an independent reviewed call edge remain pending. This origin claim does not identify original TU/source ownership or exact codegen. | `scripts/review-game-behavior-origins.py`; hash-attested target body and direct-call replay; IDA MCP decompilation of all 32 entries |
| ORIGIN-015 | observed / middle-game-behavior-provenance | Twenty-two complete entries from `0x00410000-0x0041FFFF`, totaling 2,732 bytes, were individually reviewed for stage/game objects, scheduler, projectile pool, playfield and resource behavior. Each has a target-decoded direct call edge to or from an authored owner outside the current manual behavior cohorts. They are classified authored at medium confidence; nearby generic string, constructor and interpolation bodies without that independent edge remain pending. `0x00412A10` remains origin-unknown because its ECL integer-lvalue helper could be source-written or outlined by the compiler; a dispatcher call does not resolve that ambiguity. No source unit, build mode or exactness is inferred. | `scripts/review-game-behavior-origins.py`; hash-attested target direct-call replay; IDA MCP decompilation of the 22 accepted entries; prior target-bound Enemy ECL context seam review |
| ORIGIN-016 | observed / late-game-behavior-provenance | Twenty-five complete entries from `0x00420000-0x0042FFFF`, totaling 3,316 bytes, were individually inspected for resource paths, stage transitions, player option/position, playfield and game controller behavior. Each has a target-decoded direct call edge to or from an authored owner outside the current four manual behavior cohorts. They are classified authored at medium confidence. The source-present Player callback bodies at `0x00427960` and `0x00427AE0` remain origin-unknown because their authored-adapter versus compiler-wrapper provenance requires separate evidence. No source/exact or production TU claim follows. | `scripts/review-game-behavior-origins.py`; hash-attested target direct-call replay; IDA MCP decompilation of each accepted entry; Player callback provenance notes |
| ORIGIN-017 | observed / standard-string-provenance | Nine complete entries at `0x004381F0-0x004387C1` implement VC7.1 `std::basic_string<char>` storage reset, assignment, erase, size update and growth. The target uses the 0xF-byte small-string capacity and calls already reviewed standard string throw helpers at `0x00462428` and `0x004624E5`. Entry `0x0043874C` is the normal continuation after the out-of-line EH catch at `0x00438723`, so its independent candidate boundary does not imply a separate source function. These nine entries are classified library at medium confidence; exact library object membership remains unknown. | `scripts/review-cxx-runtime-origins.py`; complete hash-attested target decode and call anchors; IDA MCP standard-string body and EH-continuation review |
| ORIGIN-018 | observed / audio-render-game-provenance | Twenty-one complete entries totaling 3,690 bytes were individually reviewed for GDI text rendering, game window setup, MIDI output, D3D drawing, ANM script random state and the TH10 sound cue/stream manager. Each has a decoded direct call edge to or from an earlier reviewed authored owner outside all five manual behavior cohorts. They are classified authored at medium confidence. In particular, the MIDI and GDI helpers are not assigned a source unit or original identifier merely from adjacency; normal COFF versus LTCG production ownership and exactness remain open. | `scripts/review-game-behavior-origins.py`; hash-attested target call-edge replay; IDA MCP behavior inspection of the 21 entries |
| ORIGIN-019 | observed / game-component-chain-provenance | Eighteen more complete entries totaling 5,902 bytes were individually reviewed for game text token parsing, render object state, GDI glyph bitmap handling, MIDI track/event processing, sprite state and sound filename lookup. A fixed acyclic sequence of target-decoded direct call edges ties each entry to an earlier reviewed authored owner, either from the existing ledger or an earlier entry in this explicit cohort. The author classification is medium confidence and does not infer original source unit or codegen. The DirectInput initializer at `0x0043B8D0` was deliberately left pending because its observed direct callee `0x00421D20` is still origin-unknown. | `scripts/review-game-component-origins.py`; hash-attested complete target decode and fixed call anchors; individual IDA MCP behavior review |
| ORIGIN-020 | observed / game-utility-and-audio-provenance | Forty further complete entries totaling 7,014 bytes have fixed direct target call edges to independently reviewed authored owners and were individually inspected in IDA. They implement scheduler/resource nodes, joystick/keyboard state, logging, game timer and moving-object helpers, plus sound stream construction, playback, volume, cursor and sample-copy operations. They are authored-origin observations at medium confidence, without source unit or codegen claims. Short destructor wrappers at `0x0044C100/0x0044C130` and several sound object lifecycle entries still require separate compiler-versus-source provenance review. | `scripts/review-game-component-origins.py`; hash-attested complete target decode and fixed call anchors; IDA MCP body review of the 40 new entries |
| ORIGIN-021 | observed / game-input-and-worker-provenance | Two sound worker callback entries at `0x0043CCF0` and `0x0043E3A0` are passed as function pointers by previously reviewed authored owners. An acyclic four-entry input chain starts at target game initialization `0x004201B0`, directly calls a reviewed authored game owner, passes `0x0043BA90` as its polling thread, which directly calls DirectInput initialization `0x0043B8D0`, which passes `0x0043BB30` as a device callback. Three sound setup/WAV helpers at `0x0043CD30`, `0x0043D390` and `0x0043D250` follow independently decoded direct calls from the reviewed sound setup thread. All nine complete entries, 2,593 bytes, were individually reviewed in IDA and classified authored at medium confidence; original source units and exact codegen remain unknown. | `scripts/review-game-pointer-origins.py`, `scripts/review-game-input-chain-origins.py`, `scripts/review-game-component-origins.py`; target pointer/call replay; IDA MCP behavior review |
| ORIGIN-022 | observed / additional-game-component-provenance | Forty-six further complete game entries totaling 3,997 bytes were individually reviewed for ANM command/interpolation helpers, score and timer updates, hint-file parsing, stage linked lists, input/worker setup, sound buffers and TH10 replay header/payload initialization. Each has a fixed independently decoded direct call edge to a previously reviewed authored owner, and the reviewer excludes its own entire cohort from the initial anchor set before replaying the acyclic chain. They are authored at medium confidence without source unit, production build mode or exactness claims. Constructor-only and source-versus-LTCG-outlined fragments remain under review. | `scripts/review-game-component-origins.py`; hash-attested target complete decode and fixed direct-call anchors; individual IDA MCP body review |
| ORIGIN-023 | observed / compiler-deleting-destructor-provenance | Three complete bodies at `0x0044C100`, `0x0044CF00` and `0x0044D710` follow the VC7.1 scalar deleting-destructor pattern: call the object destructor, test the deletion flag at `[esp+8]`, conditionally call the target free routine and return with `RET 4`. They are compiler-generated wrappers excluded from authored origin at high confidence. The underlying destructor bodies remain separately unresolved. | `scripts/review-deleting-destructor-origins.py`; hash-attested target instructions, call destinations and deletion-flag control flow; prior VC7.1 deleting-destructor precedents in the ledger |
| ORIGIN-024 | observed / target-call-backed-game-provenance | Four more complete game helpers, 196 bytes total, are authored at medium confidence: `0x0040ACE0` tests collision category masks, `0x0044B010` acquires keyboard state, `0x004501B0` updates a polar vector and `0x00450700` updates a renderer state link. Each has a target-decoded direct call site within a previously reviewed authored game owner. The original source unit and exact codegen remain unknown. | `scripts/review-game-component-origins.py`; hash-attested target and direct call sites; IDA MCP behavior inspection |
| ORIGIN-025 | observed / reviewed-attribution-unknown | Ninety remaining tracked entries were individually checked against reviewed physical boundaries and IDA body/xrefs and recorded as `indeterminate`, with an address-specific observation in the function ledger. C++ lifecycle and adapter shapes do not by themselves establish source authorship; some substantive game behavior also lacks independent source/linker provenance. Four source-present Player adapters remain unknown, with no canonical exact claim. The tracked-origin queue is now 709 authored, 518 excluded, 90 reviewed-indeterminate and zero unreviewed. The authored exact denominator remains provisional. | `scripts/review-indeterminate-origins.py`; hash-attested target extent checks; IDA MCP body/xref review; target-bound origin/function ledgers |

## Source provenance

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| SOURCE-001 | observed | The target string table contains `.\src\core\main.cpp:92 SprtCtrlInf` and `.\src\core\main.cpp:84 FuncCtrlInf`, proving that a `src/core/main.cpp` source path participated in the original build. Ghidra and bounded raw scans find no code reference to either path string, so this evidence does not assign any particular function or final-image extent to that translation unit and is not a source mapping by itself. | Attested TH10 string search plus bounded raw literal scan |
| SOURCE-002 | observed / corroborated | TH10 has maintained candidate source in `src/PbgArchive.hpp/.cpp`, `src/PbgFile.hpp/.cpp`, `src/ResFile.hpp/.cpp`, `src/Decompress.hpp/.cpp`, `src/FileSystem.hpp/.cpp`, `src/Lzss.hpp/.cpp`, and `src/ReplayManager.hpp/.cpp`. Fifty-five reviewed authored functions are source-present after the replay loader packet. These mappings assert maintained reconstruction source only; they do not identify original TH10 function identifiers/translation units, normal-COFF/LTCG physical owners, the production link graph, or exactness. `config/build.toml` remains intentionally open with no original source owner assigned. | Maintained source plus successful pinned normal-COFF and `/GL` compiles; tracking validation; TH10 target-local control-flow/ABI/data-owner evidence |
| SOURCE-003 | observed | The target string table contains `.\src\pack\File.cpp:233 void` at `0x0046F254` and `.\src\pack\ResFile.cpp:82 BYTE` at `0x0046F348`, proving those source paths participated in the original build. Attested `xrefs_to`, bounded text-immediate scans, and raw pointer scans across the complete ResFile string range find no code reference, so neither string assigns any reviewed function to an original translation unit. Maintained `src/PbgFile.cpp` and `src/ResFile.cpp` are therefore reconstruction source mappings, not claims of original `src/pack/File.cpp`/`ResFile.cpp` physical ownership. | Attested TH10 string/xref queries plus bounded direct target immediate/pointer scans and reviewed ResFile vtables |

## Toolchain

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| TOOLCHAIN-001 | observed / corroborated | Rich product IDs 95, 96, and 100 at build 6030 record 131 normal C, 15 normal C++, and 52 LTCG C++ inputs. A separately pinned VC7.1 SP1 candidate reports compiler/linker build 6030. | Direct target Rich decoding; pinned `archaic-msvc/msvc710_sp1` commit; `scripts/verify-toolchain.py --execute` |
| TOOLCHAIN-002 | observed | The pinned candidate executes normal C/C++ COFF, C++ LTCG, resource compilation, and PE32 i386 linking under a game-bound headless Wine prefix. | `scripts/verify-toolchain.py --execute`; component hashes in `config/tools.lock.toml` |
| TOOLCHAIN-003 | unknown / whole-product | Production TU partition, libraries, resources, data owners, and link order are not established. Accepted units in `config/match-units.toml` bind only their own compiler contexts and do not close the product graph. | Explicit open fields in `config/build.toml`; bounded accepted units in `config/match-units.toml` |
| TOOLCHAIN-004 | observed | A pinned VC7.1 build-6030 `/GL` diagnostic linked with `/LTCG` naturally eliminates an unused source parameter and converts surviving internal callee inputs from the normal-COFF stack ABI to private EAX/ECX register inputs. This demonstrates that the private-register/signature-shrink class seen in TH10 is a legitimate LTCG transformation, but provides no per-function TH10 ownership or exactness credit. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## Runtime

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| RUNTIME-001 | observed / canonical-archive-exact / source-less-library | The target's reviewed CRT/runtime lane now has 335 source-less archive units covering 95,963 canonical exact bytes in total. Each unit cold-extracts a complete function member from the hash-attested VC7.1 `libcmt.lib`, records the symbol extent and every target-bound DIR32/REL32 relocation, and replays zero-difference. Library provenance remains independent from source presence: these rows stay `library/exclude`, add no maintained source mapping, and do not close the product build. | Direct IDA Pro MCP boundary checks (including `_atof` 0x00452A84, `_isdigit` 0x0045685E and `??_L...` 0x0045252D); pinned archive hashes in `scripts/runtime_archive.py`; `runtime-crt-*` archive units in `config/match-units.toml`; focused `replay-exact-units.py --source .tools/msvc710-sp1/Vc7/lib/libcmt.lib` |
| RUNTIME-002 | observed / canonical-archive-exact / source-less-library | Direct IDA Pro MCP target review identifies 37 previously excluded CRT/C++ runtime bodies, including the `0x004537DC` startup owner, CRT wrappers, three C++ EH member-call adapters, and the VC7.1 `std::string`/standard-exception family at `0x00462301-0x00462DF5`. Hash-attested `libcmt.lib` and `libcpmt.lib` members reproduce 1,386 target bytes with complete DIR32/REL32 relocation manifests in focused cold replay. These claims remain library/exclude, source-less, and independent of whole-product closure. | Direct IDA Pro MCP function names/disassembly/bytes; `scripts/runtime_archive.py` archive hashes; 37 `runtime-*` `coff-archive` units in `config/match-units.toml`; repeatable `scripts/replay-exact-units.py` focused replay |

## CRT

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| CRT-001 | observed / corroborated | The target entry `0x004537DC` is VC runtime startup code rather than authored game logic: it initializes runtime state, argv/environment/heap/thread/IO support, constructs the WinMain argument set, calls `0x00438AD0`, and exits through CRT routines. | Attested Ghidra disassembly/decompile/callees plus direct target inspection |
| CRT-002 | observed | Target `0x00452706-0x00452717` is the 18-byte CRT `malloc` wrapper: it pushes the requested size and runtime new-mode global to `__nh_malloc`, is reached by 94 direct REL32 calls, and ends before CC padding. Target `0x0046054E-0x004605B6` is the 105-byte CRT `_stricmp` wrapper: its default-locale path calls the adjacent ASCII comparison implementation, its locale path performs per-byte mapping, 11 direct REL32 calls use the entry, and the next function begins immediately at `0x004605B7`. Both are classified library/CRT and excluded from the authored denominator without claiming an exact original CRT library member. | Hash-attested target bytes; complete `.text` direct-call scans; entry/tail and callee disassembly |
| CRT-003 | observed / boundary-partial | Target entry `0x00452930` is a complete 45-byte CRT `strrchr` implementation: it finds the terminator, scans backward for the requested byte, ends at `0x0045295C`, and is followed by CC padding; ten direct REL32 calls use this entry. It is classified library/CRT and excluded without claiming an exact original library member. Target entry `0x00452730` implements optimized `strchr` behavior and is reached by 18 direct references, including the `CPbgFile::GetFullFilePath` call at `0x00435619`, but match exits branch backward to the shared return tail at `0x00452720-0x00452724`, outside Ghidra's `0x00452730-0x004527ED` candidate. Its semantic entry is reviewed for the caller's linked field while its complete physical boundary/origin disposition deliberately remains unknown/review. The same PbgFile helper calls the KERNEL32 `GetModuleFileNameA` import through IAT slot `0x004660DC`, whose stored hint/name RVA resolves to that import. | Hash-attested target disassembly; complete `.text` direct-reference scans; PE import/IAT decoding; PbgFile caller control flow |

## Main/window

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| MAIN-001 | observed / corroborated | The reviewed application entry at `0x00438AD0` spans through `0x0043903C`, has a unique CRT caller, consumes the four WinMain stack arguments, and returns with `ret 0x10`. The next three bytes are `0xCC` padding. | Attested Ghidra caller/disassembly queries; direct target disassembly; pinned VC7.1 ABI probe |
| MAIN-002 | observed | The `0x00438AD0` application entry owns the D3D9 cooperative-level/reset message loop, restart path, window teardown, configuration writeback, and process-level cleanup visible in the target. | Attested Ghidra decompile/callees; direct target disassembly and target strings |
| MAIN-003 | observed / inferred | `0x00438A30` is a reviewed 43-byte authored resource-release loop used by two target-local callers. Both callers pass the object in EAX; the function releases and clears 32 COM resource pointers. This is evidence for an interprocedural/private-ABI optimization context, but does not by itself prove which Rich-record input or TU owns it. | Attested Ghidra callers/disassembly; direct target boundary inspection |
| MAIN-004 | unknown | Code at `0x00439040`, `0x00439060`, `0x00439080`, and `0x004390A0` follows WinMain padding but is not represented as Ghidra functions and has no direct Ghidra or literal-VA xrefs. Its cleanup-funclet/compiler ownership remains unresolved, so it is not added to the function denominator yet. | Attested Ghidra function/xref queries; bounded raw target VA scan |
| MAIN-005 | observed / corroborated | `0x004391F0-0x004392DB` is a reviewed authored present/reset/screenshot function. It calls the D3D9 device `Present` method, releases manager resources and resets/reinitializes render state on presentation failure, and implements the `snapshot/th%.3d.bmp` capture loop. `0x004392DC-0x004392DF` is `0xCC` padding before independent `0x004392E0`. `GameWindow::Present` is retained only as a proposed adjacent-supported name. | Attested TH10 Ghidra callers/decompile/disassembly; direct target bytes; committed TH08/TH095 source used only for naming/source-shape corroboration |
| MAIN-006 | observed / corroborated | `0x00439890-0x00439D16` is a reviewed authored D3D9 initialization function called directly by WinMain. It constructs presentation parameters, retries HAL hardware/software and REF device creation, installs view/projection matrices, queries capabilities, and invokes the render-state initializer before returning success/failure. `0x00439D17-0x00439D1F` is `0xCC` padding before independent `0x00439D20`. `GameWindow::InitD3DRendering` is a proposed adjacent-supported name, not an original-symbol claim. | Attested TH10 Ghidra caller/decompile/disassembly; direct target bytes; committed TH08/TH095 source used only for naming/source-shape corroboration |
| MAIN-007 | observed / corroborated | `0x00439D20-0x00439FE5` is a reviewed authored no-explicit-argument D3D9 render-state initializer used after initial device creation and reset. Ghidra's provisional fastcall argument is contradicted by the target: the entry `push ecx` reserves one dword reused for `1.0f`, `1000.0f`, and `5000.0f`, and the exit `pop ecx; ret` releases it. A pinned VC7.1 `/O2` normal-COFF diagnostic naturally emits the same scratch pattern for a no-argument C++ function, but this does not establish the target function's COFF/LTCG owner or exactness. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| MAIN-008 | resolved / source-present | The D3D/present seam is now maintained as target-bound `GameWindowView` source. TH10-local offsets bind the five trailing writes at `0x00439FBF-0x00439FDE` to the active ANM render manager's blend mode, color operation, vertex shader, texture and camera-mode caches. The `GameWindowView` name remains descriptive and physical TU/original identifier ownership remains unresolved. | TH10 target control flow and manager offsets; maintained `src/Main.cpp`/`src/Main.hpp`; adjacent source used only after target-local recovery for source-shape corroboration |
| MAIN-009 | observed / corroborated | `0x00439390-0x00439538` is a reviewed authored frame/render dispatcher. WinMain is its unique caller and pushes the address `0x004924F0`; the callee loads that one stack pointer and all return paths use `ret 4`. The body advances target-local timing fields, runs gameplay/draw work, and invokes reviewed `0x004391F0`. Normal VC7.1 COFF diagnostics exclude an ordinary C++ member-thiscall shape: ordinary members use ECX/`ret`, while explicit `__stdcall` member and static/free `__stdcall(pointer)` forms produce the observed stack-pointer/`ret 4` ABI. Source-level owner and LTCG/private-ABI status remain unknown. `GameWindow::Render` is only a proposed adjacent-supported name. | Attested TH10 caller/decompile/disassembly; direct boundary bytes; pinned VC7.1 ABI diagnostic |
| MAIN-010 | observed / source-present | `0x00439540-0x00439659` is a reviewed authored high-resolution/fallback timestamp routine. It enters the game critical section, uses `QueryPerformanceCounter` divided by the stored frequency when available, otherwise brackets `timeGetTime` with 1 ms timer-period calls, maintains a target-local time origin, and returns the resulting x87 scalar. Current natural source produces the same 282-byte VC7.1 `/O2 /GS` normal and selected `/GL` candidate. Both match 159/162 comparable bytes; the only differences are three occurrences of stack-frame constant `0x10` versus target `0x14`, so exactness remains unknown. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| MAIN-011 | observed / corroborated | Target storage beginning at `0x004924F0` is a coherent window/timing object-like region used by the reviewed Main seam. Observed fields include HWND `+0x00`, closing state `+0x04`, WinMain HINSTANCE `+0x08`, active/inactive state `+0x0C/+0x10`, signed frame counter `+0x14`, performance frequency/start QWORDs `+0x18/+0x20`, shortcut/relative-launch state `+0x28`, three saved `SystemParametersInfoA` values `+0x2C/+0x30/+0x34`, timing doubles `+0x38/+0x40/+0x48`, and time origin `+0x50`. These establish the field usage through `+0x57`; `MAIN-015` subsequently closes the physical storage extent at `0x58` bytes from TH10-local ownership evidence. The original type name remains unknown and is not inferred from adjacent games. | TH10 WinMain/window-proc/create-window/timing target accesses; attested xrefs/decompilation; committed TH095 layout used only as cross-game corroboration after TH10 offsets were established |
| MAIN-012 | observed / corroborated | `0x004390E0-0x004391E8` is a reviewed authored Win32 window procedure. `0x00439730` stores its address directly in `WNDCLASSA.lpfnWndProc`; the body consumes the four standard callback stack arguments and every exit uses `ret 0x10`. Ghidra's provisional extra `this` parameter is contradicted by the target. `GameWindow::WindowProc` is only an adjacent-supported proposed name. | Attested TH10 function/data-xref/disassembly; direct target boundary bytes; committed TH08/TH095 source used only for naming corroboration |
| MAIN-013 | observed / corroborated | `0x004392E0-0x00439343` is a reviewed authored system/timing initializer: it saves and disables three `SystemParametersInfoA` power states and initializes the performance frequency/start fields. Its historical pre-strict probe has now been superseded by the two cold exact normal-COFF replays in `MAIN-026`; production normal-COFF versus LTCG ownership remains unresolved. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| MAIN-014 | observed / canonical-linked-exact / private-ABI | `0x00439730-0x00439884` is a reviewed authored create-window routine. WinMain copies its HINSTANCE into EBX immediately before the unique call, and the callee consumes live-in EBX for both `WNDCLASSA.hInstance` and `CreateWindowExA` without a stack argument. A pinned normal-COFF explicit-HINSTANCE probe instead reads `[esp+4]`, so the target machine ABI is private/interprocedural rather than an ordinary standalone normal-COFF call. Natural source preserves the target's `g_GameWindowView.window` then `g_MainSupervisorView.gameWindow` publication order. The canonical `main-create-game-window` linked-PE unit reproduces the complete 341-byte owner and all 20 DIR32 fields twice; `GameWindow::CreateGameWindow` remains a descriptive recovered name, not an original-symbol claim. | Direct TH10 caller/callee disassembly; pinned VC7.1 ABI diagnostic; canonical `main-create-game-window` replay; maintained `src/Main.cpp`; adjacent TH08/TH095 source used only for naming/source-shape corroboration |
| MAIN-015 | observed | The target-local window/timing storage is physically bounded to exactly `0x58` bytes at `0x004924F0-0x00492547`. Its last observed `double` occupies `0x00492540-0x00492547`; an independent static object begins exactly at `0x00492548`, where startup code clears 17 dwords, sets its first-word flags, and registers a destructor wrapper that passes `0x00492548` to a separate cleanup routine. A default-layout VC7.1 struct using only TH10-observed field widths naturally has size `0x58` and the target offsets. The original type identifier remains unproven. | TH10 field-width accesses and xrefs; direct static-initializer/destructor-wrapper disassembly at `0x00465960`/`0x00465A50`; pinned VC7.1 layout assertions |
| MAIN-016 | observed / corroborated | Three CC-delimited authored source bodies were missing from the original Ghidra candidate denominator and are now tracked: `0x00439350-0x0043938D` restores saved system settings, `0x00439660-0x004396F7` computes unlocked elapsed time, and `0x00439700-0x0043972C` creates the D3D9 interface and reports failure. None has a direct Ghidra or raw target call/literal xref. WinMain contains inlined copies of the restore and D3D-interface logic. Historical natural VC7.1 probe size/comparable-byte claims for the first two predate the strict COFF extent fix in `a superseded COFF comparison`; strict two-build replays now establish their bounded normal-COFF exactness in `MAIN-026`. All three now have maintained source. A pinned VC7.1 /GL diagnostic emits 38 bytes for the 45-byte D3D helper and does not match, so it remains non-exact. Original identifiers and production physical ownership remain unknown. | Direct target boundaries and bounded raw xref scans; historical diagnostic probes; WinMain duplicate logic |
| MAIN-017 | observed / corroborated | `0x00439FF0-0x0043A1A9` is a reviewed authored startup/single-instance routine. WinMain is its unique direct caller and passes no explicit argument. The target creates the `Touhou 10 App` mutex, inspects startup/module/console paths, follows shortcut chains through `0x0043A290`, and writes reviewed window/timing storage `+0x28` when the resolved launch path differs from the module path. `GameWindow::CheckForRunningGameInstance` is an adjacent-supported proposed name only; the unused HINSTANCE seen in adjacent source is not transferred to the TH10 ABI. | Attested TH10 caller/decompile/disassembly; direct target boundary/data accesses; committed TH08/TH095 source only for naming/source-shape corroboration |
| MAIN-018 | observed / corroborated | `0x0043A1B0-0x0043A1B7` is a Ghidra-missed, CC-delimited authored body that consumes an HWND through live-in EAX and calls `SetForegroundWindow`. It has no direct/literal target entry xref, while the reviewed WindowProc contains the same foreground-window action inline. A normal-COFF HWND helper instead reads `[esp+4]`; pinned VC7.1 LTCG diagnostics show the private-register transformation class is possible without proving this body's physical owner. `GameWindow::ActivateWindow` is only a proposed adjacent-supported name. | Direct target bytes/imports and bounded xref scan; reviewed WindowProc inline behavior; pinned compiler diagnostics; committed adjacent source for name/order corroboration |
| MAIN-019 | observed / corroborated | `0x0043A1C0-0x0043A28E` is a reviewed authored executable-checksum routine with a unique WinMain caller. It obtains the module path, calls the target file loader at `0x0044B360` with filename in live-in EAX plus stack `size_out/mode`, sums all dwords except the final one, and stores the checksum/size at `0x00492380/0x00492384`. `GameWindow::CalcExecutableChecksum` is an adjacent-supported proposed name only. | Attested TH10 caller/decompile/disassembly; direct target global writes; committed TH08 source only for naming/source-shape corroboration |
| MAIN-020 | observed / corroborated | `0x0043A290-0x0043A39F` is a reviewed authored shortcut resolver. Its unique caller passes the shortcut path on the stack while supplying destination buffer in live-in EBX and capacity `0x104` in live-in EDI; the callee returns with `ret 4`. The body uses COM/ShellLink operations to load and resolve the shortcut. `GameWindow::ResolveShortcut` is an adjacent-supported proposed name only; source declaration and physical COFF/LTCG owner remain unresolved. | Attested TH10 caller/decompile/disassembly and target boundary; committed TH08/TH095 source only for naming/source-shape corroboration |
| MAIN-021 | observed | `0x0043A3A0-0x0043A488` is a Ghidra-missed authored dialog callback passed directly by WinMain to `DialogBoxParamA`. It handles `WM_INITDIALOG`/`WM_COMMAND`, calls the target `SendMessageA`, `GetDlgItem`, `EndDialog`, and `IsDlgButtonChecked` imports, and all callback exits use `ret 0x10`; `0x0043A489-0x0043A48F` is CC padding. The machine ABI is the standard four-argument DLGPROC family, but the original identifier/source owner remains unknown. | Direct WinMain callback pointer, target import table and raw disassembly; attested xref to `0x0043A3A0` |
| MAIN-022 | observed / source-present | Ten central Main owners now have maintained target-bound source across 5,456 reviewed bytes: `WinMain`, window procedure, present/reset/screenshot, frame rendering, timestamp, window creation, D3D9 initialization, render-state reset, single-instance/startup-path handling and executable checksum. `GameWindowView` is exactly `0x58` bytes; `MainSupervisorView` assertions bind D3D objects, matrices, viewport/presentation data, serialized configuration, gameplay/active viewports, transition/render flags, music owner, capabilities, seven critical sections, render-state cache and frame duration. `MainAllocationRegistryView` is exactly `0xA004`. The source preserves target machine boundaries separately from descriptive C++ ownership. | Hash-attested target extents, epilogues and direct field accesses; Factory-native Ghidra evidence; compile-time layouts; `scripts/report-main-corridor.py --check`; maintained `src/Main.cpp`/`src/Main.hpp` |
| MAIN-023 | observed / exact | `GameWindowView::ResetRenderState @ 0x00439D20` is canonical normal-COFF exact across the complete 710-byte extent. All 574 non-relocation bytes and all 34 DIR32 fields replay zero-difference twice: 33 fields address `MainSupervisorView +0x8` and the final field addresses `g_AnmRenderManagerView`. This bounded codegen claim remains independent of unresolved production normal-COFF versus LTCG physical ownership. | Maintained source; pinned VC7.1 SP1 normal-COFF cold replay; target-bound complete relocation manifest |
| MAIN-024 | observed / fps-owner-recovered | TH10 `0x004134B0-0x004135C7` is a 280-byte half-second FPS sampler and `0x004135D0-0x00413683` is its 180-byte display owner. Target-local control flow proves the sampler reads `GameWindowView::GetTimestamp`, repairs backward time, computes unsigned frame-count FPS, treats `>65` as a clock anomaly, resets the four GameWindow timestamps at anomaly counts two/four, disables QPC on the fourth anomaly, and accumulates lag statistics through a gameplay-state gate at `+0x58`. The display owner calls the sampler, selects three colors, emits `%2.1ffps` through the ASCII manager, and advances the frame count by `frameskip + 1`. Callback bridge `0x00413690` directly calls the display owner at `0x00413693`, correcting its former provisional boundary to reviewed/high. The adjacent TH095 `CalculateFps`/`DrawFpsCounter` family supplies only descriptive naming; all maintained fields and behavior are independently present in TH10. | Hash-attested TH10 disassembly and Ghidra decompilation; callback installation/call chain; maintained `FpsCounterView` layout/source in `src/Main.cpp`; adjacent family used only after target behavior recovery |
| MAIN-025 | observed / exact-fps-display | `FpsCounterView::DrawFpsCounter @ 0x004135D0` is canonical linked exact across its complete 180-byte owner. Adding `src/AnmManager.cpp` as the real formatter support TU lets VC7.1 recover the target private register contract (`EDI` FPS owner, `ESI` ASCII manager, `EBX` position); retaining `currentFps` as one local `float fps` keeps the x87 value live across both 30/40-FPS comparisons and reuses it directly for the variadic double argument, reproducing the target `FCOM / FCOM / FSTP QWORD` schedule. Two independent cold `/GL` links reproduce all 180 bytes after replaying nine CalculateFps, display-mode, ASCII-manager, threshold, format-string, formatter-call and frameskip fields. The retained 10-byte callback bridge at `0x00413690` is raw byte-identical in the same graph, but its authored-versus-compiler/LTCG provenance remains unresolved, so it is deliberately not promoted. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| MAIN-026 | observed / exact | The 100-byte system-parameter initializer at `0x004392E0`, 62-byte settings/IME restore helper at `0x00439350`, and 152-byte unlocked timestamp helper at `0x00439660` now have maintained source and canonical normal-COFF units. Two independent cold builds reproduce all 314 target bytes and all 27 declared global, import, constant and call fields. IDA omits a function at `0x00439350`, but all 62 target bytes independently decode through RET. Original identifiers and production normal-COFF versus LTCG owners remain unknown; exactness is limited to the declared normal-COFF codegen replay. | Hash-attested target and IDA where available; pinned Capstone dense decode; `main-initialize-system-parameters`, `main-restore-system-parameters`, and `main-get-timestamp-unlocked` cold replays; Main regression |
| MAIN-027 | observed / corrected-attribution / canonical-linked-exact | `MainSupervisorView::SetNextGameMode @ 0x0040AC90` is a 37-byte shared game-mode setter. All six direct call sites preload EAX=`0x00491C28`; target startup stores the HINSTANCE at this base and Direct3DCreate9 at base+0x04, independently identifying the MainSupervisor owner. The body tests flags at +0x3CC bit 0x1000, writes the requested mode to +0x390 when clear, and otherwise forces 2. Maintained FrontEnd source already names the +0x390 slot next game mode; Gui and Player had separate placeholder transition interfaces for the same target helper. A natural MainSupervisor member, selected through real `GuiMessageVmView::Run` /GL context, yields the target private EAX receiver and reproduces all 37 relocation-free bytes raw-equal. | Hash-attested target body, global-base/caller setup and +0x390 xrefs; maintained Main/Gui/FrontEnd source; canonical `main-set-next-game-mode` replay; pinned VC7.1 SP1 build6030 |
| MAIN-028 | observed / canonical-linked-exact / private-ABI | The target create-window owner at `0x00439730-0x00439884` is now closed by natural maintained source under a target-bound `/GL /GS` graph rooted at `GameWindowView::WindowProc`. Two independent pinned VC7.1 SP1 build6030 links reproduce all 341 bytes and the exhaustive 20-field DIR32 manifest: Win32 imports, WindowProc, `BASE`/title strings, `MainSupervisorView` `windowed`/HWND fields and `GameWindowView` state. The private EBX HINSTANCE convention is retained as interprocedural evidence; the normal-COFF explicit-argument probe remains diagnostic only. | Direct IDA Pro MCP disassembly/caller bytes at `0x00439730`, `0x004390E0`, and WinMain; focused `main-create-game-window` replay twice; pinned VC7.1 SP1 build6030 |

## Game manager

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| GAME-001 | observed / recovered-GameManager-update / non-exact-core | 0x00418190-0x00418797 is the complete 1544-byte GameManager update core, with one stack owner and RET4. Target reads timer current at +0x14 and flags+0x58, handles frame0/menu and frame30 restoration, performs two stage reset sequences with 0x21CEA0 pool clearing and a zeroed 0x40-byte EnemySpawn request, marks the observed callback chains with bit2, invokes actual Replay BeginStage and Player Rebuild twice, handles demo frame2940/3000 and capped statistics at row stride0x437C +0x4CC, then advances the timer. Idle-state call0x418A00 updates the shared faith timer/decay, rather than advancing the stage. Natural maintained GameManager.cpp restores the full behavior and real Player/Replay/Enemy interfaces; opaque dependency receiver/argument interfaces retain private-LTCG ABI unknowns. The inline stage reset models target duplicated operations without asserting original factoring. Final nine-TU /GL /GS graph emits1590/1544 bytes with frame0x40, RET4 and two direct calls each to Rebuild/BeginStage/EnemySpawn. Whole-core exactness and runtime remain unproven. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| GAME-002 | observed / recovered-startup-and-registration / non-exact-core | 0x00417870-0x00417C60 is the complete 1009-byte GameManager initialization/startup owner with one stack owner and RET4. It polls renderer/loaders, initializes target score/faith/power/lives/statistics state, registers update0x4187C0 and draw0x4187D0 at priorities10/4 with node owner+0x20, copies thirteen configuration dwords to owner+0x24, executes the observed short-circuit creation/replay paths, prepares music and publishes success/failure flags. Full maintained source restores these real callback-address escapes; no registration-only fake context is added. The existing Player lives variable0x474C70 is reused with its established volatile qualification; stage-statistic signed byte+0x1D copied to0x474C94 retains a neutral role name. Draw member-pointer spelling is a source hypothesis; VC7.1 size assertion and the emitted immediate prove the observed four-byte argument, not original callback type. Two eight-byte zeroed spans in the FPS view retain unknown original member types. The final core is1037/1009 bytes and RET4; ordinary opaque dependencies still differ from several target private register contracts. This closes source presence, not exactness, original class/TU/data ownership or product/runtime gates. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| GAME-003 | observed / canonical-linked-exact / unknown-origin | Newly inventoried registered callback0x004187C0-0x004187C6 is seven bytes: push ECX, call update core0x00418190, RET. One target registration reference at0x004179FB and CC boundaries establish the complete adapter. Before full startup recovery, the compiler uses an EAX private callback; actual registration plus natural static OnUpdate(GameManager*) preserves ECX and matches the target. Two independent pinned /GL cold links replay all seven bytes and the exhaustive sole REL32 field zero-difference. Canonical game-manager-update-callback grants7 bytes only; source-written versus compiler/LTCG adapter origin remains indeterminate. Existing draw callback31 bytes and eight complete coupled seams1444 bytes pass focused protection. No full TH10 cold replay is performed. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## Frontend

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| FRONTEND-001 | observed / inferred | Target review adds three Ghidra-missed front-end entries. `0x0042D260-0x0042D2D3` ends executable code at `0x0042D2B3` and owns an eight-entry absolute switch table through `0x0042D2D3`. `0x0042D2E0-0x0042D2E6` is `mov eax,ecx; jmp 0x0042CDF0`, and `0x0042D2F0-0x0042D2F6` is `mov eax,ecx; jmp 0x0042D260`; both are followed by CC padding and have target data references. The reviewed title/front-end chain identified `0x0042CDF0`, `0x0042D920` and `0x0042E5A0` as one coherent core recovery batch, subsequently completed in `FRONTEND-002`; that prioritization was inferred from target call structure and size rather than origin or exactness. | Direct target bytes/xrefs; Factory-native Ghidra callers/decompile; `scripts/rank-core-backlog.py` |
| FRONTEND-002 | observed / source-present | Three authored front-end owners now have maintained source across 8,315 target bytes. `FrontEndControllerView::Update @ 0x0042CDF0` dispatches all 17 screen values, advances its timer, rotates demo replays after 900 idle frames and performs the observed ANM cleanup/title-entry branches. `UpdateOptions @ 0x0042D920` covers all five state values and the six cursor rows for color depth, BGM/SFX volume, key configuration, defaults and return. `RefreshOptionsDisplay @ 0x0042E5A0` updates sound globals, queues `SetVol`, applies the observed quartic BGM attenuation curve, assigns twelve digit sprites and controls normal/selected leading-digit visibility. Descriptive identifiers do not claim original symbols or TU ownership. | Hash-attested target disassembly/tables/calls; Factory-native Ghidra decompilation; maintained `src/FrontEnd.cpp`/`src/FrontEnd.hpp`; `scripts/report-frontend-core.py --check` |
| FRONTEND-003 | observed / physical-table-ownership | Physical review extends the title owner from its RET at `0x0042D212` through one NOP and its directly indexed 17-entry absolute table at `0x0042D214-0x0042D257`, for a 1,128-byte owner. It extends the options controller from its final `RET 4` at `0x0042E57D-0x0042E57F` through its directly indexed five-entry table at `0x0042E580-0x0042E593`, for a 3,188-byte owner. Eight and twelve `CC` bytes respectively separate the next entries. Both tables were previously untracked. | Direct target branch operands, table destinations and padding; target-bound front-end report; boundary ledger |
| FRONTEND-004 | observed / corrected | Repeated front-end and GUI child-VM traversals compare the signed word at ANM VM `+0x38A`, which the independently maintained VM layout identifies as `scriptIndex`; `activeSpriteIndex` is the distinct word at `+0x384`. The GUI helper's former `activeSpriteIndex` comparison was therefore a semantic field error and is corrected to `scriptIndex`. The cursor helpers establish a complete `0xD8` cursor layout, while the current partial front-end controller binds screen/state/cursor at `+0x1C/+0x20/+0x24`, timer at `+0x2B0` and VM-id storage at `+0x2C4`. | Target compare operands and cursor helper bodies; maintained ANM VM layout; VC7 layout assertions |
| FRONTEND-005 | observed / source-present | Three additional authored front-end owners now have maintained source across 3,532 target bytes. `FrontEndControllerView::UpdateKeyConfig @ 0x0042F540` implements the five-state/seven-row controller, scans 31 signed controller-button transition bytes, edits five action bindings, handles reset/cancel/commit and drives root VM slot 2 interrupts. `RefreshKeyConfigDisplay @ 0x0042F8B0` resolves twenty child digit VMs and writes tens/ones sprites for both normal and selected rows. `AssignKeyConfigBinding @ 0x00430250` swaps a displaced duplicate binding before refreshing the display and playing sound 10. Descriptive identifiers do not claim original symbols or TU ownership. | Hash-attested target disassembly/calls; Factory-native Ghidra decompilation/xrefs; maintained `src/FrontEnd.cpp`/`src/FrontEnd.hpp`; `scripts/report-frontend-key-config.py --check` |
| FRONTEND-006 | observed / corroborated / physical-table-and-data-layout | The key-config update owner is physically `0x0042F540-0x0042F8AF` = 880 bytes: executable code ends at `0x0042F89A`, one NOP follows, and its directly indexed five-entry state table occupies `0x0042F89C-0x0042F8AF`; the independent display owner begins immediately at `0x0042F8B0`. Display ends at `0x00430243` before twelve CC bytes, and assignment ends at `0x00430317` before eight CC bytes. Runtime controller mapping storage at `0x00474E88` is nine adjacent signed shorts with target defaults `(0,1,2,3,-1,-1,-1,-1,4)`. TH10 input masks identify the first four entries as shot/bomb/focus/menu and the last as skip; up/down/left/right names for the four preserved middle entries are adjacent-supported and remain provisional because the reviewed TH10 input path derives direction bits from controller axes. Key-config scratch bindings are five shorts at controller `+0x59CC`; the UI edits the first four runtime actions plus skip, while commit preserves and copies the complete 18-byte runtime mapping to persisted storage at `0x00491D4C`. | Direct target table/tails/default data and copy instructions; input-consumer masks/axis paths in `0x0044A190`; Factory-native xrefs; committed TH08 mapping declaration used after TH10 layout recovery; compile-time layout assertions; focused report |
| FRONTEND-008 | observed / physical-table-ownership | Each selection owner directly indexes a five-entry absolute state table after its executable body. Difficulty is physically `0x00430320-0x00430697` = 888 bytes: code ends at `0x00430680`, a three-byte alignment instruction follows, and its table begins at `0x00430684`. Character is `0x004306A0-0x00430A5B` = 956 bytes with its table immediately after code ending at `0x00430A47`. Shot type is `0x00430A60-0x00430FEF` = 1,424 bytes: code ends at `0x00430FDA`, one NOP follows, and its table begins at `0x00430FDC`. These reviews add 64 bytes to tracked `.text` union coverage and move three indexed tables from outside to inside their owners. | Direct target branch operands, table entries, tails and neighboring entries; focused report; boundary inventory |
| FRONTEND-009 | observed / source-present | `FrontEndControllerView::UpdateStage @ 0x00430FF0` and `DrawStageScores @ 0x00431410` now have maintained source across 1,471 target bytes. The update owner implements the five-state six-stage controller, profile availability gate, numeric-key start-stage shortcut, ANM VM lifetime and final stage-record/game-mode transition. The display owner emits six profile-backed score rows, colors unavailable/current/other rows, blinks during confirmation and restores ASCII state. Descriptive identifiers do not claim original symbols or TU ownership. | Hash-attested target disassembly/calls/profile offsets; Factory-native Ghidra decompilation/xrefs; maintained source; `scripts/report-frontend-stage.py --check` |
| FRONTEND-010 | observed / canonical-linked-exact / private-ABI | `FrontEndControllerView::SetScreenStateTarget @ 0x0042C620` is the target's 80-byte EAX-controller/ECX-state helper: it stores screen state, clears the state timer fields and writes the target `g_AnmGameSpeed`-derived timer anchor. Moving this seam out of `UpdateScoreEntry` as a natural member method reproduces the complete PDB contribution and its sole DIR32 field in the real `/GL` score-entry graph. This exact helper does not make the 1,400-byte `UpdateScoreEntry` owner exact; the separate 95-byte VM-creation seam at `0x0042C670` remains unresolved. | Direct IDA disassembly/decompile at `0x0042C620`; canonical `frontend-set-screen-state` replay; pinned VC7.1 SP1 build6030 |
| FRONTEND-011 | observed / canonical-linked-exact / private-ABI | `FrontEndControllerView::SetScreenTarget @ 0x0042C5C0` is the adjacent 83-byte EAX-controller/ECX-screen helper: it stores screen, clears screen state and performs the same timer reset as `SetScreenStateTarget`. Routing the result-screen transition through a natural member seam reproduces the complete PDB contribution and its sole `g_AnmGameSpeed` DIR32 field. The larger screen controllers remain non-exact. | Direct IDA disassembly at `0x0042C5C0`; canonical `frontend-set-screen` replay; pinned VC7.1 SP1 build6030 |
| FRONTEND-012 | observed / physical-table-ownership | The replay update owner is physically `0x004315C0-0x00431B93` = 1,492 bytes: code ends in `RET 4` at `0x00431B79`, two-byte `MOV EDI,EDI` alignment follows, and the directly indexed six-entry table occupies `0x00431B7C-0x00431B93`. Its destination order is state 0 `0x004315F2`, state 1 `0x0043174D`, state 2 `0x004317B1`, state 3 `0x004319B4`, state 4 `0x004318FD`, and state 5 `0x00431A9E`. The display owner is `0x00431BA0-0x00431ED8` = 825 bytes and receives one stack controller argument from callback `0x0042D275`. Twelve and seven `CC` bytes delimit the two owners. This correction adds 26 tracked bytes and moves the table from the outside-owner queue into its source owner. | Direct canonical target branch/table/tail bytes and neighboring padding; update/draw callback call sites; focused report; boundary inventory |
| FRONTEND-013 | observed / source-present | `FrontEndControllerView::UpdatePractice @ 0x00431EE0` and `RefreshPracticeRecords @ 0x00432690` now have maintained source across 2,818 target bytes. The four-state update owner maintains independent six-shot, five-difficulty and page cursors, creates and retires the screen ANM VMs, and cycles a ten-row record page. The refresh owner filters the 110-entry spell-card catalog through the target difficulty map, skips prior pages, pads unlocked names to 42 bytes, displays two profile counters and blanks unused rows. The controller view binds the second and third cursors at `+0x0FC/+0x1D4`, displayed-row count at `+0x2AC`, and ten ASCII VM ids at `+0x5D4..+0x5FB`. Descriptive identifiers do not claim original symbols or TU ownership. | Hash-attested target disassembly, calls, catalog/difficulty/profile offsets and format strings; Factory-native Ghidra decompilation; maintained source/layout assertions; `scripts/report-frontend-practice.py --check` |
| FRONTEND-014 | observed / physical-table-and-secret-input | The practice update owner is physically `0x00431EE0-0x0043268F` = 1,968 bytes: code ends in `RET 4` at `0x0043267A-0x0043267C`, three-byte `LEA ECX,[ECX]` alignment follows, and its four-entry state table occupies `0x00432680-0x0043268F`. The table destinations are `0x00431F06`, `0x00432095`, `0x004320BA`, and `0x00432615`. The target's Extra/third-shot branch copies the complete 256-byte keyboard state, derives rising-edge bytes, accepts a 22-entry scan-code sequence, resets on any other press among the first 57 codes, and expires after 300 frames; completion calls the profile-unlock helper and queues sound 44. The refresh helper ends at `0x004329E1` and is followed by fourteen `CC` bytes. | Direct canonical target table/tail bytes, keyboard tables/buffers and control flow; focused target report; boundary inventory |
| FRONTEND-015 | observed / source-present | `FrontEndControllerView::Draw @ 0x0042D260` and `DrawPractice @ 0x004329F0` now have maintained source across 805 target bytes. The eight-entry draw table proves that screen discriminator 11 is Practice and 12 is Replay, correcting the reversed maintained enum: discriminator 11 calls `UpdatePractice`/`DrawPractice`, while 12 calls `UpdateReplay`/`DrawReplay`. The practice draw owner emits ten profile-backed score rows on page zero, fading each row's red and green channels by `0x10`; each 0x18-byte record supplies score, stage-name index, suffix digit, ten-byte name, timestamp and slowdown rate. It always emits the selected shot's aggregate value at `+0x4C8`, frame-derived `hours:minutes:seconds` at `+0x4CC`, and the selected difficulty's value at `+0x4D0 + difficulty*4`. `Draw` physically owns its table through `0x0042D2D3`; `DrawPractice` ends at `0x00432CA0` before fifteen `CC` bytes. | Canonical target update/draw dispatch tables, direct calls, profile field accesses, time arithmetic, formats and padding; maintained source; expanded `scripts/report-frontend-practice.py --check` |
| FRONTEND-016 | observed / source-present | `FrontEndControllerView::UpdateScoreEntry @ 0x00432CB0` and `DrawScoreEntry @ 0x00433230` now have maintained source across 2,226 target bytes. Screen discriminator 15 owns a four-state high-score insertion and name-entry flow: it selects the character/shot/difficulty score table, inserts one 0x18-byte record, restores the profile's last eight-character name, drives a wrapping 91-entry keyboard in thirteen columns, and commits the name to both the selected record and profile `+0x1D878`. The final three keyboard entries are target-rendered control glyphs for space, backspace and accept. The display draws ten profile score rows, overlays the editable name/caret on the inserted row, and draws the full keyboard. Descriptive identifiers do not claim original symbols or TU ownership. | Hash-attested target disassembly, calls, strings, record accesses and controller offsets; Factory-native Ghidra decompilation; maintained source; `scripts/report-frontend-score-entry.py --check` |
| FRONTEND-017 | observed / physical-table-and-layout | The update owner is physically `0x00432CB0-0x00433227` = 1,400 bytes: executable code ends in `RET 4` at `0x00433213-0x00433215`, two-byte `MOV EDI,EDI` alignment follows, and the four-entry state table occupies `0x00433218-0x00433227`. Its destinations are `0x00432CCC`, `0x00432EDA`, `0x00432F00`, and `0x004331AC`. The draw owner is `0x00433230-0x00433569` = 826 bytes and is followed by six `CC` bytes. Controller fields are name `+0x58DC`, name cursor `+0x58E8`, insertion-failure flag `+0x58EC`, and a complete `FrontEndCursorView` at `+0x58F4`; the target store at `+0x59C4` is that cursor's established `wraps` member. | Direct canonical target table/tail/caller bytes; target field accesses; focused target report; compile-time offset assertions |
| FRONTEND-018 | observed / exact-cursor-family | TH10 cursor helpers FrontEndCursorView::Push @ 0x0044BE20, Pop @ 0x0044BE70, and Move @ 0x0044BEA0 establish the complete 0xD8 cursor state used throughout the front end. Factory-attested caller sets contain 10/12/15 direct callers, with mapped FrontEnd owners including Update, Options, KeyConfig, Difficulty, Character, ShotType, Stage, Replay, Practice and ScoreEntry. Push and Pop already reproduced their complete 66/47-byte target contributions. Move initially had the correct 128-byte extent but only 78/128 raw agreement; direct target disassembly revealed a missing count<=0 early return before the nonvolatile-register saves. Restoring that boundary behavior naturally produces the target prologue, alignment and full 128-byte body with zero differences in the same UpdateDifficulty /GL graph. | Hash-attested TH10 target bodies; Factory-native caller/decompile review; maintained FrontEnd cursor layout; canonical frontend-cursor family replays; pinned VC7.1 SP1 build6030 |
| FRONTEND-019 | observed / canonical-coff-exact / physical-helper | `0x0040AD20` is the shared cursor-set helper used by seven FrontEnd paths. The target physical ABI is directly visible as ECX=requested value and EDX=`FrontEndCursorView*`; the helper loads `count` from +0x08, keeps an unconstrained value when count is zero, caps high values to `count-1`, clamps negatives to zero, stores +0x00, and returns the stored value in EAX. This corrects the earlier generic “frame-timer wrap” interpretation. The maintained member `FrontEndCursorView::SetCurrent` now delegates to a descriptively named `FrontEndCursorSetCurrent` physical helper so existing source keeps its natural API while the target machine boundary is modeled explicitly. Several equivalent C++ spellings were tested: ordinary out-of-line member forms emitted 31 or 37 bytes and changed register coloring; a free fastcall helper with the target control-flow lifetime emits a relocation-free 33-byte COMDAT byte-identical to TH10. The pre-existing Push/Pop/Move linked units remain exact after this refactor. | Hash-attested target body and seven-caller FrontEnd chain; maintained cursor layout/API; canonical `frontend-cursor-set-current` normal-COFF replay; pinned VC7.1 SP1 build6030 |
| FRONTEND-020 | observed / corrected-attribution / canonical-linked-exact | `FrontEndInputView::IsRepeated @ 0x0040ACE0` corrects the older “collision category mask” interpretation. The 33-byte target helper reads 16-bit `pressed` at input +0x06 and `repeated` at +0x04, testing both against a caller-supplied mask. Factory-native xrefs show 22 calls across six FrontEnd controller owners; representative Difficulty, Replay and ScoreEntry paths load ECX=`0x00474E30` before every call and place directional masks such as 0x10/0x20/0x40 in EAX. The maintained `FrontEndInputView` already had exactly `current/repeated/pressed` at +0/+4/+6. Refactoring the local `InputRepeated(mask)` wrapper to delegate to a natural member method, with source written as “return zero only when neither field matches,” lets VC7.1 /GL promote the mask to EAX while retaining the input object in ECX. Under real `UpdateDifficulty` context the PDB contribution is 33/33 raw-equal with no linked fields. A direct first attempt with two early true returns produced a target-sized 33-byte near match but reversed the final block layout; that variant was reverted. | Hash-attested TH10 body and 22 direct calls; target caller setup at shared input 0x00474E30; maintained FrontEndInputView layout; canonical `frontend-input-repeated` replay; pinned VC7.1 SP1 build6030 |
| FRONTEND-021 | observed / canonical-linked-exact / callback-adapters | The paired seven-byte FrontEnd callback adapters at 0x0042D2E0 and 0x0042D2F0 are registered by target owner 0x0042CAA0 through callback pointers stored at 0x0042CAA2 and 0x0042CACA. Both bodies are exactly MOV EAX,ECX followed by a tail JMP, to FrontEndControllerView::Update @ 0x0042CDF0 and Draw @ 0x0042D260 respectively. Natural __fastcall wrappers in src/FrontEnd.cpp, selected as their own VC7.1 /GL entries, make LTCG choose the same private EAX receiver and reproduce each complete seven-byte PDB contribution plus its sole REL32 field in independent cold builds. Their target origins remain indeterminate because this machine shape is compatible with either source-written wrappers or optimizer-generated adapters. Re-auditing Draw also establishes target physical block order Replay, Practice, ScoreEntry, Result, Stage; retaining that lexical order makes the direct-entry contribution 116 bytes, equal to the target extent, but the Draw body is still non-exact because saved-register and child-call ABI choices differ. | Hash-attested target callback registration/raw bytes/table; two cold frontend-update-callback/frontend-draw-callback linked replays; pinned VC7.1 SP1 build6030 |
| FRONTEND-022 | observed / canonical-linked-exact / selection-vm-helper | `AsciiManagerView::EnsureSelectionVm @ 0x0040C540` is the shared 75-byte selection-transition helper reached by seven target callers. Its private machine ABI keeps the ASCII manager in ESI and accepts X/Y on the stack; it always constructs an X/Y/0 position, returns the existing managed VM id at manager `+0x89A4`, or creates script 6 through `asciiAnm` at `+0x8994`, stores the new id and returns it. Restoring the source as a value-returning `AsciiManagerView` member, rather than the earlier void anonymous helper that reached the global manager, naturally changes the selected `UpdateShotType` /GL contribution from 85 bytes to the complete target-sized 75 bytes. Two focused canonical cold builds reproduce all 75 PDB-owned bytes after replaying the sole REL32 field to `AnmLoadedView::CreateVmAtScreenVariant0 @ 0x00448D50`. The four screen-position creator variants remain separately non-exact at 93/95 bytes in this same graph; the caller closure is not evidence that their private EDI/EBP/EBX ABI has been recovered. | Hash-attested TH10 target body/callers and direct disassembly; canonical `ascii-manager-ensure-selection-vm` linked-PE replay; pinned VC7.1 SP1 build6030 |
| FRONTEND-023 | observed / canonical-linked-exact / private-ABI | `FrontEndDeleteVm @ 0x0042C6D0` is an 83-byte private EDI-controller/ESI-index seam. Direct IDA shows the slot lookup at controller `+0x2C4`, the manager `FindVm` call, pending interrupt `1` stores at VM/child `+0x304`, and unconditional slot clear. Writing that full natural C++ propagation loop as a TU-visible helper, with `AnmManager.cpp`/`AnmVmId.cpp` in the real `/GL` support graph, makes VC7.1 select the target EDX manager receiver before `FindVm` and reproduces the complete PDB contribution. Two independent cold linked-PE replays match all 83 bytes after normalizing the manager DIR32 and `FindVm` REL32 fields. This is the interrupt-and-clear helper, not `AnmVmIdView::Release` (which marks deletion through `+0x35C`). | Direct IDA disassembly/decompile at `0x0042C6D0`; canonical `frontend-delete-vm` replay twice; pinned VC7.1 SP1 build6030 |
| FRONTEND-024 | observed / source-present | The contiguous difficulty, character and shot-type selection controllers at `0x00430320`, `0x004306A0` and `0x00430A60` now have maintained source across 3,268 target bytes. All three use the shared five-state opening/active/confirmed/closing protocol. Difficulty selects one of four normal entries or the Extra root, manages the auxiliary ASCII VM and transitions to character selection. Character applies the six Extra unlock bytes at profile `+0x1D888..+0x1D88D` and sends four complementary child interrupts on confirmation. Shot type applies three per-character unlock bytes, hides three child rows whose profile record dwords are zero, and commits the selected character/shot/difficulty path either to stage selection or normal-game startup. Controller VM ids are established through slot `0x97`, with the auxiliary difficulty VM id at `+0x5D0` and saved difficulty at `+0x58F0`; the existing key-config scratch remains at `+0x59CC`. Names for the three owners and the profile records are descriptive where original symbols remain unknown. | Hash-attested target disassembly, direct calls and profile/controller offsets; Factory-native Ghidra decompilation; maintained source/layout assertions; `scripts/report-frontend-selection.py --check` |
| FRONTEND-025 | observed / physical-table-and-data-layout | The stage update owner is physically `0x00430FF0-0x0043140B` = 1,052 bytes: executable code ends at `0x004313F6`, one NOP follows, and the directly indexed five-entry state table occupies `0x004313F8-0x0043140B`. The display owner is `0x00431410-0x004315B2` = 419 bytes and is reached from the front-end draw callback at `0x0042D2A8` with a private EDI receiver. Stage profile records are eight bytes, with score at `+0` and availability at `+5`, indexed by stage plus difficulty times six within a `(shot + character*3) * 0x437C` group. The keyboard helper distinguishes DirectInput scan codes `2..10` from Win32 virtual-key codes `0x31..0x39` in one 256-byte buffer. | Direct target table/tail/caller/data accesses and string pointers; Factory-native decompilation/xrefs; focused target report |
| FRONTEND-026 | observed / source-present | `FrontEndControllerView::UpdateReplay @ 0x004315C0` and `DrawReplay @ 0x00431BA0` now have maintained source across 2,317 target bytes. The six-state updater loads twenty-five numbered and up to twenty-five user replay files, derives a seven-entry stage cursor from replay stage headers, commits replay path/character/shot/difficulty and game mode 12, and releases all fifty replay owners on cancellation. The display emits twenty-five metadata rows and an animated seven-stage score view using the target character, difficulty, completion and stage-name tables. The maintained controller view binds replay fields at `+0x59D8/+0x59DC/+0x59E0` and the pointer array at `+0x59E4`. Descriptive identifiers do not claim original symbols or TU ownership. | Hash-attested target disassembly, direct calls, string/data tables and field accesses; Factory-native Ghidra decompilation; maintained source/layout assertions; `scripts/report-frontend-replay.py --check` |

## Filesystem

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| FILESYSTEM-001 | observed / corroborated | `0x0044B360-0x0044B4C7` is a reviewed authored file loader. Its machine boundary consumes path through live-in EAX, consumes `size_out` and mode from two stack slots, and returns with `ret 8`; `0x0044B4C8-0x0044B4CF` is CC padding. With mode zero it strips path components, searches 0x10-byte archive records rooted at `0x00497990`, obtains decompressed size from record `+0x08`, allocates output, and calls the archive reader. Otherwise it performs `CreateFileA`/`GetFileSize`/allocation/`ReadFile`/`CloseHandle`. `FileSystem::OpenFile` is an adjacent-supported proposed name only; source-level convention, TU/profile, source mapping, and exactness remain unknown. | Attested TH10 function/caller/callee/disassembly/xref queries; direct target bytes/imports; committed TH08/TH09/TH095 source used only after TH10 behavior was established |
| FILESYSTEM-002 | observed / corroborated | Target storage rooted at `0x00497990` is physically bounded to exactly `0x10` bytes: entries pointer `+0x00`, entry count `+0x04`, heap-owned archive filename `+0x08`, and polymorphic file backend `+0x0C`. Archive lifecycle code uses only those four dwords; a dedicated shutdown wrapper passes exactly `0x00497990` to the release body, while independent densely referenced storage begins at `0x004979A0`. `PbgArchive` is retained as a source-level proposed type name, not an original-symbol or TU-owner claim. | Attested TH10 lifecycle/decompile/disassembly/xrefs; direct static-destruction wrapper and next-storage access audit; committed adjacent layout only for later corroboration |
| FILESYSTEM-003 | observed / corroborated | `0x0044B4D0-0x0044B53E` is a reviewed authored file-existence helper using the same file-system critical section/count as `FILESYSTEM-001`. It attempts a read-only `CreateFileA`, closes a valid handle, returns 1/0, and uses one stack argument with `ret 4`; `0x0044B53F` is CC padding before `0x0044B540`. `FileSystem::CheckIfFileAlreadyExists` is an adjacent-supported proposed name only, and the callee-pop machine shape is not promoted to a source-level `__stdcall` claim. | Attested TH10 decompile/disassembly/xrefs and direct target boundary bytes; committed TH08/TH095 source used only for naming/source-shape corroboration |
| FILESYSTEM-004 | observed / corroborated | `0x0044B0D0-0x0044B212` is a reviewed authored in-place data transform. Every target caller loads the initial XOR seed in live-in AL, then pushes data pointer, size, XOR increment, chunk size, and max bytes; the callee returns with `ret 0x14`. It preserves a small untransformed tail derived from `size % chunkSize`, `chunkSize / 4`, and size parity, copies `min(size,maxBytes)` bytes to scratch, then processes each bounded chunk by consuming scratch forward while writing alternating bytes backward from the chunk end and incrementing the XOR key. It frees scratch and returns the original buffer. Maintained `FileSystem::Decrypt` is adjacent-supported naming after TH10-local ABI/behavior recovery, not an original-symbol/TU claim. | Six TH10 raw call sites and full target disassembly; attested caller/decompile evidence; committed TH09/TH095 source only for later source-shape corroboration |
| FILESYSTEM-005 | observed / corroborated | `0x0044B220-0x0044B352` is the reviewed authored inverse transform paired with `FILESYSTEM-004`. Its machine boundary consumes the initial XOR seed in live-in AL plus five stack arguments (data, size, XOR increment, chunk size, max bytes) and returns with `ret 0x14`; `0x0044B353-0x0044B35F` is CC padding before `0x0044B360`. It copies `min(size,maxBytes)` bytes to scratch, preserves the same small tail rule as Decrypt, then reads each scratch chunk in reverse odd/even order while writing sequentially back to the original buffer and evolving the XOR key. Replay save calls it twice with `(seed, increment, chunk) = (0x3D,0x7A,0x80)` then `(0xAA,0xE1,0x400)`; score save uses `(0xAC,0x35,0x10)`, and all observed calls use `maxBytes == size`. Maintained `FileSystem::Encrypt` is adjacent-supported naming after TH10-local recovery, not an original-symbol/TU claim. | Attested TH10 function/decompile/caller/callee/xref/disassembly evidence; three raw TH10 call sites and direct boundary bytes; committed TH09/TH095 source only for later naming/source-shape corroboration |
| FILESYSTEM-006 | observed / exact-normal-coff | `FileSystem::CloseWriteFile @ 0x0044B7E0` has a complete 37-byte authored boundary. When the shared file handle is valid, it closes the handle, leaves the shared critical section, decrements the active-file byte, and returns zero. Natural maintained source emits the same 37 bytes with pinned VC7.1 SP1 normal COFF; two independent cold builds and a focused canonical replay pass all five complete DIR32 fields (three globals and two Win32 imports) with zero differences. This bounded exact claim does not establish original source/TU, production codegen profile, or ownership of the referenced data. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| FILESYSTEM-007 | observed / corroborated | The replay disk-read seam contains three newly reviewed authored bodies. `0x0044B6B0-0x0044B738` opens a read-only shared handle while holding the file critical section and is maintained descriptively as `ReplayFile::Open`; `0x0044B790-0x0044B7D9` consumes a private live-in EDI byte count, allocates that many bytes, reads from the shared handle, and is maintained as `ReplayFile::Read`; `0x0044B7E0-0x0044B804` closes a valid shared handle, releases the critical section, decrements the active-file count, and is maintained as `FileSystem::CloseWriteFile`. Each extent is followed by CC padding. These names are adjacent-supported after TH10-local behavior recovery. `ReplayFile::Open`, `ReplayFile::Read`, and `FileSystem::CloseWriteFile` now have maintained source mappings; original-TU and exactness claims remain independent. | Direct TH10 raw disassembly/imports/shared-state accesses; attested function/decompile/caller evidence where provider transport succeeded; committed TH095 `ReplayFile`/`FileSystem` source used only for later naming corroboration |
| FILESYSTEM-008 | observed / compiler exact | Maintained `ReplayFile::Read` source, compiled with pinned VC7.1 SP1 `/GL` under the `ReplayFile::Open` entry graph, reproduces the complete 74-byte target extent after exhaustive resolution of six handle, allocator, and Win32 import fields. An independent diagnostic build and the canonical cold replay both agree. `ReplayFile::Open` remains non-exact at 135 candidate bytes versus 137 target bytes (18/89 comparable bytes matched). | Direct TH10 target bytes and imports; PDB-owned linked-image extents; `replay-file-read` canonical unit; independent `/GL` diagnostic |
| FILESYSTEM-009 | observed / canonical-linked-exact | FileSystem::CheckIfFileAlreadyExists at 0x0044B4D0 owns 111 bytes through RET 4; the following CC is excluded. Natural source enters the shared critical section, increments the unsigned-byte open count, opens read-only with shared-read/sequential-normal flags, closes a valid handle, and returns 1/0 through duplicated cleanup. Both paths leave the critical section before decrementing the count. The independently observed one-stack-argument callee-clean contract is represented by __stdcall; four complete target caller sites agree. This is not a claim about original declaration spelling or physical production TU/profile. A cdecl control is 107/111 with incorrect plain RET exits; the natural callee-clean form is 111/111 in normal COFF and /GL linked diagnostics. Two independent strict canonical cold links reproduce the complete 111-byte PDB-owned extent, all twelve exhaustively bound DIR32 fields, and zero ordinary differences. All 13 existing affected units pass 2,387 bytes across eight artifacts, including Main and ReplayManager caller contexts. Factory acceptance remains a separate receipt boundary. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| FILESYSTEM-010 | observed / canonical-linked-exact | `FileSystem::OpenFile @ 0x0044B360–0x0044B4C7` reproduces its complete 360-byte owner and all 26 independently reviewed fields in two canonical cold links with distinct PE/PDB identities. Natural three-parameter `__stdcall` source under the real Main checksum `/GL /GS` graph produces the target-private EAX filename plus stack sizeOut/mode and RET 8. Mode zero is archive-only; other modes are disk-only. Source preserves the second-separator search's original-path fallback, archive size publication before allocation, ignored archive-reader/ReadFile results, and Leave-before-count-decrement cleanup. The archive is the distinct singleton at `0x00497990`, not the twenty-pack array at `0x004923B0`. Field +0x65 binds the singleton with addend 4; the diagnostic map's neighboring restart-log anchor is not its semantic owner. The ordinary convention/ANM context and subsequent natural reader/context/TU controls did not close the complete owner; the selected callee-clean Main graph, null-first selections, and target-supported outlined lookup context do. All 161 existing affected exact units pass 27,909/27,909 bytes across 49 cold artifacts, including the complete transitive FileSystem header closure. Local canonical authored exact becomes 43,050 / 266,187 (16.17%); the 500 indeterminate origins are unchanged. Factory PASS and ACCEPTED on the first attempt at a129047, job de160cebc24d46f6bb09657a36783fec, receipt 8d2ac1e0ab4a90b6ab9b3ce20098f035b31f4679730cfb8260efda8b416c72b6. Original declaration spelling, TU/profile ownership, native product closure and runtime remain unknown. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## Archive

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ARCHIVE-001 | observed / corroborated | The reviewed authored archive core consists of `0x00434C10` constructor, `0x00434C20` destructor, `0x00434C30` load, `0x00434D10` release, `0x00434DD0` read/decrypt/decompress, `0x00434EF0` decompressed-size query, `0x00434F30` entry lookup, `0x00434F70` header parser, `0x004350D0` entry allocator, `0x00435220` filename copier, `0x00435250` entry destructor, retained `0x00435270`/`0x00435280` cursor helpers, and retained `0x004352C0` entry constructor. Machine ABIs include several private EAX/EBX/ECX/EDI/ESI inputs and therefore are recorded independently from the natural C++ declarations. | Attested TH10 function/caller/callee/disassembly/xref queries; direct target boundaries/padding; pinned compiler diagnostics; adjacent source only for naming corroboration |
| ARCHIVE-002 | observed / corroborated | `0x00434F70-0x004350CD` parses the TH10 archive format from a 16-byte header. It decrypts the header with `0x1B/0x37` and 16-byte chunk/max values, requires magic `0x31414854` (`THA1`), decodes file-table decompressed size by subtracting `123456789`, compressed size by subtracting `987654321`, and entry count by subtracting `135792468`, then reads the table from the end of the archive, decrypts it with `0x3E/0x9B/0x80`, decompresses it, and builds entries. | Direct TH10 disassembly/constants/backend virtual calls; committed TH095 source used only after the TH10 format was independently recovered |
| ARCHIVE-003 | observed / corroborated | TH10 `0x004350D0` allocates `count + 1` records of exactly `0x10` bytes and copies each file-table record into filename `+0x00`, data offset `+0x04`, decompressed size `+0x08`, and a third metadata dword `+0x0C`. The extra sentinel record stores the archive data/table boundary at `+0x04` and zero at `+0x08`; `ReadDecompressEntry` computes compressed size as `entry[1].dataOffset - entry.dataOffset`. The `+0x0C` dword is copied but has no observed retail reader in the reviewed scope, so maintained source names it `unconsumedMetadata` without assigning semantics. | Direct TH10 `AllocEntries`/reader disassembly and array-construction calls; adjacent archive layouts only for corroboration |
| ARCHIVE-004 | observed / corroborated | Ghidra missed several complete archive source bodies. `0x00434C10-0x00434C1F` is directly supplied as the constructor to an MSVC vector-constructor iterator for twenty 0x10-byte objects and clears all four archive fields. `0x004352C0-0x004352C8` is directly supplied as the 0x10-byte entry constructor. `0x00435270-0x0043527B` and `0x00435280-0x004352B2` are retained cursor helpers whose operations are inlined inside `AllocEntries`. Conversely `0x00434D70-0x00434DCC` is a compiler-generated scalar/vector deleting-destructor dispatcher and is tracked as an exclusion, not authored code. | Direct TH10 static initializer/array iterator references and raw boundaries; pinned normal COFF automatically generated the corresponding deleting-destructor class; committed adjacent source for retained-helper naming |
| ARCHIVE-005 | observed | `0x00434CA0-0x00434D03` is a reviewed authored retained alternate archive-load body. It has no Ghidra caller, no target little-endian VA occurrence, and no direct/literal entry reference, but its CC-delimited physical extent and source-shaped behavior are independent: live-in ECX is the 0x10-byte archive object, live-in EAX is the resource name, and it performs `Release`, allocates a 0x10-byte backend with vtable `0x0046F328`, calls the shared archive parser, copies the resource name to archive `+0x08`, returns true on success, and calls `Release` on failure. Unlike ordinary `PbgArchive::Load`, it does not reopen the backend after parsing because the resource-backed implementation already owns the complete resource bytes. Maintained `PbgArchive::LoadFromResource` is a descriptive reconstruction name, not an original identifier/TU claim. | Direct TH10 disassembly/padding, attested function/decompile/callee/xref queries, whole-target direct/literal VA scan, and the independently reviewed resource-backed `IPbgFile` vtable |
| ARCHIVE-006 | observed | The exact retail target has no `RT_RCDATA` (resource type 10) entry at all: its PE resource directory contains only type 3 icon, type 5 dialog, and type 14 group-icon resources. `CWin32ResourcePbgFile::Open` searches `FindResourceA(NULL, name, RT_RCDATA)`, while the retained `0x00434CA0` archive resource-load body has no target entry reference. Therefore no concrete archive resource name can be recovered from this executable and this source path is retained-but-unused in the current product image; absence of current use does not establish original TU or compiler ownership. | Direct `objdump -x` parse of the attested target `.rsrc` directory plus no-reference audit for `0x00434CA0` |
| ARCHIVE-007 | observed / canonical-normal-coff-exact | PbgArchive::AllocEntries at 0x004350D0 owns the complete 327-byte authored primary body through 0x00435216. Unchanged natural new PbgArchiveEntry[count+1] source compiles to 253 bytes without C++ exception support; adding only /EHsc restores the target FS exception-registration and EH vector-construction setup and all 327 primary bytes. No source/header or old profile changes are retained. The ten relocations are independently justified: associated generated handler at 0x004653CB, four __except_list ABS0 references used only as FS:[0], operator new, exact entry destructor/constructor, EH vector-constructor iterator and malloc. The pinned libcmt exsup.obj independently defines __except_list in absolute section -1 at value zero. Target handler, FuncInfo 0x004714B4, unwind map 0x004714AC and allocation cleanup 0x004653C0 establish the handler association rather than guessing from its encoded pointer. Two canonical cold builds reproduce the full 327-byte single-function COMDAT with all ten fields and zero differences. A distinct PbgArchiveEntriesEh.obj preserves the old no-/EHsc object/profile; all ten existing PbgArchive units plus the support-only CPbgFile constructor cold-replay 382/382 bytes across four artifacts. The claim covers only the complete primary function, not exactness of EH-support code/data, original production ownership/profile, whole-object/product or runtime closure. Factory receipt acceptance remains separate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ARCHIVE-008 | observed / reader source corrected / declared compiler context | `PbgArchive::ReadDecompressEntry @ 0x00434DD0` receives filename in ECX and archive/output on the stack, returning with RET 8. Target-backed source corrections restore its strlen pass followed by a counted byte-sum pass, `FILE_BEGIN` for the observed literal-zero seek origin, and byte narrowing of the modulo-eight profile index. No maintained mutable seek-mode definition supports the former table load. The target directly calls FindEntry at `0x00434DE9`; a narrowly scoped `inline_depth(0)` around that real call recovers the reader input contract needed by OpenFile, followed by bare `inline_depth()` to restore the compiler default. The bare-reset control preserves OpenFile's complete match and the reader's 284-byte candidate. No prior inline_depth setting was found in the project-header closure or the complete pinned CRT/SDK include trees. GetEntryDecompressedSize and OpenFile retain their target-inlined lookup shapes. The reader remains **284/281 and entirely non-exact**. Counted-loop, byte-index/seek corrections and Decompress/real-caller/split-TU controls alone left the caller's one-byte register seam; the explicit outlined-call constraint is a declared reconstruction context, not evidence of original pragma/TU spelling. Correct the older 278-address explanation: Ghidra's reader disassembly omits the three alignment bytes at `0x00434E5D–5F`; its decompilation separately removes the five-byte modulo correction at `0x00434E80–84`. Whole-product and runtime claims remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ARCHIVE-009 | observed / corrected-semantic-link-binding | A fresh PbgArchive Release replay failed because its manifest named constructor iterator `??_L@YGXPAXIHP6EX0@Z1@Z` at REL32+0x30 while rebuilt code called destructor iterator `??_M@YGXPAXIHP6EX0@Z@Z`. The unchanged old Decompress header control reproduced exactly the same failure, so this was a preexisting binding defect, not a decoder-ABI regression. Target `0x004525FF` walks backwards over array elements and RET16, independently agreeing with existing `runtime-crt-004525ff`/ehvecdtr.obj; constructor iterator `0x0045252D` instead walks forward and RET20. Correcting that one semantic symbol, with offset/type/target/comparator unchanged, restores all ten PbgArchive canonical units (361 bytes). No new exact-byte credit is claimed for this repair. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## PbgFile

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| PBGFILE-001 | observed / corroborated | TH10 `CPbgFile` storage is exactly `0x0C` bytes in reviewed construction sites: vptr `+0x00`, Win32 handle `+0x04`, and desired access `+0x08`. Target derived vtable `0x0046F230` has nine slots in this exact order: `Open` `0x00435370`, `Close` `0x00435460`, `Read` `0x00435490`, `Write` `0x004354D0`, `Tell` `0x00435510`, `GetSize` `0x00435530`, `Seek` `0x00435550`, compiler deleting destructor `0x00435320`, and `ReadWholeFile` `0x00435580`. The base vtable at `0x0046F210` supplies seven pure-virtual slots plus its deleting destructor. | Direct TH10 rdata/vtable bytes, constructor/temp-object writes, archive backend allocation size, and attested xrefs; committed TH08/TH095 declarations used only after target slot recovery |
| PBGFILE-002 | observed / corroborated | Reviewed authored `CPbgFile` bodies are constructor `0x00435300-0x00435314`, destructor `0x00435340-0x0043536D`, `Open` `0x00435370-0x0043545E`, `Close` `0x00435460-0x00435481`, `Read` `0x00435490-0x004354C8`, `Write` `0x004354D0-0x0043550E`, `Tell` `0x00435510-0x00435528`, `GetSize` `0x00435530-0x00435544`, `Seek` `0x00435550-0x00435574`, `ReadWholeFile` `0x00435580-0x00435608`, and `GetFullFilePath` `0x00435610-0x00435688`. The target path helper consumes filename through live-in EAX and output buffer through ECX; constructor consumes live-in EAX, while ordinary virtual methods use the expected ECX `this` plus callee-popped stack arguments where applicable. Source-level declarations are natural C++; private machine boundaries are not promoted to source calling-convention claims. | Direct TH10 disassembly/boundaries/imports/vtable; attested decompile/xrefs; maintained source compiler feedback |
| PBGFILE-003 | observed | Denominator review found five Ghidra-missed virtual methods at `0x00435490`, `0x004354D0`, `0x00435510`, `0x00435530`, and `0x00435550`, plus the Ghidra-missed `CPbgFile` constructor at `0x00435300`. `0x00435320-0x0043533D` and `0x004356A0-0x004356BE` are MSVC deleting-destructor helpers and are classified compiler-generated exclusions. Three additional CC-delimited raw bodies remain deliberately unresolved: destructor-shaped `0x00435690-0x00435696`, constructor-shaped `0x004356C0-0x004356C6`, and slash-pointer helper `0x004356D0-0x004356E3`; plausible shape is not enough to force source origin/ownership. | Direct target vtable/raw bytes; pinned normal-COFF generated helper shapes; no-origin policy for unreferenced retained bodies |
| PBGFILE-004 | observed | The connected higher-level pack/file seam is independent from the `CPbgFile` vtable. `0x004356F0-0x004357C3` derives a `.dat` name from the first path component and searches active 0x10-byte `PbgArchive` objects at `0x004923B0` using count `0x00477850`. Ghidra-missed `0x004357D0-0x004357F3` selects the next archive object, calls reviewed `PbgArchive::Load`, and increments that count on success; maintained `FileSystem::LoadArchive` source now records this behavior while its original identifier and physical owner remain unknown. A pinned VC7.1 /GL diagnostic emits 39 bytes for the 36-byte target and remains a mismatch. `0x00435800-0x004358D5` reads from a matching archive or a temporary disk `CPbgFile`; `0x004358E0-0x0043599D` returns the corresponding archive or disk size. The `0x00436400` ResFile owner seam consumes the latter two helpers. | Direct TH10 raw disassembly/global references and bounded call scans; attested decompile/xrefs; ResFile caller context |
| PBGFILE-005 | observed / exact-normal-coff | `CPbgFile::Write @ 0x004354D0` is a 63-byte authored normal-COFF leaf. Writing the final byte-count equality as an explicit `if (dataLen == numBytesWritten) return true; return false;` causes pinned VC7.1 SP1 build6030 to emit the retail post-`WriteFile` sequence `cmp esi,[esp+4]; sete al` instead of materializing the stack value in ECX and zeroing EAX first. Canonical unit `pbgfile-write` reproduces all 63 bytes and its sole DIR32 relocation to `__imp__WriteFile@20` at `0x004660D4`; two independent cold replays pass, and the six pre-existing normal-COFF exact units sharing `PbgFile.obj` remain exact. This bounded codegen fact does not establish production TU or LTCG ownership. | Maintained natural source; pinned VC7.1 SP1 normal-COFF cold replay; complete target extent and relocation replay |
| PBGFILE-006 | observed / canonical-coff-exact / compiler-lifecycle-origin-indeterminate | `IPbgFile::~IPbgFile @ 0x00435690` is a seven-byte base-vtable restoration body: `mov [ecx], ??_7IPbgFile@@6B@; ret`. The existing inline virtual base destructor declaration in `src/PbgFile.hpp`, compiled through `src/PbgFile.cpp`, emits the complete target extent and its sole vtable DIR32 relocation byte-identically under pinned normal COFF. This closes a previously source-unmapped target extent, but exact codegen cannot distinguish an explicitly authored empty virtual destructor from an MSVC-generated lifecycle contribution, so origin remains indeterminate. | Hash-attested TH10 target bytes and IDA disassembly; canonical `pbgfile-base-destructor` replay |

## Resources

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| RESFILE-001 | observed | The target has a 0x10-byte memory-backed `IPbgFile` implementation with vtable `0x0046F308`: `Open` `0x00436400`, `Close` `0x00436430`, `Read` `0x00436460`, read-only `Write` `0x00436370`, `Tell` `0x004364D0`, `GetSize` `0x004364E0`, `Seek` `0x004364F0`, and deleting destructor `0x004363A0`. Raw constructor `0x00436380-0x00436391` writes that vtable and zeroes fields. Storage is vptr `+0x00`, DWORD size `+0x04`, current byte pointer `+0x08`, and owned malloc buffer `+0x0C`. Maintained `CMemoryPbgFile` is a descriptive reconstruction name, not an original identifier claim. | Direct TH10 rdata/vtable bytes, CC-delimited raw disassembly, field accesses, and target-bound compiler probes |
| RESFILE-002 | observed | The memory-file implementation is read-only and owns its buffer. `Open` calls the reviewed archive-or-disk data and size helpers, stores buffer/size/current, and ignores its second stack argument. `Close` frees the buffer and clears all three fields. `Read` copies the requested bytes when available or the remaining tail and advances current; `Write` always returns false; `Tell` is current-minus-base and `GetSize` returns `+0x04`. `Seek` implements target-specific `FILE_BEGIN/CURRENT/END` bounds, including allowing negative current-relative movement without a lower-bound check and rejecting exact-end/exact-beginning boundary cases. Maintained source deliberately preserves these observed edge conditions rather than normalizing them. | Direct TH10 disassembly of `0x00436370-0x0043655A`; both vtable references; reviewed `0x00435800`/`0x004358E0` helper contracts |
| RESFILE-003 | observed | A second vtable at `0x0046F328` is a derived 0x10-byte Win32-resource-backed implementation. It shares `Close`/`Read`/`Write`/`Tell`/`GetSize`/`Seek` with `0x0046F308`, overrides `Open` with `0x004366C0-0x00436767`, and has deleting destructor `0x00436660`. A direct allocation site at `0x00434CA0` performs `operator new(0x10)`, clears the three fields, and writes `0x0046F328`. The override closes prior data, loads `RT_RCDATA` using `FindResourceA`/`LoadResource`/`LockResource`, mallocs and copies `SizeofResource` bytes, and target exits return true even on the reviewed failure paths after cleanup. Maintained `CWin32ResourcePbgFile` is descriptive only. | Direct TH10 allocation/vtable/disassembly/import evidence; attested decompile at `0x00434CA0` and `0x004366C0` |
| RESFILE-004 | observed / unknown | Denominator review added eight Ghidra-missed CC-delimited ResFile entries: `0x00436370`, `0x00436380`, `0x00436400`, `0x00436460`, `0x004364D0`, `0x004364E0`, `0x004364F0`, and `0x00436640`. The first seven are reviewed authored memory-file source bodies. `0x00436640-0x00436651` and existing `0x00436680-0x004366B0` remain deliberately `unknown/review`: each is consistent with a compiler-synthesized derived constructor/destructor with inlined base lifecycle, but TH10 cannot distinguish that from explicit source. `0x004363A0` and `0x00436660` are independently classified compiler-generated deleting destructors. `0x004362B0/0x00436330` were correctly excluded from the ResFile owner; later target-local compression review identifies them as LZSS tree helpers rather than unrelated ResFile code. `0x00436570` remains outside this owner as process-launch logic. | Direct CC boundaries/vtables; bounded target analysis; later LZSS tree owner evidence refines the earlier routing without changing the ResFile boundary |
| RESFILE-005 | observed / noncanonical-negative-codegen | Target `CMemoryPbgFile::Seek @ 0x004364F0` occupies 107 bytes. Its dispatcher saves ESI before testing the seek origin, routes origin values 0/1 to begin/current blocks, falls through to the end block for value 2, and shares one false-return epilogue. The maintained natural switch emits a 117-byte fixed normal-COFF candidate with 12/107 comparable bytes; reordering source cases to end/current/begin leaves that output byte-identical. Ghidra currently has no function boundary at this address; the two target references are the ResFile vtable entries. Keep the source behavior and exactness status unchanged unless new target-local source-shape evidence appears. | Hash-attested target PE disassembly; vtable xrefs; pinned VC7.1 SP1 `probe-exact-backlog.py` and `/GL` diagnostic; fixed-normal case-order replay |
| RESFILE-006 | observed / canonical-coff-exact / corrected-ABI | `CMemoryPbgFile::Open @ 0x00436400` is the 46-byte vtable Open method. Target instructions independently prove the connected archive-or-disk helper ABI: filename is in ECX for both 0x00435800 and 0x004358E0, while 0x00435800 additionally consumes one stack optional-buffer argument. The old free/cdecl declarations therefore caused the historical 51/46, 8/38 diagnostic. Maintained `ArchiveOrDiskPathView` is only a descriptive physical-call view; it does not claim an original class/TU. With that ABI and the natural assignment-expression return `return m_Current = m_Data;`, VC7.1 emits target ordering exactly: data -> +0x0C, size -> +0x04, `MOV EAX,[+0x0C]`, store to current +0x08, `TEST EAX,EAX`, `SETNE AL`. The normal-COFF COMDAT is 46 bytes with 38/38 comparable bytes and exact REL32 fields to both helpers. | Hash-attested target body and helper call setup; maintained `src/ResFile.cpp`; pinned VC7.1 SP1 normal-COFF comparison; canonical `memory-pbgfile-open` unit |
| RESFILE-007 | observed / canonical-coff-exact | `CWin32ResourcePbgFile::Open @ 0x004366C0` closes as a complete 168-byte normal-COFF unit. Natural shared failure cleanup preserves every observed quirk (true on failure, FreeResource(NULL) on LockResource failure, no FreeResource on allocation failure). Routing all four failed operations to the same source cleanup, including LockResource failure, makes VC7.1 reproduce the target's independently duplicated physical epilogues and register allocation. The intermediate three-path funnel was 168 bytes but had eleven ordinary-byte differences; pointer aliases, declaration permutations, TU order and G5/G6/G7/GS/EHsc controls were byte-neutral, while size optimization and positive success layout regressed. The winning full extent and all seven link fields passed two cold canonical builds; all nine ResFile units passed the affected replay. This session used hash-attested disk bytes/PE imports under the operator's explicit no-Ghidra instruction; no fresh Ghidra attestation or production-TU claim follows. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## Compression

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| COMPRESSION-001 | observed / corroborated | `0x00435DC0-0x00435F9A` is a reviewed authored LZSS-style decoder with four stack arguments and `ret 0x10`: input, compressed size, optional output buffer, and output size. It allocates output only when the caller supplies null, starts the ring head at 1, decodes a one-bit literal/backreference tag, uses a 13-bit match offset where zero terminates the stream, uses a 4-bit length field representing actual length minus three, mirrors every output byte into a 0x2000-byte ring with mask `0x1FFF`, and consumes trailing bits through the same fetch path. Maintained `DecompressData` is adjacent-supported naming after TH10 algorithm recovery, not an original identifier/TU claim. | Full TH10 disassembly plus four attested call sites; committed TH08/TH09/TH095 decoder source only after target-local algorithm recovery |
| COMPRESSION-002 | observed | The decoder/encoder dictionary is physically bounded to exactly `0x2000` bytes at `0x0048F868-0x00491867`. Decoder and encoder accesses mask indices with `0x1FFF`; the reviewed neighboring encoder clears all 0x2000 bytes before encoding; independent densely used storage begins exactly at `0x00491868`. The same ring is therefore shared encoder/decompressor state rather than decoder-private scratch. Maintained source defines one `g_DecompressionRing[0x2000]`; original source/data TU ownership remains unknown. | TH10 decoder/encoder target accesses and mask constants; target xrefs at the next storage boundary; compile-time maintained-source size assertion |
| COMPRESSION-003 | observed / corroborated | Two complete LZSS state helpers were missing from the Ghidra denominator. `0x00435FA0-0x00435FC3` consumes the root through live-in EAX, writes it into the sentinel tree node, sets `tree[root].parent = 0x2000`, and clears both child links; `0x00435FD0-0x00435FF9` clears the entire 0x2000-byte dictionary and all 0x2001 three-dword tree nodes. Both are CC-delimited, have no modeled entry xrefs, and their behavior is inlined in the neighboring encoder. `Lzss::InitTree` and `Lzss::InitEncoderState` are adjacent-supported proposed names only. A natural normal-COFF `InitEncoderState` probe is also 42 bytes and matches all 30 comparable non-relocation bytes; the 36-byte `InitTree` probe differs because standalone source reads the root from the stack while target consumes EAX. `an earlier canonical promotion` records the later relocation-complete exact promotion of only `InitEncoderState`. | Direct TH10 raw boundaries/storage writes and encoder inline copy; Ghidra no-function/no-xref results; pinned VC7.1 helper probes; adjacent source only for naming |
| COMPRESSION-004 | observed / corroborated | `0x004359B0-0x00435DB4` is a reviewed 1,029-byte authored LZSS encoder with three stack arguments and `ret 0x0C`: input, input size, and output-size pointer. Its two retail callers use that exact boundary and consume the returned allocated buffer. The target allocates `inputSize*2`, resets the shared ring/tree state inline, preloads up to 18 bytes, emits literal or 13-bit-offset/4-bit-length match tokens, maintains the sliding dictionary/tree, writes a zero-offset terminator, stores compressed output size, and returns the buffer. The connected target tree helpers are reviewed as `Lzss::AddString` `0x00436000-0x00436203`, `DeleteString` `0x00436210-0x0043625F`, `ContractNode` `0x00436260-0x004362AE`, `ReplaceNode` `0x004362B0-0x00436324`, and `FindNextNode` `0x00436330-0x00436360`. Their machine boundaries use private EDX/ECX/ESI/EAX inputs; maintained source remains natural C++ and does not claim those as source calling conventions. The 1,029-byte CompressData owner and 516-byte AddString owner are now canonical exact in the target CompressData `/GL` context: natural `__stdcall` declarations reproduce the target callee-pop surfaces, the `matchLength <= 2` break-even spelling, complete extents, and all 37/27 target-bound fields respectively. | TH10 full disassembly, attested call graph/callers, raw caller sequences, and shared ring/tree accesses; pinned VC7.1 linked-PE replays `lzss-compress-data` and `lzss-add-string`; committed adjacent LZSS source only after target-local recovery |
| COMPRESSION-005 | observed | The LZSS tree is physically bounded to exactly `0x00477858-0x0048F863`: 0x2001 nodes of 0x0C bytes with fields parent/left/right. `tree[0x2000]` is the sentinel/root node; its right field at `0x0048F860` is written by encoder initialization and read by AddString. `0x00477850` belongs to an independent archive counter, `0x00477854` has no observed reference, and `0x0048F864-0x0048F867` is a four-byte unreferenced gap before the separately reviewed 0x2000-byte dictionary at `0x0048F868`. Maintained `g_LzssTree[0x2001]` mirrors this target-proven extent, while original source/data-TU ownership remains unknown. | TH10 address arithmetic, all tree helper accesses, attested xrefs at both boundaries, and compile-time maintained-source layout assertions |
| COMPRESSION-006 | observed / unknown | TH10 retains source-path strings `.\src\pack\LzssUtil.cpp:201 BYTE` and `.\src\pack\LzssUtil.cpp:56 BYTE`, establishing that `src/pack/LzssUtil.cpp` participated in the original build. Neither string start has a modeled code xref, so these strings do not bind the reviewed encoder/decoder/tree functions or data objects to that original TU. Maintained names `CompressData` and `Lzss::*` are therefore source-shape/naming choices supported by TH10 behavior and adjacent committed source, not recovered original-symbol or physical-TU claims. | Attested TH10 string search and negative xref query; target-local LZSS behavior; committed TH08/TH09/TH095 naming corroboration |
| COMPRESSION-007 | observed / exact | `Lzss::DeleteString @ 0x00436210` consumes its node index in ECX, including the recursive call. The maintained static helper declares `__fastcall`, matching that target entry contract. Its canonical linked-PE unit reproduces the complete 80-byte PDB-owned extent and all seven DIR32/REL32 fields, including the recursive helper tail call. Production physical owner remains unknown. | Hash-attested target Capstone disassembly; maintained declaration/source; canonical LZSS linked-PE comparison and cold replay |

## Animation

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ANM-001 | observed / source-present | TH10's ANM VM lifecycle establishes a complete `0x3AC`-byte object view. The constructor at `0x00402050` first clears bit zero in nine nested timer flags at VM `+0x6C/+0xB0/+0xFC/+0x128/+0x174/+0x1B0/+0x1FC/+0x228/+0x378`, clears exactly `0xEB` dwords, and sets the signed active-sprite index at `+0x384` to `-1`. The destructor at `0x00401FF0` frees and clears the optional generated-vertex pointer at `+0x358`. Reusable reset `0x00401DE0` preserves owner `+0x20` and three fields `+0x340/+0x344/+0x348`, clears the full object, restores self-linked sentinels at `+0x04/+0x10`, scale `+0x3C/+0x40`, identity matrix `+0x23C`, flags `+0x35C = 7`, primary timer `+0x5C`, seven observed state fields, and sentinel `+0x2FC = -1`. Two adjacent CC-delimited retained helpers implement matrix identity at `0x00401F00` and timer initialization at `0x00401F40`; both operations also appear inline in reset. TH08/TH095 were consulted only for the broad ANM-VM lifecycle hypothesis; every size, offset, boundary and behavior here comes from TH10-local evidence. | Hash-attested TH10 constructor/destructor/reset disassembly and xrefs; target store ranges and constants; pinned VC7.1 layout/code-generation probes; adjacent games used only as hypotheses |
| ANM-002 | observed / source-present | The real renderer reached through pointer `0x00491C10` owns a `0x20000`-entry packed vertex buffer. Each vertex is `0x1C` bytes, so the observed base at renderer `+0x3ADACC` spans exactly `0x380000` bytes to end cursor `+0x72DACC`; start cursor follows at `+0x72DAD0`. Sprite count is at `+0x3ADAC8` and per-frame flush count at `+0x58`. `ClearVertexBuffer @ 0x00442F30` clears the sprite count and resets both cursors. `AddSpriteToDrawBuffer @ 0x00442FE0` copies quad corners `0/1/2/1/2/3`, advances the end cursor by six vertices (`0xA8` bytes), increments the sprite count, and returns zero. `FlushVertexBuffer @ 0x00442F50` submits two D3D9 triangle-list primitives per sprite from the current start cursor, advances start to end, clears the count, and increments the flush count. | Hash-attested TH10 disassembly and 22 flush callers; buffer-offset arithmetic; target D3D9 vtable slots; TH08/TH095 used only for the initial source-shape hypothesis |
| ANM-003 | observed / source-present | TH10's shared four-vertex draw owner is `DrawInner @ 0x00442670`. It adds renderer screen shake `+0x5C/+0x60`, optionally applies four x87 nearest-even rounds followed by the D3D9 half-pixel subtraction, copies UVs from sprite `+0x20..+0x2C` plus VM scroll `+0x54/+0x58`, culls four computed bounds against the unsigned viewport reached through global `0x00491FAC`, flushes on texture/shader changes, selects VM primary/secondary color at `+0x2FC/+0x300`, optionally mixes all BGRA channels with renderer color `+0x732458`, updates blend/filter state, and queues the quad. The axis-aligned owners at `0x00443080/0x00443290` implement the three anchor modes in VM flag bits 18-21. The no-round owner has a target-local difference: its common Z reads VM `spriteOffset.y @ +0x350`, while the rounded owner reads `spriteOffset.z @ +0x354`. | Hash-attested TH10 disassembly/decompilation and caller seams; target field accesses; compile-time layout checks; TH08/TH095 consulted only after TH10-local behavior was established |
| ANM-004 | observed / source-present | `AnmRenderManagerView::ProjectCameraFacingQuad @ 0x00443B60` builds an identity world matrix translated by VM `position + preservedPosition + spriteOffset`, projects zero and camera-right through the active owner at global `0x00491FAC`, rejects the first projected Z outside `[0,1]`, derives screen scale from the projected-vector distance, applies VM Z rotation and anchor bits 18-21, and writes the projected Z to all four shared vertices. The target-proved viewport-owner layout is camera-right `+0x30`, view matrix `+0x4C`, projection matrix `+0x8C`, and viewport `+0xCC`. The 33-byte wrapper at `0x00443F80` propagates projection failure or enters `DrawInner(vm, 0)`. Direct CC-boundary review also adds the Ghidra-missed `TranslateRotation @ 0x00443680` to the denominator. TH095 was consulted after TH10-local recovery and differs in its final Z source. | Hash-attested TH10 disassembly, import thunk, callers and global accesses; compile-time layout checks; adjacent source used only as a later source-shape hypothesis |
| ANM-005 | observed / source-present | The former combined `0x004436C0-0x00443B5E` range is two real 591-byte rotated-draw entries separated by `CC @ 0x0044390F`. `Draw @ 0x004451C0` extracts VM render mode from flags bits 22-25 and tail-jumps from mode 1 to `0x004436C0` and mode 3 to `0x00443910`. The bodies are instruction-identical except for their REL32 displacements: zero Z rotation enters `DrawNoRotationNoRound`; otherwise they use one x87 `FSINCOS`, accumulate `spriteOffset + preservedPosition + position` for X/Y/Z, scale sprite dimensions, apply anchor modes from bits 18-21, rotate all four shared XY pairs, and call `DrawInner(vm, 0)`. Maintained names are `Draw2D` and TH08-corroborated `Draw2DRotatedOrAxisAligned`; the second original identifier remains a cross-game hypothesis. | Hash-attested TH10 raw boundaries and complete bodies; Ghidra xrefs from the target Draw jump table; target-local field/control-flow evidence; TH08 consulted only for the mode-3 name hypothesis |
| ANM-006 | observed / source-present | `Project3DQuad @ 0x00444240` maintains two VM matrices at `+0x23C/+0x27C`. Unless flag bit 14 suppresses recomputation, dirty scale/rotation bits copy the first matrix to the second, multiply `_11/_22` by VM scales, apply nonzero X/Y/Z rotations through the D3DX helpers, and clear the dirty bits. A local 256-by-256 quad uses anchor bits 18-21; X/Y add `spriteOffset + preservedPosition + position` to the cached matrix translation, while TH10 replaces Z with that three-vector sum. Four `D3DXVec3Project` calls use the active viewport owner, and the completed matrix is cached at renderer `+0x3AD0F0`. `DrawProjected3DQuad @ 0x00444580` then calls `DrawInner(vm, 0)` and restores all shared vertex RHW fields to `1.0`. TH095 supplied the initial source-shape hypothesis, but the Z behavior, offsets, calls, flags and boundaries are TH10-local. | Hash-attested TH10 disassembly and call targets; target field/global accesses; compile-time VM/renderer layout checks; adjacent source used only as a hypothesis |
| ANM-007 | observed / source-present | `AnmRenderManagerView::DrawMode6 @ 0x00443FB0` first projects a camera-facing quad, computes the distance from accumulated VM position to the background camera at `0x00491D7C`, and optionally applies the renderer color multiplier. Beyond the near distance at `0x00491E78`, it interpolates BGRA toward float B/G/R channels at `0x00491E80/84/88` and zero alpha across the near/far interval, rejects a completed fade, copies the result to all four shared vertices, and enters `DrawInner(vm, 2)`. `Draw @ 0x004451C0` independently rejects invisible, draw-disabled, and zero-primary-alpha VMs, then dispatches flag bits 22-25 across modes 0-9. Mode 9 passes the VM vertex pointer and twice its count to the generated-vertex owner. The dispatcher's physical PDB contribution is 216 bytes through `0x00445297`: 173 code bytes, a three-byte alignment instruction, and the ten-entry absolute jump table at `0x00445270`. | Hash-attested TH10 disassembly, target global/field accesses and dispatch table; real caller at `0x00409230`; compiler-linked PDB contribution extent |
| ANM-008 | observed / exact | `AnmRenderManagerView::DrawMode7 @ 0x004445C0` is TH10's projected-3D photo-blend path. It calls `Project3DQuad`, then transforms each of the four 0x14-byte renderer-local source vertices at `+0x3ADA78` through cached world matrix `+0x3AD0F0` using `D3DXVec4Transform`. Each transformed XYZ is measured independently from camera global `0x00491D7C`. Vertices inside near distance `0x00491E78` retain the selected VM color; intermediate vertices interpolate B/G/R toward float channels `0x00491E80/84/88` while preserving VM alpha; vertices at or beyond the far end receive dword color `0x00491E90` with VM alpha restored. Unlike mode 6, mode 7 does not apply renderer mix color and does not reject a fully blended quad. It enters `DrawInner(vm, 2)` and restores all shared RHW fields to one. | Hash-attested TH10 disassembly and fields; canonical 402-byte linked-PE replay with all 24 fields; TH095 exact source consulted only as the initial source-shape hypothesis |
| ANM-009 | observed / source-present | `SetRenderStateForVm3D @ 0x004423E0` is the direct-3D sibling of the batched 2D state owner. It flushes before incompatible blend, texture-factor, or sampler changes; maps blend mode zero to inverse-source-alpha and modes one and two to one; selects the VM primary/secondary color; optionally applies the renderer's per-channel mix; publishes the result through D3D9 texture factor; and maintains the point/linear filter cache. `Draw3D @ 0x00444760` flushes pending 2D sprites, updates the VM scale/Euler-rotation matrix, applies anchor translation from all three VM position vectors, sets world and optional texture transforms, installs the renderer's 0x14-stride four-vertex stream, and draws a two-primitive triangle strip. TH10 binds the sprite texture before refreshing the sprite/UV transform cache, tests `uvScrollX` twice in the texture-transform condition, and briefly forms world Z without `spriteOffset.z` before overwriting it with the full three-vector sum after the state helper; maintained source preserves each observed operation. | Hash-attested TH10 disassembly, target D3D9 vtable calls, renderer/VM field accesses and dispatcher ABI; compile-time layout checks; TH095 source used only as the initial source-shape hypothesis |
| ANM-010 | observed / source-present | The generated-geometry corridor contains eight authored source bodies. `InitializeHorizontalTextureStrip @ 0x00444B10` and its coordinate-transposed vertical sibling at `0x00444BE0` fill alternating rows/columns with sprite UVs, VM primary color, and unit RHW. `SetGeneratedVertexColor @ 0x00444CB0` copies primary color across a 0x1C-stride range. `DrawGeneratedVertices @ 0x00444CE0` is dispatcher mode 9's visible/alpha-gated textured triangle-strip path; `QueueSpriteQuad @ 0x00444DC0` feeds a supplied quad into the shared batch. The 0x14-stride untextured strip/fan paths at `0x00444E60/0x00444FA0` temporarily select diffuse-only texture operations and disable Z writes, then restore renderer caches and normal texture operations. `DrawTexturedTriangleFan @ 0x004450E0` is the textured fan path called with 33 generated vertices at `0x00445880`. The initial Ghidra inventory missed complete CC-delimited entries at `0x00444CB0`, `0x00444DC0`, `0x00444E60`, and `0x00444FA0`; those four are now in the denominator. Names without target callers are descriptive or adjacent-supported, while boundaries, ABIs, state values, strides and behavior are TH10-local. | Hash-attested target disassembly and boundaries; Ghidra dispatcher/caller xrefs; bounded absolute-pointer and direct-call scans; TH08/TH095 source consulted after TH10 behavior was established |
| ANM-011 | observed / source-present | TH10's generated pulsing radial trail uses a 0x4B0-byte VM-owned payload: 33 textured 0x1C-byte vertices, 33 radii, 33 radial velocities, a two-float UV velocity and one trailing dword. `InitializePulsingRadialTrail @ 0x004452F0` replaces the allocation, stores update/draw callbacks at VM `+0x398/+0x39C`, initializes a center vertex plus 31 radial vertices, and derives radii/velocities from RNG object `0x004918B0`. `UpdatePulsingRadialTrail @ 0x00445620` scrolls both U and V with the payload X velocity, adds one to all 33 coordinates after a negative wrap, advances/closes the ring, copies VM primary color to the center, and clears ring alpha. `DrawPulsingRadialTrail @ 0x00445880` submits all 33 vertices through the textured triangle-fan path. `AnmFloat3View::FromAngleMagnitude @ 0x004458B0` uses one x87 `FSINCOS` and intentionally leaves Z unchanged. TH095 supplied the initial source-shape hypothesis only after TH10 fields, constants, callbacks, RNG state, boundaries and the V-uses-X quirk were established locally. | Hash-attested TH10 disassembly and direct caller at `0x0043FDFB`; callback stores and calls; target field/constant accesses; compiler source-shape comparison; committed TH095 source as later hypothesis material |
| ANM-012 | observed / source-present | The four script-variable helpers immediately before `ExecuteScript @ 0x0043EE30` establish VM integer locals at `+0x30C..+0x318`, float locals at `+0x31C..+0x328`, and counter locals at `+0x32C/+0x330`. `GetFloatVar @ 0x0043EAC0` accepts IDs 10000-10021: the locals and counters, three RNG forms selected between owners `0x004918A8/0x004918B0` by VM flag bit 30, VM position, background-camera Float3 `0x00491D7C`, and a second conservatively named Float3 at `0x00491DA0`. `GetIntVar @ 0x0043EC70` resolves IDs 10000-10009, converting the four float locals through `__ftol2`. The two pointer helpers at `0x0043ED00/0x0043EDA0` first test the instruction argument mask, then redirect writable float operands to four float locals or position XYZ and writable integer operands to four integer locals or two counters. Their reviewed physical contributions include the trailing absolute jump tables: 424, 144, 156, and 132 bytes respectively. TH095 corroborates source shape and names only through position Z; TH10-local reads establish the extra six global IDs and every accepted offset. | Hash-attested target disassembly and physical boundaries; attested caller/decompilation of the adjacent script executor; target global/VM accesses; compiler-generated table layouts; adjacent source used only as a later hypothesis |
| ANM-013 | observed / source-present / owner-corrected | `AnmRenderManagerView::ExecuteScript @ 0x0043EE30-0x0044151B` is the reviewed 9,964-byte physical owner. It contains 9,587 bytes of decoded executor code through the final `RET 4`, one alignment NOP, and the complete 94-slot absolute dispatch table at `0x004413A4-0x0044151B`; signed opcode+1 is bounded to 0x5D and maps to 92 physical instruction groups. Maintained source covers opcodes `-1..92`, interrupts, variable arithmetic/branches, RNG/trigonometry, sprite/render flags, child-VM creation, interpolation, generated mode-9 geometry and the shared instruction/timer tail. The owner remains non-exact; keep the reviewed 92-group order and target 0xFC frame/pre-table constraints when changing allocator lifetimes. | `config/functions.csv`; complete target disassembly/table xrefs; maintained `src/AnmManager.cpp` |
| ANM-014 | observed / source-present | The executor's interpolation family consists of a 0x4C-byte Float3 slot, two 0x4C-byte integer-color slots, two 0x2C-byte scalar-alpha slots and a 0x3C-byte Float2 scale slot. Evaluators implement modes `0..17`, including endpoint completion, state-mutating add/accelerate behavior, component-wise integer truncation in Hermite color mode, and the shared curve function at `0x0044C350`. Setup functions at `0x0041AB70`, `0x00441F50`, `0x00442050`, `0x00442220` and `0x00442300` install endpoints/mode/duration and reset their embedded timers. `SetSprite @ 0x0043E5A0` binds the loaded sprite and rebuilds texture matrices; `AddNormalizeAngle @ 0x0044BC10` bounds a signed angle with the target loop cap; timer `Add @ 0x0044BF40` applies the speed pointer outside the near-one interval. | Target helper bodies/callers and VM offsets; target constants and private ABIs; maintained executor call context; adjacent interpolation code only as later hypothesis material |
| ANM-015 | observed / source-present | TH10's ANM manager owns an inline pool of 4096 complete 0x3AC-byte VMs at manager `+0x68`, 4096 occupancy bytes at `+0x3AC068`, the next pool index at `+0x3AD068`, primary and secondary list head/tail pairs at `+0x72DAD4..+0x72DAE0`, twenty draw-layer sentinel VMs at `+0x72DAE4`, and the next nonzero VM id at `+0x732454`. Each VM has a distinct manager-order node at `+0x04` and parent/child layer node at `+0x10`; its draw-layer link and layer value are `+0x1C/+0x20`. Maintained source now covers pool allocation, front/back insertion in both manager lists, two-list lookup, stale-id clearing, interrupt/immediate-interrupt propagation, tree deletion, screen/world position propagation, position lookup, loaded-resource deletion, and all twelve creator variants. The creator family consists of four render-layer forms plus screen and world position forms for each; the world forms add TH10's directly observed `(+224,+16)` playfield origin. Six creator bodies plus `GetVmPosition` omitted by the initial Ghidra inventory are now included in the candidate denominator from direct target boundary review. | Hash-attested TH10 manager/VM address arithmetic, creator corridor and call graph; direct i386 disassembly and CC-delimited boundaries; maintained VC7 layout assertions; adjacent source used only after TH10-local recovery for name/source-shape hypotheses |
| ANM-016 | observed / source-present | Three adjacent loaded-ANM methods implement distinct script transitions. `InitializeAndExecuteScriptIndex @ 0x0043E710` performs new-VM initialization before binding and frame-zero execution; `SetAndExecuteScriptIndex @ 0x0043E7E0` binds and executes without that initializer and clears the complete VM when the script is absent or the resource is loading; `SetAndExecuteScriptIdx @ 0x0043E8B0` leaves an invalid existing VM unchanged, otherwise rebuilds flags, primary color, timer and all seven interpolation durations before frame-zero execution. Its pending horizontal-flip path negates `scaleX`, not rotation. Each valid path stores resource/script identity, clears flip bits 9-10, hides the VM, executes the script through the target's callee-cleans `RET 4` entry, and increments manager counter `+0x4C`. | Complete target bodies and direct calls; target field writes and constants; maintained complete executor context; TH08/TH095 used only for later method-family/name corroboration |
| ANM-017 | observed / source-present | `UpdatePrimaryVms @ 0x00448860` rebuilds transient draw layers 0–18 from the primary manager-order list; `UpdateSecondaryVms @ 0x00448900` clears the shared processed count and rebuilds reserved layer 19 from the secondary list. Both save each manager-node successor before callbacks, remove VMs marked by flag `0x04000000` or completed by `ExecuteScript`, run the position callback before the executor, append survivors through VM `nextInDrawLayer +0x1C`, and increment manager `+0x64` for every visited VM. `DrawLayer @ 0x00448980` skips newly marked VMs, invokes an optional draw callback at `+0x39C`, then reaches the common draw dispatcher. `RemoveVm @ 0x00448BB0` updates either manager head/tail pair, unlinks both VM nodes, clears the corresponding occupancy byte for VMs inside the 4096-entry pool, frees generated storage, and resets the VM in place; heap fallbacks are destroyed and deleted. | Fresh Factory-attested TH10 decompile/disassembly/calls/xrefs; direct target disassembly; maintained VC7 layout assertions and source; adjacent source only as an initial control-flow hypothesis |
| ANM-018 | observed / exact | `AnmRenderManagerView::AnmRenderManagerView @ 0x00445900` constructs the 4096 pool VMs, one complete embedded primary VM at manager `+0x3AD130`, and twenty draw-layer sentinel VMs before its body clears the complete `0x732460`-byte manager. It initializes the two global four-vertex UV templates, restores the manager state caches, initializes every pool VM, registers update callbacks at priorities `0x1A/0x08`, registers draw callbacks for layers 0–16 and 19 at their eighteen target priorities, and clears the active D3D9 vertex shader. The embedded VM's relative `generatedVertices +0x358` is manager `+0x3AD488`; this corrects the former interpretation of that field as a standalone allocation owner. `SetupVertexBuffer @ 0x004462F0` constructs the manager's fixed `-128..+128` XYZ/UV quad, mirrors XYZ/UV into the independent `0x00497930` background vertices, creates and fills an 80-byte managed D3D9 vertex buffer, and binds stream zero with stride `0x14`. Canonical linked-PE unit `anm-setup-vertex-buffer` reproduces its complete 472-byte extent and all 22 target-bound background-vertex/device DIR32 fields under the ExecuteScript `/GL /GS` graph. | Hash-attested TH10 disassembly and member-construction sequence; raw caller at `0x0042025B`; callback addresses/priorities; D3D9 vtable slots; compile-time manager/VM layout checks; two independent cold canonical replays with pinned VC7.1 SP1 build6030; TH095 consulted only after TH10 established the embedded-primary-VM hypothesis |
| ANM-019 | observed / source-present | `ApplyTextureAlphaBleed @ 0x004465B0` locks texture surface level zero and fills RGB for zero-alpha pixels from nonzero-alpha orthogonal neighbors. It supports serialized/D3D formats zero and 21 as 32-bit ARGB, 25 as A1R5G5B5, and 26 as A4R4G4B4; one neighbor is copied, two or more are averaged, and no neighbors produce zero RGB while alpha remains zero. Its physical contribution is 1,463 bytes through `0x00446B66`: code returns at `0x00446B38`, followed by three alignment bytes, a four-entry absolute jump table and a 27-byte selector table. Four target callers at `0x00446C38`, `0x00446D48`, `0x00446E92`, and `0x00446F1F` establish the texture-entry argument. | Hash-attested TH10 code/table bytes, direct call scan, surface ABI and target format tables; Factory-attested decompilation; TH095 source used only after target-local control flow and formats were established |
| ANM-020 | observed / source-present | The manager's texture/resource pipeline at `0x00446C70-0x004479CF` loads complete or rectangular textures, creates textures from files and serialized ANM payloads, parses synchronous/preloaded ANM resources, relocates entry-owned script/sprite offsets, uploads embedded/external texture data, services queued loads, releases resource ownership, and materializes sprites. The manager owns 33 `0x130`-byte `AnmLoadedView` records at `+0x3AD06C`; each view retains its file buffer, first entry, 32 script pointers and 32 sprite pointers. Raw entry, sprite and texture-header layouts are bound by compile-time offsets derived from direct target reads. `ServicePreloadedAnms` preserves the target-observed apparent null-slot write in the release branch instead of silently correcting it. | Hash-attested TH10 disassembly, direct manager/entry offsets and resource call graph; D3DX/D3D9 imported ABIs; maintained layout assertions; adjacent source consulted only after TH10-local structure recovery |
| ANM-021 | observed / source-present | The ANM text family at `0x004479D0-0x00447C71` formats into guarded 512-byte stack buffers, selects a texture and sprite through the VM resource identity, rasterizes within the sprite rectangle, and publishes VM visibility. VM bytes `+0x3A0/+0x3A1` are glyph width/height; flag bit one at `+0x360` selects the alternate rasterizer. Right alignment subtracts half of `strlen(text) * (glyphWidth - 1)` from sprite width, while centered alignment starts at half sprite width minus one quarter of that span. The surface/capture family at `0x00447C80-0x004485CA` loads and preloads offscreen surfaces, releases paired surface/data ownership, copies either primary or backup surfaces to the backbuffer, captures backbuffer rectangles into textures or paired surfaces, and copies texture subrectangles. Manager arrays at `+0x3AD4E0/+0x3AD560/+0x3AD5E0/+0x3AD660` hold 32 primary surfaces, backup surfaces, data buffers and sizes; 32 image-info records of stride `0x1C` begin at `+0x3AD6E0`. | Direct TH10 instruction/branch and field-offset review; target D3D9 vtable calls and D3DX imports; maintained exact-sized view types; TH08/TH095 used only for later source-shape corroboration |
| ANM-023 | observed / exact-five-units | Correcting complete terminal-instruction ownership closes five ANM exact owners. `TranslateRotation @ 0x00443680` is 52 bytes including `RET 24`; `QueueSpriteQuad @ 0x00444DC0` is 145 bytes including `RET 4`; the untextured triangle-strip/fan siblings at `0x00444E60/0x00444FA0` are each 315 bytes including `RET 8`. Their pinned VC7.1 `/GL` PDB contributions reproduce all 827 bytes and 37 declared linked fields exactly in the established renderer context. Independently, normal COFF reproduces `MarkVmForDeletion @ 0x004492A0` across all 66 bytes and its sole `FindVm` REL32 relocation. Two cold aggregate replays reproduce all five owners, 893/893 bytes. | Canonical linked-PE and COFF units; two cold `scripts/replay-exact-units.py` passes; target-bound corrected extents and relocation/linkage manifests |
| ANM-024 | observed / exact | `AnmRenderManagerView::Draw3D @ 0x00444760-0x00444B00` is now canonical exact across its complete 929-byte linked `/GL` PDB contribution and all 25 code fields. The last 13 non-field differences came from modeling target address `0x00491C30` only as an independent D3D-device pointer. TH10's Main layout and the target instruction schedule establish this access as `MainSupervisorView +0x8`: expressing that one call through a source-local supervisor prefix reproduces the target's push/cache-store/device-load order. The canonical field uses supervisor base `0x00491C28` with addend eight, which also disambiguates the diagnostic harness address shared with a neighboring four-byte anchor. Two cold replays close Draw3D; the second also replays four adjacent renderer owners for 1,756/1,756 bytes. A final full-source gate closes all 66 configured `src/AnmManager.cpp` units across ten cold artifacts and 11,997 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-025 | observed / exact-two-units | `AnmRenderManagerView::Draw2D @ 0x004436C0` and `Draw2DRotatedOrAxisAligned @ 0x00443910` are canonical exact across both complete 591-byte linked `/GL` PDB contributions and 21 code fields per owner. The former near match split each three-term coordinate sum into one assignment and two compound assignments; retaining each sum as one naturally left-associated expression makes VC7.1 complete the horizontal-anchor mask before loading the first Y operand, exactly matching the target's only differing scheduling region. The arithmetic, operand order and behavior are unchanged, and no artificial dependency or padding is used. Two focused cold replays close 1,182/1,182 bytes; the full `src/AnmManager.cpp` gate closes 68 units across ten cold artifacts and 13,179 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-026 | observed / exact | `AnmRenderManagerView::SetRenderStateForVm3D @ 0x004423E0` is canonical exact across its complete 440-byte linked `/GL` PDB contribution and all eight code fields. Its last 12 comparable differences were the same source-ownership scheduling class recovered in Draw3D: target address `0x00491C30` is both the standalone device pointer and `MainSupervisorView +0x8`. Expressing only the texture-factor call through the source-local supervisor prefix reproduces the target push/cache-store/device-load sequence, while the other device accesses remain standalone-global fields. The canonical field binds supervisor base `0x00491C28` plus eight. Two focused cold replays close 440/440 bytes; the full `src/AnmManager.cpp` gate closes 69 units across ten artifacts and 13,619 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-027 | observed / exact-two-units | `AnmRenderManagerView::DrawGeneratedVertices @ 0x00444CE0` and `DrawTexturedTriangleFan @ 0x004450E0` are canonical exact across their complete 224- and 211-byte linked `/GL` PDB contributions and all 7 and 10 declared code fields. Their last 13 differing bytes were the texture-bind schedule already established by Draw3D and `SetRenderStateForVm3D`: expressing only `SetTexture` through the TH10-proved device field at `MainSupervisorView +0x8` makes VC7.1 push the texture and update renderer cache before loading the device, while later calls retain the standalone device global. Two focused cold replays close 435/435 bytes; the final full `src/AnmManager.cpp` gate closes all 71 configured units across ten artifacts and 14,054 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-028 | observed / exact | `AnmVmView::GetFloatVar @ 0x0043EAC0` is canonical exact across its complete 424-byte linked `/GL` PDB contribution and all 44 declared linkage fields. The target physically orders its three RNG bodies as unsigned random, signed random, then signed random multiplied by pi; the maintained switch previously ordered the same mapped cases as angle, unsigned random, signed random. Reordering only those case clauses reproduces the target body while preserving enum values, jump-table destinations and semantics. The exhaustive manifest covers `__ftol2`, six RNG calls and owner globals, both pi references, two three-component global owners with base addends, the table base and all 22 case destinations. Two focused cold replays close 424/424 bytes; the full `src/AnmManager.cpp` gate closes 72 units across ten artifacts and 14,478 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-029 | observed / exact | `AnmRenderManagerView::CreateTextureFromAnm @ 0x00446F40` is canonical exact across its complete 264-byte linked `/GL` PDB contribution and all nine declared fields. The target's two destination/source rectangle `LEA` instructions resolve through their different transient stack offsets to the same local rectangle populated from the embedded header width and height. Passing that rectangle to both `D3DXLoadSurfaceFromMemory` region parameters, rather than passing a null destination region, restores the missing three bytes and all 45 formerly differing comparable bytes while accurately limiting the upload. Two focused cold replays close 264/264 bytes; the full `src/AnmManager.cpp` gate closes 73 units across ten artifacts and 14,742 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-030 | observed / exact | `AnmRenderManagerView::CaptureToSurface @ 0x00448450-0x004485CA` is canonical exact across its complete 379-byte linked `/GL` PDB contribution and all 11 declared fields. The final instruction is the three-byte `RET 36` at `0x004485C8-0x004485CA`, correcting the former claim that its last two bytes were CC padding. TH10 Main layout identifies device `0x00491C30` and backbuffer format `0x00491D14` as `MainSupervisorView +0x8/+0xEC`; expressing both through their common owner restores the render-target and fallback schedules. Natural locals for the selected secondary surface slot and D3D9 device restore the remaining register allocation through the second surface creation and both D3DX copies. Two focused cold replays close 379/379 bytes; the full `src/AnmManager.cpp` gate closes 74 units across ten artifacts and 15,121 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-031 | observed / exact | `AnmRenderManagerView::Draw @ 0x004451C0-0x00445297` is canonical exact across its complete 216-byte linked `/GL` PDB contribution: 173 code bytes, the three-byte alignment instruction at `0x0044526D`, and the ten-entry absolute table at `0x00445270`. TH10 physically orders case blocks as modes `0,1,4,5,6,7,8,9,2,3`; retaining that case-clause order closes the dispatcher body while preserving normal numeric table mappings. The target's two flag failures branch into the alpha-zero rejection block immediately before the switch. A single shared source label inside that real rejection branch reproduces the target CFG without artificial state, padding, assembly, or copied bytes. Two independent cold replays close 216/216 bytes and all 21 fields: table base, ten renderer calls and ten table destinations expressed as offsets within the same owner. The full `src/AnmManager.cpp` gate closes 76 units across ten artifacts and 15,739 target bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-032 | observed / control-flow-corrected | The `ExecuteScript` selector is a direct 94-entry table indexed by signed opcode plus one. Its 94 opcodes `-1..92` resolve to 92 unique destinations because END/DELETE share one body and NOP/INTERRUPT_LABEL share the ordinary instruction-advance body. The physical order begins UNIT_SPEED, child opcodes 88/91/90/92, sprite and basic transforms, then proceeds through control, interpolation, arithmetic, RNG and conditional-jump families before END/DELETE/STATIC. The four child cases call target owners Variant0, Variant2, Variant1 and Variant3 respectively; the earlier maintained 90/91 association was reversed. STOP_HIDE falls through the common STOP body. The executor itself does not invoke VM `positionCallback`; the two manager update owners invoke it before entering the executor, and the target executor proceeds from generated mode-9 geometry directly to the script-timer update. | Hash-attested target dispatch table and complete code disassembly; direct child call targets; manager-update call sites; `scripts/report-anm-execute-table.py` |
| ANM-033 | observed / source-shape-corrected | `AnmVmFloat3InterpolationView::Evaluate @ 0x00404610` uses value-producing vector addition in modes 7 and 17: the new initial value is assigned from `initial + final` or `initial + finalTangent`, and the acceleration tangent is assigned from `finalTangent + final`. Its completed-duration path retains separate initial/final copies and returns. In `ExecuteScript`, position interpolation branches before evaluation and consumes the evaluator's returned pointer in both branches; rotation evaluates through a shared local before copying the returned vector; scale likewise consumes its evaluator's returned pointer. These forms reproduce target-observed temporaries and copy paths without changing behavior. | Hash-attested target disassembly; Factory-native Ghidra decompilation; linked VC7.1 target/candidate data flow; maintained source |
| ANM-034 | observed / evaluator-core-near | TH10's Float2 scale evaluator uses the same separate completed-duration returns and value-object updates as the Float3 owner. Its mode-7 path assigns `initial + final`; mode 17 assigns `initial + finalTangent`, then `finalTangent + final`; Hermite materializes four weighted two-component values before summing their components. In the maintained `ExecuteScript` `/GL` context this source restores the target private `ESI` receiver and `EDI` output ABI and produces a 656-byte contribution against the 652-byte target, with 190/612 comparable bytes matching. The four-byte extent difference remains a code-generation question, so the owner stays source-present/non-exact. The enclosing executor remains at a 9,472/9,588-byte pre-table span, and all 76 canonical ANM units / 10 artifacts / 15,739 bytes cold-replay exactly. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-035 | observed / exact-leaf | `AnmRenderManagerView::GetVmPosition @ 0x004493C0` is a complete 27-byte normal-COFF exact leaf. The maintained ternary had allowed VC7.1 to reuse the null return from `FindVm`; spelling the same semantics as an explicit non-null return followed by `return NULL` reproduces the target `JE null / ADD +0x340 / RET 4 / XOR EAX,EAX / RET 4` shape. Canonical unit `anm-manager-get-vm-position` covers all 27 bytes and its sole REL32 to `FindVm @ 0x004491C0`; two cold replays also keep all six pre-existing normal-COFF `AnmManager.obj` units exact. | Maintained natural source; pinned VC7.1 SP1 normal COFF; complete target extent and relocation replay |
| ANM-036 | observed / exact-leaf | `AnmInt3View::operator* @ 0x00441E50` is canonical exact across the complete 56-byte linked `/GL` PDB contribution and all three `__ftol2` REL32 fields. Target call topology proves that the old descriptive free `ScaleAnmInt3` helper is value-returning member behavior: the unique color-Hermite caller passes the integer triplet in EDI, hidden result storage in ESI, one float on the stack, and the callee returns with `RET 4`. Restoring the member operator, evaluating the final Hermite term before the initial term, and retaining X/Z/Y constructor argument slots makes VC7.1 naturally evaluate Y, Z, then X and store Z, X, then Y, reproducing every target instruction without ABI annotations, assembly, volatile forcing, or inert dependencies. | Complete target/candidate disassembly; two cold `ExecuteScript`-context links; PDB extent; exhaustive three-field replay; same-context regression gate |
| ANM-037 | observed / physical-case-order-recovered | In the current `ExecuteScript @ 0x0043EE30` selected `/GL` entry context, the target's 94-entry table resolves to 92 physical case groups. The earlier maintained shared `advanceInstruction` label caused the NOP/interrupt group to appear at physical index 77 rather than target index 21. Giving NOP/interrupt its own ordinary advance body and writing the common switch-exit advance after the switch yields **92/92 groups in target physical order**, with NOP/interrupt at index 21. The candidate pre-table span becomes 9,408 bytes versus the target 9,588 (`-180`); size is worse than the previous `-116`, and exactness remains unknown. The 9,784-byte PDB contribution includes the separate 376-byte absolute table, whereas the target's 9,587-byte ledger extent ends before its table. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-038 | observed / child-case-control-flow | Target `ExecuteScript` child opcodes 88/90 share the tail placed after opcode 90, and 91/92 share the tail placed after opcode 92. The maintained source now uses explicit common tails and the candidate's first two child-case gaps become 44 and 47 bytes, equal to target, while all 92 physical case groups retain target order. The selected `/GL` contribution remains 9,784 bytes including the separate 376-byte table; the pre-table span remains 9,408 versus target 9,588 (`-180`). No exact claim follows. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-039 | observed / position-construction-shape | In target `ExecuteScript`, opcode 48 evaluates position operands Z, Y, X before copying the completed three-word vector to primary or alternate position. Direct X/Y/Z field assignments in the maintained source emitted the opposite evaluation order and split the position branch differently. Constructing `AnmFloat3View(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2))` in each branch reproduces the target's local right-to-left evaluation and aggregate-copy instruction pattern. The selected candidate's pre-table code span becomes 9,584 bytes versus target 9,588 (`-4`, formerly `-180`), while its PDB contribution is 9,960 bytes including a separate 376-byte table. The constructor also raises the candidate stack frame from `0xD4` to the target's `0xFC`. The saved game-speed local still lands at candidate `[esp+0x60]` versus target `[esp+0x98]`, and the alternate-position branch is placed before the interrupt-label block instead of after it. Thus the owner is non-exact despite matching frame size and close total code span. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-041 | observed / executor-tail-placement-and-child-abi | In target `ExecuteScript`, opcode 39's 105-byte float-jump body flows to the END/DELETE return at `0x00440DFC`; the script-stop update block begins after that return at `0x00440E2B`. The candidate float-jump instructions have the same local structure, but its stop block begins at `0x00407EA6` before the END return at `0x00407EFD`, making the physical interval 192 bytes. The candidate remains a 9,960-byte contribution with a 9,584-byte pre-table span versus target 9,588, and all 92 physical groups retain target order. Moving the saved-speed declaration to the top compiles to the exact same 9,960 candidate bytes; inverting the POSITION condition changes only three bytes, including a branch sense that then disagrees with target, so both source variants were reverted. Nearby private ABIs remain open: `FindVm` is 65 versus target 73 bytes with EAX+EDX rather than EDX+stack, `GetVm` is 19 versus 21, and `CreateVmVariant3` is 74 versus 73 with EBX rather than EDI hidden return storage. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-045 | observed / exact-render-layer-creators | The four render-layer VM creator frontends are now canonical exact: `AnmLoadedView::CreateVmVariant0 @ 0x00448D00`, Variant2 `@ 0x00448E30`, Variant1 `@ 0x00448F60`, and Variant3 `@ 0x00449090` are each 73-byte owners using the target-private EDI hidden-return storage, three stack arguments, and `RET 0x0C`. A later artificial `TH10_ANM_NOINLINE` on Variant0 was not target evidence: removing it while keeping Variant0 in the main ANM TU made all four siblings 74-byte mismatches. Keeping the natural member declaration/call spelling but moving only the Variant0 definition to the maintained `src/AnmVmCreate.cpp` TU restores all four siblings to the identical target template. Canonical linked-PE units replay the complete four-function family twice from cold builds: 4/4 units, 292/292 bytes, with five exhaustive linkage fields per function. Variant0 is PDB-owned by the support TU, so exact-unit tooling now supports an optional `pdb_source` that selects an already-declared primary/support source without changing link order; all pre-existing units retain the old primary-source default. The maintained TU name is descriptive and is not a claim about the original source filename. This exact helper-family result does not make `ExecuteScript` exact: the member-split focused diagnostic is 10,040 bytes with a 9,664-byte pre-table span versus target 9,588 and still keeps all 92 physical groups in target order. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-046 | observed / exact / attribution-corrected | `0x00408750` and `0x00441EF0` are separate 30-byte polar-to-Cartesian helpers maintained in `src/AnmManager.cpp`; each uses one x87 FSINCOS to write cosine times magnitude to X and sine times magnitude to Y. Canonical linked-PE replay reproduces both complete relocation-free extents. Fresh TH10 re-audit corrects the old attribution of caller `0x00406D90`: that owner is the bullet transform-program interpreter, not an ANM VM opcode handler; `0x00408750` is used by its vector-acceleration transform state. The maintained view names and TU placement remain descriptive only. | Hash-attested target extents and direct callers `0x00406D90` / `0x0043EE30`; fresh 0x00406D90 target decompilation; canonical `anm-opcode-vector` and `anm-script-vector` cold replays; pinned VC7.1 SP1 build6030 |
| ANM-047 | observed / exact | `AnmRenderManagerView::FindVm @ 0x004491C0` rejects ID zero, walks the primary then secondary manager list, and returns the matching VM or NULL. Its reviewed extent is 73 bytes through `0x00449208`, followed by CC padding. Ghidra reports 43 direct callers, including `MarkVmForDeletion @ 0x004492A0`. Selecting that real caller as the LTCG entry reproduces the full 73-byte `source.ltcg.obj` PDB contribution raw-equal with no link fields in two independent cold images. The maintained source is in `src/AnmManager.cpp`; exactness does not establish the original production TU. A same-size normal `/O2` COFF body differed at seven bytes and is not accepted. | Hash-attested target extent and callers; canonical `anm-manager-find-vm` linked-PE unit; two independent cold VC7.1 SP1 build6030 replays |
| ANM-048 | observed / exact | TH10 target functions `AnmVmIdView::GetVm @ 0x00449450`, `SetFlag2 @ 0x00449590`, `ClearFlag2 @ 0x004495E0`, and `Release @ 0x00449630` have complete physical extents of 21, 67, 67, and 21 bytes. Direct target calls and fields establish the manager-global lookup, root-child flag propagation, and delete-then-clear behavior. Two cold canonical linked-PE builds per caller graph reproduce every full PDB contribution and both declared linkage fields. Maintained definitions are grouped by the TH10 `AnmVmIdView` class in `src/AnmVmId.cpp`; this is not an original-production-TU claim. For source organization only, TH095 has an analogous `AnmVmId.cpp`, while TH08 keeps its older grouping in `AnmManager.cpp`; those remain hypothesis references. | Hash-attested TH10 target disassembly and xrefs; canonical units `anm-vm-id-{get-vm,set-flag2,clear-flag2,release}`; pinned VC7.1 SP1 build6030 cold replays |
| ANM-049 | observed / exact | `AnmVmIdView::SetInterrupt @ 0x00449470` resolves the stored ID through the manager, writes the pending interrupt at VM `+0x304`, and propagates it through the following child layer nodes only for a root VM (`+0x18 == 0`). Its 65-byte physical span includes seven alignment bytes. Two cold canonical linked-PE builds reproduce all bytes and the manager DIR32 / `FindVm` REL32 fields. The maintained method is in `src/AnmVmId.cpp`; FrontEnd and GUI interrupt helpers route to it. This source placement and name do not establish the original production TU. | Hash-attested TH10 decompile, disassembly, and callers; canonical unit `anm-vm-id-set-interrupt`; two pinned VC7.1 SP1 build6030 cold replays |
| ANM-050 | observed / exact | Hash-attested target disassembly at `0x00447280-0x004472D0` shows the preload stop check followed by `JS` and loads the Sleep IAT slot `0x004660AC` into EDI for the wait loop. The natural `/GL` candidate in `src/AnmManager.cpp`, entered through same-TU `AsciiManagerView::Initialize @ 0x00401110`, first emitted 80 bytes with 38/69 comparable bytes. Declaring `Sleep` as `dllimport` produced 81 bytes with 68/69; changing the stop condition to test the signed byte's `0x80` bit produced the target branch and full 81/81 comparable-byte equality. Rewriting the `do/while` as an equivalent `while` left codegen unchanged. Two independent canonical linked-PE cold replays cover the complete PDB contribution and all three linked fields: `ReadAnmEntries`, the Sleep IAT, and the stop flag. Ten other target callers remain source-unmapped; exactness is scoped to the declared compiler entry and does not establish original source ownership. | Hash-attested TH10 disassembly; two canonical `anm-manager-preload-anm` linked-PE replays; pinned VC7.1 SP1 build6030; `src/AnmManager.hpp`, `src/AnmManager.cpp`, `config/match-units.toml` |
| ANM-051 | observed / exact | Target 0x004493C0-0x004493DA is a 27-byte getter: look up a VM by ID, return its positionOffset address when found, or return null. Canonical normal-COFF unit anm-manager-get-vm-position reproduces the complete extent and sole FindVm REL32 with zero differences in two cold builds. Ghidra does not report a containing function at this boundary; accepted match evidence remains valid. | Hash-attested target extent and maintained source; config/matches.csv and config/match-units.toml; two pinned VC7.1 SP1 build6030 cold replays |
| ANM-052 | observed / near-codegen | Target 0x0043E8B0 has one caller, 0x004496D0, and one call site at 0x004496EC. Selecting the maintained AnmSetVmScriptIndexAndExecute wrapper as the explicit /GL entry emits a natural 287-byte candidate with 249/271 comparable bytes and four normalized linkage fields. Target code assigns the script index to EDI and the script pointer to ESI; the candidate swaps those registers. Exactness remains open; preserve the natural method and do not force register coloring. | Hash-attested target extent, disassembly and xrefs; pinned VC7.1 /GL linked-image diagnostic entered through the direct caller; src/AnmManager.cpp |
| ANM-056 | observed / canonical-linked-exact | `AnmVmIdView::SetSprite @ 0x00449670` and maintained descriptive `SetSpriteWithAnm @ 0x004496A0` are two TH10 sprite-wrapper bodies immediately after the existing exact id-wrapper family. `0x00449670` is 36 bytes: it resolves the stored id, then uses the live VM anmFile at +0x308 to call `AnmLoadedView::SetSprite`. `0x004496A0` is 34 bytes: it resolves the id, then uses an explicitly supplied `AnmLoadedView*`. Factory-native xrefs find 20 and 32 direct call sites respectively; `GuiMessageVmView::Run` owns four calls to the first and all 32 calls to the second. Refactoring the maintained GUI portrait paths to these natural wrapper methods lets the real GUI interpreter /GL graph reproduce both complete PDB contributions and all six manager/FindVm/SetSprite linkage fields zero-difference in one artifact. Maintained placement in `src/AnmVmId.cpp` follows the target class family and adjacent exact addresses; it does not prove the original production TU. | Hash-attested target bodies and xrefs; maintained AnmVmId/Gui caller source; canonical `anm-vm-id-set-sprite-own` and `anm-vm-id-set-sprite-explicit` replays; pinned VC7.1 SP1 build6030 |
| ANM-057 | observed / canonical-linked-exact | `AnmSetVmScriptIndexAndExecute @ 0x004496D0` is a 36-byte VM-id script restart helper. The three target calls at 0x0040E089/0x0040E0D3/0x0040E374 all lie in `EnemyRuntimeUpdate` and pass `&enemy->managedVmIds[0]` plus the selected primary/alternate/directional script index. Target code receives the id pointer in EAX, loads `g_AnmRenderManagerView`, calls `FindVm`, then obtains `vm->anmFile` at +0x308 and calls `SetAndExecuteScriptIdx`. Normal source was 44 bytes because implicit conversion from raw unsigned id to `AnmVmIdView` constructed a temporary. Treating the four-byte id storage as the established wrapper type removes that temporary; declaring the natural helper `__stdcall` lets VC7.1 /GL promote the first argument to EAX while retaining one stack argument and `RET 4`, producing the exact target 36-byte contribution and all three linkage fields. | Hash-attested TH10 helper/call sites; maintained EnemyRuntimeUpdate and ANM source; canonical `anm-set-vm-script-index-and-execute` replay; pinned VC7.1 SP1 build6030 |
| ANM-058 | observed / canonical-linked-exact | `AnmVmTimerView::SetCurrent @ 0x00405410` is a 57-byte shared timer seam with 15 direct callers spanning the bullet transform interpreter, Enemy dispatcher, Player, ANM executor and other owners. Target ABI is private EAX=timer plus one stack value and RET 4. A separately authored wrapper reproduces the semantics but keeps ordinary stack arguments; in contrast the existing inline `AnmVmTimerView::SetCurrent` and equivalent Player timer member naturally materialize the target-private ABI under VC7.1 LTCG. Fresh `AnmRenderManagerView::ExecuteScript` /GL /GS probing gives 53/53 normalized comparable bytes. Two independent cold canonical linked-PE builds replay the complete 57-byte PDB contribution exactly with the sole `g_AnmGameSpeed` DIR32 linkage normalized. This exact physical helper does not prove which equivalent timer class/TU supplied the retail contribution. | Hash-attested target disassembly and 15-caller xrefs; fresh AnmManager/Enemy caller probes; canonical `anm-vm-timer-set-current` two-cold-build replay; pinned VC7.1 SP1 build6030 |
| ANM-059 | historical / primary-alpha-private-ABI-gap | In the real `AnmRenderManagerView::ExecuteScript` direct-entry `/GL /GS` graph, `StartPrimaryAlphaInterpolation @ 0x00442300` emits 128 bytes against the target's 130, with 14/126 normalized comparable bytes. TH10 has exactly two direct calls, at 0x0043F43F and 0x0043FAF7. Both pass a zero-extended `mode` byte on the stack; the target helper takes EAX=VM, ECX=duration, three stack arguments and returns with `RET 12`. The candidate passes `mode` in EDX and returns with `RET 8`; changing only the member declaration to `__stdcall` or `__fastcall` produced identical candidate code and was reverted. The initial whole-integer attempt ended in two Wine failures; that inconclusive result was superseded by the successful 2026-10-18 archived 130-byte candidate and its nonexact full comparison in `probes/anm-primary-alpha/comparison/ACTUAL_COMPARISON.md`. Fresh 2026-10-19 all-integer and mode-only integer controls independently reproduce the same nonexact helper; see ANM-110 for the tangent-first closure with unchanged byte formals. The historical 2026-09-26 single-source executor probe emitted 9,752 bytes versus target executable owner 9,587, with 388/8,392 normalized matches, all 92 physical selector groups in target order, and a 9,376/9,588 pre-table span; That historical executor probe was superseded; use RE_HANDOFF.md for a dated current-source checkpoint. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-068 | observed / target-frame-and-pretable-checkpoint | Fresh TH10-local SCALE_TIME lifetime work identifies the standalone source scalar `finalScaleY` as an extra whole-function local web rather than required target structure. Keeping the target-observed Y-before-X evaluation order but writing Y directly into `finalScale.y` before evaluating X removes that redundant web. In the selected direct-entry `/GL /GS` graph with `RandomMath.cpp`, VC7.1 changes the executor frame from candidate `0x100` to the target `0xFC`, the pre-table span becomes exactly 9,588/9,588 bytes, all 92 physical selector groups remain in target order, and all 85 `OR EDI,-1` loop-tail restores remain present. The PDB contribution becomes 9,964 bytes (9,588-byte pre-table plus the 376-byte absolute jump table) and normalized comparable agreement rises from an earlier candidate to 910/8,599. A complete source-scope cold replay remains exact for all 92 configured `src/AnmManager.cpp` units across 17 artifacts, 17,331 matched bytes. This does not make ExecuteScript exact: savedGameSpeed and several case temporaries still occupy different stack-coloring lanes, and individual ROTATION/SCALE interval boundaries move under the corrected frame. The retained source is selected because the frame, complete pre-table extent, dispatch order and TH10-observed Y-before-X semantics all agree with target evidence; remaining work is allocator coloring, not restoring the removed scalar. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-070 | observed / GetVm-private-ABI-graph-correction | Rechecking a previous private-ABI graph against the already exact TH10 AnmVmIdView::GetVm target at 0x00449450 exposes a diagnostic-graph omission. GetVm has a target-private ESI receiver, but the earlier 1,723/8,599 ExecuteScript probe did not link AnmVmId.cpp, so opcode 88/91 child-creator callers emitted MOV ECX,EDI before the GetVm call instead of the target MOV ESI,EDI. Adding AnmVmId.cpp as /GL support, or equivalently exposing only the GetVm body to the ExecuteScript WPO context, restores MOV ESI,EDI at those call sites while preserving the 9,964-byte contribution, target 0xFC frame, exact 9,588/9,588 pre-table span and 92/92 physical selector order. The ABI-correct graph scores 1,676/8,599 normalized comparable bytes, 47 lower than the incomplete 1,723 diagnostic. Therefore 1,723 is not the selected allocator frontier despite its higher score. The source checkpoint from that experiment was historical; future ExecuteScript diagnosis must include RandomMath.cpp, AnmVmCreate.cpp and AnmVmId.cpp support and must prefer target-backed private ABI over transient aggregate score. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-071 | observed / canonical-linked-exact / caller-context | `AnmLoadedView::CreateVmAtScreenVariant0 @ 0x00448D50` is now exact in a target-bound `/GL` graph rooted at `AnmVmView::Initialize`. The complete 95-byte PDB contribution cold-replays twice with all five target linkages (manager global twice, `AllocateVm`, `InitializeAndExecuteScriptIndex`, and `AddVmVariant0`) replayed zero-difference. The entry context is material: direct-root linking changes the private register allocation, while the existing `AnmVmView::Initialize` context produces the target EDI/EBP/EBX position/return ABI. The four source-unmapped direct callers remain open and receive no credit from this unit. | Direct target/IDA body and caller ABI; canonical `anm-create-vm-screen-variant0` linked-PE replay; pinned VC7.1 SP1 build6030 |
| ANM-072 | observed / canonical-linked-exact / caller-context | The same target-bound `AnmVmView::Initialize` `/GL` context closes all four screen-position VM creators: `CreateVmAtScreenVariant0 @ 0x00448D50`, `Variant1 @ 0x00448FB0`, `Variant2 @ 0x00448E80` and `Variant3 @ 0x004490E0`. Each complete 95-byte PDB contribution cold-replays twice with the same five linkage offsets; only the final `AddVmVariant{0,1,2,3}` target differs. This adds 380 exact bytes without promoting any of the four source-unmapped caller bodies. | Direct target/IDA bodies and target caller ABIs; canonical `anm-create-vm-screen-variant{0,1,2,3}` linked-PE replays; pinned VC7.1 SP1 build6030 |
| ANM-074 | observed / canonical-linked-exact / private-ABI | `AnmRenderManagerView::SetVmWorldPosition @ 0x00449350` is a 111-byte helper that resolves a VM, adds `(224.0f, 16.0f, 0.0f)` to the position, and propagates the result through a root child chain. The target uses the same private ABI family as `SetVmPosition`: EDX manager, ESI position, one stack `AnmVmIdView`, and `RET 4`. Changing the maintained declaration from raw `int` to the established four-byte `AnmVmIdView` value plus `__stdcall`, and expressing the child destination as a natural `AnmFloat3View*` local, produces the target EAX/ECX allocation without ABI hacks. Canonical linked-PE replay `anm-manager-set-vm-world-position` under the same-TU `ServicePreloadedAnms` /GL context matches all 111 bytes; FindVm and the four floating-constant DIR32 fields are explicitly resolved. | Direct IDA disassembly/callers; fresh `anm-set-world-position-child-position-20260929.json`; canonical replay; pinned VC7.1 SP1 build6030 |
| ANM-075 | observed / canonical-linked-exact / private-ABI | `AnmRenderManagerView::MarkLoadedVmsForDeletion @ 0x004493E0` is a 92-byte helper with target-private EAX=manager and EDX=loaded-ANM inputs. It scans primary and secondary VM lists, compares each owner at +0x308, and ORs deletion flag `0x04000000` at +0x35C. In the real `AnmLoadedView::Release` /GL context, precomputing each node's next pointer before reading its owner is a natural source form that reproduces the target's EAX/ECX loop ownership, explicit second-loop jump, and alignment gap. Canonical replay `anm-manager-mark-loaded-vms-for-deletion` matches all 92 relocation-free bytes. | Direct IDA disassembly/callers; fresh `anm-mark-loaded-nextnode-context-20260929.json`; canonical replay; pinned VC7.1 SP1 build6030 |
| ANM-077 | historical / alpha-stack-ABI-frontier | `AnmVmView::StartPrimaryAlphaInterpolation @ 0x00442300` is a 130-byte body whose target call sites at `0x0043F43F` and `0x0043FAF7` pass EAX=VM, ECX=duration, then all three byte values on the stack; the callee returns with `RET 0xC`. The maintained natural member source instead lets the WPO member seam carry the second byte in EDX, producing a near-sized body with the wrong argument placement. A target-observed static-member rewrite and a `__stdcall` variant were both tested as natural C++ alternatives; they changed to ordinary all-stack layouts (136/138 bytes) and did not reproduce the EAX/ECX-plus-stack seam. Those edits were reverted. The source-present/nonexact state described here is historical; the unchanged byte-parameter, tangent-first source closes this owner in ANM-110 without static-member or calling-convention annotations. | Direct IDA Pro MCP target/caller disassembly; bounded pinned VC7.1 LTCG probes; canonical source restored with no match row |
| ANM-078 | observed / canonical-coff-exact / private-ABI | `AnmVmView` reset at `0x00401DE0` is a 273-byte reusable-VM reset whose target callers leave an unrelated context in ECX and pass the VM in EDX. A static `__fastcall` overload with an unused context parameter, aggregate position preservation, explicit word-sized flag initialization, and natural manager/layer node pointer locals reproduces the complete target extent in pinned VC7.1 normal COFF: 273/273 bytes, no relocations. The ordinary source-facing `Initialize()` overload remains separate; this exact row records the target's private two-register reset view and does not claim whole-product closure. | Direct IDA Pro MCP reset disassembly/bytes and caller ABI; canonical `anm-vm-initialize-private` replay; pinned VC7.1 SP1 build6030 |
| ANM-085 | observed / canonical-linked-exact / actual-member-reset | Supersedes ANM-078's separate source-facing reset and unused-context static model. Target `0x00401DE0` receives the VM in EDX, overwrites ECX before using it, and writes WORD `[VM+0x35C]=7`; an extra formal context argument is unsupported. Moving the already exact reset body into the ordinary `AnmVmView::Initialize()` member, removing the duplicate static overload, and retaining `/GL` naturally reproduces all 273 target bytes. The existing canonical unit now uses the actual member in the ExecuteScript `/GL /GS` graph, with complete PDB-owned extent and zero link fields; two independent cold builds pass raw-equal. The final six-TU Enemy graph independently passes the same complete raw-equal member and six existing seams, 1,250/1,250 bytes. Reset, SetupVertexBuffer, the only canonical exact direct reset caller RemoveVm, and four base creators pass focused canonical replay: seven units/1,305 bytes. Nine target call sites belong to seven owners; their other bodies receive no credit. Six member-rooted units and fourteen dispatcher units also cold-replay exact, 1,310 bytes; combined focused protection covers 27 distinct canonical units/2,615 bytes. Canonical function/byte totals are unchanged because this replaces an existing proof. Original class/TU/compiler partition, native product closure and runtime remain unknown. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-087 | observed / real-caller-closure-controls / non-exact | Full direct-Ghidra InitializeVm xrefs return 46 calls below the 200-row cap; the earlier 40-row query was truncated. Adding unchanged Player.cpp as a seventh real /GL support leaves the spell/helper frontier unchanged. Binding Player's actual initializer calls, then its allocator/registration calls, naturally restores InitializeVm's target incoming ECX loaded plus two stack arguments / RET 8 in the ordinary Enemy-rooted graph; the helper changes from 201 to 207 bytes, still short of the 217-byte target. No artificial entry root, context parameter or /GL removal is used. Other helper sizes remain 273/200/197/194 for reset/InitializeForLoadedScript/InitializeAndExecuteScriptIndex/binder. A height-before-width control changes the two initializer store sequences but leaves helper sizes, ABIs and spell/parent scores unchanged; it is restored. Adjacent TH095's plain float-vector source is hypothesis only; its compiler-storage padding is not adopted. Copy scheduling, private outgoing ABIs and the remaining production caller graph are unknown; incoming standard ABI does not prove normal-COFF ownership. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-089 | observed / complete-dispatch-owner-boundary / non-exact | Supersedes ANM-013/037 code-only extent: ExecuteScript owns0x0043EE30-0x0044151B,9964 bytes, comprising9587 code bytes through RET4 at0x4413A0, one 0x90 NOP and94 four-byte dispatch slots at0x4413A4-0x44151B. Signed opcode+1 is unsigned-bounded to0x5D; all94 slots target92 decoded in-body groups. Full code consumes2754 instructions; all372 direct jumps remain in-body, four RET4 exits and one indexed dispatch are present. Fresh Factory-native Ghidra queries cover every slot both directions: one incoming dispatcher reference and94 matching outgoing target references, without truncation. Raw whole-PE pointer scan finds only the dispatch field; no separate owner is evidenced. FourCC bytes precede unclassified code at0x441520, which is excluded; next tracked function441600. Fresh architecture/inventory now assigns the table to its source; focused structural ledger check passes, while unrelated historical0x401100 descriptive-metadata mismatch keeps the global check failing. Independent metric review confirms TH10 full-owned-.text convention and explicit ENEMY-076/PLAYER-029/ITEM-007 precedents;33 compare_size overrides are32 excluded and1 indeterminate, zero authored. Authored denominator265810 becomes266187 (+377), exact authored38007 and canonical103720 remain unchanged;500 indeterminate origins still make the denominator provisional. Fresh selected four-TU /GL /GS cold graph emits9964/9964,1647/8600 normalized comparable bytes and remains non-exact. No source change, metric-formula change, other boundary change or exact credit occurs. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-090 | observed / interpolation-dataflow-fidelity / non-exact | The complete ExecuteScript owner remains non-exact. Target POSITION_TIME reads both tangent vectors from writable g_AnmZeroVector491C14, uses a dword direct mode, snapshots the initial position, then evaluates final operands 4 -> 3 -> 2 into a real 12-byte vector. The old literal-zero tangents, byte mode and immediate x/y/z writes are corrected; ROTATION_TIME direct mode is also dword. Candidate still coalesces the two tangent copies into three global loads versus six target loads. SCALE_TIME constructs a real eight-byte Float2 operand before duration, preserving 3 -> 2 evaluation, target stack homes +0x104/+0x108 and the unchanged byte-mode helper ABI. A source-local derived Float2 constructor with no fields and a sizeof==8 assertion is a natural factoring hypothesis, not original-type recovery. It restores the real frame to 0xFC without inert storage; frame equality is not exactness. Two current four-TU /GL /GS cold builds reproduce the same 9,948-byte candidate against the full 9,964-byte target; all 92 groups stay ordered and 84 case spans have target size. Case-aligned 6150/8042 is diagnostic only. The globals-only control shrank to 9868/frame E4; final Float3 construction restores 9948/frame F8; the selected Float2 constructor gives 9948/frame FC. Independently reviewed source and compiled dataflow pass; all 136 existing canonical units affected through primary or support TUs cold-replay exact across 36 artifacts, 23,456 bytes. Groups 18/19 transport timeout was reconciled from completed atomic reports with no active compiler processes or duplicate builds. These are regression checks, not new Factory acceptance. Authored exact remains 38,007/266,187 and origins remain provisional. Interpolation reset-helper receiver/callee differences, flag lowering, shared-tail placement and private helper ABIs remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-091 | observed / typed-reset-and-copy-fidelity / non-exact | ExecuteScript now uses an out-of-line Float3 interpolation ResetTimer method expressing timer.SetCurrent(0), matching the target outer receiver and no-argument EAX/RET seam at 0x4050D0. The complete 50-byte candidate differs only in the independently resolved game-speed address field. Its name and original class/TU remain inferred; the separately canonical decomphelp leaf and indeterminate origin are unchanged, with no duplicate credit. Four natural 12-byte memcpy operations reproduce both x/y/z global reads for each tangent pair instead of coalescing them. The packed flip expressions toggle only bits 9/10 and set updateScale bit 3, preserving all other flags. No fields, layout, existing signatures, padding or calling-convention annotations change. POSITION_TIME reaches its full 344-byte diagnostic shape; ROTATION_TIME reaches 290 bytes; FLIP_X still uses a one-byte-longer EDX XOR rather than target EAX. Two cold four-TU /GL /GS builds produce identical complete 9,948-byte candidates versus the 9,964-byte target, frame 0xFC, all 92 groups ordered and 87 target-sized spans. Full linear 1302/8564 and case-aligned 6470/8011 scores are diagnostics, not exactness or partial credit. Independent source/dataflow/scope review and all 136 primary/support-affected canonical units pass, 23,456 bytes across 36 artifacts, with no retry or error in this regression round. Authored exact remains 38,007/266,187, provisional with 500 indeterminate origins. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-094 | observed / canonical-linked-exact | ApplyTextureAlphaBleed at 0x004465B0 reproduces its complete existing 1463-byte owned extent: 1417 executable bytes through RET, three compiler alignment bytes, four absolute case pointers and 27 selector bytes. Raw target review corrects vertical neighbors to typed pixel minus/plus (signed pitch divided by four or two), with truncation before subtraction; row bases still recompute bits + pitch*y. Three chained channel-zero initializations, pixel-before-index loop advancement and six unsigned-char channel casts match target dataflow. Both 16-bit arms preserve their alpha and perform no text-helper channel halving. The fresh seven-TU baseline is 1343 bytes; typed pitch gives 1423; chained/cursor order alone over that leaves 1423; byte casts without those orders give 1451; the combined natural source gives 1463 with every ordinary byte equal. Independent PE/map/PDB review binds exactly six complete local DIR32 fields at offsets 0x48,0x52,0x58C,0x590,0x594,0x598. Rebinding only those fields reproduces all 1463 bytes including every table/alignment byte. Two distinct canonical cold links pass, then all 136 prior affected units pass 23456 bytes across 36 artifacts with stable input hashes and no retries/timeouts. This adds 1463 local canonical authored bytes without changing boundaries, origins or denominator; Factory acceptance remains separate. ExecuteScript stays 9948/9964 and the radial initializer stays 803/803 with eight ordinary bytes open in both fresh before/after graphs. The old earlier ANM experiment large candidate is preserved; a texture-preserving rebased candidate is saved but unbuilt, and old trial-v3 guards must not be bypassed. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-096 | observed / canonical-linked-exact | ReadAnmEntries at 0x004470C0-0x00447270 and LoadExternalTextureData at 0x004472E0-0x004473B5 close their complete 433/214-byte owners in one real PreloadAnm /GL /GS graph with FileSystem, PbgArchive and GameErrorContext support. Two independent canonical cold links pass all 647 bytes and all 14+12 independently reviewed fields, including five distinct literal payloads and the real error-context global at 0x474F70. Ordinary member source naturally gives stack manager/index/path RET12 and helper ECX index/EDX loaded/stack raw-entry RET4. The selected real 260-byte path arrays, existing counter lifetime/update placement, and genuine zeroing constructor preserve the target frame/CFG, including internal six-byte alignment. Index rejection remains signed index>=33 only; slot publication precedes raw-file failure, allocation and inventory quirks stay intact, and helper embedded-data/@/version behavior is unchanged. Four source error branches reach real Fatal through three physical calls. A shared full GameErrorContext declaration also replaces Enemy's thin duplicate while preserving Log/Fatal/probe bodies and the existing Log call. All 231 prior exact units pass 33550/33550 bytes across 81 cold artifacts, including OpenFile and both old error-context contexts. Local canonical authored exact is 43697/266187 (16.42%); both new claims are Factory PASS and ACCEPTED on their first attempts at 19875ae (read receipt b122790df83fa3d70e9223eb0d655d07fca8fef5a4e82aa0cf98f4e0ec1efd5f; external receipt ccc637a19e448a022392543cd2d1a338aad6d2ff24af2459391f0654fa9896dc). Original constructor ownership, capacity macro spelling, production TU/profile, product and runtime remain unknown. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-098 | observed / canonical-linked-exact | LoadTextureData at 0x00447470-0x004476F9 and LoadSurface at 0x00447C80-0x00447EBF close their complete 650/576-byte owners. Two distinct canonical cold PE/map/PDB triples reproduce all 1,226 bytes, including eleven internal texture-owner alignment bytes and both complete return instructions, after sixteen plus twenty-two independently established semantic bindings. All 1074 ordinary bytes remain unchanged by rebase. The shared four-TU /GL /GS recipe uses AnmManager with FileSystem, PbgArchive and GameErrorContext support and the real maintained Postload entry. Postload genuinely calls LoadTextureData at 0x4473F8; for LoadSurface it is only a declared whole-TU compiler context, with no direct incoming edge or stored absolute reference found and indirect/runtime reachability still unknown. All 139 prior affected units pass 25,566 bytes across 38 cold artifacts, including all 33 support consumers; every old recipe dictionary remains unchanged. Local canonical authored exact becomes 44,923/266,187 (16.88%); both claims are Factory PASS and ACCEPTED on their first attempts at b2dbb473187b564e4238227e56010487f0eff265 (texture receipt b6eec439f684eae9b2d1a4031e07a40421cf8bf40da141c22d2873106d9ccc46; surface receipt 2b1dc81773dc70dff55e5620f94a7e2231dd39240379fa892fc61fdbac2b586b). All 500 indeterminate origins remain, with no boundary, origin, mapping or denominator changes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-100 | observed / ABI correction / bounded controls restored | LoadTextureRegion at 0x00446D70 has complete 320-byte ownership and incoming EAX format, ECX top, EDI data, plus four stack words through RET16. Entry-stack +4 is unread, +8 is the texture entry, +0x0C is size and +0x10 is hasData; the old incoming ESI-entry/EBP-size wording was wrong. All 38 current frozen linked contexts emit the same 321-byte body shape after each owner's own eight semantic fields are enumerated; no caller-context sweep is indicated. Three rectangles plus one description at function scope produce frame 0x54. Moving only branch-exclusive aggregates into their branches gives 315 bytes and target frame 0x34. Delaying the real surface=NULL assignment until just before GetSurfaceLevel recovers the full target prologue/private argument registers and gives 319 bytes. Moving raw-entry/header pointer declarations into the branch and expressing the byte address as &data[offset] are each fully byte-neutral against 319. The remaining 58/59-byte embedded-header segment starts with MOV+ADD versus target MOV+LEA and differs in ECX/EDX temporary choices/scheduling. All controls restore the complete accepted source; no partial or whole-owner exact credit is granted. The target's missing base relocations do not hide five DIR32 globals/table bases; those plus three REL32 calls are independently decoded and bound. Source-only protection scope is 141 existing units / 26,792 bytes / 39 artifacts, but no new regression or source retention is claimed for these restored controls. Original source spelling/ownership and indirect reachability remain unknown. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-101 | observed / canonical-linked-exact / focused-validation | Initialize0x401110-0x401259 owns330 bytes. Real PreloadAnm, Chain allocation/insertion, SetSprite and Log calls replace thin aliases; the three callbacks and resource publication order remain. Per-VM file/address lifetimes naturally recover EAX input and EBX preservation; explicit post-reset anmFile stores, sprite indices0/0x62 and the23-byte CP932 error literal remain. Shared Chain source uses the observed operator new, signed priorities, standard insertion callee cleanup, direct list-field reads and post-Leave depth decrement. Both canonical /GL cold triples reproduce all330 bytes and28 independently bound fields. Five modified Chain helpers remain nonexact; RegisterCalc/Draw retain their prior unresolved cleanup contract and are outside Initialize's call path. The wider dllimport-header variant also matches Create69, but selected unchanged-header context gives Create70; no Create/helper credit. Per explicit2026-10-05 operator instruction only modified functions require validation:87 already completed prior units/17478bytes in12 artifacts are partial history; remaining28 planned artifact groups were cancelled, not passed. Production TU/profile, product/runtime and500 indeterminate origins remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-102 | observed / canonical-linked-exact / focused-validation | DrawMode6 at0x443FB0-0x444232 owns643 bytes;13 following CC bytes are padding. The unchanged real AnmPairOwnerDrawCallback to Draw to DrawMode6 /GL graph initially emitted645 bytes with frame0x34. All166 instructions already aligned after resolving30 fields and42 stack operands/four frame immediates; two color stores accounted for the extra encoded bytes. Flattening the sole-use aggregate into the same cameraDelta, color, distanceRange and distance locals recovers frame0x20 and all643 bytes. Arithmetic, branches, private inputs, seven ftol2 calls and int-before-subtract channel semantics are unchanged. Two canonical cold triples independently replay643/643 with21 absolute and9 direct-call fields. Bindings use true semantic owner bases plus explicit addends despite overlapping diagnostic anchors; both1.0 float payloads match. Only this modified owner was replayed under the focused operator scope. Prior645-byte diagnostics remain history, superseded for this selected source by this full-owner proof. No boundary/origin/denominator or other-owner credit; product/runtime and original production TU remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ANM-104 | observed / canonical-linked-exact | `AnmRenderManagerView::SetVmPendingInterrupt @ 0x00449210` finds a nonzero VM through `FindVm`, writes the short pending interrupt at VM +0x304, and propagates it through the root's child layer chain. The target private ABI uses EDX for the manager, ESI for the interrupt, one stack `AnmVmIdView` value and `RET 4`; the maintained value-type parameter preserves that shape naturally. Canonical unit `anm-manager-set-vm-pending-interrupt` reproduces the complete 54-byte PDB extent and sole FindVm REL32 field zero-difference in the same-TU `ServicePreloadedAnms` /GL context. | Hash-attested TH10 disassembly and FindVm call; maintained `src/AnmManager.cpp`; canonical linked replay; pinned VC7.1 SP1 build6030 |
| ANM-105 | observed / canonical-linked-exact | `AnmRenderManagerView::SetVmPendingInterruptAndExecute @ 0x00449250` resolves a VM, writes its pending interrupt, executes it, then propagates the interrupt and execution to each child in the root layer chain. The target private ABI uses EDX for the manager, EDI for the interrupt, one stack `AnmVmIdView` value and `RET 4`; the maintained value-type parameter plus two natural `node->owner` expressions preserve the target's repeated owner loads and four-byte loop alignment. Canonical unit `anm-manager-set-vm-pending-interrupt-and-execute` reproduces the complete 76-byte PDB extent and FindVm plus both ExecuteScript REL32 fields zero-difference in the same-TU `ServicePreloadedAnms` /GL context. | Hash-attested TH10 disassembly and direct calls; maintained `src/AnmManager.cpp`; canonical linked replay; pinned VC7.1 SP1 build6030 |
| ANM-106 | observed / canonical-linked-exact | `AnmRenderManagerView::SetVmPosition @ 0x004492F0` resolves a VM, copies the three position floats into the VM's +0x340 position-offset field, and propagates that value through a root's child layer chain. The target private ABI uses EDX for the manager, ESI for the position input, one stack `AnmVmIdView` value and `RET 4`; the maintained value-type parameter preserves the stack-ID ABI while the natural vector copy preserves the ESI source. Canonical unit `anm-manager-set-vm-position` reproduces the complete 90-byte PDB extent and sole FindVm REL32 field zero-difference in the same-TU `ServicePreloadedAnms` /GL context. | Hash-attested TH10 disassembly and direct calls; maintained `src/AnmManager.cpp`; canonical linked replay; pinned VC7.1 SP1 build6030 |
| ANM-107 | observed / local-canonical-linked-exact | Release at 0x00447810 owns 242 bytes and retains one AnmTextureEntryView pointer per iteration across the COM Release and raw-buffer free. It reloads the table only at the next iteration; replacing repeated textures[i] member reads with that local preserves target aliasing and side-effect order. This restores the private EDI receiver and makes the unchanged ServicePreloadedAnms owner at 0x00447700 reproduce all 135 bytes with its ESI slot scan, stack manager/RET 4 and observed release-branch null-slot write. Both complete owners pass two cold canonical linked-PE replays with all eight Release and three service linkage fields independently target-bound. Normalization is complete; no padding, target bytes, stubs or policy edits were introduced. Local ledger gain is 377 authored bytes; Factory acceptance and whole-product closure remain separate. | Target disassembly and prior owner/runtime bindings; units anm-loaded-release and anm-resource-service-preloaded; focused canonical cold replays on 2026-10-14 |
| ANM-108 | historical / radial-initializer-context / superseded | InitializePulsingRadialTrail at 0x004452F0 owns 803 bytes. Its real ExecuteScript /GL /GS context with RandomMath, AnmVmCreate and AnmVmId visibility produces an 803-byte candidate with 623 of 631 comparable bytes equal and complete normalization, unlike the unrelated service-entry 571-byte diagnostic. The target and candidate retain three initial signed-RNG calls plus two inlined two-step RNG sequences per ring iteration, with matching seed and generation-count writes. Eight comparable differences remain: radius FMUL versus radial-velocity load scheduling, and the final translation Y operand order. Whole-vector operand reversal, a radius local before publication and early radial-velocity publication all regressed matching and were reverted. This was context evidence, not exact credit for that earlier source. The later signed-RNG float-local source closes the existing 803-byte canonical unit and was accepted at 504539f9 on 2026-10-18; see evidence/radial-rng-local/README.md. A strict same-image warm check of the retained ANM-110 second cold image again passes all803 bytes/all43 fields, with no duplicate credit. | Target 803-byte disassembly; real ExecuteScript caller and existing /GL /GS support graph; bounded probes on 2026-10-14; Git history |
| ANM-109 | observed / major-owner-binding-and-lifetime / non-exact | Fresh ExecuteScript baseline under its genuine /GL /GS graph with RandomMath, AnmVmCreate and AnmVmId was9900/9964 bytes, frame0xF8,79/92 target-sized groups and5292/8013 case-aligned comparable bytes; the older9964/84-group checkpoint is historical, not current. Target mode9 calls at0x00441210/0x00441298 bind the separate script polar owner0x00441EF0, not radial owner0x004458B0. Moving the existing AnmScriptVectorView declaration before ExecuteScript and changing only its two receiver views repairs those REL32 identities; helper bodies, arithmetic and three radial calls remain unchanged. This binding correction alone is byte-score neutral. F_SET_RANDOM opcode41 target selects its branch before operand decoding, then selects the concrete RNG receiver after GetFloatVar; the previous common RNG pointer introduced an unsupported lifetime across that call. Explicit branch-local decode/multiply reproduces the full130-byte group with98/98 comparable bytes, including getter side effects before RNG, extended x87 multiply, final float spill before destination resolution, and one selected RNG call. Two fresh cold owner checks now reproduce9948/9964 bytes,9572/9588 pre-table span, frame0xF8, all94 selectors/92 ordered groups and83 equal-sized groups. Full-owner comparison is1386 raw matches and1212/8564 comparable; case-aligned comparison6331/8000. Both normalizations complete, but nine unequal groups and other byte differences remain. No partial owner or leaf exact credit. Independent source/target/binding review passed. SCALE_TIME60 aggregate controls were then bounded and rejected: adding a constructor to the existing Float2 fails VC7 C2620 because the type is used in a vertex union; no union/layout change was made. A constructor-free by-value factory preserves Y-before-X and rounding but creates an extra aggregate copy, grows the frame to0x104 and owner to9980/9964, reduces same-sized groups to81 and aligned matching to6073/8008. Both changes were reverted. Keep the retained9948/83-group checkpoint and park this aggregate route rather than sweep spellings. | Fresh attested target; preserved hash-bound full-owner/table/case reports; independent review; two scoped cold diagnostics on2026-10-15 |
| ANM-110 | observed / local-canonical-linked-exact / source-order control | StartPrimaryAlphaInterpolation at 0x00442300 owns130 bytes. Two independent canonical cold four-TU ExecuteScript /GL /GS links pass the full PDB-owned extent and sole game-speed DIR32 at +0x5F, with Initialize273 and SetupVertexBuffer472/all22 fields as same-image guards. Only the two zero-tangent stores move before duration/endpoints; byte formals, callers, layout and profiles stay unchanged. LTCG naturally recovers target EAX VM/ECX duration/three stack bytes/RET12 even though emitted target stores duration/mode/initial before the zeros. Machine store order therefore does not identify unique C++ statement order or original declarations. All-integer and mode-only integer controls each emit130 bytes with93/126 comparable helper positions, the same helper hash but differing callers; both are reverted. ANM-059 early Wine failures were superseded by the successful nonexact2026-10-18 archived candidate. Both retained images give ExecuteScript9948/9964, frame0xF8/0xFC, all92 groups ordered,84 same-sized,1312/8564 whole-owner and6492/8000 case-aligned comparable positions. Opcodes55/58 now align, but eight other spans and the complete parent remain nonexact; no partial-case credit. | Local read-only hash-attested Ghidra; independent complete target decode and +0x5F field; canonical unit anm-start-primary-alpha-interpolation; docs/evidence/anm-primary-alpha-local/README.md |
| ANM-111 | observed / complete-world-creator-context-controls / non-exact | Four118-byte world creators and shared203-byte InitializeAndExecuteScriptIndex now have explicit noncanonical strict recipes: all32 fields are target bound, but each creator has13 ordinary scheduling differences and the helper one receiver byte at+27 (MOV ECX,EDI versus target EDX). Existing single-TU /GL screen-family entry Initialize gives target118/203 sizes, but Initialize itself is275/273 with unlike ECX interface. Real screen0 caller entry restores Initialize273 raw equality while helper/worlds coalesce to197/116. Four-TU /GL /GS and real EnemyKillAll contexts also emit197/116; preserving world0 outline shrinks caller404 to183/target182 without closing creators. Layer-first, flags-local, existing bitfield and ordinary three-real-parameter free-helper controls do not close the owner; all source/API/annotation changes are restored. Four existing screen95 owners and Setup472 strictly pass852 bytes/42 fields in the retained image. No original ownership/profile, runtime or partial exact credit follows. Reference volatile/asm/dead-initializer-slot shapes are not adopted. Reopen only with new TH10 alias/lifetime/caller evidence. | Local read-only hash-attested Ghidra; complete target field/constant review and PDB-owned cold contexts; docs/evidence/anm-world-creator-context/README.md |
| ANM-112 | observed / canonical-linked-exact / slot sampling | ReleaseAnm4477D0..447806 owns55 dense bytes. It tests negative index then unsigned33-slot bound, calls Release on the selected entry, reloads loadedAnms[index] for delete and clears that slot. Replacing the old cached-pointer delete restores48 to55 bytes; unsigned sizeof-array count changes the final JGE to target JNC. Two independent eight-TU /GL /GS colds freeze26 inputs and strictly pass55/all2 fields plus actual Release242/all8 fields in each image. Incoming target caller40C2F8 belongs to source-absent1494-byte40BD80; OPT:NOREF retention does not reproduce that source edge or original production partition. Host alias-replacement witness/old-cache mutant passes ASan/UBSan; target runtime is open. Larger dispatcher14524/14416 and ANM9948/9964 remain nonexact. | [Target review, full proofs and limits](evidence/anm-release-slot/README.md) |

## ECL VM

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ECLVM-001 | observed / corroborated | `EclVmContext::Run @ 0x0044E1A0` is the generic TH10 typed-stack ECL interpreter beneath the Enemy-specific `0x0040E770` high-opcode dispatcher. The target checks `float(instruction.time) <= currentTime`, gates on the context/instruction difficulty masks, dispatches 59 active opcodes in `0x00..0x57`, and forwards the remaining 29 selector values plus every opcode outside that range through host vtable slot zero. Its `0x1024`-byte context has time/instruction at `+0/+4`, a `0x1000`-byte stack at `+8`, stack top/frame base at `+0x1008/+0x100C`, thread id/host at `+0x1010/+0x1014`, thread control/difficulty at `+0x1018/+0x101C`, and flags at `+0x1020`. The maintained core covers call/return/jump, threads, frames, typed operand/local/host resolution, arithmetic/comparisons, logical/bitwise operations and trigonometric/vector instructions. Raw opcode `0x55` executes integer `NEG` on the float storage dword before pushing type `f`; source preserves that observed behavior. The reviewed physical owner is `0x0044E1A0-0x0044FD0B` = 7,020 bytes, including two alignment bytes, the 60-entry destination table and 88-byte selector table after code ends at `0x0044FBC1`. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-002 | observed / nested-stack-owner | `EclVmContext + 0x08` is a distinct `0x1008`-byte `EclVmStackView`, containing `data[0x1000]`, `stackTop` at stack `+0x1000` and `frameBase` at stack `+0x1004`; the enclosing context remains exactly `0x1024` bytes with those fields at context `+0x1008/+0x100C`. `Push @ 0x004505B0` optionally emits a four-byte typed-slot tag and copies an arbitrary-sized value. `EnterFrame @ 0x00450690` reserves locals, saves the prior frame base and publishes the prior top. `LeaveFrame @ 0x004506D0` restores the saved base and resets the top to the frame captured on entry. Their reviewed target ABIs use the nested stack receiver in private EAX. | Hash-attested target code and callers; Factory-native Ghidra helper decompilation; maintained exact-size layout assertions and natural source |
| ECLVM-003 | observed / recovered-missed-owner | Direct target review recovers the Ghidra-missed `EclVmStackView::Pop @ 0x00450610-0x00450689` as a 122-byte owner ending in `RET 4`, followed by six `CC` bytes before `EnterFrame`. Its private ABI uses EAX for the nested stack receiver, ECX for byte count, BL for requested scalar type and one stack output pointer. It copies the value, optionally consumes a four-byte type slot and converts stored `i`/`f` values when the requested type differs. | Complete hash-attested target disassembly and neighboring boundaries; exact linked contribution; maintained natural source |
| ECLVM-004 | observed / canonical-exact-six | The reviewed operand corridor `0x0044FDB0-0x004500CC` contains six `EclVmContext` helpers for immediate operands, frame locals, typed-stack pops and host-owned integer/float variables. `ReadInt` (144 bytes), `ReadFloat` (183), `ReadIntValue` (124), `ReadFloatValue` (171), `ResolveInt` (58) and `ResolveFloat` (93) are all canonical exact in their accepted linked contexts. Treat their private receiver/register seams and typed-pop lowering as closed constraints when changing the generic runner or Enemy dispatcher. | `config/functions.csv`; `config/matches.csv`; canonical ECL match units and replay |
| ECLVM-005 | observed / host-thread-and-database-owners | The generic ECL host has a target-proven `0x103C` prefix: active context `+0x4`, embedded `0x1024` context `+0x8`, script database `+0x102C`, and an embedded three-pointer thread-list sentinel at `+0x1030`. `FindThread` searches that sentinel and its secondary nodes and returns the containing node; target `Run` callers then dereference the context. `StopAllThreads` begins at the sentinel's next pointer and clears only secondary context instructions. `SpawnThread` allocates a new context and node, inherits the active difficulty mask, links it at the head and starts the requested subroutine. `StartSubroutine` copies and converts call arguments, records return time/instruction state, changes the active context and resolves the named instruction through the script database. | Canonical target disassembly and callers; hash-attested local Ghidra decompilation; complete offset assertions; maintained source in `src/EclVm.cpp` |
| ECLVM-006 | observed / source-present-script-database | `EclVmScriptDatabase::AddScriptData @ 0x00450220` validates SCPT magic/version, retains up to 32 file buffers, locates a subroutine-offset array after the 0x24-byte header and variable include block, and merges packed names plus relative subroutine headers into an eight-byte-entry lexically sorted table. The first file initializes the table; later files copy and free the previous allocation, insert each new entry before the first greater-or-equal name, and shift later entries. Nonzero include length dispatches virtual slot one on file `+0x24`. The database is the same target-proven polymorphic 0x1098-byte resource owner whose derived Enemy vtable binds slot zero here, slot one to the ANIM/ECLI package loader and slot two to the filename loader. | Complete target owner disassembly and sole direct caller; hash-attested Ghidra decompilation; target vtables `0x0046D0B4`/`0x0046D0F0`; maintained layout assertions and natural source |
| ECLVM-007 | observed / exact-host-runner | `EclVmHost::Run @ 0x0044FD10-0x0044FDA1` walks the sentinel at host `+0x1030` and every spawned-thread node, publishes each context at host `+0x4`, and calls the generic context runner with one float delta. Failure of the embedded context returns `-1`; failure of a spawned context deletes that context, repairs both neighboring links, clears the removed node links and deletes the node. Normal completion restores host `+0x4` to the embedded context at `+0x8`. The only target callers are the two maintained `EnemyRuntimeUpdate` sites at `0x0040E0FE/0x0040E205`, which keep the host in EDI. | Complete target disassembly and boundary; Factory-native Ghidra decompile/callers/callees/xrefs under passed target attestation; maintained source and exact layout assertions |
| ECLVM-008 | observed / source-shape-refined | Target `EclVmContext::Run` expands typed `Pop` paths in each opcode body, keeps typed pushes as calls to the independent exact `EclVmStackView::Push` owner, and embeds formatted-operand evaluation inside the 7,020-byte physical owner. The maintained source now expresses that asymmetric VC7.1 shape, uses the target's one-byte format flag index and unsigned spawn-thread operand-index shift, and removes a target-absent raw-return-pop initializer. The target entry continues to consume context in EAX and one float delta on the stack with `RET 4`. Exactness remains unknown. | Hash-attested target case disassembly; exact stack-owner boundaries; selected real-host linked diagnostic; maintained source |
| ECLVM-009 | observed / control-flow-refined | The target physically groups generic VM cases by execution family rather than opcode number: integer arithmetic precedes float arithmetic, integer comparisons precede float comparisons, and the thread, jump, vector and formatted-operand groups each have target-specific order. The maintained switch now emits the same 59-case physical order. RETURN pops the restored instruction and time directly into their context fields. Conditional true/false jumps share the single JUMP body, and jump offsets plus ordinary instruction sizes join at one instruction-writeback tail. These changes reproduce target control flow rather than duplicating equivalent case bodies. | Target and candidate jump-table destination order; target case/tail disassembly; maintained natural switch and shared labels; `report-ecl-vm-table.py` candidate comparison |
| ECLVM-010 | observed / arithmetic-core-recovered | The nine arithmetic cases in generic `EclVmContext::Run @ 0x0044E1A0` pop right and left operands directly through `EclVmStackView::Pop`, mutate the left typed local in place and push that same local. Integer and raw-float-storage negation likewise reuse the popped local, and post-decrement pushes its preserved original value after writing the decremented destination. These source forms reproduce the target's duplicated typed-conversion paths rather than merging them through scalar wrapper return values. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-011 | observed / exact-stack-read-leaves | Direct state updates recover the original source/codegen seam shared by the stack/read leaf corridor. `EclVmStackView::Pop` tests `stackTop - size`, then performs `stackTop -= size` and copies from the updated member; `LeaveFrame` similarly decrements `stackTop` directly before loading the saved frame base. The standalone exact `Pop` body is unchanged, while LTCG constant-propagated four-byte pops now use the target `ADD reg,-4` form. This closes `ReadFloat @ 0x0044FE40` (183 bytes), `ReadIntValue @ 0x0044FF00` (124), `ReadFloatValue @ 0x0044FF80` (171), and `LeaveFrame @ 0x004506D0` (41) without ABI forcing or byte patches. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-015 | observed / private-receiver-context | IDA shows only two direct callers of `EclVmContext::Run @ 0x0044E1A0`, both inside `EclVmHost::Run @ 0x0044FD10`. Each leaves the context in EAX and pushes the time delta before the call. Selecting the real host as the focused `/GL` link entry makes the maintained candidate use EAX as the runner receiver too; selecting the runner itself as an external entry makes the candidate use ECX. Both selected contributions are 7,040 bytes versus 7,020 target. The real host context therefore recovers this private receiver convention, while the EBX/EBP coloring and body differences remain unresolved. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-019 | observed / loop-entry-source-shape | Target `EclVmContext::Run` checks the instruction pointer once on entry, then tests instruction time without a repeated null guard on each loop iteration. Removing the extra guard from the maintained source makes the selected real-host `/GL` candidate's entry through the first case selector instruction match the target instruction sequence and branch layout except for the unresolved `0x104` versus `0x108` frame constant and linked addresses. The candidate's return-to-loop path now uses `FILD/FCOMP` as the target does, rather than a pointer-null comparison. Its contribution grows from 7,020 to 7,036 bytes against target 7,020, but the sum of absolute physical case-gap differences improves from 68 to 60 bytes and raw comparable-byte agreement improves from 591 to 694 of 6,268. The target's shared advance computes `ECX = EAX + offset` then stores the pointer; the candidate adds the offset directly in memory and reloads it. Changing only the tail spelling or offset signedness compiled identically; reloading jump operands through the cursor changed the whole candidate to 6,972 bytes and was reverted. This remains non-exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-020 | observed / direct-store-operand-shape | Target `Run` opcodes 0x2B `STORE_INT` and 0x2D `STORE_FLOAT` resolve the destination first, then inline `EclVmStackView::Pop` so the popped value is copied directly to that destination before tagged-type conversion. The previous source used a temporary return from `PopInt`/`PopFloat` and assigned it afterward, changing both order and underflow behavior. Replacing those two cases with direct `stack.Pop(type, sizeof(value), ResolveX(0))` makes their selected candidate physical spans exactly target-sized: 80/80 and 86/86 bytes, with matching local instruction shapes apart from linked destinations. The selected whole runner is now 7,032 versus 7,020 target bytes, all 59 opcode groups remain in order, and the sum of absolute case-gap differences falls from 60 to 52. The frame moves to `0xFC` versus target `0x108`; cases 0x34/0x3F acquire offsetting `+12/-12` gaps, and the complete owner is non-exact. Target `ReadInt @ 0x0044FDB0` takes an EDX receiver plus stack argument while the probe-private candidate helper currently takes EDX plus ECX; this explains part of the still-open 0x14 call-shape difference and must not be disguised as an exact ABI. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-021 | observed / exact-polar-helper-and-runner-shape | The runner's opcode 0x51 calls a separate 30-byte `__thiscall` polar-vector helper at `0x004501B0`; its x87 `FSINCOS` writes cosine and sine times magnitude to two floats. The complete helper is byte-identical to the target's ANM `AnmFloat3View::FromAngleMagnitude @ 0x004458B0`. A maintained `EclVmPolarVectorView::FromAngleMagnitude` compiled in the real `EclVmHost::Run` `/GL` link context reproduces all 30 PDB-owned bytes raw-equal in two cold builds, with no linkage fields. Opcodes 0x51 and 0x52 both use the separately exact `EnemyWrapAngle @ 0x0044BC70`; expressing the 0x51 call with nested argument evaluation gives target-sized 97/97 and 36/36 physical case spans. The complete runner remains non-exact: 7,032 candidate versus 7,020 target bytes, `0x104` versus `0x108` frame, although all 59 physical opcode groups retain target order. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-022 | observed / polar-local-stack-shape | The target runner's 0x51 call addresses its temporary vector's X/Y at `[esp+0x10C]` and `[esp+0x110]` after the helper returns. Its 30-byte helper has the same complete instruction body as the ANM 12-byte `AnmFloat3View::FromAngleMagnitude`; the target caller reserves a `0x108` frame. Modeling the ECL local as a three-float vector raises the candidate frame from `0x104` to `0x108` while preserving the complete 30-byte helper exact match, the 97/97 polar case span, all 59 opcode-group order, and the 7,032-byte candidate owner size. In the anchored linked diagnostic, the first raw difference moves from offset `0x2` (frame immediate) to `0x32` (branch displacement). The third float's semantic use remains unproven, and this local improvement does not make the whole runner exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-025 | observed / index-parameter-pop-abi | The target `ReadInt @ 0x0044FDB0` uses EDX for its ECL context, reads the index from the stack, and returns with `RET 4`; its typed pop can overwrite that stack argument. Writing the pop result directly into the maintained `index` parameter makes selected EclVmHost and Enemy-supported `/GS` LTCG candidates use the same private ABI and the full 144-byte extent. Against the hash-attested target, the Enemy-supported candidate differs at raw offsets `0x51,0x52,0x55,0x5C` because it uses `LEA ECX,[ESI-4]` and subsequent ECX references where target uses `ADD ESI,-4`; raw offsets `0x6A-0x6C` are the expected linked `__ftol2` call displacement. The function is still non-exact. Changing only the index parameter's signedness gave the same 144-byte body and was reverted. The EclVmHost-entry runner becomes 7,016 versus target 7,020 bytes, with all 59 physical opcode groups in target order and a four-byte pre-table deficit. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-036 | observed / exact-spawn-thread-and-receiver-lifetime-checkpoint | Delaying the local `EclVmHost *host` cache in `StartSubroutine` until after saving `previousContext` and publishing `destination` changes VC7.1 whole-program register allocation without changing semantics. The helper now receives destination in target-private EAX, saves ESI, and begins with `MOV ESI,EAX`; both target callers naturally regain `MOV EAX,ESI` before the call. `EclVmHost::SpawnThread @ 0x004500D0` consequently becomes canonical exact at 142/142 bytes: two independent cold linked-PE replays reproduce all 142 bytes after normalizing the two operator-new REL32 fields and the StartSubroutine REL32 field. In the same selected real-host graph, `StartSubroutine` improves from 522 to 533/550 bytes and 78/534 comparable bytes, while `Run` remains 7,020/7,020 with a 6,692/6,692 pre-table span and rises from 946 to 975/6,264 comparable bytes. Existing `EclVmHost::Run` remains canonical exact, and the full `src/EclVm.cpp` gate closes 15/15 units across three artifacts for 1,473/1,473 bytes. The remaining StartSubroutine and Run gaps are still allocator/source-shape problems; exact SpawnThread does not make either owner exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-053 | observed / complete-target review / bounded controls restored | Fresh review confirms AddScriptData at 0x00450220 owns all 589 bytes through 0x0045046C: 198 decoded instructions, RET 4 and three following CC padding bytes. Target body SHA256 is 4402f4e16d17bf79a1dce1cea5712ca2fc9f70af68839a46117be70f0456ed58. Unchanged maintained source emits 547 bytes/180 instructions with a 0x0C frame. Target's 0x10 frame holds name, sortedCount, scan pointer and added count, while its offset cursor occupies the incoming argument home. It retains the successful-path file-slot reload and conditional free guard that baseline removes; the free-guard TEST precedes REP MOVSD and its branch follows it. Positive validation, direct parameter cursor, merge-for, first-for, validation label and slot-reference controls all remain 547 bytes/180 instructions with a 0x0C frame. This is extent/frame neutrality, not universal byte neutrality: parameter-cursor and first-for change operands. Merge labels produce 551 bytes/181 instructions with changed block ordering. Isolated /Og- produces 944 bytes/296 instructions with an EBP frame and is rejected. A fresh isolated /GL entry stays 547 bytes and non-exact. Existing sorting, total-count shift bound, strlen(name)+1 and 43-byte callback/return tail remain supported. All trials were restored; no source/header/ledger changes or partial exact credit were retained. Original TU/profile and complete-owner exactness remain open; this bounded batch is closed pending a new evidence-backed hypothesis. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ECLVM-060 | observed / start-subroutine-current-constraint | `EclVmContext::StartSubroutine @ 0x0044DF70` is the reviewed 550-byte private-EAX owner with exactly two explicit stack arguments and `RET 8`. Current maintained source preserves the target-supported member-owner shape, typed operand-word indexing through `valueOffset / 4`, caller-instruction pointer state, empty-stack zero return-word behavior, and active-context restoration through `caller->host`; the owner remains non-exact. `EclVmHost::SpawnThread @ 0x004500D0` is canonical exact at 142 bytes and therefore fixes the caller/private-receiver seam: future StartSubroutine work must preserve that exact unit and rebuild the large runner diagnostic from current source. | `config/functions.csv`; canonical `ecl-vm-spawn-thread` unit; target disassembly; maintained `src/EclVm.cpp` |
| ECLVM-064 | observed / rejected-typed-copy-control | In the current real-host context, StartSubroutine is551/550 bytes and329/534 comparable. Reversing only the integer-source typed-copy conditional to put the float destination first triggers unwanted block sharing, yielding543 bytes and183/534 comparable; the change was reverted. Run remains6125/6264 comparable over all7020 owned bytes, with incomplete selector normalization. This failed control does not justify repeating a broad FORMAT or typed-copy spelling search. | Bounded real-host diagnostic on2026-10-14; Git history |
| ECLVM-061 | observed / argument-scheduling-refined / non-exact | The target Run opcode 0x10 pushes the SpawnThread skip operand before evaluating ReadIntValue. Nesting that evaluation in the call restores the target push sequence and call offsets, with no helper body or interface changes. In two cold real-host-entry VC7.1 /GL probes, the full 7,020-byte owner remains target-sized and all 59 case destinations align; matched-comparable bytes improve from 6,102 to 6,115 of 6,264, separately from raw matches 6,210 to 6,223 of 7,020. The argument still occupies EDI rather than target EDX, and other differences remain. Normalization is incomplete because owner offsets 0x1B14-0x1B6B are the 88-byte selector table, outside decoded bytes and DIR32 fields; the checker is unchanged and no exact credit is claimed. Two FORMAT pointer/switch controls worsened full-owner matching and were reverted. | Hash-attested target opcode disassembly; unchanged full owner and table report; two cold EclVmHost::Run-entry probes on 2026-10-14; current source and Git history |
| ECLVM-062 | observed / format-store-scheduling-refined / non-exact | Moving only percent = found ahead of the FORMAT null-result check restores the target TEST/store/conditional-branch scheduling at owner offset 0x190F. The target stores at ESP+0x10 while the candidate still uses ESP+0x14, and the scratch pointer has the opposite home; no stack-slot exactness is claimed. Two cold real-host-entry probes reproduce the complete 7,020-byte owner with all 59 dispatch destinations aligned. Matched-comparable bytes improve 6,115 to 6,125 of 6,264; raw matches separately improve 6,223 to 6,233 of 7,020. The 88-byte selector suffix still leaves normalization incomplete. All three source changes in the earlier compound FORMAT controls were not equivalent to this isolated retained move. No helper was modified, no checker was changed and no exact credit is added. | Complete target/pinned candidate disassembly; full table and extent report; two cold probes on 2026-10-14 |
| ECLVM-063 | observed / isolated-negative-controls | After retaining the FORMAT store-before-check change, removing only percent pointer volatility grew Run from 7,020 to 7,024 bytes and regressed matched-comparable bytes from 6,125 to 5,814 of 6,264. Replacing only the conversion if-chain with a natural switch preserved signed-char/default semantics but also emitted 7,024 bytes and 5,872 of 6,264 comparable matches. Both preserve physical group order while shifting the pre-table span by four bytes; both were reverted. Stop this spelling branch unless new lifetime evidence appears. | Complete owner and table diagnostics from pinned real-host probes on 2026-10-14; retained source checkpoint |

## Enemy and ECL dispatcher

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ENEMY-001 | observed / corroborated | `0x0040DC80-0x0040E5EB` is the reviewed 2,412-byte hostile runtime update owner mapped descriptively as `EnemyRuntimeUpdate`. Both direct TH10 callers pass full enemy object `+0x103C`; the body itself loads exactly one stack pointer and every normal/early exit uses `RET 4`. The owner copies the prior 0x2C motion record, updates two position and four scalar interpolation records, composes three 0x2C motion records, applies optional movement bounds/offscreen retirement, switches animation state, runs ECL, calls the reviewed Player damage owner, applies Player/game-state damage scaling, subtracts life at runtime `+0x13C0`, resolves callbacks/death/score reward, checks Player collision, updates eight managed VM ids, selects a Player tracked-enemy pointer, applies four-frame damage flash state and advances timers. Player `+0x3504/+0x3508`, previously neutral scratch fields, are target-observed here as a full-enemy tracked pointer plus per-frame validity byte. Committed TH08 `EnemyManager::OnUpdate` was consulted only after the TH10 object/update structure was independently recovered and corroborates the broad enemy-update/damage source family; no TH08 address, layout, ABI or exactness fact is transferred. | Attested TH10 function/caller/callee/decompile/disassembly evidence; direct raw entry/tail/call sites; committed clean TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` only for later naming/source-family corroboration |
| ENEMY-002 | observed | TH10 allocation/constructor/teardown evidence closes the maintained hostile object extents independently of decompiler types. `EnemySpawn` requests exactly `0x2518` bytes. Constructor `0x0040D830` clears exactly `0x537` dwords = `0x14DC` bytes from full object `+0x103C` to the allocation end and stores the full-object back-pointer at runtime `+0x14D8` (full `+0x2514`). Within that tail, four consecutive 0x2C motion records occupy `+0x000/+0x02C/+0x058/+0x084`; two 2D collision-size pairs begin `+0x0B0/+0x0B8`; teardown proves ten managed VM ids at `+0x0C0` while the runtime update consumes the first eight; spawn parameters occupy eight dwords at `+0x0FC`; a 0x14 timer is at `+0x11C`; the exact 12-byte manager list node is `+0x130`; two 0x4C position interpolators begin `+0x13C`; four 0x3C scalar interpolators begin `+0x1D4`; reward/life are `+0x13BC/+0x13C0`; item-drop type/count storage begins `+0x13CC`; constructor writes `32.0f` at `+0x1400/+0x1404`; death sound/effect controls begin `+0x1408`; damage-flash frames are `+0x1414`; timers begin `+0x141C/+0x1430`; flags are `+0x1444`; manager slot is `+0x1450`; eight 0x10 callback-threshold records begin `+0x1458`; and owner is `+0x14D8`. Maintained VC7 assertions keep `sizeof(EnemyRuntimeView)==0x14DC` and `sizeof(EnemyFullObjectView)==0x2518` without claiming original type/member identifiers. | TH10 spawn allocation; constructor clear/nonzero writes; teardown ten-id scan; runtime raw offsets; pinned VC7 layout assertions |
| ENEMY-003 | observed / denominator-expanded | The reviewed Enemy lifecycle seam consists of `EnemySpawn` at `0x0040CFB0`, private-EDI `EnemyManagerUpdate` at `0x0040D750`, the directly registered update/draw callbacks at `0x0040D810/0x0040D820`, ESI-bound constructor `0x0040D830`, stack-bound teardown `0x0040DAE0`, and death finalizer `0x0040E5F0`. Spawn corrects the initial Ghidra boundary: executable code ends at `0x0040D1A4`, a three-byte alignment sled occupies `0x0040D1A5-0x0040D1A7`, its directly indexed five-entry jump table is `0x0040D1A8-0x0040D1BB`, and its 53-byte selector table is `0x0040D1BC-0x0040D1F0`; `0x0040D1F1-0x0040D1FF` is CC padding. Therefore the source-owned physical span is 577 bytes, not the imported 501-byte candidate. Manager initialization directly stores `0x0040D810` as update callback priority `0x12` and `0x0040D820` as draw callback priority `0x14`, so those Ghidra-missed 10-byte and 6-byte bodies are added to the denominator. The primary enemy vtable slot `+0x14` points to scalar-deleting wrapper `0x0040CC50`, which calls `EnemyTeardown` before optional deallocation; maintained source keeps wrapper/delete mechanics separate from the teardown body. | TH10 raw entry/tail/table bytes; direct table xrefs; manager callback registration; vtable contents and indirect deletion call; reviewed caller/callee seams |
| ENEMY-004 | observed | The lifecycle seam closes several cross-object references. `EnemyManagerView` is exactly `0x68` bytes; its timer begins `+0x40`, script database `+0x54`, list head/tail `+0x58/+0x5C`, active count `+0x60`, and spawn counter `+0x64`. The embedded enemy list node at full `+0x116C` (runtime `+0x130`) stores the full object, next and previous links. Teardown scans ten managed VM ids and marks resolved VM/child states for deletion, then clears Player `+0x3504/+0x3508` if it tracks this enemy and scans 128 Player shot rows at Player `+0x4E8` with stride `0x5C`; because `shots[]` begins at `+0x49C`, this proves each `PlayerShotRuntimeView +0x4C` is an `EnemyFullObjectView *trackedEnemy`, which teardown nulls before object destruction. Death finalization uses direct drop type at runtime `+0x13CC` plus eleven count slots at `+0x13D0`, position at runtime `+0x02C`, and target-observed sound/effect/item helpers. Raw aligned bodies at `0x0040D200`, `0x0040D730`, `0x0040D800`, and `0x0040DC70` remain inventory gaps with no modeled xref or absolute-pointer occurrence in the reviewed PE scan; they are not promoted to authored merely from body shape. Subsequent manager-lifecycle review independently promoted `0x0040D260` from repeated constructor semantics and classified `0x0040D710` as compiler-generated; see `ENEMY-005`/`ENEMY-006`. | TH10 teardown/death raw control flow; Player shot offset reconciliation; manager factory/list users; bounded PE absolute-pointer scan plus attested Ghidra xrefs |
| ENEMY-005 | observed / denominator-expanded | The reviewed Enemy manager lifetime is a coherent source family around the exact `0x68` manager: `0x0040D260-0x0040D27C` is a Ghidra-missed 29-byte manager constructor body, `0x0040D280-0x0040D3CD` is the EBX-bound initializer, `0x0040D530-0x0040D65B` is the EAX-bound destructor body, and `0x0040D6B0-0x0040D70E` is the one-argument factory/rollback owner. The retained constructor clears timer flag bit zero at `+0x50`, zeroes all 26 dwords, sets manager flags bit 1, publishes `g_EnemyManager`, and returns the object; the same construction sequence appears inline inside the factory. Initialization installs the primary resource and registers 0x24-byte update/draw callback nodes at priorities `0x12/0x14`; each target node constructor clears only flags bit zero, stores `self` at `+0x14`, clears `+0x0C/+0x10/+0x18/+0x1C`, and the initializer later stores the manager owner at `+0x20`. Teardown clears live enemy state, removes both callback nodes under the callback critical section, releases ECL and animation resources, and clears the global owner. Pinned VC7.1 natural constructor/destructor/member source reproduces the same object family and also synthesizes the deleting-destructor helper class, but fixed normal target comparisons remain mismatches and `/GL` feasibility has no exactness authority. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-006 | observed / denominator-expanded | The manager-owned ECL resource is exactly `0x1098` bytes and is naturally modeled as a polymorphic base/derived partial type. Target derived vtable `0x0046D0B4` contains inherited slot `0x00450220`, package loader `0x0040D400`, and file loader `0x0040CD20`; base vtable `0x0046D0F0` contains `0x00450220` and `0x0040C810`. `0x0040D3D0-0x0040D3F1` pre-clears neutral nested dwords at `+0x1090/+0x1094`, then clears the complete `0x1098` bytes and installs the derived vptr. Natural VC7.1 source with those two nested dword constructors and a containing full-object clear emits the same core ordering, differing by the conventional ECX-to-EDX receiver setup. `0x0040D400-0x0040D4EE` parses `ANIM`/`ECLI` package lists and uses a virtual file-load dispatch; `0x0040CD20-0x0040CD7E` loads one ECL file and passes the target file loader all three logical values (filename plus zero size-out/mode); `0x0040D680-0x0040D6A3` transitions to the base vptr and releases the owned lookup table at `+0x8C`. Teardown independently proves 32 owned script-data pointers at `+0x0C..+0x88`. The scalar deleting wrappers at `0x0040CC50`, `0x0040D660`, and `0x0040D710` are compiler-generated/excluded; VC7.1 automatically emits the corresponding helper family from maintained natural destructor/delete source. No body in this packet is exact under the fixed normal diagnostic. | TH10 vtable contents, raw constructor/file-loader/package/destructor bodies and manager teardown; existing target-local file-loader ABI evidence; pinned VC7.1 class/vtable/deleting-helper output and relocation-aware diagnostics |
| ENEMY-007 | observed / denominator-expanded | TH10 establishes a six-slot derived Enemy ECL-evaluation vtable at `0x0046D0C0-0x0046D0D4` and a separate six-slot base-state vtable at `0x0046D0D8-0x0046D0EC`; they are not one twelve-slot table. `EnemyConstruct` writes the derived vptr at full-object `+0x00`, teardown restores that same derived vptr before the base destructor path, and the base constructor at `0x0040C710` writes `0x0046D0D8`. Derived slot zero is the reviewed 11-byte `0x0040E760-0x0040E76A` forwarding method: it adds the independently proven runtime-tail offset `0x103C` to ECX and tail-jumps to `0x0040E770`. The runtime target is an authored Enemy-specific ECL instruction dispatcher with machine boundary `ECX EnemyRuntimeView* / RET`; it reads the full owner from runtime `+0x14D8` and switches over opcodes `0x100-0x1B4`. Its source-owned physical span is `0x0040E770-0x00411FBF` = 14,416 bytes, not Ghidra's 13,759 reachable-code bytes: after the final RET at `0x00411D2E`, the owner includes one-byte alignment, a 108-entry jump table, a 181-byte selector table, three-byte alignment, and two five-entry nested jump tables before the next method at `0x00411FC0`. Derived slots one through four are separately reviewed authored operand interfaces at `0x00411FC0-0x004122F3` (`ReadIntOperand`, 820 bytes), `0x00412300-0x0041234F` (`ResolveIntOperand`, 80), `0x00412350-0x004126C3` (`ReadFloatOperand`, 884), and `0x004126D0-0x0041271F` (`ResolveFloatOperand`, 80); their maintained names are descriptive. At this checkpoint their source bodies remained unknown; `ENEMY-008` records the later source reconstruction. Committed TH08/TH095 ECL source was consulted only for broad execute/operand-family context after the TH10 vtable and switch structure were recovered; it does not supply TH10 names, addresses, layouts or exactness. | TH10 raw vtable bytes and constructor/destructor vptr writes; attested vtable/data xrefs and ECL-dispatch decompilation; direct switch-table references and target-owned table extents; committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and committed TH095 HEAD `3c54464e6d567a37219b31c4ee8bc49e033b61c1` used only as later source-family hypotheses |
| ENEMY-008 | observed / corroborated | The four reviewed Enemy ECL operand methods now have maintained source: integer reader `0x00411FC0-0x004122F3`, integer lvalue resolver `0x00412300-0x0041234F`, float reader `0x00412350-0x004126C3`, and float lvalue resolver `0x004126D0-0x0041271F`. The two 51-entry readers normalize negative selectors by adding decimal 10000 (`0x2710`) and preserve target-specific type-dependent behavior. Direct jump-table entries prove integer selectors `-9996` and `-9976` both route directly to target `0x0041219A` while `ECX` still names this Enemy, so they read this Enemy world Y before float-to-int conversion; only selector `-9962` enters through `0x00412192`, replaces `ECX` with the manager primary special-enemy pointer, and then falls through to the same Y load, while `-9963` exposes the corresponding special-enemy X directly. The last four selectors are direct difficulty equality tests: the integer reader compares against `0/1/2/3`, and the float reader compares the same selector sequence against `0.0/1.0/2.0/3.0`. The two lvalue methods independently prove the full-object `+0x1138..+0x1157` / runtime `+0x0FC..+0x11B` block is four writable integer values followed by four writable float values, refining the earlier eight-dword spawn-parameter observation. Target spawn copies exactly eight dwords from request `+0x20` into this block; maintained source uses the same 0x20-byte view on both sides, while the original request-side C++ declaration remains unknown. Reader cases are grounded in TH10-local fields/helpers: RNG state, Player position and angle-to-point helper, current/special Enemy motion, update timer, runtime flag bit `0x10000`, managed-VM id lookup and VM word `+0x38A`, life, rank-like global at `0x00474C98`, and difficulty at `0x00474C74`. TH10 initialization uses `0x00474C74` as a `0..4` difficulty index and the operand protocol exposes direct value plus equality-to-0/1/2/3 tests; `0x00474C98` is target-observed as the bounded `[-0x400,+0x400]` rank scalar: gameplay paths increment/decrement and clamp it, while the ECL operand readers expose its integer/float value. Committed TH08 and TH095 operand sources were consulted only after these TH10 tables/offsets were decoded and corroborate the broad split int/float/lvalue operand family; no adjacent selector id, address, layout or exactness fact is transferred. | Direct TH10 raw switch tables and case bodies; attested helper/global xrefs and decompilation; spawn-copy offsets plus exact lvalue extents; committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` and committed TH095 HEAD `f18d7c5e25087d8134146220917e2e33bfb2ab28` used only for later source-family corroboration |
| ENEMY-009 | observed / origin-unknown-helper-seam | TH10-local review of the source-present but non-exact `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` establishes a central ECL execution-context seam. Full Enemy `+0x008..+0x101F` is a `0x1018`-byte execution context; full `+0x004` points at the active context. Context `+0x004` is the current variable-length ECL instruction pointer, whose target-observed header has opcode at `+0x004`, operand-variable mask at `+0x008`, and dword operands from `+0x010`. Generic operand helpers prove context `+0x1008` is operand-stack state, `+0x100C` participates in local-variable address formation, and context `+0x1014` is the operand-resolver owner. Constructor target code stores the full Enemy itself at full `+0x101C` = context `+0x1014`, binding that resolver to the Enemy vtable typed operand methods. Adjacent machine wrappers `0x00412A00-0x00412A0F` and `0x00412A10-0x00412A5A` use private EAX=`EnemyRuntimeView*`, ECX=argument-index ABIs for integer argument read/lvalue resolution; all 30 and 9 direct calls respectively are inside the reviewed dispatcher, but source-written versus compiler/LTCG-outlined origin remains unknown. The top-level selector maps opcodes `0x15D..0x164` to `0x004118BB,0x004118E2,0x0041198B,0x004119FC,0x00411A3E,0x00411B23,0x0041009B,0x004100EA`; this family performs rank/difficulty-selected typed writes and interpolation, including float rank interpolation using target constants `1024.0f` and `1/2048.0f` and integer interpolation at `0x00411B23` with signed truncation-toward-zero correction. The two five-entry nested tables at `0x00411F98` and `0x00411FAC` are reached by the difficulty cases. Three additional CC-delimited bodies `0x00412A60-0x00412A6D`, `0x00412A70-0x00412A7D`, and `0x00412A80-0x00412A90` were missed by the initial inventory; complete target direct-REL32 and absolute-pointer scans find no references, so they are added only as origin-unknown review candidates, not authored/source-present functions. Committed adjacent-game source was consulted only as source-family hypothesis material after TH10-local recovery: clean TH08 `a45e99fb1942714e6edded20847e32a654d56f97`, clean TH09 `f1d4c1e332828a0cb8b9308b61b79ce77f86e4d0`, and only committed HEAD content from dirty TH095 `dee282c33dbb75ca0545d3115cd421a6e4f6a7bd`; no adjacent address, opcode, layout, owner or exactness fact is transferred. | Direct TH10 raw dispatcher/table/helper bytes and complete target reference scans; attested `th10-ghidra` helper/decompile/xref evidence with passed `factory-native-command` target attestation; target constructor initialization; committed adjacent repositories only after TH10-local recovery |
| ENEMY-010 | observed / source-present | TH10 has a reusable ECL-host lifecycle separate from the 14,416-byte Enemy opcode dispatcher. At both callback-subroutine transitions, `EnemyRuntimeUpdate` obtains a callback identifier, calls `0x0040C6E0` to free every payload/node pair in the full object's list at `+0x1034`, then calls `0x0040C730` to restore the embedded `0x1018` context at full `+0x008` as both active pointer `+0x004` and mirror `+0x1030`. Reset clears the current instruction and state, installs the full object at context `+0x1014` as the operand resolver, writes `-1` at context `+0x1010`, and clears allocation ownership without erasing the `0x1000`-byte operand payload. The maintained runtime source now expresses both target calls instead of collapsing them into one unresolved reset helper. The release body is exact; reset remains a 69-byte LTCG candidate versus the target's 71-byte ESI-preserving form. | TH10 direct disassembly and both call sites at `0x0040E1D3/DE` and `0x0040E28C/97`; target object offsets; current VC7.1 normal and linked diagnostics |
| ENEMY-011 | observed / source-present | The primary Enemy object is a real C++ derivation from a reusable `0x103C`-byte ECL host. The base vtable at `0x0046D0D8` contains five typed execution/operand slots followed by scalar deleting destructor `0x0040C7B0`; the derived vtable at `0x0046D0C0` replaces the five slots and points its deleting slot at `0x0040CC50`. Base construction at `0x0040C710` installs the base vtable and clears the embedded context cursors; derived construction at `0x0040D830` performs the same base/context initialization before installing the derived vtable and appending the runtime tail. Derived teardown at `0x0040DAE0` restores the derived vtable and ends with the inline base-destructor sequence that restores `0x0046D0D8` and releases the `+0x1034` allocation list. Enemy-manager deletion reaches virtual slot `+0x14` with deleting flag one. Maintained source therefore uses `EnemyEclHostBaseView` as an actual polymorphic base, `EnemyFullObjectView` as its derived class, real constructors/destructors, `new`, and virtual `delete`, while preserving the exact `0x103C`/`0x2518` layouts. | Direct TH10 vtable contents/xrefs, constructor/destructor sequences and manager virtual-delete call; pinned VC7.1 layout and code-generation probes; no adjacent-game fact transferred |
| ENEMY-012 | observed / source-present | `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` now has a complete maintained switch covering all 124 target-active opcode values across `0x100-0x1B4`. The families implement enemy creation and ANM binding, offset/base motion and interpolation, hitboxes and lifecycle flags, item drops, callbacks, dialog/spells, rank/difficulty selection, bullet-pattern state and four laser request forms. The target's physical owner remains `0x0040E770-0x00411FBF` = 14,416 bytes: code ends at `0x00411D2E`, followed by alignment, a 108-entry destination table, the 181-byte selector table, alignment and two five-entry nested difficulty tables. The 108 table entries are destination slots rather than the number of semantic opcode cases. Typed 0x40-byte spawn storage and 0x1F8 union-backed laser storage replace Ghidra stack-overlap/array-overrun expressions while preserving target-observed field positions and packed shorts. Opcode/helper identifiers are descriptive where original symbols remain unknown, and the private argument-helper original declarations remain unknown. Later maintained mappings include ReadIntArgument, ResolveIntArgument and ReadRawFloatArgument; see ENEMY-019/054/092 for their separate source/codegen states. | Hash-attested target selector/destination tables and instruction bodies; Factory-attested Ghidra decompilation/callees/xrefs; direct target disassembly; maintained typed runtime/request layouts; adjacent ECL maps used only after target recovery to suggest descriptive family names |
| ENEMY-013 | observed / source-present | `EnemyResolveRuntimeCallback @ 0x004127A0` is the ECX-bound 593-byte core called at both Enemy runtime callback checkpoints. It scans eight `0x10`-byte records at full Enemy `+0x2494`; each record holds a life threshold, timer threshold, life callback name and timer callback name. The life path stores `life-threshold` at full `+0x2404`, clamps life at threshold, disables the record, resets the update timer, clears runtime flag `0x10000` and returns the life callback name. The timer path publishes a seconds countdown capped at 99 to visual owner `+0x9EC0`; at expiry it clamps life, disables the record, resets the timer, sets flag `0x10000`, reduces global `0x00474C4C` by 3000 with a floor of 5000, and conditionally clears bit one in Player words `+0x378C/+0x0AD4/+0x0E80/+0x15D8/+0x1984/+0x1D30/+0x20DC/+0x2488` before returning the timer callback name. The timer reset writes `current=0`, `previous=-999999`, `subframe=0`, scale `0x00476F78` and flag bit zero during initialization, then always ends with `current=0`, `subframe=0`, `previous=-1`. | Hash-attested target disassembly; Factory-attested decompilation, two callers and full target extent; maintained exact-offset views; TH08 consulted only after TH10-local recovery for callback-family corroboration |
| ENEMY-014 | observed / exact | `EnemySetMotionAngle @ 0x00413170` is a complete 20-byte stack-argument `RET 8` helper that normalizes an angle through `EnemyWrapAngle @ 0x0044BC70` and stores the x87 result at motion `+0x1C`. The 94-byte angle wrapper uses signed-pi bounds, two-pi steps, one shared iteration counter, and `RET 4`. Two independent cold normal-COFF builds reproduce both entire target extents and all seven declared call/constant relocations. The `Enemy.cpp` declaration uses the target-proved `__stdcall` ABI; its two pre-existing local-table units remain exact after their compiler label numbers shift. | Hash-attested target; IDA disassembly/decompilation; `enemy-set-motion-angle` and `enemy-wrap-angle` canonical replay; four-source regression |
| ENEMY-017 | observed / projection-call-shape | Target `DispatchEclInstruction` calls the imported `D3DXVec3Project @ 0x0045218E` with six stack arguments and no caller cleanup; the maintained declaration had accidentally used the caller-cleanup convention. Declaring `__stdcall` removes that cleanup in the focused `/GL /GS` candidate. Target projection input X/Y/Z are stored before `EnemyPrepareProjection`, so the source now builds `projectionInput` before the call. Together these changes reduce the candidate PDB contribution from 14,532 to 14,508 bytes against 14,416 target; the entire 92-byte excess lies before the jump table. The 108 case groups remain in target order, the 181-byte selector and 43-byte suffix agree, and the frame stays `0x2BC` against target `0x2C4`. Linked-field normalization is incomplete. No exact claim follows. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-018 | observed / security-cookie-profile-and-entry-gap | The target Enemy dispatcher begins with an EBP-aligned `0x2C4` frame, reads the security cookie at `0x00473660`, and XORs it with EBP. Pinned `/GS` with the real dispatcher entry and ECL support reproduces the prior 14,508-byte candidate against the 14,416-byte target; its physical case order matches and the 181-byte opcode selector is equal. Omitting `/GS` yields a 12,988-byte diagnostic with ECL support or 12,692 bytes without it; these shorter bodies are the wrong codegen profile. The `/GS` candidate uses a `0x2BC` frame, so its remaining 92-byte owner gap cannot be treated as case-body exactness. Marking `ReadInt` noinline leaves the `/GS` dispatcher and helper at 14,508/117 bytes; storing opcode in a 32-bit temporary grows the dispatcher to 14,516 bytes and both probes were reverted. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-019 | observed / argument-resolver-codegen | Target dispatcher `0x0040E770` calls private `EnemyRuntimeView::ResolveIntArgument @ 0x00412A10` nine times. The complete 75-byte target helper loads the active ECL context and implements the same operand-mask/local-slot/virtual-fallback path as `EclVmContext::ResolveInt`. A natural maintained forwarding method in `src/EnemyEclDispatcher.cpp` emits one complete 75-byte PDB-owned LTCG contribution at candidate `0x004010F0`, raw-equal to target in two independent selected-entry `/GS` cold links. The target's game-source versus compiler-outlined origin remains indeterminate, so this is source presence and diagnostic byte equality, **not** a canonical `matches.csv` exact unit. The neighboring `ReadIntArgument @ 0x00412A00` is mapped to its maintained source, but the candidate is 14 versus target 16 bytes because the selected private `ReadInt` ABI still differs. The dispatcher grows from 14,508 to 14,512 bytes against target 14,416 and remains non-exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-021 | observed / difficulty-count-call-shape | In Enemy dispatcher opcode `0x1B4`, the target selects difficulty-dependent operand indices with small branches and joins at one `ReadInt` call for each count field. The maintained source formerly selected an index then called `ReadIntArgument`, causing the selected `/GL /GS` candidate to duplicate the second call and return epilogue in several branches. Placing `ReadIntArgument` in each source branch gives the compiler a closer common-call shape: the 0x1B4 physical interval falls from 254 to 216 bytes against target 151, the whole candidate from 14,284 to 14,244 against target 14,416, and comparable-byte agreement rises from 562 to 595. The 108 case groups remain in target physical order and the 181-byte selector remains equal. The dispatcher is still non-exact; ternary index selection left its span at 14,284 and was reverted. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-022 | observed / float-difficulty-index-codegen | Target dispatcher opcode `0x1B3` selects an integer operand index in ECX before each float read. The prior source carried that integer through a bit-encoded `float`, forcing candidate stack stores and reloads. A case-local `int speedIndex` removes those artificial conversions: the physical interval becomes 169 bytes versus target 170, down from 211, while the complete `/GL /GS` candidate changes from 14,244 to 14,204 bytes versus target 14,416. All 108 case groups retain target physical order and the 181-byte selector is equal. The candidate still scales the bullet index before its first float read where target saves the index in ESI and scales it after the call, so neither this case nor the owner is exact. Seven other variants were checked: a case-local bullet index and separate float-result locals generated the same candidate; an explicit difficulty-2 branch shortened the case to 161 bytes and was reverted; changing the neighboring 0x1B4 selector to a local or preloaded context worsened its whole-owner diagnostic and was reverted. A `switch` raised broad comparable-byte agreement but used the wrong local `sub/dec` branch shape and was reverted. Replacing the shared rank/difficulty float selector with an integer improved one rank case but collapsed another target 169-byte interval from candidate 174 to 110; it too was reverted. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-024 | observed + inferred / scratch-layout-and-count-join | IDA target bytes place the 0x1F8-byte laser request scratch at dispatcher `ESP+0x50` and the 0x84-byte spawn request at `ESP+0x248`, immediately below the security cookie. Modeling the laser request as an array union caused `/GS` to reverse those objects: the selected candidate used `ESP+0xC8` for the laser request and `ESP+0x40` for the spawn request. Modeling the still-semantically-unknown laser request as a 0x1F8-byte scalar-field struct restores the target object order; candidate offsets become `ESP+0x40` and `ESP+0x238`, each uniformly 0x10 below target. This raises comparable-byte agreement from 596/11,660 to 639/11,660. In the same source checkpoint, selecting opcode 0x1B4's two difficulty operands through a case-local integer gives one reader call per field and shrinks its physical interval from 216 to 142 bytes against target 151; combined agreement rises to 671/11,676. The complete owner becomes 14,080 versus 14,416 and remains non-exact; 0x1B3 remains 169/170, all 108 physical groups retain target order, and the 181-byte selector remains equal. The struct field meanings remain unknown and are not semantic claims. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-025 | observed + inferred / rank-float-two-tail | Target opcode 0x164 supplies backward-shared rank-float paths to 0x15D/0x15E, while 0x15E owns two distinct read-resolve-store-return tails. Replacing float-bit operand-index transport with integer indices and preserving those two tails raises complete-owner comparable-byte agreement from 671/11,676 to 688/11,684. Restoring the target-observed AIM_BULLET_AT_PLAYER edge into the second shared resolve-store tail raises it again to 693/11,680. The resulting intervals are 0x164 47/65, 0x15D 41/39, 0x15E 128/169, and 0x1AE 107/76 candidate/target bytes. The complete owner is 13,996 versus 14,416 and remains non-exact; physical case order and the 181-byte selector remain equal. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-026 | observed + inferred / generic-float-resolver-boundary | Target `Enemy ResolveFloat` helper `0x00412A70-0x00412A7D` is a 14-byte adapter that tail-jumps to the separately exact 93-byte generic `EclVmContext::ResolveFloat @ 0x00450070`; dispatcher rank tails inline the adapter while retaining the generic call boundary. Without an explicit generic no-inline boundary, the selected `/GL` probe instead expands the generic resolver into the adapter and leaves the enlarged adapter out of the dispatcher. Declaring the generic resolver no-inline restores the 14-byte adapter and the target call topology. The combined 0x15E/0x1AE physical intervals move from 235 against target 245 bytes to 249 against 245, and the complete dispatcher grows from 13,996 to 14,016 versus target 14,416. Whole-owner comparable-byte agreement is 673/11,680 and remains non-exact; the 93-byte generic resolver and the three Enemy-adjacent exact units still focused-replay exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-027 | observed + inferred / integer-reader-callgraph | The target dispatcher has exactly 30 direct calls to the 16-byte integer-reader adapter at `0x00412A00`; they are confined to the 0x1B1/0x1B2 laser groups, six laser mutation cases, chapter selection, integer rank selection/interpolation, spell timeout, ANM mode, and difficulty subtraction. Other integer reads inline the adapter and call the generic reader directly. The prior candidate inlined every adapter use and had zero direct calls. Preserving the 30 target-observed boundaries produces 30 candidate calls and a target-sized 16-byte adapter; 0x161 and 0x162 become exact-sized at 229/229 and 97/97 bytes. Its private registers remain reversed (`ECX` runtime/`EAX` index versus target `EAX` runtime/`ECX` index), so the complete dispatcher is still non-exact at 14,020/14,416 bytes and 636/11,664 comparable bytes. Physical group order and the 181-byte selector remain exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-028 | observed / spell-call-abi | Target 0x00409280-0x00409BE2 consumes four stack arguments and ends in RET 0x10; the Enemy dispatcher call at 0x00410F3A therefore has no caller cleanup. The maintained EnemyBeginSpell declaration was accidentally caller-clean, and the linked candidate emitted add esp,0x10 after its corresponding call. Restoring __stdcall removes that false cleanup. In the current clean /GL /GS Enemy diagnostic the complete contribution remains 14,296 versus the 14,416-byte target, pre-table span remains 13,640 versus 13,760, the 181-byte selector and physical case-group order remain equal, while normalized comparable-byte agreement improves from 608/11,544 to 657/11,544. The shared START_SPELL group is 214 versus target 219 bytes. This is a target-proven ABI correction and linked-codegen improvement, not an exactness claim. | Hash-attested target call site 0x00410F3A and callee tail 0x00409BE0 RET 0x10; Factory-attested Ghidra target binding; fresh enemy-spell-stdcall probe/layout diagnostics; focused canonical replay of the existing exact Enemy source unit |
| ENEMY-029 | observed / screen-shake-call-abi | Target EnemySetScreenShake @ 0x0043C8B0 consumes five stack arguments and ends in RET 0x14; the Enemy dispatcher call at 0x00410DAD performs no caller cleanup. The maintained declaration was accidentally caller-clean, and the retained spell-ABI candidate emitted add esp,0x14 immediately after the corresponding call. Restoring __stdcall removes that false cleanup. In the selected /GL /GS linked diagnostic, the complete dispatcher contribution changes from 14,296 to 14,292 versus the 14,416-byte target, and the pre-table span from 13,640 to 13,636 versus target 13,760; normalized comparable-byte agreement improves from 657/11,544 to 680/11,544. The 181-byte selector, 43-byte suffix and physical case-group order remain target-equal. This is a target-proven ABI/codegen improvement; the owner remains non-exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-030 | observed / exact | EnemyMarkPendingInterrupt @ 0x00409E50 is a 65-byte private helper used by the spell-state path. It receives an int-pointer VM id in EAX, resolves the VM through AnmRenderManagerView::FindVm, writes pending interrupt 1 at VM +0x304, and when the VM is the root of a layer chain propagates the same interrupt through every child node. The maintained natural source in EnemyEclDispatcher.cpp reproduces the complete PDB-owned extent in the real dispatcher /GL /GS entry context. Two independent cold exact replays cover all 65 bytes plus the renderer-global DIR32 and FindVm REL32 fields zero-difference. The original source filename remains unknown. | Hash-attested target 0x00409E50-0x00409E90; target ABI/disassembly; canonical unit enemy-mark-pending-interrupt; two cold EnemyEclDispatcher exact replays |
| ENEMY-031 | observed / exact | EnemySoundQueueView::QueueSoundSample @ 0x0043DC90 is a 123-byte queue helper with ECX queue owner, EDI sound id and one callee-popped stack sample (RET 4). It reads the cue field at metadata table base 0x004749C8 + 6, scans twelve active slots, caps each slot at 128 samples, appends to an existing matching sound id or initializes the first free slot, and increments the selected sample count. The maintained source reproduces the complete PDB-owned extent in the Enemy dispatcher /GL /GS context. Two independent cold exact replays cover all 123 bytes and the metadata DIR32 field zero-difference. The original source filename remains unknown. | Hash-attested target 0x0043DC90-0x0043DD0A; target ABI/disassembly; canonical unit enemy-sound-queue-sample; two cold EnemyEclDispatcher exact replays |
| ENEMY-032 | observed / exact | EnemySoundQueueView::QueueSoundCue @ 0x0043DD10 is a 149-byte position-aware sound queue helper. The shipped private ABI keeps the queue owner in ESI and sound id in EBX, consumes one callee-popped float position argument, multiplies it by the target 5.2083335 scale constant, converts the result through __ftol2, then uses the same twelve-slot and 128-sample queue structure as QueueSoundSample. The maintained source reproduces the complete PDB-owned extent in the Enemy dispatcher /GL /GS context. Two independent cold exact replays cover all 149 bytes plus the scale-constant DIR32, cue-metadata DIR32 at base +6, and __ftol2 REL32 fields zero-difference. The original source filename remains unknown. | Hash-attested target 0x0043DD10-0x0043DDA4; target ABI/disassembly; canonical unit enemy-sound-queue-cue; two cold EnemyEclDispatcher exact replays |
| ENEMY-033 | observed / exact | EnemyDropVectorView::FromAngleRadii @ 0x00413270 is a 30-byte x87 polar-to-Cartesian helper used by the Enemy item-drop core. The target unique caller at 0x0040CA21 passes angle, X radius and Y radius with ECX pointing at an 8-byte temporary output. The helper uses FSINCOS, stores cos(angle)*radiusX at +0 and sin(angle)*radiusY at +4, and returns with RET 0x0C. The maintained source reproduces the complete relocation-free PDB contribution raw byte-identical. Two independent cold dispatcher-context exact replays confirm all 30 bytes. The original source filename remains unknown. | Hash-attested target 0x00413270-0x0041328D; unique caller 0x0040CA21 in the item-drop core; canonical unit enemy-drop-vector-from-angle-radii; two cold EnemyEclDispatcher exact replays |
| ENEMY-035 | observed / exact-runtime-constructor / full-ctor-open | Target EnemyRuntimeView::EnemyRuntimeView @ 0x0040CC70 is the 145-byte runtime-tail default constructor called from the full Enemy constructor with runtime this in EDX. Restoring the target-proven member model exposes six interpolation records whose embedded timer constructors clear bit zero and eight 0x210-byte bullet-pattern records whose natural constructor clears the complete record and restores the -1 sentinel at +0x204. In the real EnemySpawn /GL /GS entry context, pinned VC7.1 SP1 build6030 reproduces the complete 145-byte PDB-owned runtime constructor raw byte-identically; there are no linkage fields. The same source-shape work improves EnemyFullObjectView::EnemyFullObjectView @ 0x0040D830 from 622/673 bytes and 92/653 normalized comparable bytes to 672/673 and 137/653, but that larger constructor remains non-exact. EnemySpawn stays at the retained 577/577 non-exact source-shape checkpoint. | Hash-attested target 0x0040CC70-0x0040CD00; target full-constructor call at 0x0040D849; canonical unit enemy-runtime-constructor; focused enemy-full-ctor-flags-local-probe.json and runtime structural comparison. |
| ENEMY-037 | observed / direct-float-callgraph-checkpoint | Target direct-reference review shows the 14-byte adapter at `0x00412A60` has no dispatcher xrefs, while `EnemyRuntimeView::DispatchEclInstruction` calls generic `EclVmContext::ReadFloat @ 0x0044FE40` directly throughout the float cases, including the `0x1B3` difficulty-speed reads. The explicit `EclVmContext *context` local introduced by an earlier local experiment caused the selected candidate to retain four false calls to its `ReadFloatArgument` adapter. Restoring the equivalent one-expression member source makes VC7.1 retain a separate 14-byte tail-jump adapter with zero dispatcher calls and routes those sites directly to the generic reader, matching the target call topology. In the current `/GL /GS` support graph the complete dispatcher remains 14,292/14,416 with pre-table 13,636/13,760, selector 181/181 and target-equal physical group order; normalized comparable-byte agreement is 680/11,544 rather than an earlier 696/11,536 heuristic coloring score. The structurally relevant groups improve from 231/287 to 289/287 for 0x10E and from 227/170 to 167/170 for 0x1B3. All five canonical exact `EnemyEclDispatcher.cpp` units cold-replay zero-difference (461/461 bytes). This superseded an earlier experiment at that time but is not an exactness claim for the giant owner. | Hash-attested target `ReadFloat` xrefs and raw adapter bytes; `enemy-readfloat-direct-callgraph-{probe,layout}.json`; focused five-unit canonical replay; pinned VC7.1 SP1 build6030. |
| ENEMY-040 | observed / laser-private-caller-graph | Target `EnemyFireLaser @ 0x0041C510` has ten direct call sites from six reviewed owners: four in `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770`, two in `0x0041CFD0`, and one each in `0x0041D880`, `0x0041DD80`, `0x0041EB00`, and `0x0041EFA0`. Backward register review at every call site proves one consistent private LTCG seam: ESI carries the same global manager from `0x0047781C`, EDI carries the request object (dispatcher stack request, object `+0x424`, or caller stack scratch), and the only explicit stack argument is type 0 or 1. The callee allocates `0xD58` or `0xD74`, calls derived initializers at `0x0041C5B0` or `0x0041C680`, assigns the monotonically nonzero id at object `+0x54`, links the object through manager `+0x434`, increments manager `+0x438`, and dispatches vtable slot `+4` with the request. The common base initializer at `0x0041C030` is a 98-byte EDX-receiver body; the two derived initializer bodies are 202 and 219 bytes before alignment. Recasting only the dispatcher declaration as an ordinary C++ manager member without restoring these external owners produces the wrong ordinary member ABI and worsens the selected dispatcher diagnostic to 14,152/14,416 and 677/11,572, so that experiment is reverted. This strengthens the conclusion that the shipped ESI/EDI convention is a cross-owner LTCG allocation problem and must not be faked locally. | Hash-attested target caller/callee disassembly; Factory-attested callers/decompilation for `0x0041C510`; six-owner backward ESI/EDI register trace; negative `enemy-exp-firelaser-member-seam-{probe,layout}.json`; pinned VC7.1 `/GL /GS` diagnostic |
| ENEMY-041 | observed / difficulty-count-call-topology | Target dispatcher opcode `0x1B4` is a 151-byte physical group with two distinct difficulty-selection trees. After the initial direct generic `ReadInt(0)`, each tree tests difficulty 0/1/2, pushes one immediate operand index (1/2/3/4 for the first count, 5/6/7/8 for the second), and joins one local generic `EclVmContext::ReadInt` call; the second join destructively changes EBX from the runtime tail to the full Enemy owner before loading active context. The retained source instead selects an index in a register and its second generic call is physically shared outside the 0x1B4 group, yielding 140/151 bytes. Focused source-shape probes bracket the target but do not reproduce it: first-tree branch-specific calls give 146/151 with 663/11,556 whole-owner agreement; a first-tree switch gives 143/151 and 680/11,556; an explicit first-tree context local gives 128/151; direct calls in both trees give 214/151; other second-tree context/branch variants give 171, 177, 208 and 226 bytes. All preserve the 181-byte selector and target physical group order but worsen the retained 731/11,556 whole-owner frontier, so all source variants are reverted. The missing five bytes in the closest local probe must not be treated as padding: the remaining mismatch is tied to common-call placement and whole-function register allocation. | Hash-attested target `0x0041054A-0x004105E0`; target/candidate x86 disassembly; `enemy-exp-1b4-{first-direct-only,first-switch,first-shared-context,inline-helper,second-context,second-branch-only,first-branch-second-context,second-shared-context-branches,direct-branches,fourway}-{probe,layout}.json`; pinned VC7.1 `/GL /GS` diagnostics |
| ENEMY-047 | observed / exact-laser-polar-helper | Target 0x0041F800 is the 30-byte polar-to-Cartesian member helper directly used by laser caller 0x0041CFD0. The maintained EnemyLaserVectorView::FromAngleMagnitude in src/EnemyLaser.cpp emits the same FSINCOS/multiply/store sequence. Canonical linked-PE comparison is raw 30/30 equal with no relocation fields, and two independent cold replays both pass. This establishes the first exact source owner in the missing FireLaser support graph; it does not by itself make EnemyFireLaser or the giant dispatcher exact. | target disassembly; canonical enemy-laser-vector-from-angle-magnitude linked-PE unit; two cold replay-exact-units.py passes |
| ENEMY-051 | observed / complete-firelaser-caller-graph | The two remaining external EnemyFireLaser caller owners are now source-present from TH10-local evidence: type-1 box collision at 0x0041EB00-0x0041EF92 and type-1 circle collision at 0x0041EFA0-0x0041F3F3. Both scan a 256-byte hit bitmap in 12-unit steps, retain the leading unhit prefix in the current laser, then respawn later unhit gaps through freshly zeroed 0x77-dword type-0 requests populated from type-1 owner fields at +0x24/+0x3C/+0x40/+0x44/+0x444/+0x464/+0x466, including speed 8.0f. This closes source coverage for all ten target EnemyFireLaser call sites across six owner functions. A first graph containing only the six real owners still emits central EnemyFireLaser as 154/153 with 29/137 normalized agreement and the dispatcher as 14,320/14,416 with 699/11,556, so missing caller count/source coverage is ruled out as the cause of the remaining private ABI mismatch. Free-function parameter permutations are ABI-identical; member-method variants recover RET 4 but leave type in EAX and request on the stack. A switch(type) central variant reproduces the target sub/dec control shape and forces type to the stack, but yields EDI=manager/ESI=laser/stack-request instead of target ESI=manager/EDI=request/EBX=laser. Target Type0/Type1 constructors at 0x0041C5B0/0x0041C680 keep object in EBX and mask -2 in ESI, while current diagnostic constructors keep object in EDX and mask in EBX, identifying constructor private ABI/lifetime as the next register-coloring seam. The canonical 30-byte laser polar helper remains exact after both type-1 source additions. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-052 | observed / exact | `0x0044C5D0` is a 30-byte polar-to-XY helper called from `EnemyRuntimeUpdate @ 0x0040DC80`. Its maintained view is in `src/Enemy.cpp` alongside that runtime owner and replays all relocation-free bytes through the canonical linked-PE Oracle. The local view and TU names are organizational descriptions; original ownership remains unknown. | Hash-attested target extent and caller; canonical `enemy-runtime-vector` cold replay; pinned VC7.1 SP1 build6030 |
| ENEMY-053 | observed / exact | `0x0041F7A0` is a complete 91-byte bounds helper whose target bytes are identical to `0x00428D70`; both use the same four playfield float references. Two direct callers are the maintained type-0/type-1 laser collision owners at `0x0041D880` and `0x0041EB00`; the third caller `0x0041D3D0` remains unassigned. The local `EnemyLaserVectorView::IsOutsidePlayfield` method lives in `src/EnemyLaser.cpp` with those two callers and reproduces the whole target extent plus four DIR32 fields through two independent canonical linked-PE builds. The view/TU names are descriptive; original class and production ownership remain unknown. | Hash-attested target bytes and callers; canonical `enemy-laser-playfield-bounds` and `enemy-laser-vector-from-angle-magnitude` replay; pinned VC7.1 SP1 build6030 |
| ENEMY-054 | observed / canonical-linked-exact | Target `0x00412A10-0x00412A5A` is the 75-byte `EnemyRuntimeView::ResolveIntArgument` extent; it resolves local operands and delegates negative operands through the ECL host resolver. Two cold canonical linked-PE replays of `enemy-resolve-int-argument`, under the real `DispatchEclInstruction` entry with ECL/ANM support sources, produce one complete 75-byte PDB-owned contribution, fully decoded, raw-equal, and without linked fields. Exactness is scoped to that declared compiler context; original source-versus-outlined origin and production TU remain unknown. | Hash-attested target bytes, extent, and callers; two canonical `enemy-resolve-int-argument` cold replays; pinned VC7.1 SP1 build6030 |
| ENEMY-055 | observed / canonical-linked-exact | EnemyBulletPositionView::IsOutsidePlayfield at 0x004086B0 is the 91-byte bounds helper used by the TH10 bullet-cancel pool. Both direct callers, 0x00408030 and 0x00408100, operate on the same 2000-row pool with 0x7F0-byte records and position at row +0x3B4; maintained EnemyCancelBulletRecord and EnemyCancelManagerView::CancelBulletPattern already express those target-local layouts in src/EnemyEclDispatcher.cpp. The target 91 bytes are byte-identical to exact laser sibling 0x0041F7A0. The original natural bullet spelling compiled to the right 91-byte extent and all four correct DIR32 fields but differed in six floating-comparison branch bytes; using the same negated-boundary source shape independently proven by the laser sibling makes the normal COFF body structural-exact (75/75 comparable bytes plus four matching relocations). A canonical /GL /GS linked-PE unit with real CancelBulletPattern entry then reproduces the complete 91-byte PDB contribution and all four fields exactly. Focused replay of all seven canonical units sourced from EnemyEclDispatcher.cpp passes 627/627 bytes after the change. | Hash-attested TH10 target bytes/callers; maintained TH10 bullet-pool source; exact target sibling 0x0041F7A0; canonical enemy-bullet-playfield-bounds replay; seven-unit EnemyEclDispatcher focused replay; pinned VC7.1 SP1 build6030 |
| ENEMY-056 | observed / canonical-coff-exact / mixed-origin | Five small `src/EnemyEclDispatcher.cpp` helpers close as relocation-free normal-COFF exact bodies: `EnemySetLaserPosition @ 0x0040CF60` (23B), `EnemyFindLaser @ 0x0040CF90` (22B), `EnemyConfigureInterrupt @ 0x00412720` (48B), `EnemyWaitForEffect @ 0x0041C7D0` (44B), and `EnemyApplyBulletClear @ 0x0041C850` (44B). Their maintained source already expresses target-local laser position/list traversal, interrupt-record writes, effect wait flagging and bullet-clear traversal. Pinned VC7.1 /O2 /Gy gives one independent COMDAT per helper, with target-sized contributions and no relocations. Codegen exactness does not collapse provenance: the laser-position, laser-find and interrupt helpers remain origin-indeterminate because their short internal boundaries may be source-written or optimizer-retained; the two effect-list helpers are independently authored-game/Enemy by dispatcher-owned semantics. | Maintained source and target bodies/call paths; canonical `enemy-{set-laser-position,find-laser,configure-interrupt,wait-for-effect,apply-bullet-clear}` normal-COFF replays; pinned VC7.1 SP1 build6030 |
| ENEMY-057 | observed / canonical-linked-exact | `EnemyAngleFromPlayer @ 0x00426660` is the 67-byte reverse-direction sibling of the independently exact Player angle helper, but its ownership is established directly rather than by sibling analogy. Player construction at 0x004247AA stores the new Player to target global `0x00477834`. Five target callers load that Player into ECX and place a point in EAX; three calls at 0x004123C5/0x00412519/0x0041252D are inside source-present `EnemyFullObjectView::ReadFloatOperand`, whose maintained cases already call `EnemyAngleFromPlayer(g_Player, &runtime.worldMotion/offsetMotion/baseMotion.position)`. The target subtracts point X/Y from Player drawPosition +0x3C0/+0x3C4, compares dy then dx against shared 0.0f at 0x00470B04, returns π/2 from 0x00470B94 for the zero vector, and otherwise executes `FXCH; FPATAN`. A natural maintained implementation with scalar dx/dy and `atan2(dy, dx)` lets VC7.1 /GL recover the private EAX-point / ECX-Player entry. Canonical linked replay reproduces all 67 bytes and three DIR32 fields zero-difference. Appending the implementation at the TU tail preserves every existing Enemy exact body; VC7.1 only renumbers the offset-0x11 compiler-local relocation symbols in the two 80-byte lvalue resolvers (`$L4106` -> `$L4115` and `$L4146` -> `$L4118`) while keeping relocation offsets/types/targets unchanged. The full current `src/Enemy.cpp` canonical replay passes 26 units / 1000 bytes. | Hash-attested TH10 Player constructor/global, caller setup and helper body; maintained Enemy operand-reader source; canonical `enemy-angle-from-player` replay; pinned VC7.1 SP1 build6030 |
| ENEMY-058 | observed / canonical-linked-exact | EnemyEnableBombShield @ 0x0040CEA0 is the complete 36-byte helper reached only by the ENEMY_ECL_ENABLE_BOMB_SHIELD dispatcher case. Target setup loads g_EnemyGameState @ 0x004776F4 into ESI; the helper directly ORs bit 0x10 into game-state +0x378C, passes the four-byte managed VM id at +0x774 to AnmRenderManagerView::MarkVmForDeletion, then clears the id. Re-auditing the maintained helper exposed two source-shape issues rather than missing semantics: ordinary implicit conversion of the integer id to AnmVmIdView creates a temporary and a 48-byte linked contribution, while viewing the established four-byte id storage directly as AnmVmIdView removes that temporary and yields 44 bytes; pinned VC7.1 probes further show that only the natural address-taken spelling unsigned int *flags = &gameState->flags378C; *flags |= 0x10; emits the target direct memory-OR rather than a load/OR/store sequence. In the real dispatcher /GL /GS graph these two evidence-backed spellings produce the exact 36-byte contribution with only the manager DIR32 and MarkVmForDeletion REL32 fields to replay. Making the maintained noinline helper externally visible leaves its 36-byte body unchanged and exists only to give the canonical PDB comparator a public symbol; it does not claim original source linkage. A focused replay passes all 14 canonical exact EnemyEclDispatcher.cpp units after the change. | Hash-attested target helper and unique dispatcher callsite; fixed VC7.1 source-shape probes for id temporary and memory OR; canonical enemy-enable-shield linked replay; 14/14 focused exact source replay; pinned VC7.1 SP1 build6030 |
| ENEMY-067 | observed / canonical-linked-exact / source-shape-correction | Direct IDA Pro MCP confirms the target's signed modulo-four alignment sequence, four-byte ECLI-loop alignment sled, and failure path calling `GameErrorContext::Log @ 0x0044B810` with format data at `0x0046CB68` and receiver `0x00474F70`. A natural repeated `% 4` source expression plus a recovered TU-level error-format string now reproduces the full `EnemyEclResourceView::LoadPackage @ 0x0040D400` extent. The canonical linked-PE unit cold-replayed twice with six explicit DIR32/REL32 fields and compared all 239 bytes zero-difference; exactness does not assert the original TU spelling or production owner. | Direct IDA Pro MCP target disassembly/decompilation/bytes; canonical `enemy-ecl-resource-load-package` replay; pinned VC7.1 SP1 build6030 |
| ENEMY-076 | observed / spell-start-owned-switch-table / source-absent | Target 0x00409280's former 2,403-byte ledger extent ended at 0x00409BE2 after RET 0x10, but omitted one NOP at 0x00409BE3 and its seven-entry absolute table at 0x00409BE4-0x00409BFF. Complete executable decode shows the global 0x00474C7C loaded into EAX, DEC EAX, CMP EAX,6, unsigned JA to the common return, then JMP [EAX*4+0x00409BE4] at 0x0040995D. The seven destinations are 0x00409964, 0x004099CB, 0x00409A16, 0x00409A61, 0x00409AAC, 0x00409AF7 and 0x00409B3F; all are internal instruction boundaries. Direct read-only Ghidra confirms the table's DATA xref from that jump, no alignment-byte xref, and the independent next function at 0x00409C00, called by the dispatcher's spell-end handler. Both ledgers now own the complete 2,432-byte span through 0x00409BFF. Fresh boundary inventory removes the 29-byte unresolved gap and changes this table from unowned to owned; no other recorded boundary is changed. Its regenerated global check already failed before the edit on historical manual evidence text at 0x00401100, so unrelated rows are preserved. This correction adds 29 authored physical-owner bytes to the denominator and no canonical exact bytes. EnemyBeginSpell remains declaration-only; Ghidra's provisional decompilation loses private call arguments (notably QueueSoundSample's EDI sound id 14), so reconstruct its ANM/resource/stage setup from target ABI and assembly rather than copying that output. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-079 | observed / spell-stage-chapter-correction / non-exact | An earlier maintained spell source mistakenly bound the bonus stage and stage selector to chapter +0x44; its structural diagnostic masked absolute addresses and did not prove scalar bindings. Complete target ECL opcode 0x158 supplies EAX=0x00474C40 to EnemySetChapter @ 0x00412ED0, whose 16-byte body writes receiver+0x44 and conditionally resets +0x4C. Spell bonus @ 0x00409874 and selector @ 0x0040994E read 0x00474C7C (receiver+0x3C); stage-7 comparison @ 0x00409B3F reads 0x00474C84 (the same chapter field) with threshold 24. The partial view now has distinct stage/chapter fields and offset assertions, and the redundant independent extra-counter alias is removed. report-enemy-spell-state.py verifies the hash-attested target-only address relationships independently of source names. Four-byte non-runnable link anchors can collide when large view offsets are added; candidate bindings are therefore checked by their bonus arithmetic, table guard and chapter comparison roles, not absolute candidate address alone. These observations establish scalar/address relationships, not the original class, full layout, data owner, exactness or runtime behavior. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-085 | observed / rank-storage-and-signed-quotients / non-exact | Fresh seven-TU Enemy-rooted /GL /GS baseline at 47e6a85 reproduces 14384/14416 bytes, frame 0x2BC/0x2C4 and 181 selectors/108 groups. Rank is now represented as +0x58 in the existing partial chapter-state view rooted at target 0x00474C40, matching rank address 0x00474C98 and restoring target-observed reloads after the first field write in both interpolated bullet-speed and bullet-count cases. This is a target storage/access constraint, not recovered original object ownership. Natural signed /2048 replaces three equivalent signed-bias shifts. The selected owner is 14416/14416 with unchanged frame and selector/group order; size agreement alone is not exactness. Opcode 0x162 keeps its 97-byte extent and improves 54/77 to 72/77 ordinary-byte agreement; its five residual bytes are two ReadIntArgument register-ABI pairs and the stack-cookie offset. Cases 0x1A8 and 0x1AB become 203/206 and 204/206 bytes. Whole-owner structural diagnostic is 748/11528, case-aligned 2491/11123, complete address normalization; changed link-field masks prevent treating these scores as canonical byte gains. Swapping the float multiplication operands is a neutral control and is not retained. All 14 affected canonical units cold-replay exact at 860/860 bytes across three artifacts. Full owner remains non-exact, exact ledger unchanged; private ABIs, x87 homes and branch layout remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-086 | observed / corrected-movement-sentinels-and-angle-wrap / non-exact | Raw target review supersedes the zero-threshold assumption in the prior movement source and the unretained circle-ordered-positive trial. Exactly 19 dispatcher FCOM/FCOMP references use qword 0x470D58, bits C12E847E00000000, value -999999.0; they cover 20 source conditions because polar mode 7 and normal angle paths share one compare. TEST AH,0x41 selects the supplied argument only for ordered values strictly above this sentinel; below/equal/unordered select the prior motion value, or genuine +0.0 in mode 7. All 20 guards are corrected without changing those zero assignments. Polar shortest-path adjustment compares against binary32 pi at 0x470B18, then FADD at 0x40F5A1/0x40F5B1 uses +2pi from 0x470B14, bits 40C90FDB; two incorrect -pi additions become +6.2831855f. Pi/2 reflection constants stay unchanged. Independent complete movement-literal and final-diff reviews pass. Fresh seven-TU /GL /GS graph remains 14416/14416 bytes and frame 0x2BC/0x2C4, with 181 selectors/108 groups in target order. Candidate pool bytes independently verify the corrected literals; it still emits 20 sentinel comparisons versus target 19, retaining a polar CFG opportunity. Structural scores are unchanged by the literal corrections because address masking does not validate referent values; normalization remains incomplete and grants no exactness. All 14 affected canonical units cold-replay 860/860 bytes in three artifact batches. No exact ledger change, accepted new owner claim, native product or runtime validation follows. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-087 | observed / random-range-literal-repair-and-natural-controls / non-exact | Target MOVE_RANDOM 0x124/0x125 at 0x40FB1E and 0x40FB24 multiplies by binary32 pi (0x470B18, bits 40490FDB) then 0.5 (0x470B0C, bits 3F000000); the source final factor 2.0f was wrong and is corrected to 0.5f. Independent re-review confirms only that line changes after ENEMY-086. The complete 13760-byte pretable decode and pooled-literal audit finds no other concrete value/width mismatch among the 18 target references outside the movement cluster; nonzero float-like MOV/PUSH sequences also agree by case, including actual 8.0f at 0x41114F. This bounded audit is not semantic or exact completion. Selected seven-TU graph still emits 14416 bytes, frame 0x2BC versus target 0x2C4; the compiler folds pi/2 and pi/3 products while target retains two multiplies. Four isolated source hypotheses are restored: a shared polar bool emits unwanted Boolean materialization (395/405-byte case), normal-first polar branches remain 396/405, and case-local circle pair representations with either float or raw-bit rate snapshots emit owner14448/frame2CC and circle531/525. A separate /Op compiler-context control emits owner14692/frame2CC and still folds RNG multiplications; it is not retained. These are natural CFG/lifetime/profile controls, with no padding or ABI intervention and no exact credit. All 14 affected canonical units freshly cold-replay 860/860 across three artifacts after the literal correction. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ENEMY-088 | observed / canonical-linked-exact / local-table-tail | `EnemySpawn @ 0x0040CFB0` now has a canonical linked-PE unit under the actual stdcall-decorated LTCG symbol. Two independent pinned VC7.1 SP1 build6030 replays reproduce the complete 577-byte PDB contribution with zero differences after replaying five external fields, six DIR32 references to compiler-owned jump-table blocks within the same contribution, and the explicit one-byte selector-table tail left undecoded by Capstone. The target physical extent includes the RET/alignment sled and both switch tables; no bytes are omitted from the exact claim. | Hash-attested target `0x0040CFB0-0x0040D1F0`; IDA target disassembly/decompile and table xrefs; canonical `enemy-spawn` replay twice; pinned VC7.1 SP1 build6030 |

## Bullets

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| BULLET-001 | observed / source-present / partition-corrected | `BulletRuntimeView::AdvanceTransformProgram @ 0x00406D90` corrects the prior ANM-VM interpretation. TH10 walks at most 18 0x18-byte records at runtime +0x464, dispatches bit-valued transform kinds, mutates active flags +0x43C and cursor +0x45C, and operates nine 0x34-byte extension-state slots from +0x614. The recovered cases cover skip, deceleration, vector/polar acceleration, direction changes, bounce variants, cancel behavior, cull delay, sprite change, wait, despawn, sound, X/Y wrap, child-pattern spawn, owner tag, jump and the TH10 state-8 transform. The maintained owner now lives in `src/BulletTransform.cpp` with its private angle resolver. Earlier 1,520/1,548 all-/GL transform-body diagnostics remain useful historical codegen measurements but are not the recovered production partition; caller evidence requires this TU to be supplied outside runtime WPO. | Hash-attested TH10 decompilation/disassembly and callers; maintained Bullet runtime layout; mixed-profile partition probe; pinned VC7.1 SP1 |
| BULLET-002 | observed / canonical-linked-exact / owner-seam-correction | Re-audit of the Bullet calc chain corrected the prior `0x0040678E` standalone-boundary assumption: raw target decode shows a single 40-byte callback owner at `0x00406770-0x00406797` with `0x0040678E` only an interior tail entry. That callback is canonical exact and provides the real `/GL /GS` entry needed to reconstruct `BulletManagerView::UpdateBullets @ 0x004065C0`. Keeping `BulletUpdateRuntime @ 0x00406240` in a separate maintained `src/BulletRuntime.cpp` prevents whole-program optimization from erasing the retail cross-TU stack-call seam. With the target-proven six-head/six-tail clear order, three-part update gate, repeated draw-bucket-index load, and natural timer logic written as the positive `0.99 < scale < 1.01` branch plus an ordered volatile subframe read on the scaled path, pinned VC7.1 emits the complete 249-byte manager owner. Canonical linked-PE comparison is 249/249 with six declared linkage fields; the already-exact 164-byte deceleration helper remains exact under the same timer source. This exact manager result does not imply the separate 885-byte BulletUpdateRuntime owner is exact. | Hash-attested target callback/manager disassembly; canonical `bullet-manager-update-callback`, `bullet-manager-update`, and `bullet-update-deceleration` replays; pinned VC7.1 SP1 build6030 |
| BULLET-004 | observed / canonical-linked-exact / wrap-bounds-seam | BulletWrapPositionView::IsOutsidePlayfield @ 0x004061D0 is the distinct bounds helper called only by horizontal/vertical wrap owners 0x00407DA0/0x00407E40. TH10 uses the same lazy x87 half-width/half-height control-flow shape as exact culling sibling 0x00406160, but its Y interval is visible playfield [0,448] rather than [-64,448]. Natural maintained source reproduces the full 109-byte owner in the Bullet manager /GL /GS graph; canonical comparison is exact with six declared DIR32 fields covering 0.5f, left/right, zero-float top and bottom constants. | Hash-attested TH10 decompile/disassembly and caller set; canonical bullet-wrap-position-bounds replay; pinned VC7.1 SP1 build6030 |
| BULLET-005 | observed / canonical-normal-coff-exact / state8-angle-difference | TH10 helper 0x00408660 is an 80-byte callee-clean float function used only by the transform-state owner at 0x00407EF0. Target code computes angle-reference, returns angle-(reference+2*pi) when the difference exceeds +pi, returns angle-(reference-2*pi) when the reverse difference exceeds +pi, and otherwise returns the direct difference. Natural maintained source in src/BulletManager.cpp compiles under pinned VC7.1 to an 80-byte normal-COFF COMDAT with 64/64 comparable bytes and four DIR32 constant fields exactly at offsets 0x0A/0x1D/0x32/0x45, resolving to pi at 0x00470B18 and 2*pi at 0x00470B14. Canonical comparison is exact. | Hash-attested TH10 0x00408660 and sole caller 0x00407EF0; canonical bullet-angle-difference replay; pinned VC7.1 SP1 build6030 |
| BULLET-006 | observed / canonical-linked-exact / boundary-bounce | `BulletRuntimeView::UpdateBoundaryBounce @ 0x00407BE0` is a 446-byte private-ESI owner driven by `exStates[4]`. Target disassembly distinguishes the outer activation bounds (`x <= left`, `y <= 0`) from the strict inner reflection tests. Linking the maintained source with the already-exact `EnemySoundQueueView::QueueSoundSample` recovers the retail ECX-owner/EDI-sound-id/private call shape; the candidate changes from 445 bytes to target-sized 446 bytes. Canonical linked-PE replay then matches all 446 bytes with 20 declared linkage fields. | Hash-attested TH10 target 0x00407BE0; exact QueueSoundSample support; canonical bullet-update-boundary-bounce replay; pinned VC7.1 SP1 /GL /GS |
| BULLET-009 | observed / canonical-linked-exact / corrected-caller-checkpoint | `BulletRuntimeView::UpdateState8 @ 0x00407EF0` is the 306-byte private-ESI transform owner called only from `BulletUpdateRuntime`. TH10 proves exStates[8], g_Player draw-position aiming with the zero-vector pi/2 case, AddNormalizeAngle(value1, aimed), canonical-exact BulletAngleDifference, angle-delta scaling by state.value0 and g_AnmGameSpeed, a second AddNormalizeAngle, exact polar-vector rebuild, bit 0x04000000 expiry clear and the timer tick. Re-audit found two source-shape details hidden by the first semantic reconstruction: retail branches expiry-first, and the timer slow path loads `*scale` before adding subframe. With `*scale + subframe` and no volatile, canonical linked-PE replay reproduces all 306 bytes with twelve linkage fields. The earlier handoff statement that reconnecting State8 left BulletUpdateRuntime at 885/885 was a report-reading error: the structured real-caller result was 887/885 with 86/729 normalized comparable bytes (88 raw). | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| BULLET-010 | observed / canonical-linked-exact / direction-cfg | `BulletRuntimeView::UpdateRelativeDirectionChange @ 0x00407780` is the 338-byte private-ESI transform owner using exStates[3]. Target disassembly proves an expiry-first physical CFG, int2>=int1 comparison ordering, speed-first value0 spill lifetime, duplicated-speed x87 interpolation, exact sound queue and polar-vector calls, and the inline timer slow path as `*scale + subframe`. Canonical linked-PE replay reproduces all 338 bytes with eight linkage fields. The same source-shape work moves absolute direction to 335/335; an integer-bit lifetime for value1 plus a separate new-speed lifetime further narrows it to 299/303 normalized comparable bytes and 331/335 total linked bytes, with only four register-encoding differences at +0xD4/+0xD7/+0xDC/+0xDD (candidate EAX/ECX versus target ECX/EDX). Replacing that new-speed lifetime with the semantically equivalent `nextSpeed = speed` form regresses the real `BulletManagerUpdateCallback` graph to 337 bytes and 101/303 comparable bytes, so it was reverted. Real BulletUpdateRuntime-entry and pointer/reference/local argument variants retain that coloring. Aimed direction is 431/431 with 366/383; both remain non-exact. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| BULLET-012 | observed / canonical-linked-exact / aimed-direction | `BulletRuntimeView::UpdateAimedDirectionChange @ 0x00407A30` is the 431-byte private-ESI exStates[3] direction transform. After expiry-first CFG and speed-first spill recovery, the only remaining mismatch was the unexpired speed interpolation. Spelling it as `nextSpeed = speed; nextSpeed -= (nextSpeed * subframe) / int0` produces the target x87 duplication/lifetime and canonical linked-PE replay matches all 431 bytes with twelve declared fields. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| BULLET-013 | observed / production-partition recovered | `BulletUpdateRuntime @ 0x00406240` is highly sensitive to WPO visibility of `BulletRuntimeView::AdvanceTransformProgram @ 0x00406D90`. The earlier hidden-definition experiment raised the runtime from 228/729 to 498/729 normalized bytes but could not prove a real link boundary. The maintained reconstruction now moves the transform owner plus `BulletResolveTransformAngle` into `src/BulletTransform.cpp`, extends the LTCG harness with per-support `--normal-support`, and links that TU without /GL while `BulletRuntime.cpp`, `BulletManager.cpp`, ANM and Enemy remain /GL /GS. This real resolved mixed-profile graph emits a 887/885 runtime contribution with 498/729 normalized comparable bytes (532 raw), complete normalization. Compiling the whole BulletManager without /GL instead produces 862/885 and only 84/733, proving the partition is transform-specific rather than subsystem-wide. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| BULLET-015 | observed / canonical-linked-exact / runtime-and-collision | A previous noncanonical bullet diagnostic is superseded. TH10 target xrefs prove BulletUpdateRuntime and EnemyRuntimeUpdate both call 0x004266B0; the maintained callers now share one `PlayerCollisionPositionView::CheckPlayerCollision`. Spelling hit/graze bounds as two two-dimensional min/max values gives the pinned VC7.1 mixed-profile graph the target 0x10 collision stack frame and ECX=position / EDX=Player / EAX=size private ABI. Canonical linked-PE replay reproduces the complete 885-byte BulletUpdateRuntime contribution with 39 declared link fields and the complete 316-byte collision contribution with eight declared fields, both zero-difference in one cold build. The collision view name and maintained TU are descriptive; neither exactness claim closes the product build. | Hash-attested TH10 0x00406240/0x004266B0; Factory-native xrefs and target bytes; canonical `bullet-runtime-update` and `player-collision-check` linked-PE units; pinned VC7.1 SP1 mixed /GL profile |
| BULLET-016 | observed / canonical-linked-exact-repair / timer-operand-order | Focused protection after an earlier Enemy source checkpoint exposes two pre-existing shared-Tick failures: aimed direction at 424/431 and BulletManagerView::UpdateBullets at 247/249. The target slow paths load subframe first: aimed +0x194 uses FLD [ESI+0x6B8], FADD [ECX]; manager +0xC6 uses FLD [EDI], FADD [ECX]. Current shared Tick emits the reversed x87 operand order, differing at seven and two ordinary bytes respectively. Cold controls using the prior Enemy or Bullet Git blob reproduce the same differences, so neither is introduced by the scalar-view change. Direct natural aimed timer expansion and a plain nextSubframe local remain 424/431. An ordered read of the actual subframe numeric input, scoped to these two owners' scaled paths, restores the complete 431-byte and 249-byte canonical contributions with their twelve and six existing linkage fields in first cold builds. The shared timer header and scale-first relative-direction sibling remain unchanged. The ordered reads are reconstruction constraints; original volatile qualification/source factoring remain unknown. Two independent cold builds for each repaired owner pass against the final source. Focused protection covers 31 distinct existing units across eleven artifacts, 4,676/4,676 bytes; eighteen unaffected cold-prefix units are retained and thirteen Bullet-dependent units rebuilt. Unit IDs, ABIs, profiles and normalization declarations are preserved; these repairs add no canonical bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| BULLET-017 | observed / register-coloring-negative | `BulletRuntimeView::UpdateAbsoluteDirectionChange @ 0x004078E0` remains target-sized at 335 bytes in its cold `/GL /GS` callback graph. Replacing float temporaries with natural integer-bit lifetimes raises normalized agreement to 301/303 comparable bytes: the new speed reaches target ECX, while the angle remains EAX instead of target EDX, leaving only register encodings at +0xD7/+0xDD. Reversing declaration order regresses to 299/303, so all variants were reverted. | Hash-attested target disassembly through Factory-native Ghidra; pinned VC7.1 SP1 linked diagnostics |
| BULLET-018 | observed / local-canonical-linked-exact | UpdatePolarAcceleration at 0x004076A0 owns 209 bytes with private ESI receiver and RET. The expired-duration clear precedes the active update physically; the scaled timer path loads scale before adding subframe. A narrow volatile-qualified scale read preserves this observed order without declaring original storage volatile. Target FST at 0x0040775E stores subframe but retains the x87 sum for __ftol2; no forced float reload or rounding is added. Two cold seven-TU /GL /GS canonical replays cover the full owner and eight reviewed fields with zero differences. VerticalWrap remains unchanged and non-exact: a duplicated-wrap-branch control worsened 172 to 166 bytes and was reverted. | Target-attested independent review; literal float encodings; unit bullet-update-polar-acceleration; two focused canonical cold replays on 2026-10-14 |

## Player

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| PLAYER-001 | observed | `0x00426F70-0x0042792D` is the reviewed 2,494-byte executable body of the player option-rebuild owner (PLAYER-029 extends complete ownership to 2,520 bytes through `0x00427947`) mapped descriptively as `RebuildPlayerOptions`. Nine direct call sites across five caller functions push the same player/runtime owner; the callee reads one stack argument and returns with `ret 4`. Ghidra reports 2,471 reachable body bytes because three retained unreachable alignment sleds at `0x0042701A-0x0042701F` (6 bytes), `0x00427197-0x0042719F` (9 bytes), and `0x004278D8-0x004278DF` (8 bytes) account exactly for the 23-byte difference from the physical extent. TH10 retains `.\\src\\game\\player.cpp:193 PlayerInf`, but the original function identifier, TU ownership, and source calling convention remain unproven. | Attested TH10 function/caller/xref/decompile results; raw entry/epilogue and all nine call-site disassemblies; target player source/type string |
| PLAYER-002 | observed / corroborated | The reviewed option-rebuild owner establishes four 0x98-byte option records beginning at player `+0x32A0`, not at replay snapshot offset `+0x32D4`. Target-observed option fields include state `+0x00`, four 8-byte position pairs at `+0x34/+0x3C/+0x44/+0x4C`, primary/secondary VM ids `+0x68/+0x6C`, option index `+0x88`, reset flag `+0x8C`, and callback `+0x90`; option count is player `+0x3500`. Player position is `+0x3CC/+0x3D0`, option resource data is reached through `+0x45C`, a 33-pair/0x108-byte position history occupies `+0x436C-0x4473`, and mode is `+0x4474`. The body rebuilds secondary VMs, derives count from signed power divided by 20 and clamps it to four, recreates active options according to character/shot data, retires inactive VM state, writes the count, and sets all reset flags. Committed TH08 Player source corroborates a four-option Player subsystem only after these TH10-local facts were recovered; its different option layout is not transferred. | TH10 raw/decompiled player rebuild body and replay-stage copy seam; committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` used only for source-family corroboration |
| PLAYER-003 | observed / canonical-linked-exact / unknown-origin | `0x00427AD0-0x00427ADB` is a tracked 12-byte CC-delimited retained callback wrapper missed by the original Ghidra denominator. The player rebuild stores its address at `0x00427372`; the wrapper preserves EDI, moves live-in ECX to EDI, calls the separate `0x00427AE0` candidate, returns zero, and uses plain RET. It is preceded by CC padding after the `0x00427960-0x00427AC6` body and followed by `0x00427ADC-0x00427ADF` CC padding. Maintained source presence is recorded for the callback entry (see `PLAYER-004`/`PLAYER-005`). Two independent canonical linked-PE builds reproduce the complete wrapper and its REL32 to `0x00427AE0`; source-written adapter versus compiler/optimizer wrapper origin remains unresolved. | Direct TH10 raw callback-corridor disassembly; attested `xrefs_to` for `0x00427AD0` |
| PLAYER-004 | observed / unknown-origin | The option record has two distinct callback-like tail slots. In the player movement/update owner, `0x004255CC` forms ECX as the option record base, `0x00425600` loads the function pointer at option `+0x90`, and `0x00425607` calls it. Separately, `0x004263B3-0x004263D5` iterates four 0x98-byte records, loads a pointer at option `+0x94`, and also calls it with ECX equal to the record base. Therefore the rebuild writes of `0x00427950`/`0x00427AD0` at player `+0x3330` are correctly option `+0x90`; the `+0x94` slot is a different callback. Later TH10-local draw-chain evidence identifies its functional role as the per-option draw callback, while its producer/assignment remains unidentified. `0x00427950` and `0x00427AD0` are source-present callback entries with canonical-exact 12-byte wrappers; their source-written-versus-compiler-adapter origin remains `unknown/review`. The special body at `0x00427AE0` is now canonical exact, while the trail body at `0x00427960` remains an exactness candidate. | Direct TH10 disassembly at `0x004255B9-0x00425609`, `0x004263B3-0x004263D5`, `0x00427372`, and `0x004275D9`; attested xrefs/call graph |
| PLAYER-005 | observed | The `+0x90` callback cohort establishes option `+0x84` as a prior/current option-mode field. The trail callback entry `0x00427950` adapts ECX to an ESI-bound body at `0x00427960`; that body uses option `+0x88` to maintain the player's eight-sample-per-option history, derives/stores replay pair `+0x44` when mode is zero, reconstructs seven intermediate samples when mode is nonzero using the target `0.125f` constant at `0x00470B90`, then refreshes pair `+0x34` and stores the current mode at `+0x84`. Its body remains an exactness candidate. The special callback entry `0x00427AD0` adapts ECX to an EDI-bound body at `0x00427AE0`; on mode transitions it calls `AnmVmIdView::SetInterrupt @ 0x00449470` with interrupt 6 or 3 on primary VM id `+0x68`, copies pair `+0x3C` to `+0x4C` when returning to mode zero, or pair `+0x4C` to `+0x34` when entering nonzero mode, and updates `+0x84`. The special body is now canonical exact under its declared linked context. | Attested TH10 decompile/disassembly/callees for `0x00427950`, `0x00427960`, `0x00427AD0`, `0x00427AE0`, and `0x00449470`; direct target constant bytes; two canonical `player-option-special-body` replays |
| PLAYER-006 | observed / unknown-origin | Player initialization registers two CC-delimited callback entries through the shared 0x24-byte callback-node manager. `0x00426500-0x00426506` is stored in the first list (`0x00449AE0`, node retained at Player `+0x08`) and adapts live-in ECX to the large `0x00425730` Player update owner by pushing the Player pointer. `0x00426510-0x00426516` is stored in the second list (`0x00449B70`, node retained at Player `+0x0C`), moves ECX to EAX, and tail-jumps to `0x00426360`. The main frame owner executes the paired first-list runner `0x00449C00` before rendering and the second-list runner `0x00449D40` during the render path, establishing maintained update/draw roles without proving original identifiers or source-written adapter ownership. Both 7-byte entries were absent from the Ghidra denominator and are retained as `unknown/review`. | Direct TH10 initialization disassembly at `0x00424856-0x004248A8`; raw callback corridor `0x004264F9-0x0042651F`; attested callback-manager decompilation/xrefs and frame owner `0x00439390` |
| PLAYER-007 | observed / corroborated | `0x00426360-0x004264F8` is the 409-byte EAX-bound body behind the registered Player draw entry `0x00426510`. When Player `+0x458 != 2`, it writes the embedded draw VM at Player `+0x14` positions `+0x340/+0x344/+0x348` from Player float coordinates `+0x3C0/+0x3C4/+0x3C8` with target constants `224.0f` and `16.0f`, calls the draw-VM owner, then iterates four 0x98-byte options from Player `+0x3334` and invokes each non-null option `+0x94` hook with ECX equal to the option base. A complete TH10 `.text` displacement scan found no Player-local `+0x94` producer, so the producer remains unknown; the maintained field name `drawCallback` is justified by the TH10 draw-chain execution context, not by adjacent transfer. Committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` independently has separate option update/render callback slots and is used only as later family corroboration. | Attested TH10 body/registration/callback-manager/frame analysis; full target `.text` `+0x94` displacement scan; committed clean TH08 source only after TH10-local role recovery |
| PLAYER-008 | observed / unknown-adapter-origin | The first Player callback lane registered during initialization resolves to a seven-byte CC-delimited entry at `0x00426500-0x00426506` and a separate 3,089-byte body at `0x00425730-0x00426340`. The entry receives Player in ECX, pushes that pointer, calls the body, and returns its result. The body independently reads exactly one stack Player pointer after its prologue and ends in `ret 4`; its bespoke Player power/state/option/effect/timer/shot logic establishes `authored_game` origin even though the source-written-versus-compiler/LTCG origin of the seven-byte adapter and the physical source ownership relation between the two chunks remain unknown. Five unreachable alignment sleds inside the body total exactly 29 bytes, explaining Ghidra's 3,060 reachable bytes versus the 3,089-byte physical extent. A separate three-byte alignment region and five-entry jump table follow the body before CC padding. | Direct TH10 callback registration and raw entry/tail disassembly; attested function/xref/decompile/callee analysis; full raw alignment-gap inventory |
| PLAYER-009 | observed / corroborated | The central Player update body is a five-state machine whose compiler jump table selects numeric states 0-4 while the physical case-block order is `0, 1, 4, 2, 3`; the state-4 block can transition and fall through into state 2. Target-local behavior includes respawn/history handling, power-triggered `RebuildPlayerOptions`, the connected movement/options owner, 32 repeated 0x6C effect rows at Player `+0x350C`, 0x14 timer-shaped records at `+0x460/+0x474/+0x488/+0x430C`, six derived Float3 rows at `+0x4324`, and the final connected input/shot updates. Maintained partial views expose only these observed offsets and operations; original type/member names remain unknown. Committed TH08 Player source was consulted only after this TH10 structure was recovered and independently corroborates that VC7 Player state-machine lexical block order and timer/source shape can be target-visible; no TH08 address, layout, state value, or exactness fact is transferred. | TH10 target state table, raw body instructions, attested helper decompilation, direct constants; committed clean TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` used only as later source-shape corroboration |
| PLAYER-010 | observed / unknown-optimizer-ABI | `0x004250B0-0x004256E6` is the 1,591-byte central Player movement/options body. Its sole TH10 caller at `0x00425A8C` first loads EDI with the Player owner; the callee then uses live-in EDI for Player storage and returns zero with plain RET. Ghidra's provisional fastcall parameter is therefore not adopted. The physical body contains one retained unreachable three-byte LEA sled at `0x0042558D-0x0042558F`, explaining 1,588 reachable bytes. After the body are a one-byte NOP, two separate 32-byte/eight-entry switch tables at `0x004256E8-0x00425707` and `0x00425708-0x00425727`, then CC padding through `0x0042572F`. Both tables map direction values 1-8 onto target physical case-block order `4, 3, 1, 2, 5, 7, 6, 8`. The body is `authored_game`; original source declaration/TU and compiler/LTCG private-ABI/table ownership remain unknown. | TH10 attested function/decompile/disassembly/xrefs; direct raw caller/tail/table bytes |
| PLAYER-011 | observed / corroborated | The movement owner establishes Player `+0x3D4/+0x3D8` as mode-0/mode-1 axis speeds and `+0x3DC/+0x3E0` as the corresponding diagonal speeds, not independent X/Y speed fields. Input bits select directions 0-8; fixed-point deltas at `+0x3F0/+0x3F4` are speed multiplied by target timer scale, fixed position `+0x3CC/+0x3D0` is clamped to `[-18400,18400] x [3200,43200]`, and float draw position `+0x3C0/+0x3C4` is fixed position times target `0.01f`. `+0x329C` is a managed-VM handle created only in nonzero option mode with target layer 9/script `0x160`/runtime flag, retired with delete state 1 in mode zero, and repositioned from the Player draw position. The four-option loop selects offset pair `+0x44` in mode zero or `+0x4C` in nonzero mode, forms target pair `+0x34`, invokes option `+0x90` with ECX=option base, interpolates pair `+0x3C` when transition field `+0x4308 >= 30`, clears reset flag `+0x8C` on snap, and updates primary/secondary VM ids `+0x68/+0x6C`. Committed TH08 source independently corroborates the direction/speed-switch source family only after these TH10 facts were recovered; no TH08 layout or state value is transferred. | TH10 raw/decompiled movement owner and managed-VM callees; target constants; committed clean TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` only as later source-shape corroboration |
| PLAYER-012 | observed / unknown-optimizer-ABI | `0x004247F0-0x00424D8A` is the reviewed 1,435-byte Player initialization owner. Its sole target caller allocates `0x4478` bytes, runs the adjacent zeroing constructor, moves the returned Player to EBX, then calls this body; the initializer itself never establishes EBX and consistently uses it as the Player base. The body chooses `pl00.anm`/`pl01.anm`, acquires character/shot `.sht` data, registers the reviewed update/draw callbacks in the two callback lists with priorities `0x10/0x16`, initializes the embedded draw VM, fixed/float starting positions, four mode speeds, 33-entry replay history, three timer-shaped records, three extent vectors and six derived vectors, then calls `RebuildPlayerOptions`. The only unreachable bytes inside the physical extent are the seven-byte `lea esp,[esp]` sled at `0x00424939-0x0042493F`; `0x00424D8B-0x00424D8F` is CC padding before independent `0x00424D90`. The body is `authored_game`; original source identifier/declaration/TU and the optimizer-owned EBX ABI remain unknown. | Attested TH10 function/caller/decompile/disassembly/callee evidence; direct raw gap/tail/data inspection |
| PLAYER-013 | observed | TH10-local allocation/reset evidence closes two important Player layout extents. The Player factory requests exactly `0x4478` bytes and `0x004246C0` clears exactly `0x111E` dwords from that object before publishing it as the global Player, establishing `sizeof(Player)==0x4478` for the maintained i386 layout. Separately, the initializer passes Player `+0x14` as the embedded draw-VM base and its reset owner `0x00401DE0` clears exactly `0xEB` dwords while preserving selected VM fields, establishing `0x3AC` bytes of embedded VM storage through Player `+0x3BF`; draw position begins immediately at `+0x3C0`. Callback-node handles at Player `+0x08/+0x0C` point to independently allocated `0x24`-byte nodes whose callback slot is `+0x08` and owner slot is `+0x20`; the paired Player teardown owner later removes those same nodes. | TH10 factory/constructor/VM-reset raw disassembly; initializer callback registration; teardown decompilation |
| PLAYER-014 | observed / unknown-optimizer-ABI | The reviewed Player lifecycle cohort is `0x004246C0-0x004247B3` (`Player::Player`, 244 bytes), `0x00424D90-0x00424EA0` (`PlayerResetRuntimeState`, 273), `0x00424ED0-0x00425014` (`Player::~Player`, 325), and `0x00425020-0x00425064` (`PlayerCreate`, 69). The constructor receives Player through live-in ESI from the factory, performs constructor-visible subobject initialization, clears exactly `0x111E` dwords = `0x4478` bytes, publishes `g_Player`, returns the same object and RETs. The reset owner receives Player through live-in ESI from all three observed callers, sets runtime state 1, resets three timer-shaped records, retires the mode VM id and tail-jumps into VM-group state maintenance. The destructor receives one stack Player pointer and RET 4 after MSVC SEH-framed callback unlink, resource/cache/option ownership cleanup and draw-VM owned-data release. The no-argument factory performs `new(0x4478)` -> constructor -> `PlayerInitialize`, and destroys/frees the object on initialization failure. All four bodies are bespoke Player lifecycle code and are `authored_game`; their natural maintained C++ declarations do not claim the target private optimizer ABIs or original TU. | Attested TH10 function/caller/callee/decompile/disassembly/xref evidence; direct raw caller/tail/SEH/lifetime corridors; pinned VC7 compiler feedback |
| PLAYER-015 | observed / denominator-expanded | TH10 constructor order exposes additional Player layout/source-shape facts. Player `+0x49C..+0x329B` is exactly 128 rows of `0x5C`; each row has a constructor-visible timer flag at row `+0x10`. The four `0x98` option rows have constructor flag `+0x80`. The constructor iterates 33 `0x6C` effect rows from `+0x350C`, clearing the nested timer flag at row `+0x54`; central update still processes only the first 32 rows, so the 33rd remains structurally present without invented gameplay semantics. Draw-VM relative `+0x358` (Player `+0x36C`) is an owned pointer freed by the Player destructor. Before the draw-VM parent constructor clears its 0x3AC bytes, nine neutral nested subobject flags at VM-relative `+0x6C/+0xB0/+0xFC/+0x128/+0x174/+0x1B0/+0x1FC/+0x228/+0x378` clear bit zero, matching normal C++ member-constructor ordering. Denominator review added Ghidra-missed bodies `0x004247C0-0x004247C6`, `0x004247D0-0x004247D9`, `0x004247E0-0x004247E6`, `0x00424EB0-0x00424EB2`, `0x00424EC0-0x00424EC2`, `0x00425070-0x00425088`, and `0x00425090-0x004250A1`. Only `0x00425070` is classified: it is the compiler-generated Player scalar deleting-destructor helper, by opcode-template identity with the reviewed Replay helper plus pinned VC7 synthesis. The other six remain `unknown/review`, including the three constructor-shaped bodies despite exact normal source-shape diagnostics. | Direct TH10 raw boundaries/layout/counts; target destructor lifetime evidence; pinned VC7 constructor/deleting-helper synthesis; no-origin policy for unreferenced retained bodies |
| PLAYER-015A | observed / exact-codegen-origin-indeterminate | The three Ghidra-missed Player lifecycle bodies at `0x004247C0-0x004247C6`, `0x004247D0-0x004247D9`, and `0x004247E0-0x004247E6` now have canonical normal-COFF units from `src/Player.cpp`: `PlayerTimerView::PlayerTimerView` (7 bytes), `PlayerOptionRuntime::PlayerOptionRuntime` (10), and `PlayerEffectRowView::PlayerEffectRowView` (7). Pinned VC7.1 SP1 build6030 replays all three complete extents zero-difference with no relocations. This proves exact codegen for the maintained source shapes, but target xrefs and raw references do not resolve explicit/defaulted source versus compiler-generated lifecycle origin; the origin ledger remains indeterminate. | Direct TH10 raw boundaries and Player constructor field offsets; target-bound normal-COFF comparator; canonical focused replay |
| PLAYER-015B | observed / canonical-linked-exact / unknown-origin | `PlayerOptionSpecialCallbackBody @ 0x00427AE0` is the 99-byte EDI-bound body reached only from the exact 12-byte callback adapter at `0x00427AD0`. The target reads `g_Player->optionMode`, tests option `+0x84`, sends primary-VM interrupts 6/3 on transitions, copies the two replay coordinates in target-observed load/store order, and writes the current mode back. Declaring the target-observed ordered field accesses in maintained `src/Player.cpp` yields a complete 99-byte PDB-owned contribution under the real `RebuildPlayerOptions` `/GL` graph. Two independent cold links reproduce all 99 bytes and the exhaustive `g_Player` DIR32 plus two `SetInterrupt` REL32 fields after target-address replay. The body’s source-written versus compiler/LTCG-outlined origin remains unresolved. | Hash-attested TH10 body/wrapper disassembly; exact `SetInterrupt @ 0x00449470`; canonical `player-option-special-body` and wrapper replays; pinned VC7.1 SP1 build6030 |
| PLAYER-016 | observed / corroborated | `0x00428280-0x004285EB` is the reviewed 876-byte Player shot-update owner mapped descriptively as `PlayerUpdateShots`. Its sole caller is the central Player update body, and its machine boundary is one stack Player pointer with `RET 4`; the only bytes inside the physical extent omitted from Ghidra's reachable-body count are the six-byte unreachable alignment region at `0x004282AA-0x004282AF`. The body iterates exactly 128 `0x5C` rows at Player `+0x49C`. Each row has the established `0x14` timer, a `0x2C` motion block at `+0x14`, state `+0x40`, primary/secondary managed-VM ids `+0x44/+0x48`, collision handoff fields `+0x50/+0x54`, and a descriptor pointer `+0x58`. The pointed `.sht` descriptor is `0x34` bytes; reviewed fields include signed schedule bytes `+0x00/+0x01`, signed damage short `+0x02`, spawn offsets `+0x04/+0x08`, hitbox extents `+0x0C/+0x10`, angle/speed `+0x14/+0x18`, signed source index `+0x1C`, type `+0x1D`, animation/sound shorts through `+0x22`, and callbacks at `+0x24/+0x28/+0x30`. The update callback at `+0x28` receives Player in ECX and the shot row in EDX; its return value is not consumed. Type-3 shots use source slots 1-4, retire their VMs when their source option disappears, and use the four occupancy ints at Player `+0x42F8` through natural `sourceIndex - 1` indexing; this produces the target address expression `+0x42F4 + sourceIndex*4` while preserving the independently proven 33-row effect constructor at `+0x350C`. Motion uses target polar helpers, managed-VM position is offset by `224.0f/16.0f`, ordinary shots are culled after timer frame 10, and the familiar timer-scale update closes each surviving row. Committed clean TH08 Player source was consulted only after these TH10-local facts were recovered and independently corroborates the broad fixed-slot/update-callback source family; its `0x484` shot layout, addresses and exactness are not transferred. | Attested TH10 function/caller/callee/decompile/disassembly/xrefs; direct spawn/collision owner context; target raw boundary and constants; committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` only for later source-family corroboration |
| PLAYER-017 | observed / denominator-expanded | Raw review immediately after `PlayerUpdateShots` found a complete Ghidra-missed body at `0x004285F0-0x00428626`: four `CC` bytes separate it from the shot owner, it performs floating-point min/max-style pair construction through register-bound pointers using target constant `0x00470B0C`, ends in plain RET, and is followed by nine `CC` bytes before independent `0x00428630`. Native Ghidra reports no containing function and no xrefs to `0x004285F0`. It is therefore added to the denominator as `unknown/review`; physical source owner, exact ABI, role and origin are not guessed. | Direct TH10 raw boundary/disassembly; attested Ghidra function/xrefs_to negative results |
| PLAYER-018 | observed / corroborated | `0x00428630-0x00428AC1` is the reviewed 1,170-byte Player damage/collision owner mapped descriptively as `Player::CalculateDamageToTarget`. Its sole direct caller at `0x0040E13C` pushes the global Player pointer, target position and target size, clears ECX, calls this body, and subtracts EAX from a hostile health-like field. The body ends in `RET 12`. Target-private optimization rotates the four logical member-function values: the optional output pointer is live in ECX, while Player, position and size are the three stack values; maintained source keeps the natural Player-member spelling and does not claim that private ABI as the original source convention. The function processes all 128 `0x5C` shot rows, uses half-extents for AABB collision, invokes descriptor `+0x30` as an ECX/EDX Player/shot callback with target position on the stack, marks hit state/VM transitions, accumulates descriptor `+0x02` as signed damage, rate-limits type-3 damage on four-frame timer boundaries, and spawns a collision/effect row for type 2. It then consumes the first 32 `0x6C` Player rows as circle or axis-aligned/rotated rectangular damage regions. TH10 spawn/update/collision paths jointly establish row radius/radius delta `+0x00/+0x04`, angle/angle delta `+0x08/+0x0C`, extents `+0x10/+0x14`, Float3 position `+0x18`, Float3 velocity `+0x24`, timer at `+0x44`, damage/hit accumulator/hit cap/collision interval `+0x58/+0x5C/+0x60/+0x64`, and active/mode flags `+0x68`. The 33rd constructor-visible row remains structurally present but is not assigned these first-32 runtime semantics. Committed clean TH08 `Player::CalcDamageToEnemy` was consulted only after the TH10 layout/control flow was recovered and corroborates the broad source family; no TH08 address, extent, layout or exactness fact is transferred. | Attested TH10 function/caller/callee/decompile/disassembly/xrefs; direct raw caller and target constants; TH10 row-spawn helper; committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` only for later naming/source-family corroboration |
| PLAYER-019 | observed / denominator-expanded | Physical review after the damage owner establishes `0x00428AC2-0x00428ACF` as 14 bytes of `CC`, followed by a separate `0x00428AD0-0x00428B04` 53-byte `RET 4` body, 11 bytes of `CC`, a separate `0x00428B10-0x00428C19` 266-byte plain-RET body, and 6 bytes of `CC` before the existing `0x00428C20` candidate. Ghidra reports no containing function and no xrefs for either raw entry. Their Player-shot-compatible offsets make them relevant denominator candidates, but indirect callback/source ownership, exact logical ABI, authored-versus-retained origin and exactness are not inferred; both remain `unknown/review`. | Direct verified TH10 PE corridor extraction and i386 disassembly; attested negative Ghidra function/xrefs_to results |
| PLAYER-020 | observed / source-boundary-corrected | `PlayerUpdateCallbackBody @ 0x00425730-0x00426340` and the registered update entry at `0x00426500-0x00426506` are distinct target boundaries. The entry receives Player in ECX and is exactly `push ecx; call body; ret`; the body loads the sole Player pointer from its stack frame and ends in `ret 4`. The maintained five-state update implementation now belongs to `__stdcall PlayerUpdateCallbackBody`, while a separate `__fastcall PlayerUpdateCallback` models the registered entry. This adds source presence to the 3,089-byte authored core rather than crediting its semantics only to a seven-byte adapter. Ghidra's provisional two-parameter `__thiscall` signature is contradicted by the raw boundary, sole incoming call and stack cleanup. | Fresh hash-attested target entry/tail/table/adapter disassembly; Factory-attested decompile, calls and xrefs; maintained full Player update source and layout |
| PLAYER-021 | observed / canonical-coff-exact | The registered Player update entry `0x00426500-0x00426506` is a seven-byte adapter that pushes its live-in ECX owner, calls `PlayerUpdateCallbackBody @ 0x00425730`, and returns its result. The maintained `PlayerUpdateCallback` now sits in `src/Player.cpp` beside that body and the paired draw-chain callback. Two cold canonical normal-COFF replays reproduce all seven bytes and the sole REL32 to `0x00425730`. Exactness is limited to the declared compiler profile; the target's authored-versus-LTCG origin and production TU remain unknown. | Hash-attested target bytes, callback registration and body extent; canonical `player-update-callback` cold replays; pinned VC7.1 SP1 build6030 |
| PLAYER-022 | observed / canonical-linked-exact | `PlayerAngleToPoint @ 0x00426610` is the 67-byte Player-relative angle helper already referenced by maintained `PlayerUpdateCallbackBody`. The sole target call at 0x00425B22 is preceded by `LEA EAX,[ESP+0x14]` for the temporary target vector and `MOV ECX,EBP` for the Player owner. The target loads target X/Y, subtracts Player drawPosition at +0x3C0/+0x3C4, compares dy then dx against the shared 0.0f constant at 0x00470B04, returns π/2 from 0x00470B94 when both deltas are zero, and otherwise executes x87 `FXCH; FPATAN`. A natural maintained implementation with float dx/dy and `atan2(dy, dx)` lets VC7.1 /GL recover the private EAX-point / ECX-Player machine entry. The canonical linked unit reproduces all 67 bytes and all three DIR32 fields zero-difference. | Hash-attested TH10 caller/body/constants; maintained Player call site and +0x3C0 drawPosition layout; canonical `player-angle-to-point` replay; pinned VC7.1 SP1 build6030 |
| PLAYER-023 | observed / exact | `0x00428D70` is a complete 91-byte ECX-bound helper called by `PlayerUpdateShots @ 0x00428280`. It tests shot extents against X bounds `(-192, 192)` and Y bounds `(0, 448)` through four target data references at `0x00470B40`, `0x00470B3C`, `0x00470B04`, and `0x00470B38`. Maintained `PlayerShotBoundsView` lives in `src/Player.cpp`; its four C-linkage names are descriptive unresolved aliases used by the non-runnable exact-link harness, not recovered target symbol names. Canonical linked-PE replay covers all 91 bytes and all four DIR32 fields. | Hash-attested target bytes, direct caller and referenced float values; canonical `player-shot-outside-playfield` cold replays; pinned VC7.1 SP1 build6030 |
| PLAYER-024 | observed / canonical-linked-exact / private-ABI | `PlayerResetRuntimeState @ 0x00424D90` is a complete 273-byte target owner with live-in ESI and a tail jump to the exact GUI life-count helper. Direct IDA shows three timer initialization/reset sequences, the manager-global join load, direct `MarkVmForDeletion`, duplicate mode-VM-id clears, and the GUI count tail. Natural volatile lifetime/store qualifiers plus owner-local manager/member bridges reproduce all 273 bytes in canonical linked-PE replay under the Player constructor `/GL` context; six declared DIR32/REL32 fields replay zero-difference. The bounded exactness proves this maintained compiler context, not original retail TU ownership. | Direct IDA disassembly/decompile and five callers; canonical `player-reset-runtime-state` and `gui-set-lives-display-count` replays; pinned VC7.1 SP1 build6030 |
| PLAYER-026 | observed / canonical-coff-exact / timer-helper | `PlayerTimerView::HasIntervalTick @ 0x00427C70-0x00427C8C` is a complete 29-byte `__thiscall` timer predicate used by the Player shot collision path and one FrontEnd-side caller. Direct IDA decompiles it as `current != previous && !(current % interval)` with timer fields at `+0x04/+0x00` and `RET 4`. A natural `PlayerTimerView` member in `src/Player.hpp`/`src/Player.cpp`, called through the maintained shot timer, reproduces all 29 bytes in the canonical normal-COFF unit `player-timer-interval` with zero relocations. Exact codegen does not prove the original timer class/TU, so origin remains indeterminate. | Direct IDA Pro MCP disassembly/decompile/callers/bytes; adjacent TH08 `ZunTimer::IsPeriodic` consulted only as source-shape corroboration; focused `player-timer-interval` replay; pinned VC7.1 SP1 build6030 |
| PLAYER-027 | observed / canonical-coff-exact / CRT-wrapper | `0x00427C50-0x00427C64` is a complete 21-byte `__stdcall` float-to-double floor wrapper called twice by the Player draw body at `0x00426360`. Direct IDA decompiles it as `return floor(float)`, with x87 stack staging, the target `_floor @ 0x00452FF0` REL32 and `RET 4`. A natural `PlayerFloor(float)` wrapper in `src/Player.cpp` reproduces the full extent and normalized CRT relocation in the canonical normal-COFF unit `player-floor`; source-written versus retained/compiler-generated origin remains indeterminate. | Direct IDA Pro MCP disassembly/decompile/callers/bytes and `_floor` lookup; focused `player-floor` replay; pinned VC7.1 SP1 build6030 |
| PLAYER-029 | observed / reviewed-complete-boundary | RebuildPlayerOptions owns 0x00426F70-0x00427947, 2520 bytes: the prior 2494-byte executable body plus two-byte MOV EDI,EDI alignment and the six-slot table at 0x00427930-0x00427947. The indexed jump at 0x0042704E bounds unsigned(character*3+shot) to 0-5; all slots point to body instruction boundaries. Fresh read-only Ghidra xrefs show one indexed table-base reference and no competing slot references. Eight CC bytes follow before independent callback 0x00427950. Ghidra reachable body bytes remain 2471; the three internal alignment sleds account for the other 23 executable-body bytes. The focused target-bound inventory passes; unrelated historical global ledger metadata at 0x00401100 still fails automatic regeneration comparison. Only this ownership row is extended, adding 26 authored bytes without exact credit. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| PLAYER-031 | observed / real-caller-recovery / non-exact-frontier | GAME-001/002 restore the two GameManager Rebuild calls and their actual registration path. Seven of nine target calls now have maintained callers; only two calls in ItemManager0x41AFD0 remain source-absent. Adding full GameManager and ReplayManager sources to the real Enemy graph leaves Rebuild2404/2520, frame0x38 and plain RET, InitializeVm207/217 with ECX plus two stack arguments / RET8, spell2476/2432 and771/2032, and parent14384/14416 and580/11544 unchanged. Earlier handoff prose called the parent's normalization incomplete; the actual prior id-local and current diagnostic reports both flag normalization_complete=true. This diagnostic flag is corrected here without granting exhaustive semantic linkage or exactness authority. Original production partition, remaining caller/source shape and private ABI causes remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| PLAYER-032 | observed / complete-maintained-caller-coverage / non-exact-ABI | ITEM-008 restores the final two source-absent Rebuild callers, so all nine direct target call sites now have maintained source. The natural ten-TU candidate remains plain RET rather than target RET4 despite the complete mapped call set. Source presence does not prove faithful production graph, declarations or linker ownership. Rebuild body/ABI exactness remain unknown; no forced stdcall annotation, fake escape root or /GL removal is adopted. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## Items

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ITEM-001 | observed / owner-recovery | The 0x0041AD90-0x0041BECD cluster is TH10-locally identifiable as the item subsystem. 0x0041AED0 allocates 0x21CEC0 bytes, vector-constructs 0x896 records of size 0x3F0 at manager +0x14, publishes the allocation, and calls 0x0041AD90; that routine registers callbacks at 0x0041BA00 and 0x0041BA30. 0x0041AFD0 iterates the same record count/stride for movement and collection, while 0x0041BB00 is a 13-caller creation path. Raw re-audit also exposes previously untracked executable bodies 0x0041B8E0-0x0041B9F8, 0x0041BA00-0x0041BA2F, and 0x0041BA30-0x0041BA4C, separated by CC padding. | Hash-attested target bytes; Factory-native Ghidra decompile/xrefs/callers; direct raw gap disassembly |
| ITEM-002 | observed / canonical-linked-exact | GameScoreStateView::AddFaith @ 0x0041BE80 is a 46-byte __thiscall helper on global state 0x00474C40: it adds signed amount / 10 to dword +0xC and caps above 99999. TH10-local item paths pass 10, 100 and difficulty-scaled 5000/8000/10000 amounts; the GUI displays the same stored field as five digits and uses it in stage-clear scoring; the separate Enemy timeout path at 0x00412932 directly subtracts 3000 before flooring at 5000, while the Player decay helper is called with 3000 and applies /-10 so that path lowers stored faith by 300. Normal COFF is 48 bytes, but the adjacent item polar helper as /GL entry emits the exact 46-byte target contribution. Two independent canonical cold replays are relocation-free and raw-equal. Names are maintained/descriptive. | Hash-attested target extent; TH10-local item/GUI/Enemy evidence; two cold item-faith-add replays; pinned VC7.1 SP1 build6030 |
| ITEM-003 | observed / canonical-linked-exact | ItemVectorView::FromAngleMagnitude @ 0x0041BEB0 is the 30-byte x87 polar helper used by item update 0x0041AFD0 and creation path 0x0041BB00. It writes cos(angle) * magnitude to x and sin(angle) * magnitude to y with one FSINCOS. The maintained x87 spelling is 30/30 structural-exact in normal /Gy COFF and raw-equal in two independent canonical linked-PE cold replays. | Hash-attested target extent/callers; TH10-local item owner recovery; normal COFF comparison; two cold item-vector-from-angle-magnitude replays; pinned VC7.1 SP1 build6030 |
| ITEM-004 | observed / inventory-correction / mixed exactness | The old reviewed-candidate denominator skipped three real ItemManager code bodies between 0x0041AFD0 and 0x0041BA50: draw core 0x0041B8E0-0x0041B9F8 (281 bytes), registered update callback 0x0041BA00-0x0041BA2F (48 bytes), and registered draw callback 0x0041BA30-0x0041BA4C (29 bytes). All three have reviewed boundaries and authored ItemManager origins. Rechecking current source supersedes the old 31/29 draw-wrapper diagnostic: with maintained `ItemManagerView::Draw` present in the same /GL translation unit, VC7.1 naturally moves callback ECX to target EAX and tail-jumps to the draw core. Two independent cold canonical linked-PE builds reproduce the full 29-byte draw callback and both linkage fields exactly. The update callback remains non-exact: its verified __stdcall core plus target-equivalent ((flags >> 2) | flags) gate expression still emits 42 bytes versus target 48 because target saves the manager in ESI and reuses ECX for the bit-test lifetime. | Raw target disassembly and callback-registration xrefs; Factory-native update-core calling-convention check; two cold canonical item-manager-draw-callback replays; pinned VC7.1 SP1 build6030 |
| ITEM-005 | observed / corrected-attribution / canonical-linked-exact | `GameScoreStateView::AddRank @ 0x00405B60` corrects the older generic “effect-coordinate clamp” interpretation. The 42-byte target body receives state in EAX and a signed delta in ECX, adds the delta to state +0x58, then clamps the result to `[-0x400,+0x400]`. All five target calls are inside `ItemManager update @ 0x0041AFD0` and explicitly load EAX=`0x00474C40`; their deltas are -4, +12, +8, +24 and +256. Thus state +0x58 is `0x00474C98`, independently identified by the Enemy ECL readers as TH10’s bounded rank scalar. Extending the already established `GameScoreStateView` from faith +0x0C to rank +0x58 and using the natural add/clamp spelling produces a 42-byte relocation-free PDB contribution under the existing ItemHelpers /GL graph, raw-equal to the target. | Hash-attested TH10 five-call ItemManager xrefs and call setup; independent Enemy ECL rank evidence; canonical `item-rank-add` replay; pinned VC7.1 SP1 build6030 |
| ITEM-006 | observed / corrected-attribution / canonical-linked-exact | `GameScoreStateView::DecayFaith @ 0x00412E70` is the 46-byte lower-bound companion to AddFaith. Its sole target call at `0x00425A1F` comes from `PlayerUpdateCallbackBody`, loads ECX=`0x00474C40`, and pushes 3000. The helper updates state +0x0C, independently established as faith, using signed division by -10 and then clamps values below 5000. Thus this observed helper call lowers stored faith by 300. This is distinct from the independently verified Enemy timeout path at 0x00412932, which directly subtracts 3000 from the same field before flooring it at 5000. A natural `faith -= amount / 10` method is 48 bytes in normal COFF, while the existing ItemHelpers `/GL` graph emits the target-shaped 46-byte contribution with zero differences and no linked fields. | Hash-attested target body and sole call setup; established GameScoreStateView faith offset; canonical `item-faith-decay` replay; pinned VC7.1 SP1 build6030 |
| ITEM-007 | observed / reviewed-complete-boundary | Item update owns0x41AFD0-0x41B8DF,2320 bytes:2253-byte executable body through RET4 at0x41B89A, three-byte LEA ECX,[ECX] alignment, eleven-slot kind table0x41B8A0-0x41B8CB and five-slot difficulty table0x41B8CC-0x41B8DF. Selectors bound unsigned(kind-1)<=10 and unsigned(difficulty)<=4; every slot reaches a body instruction. Fresh Ghidra table/slot xrefs show no competing owner; the second table's indexed reference is represented at+4 rather than its raw base. Draw begins immediately at0x41B8E0. Ghidra reachable body2245 excludes the eight-byte alignment at0x41B008. The corrected denominator gains67 authored bytes without exact credit. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-008 | observed / recovered-update-and-registration / non-exact-core | Full natural ItemManagerUpdateBody restores the0x896-by3F0 pool: world position3AC, velocity3B8, timer3C8, active3DC, kind3E0, spriteKind3E4, attractionSpeed3E8 and delay3EC. State5 countdown activates state2/ANM script kind+0x176; states1/2 apply scaled velocity and0.03 gravity, states3/4 attract with speed growth0.2 up to12, and Player state4 cancels attraction. Player4324..436B supplies collect/focused/unfocused rectangles. Types1/10 and4/11 invoke actual Rebuild at0x41B420/0x41B67A after adding power1/20; popups preserve explicit owner0x477840, item position, value and color. Point reduction is trunc((playerY-144)*float(1/304)*(faith*0.5-5000)), subtracted from faith*10/2 and rounded toward a multiple of10. Collection invokes cue20; power-level transitions also cue29 and defer ConvertPowerItems. Only animated survivors advance the timer/count; delayed, retired and collected rows skip that path. Registration0x41AD90 creates both callback nodes, clears flags bit1, supplies manager at node+20, registers priorities21/25 and stores nodes at+8/+C. Existing full Draw source is reconciled with source mappings. Opaque helper contracts, original source partition and runtime remain open; TH08 is source-shape hypothesis only. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-009 | observed / canonical-linked-exact | Registered update adapter0x41BA00-0x41BA2F tests ((flags58 OR (flags58 SHR2)) AND1) or flags58 bit10, then pushes its ECX manager and calls the RET4 update core. Full recovered update and actual callback registration naturally emit all48 bytes. Two independent cold /GL builds have distinct images/PDB GUIDs and exhaustive FPS DIR32/update REL32 zero-difference; canonical item-manager-update-callback grants48 authored exact bytes. Seven prior affected canonical Item units cold-replay325/325 bytes across three artifacts. Core/register/Draw exactness and product/runtime gates remain independent. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-011 | observed / canonical-linked-exact | Power digit display0x4054B0-0x4054FE uses Gui owner ESI, integer value EDX and stack percent/RET4. It binds sprite(value+8) at owner+0x6A8C, sprite(percent/10+8) at+0x71E4 and sprite(percent%10+8) at+0x7590, reloading front ANM+0x9EC8 before all three actual SetSprite calls. Natural member source in the real Item registration/update graph with AnmManager /GL support reproduces the complete79-byte PDB extent and all three REL32 fields across two independent cold links. GUI ownership follows existing target-local offsets; original class/TU/declaration and product ownership remain unknown. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-012 | observed / canonical-linked-exact | Faith timer helper0x412FF0-0x41304C receives state EDI and stack amount/RET4. If current+0x18<130 it converts amount to float and invokes the shared timer Add0x44BF40 on+0x14; an overshoot lazily initializes then sets current130/subframe130/previous129. Independent storage views have the same reviewed0x14-byte layout. The natural GameScoreStateView member plus actual AnmVmTimerView::Add definition reproduces all93 bytes and exhaustive Add REL32/game-speed DIR32 fields twice. The maintained cast expresses the shared machine boundary, not an original-class claim. Ten prior affected exact units across six artifacts replay411/411 bytes. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-015 | observed / lifecycle-source-and-extent-recovery | Full maintained source covers allocation0x41AED0, destructor0x41ADF0, power-item conversion0x41BA50 and Spawn0x41BB00. Allocation constructs896 records at+14, clears the entire21CEC0-byte manager, sets flags bit1, publishes0x477818 and calls real registration even onNULL; failure destroys/releases a non-NULL owner. Destructor captures the chain owner before each callback lock, updates the byte-depth counter, unlinks nodes, clears the manager global and naturally destroys all records. Spawn uses150 normal rows plus2048 delayed kind8 rows, signed ring modulo and delay bands determined by request thresholds256/512/1024. A busy delayed row still advances request count/cursor; only normal rows clamp X. Power>=100 converts kinds1/4 to9 and10/11 to5. The old row kind3 is tested before installing the new kind; both Spawn and conversion use actual CreateVmAtWorldVariant0(script189). Conversion retains the target's unreachable10/11 arm after its active kind1/4 predicate. Spawn physical ownership is793 body +3 alignment +12 jump-table +11 selector =819 bytes through41BE32; all three table destinations are internal and13 CC bytes separate41BE40. The newly inventoried record constructor104 has a real EH callback pointer and eight CC bytes before the34-byte record destructor. Both record origins remain indeterminate. TH08/TH095 suggest source shapes only; their differing behavior and provisional tricks are not imported. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-016 | observed / canonical-linked-exact / indeterminate-origin | Record constructor0x41ACF0-0x41AD57 composes nine VM timers, whole-VM clear, invalid sprite index and a tenth timer flag constructor. Using the real AnmVmTimerView member yields all104 bytes raw-equal; clearing the same flag in the Item constructor body instead reorders the invalid-sprite store and agrees at86/104 bytes. Natural member destruction produces the34-byte0x41AD60 owner, conditionally releasing the VM buffer and clearing it. Two independent pinned VC7.1 /GL cold links with actual GameCreateItemManager entry and real AnmManager support replay both complete PDB extents; destructor free REL32 offset0xF binds directly to0x452422. Images/PDB GUIDs are distinct. Omitted free relocation and constructor extent103 are both rejected. Canonical item-record-constructor/destructor grant138 overall exact bytes, with no authored exact credit: source-authored inline lifecycle versus compiler-generated contribution remains indeterminate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-017 | observed / focused-non-exact-frontier | Final Item-plus-Anm graph uses actual GameCreateItemManager as entry, with no private register annotations or profile changes. Spawn naturally recovers EAX owner, ECX position and RET16 but remains795/819 with different branch/tail layout. Allocation122/185 outlines construction and folds registration's known-zero result; destructor197/216 uses a private EBX owner/plain RET; ConvertPowerItems289/165 uses a stack owner/RET4 and inlines world-VM creation. Item update2228/2320, AddPower224/166, popup171/167 and world creator116/118 remain non-exact. Earlier guarded-do-loop popup controls do not change either171/167 or176/167 output; power-notice pointer capture changes two diagnostic bytes, while /Ob1 gives163/166 and incorrect geometry. None is promoted. Ten pre-existing affected canonical units across five compiler artifacts replay545/545 bytes under final physical source. Previous ten-TU images remain historical controls tied to their older Item source hash; current production partition, absent external Item caller contracts and product/runtime closure remain open. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| ITEM-018 | observed / rejected-structure-controls | Fresh target review confirms Spawn uses EAX manager, ECX position and RET16; the linked ConvertPowerItems caller preserves this ABI. At the old-kind-3 call, position is live item worldPosition in EDI. The stack-relative pointer is the hidden AnmVmIdView result buffer, consumed through EBP/EAX by CreateVmAtWorldVariant0 and AddVmVariant0; it is not a position temporary. Normal-first source with separate color-return tails restores the 819-byte extent but leaves widespread instruction/binding-position differences (78/747 comparable, 82 raw), so it was reverted. A bounded for-loop control unrolls six records, changes private cleanup to RET20 and grows to907 bytes; it too was reverted. No semantic source correction or exact credit results. Keep the complete793-byte body and26-byte alignment/table suffix; neighboring13 CC bytes remain outside this owner. | Fresh hash-attested target and linked caller/callee disassembly; bounded VC7.1 probes on2026-10-14; Git history |

## Sound

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| SOUND-002 | observed / corroborated | Two additional Ghidra-missed, CC-delimited authored sound bodies are tracked. `0x0043D080-0x0043D0EA` is passed as a thread start address, loads all 37 target `se_*.wav` entries through `0x0044B360`, stores their buffers, aborts on stop state 2, and waits with `Sleep(1)` while stop state is zero. `0x0043D0F0-0x0043D111` launches it with `CreateThread`, stores the handle at `0x004977AC`, and returns it; `0x00420100` contains the same launcher sequence inline. The target KERNEL32 IAT binds `0x004660AC` to `Sleep` and `0x00466118` to `CreateThread`. `SoundDataLoaderThread` and `StartSoundLoadThread` are adjacent-supported proposed names only. | Direct TH10 raw disassembly, filename-table decoding, and PE import-table mapping; attested xrefs; committed TH095 source only for naming/source-shape corroboration |
| SOUND-003 | observed / canonical-coff-exact | `CSound::Pause @ 0x0044D5B0` is the 29-byte pause helper from the DirectSound utility family. TH10 retains `.\src\core\zwave.cpp` source-path strings, and natural `CSound::Pause` in maintained `src/ZWave.cpp` reproduces the complete relocation-free target body. Earlier standalone Stop/Unpause/Reset probes exposed private receiver differences; SOUND-007 and SOUND-008 supersede the old Reset and Stop diagnostics through real caller graphs, while Unpause remains source-present but non-exact under SOUND-011. | Hash-attested TH10 target disassembly and source-path strings; neighboring TH10 CSound layout; canonical `zwave-csound-pause` replay; pinned VC7.1 SP1 build6030 |
| SOUND-004 | observed / canonical-coff-exact / origin-indeterminate | `CStreamingSound::~CStreamingSound @ 0x0044D730` is the 11-byte retained derived-destructor body immediately after the reviewed scalar-deleting destructor at 0x0044D710. TH10 writes vtable `0x004705D4` and tail-jumps `CSound::~CSound @ 0x0044D080`. Completing the maintained `CSound` layout to 0x5C with `DSBUFFERDESC` at +0x34 and manager pointer at +0x58 makes `CStreamingSound` 0x78 bytes, matching the constructor stores through +0x6C. An empty derived destructor in `src/ZWave.cpp` produces an 11-byte normal-COFF COMDAT with exactly two relocations: derived vtable DIR32 at +0x02 and base-destructor REL32 at +0x07; both resolve to the target and all three non-relocation bytes match. This proves codegen/source mapping but not whether the original destructor was explicitly written or implicitly synthesized, so origin remains indeterminate. | Hash-attested TH10 constructor/destructor/scalar-deleting-destructor bytes; retained `.\src\core\zwave.cpp` source-family strings; canonical `zwave-streaming-sound-destructor` replay; pinned VC7.1 SP1 build6030 |
| SOUND-005 | observed / canonical-coff-exact / origin-indeterminate | `CSound::~CSound @ 0x0044D080` is a complete 136-byte standard-ECX lifecycle body in the TH10 `src/core/zwave.cpp` family. It installs vtable `0x004705D8`, releases each non-null DirectSound buffer and clears its slot, deletes the buffer-pointer array, then destroys/frees the owned `CWaveFile`. The target CWaveFile close path uses flags +0x78 and HANDLE +0x8C; completing the maintained 0x94-byte CWaveFile layout lets VC7.1 inline that close/destructor path naturally. The canonical normal-COFF body is 136 bytes with 120/120 non-relocation bytes identical and four exact fields: CSound vtable DIR32, two operator-delete REL32 calls, and imported CloseHandle DIR32. Codegen exactness does not establish whether the original destructor was explicit source or a compiler-synthesized lifecycle body, so origin remains indeterminate. | Hash-attested TH10 CSound/CWaveFile target bytes and retained `.\src\core\zwave.cpp` source-family strings; canonical `zwave-csound-destructor` replay; pinned VC7.1 SP1 build6030 |
| SOUND-006 | observed / canonical-coff-exact | `StartSoundLoadThread @ 0x0043D0F0` is the 34-byte retained launcher for the sound-data loader. TH10 retains `.\src\core\sound.cpp` source-path strings, and the target body allocates one local thread-id DWORD, calls imported `CreateThread` with start address `0x0043D080` and parameter `g_MainSoundOwner @ 0x00492590`, stores the returned HANDLE at `0x004977AC`, and returns it in EAX. Natural maintained source in `src/Sound.cpp` uses the same Win32 expression and only casts the unresolved worker address to `LPTHREAD_START_ROUTINE`; pinned VC7.1 normal COFF reproduces all 34 bytes with exact DIR32 fields to the sound owner, worker, CreateThread IAT `0x00466118`, and HANDLE storage. This exact launcher does not establish a WINAPI/source ABI for the worker, whose target body still returns with plain RET. | Hash-attested TH10 disassembly and retained sound.cpp path; two cold normal-COFF builds; canonical `sound-start-load-thread` replay; pinned VC7.1 SP1 build6030 |
| SOUND-007 | observed / canonical-linked-exact | TH10-local disassembly identifies the `CSound::Play` helper family. Natural maintained source under Play as the `/GL` entry makes VC7.1 select target-private receivers for `RestoreBuffer @ 0x0044D300`, `GetFreeBuffer @ 0x0044D370`, `SetVolume @ 0x0044D4E0`, and `Reset @ 0x0044D600`. Independent cold links reproduce 98/98, 103/103, 106/106 and 61/61 bytes respectively; only declared Sleep/rand/BGM-volume/float/__ftol2 linkage fields differ before relocation. TH10 itself proves `0x00497854` is the BGM volume scalar because FrontEnd options writes `g_MainSupervisorView.bgmVolume` there. The same target-bound `ProbePlayRoot` context now also closes the Play body and its FillBufferWithSound call; the separate Fill body is recorded in SOUND-013. | Hash-attested TH10 caller/disassembly and options-global writes; maintained `src/ZWave.cpp`; independent cold linked replays; pinned VC7.1 SP1 build6030 |
| SOUND-008 | observed / canonical-linked-exact | CSound::Stop @ 0x0044D550 is the 82-byte stop/rewind helper in the retained src/core/zwave.cpp family. TH10 target code keeps the CSound receiver in EDI, clears m_bIsPlaying, iterates all DirectSound buffers, OR-accumulates each Stop and SetCurrentPosition(0) result, then clears the fade type. The real target caller 0x0043DAB0 is recovered naturally as SoundPlayerView::StopBgm in src/Sound.cpp; linking that caller as the /GL entry with src/ZWave.cpp support makes VC7.1 select the target EDI receiver and exact loop shape. Two independent cold builds produce the same complete 82-byte relocation-free PDB contribution, raw-equal to TH10. | Hash-attested TH10 Stop/StopBgm disassembly and retained sound/zwave source-path strings; maintained src/Sound.cpp and src/ZWave.cpp; canonical zwave-csound-stop replay; pinned VC7.1 SP1 build6030 |
| SOUND-009 | observed / canonical-linked-exact | CWaveFile::Reopen @ 0x0044DCA0 is the 78-byte BGM-format rebind helper. TH10 keeps the CWaveFile receiver in ESI and the ThBgmFormat pointer in ECX, rejects memory-backed files and INVALID_HANDLE_VALUE, stores the format at +0x90, seeks the disk handle to format->startOffset plus g_SoundBgmFileBaseOffset, updates the current size from format->totalLength, mirrors it to +0x2C, and returns S_OK. The real 55-byte caller SoundPlayerView::ReopenBgm @ 0x0043D790 is present in maintained src/Sound.cpp; selecting that caller as the /GL entry with src/ZWave.cpp support makes VC7.1 emit the exact private ESI/ECX contract. Two independent cold canonical builds reproduce the complete 78-byte contribution, with only the declared BGM-file-base and SetFilePointer DIR32 fields requiring replay. | Hash-attested TH10 Reopen/ReopenBgm disassembly; maintained src/Sound.cpp, src/ZWave.cpp and SoundFormat.hpp; canonical zwave-wave-file-reopen replay; pinned VC7.1 SP1 build6030 |
| SOUND-010 | observed / canonical-linked-exact | CWaveFile::ResetFile @ 0x0044DD40 is the complete 194-byte disk/memory BGM read-state reset helper. TH10 keeps the CWaveFile receiver in ESI and the loop boolean in AL. Memory mode restores +0x84 from +0x80, refreshes +0x88 from format totalLength when positive and, when looping with positive introLength, advances the memory cursor. Disk mode rejects a null handle, optionally seeks to startOffset+introLength+BGM-base and sets remaining size to totalLength-introLength, otherwise seeks to startOffset+BGM-base and restores totalLength. The real 55-byte SoundPlayerView::ReopenBgm caller graph retains ResetFile as an independent PDB contribution; under that /GL graph the maintained source reproduces all 194 bytes. The only replay fields are two g_SoundBgmFileBaseOffset DIR32 references and two SetFilePointer IAT DIR32 references. | Hash-attested TH10 ResetFile disassembly; maintained src/ZWave.cpp, src/Sound.cpp and SoundFormat.hpp; canonical zwave-wave-file-reset replay; pinned VC7.1 SP1 build6030 |
| SOUND-011 | observed / source-present / nonexact-callers | The real support callers used by recent ZWave exact units are now explicitly source-mapped instead of remaining ledger-unknown. `SoundPlayerView::StopBgm @ 0x0043DAB0` uses ESI as its owner, stops the active BGM, shuts down the BGM thread/event handles and destroys the BGM object; it is the real /GL entry that closes `CSound::Stop`, but fixed normal COFF is 177 bytes versus target 176 and its own exactness remains open. `SoundPlayerView::ReopenBgm @ 0x0043D790` uses EAX for the owner, selects a 0x34-byte format row and calls exact `CWaveFile::Reopen`; fixed normal COFF was 64 versus target 55; SOUND-018 later closes it under the real /GL /GS /EHsc LoadBgm graph. `CSound::Unpause @ 0x0044D5D0` is source-present in `src/ZWave.cpp`; natural normal COFF has the correct 41-byte extent and 32/41 bytes identical, with the remaining difference driven by target EAX versus ordinary ECX receiver. Its sole caller `0x0043DDF0` remains source-unmapped. | Hash-attested TH10 disassembly; current maintained Sound/ZWave source; fixed normal-COFF probes; exact child units only as caller-context evidence, not caller exactness |
| SOUND-012 | observed / canonical-linked-exact | `CSound::Unpause @ 0x0044D5D0` is the 41-byte primary-buffer resume helper. The target keeps the receiver in EAX, checks `m_apDSBuffer`, sets `m_bIsPlaying`, and calls the primary buffer's Play vtable slot with the saved priority/flags. Natural maintained `src/ZWave.cpp` is exact when replayed in the real `CSound::Play` `/GL` caller context: the complete extent is raw-equal and has no relocation fields. | Hash-attested TH10 disassembly; maintained ZWave source; canonical `zwave-csound-unpause` linked replay under pinned VC7.1 SP1 |
| SOUND-013 | observed / source-shape-correction / canonical-linked-exact | Direct IDA Pro review recovers the Microsoft DirectSound utility pair behind `CSound::Play`: `FillBufferWithSound @ 0x0044D110` is a 483-byte EAX-bound `CSound` body with ESI buffer and two stack arguments, while `CWaveFile::Read @ 0x0044DE10` is a 202-byte EAX/EDX-bound read helper with two stack arguments. Natural TH08/TH095-style `Read` and `FillBufferWithSound` bodies were adapted into maintained `src/ZWave.cpp`. In the target-bound `ProbePlayRoot` LTCG graph, a scoped `inline_depth(0)` around only the repeat-loop `ResetFile(false)` call leaves the first reset inlined and the repeat reset as the target direct call; canonical `zwave-csound-fill` now reproduces the complete 483-byte body and six target-bound linkage fields, while `CWaveFile::Read` remains canonical exact through SOUND-015. The canonical `zwave-csound-play` linked-PE unit still reproduces the complete 145-byte `CSound::Play @ 0x0044D440` extent and all five child REL32 fields. The diagnostic root is compiler-context evidence, not a claim that the original retail source exposed that public entry. | Direct IDA Pro MCP disassembly/decompile at `0x0044D110`/`0x0044DE10`/`0x0044D440`; adjacent-game source used only for hypothesis; maintained `src/ZWave.cpp`; canonical `zwave-csound-fill` and `zwave-csound-play` cold replays; pinned VC7.1 SP1 build6030 |
| SOUND-014 | observed / source-shape-correction / canonical-linked-exact | `SoundPlayerView::StopBgm @ 0x0043DAB0` is a complete 176-byte owner cleanup body. The target keeps the owner in ESI, stops the active `CSound`, posts `WM_QUIT`, waits for the BGM thread, closes the thread and update-event handles, and destroys the active BGM object. The final target branch skips both deletion and owner-slot clearing when the BGM pointer is null; the maintained source therefore uses a natural null-guarded `delete` block. With `SoundPlayerView::ReopenBgm` as the real `/GL` entry and `src/ZWave.cpp` support, the canonical linked-PE unit reproduces the complete PDB contribution and all four target-bound linkage fields (the `CSound::Stop` REL32 and three Win32 import DIR32 fields) in two independent cold replays. This establishes the source mapping and exact codegen for the bounded body without claiming that `ReopenBgm` itself is exact. | Hash-attested TH10 disassembly and target caller graph; maintained `src/Sound.cpp`; diagnostic and canonical linked-PE replays; pinned VC7.1 SP1 build6030 |
| SOUND-015 | observed / source-shape-correction / canonical-linked-exact | `CWaveFile::Read @ 0x0044DE10` is a complete 202-byte EAX/EDX-bound helper with `buffer` and `bytesRead` on the stack and `RET 8`. Direct target CFG review shows the memory-backed branch, a shared not-initialized epilogue, then a distinct file-mode `else` branch whose validated path stores `*bytesRead` directly after `ReadFile`; the maintained source now preserves that natural control-flow shape. The canonical `zwave-wave-file-read` linked-PE unit uses `CSound::FillBufferWithSound` as the real `/GL` entry, reproduces all 202 bytes and the imported `ReadFile` DIR32 field, and passes two independent cold replays. The caller body is now canonical exact through SOUND-013. | Hash-attested TH10 disassembly/decompile; maintained `src/ZWave.cpp`; diagnostic CFG probe and canonical linked-PE replays; pinned VC7.1 SP1 build6030 |
| SOUND-018 | observed / canonical-linked-exact plus source-present helper | Natural `CSoundManager::CreateStreamingFromMemory @ 0x0044CBF0-0x0044CE7A` is mapped in ZWave.cpp. Its TH10 body checks the DirectSound interface at manager+0, constructs a 0x94-byte CWaveFile with format/data/size fields, creates a DirectSound buffer from a 0x24-byte description, queries IID_IDirectSoundNotify, allocates and installs eight-byte position-notification rows, releases the notify interface and array, then constructs a 0x78-byte CStreamingSound and stores the 0x24-byte description plus manager/event/lock fields. The reviewed private ABI receives output in ECX, creation flags in EDX and 11 stack dwords with RET44. A natural /GL /GS /EHsc standalone diagnostic emits 648/651 bytes; as ZWave support under the real SoundPlayerView::LoadBgm entry it emits 647/651. Neither grants helper exactness; the 1419-byte queue caller and the streaming-sound constructor graph remain incomplete. In that same LoadBgm entry with ZWave support, `SoundPlayerView::ReopenBgm @ 0x0043D790` emits the complete 55-byte target body, including the two correctly typed target-bound calls to GetFmtIndexByName and CWaveFile::Reopen. Two independent cold VC7.1 SP1 linked-PE replays verify all 55 bytes and both REL32 fields. The lookup stays non-exact at 236/227 and LoadBgm at 317/298. The full changed-source Sound/ZWave replay keeps 17 accepted units exact, 2034/2034 bytes across nine artifacts. TH09/TH095 source shaped the helper candidate only; all accepted TH10 facts come from target-local evidence. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| SOUND-020 | observed / canonical-linked-exact | CStreamingSound::InitSoundBuffers at 0x0044CF20 owns all 338 bytes through 0x0044D071, followed by fourteen CC bytes. Complete raw review decodes 133 instructions and all three return paths; both Release blocks omitted by provisional Ghidra flow are live. Natural source clears playing state, releases/nulls prior buffers, guarded-deletes/nulls the old pointer array, creates a new uninitialized array, then creates each DirectSound buffer, queries IID_IDirectSoundNotify and installs sixteen notification rows. Row offsets are notifySize*(index+1)-1. Failure cleanup is intentionally asymmetric: creation/query failures return E_FAIL without rollback; notification allocation failure returns E_OUTOFMEMORY without releasing notify; SetNotificationPositions failure releases/nulls notify and deletes the notification array before E_FAIL. Success does that same notify/array cleanup and returns S_OK after all buffers. No new pointer-array allocation check, zero-initialization or safety rollback is introduced. Normal COFF emits 341 bytes; the real ProcessQueues /GL /GS /EHsc graph naturally selects the target ESI receiver and reproduces all 338 bytes without source ABI annotations. Independent review verifies five new/delete REL32 fields and the GUID DIR32 from target allocation wrappers, SDK IID payload and candidate map; PE/map/PDB uniquely identify module2 support-00-ZWave.ltcg.obj. The canonical unit keeps Sound.cpp primary with ZWave.cpp support and pdb_source=src/ZWave.cpp, already supported by existing tooling. Two cold links reproduce the full owner and all six fields with zero differences. All seventeen related old Sound/ZWave units pass2034/2034 across nine artifacts, including the two Sound-primary dependents of ZWave and the independent thread-launch unit. Original production ownership/profile, complete caller exactness, data-owner closure and runtime remain independent; Factory acceptance is a separate receipt gate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| SOUND-021 | observed / source-present-non-exact / bounded-controls | SoundPlayerView::PreloadBgm owns 369 bytes at 0x0043D7D0 through 0x0043D940, with target ESI receiver, EAX path, stack index and RET4. Natural source and the existing ordinary member declaration recover the real ProcessQueues context without ABI annotations. An existing allocation plus matching receiver filename returns zero; otherwise strcpy publishes the name through the global sound owner before the option-bit/manager gate. Only the prior allocation is freed and cleared; old format/data/size remain until success. CreateFileA opens the receiver unknownNameSlot, the format helper selects a 0x34-byte row, SetFilePointer uses its startOffset without a BGM-base addition, malloc and ReadFile use its requested preloadAllocSize, and read/seek/close outcomes are unchecked. Failed open or allocation returns minus one; success publishes format, allocation, data and requested size, not bytesRead. The fresh normal diagnostic is 373/369; the real ProcessQueues /GL /GS /EHsc Sound+ZWave graph is 369/369 with 323/329 ordinary bytes equal. Six differences at offsets 0x11B,0x121,0x122,0x127,0x12A,0x12B exchange the ReadFile setup scratch registers: target table ECX/size EDX/output address EAX versus candidate EDX/EAX/ECX. Ten linkage identities and receiver/global addends were reviewed independently, but no canonical exact unit or matches row is added. Named read-size, earlier bytesRead declaration and named output-pointer controls are byte-neutral; a late format-pointer local, without changing publication order, regresses to 367 bytes and 276/329 ordinary agreement. All controls are restored to the selected 369-byte candidate. All seven affected canonical units pass 957/957 bytes across six artifacts, including the accepted 338-byte initializer. Full source presence is distinct from codegen exactness, original production ownership and runtime. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| SOUND-022 | observed / source-present-non-exact / bounded-CFG-controls | CStreamingSound::HandleWaveStreamNotification owns all 637 bytes at 0x0044D850-0x0044DACC. Complete raw decoding includes the internal three-byte LEA alignment at 0x0044D9ED omitted by Ghidra, all 238 instructions and the final null-failure tail; three CC bytes precede Reset. Ordinary member source and the real worker context preserve the private stack receiver plus BOOL/RET 8 contract without annotations. The first cursor query is unchecked; unsigned nextWriteOffset exclusion uses writeCursor-notifySize. Restore success refills with FALSE and normalizes nonnegative result to S_OK. Lock initializes only both output pointers; its second nonnull buffer returns E_UNEXPECTED without Unlock. Read/Reset failures also omit Unlock; looping short reads ResetFile(true) before every refill and preserve a possible zero-read infinite loop. Silence is 0x80 only for 8-bit format. Unlock is unchecked; the later cursor query propagates failure, unsigned progress wraps, direct COM Stop is unchecked, and the next offset advances by actual first locked length rather than notifySize. The initial 634-byte candidate has frame 0x18. A separate later play-cursor local recovers target frame 0x1C and initial stack homes; this clear natural source is retained at 634/637. Inner nonloop-first and outer positive-silence controls each emit 637 but arrange the read/nonloop-fill/latched-fill/repeat blocks differently. Guarded do and inverted shared-join controls are byte-neutral to nonloop-first; the positive shared-join control is byte-neutral to the selected 634 candidate. All five CFG controls are restored. Independent review finds no further distinct bounded CFG hypothesis; full codegen, original ownership and runtime remain open. No refill bytes enter matches.csv. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| SOUND-023 | observed / canonical-linked-exact / real-refill-caller | SoundPlayerView::BgmPlayerThread at 0x0043E3A0 owns 189 bytes through 0x0043E45C. Its address is passed to CreateThread by the maintained LoadBgm owner; ordinary static DWORD WINAPI(LPVOID) preserves the ignored callback parameter and RET 4. The worker waits on global BGM event+0x520C with mask 0xBF expressed by named queue flags, tests global BGM+0x5208 and playing+0x30, writes lock+0x74, calls the real refill with TRUE, reloads global BGM and clears lock. It ignores refill status, drains every queued message after WM_QUIT without Translate/Dispatch, and keeps the target wait-error behavior. Minimal Sound view splitting exposes the already-proven BOOL playing field while preserving sizes and offsets. Independent review binds all seven fields: two USER 32 IATs, four g_MainSoundOwner references with 0x5208/0x520C addends, and the actual refill symbol with matching private ABI. Both canonical cold links reproduce the full 189-byte primary source.ltcg.obj contribution with zero differences under the ProcessQueues Sound+ZWave /GL /GS /EHsc graph. The unit shares the existing SoundInitBuffers artifact because identical build groups must share an output; the first invalid separate-output recipe was rejected before compilation, then narrowly corrected without changing existing units. All 18 affected prior units pass 2372/2372 across 10 artifacts. This small caller was recovered as required context for the 637-byte medium owner, not as a tiny-owner queue. Refill, PreloadBgm and ProcessQueues remain non-exact; no partial credit is assigned. Factory acceptance remains a separate receipt gate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| SOUND-024 | observed / source-present-non-exact / data-owned | SoundDataLoaderThread at0x0043D080 owns107 bytes through plain RET at0x0043D0EA. The existing launcher passes an unused API argument through an explicit function-pointer cast; the body has no stable exit-code contract. Target state2 cancels before each of37 archive reads, publishes each result includingNULL at owner+0x5230 before failure logging, and waits via Sleep(1) while state+0x5224 remains0 after all reads. It never writes state or frees allocations. Existing sound consumers establish these offsets in the0x52D0 view. The37 mutable filename-pointer slots and CP932 error format now have ordinary semantic source definitions; linked string contents and order agree with target tables0x474B40/0x46FDD4. No duplicate tracked data owner was found. Genuine launcher plus FileSystem, GameErrorContext, AnmManager and PbgArchive visibility preserves the out-of-line EAX-path/RET8 archive seam and ECX logger seam. Candidate112/107 remains non-exact: byte-offset ESI plus error index EDI replace the target single ESI index. Moving the error handler out of the loop is byte-neutral and reverted; no speculative induction variants are retained. Independent source/ABI review passed. Data source ownership does not establish complete runnable product closure. | Fresh target hash/boundaries/callers and37-string data comparison; focused VC7.1 source diagnostic on2026-10-14; Git history |
| SOUND-025 | observed / canonical-linked-exact / worker-join | MainJoinSoundThread at0x0043CC40 owns175 bytes through0x0043CCEE. Real WinMain shutdown passes the ESI sound owner after setting cancellation state2. A NULL first handle skips all work; otherwise state0 becomes1, two handles at+0x5218/+0x521C are waited in order using100ms and Sleep(1) only on WAIT_TIMEOUT, both are closed, then both cleared. Other wait results and close results are ignored as in target. Ordinary source recovers zero return/plain RET. Two canonical cold Sound+Main /GL /GS links reproduce175/175 bytes and three IAT fields. The initial underscored linker-entry spelling was invalid: VC7 redecorated it into an unresolved data anchor. Correct WinMain@16 selects actual _WinMain@16 at PE RVA0x1CE0, matching executable map and maintained PDB contribution. The shared cold-link path now rejects unresolved entry anchoring and requires PE/map/PDB source-entry agreement; nine regression tests run in CI. Standalone preexisting-image comparisons remain entry-unaware unless this guard is explicitly applied. Initial name-hint diagnostic selection rejected C-linkage WinMain before linking; no checker relaxation was used. Factory acceptance is separate. | Independent full target/source/ABI/IAT review and entry-repair review; two corrected canonical cold replays on2026-10-14; canonical manifest |
| SOUND-026 | observed / canonical-linked-exact / sound-owner-release | MainReleaseSound at0x0043D120 owns301 bytes through0x0043D24C, with EAX owner and zero result/plain RET from real WinMain after joining workers. It frees format storage, releases duplicate before primary COM buffers for128 slots, and frees37 loader allocations. A NULL manager skips the remaining cleanup, including16 preload allocations. Otherwise it kills timer1, calls StopBgm, clears the DirectSound alias, unconditionally calls initial-buffer Stop, reloads/releases that buffer, rechecks/deletes BGM, and rechecks/deletes manager before freeing preloads. The manager destructor caches its object across member Release, nulls the member, then scalar-deletes the cached object. Both maintained CSoundManager declarations retain the same inline destructor. No extra null guard, rollback or cleanup is invented. Independent source/ABI/binding review and two guarded canonical cold links reproduce301/301 bytes with6 fields: three free REL32s, KillTimer IAT, StopBgm REL32 and scalar-delete REL32. Entry PE/map/PDB agree on real WinMain. Factory acceptance remains separate. | Fresh target hash and complete decode; independent review; canonical sound-release-owner unit; two cold replays on2026-10-14 |
| SOUND-027 | observed / setup-thread-exact / initialization-source-prerequisites | The game-specific57-byte SoundSetupThread at0x0043CCF0 ignores its API argument and initializer result, waits via Sleep(1) while global state+0x5224 is0, then writes finished1 at+0x522C. The real MainStartSoundThread seam publishes window+0x5228 and creates this callback after WinMain clears the owner. Two guarded WinMain canonical cold replays reproduce57/57 bytes and all7 fields. Plain RET is target behavior, not portable WINAPI proof. New Initialize(HWND) source preserves its786-byte target contract, including the cached newly allocated manager across Initialize/format calls, later owner reloads, fixed0x8000 clear after Lock, ignored format/Unlock/Play/timer results and global sample/cancellation access. Its checkpoint candidate was753/786 and non-exact; sample source and private ABI were subsequently recovered in SOUND-028. The260-byte primary-format prerequisite is structural-equal but gets no exact/authored credit: official [Microsoft SDKsound.cpp](https://github.com/microsoft/DirectX-SDK-Samples/blob/main/C%2B%2B/DXUT/Optional/SDKsound.cpp#L70-L103) corroborates SDK lineage and success-only primary-buffer release, not the exact TH10 version. Existing origin/denominator stays unchanged pending classification audit. Both initialization diagnostic strings are source-defined and target-equal. g_MainSoundOwner storage remains unresolved and the diagnostic image remains nonrunnable; distinct existing CSound/CWaveFile partial-view ODR debt is not closed. | Independent target/source/control/storage review; official SDK provenance; target string checks; canonical sound-setup-thread unit and two cold replays on2026-10-14 |
| SOUND-028 | observed / chunk-scanner-exact / sample-loader-non-exact | FindWaveChunk at 0x0043D250 owns 71 bytes through 0x0043D296. Its real LoadSoundSample calls at 0x0043D4BA and 0x0043D4F0 pass ECX data, EAX unsigned remaining, EDI output-size pointer and one stack tag; RET 4 owns cleanup. Ordinary __stdcall source recovers that private ABI. The helper publishes each chunk size before strncmp, subtracts size plus eight with unsigned wrap and advances literally, with no padding rounding or malformed-input repair. The target callee 0x00452F80 is independently identified CRT strncmp; the initial memcmp draft was corrected, not retained. Two guarded real-WinMain canonical cold links reproduce all 71 bytes and the single REL32 at +0x1A. The 684-byte sample loader at 0x0043D390 now has natural source and genuine EDX-owner/ECX-index/stack-path/RET4 ABI: manager-null returns zero, old primary buffer releases before waiting, Sleep(10) precedes global state2 cancellation, allocation is consumed and slot cleared, RIFF/WAVE checks use strncmp, RIFF size is reduced by 12, and the format copy is exactly 18 bytes. It creates a buffer with flags 0x80C8, copies both Lock spans and ignores Unlock status. Failure frees archive data but leaves any created COM buffer published; cancellation returns zero before acquisition. Initial diagnostic was 679/684 bytes, 189 raw matches and 177/584 comparable. The target-evidenced advancing serialized cursor (allocation plus four, read length, then advance four) restores 684/684 bytes, 605 raw matches and 576/584 comparable in two cold diagnostics, complete normalization. Remaining differences are ADD EBP,-12 versus SUB EBP,12 at +0x11A and the MOV EAX,EBP / LEA EDI,outsize scheduling at +0x122. A negative-increment spelling control was byte-neutral and reverted; stop these variants. Initializer is 754/786. Neither receives partial exact credit. The helper is 68 raw matches and 67/67 comparable before canonical binding replay. Sound setup remains structurally equal in this graph. Error strings have semantic CP932 definitions. Global sound-owner storage, runnable closure and wider partial-view ODR debt remain open. | Fresh hash-attested target, real caller graph, independent review and two canonical helper cold replays on 2026-10-15; canonical sound-find-wave-chunk manifest |

## MIDI

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| MIDI-001 | observed / canonical-linked-exact | TH10 retains `.\src\core\midilib.cpp` diagnostics. `MidiOutputView::UnprepareHeader @ 0x0043AEB0` scans the 32 pending MIDIHDR pointers at +0x14, clears the matching slot, calls `midiOutUnprepareHeader` on HMIDIOUT +0x13C, frees and clears `lpData` when present, then frees the header. Maintained `StopPlayback @ 0x0043AE20` is the same-TU /GL caller and establishes the optimizer context without being exact itself. Two independent cold builds reproduce all 79 bytes and the one MIDI-IAT plus two `_free` fields exactly. StopPlayback remains source-present/non-exact; child exactness is not promoted to caller exactness. | Retained midilib.cpp path; hash-attested target behavior; maintained `src/Midi.cpp`; canonical `midi-unprepare-header` linked replay; pinned VC7.1 SP1 build6030 |

## Text

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| TEXT-001 | observed / canonical-normal-coff-exact | TextRenderBufferApplyAlphaBleed at 0x00436DA0 owns all 946 bytes through 0x00437151; fourteen CC bytes follow. The reviewed authored body has no calls, globals, jump tables or relocations. It receives owner and processed-row count on the stack, returns with RET 8 and always sets AL to one. Formats 0x15 and 0x1A process transparent pixels using nontransparent left/right/top/bottom RGB neighbors. The cursor advances continuously across rows, while the lower-neighbor bound uses the full owner height. Vertical offsets divide signed pitch by four or two with truncation toward zero. Unsigned channel sums divide only when the neighbor count exceeds one; the 16-bit path then halves every channel before writing its four-bit field. Alpha remains unchanged. Partial owner offsets 0x100/104/108/110/120 and the callee-clean spelling are TH10-supported views, not original class/TU claims. Natural chained channel initialization, staged channel halving and cursor-before-index advancement reproduce all 946 raw bytes in two strict normal-COFF cold builds and an independent /GL linked diagnostic. The initial array initializer emitted 955 bytes; member spelling, fastcall helpers and component alias removal were neutral, while chained initialization alone emitted 930 and staging alone 948. Combining the two emitted 946 with eleven increment-order bytes different; the final natural loop order closes them. No existing canonical source graph uses this new TU/header, and no old recipe changes. Factory acceptance is a separate receipt gate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| TEXT-003 | observed / canonical-linked-exact plus source-present allocator | TextHelperView::CreateTextBuffer at 0x00437A00 owns 781 bytes through 0x00437D0C; TextRenderBufferView::ReleaseBuffer at 0x00436A30 owns 107 bytes through 0x00436A9A. The initializer tries 1024x64 format 0x1A, retries 0x15 on failure without checking the retry, fills a 256-byte prefix using GetRandomU16 shifted by nine, then creates 15 fonts at heights 32 through 60 with the target GDI arguments and CP932 MS Gothic face. The face bytes including terminator are 826c827220835383568362834e00 at target 0x0046F36C. Cleanup returns false without mutation for a null DC; otherwise it restores/deletes the GDI objects, clears width/height/handles/pixels, sets format to -1 and leaves imageSize/pitch untouched. Ordinary member source naturally lowers to ESI cleanup and EAX allocator receiver contracts in the TextRenderer plus unchanged RandomMath /GL caller graph. Two canonical cold links reproduce both full PDB-owned extents with all 40 initializer and three cleanup linkage fields independently bound, including the RNG counter at g_RngView+4 despite diagnostic anchor overlap with font17. The two PE hashes and PDB GUIDs differ; both full comparisons have zero differences. The only affected existing exact unit, text-buffer-apply-alpha-bleed, cold-replays all 946 bytes. The required TryAllocateBuffer at 0x00436AF0 is source-present but remains non-exact at 421/421 bytes with 33 ordinary store-scheduling differences. Its native SDK BITMAPV4HEADER is 108 bytes, signed pitch division truncates toward zero, DIB height is -(height+1), and the extra DIB row is not included in the cleared imageSize. Format lookup returns the sentinel row for unknown formats but null for requested -1; compression/masks and unchecked GDI failure paths follow the target. Height-before-width, pixel-pointer type and conventional parameter-name controls were restored. No partial allocator or rasterizer credit is granted. Original class/TU/profile, native product and runtime closure remain open; Factory acceptance is separate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |
| TEXT-004 | observed / canonical-linked-exact | TextRenderBufferView::TryAllocateBuffer at 0x00436AF0 owns all 421 bytes through 0x00436C94, followed by eleven CC bytes. A real HGDIOBJ local and coherent successful-resource publication order close the 33 ordinary tail differences from TEXT-003; the local-only control is neutral. The winning order was an adjacent TH095 source-shape hypothesis, then accepted only against TH10 target evidence. API order, fields, arithmetic, SDK header, false exits and unchecked GDI behavior are unchanged. No observer/call occurs between the reordered nonvolatile success stores. Independent review identifies one complete PDB contribution, 130 decoded instructions, internal branch destinations and the private EAX receiver/RET 0xC contract naturally emitted from the existing initializer graph. Actual PE relocations plus decode identify exactly six DIR32 fields and one cleanup REL32. Target import descriptors independently identify CreateDIBSection, CreateCompatibleDC and SelectObject; separately read target format records establish the three table-base references. Applying only those seven semantic bindings gives complete equality. Two canonical cold linked builds reproduce all 421 bytes and retain the existing 781/107 owners; normal-COFF protection retains all 946 bytes, totaling 2255 affected bytes. No signed-imageSize trial was needed or performed. No boundary/origin/denominator changed. Original class/TU/profile, native product and runtime closure remain open; Factory receipt acceptance is separate. | TH10 target-bound review; canonical manifest and replay remain authoritative for exact units; prior scratch receipts were pruned |

## GUI

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| GUI-001 | observed / source-present | `GuiView::UpdateStageElements @ 0x00414900` is the central TH10 stage-HUD update owner and `GuiMessageVmView::Run @ 0x00415E90` is its complete stage-message interpreter. The exact `0x9ED0` GUI view includes the six constructor-proved embedded VM arrays, the separate boss-position VM, managed ids, boss gauge state, four interleaved gauge values, message pointer, stage/front ANM resources, spell-second state and ending counter. The exact `0x90` message view contains three timers, six VM ids, current instruction, two text positions, input cooldown, flags, alternating-line/side state and two colors. Its variable-sized instruction header is `u16 time`, `u8 opcode`, `u8 payloadSize`, then payload. The maintained interpreter covers all opcodes `0..23`, including wait/skip behavior, portrait/text control, target stage-specific sprite and enemy-name maps, stage events and completion. The updater preserves target side-panel and boss-gauge position thresholds, `0.025f` gauge interpolation, four-phase boss thresholds and the distance-based boss marker alpha. Both entries consume one pointer on the stack and return with `RET 4`. | Hash-attested target disassembly and table bytes; Factory-native Ghidra decompile/calls/xrefs; constructors `0x00413810/0x00415B00`; maintained exact-offset layouts and source; `scripts/report-gui-core.py` |
| GUI-002 | observed / physical-table-ownership | The reviewed physical owner for `0x00414900` is `0x00414900-0x004157FB` = 3,836 bytes: code ends at `0x004157DC`, three alignment bytes follow, and the seven-entry boss-gauge stage selector occupies `0x004157E0-0x004157FB`. The reviewed physical owner for `0x00415E90` is `0x00415E90-0x0041700F` = 4,480 bytes: code ends at `0x00416F61`, two alignment bytes follow, and 43 table pointers occupy `0x00416F64-0x0041700F`. The latter split exactly into a 24-entry opcode table, seven enemy-portrait stage cases, seven enemy-name stage cases and five difficulty bonus cases. Every selector points inside its owning code body and every partition has distinct destinations. | Direct canonical target bytes and branch operands; adjacent independent function boundaries; target-bound `scripts/report-gui-core.py --check` |
| GUI-003 | observed / canonical-linked-exact | `GuiSetLivesDisplayCount @ 0x00413790` takes the GUI owner in EAX and count in ECX, sets flag bit one in `count` consecutive `0x3AC`-stride life VM slots, then clears that bit up to slot nine when `count < 9`. Four code references come from three functions: calls at `0x004188C3`, `0x00418925`, `0x00426A3F`, and a tail jump at `0x00424E9C`. The canonical linked-PE unit uses `PlayerResetRuntimeState @ 0x00424D90` as its primary entry and maintained `src/Gui.cpp` as a support source. Two independent cold VC7.1 SP1 build6030 links reproduce the complete 77-byte PDB-owned contribution, fully decoded and raw-equal with zero linked fields. The other target callers `0x004188A0` and `0x004269D0` remain source-unmapped. Exactness is scoped to the declared compiler context; original physical TU remains unknown. | Hash-attested TH10 target boundary and callers; Factory-native Ghidra xrefs; two cold `gui-set-lives-display-count` canonical linked-PE replays; pinned VC7.1 SP1 build6030 |
| GUI-004 | observed / canonical-linked-exact | `GuiScoreView::Add @ 0x00409D90` is a 46-byte ordinary `__thiscall` helper on the object based at `0x00474C40`. It adds signed `amount / 10` to the dword at `this+4`, stores the sum, and when the sum is at least `1000000000` replaces it with `999999999`. Factory-attested direct callers are `EnemyRuntimeUpdate @ 0x0040DC80`, `GuiMessageVmView::Run @ 0x00415E90`, and source-unmapped `0x0041AFD0`; the GUI caller and the pre-existing maintained `GuiScoreView` make `src/Gui.cpp` a caller-supported maintained placement without proving the original production TU. The initial natural spelling `score > 999999999` produced the correct 46-byte `/GL` extent but differed in only three bytes: `CMP 999999999`/`JLE` instead of the target `CMP 1000000000`/`JL`. Rewriting the mathematically equivalent condition as `score >= 1000000000` produces the target instruction sequence exactly. Two independent cold canonical linked-PE replays under real `GuiMessageVmView::Run` entry context reproduce 46/46 relocation-free bytes raw-equal. | Hash-attested target extent; Factory-native Ghidra caller/decompile/disassembly review; diagnostic 43/46 then 46/46 linked probe; two cold `gui-score-add` canonical replays; pinned VC7.1 SP1 build6030 |

## Replay

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| REPLAY-001 | observed / corroborated | `0x00429B60-0x0042A1F0` is a reviewed 1,681-byte authored replay-save writer. `0x00429B54-0x00429B5F` and `0x0042A1F1-0x0042A1FF` are CC padding. Two direct TH10 callers at `0x00423F16` and `0x0043399D` load the replay owner from `0x00477838` into ECX, place the formatted replay path in EDX, push one replay-name pointer, and ignore the return value; the callee ends with `ret 4`. The target also retains `src/game/replay.cpp`/`.h` source strings and replay allocation/type labels. `ReplayManager::SaveReplay` is retained as a maintained descriptive name after TH10-local recovery and committed adjacent corroboration, not as an original symbol or source-level calling-convention claim. | Direct TH10 caller/body/tail disassembly; attested function/caller/xref evidence; target replay source/type strings; committed TH08/TH09/TH095 source only for later naming corroboration |
| REPLAY-002 | observed | The replay-save and replay-load paths establish a 0x64-byte replay-data header and 0x1C4-byte per-stage header with signed stage index at `+0x00`, six-byte record count at `+0x04`, and packed payload size at `+0x08`. Runtime frame storage has 3,600 six-byte records through `+0x545F`, record-end pointer at `+0x5460`, 3,600 one-byte FPS samples at `+0x5464`, FPS-end pointer at `+0x6274`, and an embedded 0x0C intrusive list node at `+0x6278`; `0x0042AA50` allocates exactly `0x6284` bytes and initializes the node value to the frame base. The writer serializes only the record/sample streams, while `0x0042A200` independently reconstructs record/fps cursors from the same `recordCount * 6` relationship. | Full bounded TH10 writer/load disassembly; raw `0x0042AA50` allocation/init body; target `ReplayRecDataInf`/`ReplayFrameDataInf` strings |
| REPLAY-003 | observed / corroborated | After packing, the replay-save body calls reviewed `CompressData`, applies `FileSystem::Encrypt` with `(seed, increment, chunk) = (0x3D,0x7A,0x80)` and then `(0xAA,0xE1,0x400)`, stores uncompressed/compressed sizes at replay file-header `+0x20/+0x1C`, and sets user-data offset `+0x0C` to `compressedSize + 0x24`. It opens `replay/%s`, writes the 0x24-byte file header and compressed payload, then emits two aligned `USER` blocks from a 0xFFFF-byte scratch buffer: type 0 contains version/name/date/character/rank/stage/score/slow-rate text and type 1 contains the retained comment text. Short-write paths close the handle and release the write critical section. Committed TH095 shows the same file-header/encryption/USER family, but TH10's eight-stage front half and all offsets here are target-local facts. | Direct TH10 raw disassembly/imports/format strings plus reviewed CompressData/Encrypt bodies; committed TH095 source used only after TH10 behavior recovery |
| REPLAY-004 | observed / corroborated | The reviewed replay lifecycle cohort is `0x004294A0-0x0042960E` (`ReplayManager::~ReplayManager`, 367 bytes), `0x00429610-0x004296C8` (`ReplayManager::Create`, 185), `0x004296F0-0x004297A4` (`ReplayManager::Load`, 181), `0x004297B0-0x004297C1` (`ReplayManager::Destroy`, 18), and `0x004297D0-0x00429A23` (`ReplayManager::ProcessFrame`, 596). The maintained names were selected only after TH10-local lifecycle/callsite behavior was recovered and are corroborated by committed adjacent ReplayManager source; they are not original-symbol, source-ABI, TU, or physical-owner claims. A previously untracked CC-delimited body at `0x004296D0-0x004296E8` has the MSVC scalar-deleting-destructor pattern and is classified compiler-generated/excluded, expanding the candidate denominator without expanding authored code. | Attested TH10 function/decompile/xref analysis; direct raw boundaries/calls; pinned VC7.1 deleting-helper synthesis; committed TH095/TH08 source used only for later naming corroboration |
| REPLAY-005 | observed | TH10 replay-object allocation/clear sites establish an exact maintained object size of `0x2D4`. Target-observed fields include mode `+0x10`, file header `+0x14`, replay data `+0x18`, eight stage-header pointers `+0x1C`, eight 0x0C intrusive frame-list roots beginning `+0x3C`, current frame link `+0x9C`, eight 0x24 stage states beginning `+0xA0`, decompressed payload `+0x1C0`, replay FPS byte `+0x1C4`, frame counter `+0x1C8`, a chain pointer `+0x1CC`, active stage `+0x1D0`, and 0x100-byte replay path `+0x1D4` through the object end. Each 0x24 stage state contains record/fps start+cursor pairs at `+0x00/+0x04/+0x08/+0x0C`, stage header `+0x10`, record index `+0x14`, and an embedded 0x0C intrusive node at `+0x18`. | Direct TH10 allocation/clear loops, raw `0x0042AC20/0x0042AC60` stage-state ctor/dtor bodies, `0x0042A200` load-state reconstruction, `0x0042AA50/0x0042AB20` frame-list allocation/free behavior |
| REPLAY-006 | observed | `ReplayManager::ProcessFrame` is target-locally a record/playback dispatcher. A wrapper at `0x0042A3D0` receives the replay owner in ECX, moves it to EDI, and calls `0x004297D0`. Record mode snapshots three u16 input fields into six-byte records, samples FPS every 30 frames using target constants `0.5f` and `256.0f`, and allocates a new 0x6284 frame block when the current block fills. Playback mode consumes the same six-byte records from the selected 0x24 stage state, reads the current FPS byte every frame while advancing its FPS cursor every 30 frames, clears inputs when the stream or active stage is unavailable, and advances record/frame indices. The private EDI machine ABI is not promoted to a source-level calling convention. | Attested decompile/xref plus direct wrapper/raw disassembly and target `.rdata` constants |
| REPLAY-007 | observed / corroborated | `0x00428F60-0x0042949F` is the reviewed 1,344-byte shared replay initializer mapped descriptively as `ReplayManager::Initialize`. `0x00428F5A-0x00428F5F` is CC padding after the preceding body, and the initializer ends at the `0x0042949F` RET immediately before the reviewed destructor at `0x004294A0`. Its sole caller loads the 0x2D4 replay owner in EBX, mode in EAX, and path in ECX; the body itself uses plain RET. Mode 0 publishes the global replay owner, initializes frame/header/stage storage and three chain elements; mode 1 loads replay data, restores runtime/current-stage state, installs the same chains, and leaves active stage `-1`; mode 2 performs load-only initialization without publishing the global owner or installing callbacks. The private EBX/EAX/ECX boundary is not promoted to a source calling convention. | Attested initializer function/decompile/caller/callee/disassembly results; direct raw pre/post boundaries; reviewed `ReplayManager::Create`, `LoadReplay`, frame allocation/clear context; committed TH095/TH08 used only after TH10 recovery for natural naming/source-shape corroboration |
| REPLAY-008 | observed / unknown-origin | Initializer code establishes a maintained 0x24-byte chain-element layout: dword priority `+0x00`, flag word/dword storage `+0x04`, callback/lifetime slots `+0x08/+0x0C/+0x10`, embedded 0x0C intrusive node beginning `+0x14`, and replay-owner argument `+0x20`. It installs calc callbacks with priorities `0x0B` and `0x1B` and a draw callback with priority `5`. Six previously untracked CC-delimited replay bodies were added to the candidate denominator but deliberately remain `unknown/review`: `0x00429A30-0x00429A69`, `0x00429A70-0x00429B25`, `0x00429B30-0x00429B53`, `0x0042A3D0-0x0042A3D9`, `0x0042A3E0-0x0042A423`, and `0x0042A430-0x0042A44B`. The three `0x0042A3xx` entries are active callback addresses written by the initializer; `0x0042A430` tail-jumps to `0x00429A70`. Source-written callback versus retained/LTCG adapter/clone origin is unresolved, so none receives authored or source-present credit. | Direct target raw boundaries; initializer callback-address stores; attested xrefs; callback/FPS-body disassembly; whole-target raw VA reference checks; committed TH08 chain source only for later abstraction corroboration |
| REPLAY-009 | observed / corroborated | `0x0042A200-0x0042A3C1` is the reviewed 450-byte replay loader mapped descriptively as `ReplayManager::LoadReplay`. `0x0042A1F1-0x0042A1FF` and `0x0042A3C2-0x0042A3CF` are CC padding, and all three direct call sites pass the replay owner in live-in ESI, push one replay path, and receive a callee-popped `ret 4`. The disk path builds `replay/%s`, opens a stateful replay read handle, validates file magic `0x72303174` plus version 5, and reads the compressed payload; the archive-selected path calls reviewed `FileSystem::OpenFile`. Both paths apply `Decrypt` with `(0xAA,0xE1,0x400)` then `(0x3D,0x7A,0x80)`, call `DecompressData`, and reconstruct per-stage header/record/FPS starts from the 0x64-byte replay header and 0x1C4-byte stage records. The target deliberately replaces `stageCount >= 8` with loop bound 6. | Attested TH10 function/decompile/caller/callee/xref results; direct target entry/tail/call-site disassembly; reviewed replay layouts and transform helpers; committed TH095 source only after TH10 behavior recovery for naming/source-shape corroboration |
| REPLAY-010 | observed / corroborated | `0x0042A450-0x0042A695` is the reviewed 582-byte replay stage-transition owner mapped descriptively as `ReplayManager::BeginStage`. Both direct call sites inside `0x00418190` load the global 0x2D4 replay owner into live-in EBX; the callee has no stack arguments and uses plain RET. The body marks each non-null replay chain element through flag bit 1. Record mode rotates the current stage frame list, conditionally refreshes stage-header scalar fields, then snapshots runtime coordinates, a 0x108-byte block, four groups of four 8-byte runtime pairs, `+0x1B8`, and runtime `+0x4474` into the 0x1C4 stage header. Playback mode restores the same snapshot, invokes the connected runtime rebuild/reset helpers, applies the target special-case `shotType + character * 3 == 5`, clears the four slot `+0x58` reset fields, and resets the replay frame counter. Ghidra reports 579 reachable body bytes because `0x0042A60D-0x0042A60F` is a retained three-byte alignment LEA skipped by the branch at `0x0042A60B`; the physical extent remains 582 bytes and is followed by CC padding at `0x0042A696-0x0042A69F`. The maintained `BeginStage` name is TH09-correlated only after TH10-local recovery; original TH10 symbol/TU remain unknown. | Attested TH10 decompile/caller/callee/xref results; direct raw entry/tail/call-site and state-copy disassembly; reviewed ReplayManager/stage layouts; committed TH09 BeginRecordingStage/BeginPlaybackStage used only for later source-family naming corroboration |
| REPLAY-011 | observed / source-fidelity-correction / non-exact | SaveReplay at 0x00429B60 owns 1681 bytes. Fresh independent target review found that each stage writes every linked node's records first at 0x00429D80-0x00429DDD, then restarts the list and writes all FPS samples at 0x00429DF0-0x00429E2F. The former per-node records/FPS interleave was incorrect for multiple nodes and is now replaced by these two passes. The four inline write sites now have maintained WriteToOpenFile source: invalid handle skips; WriteFile BOOL is ignored; short count reloads/closes the handle, leaves the critical section and decrements count without resetting/rechecking the handle. Repeated later cleanup is original target behavior, not a safe API recommendation. No separate target boundary or tiny-fragment exact credit is invented for this inline seam. The source-extracted copy-loop regression compiles/runs synthetic unequal two-node, one-node and empty cases on the host, explicitly rejects old interleaving, and propagates compile/run failures; missing host C++ is explicitly skipped. CI always checks the two-pass source shape. This is a bounded serialization test, not full-game runtime or historical-codegen proof. The guarded SaveReplay cold diagnostic is1748/1681 bytes,76 raw matches and60/1326 comparable, normalization complete; no exact credit. Its entry RVA0x24B0/PDB contribution is verified; opener137 remains structurally equal. Shared storage and the rest of SaveReplay remain open. | Target-attested review; independent delta/test review; test-replay-payload-order.py; focused cold diagnostic on2026-10-15 |

## Decompiled helpers

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| DECOMPHELP-001 | observed / external-hypothesis-audited | External commit `649147241f6a931c849d905ca9badc264049db5b` is useful hypothesis material, not wholesale source authority. Its natural C++ candidates, decompiler output, opcode-oriented transcriptions, and portable implementation are independent evidence classes. Durable acceptance exists only where TH10-local target review and canonical replay have been absorbed into maintained source and ledgers; unverified external material remains rejected or hypothesis-only. | External commit `649147241f6a931c849d905ca9badc264049db5b`; maintained source; `config/match-units.toml`; `config/matches.csv`; TH10-local target-bound replay |
| DECOMPHELP-002 | observed / canonical-linked-exact / external-repo-review | The repo-wide audit of external commit `649147241f6a931c849d905ca9badc264049db5b` accepts two natural C++ functions from its Chain candidate: `Chain::RemoveAllFromList @ 0x00449E50` (82 bytes) and `Chain::UnregisterElem @ 0x00449F60` (114 bytes). Direct IDA Pro MCP confirms the cleanup body's EAX list-base/private stack Chain ABI and `retn 4`, plus the Unregister calc/draw search, bidirectional unlink, callback clearing and `j__free @ 0x004524A1`; the latter has 53 caller sites. The canonical `/GL` units retain the full Chain TU under the `Chain::AllocElem` entry context. RemoveAllFromList requires the target-observed `__stdcall` declaration and seven explicit import/global/callee linkages; two cold VC7.1 SP1 build6030 links replay 82/82. Unregister's two cold links replay 114/114 after its `REL32 +0x68 -> _free` rebinding. Chain's other natural candidates remain open after size/ABI/context mismatches; all naked, unverified PDG, portable, port and unfinished salvage material remains rejected or hypothesis-only. | External commit `649147241f6a931c849d905ca9badc264049db5b`; direct IDA Pro MCP `0x00449E50`/`0x00449F60` and `j__free`; `decomphelp-chain-remove-all` and `decomphelp-chain-unregister` linked units and focused replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-003 | observed / canonical-linked-exact / external-repo-review | The external natural C++ `GameErrorContext::Log @ 0x0044B810` is accepted after direct IDA Pro MCP review and target-bound replay. The target is a complete 201-byte `/GS` body with ECX context, stack format/varargs, critical section `0x004922BC`, `g_LogLockDepth @ 0x0049231F`, `_vsprintf @ 0x004524A6`, bounded append and `char *` format-pointer return. The maintained source corrects the external `void` draft to a natural `self = this` alias and `char *` return; retaining `Fatal` in the same source gives the target register lifetime. Twelve import/global/runtime linkage fields replay zero-difference in two independent pinned VC7.1 SP1 build6030 cold links (201/201). | External commit `649147241f6a931c849d905ca9badc264049db5b`; direct IDA Pro MCP `0x0044B810`, callers and imports; `decomphelp-game-error-log` linked unit and two focused cold replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-004 | observed / canonical-linked-exact / external-repo-review | The external natural C++ `GameErrorContext::Fatal @ 0x0044B8E0` is accepted after direct IDA Pro MCP review and target-bound replay. The target is a complete 201-byte `/GS` body with a private EDI receiver, the same critical section/log-depth/vsprintf pipeline as `Log`, a fatal flag write at `+0x2004`, and a `char *` format-pointer return. The maintained source corrects the external `void` draft, and a source-local natural `ProbeFatalRoot` entry context preserves the target WPO register lifetime without claiming EDI as a public source ABI. Eleven linkage fields replay zero-difference in two independent pinned VC7.1 SP1 build6030 cold links (201/201). | External commit `649147241f6a931c849d905ca9badc264049db5b`; direct IDA Pro MCP `0x0044B8E0`; `decomphelp-game-error-fatal` linked unit and two focused cold replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-005 | observed / canonical-linked-exact / external-repo-review | The external natural C++ `GameManager::OnDraw @ 0x004187D0` is accepted after direct IDA Pro MCP review and target-bound replay. The target body is 31 bytes: it tests `GameManager + 0x58` bit 2, clears the shared ANM manager fields at `0x00491C10 + 0x50/+0x54/+0x4C/+0x58`, and returns one. The maintained source keeps the target-observed `__fastcall` receiver and one external `g_AnmManagerPtr` DIR32 field; two independent pinned VC7.1 SP1 build6030 cold linked-PE replays (`d85fe054c68ddda8a1e165595895f6657193af13858126385f30135fd2c423ad`, `8e5c4f7a6f3cec3a696f42d37ba42ac2c8e4bab3b217fc415af8b011afda1887`) are zero-difference (31/31). Other GameManager stubs/naked rows remain rejected. | External commit `649147241f6a931c849d905ca9badc264049db5b`; direct IDA Pro MCP `0x004187D0`; `decomphelp-game-manager-on-draw` linked unit and two focused cold replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-006 | observed / canonical-linked-exact / external-repo-review | Rechecking the external natural leaf TU against the current TH10 target promotes ten previously unclassified raw-exact extents: `Fn004050D0` (50), `Fn0040CC40` (7), `Fn0040CF40` (23), `Fn00412DB0` (50), `Fn00417800` (58), `Fn00424EB0`/`EC0` (3 each), `Fn00436640` (18), `Fn004452E0` (10), and `Fn004458A0` (7). Direct IDA Pro MCP confirms each reviewed boundary and body; the canonical linked replay of the complete `DecomphelpLeafAccessors.cpp` TU reproduces all 229 bytes with zero differences. Function-origin ledgers remain independent: the source artifact proves codegen exactness, while source authorship is still indeterminate except the existing reviewed game ownership of `0x00417800`. | Direct IDA Pro MCP target disassembly; maintained `src/DecomphelpLeafAccessors.cpp`; canonical units in `config/match-units.toml`/`config/matches.csv`; focused `replay-exact-units.py` replay; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-007 | observed / canonical-linked-exact / external-repo-review | The natural decomphelp leaf `th10::leaf::Fn00427D80 @ 0x00427D80` is now canonical exact for 21 bytes. Direct IDA byte review confirms the complete multiply/store body (`EAX` destination, `ECX` source pair, `EDX` scalar), while the target has no IDA function inventory entry or code xref; a pinned VC7.1 SP1 `/GL` replay of the complete leaf TU nevertheless produces a complete PDB-owned contribution with zero differences. Origin remains indeterminate and is not inferred from the exact codegen. | Direct IDA Pro MCP bytes/xrefs at `0x00427D80`; canonical `game-decomphelp-fulltu-00427d80` focused replay; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-008 | observed / canonical-linked-exact / external-repo-review | Direct IDA Pro MCP review plus canonical focused replay closes three natural decomphelp leaves: `Fn004175E0 @ 0x004175E0` (32 bytes), `Fn00418A90 @ 0x00418A90` (20 bytes), and `Fn0041C030 @ 0x0041C030` (98 bytes). The first preserves the target signed `jge` with an explicit `(i32)` comparison; the second naturally returns its receiver in EAX; the third preserves the target ordered vtable/flag stores with a natural `_ReadWriteBarrier`. Exactness is recorded independently of origin: the 41C030 initializer remains source-authored versus compiler-generated indeterminate. | Direct IDA Pro MCP disassembly; canonical `game-decomphelp-fulltu-004175e0`, `game-decomphelp-fulltu-00418a90`, and `game-decomphelp-fulltu-0041c030` focused replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-009 | observed / canonical-linked-exact / external-repo-review | `Fn00417010 @ 0x00417010` is now a canonical 41-byte exact leaf. Direct IDA shows the EAX-input encoded-text buffer transform and EAX return of `0x00497D40`; the natural pointer loop uses an intrinsic compiler barrier only to preserve the target store/update ordering, with no target-byte or ABI fabrication. Exactness remains separate from the reviewed game ownership and unresolved original TU. | Direct IDA Pro MCP disassembly/decompilation; canonical `game-decomphelp-fulltu-00417010` focused replay, raw-equal complete PDB extent; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-010 | observed / canonical-linked-exact / external-repo-review | Three previously byte-different decomphelp leaves now close under the complete TU replay: indexed setters `Fn0040C950 @ 0x0040C950` (4 bytes) and `Fn0040CD90 @ 0x0040CD90` (5 bytes), plus the Enemy record setter `Fn00412790 @ 0x00412790` (12 bytes). Direct IDA confirms the EAX/EDX/ECX private register seams; source parameter order follows those target-proven register roles, and all three have no direct callers, so origin remains indeterminate. | Direct IDA Pro MCP disassembly/decompilation and caller checks; canonical `game-decomphelp-fulltu-0040c950`, `game-decomphelp-fulltu-0040cd90`, and `game-decomphelp-fulltu-00412790` focused replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-011 | observed / canonical-linked-exact / external-repo-review | Two tiny decomphelp candidates with target-proven member ABI are absorbed as address-based identity views: `0x00401100` and `0x00401FA0` each decompile in direct IDA Pro MCP to `__thiscall` return-`this` bodies (`mov eax,ecx; retn`). Natural C++ member methods reproduce each complete three-byte extent when selected as the LTCG entry in independent cold replays. No xrefs or owner evidence was found, so origin and semantic ownership remain indeterminate. | Direct IDA Pro MCP disassembly/decompilation and xref review; canonical `game-decomphelp-identity-00401100` and `game-decomphelp-identity-00401fa0` replays; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-012 | observed / canonical-linked-exact / external-repo-review | Nine additional decomphelp candidates are absorbed as address-based identity views: `0x00405400`, `0x00413240`, `0x00427E10`, `0x00438400`, `0x0043BF90`, `0x004458D0`, `0x004458E0`, `0x004458F0`, and `0x00446210` each decompile in direct IDA Pro MCP to a one-argument `__thiscall` return-`this` body (`mov eax,ecx; retn`), with three target bytes and no xrefs. Natural C++ member methods reproduce each complete extent when selected as independent LTCG entries; original owner and authorship remain indeterminate. | Direct IDA Pro MCP disassembly/decompilation/xref review; focused VC7.1 probes and canonical linked units `game-decomphelp-identity-00405400`, `game-decomphelp-identity-00413240`, `game-decomphelp-identity-00427e10`, `game-decomphelp-identity-00438400`, `game-decomphelp-identity-0043bf90`, `game-decomphelp-identity-004458d0`, `game-decomphelp-identity-004458e0`, `game-decomphelp-identity-004458f0`, and `game-decomphelp-identity-00446210`; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-013 | observed / canonical-linked-exact / external-repo-review | Eleven additional natural decomphelp leaves close under independent canonical linked-PE replays: the __thiscall value setters at 0x00405370 (9 bytes), 0x0040CF00/10/20 (10 each), 0x0040CF80 (13), 0x00412F10/20 (10 each), flag clear 0x00405E10 (7), dword getter 0x00438250 (4), and identity helpers 0x004383F0/0x00438410 (5 each), for 93 exact bytes. Direct IDA Pro MCP confirms every complete boundary, ABI and zero-xref review; each natural member view was cold-linked as its own /GL entry and replayed zero-difference. Origin and original owner remain indeterminate. | Direct IDA Pro MCP disassembly/decompilation/xrefs; canonical focused replays of all eleven units; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-014 | observed / canonical-linked-exact / external-repo-review | Twenty-eight natural decomphelp leaves are now registered as exact: the signed compare at 0x00404E90 (15 bytes), float getter at 0x00404EC0 (4 bytes), and 26 existing natural accessors at 0x00401CA0, 0x00405380/0x00405490/0x00405BA0, 0x004088A0, 0x00409F60, 0x0040ACC0/0x0040ACD0/0x0040C530, 0x00412F30/40/90/B0/C0/D0, 0x004130F0/0x00413110, 0x00417610/20, 0x00421CD0, 0x00424610, 0x00428D60/0x00428F20, 0x0044BFF0/0x0044C760, and 0x00449860, totaling 165 canonical bytes. Direct IDA Pro MCP byte reads show each CC-delimited ret extent; the pinned VC7.1 /GL linked replay of the natural DecomphelpLeafAccessors.cpp TU is zero-difference for all 28 selected units. Most target entries lack an IDA-created function boundary, so origin, ownership and authorship remain indeterminate rather than being inferred from exact codegen. | Direct IDA Pro MCP raw-byte/tail review; focused `replay-exact-units.py` replay of all 28 decomphelp units; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-015 | observed / canonical-linked-exact / external-repo-review | Thirty-two additional natural decomphelp leaves (`0x00404CA0` through `0x0044C6D0`, sparse) are now canonical exact for 674 bytes. Direct IDA Pro MCP reviewed each complete byte/tail extent and found no entry xrefs; none had an IDA-created function boundary, so the boundary state remains reviewed/medium and origin remains unknown/indeterminate. The complete `DecomphelpLeafAccessors.cpp` pinned VC7.1 `/GL` linked replay is raw-equal for all 32/32 units. This is a target-bound codegen result, not wholesale acceptance of the external 606-leaf report. | Direct IDA Pro MCP byte/tail/xref review; canonical focused `replay-exact-units.py` batch (`game-decomphelp-fulltu-00404ca0` … `game-decomphelp-fulltu-0044c6d0`), 32/32 and 674/674; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-016 | observed / canonical-linked-exact / external-repo-review | `Fn00427E20 @ 0x00427E20` is now a canonical 23-byte exact leaf. Direct IDA Pro MCP shows a private-register pair copy (`EDX` input pair → `ECX` middle pair → `EAX` output pair) ending in `ret`; no code caller or owner evidence is present. Replacing the prior scalar temporaries with two natural aggregate `Pair` assignments reproduces the target's VC7.1 register schedule in the complete `/GL` leaf TU; the focused replay is raw-equal and uses no target-byte or ABI fabrication. Origin remains `unknown/indeterminate`. | Direct IDA Pro MCP bytes/disassembly at `0x00427E20`; canonical `game-decomphelp-fulltu-00427e20` focused replay; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-017 | observed / canonical-linked-exact / external-repo-review | Two previously target-size-missing leaf rows are now canonical exact: `Fn00417600 @ 0x00417600` (10 bytes, bit 4 of `+0x60`) and `Fn004501E0 @ 0x004501E0` (7 bytes, EAX/ECX dword setter at `+0x102C`). Direct IDA Pro MCP confirms both complete ret-delimited boundaries and no code callers. The complete natural `DecomphelpLeafAccessors.cpp` `/GL` TU replay is raw-equal for both units; origin remains `unknown/indeterminate`. The existing canonical `Fn00409F60` ledger row is likewise synchronized with its verified target extent. | Direct IDA Pro MCP disassembly/caller review at `0x00417600` and `0x004501E0`; canonical `game-decomphelp-fulltu-00417600` and `game-decomphelp-fulltu-004501e0` focused replay; pinned VC7.1 SP1 build6030 |
| DECOMPHELP-018 | observed / canonical-linked-exact / external-repo-review | Six natural decomphelp copy leaves are now canonical exact: `Fn00412D60/70/80/90` and `Fn0041AC20/30`, totaling 70 bytes. Direct IDA Pro MCP confirms each complete two-dword copy and `retn`, with no code callers. Natural scalar locals plus `_ReadWriteBarrier()` preserve the target first-store/second-load schedule; six focused linked-PE replays are raw-equal. Exactness remains separate from ownership and authorship, which stay `unknown/indeterminate`; no target bytes or fabricated ABI were used. | Direct IDA Pro MCP disassembly/caller review; focused `game-decomphelp-fulltu-00412d60`, `...d70`, `...d80`, `...d90`, `...0041ac20`, and `...0041ac30` replays; pinned VC7.1 SP1 build6030 |

## Small functions

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| SMALLFUNC-003 | observed / exact-plus-open-neighbors | `AnmRenderManagerView::SetVmPendingInterrupt @ 0x00449210`, `SetVmPendingInterruptAndExecute @ 0x00449250` and `SetVmPosition @ 0x004492F0` are now exact: spelling the four-byte ID as the maintained `AnmVmIdView` value type and retaining the target `__stdcall`/noinline boundaries gives the target stack-ID ABI; canonical replay covers all 54 + 76 + 90 bytes, including FindVm and ExecuteScript linkage fields. Only MarkLoadedVmsForDeletion remains open in this local group (82/92 with 18/92). The former CMemoryPbgFile::Open 51/46 diagnostic remains superseded by RESFILE-006. | Hash-attested target disassembly; maintained AnmManager source; canonical `anm-manager-set-vm-pending-interrupt{,-and-execute}` and `anm-manager-set-vm-position` linked replays; pinned VC7.1 SP1 build6030 |
| SMALLFUNC-005 | observed / canonical-linked-exact | AnmSetVmScriptIndexAndExecute @ 0x004496D0 is a complete 36-byte VM-id script restart wrapper with three real calls inside EnemyRuntimeUpdate. The old 42-byte free-helper and 40-byte manager-member diagnostics are superseded. Treating the four-byte managed id storage as the established AnmVmIdView removes an implicit conversion temporary; declaring the natural helper __stdcall lets VC7.1 /GL promote the first argument to target EAX while keeping the script index on the stack and RET 4. Canonical linked replay reproduces all 36 bytes and the g_AnmRenderManagerView, FindVm and SetAndExecuteScriptIdx fields. Source-scope regressions after the change are exact for Enemy.cpp (26 units / 1000 bytes) and AnmManager.cpp (90 units / 17186 bytes). | Hash-attested target body/call sites; canonical anm-set-vm-script-index-and-execute replay; source-scope replays; pinned VC7.1 SP1 build6030 |

## Startup

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| STARTUP-001 | observed / corroborated | `0x0041F990-0x0041FABA` is a Ghidra-missed, CC-delimited authored startup worker body. `0x0041FAC0` stores its address at an object field and passes it to `_beginthreadex` with the same object as thread argument; its unique caller `0x0041FD00` first allocates 0x3F0 bytes, invokes `0x0041F850`, and then calls the thread launcher. The worker loads `sig.anm` and `text.anm`, reports text-initialization failure, loads `../../bgm/thbgm.fmt`, checks `thbgm.dat`, and reports BGM-initialization failure. The target returns with plain `ret`, so source callback convention and object type remain unresolved; the 0x3F0 allocation is not used to infer a type. `Supervisor::StartupThread` is an adjacent-supported proposed name because corresponding TH08/TH09/TH095 startup functions own the text/BGM sequence, not an original-symbol claim. | Direct TH10 raw boundary/string inspection; attested caller/decompile chain through `0x0041FAC0`/`0x0041FD00`; committed TH08/TH09/TH095 startup source used only for naming/source-shape corroboration |

## References

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| REFERENCE-001 | observed / external-hypothesis-only | The shallow local clones `_reference/th10-switch` (`fd6de4b`, `v1.00a-r5`) and `_reference/th10` (`0074e58`, `portable`) are not byte-exact TH10 reconstructions. The switch README explicitly calls its game logic a clean C++ reimplementation rather than a decompiled EXE; it ships generated `wasm2c` plus SDL/OpenGL host code and contains no target hash or executable. The portable README calls itself source-only C++/SDL3/Web, uses C++17/Emscripten, and omits the original executable/data; its roughly 397 target-address comments and static layout assertions are semantic reconstruction hints, not VC7.1 linked-image evidence. Useful hypothesis sources are its ANM interpreter/interpolation, ECL command partition, GUI frame order, and Player movement/shooting logic; accept none without TH10-local IDA, ABI, and replay validation. | Local read-only review of `_reference/th10-switch/README.md`, `CMakeLists.txt`, generated `wasm2c/`, `_reference/th10/README.md`, `portable/build.mjs`, and `th10_web/cpp/game/*`; no target-bound byte comparison |
| REFERENCE-002 | observed / external-behavioral-hypothesis-only | The fetched upstream `main` tip of `_reference/th10` (`e8a939e11d989122eaf83f50cb2849a166a54392`; the local worktree remains `portable` at `0074e58`) is substantially richer than the portable branch but still not an exact reconstruction. Its own README calls the product a behavioral reimplementation; provenance says the standalone C++/Wasm game logic was reconstructed from local Japanese/Simplified-Chinese TH10 v1.00a resources, disassembly, and function/frame observations, with thprac plus v86/Unicorn used during development. It reports 96,000 original-comparison frames / 6,526,503 draw packets, 307 regression checks and seven replay consistency runs, while the replay ledger explicitly says it is not stepwise reconstruction; one retained Stage 5 failure was diagnostic-budget-only. The implementation uses WASI SDK 34 clang/LLD WebAssembly and SoftFloat, with project-authored names/layouts, no target PE/hash or VC7.1 linked-image evidence. Its address-annotated ANM, Enemy, FrontEnd/GUI and Player code is therefore a valuable semantic hypothesis/test-vector source only; no exactness promotion follows. | Local `git show` review of upstream `README.md`, `docs/SOURCES.en.md`, `docs/NATIVE-APPLICATION-VALIDATION.en.md`, `docs/REPLAY-VALIDATION.md`, `validation/*`, `source/cpp/game/*`, and `release.json`; no target-bound byte comparison |

## Randomness

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| RNG-001 | observed / exact-four-units | The eight-byte gameplay RNG view contains a 16-bit seed, saved seed and 32-bit generation counter. `GetRandomU16 @ 0x0044B9B0` applies `((seed ^ 0x9630) - 0x6553)`, rotates the top two bits into the low end through multiply-by-four arithmetic, increments the counter and returns the new seed. `GetRandomU32 @ 0x0044B9E0` concatenates two successive results. `GetRandomF32 @ 0x0044BB20` divides by `UINT_MAX`; `GetRandomF32Signed @ 0x0044BB90` divides by `INT_MAX` and subtracts one. Natural `RandomMath.cpp` under pinned normal VC7.1 reproduces all 341 bytes and all five constant relocations exactly. Boundary review additionally found an unreferenced instruction-identical 78-byte body at `0x0044BA30`; it remains origin-unknown and source-absent. | Complete target bodies and direct caller scan; normal COFF symbol extents/relocations; canonical cold replay of four units; TH095 consulted only for naming/source-shape corroboration |

## Game state

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| GAMESTATE-001 | observed / corrected-owner / canonical-linked-exact | TH10 global `0x00474C40` is modeled as a shared `GameScoreStateView` rather than a Replay-only timer object. Existing exact helpers already establish score at +0x04, faith at +0x0C and rank at +0x58. Target `SetFaith @ 0x00418B80` and `SetTimerCurrent @ 0x0042A930` establish an embedded five-field timer at +0x14..+0x24: previous/current/subframe/scale/flags. `SetFaith` writes faith=amount/10, lazily initializes the timer, then resets current/subframe to zero and previous to -1; its startup call passes 50000 and both replay-stage calls pass recorded faith * 10. `SetTimerCurrent` lazily initializes the same timer and writes current=value, previous=value-1 and subframe=float(value). The maintained Replay source previously called neutral `ReplayInitializeStageTimer`/`ReplaySetStageTimerValue` functions through a misleading `g_ReplayStageTimer` byte array; target call setup proves both receivers are the game-state base, so maintained source now uses shared `GameScoreState.hpp` and `g_GameScoreState`. Under real `ReplayManager::Initialize` `/GL` context the two methods replay 73/73 and 59/59 bytes exactly with their sole `g_PlayerTimerScale` DIR32 fields. | Hash-attested TH10 call setup at 0x00429300/0x0042930E and 0x00417941/0x0042A787; target helper bodies; inline timer assignment in 0x0042A6A0; canonical `game-score-set-faith` and `game-score-set-timer-current` replays; pinned VC7.1 SP1 build6030 |

## ASCII manager

| ID | State | Durable fact or constraint | Evidence |
| --- | --- | --- | --- |
| ASCII-001 | observed / source-present | The `0x89AC` owner published at `0x004776E0` is TH10's ASCII text manager, while the distinct ANM renderer is consumed through `0x00491C10`. TH10-local proof is the pair of `0x68`-byte text queues at `+0x076C` (256 records) and `+0x6F6C` (64 records), counts at `+0x896C/+0x8970`, append/format functions at `0x00401530-0x0040175D`, and renderers at `0x00401760/0x00401A50`; adjacent TH095 supplied only the initial class-role hypothesis. The owner embeds ANM VMs at `+0x14/+0x3C0`, loads `ascii.anm`, `text.anm`, and `capture.anm`, and registers update plus two draw callbacks. Constructor `+0x8974 = -1` is default color `0xFFFFFFFF`, `+0x8978/+0x897C` are scales, and `+0x898C = 9` is character-space width. The earlier lifecycle/layout observations remain valid under the corrected `AsciiManagerView` owner. | Hash-attested TH10 disassembly and raw PE bytes; append/draw field agreement; call sites; vtable/data xrefs; adjacent source used only to propose the role/name |
| ASCII-002 | observed / source-present | TH10's maintained ASCII pipeline covers the retained reset, regular/GUI appenders, three variadic formatters, and both queue renderers. `AsciiManagerStringView` is exactly `0x68` bytes: 64-byte text, `Float3` position, color, two scales, one untouched dword, viewport index, font-bank selector, and shadow flag. `AddString` bounds at 256 and copies the shadow field; `AddGuiString` bounds at 64 and leaves that final field untouched. The regular renderer selects glyph `(smallFont * 0x62 + character - 0x20)` from a `0x44`-byte table, applies 7/9 or scaled-space/14 advances, and can draw a two-pixel half-alpha shadow. The GUI renderer uses the first glyph bank and selects rounded versus non-rounded drawing through an exact `scaleX == 1.0f` comparison. Both switch viewports only on state changes and restore the default after flushing. | Target disassembly for `0x004014D0-0x00401C89`; raw boundary/padding review; target constants; formatter caller ABIs; TH095 used only as a source-shape hypothesis |

## Filesystem writer recovery

| ID | Evidence | Fact | Source |
| --- | --- | --- | --- |
| FS-001 | observed / canonical-linked-exact / real-config-caller | FileSystem::WriteFile at 0x0044B540 owns 218 bytes through 0x0044B619. WinMain calls it at 0x00438F09 with EDI size 0x34 and stack path/data; natural stdcall source reproduces RET8 private lowering. It enters the existing filesystem critical section, increments the byte count, creates/replaces with GENERIC_WRITE and FILE_SHARE_READ, and returns -1 after FormatMessage/LocalFree on open failure. No message is logged. WriteFile BOOL is ignored; only byte-count equality chooses success0 versus short-write -2. Both write branches close the handle, leave the critical section, then decrement the count; API failures and FormatMessage result remain unchecked as in target. Reuses existing extern storage at 0x004922A4/0x0049231E without adding competing definitions. Independent review and two guarded FileSystem+Main canonical cold links reproduce all218 bytes and20 DIR32 fields, with target imports independently parsed. PE/map/PDB entry is real WinMain RVA0x1670. Only the maintained config call is closed: target second log-flush caller at0x00438F95 remains behind unresolved MainFlushLog. Diagnostic anchors and absent storage identity prevent runnable/full-WinMain closure. No file writing was executed. | Fresh target/source/caller/storage review; two canonical filesystem-write-file replays on2026-10-15; canonical manifest |
| FS-002 | observed / canonical-linked-exact / persistent-write-handle | FileSystem::OpenWriteFile at 0x0044B620 owns 137 bytes through 0x0044B6A8. Target SaveReplay pushes its path and calls at 0x00429EB7; RET4 establishes callee cleanup. It enters the existing critical section, increments the byte count, and publishes CreateFileA result to the shared handle even on failure. CREATE_ALWAYS, GENERIC_WRITE and FILE_SHARE_READ match the one-shot writer. Failure formats/frees the system error without logging, leaves the critical section, decrements and returns -1. Success returns0 while retaining handle/lock/count for the existing CloseWriteFile path. No handle close/reset or API-error repair is added. Independent review and two canonical cold replays reproduce the unique137-byte source.ltcg.obj contribution with all12 fields. Guarded SaveReplay entry is RVA0x2470 and its1511-byte candidate is nonexact versus1681 target bytes; its OpenWriteFile call is inlined, while the separate owner is naturally retained by the existing pinned /OPT:NOREF harness. No dummy call or inline-control annotation was added. This proves only the bounded emitted owner, not target call-graph equality or runnable replay writing. Shared extern storage remains unresolved; the subsequently recovered inline WriteToOpenFile seam is documented in REPLAY-011. | Fresh full target/source/caller review; two canonical filesystem-open-write-file cold replays on2026-10-15; canonical manifest |

## Integrated Enemy laser construction prerequisite

| ID | Evidence | Fact | Source |
| --- | --- | --- | --- |
| ENEMY-089 | observed / integrated-source / non-exact | Fresh five-TU EnemyEclDispatcher + EclVm + AnmManager + AnmVmCreate + EnemyLaser baseline at50cfd58 is14384/14416, frame2BC versus2C4,928 raw and630/11540 comparable. Integrated shared request/layout and missing factory/construction source is14480/14416,894 raw and625/11584 comparable, normalization_complete=true. All181 selector bytes,108 physical destination order and43 suffix bytes remain intact; the table reporter overall still warns about three duplicate nested spell-case labels, not a pass. Four laser groups own1485 target bytes. Factory capacity256, preallocation nonzero ID increment, unknown-type ID consumption, allocation/null-write behavior, previous/head/next insertion and slot1 apply ordering match target. Common424 prefix, type0 request1DC+VM600/9AC=D58 and type1 request1F8+VM61C/9C8=D74 are statically asserted. Existing real AnmVm and timer constructors are placement-invoked in observed order, but extra null checks/out-of-line VM calls remain; factory153/153 is only16/137 comparable and has wrong private cleanup/register ABI. The non-instantiated virtual-prefix view emits no replacement vtable; all three19-slot tables remain unresolved data anchors. Raw-storage object lifetime and reinterpret-polymorphic dispatch remain partial implementation-specific views, not a standard C++ or runnable closure claim. Independent source review approves semantic partial retention. Existing exact leaves40CF40/41C030 are unchanged and receive no duplicate credit; derived initializer origin stays indeterminate. Request initializer23/23 raw in this graph is diagnostic only. No new exact bytes. | Target-attested audit and independent integrated source review2026-10-15; saved full-owner image hash24f23d99e94b570d3e399c993c77e21f159237e94115f1a57ff9e8c99ef56139 |
| ENEMY-090 | observed / polar-input-lifetime-and-selector-scope / non-exact | Fresh eight-TU /GL /GS direct-entry dispatcher baseline is14512/14416 bytes, not the older14384 context. Comparing mode7 inputs before assigning output locals restores one common angle FCOMP before mode store/CMP/FNSTSW; putting the normal-mode arm first restores JZ-to-additive. The interval now has3 sentinel comparisons and the whole owner19, matching target rather than baseline4/20. Two independent retained cold links give14500/14416, frame2BC/2C4, all181 selector bytes and108 ordered groups,35 equal-sized spans; full/case-aligned diagnostics533/11576 and2424/11133. Polar386/405 and aggregate span gap786 versus baseline776 remain nonexact; no partial credit. Unsigned-index prototype is byte-neutral and restored. Further ordinary-output deferral shrinks owner14484 but adds a live x87 speed value/FSUB ST(2), expanding the existing missing float32-output-store mismatch, and is rejected. Both retained images strictly preserve ResolveIntArgument75, ReadInt144 and ReadFloat183/all5 fields in the declared carrier, without canonical recipe changes. Selector reporting now excludes three inner spell cases from the124 outer labels; seven independent scope/rejection tests pass, and target/candidate table coverage passes without changing byte acceptance. | Local read-only hash-attested Ghidra and target constant/instruction review; complete PDB-owned comparisons; docs/evidence/enemy-dispatch-polar-lifetime/README.md |
| ENEMY-091 | observed / polar-pair-storage-and-adapter-controls / non-exact | Actual initial/final EnemyFloat2 values feed polar0x11D/0x11F and random movement through the observed common setup tail. This restores target frame2C4, final-angle float32 FSTP before the common difference, memory FSUB, and initial-speed copy after subtraction. Complete eight-TU /GL /GS owner remains14524/14416, polar396/405,19 sentinel comparisons,181 selectors/108 ordered groups; no partial credit. Scoped pairs without the shared tail emit14540/424; shared pairs14524/396. Initial-angle-first comparison changes4 ordinary bytes and restores operand direction without changing whole-owner613/11580 or case2447/11143 counts; branch encoding remains unequal. Ordinary nonmember runtime/index and index/runtime adapter controls both preserve the same16-byte register gap and are restored. Full same-image guards bind ResolveInt75, ECL ReadInt144/ReadFloat183 and WrapAngle94:496 bytes/11 fields; canonical recipes unchanged. WrapAngle returns live x87 with34-update one-direction cap and no float32 result store. Exact rational RN-even PC53/PC64 model at input447 and initial bitsC1575CFE demonstrates target stored-angle adjustment false versus old live-angle true. Finite conditional storage counterexample only; game reachability, runtime FP control and normal-input bug unknown. No exact numerator change. | Hash-attested read-only local Ghidra, pinned target instructions/constants, complete PDB-owned cold comparisons and same-image guards; docs/evidence/enemy-dispatch-polar-storage/README.md; scripts/check-enemy-polar-rounding-model.py |
| ENEMY-092 | observed / dispatcher-storage-and-raw-float-source / non-exact | Fresh target/local read-only Ghidra confirms spawn64-byte zeroing and spell decoding both use final ESP+248; the target frame is2C4 and cookie final ESP+2CC. Physical sharing does not prove an authored union or original array capacity. Separate function-scope request plus block-local spell buffer, and a request scoped to spawn cases with its actual shared tail, both emit14552/frame30C versus target14416/2C4; all source restored. Existing26-byte0x412AA0 gains the descriptive ReadRawFloatArgument mapping:13 dispatcher calls, EAX runtime, two real stack words, owner14D8/context4, ECX index into44FF80 and RET8. Origin remains indeterminate. Direct receiver syntax changes this to ECX despite closer13/22 ordinary positions and is rejected; argument snapshot and unsigned-index controls are byte-neutral and restored. Baseline and full-cold unsigned control share the complete14524-byte parent hash,181 selectors/108 groups/19 sentinel compares/35 equal-sized spans. Four primary-only controls reuse seven unchanged hash-checked support objects; no incremental or partial-case credit. Six complete existing guards retain791 bytes/all16 fields in the baseline actual eight-TU carrier. No boundary, authored denominator, exact numerator, canonical recipe or source body change. Current ReadIntArgument is a direct return; its older assignment experiment is historical. | Independently attested local Ghidra, target raw decode and13 direct calls; complete native extents and reconstructible rejected overlays; docs/evidence/enemy-dispatch-storage-and-forwarder/README.md |

## Complete stage GUI reset recovery

| ID | Evidence | Fact | Source |
| --- | --- | --- | --- |
| GUI-005 | observed / complete-source / non-exact | GameResetGui at413BC0-41436C owns1965 bytes, with real calls4182A4/4184A4 in maintained GameManagerUpdateBody. Source uses existing exact-size GuiView arrays/IDs/frontAnm and real ANM allocation, initialization, list/ID and interrupt contracts. Callback flags are ORed2 when present. Missing front IDs trigger creation; target publishes only9E58 and discards the second returned ID without writing9E5C. Conditional six-array initialization uses paired10/20 ranges then30,47,80,51 ranges, and retains resource reloads. Lives remain unclamped; signed power digits bind VM indices0/2/3. Stage/front overlays and ASCII boss VM follow original global-condition order. New-session ID9E50 and unconditional9E54 are published, but both interrupt walks use9E50; no repair is added. Guarded five-TU Gui+GameManager+AnmManager+AnmVmCreate+AnmVmId cold link selects real GameManager entry402EA0 and1786-byte owner402540 with EDI/plain RET,421 raw and399/1745 comparable, normalization complete. No exact credit. Independent review confirms source/control/ABI. Host test compiles actual extracted body with synthetic dependency sinks and checks event ordering, resource/VM receiver identities, signed digits, discarded/repeated IDs, skipped arrays and separate short-circuit gates; this is not game IO or runtime closure. External storage and nonexact dependencies remain open. | Fresh target attestation and independent review2026-10-15; guarded image SHA c078903d122af5b67aa5031e6a5c46e0fb3e2164dfbd656e441e612d368cfd40; scripts/test-gui-reset-order.py |

| GUI-006 | observed / target-local-interrupt-structure / non-exact | The initial1786-byte reset used two general AnmVmIdView::SetInterrupt calls; target instead expands both complete two-list lookups and root-child walks. A local helper control remained out-of-line (1761/1965) and was rejected. Two direct lexical blocks, with unchanged general helpers/compiler flags, restore target28-call topology and emit1981/1965 bytes. Complete owner217 raw and198/1753 comparable, normalization_complete=true; shifted whole-owner score is lower and no byte gain is claimed. Diagnostic regions: prefix1560/1541 bytes, first publication/walk120/123 bytes (lengths, not matching-byte counts), final creation/walk301/301 with273/285 comparable, final old-ID tail140 raw-equal bytes. These are subregions of one1965-byte owner, never separate exact credit. Independent review passed semantic/order/ABI changes and verified both lexical blocks plus actual maintained FindVm in synthetic tests, including primary/secondary preference, invalid/missing IDs, root children and nonroot suppression. Audit correction:26fceebb did not actually contain the independently claimed startup/transition combination fixtures; an insertion assertion failed before an old test passed. Current fail-fast edit and independent source/log inspection confirm those fixtures are now present and passing. | Full guarded lexical-interrupt.json and lexical-regions.json; image SHA b015b9de780232313e4b9eac18797fbbb988d81c1ee617f787cc8dc5c899591f; independent review2026-10-15 |

## GUI Run shared-storage and replay-exit review

| ID | Evidence | Fact | Source |
| --- | --- | --- | --- |
| GUI-007 | observed / target sampling and rejected compiler models / non-exact | Both opcode20 replay-mode checks share target mode4 tail416E8A. Target reloads character474C68, shot474C6C, difficulty474C74 and stage474C7C after its first unlock-byte write; GUI flags are ORed20 before faith474C4C is read. Common-storage source controls recover these sequences but complete Run remains nonexact:4464/4480 for four typed configuration fields,4456/4480 for six further state aliases, frameD8/150 calls. Original state class and full layout remain unknown; source changes are restored. In opcode13, all32 physical return-slot LEA displacements and lengths match, while15 short EAX absolute loads instead of target ECX account for its15-byte size gap. POD getter, repeated count-address expression and opaque cursor are raw-neutral in their immediate contexts; complementing stage7 and swapping arms changes raw bytes despite equal diagnostic totals. No partial-case credit or ledger changes. | [Target-local review, complete-owner controls and limits](evidence/gui-run-shared-state/README.md) |

## Music-room state recovery


Music-room full source recovery (2026-10-15): FrontEndUpdateMusicRoom at433EF0
now covers all four states and its necessary59/92-byte line parsers and206-byte
position initializer. Independent target/source and strengthened-test review
passed. Guarded maintained FrontEnd::Update entry RVA2320; full candidate2560
versus ledger2778,135 raw and118/2378 comparable, normalization complete.
Target code ends4349C9, then8BFF alignment and four-entry table4349CC-4349DB;
physical2796-byte comparison118/2396. Literal table0e3f4300464343001b4443004e494300
and ordered dispatch identity are audited separately. Existing ownership rows
and denominator remain unchanged; no exact claim or Factory promotion.
Position initializer206-byte structural comparison178/178 is only diagnostic:
component anchor symbols do not prove coherent zero-vector aggregate storage.
Music command/load/play and other storage externs leave runtime closure open.
Preserve original count/@/unbounded-copy quirks, null-path closing, track-zero
profile write, confirm precedence, double cursor pop and unordered position
copy. These are fidelity observations, not safe parser/API recommendations.
Only synthetic fixtures have sentinel padding; actual OpenFile mode0 allocates
exact file size. Zero-count output tests cannot establish read-before-check.
Actual-source fixtures cover four states, CP932 copying, warnings and receivers,
12-track scroll visibility,39.999/40/40.001/NaN and simultaneous confirm/cancel.
Dependency/test strings are stubs; separate target audit verifies all9 actual
CP932 literals, not these stubs. Full runtime/precision equivalence is unproved.
Artifacts: .analysis/gpt-dots-music-room/unordered-fixed.json,
owner-table-audit.json, strengthened-test.log; image retained separately in
build/probe-ltcg/MusicRoom-unordered-fixed/source.exe, SHA256
33b28b51989b47dac155571524b93ba3f60f995ad19759ea378f9c786ca951bd.

Music-room interrupt refinement: the general ID helper stayed out of line in
both paths. A manager-wrapper control still retained the confirm wrapper call
and was rejected. Direct lexical bodies naturally produce target call depths:
confirm calls FindVm then walks root children; close expands both two-list
lookup and root-child walk. General helpers/compiler flags are unchanged.
Latest contribution2808 =2791 code +1NOP at406467 +16 table, versus target
2796 =2778 code +2alignment +16 table. Whole physical comparison309 raw,
286/2396 comparable, normalization complete; no exact credit. Code-region
lengths candidate/target: entry30/30, state0 1082/1080, state1 213/213,
state2 1302/1331, state3/shared epilogue164/124. Original table ownership
remains provisional; no denominator/boundary edits. Independent delta review
passed actual source/list tests for both paths, primary/secondary precedence,
zero/missing IDs, root children/nonroot suppression and unchanged IDs/flags.
All previous parser/state/warning/scroll/NaN/precedence fixtures remain.
Evidence .analysis/gpt-dots-music-room/lexical-interrupt.json,
lexical-table-audit.json, lexical-regions.json, lexical-tests.log; retained
image SHA d676ad755efb8203d8956c406fa4320884ea023c391244c9e9d88339c0032c7d.

MusicRoom closing semantic correction and bounded prerequisites (2026-10-15):
Target42C770 only resolves/marks pending interrupts and root children. Historical
InterruptVmNow instead executes scripts immediately. Only the two MusicRoom
closing calls now use recovered FrontEndMarkVmInterrupt; other callers are
explicitly unaudited. Tests observe zero immediate ExecuteScript calls, pending
root/child and nonroot suppression before an explicit later synthetic tick.
Load420A90 copies into256-byte local buffer, finds last dot, overwrites exactly
three extension bytes without truncating a longer suffix, queues mode1/slot,
returns1/RET8. Play420B10 optionally queues4 then2/slot, publishes the specified
profile-track byte even on full queue, returns0/RET8. No invalid-input repair.
Queue43E460 uses the existing SoundPlayerView:32 physical slots at1F88, stride10C,
but admission scans only31. Opcode/argument/step/path offsets0/4/8/12 asserted.
It writes opcode/argument, unbounded-copies name then clears step; full queue
silently drops. Global sound-owner storage remains extern-only. Natural real
caller graph recovers EAX owner/EDX path/RET8, not an anchor ABI assertion.
Six-TU guarded diagnostic: MusicRoom2772=2755code+1padding+16table versus target
2796=2778+2+16. Physical329raw,300/2380comparable, normalization complete. Table
ownership remains provisional with no denominator change. Mark65/65 has50/57
comparable; queue90/104 has3/104. Load120 and play73 structural matches are only
diagnostic with open bindings/storage and no standalone exact credit.
Independent semantic review passed. Existing MusicRoom synthetic test still
mocks music calls; a separate test extracts actual load/play/queue bodies and
checks holes/full queue/reserved slot31, last-dot/long suffix preservation,
input immutability, command order and profile publication despite dropped
commands. Both tests pass from a fresh temporary root without build directory
via shared setUp; no game IO or actual audio execution.
Evidence .analysis/gpt-dots-music-room/prerequisites.json,
prerequisite-table-audit.json, prerequisite-owners.json, prerequisites-tests.log,
clean-root-test.log. Image SHA
3bc0a6b50cc32d84b4d8b3180ee6789142bfe27274d63a4cda5e6296516a4835.

MusicRoom final warning structure control: replace arithmetic ternary mode
selection with target-observed separate mode4 and mode3 queue/publication/return
arms. This is semantic-neutral structural fidelity, not a bug fix. Independent
review verifies both call roles and actual-source fixtures (one command, no
playback, warning/timer publication). Full physical2804/2796, code2787/2778,
padding1/2 andtable16/16;454raw and412/2384comparable, normalization complete.
All state call counts now agree; candidate/target region lengths1082/1080,
213/213,1334/1331,128/124. These lengths are not exact-byte counts. Residual
state3+4 includes unresolved EnemyMarkPendingInterrupt call ABI; no additional
graph expansion or register/size spelling control is justified here. Park
remaining near-match shape; zero exact credit and table ownership unchanged.
Evidence warning-arms.json, warning-table-audit.json, warning-regions.json,
warning-arms-tests.log under .analysis/gpt-dots-music-room; image SHA
7da9a53933bae764948b3ccb1fc30c68b2d731b07661e8d868bd5201646dee6b.

Stage timed-script recovery (2026-10-15): new StageScript.cpp/.hpp implement
all15 commands at403C80, real callback402720, active-object updater403990 and
fog-value constructor405040. Signed opcode/record-size semantics, time-relative
jump, opcode0 yield without pointer consumption/timer tick, direction-before-
position-before-fog evaluation and rocking behavior are preserved. Opcode14
uses actual maintained ANM initialization; negative script index only clears
flag1. Layout assertions bind two Float3 interpolators,0x8C fog interpolation,
eight script VMs,three auxiliary VMs and280-byte camera state. Callback gates,
normalization, object VM update, dispatcher,8 VMs,optional3 at game speed1,
speed restore, auxiliary clear, camera publication and frame increment follow
target source order. Object updater reads post-execution instruction state.
Fog evaluator688 remains a declared unresolved seam; no private ABI validation
from that declaration. Published camera is an extern-only overlapping aggregate
view of target491D7C; existing smaller ANM aliases are not closed ownership.
No source/byte claim for a complete stage registration/rendering subsystem.

Ledger dispatcher1806 ends40438D;2-byte8BFF alignment and15-slot table404390-
4043CB imply provisional physical1868. Literal table and destinations saved;
no boundary/origin/denominator edits. Guarded callback entry401AA0 belongs to
maintained source. Dispatcher2192=2129code+3pad+60table; physical92raw89/1804
comparable, normalization complete. Callback294/304,175raw170/268comparable.
Updater154/154150/150comparable and fogctor94/9478/78 are diagnostic only, no
standalone credit. Native dispatcher is EBX/plainRET versus target stack/RET4;
native callback is ECX versus target EAX. Native callback copies camera before
clearing auxiliaryActive and hoists frame read, unlike target machine order.
Source order is correct but no emitted-order/ABI closure is claimed. Rocking
x87 sequence independently reviewed; host tests do not prove x87 equivalence.

Independent source and strengthened-test review passed. Test compiles actual
stage source plus actual shared timer source against synthetic ANM/interpolation
and normalization sinks. It checks signed backward record size in allocated
fixtures, jump/yield/time, setup selection and all Float3 components, object
inactive/zero-count/empty primitives/VM-driven termination, and dependent
normalize/object/dispatcher/8+3 VM/camera-publication behavior. Fog initial/end/
all channels are not exhaustively asserted. Publication after VM mutations is
observed; relative ordering among final clear/copy/frame/speed-restoration is
not independently established by host sinks. No invalid-index/overflow safety,
standard object lifetime, x87, target ABI, real game IO or runtime claim.
Evidence .analysis/gpt-dots-stage-script/attested-decompile.json,
layout-corrected.json, owner-table-audit.json, strengthened-test.log; image SHA
aa87fbff37b064f94ceb8699304af55caaca7c41401e37212f8923daec8da645.

Stage fog dependency recovery and precision correction (2026-10-15):
The0x1C aggregate is six float channels(start,end,B,G,R,A) plus derived packed
BGRA, not seven independent floats. Target arithmetic constructors retain
extended x87 color results after FST stores and __ftol2 consumes those results;
low bytes wrap without saturation. Storedfloat1 can therefore coexist with
packed0 for an extended result1-2^-26. Tests describe that representation
witness separately and never claim ordinary host operators reproduce x87.
Recovered evaluator modes7 additive,17 acceleration,8 Hermite and maintained
CalculateAnmInterpolation easing, with positive-duration tick/completion and
unmodified zero/negative-duration behavior. Completion returns initial only
for mode7, otherwise final, copying packed dword rather than repacking.
Real dispatcher call recovers EBX/output-stack/RET4 for evaluator, no anchor
ABI assertion. Full evaluator664/688,234raw228/600comparable, normalization
complete.92/101/92 arithmetic structural matches are diagnostic prerequisites,
not independent credits. No new canonical replay/Factory acceptance attempted.

Independent review found initial Hermite expressions lost target's(t-1)float32
spill and used sequential3-t-t. One natural shared twice=t+t correction restores
both: candidate401DF9 FSTP and401E05/09 memory FMUL match target404B34/40/44,
with retained doubled-t grouping and subtraction. No volatile/flags/coercion.
This verified numerical improvement is distinct from remaining whole-owner
nonexactness. Two source tests pass: dispatcher keeps explicit fog sink;
separate actual fog/timer/easing bodies cover all six arithmetic channels,
packed finite boundaries, linear/ease-in/Hermite/add/acceleration/constants/
default, scaled timer and completion modes0..17. That completion loop does
not test all easing curves. Nonbinaryt0.1 differentiates grouped/sequential
subtraction in the host model; native equivalence comes from PE instruction
review. Invalid/out-of-range conversion, ABI and alias/lifetime proof excluded.

With real fog source, full stage dispatcher2052=1991code+1pad+60table versus
provisional1868=1806+2+60;162raw155/1788comparable, normalization complete.
Camera aggregate storage, callback native publication ordering and full stage
ABI remain open; ownership/denominator unchanged. Evidence fog-hermite-grouped.json,
fog-grouped-owner.json,fog-grouped-candidate.txt,fog-grouped-table-audit.json,
fog-grouped-tests.log under .analysis/gpt-dots-stage-script. Retained image SHA
f4b4a878cbf3aa992d1b80fb42f4888e711427c5904446e6e4ee9758365688cb.

Stage direct XYZ setter refinement: opcodes2/4/6 now perform target-observed
sequential field reads/stores rather than snapshotting constructor temporaries.
Opcode2 preserves old-position capture and subsequent x87 delta computation;
its residual4 ordinary-byte differences are a cached EBX constant, not camera
arithmetic. Independent native review verifies target load/store order and
retained fog precision fix. Region spans113/113,32/32,32/32 with105/109,28/28,
28/28 comparable are not independent owners or credits. Full1972physical=
1911code+1padding+60table versus1868target;203raw193/1776comparable,
normalization_complete=true. Physical instruction decoder reports1971/1972:
normalization completeness does not imply every table byte is an instruction.
Actual code ends RET4 before padding/table, each table remains literal evidence.
No valid runtime overlapping-record/camera alias is claimed; no artificial
undefined-overlap fixture added. Both prior actual-source tests pass unchanged.
Evidence direct-setters.json,direct-table-audit.json,direct-cases.json,
direct-dispatcher.txt,direct-setters-tests.log under .analysis/gpt-dots-stage-script;
image SHA75a3aae387723d53b70c5b22e0fef0c347b5925620189a07092ce7da27135a13.

### Player update power/respawn timer contracts (2026-10-15)

Target PlayerUpdateCallbackBody 0x425730 calls shared timer SetCurrent0x405410,
private EAX receiver plus stack value/RET4, at five sites:4259C9 highlight+430C
270;425C28 update+474 60;425C38 highlight200;425D12 highlight280;425D1E update0.
The prior descriptive PlayerPlaySound declarations/calls were semantically
wrong. State-4 successful transition calls RebuildPlayerOptions0x426F70 at
425C4C after the power-word subtraction425C43 and before display425C7F.
Respawn425D08 writes43F00000 (480.0), not43700000 (240.0), after saving the
post-effect draw vector. Source now preserves these contracts. Actual-source
switch tests and independently reviewed native diagnostics support these
bounded corrections; full owner remains nonexact and callback/body ABI, global
scale alias storage, other private dependencies and product runtime remain open.

### Stage projection and instance drawing (2026-10-15)

0x403090/1984 is projected stage-object visibility, not player collision.
Camera EAX, object ECX, instance XYZ EDX, stack squared limit/RET4. Eight
half-extent corners use instance translation and camera view/projection/viewport.
Depth0<=z<=1, original else-if extrema and inclusive overlap32..416,16..464 are
preserved; unordered distance does not early-reject. 0x403A30/592 takes stage and
signed layer on stack/RET8, walks16-byte instances until negative index, culls
then draws type0 primitives. Render-mode bits>=0x1000000 enable position and
nonzero dimension scaling; mode0x2000000 enables fog, with flush before changed
full-dword state. Camera setup0x4215A0, globals/view alias storage and native
x87/D3DX precision remain open. Independently reviewed source and synthetic
projection fixtures receive zero exact credit; complete diagnostics mismatch.

Stage camera setup0x4215A0/330 is now maintained and connected to the real draw
caller: EDI/plainRET naturally recovered, but complete comparison nonexact.
Eye=position+rocking, at=position+direction, left-handed LookAt and perspective,
FOV+48, unsigned viewport width/height, near30/far1800, transforms2 then3, followed
by normalized direction-cross-up and post-call renderer reload/shake publication.
Native unsigned aspect conversion uses FILD plus2^32 correction and divides
before float spill. Actual-source host fixtures establish bounded call/state
behavior only; D3DX math, alias storage and product runtime remain open.

Stage bounds correction: target depth interval is inclusive0<=Z<=1. Earlier
strict-upper-bound interpretation was wrong; TEST AH,41 / JP after FCOMP1 accepts
equality and rejects unordered. All eight candidate checks now reproduce it.
Target rounds center and delta XYZ separately through float32 before its native
(Z²+Y²)+X² distance comparison. Reused ordinary aggregate assignments and XYZ
source dot expression restore those boundaries/order in the pinned compiler.
Independent actual-PE review passed; whole1925/1984 owner still nonexact.


## 2026-10-15 Stage early/late draw pass source recovery

Recovered complete 0x402850/1099-byte early and 0x402CA0/929-byte late bodies
with their necessary seven-byte registration adapters in StageDraw.cpp.
Early owns layers 0..7, late owns 8..11 with ANM layers 17/18 preceding them.
Raw target verification covers D3D Clear slot 0xAC, depth/color clear arguments,
fog/Z-write/Z-function ordering, camera publication, mix-color restoration,
transition allocation failure, fractional countdown, and repeated final state
writes. Ordinary local source helpers express these operations without forced
inlining or ABI controls. No executable game or D3D calls were run.

Read-only initializer 0x402230 evidence establishes adapter registration with
priorities initializer argument+7 and +10, node owner at offset 0x20. Initializer
source is not recovered here. The existing viewport helper, supervisor fog
helper, and effect factory remain externs: respectively target ESI/EBX,
EDI, and EBX priority plus five stack arguments/RET20. Ordinary declarations
and diagnostic anchors do not validate those private ABIs or allocate actual
global alias storage. No runtime or registration closure is claimed.

Diagnostic passes.json / build/probe-ltcg/StageDraw-passes/source.exe SHA256
cfbcb229fbb33cfd000da595b9422ad01d1138f0d9c650d1355fc59564ac1c33:
early 918/1099 bytes, 67 raw equal, 57/867 comparable; late 917/929,
135 raw equal, 109/645 comparable. Guarded early entry 0x4024C0 pushes ECX;
late adapter 0x4024D0 pushes EAX while both target adapters push ECX. Late
callback ABI remains open. All metrics are full-owner diagnostics, zero exact
credit and zero adapter credit. Prior bounds/camera closure limits persist.

Independent full source/target review passed. Actual-source host trace tests
cover both full passes, gates, VM0-null suppression, active tint, transition
failure and thresholds, fractional Add with previous/subframe values, and
fog snapshot before Flush with device replacement after Flush. CI runs this
fixture. Synthetic host sinks establish source event/state contracts, not
native D3D, x87, private ABI, alias storage or product behavior. Evidence under
.analysis/gpt-dots-stage-draw includes pass-early-target.txt,
pass-late-target.txt, pass-registration-target.txt, passes.json and
passes-test.log. Complete source retention is provisional and nonexact.


## 2026-10-15 Stage lifecycle supporting the full draw/script graph

StageLifecycle.cpp recovers the coherent five-owner graph: constructor
0x402160/188, Initialize0x402230/522, destructor0x402440/499,
StageCreate0x402640/151, and LoadData0x403850/308. Header layout exposes the
three chain slots, original cache/size, 0x90 STD header and mutable tables.
Existing maintained Stage, ANM, FileSystem and Chain types are reused.
Constructor origin remains indeterminate; no ownership/denominator change.

Target contracts: 0x2B64 allocation; eight plus three real VM constructors
before whole-object zeroing; separately cached original and mutable relocated
file copies; ANM slot (startupMode&1)+4; raw object-VM malloc; camera defaults;
three callback registrations with base+12/+7/+10 priorities and owner+0x20;
three unregister operations caching the chain before lock entry; data/cache/VM
free order; runtime flag-gated ANM Release followed by slot reload/delete;
matching global-pointer clears; then automatic reverse 3+8 VM destruction.
Target unchecked allocation, path, counts and null-new Initialize behavior is
preserved and documented, not recommended or made safe by the host fixtures.
Factory failure invokes normal destructor/delete and returns null.

Fresh pinned image3cdb24e01a358885578ed92411778638098f0720547ad565e9769d25bad9b564
has genuine StageCreate entry0x401770. Complete sizes: factory85/151,
constructor291/188, initializer478/522, loader308/308, destructor451/499.
Raw/comparable pairs respectively2 and0/131;15 and15/188;48 and43/438;
263 and252/264;20 and12/351. All mismatch; zero exact credit. Native constructor
EBX and destructor EAX differ from target stack/RET4; exception handling also
remains open. The full graph retains early918/1099, late917/929 and the prior
bounds/camera mismatch limits. Ordinary declarations, typed alias externs and
anchors do not establish global storage, full caller ABI or executable closure.

Independent source/target and retained-image review passed. Actual-source host
fixtures cover source construction before clear, pre-captured reverse member
addresses, successful/failing factory, cached reload, relocated pointers,
callback priorities/owner flags, null raw-VM allocation, conditional ANM retain,
replacement-slot deletion identity, and chain capture across lock mutation.
The dispatcher-only fixture explicitly isolates lifecycle; native layout and
EH/x87/private ABI are not host-test claims. Unsafe null factory allocation,
negative counts and malformed input are not executed. CI includes the new test.

Evidence .analysis/gpt-dots-stage-draw/lifecycle-*-target.txt,
lifecycle-attested.json, lifecycle.json, lifecycle-test.log. Corrected report
metadata initially mislabeled the primary source as StageDraw.cpp; it now
names StageLifecycle.cpp, regenerated against the unchanged PE/map/PDB and
all ten contributions verified equal. No compiler rerun was needed for that
metadata-only repair. Required source/storage caveats remain explicit.


## 2026-10-15 Genuine GameManager-to-Stage source binding

GameManagerInitialize0x417870/1009 now calls maintained StageCreate through
its shared header at the two target call sites0x417A9E/0x417B17. Previously
both called the unresolved GameCreateMenu alias. Path and zero priority,
short-circuit order, replay branch and failure destinations are unchanged.
The unrelated zero-argument GameCreateStageOwner (target0x419090) is untouched.
This is a caller identity/source graph repair, not a new startup logic claim.

Fresh GameManager-entry graph: image
86160b10e99afdb1326ed8e534a2da4ad23a1270bf85e9635a199cdabe1a3946,
guarded entry0x4018B0, full initializer1037/1009. The 14 complete contributions
are saved in .analysis/gpt-dots-stage-draw/game-stage.json with matching
PE/map/PDB under build/probe-ltcg/StageDraw-game-stage. Stage body sizes remain
unchanged. StageExecuteScript1972physical compares with provisional1868
(1806 ledger code plus separately audited padding/table); this does not change
ledger ownership or denominator. No exact credit is claimed.

Context-qualified ABI finding: actual callback registration now makes both
draw adapters push ECX, and update adapter convert ECX to EAX before tail jump,
as in the target. This already held in the lifecycle image3cdb24...b564 and is
retained here. The earlier EAX late-adapter mismatch belonged to the draw-only
imagecfbcb2...1c33; it is not a property of this current graph. Other private
ABIs, constructor/destructor EH, global aliases/storage, external dependencies
and runtime closure remain unresolved. Independent full caller binding and
14-contribution/entry review passed. No further Stage spelling controls planned.


## 2026-10-15 Enemy runtime callback scale lifetime correction

Full EnemyRuntimeUpdate0x40DC80/2412 audit found a substantive callback-state
mismatch: source reused the scale cached before the first ECL run when invoking
ECL again after ReleaseEclAllocations, ResetEclState and EnemyRunCallbackEcl.
Target0x40E1F0..0x40E205 reloads both runtime+0x128 scale pointer and its pointed
value after those calls. Only the second argument expression is corrected;
first-run capture/order, bounds, damage and all other logic remain unchanged.

Fresh same-context pinned baselines use the real EnemyManagerUpdateCallback
entry and existing canonical profile. Full owner2358/2412 remains nonexact;
raw-equal191→201, comparable174→182 of2156. Corrected image
97960b3c91145ea44e2a0ff888d2f4d65d49c4677c14175b4f1a14799f631f83
loads [EBP+0x128] then [EDX] at0x401F6D/0x401F73 immediately after callback and
before second ECL call. Complete instructions/normalization checked; ECL helper
calls still use ordinary declarations/data anchors, so private callee ABI and
runnable closure are not established. No exact credit.

Independent source/native review passed. The host regression extracts the
actual affected ECL/damage block and distinguishes pointed-value mutation from
pointer replacement, verifies first-scale ordering, both ECL failure returns,
no-callback/no-damage/gated paths and death return. Movement, collision, VM and
common timer tail are deliberately outside this fixture; host behavior does
not prove native x87 or ABI. CI includes it. Evidence is retained under
.analysis/gpt-dots-enemy-runtime: target.txt, baseline.json, scale-reload.json,
scale-native.txt and scale-test.log. No repeated register-spelling experiment.


## 2026-10-15 Enemy callback installation source identity

Both EnemyRuntimeUpdate callback checkpoints now use shared
EnemyEclHostBaseView::SetSubroutine, target0x450700/30 and inline40E29C..40E2B7.
This resolves the two disconnected RunCallback/InstallCallback aliases: lookup
through the pre-call database, then reload active context for instruction and
again for time zero. Installation does not execute ECL. An explicit lookup-result
local enforces sequencing; the initial assignment-only expression failed the
host context-replacement fixture despite matching the pinned reload sequence.
The retained local preserves the same complete native owner contribution hash.
Existing shared Enemy/Ecl database declaration is reused; no new layout or ODR
view is introduced. The old ledger renderer-state description was incorrect.

Real EclVm.cpp support changes the context baseline from2358 anchor-only bytes
to2367; compare the retained change only against that fresh real-source baseline.
Final owner2375/2412, raw196, comparable181/2156, normalization complete and all
2375 candidate instruction bytes decoded. Entry0x402F50 is the genuine maintained
EnemyManagerUpdateCallback. Final PE SHA256
b3ae1242ec6a15cf79365d5daa7bf9f170c25b045e57143d99a681b9ca5fa409;
reports/probe and native decode are under .analysis/gpt-dots-enemy-runtime/
subroutine-sequenced.json and subroutine-sequenced-native.txt, with separate
ECL-context baseline and earlier expression control retained. Actual Run and
FindSubroutine contributions come from EclVm.cpp, not diagnostic anchors.
First installation is out of line, second naturally inline; both post-lookup
context reloads and prior scale fix survive. The 30-byte installer takes name
in EAX versus target ECX, so private ABI/exactness is open. Other externs,
partial runtime views, global storage and product closure remain open.

Independent source/native/entry review passed. Actual-source host fixtures
retain earlier scale/return tests and add both installation checkpoints,
lookup-side context/database replacement, null lookup and absent callback,
and no additional ECL execution. Lookup mutation is a synthetic lifetime witness,
not a claim that the maintained lookup normally mutates the host. No host test
proves native x87/ABI or observes a mutation between the two plain stores.
No exact credit, helper promotion, or denominator change.


## 2026-10-15 Enemy interpolation and score caller identities

EnemyRuntimeUpdate now calls the maintained AnmVmFloat3InterpolationView::Evaluate
at both target404610 seams, consumes its returned pointer, and calls the existing
GuiScoreView::Add at death publication before EnemyFinalizeDeath. Target score
receiver is474C40. The two undefined free-function aliases are removed. The exact
existing GuiScoreView class is shared via GuiScore.hpp by Enemy, Gui and Item;
Item's token-identical duplicate was also removed and its TU compiled with the
pinned toolchain. Score arithmetic, including existing signed-overflow limits,
is unchanged. ANM header inclusion replaces conflicting local C critical-section
prototypes with its existing void-pointer declarations; actual imports/calls and
Enemy critical-section storage are unchanged.

Enemy and ANM position interpolation views have asserted matching size and all
member offsets. The two reinterpret call seams remain partial object-lifetime/
alias views, not a complete type unification. No dispatcher assignment rewrite.
The diagnostic now links actual EnemyCallback, EclVm, AnmManager and Gui sources.
Fresh same-graph baseline2374/2412 (raw189,174/2156 comparable) becomes2358/2412
(raw178,158/2152 comparable), complete normalization/instruction decode. This
is meaningful source binding recovery, not a byte-match improvement. Final PE
SHA256 dd9cbf04c54a7d9f58c00abdd19150d3817202cc430a65b26313e25c0aa53ac9;
.analysis/gpt-dots-enemy-runtime/connected-identities.json and connected-native.txt
retain full owner/PDB evidence. Both Evaluate calls use EAX receiver/EDI output,
then consume returned EAX. Score uses ECX receiver and RET4; callback resolver
is the real606-byte EnemyCallback contribution, not an anchor. g_GuiScore still
has diagnostic-only anchor storage; no global alias/runtime closure claimed.

Independent review passed. Actual-source tests retain all callback/scale paths,
check score is already updated at death, and test both interpolation call bodies
with distinct returned versus output pointers and lookup-side position mutation.
These are synthetic seam witnesses, not full interpolation arithmetic or complete
Enemy runtime tests. Existing 46-byte score exactness is not newly credited.
All helpers and the full2412-byte owner receive zero new exact credit.


## 2026-10-15 Enemy movement prerequisites, provisional numeric source

Recovered the complete652-byte target two-component evaluator412AC0 and171-byte
motion updater44C2A0 in their real EnemyRuntimeUpdate2412 graph. Four scalar
interpolation calls now use the declared member; three motion calls have real
source. Timer completion/lazy initialization, additive and acceleration modes,
Hermite/shared easing, Cartesian/polar state and floor-to-hundredths are covered.
The target returns the evaluator output pointer. No new subsystem/leaf credit.

Pinned final PE bed76ceb85d0d6f6e67f57317f110073be54da3c21df23723c4517c9dc7faab5,
real EnemyManagerUpdateCallback entry403270. Full runtime2326/2412 raw206,
194/2152 comparable; scalar551/652 raw26,26/612; motion160/171 raw75,73/143.
All three complete candidate extents decode and normalize, but all mismatch.
Reports/native/raw target and precision classification are saved under
.analysis/gpt-dots-enemy-runtime/movement-components*,
motion-interpolation-target.txt and movement-precision.txt.

Native timer FST retains the sum before __ftol2; Hermite t-minus-one float32
spill/reload and3-(t+t) grouping match target. Motion preserves QWORD floor
arguments after multiply100, then float0.01 multiply/store, including negatives.
Known numerical mismatch remains: candidate removes target Hermite term-product
spills and reassociates weighted sums, and removes eased-Y delta/product float32
boundaries. Target locations412C85/8C/95/9B/CA4/CBA/CC8 and412D16/D22 are recorded
against candidate401C00..C3F and401C75..C7D. No equivalence or precision closure
claim. Candidate scalar receiverEAX differs from targetESI; motionESI agrees.
Earlier aggregate control807/652 introduced extra roundings; no more spelling,
volatile/barrier or flag controls pursued. Global aliases/storage remain open.

Independent provisional source/native/test review passed. Actual-source host
fixtures test completion modes0..17 (not active coverage of all easing curves),
fractional and lazy-completion timers, repeatedADD/ACCEL and returned pointers,
nonbinary Hermite both components, Cartesian and nonzero-angle polar motion,
extra flag bits and positive/negative floor behavior. These do not prove x87,
precise threshold/alias behavior or host trig/native equivalence. The fixture
uses a static zero-initialized interpolation seed copied before setup, avoiding
indeterminate flag reads by the maintained constructor; production is unchanged.
CI includes the test. No exact numerator or denominator change.


## 2026-10-15 Enemy runtime tail receiver identities

The two eight-VM loops now call existing ANM world/screen-position setters and
reuse the same cached manager through the later FindVm, matching target EBX
lifetime after loading491C10. The falsely named EnemySpawnDamageEffect alias
is replaced with the actual sound QueueSoundCue call on492590; sound selection
and publication precede both existing ANM timer Add(-1) calls. Null VM clears
stored id, but flash/damage gates still require a valid VM just as target;
this is not a blanket null-safety repair. Partial VM/timer reinterpret views
and global aliases/storage remain unclosed.

Existing queue class is shared among Enemy, EnemyEclDispatcher and Item via
EnemySoundQueue.hpp, with no method implementation change. This is not repo-wide
ODR closure: EnemyLaser/BulletManager/BulletRuntime retain distinct preexisting
views outside this graph. Item also retains its preexisting pointer declaration
for g_MainSoundOwner while Main/Sound/Enemy use array storage; this important
identity risk is queued for its own target audit, not silently repaired here.
The header therefore shares only the class. Item's affected TU compiles pinned.

Real graph now includes EnemyEclDispatcher for sound/angle helpers in addition
to EclVm, EnemyCallback, AnmManager and Gui. Same-context baseline2326/2412
(raw205,193/2152 comparable), final2356/2412 (raw187,175/2144 comparable).
Full decode/normalization complete, no exact claim. Final PE
b66b83e834400966e44baa02a3353e8923b2b099b2baaf38d915889d39f68056,
guarded EnemyManagerUpdateCallback entry4032A0. Evidence is
.analysis/gpt-dots-enemy-runtime/tail-final.json and tail-final-native.txt.
Actual native calls retain EDX manager/ESI position/stack-id setters and FindVm,
ESI sound-owner/EBX sound-id/stack-position queue, and ESI timer/stack-delta Add.
Existing helper exact status is not new credit. Earlier interpolation numeric
mismatches remain open; adding real sources does not establish runnable closure.

Independent full owner/source/native/test review passed. Actual tail, maintained
setter bodies and timer Add are tested with synthetic lookup/sound boundaries:
first lookup replaces the global manager yet all nine calls use the captured
receiver; root-child propagation, non-root suppression, both position spaces,
missing VM with mutation gates off, sound thresholds and timer gating, and sound
mutating timer before fractional decrement are exercised. The sound fixture
owns a real object behind a synthetic byte pointer; it does not prove production
array storage/alias lifetimes. Player selection and dangerous null-gated paths
are not fully covered, and host tests do not establish x87 equivalence.
No exact numerator/denominator changes.

### Item collection sound receiver identity (2026-10-15)

ItemManagerUpdateBody's three QueueSoundCue calls target immediate owner0x492590 (0x41B446/0x41B68C/0x41B72D, cues29/29/20). Declaring g_MainSoundOwner as a pointer incorrectly dereferences its first word; the existing0x52D0 array declaration restores target addressing. Focused real-registration/queue-source diagnostic confirms all three immediate ESI loads. Full owner remains2272/2320 nonexact; global diagnostic anchor is only four bytes, not owned production storage. Source-extracted tests validate the collection arms/common sound and reject the old declaration through an array-definition type conflict. No new authored exact credit or broad ODR/storage closure.

### Player damage callback descriptor lifetimes (2026-10-15)

CalculateDamageToTarget0x428630 reloads shot->descriptor after collision/pending-interrupt callbacks, caches its type across HasIntervalTick, reloads the hit-animation script after VM removal, then reloads type/damage after VM creation/resolution. A single descriptor captured before the mutable collision callback loses observable state changes. The maintained correction is native-verified at these boundaries and source-extracted regression distinguishes it from the saved old source (old compiles, then assertion fails). The interval mutation fixture is adversarial only; real HasIntervalTick is read-only. Fullowner1148/1170 remains nonexact, with unresolved helper ABI/storage and no new credit.

### Player damage ANM returned identity (2026-10-15)

Target448D00 returns AnmVmIdView through hidden aggregate storage. The damage caller publishes that ID and resolves it through449450; it does not receive a VM pointer from the factory. The maintained damage path now uses existing CreateVmVariant0/GetVm/Release definitions with native EDI-return/ESI-ID evidence and preserved descriptor lifetimes. The full1170-byte damage owner remains1148/nonexact; unsigned-ID reinterpretation and unresolved damage prerequisites prevent lifetime/runtime closure. No helper credit is added.

### Player pending interrupt and effect-row prerequisites (2026-10-15)

0x40C480 is pending interrupt2 publication to a resolved VM/root children, not deletion. It leaves the ID unchanged and does not execute ANM. Recovered PlayerSetShotVmInterrupt2 replaces the misleading delete-state declaration. Effect spawner0x427B50 selects the first inactive of32 rows, preserves full-clear/prefix-clear/sequential-copy order and returns row32 unchanged when full. Existing Player storage has33 rows. These source prerequisites support damage1170/update3089; diagnostics and tests are reviewed but no new exact credit. Effect ABI remains plainRET versus targetRET20, and the auxiliary202-byte damage owner remains unresolved.

### Auxiliary Player damage owner (2026-10-15)

0x4059F0 reads owner4776EC active28/position30/radius3C/mode44, game flag4776F4+378C bit0,477704+10 and474C68. Strict XY circle inside/outside selects the recovered damage return table; equality/NaNs are outside. The maintained helper naturally receives ECX/EAX in its real damage caller and reproduces the target retained-x87 numeric sequence. Helper189/202 and full damage1145/1170 remain nonexact. Shared EnemyGameState.hpp is a narrow Enemy/Player view only, with broader ODR/global storage still open; no new exact credit.

### Player shot bounds storage precision (2026-10-15)

The target damage loop rounds shot minX/minY/maxX through float32 storage,
retains maxY on x87 until its comparison, and retains both half extents. Scalar
locals lost the first three boundaries in the pinned candidate. One ordinary
paired min/max aggregate restores all three spill/reloads without rounding
maxY or changing comparison order/NaN masks. Target 0x4286D7/0x4286E9/0x4286F2
correspond to candidate 0x40297E/0x402987/0x402990; candidate maxY remains on
x87 from 0x402994 to 0x4029BD. This is a native precision correction, not an
exact owner claim: 1151/1170 bytes, 53 raw and 48/1074 comparable, normalization
and full instruction decode complete; RET8 versus target RET12 remains open.

A dyadic precision model distinguishes shotY=1, extentY=3*2^-25, pointY=1-2^-24,
pointSizeY=0: rounded minY equals the point and accepts overlap; the old retained
minimum is greater and rejects it. The actual-source host fixture checks that
finite case, the next lower point and an unordered type-3 case; it does not
prove pinned x87 execution, nor claim the old source fails on SSE hosts.
Target outer-bound half extents still have different spill placement, and the
remaining effect geometry/full owner have not gained numeric or ABI closure.
No authored exact credit is added.

### Rotated effect target-size lifetime (2026-10-15)

The damage owner's rotated-effect branch reloads targetSize after shot callbacks
(target pointer428956, X428996, Y4289D8 after X tests). Reusing the entry-time
half extents was a semantic stale-read error when a mutable callback changes
the underlying non-const size object. The corrected branch reads X and Y at
their separate use points, while axis-aligned effects intentionally retain the
original rounded min/max bounds. There are no opaque calls between the X/Y
reads; FSIN/FCOS precede both. Actual-source host fixtures use a valid global
alias, distinguish rotated versus axis-aligned results, and the saved old source
compiles then fails the rotated assertion. This proves the source lifetime
contract, not that a particular production callback performs this mutation.

The changed lifetime naturally removes initial half-extent spills and restores
the observed target private boundary in this graph: ECX optional flag, stack
Player/position/size, RET12. Real caller4054F9 pushes size, position and owner,
then zeros ECX. Prior shot min/max boundaries remain intact. Full owner remains
1162/1170 nonexact, raw70 and68/1066 comparable, complete decode/normalization.
Other geometry rounding/order, helper conventions and storage remain open.

### Player shot update descriptor lifetimes (2026-10-15)

PlayerUpdateShots0x428280/876 reloads its descriptor after each interrupt pair,
before subsequent type/update-callback selection, and after mutable update and
motion before culling (target4284AD). The maintained source formerly retained
the entry descriptor, so an update callback replacing type1 with type3 could
incorrectly cull the shot, and the reverse could incorrectly preserve it.
Explicit target-observed reloads restore these boundaries. Actual-source tests
exercise both replacements, null callbacks, and adversarial interrupt-boundary
replacement for source-index clearing/current callback selection. The real
interrupt helpers are not claimed to replace descriptors. Saved old source
compiles then fails the first callback/culling assertion; corrected source passes.

The real Player update caller remains in the guarded Enemy-rooted graph.
Shot owner738/876 and caller2808/3089 remain nonexact; motion/interrupt aliases,
manager lifetime and full helper ABI/storage are still open. No new exact credit.

Parked damage geometry detail: axis-aligned effect Y half extent is computed
and retained only after X gates in target428919 onward; current candidate
402BB6 spills it before X gates. A difference from multiplying by0.5 requires
subnormal underflow rather than ordinary normal finite values. This residual
was documented without another source-shape experiment; rotation/circle/full
numeric closure remains unclaimed.

### Player movement/options ANM identities (2026-10-15)

The1591-byte movement/options code now calls maintained SetVmPendingInterrupt,
SetVmPosition, SetVmWorldPosition and InitializeAndExecuteScriptIndex instead
of disconnected descriptive aliases. Target449350 adds224/16 to world position;
the former ScreenPosition label was misleading. The option pair captures the
manager once but reads the secondary ID after the first call. Main scripts1–4
use Player resource+10 and embedded VM+14 with an explicit size/offset-asserted
partial storage view; object lifetime and broader alias closure remain open.

Target code4250B0–4256E6 is1591 bytes, followed by one alignment byte and two
8-slot jump tables4256E8–425727 (physical1656). Candidate physical1588 is1522 code,
two alignment bytes and64 table bytes. Same-scope code comparison is214 raw,
202/1455 comparable, complete normalization/decoding. Earlier comparison in the
local corrected.json is mixed physical/code scope and must not be used as a
full-owner exactness ratio. Both literal tables and all16 destinations are
recorded separately; ledger ownership/denominator is unchanged, zero credit.

The extracted complete movement-owner body host fixture covers scripts/receiver/resource,
mode pending interrupt and position, and option world-position calls using the
maintained setter bodies. First lookup replaces the global manager and changes
the second ID, yet both calls use the captured manager and updated ID. Root
children receive the transformed coordinates. Synthetic boundaries do not prove
native math, embedded-object lifetime, real callback registration or whole runtime.

### Result and replay-name controller (2026-10-15)

Recovered the complete 3227-byte result controller at 4236F0 and its real
179-byte state dispatcher at 4223F0, with six coherent prerequisites (377-byte
score preparation, 89-byte close, 28-byte score synchronization, 70-byte VM-tree
script selection and two 65-byte pending-interrupt helpers). The maintained
0x2C4 prefix exposes two D8 cursors, 25 replay pointers, eight-character name
plus NUL, completion/rank state and saved speed. Original allocation/constructor
and runtime storage remain unresolved. The existing 316-byte InsertScore seam
and two other pause owners (382/1338 bytes) remain declarations.

Confirm and cancel are independent continuations in states8/11/12, while state10
confirm returns before cancel. Saving destroys the previous slot, ignores the
SaveReplay result, reloads the selection, and loads a replacement even on save
failure; a failed load publishes NULL. Cancel from the list deletes all25
non-null owners then clears each slot. Opaque save selection mutation can leave
an old dangling slot; tests clear that synthetic artifact only during teardown,
not in production. This documents original behavior, not a safety guarantee.
Score preparation publishes stage8/format then rechecks stage7 after insertion.
Time publication retains the replay-data pointer field address, then reloads
that field for completion state. Script selection returns an ID aggregate and
has the target's valid-VM precondition even after clearing a missing input ID.

Target result physical3260 comprises3227 code+1NOP+32 table bytes; dispatcher
physical206 comprises179 code+1NOP+12 table+14 selector bytes. Candidate physical
2752/214 has code2719/186. Same-scope code comparisons are135 raw103/2655 comparable
and3 raw1/127; both complete code decodes and normalization pass, but no owner is
exact. Preliminary initial.json mixes physical candidate/code target and must
not be used as a full-owner ratio. The raw selector matches; states11/12 share a
candidate body whereas target has separate bodies. Native dispatcher takes a
stack owner versus target ESI; result preserves stack/RET4. No ABI/runtime or
helper exact promotion follows from successful diagnostic linking.

Actual-source host tests use the verified91-byte ASCII alphabet (seven13-key
rows; special indexes88/89/90). They cover grid wraps, eight-character overwrite,
backspace/finish, all25 occupied or mixed-null slots, save/load failures and
mutation order, simultaneous inputs, timer gates, practice-score maximum,
insertion stage mutation, time-field reload, root/child pending propagation,
negative score-row cancel and complete/incomplete exit. Synthetic types/dependency
sinks establish source contracts, not original callback behavior, x87/native
precision, allocation safety, external storage or whole runtime closure.

### Shared high-score insertion prerequisite (2026-10-15)

The preceding result batch's unresolved InsertScore421FA0/316 is now maintained
and genuinely called by both ResultMenuPrepareScore and the1400-byte FrontEnd
score-entry owner. Signed ties insert ahead of equal scores; lower-than-all
returns-1 without writes. Rows shift downward as complete24-byte records.
Stage/suffix publish their low bytes, time writes the original selected record,
then fresh difficulty selects the blank-name/slowdown destinations. Exactly9
name bytes are written, preserving the tenth byte. A time-boundary fixture
changes difficulty and the timing pointer to distinguish these lifetimes; this
is a contract control, not a claim that real time() mutates game globals.

Native helper4073F0 uses target EDI/RET and one FLD64/FDIV64/FMUL/FSUBR/FSTP32
chain. Its100 constants use64-bit operands rather than target32-bit operands,
but both encode exactly100 and introduce no intermediate rounding. The test
ratio67108863/67108864 distinguishes the retained double quotient from an
incorrect float quotient: correct final float is100/67108864, premature rounding
gives0. Host tests are not pinned x87 execution. Zero denominator/NaN cases
assume the host default IEEE environment and do not prove target FPU masks.

ReplayTimingSource's existing packed+24/+2C double view is now shared by header;
ReplayManager's save arithmetic is unchanged. No duplicate timing class or new
storage allocation is introduced. Profile-record pointer views and generic
storage anchors remain incomplete lifetime/runtime models. Helper263/316 and
both large callers are nonexact; no new authored exact credit.


## 2026-10-16 Pause/capture draft recovery

Four existing dirty bodies are retained as source-present/nonexact: OpenPause
422AB0/382, Resume422C30/74, UpdatePause422C80/1338, and capture request
424480/162. Fresh pinned decoding reproduces every reviewed instruction in
those extents; Resume's old listing also showed four out-of-owner INT3 bytes,
which remain excluded. Natural declarations do not establish private ABI.

OpenPause preserves cached manager/resource identities through allocation and
reloads the global manager only at the observed boundaries. Capture lookup
uses the current global manager while writes use the cached receiver; all four
float conversions precede busy rejection in the target. Resume restores speed
after sound/deletion and clears only the auxiliary ID, not state or primary IDs.
Pause confirm, shortcuts and cancel are independent checks. Confirmation setup
occurs only at timer20, and input activation at30; jumping past20 skips setup.
Original unchecked allocation/null behavior is retained, not repaired.

Target UpdatePause code ends at4231BA, followed by8BFF alignment and the five
entries at4231BC..4231CF. All destinations are valid instruction starts; physical
case order is1,2,4,5,3. A single intact lexical case3 move aftercase5 reproduces
this order without altering behavior. This remains a source-order hypothesis,
not proof of original source. Candidate code1275->1271 and physical1296->1292
remain smaller than target1338/1360: the gap increases by4. Code-only diagnostic
matches are80raw/67of1042 comparable before,70raw/55of1042 after. No byte gain
or exact credit is claimed; the new order is retained for observed topology.

The final guarded ResultMenuUpdate entry is407300 in PE
`d1b47e60e85c75e1191d5370882074eac5325672a74caa29224e0044a03102bf`.
Other draft contributions remain OpenPause465/382, Resume74/74 and capture
168/162, all nonexact. The dispatcher214/179 comparison mixes a physical
contribution with the old code-only ledger and is not an exactness ratio.
Global storage, complete calling conventions and product/runtime closure stay
open. No boundary denominator or exact numerator changes were made.

`scripts/test-pause-menu-state.py` extracts the actual four bodies, cursor
methods and input helper. It tests state/timer boundaries, simultaneous input,
manager/resource replacements, returned VM identity, ID/sentinel preservation
and speed order. Timer and pending/script internals are mocked; finite host
casts do not prove x87 conversion timing or exceptions. The test runs no game
or file IO. It is wired into the existing serial public CI.

Reference review is pinned to
[levzzz5154/th10-decomp eb02753](https://github.com/levzzz5154/th10-decomp/tree/eb027539e55372735db1d60ae5da781a9d1caa8a).
It supplies hypotheses only: assembly fallback, equivalent-region scores,
alternate compilers and context-only steering cannot grant our exact credit.
No upstream code was imported; explicit reuse licensing was not found.


## 2026-10-16 ANM color x87 context audit

[Conditional precision model and negative controls](ANM_COLOR_ROUNDING.md)
separate the pause-linked graph from the genuine ExecuteScript-entry graph.
For the same maintained source, the former retains four scale calls and the
latter inlines them; they have different intermediate precision boundaries.
Small exact-rational witnesses distinguish target and candidate arithmetic
under RN-even PC53/64, conditioned on target conversion-helper binding. Neither
runtime FPU state nor reachable game-state behavior is established. Two
isolated natural-source controls were reverted after failing target topology;
all canonical exact metrics remain unchanged. The reproducible model does not
replace actual-source tests or native acceptance.


## 2026-10-16 Options display lookup and value lifetimes

The 3999-byte RefreshOptionsDisplay owner at0x42E5A0 was reviewed directly
against the pinned TH10 target after reference-inspired questions. Source was
independently written; no reference implementation or synthetic context was
imported. The old generic digit helper sampled a volume once, before lookups,
and shared lookup helpers reread the manager. Those did not preserve the
target-observed value/owner lifetimes.

The corrected owner snapshots the manager after queueing and attenuation;
re-resolves the root ID for each label, searches its layer-node chain, obtains
the child ID and resolves that ID through the same manager. Existing typed
AnmVmIdView storage is passed by value rather than stripped into constructor
temporaries. Failed parent lookup clears the root ID but retains the observed
unchecked parent traversal. Sprite operations guard a null resolved child;
visibility operations retain their unchecked child contract. These invalid
states are not exercised by the host fixture. Other owners' shared lookup
helper and null behavior are unchanged.

Twelve sprite updates preserve label order25/26/27,29/2a/2b (BGM), then
2d/2e/2f,31/32/33 (SFX). Each successful resolution precedes a fresh signed
volume-byte read and its /100, /10%10 or %10 arithmetic. Normal/selected bases
remain0x33/0x3d. Visibility then samples BGM at its branch boundary and selects
<10, <100 or else; the four writes are0000,0101 or1111. SFX is sampled only
after all four BGM writes. Both fields are signed chars in Main.hpp; drawEnabled
is bit1 of flags35C. All other flags and color/alpha fields are preserved.

The unchanged attenuation source was also checked in the selected native
image: its division spelling compiles to FMUL by the same float0.01 bits
0ad7233c as target, with matching x87 grouping through __ftol2 and no intervening
binary32 stores. Constants1.0 and-5000.0 agree too. This narrow arithmetic
observation does not establish full-owner/runtime equivalence; external
storage, diagnostic anchors, private ABI and the remaining native topology
still require closure.

The synthetic actual-source fixture scripts/test-options-refresh-state.py
passes122 cases with read instrumentation and122 with plain signed-char fields.
The previous source has55 functional mismatches even after read logging is
excluded. Seven deliberate mutations fail through observable state/events:
cached digit volume, early SFX visibility sample, manager reload, cached root
ID, unsigned digits, alpha corruption and visibility resampling. The fixture
does not execute the game, prove layout/ABI, model invalid null states, or
establish native attenuation precision. It is part of the serial public CI.

The native graph is rooted at the actual UpdateOptions caller, with FrontEnd
plus ReplayManager, AnmManager, AnmVmCreate, AnmVmId, RandomMath,
EnemyEclDispatcher and Sound under the established /GL /GS profile.
Baseline refresh is201 bytes with outlined digit helpers, not a target-sized
owner. Lifetime correction initially emits1381 bytes and outlines the label lookup.
A single ordinary inline hint emits the same1381-byte owner; the hint was
reverted. Preserving the existing typed ID value then removes constructor
argument temporaries and naturally inlines all12 digit lookups. The selected
owner is2124/3999 bytes, with24 direct FindVm calls,12 SetSprite calls and22
remaining outlined visibility lookups. It is still nonexact:227 raw and218/3647
diagnostic comparable bytes. Selected PE SHA256:
61b21ad1564eca88bdb5c0ea4d95f09ec9fdbc2f8f30baf2cc601110d831ebe2.
The real UpdateOptions entry is402370; its948-byte contribution remains
nonexact against3188. The typed-ID tool request timed out after the work
completed; complete JSON, empty stderr, PE/PDB contribution and source state
were read back before proceeding, without repeating the build. No forced-inline/noinline/profile or artificial-context sweep was used.
The target has70 static FindVm calls; the candidate's distinct helper calls
cannot be counted as byte-exact target code. All native results are diagnostic,
with zero new exact credit. Exact numerator/denominator and canonical manifests
remain unchanged.

Local-only evidence: .analysis/gpt-web/th10-resume-20261016/options-*.json,
options-refresh-target.txt, options-lifetimes-native.txt, options-lookup-native.txt,
baseline source snapshot and test logs. Retain only core source, tests and
conclusions in Git; binaries and generated dumps are reproducible.


## 2026-10-16 Options controller protocol and owner bindings

UpdateOptions42D920 is a3188-byte physical owner (3168 pre-table bytes plus
five dispatch entries). Independent full-target review identified behavior and
binding gaps, not just compiler-register differences. Left volume now checks
the signed byte against5 before subtracting; this matters for-128..-124. Right
volume intentionally retains byte-wrap-before-signed-cap behavior, including
123->-128 and127->-124. Color-right compares the byte unsigned; color lookup
uses0,1,else branches rather than an unchecked three-row array.

Color-left queues sound12 after its sprite pair. Color-right queues12 before
choosing the pair, so mode is re-read after the call. Defaults similarly read
mode after RefreshOptionsDisplay. Each selected pair retains its branch across
its two callbacks. Key-configuration confirmation closes with sound10; return
and cancel close with11. Cancel-to-return uses the actual cursor setter, then
reads cursor.current after the immediate interrupt to form the deferred code.
The low16-bit cast precedes+7/+0x11, preserving native word arithmetic even for
an adversarial INT_MAX callback without signed-int overflow. Initialization
also uses the existing cursor setter. Closing immediate interrupts2 are unchanged.

The parent lookup manager is captured after the left-input getter in the active
state and retained through right/default processing. Opening captures it after
its immediate interrupt. Child lookup now returns the existing AnmVmIdView;
color sprites call its canonical SetSprite seam, which separately resolves each
ID through the current global manager. This distinction is tested with different
parent/child managers and callback mutations. The shared ID-search helper also
serves RefreshOptionsDisplay; that owner's122+122 cases still pass.

Deferred calls now name the existing FrontEndMarkVmInterrupt owner at42C770.
That helper already propagates pending interrupts through root children; no
new propagation behavior is claimed. Screen/state transitions call the existing
canonical members at42C5C0/42C620 instead of duplicate descriptive helpers.
A scoped QueueOptionsSound bridge binds the already-implemented/canonical
EnemySoundQueueView::QueueSoundSample43DC90 to the proven shared sound owner
at492590 with sample0. This removes an unresolved FrontEndPlaySound diagnostic
anchor from this owner. Other FrontEndPlaySound callers remain unchanged. No new storage, owner layout or target body was invented.

scripts/test-options-controller-state.py extracts the actual owner, transitive
helpers, cursor/input bodies and cursor declaration from repository sources.
Its2909 cases pass under UBSan. Thirteen semantic mutation controls are
detected through functional differences, and a separate sample-zero guard passes. Historical source has108 functional failures
in2139 safe cases; its unsafe color-table indices and overflowing INT_MAX paths
are excluded from that baseline run. Creation, refresh, timer, sound and immediate
callbacks remain explicit mocks, and invalid null parents plus native ABI/runtime
are excluded. There is no private snapshot dependency. Tests verify call/state
contracts, not actual audio output or execution of the retail game.

The final unchanged-profile eight-source UpdateOptions-root diagnostic emits
1128/3188 for UpdateOptions and2124/3999 for RefreshOptionsDisplay. Color/lookup
helpers remain partly outlined; both owners stay nonexact. Selected PE SHA256:
94650c98f68b5a88bfcf86788f2ecc65abcfaf6ab64aeab533fcb40d24e9e04a.
This supersedes the earlier source-batch native context, not its historical
observations. The two existing canonical setter owners pass a final focused cold regression
replay:80+83=163/163 bytes and both linkage fields, in one canonical artifact
with image SHA93184d3943e1e30df364fada75c26a75b8711dd7d0afb445b40c680258dba8ce.
Those are existing bytes, not new credit. No match-unit,
boundary, origin, exact numerator or denominator changed.

Evidence is local-only in the existing campaign: options-update-target.txt,
options-update-helpers.txt, options-controller*.json, source/test snapshots,
final setter replay and CI logs. Three source/call-binding diagnostics and the
focused replays were serialized at nice10, with bounded timeouts. A completed
sound-bound build was recovered by reading its complete JSON/empty stderr after
transport timeout rather than recompiling. Reference implementation/context was
not imported; source changes follow TH10-local instructions and existing owners.


## 2026-10-16 Options lookup and color expansion controls

This batch investigates source expansion boundaries, not a recovered historical
keyword or a change of ABI/profile. The production link graph is still unknown.
All controls use the existing eight maintained TUs, UpdateOptions entry,
historical VC7.1 /GL /GS profile, one low-priority bounded build at a time.
No upstream code or synthetic reference context is imported.

Independent target evidence:
- RefreshOptionsDisplay42E5A0/3999 has 70 FindVm call instructions and twelve
  AnmLoaded::SetSprite calls, plus QueueCommand and conversion calls. The prior
  source graph emitted24 FindVm and22 outlined visibility lookup calls.
- Lookup failure produces zero after traversal; successful matches branch to
  separate ID-load blocks. A typed early return on match and default typed
  return on exhaustion expresses that topology without changing list traversal,
  manager/root-ID lifetimes or null/failure contracts.
- UpdateOptions42D920/3188 expands the full three-way color choice at all four
  sites: opening42D9BD-42DB91, left42DDA7-42DFBA, right42E082-42E250,
  reset42E2FF-42E4C1. Across them are24 parent FindVm calls and16 typed SetSprite
  call instructions after tail sharing. Reset still resamples color after
  Refresh; it is not a constant-color specialization.
- Parent lookup retains the cached manager; typed SetSprite449670 reloads the
  current global manager. Keep that typed setter boundary.

Three scoped controls:
1. Add __forceinline only to the two real options lookup helpers:
   PE87438bc7da1a4c7294069cc67349595c353648b5c85b8a2c4864282a6733a138,
   Refresh3590, Update1112.
2. Return the typed ID immediately on match/default after exhaustion:
   PEa761085daba03249f895796a74302893db7d7eef83e6a5ad9f55a7d5e6ce9c40,
   Refresh3945, Update1136.
3. Expand SetColorModeSprites at its four observed owner sites:
   PE2f2c92b0e8e27d08dbd4d65c1ab362c373dfbc13504dee4a319301b33a01940c,
   Refresh3945/3999, Update3228/3188. These are physical extents, not matched
   byte totals. Refresh structural diagnostic1566/3551 and controller241/2661
   remain mismatches; structural normalization is not canonical acceptance.

The annotations are explicit, reversible reconstruction hypotheses supported
by native call topology and independently tested source contracts. They are
not evidence that the original author used __forceinline. No broad annotation,
register/ABI coercion, padding, source/TU sweep or alternative compiler profile
is justified. Remaining close-wrapper/marking boundaries and internal branch
layout require their own evidence; do not blindly inline all helpers.

Actual-source semantic gates pass2909 controller cases under UBSan and122
instrumented plus122 plain-byte refresh cases. Full target-independent CI
passes. Focused canonical setter replay remains163/163 bytes, single artifact
PEe76b8dd3cf6d68082b8d7e0bd1e0793f09fa026950603d2546a082c354e2ee91.
These are existing accepted bytes, not new credit. No ledger, boundary or
denominator change is made. Large native artifacts stay local and regenerable.

That earlier helper4497D0 review corroborated aggregate ID ownership: both branches
write through hidden output EDI and return that pointer. Its success path,
like refresh cold blocks42ED23 onward, reloads node->owner before reading ID.
That historical candidate kept the previously loaded owner. The complete70-byte
helper now closes in [the 2026-10-20 post-search proof](evidence/gui-child-search/README.md).
Do not replace the aggregate with a scalar merely to obtain EAX or add volatile.
The historical Refresh comparison had29 more target two-byte MOVs and26 more
padding bytes, with offsetting instruction differences; that54-byte extent
gap alone established no missing semantic effect.

All thirteen controller and seven refresh mutation controls were re-run after
the final expansion source and detected functional failures as intended.

### Reproducible Options call topology

Use scripts/repo-python scripts/report-options-owner-topology.py with --image,
--map and --pdb pointing to an existing candidate. It checks the exact target
SHA256 and pinned decoder, resolves private map symbols, and emits JSON call
inventories without building or accepting exactness. Physical contributions,
decoded spans and known code spans are separate. A candidate five-entry tail
is excluded only when bound/guard/index-preservation/table-target evidence
passes the narrow recognizer; unknown spans stay explicitly untrusted.
Twenty synthetic recognizer tests run in public CI without the private target.

For the selected color-expansion image, the controller is3228 physical bytes,
3208 decoded bytes plus20 table bytes at403218, compared with target3188/3168.
It has76 direct call instructions versus78 target; Refresh has84 observed calls
in both images. Matching call counts alone are not semantic or exact proof.
Store generated reports locally, not in Git.


## 2026-10-16 Options close-path ownership

Remove the invented BeginOptionsClose abstraction and place its three existing
statements at the three actual owner sites. Target cancel tail42E27B calls
mark6 at42C770, then queues sound11 and sets state4. Select42E4F6 calls mark6,
selects sound10 and joins the sound/state tail at42E291. No global noinline
annotation or changes to other interrupt callers are needed.

Actual-caller PE043f372e6b8341f9e9f0e2897f799fe941d4a4a90315b5eee0660fc14233578b
now retains all five direct mark-helper calls and seven sound calls naturally;
the close wrapper is gone. It has78 direct call instructions, matching the
target total but not every boundary: three target interrupt adapters remain
expanded into direct manager calls, and input/timer wrappers differ.
Do not infer full graph equivalence from that count.

UpdateOptions grows3228 to3280 physical bytes versus3188 target
(3260 decoded prefix plus20 validated table bytes at4031EC). Structural
diagnostic falls241/2661 to208/2664; retain the source for independently observed
call ownership, not a monotonic score. Refresh stays3945/3999 and1566/3551.
Both owners remain nonexact, zero new credit.

The actual-source fixture supports historical helper and current lexical
forms; its close-sound mutation selects exactly the key-config close site,
excluding defaults. Current2909 cases and all13 mutation controls pass;
historical baseline retains108 expected failures in2139 safe cases.
Full CI passes. Focused existing setter replay remains163/163,
PE3e4ca90636d4beb15873fde6be8ae83d70e46bfbd9abb57e56e383fde37e25db.


## 2026-10-16 Options remaining dependency audit and rejected input control

Complete native target-to-candidate inspection, using target SHA256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040 and lexical
candidate043f372e6b8341f9e9f0e2897f799fe941d4a4a90315b5eee0660fc14233578b,
finds no missing helper semantics in these extents:

- CreateVm42C670/95 versus candidate4013F0/93 captures frontAnm before allocation,
  allocates through the current manager, sets layer15 and flag40000000,
  initializes using captured loaded ANM and original script index, reloads the
  manager, adds variant0 and publishes the returned ID. Instruction streams
  agree apart from linkages and final RET versus RET4.
- Timer427C70/29 versus candidate4014D0/25 reads signed current+4/previous+0,
  skips division when equal, otherwise performs signed IDIV and returns the
  zero-remainder predicate. Two RET/RET4 differences and branch displacement
  account for the four-byte gap; there is no missing comparison.
- Immediate adapters42C730/27 and42C750/27 read the selected ID, load the manager,
  preserve EDI, supply interrupt2/3, call449250 and restore EDI. Source behavior
  agrees. The target places adapters only at cancel-to-row5 (42DCA4) and the two
  closing calls (42E531/42E53D); opening and cursor-change execute directly.
  Uniform wrapper insertion or expansion would not restore this asymmetry.
- Input40ACE0/33 tests pressed+6 before repeated+4. The private wrapper4014A0/38
  specializes the global object; even its wider repeated load tests only the
  relevant low16 bits. Ordinary predicate semantics agree.

A single direct-member control replaced the four UpdateOptions InputRepeated
uses with g_FrontEndInput.IsRepeated, without attributes/profile/TU changes.
PE89d5b2ffd1c30db872a8e2b552ad2b35ce9f6caba63204e049bc39b93607ed02 emits
controller3296 physical bytes and inlines all four input calls, losing the two
target call boundaries (76 total calls rather than78). Refresh stays3945.
The source and fixture changes were reverted exactly to the lexical checkpoint;
the bounded patch/report remain local evidence. Do not force partial inlining
with arbitrary annotations.

PlayerTimerView::HasIntervalTick is the genuine existing canonical29-byte
normal-COFF owner, but the options field is a separate AnmVmTimerView. Shared
20-byte field layout alone does not justify reinterpret-casting whole timer
objects or duplicating a forced member; their maintained Add APIs differ.
A sound binding needs shared type/owner evidence and an appropriate producer
graph. Current controller tests explicitly mock creation, timer and immediate
execution; they do not certify those bodies. No exactness credit is claimed.


## 2026-10-16 Options refresh canonical code-generation candidate

Two further independently evidenced source expressions close the native gap:
successful child-ID return repeats node->owner (as target cold return blocks
and helper4497D0 do), rather than retaining the earlier comparison pointer;
attenuation's nonzero arm precedes its zero arm. The arithmetic, typed ID
ownership and all audited state/read contracts are unchanged. No volatile,
register annotations, padding, ABI changes or profile changes were introduced.

The complete3999-byte RefreshOptionsDisplay extent42E5A0-42F53E, including its
169 bytes of interior compiler alignment, now passes two independent canonical
cold links using frontend-options-refresh:
- PE5ebd74baa0c338d98d9aab2b840e6866caf66c57a7a9319d466b80e90bcc854b
- PEf39f571d4734c3215db0d0a4f7431d2b4c38fba6190da8a32b99809b5051658a
Both are3999/3999 with112 exhaustively decoded and semantically resolved fields,
unique source.ltcg.obj PDB ownership, complete Capstone normalization and no
differences. The target extent SHA256 is
7bdfa912c5ce94c888313ef961b788ec6a2f47a074d9534ba251776601cb9c6b.
These are canonical code-generation results, not raw whole-image equality.

The112 fields comprise84 REL32 calls (70 FindVm,12 SetSprite,QueueCommand and
__ftol2) and28 DIR32 data references. The initial per-owner recipe used
g_MainSupervisorView base491C30 with offsets138/139; the shared-owner audit
below supersedes that decomposition with491C28+140/141. The diagnostic anchor's
nearest-public Sleep label is incidental and is not used as semantic identity.
Other destinations are manager491C10, sound owner492590, volume slots497854/
497858, attenuation49785C, SetVol46ECE8, and float constants470B00/470AFC/470B80.
SetVol's seven bytes and the three float32 encodings independently match;
__ftol2 is already supported by canonical runtime-crt-00463b2c's117-byte
libcmt.lib member. The harness remains nonruntime data anchors: live storage
identity, whole-product linking, startup and gameplay are not established.

Producer context is the maintained UpdateOptions caller and the same ordered
eight real TUs used for the diagnostics, historical VC7.1 /GL /GS, and the
standard linked-PE link profile. The112-field manifest, source and tests are
the retained reproducible evidence; private images/PDBs/dumps remain ignored.
No upstream code or reference synthetic context was imported.

Final source gates pass2909 controller cases,122 instrumented plus122 plain
refresh cases, all13+7 mutation controls, and20 topology recognizer cases.
New exact-source TimerReachedMultiple tests add6716 defined UBSan cases in
normal and Python-O modes, with three deliberate mutations rejected. They
cover unchanged-state short circuit, backwards transitions and signed
divisibility, while excluding undefined changed-state divisions. They prove
the predicate separately; controller creation/immediate-execution mocks and
native ABI/runtime limitations remain explicit.

The exact ledger row registers an imported Factory replay candidate based on
these local canonical results. At this checkpoint, fresh Factory receipt
acceptance is still pending; it must report pass plus accepted before a live
accepted-fact claim. UpdateOptions itself remains nonexact (latest3276/3188).

Final toolchain verification, tracking validation and full public CI pass.
The two previously accepted screen/state setters remain163/163 under focused
canonical replay, PE2f6c35c544d6d7f71475f8098371dca0735ca66f8acf755969fc620022f21220.
This existing-byte regression check adds no new credit.


## 2026-10-16 Supervisor storage identity and hint-field correction

A cross-unit check exposed a real integration defect after the first Options
codegen acceptance. That result compared3999 bytes correctly, but its
491C30+138/139 base/addend decomposition was inconsistent with the shared
supervisor owner used by the other nine units. Per-owner symbolic codegen
equality alone did not establish shared runtime storage identity.

Independent native evidence fixes the owner at491C28:
- Initializer4658F1 loads ESI=491C28 and calls421F00; destructor4659F0 uses
  the same base. Constructor421F01 forms configuration pointer ESI+120.
- Constructor421F30-421F37 clears1E1 DWORDs (784 bytes); this is an observed
  clear extent, not proof that the current partial C++ view models every field.
- Instance storage438B51 uses491C28; Direct3DCreate9 result438BCA uses491C2C;
  the device is491C30. These corroborate existing source offsets0/4/8.
- Configuration initialization/copy at420876 and42089F-4208AA use491D48,
  thirteen DWORDs; shutdown438EFA-438F09 saves the same34-byte record.
- Default initializer418C40 writes record+20/+21/+22 at418CB3/418CB7/418CB0:
  BGM100, SFX80 and front-end option0.

Correct retained layout:
- D3D colorMode: +13A, address491D62, used by Main.cpp format selection
  (4398DE/4398E7/439902).
- windowed +13D and frameskip +13E remain unchanged.
- bgmVolume +140 (491D68), sfxVolume +141 (491D69).
- hintMode +142 (491D6A), separate from D3D depth. At419009 this byte gates
  automatic export/overwrite of hint/hint_auto.txt when equal to 2 (string46D6D8); no individual mode-label meaning
  beyond the audited byte protocol is assumed.
- serializedConfiguration +120 and options +150, and all later offsets,
  remain unchanged. Only configuration padding is repartitioned.

FrontEnd now uses hintMode and SetHintModeSprites; Main.cpp's colorMode uses
are untouched. Refresh's16 supervisor linkages now use the common491C28 base
with eight140 and eight141 addends, leaving every target operand unchanged.
No guessed pointer aliases, moved whole object, volatile or ABI changes are
introduced. The corrected refresh canonical replay is again3999/3999 with
112 fields, PE438159b932caf465fec4dc6463c2c33645178cda5779d26fb031642393cb3aa3.

The controller fixture now gives D3D colorMode its own untouched canary:
2909 cases pass; all14 mutations are detected, including wrong-field access
(903 functional and865 canary failures). A target-independent cross-unit
check covers all72 exact-symbol bindings and requires one constructor-attested
base; four failure controls reject shifted base or missing/obsolete addends.
Old source field conflation is rejected rather than silently aliased by tests.
All nine existing supervisor-referencing units were replayed separately from
the corrected refresh; see the bounded regression summary below.

This repairs shared declaration consistency, not complete product/runtime
closure. Existing Options codegen acceptance at a3e075b is historical evidence;
the corrected checkpoint requires a new fresh Factory receipt. No new exact
bytes are added by this repair.

Affected-unit regression totals: nine existing units3696/3696 bytes in six
serialized producer groups; including corrected refresh, ten units7695/7695.
The wider manifest scan finds83 named-global symbols and no inconsistent
same-symbol base. Public checks retain this invariant, while compiler-generated
literal pools are deliberately outside that named-global rule.


## 2026-10-16 Key-configuration digit display reconstruction

Native owner 42F8B0-430243 covers2452 bytes, including15 interior alignment
bytes; the following12 CC bytes are excluded. The EAX receiver and retained
manager in EDI are observed private compiler context, not a guessed public ABI.
The owner reads manager491C10 once, then performs twenty label operations:
43/44/4D/4E for shot,45/46/4F/50 for bomb,47/48/51/52 for focus,
49/4A/53/54 for menu, and4B/4C/55/56 for skip. All digit bases are33.
Each row alternates signed-short division by10 and signed remainder by10;
there is no clamp, unsigned conversion, or added one.

Every operation resamples root slot2 (+2CC), resolves its typed ID, traverses
from the parent node, selects the first matching signed script index, rereads
that node's owner for its ID, and resolves the child through the retained
manager. An exhausted list still resolves zero ID. Missing resolved children
skip their digit write. Parent resolution failure clears the root ID but is
followed by unchecked parent traversal in the target; invalid parent/list/file
states remain outside the host fixture's valid-state contract.

Only after successful child resolution does each operation read its binding
(short fields59CC through59D4) and current resolved VM's anmFile. Native code
contains40 FindVm calls,19 SetSprite calls and one final SetSprite tail jump.
The historical source used root1 and cached binding arguments too early.
The replacement uses explicit guarded operations and separate typed lookup
helpers. Their scoped __forceinline markers are target-observed expansion
hypotheses, not claims about original spelling. No external code was copied.

Source-extracted tests cover244 instrumented and244 plain-short cases,
including all callback positions for root/manager/binding changes, child-ID
remapping, resolved-file replacement, shared children, signed endpoints,
missing/unresolved children, parent-first matching and duplicate-first choice.
Eleven deliberate mutations fail functionally. These tests prove the scoped
host behavior only; native layout, private ABI, call topology and exactness
require independent target and compiler evidence.

The first cold probe under the actual enclosing FrontEnd::Update caller and
maintained eight-TU /GL /GS graph reproduces2452 bytes with2208/2208 nonfield
bytes equal. Independent linkage review covers all61 fields: forty FindVm
REL32 to4491C0, twenty SetSprite REL32 to43E5A0 (including the tail jump), and
one manager DIR32 to491C10. Target extent SHA-256:
1c8190ce6d9917294072ae0c8f07ae588851c615e2130d2b9f4ffb51d1eb25c3.
The same diagnostic preserves the3999-byte OptionsRefresh structure.
UpdateKeyConfig remains nonexact (536/880 in this context); it receives no
exact credit. Nonruntime unresolved data anchors, whole-product linkage,
resource ownership, actual runtime and portability remain separate open work.

Two subsequent cold canonical builds reproduce all2452/2452 bytes with complete
normalization and61 reviewed fields. PE hashes are
7b4145567a202878148f96db9f71b98f753f3ef1ed51a95350e8dc2662b051b4 and
ed25ac8be4930ca208db41ba626d67aed43f6dae0405b9f83ab76c8089fc8997.
Current-source OptionsRefresh cold regression remains3999/3999, PE
77c02488a30e2cf3e23bc4320bebc0f7babc92221442e3f1bdf626b8bcdb73d7.
The exact ledger registers this canonical candidate; fresh Factory acceptance
is pending the committed checkpoint and is not implied by registration.


## 2026-10-16 Practice controller and explicit profile-unlock contract

Native UpdatePractice owns 431EE0-43268F (1,968 bytes, including its state
table). Three target/source discrepancies are corrected:
- The initial FindVm at 431FB7 tests slot 5E without clearing a stale ID before
  CreateVm at 431FC6. The source now performs that direct existence check.
- Final initialization CreateVm at 432090 falls directly into the opening
  timer comparison at 432095. The source preserves that fallthrough, including
  when a creation callback has changed timer/state. Opening is strictly >6;
  closing is >=6.
- Secret progress at 432447/43244A uses CMP 22 / unsigned JB. Negative signed
  bit patterns therefore take completion rather than indexing before the key
  table. The source uses an explicit unsigned comparison at this boundary.

The completion path reloads profile slot 47783C at 43244C and passes it to
42C8C0 at 432451, then queues sound 44, then resets progress. The old unresolved
zero-argument declaration omitted this receiver. A natural unsigned-char*
argument and maintained body now expose the dependency. The 93-byte target
42C8C0-42C91C has no calls or global rereads and writes exactly 160 bytes to 1:
16 at [1D882,1D892), plus 6 groups x 4 difficulties x 6 stages at
4E9 + group*437C + (difficulty*6+stage)*8. All other bytes remain unchanged.
Its original authored/library/outlined origin remains indeterminate; source
presence and a compiler match do not change that classification.

The fixture extracts actual cursor, count, row-interrupt, secret, unlock and
controller bodies. Twenty-five scenarios pass in plain and instrumented UBSan
builds; twelve mutations fail by explicit functional assertions. It covers
four states, callback reload ordering, confirm/cancel coexistence, row ID
retention, keyboard transitions, timeout boundaries, full unlock write-set,
fresh profile identity, and sound-before-reset. A checked proxy rejects the
historical negative key index before memory access; crashes or sanitizer
failures never count as mutation detection. Host dependency mocks and synthetic
callback mutations are not claims about reachable game behavior or native ABI.

A related C++ view-bound defect is also corrected without shifting storage.
CreateVm 42C6C3 stores [controller + script*4 + 2C4]; DeleteVm reads/clears the
same address. Practice consumes slots 98-AC at +524..+574. These lie inside the
previous anonymous unknown524[AC] region, before difficultyAuxVmId at +5D0.
The minimum attested vmIds[AD] now consumes that region through +578, leaving
unknown578[58]. Assertions retain every subsequent field and check slot AC,
array byte size 2B4, and remaining padding start. Constructor 42C920 clears a
5ACC-byte allocation, but does not prove a larger individual VM-array bound.

The first actual Update caller diagnostic remains nonexact for UpdatePractice:
1,540/1,968 candidate/target bytes versus the previous 1,556-byte candidate.
This is a semantic recovery, not a similarity improvement claim. Both display
owners retain their complete structural matches. The explicit unlock helper
naturally emits all 93 target bytes raw-equal, including EAX receiver/plain RET,
without ABI annotations or artificial padding. Its target extent SHA-256 is
819a3df48392e782e0c6da243a93c3423461f1ed18edcb60e2c3774e4b64742e.
Canonical replay and Factory acceptance remain separate required steps.

Two canonical cold shared-producer replays after the view-bound correction
reproduce 93/93 raw-equal unlock bytes and 2,452/2,452 KeyConfig bytes. Images:
4dfbb5a9df7ab9fff38987e959a9adda3a80b3126327f4c356b22b4a7c186abe and
e7eca6f2491fc7cb5bdda744476f8f4bdbf2fa5ce55d41e1227e90382fcf0db2.
The shared producer requires the same output directory as KeyConfig; no
additional context or source sweep was introduced. Registration imports a
candidate; fresh Factory pass/accepted verification is pending this checkpoint.

Bounded primary-source regression: all14 canonical units whose primary source
is FrontEnd.cpp pass, totaling7,186 bytes including the new93-byte dependency.
This is not a replay of every transitive repository unit or runtime closure.
The complete public CI and source-extracted checks pass at this checkpoint.


## 2026-10-16 Stage-HUD selector, sounds and receiver lifetimes

The 3,836-byte UpdateStageElements owner has two concrete input corrections.
The seven-way table at4157E0 maps stage7 to414E71: signed state CMP24/SETGE
plus8C chooses script8C below24 and8D otherwise. The previous source used
8D/8E. The selector fixture extracts the maintained helper and checks96
stage/state combinations, including signed endpoints and out-of-range stages;
three compiling mutations are rejected by explicit functional diagnostics.

Countdown decreases to0..5 set both pending interrupts9, then queue sound24
(sample0) through owner492590 at415501. Decreases to6..10 set interrupts8 and
queue sound1B at415534. At11 and above no sound is queued; increasing seconds
sets interrupts7, while unchanged seconds do not update sprites or sound.
The old GuiPlaySound(0) placeholder lost this identity. HUD now uses the
existing EnemySoundQueueView::QueueSoundSample contract and shared sound owner;
the unrelated message WAIT placeholder is unchanged.

Three native lifetime contracts are preserved without claiming a demonstrated
reachable gameplay bug from synthetic callbacks:
- Counter474C4C is captured at414A60 before ExecuteScript at414A72. The five
  digit calculations consume that retained value, not a post-call global read.
- Message is captured at41513C. Run success ticks this same object at415154
  onward; failure deliberately reloads gui->message at4151A5 before cleanup.
- Enemy manager is initially selected at41549A. Countdown gates require a
  live boss and absent message; after sprite calls,4155AE reloads the manager,
  then4155BA selects the boss retained across marker execution4156D5. The
  player is read after that execution. Existing null/valid-state assumptions
  are preserved, with no invented post-callback null-manager guard.

Native SetSprite and QueueSoundSample are call-free. Their maintained writes
are to supplied VM fields and sound queue state; no real manager/counter
replacement was established. Run still has unresolved transition/text/effect
seams. Tests deliberately vary these identities to distinguish source ordering,
not to establish arbitrary callbacks as valid game executions. The full-source
HUD and spell helper fixture also checks fresh sprite resources/seconds,
message replacement/nulling on both outcomes, countdown thresholds/gates,
and boss retention. Nine compiling mutations require normal exit1 plus a
CHECK diagnostic; crashes and UBSan failures are not accepted as detection.
Both test suites compile under UBSan and preserve explicit scope limits.

A first fresh diagnostic uses the maintained UpdateStageElements entry with
Gui primary and actual ANM/create/ID/RNG/sound/Main definitions under the
existing GUI LTCG profile. PE5fa379aa59af8ce9e137fa643bde3bc33674cc66641d988e22077cc9f2210c0f
emits HUD682/3836 and message Run1296/4480, both nonexact. Major source helper
boundaries still remain emitted where target phases are lexical. This is a
baseline for bounded ownership work, not a flag/TU sweep or exact claim.
The external raw callback415AE0 is not newly reconstructed by this diagnostic.

The malformed functions.csv owner field at4496A0 is repaired to authored,
as already independently recorded in function-origins.csv. Progress derives
origin from that authoritative table, so this fixes metadata consistency
without changing any counts or bytes. No origin classification is promoted.

Focused canonical regressions pass four existing units totaling153 bytes:
GuiScore::Add46, the two ID sprite wrappers36+34, and Main next-mode setter37.
This is a bounded dependency check, not all transitive units or a new exact
claim. Full public CI and both source-extracted UBSan fixtures pass.


## 2026-10-16 Lexical ownership of the HUD boss-gauge phase

The fresh HUD baseline still emitted UpdateBossGauge, SetBossGaugeInterrupt,
BossGaugeScript and CreateGuiVm calls that do not exist in native
414ACF-41513B. One bounded control restores this phase directly in its owner,
leaving canonical ANM wrappers, compiler profile, support graph, message cleanup
and spell-marker phase unchanged. No inline directive or ABI annotation is added.

The phase owns six primary-before-secondary lookup/root-child interrupt walks,
its seven-stage selector, and two creation/publication expansions. Manager ESI
is captured at414B60 and retained through side transitions and allocation.
Front ANM is captured before AllocateVm at414E8B/414F59; after InitializeVm,
414EBA/414F83 reload the manager for list insertion, ID allocation and later
segment allocations. Node owner/next/previous, head/tail insertion including
successor splicing, modulo-32-bit ID increment with zero skipped, VM ID and GUI
ID publication follow target order. The shared wrapper definitions are untouched.

Native active-boss paths require a live player and ANM owner; creation also
requires a live resource and successful VM allocation. The former helper's
extra null-player/resource suppression is not reproduced. Traversed node owners
must be valid. Side-transition loops use signed segmentCount with no invented
clamp; upkeep visits exactly ten slots and removes excess IDs. Tests keep
positive counts within the ten-slot storage domain. Hidden paths preserve
segment IDs, life, target and flags while clearing displayed gauge and only
the four interleaved value fields, matching415132's final+9EAC store.

The new source-extracted phase fixture covers list priority, root/child and
nonroot propagation, successful removal and ID clearing, missing/zero IDs,
empty and nonempty insertion, preserved successor splice links, manager/resource
capture and reload, ID wrap, count boundaries, stage gates, position thresholds
and interpolation clamp. Twelve compiling mutations require explicit exit1
CHECK diagnostics without UBSan failure. Existing counter/message/countdown
checks retain a declared phase seam; this new suite executes the actual current
lexical block. Selector tests also extract the current lexical selector. No old
helper copy substitutes for the candidate. Synthetic pointer changes establish
ordering only, not game reachability, native floating precision or runtime safety.

One fresh same-graph build yields PE
a46288358999aab3382e7d9e082afbe4490c080a9b684bd8d8e5e6f83a8cc119.
HUD is2,388/3,836 bytes, with boss phase4020D4-402747 at1,652 bytes versus
native1,645. Its only phase calls are AllocateVm at4024AB/402587 and InitializeVm
at4024CF/4025AE. The complete seven-entry selector at402838-402853 lies inside
its PDB contribution, ending402854 exclusive. Former phase helper calls are gone.
This is topology/ownership evidence, not a match percentage or exact claim.
FinishMessage102 and spell helper587 remain outlined. Message Run remains1,296
bytes and is separate future work. Diagnostic table-tail decoding is incomplete;
only complete canonical replay can grant codegen credit.

After lexical recovery, the same four focused canonical dependency units
again pass153/153 bytes. All three UBSan suites (12 phase,9 lifetime and3
selector mutation controls), target GUI report and full public CI pass.
No exact ledger or origin count is changed by this owner-boundary checkpoint.


## 2026-10-16 Complete lexical HUD workflow and remaining codegen gaps

Message cleanup is now owned directly by UpdateStageElements. Entry message
41513C is retained for Run and the success timer. Failure reloads the current
message once at4151A5 and manager once at4151B8, then processes six slots
+40 through+54. Each primary-before-secondary lookup marks the selected VM and,
only for a root, its flat child chain with deletion bit04000000; unrelated
flags are preserved. Each ID clears after its walk. Captured message is freed
at415488 after the final clear, and the GUI owner clears unconditionally on
failure at415490. Null post-Run owner skips walks/free but still clears owner.
Source-extracted tests cover valid doubly linked lists, stale/zero IDs, both
lists, root/nonroot behavior, captured free and owner clear. Sixteen compiling
mutations fail by explicit assertions. Manager retention and intra-walk order
are native/source evidence, not invented production callbacks.

Spell/countdown and marker operations are also lexical. Every native skip
reaches the existing GUI frame timer at415764. Active marker paths require
nonnull game/player owners, matching4155EB and415702; former unsupported null
fallbacks are removed. Countdown sound IDs, sprite reloads, post-countdown
manager selection and retained boss remain as previously audited. Four marker
phases use strict thresholds and preserve unrelated flag bits. A first full
build genuinely emitted three artificial threshold/interrupt data tables, so
one bounded control replaces them with target-observed mode-first immediate
branches. Deep-crossing tests prove at most one transition per call.

The fade expression separately widens before subtracting. Target sequence is
FLD, FSUB415702, FABS415708, FCOM qword64, FMUL dword(-191/64), then integer
conversion415728, with no intermediate single-precision store. The current
candidate also has no F32 spill. A defined field-value witness is boss64 and
player2^-20: distance64-2^-20 and scaled product-191+191/2^26 give alpha254
with round-to-nearest PC53/PC64. PC24 can round subtraction to64 and give255
without a store. Runtime precision/rounding control is not established; host
double tests are a conditional model, not observed gameplay, historical source
type inference, or all-mode/NaN equivalence. The prior VC7 expression's spill
behavior was not used as evidence of a runtime bug.

One final native/source audit removes only the counter loop's extra frontAnm
null guard: target414A87-414A98 unconditionally calls SetSprite. Full-owner
fixtures now provide a valid front resource; no null-member crash is tested
as success. The other player-overlap branch factoring remains unchanged.

Reproduction uses the maintained UpdateStageElements entry, Gui.cpp primary,
and ordered AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp, RandomMath.cpp,
EnemyEclDispatcher.cpp, Main.cpp supports, with the public LTCG_PROFILE and
standard cold-link profile/harness. There was no graph/profile sweep.
Progression in that same context: message recovery3,112 bytes; complete spell
recovery3,668; immediate branches3,772; direct counter call remains3,772,
versus native3,836. Final PE:
fdc95d2154daeb930cd4ae08db3ae8ee996b3a81f036dcb14973fa0aa5b216e1.
These are extent sizes, not matching percentages. All21 native call identities
and counts are represented; private phase helpers are gone. The reviewed
seven-entry table remains inside the PDB owner. Linear instruction decoding
alone is not a table proof; the existing comparator accounts for authoritative
PE DIR32 relocations, and a future canonical claim must bind every field.

Remaining differences are explicitly open: register/branch scheduling and
alignment; qword versus dword multiplication of the same exactly representable
coefficient; ordered versus unordered comparison paths; and an optimizer-held
manager value where native reloads the global. Maintained queue and SetSprite
bodies do not mutate that manager, so do not manufacture volatile or opaque
callee effects to force a reload. No further source-contract mismatch was
established in this bounded pass. HUD remains nonexact, with zero new credit;
Run1296/4480 remains separate work. Preserve this reviewed workflow rather than
launch an unbounded spelling, register or translation-unit sweep.

Final serialized regression covers all seven configured canonical units whose
primary or support source is Gui.cpp:568/568 bytes across their declared
producer groups. This does not grant exactness to the HUD owner. Four UBSan
suites and55 explicit-failure controls, target GUI report and full public CI
pass. All source, tests and compact notes are retained; generated builds stay
local and ignored.

### 2026-10-18: Radial initializer signed-RNG local canonical closure

The signed RNG conversion now uses one explicit float local before the unchanged division/subtraction. One candidate cold link in the genuine four-TU ExecuteScript /GL /GS graph supplied the complete 803-byte radial owner; canonical replay matched 803/803 bytes, and all 29 affected existing canonical units were preserved. Factory-controlled acceptance remains pending. ExecuteScript remains 9948 bytes against target 9964; no runtime semantic or physical diagnostic-anchor storage proof is claimed.

- Candidate RandomMath.cpp SHA256: `5866b8573e21b45ff421d4e3930a1d58660a4cf7725e39fd9ec319cc04f7fca1`.
- Complete radial owner SHA256: `397b4d342d864ceb7c6d94b4da09a486c7337184f9c951f3ba2e616893796f08`.
- Private binding inventory SHA256: `4342ab97b08e082719bdd0f6e00f2c88521350b98ee10dbc9a9576dcfe2e9e58`.
- Actual complete canonical replay: `fc309434dbb1496aad942db711ef2761`; additional 23-guard finalization completing all 29 guards: `3f15976898af4bbd951471e67e66a12e`.

See [Radial signed-RNG local evidence](evidence/radial-rng-local/README.md) for source/tool/graph identities, reviewed compile-address binding scope, reproduction limits and the pending Factory handoff.

### 2026-10-18: radial initializer Factory acceptance

The complete 803-byte `anm-initialize-pulsing-radial-trail` claim passed Factory forced-recompile replay and was accepted at clean commit `504539f9fcd1e465f7fd7e902af05c042e5d077d`. Receipt: `dd3a0154fc4a5a6ac0ae07cb3e17cde3bf0326b91382e7abf1bb7a78c8a334b0`; job: `40f82069fb8d46dea5d5441fe851b0e6`. The separate 115-byte signed-RNG claim was also accepted at the same source binding, receipt `64fd20e83b5c945ac8005e3af3977730dd9b5550709a6db3e5deebc32e9bce58`. See [the dated radial acceptance record](evidence/radial-rng-local/README.md#factory-acceptance-at-504539f9-2026-10-18) for exact source binding and limits. Earlier Factory-pending wording is historical; ExecuteScript remains outside this exactness claim.

### 2026-10-19: GameManager static callback retirement closure

The previously unmapped authored owner `0x00409E20..0x00409E3B` is reproduced
by the existing static inline `MarkGameChains` in `GameManager.cpp`. Ghidra
confirms a direct target caller at `0x00417C53`; owner `+8/+12`
callback pointers receive independent null tests and node flags `+4 |= 2`.
Two cold pinned VC7.1 SP1 links reproduce all 28 bytes raw-equal, with complete
decoding and no link fields. The pre-existing qualified static owner lane binds
the exact MAP symbol and unique nonoverlapping primary-object PDB contribution;
no C++ body, ABI, profile or comparator was changed. This adds 28 local canonical
authored bytes and one source mapping without changing the origin denominator.
The retained diagnostic static copy does not close the parent's call topology;
GameManagerInitialize and production/runtime ownership remain nonexact/open.
See [the bounded evidence and negative ECL/Bullet controls](evidence/game-manager-static-mark/README.md).

### 2026-10-19: script browser inventory and native owners

Fresh local Ghidra temporary functions and complete pinned-target decoding
close 13 missing boundaries around the debug script browser: 2,371 bytes,
of which 2,293 are reviewed authored game code. Five callback/delete adapters
retain indeterminate origin. The temporary metadata is rolled back under
headless readOnly; target bytes do not change. The existing global boundary
check differs in historical evidence text at 0x401100; prior ledger rows were
preserved rather than rewritten. See [the per-owner evidence](evidence/script-viewer/README.md).

Natural `ScriptViewerView` source reproduces complete ClearScriptNames 86 at
0x409EB0, constructor 234 at 0x40A040, RegisterTasks 106 at 0x40A130 and update
callback 7 at 0x40ABE0. Two independent cold pinned `/GL /GS` links pass all
433 owned bytes and all 17 target-bound fields: **426 authored exact bytes**
and 7 exact bytes whose origin remains unknown. The shared diagnostic also
passes existing cursor Move 128 and input-mask 33 controls. Registration uses
flags OR 2 and priorities 5/39. A masked structural match with reversed stage
global writes was rejected by strict symbol binding before promotion.

Constructor offset 0x10 is a worker-shaped member; Start/Stop establish thread
handle +4, thread ID +8, stop request +12, running flag +16 and entry +0x18. Its
four initial zero stores precede the three cursor constructors, embedded ANM
constructor and whole 0x688 clear. First-word bit 1 and final-word flags +0x684
are separate storage. Fresh control-flow review confirms 0x40A655/0x40A685
calling MarkGameChains and 0x40A749 calling the Player release candidate,
correcting database-limited caller observations without changing origin.

The complete Update 1251 source candidate emits 1280 bytes; manager
initializer 153/101, creator 68/67, loader 16/16 and suspension 39/36 remain
nonexact. Current RebuildPlayerOptions cleanup differs on the latter edge;
its ABI was not falsified to obtain equality. Host fixtures exercise actual
maintained browser bodies and independently reviewed event/receiver/input
contracts, without claiming Windows runtime. The destructor 412, Draw 647,
missing release entries, data owners and product closure remain open. The
reference contributes MIT-licensed source hypotheses; local target and native
Oracles alone establish the accepted facts.

The next browser dependency at `0x405ED0` is a complete 45-byte Bullet pool
reset. It passes manager `+0x3E0B50` to the exact
`AnmRenderManagerView::MarkLoadedVmsForDeletion @0x4493E0`, clears exactly
`0xF82BC` dwords from `+0x60`, stores that array address at `+0x10`, and sets
word `+0x3E07A6` to 5. Existing `BulletManagerView` and `BulletRuntimeView`
assertions resolve these to all 2,001 records and `bullets[2000].state` at
record offset `0x446`. This is target/source-layout evidence only; no new
implementation or exact credit is registered for this dependency.

### 2026-10-19: browser manager and debug input dependency closure

The [dependency checkpoint](evidence/script-viewer-dependencies/README.md)
closes complete Bullet reset 45 at 0x405ED0, Enemy clear 118 at 0x409F90,
Enemy suspension 28 at 0x40D510 and debug input repeat 109 at 0x40AC20.
Two frozen-source cold `/GL /GS` links pass all 300 bytes and four bound fields;
**272 bytes** retain authored origin, while the new 28-byte suspension boundary
stays origin-indeterminate. Temporary local Ghidra metadata and a canonical
mapped-region hash establish its caller 0x40A667 and complete extent, with no
persistent database or target-byte patch. The earlier paragraph's absence of
implementation/credit records the preceding checkpoint, now superseded here.

Enemy clear uses the renderer's actual loadedAnms 9..12, saves next before
virtual deletion, clears +0x64 and the first special slot +0x10, and independently
resumes nonnull callbacks. It does not itself clear active count +0x60. The
local renderer snapshot produced twelve ordinary differences; the shared
array expression recovers the target load/save/register schedule. Host fixtures
include destructor-mutated links and a null resource among all four calls.

The debug input updater clears +4, processes sixteen unsigned 16-bit counters at
+0x38, wraps their increment modulo 65536, and at 26 subtracts 8 and ORs only
bit 0 into +0x30. That field accumulates and the bit never follows the key index.
Edge outputs are +0x32/+0x34. A short explicit changed-bits local differs by
five bytes; AND reversal is byte-neutral, and wide locals regress. Direct
edge expressions enable shared-XOR code and match all 109 bytes raw-equal.
The common minimum InputState view preserves the FrontEnd prefix but establishes
no allocation size, array count or indexed stride; the archived input-native
experiment is not integrated. Browser mask+8 remains neutrally named.

The four earlier browser exact owners pass in the expanded real caller graph,
along with cursor Move 128, input mask 33 and mode switch 37. Update improves
from 1280 to 1267 target-owned candidate bytes against 1251, with 539/923 ordinary
agreement after pointer-based cursor remembrance, but remains nonexact. Its
matching regions earn no separate credit. Worker launch/stop, manager release,
browser destructor/Draw, remaining private ABIs and native product/runtime
closure remain open.

### 2026-10-19: shared worker and Player option-rebuild interface

The [worker checkpoint](evidence/script-viewer-worker/README.md) closes Stop101
at0x44C150, Start56 at0x44C1C0 and browser player suspension36 at0x40A010:
**193 authored exact bytes**, all six bound fields and two independent
frozen-source cold `/GL /GS` links. Existing boundary/origin classifications
and worker medium origin confidence remain unchanged. Fourteen existing
complete-owner guards pass in the two relevant real-entry graphs; no broader
historical native replay is claimed.

Stop's null-handle path preserves every field. Otherwise it repeats stop=1,
running=0 before each timeout Sleep1 and Wait200, then closes the current handle
on any non-timeout status, clears handle/entry and preserves thread ID. Failed
wait/close results are not repaired. Start stops the previous thread first,
stores new entry/running/stop flags, passes the actual six `_beginthreadex`
arguments and stores even a null result without clearing running. Natural
source plus the real browser caller reproduces the target private registers,
without register annotations or code-segment shaping. The browser's no-argument
plain-RET callback address is preserved through the CRT callback-type cast;
original source typing and Windows thread behavior remain unknown.

RebuildPlayerOptions reads its sole pointer at entryESP+4 and returns RET4.
Fresh local Ghidra and complete target decode agree; ten direct calls push the
Player pointer. The database's nine xrefs omit the separately reviewed browser
call0x40A02D. The maintained interface now expresses that cleanup as `__stdcall`
without claiming the original source annotation or changing the algorithm.
The old cdecl suspension caller's extra ADD ESP,4 disappears. All three original
PlayerOptionCallbacks canonical units pass a focused cold replay with the new
entry symbol. Earlier nine-call/plain-RET descriptions are historical contexts.

Full Rebuild remains2408/2520 owned bytes and Update1264/1251. Update's ordinary
structural score changes539/923 to522/923; neither owner earns partial exact
credit. Creator68/67 differs exactly at its unresolved destructor receiver:
native MOV ECX,EBX versus target PUSH EBX. Restoring the real browser destructor
and remaining release bodies is the next concrete calling-context work.
Host fixtures cover timeouts, changed handles/flags, failed waits/close/create,
API arguments and ordering. These checks do not open product/runtime, semantic
or portability gates.

Initializer153/101 has one localized52-byte expansion: target's first edge
0x40A351 calls PlayerCreate425020, while native inlines its allocation,
construction, registration and failure cleanup. The subsequent creator call
is native+0x3A versus target+0x06; the remaining sequence retains that52-byte
offset. This is an outlining/context lead, not proof of an original noinline
annotation or a tested source fix.

### 2026-10-19: complete browser Update and native member lifetimes

The [native lifecycle checkpoint](evidence/script-viewer-native-lifecycle/README.md)
closes complete Update 1,251 at 0x40A450, creator 67 at 0x40A3C0 and
initializer 101 at 0x40A350: **1,419 authored exact bytes**. Loader 16 at
0x40A340, worker destruction 16 at 0x44C130 and six nullable release helpers
(108 bytes) remain origin-indeterminate after exact replay. Six independently
reviewed release boundaries add 109 indeterminate bytes, including the still
nonexact Enemy release 19. Both frozen-source cold links pass all 11 new
owners, 114 new fields and 17 earlier complete-owner guards. These supersede
older browser diagnostic sizes without promoting original ownership or runtime.

PlayerCreate's observed call at 0x40A351 is retained out of line, removing the
52-byte initializer expansion. The worker's table 0x4703E4 enters scalar
cleanup 0x44C100; its +0x10 browser unwind member calls 0x44C130. A true virtual
destructor reproduces all 16 bytes and preserves constructor 234 with a
compiler-owned vtable. Player's unwind at 0x465280 calls ANM VM destruction
0x401FF0 on +0x14. Its draw VM now owns buffer cleanup at VM +0x358 through a
real member destructor; Player destruction itself remains nonexact. Popup's
unwind similarly establishes VM +0x18 and buffer +0x370. These are bounded
lifetime models, not proof of original class declarations or complete sizes.

A nullable explicit destructor/deallocation helper must capture the allocation
before a destructor can clear its global. The reference's pointer-by-reference
hypothesis produced Update 1,296/1,251 with extra address exposure/reloads;
a pointer-by-value parameter recovers the target-sized body. Uniform
`/GL /GS /EHs` still differs at Update +0x273 (LEA ECX versus target EAX).
Keeping Enemy.cpp's earlier canonical `/GL` profile closes both Update 1,251
and EnemySpawn 577 in the same image twice. All inputs retain `/GL`; the
reproduced per-TU context does not establish original production flags.
Uniform `/EHa` regresses constructor to 286/234 and creator to 136/67.
Object order and real-entry controls do not resolve the request edge.

The observed unregister boundary is ECX task, EDX chain, plain RET. Maintained
`__fastcall` preserves complete unregister 114 and RemoveAll 82 in both images.
It does not resolve the four ordinary argument-move order differences in each
of Bullet and Bomb destruction. Declaration/definition/default destructor
controls all emit QAE symbols under VC7.1; ineffective `__stdcall` annotations
were removed rather than used as an ABI fix. Item clear uses the existing typed
manager and exactly **0x896 = 2,198** records of 0x3F0 bytes, total 0x21CEA0
from +0x14. Earlier decimal-896 lifecycle wording was incorrect.

Two Update calls at fields +0x206/+0x236 enter the existing 28-byte primitive
0x409E20. Their static source selectors require the complete reviewed callee,
qualified MAP/PDB owner, dense zero-field decoding, raw full-callee equality
and a direct CALL to the entry. There is no public-to-static fallback and no
duplicate credit for the helper. Synthetic controls reject malformed profiles,
interior/cropped/nonreviewed callees, linked child fields and JMPs. Host lifetime
fixtures cover pre-lock snapshots, changed callbacks, byte-depth wrapping,
reverse record cleanup, global/member order and exceptional buffer cleanup.
They do not establish Windows runtime or production storage ownership. Browser
Draw, larger nonexact destructors, Rebuild and the whole-product gate stay open.

### 2026-10-19: complete browser Draw and owned switch table

The [Draw checkpoint](evidence/script-viewer-draw/README.md) closes complete
ScriptViewerView::Draw at 0x40A940: **664 authored exact bytes**, plus callback
10 at 0x40ABF0 with unchanged indeterminate origin. Two frozen-source cold links
pass both complete owners, all 43 new fields and four existing complete-owner
guards (RegisterTasks 106, Update 1251, AddSmallFormatText 100, DrawNoRotation
515). The existing real browser graph and mixed `/GL` profiles are unchanged;
no assembly, register bridge or code-segment pragma is added. The reference
contributes a hypothesis; TH10-local target and native Oracles establish the claim.

The old boundary stopped at RET after 647 instruction bytes. A single NOP at
0x40ABC7 aligns a four-slot absolute table at 0x40ABC8..0x40ABD7. The selector
bounds unsigned(state-1) to 0..3; slots reach 0x40A9E8, 0x40A988, 0x40AB10 and
0x40AACC, all internal instruction starts. Fresh Ghidra has only the switch's
base reference 0x40A981, no competing body, and an eight-CC suffix before the
separate 0x40ABE0 callback. Thus the full owned extent adds **17 bytes to the
provisional authored denominator**, without changing authored confidence.
Ghidra reachable-body size remains 647. The original PE has no base-relocation
entries; ownership is established by target control flow/pointers, while the
linked candidate's complete DIR32 fields bind all four entries. No partial
647-byte match is credited and no boundary-check policy is relaxed.

Draw always emits the SprtView title. States 2 and 4 return after their loading
or enemy-count text, even with draw flag 2. States 1 and 3 reset the currently
published ASCII manager to white and may draw the embedded VM at (320,240,0)
through DrawNoRotation; its return is ignored. The state-3 position text casts
X/Y to integers. Target reloads the published ASCII/enemy/render pointers
between relevant calls rather than caching the first publication. Host fixtures
extract the actual bodies and exercise these state/format/color/position rules,
manager replacement and draw-flag changes, using finite in-range conversion
witnesses. They do not establish Windows runtime or x87 edge behavior.
All nine reviewed authored ScriptViewer owners now have source; only its
412-byte destructor remains nonexact. Larger manager destruction, Rebuild,
production owners and the complete product/runtime gate remain open.

### 2026-10-19: ECL Run jump snapshot and alias constraints

The [jump-lifetime checkpoint](evidence/ecl-run-jump-lifetime/README.md)
keeps the complete Run7020 owner in scope. Fresh target instructions0x44E40F,
0x44E411,0x44E414 and0x44E416 load one live instruction pointer, read its time,
store currentTime, then read delta from the same pointer. Maintained jump time
and delta now explicitly share one scoped snapshot. Native delta addressing
changes EDI+0x10 to the target's EAX+0x10; result EAX still differs from target
ECX. The single changed ordinary byte remains unequal, so the diagnostic count
stays6125/6264 rather than gaining a matched byte. No normal-input runtime bug,
full jump-base reconstruction, original source identity or exactness is claimed.

Two frozen-source cold real-EnemyRuntimeUpdate `/GL` links retain7020 bytes,
all60 relative switch destinations and88 selector bytes. Seven existing complete
host/operand guards pass all919 bytes and17 target-bound fields twice. Source
and compiler helper interfaces are unchanged. Full fresh-base jump variants
trigger cross-case optimization and shrink Run to6948; a separate member read
for delta retains7020 but shifts later table destinations by2. Neither is
adopted. The scoped operand model keeps the original common advance/base
structure; its remaining alias/lifetime reconstruction is open.

The fresh whole-owner ordinary discrepancy inventory is13 before the FORMAT
physical interval,118 within it and8 after it. Candidate-recognized field
exclusion does not prove semantic case ownership or complete normalization.
The existing88-byte undecoded selector limitation is preserved; no comparator
relaxation, partial credit or parked FORMAT retry occurs. This changes no exact
ledger count or byte numerator. Remaining large-owner register/stack/context
work and native product/runtime closure remain open.

### 2026-10-19: Complete stage Fog and object-VM native closure

The [native closure evidence](evidence/stage-fog-native/README.md) adds533
reviewed authored exact bytes without source/boundary/origin changes: six-float
Fog constructor94, subtraction92, addition92, scalar multiplication101 and
UpdateObjectVms154. Two independent cold real-Stage-Update /GL /GS links reproduce
all complete PDB-owned extents and17 independently target-bound fields. All16
conversion fields bind __ftol2 at0x463B2C; the updater binds ANM ExecuteScript.
Thirteen project CPP/quoted-header hashes agree. The new canonical units pass
at their declared default output directory. Earlier structural-only/no-helper-
credit wording is historical and does not grant the larger evaluator credit.

The updater's148-byte Ghidra reachable body excludes six internal compiler
alignment bytes inside the existing154-byte physical owner. Full replay includes
those bytes; no padding is added and the authored denominator does not change.
Fog arithmetic retains live x87 color results across FST before low-byte packing
without saturation. Its actual complete codegen, including hidden-result and
private argument contracts, is proved only in the declared native context;
original source/class/TU identities and host conversion semantics remain unknown.

The unchanged evaluator remains664/688 with matching0xD4 frame and14 direct-call
identities/order. Stage Update remains294/304. Dispatcher1972 includes its
compiler contribution while the tracked1806 boundary is code-only and table
ownership remains separately provisional; no parent or partial-case credit,
boundary rewrite or native product/runtime closure follows. The next coherent
frontier is the complete evaluator's control flow/temporary lifetime, preserving
the newly exact arithmetic dependencies rather than changing their ABI or
forcing stores. One canonical image remains; duplicate cold artifacts are removed.


## Fog evaluator and easing arithmetic checkpoint (2026-10-19)

The [bounded evidence](evidence/stage-fog-evaluator/README.md) corrects two
native source-model gaps without new exact credit. CalculateAnmInterpolation
uses private EAX mode plus two float stack arguments and RET8 in the target.
The maintained three-argument callee-pop spelling naturally reproduces this
under VC7.1 LTCG and removes caller ADD ESP8 in all six reviewed consumers.
This does not establish the original C++ keyword. Fog modes7/17 really copy
all seven initial dwords before addition; the source now has scoped snapshots.
Its addition remains outlined, retaining its full92-byte exact helper.

Repeated cubic/quartic inverse expressions reproduce the target's live x87
instruction paths: all40 diagnostic mode/arm traces agree after only constant
address substitution and unconditional-transfer elision, versus30 for the
prior stdcall-only source. This is arithmetic-shape evidence, not complete-byte,
execution, exceptional-FP, reachable-input or runtime proof. The complete
candidate still has547 code bytes plus alignment/table for612, versus509+3+64
for the576-byte target window. Cubic/quartic common-tail placement is unresolved.
The target's16 table destinations all stay inside its code; local read-only
Ghidra also decodes a separate57-byte body at44C590 immediately after the table.
No origin, inventory boundary, denominator or helper exactness is promoted.

Fog's selected owner is691/688 with target frameD4/all14 calls. Only six LEA
stack operands differ in an instruction-order diagnostic; physical equality
remains open. Named/temporary/shared initial snapshots, result scopes and a
natural value-return interface do not close it. Nonmember value-add breaks the
helper extent99/92. Rejected APIs are restored; do not repeat these models or
add forced slots/flags without new evidence. The selected real Stage Update
cold graph passes all five existing canonical units533/533 and17/17 fields.
The separate Enemy consumer remains548/652 with the corrected call protocol.
The strict transition fixture's extracted timer/viewport/random dependencies
are unchanged; only reviewed easing declaration/definition pins are recertified.
Original source/TU ownership, whole product/runtime, semantic and port closure
remain unknown.


## Easing owned table and local-link Oracle checkpoint (2026-10-19)

The [complete ownership review](evidence/easing-tail-ownership/README.md)
supersedes the509-byte code-only boundary of CalculateAnmInterpolation.
DEC EAX, unsigned bound15 and indexed scale4 establish all16 entries at44C550.
Each points to an instruction start in the509-byte body. Code, three internal
alignment bytes and64 table bytes form576 owned bytes through44C58F; the next
separately decoded57-byte body starts44C590. Only the indexed jump references
the reviewed tail. The target has no PE relocation directory: target binding
uses actual operands/table values, not invented target relocation records.
A fresh attested architecture export confirms the reachable509-byte body;
physical ownership remains a separate claim. Only the two affected ledger rows
change, adding67 owned authored denominator bytes with zero new exact credit.
Origin and source dispositions remain unchanged.

A causal support-only `/Os` probe, with unchanged source, real Stage Update
entry and preserved /GL /GS, produces560 complete bytes:494 code +2 alignment
+64 table. It coalesces all returns to one RET8 versus the target17. Default
source remains612/576 and Fog691/688; the override is not adopted. This size
policy is closed without further flags or source-lifetime sweeps.

Preparing full table replay exposed an Oracle false positive: local DIR32
fields were accepted whenever their destinations stayed inside the candidate.
A synthetic complete24-byte PE/MAP/PDB contribution, relocated between target
and candidate, still returned24/24 exact after exchanging two table entries
under comparator06106ea. The corrected comparator requires both semantic bases
inside their compared extents and equal contribution-relative destination
offsets after accounting for addends, before rewriting. Five end-to-end controls
pass normally and under Python-O; existing eight context controls also pass.
All three existing canonical units using local fields revalidate2704/2704
bytes/all60 fields, including all18 internal destinations, in retained images.
No native historical cohort is rebuilt and no accepted exact state changes.
Original physical compiler partition and native product/runtime/semantic/port
closure remain open.


## Stage registration graph and complete dispatcher checkpoint (2026-10-19)

The [complete evidence](evidence/stage-real-callback/README.md) closes376
authored exact bytes: StageScriptView::Update304 and AnmVmTimerView::Tick72.
Three independently reviewed seven-byte registration adapters add21 exact bytes
with indeterminate origin. Two independent cold CL13.10.6030/LINK7.10.6030
links, with identical19 project input hashes, pass all987 complete bytes and36
independently target-bound fields, including590 existing guard bytes. The
canonical default recipes pass without another build. The genuine StageCreate
graph uses nine real TUs; no artificial root/caller, ABI sweep or forced stores.

Three explicit auxiliary ExecuteScript calls replace a three-iteration loop,
recovering Update's EAX/plainRET and camera-publication/store scheduling without
memcpy or volatile. Tick's old int candidate74 differs from target72 by one
MOV EAX,EDX. All10 fresh target call sites overwrite EAX before use; void models
the unconsumed result while preserving every mutating statement. Original C++
return spelling, class names and production TU remain unknown. Its inline body
is in AnmManager.hpp; declared native PDB materialization is StageScript.cpp.
The strict transition header pin changes only for that reviewed Tick block.

The dispatcher ownership review includes1806 code bytes, two-byte MOV EDI,EDI
alignment and15 DIR32 table entries:1868 physical bytes through4043CB. Selector
bound14, EAX*4 and all15 internal instruction destinations agree. No owner
overlaps the tail. The target has no PE base relocations; literal scanning
finds the actual indexed operand and one incidental opcode/modrm/disp/imm byte
pattern at41522F, explicitly classified rather than treated as a pointer. This
adds62 owned authored denominator bytes and zero dispatcher exact credit.

Rocking's nonzero/switch-case1 structure restores genuine Tick/SetCurrent
outlining. Case1 re-reads its jump operand after SetCurrent; mode8 stores precede
final tangents; case14 initializes before its negative-index flag path. Current
native contribution1868 and frame8C agree, but complete bytes do not: the dated
ordinary diagnostic is1236/1724. Color loads, vector temporaries, op12/13
registers and op14 VMInit receiver remain open. Old EBX/plainRET dispatcher
and ECX Update claims do not describe this real registration graph.

Four channel snapshots regress the dispatcher to1892/frame98; they are removed.
Same-layout argument slots and one real BulletManager VMInit caller are neutral
and restored. Fresh target VMInit xrefs count23 calls in eight callers, including
15 in EnemyBeginSpell. Adding its genuine EnemyEclDispatcher.cpp in a ten-TU,
27-input control preserves the exact same normalized dispatcher instruction/size
sequence; all987 strict bytes/all36 fields pass. This caller-context route is
closed. EnemyBeginSpell itself remains2476/2432, with771/2032 ordinary comparable
positions in that dated context, and receives no credit. Reference
three-call/rocking/void-timer ideas are corroborative only.
No draw-core, lifecycle, Fog/easing, fragment, runtime, semantic or port credit.

## Spell lifetime and compiler-visibility review (2026-10-19)

The [fresh review](evidence/enemy-spell-lifetimes/README.md) leaves source,
boundaries, origins and exact ledgers unchanged. Four-TU ECL and ten-TU Stage
graphs emit the same normalized2476-byte spell sequence against2432. Ordinary
child-return and node-owner-return controls emit2472 and2484; the latter models
the target success-tail re-read and improves diagnostic alignment, but remains
nonexact and is restored. Moving the shared initializer's flags read before
short metadata stores is byte-neutral: native already hoists the load. Three
separately scoped name-copy forms are also neutral and restored. These are
bounded observations, not an excuse to repeat old uniform-copy or per-arm
lifecycle source experiments.

Target and native AnmVmView::Initialize match all273 raw bytes, contain no calls
and no EDX-defining instructions. Current shared initializer callers exploit
that preservation, holding the VM in EDX and emitting197 bytes; target uses
EAX/ECX/EBX inputs with three other preserved registers and203 bytes. Embedded
initializer is likewise200/202, EDX versus target ESI receiver. The full spell
residual therefore includes a concrete compiler-visibility/register-contract
gap; original physical partition and interprocedural policy remain unknown.
Link-target identity alone cannot make these unlike interfaces exact. No fake
argument, artificial clobber, profile sweep, partial credit or runtime claim.
One minimal baseline image is retained; duplicate controls are removed.

## Primary-stage transition delay closure (2026-10-19)

The [complete helper evidence](evidence/stage-transition-delay/README.md)
adds92 authored exact bytes at4045B0. Actual GameManagerUpdate loads the primary
Stage pointer from4776E8 at4181F7 and calls the helper at4181FC; the earlier
call4181F2 targets404530 with the distinct menu pointer4776E4. This corrects a
preliminary routing label, without changing prior exact/boundary/origin state.

Natural GameResetAnimationSlots reuses StageScriptView::transitionTimer at2A1C
and flags at2A18: SetCurrent60 then OR4. The emitted EAX/plainRET helper matches
all92 bytes and the sole speed-scalar DIR32 at+2C. Two independent cold real
OnUpdate links use the existing single-TU /GL profile and identical20 inputs;
existing callback7 and static chain marker28 also pass,127 bytes/all2 fields
per image. Their recipes share the new output directory with no duplicate
credit. Original identifiers/TU and complete alias/storage ownership remain
unknown. Menu helper119, full update core, product/runtime, semantics and port
remain open.

## Transition allocator and scheduler interfaces (2026-10-19)

Read-only local Ghidra attests all mapped text before/after temporary metadata,
with rollback. The target43C8B0 factory calls452493 with a single size0x44 and
ADD ESP4, then uses EBX priority/five real stack operands and RET20. Fresh pinned
libcmt extraction proves all14 allocator bytes and its __nh_malloc field: the
ordinary one-size operator-new symbol. This supersedes the nothrow hypothesis;
handler/throw/failure and prezero timer object-lifetime behavior remain unknown.

The actual44A000/44A030 convenience registrars consume callbackEAX, priorityEDI
and one stack user argument, RET4. Source/header callee-pop annotations alone
restore both complete43-byte bodies; their mutation statements/layouts are
unchanged. Target43BD40 falling alpha93 and43C3C0 full-screen dim68 have complete
new boundaries, distinct game-specific behavior and only initializer entry
references, no external interior references. Authored_game medium classification
is independent of reference names and compiler matching. Previous registrar
origin confidence is unchanged. Both canonical cold links, with42 identical
CPP/header inputs, pass all591 bytes/24 fields:247 new and344 existing guards.

Initializer43C970 owns396 bytes through43CAFB:357 code +3 LEA alignment +36
nine-slot table43CAD8. All9 destinations are internal starts, selector bounds
are unsigned0..8, physical order0/1/2/3/5/4/6/7/8, and next cleanup starts43CB00.
The corrected ownership adds39 authored denominator bytes with zero parent
credit; the two callbacks add161 reviewed bytes. Native factory remains88/82,
initializer396/396 still has scheduling differences, cleanup127 remains unequal.
The positive factory branch/common source return is byte-neutral. Do not infer
exactness from sizes or relaxed static callback-address normalization.

The14-scenario/843-assertion host baseline, focused init_flags negative control
and19 EOL pin controls pass. Only three dependency LF pins change, with full-file
reversal proof for calling-convention annotations alone. Ordinary-new host
allocation is an explicit failure/prefill seam, not a native exception Oracle.
The MIT sparse reference commitc6791d82 suggests hypotheses; target facts and
canonical comparisons establish acceptance. See [the full scoped evidence](evidence/effect-allocation-abi/README.md).

## Constructor-owned transition reset closes factory (2026-10-20)

StageCreateTransitionEffect43C8B0 now reproduces all82 owned bytes and its two
REL32 fields in two independent cold14-TU Gui updater links, with42 identical
CPP/header hashes and different PE/PDB identities. The existing reviewed
boundary/authored confidence remain unchanged;82 bytes are new exact credit,
with no denominator or independent constructor credit. Existing eight family
units retain591 bytes/all24 fields in both images, for673 bytes/all26 fields.

The source simply moves whole-owner zero-fill and flags2 into an ordinary
out-of-class constructor. Its existing timer member constructor still performs
the one prezero flags+0x40 read/mask. Factory allocation/null-check/Initialize
retain all real operands and callee-pop model. This natural new-expression
construction restores target XOR EAX,EAX before null test, pointer/lifetime
schedule and one shared RET20. No extra timer write, fake result, forced inline,
profile change or static-address Oracle relaxation is needed. This supersedes
factory88/82 at46eddb2, not the initializer396 or cleanup127 nonexact states.

The pinned MIT reference supplied a constructor-ownership hypothesis, not TH10
proof; its redundant timer statement/noinline controls are not copied. A full
byte match does not uniquely identify original class/TU/constructor partition.
Fresh read-only local Ghidra attests all mapped text, complete factory82 and
nine entry references. The field at+4 binds ordinary allocator452493; +72 binds
initializer43C970. Allocation exception/failure handling, inherited prezero timer
object lifetime, global ownership and whole-product/runtime remain open.

Actual-source host843 assertions and the moved constructor-scoped init_flags
control pass. Header is included only by TransitionEffect.cpp; shared dependency
pins and their rejection policy do not change. Whole major-owner previews in
this retained Gui carrier remain nonexact, and are not a basis for changing
their canonical profiles or granting partial credit. See [the complete scoped
proof](evidence/effect-constructor/README.md).

## Fresh ANM operand-stack and FLIP_X controls (2026-10-20)

The current-source four-TU ExecuteScript /GL /GS baseline still emits9948/9964,
frameF8/FC, all92 physical groups in order and84 equal-sized spans. Fresh read-only
local Ghidra attests the full mapped target text before/after. ADD9's binary32
RHS is at targetESP40 versus nativeB4; SUB11 target88 versus native40. MOD27
uses target80/B4 versus native7C/C4. Baseline conditional getter/destination/math
ordering already agrees; the length swaps are displacement-width effects, not
proof of wrong arithmetic. FLIP_X target EAX/XOR-immediate costs one byte less
than baseline EDX, while FLIP_Y already uses EAX. Original source variables and
optimization visibility remain unknown.

Explicit ADD/SUB value/pointer lifetimes and float snapshots plus explicit double
fmod produce the identical raw whole-parent hash. Existing-bitfield FLIP_X emits
extra Boolean ALU operations and grows its own span61/52; the resulting parent
is coincidentally9964 but still frameF8, hence rejected. Moving multiplication
before raw flag update does not improve it. All modified bytes and11 input
hashes are restored to785e91c. No source, boundary, origin, denominator, parent,
fragment or exact credit is added. Do not repeat these controls as new hypotheses.

Only the baseline is fully cold; four diagnostics each recompile the changed
primary and reuse three byte-hash checked /GL support objects after verifying
all non-primary inputs unchanged. Strict same-carrier validation preserves all
seven existing owners1731 bytes/all107 fields, including the complete four
operand getters856/all84 fields. This establishes their bytes in this carrier,
not unique original compiler partition/visibility or whole-product behavior.
The reference explicitly calls its ANM parent equivalent with assembly fallback;
its forced helpers/redundant stores are not natural exact proof. See [the target
facts, rejected controls and complete guards](evidence/anm-execute-stack-review/README.md).

## 2026-10-20 transition initializer and callback address binding

The complete396-byte0x43C970 initializer closes after only the real registrar
formal order changes to owner/priority/callback. All16 calls change together;
callee-pop annotations, all three operands and registrar mutation bodies remain
unchanged. Target EAX callback/EDI priority/stack owner and both43-byte registrars
are preserved. Two independent cold14-TU Gui updater links freeze identical42
inputs and pass1069 complete bytes/all63 fields:396 new plus673 existing guards.
Original source order, class/TU and production compiler partition remain unknown.

The37 parent fields are15 static function addresses,11 registrar calls, one
speed pointer and10 local table fields. Independent target decoding and the
complete nine-slot table bind them; local destinations retain exact relative
offsets. Explicit static-address metadata checks source/name, both whole extents,
dense code and unique qualified MAP/PDB entry, with no addend/fallback. This is
address binding only, even for124/133 or297/349 callbacks. No callee bytes are
promoted; the old direct static CALL zero-field/raw-equal gate is unchanged.
Fifteen synthetic controls retain both policies and adversarial failures.

Focused Ghidra and target gap review discovers five complete missing callback
owners:43C230/133,43C310/175,43C460/154,43C550/439 and43C710/349. Entry-only
initializer references, dense RET bodies, following CC padding and specific
game effect/timer/pause/RNG/camera operations establish1250 authored bytes at
medium origin confidence, independently of native scores. Their source already
exists; all five remain nonexact. Cleanup127 remains four ordinary move bytes
away. The global boundary audit still has the known401100 evidence-text-only
difference; no unrelated ledger rewrite is adopted. Host843, three affected
strict controls and19 unchanged EOL controls pass. See [the complete target,
field and native proof](evidence/effect-initializer/README.md).

## 2026-10-20 complete Pulse and Dim callbacks

This supersedes the preceding nonexact states for43C460/154 and43C310/175.
Moving the real Pulse alpha reset before unsigned count decrement restores its
ECX count register and target write order. Positive Dim release branching puts
return7 after the common timer tail, leaving four ordinary slow-tick load bytes;
a normal reference to the actual embedded timer restores scale-first FPU
loading without changing Tick, types, ABI, flags or object layout. Two full
independent cold14-TU Gui updater links freeze42 identical inputs and pass1398
complete bytes/all77 fields, including1069 existing guard bytes. Canonical
transition-pulse154/6 fields and transition-dim175/8 fields grant329 authored
exact bytes; no boundary/origin/denominator change or callee-only credit.

Seven source diagnostics reuse13 unchanged hash-checked objects after compiling
only TransitionEffect.cpp; none is a fresh cold claim. The two Dim positive
branch models remain139/143 until the real timer reference. Linear timer-ref
and duration/start snapshots reproduce its complete raw439-byte hash; staged
amplitude regresses332/359 to328/359 and is restored. Its FIDIV is still earlier
than the target within the second RNG recurrence; partial score/size agreement
is not exactness. All Linear source bytes are restored. Host843, five strict
mutants and unchanged host dependency pins pass. Remaining FadeIn133, Linear439,
Envelope349 and Cleanup127, production/runtime, semantic and port gates remain
open. Original class/TU/source spelling is unknown. See [the full target facts,
source overlays and independent native proofs](evidence/effect-pulse-dim/README.md).


## 2026-10-20 GUI child-ID search exact; parent controls restored

ResultMenuFindScript4497D0..449815 closes the existing70 authored bytes with
unchanged boundary, maintained source file and origin. Read-only local Ghidra attests the
original target and complete mapped text;52 incoming direct calls have no
outside interior references. Passing the actual ID value and finishing the
child search before constructing the aggregate restore ESI id, EBX script,
EDI hidden result, the success node-owner reload and plain RET. Root inclusion,
first-match return, zero result and the existing valid-VM precondition remain.
No scalar substitution, ABI annotation, volatile or forced padding is added.
Two independent full14-TU /GL real-Gui-updater links agree on all42 project
inputs and replay all70 bytes/all2 fields. Actual-callee FindVm73 is raw-equal
in both carriers, without duplicate credit or a canonical recipe change.

All Gui.cpp parent experiments are restored byte-for-byte: physical case order,
separate character arms, direct ID objects, consumed sprite expressions,
loop/wait/sound branch shapes and packed flags remain nonexact. Cumulative
controls range4464..4528 bytes; restoring frameD8 and17/24 physical spans still
does not close the whole owner. The final control has151 calls versus150,
including separate opcode20 mode4 copies; its prepared replay-branch rewrite
was not compiled. Current Run4480/4480 remains253/3160 ordinary comparable
positions; HUD3772/3836 remains527/3552. No parent/partial-case credit follows.
See [the complete evidence and restored-control table](evidence/gui-child-search/README.md).
Original class/spelling/TU and native product/runtime, semantic and port gates
remain open; exact bytes do not identify a unique original C++ formulation.

## 2026-10-20 Ending text workflow and complete small-owner proofs

Ending.cpp/Ending.hpp now map all ten reviewed authored owners (3889 owned
bytes), plus three indeterminate wrappers. The 40BD80 interpreter's guarded
18-way jump at40BDF0 owns the complete72-byte table40C358..40C39F. Its physical
extent is1568:1488 reachable bytes,6 internal alignment,2 tail alignment
(MOV EDI,EDI),72 table bytes. All destinations are local; the next callback
starts40C3A0 and the sole outside code reference enters40BD80. This corrects
the old1494-byte ledger and adds74 authored denominator bytes without exact
credit. The target has no PE base relocation table; ownership follows raw
control flow/xrefs/neighboring entries, not hypothetical target relocations.

The new10-byte40BA70 update callback and18-byte40C3A0 background loader callback
have genuine address references in initialization/Run and complete dense bodies.
Authorship remains indeterminate, as for the old18-byte40B9D0 nullable delete.
40C3C0 is a bank loader, not Draw. Records have a four-byte header and byte
payload length, with integer operands only when consumed. Live cursor reads
after rendering/VM creation/PreloadAnm, OpenFile-before-current-buffer-free,
reload's immediate first-command continuation, and pressed-edge input retain
TH10 order. MIT reference hypotheses do not substitute for these target facts.

Ending's EC allocation and D0 worker member establish the shared1C consumed
worker prefix. Moving the browser's former unknown trailing word into its
container at2C preserves browser offsets and size. Original full worker size,
animation-ID capacity, production TU/data owners and modern C++ lifetime remain
unknown. Target construction and reload both clear the text object including
the worker vptr; this behavior is retained, not presented as runtime closure.

Two independent full17-TU /GL /GS /EHs colds with identical project inputs
strictly replay EndingCreate99, Update111, TextTick93 (303 new authored bytes),
nullableDelete18 and the two callbacks10/18 (46 indeterminate bytes). Their
19 fields bind independently to target callees/globals/constants. Complete
worker Stop101/Start56/destructor16, browser constructor234 and actual outgoing
ReleaseAnm55 add462 existing guard bytes/all11 fields per image, no duplicate
credit. All811 complete bytes/all30 fields pass in each carrier. Init, both
destructors/constructor, LoadScript, Run and bank loader remain nonexact.

Actual-source host tests pass ASan/UBSan/leak detection and four strict cursor,
reload and input mutants. SDK/assets/rendering are role stubs; host clearing
preserves a live vptr, timer/node prefill is defined, thread startup is simulated
and native layout assertions are omitted in host copies. These explicit seams
cannot prove native lifetime, scheduling, ABI or target runtime. The known
global401100 evidence-text-only boundary audit discrepancy remains unchanged.
See [the target, source, exact and host scope](evidence/ending-script/README.md).

## 2026-10-20 Ending reload exact and native creation corrections

LoadScript40B480..40B4E9 closes106 authored bytes with its maintained source
unchanged. The former EndingCreate-rooted /GL diagnostic passes file mode0 in
EBX, producing108 bytes. Target OpenFile instead consumes filenameEAX and
sizeOut/mode on the stack (RET8). The real maintained Main checksum entry uses
mode1; with that entry and the actual PbgArchive dependency, the declared18-TU
/GL /GS /EHs graph restores the complete106-byte loader and target call ABI.
This is a combined context observation, not proof of minimal graph or original
production partition; no /GL removal, signature change or flag sweep follows.
Two independent full colds freeze52 inputs and replay943 complete bytes/all57
fields each: new106/all5, actual OpenFile360/all26, primary creator73/all5,
ReleaseAnm55/all2, and six prior Ending owners349/all19. No duplicate guard
credit or old canonical recipe migration is made.

Raw TH10 creation calls in Run opcodes8/15/16/17 target448D00, the exact
primary-list CreateVmVariant0. The earlier source used secondary-list Variant1
448F60, while the old host shim routed that wrong call to the primary list.
That recovery error is corrected. Target destination operands are read after
creation. The old VC7.1 output reads them before the call, but the host C++17
assignment order masked the difference. Explicit ordinary ID temporaries now
sequence creation before the live destination read. Opcode8 borrows its actual
old ID; opcode7 publishes the path before its busy-flag store. No fabricated
return slot, new forced helper, volatile or ABI annotation is introduced.

The host suite now uses both actual production creator bodies and distinct
primary/secondary insertion roles, checks script/layer/creation flags, and
rejects the wrong-chain mutant alongside four existing strict mutations.
ASan/UBSan/leak detection and a failed-load-current-buffer release scenario
pass. SDK/assets/allocation/list insertion, host vptr preservation, prefill and
simulated worker startup remain explicit seams, not target runtime proof.
The native Run auditor independently binds actual creator calls and follows
the post-call cursor/operand/store register dependency: all four old cases
fail both contracts; both final carriers pass. Its target-specific contract
scope gives no parent exactness credit.

Run1576/1568 and initializer584/584 remain nonexact. Auxiliary-ID borrowing,
named reference/scalar and ordinary publication-helper controls do not close
the full owners; the helper/scalar forms are discarded. Only the explicit
aggregate sequencing and TH10-backed behavior corrections are retained.
Product/runtime, dual-Oracle semantics and port gates remain open. See
[the complete native, source and host evidence](evidence/ending-load-script/README.md).

## 2026-10-20 Ending initialization sampling corrected

Target40B560/584 captures ASCII+89A4's destination address before screen creation
448D50 and stores the returned ID through that address afterwards. The previous
source/native code reloads the global ASCII owner after creation. A normal
reference now retains the original destination. Target text allocation452493
precedes reads of the current scriptData member+14 and header offset+4 before
constructor40BA90. The old source reads that offset before allocation. The
ordinary EndingScriptStart helper now appears in the new-expression argument
and the existing opcode12 reload; it expresses the same observed file format
without invented bounds, types or parser rules.

The optional initializer mode of the native Ending auditor resolves the unique
PE/MAP/PDB owners and checks captured destination/register use and post-allocation
header sampling. The b35f147 carrier passes the old four Run checks but fails
both initialization properties. Selected and final current carriers pass all
six. One fresh full18-TU /GL /GS /EHs cold agrees on52 maintained source/header
inputs and strictly retains943 complete existing guard bytes/all57 fields. Its
byte-identical artifacts update the existing loader carrier without a recipe
change. No new exact, origin, boundary or denominator credit is granted.

The host selection witness swaps ASCII owners; its original destination and
replacement sentinel distinguish the source versions. The allocation witness
replaces the current buffer at a modeled event. Direct replacement-new and
tagged-allocation attempts in host G++8.4.0 still pass an old argument after
the hook changes the member; GDB observes both values. This is a bounded host
observation, not a language rule or compiler-defect claim. The accepted host
copy explicitly sequences allocation-event/placement-construction with matching
throw cleanup. The actual unmodified VC7.1 new-expression is checked by the
native auditor, independently of this seam. Positive ASan/UBSan/leak checks and
seven exact named mutants pass; older vptr/prefill/layout/SDK/list/worker seams
remain. No target runtime or general host-new-expression proof follows.

Ordered next/previous/callback clears and observed>=1/>=6 ranges preserve the
defined behavior. Common configuration storage regresses and is discarded;
GameScoreState.hpp is unchanged. Init584/584, Run1576/1568, text constructor
772/648 and bank loader131/126 remain nonexact. Node flag scheduling and other
register/dataflow choices require fresh evidence; no equivalent-expression
sweep, fake slot, ABI annotation or forced compiler control is adopted.
See [the target, native and host scope](evidence/ending-initialize/README.md).

## 2026-10-20 Front-end replay-save owners and contextual replay deletion

Screen16 is the replay-save page, not the in-game result controller. Raw update
and draw dispatcher table entries select433570 and433B30 with one pushed
controller; both handlers end inRET4. The update's physical owner is1460 through
433B23: dense code1440 plus a guarded unsigned0..4 five-entry absolute table at
433B10. All destinations are local starts;12 CC bytes precede433B30. Draw955
includes7 internal alignment bytes and ends before five CC bytes/433EF0.
Read-only local Ghidra verifies target and complete mapped text before/after,
rolls back temporary metadata, and observes only the respective dispatcher
entry calls into these reviewed ranges. The public focused audit reproduces
guard/table/branch/call/neighbor and ledger checks. The authored denominator
increases20; the source mappings recover2415 bytes and grant no parent exactness.

The maintained five-phase source preserves initialization fallthrough, opening
current>6 versus closing>=6, slot-cancel priority and name-confirm-before-cancel.
Empty backspace exits before common sound and cancellation. Time retains the
original replay object but reloads its header after the call. Saving ignores
its return value, destroys the old numbered slot before writing, and captures
the destination slot after SaveReplay but before Load. Closing marks the
observed VM IDs, retires controller+5D0, returns/pops, deletes the captured global
replay and starts music before numbered-slot destruction. Exactly25 objects
are destroyed, but all50 pointers are cleared. Drawing keeps a replay header
across localtime, reloads ASCII globals around formatter calls and reads the
timer again after the moving-preview summary. The first preview inherits color;
completion8 and conditional final color/shadow stores are target observations.

With the actual recovered front-end callback and real ReplayManager body,
unchanged Destroy4297B0 now exactly reproduces18 bytes and its two complete
independently bound calls to destructor4294A0 and scalar delete4524A1. Earlier
27-byte standalone diagnostics lacked this caller context; delete syntax and
private ABI annotations are not changed. Two independent full18-TU /GL /GS /EHs
cold links freeze53 source/header inputs and each strictly compare422 complete
bytes/all4 fields, including five existing cursor/screen guards404/all2.
Only new18-byte authored credit is added. The related update callback passes
7/all1 in this graph. Draw callback5/7 in the alternate graph triggers a focused
cold of its original one-TU canonical recipe; that independently passes7/all1,
with no recipe migration or extra credit. Update1364/1460 and draw961/955 remain
nonexact; six native repeated-input helper calls versus target-expanded tests
identify a further context hypothesis, not grounds to force inlining. Original
partition, native product/runtime, dual-Oracle semantic and port gates stay open.

Actual-owner/cursor/input/delete-wrapper host tests consume canonical headers
with explicit layout/prefill/int32-time seams and callback-sensitive dependency
roles. Positive ASan/UBSan/leak checks and eight strict named mutants pass.
These synthetic witnesses do not establish native ABI, x87 or real target
callbacks. The MIT pinned reference is hypothesis material; its forced attributes,
linker placement and symbol-number tuning are not adopted. The unrelated401100
boundary metadata discrepancy and pre-existing D3D evidence are unchanged.
See [the bounded target, source, native and host evidence](evidence/frontend-replay-save/README.md).

## 2026-10-20 Replay factory exact, generated EH binding and FPS source

Load4296F0's seven raw callers pass a path on the stack and both exits useRET4.
The former maintained cdecl factory routes mode2 through shared Initialize and
emits179 bytes with plain RET and NEG/SBB normalization. The target instead
writes mode2 and directly checks reader42A200. The maintained stdcall factory
and direct reader expression restore181 complete bytes. These are bounded
protocol/source observations, not original declaration or inlining claims.
Two independent full18-TU /GL /GS /EHs colds freeze53 inputs and each strictly
replay759 bytes/all17 fields: new Load181/all12 and eight old dependencies578/
all5. Only181 authored bytes are added. Existing Destroy's current carrier is
refreshed without changing its recipe. Callee/production ownership and runtime
remain independent.

The factory's EH operand names a MAP-static parent-specific generated handler.
Its10-byte dispatch shares one21-byte PDB contribution with the11-byte unwind
action. The new explicit allocation-EH address lane verifies the parent symbol,
unaliased MAP entry, same PDB object/module and full contribution, immutable
one-state FuncInfo and its state-1 saved-storage deletion action atEBP-10.
The handler enters independently bound CxxFrameHandler46276D, and the action
uses scalar delete4524A1. Target handler46564B points to FuncInfo47169C/map471694/
action465640. This is address binding only; no generated-child, metadata-owner
or EH-runtime credit is added. Other shapes fail closed. Thirty-five synthetic
rejection controls and both actual cold integration proofs pass; old public,
static and local-field rules remain intact.

The target QueueSoundSample43DC90 has ECX owner/EDI sound/stack sample, whereas
the previous save-page sound shim remains an unresolved cdecl single-argument
symbol in native output. Seven calls now use the actual maintained queue method
with sample0. Direct repeated-input method calls also remove six extra wrapper
calls. Save first changes1364 to1452, then corrected factory ABI yields1444/1460.
Direct cursor-setter syntax has identical code and is restored. Draw961/955 and
both whole update/draw owners stay nonexact; no variable-count, forced inline,
register-ABI or padding tuning is adopted.

FPS display429A70's182-byte owner is now source-present. Target game pointer
477810 gates playback mode1; unsigned byte+1C4 is displayed at383/450 with%3d,
red below30/yellow below50, followed by a fresh global ASCII white reset.
Its sole observed incoming code edge is the28-byte adapter42A430. Current native
172/182 remains nonexact; the adapter's source and fastcall chain-type contract
remain open. No dummy first register argument is added. The currently Replay-
tagged10 authored rows/5586 bytes are all mapped; this does not prove complete
untagged subsystem inventory or close indeterminate adapters.

Host regression now executes the actual factory/FPS/queue bodies with the old
save/cursor/input/delete source. A queue-entry observation, typed objects,
metadata and sample buffers expose the real owner/sample0 contract. The lower
reader remains an explicit I/O role; mode2, arbitrary nonzero failures, cleanup
and preservation of the active global are checked. FPS gate/threshold/format/
position/color and formatter-driven global replacement witnesses pass. Positive
ASan/UBSan/leak checks and14 strict named mutants pass; SDK/layout/prefill/int32-
time/renderer/allocation/I/O roles remain host limits. Native SDK declarations
use the existing Main.hpp shield. Product/runtime, dual-Oracle semantic and
port gates remain open. Pre-existing D3D evidence and the401100 boundary metadata
discrepancy are untouched. See [the full scope and reproducible checks](evidence/replay-load/README.md).

## 2026-10-20 Replay callback protocol and FPS exact closure

All three replay callback entries use ECX owner, consistent with generic Chain
fastcall dispatch. The replay-local cdecl callback pointer/declarations are
corrected without changing the24-byte element layout. Four maintained source
mappings recover the58-byte retained control and10/68/28 adapters; their origin
remains indeterminate. The68-byte adapter uses runtime+58 bit4 and current
input474E30 bit100, not the older vague behavior flag. Signed frameCounter%4!=0
selects return6; other paths return1. An ordinary shared member expresses the
58-byte retained logic. The wrapper remains62/68; result-local63 is rejected.
The10-byte process adapter remains nonexact because native ESI differs from
its target EDI receiver; no register annotation is added.

The real28-byte draw adapter restores FPS's target private EDX receiver. Separate
record exclusion/playback selection retain the two target mode checks and close
the full182-byte body. Existing ReplayRuntimeState supplies slot477810/+58;
removing the opaque duplicate game pointer does not establish complete data
ownership or original types. Both bodies preserve FPS/format/threshold/position/
color and fresh global ASCII reset. No dummy first register parameter is used.

Two independent full18-TU /GL /GS /EHs colds freeze53 inputs and each strictly
pass1112 complete bytes/all32 fields: new authored182/all7, indeterminate58/all2
and28/all2, plus844 existing bytes/all21 fields including actual formatter85
and the full allocation-EH-bound load181. Only182 authored/86 indeterminate
credit is added, without origin, boundary or denominator change. Existing
recipes remain and their shared replay carrier is refreshed. Save1444/1460 and
draw961/955 stay nonexact; original partition and native product/runtime stay
open.

Actual callback/control/FPS host bodies, canonical runtime/type declarations
and function-pointer calls cover NULL/runtime flags/modes/current versus pressed
input, signed counter boundaries, draw suppression and process-return forwarding.
ProcessFrame itself is an explicit host role, not target behavior proof. Positive
ASan/UBSan/leak checks and17 exact named mutants pass, including three callback
controls. Existing SDK/layout/prefill/time/I/O/renderer/queue seams remain.
The affected GameManager teardown baseline consumes the canonical fastcall
pointer typedef and passes. Pre-existing D3D evidence is untouched. See
[the complete target, native and host scope](evidence/replay-callbacks/README.md).


## Title entry and interactive menu (2026-10-20)

Target/local read-only Ghidra review identifies 42D300 as the title-entry path
and 42D420 as the interactive eight-item menu. Their complete physical owners
are 288 and1280 bytes: the former has268 code+20 table bytes; the latter has1195
code+one NOP+84 table bytes. Four unsigned-guarded local tables, their complete
destinations, direct dispatcher callers and adjacent independent owners establish
ownership. This adds105 authored denominator bytes without inventing code or
using nonexistent target PE relocations. Maintained natural source plus the69-
byte six-progress-marker query42C850 maps1637 authored bytes. Source names do
not identify original symbols/TUs or the full score-file owner.

The title resource at controller+18 and auxiliary VM ID at+684 are now typed
and asserted. Title entry only recreates slots88/90 and the auxiliary when88 is
missing; pressed160B starts its30-frame close. The interactive menu uses six
profile bytes1D888..1D88D to gate Extra, preserves cancellation-on-quit priority,
queues music-room sound10 both before and after phase4 reset, and routes eight
items to6/6/6/12/11/14/4/3 after20 frames. Extra saves difficulty before setting4;
ordinary/practice selection caps values>=4 to1. Source Update now preserves the
observed transition-owner flag2 write before the screen2 call.

Cached diagnostic retained entry284/288 and menu1308/1280; they remain nonexact.
The progress-query body is69/69 raw-equal, fully decoded with zero link fields.
Two independent complete18-TU /GL /GS /EHs colds freeze53 source/header inputs
and each pass1181 complete bytes/all32 fields, including1112 prior guard bytes.
Only the query adds69 authored exact bytes. The shared ReplayManagerDestroy
carrier is refreshed for its same build group. The strict cold proof and
host scope are recorded in [the focused evidence](evidence/frontend-main-menu/README.md).
Actual owners/cursor/input/queue bodies and canonical headers are host-tested
with explicit VM/state/layout/prefill roles; nine named mutants include input
priority, frame boundaries, Extra persistence and auxiliary identity. No native
product, runtime, complete source ownership or semantic/port claim follows.


## MIDI event/timer owner recovery (2026-10-20)

TH10-local read-only Ghidra, complete raw code/guards and adjacent padding identify
parser43B110 as1670 owned bytes:1323 executable plus347 of pointer tables,
compressed113/65/94-byte indices and alignment through43B795. This corrects the
authored denominator; no parser exact credit. Together with virtual Tick473,
reset132, volume140 and VLQ reader26,2441 authored bytes now have maintained
source. New default timer callback43B8B0/1 is named by base vtable46F810 and has
fifteen following CC bytes. Its source-written/generated/shared origin stays
indeterminate. The trampoline43A6E0 dispatches vptr slot0; derived slot46F80C names
Tick43AF30. A16-byte timer base preserves all native offsets while a flat virtual
class fails VC7.1 offset assertions. Descriptive types do not close original
class identity, vtable data, constructors or the complete allocation owner.

The parser's tempo recurrence is target IMUL101h, not a conventional base100h;
payload07 A1 20 yields503752. Both signed clock parameters are sign-extended to
unsigned64 before modular multiplication/division. End-of-track leaves running
status/time unchanged; loop2/4 save/restore cursors/event times and all clock
state before the final delta read. SysEx retains pending allocations without a
device and frees both objects on prepare/long failure, with live handle/ring
sampling. Tick checks sign-extended event time as unsigned, observes the current
track table after processing, increments elapsed before empty-track rewind and
returns immediately on terminal fade. These are bounded target/source facts;
finite SDK/allocation-role host checks are not TH10/native runtime validation.

Two independent complete single-TU /GL colds freeze Midi.cpp/Midi.hpp and each
strictly pass378 complete bytes/all5 fields: reset132, reader26 and volume140 add298 authored
exact bytes, default callback1 adds one origin-indeterminate byte, and existing
UnprepareHeader79 remains exact. The old canonical StopPlayback-entry recipe
also cold-replays79/all3. Actual complete-source ASan/UBSan/leak checks and13
strict named mutants pass. Only three transmitted message bytes are observed;
the target's untouched high byte and x87/runtime behavior remain outside host
proof. Tick481/473, parser1614/1670 and StopPlayback200/129 remain
nonexact. Ordinary member declaration is byte-neutral; early calculation of the used
status byte before FTOL closes all140 volume bytes and both fields. The Tick
fade polarity control484/473 is rejected/restored. No forced volatile,
packing, placement or invented register context is adopted. See
[the complete evidence and retained license](evidence/midi-events/README.md).


## Spell-card panel frame owner (2026-10-20)

Update408D60 now maps its complete1057 authored bytes; registered adapter409220
adds ten source-present indeterminate-origin bytes. Ghidra's1045-byte body omits
two internal six-byte alignment instructions; raw extent ends at409180 and
needs no denominator change. Registration4089C2 and the CC-delimited callback
establish ECX-to-EDI forwarding, not original source authorship. Shared
EnemyGameStateView now has one canonical header instead of incompatible
same-name definitions. Bonus current3790/initial3794/duration3798 names follow
the target. BeginSpell still caps only3794, preserving its observed quirk.

Digit calls43E5A0 are SetSprite, not43E7E0 script initialization/execution. The
actual sprite helper has no callbacks. Bonus execution retains the value but
resamples resources; counters capture one GUI, reload its resource four times
and resample their second statistics record before all five VMs execute.
Pre-frame60/300, post-frame120, strict playerY96/128 and flag8 behavior are
target-audited. Marker slot0 uses enemy+1068 and the actual world-position helper.

Final complete three-TU /GL /GS cold freezes16 inputs and emits1060/1057 plus
callback7/10, both nonexact. The call-then-return-one callback control is13/10
and rejected/restored. Existing helpers65/356/111 are structurally unchanged
modulo2/2/5 fields in this graph; no new canonical replay/credit follows.
Actual-source/helper ASan/UBSan/leak checks and13 strict named mutants pass.
ECL/Enemy/Player compile-only native interface checks and the actual auxiliary
player fixture pass. All86 public CI checks pass. Older FrontEnd cache inputs
are now stale for this shared
header/ECL TU; retain its hashes and reject unreviewed whole-cache reuse.
MIDI cache remains independent. x87, construction, data ownership and native
product/runtime stay open. See [the focused evidence](evidence/spell-panel/README.md).


## Result rows and name-entry drawing (2026-10-20)

Name/grid4224C0 and rows422660 now map1466 authored owned bytes; draw adapter
422AA0 adds7 source-present indeterminate-origin bytes. Rows owns1062:1035
code/alignment, NOP422A6B, four pointers422A6C and ten selectors422A7C..422A85.
Unsigned guard9 after phase-minus10, both reads, local destinations and CC
neighbors establish27 missing authored denominator bytes. Ghidra's1026-byte
body separately omits nine internal alignment bytes already in the old code
extent. Name entry uses a by-value float3 plus owner and two RET16 exits.

ASCII8988 is the draw-shadow field, not the reference's game busy flag. Phases
10/17 draw25 live-selected slots;11/18 interpolate the name at frame0..9 and
use current-stage completion;12/19 draw ten score rows with unavailable-name
highlight suppression. Replay headers survive localtime; score records and
selection/profile resample after it. Suffix bytes remain signed. Name input
uses13 columns,18/16 pitch, control glyphs81/7F/80 and the length8 underline
correction. Actual formatter/queue bodies, not callbacks inside mock formatters,
supply host output. localtime remapping is an explicit sampling probe with no
claim that those global changes occur in CRT/game runtime.

Two independent complete two-TU /GL /GS /EHs colds freeze18 inputs and each
strictly pass628 complete bytes/all21 fields. Name404 adds authored exact bytes;
adapter7 adds indeterminate exact bytes; old ASCII formatter85 and queue132
retain217 guard bytes/all4 fields. Source presence and parent exactness remain
independent. Rows1034/1062 stays nonexact; native code1005 plus29 tail bytes
requires data-aware review. A typed profile-array control is neutral; earlier
source is restored. Do not repeat shape/ABI/placement sweeps without new evidence.
Actual-source ASan/UBSan/leak checks and14 strict named mutants pass. The initial
AddString140 diagnostic size was wrong; both canonical guard replays use its
complete132 bytes. Production class/TU/storage/runtime remain open. See
[the complete proof and reference provenance](evidence/result-rows/README.md).

The final local Ghidra review corrects an old private full-text hash off-by-one:
401000..465A80 is412289 bytes/64A81h with SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
The old64A80h digest is only a prefix. Shared distributed end probes already
include the last byte. See the inclusive-range note in ORACLES.md. All87 public
CI checks and final focused/toolchain/tracking/progress/whitespace checks pass.

## Laser line owners and missing command parser (2026-10-20)

Target slot46DA60+0 identifies omitted41CA80:1269-byte code span contains10
alignment bytes omitted by Ghidra1259, then3 alignment/20 pointers/64 selectors
through41CFCB. DEC/CMP3F/JA guards the selector, all five destinations are local,
and CC separates both neighbors. Raw game opcode/flag/timer/Player/resource/
sound behavior independently supports authored classification. Admit1356
denominator bytes with no source mapping or exact credit; the virtual source
declaration is not a body. Local Ghidra is read-only, complete mapped hash
attested, transaction rolled back, with no external interior reference seen.

Update41D3D0/1001, Draw41D7C0/173 and Player laser contact4267F0/471 now map1645
authored source bytes. Existing primary resource4776F0+3E0B50 has one shared
header; the canonical sound-queue definition replaces an inconsistent local
same-name type. Native assertions retain lineD58 and VMs600/9AC. Scale divides
sprite width34/height30; Draw writes VM.position334, not positionOffset340.
Motion calls re-read live flags in target slot order. Hits re-read Player after
the actual hit boundary; grazes use signed age%5, primary script434, then fresh
line X for actual cue28. Player contact retains negated unordered rejection,
lower-Y graze bounds, GUI/state/highlight gates and stack/RET4 hit boundary426CF0.
The hit body remains source-absent; the old option-position label was wrong.

Motion0010/269 now has source and two independent single-TU /GL /GS canonical
colds with12 frozen inputs, all269 bytes and10 explicit fields. Its origin
remains indeterminate: no authored exact increment. Four-TU Update1001/1001
is still847/877 comparable, with differences beginning at380h in scale-tail
scheduling; Draw168/173 and contact453/471 remain nonexact. Real Draw-entry
control172/173 stays nonexact. Existing bounds91/timer88/sound149 structural
diagnostics grant no new credit. The Player header client compiles separately.

Actual-owner/helper ASan/UBSan/leak checks pass with21 strict named mutants.
Native polar writes onlyXY; scratch Z and x87 exception effects are unknown.
Host adapts the two local Z prefills to17 and explicitly starts the acceleration
view lifetime in opaque storage. Unknown virtuals, math, VM/render/hit calls
and outer object lifetimes are declared roles; no general numeric, native
reachability, constructor/vtable installation, product or port claim follows.
The existing PlayerHandleCollisionHit stack/RET4 declaration is reused from
PlayerCollision.cpp. Final four-TU diagnostics preserve the same nonexact
sizes/scores, and all88 public CI checks pass.
Header dependency hashes invalidate affected older FrontEnd/SpellPanel objects;
independent MIDI cache remains separate. Next recover parser1356 or another
source gap; require new lifetime/caller evidence before more parent shape work.
See [the complete proof and reference scope](evidence/laser-line/README.md).

## Laser command parser recovery (2026-10-20)

RunCommands41CA80 now has source for its existing1356 authored bytes; no new
denominator or exact credit. Type-0 request+2C (line450) holds eighteen24-byte
commands, four operand words/opcode10/gate14. Anonymous word unions preserve
existing raw copies and1DC/34 native sizes, without asserting original C++
types or union lifetimes. The target uses signed PC<18 without a negative
guard. Zero stops, active flags plus zero gate stop, SKIP80000000 continues
within the frame, unknown commands advance, ID preserves operand bits and
jump2000000 replaces PC without an extra increment. Positive400/800/8000000
counts decrement the actual command operand; copying the command is wrong.

Both100000 and200000 set timer6 at190. The angle sentinels are<=-990 and>=990,
not the reference comment's1. NaN is literal for angle, fallback for strict
width>-999 and ordered growth>=0. The fallback uses growSpeed48, not width44.
Native operand and counter words retain integer/float reinterpretation. Existing
Bullet caller40708C independently reads script base474170, so the same global
declaration is reused. Opcode4000 calls actual InitializeForLoadedScript404F30
with primary VM600 and resource4776F0+3E0B50, then reloads current PC and next
gate. Only real script binding/execution supplies host callback roles; VM reset,
aim, timers, polar and sound helpers remain pure. The later0040 consumer shows
that the previously named mode word is a float angular delta for that slot.

Full5-TU /GL /GS diagnostics freeze25 project inputs. Cached-angle source1364
is superseded by target-observed field reloads/conditional stores at1368/1356,
still nonexact. Existing57/67/30/123/149 helper structural diagnostics add no
credit. The embedded VM receiver is private EDX versus targetESI; shared-tail
scheduling and production TU context stay unresolved. A Draw primary-VM
pointer-lifetime control173/173 yields86/133 comparable and is restored; size
equality is not exactness. Existing Motion269/all10 fields passes a focused
canonical cold replay after the shared-header change, with origin unchanged.

Actual-owner/timer/aim/sound/VM-wrapper ASan/UBSan/leak checks and23 strict named
mutants pass. Valid live storage/operand indices and finite programs are the
host domain. Negative PCs, infinite loops, overflow, invalid pointers, original
union/object lifetime, native x87 bits/exceptions and product/runtime closure
remain unknown. The original D3D work is preserved. Next inspect41C8C0 or
another source gap; require new context before further parser/Draw shape work.
See [the target, compiler and host evidence](evidence/laser-script/README.md).
All89 public CI checks pass; focused target/toolchain/tracking/progress and
whitespace checks pass. Superseded controls/host binaries free about4.2 MB.

## Line initialization and timed turning (2026-10-20)

New reviewed owners Initialize41C8C0/442 and Motion004041D170/329 add771
authored denominator/source bytes. Raw full decodes, CC neighbors and virtual
slots46DA64/46DA90 establish their boundaries. Game request/VM/state setup and
motion's game sound owner/callee plus scheduler flags independently establish
authored origin. Existing constructor41C5B0 remains indeterminate. Neighbor
41C880/54 is also admitted, with no source and indeterminate origin: it sums
virtual-slot20 results over prefetched non-state-1 nodes, but no Ghidra refs,
direct-CALL byte pattern or literal VA is found. No authorship/reachability or
exact claim is inferred from its complete boundary.

Initialize is ECX=this, one stack request/RET4/EAX0 and copies77 dwords to424.
It sets stateC=2, initializes primary VM600 using signed type/color and the
existing Bullet script table, sets pendingInterrupt=2, explicitly executes,
then samples live request flags and VM flags. Secondary9AC uses fresh laser
manager47781C/resource458 and current color+103; this is distinct from the
parser's Enemy-primary resource. Mask pairsFC63FFFF/00600000 andFC7FFFFF/
00400000 preserve other bits. Timer30 and geometry follow both script steps.
The owned request is read late; mutation of the incoming request is not a
replacement for it. Geometry widths/speed/length use1C/20/14, soundId=24,
positive length gives traveled=.01 and other/unordered lengths give +0.
Polar writesXY only, preserving velocity Z. Reference names length_step44,
track_count40C and acceleration4C are not promoted over reviewed consumers.

Motion0040 decays velocity magnitude using fractional elapsed time while
retaining base speed. Expiry plays the optional sample, increments count,
clears flag40 at the repeat limit, adds an unwrapped float angular delta,
installs the next speed, resets timer3, and always performs polar/Tick even
after flag clear. The parser's formerly named mode word10C is this angular
delta, next speed108 and duration/limit/count are11C/120/124.

Three complete /GL /GS TUs with a normal by-value deceleration helper produce
329/329. Two independent cold canonical builds freeze17 inputs and compare
every byte plus8 target-bound fields exactly, adding329 authored exact bytes.
Direct-field326 and prebranch-value331 controls are nonexact; the helper
retains a used value without volatile/forced attributes or byte tricks.
The unchanged Motion0010 guard retains269/all10 fields after slot4 is typed.
Init's initial complete4-TU451/442 and VM helper200/202 remain nonexact; later
motion-entry unused Init450 is not conflated with that graph.

Actual init/motion/parser/timer/aim/sound/VM-wrapper host checks pass ASan/
UBSan/leaks and27 strict new mutants. VM Initialize is pure field reset:
the older fixture's reset callback was overbroad, is removed, and23 parser
mutants pass again with state changes only at actual script binding/execution.
Host constructor/layout/union and polar roles remain explicit. Original C++
lifetimes, native x87/exception state, invalid storage/overflow and product/
runtime reachability are open. Next inspect cleanup609/type-1 init or the
unreferenced54/query226 independently. See [complete scope](evidence/laser-init/README.md).
All90 public CI checks pass, with final target/toolchain/tracking/progress/
whitespace checks. About5.1 MB of superseded controls and host binaries are pruned.

## Laser line clear and actual Item Spawn binding (2026-10-20)

- Clear41E260 owns609 bytes through41E4C0, slot14h of46DA60, one stack mode,
  RET4/EAX count. It samples at6 then stride12 with strict ordered initial
  12<length and live distance+6<length continuation. Explicit zero stepZ is
  added after each segment; initial signed-zero Z is not preserved forever.
  Finally stateC=1, distinct from deletion byte50. Source present, not exact.
- Signed color44A selects2*color+17 from current laser manager47781C resource458.
  The actual world creator writes positionOffset340 plus224/16, executes
  binder43E710 and registers with the post-script current renderer491C10.
  Resource/color/length are live between script executions; geometric step and
  original position are captured once. Entry/equality/NaN rules are distinct
  from the four negated margin32 playfield rejects, which accept unordered.
- Enabled item drops use actual ItemManager477818/Spawn41BB00 with pointECX,
  receiverEAX, stack8/FFFFFFFF/-pi/2/0.6 and RET16. Four existing collision
  owners41D880/41DD80/41EB00/41EFA0 share that helper/global. The invented
  cancel-effect manager alias is removed; canonical ItemManager.hpp is shared.
- Natural three-float construction/assignment reflects the observed temporary
  copy, but does not solve the private binder registers or ANM scheduling.
  No forced volatile/placement/ABI fix is accepted. Native exactness and product
  closure stay open; see [complete scope](evidence/laser-clear/README.md).

## Type1 laser lifecycle and state table (2026-10-20)

- Missing slots41E5C0/309 and41EA40/173 of table46DA10 now have independent
  complete boundary/source reviews. Update41E700 has816 code bytes plus16
  owned table bytes41EA30: unsigned(state−2)<=3 selects four internal entries
  for states2/3/4/5. No relocations are present in the original image; the
  guarded domain and destinations establish table ownership. Adds498 authored
  owned bytes at medium-confidence origin, independent of codegen results.
- Request424/1F8 and VMs61C/9C8 keep D74 layout. Request18/1C are angle/angular
  velocity,20/24 maximum/initial length,28 width,2C growth speed; signed
  durations30/34/38/3C and signed type40/color42 are target-observed. Init's
  runtime width44 starts2; runtime48 is growth speed. Reference width names
  and sprite-axis names are wrong for our independently established fields.
- Init copies126 DWORDs, sets state3, uses actual VM wrapper404F30 and explicit
  Execute43EE30, live resource47781C+458/flags and late owned geometry/sound24.
  It does not reset age or assign velocity/traveled. Storage constructor219
  remains separately indeterminate; host object construction is an adaptation.
- Update attaches to Enemy477704 slot0/full1068 before applying request velocity,
  uses signed/fractional wait/grow/hold/shrink phases, with grow→hold→shrink
  fallthrough and immediate retirement. It resets but does not tick age.
  External advancement remains unreviewed. State2/4 and length>16 enable
  contact without the type0 width>3 gate; wide collision taper uses1/3.
- Scale axes remain sprite.width34/height30. Draw950/CFC are VM.position334,
  distinct from allocating positionOffset340. Post-script/post-draw traveled,
  Player, resource, sound position and renderer reads remain live. Polar scratch
  Z has no established native prefill; host17 is an explicit diagnostic role.
- Formal binder-argument-order control is neutral for the three parent native
  diagnostics; do not alter the already exact273-byte pure VM reset or invent
  register parameters to fit these callers. See [full scope](evidence/laser-type1/README.md).

- Used sprite snapshots before X scaling and freshly before Y scaling reproduce
  the shared final30-byte scheduling difference in type0 and type1. Type1's
  complete832-byte owner and type0's complete1001 now each pass two independent
  completefive-/four-TU canonical colds:24/22 frozen inputs,34/31 exhaustively
  bound fields, all1833 owned bytes. The new type1 table base/four pointers are
  local fields with preserved offsets. Motion0010269/all10 fields passes the
  focused existing guard. No forced attributes/ABI or product/runtime claim.

## Type1 laser Clear and offscreen counts (2026-10-20)

Clear41F400/623, slot14h46DA24, now has complete maintained source. Its result
is integer EAX count, not Ghidra's provisional float. Raw polar41F800 preserves
EDX; the caller's zero establishes stepZ and the empty result. Every6+12 sample
counts before bounds. Outer margin16 gates actual allocation/binding/outlined
registration; enabled Item Spawn additionally uses nested margin32. All eight
rejects are negated and accept NaN; ordered entry/tail exclude NaN/equality.
Cached y+16 is reused for VM.offset344 after allocation; X adds224 and Z is
copied, then zero Z is added per segment. Script execution precedes fresh
registration/Item/length reads and final stateC=1. Fullfour-TU627/623 remains
nonexact with receiver spill/private binder registers/scheduling differences.
No exact or denominator increment;29 actual-source ASan/UBSan/leak mutation
controls pass. Slot20 query41F670/226 stays separately untracked, origin unknown.
See [complete scope](evidence/laser-type1-clear/README.md).

## Input polling, word records and button helper returns (2026-10-20)

- Joystick44A190 and frame44A5F0 independently establish6A stride at474E30.
  Current/previous0/2, repeat4, pressed6/released8, sixteen gameplay countersA,
  separate debug block2C..57 and nine signed bindings58..69 form a natural
  all-word view. The older partial FrontEnd aggregate alias is not extended;
  complete array ownership, original common type and C++ lifetime remain open.
- WinMM joystick ID is boolean(padIndex), while unsigned capabilities use the
  original index. Raw button query44A4E0 always uses WinMM ID0. Mapped actions
  use signed entries0/1/3/2/8; four intermediate direction bindings are unused.
  Unsigned WinMM quarter zones and signed DirectInput thresholds are strict.
- Failed DirectInput Poll attempts Acquire once plus up to400 input-lost
  retries and never fetches state afterward. Merge zeroes its272-byte record
  and rejects negative GetDeviceState results. Raw query leaves its record
  uninitialized and ignores that result before copying128 bytes. Preserve the
  discrepancy; conditional host error roles supply written bytes explicitly.
- Enabled4924FC gates keyboard only; flags200/400 select independent keyboard/
  pad backends. BOOL GetKeyboardState is unchecked, while any nonzero DirectInput
  keyboard result acquires once and contributes no key bits. Joystick still
  merges before late state publication. Held gameplay counters wrap16 bits,
  repeat their own bit at26 and subtract8; released counters reset. This differs
  from the already exact debug routine's intentional bit-zero accumulation.
- Missing helpers44A130/37 and44A160/43 have no entry references; both update
  output and return zero-extended selected action or0. Their complete raw
  bodies justify real helper returns, but cannot prove source-written origin
  versus optimizer outline. They remain indeterminate, source-present/nonexact.
  WinMM natural C++ shift is defined only for valid0..31 bindings; x86 CL
  masking outside that domain is not a portability guarantee.
- Complete query264/all12 bound fields closes authored exact bytes in two
  independent frozen three-input canonical /GL /GS colds. Merge865/833 and
  frame854/850 remain nonexact. No original source ABI/TU/data owner or native
  product/runtime claim follows. Actual-owner/helper source passes32 strict
  ASan/UBSan/leak mutation controls. See [full scope](evidence/input-polling/README.md).


### 2026-10-20: Complete ECL Run disabled-trace and advance-base model

The [complete-owner checkpoint](evidence/ecl-run-disabled-trace/README.md)
retains all7020 bytes,60 relative switch destinations and88 selectors. A natural
empty variadic trace consumer, suggested by the pinned MIT reference, preserves
FORMAT's redundant percent arm and argument-evaluation lifetimes. A single
counter increments within each typed arm; VC7.1 derives the three observed
induction values without either old volatile qualifier. Original logging
identity/definition and production TU ownership remain hypotheses. A common
increment produces7028/7020; typed-arm increments recover7020 and6231/6264
comparable matches. Removing the sink removes the percent arm, so this is
new consumer/lifetime evidence rather than a repetition of the old isolated
switch/qualifier controls.

One explicit advance snapshot then restores the fresh EAX jump base / ECX delta
and ordinary advance sequence observed at44E40F/44FB77/44FB81. In the new trace
context this does not reproduce the old6948-byte regression. Two final16-input
cold real-EnemyRuntimeUpdate links reproduce7020 and6236/6264 comparable
positions; ordinary differences fall139 to28 (16 before FORMAT,10 within its
physical interval,2 after). The selector suffix still leaves generic
normalization incomplete. No full-owner or partial exact credit is added.

Counter declaration placement, squared-sum expression order and named trace
results are code-neutral in the shared-base context; a point-angle result local
is code-neutral against the old baseline. All controls are restored. No forced
registers, noinline, padding, fake ABI or copied target code is used. Eight
complete same-image guards strictly preserve1061 bytes/all20 fields twice.
FORMAT actual-source host checks pass10 scenarios/eight strict functional
mutants under ASan/UBSan/leak checking, including live instruction replacement.
A reviewed private control-projection adapter retains all11 assertion-only
mutants; its stale-base mutation assigns the cached current pointer explicitly,
not an uninitialized pointer. Neither suite establishes full opcode semantics,
original/native execution or malformed-input safety. Exact numerator and owned
byte denominator stay unchanged; large-owner, product/runtime and later gates
remain open.


### 2026-10-20: Complete ANM Float2 construction and UV storage

The [new complete-owner checkpoint](evidence/anm-float2-storage/README.md)
supersedes the old constructor C2620 blockade and frameF8 diagnostic. Vertex
UV has raw x/y words at14/18 in a1C-byte record; its anonymous union need not
contain the same maintained class as math Float2. A separate raw storage view
preserves aliases/size/offsets and permits ordinary empty/two-float math
constructors. Original type identity and production TU remain hypotheses.
UV separation and constructors alone are code-neutral. Explicit SCALE_TIME
value construction recovers target Y/X operand lifetimes and first79 interval
bytes, plus targetFC frame and9964 complete extent (9588 pre-table bytes).

Two complete independent11-input/four-TU /GL /GS colds reproduce89/92 target-
sized groups, absolute length gap14 and7184/7995 aligned comparable matches;
fresh unchanged HEAD2cf915c has9948/F8,84 groups/gap56 and6492/8000. Remaining
unequal intervals61(+1),60(-7),2(+6) include the common tail and internal
alignment in interval2. First-difference alignment and total size grant no
partial exact credit. Whole-owner comparison remains1846/8564. Nine complete
existing guards preserve2670 bytes/all151 fields in both images. Current Float2
Eval653/652 and frame28/20 supersede ANM-034's historical656-byte diagnostic.
The only target caller supplies stack output at4410C0; output-buffer semantics
do not prove the original source return spelling.

Named/cached duration variants are neutral; interleaving duration between Y/X
obtains gap2 but contradicts the actual call order and is rejected. Decoded
scalar construction regresses the parent; constructor-return operators shrink
the helper to575; by-value Eval grows it to685; in-place updates shrink it to615.
Whole-scale copies/raw flag OR do not recover target update scheduling. Captured
flip flags and live instruction duration regress other bytes/groups. All these
controls are restored. No new exact or denominator credit; product/runtime and
later gates stay open. The transition host's range helper/timer/viewport are
unchanged; only their reviewed whole-input pins are refreshed, with strict
normalization and rejection retained.

### 2026-10-20: ECL Run residual controls remain negative

Eighteen further controls against the frozen HEAD2cf915c two-TU source retain
or regress Run's28 ordinary differences. Broad/shared math locals, typed payload
views, percent-loop/index spelling, captured thread/spawn/sine values, mutable
bitwise operands, unsigned raw negation and moving/splitting stack definitions
provide no improvement. Removing forced inlining from the pre-existing Pop
wrappers outlines the body and regresses the parent; no new forced attributes
are introduced. Adding the actual ReadRawFloatArgument412AA0/26 adapter's
EnemyEclDispatcher TU with its target-backed /GS cookie adds one difference;
it does not repair Run's register selection or establish that adapter exact.
All controls and graph changes are restored, with zero exact credit. See
[scoped results](evidence/ecl-run-disabled-trace/residual-controls.json).
These are historical frozen-source observations: the subsequent ANM header
change invalidates that ECL cache for current-source replay. Do not reuse it
without checking every recursive input or rebuilding the necessary graph.


### 2026-10-20: Exact Float2/Float3 interpolation value lifetimes

[Two complete canonical vector evaluators](evidence/anm-vector-interpolation/README.md)
close1539 authored bytes: Float3 404610/887 and Float2 441AD0/652. Both use
ordinary initial-tuple snapshots for modes7/17 and one shared `t+t` before
minusOne/Hermite weights. Float2 constructor-return arithmetic and whole-vector
expressions reproduce target component storage; Float3 completed-duration
returns use final first, additive initial second. Natural source restores
frames20/44, all timer/state/math/return instructions and all20 address fields.
Two independent four-TU /GL /GS colds freeze11 project inputs and strictly pass
1539/1539 bytes. Nine existing same-image guards preserve2670 bytes/151 fields;
with the pair, each image passes4209 bytes/all171 fields. Original source type,
return spelling and physical TU remain unknown; no fake ABI or forced attrs.
ANM-034 and the prior Float2 frame28/size653 diagnostics are now historical.

Expression order, constructor returns, scalar grouping and output API each
change C1/C2 materialization; isolated negative controls did not rule out their
coupled value model. Named/grouped/multiplied2t controls stay nonexact until an
explicit reusable addition produces the full match. Float3 applies that pattern
and closes on its first family control. RGB/alpha factor/snapshot/value-interface
trials remain nonexact and are restored, without general integer-template
inference. All29 source observations retain frozen scope. See the linked proof.

Numerator66315/273274 (24.27%), unchanged denominator. ExecuteScript stays
9964/frameFC,89 equal-sized groups/gap14 and7184/7995 aligned comparable;
matching sizes grant no exact block credit. The current vector-family carrier
supersedes the preceding ANM source cache. The transition host's extracted
RNG/timer/viewport blocks are unchanged; reviewed whole-file pins only. Product/
runtime/semantic/port planes remain open.


### 2026-10-20: Scalar alpha conversion boundary and RGB outlining frontier

[Complete alpha proof](evidence/anm-integer-interpolation/README.md) closes
441950/370 with12 independent target fields in two complete four-TU /GL /GS
canonical colds,11 recursive inputs frozen. Target scalar easing performs
`FIMUL`, `FIADD`, then tail-jumps to ftol2: truncation encloses the entire
affine sum. A coefficient0.5 fade255 to0 gives127; product-only truncation
previously gave128. RGB intentionally has product truncation followed by
integer addition; scalar repair must not be generalized to it.

Named four Hermite weights and shared t+t recover alpha size370/frame8;
several return/product/declaration permutations remain6 ordinary bytes off.
A named float result accumulating final endpoint, final tangent, initial
tangent, initial endpoint before one cast reproduces all instructions.
Twelve existing same-image guards preserve4265 bytes/all174 fields twice;
with alpha,13 complete owners pass4635/all186. Actual body/owner/enum host
extraction passes12 scenarios,8 strict named-exit mutants and ASan/UBSan/leaks
using explicit timer/linear-curve roles. Original/native runtime, physical
source identity and later product/semantic/port planes remain unknown.
The reviewed authored numerator is66685/273274 (24.40%).

Fresh target/Ghidra RGB review confirms private EAX receiver, stack result,
RET4 and4 real calls to Int3Scale441E50/56 at4417C8,4417FB,44183F,44187F.
Target RGB has4 ftol2 calls overall; current candidate has16, with all four
scale operations inlined. The exact standalone56-byte leaf did not establish
parent call topology. By-value return API and a normal out-of-class scale
definition in the same /GL TU do not solve this. Global /Ob1 recovers4 calls,
but regresses radial and parent owners; /GL stays enabled and all such
controls are restored. Reference inliner-threshold notes are hypotheses;
forced attributes are not adopted. Real caller/TU/profile evidence is the
next step before changing that seam again.

Target executor loads/stores both RGB results in R/G/B order. Retaining that
ordinary source order raises whole diagnostic1846 to1850 of8564, while the
9964/frameFC parent still has89 target-sized groups/gap14 and7184/7995 aligned
agreement. Scale whole-value casts and explicit flag-word OR do not repair
the common copy/flag seam; small score variations are not ownership or type
proof, so those controls are restored. All28 controls are scoped in the linked
proof. Current scalar-family carrier supersedes vector-family source-cache
hashes; old baseline paths are historical after the canonical image rebuild.
