# TH10 reconstruction — live next-agent handoff

**Verified 2026-11-07, source checkpoint `6f1b83c`.** This is a *working
resume brief*, not a list of every experiment. The previous 603-line handoff
is available with `git show 9996cd6:docs/RE_HANDOFF.md`; the detailed dated
hypotheses are in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) and
[`docs/evidence/`](evidence/). Check live status and hashes again; a source
or build epoch may have changed after this snapshot.

## Start here — actual repo and rules

```sh
git status --short --branch && git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

1. Read [AGENTS.md](../AGENTS.md). Target is the **read-only** Japanese TH10
   v1.00a `resources/th10.exe`, **487,936 bytes**, SHA-256
   `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
   Use pinned VC7.1 SP1, the **real** `/GL /GS` graph and original functions.
2. **Treat dirty changes as work to continue**, not a reason to reset. Never
   blindly `git reset`, stash/discard, force-push or push. Make meaningful
   source commits as `gpt-web: ...`; separate verified stages into multiple
   commits when appropriate.
3. **Never run an all-unit historical cold replay.** Use repeated `--unit`
   selectors only for the modified producer and affected exact callees:

   ```sh
   scripts/repo-python scripts/replay-exact-units.py \
     --unit ecl-vm-stack-push --unit ecl-vm-stack-pop --json
   ```

4. **Exact acceptance means the complete unique PDB/COFF function owner,
   full original bytes, independently original-bound code/data fields,
   local branches/tables, and two independent current-source cold builds.**
   A candidate-vs-candidate zero-diff experiment, matching *length*, or an
   unchanged canonical regression guard earns **zero new exact credit**.
   Watch compiler-graph context and all source/header SHA pins. The target's
   original private compiler source partition is not necessarily reconstructed.

## Authoritative progress and current nonexact work

## Alternate text render local stack-frame improvement (2026-11-08)

`TextHelperView::RenderTextToTextureAlternate @0x00437FE0` now scopes
its late D3D upload rectangle and surface pointer naturally, restoring
the target **0x14-byte** stack frame from previous 0x18. Its full
509-byte normal-COFF owner remains **NONEXACT**, but improves from
**185/381** to **193/381 comparable ordinary original bytes**.
Nine additional width/font local lifetime variants did not improve it
and were rejected. Selected current-source cold replay still proves the
seven previous exact text owners **3,030/3,030** including the new
**554-byte** main renderer with all 35 target fields. No new exact
credit. [Evidence](evidence/text-render-alternate-stack-scope-20261108/README.md).

## BulletManager native draw-bucket head/tail address words (2026-11-08)

The complete 249-byte `UpdateBullets` now obtains all unchecked
`drawBucketHeads` and `drawBucketTails` slots through a typed low-32-bit
index-times-four helper instead of C++ array indexing. Five distinct
natural `/GL /GS` source variants preserve **249/249 original bytes,
6/6 target-bound fields**, and the separate **40/40 callback / 2 fields**.
The installed-source selected cold replay also protects deactivate and
two position-bounds helpers: **665/665** old exact bytes with **22/22**
original fields. No new exact codegen credit.
[Evidence](evidence/bullet-drawbucket-native-slots-20261108/README.md).

## Bullet manager native 2,000-entry pool cursor and active count (2026-11-08)

The 249-byte complete original `BulletManagerView::UpdateBullets` walks
2,000 entries at **0x7F0-byte** strides. The source now models this as a
typed low-32-bit pointer advance rather than unchecked `++bullet`, and
publishes the unsigned-word increment of the signed activeBulletCount
field in the target's native INC form. Four independent real `/GL /GS`
source variants preserve **249/249** UpdateBullets and **40/40** callback
with all original fields. Five selected already-exact bullet functions
cold-pass **665/665** in two original graph families, no new exact credit.
[Evidence](evidence/bullet-manager-native-pool-cursor-20261108/README.md).

## EnemySpawn native active/spawn counter words (2026-11-08)

Original **577-byte EnemySpawn** now increments the signed manager
`activeEnemyCount` and `spawnCounter` via the original low-32-bit INC
semantics instead of undefined signed C++ overflow. Three separate
current-source 14-TU compiled variants preserve **577/577 original
bytes, 12/12 fields**, while 118-byte manager clear and 28-byte
callback suspension remain exact. Installed-source selected cold replay
across the 14-TU spawn graph and manager-update graph totals **899/899**
old exact bytes with **20/20** target fields. No new codegen credit.
[Evidence](evidence/enemy-spawn-native-manager-counts-20261108/README.md).

## Enemy manager native 32-bit timer increment (2026-11-08)

`EnemyManagerUpdate` now reproduces its original `INC EDX` timer-word
wrap with an unsigned addition published to the signed field. Three
natural-source native `/GL` trials and installed-source selected cold
replay preserve the full **166/166** manager update and **10/10** callback
with **6/6 original-bound fields**, total **176/176**. These are already
exact owners, with no new codegen credit.
[Evidence](evidence/enemy-manager-update-native-timer-20261108/README.md).

## ANM 9,964-byte interpreter terminal vertex word addresses (2026-11-08)

The generated-vertex tail now uses typed low-32-bit `0x1C`-stride Win32
addresses for the second source/destination vertex and V-coordinate
write, replacing unchecked abstract C++ `[1]` pointer formation. Five
separate five-TU original-bound trials and a fresh final-source cold
retain **9,962/9,964** original `ExecuteScript` bytes, all **350** target
fields and only opcode-91 EDX/ECX differences at `+0x113,+0x115`.
Six related existing ANM exact helpers cold-pass **694/694**.
The giant owner is still NONEXACT, no new exact ledger credit.
[Evidence](evidence/anm-render9-terminal-vertex-words-20261108/README.md).

## Memory-file Seek FILE_BEGIN native address word (2026-11-08)

The installed FILE_BEGIN pointer now uses i386 low-32-bit base+offset
arithmetic, keeping the complete 107-byte candidate at **100/107**
original bytes, zero relocations, with seven branch/register mismatches.
Ten selected file-interface exact units cold-pass **491/491**.
[Evidence](evidence/resfile-seek-begin-native-word-20261108/README.md).

## Text main render source stack lifetime recovery (2026-11-08)

`TextHelperView::RenderTextToTexture @0x00437DB0` now scopes its late
rectangle/surface upload naturally, recovering the original **0x14-byte
stack frame** instead of 0x18. Fresh normal-COFF compilation matches
**414/414 ordinary bytes** and all 35 relocation positions. A real
TextFontRelease/TextRenderer/RandomMath **mixed COFF+LTCG** graph
strictly matches the **entire 554/554 original owner, all 35 semantic
target-bound fields**; the all-LTCG graph instead chooses the wrong
EAX receiver and differs at 11 bytes. Six separately canonical text
unit guards pass **2,476/2,476**. **Canonical acceptance complete:**
`text-render-main` has passed two independent current-source cold rebuilds,
**554/554** complete original bytes and **35/35** original fields, alongside
the separate FontRelease exact graph. **+1 authored exact owner, +554 codegen
bytes**; see its linked unit and the two focused receipts.
[Evidence](evidence/text-render-main-stack-scope-20261108/README.md).

## ECL FORMAT native counted text and argument addresses (2026-11-07)

In the full **7,020-byte** `EclVmContext::Run`, the inline FORMAT
scratch terminator, `%` character fetch and counted argument-record
address now use native modulo-32-bit words, not C++ out-of-array
pointer arithmetic. Four real ECL/Enemy two-TU experiments show that
the record-address-only variant regresses **6,997 to 6,996/7,020**, but
the installed three-way combination preserves **6,997/7,020** and all
**189** original field bindings. Eight selected previously exact VM
owners cold-pass **1,081/1,081**. Run still has 23 differing bytes;
no new exact credit. [Evidence](evidence/ecl-format-native-text-addresses-20261107/README.md).



## FileSystem Encrypt native replay shuffle (2026-11-07)

`FileSystem::Encrypt` now uses the existing low-32-bit native byte-offset
helper for both alternating XOR cursor directions, plus unsigned-word
chunk counts and size updates. Four independent normal-COFF trials and
an installed-source complete comparison preserve its **279-byte nonexact
candidate vs original 307 bytes**, including two CRT relocations; its
sibling Decrypt remains 278 vs original 323. Affected existing exact
FileSystem owners retain **760/760** in selected cold replay. No new
exact codegen credit.
[Evidence](evidence/filesystem-encrypt-native-cursor-words-20261107/README.md).



[PROGRESS.md](PROGRESS.md) is generated by `scripts/progress.py` from the
canonical ledgers: **1,829 reviewed candidates, 1,309 source-present mappings,
1,500 complete exact functions / 196,819 original code bytes**, of which
**590 confirmed authored exact / 126,703 bytes**. Source-present authored
**nonexact backlog: 160**. These numbers do **not** establish a working
rebuild, complete original inventory, correct runtime, or source authorship.
Never edit generated totals by hand.

**New canonical exact owner (2026-11-08):** `TextHelperView::RenderTextToTexture
@0x00437DB0` **554/554 bytes** with all **35 independently original-bound
relocation/call fields**. Two source-identical, PE-distinct cold builds of
the *real* TextFontRelease `/GL` caller + normal-COFF TextRenderer +
RandomMath `/GL` graph reproduce the complete function and its original
20-byte local stack frame. A separate seven-unit selected cold retains
**3,030/3,030** exact text bytes including already accepted neighboring
owners; the two-owner second cold passes **703/703**. The already exact
149-byte TextFontRelease uses its own all-`/GL` graph. Canonical unit
`text-render-main`: **+1 exact authored owner / +554 bytes**.
[Acceptance proof](evidence/text-render-main-stack-scope-20261108/README.md).

**New canonical exact owner (2026-11-07):** `SoundPlayerView::LoadBgm
@0x0043D950` **298/298 bytes**, all **12 independently original-bound
fields**. Two separate native VC7.1 `/GL /GS /EHsc` cold builds in the
real Sound+ZWave `ProcessQueues` graph, alongside 1,452/1,452
`ProcessQueues` and 55/55 `ReopenBgm`, passed **1,805/1,805** bytes
each. Canonical `sound-load-bgm` admitted to `config/matches.csv` and
the reviewed source inventory: **+1 owner, +298 exact authored bytes**.
[Acceptance proof](evidence/sound-load-bgm-format-lifetime-20261107/README.md).

These are the latest relevant **whole-owner** baselines, not acceptance:

| Original target | Original / source PDB bytes | Current evidence and unresolved action |
| --- | ---: | --- |
| ANM `ExecuteScript @0x0043EE30` | 9,964 / 9,964 | **9,962/9,964 original ordinary bytes, all 350 original-bound fields**; opcode 91 at `+0x113,+0x115` uses EDX in target, ECX candidate. Fourteen-/five-TU contexts differ. Seventeen new local register/lifetime forms failed. [Current vertex-index word proof](evidence/anm-render9-native-vertex-index-20261107/README.md), [earlier mode-9 proof](evidence/anm-render9-native-vertex-words-20261107/README.md), [original opcode-91 frontier](evidence/anm-variable-stream-native-address-20261107/README.md). |
| ECL `Run @0x0044E1A0` | 7,020 / 7,020 | **6,997/7,020 original bytes, all 189 target-bound fields** in real ECL/Enemy **two-TU** graph; single-TU context can be **6,996/7,020** without a regression. Remaining 23 bytes span FORMAT register/stack homes, LENGTH_SQUARED, POINT_ANGLE, loop/callback. Some `Pop` unsigned pointer forms regress whole Run despite leaf exact. [Current FORMAT native-address proof](evidence/ecl-format-native-text-addresses-20261107/README.md), [FORMAT evidence](evidence/ecl-format-argument-index-wrap-20261107/README.md), [Pop rejection](evidence/ecl-stack-push-native-word-20261107/README.md). |
| Enemy ECL `DispatchEclInstruction @0x0040E770` | 14,416 / 14,352 | 64-byte size gap, **716 candidate fields only**; not 716 independently target-bound links. Fresh 11-TU source/field graph, 87/87 related exact operand guards. [Current spell-name proof](evidence/enemy-ecl-spell-name-native-words-20261107/README.md), [STOSD proof](evidence/enemy-ecl-pattern-clear-stos-word-20261107/README.md), [earlier pattern/spawn](evidence/enemy-ecl-spawn-pattern-index-words-20261107/README.md). |
| `EnemyRuntimeUpdate @0x0040DC80` | 2,412 / 2,371 | 69 **candidate** fields; native life SUB, flash DEC and ANM script ADD source semantics installed. Its independently compiled 593-byte target / 606-byte source callback now has native 32-bit countdown and timeout words, still nonexact. [Callback proof](evidence/enemy-callback-native-arithmetic-20261107/README.md). PDB candidate unchanged, not exact. [Script word proof](evidence/enemy-runtime-animation-script-word-20261107/README.md). |
| `SpawnSingleBullet @0x004067D0` | 1,413 / 1,504 | 44 **candidate** fields; odd-angle index, both 0x7F0-byte pool advances, ANM/effect-script words installed. Nine seven-TU source probes leave entire candidate unchanged; canonical SpawnBulletPattern194 must be replayed in its **different** correct graph. [Final source](evidence/bullet-spawn-script-words-20261107/README.md). |
| ECL `StartSubroutine` | 550 / 551 | Four candidate call fields; native counted metadata, stack addresses and signed CALL argument-counter word increment restored. Remains nonexact. [New argument-counter proof](evidence/ecl-call-argument-index-wrap-20261107/README.md). [CALL proof](evidence/ecl-start-subroutine-native-argument-address-20261107/README.md). |
| ECL `AddScriptData @0x00450220` | 589 / 589 | 2 original CRT call fields; **502/581 comparable ordinary bytes** (510/589 including eight linked operand bytes). 79 ordinary mismatches; native SCPT pointers and merge counter words installed. [Native sorted-record mover](evidence/ecl-sorted-record-native-copy-20261107/README.md), [SCPT](evidence/ecl-serialized-script-native-address-20261107/README.md), [merge counters](evidence/ecl-record-merge-counter-native-20261107/README.md). |
| `TransitionShakeEnvelope @0x0043C710` | 349 / 350 | **13/16 original field positions**; 16 referents known, three position/register errors and 1-byte extent gap. Do *not* repeat the old incorrect 14/16 claim. Selected four related exact callbacks protect 689/689. [Envelope proof](evidence/transition-envelope-shared-second-limit-20261107/README.md). |
| Bullet `UpdateAbsoluteDirectionChange @0x004078E0` | 335 / 335 | **333/335**, all **8 original-bound fields**; EDX/EAX difference at `+0xD7,+0xDD`. Recent 12 argument/struct variants failed. [Earlier proof](evidence/bullet-direction-counter-wrap-20261106/README.md). |
| Sound `PreloadBgm @0x0043D7D0` | 369 / 369 | **365/369**, all **10 original fields**; request-size/output-count register assignments in ReadFile. 23 read-call and nine format-index variants failed to close; [current native stride](evidence/sound-preload-native-format-stride-20261107/README.md). |
| `CMemoryPbgFile::Seek @0x004364F0` | 107 / 107 | **100/107**, no relocations; seven branch/register bytes still differ. Native FILE_BEGIN/CURRENT/END/remaining 32-bit word addresses installed. [Latest FILE_BEGIN proof](evidence/resfile-seek-begin-native-word-20261108/README.md), [prior CURRENT proof](evidence/resfile-seek-remaining-native-word-20261107/README.md). |
| Game stats constructor / factory | 132 / 137; 97 / 98 | Calling convention/real caller still differs (original constructor EAX/RET). Real six-TU statistics loader/finalizer/tail exact guard 1,129/1,129. [Real caller](evidence/statistics-record-real-caller-20261107/README.md). |
| `InitializeGameStatisticsRecord @0x0042ACB0` | 274 / 274 | **7/8 original field offsets**, eight real identities; second name-word `+0x50` vs original `+0x53`, still NONEXACT. Old fixed-mask diagnostic differences are *not* a canonical score. [Copy proof](evidence/statistics-record-name-copy-schedule-20261107/README.md). |

Further genuinely open large owners (Replay `Initialize`, PollInputFrame,
laser collision box/circle, camera publication, LoadSoundSample, ending and
stage orchestration) are indexed in
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Before acting on a dated proof,
**rebuild or verify its full source/header graph**: isolated sizes can differ
from integrated canonical graphs even when source is unchanged.

## Player shot option table continuation

`PlayerShotSetOptionOrigin @0x00428C20` now addresses unchecked
signed-byte `sourceIndex` records through a native modulo-32-bit
`0x98`-stride word, and `Enemy.hpp` explicitly includes the ANM vector
layout that its assertions use. Seven source variants preserve the
**122-byte normal-COFF candidate vs original 125**, still **NONEXACT**.
Current-source selected exact PlayerShot 53+80 and ECL host 146
replay **279/279** original bytes in three scoped artifacts. No new
exact credit. [Evidence](evidence/player-shot-option-native-word-20261107/README.md).

### Enemy interpolation timer source word checkpoint

The original 652-byte `EnemyScalarInterpolationView::Evaluate` executes
`INC EDX` and `DEC ECX` for its 32-bit timer word. The maintained inline
`EnemyInterpolationTimerView` now uses native wrap arithmetic in `Tick`
and `SetCurrent`. Selected original-bound cold replays preserve **964/964**
bytes across five previously exact Enemy functions. The separately linked
EnemyRuntimeUpdate (2,371 candidate / 2,412 original) and callback
(606 / 593) remain NONEXACT, with zero candidate ordinary differences and
unchanged candidate field geometry. [Evidence](evidence/enemy-interpolation-timer-words-20261107/README.md).

## GDI text format and DIB native word arithmetic (2026-11-07)

`FindTextRenderFormat` now traverses the unchecked 0x18-byte format
records using the established typed Win32 word-address helper.
`TextRenderBufferView::TryAllocateBuffer` now preserves modulo-32-bit
width/bit-count multiplication, height negation and image-byte product
while retaining the original signed pitch divisions and DIB semantics.
Six independent real two-TU native trials and final installed-source
cold replay preserve **2,327/2,327 original bytes** across five exact
text owners (including **781-byte CreateBuffer and 946-byte alpha-bleed**).
The 157/554/509-byte neighboring owners remain nonexact, with unchanged
current comparable scores. No new exact credit.
[Evidence](evidence/text-format-row-native-words-20261107/README.md).

## Native Sound command shift cursor (2026-11-07)

The 1,452-byte `ProcessQueues` exact owner now uses typed native 32-bit
address words for both 0x10C-byte record cursor increment and one-next
`memcpy` source in its command removal loop. Five real Sound/ZWave
full-extent trials and independent current-source original-bound cold
replay retain **1,452/1,452 / 43 target links**; selected Sound guards
**2,034/2,034**, WAV guards **178/178**. The new `LoadBgm` complete
**298/298 / 12 target fields** was still a pre-admission candidate
at this cursor-change checkpoint; it was admitted after two separate
canonical cold builds. [Evidence](evidence/sound-process-queue-native-shift-20261107/README.md).

## BGM format and preloaded table native indices (2026-11-07)

The previously exact **55-byte** ReopenBgm now uses a typed Win32
low-32-bit `0x34`-stride BGM format row address. The separate original
**298-byte** LoadBgm (304-byte candidate) uses native indices into the
filename and four preload tables, without adding a range check. Seven
complete two-TU native source trials preserve ReopenBgm **55/55**,
ProcessQueues **1452/1452**, the unchanged LoadBgm owner and PreloadBgm
**365/369** (four ordinary differences, original fields intact).
Installed-source selected cold replay: **2212/2212** already exact Sound
bytes in six units/four artifacts, no new exact credit.
[Evidence](evidence/sound-bgm-native-index-words-20261107/README.md).

## Sound sample loader native address recovery (2026-11-07)

`LoadSoundSample` has unchecked sound-buffer/pending-data indices, RIFF
byte cursors and split PCM data offsets. Typed Win32 word-address helpers
now express these while preserving the complete five-TU current source
owner at **684 bytes / 578 of 584 ordinary original bytes** and **25
candidate fields** (still nonexact, six ordering bytes). Seven independent
full parent trials, plus an extended installed-source original-bound replay:
**2,212/2,212 exact bytes across six existing Sound units**. No new
codegen exact credit. [Evidence](evidence/sound-sample-native-address-words-20261107/README.md).

## Sound command stage native increments (2026-11-07)

`SoundPlayerView::ProcessQueues` now preserves native 32-bit signed-stage
wrap for four command increments and its RELEASE retry decrement. Six
natural-source current-epoch real two-TU native experiments preserve the
complete original **1,452/1,452 bytes and 43/43 field bindings**. The
*committed*, reformatted current `src/Sound.cpp` was independently cold
replayed: **2,034/2,034** original bytes in four existing Sound exact
owners. No new exact credit. [Evidence](evidence/sound-process-queue-native-step-words-20261107/README.md).

## Current 2026-11-07 source-only closure since the older checkpoint

- **Enemy ECL giant (14,416 target / 14,352 native)**: direct original
  disassembly confirms `REP STOSD` at `+0x19DE` for 132 pattern words and
  `8A 0C 31` at `+0x273B` for the spell-name cipher. The maintained
  32-bit pattern cursor and counted name input/output address words were
  checked in a source-pinned eleven-input native graph. Five complete PDB
  candidate owners retain their prior ordinary bytes and field layouts,
  and selected independently original-bound argument leaves pass **87/87**.
  Three resource-index, three VM-index, and three combined name/VM forms
  change the cipher's SIB to `8A 0C 0E`, contrary to the original; **none
  were installed**. See [pattern](evidence/enemy-ecl-pattern-clear-stos-word-20261107/README.md)
  and [name](evidence/enemy-ecl-spell-name-native-words-20261107/README.md).
- **ANM giant (9,964 / 9,964)**: render-mode-9 script count decrement and
  two vertex steps now use native wrapping address words. A complete
  current-source five-input original-bound native comparison remains
  **9,962/9,964**, **350/350 target fields**, with only the opcode-91
  two-register frontier. Three selected exact ANM guards pass **164/164**.
  See [mode-9 evidence](evidence/anm-render9-native-vertex-words-20261107/README.md).
- **Existing exact sound-queue leaves**: typed native metadata/cue/sample
  slots preserve **123/123** and **149/149**, all four original-bound fields
  in one current-source cold graph. See [ID words](evidence/enemy-sound-queue-native-words-20261107/README.md)
  and [sample words](evidence/enemy-sound-queue-native-sample-words-20261107/README.md).
- **Rejected focused controls, not progress**: 13 natural Seek-tail forms
  remained **100/107**; 8 new Run-advance variants and 7 Pop-address forms
  failed to improve **6,997/7,020** ECL original bytes. Retain the
  original-target instruction witnesses before revisiting; do not promote
  these uninstalled trials. No historical full replay was run.

The current ledger totals remain unchanged: these are native behavioral
source recoveries and strict regression guards, **not new exact owners**.

## 2026-11-07 current-source recovery checkpoints (no new exact credit)

Five source/evidence checkpoints and a separate evidence note after
`a30249b` reconstructed **native low-32-bit address arithmetic**, in both
large and small owners, without promoting any nonexact owner:

- Enemy ECL `DispatchEclInstruction` **14,416 target / 14,352 source PDB**:
  the FIRE_BULLET_PATTERN offset vector, pattern initialization, and
  opcode 0x199 overlapping extra-index and floatB store now use native
  unsigned word calculations, not C++ signed overflow. Full fresh current
  11-TU producer **retains its prior candidate** (all five observed owners
  identical); five selected original-bound ECL argument leaves **87/87**.
  [Fire offset](evidence/enemy-ecl-fire-offset-word-20261107/README.md),
  [extra words](evidence/enemy-ecl-remaining-pattern-word-20261107/README.md).
- ECL `Run` **7,020 source/target, 6,997/7,020 original bytes and all
  189 original-bound fields**: `OperandWord` replaces one-element C++
  variable-stream indexed pointer arithmetic in the int/float readers;
  eight affected prior exact owners **1,037/1,037**.
  [Proof](evidence/ecl-operand-word-address-20261107/README.md).
- `CMemoryPbgFile::Read` / `Tell`: both already exact, native 32-bit
  subtraction/advancement recovered. Ten affected file units **491/491**
  selected original bytes; Seek remains **100/107 nonexact**.
  [Proof](evidence/resfile-native-read-tell-words-20261107/README.md).

Fresh real 2-TU ECL Run experiments swapping the length-squared expression
(4 source variants) either retained **6,997** matching bytes, regressed to
**6,995**, or changed the whole owner extent; none were installed. Three
fresh FORMAT `OperandWord` source variants all regressed to **6,996/7,020**
(from 6,997), despite restoring some SIB bytes; they were also rejected.
Private trial scripts and complete linked artifacts are under
`.analysis/gpt-web/20261107-session-th10/` and `build/gpt-web/`.
**Do not mistake individual new C++ well-definedness changes for increases
in the canonical exact scoreboard. No full historical replay was run.**

- Sound `FindWaveChunk` remains **71/71 exact** after recovering native
  32-bit unchecked RIFF cursor arithmetic. The actual **684-byte**
  `LoadSoundSample` parent still differs in exactly six ordinary bytes at
  `+0x122..+0x127` (MOV/LEA schedule); 25 candidate code fields are
  not original-bound promotion. Four selected Sound exact owners pass
  **1,819/1,819**. [Current proof](evidence/sound-riff-native-cursors-20261107/README.md).

- ECL `Run` FORMAT scalar indexing now uses original low-32-bit
  byte-stream addresses for both float/int records instead of out-of-bounds
  one-element C++ array indexing. Six complete native variants: five
  rejected at **6,996/7,020**, one selected at **6,997/7,020** with all
  **189 original-bound fields**. Eight affected exact units cold-pass
  **1,037/1,037**. [Proof](evidence/ecl-format-native-scalar-byte-address-20261107/README.md).

- ECL `StartSubroutine` now reads both types of counted CALL payload
  through the native typed `OperandWord` helper instead of indexing a
  one-element C++ array. Five native source variants compared; a raw-int
  alternative regressed **76** ordinary candidate bytes and was rejected.
  Installed source retains the **551-byte** nonexact CALL candidate and
  7,020-byte Run at **6,997** with all 189 target fields; nine selected
  original-bound exact ECL units pass **1,161/1,161**.
  [Proof](evidence/ecl-call-scalar-word-address-20261107/README.md).

- ECL `StartSubroutine` CALL scalar payloads now use `OperandWord`
  for native low-32-bit counted-stream addresses in both tag branches.
  Current **551-byte candidate vs 550-byte target** keeps four candidate
  fields unchanged; **nonexact**. Whole original-bound `Run` remains
  **6,997/7,020**, all **189** target fields; nine ECL exact guards
  **1,161/1,161**. [Proof](evidence/ecl-call-scalar-word-overlay-20261107/README.md).

- Enemy ECL opcode pattern initialization/aim-mode and rank-interpolated
  count stores now share native modulo-2^32 index/address arithmetic.
  Source-pinned full **11-TU** dispatcher remains **14,352 candidate vs
  14,416 original**, four neighboring owners unchanged; five selected
  original-bound ECL argument leaves **87/87**. Giant owner NONEXACT.
  [Current proof](evidence/enemy-ecl-pattern-index-helper-20261107/README.md).

- Bullet `AdvanceTransformProgram`, original **1,454 bytes**, is still
  NONEXACT (normal-COFF current candidate **1,540 bytes / 32 relocations**).
  Native word addresses now handle the script's signed record index,
  second child pattern record and sprite script arithmetic. Five fresh
  normal-COFF and five combined trials; installed code preserves all
  1,540 candidate bytes and 32 resolved reloc referents. Three selected
  original-bound Bullet units cold-pass **1,339/1,339**.
  [Proof](evidence/bullet-transform-native-record-words-20261107/README.md).

- Enemy ECL full pattern copy, rank-speed interpolation and 12-byte
  offset-vector indexing now use native unsigned i386 cursor words via
  `EnemyPatternAtRuntimeIndex`/`EnemyPatternOffsetAtRuntimeIndex`.
  Fresh whole current eleven-TU compilation preserves **14,352** candidate
  bytes and all 716 candidate address fields, with four neighboring owners
  unchanged; five original-bound Enemy argument leaves **87/87**. Original
  14,416-byte dispatcher is still NONEXACT.
  [Evidence](evidence/enemy-ecl-typed-pattern-array-words-20261107/README.md).

- Enemy ECL spawn opcode families now use a native 32-bit byte-stream
  helper for all **28** scalar operands following counted inline names.
  Fresh current-source 11-TU complete dispatcher candidate remains
  **14,352 bytes** with 716 candidate field positions and four neighboring
  owners unchanged; five original-bound ECL leaves pass **87/87**.
  Original **14,416-byte** dispatcher still NONEXACT.
  [Proof](evidence/enemy-ecl-variable-spawn-scalar-words-20261107/README.md).

- Fresh current-source **five-TU** ANM `ExecuteScript` 9,964-byte
  original-bound comparison (350 original fields) still passes
  **9,962/9,964**. Seven new natural opcode-91 source variants failed to
  recover the EDX layer register: five byte-neutral, one to **9,960**,
  one rejected with a 10,044-byte PDB extent. All temporary source changes
  restored. [Scope and source pins](evidence/anm-opcode91-current-structure-20261107/README.md).

- `ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY` now uses i386 native
  32-bit address addition for both speed stores. Fresh eleven-TU candidate
  dispatcher remains **14,352 bytes** with five complete owners unchanged;
  **original target 14,416** is NONEXACT. Five selected exact ECL argument
  leaves pass **87/87**. [Proof](evidence/enemy-ecl-difficulty-speed-address-word-20261107/README.md).

- ANM `TickScriptTimerScale` now advances its signed script timer as a
  native unsigned 32-bit word (no pointer alias or signed overflow). Current
  five-TU source **9,962/9,964** `ExecuteScript` original bytes, **350/350**
  target-bound code fields, and selected ANM guards **876/876**. Original
  opcode-91 two-register byte frontier is still open. [Evidence](evidence/anm-script-timer-native-counter-20261107/README.md).

- Bullet `SpawnSingleBullet` now computes all five script type/color
  table accesses through a native typed low-32-bit address-word helper.
  Current 7-TU **1,504 candidate bytes** and four neighboring owners are
  unchanged (44 candidate spawner fields); original **1,413-byte** target
  remains NONEXACT. Five selected original-bound Bullet guards pass
  **1,028/1,028**. [Proof](evidence/bullet-spawn-native-table-index-20261107/README.md).

- Enemy ECL now also expresses six unchecked typed gauge/drop/slot/timeout
  addresses as native 32-bit words. Fresh rebuilt eleven-TU current candidate
  still 14,352 bytes/716 candidate fields, four neighboring owners
  unchanged; five original-bound ECL leaves pass **87/87**. Target
  dispatcher 14,416 is NONEXACT.
  [Proof](evidence/enemy-ecl-native-unchecked-index-20261107/README.md).

- ECL `FindSubroutine` now uses a typed native dword word-address
  helper for its eight-byte binary-search records. Installed source exact
  leaf **136/136**, full `Run` **6,997/7,020** and all **189 target-bound
  fields** unchanged; four selected exact units **575/575**. Five other
  attempted `AddScriptData` old-count forms grew the 589-byte owner to 605
  and were rejected. [Proof](evidence/ecl-native-subroutine-record-index-20261107/README.md).

- `EclVmScriptDatabase::AddScriptData` now uses native 32-bit file-slot
  word addressing across its unchecked SCPT file table. Installed original
  **589-byte** owner still 502/581 comparable ordinary bytes, nonexact;
  `FindSubroutine` **136/136**, full `Run` **6,997/7,020** with **189**
  original fields, and four focused exact guards **575/575** preserved.
  [Evidence](evidence/ecl-scpt-native-file-slots-20261107/README.md).

- ECL `AddScriptData` SCPT table slot reads/writes now use native
  32-bit file-index words; complete candidate **589**, original ordinary
  **502/581**, 79 differences; original Run **6,997/7,020** (189 target
  fields). Four selected ECL exact leaves cold-pass **575/575**.
  [Proof](evidence/ecl-scpt-file-slot-native-words-20261107/README.md).

- Enemy ECL `SET_ANM_SCRIPT` now uses native `EnemyNativeWordIndex` for
  five script-provided VM ID slot accesses. Fresh current-source 11-TU
  **14,352 candidate** with four unchanged neighbor bodies; original
  **14,416 NONEXACT**. Selected exact ECL argument leaves **87/87**.
  Combined ANM-resource + managed-ID codegen regressed one byte and was
  rejected. [Evidence](evidence/enemy-ecl-anm-managed-slot-words-20261107/README.md).

- ECL `AddScriptData` sorted-record header/name writes and traversal
  now use native 8-byte low-word pointer steps. Current two-TU complete
  AddScriptData **589** with **502/581** original ordinary matches,
  original `Run` **6,997/7,020** with all 189 target-bound fields;
  selected four ECL exact guards **575/575**. The record-copy candidate
  regressed **439/581** and was rejected. [Proof](evidence/ecl-scpt-sorted-record-word-20261107/README.md).

## Already accepted exact examples and good guards

- **Large owners:** `ReplayManager::SaveReplay` **1,681/1,681** (83 bound
  fields) [proof](evidence/replay-save-1681-exact-20261106/README.md);
  `EndingTextVmView::Run` **1,568/1,568** (92 fields)
  [proof](evidence/ending-run-1568-exact-20261106/README.md);
  `SoundPlayerView::ProcessQueues` **1,452/1,452** (43 fields);
  `GameFinalizeStatistics42B1E0` **564/564** (24 fields).
- **Leaves and callbacks:** `TransitionFadeIn` **133/133**, 5 fields and
  two independent 14-TU colds
  [proof](evidence/transition-fade-in-133-exact-20261107/README.md);
  `TransitionDelete` **26/26**, authorship indeterminate;
  Sound `GetFmtIndexByName` **227/227**, four fields
  [proof](evidence/sound-getfmt-record-native-address-20261107/README.md);
  `SpawnBulletPattern` **194/194** (six fields in its **canonical** graph)
  [proof](evidence/bullet-spawn-pattern-194-exact-20261106/README.md).
- Recent source-epoch *regression* receipts, **not new credit**: selected
  Bullet **1,028/1,028** (5 units), Sound **2,261/2,261** (5 units), ECL
  **1,000/1,000** (7 units), Enemy **570/570** (5 units). Source pins and
  unit selectors are in their linked evidence; always replay the affected
  subset from the installed source, not an older private image.

## Next useful experiments

- For ANM opcode 91, investigate a new **register-liveness cause** for
  EDX vs ECX across its real `CreateVmVariant2` call; avoid already failed
  `register`/volatile/aggregate/source-order aliases. Full 9,964 bytes,
  350 target fields, and all local tables must stay valid.
- For ECL Run, derive the stack-home schedule for FORMAT and x87 expression
  paths from target/current instruction pairs. Rejected `Pop` pointer-word
  variants can leave Pop122 exact **but lower Run to 6,950/7,020**; never
  accept a leaf change without a full parent comparison.
- For Enemy ECL and SpawnSingleBullet, **do not** interpret candidate-field
  counts as original-bound acceptance. Seek an actual control/data-flow
  recovery, not endless byte-neutral unsigned rewrites.
- A focused attempt to resolve AbsoluteDirection's two register bytes,
  PreloadBgm's four, or Seek's seven is worthwhile; save any failed full
  variants under ignored `.analysis/gpt-web/` with original/source pins.

## Current-source proofs and candidate epoch pins

These are **installed source bytes**, not historical artifacts. The
lightweight `scripts/repo-python scripts/verify-handoff-pins.py` checks each
file's SHA-256 and the target; use it before relying on cached native PDBs.
All values were reverified at checkpoint `6f1b83c`.

| Installed source | SHA-256 |
| --- | --- |
| `src/GameStatistics.cpp` | `dc31f5c888d68d363571a74457ed3da000c2a36386e790cd9234b0798c229666` |
| `src/FrontEnd.cpp` | `0bab7e7448c0762a1410d627301652bfc8cfe22e7aaa88b76e979151cdaaf8c6` |
| `src/AnmManager.cpp` | `4e5cbaad2538269266088d16671a880cae13a4a131fc4f264b0e2c2a5abb29e9` |
| `src/AnmManager.hpp` | `7baea2b709d535639ab5a3d44a8bb1adb6eefd9c26b70f50a89fc248c5ce0bc3` |
| `src/EclVm.cpp` | `62cbc480c658beec6fca953680cb1267faf546d6dcaf3d19d0937472f4c74766` |
| `src/EclVm.hpp` | `96764231991c72ff5eebb2d56884fbaafef6406d87d3a751cb53cbf96679d1fc` |
| `src/Ending.cpp` | `ed016273f068605e633f3faaff6a89556c953ec3a30b724ec52b8c54706a23ff` |
| `src/ResFile.cpp` | `5784637d77b9bc502f5ad4157aa1f199355a17668cb8a056c43a50dbe4541126` |
| `src/EnemyEclDispatcher.cpp` | `cebadf0dd493c970699b98b371ebd9792d1dd3c51c26fe49cdc591df0e0d581f` |
| `src/Enemy.cpp` | `7fbc4b94ea173a05bdea370e0eaf6660fb5855de7ebdfa8281642dc200a068b7` |
| `src/Enemy.hpp` | `372ef4e3ca68687434092efa8b50573ef6ebcf8aa0fc0bb2e14058d60b85f668` |
| `src/EnemyLaser.cpp` | `7d5f898f3fe4718ec4d44637b71883f300cd9493771ff2296538baf986adead7` |
| `src/EnemyLaserType1.hpp` | `dfd675f59897f541739888e99067a4306fcbe67b6773ec184b8baca90984facc` |
| `src/LaserType1.cpp` | `b7f759886f4897a9c1e2cd8fb99007f045e932d7a3826417ecd194f9006ead51` |
| `src/ItemManager.cpp` | `b029539ab91eba926525a1de3dcb6baaa05d3b6c53e181702b325b8a4ad32fcc` |
| `src/BulletTransform.cpp` | `05a488b848b78fd97561676973f2e02619d7e28fe403150e59da1e55cf309e24` |
| `src/BulletManager.cpp` | `2050ed5197807702de5f250db77d6e37358a612cdec323849baaab1d786c7c35` |
| `src/Sound.cpp` | `977faf3e332e03a8d1ef3200a1cfd047147c464b6b9f295d243e4aed5ce2bc02` |
| `src/StageCameraDefaults.cpp` | `4ac0d9c1eeb14e1763f5ed642e50895e5081fdf2f41dcda819a6ca7ee7255fb8` |
| `src/StageScript.hpp` | `3a1a5402fdd969c150844102680d007f1080f6e38e44f429e70f0594b537bf5b` |
| `src/GameStartupVersion.cpp` | `125b80a073015e9d0fd2694b989a0983bf3f3777fb29634825818ced0f125b4f` |
| `src/GameStatisticsLoad.cpp` | `788036ed9a8881282db93f878bb2b899dd949b3c268bd45f5a7591e402650ce8` |
| `src/GameStatisticsRecordDefaults.cpp` | `2c0c4d290118b9a65a781fef030825b07b89dde090a24cbc1252c200cf3173df` |
| `src/InputPolling.cpp` | `f3bb9e5230f9db7e141e187b054e9bbd68d1282c7cd27f02723176f4415d641f` |
| `src/ReplayManager.cpp` | `7c6a50d945a9c83f694bda661880d9211b29b1970c99a8a4ddd4844dab6fcaf3` |
| `src/TextRenderer.cpp` | `f016d9482e8d94015c6dc18ffc1123407edc68e7b4c355b057c6448924d51ddb` |
| `src/TransitionEffect.cpp` | `711fb19a08a7bdfa45cd600a8f3bb82af5d2c90f3292d214e91e63e3625ebcd8` |

## Private evidence, transfer and storage

The original EXE, VC7.1/Wine `.tools/`, Ghidra database, `_reference/`,
`.analysis/` native source snapshots, archives and current PDB/PE **are
ignored and do not travel with Git**. Transfer these separately if the
next agent uses a different machine. Preserve **all** `build/match-linked/`
canonical graphs, original inputs, private toolchain/database and the
compressed `.analysis/archive/` registry. Regenerable rejected variant
binaries under `build/gpt-web/` may be pruned after recording a manifest
and retaining their small `*.json`/trial source evidence. Consult the
separately maintained [storage registry](RE_ARTIFACTS.md); old source
hashes do not validate a restored PDB. Do not cold replay all units for
handoff/doc/storage maintenance: run `scripts/repo-python scripts/ci.py`
and `git diff --check` instead. **Never push.**
