# TH10 reconstruction handoff — 2026-11-02

This is the **current resume brief**, not a running log of every experiment.
Use [PROGRESS.md](PROGRESS.md) for exact counts, the function and origin
ledgers for individual claims, [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md)
for durable target facts, and [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the
current private working set and artifact cleanup. Prior dated narratives
and historical numbers are retained in Git and the
[2026-10-30 history](RE_HANDOFF_HISTORY_20261030.md). They are
not current acceptance receipts.

## Target, rules and validation

- Target: **original Japanese TH10 v1.00a** at ignored
  resources/th10.exe, 487,936 bytes, SHA-256
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main, upstream origin/main. The latest source checkpoint in
  this handoff is e0ad99a; use git status/log to see subsequent docs
  or local commits. **Never push automatically.**
- Local commit format: **gpt-web: xxx**, author
  N0zoM1z0 <1705004398@qq.com>, with the existing gensokyo-dev
  co-author trailer. Take multiple useful checkpoints per working session.
- **Do not discard dirty work**; inspect and finish or document it. Work on
  both large core functions and useful leaves. Do not trust old claims
  without independent source/target comparison.
- Cold replay **only new/affected --unit selectors**; no full historical
  cold replay. A matched size, masked bytes, successful compile or local
  commit does not equal codegen exactness.
- Run the local repository Python wrapper. Original source/TU,
  game product, runtime semantics and portability remain unproven.
  Ghidra/IDA names are provisional until confirmed.

    git status --short --branch
    git log -6 --oneline
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/ci.py

Default CI has seven lightweight checks. Only a complete target-bound,
PDB/COFF-owned and cold-reproducible zero-difference result with *every*
external address and required local destination bound can enter
config/matches.csv. Generate ledger statistics with:

    scripts/repo-python scripts/progress.py
    scripts/repo-python scripts/report-reconstruction-status.py

## Current accepted snapshot

| Measure | 2026-11-02 accepted state |
| --- | ---: |
| Tracked target extents / reviewed boundaries | 1,819 / 1,819 |
| Source-present target mappings | 1,230 |
| Canonical exact owners / bytes | **1,430 / 184,096** |
| Reviewed authored exact owners / bytes | **542 / 115,605** |
| Reviewed authored byte denominator | 275,890 (provisional) |
| Authored source-present nonexact backlog | 164 |
| Classified excluded / origin-indeterminate owners | 520 / 552 |

These values are generated from current ledgers. The denominator is not
an established total for all original game-authored code.

### Recent substantive complete-owner proofs

| Area | Accepted full-owner result | Reproduce / evidence |
| --- | --- | --- |
| Game owner initialization @0x0040AE70 | **134/134 authored exact**, 14 strict fields, two independently selected colds; cleanup @0x0040AF00 stays 137/137 | [Initializer](evidence/game-owner-40ae70-initialize-exact-20261102/README.md), [cleanup](evidence/game-owner-40af00-exact-20261102/README.md) |
| Game callback cleanup @0x0040AF00 | **137/137 authored exact**, 15 fields; correct final global is 0x004776FC, not the similar bomb object's 0x004776EC | [Complete game owner proof](evidence/game-owner-40af00-exact-20261102/README.md) |
| Hint automatic-entry @0x0041A120 | **220/220 authored exact**, seven fields | [Hint allocator proof](evidence/hint-automatic-entry-exact-20261102/README.md) |
| ResultMenuResume @0x00422C30 | **74/74 authored exact**, seven fields, target VM-deletion/speed scheduling | [Result menu proof](evidence/result-menu-resume-exact-20261102/README.md) |
| Stage cross product @0x00421B90 | **77/77 codegen exact** and camera330 guarded; original target entry/authoring indeterminate | [Stage proof](evidence/stage-cross-product-exact-20261102/README.md) |
| Duplicate RNG target @0x0044BA30 | **78/78 codegen exact** using existing relocation-free RNG symbol; do not invent a second C++ implementation or authored origin | [RNG proof](evidence/rng-duplicate-u32-exact-20261102/README.md) |
| BulletRuntimeView destructor @0x00405DE0 | **34/34 codegen exact**, one strict free call, origin indeterminate | [Bullet lifecycle proof](evidence/bullet-runtime-generated-destructor-exact-20261102/README.md) |
| ScriptViewer deleting destructor @0x0040A410 | **27/27 codegen exact**, two strictly bound calls, origin indeterminate | [ScriptViewer proof](evidence/script-viewer-scalar-deleting-dtor-exact-20261102/README.md) |
| ScriptViewer enemy release @0x0040D730 | **19/19 codegen exact**, two strict callee fields, origin indeterminate | [Release proof](evidence/script-viewer-release-enemies-exact-20261102/README.md) |
| ZWave GetBuffer and Get3DBuffer | **19/19** and **52/52 codegen exact**, source origin indeterminate | [GetBuffer](evidence/zwave-getbuffer-exact-20261102/README.md), [Get3D](evidence/zwave-get3d-buffer-exact-20261102/README.md) |

The Sound/ZWave duplicate GetBuffer definition was also fixed at the
source level; true Sound+ZWave compilation now links and its selected
exact Sound owners remain 1,641/1,641. This is **integration work,
not additional codegen credit**:
[producer repair](evidence/sound-zwave-single-owner-20261102/README.md).
Other earlier exact functions remain accepted according to the ledgers.

## Open complete functions: next agent's priorities

Every row below is **NONEXACT**, even when a candidate has the right length.
The numerical comparison is a particular source-epoch diagnostic; recompile
the current source before claiming an improvement.

| Owner, target extent | Last useful diagnostic and next concrete lead |
| --- | --- |
| **ANM ExecuteScript @0x0043EE30, 9,964 bytes** | Last complete native diagnostic **9,962/9,964**, 350 external fields, 95 local obligations. Only opcode91 child-create ECX-versus-EDX two-byte scheduling remains. The CreateVmVariant2 member/stdcall ABI is separately verified; don't fake the registers or change its three-stack-argument contract. [Boss frontier](evidence/anm-executor-two-byte-frontier-20261031/README.md), [member ABI](evidence/anm-child91-member-stdcall-20261101/README.md) |
| **EclVmContext::Run @0x0044E1A0, 7,020 bytes** | Maintained volatile-x LENGTH_SQUARED scope gives **6,997/7,020** with all 189 fields; 23 ordinary bytes remain in x87/format trace locals. Failed POINT_ANGLE, aggregate and reversed-expression source controls were restored. [Large ECL proof](evidence/ecl-length-squared-stack-20261102/README.md) |
| **GameCreateOwner40AF90 @0x0040AF90, 86 bytes** | Value-init plus a scoped compiler memory barrier now yields **86 native bytes against 86 target bytes**, recovering the actual memory OR; **NONEXACT** because EBX publication and null-branch layout still differ. [Flag-update comparison](evidence/game-owner-creator-barrier-20261102/README.md), [value-init](evidence/game-owner-creator-value-init-20261102/README.md), [initializer](evidence/game-owner-40ae70-initialize-exact-20261102/README.md) |
| **GameManagerCleanup @0x00417C80, 1,105 bytes** | Historical 34-input candidate **1,098/1,105**; source now includes additional exact helper definitions, so that number is **not asserted current**. Rebuild before testing flags, branch blocks and VM calls. [Previous cleanup context](evidence/gui-message-teardown-20261031/README.md) |
| **MainConfigurationDialogProc @0x0043A3A0, 233 bytes** | Current maintained barrier near WM_COMMAND gave **216/233**, 14 fields; ESI saves and return epilogue order still differ. [Command scheduling](evidence/main-dialog-command-scheduling-20261102/README.md) |
| **Sound LoadSoundSample @0x0043D390, 684 bytes** | Last source graph **678/684**, 25 address-field positions (not all original roles proven); six bytes at +0x122..+0x127 are MOV/LEA argument-order scheduling. [Sound proof](evidence/sound-sample-lifetime-20261102/README.md) |
| **Sound PreloadBgm @0x0043D7D0, 369 bytes** | Same-size candidate, **not exact**; last useful proof was **365/369** with ten address fields, remaining EAX/EDX ReadFile setup bytes. Do not use the older six-difference note as current. [Bounded proof](evidence/sound-preload-barrier-20261031/README.md) |
| **Hint ConsumeEntries @0x00419120, 1,317 bytes** | Historical **1,308/1,317**, still nonexact; ID result and script-push register scheduling. The Hint Update571 and AddAutomaticEntry220 are already exact. [Hint consumption](evidence/hint-consume-context-20261031/README.md) |
| **GUI DrawStageElements @0x00415800, 724 bytes** | Full-size owner and 33 native fields, **not exact**: local rectangle LEA/input field order still differs. [GUI draw proof](evidence/gui-draw-read-order-20261031/README.md) |
| **ANM LoadTexture / LoadTextureRegion** | Native owners **242/243** and **319/320** respectively, wrong embedded-header ADD/LEA length. Three selected existing texture guards still replay 437/437. [Texture upload](evidence/anm-texture-load-scope-20261102/README.md), [region](evidence/anm-texture-region-scope-20261102/README.md) |
| **GUI message Run / Enemy float operands / Ending Run** | Earlier complete contexts remain nonexact, but their source pins are stale. Consult the original evidence and rebuild a chosen scope; do not repeat old scores as current facts. |

Do not waste runs on a previously rejected cosmetic permutation unless a
new source/ABI observation yields a falsifiable instruction-level prediction.
Continue substantial large-owner work alongside small exact candidates.

## Artifacts and storage after handoff cleanup

The **2026-11-02 storage cleanup** removed about **200.41 MiB** of
regenerable experimental PE/OBJ/PDB and Python bytecode. It preserved
six particularly useful nonexact/near-exact witnesses under
build/gpt-web, the entire **build/match-linked** canonical producer
cache, prior build/gpt-6.1-sol contexts, private analysis archives,
target, toolchain, Ghidra project and D3D adapter evidence.

[Cleanup receipt](evidence/handoff-storage-20261102/cleanup-summary.json)
and [artifact recovery guide](RE_ARTIFACTS.md) give exact paths and
storage policy. Deleted experimental paths must be **regenerated**; the
ignored pruned-file inventory records names and hashes, not payloads.
Historical build manifests may have mismatched source/header pins.
Never silently reuse an older compiled PE/PDB as a current-source witness.

There is no global whole-product exactness or native runnable binary.
Make new commits after substantial verified source improvements, and
leave a concise current handoff rather than appending duplicate
sections to this page. Older handoff history remains in Git.
