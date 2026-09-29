# `th10-decomphelp-forN0` 全仓库接收/拒绝记录

本文件是对外部仓库
`/home/pentester/coding/codex_ida/th10-decomphelp-forN0` 的目录级审计。
它和 [`DECOMPHELP_AUDIT.md`](DECOMPHELP_AUDIT.md) 的逐 leaf CSV 审计互补：本文件
覆盖外部仓库的全部 tracked material，并记录为什么接收、保留为假设或拒绝；leaf
逐地址状态仍以 [`DECOMPHELP_LEAF_AUDIT.csv`](DECOMPHELP_LEAF_AUDIT.csv) 为准。

## 身份和门槛

| 项目 | 值 |
| --- | --- |
| 外部 HEAD | `649147241f6a931c849d905ca9badc264049db5b` (2026-09-16) |
| 外部 tracked 文件 | 797 |
| canonical target | 日文 TH10 v1.00a |
| target SHA-256 | `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040` |
| 本次 target 观察 | 直接 IDA Pro MCP；IDA metadata 的 base `0x400000`、image size `0x9c000`、MD5 `7dc488d82c81dd4aee4ba098b8804d83` 与 canonical target 一致 |

外部仓库没有 tracked `resources/th10.exe` 或 `th10.dat`；其 `.gitignore` 也明确把它们
排除。因此外部脚本、reccmp 数字和 PDB 不能替代本仓库的 target-bound evidence。

TH10 接收 contract 是：

1. function boundary 必须来自本地 target/直接 IDA 或已审核 ledger；外部 census size
   不能单独建立边界。
2. source 必须是自然 C/C++；naked 汇编、复制 opcode、伪造 return 和 inert padding
   不算 reconstruction source。
3. exact 必须经过 pinned VC7.1 SP1 build6030 的完整 normal-COFF 或 `/GL` linked-PE
   artifact，拥有完整 PDB/COFF extent，并把全部 relocation/linkage 目标显式 replay
   到 TH10。外部 semantic verifier、reccmp 百分比和单独的解释器测试都不授予 exact。
4. boundary、origin、source presence、exact codegen、product closure 和 portability
   分开记账。external source 与 target bytes 恰好相同也不自动证明原始 TU/owner。

## 总体清单

| 路径 | 文件数 | 决定 |
| --- | ---: | --- |
| `src/` | 25（12 个 `.cpp`，含 `th_pch.cpp`；12 个 `.hpp`；`th_pch.h`） | 逐文件检查；目前四个 non-leaf 函数和 leaf 子集通过 canonical exact gate；其余逐文件结论如下 |
| `docs/unverified/` | 537（含 510 个 `r2ghidra/pdg_*.c`） | 全部 hypothesis，不进入 canonical source |
| `docs/compare-evidence/` | 8 | 机器报告/triage；不直接进入 `matches.csv` |
| `portable/` | 27（12 source、14 headers、1 tool） | 语义/可移植阶段材料，exact 阶段拒绝 |
| `port/` | 55 | Switch/Linux/SDL2 层，不能证明 Windows i386 codegen |
| `salvage/` | 48（9 pipeline、5 support、33 state、README） | 未闭合的中间流水线，拒绝作为 source 或 exact evidence |
| `config/` | 9 | census、globals、reccmp 和 milestones，只能排队/交叉检查 |
| `scripts/` | 50 | 方法和假设参考；外部 verifier 的 acceptance 不同，不复制到 canonical |
| `tests/` | 8 | 格式/行为测试，不是 target-bound function source |
| `bridge/` | 5 | CI/patch 基础设施，不是 TH10 source |
| `resources/` | 5 | placeholder/progress 资源，无 canonical game input |
| `.github/`、root manifests/readmes、`tools/` | 5、7、1 | 工作流、说明和辅助工具；不吸收为 exact source |

## `src/` 的逐文件决定

### 已接收：`src/Chain.cpp` 的两个函数

外部 Chain TU 提供了自然 C++ 的 list/element 结构和 callback 语义。直接 IDA
Pro MCP 复核 `0x00449F60`：完整边界为 114 bytes，函数先搜索 `chain+0x14` 的
calc list，再搜索 `chain+0x38` 的 draw list；命中后修复相邻 prev/next，清除
callback 字段，并在 flags bit 0 置位时调用 `j__free @ 0x004524A1`。IDA 返回的
物理输入是 ECX=`ChainElem*`、EDX=`Chain*`；该函数有大量 target callers（包括
`0x00449E50` 的链表清理路径）。

本地接收文件为 [`src/Chain.cpp`](../src/Chain.cpp)，unit 为
`decomphelp-chain-unregister`：

| 项目 | 结果 |
| --- | --- |
| compiler/link | pinned VC7.1 SP1 build6030，`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`，canonical linked-PE harness |
| entry context | `Chain::AllocElem`（保留整个 Chain TU 的 LTCG context；直接以 `UnregisterElem` 为 root 会得到不同的 128-byte private context） |
| PDB contribution | 完整 114 bytes，Capstone 5.0.6 解码完整 extent |
| declared linkage | 仅 `REL32 @ +0x68`，semantic `_free`，replay target `0x004524A1` |
| cold replay 1 | PE SHA-256 `d3ef4c33360c1775b288c180462c91d893648ab5460c58fb6cb456cc806cad5e`，114/114 |
| cold replay 2 | PE SHA-256 `03ca769e677177c401e3fd96529d7b9c2d3c35be8888f108f1424d5bdc8726d0`，114/114 |
| ledger | `config/functions.csv`、`reccmp-functions.csv`、`implemented.csv`、`matches.csv` 和 `match-units.toml` 已接通 |

这是本次 repo review 中新增的四个 non-leaf canonical exact 之一。origin 仍保持原有
target ledger 的 `authored_game / GameUnassigned` 结论；exact replay 不被用来推断
原始作者或生产 TU。

同一 TU 的 `RemoveAllFromList @ 0x00449E50` 也在直接 IDA 和 canonical replay 后接收。
IDA 的物理边界是 82 bytes，输入为 EAX=`listBase`、栈上的 `Chain*`，尾部为
`retn 4`；函数在目标 critical section 内遍历 `listBase + 0x18`，对每个节点调用
`UnregisterElem`，最后递减 `g_ChainNestCounter`。外部草稿的语义保留，但普通 cdecl
声明不能得到目标尾部；将声明校正为 target-observed `__stdcall` 后，保留同一个
`Chain::AllocElem` `/GL` entry context，在 7 个 target-resolved linkage fields 下
复现完整 82-byte PDB contribution：

| 项目 | 结果 |
| --- | --- |
| unit | `decomphelp-chain-remove-all`，同一 `Chain.cpp` / `Chain::AllocElem` entry context |
| linkage | `EnterCriticalSection`/`LeaveCriticalSection` IAT、两次 critical-section global、两次 chain-nest global、`UnregisterElem` REL32 |
| cold replay 1 | PE SHA-256 `504bbc25207e386663398dfa5bf5b30be13337249eff1865c69379cac30ddb7f`，82/82 |
| cold replay 2 | PE SHA-256 `b64af223e34abde5b9afac28c39b64e5423af1ca24d29b507fdcc70dc8671399`，82/82 |
| ledger | `functions.csv`、`reccmp-functions.csv`、`implemented.csv`、`matches.csv` 和 `match-units.toml` 已接通 |

同一外部 Chain TU 的其它候选没有吸收：

| 外部候选 | 外部 report/语义 | 本地结论 |
| --- | --- | --- |
| `AllocElem @ 0x449ED0` | reccmp `0.7917`；malloc/flags 形状合理 | `/GL` probe 在 tested context 为 69 bytes，target 为 74；ABI/TU context 未闭合，保留 open |
| `AddToCalcChain @ 0x449AE0` | reccmp `0.1111` | natural helper 被折叠/分裂为 wrapper（25 或 8 bytes），target 为 131；拒绝 exact |
| `AddToDrawChain @ 0x449B70` | reccmp `0.1111` | 同上，拒绝 exact |
| `RemoveAllFromList @ 0x449E50` | 外部 reccmp `1.0` | 已接收：直接 IDA 确认 82-byte `retn 4` 边界；自然 source 声明改为 `__stdcall` 后，`decomphelp-chain-remove-all` 在 AllocElem `/GL` context 下两次 82/82 exact replay |
| `RegisterCalc @ 0x44A000`、`RegisterDraw @ 0x44A030` | 外部各 `0.6` | context-dependent 34–50 bytes，target 各 43；未闭合 linkage/TU，拒绝 exact |
| `Chain::Remove` | source 有自然实现，但没有可独立绑定的 target address/ledger row | 不建立 target mapping |

### 已接收：`src/GameErrorContext.cpp` 的 `Log`

外部 `GameErrorContext.cpp` 的 `Log` 是本次逐文件 review 中另一个通过完整门禁的
自然 C++ 候选。直接 IDA Pro MCP 复核 `0x0044B810`：边界为 201 bytes，函数以
ECX=`GameErrorContext*`、栈上 `fmt/varargs` 输入，建立 0x2004 的 `/GS` frame，锁住
`g_CriticalSections + 0x48 = 0x004922BC`，递增 `g_LogLockDepth @ 0x0049231F`，调用
`_vsprintf @ 0x004524A6`，执行 0x1FFF 上限的 NUL-terminated append，最后解锁、递减
depth、校验 cookie 并返回 `fmt`。直接 callers 包括 `0x00401110`；target IDA 的
`return_type=char *` 和实际尾部 `mov eax, [fmt]` 是外部 `void` 草稿需要修正的 ABI/源
形状事实。

本地接收文件为 [`src/GameErrorContext.cpp`](../src/GameErrorContext.cpp)，canonical
unit 为 `decomphelp-game-error-log`。为得到 target-observed register lifetime，source
保留外部的 `Fatal` 同 TU 作为 entry context；`Log` 自身使用自然的 `self = this` 别名
和 `char *` 返回。该 source 形状不是复制 target bytes，也不推断原始 production TU：

| 项目 | 结果 |
| --- | --- |
| compiler/link | pinned VC7.1 SP1 build6030；`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`；canonical linked-PE harness |
| entry context | `GameErrorContext::Fatal`（目标 `Log` 的 201-byte WPO contribution 在该同 TU context 下闭合；直接以 `Log` 为 entry 会得到不同 extent） |
| PDB contribution | 完整 201 bytes，Capstone 5.0.6 解码完整 extent |
| declared linkage | 12 fields：`__chkstk`、cookie、critical-section `DIR32 +0x48`（两次）、Enter/LeaveCriticalSection、`g_LogLockDepth`（四次）、`_vsprintf`、cookie check |
| cold replay 1 | PE SHA-256 `b59502d2cf755cdd239d6772d956a2220caef72d041dfb4b4ad6efac4c873ace`，201/201 |
| cold replay 2 | PE SHA-256 `3e7f8b7b4e4dd5a9f2614366de2410a4fde073229c53e5e49442d0f683427965`，201/201 |
| ledger | `functions.csv`、`reccmp-functions.csv`、`implemented.csv`、`matches.csv` 和 `match-units.toml` 已接通 |

同一外部文件的 `Fatal @ 0x0044B8E0` 也已接收。直接 IDA 显示它是完整 201-byte
`/GS` body，目标保留 EDI 私有 receiver、在 `+0x2004` 写 fatal flag，并返回 format
指针。将外部 `void` 草稿改为自然 `char *` 返回，在同一 TU 加入只用于 entry-context
的自然调用根后，pinned VC7.1 linked-PE 贡献闭合为完整 201 bytes；目标体的十一条
import/global/runtime linkage 均已显式 replay。这个 entry root 不是 target mapping，也
不把 EDI 私有寄存器提升为公开 source calling convention。

| 项目 | 结果 |
| --- | --- |
| unit | `decomphelp-game-error-fatal` |
| entry context | source-local `ProbeFatalRoot`（自然地调用全局 `GameErrorContext::Fatal`；仅用于 WPO register lifetime） |
| PDB contribution | 完整 201 bytes，Capstone 5.0.6 解码完整 extent |
| declared linkage | 11 fields：cookie、critical-section `DIR32 +0x48`（两次）、Enter/LeaveCriticalSection、`g_LogLockDepth`（四次）、`_vsprintf`、cookie check |
| cold replay 1 | PE SHA-256 `eed84de81ca1e0e7f4152062532eb886213684f0e8850aab958c0f83ec741821`，201/201 |
| cold replay 2 | PE SHA-256 `8c05a9314240bad2e55e1e2113515c34f675d0c27ca3e99e649f6625ea006f03`，201/201 |
| ledger | `functions.csv`、`reccmp-functions.csv`、`implemented.csv`、`matches.csv` 和 `match-units.toml` 已接通 |

### Leaf source：局部接收、其余逐行保留

`src/LeafAccessors.cpp` 的 606 个 C++ definitions 已按直接 IDA boundary、完整
606-function `/GL` TU 和 focused replay 审计。当前 CSV 分布为 `379 exact / 180
absent / 45 unclassified / 2 excluded`；审计开始时尚未进入 canonical ledger 的
311 个、2,103 bytes 已在九个 focused batches 中全部吸收。每个 exact row 的 unit、
target boundary、linkage 和 origin 独立记录在 [`DECOMPHELP_AUDIT.md`](DECOMPHELP_AUDIT.md)
及 CSV 中；这不等于把整个外部 TU 或外部 semantic verifier 当作 exact。

`src/Leaves.cpp` 有 190 个 `__declspec(naked)` 函数。它们只保留为 opcode/语义
假设，不能复制进 canonical source；这条规则也适用于外部其他文件里的 naked
functions。

### 其它 `.cpp`：逐文件拒绝或保留为 open hypothesis

| 文件 | 观察 | 决定/原因 |
| --- | --- | --- |
| `AnmManager.cpp` | 大部分目标函数是 naked 转写；含若干地址注释和全局假设 | 不吸收 naked/未闭合 TU；已有 leaf/Anm exact 必须走本地 source/unit，不继承外部命名或 owner |
| `AsciiManager.cpp` | `RegisterChain`、`OnUpdate`、draw wrappers 多为 naked；`OnDrawLowPrioImpl @ 0x401760`、`OnDrawHighPrioImpl @ 0x401A50` 是 volatile-read/`return 0` stub，外部 report 均 0 | stub 不是 faithful source；拒绝 |
| `GameErrorContext.cpp` | `Log @ 0x44B810` 与 `Fatal @ 0x44B8E0` 是自然 C++；后者目标为 EDI 私有 receiver | 两者均已接收并进入 canonical ledgers；`Fatal` 保留 source-local natural entry context，但不宣称公开 EDI ABI |
| `GameManager.cpp` | `MainThread @ 0x417870`、`OnUpdate @ 0x418190` 是保持调用存活的 volatile stub；大量其它函数 naked | report `MainThread=0`；未闭合，拒绝 |
| `GameWindow.cpp` | WindowProc 和 helper 多为 naked；`InitD3DRendering @ 0x439890` 是 stub；`SetupSystemParameters @ 0x4392E0` 与 `RestoreSystemParameters @ 0x439350` 是自然 C++ | 两个 system-parameter helper 已由本地 `MainInitializeSystemParameters`/`MainRestoreSystemParameters` 在同一 target extent 上 exact，外部版本不重复吸收。WindowProc/CreateGameWindow/CheckForRunningGameInstance 仍受私有 ABI 或 report 低分限制；`InitD3DRendering=0`，拒绝 |
| `SmollScore.cpp` | 主要 lifecycle/update/draw 都是 naked，包括地址化的原始指令 | 即使外部 report 多行 `1.0`，仍不是自然可移植 source；拒绝 |
| `TitleScreen.cpp` | `SetState @ 0x42C5C0`、`SetSubState @ 0x42C620`、callbacks @ `0x42D2E0/0x42D2F0` 与 canonical FrontEnd rows 重叠；`PlaySoundEffectImpl @ 0x42C670`、大状态机/draw 有低分或 stub | 不重复吸收：canonical 已有 `FrontEndControllerView::SetScreenTarget`、`SetScreenStateTarget`、`FrontEndUpdateCallback`、`FrontEndDrawCallback` exact。直接 IDA 显示 `RegisterChain @ 0x42CAA0` 为 192 bytes、receiver live-in EBX，且 `AddToCalc/DrawChain` 使用 ESI/EDI + stack 的私有 seam；自然 `/GL` probes 仍保留标准 receiver 或 stack cleanup 差异，不能 exact。`PlaySoundEffectImpl` 的 target 95-byte body 同样使用 stack title + live-in EDI sound id 和 private ANM helper seams，外部 report 为 0，未吸收。`RegisterChain`（`0.5979`）、`SetState`（`0.8125`）、大 update/draw（`0.278/0.3284/0`）仍 open |
| `main.cpp` | WinMain 周边多为 opaque/volatile stub；不是完整 game TU | 仅作调用图/全局假设参考，拒绝 |
| `th_pch.cpp`, `th_pch.h`, `*.hpp`, `ZunBool.hpp`, `ZunResult.hpp`, `inttypes.hpp`, `diffbuild.hpp` | PCH、声明、类型和构建宏 | 不产生独立 target exact，不能作为 canonical absorption |

## 非 source 目录

### `docs/`

- `docs/unverified/` 的 537 个文件（其中 510 个 PDG 草稿）按其自身 README 明确
  标记为可能正确、猜测或错误；没有 target-bound boundary/ABI/replay，全部拒绝为
  fact。需要使用时必须先用本地直接 IDA 和 pinned replay 重建证据。
- `docs/compare-evidence/` 的 8 个文件是外部 CI 的原始输出。最新
  `reccmp-report-2026-08-21T20-12Z.json` 有 757 rows，其中 657 rows 的
  `matching=1.0`、53 rows 为 0；这些数字仅用于候选排序。外部 README 自己也把
  `PROGRESS.md` 的 `605/767` 描述为“bytes/semantic progress”而非本项目 exact
  ledger。不能批量复制到 `config/matches.csv`。
- `ANM_VM_INTERP.md`、`GHIDRA.md`、`LEAF_CPP_DECOMP.md`、screenshots 和根文档是
  方法/语义/展示资料；`LEAF_CPP_DECOMP.md` 还记录过 CI artifact 下载失败，因此
  外部 `verify_leaves` 不能替代本地 compiler Oracle。

### `portable/` 与 `port/`

`portable/` 的 12 个 source、14 个 headers 和 `dattool` 是 Switch/Linux 可移植
实现；`port/` 的 55 个文件覆盖 app、backends、Switch、tests、th10 glue 和 tools。
它们对后续 semantic/portability gate 有价值，但当前 exact gate 需要 Windows i386
machine output，故不吸收为 TH10 source 或 byte claim。

### `salvage/`

外部 README 明确写着 phase 5b emit 尚未完成，最终 `.cpp` 没有生成，build/link 没有
验证；state 还依赖 `/tmp/w07952`、`/tmp/th10.exe` 等作者本地硬编码路径。pipeline
的 1,563 survivors、BAD/rescued/fresh 数字是中间状态，不是 canonical ownership 或
exact proof。全部拒绝直接吸收；只保留算法/负面线索。

### `config/`、`scripts/`、`tests/`、CI 和资源

- 外部 `config/reccmp-functions.csv` 有 2,285 data rows（加 header 为 2,286），
  `census.json` 的 conservative denominator 是 1,705 functions / 94,481 bytes，
  discovered game upper bound 是 2,948 / 162,213 bytes。它们是外部 inventory，不能
  覆盖本地 1,635-row boundary/origin ledger；globals/floats/strings/milestones 也
  只作候选和语义参考。
- 50 个 scripts 可以帮助理解 external census、leaf conversion、port progress 和
  salvage，但它们默认不同的 target/input 路径，且 `verify_leaves` 偏向 semantic
  execution。没有复制到 canonical toolchain。
- 8 个 tests 是 ANM/ECL/MSG/RPY/STD/THA1/THTX 等格式或行为测试，不提供完整
  Windows i386 function extent；保留作未来 runtime/semantic 参考。
- `.github/` workflows、`bridge/patches` 和 placeholder resources 不改变目标代码，
  不进入 `src/` 或 exact ledgers；外部仓库没有可接收的 game binary/data。

## 当前结论和后续队列

本次 repo-wide review 的实际 canonical 增量是：

- `Chain::RemoveAllFromList @ 0x00449E50`：82 bytes，两个独立 cold linked-PE replay
  exact，已进入 match unit 和全部 tracking ledgers。
- `Chain::UnregisterElem @ 0x00449F60`：114 bytes，两个独立 cold linked-PE replay
  exact，已进入 match unit 和全部 tracking ledgers。
- `GameErrorContext::Log @ 0x0044B810`：201 bytes，两个独立 cold linked-PE replay
  exact，已进入 match unit 和全部 tracking ledgers。
- `GameErrorContext::Fatal @ 0x0044B8E0`：201 bytes，两个独立 cold linked-PE replay
  exact，已进入 match unit 和全部 tracking ledgers；其 EDI receiver 仍只作为
  target-observed private context 记录。
- leaf 方面：此前九批的 311 个/2,103 bytes 已全部 canonical exact；完整逐行结果
  仍以 leaf CSV 为准。
- 其它外部材料：Chain 的其余候选以及其它 `.cpp`、naked、PDG、portable、port、
  salvage 材料仍没有同时满足“直接 IDA boundary + natural source + 完整 pinned
  VC7.1 replay + exhaustive linkage”的证据，不能以 external reccmp `1.0` 批量提升。

本次对 `src/Chain.cpp` 和 `src/GameErrorContext.cpp` 做了 focused replay；没有宣称
whole-product cold replay、native product closure、runtime closure 或 portability 已
完成。后续若继续处理 Chain/Title/Anm/GameManager/Player 候选，仍按单函数或小批
focused replay，积累一批后再跑完整 cold/product gate。
