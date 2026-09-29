# `th10-decomphelp-forN0` 全量审计

**审计对象：** `/home/pentester/coding/codex_ida/th10-decomphelp-forN0`，commit
`649147241f6a931c849d905ca9badc264049db5b`（2026-09-16）。

**TH10 目标：** 原版日文 v1.00a，SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`。

本文件记录外部仓库的来源分类、逐 leaf 审计结果、吸纳规则和拒绝原因。外部
仓库不是 canonical source；任何 exact 结论都必须重新通过本仓库的目标绑定
边界、ABI 和 VC7.1 replay。逐函数机器结果保存在
[`DECOMPHELP_LEAF_AUDIT.csv`](DECOMPHELP_LEAF_AUDIT.csv)。

## 1. 来源清单和证据等级

| 外部材料 | 当前数量 | canonical 处理 | 原因 |
| --- | ---: | --- | --- |
| `src/LeafAccessors.cpp` C++ leaf | 606 functions | 逐函数复核；只有本地 target-bound replay 才能进入 `matches.csv` | 外部 `verify_leaves` 只证明语义/内存效果，不证明本仓库的 MSVC 代码生成上下文 |
| `src/Leaves.cpp` naked 转写 | 190 `__declspec(naked)` functions | 不作为可移植 source；只作 opcode/语义假设参考 | 直接复制目标字节不是自然 C++，不能满足本项目的 reconstruction 目标 |
| `docs/unverified/r2ghidra/pdg_*.c` | 510 PDG 草稿 | 全部保持 hypothesis，不能直接吸纳 | 外部目录自身标为 `unverified`，没有 canonical replay 或完整边界/ownership 契约 |
| 外部大型 TU（`AnmManager.cpp`、`AsciiManager.cpp`、`TitleScreen.cpp` 等） | 25 `src` files | 只抽取可被 TH10 事实验证的局部语义 | 外部类型、TU 分区、全局 owner、ABI 和 product graph 与本仓库不同 |
| `portable/` Switch/Linux 实现 | 12 `portable/src` files、14 headers | 留作后续 portability/语义假设 | 当前仍处 exact reconstruction 阶段；不能用移植行为替代 Windows i386 target Oracle |
| `salvage/` pipeline/state | 9 pipeline、5 support、33 state files | 只参考方法和负面线索 | `.pkl` verdict、旧 worktree 路径和未完成 phase5b 不是 TH10 canonical evidence |
| 外部 CI/reccmp/config | 4 compare text、3 JSON report、2285 mapping rows | 仅作候选排序 | 报告的编译图和目标边界没有经过本仓库的 acceptance contract |

外部仓库自己的 `docs/LEAF_CPP_DECOMP.md` 明确说明早期 C++ leaf 的 CI compare
曾因 artifact 下载失败而不能计为 verified；当前 `PROGRESS.md` 的
`606/606` 是外部语义 verifier 数字，不是本仓库 exact 数字。外部最新报告的
`605/767` 也只作为 hypothesis，不能整体搬入 canonical ledger。

## 2. 606 个 C++ leaf 的直接审计

审计步骤如下，均针对上述 hash-attested TH10；目标观察使用直接 IDA Pro MCP：

1. 对 `LeafAccessors.cpp` 的 606 个地址逐一调用 IDA `get_function_by_address`，
   并对 Enemy 批次用 `disassemble_function`/xref 复核尾部和边界。
2. 用 pinned VC7.1 SP1 `/O2 /Gy /GF /Oi /GL` 编译完整的 606-function leaf TU，
   再用 canonical linked-image harness 绑定 PDB contribution。
3. 仅把外部 `config/census.json` 的 size 当作临时比较输入；它本身不是边界
   authority。IDB 边界、canonical ledger 和完整 replay 仍是最终门槛。

结果：

| 项目 | 数量/字节 | 含义 |
| --- | ---: | --- |
| Leaf definitions | 606 / 606 unique addresses | 外部 C++ 候选全集 |
| IDA 已有 function boundary | 477 | 可进入边界/ABI复核 |
| IDA 没有 function boundary | 129 | 不能仅凭外部 size 建立 canonical owner；CSV 中标为 `boundary unavailable in direct IDA` |
| 完整外部 leaf TU 本地隔离链接 raw-exact | 397 / 3,473 bytes | 仅为 pinned-compiler triage，不是 canonical exact credit |
| 同 size 但 bytes 不同 | 30 | 保留为 codegen mismatch |
| size mismatch | 172 | 保留为 ABI/TU/边界或 source-shape mismatch |
| 外部 census 没有 size | 7 | 不能比较完整 extent |
| 直接 IDA boundary + raw-exact + size 一致 | 337 / 2,549 bytes | 可进入 canonical source/replay 队列 |
| 初始尚未进入 canonical function ledger | 311 / 2,103 bytes | 审计开始时的待吸纳候选总量 |
| 前两批吸纳后仍未进入 canonical ledger | 262 / 1,843 bytes | 49 个 / 260 bytes 已通过 focused replay 并移入 canonical exact |

逐行状态、canonical 现状和失败理由见 CSV；因此 606 个 leaf 没有被“整体
认可”，每个地址都有可检索的审计行。

一个重要的上下文验证是 `Fn00401CB0`：在完整 606-function TU 中生成目标的
7-byte private ABI；把它抽成只有 20 个函数的小 TU 后变成 15-byte stack ABI，
而同批其他简单 setter 仍可能相同。这证明不能按外部百分比或单函数小 TU
直接授予 exact，canonical replay 必须保留完整编译上下文。

## 3. 已吸纳和明确拒绝

此前已完成并提交的 Enemy 小批次（commit
`b27d206 gpt-5.6-luna-max: absorb verified Enemy leaf helpers from decomphelp`）
以及本次完整-TU batch 是目前新增的 canonical exact 吸纳：

| 地址 | 大小 | 状态 |
| --- | ---: | --- |
| `0x0040CC10` | 4 | canonical linked replay exact |
| `0x0040CC20` | 6 | canonical linked replay exact |
| `0x0040CC30` | 6 | canonical linked replay exact |
| `0x0040CC40` | 7 | 语义吻合但拒绝 exact；本地自然 source 在 isolated `/GL` context 生成 11-byte stack-argument ABI，而目标是 7-byte EAX ABI |

本次 focused replay 又吸纳了 `0x0040C460`–`0x0040CED0` 中的 27 个独立 leaf，
合计 153 bytes。每个 unit 都以完整 `DecomphelpLeafAccessors.cpp`（606 个函数）
作为 `/GL` TU，PDB contribution 为完整 function extent，27/27 均逐字节 exact；
这批函数的 origin 仍记录为 `unknown/indeterminate`，不因为 exact 而改变作者归属。

第二批 focused replay 吸纳了 `0x0040D4F0`、`0x0040D500`、`0x0040D800` 以及
`0x00412D50`–`0x004137E0` 中另外 22 个 leaf，合计 107 bytes；同一完整 TU
下 22/22 个 PDB contribution 逐字节 exact，origin 仍保持 `unknown/indeterminate`。

后续候选会按模块和可复现批次逐步进入 `DecomphelpLeafAccessors.cpp`
的完整 TU，并分别建立 canonical match unit；未通过的候选保留在 CSV，不得
写入 `matches.csv`。

## 4. 当前吸纳规则

- 目标边界必须由直接 IDA/已有 reviewed ledger 证明；外部 census size 不能单独
  建立 function owner。
- 源码必须是自然 C/C++。`Leaves.cpp` 的 naked bytes、复制机器码、伪造 return
  或 inert padding 一律拒绝。
- `/GL` leaf 必须在完整 TU/声明的 entry context 下获取 PDB contribution，解码
  完整 extent，并逐字节、逐 linkage replay；局部 size 或 normalized score 不够。
- target origin、source presence、exact codegen 和 product ownership 分开记录。
  没有 xref/作者归属证据的 leaf 使用 `origin=unknown,
  disposition=indeterminate`，即使代码 exact 也不改变 origin 结论。
- 每次批次只做受影响函数的 focused replay；累计多个批次后再做完整 cold replay。

## 5. 待处理队列

当前机器清单中，扣除前两批已吸纳的 49 个后，尚未 canonical 化但已满足“直接 IDA
boundary + 完整外部 leaf TU raw-exact”triage 的 262 个候选（1,843 bytes）按
地址区间分布如下：

| 区间 | 候选数 | 候选字节 |
| --- | ---: | ---: |
| early/ASCII | 93 | 638 |
| Enemy-ish (remaining) | 65 | 410 |
| Player/FrontEnd | 47 | 365 |
| Anm | 35 | 289 |
| Anm/ECL | 22 | 141 |

上表按原始区间汇总，当前 Enemy 区间已从队列扣除；下一批转向
AnmManager/FrontEnd/Player 区域，并在每个批次的 commit 中更新本文件和 CSV。
