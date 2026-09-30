# `th10-decomphelp-forN0` 全量审计

**审计对象：** `/home/pentester/coding/codex_ida/th10-decomphelp-forN0`，commit
`649147241f6a931c849d905ca9badc264049db5b`（2026-09-16）。

**TH10 目标：** 原版日文 v1.00a，SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`。

本文件记录外部仓库的来源分类、逐 leaf 审计结果、吸纳规则和拒绝原因。外部
仓库不是 canonical source；任何 exact 结论都必须重新通过本仓库的目标绑定
边界、ABI 和 VC7.1 replay。逐函数机器结果保存在
[`DECOMPHELP_LEAF_AUDIT.csv`](DECOMPHELP_LEAF_AUDIT.csv)。

目录级全仓库接收/拒绝矩阵（包括 Chain、TitleScreen、portable、port、salvage、
scripts 和 tests）见 [`DECOMPHELP_REPO_REVIEW.md`](DECOMPHELP_REPO_REVIEW.md)。
本文件继续作为 leaf 逐地址审计的详细记录。

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
| 无 IDA function boundary、但直接字节/tail/no-xref review + 完整 TU raw-exact | 32 / 674 bytes | 作为 boundary-indeterminate leaf 单独登记；不把外部 size 当作 owner 证据 |
| 初始尚未进入 canonical function ledger | 311 / 2,103 bytes | 审计开始时的待吸纳候选总量 |
| 前九批吸纳后仍未进入 canonical ledger | 0 / 0 bytes | 311 个 / 2,103 bytes 已通过 focused replay 并移入 canonical exact |

逐行状态、canonical 现状和失败理由见 CSV；因此 606 个 leaf 没有被“整体
认可”，每个地址都有可检索的审计行。

在上述有 IDA boundary 的队列清空后，另有 32 个原始 leaf 没有 IDA-created
function boundary，但直接 IDA Pro MCP 的完整字节/tail review（尾部 `ret` 后为
`CC`，且没有 entry xref）与完整 `DecomphelpLeafAccessors.cpp` 的 pinned VC7.1
linked-PE replay 同时闭合。这批共 674 bytes，已逐地址建立 canonical linked unit
并写入 ledger；它们的 `origin=unknown, disposition=indeterminate` 保持不变，不能
据此推断 target owner，也不代表外部 606 个 leaf 可以整体吸收。

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

第三批 focused replay 吸纳了 `0x00417580`–`0x004177E0` 与
`0x004189E0`–`0x004198B0` 两个稀疏地址段中的 39 个 leaf，合计 280 bytes；
同一完整 TU 下 39/39 个 PDB contribution 逐字节 exact。`0x004198B0` 有一个
数据 xref，但没有 code caller；这批仍保持 `module` 空、origin
`unknown/indeterminate`，不据此推断 owner。

第四批 focused replay 吸纳了剩余 Enemy-ish 区间的 26 个 leaf（
`0x0041AB50`–`0x0041FF40` 的稀疏地址），合计 130 bytes；同一完整 TU 下
26/26 个 PDB contribution 逐字节 exact。全部没有 code caller；其中若干
`nullsub`/返回零 helper 只有 vtable 或其它 data xref，因此仍保持 `module` 空、
origin `unknown/indeterminate`，data-only 引用不提升 owner 结论。

第五批 focused replay 吸纳了 Player/FrontEnd 地址区间的 47 个 leaf（
`0x004200C0`–`0x0042C830` 的稀疏地址），合计 365 bytes；同一完整 TU 下
47/47 个 PDB contribution 逐字节 exact。全部没有 code caller；`0x004200C0`
只有一个 data xref，其余没有 entry xref，因此仍保持 `module` 空、origin
`unknown/indeterminate`，不把地址区间标签当作 owner 结论。

第六批 focused replay 吸纳了 Anm 地址区间的 35 个 leaf（
`0x00434B70`–`0x0043E570` 的稀疏地址），合计 289 bytes；同一完整 TU 下
35/35 个 PDB contribution 逐字节 exact。全部没有 code/data entry xref，仍保持
`module` 空、origin `unknown/indeterminate`，不把地址区间标签当作 owner 结论。

第七批 focused replay 吸纳了 Anm/ECL 地址区间的 22 个 leaf（
`0x00441940`–`0x0044DF60` 的稀疏地址），合计 141 bytes；同一完整 TU 下
22/22 个 PDB contribution 逐字节 exact。全部没有 code/data entry xref，仍保持
`module` 空、origin `unknown/indeterminate`，不把地址区间标签当作 owner 结论。

第八批 focused replay 吸纳了 early/ASCII 地址区间前段的 59 个 leaf（
`0x00401CB0`–`0x00408AB0` 的稀疏地址），合计 413 bytes；同一完整 TU 下
59/59 个 PDB contribution 逐字节 exact。全部没有 code caller；`0x00405850`
只有一个 data xref，其余没有 entry xref，仍保持 `module` 空、origin
`unknown/indeterminate`。

第九批 focused replay 吸纳了 early/ASCII 地址区间剩余的 34 个 leaf（
`0x00409DC0`–`0x0040BA80` 的稀疏地址），合计 225 bytes；同一完整 TU 下
34/34 个 PDB contribution 逐字节 exact。全部没有 code caller；
`0x0040B050`、`0x0040B060`、`0x0040BA80` 各只有 data xref，仍保持
`module` 空、origin `unknown/indeterminate`。

第十批 focused replay 处理了一个此前仅差寄存器分配的 leaf：
`Fn00427E20 @ 0x00427E20`（23 bytes）。直接 IDA Pro MCP 复核了完整的
`EDX → ECX → EAX` 两个 dword pair-copy 边界；把外部标量临时变量改为两个自然
`Pair` aggregate assignments 后，完整 `DecomphelpLeafAccessors.cpp` `/GL` TU
产生与目标逐字节相同的 23-byte PDB contribution。该 leaf 没有 code caller，
因此 origin 仍为 `unknown/indeterminate`。

第十一批 focused replay 关闭了两个此前仅因 audit census 缺失 target size 而未登记的
leaf：`Fn00417600 @ 0x00417600`（10 bytes）与 `Fn004501E0 @ 0x004501E0`
（7 bytes）。直接 IDA Pro MCP 分别确认 bit-test 与 EAX/ECX setter 的完整 ret 边界、
无 code caller；完整 leaf TU replay 对两者均 raw-equal。另同步了已有 canonical
`Fn00409F60` 的 stale audit row；三者 origin 都保持 `unknown/indeterminate`。

第十二批 focused replay 关闭了六个此前仅差自然 copy 顺序的 decomphelp leaf：
`Fn00412D60/70/80/90` 与 `Fn0041AC20/30`，合计 70 bytes。直接 IDA Pro MCP
确认六个完整 ret 边界且均无 code caller；以标量局部、首个 store 后的
`_ReadWriteBarrier()`、第二次 load/store 与返回值的自然 C++ 形状，完整 leaf TU
的六个 canonical linked-PE replay 均 raw-equal。六者 origin 仍为
`unknown/indeterminate`，exactness 不被提升为 authorship 或 owner 结论。

另一个独立的 32-leaf batch（674 bytes）处理了审计 CSV 中仍标为 absent、但没有
IDA function boundary 的地址。每个地址都先经直接 IDA Pro MCP 字节/tail 与 entry
xref review，再在完整 leaf TU 的 canonical linked-PE context 中逐字节 replay；
32/32 units 均 raw-equal。该 batch 只证明 exact codegen，不改变 source presence、
owner 或 origin 结论。

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

当前机器清单中，扣除前九批已吸纳的 311 个后，尚未 canonical 化但已满足“直接 IDA
boundary + 完整外部 leaf TU raw-exact”triage 的 0 个候选（0 bytes）按
地址区间分布如下：

| 分类 | 候选数 | 候选字节 |
| --- | ---: | ---: |
| early/ASCII | 0 | 0 |
| Enemy-ish (remaining) | 0 | 0 |
| Player/FrontEnd | 0 | 0 |
| Anm | 0 | 0 |
| Anm/ECL | 0 | 0 |

上表按原始区间汇总，当前 Enemy 区间已从队列扣除；32 个 boundary-indeterminate
leaf 已另行登记，不计入这张“有 IDA boundary”队列。下一批转向
AnmManager/FrontEnd/Player 区域，并在每个批次的 commit 中更新本文件和 CSV。
