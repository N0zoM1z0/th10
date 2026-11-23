# TH10 reconstruction knowledge

Durable findings for the byte-exact VC7.1 SP1 reconstruction of Japanese TH10
v1.00a, organized by topic. Exact status lives in `config/matches.csv` and the
generated [PROGRESS.md](PROGRESS.md); current work and commands are in
[RE_HANDOFF.md](RE_HANDOFF.md). This file replaced a 14,000-line dated journal
on 2026-11-15. The journal and the per-topic reports it absorbed remain in Git
history before that date, if an old experiment needs its full detail.

Addresses are target virtual addresses. Offsets written without `0x` in
compact lists are hexadecimal. A measurement such as `660/668` means candidate
versus target size unless it says ordinary bytes or normalized instructions.

## Codegen lessons (VC7.1 SP1, mostly `/GL`)

### Graph completeness and private ABIs

- LTCG chooses private register ABIs only when it can see both the real caller
  and the real callee. A callee whose TU is missing becomes an anchored
  external with a stack ABI, and the caller changes with it. Build the real
  producer/consumer graph instead of declaring an ABI. Examples:
  `AnmVmInitialize` in `src/AnmVmInitialize.cpp` (missing, it adds two
  `xor ecx,ecx` to `AllocateVm @0x449950`); `GuiMessageLeaves.cpp` and
  `VmIdInterrupt.cpp` for `GuiMessageVmView::Run`; `AnmVmId.cpp` for the result
  menu's `SetInterrupt`; Chain.cpp for the transition factory `@0x43C8B0`
  (EBX priority, five stack arguments, RET20); the real `GameErrorContext::Fatal`
  for `InitD3DInterface @0x439700`.
- Visibility is necessary but not sufficient, and adding support can move a
  parent the other way: real AnmManager.cpp makes `SetInterrupt` exact but grows
  `ResultMenuUpdateResult`. Sizes measured in different graphs are not
  comparable.
- The /GL entry symbol matters. Rooting at the real caller gives the target
  receiver (`EclVmContext::Run` takes EAX when rooted at `EclVmHost::Run`, ECX
  when it is the entry). Screen creators are exact only rooted at
  `AnmVmView::Initialize`; `FindVm @0x4491C0` only through `MarkVmForDeletion`.
- TU placement changes inlining. Moving `CreateVmVariant0` into
  `src/AnmVmCreate.cpp` made all four 73-byte siblings exact; HintLoad in its
  own TU fixed `HintOwnerView::Initialize @0x418E30`. Pulling a whole legacy
  leaf TU into a real graph causes LNK2005; move the single body to its caller's
  TU instead.
- Profiles are per TU. `RenderTextToTexture` is exact only in a mixed graph
  (TextFontRelease `/GL`, TextRenderer normal COFF). Enemy.cpp uses its own
  profile; uniform `/GL /GS /EHs` broke ScriptViewer Update. `/GS` is required
  wherever the target has a cookie (Enemy dispatcher, result menu). `/EHsc` or
  `/EHs` is needed where the target has an FS:[0] frame (`new T[n]`,
  destructors of members with destructors). `/Os`, `/Op`, `/Og-` and global
  `/Ob1` all broke other owners.
- Calling convention follows from callee cleanup: if the caller has no
  `ADD ESP`, declare the callee `__stdcall`; LTCG may then still move its first
  argument into a register (`AnmSetVmScriptIndexAndExecute @0x4496D0`,
  `FileSystem::Encrypt`, `MainLoadConfiguration @0x420870`). `__fastcall` gives
  ECX/EDX and plain RET. Member functions give callee cleanup plus a this
  register where a free function emits caller cleanup (`SpawnEffectRow
  @0x427B50`).
- Formal parameter order selects private registers: `(int index,
  Controller*)` gives EAX controller/ECX index; reordering registrar formals
  closed `TransitionEffectView::Initialize @0x43C970`. Count-first argument
  order fixed `GameAddLives @0x4188A0`.
- Natural members recover private receivers: `AdvanceMotion()` as a member of
  the 0x2C-byte motion record gets ESI; a plain const member gives
  `GetPlanarBounds @0x4285F0` its register-only ABI. Membership can also hurt:
  `HintLinkView::Append` is 52 bytes as a member and 40 as a free function.
- Parameter lowering: `MergeJoystickInput`'s first parameter reaches ECX only
  when the body copies it into a local before taking an address. An
  address-taken parameter is never register-lowered.
- Return the natural type: void when no caller consumes it (`AnmVmTimerView::Tick`
  with int adds `MOV EAX,EDX`), the merged `unsigned short` for
  `PollInputFrame`.
- Keep real helper boundaries. A 9-byte `GuiMessageInstructionNext` is its own
  PDB owner; inlining it by hand removes an owner. `AnmVmTimerView::Add` must
  stay a call (WAIT wrapper 20 bytes). Explicit `noinline` on real definitions
  and a scoped `#pragma inline_depth(0)` around one call site have been
  accepted where the target keeps a call (`ReadDecompressEntry @0x434DD0`,
  `CSound::FillBufferWithSound @0x44D110`); blanket noinline was wrong.

### Lifetimes, reloads and store order

- Reread through pointers after any opaque or mutating call when the target
  reloads (ECL scale after reset, shot descriptors after callbacks, textures
  after `FlushVertexBuffer`, the opcode after operand reads). Conversely hold
  one element pointer across calls when the target reads the table once per
  iteration (`Release @0x447810`).
- Read a field once into a typed local and reuse it when the target keeps it in
  a register; split a published value from a destructive cursor that shares
  storage (`PollInputFrame`).
- Reread a just-published field instead of the source global
  (`ResultMenuSynchronizeScore @0x421F60`). Reread the manager after a call
  that the target treats as opaque (`CreateConfiguredAnmVm`).
- Write through the same global aggregate and member retail uses. The Chain
  runners need `g_MainSupervisorView.criticalSections[0]` and
  `criticalSectionLockCounts[0]` (0x491C28 +64C/+6F4), not scalar globals.
  `g_GameScoreState` (0x474C40) must be one escaped struct: separate globals get
  CSEd across calls (`UpdateSpellPanel`). `g_MainSoundOwner` is an object, not a
  pointer.
- Initialize a returned ID directly (`AnmVmIdView id = manager->AddVmVariant0(vm);`)
  instead of assigning into a default-constructed one; receive by-value results
  into a fresh local.
- Statement order only partly controls store order. Branch-local writes narrow
  lifetimes; a local reference to an embedded link gives self-link-first order
  (`HintEntryView @0x418D00`); assigning `element->arg` before `flags |= 2`
  matched `MainRegisterChain @0x420470`. Moving zero-initialization ahead of a
  search fixed register coloring (`GetFmtIndexByName @0x43D2A0`).
- Inline pointer helpers block store sinking and CSE where the target keeps
  separate computations (ItemManager Draw's playfield-to-screen helper,
  Player hit-effect position).
- Scope late locals tightly to shrink the frame (`RenderTextToTexture`: 0x18 to
  0x14). Model stack temporaries with their true size and as scalar-field
  structs rather than array unions, because `/GS` reorders array-containing
  locals.

### Control flow and block order

- Physical block order follows lexical order: success-first `if`, separate
  early-return guards, case clauses in the target's physical order (jump-table
  mapping is unchanged). Reverse `if/else` versus `goto` is usually identical.
- Use `switch` for a small selector cascade (`SUB EAX,0; JE; DEC EAX; JE`); VC7.1
  folds duplicate case tails. A ternary between two calls compiles branchless;
  write `if/else` when the target has two PUSH arms.
- One shared cleanup label for every failure path reproduces duplicated
  epilogues (`CWin32ResourcePbgFile::Open @0x4366C0`). One common return can
  beat per-case returns (`WindowProc @0x4390E0`), but separate returns beat a
  shared exit that saves registers early (`PlayerUpdateInputAction @0x4281D0`).
- Unroll when the target does (three explicit ExecuteScript calls closed
  `StageScriptView::Update @0x402720`); use an ordinary counted loop when the
  target's unrolling came from the compiler (`InitializeGameStatisticsRecord`).
  Pointer induction with a countdown fits fixed release loops; a
  `do { ... } while (n > 0)` trim loop matches the result menu.
- Comparison spelling is visible: `>= 1000000000` versus `> 999999999`;
  `if (a == b) return true; return false;` gives `SETE`; an explicit `(int)`
  keeps a signed `JGE`; an unsigned `sizeof` bound gives `JNC`.
- NaN behavior: `!(a > b)` instead of `<=` keeps unordered inputs on the target
  path; `x <= 1` adds a `JP`.

### Arithmetic, types and x87

- Native 32-bit wrapping arithmetic (unsigned intermediates, low-32-bit
  addresses) is almost always byte-neutral; use it for defined behavior, not to
  move bytes. It changed code when it altered a comparison's signedness or a
  counter's type, or inside very large inlined owners (ECL Run, ExecuteScript).
  Publish through the real signed lvalue, not a reinterpreted `unsigned*`.
- Publish float bit patterns with an `unsigned int` scratch plus a 4-byte
  `memcpy` (`ResetRenderState @0x439D20`).
- Use float locals where retail uses float; double locals emit qword constants.
  Put the float on the left of a comparison with a double sentinel
  (`y > -999999.0`). Use literals instead of stand-in globals bound to the
  constant pool.
- Truncation scope is per owner: the scalar alpha evaluator accumulates one
  float and casts once; RGB truncates each product. Do not generalize.
- Hermite easing shares one `twice = t + t`; value-returning Float2/Float3
  operators and initial-value snapshots reproduce the target's float32
  roundings. Aggregate min/max pairs keep the target's spills where scalar
  locals lose them.
- Vector temporaries: assign a constructed `AnmFloat3View` to the full XYZ for
  one reused 0x0C temporary; direct component stores evaluate in the opposite
  order. Four separate 12-byte `memcpy` calls keep per-pair global loads that a
  struct copy coalesces.
- Bitfields and narrow types are visible: a `colors:15, alpha:1` view matched
  `InvertAlpha @0x437160`; 16-bit results give `TEST AX,AX`; a byte storage view
  gives a byte load where a narrowing cast still shares the DWORD load.

### Managed VM IDs

- Pass existing `AnmVmIdView` objects by value; constructing one from an int adds
  a conversion temporary and changes registers. If typing a slot as
  `AnmVmIdView` adds default-constructor stores to the owner's constructor,
  keep the slot `unsigned int` and view it as `AnmVmIdView` at the call.
- Wrapper types with conversion operators change inlining
  (`ResultMenuFindScript` got inlined); call the real function instead.
- `AddVmVariant0 @0x4489D0` returns the ID through a hidden struct-return
  pointer in EAX; keep the aggregate return.

### Ordering tools accepted as natural source

- Compiler-only `_ReadWriteBarrier()` between memory operations fixed several
  load/store schedules (`DrawBucket @0x4066E0`, GUI message teardown
  `@0x4136C0`, `UpdateStageElements`). It emits nothing, but it is not how the
  original was written; prefer a real data-flow change when one exists.
- A scoped `const volatile &` read of an existing global was accepted where the
  target reads it twice or late (`EnemyReadDialog @0x415DB0`,
  `AddAutomaticEntry @0x41A120`, `SetSubroutine @0x450700`). Elsewhere
  `volatile` usually grows or disturbs code.

### Write a plain `switch`; do not hand-build the compiler's tree

- Hand-writing the compiler's decision tree (nested range tests, `goto`
  labels, `volatile` cursors, `_ReadWriteBarrier`, `register` hints) locks the
  allocator into the wrong registers. `AdvanceTransformProgram @0x406D90`
  stayed at 318/409 aligned instructions in that form. Rewritten as one
  `while (index < 18)` loop and a single `switch` over every opcode, with
  `continue` for opcodes that set the index themselves, the first 0x2F9 bytes
  match retail (372/409 instructions), including the
  `mov eax,ecx; dec eax; cmp eax,0x7f` dispatch and both jump tables. With
  the natural `AnmVmStartScript` order the owner reaches the full 0x644
  extent and 395/409 instructions; only the child-spawn block differs.
- The retail dispatch copies the opcode (kept in ECX for the range tests) into
  EAX for the table index. That shape comes from a `switch` over a separately
  read operand, `switch (record->kind)`, with the earlier tests on a
  `const unsigned int kind` copy. [2026-11-22]

### Argument evaluation order follows the callee's parameter order

- VC7.1 evaluates call arguments right to left, and the scheduler keeps that
  IL order when loads tie. A one-use local is forwarded into its argument
  slot, so declaring it earlier does not move its loads.
  `AdvanceTransformProgram`'s sprite case computes the script index first
  (`mov edx,[esi+8]` … `mov edx,[esi+0xc]` … `add eax,edx`), and only then
  reads the bullet manager. The former `AnmVmView::StartScript(int
  scriptIndex, AnmLoadedView *, void *)` made the index the leftmost argument.
  It was therefore evaluated last and folded into `add eax,[esi+0xc]`, which
  cost 2 code bytes plus the 2 alignment bytes before the jump table: 0x640
  instead of 0x644.
- 0x404F30 is now the free function
  `__stdcall AnmVmStartScript(AnmLoadedView *resource, AnmVmView *vm, int
  scriptIndex)` calling `AnmVmInitialize(0, vm)`, following
  `_reference/th10-decomp`. All 28 callers pass the script index last.
  `SpawnSingleBullet` and the interpreter's sprite case now form the index
  before the resource load without `_ReadWriteBarrier`. The old
  `AnmVmView::StartScript(int, AnmLoadedView *, void *unused)` forwarded
  whatever each caller left in ECX (the manager in one caller, the resource
  in another). That was a fake ABI parameter compensating for an opaque
  `AnmVmInitialize`, and it is gone. [2026-11-22]

### `AnmVmStartScript` and `AnmVmInitialize` visibility (open)

Retail 0x404F30 calls 0x401DE0 with `mov edx, esi` and never sets ECX. None
of the 10 retail call sites of 0x401DE0 sets ECX, and none takes its address.
LTCG therefore saw the initializer's dead first parameter. Yet it kept the VM
in ESI across the call. Our natural source cannot reproduce both at once:

| Graph for `entity-start-script` | Result |
| --- | --- |
| `AnmVmInitialize.cpp` absent (opaque), source passes `0` | 204 bytes: retail plus one `xor ecx, ecx` |
| `AnmVmInitialize.cpp` present, appended or first in `/GL` input order | 200 bytes: VM arrives in EDX because LTCG knows EDX survives the call |
| Present, initializer redeclared as one-parameter `AnmVmInitialize(AnmVmView *)` | 200 bytes, the same as above |

Neither our build nor retail writes EDX inside the 273-byte initializer.
`_reference/th10-decomp` reaches 202 bytes only by linking a 23-byte
stand-in initializer that "hides its register kill set behind an external
call". That is build-context engineering, not source, so we do not use it.
The owner stays a candidate. The interpreter and `SpawnSingleBullet` match
retail with the opaque graph, which suggests that retail's callers saw the
starter keep its VM in ESI. [2026-11-22]

The StageCreate graph has two independently checked configurations. With
the initializer opaque, the script loader at `0x43E710` matches all 203
bytes and four fields using the actual `ExecuteScript` call and the shared
game-speed and renderer declarations. Its receiver and arguments are
ECX resource, EAX VM and EBX script index. The stage dispatcher also
matches, with the starter accepting its VM in ESI.

Adding the real initializer gives 273 exact bytes for that body and
changes all three owners above it. The starter takes the VM in EDX and
shrinks to 200 bytes. The loader shrinks to 197 bytes and receives the
resource in ESI, the VM in EDX and the index in EAX. The stage dispatcher
then matches 1,850/1,868 bytes with all 36 fields. Adding
`GamePrepareAnimationSlots` and selecting it as entry preserves this
result, as do the additional real StartSpell and popup callers. A named
reference to each of the three starter position fields is byte-neutral.
Sharing one zero vector lets C2 collapse their copies and shortens the
starter to 130 bytes. [2026-11-23]

### Non-local regressions through calling conventions

- `e569a902` (2026-11-19) added a `QueueSoundCue` call to
  `src/BulletTransform.cpp`. `bullet-runtime-update` compiled that file as a
  normal (non-`/GL`) support object, so `QueueSoundCue @0x43DD10` lost its
  private EBX/ESI ABI. `BulletUpdateRuntime @0x406240` silently dropped from
  885 to 878 bytes while its `matches.csv` row still said exact. After
  compiling every support source with `/GL` and adding the `StartScript`
  sources, it is 884/885. The one remaining difference is
  `mov ecx, ebp` (retail) against `push ebp`: in this two-caller graph LTCG
  passes the interpreter's `this` on the stack. Retail has exactly those two
  call sites and no address reference, so the cause is still unknown.
  Adding `RandomMath.cpp` was neutral.
- When a support file gains a call to a private-ABI callee, replay every exact
  unit whose graph compiles that file as normal COFF.
- `SpawnBulletPattern @0x4073E0` takes its descriptor in EAX in its own exact
  graph and in retail, but in ECX inside the interpreter's graph. This is the
  interpreter's last structural gap (+0x3E0..+0x495, child spawn; its only
  field mismatch is the manager load at +0x46A). Adding
  `RandomMath.cpp` or making `BulletManager.cpp` the first `/GL` input did not
  change it. [2026-11-22]

### Compiler evidence from TH09

TH09 pins VC7.1 13.10.3077. Its `c2-analysis/c2.dll` matches that
checkout's tool lock. TH10 pins 13.10.6030, so recover breakpoint
addresses and record layouts from TH10's backend.

- TH09's `RunEcl` recovered 30 residual bytes in two handlers by restoring
  direct assignments, a bitfield store and ordinary switch flow across
  the dispatch. C2 still merged the common tails. TH10's Run trials move
  dispatch-table destinations when the arithmetic case bodies are reordered.
  Compare the whole owner after changes to neighboring cases.
  [TH09 RunEcl register coupling](../../../th09-reconstruction/th09/docs/KNOWLEDGE_BASE.md#runecl-register-coupling-across-handlers)
- TH09's enemy update needed both the real timer definition and a
  draw-list append in each source branch. The extra scratch allocation
  persisted after the compiler merged the appends. When a register change
  survives scheduling or tail merging, check the allocation trace for
  earlier requests in the same owner.
  [TH09 Enemy update](../../../th09-reconstruction/th09/docs/KNOWLEDGE_BASE.md#2026-11-21-enemy-update-exact)
- TH09's playback restore needed the exact seed setter visible in the
  same translation unit. The call stayed in place. The visible body
  removed the dependency from the side-pointer load and retained the
  dependency from the lives store. For a TH10 scheduling difference
  around a call, compare those dependencies with the real callee visible
  under `/GL`.
  [TH09 playback restore](../../../th09-reconstruction/th09/docs/KNOWLEDGE_BASE.md#2026-11-21-co-visible-replay-seed-and-exact-playback)
- The TH09 movement trace identifies a load introduced by the allocator.
  Its order was already set when the equal-priority loads reached
  scheduling. In the title menu, changing the index's later float
  conversion moved it from global to local allocation. A shared field
  reference also restored the stack operand of the conversion and
  matched the whole draw routine. Compare the operands and liveness
  at allocator entry before editing their final instruction order.
  [TH09 Player movement](../../../th09-reconstruction/th09/docs/KNOWLEDGE_BASE.md#replacement-of-the-configuration-load)
  [TH09 selected-row conversion](../../../th09-reconstruction/th09/docs/KNOWLEDGE_BASE.md#selected-row-conversion)

In TH10's C2, the stack-colour comparator at `0x1078E2D9` orders by
class, then by signed integer `weight * 1000 / size`. The midpoint
pivot in the sort at `0x1078CB20` can reorder equal-priority records.
An isolated stack-slot difference can therefore originate before the
sort, when C2 groups values and counts their uses.

### Byte-neutral steering removed from `BulletManager.cpp` (2026-11-22)

Each of these was removed and the ten owners they touched stayed exact. Do
not reintroduce them as exactness tools:

- `memcpy` "wrapped INC" (`unsigned next = (unsigned)x + 1u; memcpy(&x, …)`); use `++x`;
- `BulletNativeTableRead` and `BulletBucketAtWord` unsigned-address indexing; use `table[index]`;
- pointer advance through `reinterpret_cast<unsigned int>`; use `++bullet`;
- the four `_ReadWriteBarrier()` calls in `SpawnSingleBullet`;
- `#pragma var_order(...)`.

The ten owners are `SpawnSingleBullet`, `UpdateBullets`, the update callback,
deceleration, vector and polar acceleration, relative and aimed direction
change, homing and boundary bounce.

The same idea does not apply blindly. Splitting `ExecuteScript`'s two merged
child-creation tails (opcodes 88/90 and 91/92) into four self-contained cases
made LTCG inline `InsertAfter` and regressed the owner from 9,962/9,964 to
9,884 bytes, so the two shared tails are retail structure. [2026-11-22]

### Neutral controls (do not retry without new evidence)

Operand, XOR and addend order; local declaration order and renames; naming float
products; `int limit`/`int length` locals; const/reference declarations;
reverse `if/else` versus `goto`; memset spellings; whole-word versus bitfield
initialization (Chain ctor); `/G5`, `/G6`, `/G7`, `/Zi`; native-word index
rewrites in the ECL dispatcher; moving ECL stack services to another TU.

## Comparison, binding and units

- Exactness means the complete owner extent from the PDB contribution,
  including jump tables, selector bytes, alignment and the full `RET imm16`,
  with every relocation bound to its semantic target. Equal size, equal case
  offsets, masked operands or a relocation-normalized match are never
  acceptance. The target PE has no base-relocation directory; field inventories
  come from decode and table review.
- Bind fields by semantic base plus addend (supervisor 0x491C28+8 for the
  device, 0x474C40+offset for score state). Nearest map labels are incidental:
  small harness anchors coincide numerically with unrelated fields. Do not
  apply an addend twice.
- An exact leaf can stay exact while its inlined copy regresses the parent
  (`EclVmStackView::Pop` versus Run), and an exact child does not make its
  caller exact.
- Nonexact owners live in `config/frontiers.toml`, checked by
  `scripts/check-frontier.py`: it rebuilds each graph, masks the candidate's
  REL32 and base-relocation fields and fails when a previously matching byte
  differs. Triage units carry `candidate = true` and are excluded from the
  exact ledger check and replay. A frontier residual measured in one graph does
  not transfer to another.
- Identical build groups must share one output directory, and two units cannot
  own the same target address. Never edit `config/match-units.toml` while a
  replay runs, because each unit build rereads it.
- `pdb_source` selects which declared source owns the PDB contribution without
  changing link order. TU-static owners need explicit static owner selection.
- EH binders check the parent's whole generated handler region, the 28-byte
  FuncInfo with its state count, the saved this or allocation slot and the real
  child destructor. `__except_list` is an absolute symbol from libcmt's
  `exsup.obj`. Destructor iterator `??_M @0x4525FF` walks backward (RET16);
  constructor iterator `??_L @0x45252D` walks forward (RET20).
  `scripts/linked_item_array_eh.py` has a PARENTS table for array parents.
- Bind CRT calls by identity: `operator new @0x452493` (`__nh_malloc`, not
  nothrow), scalar delete `@0x4524A1`, `malloc @0x452706`, `free @0x452422`,
  `strncmp @0x452F80`, `_floor @0x452FF0`, `_vsprintf @0x4524A6`, `__ftol2
  @0x463B2C`. Missing `dllimport` on Enter/LeaveCriticalSection silently broke
  five comparisons.
- Some data lies in the `.data` virtual tail and cannot be raw-read: the tangent
  vector 0x491C14 and the sound owner 0x492590.
- Stale objects and caches mislead after header changes; rebuild cold. Wine
  `LNK1104` or page-fault linker crashes are transient; retry serially.
- Old single-TU recipes can fail after source changes. Bisect a suspected stale
  exact claim in a detached worktree with `.tools` and `resources/th10.exe`
  symlinked in.
- `_reference/th10-decomp` (MIT, see `docs/licenses/`) has natural MSVC sources
  for most remaining owners, but many of its exact builds rely on synthetic
  LTCG callers, escape functions, `volatile` reads, private RNG copies or
  stand-in callees. Port the natural spelling and verify in our graph.
  Adjacent games (TH08/TH09/TH095) are hypotheses only.

## Rejected approaches

Not acceptable for exact credit: `__declspec(naked)` or inline asm, copied
target bytes, fake ABIs or dummy parameters to force registers, padding or
inflated arrays to fit a frame (`replayPath[65]` against the confirmed 64-byte
array), inert locals or dummy writes, fabricated returns, forced
calling-convention attributes on factories, profile sweeps or global flags to fit
one owner, volatile sinks used only to force a schedule, cropping oversized
candidates, and scores measured in another source epoch or an incomplete graph.
A higher score from an incomplete graph is not progress (ExecuteScript's 1,723
without AnmVmId support). Raw-equal spans earn no partial credit.

## Open owners

Latest useful linked-owner state, largest first. Rebuild the affected graph after
source changes. `REGRESSED` means residual offsets moved relative to the stored
frontier; do not refresh a baseline merely to accept the new positions.

- **EnemyRuntimeView::DispatchEclInstruction @0x40E770 (14,416)**: the
  current primary 36-support-TU `/GL` candidate is 14,452 bytes with 10,414
  differing bytes; the secondary 12-TU `/GS` diagnostic is 14,456 bytes with
  10,240 differing bytes. Both frontiers report `REGRESSED` because the full
  residual-offset sets moved (their stored counts are 10,416 and 10,574);
  keep those baselines unchanged. Runtime +0xE8/+0xEC/+0xF0 now use explicit
  ANM resource/script index typedefs and independent offset assertions. The
  ten managed-VM slots have a named count, and slot zero is identified as the
  primary ANM VM. This is semantic layout recovery, not an exactness claim.
  The 627-byte collision owner and 44-byte bullet-clear owner remain exact in
  the affected graphs.
  - The 181-byte opcode map selects 108 handlers; the main table starts at
    +0x35C0. The owner extent ends at 0x411FBF. Rank-3 thresholds are +/-512;
    rank-5 thresholds are +/-600 and +/-200. The spell-name cipher's
    `8A 0C 31` SIB remains at +0x273B.
  - `FIRE_BULLET_PATTERN` now keeps one `EnemyBulletPatternView*` across
    XYZ publication, the protection-radius check and `SpawnBulletPattern`.
    This was the largest recent improvement: 36-TU 14,468/14,416 with 10,495
    differences became 14,452/14,416 with 10,414; 12-TU 14,472/14,416 with
    10,531 became 14,456/14,416 with 10,240. The owner is still nonexact.
  - Paired script writes for bullet sprite IDs, offsets, angles, counts,
    speeds, sounds and rank interpolation retain the typed pattern/offset
    receiver after the first store. Preserve the original unchecked
    modulo-32-bit index arithmetic and operand-read order; hoisting receivers
    before the first store changed register lifetimes and regressed the owner.
  - Rank-count and rank-speed branches use typed `EclVmHost*` / active-context
    access for the second operand read. `AIM_BULLET_AT_PLAYER` retains the
    original X address and a typed `Player*` for Y. These are natural source
    repairs; the dispatcher remains nonexact.
  - `dispatch_copy_player_to_motion` transfers all 12 XYZ bytes with
    `memcpy`, preserving NaN payloads and signed zero. Pattern initialization
    clears the complete 0x210-byte record before explicit defaults;
    `COPY_BULLET_PATTERN` uses complete typed assignment because source and
    destination indices may be equal. Opcode 0x199 has 22 overlapping
    0x18-byte extra records per pattern.
  - Type-0/type-1 laser request paths copy the serialized pattern words into
    their verified request layouts. Keep their distinct 0x1DC/0x1F8 request
    sizes and the `EnemyLaserType0/1RequestView` field offsets.
  - `EnemyManagedVmIdAt` retains the ten four-byte VM handles with unchecked
    32-bit address arithmetic. Only the flag loops have a proven 0..9 bound.

- **EclVmScriptDatabase::AddScriptData @0x450220 (589)**: the SCPT file
  header is a verified 0x24-byte native layout. Both the subroutine-offset
  table base and optional include-package payload now derive from
  `sizeof(EclVmScriptFileHeader)` rather than repeating the literal `0x24`.
  The selected five-source `/GL` graph re-links to the complete 589/589-byte
  exact function, including both malloc/free REL32 fields. [2026-11-19]

- **AnmRenderManagerView::ExecuteScript @0x43EE30 (9,964)**: the current
  GameManager and player-collision graphs strictly compare 9,962/9,964
  bytes using the 350-field [binding contract](evidence/anm-child-request-record/binding-contract.json),
  including 95 fixed local destinations. At +0x113 and +0x115, opcode 91
  loads and pushes the layer through ECX where retail uses EDX.
  - In pinned C2 13.10.6030 under `link.exe /LTCG`, opcode 91's layer
    destination is already ECX at `RegSimpleAssign` (0x1079BD18). The
    layer loads in opcodes 88, 90 and 92 still have virtual destinations
    at that point. `RegAssign` selects ECX, ECX and EDX for those loads,
    respectively. The opcode 91 resource load also reaches this pass
    with a virtual destination and gets EAX. `Riscify` leaves these
    assignments untouched.
  - The load's destination operand points to the allocation symbol at
    `+0x18`. At `0x107AAC16` that symbol is candidate 45, with weight 32
    and ECX, EDX, ESI and EDI available. The compiler selects ECX.
    Moving either `GetIntVar` or the whole
    interpreter to a separate `/GL` translation unit preserves the two
    differences. The moved `GetIntVar` independently matches all 144 bytes
    and 15 fields. Aggregate initialization of the script/layer pair also
    preserves the residual. Capturing just the script index moves the
    resource load before the argument pushes. Keeping the returned IDs
    alive until an explicit shared resolve call shortens the interpreter
    by 64 bytes with two IDs and 80 bytes with one.
  - Splitting the child pointer into separate locals for the front and
    back tails preserves the opcode 91 mismatch and reverses the two
    radial Z operands at +0x2476/+0x247C. Expanding the operand ternary
    into an if/else inside the request is byte-neutral. Moving the real
    `CreateVmVariant2` definition to `AnmVmCreate.cpp` also preserves all
    9,962 matching bytes and every field.
  - The default member convention with direct arguments still generates
    the exact 73-byte factory in this link, but moves the resource load
    before the first argument push, leaving ten caller differences. The
    reference's inlined attachment helper changes the call and tail
    layout in this graph. Its explicit result-pointer shape also changes
    layout because the maintained handle's default constructor zeros
    the result slot. The maintained source retains both shared tails
    and the value-returning factory.
- **EclVmContext::Run @0x44E1A0 (7,020)**: current linked owner is 7,020/7,020
  with 23 differing bytes. The frontier is `REGRESSED` because residual offsets
  moved, not because the count changed. The remaining schedule includes
  commutative SIB order at +0x1F9/+0x241, operand/register choices near
  +0xEB/+0xF1 and +0x129/+0x12B, and several spill-home differences. Do not
  update the frontier to accept moved offsets. `PopInt` uses the native
  `EclVmScalar::integer` word; `EclStopThreadContext` retains a typed
  `EclVmContext*`; `Pop` keeps the typed `stackData + stackTop` address.
  Rewriting the pointer arithmetic or spawning-operand address expanded the
  parent residual and was reverted.
  A complete comparison in the `16572e6b` player-collision graph confirms
  all 189 fields and 62 fixed local destinations. Direct typed stack pops
  for bitwise OR, primitive word locals for add and subtract, and relocating
  the real `ReadFloatValue` implementation to a separate `/GL` TU all leave
  the same 23 differences. The relocated resolver still matches 171/171.
  Separating FORMAT's integer and floating conversions into two typed
  source branches instead emits a 6,956-byte owner.

  The current GameManager link reproduces the same complete comparison.
  Renaming the two arithmetic locals preserves every byte and identifies
  the twelve stack-offset differences as the left/result words for integer
  addition and subtraction. They both have weight six. At C2's stack-colour
  sort (`0x10768DBA` to `0x10768DF7`), the pair moves from colours 2 and 3
  to 6 and 5. The final homes are `ESP+0x24` for add and `ESP+0x20` for
  subtract. The target puts add at `ESP+0x20` and subtract at `ESP+0x24`.
  The offsets after two argument pushes refer to the same words.
  Separate `/GL` units for `Pop` and `Push` preserve the parent residual
  and the exact 122-byte and 96-byte leaf bodies, respectively. Extracting
  `Pop`'s raw copy into an inline helper expands the parent residual to
  119 bytes. Publishing the wrapped sum through a named word retains the
  home order and changes four SIB encodings elsewhere in `Run`. [2026-11-23]
- **EclVmHost::StopAllThreads @0x450190 (31)**: exact 31/31 after making the stop helper accept the already-resolved `EclVmContext*`; both STOP_THREAD and STOP_ALL_THREADS now pass the context explicitly. The parent `EclVmContext::Run` schedule remains at its existing 23-byte residual. [2026-11-19]

- **EclVmStackView::Pop @0x450610 (122)**: exact 122/122 with the typed
  stored-type view and private EAX/ECX/BL ABI. The adjacent stack owners are
  also exact: `Push` 96/96, `EnterFrame` 64/64 and `LeaveFrame` 41/41.
  Keep the leaf spelling that preserves the parent `Run` schedule; solve the
  23-byte parent residual independently.

- **GuiMessageVmView::Run @0x415E90 (4,480)**: [2026-11-17]
  The current `gui-message-run-candidate` 16-support-TU linked graph
  produces 4,464/4,480 bytes with 1,935 differences. Its recorded
  1,910-byte residual frontier is older, so `REGRESSED` describes
  moved offsets, not a current exactness result. All 36 portrait sprite
  script-offset additions now use unsigned 32-bit arithmetic, with no
  machine-byte change. Collapsing the eight syntactically identical
  player-character branches shrank the body to 3,796 bytes and was
  rejected; they must stay separate for the target branch layout.
  Remaining work: portrait operand register lifetimes, second enemy
  portrait resource-load order, inline clear-count increments and the
  shared stage 6/7 game-mode tails. The separately owned 3,836-byte
  GUI stage updater remains exact.
- **ResultMenuUpdateResult @0x4236F0 (3,227)**: four-TU graph (FrontEnd `/GL /GS
  /EHs`, VmIdInterrupt, real Enemy sound callee, AnmVmId), 3,228 bytes,
  784/907 normalized, 173 fields. Open: states 7/8 and 10-13, the GUI score
  receiver and the 0x48 versus 0x4C frame. Next: a faithful separate DWORD
  lifetime with no extra copy. [2026-11-13]
- **FrontEndControllerView::UpdateOptions @0x42D920 (3,188 incl. 5-entry
  table)**: 3,196; three interrupt adapters still expand. [2026-10-29]
- **PlayerUpdateCallbackBody @0x425730 (3,112)**: 3,100. `FLD 1.0; FMUL scale`
  operand order is reversed, the table base is 4 bytes early and the effect-loop
  EDI subframe differs. [2026-11-12]
- **FrontEndUpdateMusicRoom @0x433EF0 (2,796)** now has the full target
  extent and all 106 fields. The phase 1 and 2 table entries start two
  bytes early. The current sound helper restores both queue receivers that
  were missing in the older 2,788-byte build.
  `Select(0)` recovers both count-dependent cursor stores. A shared cursor
  pointer keeps the browse selection and previous value on the same base.
  Testing the near-row condition first gives the target x87 branch.
  `StartMusicRowInterpolation` places duration before the vector
  parameters and restores both PUSH sequences while preserving its exact
  206 bytes and seven fields. Guarding the parser with an explicit first
  iteration test aligns its early block but grows the owner by 16 bytes.
  The parser's comment cursor selects EBX at global allocation
  (`0x107AAC16`), with weight 36 and EBX as its sole available register.
  The target uses ESI there. Check the competing saved-register lifetimes
  before local allocation. The prologue and phase offsets also remain open.
  [2026-11-23]
- **RebuildPlayerOptions @0x426F70 (2,520)**: about 2,404, plain RET instead of
  RET4 even with all nine callers present. [2026-10-19]
- **EnemyGameStateView::StartSpell @0x409280 (2,432)**: current linked
  candidate 2,404/2,432 with 1,129 differing bytes; 14 receiver LEAs use
  EDX instead of ESI and eight PUSH schedules differ.
- **ItemManagerUpdateBody @0x41AFD0 (2,320)** is exact in the registration-rooted
  `/GL` link, with 154 bound fields, 73 direct local branches and both jump
  tables at their target offsets. Expressing the distance scale as
  `(playerY - 144.0f) / (448.0f - 144.0f)` lets C2 evaluate distance first
  and emit `FSUB` against positive `5000.0f`. The reciprocal literal with
  compound assignment gave `FADD` against negative `5000.0f`. An inlined
  penalty helper emitted `FSUB` and moved the faith calculation ahead of
  distance. Both orders are visible at C2's global allocation entry
  (`0x107A5471`). Link object order leaves that earlier residual unchanged.

  Above the collection line, rounding a named score and passing it to its
  own popup call lets C2 share that tail with case 5. It keeps the case 2
  load in EAX and the case 5 load in EBX. The large-power timer label
  restores both the branch destination and the conversion flag store.

  The independent update translation unit keeps the item caller's three
  EDI reloads alongside the real 46-byte `AddFaith` body in `ItemManager.cpp`.
  Normal COFF and the `AddFaith`-rooted link produce a 48-byte helper with
  an extra EDX load. The reload after `AddRank` uses the distinct
  `g_GameStartupState.score` view of the same target storage.

  The 141-byte `AddLives` member retains the private EAX and ECX arguments
  required by this caller and the score HUD.
  [2026-11-23]
- **GameUpdateGui @0x417040 (671)** is exact with 36 bound fields using the
  same `AddLives` member as the item update. Keep the complete award within
  each qualifying difficulty branch, as in the reference score HUD. VC7.1
  merges the shared tail and loads the receiver before the amount. A single
  award reached by two early threshold exits reverses those loads at +0x286.
  The item update, the 141-byte member and GameManager's update also match
  with the branch-local awards.
  [2026-11-23]
- **AnmRenderManagerView ctor @0x445900 (2,315)**: 655/670 normalized
  instructions after setting `element->arg = this` before `flags |= 3` and
  resetting the capture surface index first; the EH frame, VM flag loop and Chain
  scheduling still differ. [2026-11-14]
- **ExportAutomaticHints @0x41A200 (2,247)**: native inlines all 24 writer uses;
  the target makes 9 calls and inlines 15. [2026-10-25]
- **GameResetGui @0x413BC0 (1,965)** is exact with 52 bound fields and
  73 fixed local branches in the GameManager update graph. Each loop
  walks flag-word addresses with a stride of `sizeof(AnmVmView)`. C2
  folds the array and flags offsets into +0x4CDC in both loops, and
  preserves the EDX first-hidden index and ECX countdown. The JLE at
  +0x1D0 enters the hide setup at +0x1F0 for a non-positive count.
  The upper-bound test at +0x1EB remains on the positive path after the
  show loop. With an `AnmVmView *` in the hide loop, C2 calculates the VM
  address first and adds +0x35C separately. Using a flags address only in
  the hide loop restores the folded LEA but reverses the show address
  and count-copy order. The standalone life setter still matches all
  77 bytes in the reset graph. The `script-viewer-update` direct caller
  retains 1,251 bytes and all 83 fields after relinking with the new GUI
  object. [2026-11-23]
- **StageExecuteScript @0x403C80 (1,868)** is exact with 36 bound fields,
  including the switch base and all 15 table entries. In opcode 9, write
  the fog interpolation mode before copying its initial and final records,
  as in the reference background script. The earlier source held the mode
  in EAX across both copies at C2's local allocation entry (`0x1079BD18`).
  With the direct store, its load is still virtual at entry and gets EAX
  locally, advancing the scratch cursor at `0x1079C39D` to ECX. The opcode 9
  instructions before the exit jump remain equal after address binding.
  The extra request restores the register rotation in opcodes 10–12 and
  the five-byte EAX store to `Main.clearColor` in opcode 13. The shorter
  store restores the following branch destinations and the table alignment.
  Opcode 14 calls the real `AnmVmStartScript` with ESI for the VM, EAX
  for the script index and the resource on the stack. In this StageCreate
  link the starter itself remains 204/202 bytes with the extra ECX clear
  before `AnmVmInitialize`. [2026-11-23]
- **PlayerUpdateMovementAndOptions @0x4250B0 (1,656)**: 1,616; both direction
  table bases 40 bytes early. [2026-11-12]
- **GameManagerUpdateBody @0x418190 (1,544)**: exact with 114 bound fields
  and 60 fixed local branches in `game-manager-update-body`. Choosing the real
  `GamePrepareAnimationSlots` as the LTCG entry keeps its stack receiver. With
  the same update source, the usual ANM entry gave that callee EDI and the
  update kept its context in EBX. The stage entry returns the update context to
  EDI and reduces the remaining mismatch to the delayed reset's second VM ID.
  Naming the VM ID pointer restores `mov esi` followed by `add esi, 0x89a4`.
  A reference to the ID also matches. The disposal guard and typed `activeFrames`
  access preserve the retail code shape. Snapshotting the elapsed time inside
  the demo branch preserves its two tests. An entry-wide frame snapshot added
  a four-byte stack slot. [2026-11-22]
- **LoadHintFile @0x419960 (1,498)**: 1,533. [2026-10-20]
- **UpdateReplaySave @0x433570 (1,460)**: 1,444; the target reloads the pressed
  input after the movement sound. [2026-10-30]
- **BulletRuntimeView::AdvanceTransformProgram @0x406D90 (1,454 code bytes)**: the `bullet-transform-program` unit uses VC7.1 SP1 LTCG `/GL` + `/GS` and a 0x644-byte complete code/local-table comparison extent, including five local switch-table destinations at +0x5B0..+0x5C0 and the 128-byte selector span at +0x5C4. A fresh build against the current source still has a 0x640 PDB contribution; strict comparison stops with `PDB contribution size 0x640 differs from manifest 0x644` before comparing the complete owner. The target computes `index * 3` in EAX, loads kind into ECX, and forms the record pointer in ESI; the current baseline candidate assigns scaled index to ECX and kind to EAX. Several natural C++ type/scope/expression variants were explored without improving the complete owner. The source models 18 records of stride 0x18 at runtime +0x464, kind +0x10, allow-while-active +0x14, active flags +0x43C, cursor +0x45C, and nine extension-state slots at +0x614. Next focus is EAX/ECX/ESI lifetimes, child descriptor initialization order, both loop tails, and complete local tables. Do not shrink the comparison extent or claim partial exactness. The historical `bullet-runtime-update` source-epoch caveat remains separate. [2026-11-20] On 2026-11-22 both the committed `9957cd86` register-lifetime source and an uncommitted raw-offset record-access variant still produced 0x640, so the extent gate is unchanged. [2026-11-22]
- **PlayerInitialize @0x4247F0 (1,435)**: 1,380 with 49 fields; current source
  carries five barriers and a scoped volatile manager capture. [2026-10-23]
- **BulletSpawnDescriptorView::SpawnSingleBullet @0x4067D0 (1,413 code + 59 local-table bytes)**: exact since 2026-11-22. A selected cold replay reproduces 1,472/1,472 bytes with all 44 fields. The last 54 bytes closed through three natural changes. First, the call became `AnmVmStartScript(manager->bulletAnm, &bullet->vm, table[type] + color)`, which removed a `_ReadWriteBarrier` and the fake ECX argument. Second, the callee `AdvanceTransformProgram` became a plain switch, which fixed the effect-switch registers; this is a non-local effect through LTCG. Third, the cursor tail was written as `++bullet; if (bullet->state == 5) cursor = &bullets[0]; else cursor = bullet;` instead of an inverted early return. Earlier fixes still hold: the ordered `if/else if` interrupt selection and the collision-height/width store order. [2026-11-22]
- **EnemyLaserLineView::RunCommands @0x41CA80 (1,356)**: 1,368; embedded VM
  receiver EDX versus ESI. [2026-10-20]
- **HintOwnerView::ConsumeEntries @0x419120 (1,317)**: 1,308 with all 34
  identities. [2026-10-31]
- **EnemyLaserType1View::CheckCollisionBox @0x41EB00 (1,171)** and
  **CheckCollisionCircle @0x41EFA0 (1,108)**: 1,209 and 1,178; the type-0
  factory branch still inlines. [2026-11-06]
- **GameManagerCleanup @0x417C80 (1,105)**: the selected VC7.1 SP1 LTCG replay
  matches the complete owner and all 110 fields. A named animation resource,
  a shared byte-sized replay flag and separate mode 4 and 15 arms recover the
  branch layout. The previous switch emitted 1,120 bytes. Adding the real
  statistics finalizer initially left the caller unchanged because the caller
  declared cdecl and the `/Gz` definition emitted stdcall. A shared explicit
  `__stdcall` declaration lets LTCG pass the statistics owner in EBX. Compiling
  the real `Player.cpp` with `/EHs` restores the destructor's stack receiver
  and the cleanup function's ESI allocation around the delete. [2026-11-22]
- **EnemyFullObjectView::ReadFloatOperand @0x412350 (884)**: 876. [2026-10-30]
- **MergeJoystickInput @0x44A190 (833)**: 235/282 normalized in
  `input-controller-buttons`. [2026-11-14]
- **ItemManagerView::Spawn @0x41BB00 (819)**: 795 with the right EAX manager,
  ECX position and RET16; branch and tail layout differ. [2026-10-21]
- **SoundPlayerView::Initialize @0x43CD30 (786)**: 755; the target saves EBX
  conditionally. [2026-10-29]
- **GuiView::DrawStageElements @0x415800 (724)**: [2026-11-17] exact
  across the complete 724-byte function and 33 original-bound DIR32/REL32
  fields in the 17-input GUI draw `/GL` graph. The recovered natural C++
  first rectangle uses staged updates to `rectangle.right`: first the
  converted state integer, then `*= 0.2923077f`, then `+= 51.0f` after
  storing the bottom edge. That expression/order yields the retail
  `FILD`, color `MOVZX/SHL`, `FMUL`, rectangle `LEA`, `FADD` and final
  `FSTP`, resolving the earlier 6-byte `LEA`/`FMUL` inversion at +0xD0.
  All 173 original instructions and the 23 local control destinations
  remain whole; no code padding or inline assembly was introduced.
- **GuiMessageVmView ctor @0x415B00 (683)**: [2026-11-17]
  In the six-input `EnemyReadDialogEh` graph the current candidate is
  684/683 bytes, with 81 ordinary (non-linkage) byte differences.
  That improves on the 702-byte initial candidate: six VM ID slots
  now use four-byte storage with no pre-memset zero stores, the 2,000
  bullet records use the original advancing pointer walk, and both
  `AddVmVariant0` return values have independent C++ scopes.
  An intermediate 683-byte candidate had 227 ordinary differences;
  choosing that size alone would regress the actual comparison. The
  one-byte extra instruction in the better 684-byte build is near the
  final enemy-manager reload. Four-byte storage and offset-zero
  assertions are retained. Alternate field orders, manager captures
  and ID temporary placements did not improve this residual.
  `EnemyReadDialog` (124/124), `EnemyKillAll` (182/182) and the GUI
  stage updater (3,836/3,836) remain exact in affected graphs.
- **GuiShowScoreNotice @0x4172E0 (668)**: 660. [2026-10-25]
- **MainSupervisorView::UpdateGameMode @0x4218D0 (666)**: 678. [2026-10-29]
- **CStreamingSound::HandleWaveStreamNotification @0x44D850 (637)**: 634. The
  637-byte CFG variants order the read and fill blocks wrongly. [2026-11-11]
- **EnemyResolveRuntimeCallback @0x4127A0 (593)**: natural C++ callback now
  uses a 32-bit flag-word bit-field and `_ReadWriteBarrier()` to preserve the
  timeout state write ordering without inline assembly. The full 593-byte
  PDB-owned contribution matches all 147 instructions; seven target-bound
  DIR32 fields, including `g_Player` at +0x1B3, normalize completely with
  zero differences in the pinned 17-support-TU VC7.1 SP1 /GL /GS graph. A
  scoped canonical cold replay independently rebuilt this unit and passed
  the complete PDB-owned extent, all seven DIR32 bindings and normalization
  checks (PDB GUID `e2fe72994a11224b8fb8ca595ee88ec9`, age 1). No inline
  assembly or padding is used. [2026-11-17]
- **EndingView::Initialize @0x40B560 (584)**: full size, node-flag scheduling.
  [2026-10-30]
- **ReplayManager::BeginStage @0x42A450 (582)**: 566. [2026-10-31] Porting
  LoadReplay together with it broke `replay-manager-create`.
- **EclVmContext::StartSubroutine @0x44DF70 (550)**: 551. [2026-11-07]
- **ScriptViewerPopupOwnerView::Draw @0x42B780 (540)**: current
  four-source linked graph yields 537/540 bytes and 266 non-linkage
  differences; the 10-byte draw callback is also nonexact.
  [2026-11-17] The count-dependent digit cursor now uses native
  modulo-32-bit address addition to `record->digits` and decrements the
  byte cursor through the same unsigned word representation. This avoids
  undefined one-before-array pointer arithmetic when `digitCount == 0`
  or after the last digit, while preserving the target
  `LEA/DEC` behavior and ordinary C++ digit rendering. A fresh focused
  four-source popup linked image remains 537/540 bytes with 266
  non-relocation differences; the 10-byte draw callback remains nonexact,
  and the separate popup Update owner passes its strict 134/134-byte
  link comparison. This is a semantic repair, not exact-code credit.
- **EnemyKillAll @0x40E6A0 (182)**: canonical `enemy-kill-all` linked-PE
  unit cold-replays the complete 182-byte PDB-owned extent with all six
  target-bound DIR32/REL32 fields at zero differences. The real
  `AnmLoadedView::InitializeVm` caller graph owns the `src/GuiMessage.cpp`
  contribution; a volatile scale pointer preserves the retail x87 operand
  order in the timer fallback. [2026-11-16]
  [2026-11-17] The timer-fast-path `++current` now publishes an unsigned
  modulo-32-bit increment through a four-byte `memcpy`, so it also models
  the target i386 increment when the signed current value is `INT_MAX`.
  This makes the native word transition explicit without signed-overflow
  undefined behavior, ASM or filler. The affected 22-support-TU canonical
  caller graph retains the full 182/182 exact target and six fields; the
  separate six-input EnemyReadDialog graph preserves its 124/124 exact
  owner. No new exact byte credit from the semantically repaired spelling.
- **TransitionShakeLinear @0x43C550 (439)** is exact in the 14-input GUI
  transition link. At 0x43C5CA and 0x43C601 the target reads both RNG seed
  halfwords as one 32-bit word. The callback copies that word with
  `memcpy` and updates only the live halfword. This representation and the
  reference's named, explicitly converted float product move `FIDIV` from
  +0xAA to +0xCE, inside the second RNG update. The earlier product-only
  change kept the divide early with `RngView::GetRandomU16`.
  A selected cold replay binds all 20 fields and preserves the 396-byte
  initializer and 349-byte envelope callback. [2026-11-22]
- **TransitionShakeEnvelope @0x43C710 (349)**: exact in the actual
  Transition/GUI 14-input `/GL` graph, including 16 complete REL32/DIR32
  fields. A volatile reference to the real `effect->value2` phase tail,
  added after the timer tick, and summing that value before `value1` and
  `value0` recover the target phase-limit instruction schedule. The source
  still computes the unsigned 32-bit phase endpoint and preserves the
  ramp/hold/decay envelope, both RNG calls, and camera X/Y updates.
  Two independent selected colds pass this whole 349-byte PDB owner plus
  `TransitionEffectView::Initialize` 396/396 with 37 fields. A separate
  selected 17-unit cold of this graph passes 2,477 bytes in total.
  [2026-11-17]
- **BulletManagerView::CancelBulletsInArea @0x408210 (585)**: the
  `bullet-cancel-in-area` linked-PE unit (VC7.1 SP1 `/GL /GS`, nine-source
  bullet graph including `ItemManager.cpp`) is strict 585/585 with all 15
  fields bound. Writing the size thresholds as `width <= 16`, then
  `width <= 32`, then the large palette reproduces the target's two x87 `jp`
  branches. A selected cold replay passed and the owner is canonical exact.
  [2026-11-22]
- **BulletRuntimeView::UpdateAbsoluteDirectionChange @0x4078E0 (335)**: current
  linked candidate is 335/335, with two differences at +0xD7/+0xDD
  (target EDX, candidate EAX). `register` hints were byte-neutral; removing the
  angle-copy local and passing `angle` directly adds two differences; volatile
  forms also regress. All trials were reverted.
  The `_reference/th10-decomp` `EaseStep` shape (plain float fields, direct
  `++int2`, `angle = value1`, reset speed copied before the timer reset)
  regressed to 320/335 on 2026-11-22 and was reverted.
- **Player::~Player @0x424ED0 (325)**: 333. [2026-10-31]
- **FrontEndInsertScore @0x421FA0 (316)**: 263; the helper uses 64-bit `100.0`
  where the target uses 32-bit. [2026-10-15]
- **InitializeGameStatisticsRecord @0x42ACB0 (274)**: the pinned five-source
  `/GL` statistics graph produces the complete 274-byte PDB function with
  75 decoded instructions. Expressing rank-kind lookup through the natural
  signed-byte table pointer (`*(g_GameStatisticsRankKind + index)`) improves
  normalized instruction agreement from 45/75 to 58/75 without changing
  the two 4-byte name copies or the nine-byte destination. The remaining
  differences concern the zero-word register, name-copy register allocation,
  group-count register and two commutative SIB byte encodings; the complete
  function is not yet exact. Its separate 142-byte tail initializer, 423-byte
  statistics loader and 564-byte finalizer remain exact in the same image.
  The selected function has its own residual ratchet, not exact credit.
  [2026-11-19]
- **PlayerLoadOptionData @0x426520 (229)**: canonical scoped 20-support-TU
  VC7.1 /GL cold replay in the real `EnemyReadIntOperand`/`PlayerInitialize`
  caller graph. The complete 229-byte `src/Player.cpp` PDB contribution
  normalizes all six reviewed external REL32/DIR32 fields with zero residual.
  The canonical 2026-11-16 replay is 229/229 bytes with all six fields exact;
  the former float-order/alignment caveat no longer applies. [2026-11-17]
- **GamePrepareAnimationSlots @0x404450 (214)**: the target uses a stack owner
  and RET4; the real graph gives EDI and plain RET. [2026-10-31]
- **AnmVmStartScript @0x404F30 (202)**: candidate again since 2026-11-22.
  The natural `(resource, vm, scriptIndex)` body is retail plus one
  `xor ecx,ecx` while `AnmVmInitialize` stays opaque; see "`AnmVmStartScript`
  and `AnmVmInitialize` visibility (open)" above. [2026-11-22]
- **EnemyMotionView::AdvanceMotion @0x44C2A0 (171)**: canonical selected
  17-source `/GL` VC7.1 SP1 cold replay matches the complete PDB-owned 171
  bytes, including all seven bound REL32/DIR32 operands. The original first
  motion branch returns after snapping, whereas the polar branch snaps at its
  tail; a shared inlined `SnapEnemyMotionToHundredths` helper lets LTCG merge
  those tails. Reading the newly published X position through a genuine
  `const volatile float&` makes the compiler reload that field, retaining the
  target `FSTP` and `SUB ESP,8` before multiplication instead of an extra
  live x87 value. This closes the earlier 168-byte / 10-difference candidate
  with natural C++; the independent strict comparator also passes. [2026-11-19]
- **EnemyFireLaser @0x41C510 (153)**: target ESI manager, EDI request, stack
  type, RET4; forms give 157/RET8 or 153 with plain RET. The laser vtable split
  is the likely lever. [2026-11-14]
- **CreateFpsCounter @0x413350 (151)**: exact 151/151 in the real WinMain
  `/GL /GS` graph. The factory allocates/publishes the shared 0x8c FPS/replay
  timing owner, initializes a 0x24 ChainElem, assigns its draw callback,
  registers priority 47 and stores the task. Both independent scoped cold
  builds reproduce all six original-bound relocation fields. [2026-11-17]
  Its adjacent `FpsCounterDrawCallback @0x413690` is now separately exact
  at 10/10 bytes with the REL32 call to `DrawFpsCounter` bound at +4.
  The `src/Main.cpp` __fastcall wrapper is PDB-owned in the real FPS
  draw graph and passes its complete scoped cold replay. [2026-11-17]
- **GameStatisticsOwner ctor @0x42AE60 (132)** and **CreateStatistics42AF20
  @0x42AF20 (97)**: 137 and 98. [2026-11-13]
- **ScriptViewerPopupOwnerView::Initialize @0x42B4D0 (145)** and
  **GameCreateOwner42B660 @0x42B660 (71)**: the current popup graph anchors
  `AnmVmInitialize` as an external, so Initialize takes `this` in EBX instead
  of EAX. Build them in a graph with AnmVmInitialize.cpp; an earlier trial with
  `delete owner;` and that TU reached 141/145 and 67/71. [2026-11-15]
- **th10::Chain::AllocElem @0x449ED0 (74)**: exact. Keeping the
  `ChainElem` self-link publication before both volatile next/prev pointer
  clears restores the target's inlined constructor order within `new`.
  The selected four-TU ScriptViewer popup LTCG graph produces the entire
  74-byte PDB-owned function, with no ordinary byte differences and its sole
  `operator new` REL32 bound to 0x452493. Targeted cold replay is exact;
  Popup Update (134 bytes), ChainElem destructor (12) and Chain destructor
  (103) retain complete strict equality in affected graphs. No ASM or
  padding is used. [2026-11-17]
- **Chain::Chain @0x449AA0 (59)**: still nonexact. Its 59-byte current
  candidate differs at eight bytes associated with the first self-link
  store / second sentinel address scheduling. Before explicitly ordering
  both next/prev clears the constructor was six bytes away, but that source
  left the allocator nine bytes away. Keep these separate and do not claim
  exactness for the constructor. [2026-11-17]
- **ReplayManager::ProcessFrame @0x4297D0 (596)**: [2026-11-17]
  The frame counter now advances through an unsigned 32-bit word and
  bit-preserving `memcpy` instead of a potentially overflowing signed
  `frameCounter++`. This models the target i386 DWORD increment even at
  `INT_MAX`, leaving its playback modulo-30 FPS sampling, input records
  and frame-control handling unchanged. The selected ReplayFrame `/GL`
  source graph reproduces all 596 bytes and all 33 target-bound fields;
  the separate frame callback remains 10/10 and one bound REL32 field.
  Selected two-unit cold replay passes all 606 bytes and 34 fields.
  This repairs overflow semantics, not the already-exact byte count.
- **ReplayPlaybackFrameControlCallback @0x42A3E0 (68)**: the complete
  `src/ReplayManager.cpp` callback naturally inlines its runtime/flag guard
  and frame-skip decision in the real `frontend-options-refresh` graph. Before
  the explicit split the compiler emitted 62/68 bytes with 38 ordinary
  differences. The revised 68/68 body has two bound DIR32 operands and four
  remaining conditional-branch destination bytes at +0x13/+0x18/+0x22/+0x37
  (target shared RET, candidate late return-1). The independent 3,999-byte
  FrontEnd owner stays canonical exact. A fresh 2026-11-17 relink reconfirmed
  the same four branch bytes; the callback remains a nonexact frontier.
  [2026-11-17]
- **th10::leaf::Fn0041FD90 @0x41FD90 (63)**: no callers or stored pointers; its
  EDX input ABI cannot be recovered from a caller.

Systematic seam: retail callers sometimes treat an LTCG callee as opaque (no
register preservation or mod-ref assumptions) where our builds exploit it
(`AnmVmInitialize`, `ReplayRestoreStagePosition`, the popup ctor). Removing
`/EHs` from the callee TU and dropping `noinline` did not help.

## Topic-report residue

- Chain calc dispatcher `@0x449C00` releases the critical section before each
  callback and reacquires it after; its table at 0x449D14 maps 0 unregister,
  1 continue, 2 repeat, 3/4/5 terminate with 1/0/-1, 6 restart, 7 deleted
  callback. It caches the next node first.
- GUI Run's code ends at 0x416F61, then 2 alignment bytes and 43 table pointers
  (24 opcodes, 7+7 stage cases, 5 difficulty).
- `GuiUpdateCallback @0x415AE0` is the independent seven-byte fastcall
  adapter `push ecx; call 0x414900; ret`. The existing ordinary C++
  `GuiView::UpdateStageElements` forwarding wrapper reproduces the complete
  PDB-owned seven bytes and its sole target-bound REL32 field. Two selected
  17-input `/GL` colds reproduce this owner together with exact GUI draw
  (724 bytes) and its separate draw callback (7 bytes), for 738 exact bytes
  and 35 complete target-bound fields in one image. [2026-11-17]
- `GuiDrawCallback @0x415AF0` is the reviewed, seven-byte
  `push ecx; call 0x415800; ret` adapter forwarding the native ECX GUI
  owner to the stdcall draw function. The existing natural C++ callback in
  `src/GuiDraw.cpp` reproduces all seven bytes and the target-bound REL32
  call in the same 17-input graph as the now-exact 724-byte draw owner.
  A selected cold build passes both units (731 bytes, 34 fields).
  The original explicit-versus-compiler-generated source origin is unknown.
  [2026-11-17]
- `GuiView::~GuiView @0x4145F0` uses FuncInfo 0x471740 with seven unwind states
  and the vector destructor iterator for six arrays; it needs `/EHsc`.
- Statistics: factory 0x42AF20 allocates 0x1DCB4 bytes and publishes to
  0x47783C; the constructor clears it, loads the score file, initializes seven
  records and the tail, then calls loader 0x42B030; destruction is at 0x42AEF0.
- Hint entries link at +0x0C, text at +0x1C, count/alignment/base/time/remaining
  at +0x60..+0x70, scale +0x7C; the owner has eight list sentinels from +0x78
  with a 12-byte stride.

## Target facts

### Chain

- Owner is 0x48 bytes with two 0x24-byte elements. Each element has self/next/prev at +14/+18/+1C and an untouched argument at +20.
  - Construction clears only ownership bit0. Insertion runs before the first next element with signed priority >= new, so equal-priority newcomers precede older entries. It locks 0x492274 and increments byte 0x49231C. Main's 0x491BE4 aliases g_MainCallbackOwner/g_Chain.
  - Runners: stack owner, RET4, return the processed count. Next is cached before each callback. Null callbacks are uncounted; callbacks with enable bit 0x2 clear are counted but not run.
  - Callback results: 0 unregisters; 2 retries; 3/4/5 force 1/0/-1 and break. Calc 6 restarts; Calc 7 calls an optional locked delete callback.
- g_Chain 0x491BE4, 48-byte object, two 24-byte sentinels. The ChainElem (12 bytes) dtor clears callbacks +8/+C/+10. Chain dtor 0x438880.

### Hint

- HintReadLine @0x419F40 takes the cursor in EAX and three stack arguments (RET12).
  - The entry is 0x88 bytes, with packed BGRA written at 418D18. AddAutomaticEntry allocates 0x88 and publishes into the current-stage export list before copying XYZ and 64 text bytes.
  - AddCaution @0x424650: ESI owner plus text and position on the stack, RET8. It decrements only positive remaining counts, floors count-60 at 1, and writes time 90, color FFFF8080, remaining 30.
  - AddAutomaticEntry @0x41A120: EBX position plus owner and text on the stack, RET8. The stage global is read twice (+B6/+BE).
  - HintOwnerView::Initialize reads supervisor 0x491C28+0x142.
- Hint writer 0x44B740 returns -1/-2/0 (byte count, not BOOL). Popup Draw uses 723×40-byte records at owner+0x3C4. The Spt Test ECL database is at +0x2D8.
- Hint `ConsumeEntries` 419120: EAX first link, owner/freeEntries on stack, RET8. Effect owner 4776E0+899C; manager 491C10. World creator 448DB0: EDI position, stack resource/ID-out/script, RET12, returns the pointer in EAX.
- Hint `Update`: owner EBX; callback 4198A0 moves ECX to EBX.

### Laser

- The type-1 vtable 0x46DA10 has Draw at +0xC, the box cutter 0x41EB00 at +0x18 and the circle cutter 0x41EFA0 at +0x1C (RET 0xC, frame 0x32C). The box frame is 0x344.
- The type-0 vtable 0x46DA60 has CheckPoint 0x41E4D0 at +0x20 and Bounce 0x41CFD0 at +0x3C. Update calls Bounce under mask 0x08000C00.
- Cutter predicates accept unordered values: the radial test rejects only on ordered squared distance > squared radius, and the box test uses negated XY rejects. Sample Z is cleared after vector addition. Bounce's Y reflection uses ordered `<min` and `>=max`.
- AccumulatePointChecks takes EAX as its receiver, calls the virtual at +0x20 and returns RET 8.
- Type-0 laser (vtable 46DA60, line D58, VMs at 600/9AC, request at 424):
  - RunCommands slot 0: 18 commands of 24 bytes at request+2C (operands, opcode 10, gate 14); signed PC<18. Opcode 4000 calls InitializeForLoadedScript @0x404F30 with VM 600 and resource 4776F0+3E0B50. Angle sentinels are <=-990 / >=990.
  - Initialize: ECX this, stack request, RET4, EAX 0; copies 77 dwords to 424; stateC=2; masks FC63FFFF/00600000 and FC7FFFFF/00400000; secondary VM uses manager 47781C+458, color+103.
  - Motion0040 fields: angular delta 10C, next speed 108, duration/limit/count 11C/120/124.
  - Scale divides sprite width 34 / height 30. Draw writes VM.position 334, not offset 340.
  - Clear (slot 14h): stack mode, RET4, EAX count; samples start at 6 with stride 12 and finish with stateC=1.
- Type-1 laser (table 46DA10, D74 layout, VMs 61C/9C8):
  - Request fields: 18/1C angle/angular velocity, 20/24 max/initial length, 28 width, 2C growth, 30..3C signed durations, 40 type, 42 color.
  - Init copies 126 dwords and sets state 3.
  - Update table at 0x41EA30 covers states 2..5. Update attaches to Enemy 477704 slot 0 (+1068).
  - Clear @0x41F400 returns an integer count.
- FireLaser 0x0041C510: alloc 0xD58/0xD74, type-0/1 ctors 0x0041C5B0/0x0041C680, base init 0x0041C030 (EDX receiver), id at +0x54, manager list +0x434, count +0x438, vtable slot +4. Type-1 collision respawns gaps via zeroed 0x77-dword type-0 requests, speed 8.0f.

### ECL VM

- Host `+0x102C` = script database; host `+4` = active context (instruction at `+4`, time at `+0`). The stack limit is 0x1000 bytes; `ReadInt` with a `-1` operand pops a value word and then a type-tag word.
- `EclVmScriptDatabase::FindSubroutine` returns `header + 0x10`. SCPT sorted entries are 8 bytes. The FORMAT scratch buffer is 0x400 bytes. CALL arguments follow `scalarOperands[1]` as a stream overlay.
- Instructions are variable length. Scalar operands sit at `instruction+0x10+index*4`. JUMP has a signed byte displacement at payload+0 and time at +4. SPAWN_THREAD_WITH_ID has counted inline bytes at payload+0/+4 and computes `(count+4)>>2` as a logical shift. FORMAT has a 4-byte text-length header followed by tagged 8-byte argument records indexed by signed `count/4`.
- Stack slots are four bytes; the first byte is the scalar tag. `ReadInt(-1)` pops two words (scalar and type) after a signed check. The SCPT table has 32 file slots; `AddScriptData` reloads the slot after the magic/version checks. Subroutine records are 8 bytes and are found by binary search.
- EclVmContext is 0x1024: time/instruction +0/+4, nested EclVmStackView (0x1008: data[0x1000], stackTop +0x1000, frameBase +0x1004) at +8, thread id +0x1010, host +0x1014, thread control +0x1018, difficulty +0x101C, flags +0x1020.
- Run @0x0044E1A0: owner 0x0044E1A0-0x0044FD0B = 7020 (code ends 0x0044FBC1, 2 align, 60-entry dest table, 88-byte selector). Checks `float(instr.time) <= currentTime`, difficulty mask, 59 active opcodes in 0x00..0x57, rest to host vtable slot 0. Opcode 0x55 does integer NEG on float storage, pushes 'f'. EAX context, float delta on stack, RET 4. Cases are physically grouped by family (int arith before float, etc.); conditional jumps share JUMP body; one instruction-writeback tail. Typed Pop is inlined per case; Push is a call.
- Pop @0x00450610 (122, RET 4): EAX stack, ECX byte count, BL requested type, one stack out ptr. Push/EnterFrame/LeaveFrame use EAX stack receiver.
- Host 0x103C prefix: active ctx +4, embedded ctx +8, script DB +0x102C, thread-list sentinel +0x1030. Run's only callers are EclVmHost::Run; host Run callers keep host in EDI (EnemyRuntimeUpdate 0x0040E0FE/0x0040E205).
- Opcode 0x51 polar helper 0x004501B0 (30B thiscall FSINCOS) is byte-identical to AnmFloat3View::FromAngleMagnitude @0x004458B0; 0x51/0x52 use EnemyWrapAngle @0x0044BC70.
- StartSubroutine @0x0044DF70: private EAX, two stack args, RET 8; operand-word indexing via `valueOffset/4`, empty-stack zero return word, restore active ctx via `caller->host`.
- Script DB (0x1098 polymorphic resource): AddScriptData @0x00450220 validates SCPT, ≤32 file buffers, header 0x24, sorted 8-byte name table; derived vtable 0x0046D0B4 (slots: AddScriptData, package loader 0x0040D400, file loader 0x0040CD20), base 0x0046D0F0 (AddScriptData, 0x0040C810). Owned script pointers +0x0C..+0x88, lookup table +0x8C, nested dwords +0x1090/+0x1094.
- Opcode 55 negates the 32-bit raw word (NEG EDX at 44F851) after tag conversion and pushes it with float tag 66h.
  - Run loads the instruction into EAX for the time comparison, then separately loads EDI at body+40.

### Enemy

- EnemyManager `activeEnemyCount` is at +0x60 (`DEC [ECX+0x60]` in the 386-byte enemy-object destructor; `INC` in Spawn). `spawnCounter` also wraps.
- `EnemyConfigureInterrupt` records are 16 bytes at EnemyFullObjectView+0x2494; the timer callback is at `(slot+0x24A)*0x10`.
- `managedVmIds` has 10 entries; script indices into it are unchecked. `loadedAnmResources` has 4 pointers, also accessed with unchecked indices.
- Dispatcher details:
  - the pattern clear is `LEA EDI,[EDX+0x2C4]` then `REP STOSD` of 0x84 dwords at +0x19DE;
  - the pattern-initialize record is 0x210 bytes; there are 22 overlapping extra records of 24 bytes;
  - the spell-name cipher XORs with a key that adds 0x10 per byte as an unsigned char; its SIB `8A 0C 31` is at +0x273B.
- ECL dispatcher source decisions retained on 2026-11-18 (VC7.1 SP1 /GL):
  - `dispatch_copy_player_to_motion` copies the 12-byte XYZ position record with `memcpy`. This is a bit-preserving transfer (including NaN payloads and signed zero), not vector arithmetic; the bounded main owner improved by 27 differing bytes after this change.
  - `ENEMY_ECL_INITIALIZE_BULLET_PATTERN` clears the full 0x210-byte pattern, then publishes its count, angle/speed bit patterns, sound and flag defaults. A temporary value-initialized pattern assignment expanded the owner; a local declared directly in the unbraced switch case also triggered MSVC C2360 at following labels. The retained implementation is the complete `memset` followed by explicit field stores.
  - `ENEMY_ECL_COPY_BULLET_PATTERN` uses a complete `EnemyBulletPatternView` assignment. The two unchecked script indices can be equal, so replacing it with `memcpy` would give overlapping/self-copy undefined behavior.
  - Type-0 and type-1 laser request paths copy their serialized pattern-word arrays into the corresponding request layouts. Type-1 carries 0x6C DWORDs; keep the offsets and sizes tied to `EnemyLaserType0RequestView` / `EnemyLaserType1RequestView`, not to a guessed common struct.
  - `EnemyManagedVmIdAt` centralizes the four-byte managed-VM handle overlay. Script-selected slots are unchecked and use modulo-32-bit address arithmetic; only the SET_FLAGS/CLEAR_FLAGS loops have a proven 0..9 bound.
- Current bounded dispatcher measurements: the primary 36-TU `/GL` graph is
  14,452/14,416 bytes with 10,414 differing bytes; the secondary 12-TU `/GS`
  graph is 14,456/14,416 with 10,240 differing bytes. Both report `REGRESSED`
  because residual offsets moved from the stored sets. Neither is exact; do not
  update the frontier merely to bless offset movement.
- The interpolation timer is 20 bytes (`INC` at +0x3B, `DEC` at +0x99 in the 652-byte evaluator).
- Bomb owner: Owner `+0x2C` is the VM id; `+0x28` is the mode; `+0x3C` is a float radius.
  - `EmitAuxiliary` tests `spellId == 109` as an OR alternative to `(flags&1) && 93<=spellId<=96`, re-reading the id from game state for the 109 test.
  - Cancel callee `0x408100`; laser propagation `0x41C800`; data `0x4776F0/0x4776F4/0x47781C`.
- Bullet-pattern records are 0x210 bytes. Pattern offsets at +0x1344 are 12-byte xyz entries. The opcode-0x199 extra records are 24 bytes, 22 per pattern. Life is at +0x13C0 (`SUB`); the damage-flash counter at +0x1414 is decremented as a signed value. The circle opcodes copy motion +0x20/+0x24 as raw binary32 bits.
- EnemyMarkPendingInterrupt is @0x00409E50.
- Full object 0x2518 (EnemySpawn allocs); runtime tail at +0x103C, size 0x14DC, owner back-pointer runtime +0x14D8. Runtime: four 0x2C motion records +0/+0x2C/+0x58/+0x84; collision pairs +0xB0/+0xB8; ten VM ids +0xC0 (update uses 8); spawn params +0xFC (4 int then 4 float, writable lvalues); timer +0x11C; list node +0x130; two 0x4C pos interps +0x13C; four 0x3C scalar interps +0x1D4; reward/life +0x13BC/+0x13C0; drop type +0x13CC, 11 counts +0x13D0; 32.0f at +0x1400/+0x1404; death sound/effect +0x1408; flash +0x1414; timers +0x141C/+0x1430; flags +0x1444; manager slot +0x1450; eight 0x10 callback-threshold records +0x1458.
- Enemy derives from a 0x103C ECL host (base vtable 0x0046D0D8, five slots + deleting dtor 0x0040C7B0; derived 0x0046D0C0, deleting 0x0040CC50, virtual delete via slot +0x14). Execution ctx full +0x008..+0x101F (0x1018); full +4 active ctx; resolver at ctx +0x1014 = Enemy; alloc list +0x1034; mirror +0x1030. Instruction: opcode +4, operand mask +8, operands from +0x10.
- EnemyManagerView 0x68: timer +0x40, script DB +0x54, list head/tail +0x58/+0x5C, count +0x60, spawn counter +0x64; callbacks priority 0x12 (update 0x0040D810) / 0x14 (draw 0x0040D820), 0x24-byte nodes with owner at +0x20.
- Dispatcher @0x0040E770: ECX runtime, RET; opcodes 0x100-0x1B4 (124 active); owner 14416 to 0x00411FBF (code ends 0x00411D2E, 108-entry table, 181-byte selector, two 5-entry difficulty tables at 0x00411F98/0x00411FAC). Laser request scratch at ESP+0x50, spawn request (0x84) at ESP+0x248 below cookie.
- Enemy ANM script state is runtime +0xE8..+0xF8 (five DWORDs): selected resource (+0xE8), primary VM resource/script (+0xEC/+0xF0), base script (+0xF4), and direction (+0xF8). `SELECT_ANM_RESOURCE` writes the selector; `SET_ANM_SCRIPT`/`SET_MAIN_ANM_SCRIPT` publish the primary pair; `PLAY_SELECTED_ANM` and `EnemyRuntimeUpdate` consume the base script/direction; `EnemySpawn` selects the death effect from the primary pair. Source documents the contiguous words with `EnemyAnmScriptStateLayout` (0x14 bytes) and size/member-offset assertions; `EnemyRuntimeView` exposes descriptive direct members so the dispatcher retains its existing LTCG lowering. Targeted exact replays match `EnemyRuntimeUpdate` 2412/2412 and `EnemySpawn` 577/577 (2,989 bytes total).
- Enemy bullet-pattern records are 0x210 bytes and share the `BulletSpawnDescriptorView` wire layout. The former +0x20 opaque block is now 18 shared `BulletTransformRecordView` records (0x18 each), followed by laser start/end offsets, start length, width and five timing fields through +0x1F3. Tail members +0x1FC..+0x20C are `transformFlags`, `spawnSound`, `transformSound`, `transformStartIndex` and `templateSprites`; +0x1FA remains an unclassified halfword. ECL opcode 0x199 uses a 22-record overlapping 0x18-byte view beginning at +0x20, so records 20/21 intentionally cross the pattern tail and next-pattern boundary. The opcode now writes the shared transform payload/kind/allow fields rather than a duplicate provisional record; cross-view offsets are asserted.
- Adapters: ReadIntArgument 0x00412A00 (EAX runtime, ECX index; 30 direct dispatcher calls, others inline), ResolveIntArgument 0x00412A10 (9 calls), ReadFloatArgument 0x00412A60 (no callers), ResolveFloatArgument 0x00412A70 (tail-jumps to 0x00450070).
- Operand readers normalize negative selectors +10000 (51 entries); last four selectors test difficulty == 0..3. Globals: difficulty 0x00474C74 (0..4), rank 0x00474C98 (clamped ±0x400, = chapter-state 0x00474C40 +0x58), stage 0x00474C7C (+0x3C), chapter 0x00474C84 (+0x44; EnemySetChapter 0x00412ED0 writes +0x44, may reset +0x4C), g_Player 0x00477834, g_EnemyGameState 0x004776F4, laser manager 0x0047781C, global 0x00474C4C (−3000 floor 5000 on timeout).
- Rank interpolation constants 1024.0f and 1/2048.0f. Movement: 19 FCOM refs to -999999.0 (qword 0x470D58) sentinel; select argument only if ordered and > sentinel. pi 0x470B18, 2pi 0x470B14, 0.5 0x470B0C, 0.0f 0x00470B04, pi/2 0x00470B94. MOVE_RANDOM multiplies pi then 0.5.
- Callback core 0x004127A0: eight records at full +0x2494; timer publishes seconds (≤99) to +0x9EC0; clears bit 1 in eight Player words on timeout.
- Spell start 0x00409280: four stack args RET 0x10, owned extent 2432 incl. NOP + 7-entry table 0x00409BE4 (switch on 0x00474C7C-1); Ghidra decompile drops private args (QueueSoundSample EDI id 14).
- Spell bonus fields: current 3790, initial 3794, duration 3798; BeginSpell caps only 3794. Digit calls @0x43E5A0 are SetSprite.
- `ReadIntOperand` 411FC0: a 51-entry table at 412228 gated by unsigned(sel+10000)<=50. The VM selector calls GetVm 449450 with ESI and reads signed short +38A.
- `ReadFloatOperand` 412350: 679 code bytes, NOP, then a 204-byte table at 4125F8.
- `EnemyRuntimeUpdate` literals: -192, 192, 0, 448, ±0.1.
- Teardown 40D530 (EAX): 32 script frees, ANM slots 9..12 released through the slot protocol.
- Manager fields: +0x38 = effectResources[2] (stage ANM), timer +40, list +58/+5C.
- Spell history rows are 0x437C bytes with 110 entries of 0x90 from row+0x59C (slot 6 = 0x19A8C). Captures +0x80 (EndSpell), attempts +0x84 (BeginSpell).
- `EnemyBeginSpell` sets flag 0x40000000 and layer15.
- Dispatcher: 181-byte opcode map onto 108 handlers; main table at +0x35C0. Rank-3 thresholds ±512, rank-5 ±600/±200.
- Callback: `OR [ecx+0x2480],0x10000` precedes the timeout-global read. Player flags at +0x378C/+0x3790.

### Bullet

- Bullet rows: 2000 from manager+60, stride 0x7F0. Cancel distance uses XY only, but full XYZ differences are stored. Bounds margin 2. The resource pointer for these paths is 0x4776F0 (resource+3E0B50).
  - ExtendFaithTimer has 9 direct call sites. RngView is at 0x4918B0.
  - Enemy laser:
  - FireLaser @0x41C510 has 10 call sites, 4 in the dispatcher.
  - Opcodes 1AF/1B0 copy 108 dwords from runtime+2E4 into request+2C/+48.
  - Dispatcher:
  - Opcode is loaded as 16-bit and sign-extended (target MOVSX ECX,AX), with the packet in ESI.
  - Groups 0x100/0x101 are 403/81 bytes, and 0x101 jumps back into 0x100's spawn call.
  - Opcode 1B4 makes three ReadInt calls on every path.
  - Aim at 411C5A ends with a jump into the 49-byte rank-five result tail at 411956.
- 2000 bullets with stride 0x7F0; draw buckets are six heads and six tails.
- DrawBucket uses a private ABI (EAX = manager, ECX = bucket) and reads `manager + 0x14 + bucket*4`.
  - world offset X = 0x470B4C, Y = 0x470B48;
  - AddNormalizeAngle = 0x44BC10; render-manager global = 0x491C10; ANM Draw = 0x4451C0.
- Transform-program records are 0x18 bytes. The large `AdvanceTransformProgram` owner in `src/BulletTransform.cpp` has 0x5AE code bytes plus local switch/selector data in the configured 0x644 owner extent. The latest current-source linked image is 0x640 and fails the complete-extent gate; the strict comparator cannot assess the full owner. See `RE_HANDOFF.md` for the current register-lifetime and descriptor-store investigations. This owner remains nonexact.
- The pool holds 2001 BulletRuntimeView records with a 0x7F0 stride, at manager +0x60. SpawnSingle +0x152 computes `LEA EAX,[ECX+1]` and then a signed `/2` for the alternating aim. UpdateVerticalWrap takes EBX as its receiver.
- Runtime transform program: ≤18 0x18-byte records at +0x464, active flags +0x43C, cursor +0x45C, nine 0x34 exState slots at +0x614. Cancel pool 2000 × 0x7F0 rows, position +0x3B4. Wrap bounds Y [0,448] vs cull [-64,448].
- Bullet callback 0x00406770 is a single 40-byte owner (0x0040678E is an interior tail entry).
- Bullet reset @0x405ED0: MarkLoadedVmsForDeletion(+0x3E0B50), clears 0xF82BC dwords from +0x60, stores array at +0x10 and sets word +0x3E07A6 = 5.

### Item

- Items: the manager is 0x21CEC0 bytes and holds 0x896 records of 0x3F0 at +0x14; global at 0x477818. Record fields: pos 3AC, vel 3B8, timer 3C8, active 3DC, kind 3E0, spriteKind 3E4, attractSpeed 3E8, delay 3EC.
- Item clear: 0x896 (2198) records of 0x3F0 bytes from +0x14 (decimal 896 was wrong). Item Spawn @0x41BB00: point in ECX, receiver in EAX, stack 8/FFFFFFFF/-pi/2/0.6, RET16, manager at 477818.
- Spawn rows: 150 normal rows plus 2048 delayed kind-8 rows. Power >= 100 converts kinds 1/4 to 9 and 10/11 to 5.
- Registration uses priorities 21/25 with nodes at +8/+C. The update gate tests `((f>>2)|f)&1` or bit 10 of +0x58.
- Physics: gravity 0.03; attraction speed +0.2 per frame up to 12.
- Point value = trunc((playerY-144)*(1/304)*(faith*0.5-5000)). Collect plays cue 20; a power level change plays cue 29.

### Player

- `Player::CalculateDamageToTarget @0x428630`: private ABI with ECX as a nullable optional hit-flag pointer; `Player*`, target position and target size on the stack; `RET 12`. The source uses a 4-byte `DamageTargetView` overlay.
  - `EnemyAdvanceMotion @0x44C2A0` is an ESI-receiver operation on the 0x2C-byte motion record.
  - `EnemyRuntimeUpdate` keeps ten raw VM-ID DWORDs, accessed through a trivial 4-byte slot view, and uses a 0x14-byte enemy timer view with a volatile scale read.
  - The `.sht` option data stores relative pointer words.
- Player +0x3504/+0x3508 tracked-enemy pointer/validity; shot rows at +0x49C stride 0x5C, +0x4C trackedEnemy.
- Option history: 8 samples (64 bytes) per option, cursor `optionIndex << 3`. The history-loop threshold is `optionCount << 3`, compared signed.
- Option records are 0x98 bytes, indexed by the signed byte shot `sourceIndex`.
- State zero computes `timer*8000/60` (wrapped IMUL, then signed divide).
- Option interpolation is `(dst - prev) * transitionFrames / 100`.
- Player (`sizeof==0x4478`; global `g_Player`; factory `new(0x4478)` -> ctor (ESI live-in, clears 0x111E dwords) -> `PlayerInitialize` (EBX live-in); dtor stack Player/RET4 with SEH):
- The 0x34-byte `.sht` descriptor has schedule bytes at +0/+1, signed damage short at +2, spawn offsets at +4/+8, hitbox at +0xC/+0x10, angle/speed at +0x14/+0x18, signed source index at +0x1C, type at +0x1D, and callbacks at +0x24/+0x28 (update, ECX=Player EDX=row)/+0x30 (hit). Type-3 shots use occupancy ints at `+0x42F8[sourceIndex-1]`.
- `Player::CalculateDamageToTarget @0x428630` uses a rotated private ABI: output pointer in ECX, Player/pos/size on the stack, `RET 12`. Draw offsets are 224.0f/16.0f. 0.125f is at 0x470B90, pi/2 at 0x470B94, 0.0f at 0x470B04.
- `RebuildPlayerOptions @0x426F70` (stack Player, RET4): jump table indexed by `character*3+shot` (0-5) at 0x427930. It has nine call sites in five callers.
- `PlayerUpdateMovementAndOptions @0x4250B0` takes Player in live-in EDI and returns 0 with plain RET.
- Player: SetCurrent timer 0x405410 is EAX receiver + stack value/RET4; respawn writes 480.0 (0x43F00000); effect spawner 0x427B50 picks first inactive of 32 rows (storage has 33). CreateVmVariant0 @0x448D00 returns AnmVmIdView via hidden aggregate. SetVmWorldPosition @0x449350 adds 224/16.
- Player unwind @0x465280 destroys ANM VM @0x401FF0 at +0x14; draw-VM buffer at VM+0x358. Popup has VM at +0x18 and buffer at +0x370. PlayerHandleCollisionHit @0x426CF0 takes a stack argument and returns RET4.
- PlayerTransitionToState2 is at 0x4269D0, called from 425AA4. When spellTimer >= 60, it clears frontVm indices 0, 1 and 3..7, keeping index 2.
  - Pending helpers: VM IDs start at Player+4E0, stride 5C.
- the VM is at Player+0x14 and the resource at +0x10; optionData is at +0x45C. The .sht table at 0x476FC0 has 6 pointers (pl00a..pl01c). Of the color offsets, secondaryColor maps to +0x300 (primary +0x2FC → +0x300 was wrong).
- Main `PlayerUpdateCallbackBody` 425730..426357: 3089 code bytes (29 alignment), 3 unreachable LEA bytes, then a 20-byte five-state table at +C14 (indexed JMP at 42574A). State entries are +21/234/379/5F5/366. Stack Player, RET4. 470AFC is .rdata float 1.0, used as a memory FMUL operand in the scaled decrements (+6E0, +760) and read by fast decrements and increments. 477830 is the fallback object. Player global 477834 is stored only by ctor/dtor (71 direct reads).
- Movement 4250B0..425727: 1591 code bytes, 1 NOP and two 32-byte direction tables (16 entries). Caller 425A8C passes Player in EDI; the body returns 0 with plain RET. At +317 it calls script2, then rereads Player+44C and tests it >0 for script4: four logical selections, three physical calls.
- `PlayerHandleCollisionHit` 426CF0: stack Player, RET4. It creates script162 plus 32 script163 effects at the draw position +(224,16,0), conditionally queues sound4, starts draw script0, and invalidates bonus/front VM slots except slot2 when spell timer >= 60. MainSoundOwner is object storage, not a pointer.
- `CheckLaserContact` 4267F0: ECX Player, EAX origin, three stack floats, RET12.
- Draw body 426360: writes VM+340/344/348 (positionOffset), i.e. Player+354..35C. Bit7 of byte+54 is the sign gate. Adapter 426510 moves ECX to EAX and tail-JMPs.
- `CalculateDamageToTarget`: ECX optional-hit pointer; stack Player, target position, target size; EAX result; RET12. Sole caller 40E13C.
- `GetDamageAtPosition`: ECX owner 4776EC, EAX position. Globals: 4776F4 game state, 477704 gate, 474C68 int. Results are 3, 38, 38/5 (gate), 23/26, 20/16 or 0.
- `GetPlanarBounds`: ECX center, ESI min, EDX max, EAX extent; 0.5 at 470B0C.
- Shot rows 428160: unsigned16 power 474C48 /20, clamped to 4; optionMode+5; .sht+110. Descriptor rows are 34 bytes, ending at a negative interval. `SpawnShot` takes 3 stack values, RET12. `UpdateShots`: stack Player, RET4. Shot callbacks get ECX Player, EDX shot. There are 128 shots and 32 effects.
- `StartPowerTransition` 405860: stack owner, RET4, returns -1/0. Timer14, state28, VM2C, XYZ30, radius32/value40=4, scripts 400/407/441, sound38, rank clamped at -128, mode44.
- Option reset flags are at Player 332C/33C4/345C/34F4; draw position 3C0/3C4; signed coordinates 3CC/3D0 scaled by 0.01f (470B00).
- `g_AnmGameSpeed` is 476F78.
- `+0x08/+0x0C`: update/draw callback nodes (0x24-byte nodes; callback at +0x08, owner at +0x20). Priorities 0x10/0x16. Update entry 0x426500 does `push ecx; call 0x425730; ret`. Draw entry 0x426510 does `mov eax,ecx; jmp 0x426360`.
- `+0x14..+0x3BF`: embedded draw VM (0x3AC bytes; VM-relative +0x358 is an owned pointer freed in the dtor). `+0x3C0/C4/C8`: float draw position. `+0x3CC/+0x3D0`: fixed position, clamped to [-18400,18400]x[3200,43200]; draw = fixed*0.01f. `+0x3D4/D8`: mode 0/1 axis speeds. `+0x3DC/E0`: diagonal speeds. `+0x3F0/F4`: fixed deltas. `+0x458`: state (draw skipped when 2). `+0x45C`: option/shot resource. Timer records at `+0x460/+0x474/+0x488/+0x430C`.
- `+0x49C`: 128 shot rows of 0x5C. Each row has a timer at +0x00 (constructor flag +0x10), motion at +0x14, state at +0x40, VM ids at +0x44/+0x48, collision fields at +0x50/+0x54 and an `.sht` descriptor pointer at +0x58.
- `+0x329C`: mode VM handle (layer 9, script 0x160). `+0x32A0`: four 0x98-byte options. Option fields: state +0, pairs +0x34/3C/44/4C, VM ids +0x68/+0x6C, ctor flag +0x80, prior mode +0x84, index +0x88, reset flag +0x8C, update callback +0x90, draw callback +0x94 (producer unknown). `+0x3500`: option count = clamp(power/20, 4).
- `+0x350C`: 33 effect rows of 0x6C (only 32 are updated). Row fields: radius/delta +0/+4, angle/delta +8/+0xC, extents +0x10/14, pos +0x18, velocity +0x24, timer +0x44, damage/accum/cap/interval +0x58..+0x64, flags +0x68. `+0x4308`: transition counter (interpolate when >=30). `+0x4324`: six Float3 rows (collect rectangles). `+0x436C..0x4473`: 33-pair history. `+0x4474`: mode.

### GUI

- **GuiView::UpdateStageElements @0x414900 (3,836)**: [2026-11-17]
  its frame-timer fast path now increments the stored `current` word
  using unsigned modulo-32-bit arithmetic and bit-preserving `memcpy`,
  so `INT_MAX` wraps to `INT_MIN` just as the target i386 `INC` does.
  This eliminates undefined signed overflow from the natural C++ source
  without adding ASM, padding or changing the prior compiler schedule.
  The real 16-support-TU linked graph compares the complete 3,836-byte
  function and all 72 original-bound relocation fields as exact; a
  targeted selected VC7.1 SP1 `/GL` cold replay independently confirms
  3,836/3,836. This is a semantic repair, not newly recovered exact bytes.

- `0x474C48/0x474C4C/0x474C70` are fields of one escaped globals object at `0x474C40` (the `Add` receiver), not separate globals.
- `GameCreateOwner42B660` is `new` (null-checked constructor call), `Initialize`, and on failure a null-checked destructor plus `operator delete`, which is what `delete owner;` emits for a non-virtual destructor.
- `GuiMessageVmView::Run` indexes 4-byte `textColors` and 12-byte `textPositions` by `message->activeSide`.
  [2026-11-17] All 36 portrait sprite script IDs in the 4,480-byte
  GUI stage-message interpreter now add the literal sprite offset as
  an unsigned i386 DWORD before converting back to the signed script
  argument. A single ordinary `__forceinline` C++ helper preserves the
  actual field read and native integer wraparound without ASM or padding.
  Rebuilding the 16-support-TU `gui-message-run-candidate` graph yields
  4,464/4,480 bytes and 1,935 differences, identical to its fresh
  baseline (still nonexact and REGRESSED against the older 1,910-byte
  residual ledger). The independently linked 3,836-byte GUI stage
  updater remains exactly matched with all 72 target-bound fields.
  This is a semantic overflow repair, not extra exact byte credit.
  [2026-11-17] `GUI_MSG_SET_SIDE_0/1` now uses a typed
  `GuiMessageSideTextPosition` accessor for the second text-line VM.
  It retains the original unchecked, wrapped 32-bit address and real
  12-byte `textPositions` stride rather than repeating two raw pointer
  casts. The current 16-TU message graph remains 4,464/4,480 bytes
  with 1,935 differences; the separate GUI stage updater passed its
  complete 3,836-byte / 72-field strict comparison and selected cold
  replay. Rewriting the five color accesses through a shared helper
  shortened the message owner to 4,460 bytes and was rejected.
  [2026-11-17] The five message text-color reads now use the actual
  `GuiMessageVmView::textColors` array instead of repeated integer
  base-address and stride casts. `activeSide` starts at zero and is
  explicitly set to zero or one by the two side-selection opcodes.
  The complete 16-support-TU interpreter candidate stays exactly
  4,464/4,480 bytes with the same 1,935 differing offsets as before.
  GUI stage Update 3,836/3,836 (72 fields) and Draw 724/724
  (33 fields) retain strict whole-owner comparisons after rebuilding
  both affected TU graphs. This is source recovery, not new exact credit.
- Persistent stage counter address: `(stage*3+shot)*0x437C + 0x4D0 + difficulty*4`.
  [2026-11-17] The shared `GuiStageClearCount` helper now expresses this
  as native unsigned DWORD arithmetic on the serialized stage, shot and
  difficulty words before flat 32-bit base-address addition. This removes
  signed-multiplication/offset overflow and out-of-array pointer arithmetic
  from the C++ address derivation; it preserves the target modulo-32
  indexing, including malformed inputs. A focused 16-support-TU
  `gui-message-run-candidate` relink remains 4464/4480 and 1935 residual
  bytes (still nonexact), while the corresponding 3836/3836 GUI stage
  updater strict-linked guard stays exact. No new exact credit.
- Stage-bonus score terms:
  - `g_GuiPowerLevel * {20M, 25M, 35M}` and `g_GuiPowerLevel * 40M`;
  - `g_GuiPower * {100k, 200k, 300k, 400k}`;
  - `g_GuiCounter4C4C * 1000`.
- The next instruction is at `base + 4 + sizeByte`.
- EnemyReadDialog has a private ABI: EDI = GUI receiver, ESI = script index.
  - Dialog records are 8 bytes.
  - `g_EnemyDialogIndex` = 0x00474C84; dialog-state global = 0x00474C8C.
  - operator new = 0x00452493; GuiMessageVmView constructor = 0x00415B00.
- Popup owner 0x477840. Power digit display 0x4054B0 uses ESI owner, EDX value and stack percent.
- GUI: the GuiView is 0x9ED0 bytes and the message VM is 0x90. A message instruction is `u16 time, u8 op, u8 size, payload`, with opcodes 0-23. `GuiSetLivesDisplayCount` uses EAX owner and ECX count over 0x3AC-stride slots.
- the stage-7 HUD selector (table @0x4157E0, entry @0x414E71) uses a signed state CMP 24, picking script 0x8C below 24 and 0x8D otherwise. Countdown: values 0..5 set interrupts 9 and queue sound 0x24 @0x415501; 6..10 set interrupts 8 and queue sound 0x1B; >=11 makes no sound; an increase sets interrupts 7. Counter 474C4C is captured before ExecuteScript. Message cleanup walks six slots +40..+54 with deletion bit 0x04000000. Gauge hide clears four interleaved value fields plus final +9EAC.
- `UpdateStageElements` 414900: 3805 code bytes, 3 alignment bytes, 7-pointer selector table at +EE0. Sound-object immediates sit at +BFD/+C30.
- `DrawStageElements` 415800: stack GUI pointer, RET4; registered callback 415AF0. Boss flags are tested as a DWORD (bit4 shifted and inverted, then bit0).
- Message `Run` 415E90..41700F: 4306 code bytes, 2 alignment, then 24/7/7/5 selector tables. Physical opcode order: 14,15,16,17,3,1,2,22,23,4,5,6,7,8,9,12,13,10,11,18,19,20,21,0. 150 direct calls, 54 hidden EDI stack outputs (18 VM creations, 36 child lookups). It calls the transition factory with priority31 in EBX and 0/0/0/120/5 on the stack.
- GUI reuse 414570 (EBX). Message teardown 4136C0 handles ids 40..54 in six triples.

### Input

- `MergeJoystickInput @0x44A190` takes `(unsigned short buttons /*ECX*/, int bindingIndex, int padIndex=0)`, with the last two on the stack.
- Key binding shorts at controller+0x59CC..0x59D4; key-config display labels 0x43..0x56, digit base 0x33, signed-short /10 and %10.
- Debug input (@0x40AC20): clears +4, 16 u16 counters at +0x38; at 26 subtracts 8 and ORs only bit 0 into +0x30. Edges are +0x32/+0x34.
- Gameplay input: stride 0x6A at 474E30:
  - 0/2 current/previous, 4 repeat, 6/8 pressed/released
  - +A sixteen counters, 2C..57 debug block, 58..69 nine signed bindings
  - WinMM joystick ID is bool(padIndex); raw button query @0x44A4E0 always uses ID 0. Failed DirectInput Poll: Acquire once plus up to 400 retries, never fetching state. Merge zeroes a 272-byte record. Flags 200/400 select the keyboard/pad backends, and 4924FC gates the keyboard.
- Input object 0x474E30 has current/repeated/pressed at +0/+4/+6. Key mapping 0x474E88 is nine shorts with defaults (0,1,2,3,-1,-1,-1,-1,4); persisted storage is 0x491D4C.

### Sound and MIDI

- Sound queue: only `count >= 128` is rejected, so negative counts pass. soundId metadata records are 8 bytes, with the cue at +6.
- Sound queue: QueueSoundSample 0x0043DC90 (ECX queue, EDI id, stack sample RET 4); QueueSoundCue 0x0043DD10 (ESI queue, EBX id, float pos ×5.2083335). 12 slots, 128 samples, cue table 0x004749C8+6.
  [2026-11-17] The first-free-slot branch does not validate the old signed
  sample count, unlike the existing-ID branch which rejects `count >= 128`.
  Both new-slot count increments now perform unsigned DWORD `+1` followed
  by bit-preserving `memcpy` to the signed field. This models the retail
  unconditional i386 increment at `INT_MAX` without signed-overflow UB.
  A fresh four-source `EnemyDispatcherHelpers` linked-PE graph retains both
  exact whole owners (123/123, 149/149) and all four total linkages. The
  refreshed 36-support-TU parent retains exact `PlayerHandleCollisionHit`
  (627 bytes); the 14,416-byte dispatcher remains nonexact, and its residual
  count is 10,570 in the current graph.
- SoundPlayerView: 32 command slots at +0x1F88, stride 0x10C (opcode/arg/step/path at 0/4/8/12), QueueCommand @0x43E460 scans only 31, drops when full. FrontEndLoadMusic @0x420A90 replaces three extension bytes after last dot; FrontEndPlayMusic @0x420B10 publishes profile track byte even when the queue is full.
- LoadBgm keeps the 16-bit alignment in EBP and uses `DIV EBP`.
- BGM format records are 0x34 bytes; there are 16 preload slots; command records are 0x10C bytes.
- `CWaveFile::Read` (202) has its only ReadFile import call at +0xAD.
- BGM format records are 0x34 bytes. A RIFF chunk is consumed as size+8 bytes with no padding. The sound list has 37 names.
- Sound (`g_MainSoundOwner @0x492590`, view 0x52D0): BGM volume 0x497854, loader allocations +0x5230 (37 se_.wav), state +0x5224 (2 = cancel), finished flag +0x522C, thread handles +0x5218/+0x521C, BGM event +0x520C, BGM flag +0x5208. The load-thread handle is at 0x4977AC.
- `CSound` is 0x5C bytes: DSBUFFERDESC at +0x34, manager at +0x58. `CStreamingSound` is 0x78 bytes. `CWaveFile` is 0x94 bytes: flags +0x78, HANDLE +0x8C, format +0x90. Vtables: CSound 0x4705D8, CStreamingSound 0x4705D4. The BGM format row is 0x34 bytes.
- Sample buffer flags are 0x80C8, and LoadSoundSample copies 18 format bytes.
- Parser @0x43B110 is 1670 bytes (1323 code + tables).
  - Tempo uses IMUL 0x101.
  - Clock parameters are sign-extended to u64.
  - Timer base is 16 bytes; vtable 46F810 slot 0 is default Tick @0x43B8B0, and derived slot 46F80C names Tick @0x43AF30.
  - Trampoline @0x43A6E0 dispatches vptr slot 0.
- ProcessQueues 0x43DDF0 is 1452 owned bytes, with an 8-entry table and selector opcode-1 ≤ 7. PreparePlayback metadata is 47 rows of 8 bytes at 0x4749C8. Volume attenuation reads signed bgmVolume but the zero gate tests sfxVolume (keep it). Fades in 0x421E00 use current CSound+0x5208. SetPrimaryBufferFormat's private ABI is stack owner, EBX channels, EDI rate, ECX bits.
- `QueueCommand`: EAX owner, EDX path, opcode/arg on stack, RET8. 0x10C-byte records at owner+1F88, slots 0..30.
- Dummy music 420C00 uses opcode 4 or 3 with dummy 46DF98.
- MusicRoom 433EF0..4349DB ends with a four-word table at 4349CC.
- `SkipLine`: EAX cursor, EDX remaining. `CopyLine`: ECX cursor, EDI remaining, stack destination, RET4.

### Replay

- `SaveReplay`: ECX owner, EDX path, stack name, RET4.
- Fields: mode +0x10, header +0x14, data +0x18, eight stage headers +0x1C, frame lists +0x3C, eight 0x24 stage states at +0xA0, payload +0x1C0, FPS +0x1C4, path +0x1D4.
- A frame block is 0x6284 bytes: 3600 6-byte records plus 3600 FPS bytes.
- File magic is 0x72303174, version 5. Encryption is (0x3D,0x7A,0x80) then (0xAA,0xE1,0x400); decryption reverses the order.
- Replay Chain priorities: calc 0x0B/0x1B, draw 5.
- Screen 16 is replay save (update @0x433570 with a 5-entry table at 0x433B10, draw @0x433B30, both RET4).
  - Exactly 25 objects are destroyed but 50 pointers are cleared.
  - QueueSoundSample @0x43DC90: ECX owner, EDI sound, stack sample.
  - Replay callbacks take ECX owner (Chain fastcall); the 68-byte adapter uses runtime+58 bit 4, input 474E30 bit 0x100 and signed frame%4.
  - FPS display: game ptr 477810, byte +1C4 drawn at (383,450) with `%3d`; red below 30, yellow below 50.
  - Title entry @0x42D300: 268 code + 20 table bytes. Menu @0x42D420: 1195 code + NOP + 84 table bytes; Extra is gated by profile bytes 1D888..1D88D; items route to 6/6/6/12/11/14/4/3. Title resource at +18, auxiliary VM ID at +684.
  - Result rows @0x422660: four pointers at 0x422A6C plus ten selectors (phase-10, bound 9). ASCII+8988 is the draw-shadow field. Name grid uses 13 columns, pitch 18/16, glyphs 81/7F/80.
- Replay frame list: stride 0x0C, base 0x3C. Frame allocator 42AA50: ESI stage, stack manager, RET4, 6284 bytes.
- `BeginStage` 42A450..42A695 includes a skipped 3-byte LEA at 42A60D. Playback pair copies are a counted 4-iteration loop followed by the reset-flag writes. Character/shot at +28/+2C are read inside the playback loop.
- `ProcessFrame` 4297D0: EDI. InputState 474E30: hold 2A, current/previous words 2C/2E, masks 32/34. Calls `UpdateDebugRepeat` 40AC20. Samples every 30 frames.
- `AppendRecord` 42A8A0: EAX frame, EDX input, two stack args, RET8. recordEnd at +5460. Returns count>=3600.
- `GameRestoreReplayState` 42A6A0: EBX manager. RngView 4918B0, Main 491C28, GameScoreState 474C40.

### FrontEnd and result menu

- `UpdateReplaySave` 433570: 1440 code bytes plus five table words at 433B10.
- `ResultMenuUpdateResult` has a 0x4C frame: `char replayPath[64]`, two temporary DWORDs (one is the 25-item replay-list cleanup countdown) and the `/GS` cookie. The state-8 VM id is built through hidden-return storage `[ESP+0x14]` before `ResultMenuPendingSix`.
  - States 11 (replay name) and 12 (score name) are separate switch bodies.
  - The table index is `shot + character*3`.
  - `GuiView::value9E78` holds the published score.
- ResultMenuUpdateResult has a /GS frame of 0x4C.
  - State 6 calls `QueueSoundSample(owner=ECX, EDI=10, stack sample 0)`.
  - State 10 releases 25 replay-file objects, trims up to 8 trailing spaces from the replay name, and writes the timestamp/completion value (8 or `g_FrontEndSelectedStage`).
  - States 11 and 12 are replay naming and score naming. The keyboard has 12 columns: left checks `rem != 0` (Move -1 / Move 12), right checks `rem != 12` (Move 1 / Move -12).
  - The close path (12 ticks) stores state 21, then option 0 → game transition and options 1/2 → `SetNextGameMode(4)`.
- Profile/score layout:
  - shot group stride 0x437C;
  - stage record DWORDs at 0x4D0/0x484C/0x8BC8;
  - stage row 0x4DC;
  - Extra unlock bytes indexed by character*3;
  - music unlock flag at profile byte 0x1D892+track;
  - practice catalog rows 0x90; practice result at 0x437C-stride + 0x624; practice score records 0x18.
- FrontEndCursorView is 0xD8 bytes; SetCurrent @0x0040AD20 takes the value in ECX and the cursor in EDX. Screen 11 is Practice, 12 is Replay and 15 is ScoreEntry.
- Stage profile records are 8 bytes (score +0, availability +5), indexed `stage+difficulty*6` within a `(shot+character*3)*0x437C` group.
- FrontEndControllerView::MarkVmInterrupt @0x42C770 only marks pending interrupts on the VM and root children (does not execute); immediate adapters 0x42C730/0x42C750 supply interrupt 2/3. CreateVm @0x42C670 stores IDs at controller+script*4+0x2C4; practice uses slots 0x98-0xAC (vmIds[0xAD] through +0x578, unknown578[0x58] then difficultyAuxVmId +0x5D0); constructor 0x42C920 clears 0x5ACC bytes.
- FrontEndUnlockPracticeRecords @0x42C8C0 (EAX profile receiver from 0x47783C) writes 160 bytes of 1: 16 at [0x1D882,0x1D892) plus 6 groups x 4 difficulties x 6 stages at 0x4E9+group*0x437C+(diff*6+stage)*8.
- InsertScore @0x421FA0: 24-byte rows, signed ties insert ahead, lower-than-all returns -1, exactly 9 name bytes written. Result name alphabet 91 chars (seven 13-key rows; specials 88/89/90).
- 0x5ACC-byte owner; worker +0x5AB0; global 0x47784C. Vtable 0x46ECF0 has a single size-query entry. Mode 2 returns to replay screen 12 (0x4315C0). The global at 0x477820 is the Startup worker (frames at +0x3EC), not statistics.

### Stage, transition and camera

- Stage dispatcher @0x403C80: 1806 code bytes plus MOV EDI,EDI plus 15 table entries make 1868 bytes through 0x4043CB; selector bound 14. VMInit has 23 calls in 8 callers, 15 of them in StartSpell. AnmVmView::Initialize (273) is call-free and never defines EDX.
- `TransitionShakeEnvelope` is TU-static. It loads the nullable game manager in ECX and its flags in EAX, then runs SHR/OR/TEST and a bit-0x72 check.
- `GameUiVmTransitionView::Initialize @0x00420F30(position)` and `GameUiTransitionInitializeSecondary @0x00421180(owner, position)`: both create three root VMs (scripts 0/1/2) from the ANM at owner+0x3C8; the transition state is at +0x6FC. When the capture index is negative, the secondary resets renderer viewport words +0x2C..+0x48 to `0,0,0x280,0x1E0,0,0,0x280,0x1E0`. The owning class name is unknown.
- Stage: StageScriptView 0x2B64 bytes; 8 script VMs + 3 auxiliary VMs; camera state 280 bytes published at 0x491D7C; fog aggregate 0x1C = six floats (start,end,B,G,R,A) + packed BGRA (low bytes wrap, no saturation); fog evaluator modes 7 additive, 17 acceleration, 8 Hermite. Depth test inclusive 0<=z<=1, screen overlap 32..416 x 16..464. Draw pass registration priorities base+7/+10/+12, owner at +0x20.
- Fade: FLD, FSUB, FABS, FCOM qword 64, FMUL dword(-191/64), with no f32 spill.
- Factory @0x43C8B0 allocates 0x44 bytes, passes EBX priority plus five stack operands and returns RET20.
  - The initializer table at 0x43CAD8 has 9 slots in physical order 0/1/2/3/5/4/6/7/8.
  - GameResetAnimationSlots uses transitionTimer at 2A1C and flags at 2A18 (SetCurrent 60, then OR 4).
  - GameManagerUpdate loads the primary stage from 4776E8 (call @0x4181FC); the menu pointer at 4776E4 goes to 0x404530.
- Easing @0x44C350: 509 code bytes, 3 alignment bytes and 16 entries at 0x44C550 (576 total); a separate 57-byte body starts at 0x44C590. Fog converts through `__ftol2` @0x463B2C.
- Camera: selector 0x405300 (ESI supervisor, EBX index). 0x421480 screen camera: unsigned dims, eyeZ = h/2/tan(pi/20), FOV pi/10, near 1/far 10000, up (0,-1,0). Stage camera 0x4215A0 has an EDI receiver, near/far 30/1800 and a rocking Float3 at camera+0x3C.
- Stage: alpha is a byte at stage+0x1EEB. Fog start/end are raw DWORD captures. VM positionOffset is at +0x340/344/348. StageFogView is 28 bytes with packed color at +0x18. The bounds callee takes ECX object, EDX position, EAX camera, stack distSq, RET4. StageDrawInstances is stack stage/layer, RET8. 0x452182/0x452188 are IAT thunks for D3DXMatrixTranslation/D3DXVec3ProjectArray.
- `GameEnterMenuState` 404530: transition priority15/kind2/30 frames; transitionTimer 2A1C=30; flags 2A18 |= 2.

### Files, archives and compression

- The archive singleton (0x10 bytes) is at 0x497990; the twenty-pack array is at 0x4923B0. The THA1 header is decrypted with 0x1B/0x37 and its sizes are biased by -123456789, -987654321 and -135792468. The table is decrypted with 0x3E/0x9B/0x80. Entries are 0x10 bytes plus a sentinel, and +0xC is copied but never read. The image has no RT_RCDATA resource.
- Decrypt/Encrypt take the seed in AL plus five stack arguments, RET 0x14. Replay uses (0x3D,0x7A,0x80) then (0xAA,0xE1,0x400); score uses (0xAC,0x35,0x10).
- OpenFile takes the filename in EAX and sizeOut/mode on the stack, RET 8. Mode 0 reads only from the archive. ReplayFile::Read takes its byte count in EDI. ReadDecompressEntry takes the filename in ECX, RET 8.
- Memory-file Seek allows negative FILE_CURRENT seeks and its remaining-space check is signed.
- `OpenFile`: EAX filename, stack sizeOut/mode, RET8.
- CPbgFile is 0x0C bytes (vptr, handle +4, access +8). Vtable 0x0046F230: Open 0x435370, Close 0x435460, Read 0x435490, Write 0x4354D0, Tell 0x435510, GetSize 0x435530, Seek 0x435550, deleting dtor 0x435320, ReadWholeFile 0x435580. Base IPbgFile vtable is 0x0046F210. The constructor at 0x435300 takes EAX live-in; GetFullFilePath 0x435610 takes filename in EAX and output in ECX.
- Archive seam: 16-byte PbgArchive objects at 0x004923B0, count at 0x00477850. 0x4356F0 finds an archive by `.dat` name; 0x4357D0 is LoadArchive; 0x435800 reads archive-or-disk data (filename in ECX plus one stack optional-buffer argument); 0x4358E0 returns size (filename in ECX).
- CMemoryPbgFile is 0x10 bytes, vtable 0x0046F308 (size +4, cur +8, owned buffer +0xC). It is read-only: Write returns false. Seek keeps target quirks: no lower bound on current-relative moves, and it rejects exact end/begin. CWin32ResourcePbgFile vtable is 0x0046F328 (Open 0x4366C0; RT_RCDATA; returns true even on failure; FreeResource(NULL) if LockResource fails); it is allocated at 0x434CA0.
- Decoder 0x435DC0 (`ret 0x10`): ring starts at 1, 13-bit offset where 0 means end, 4-bit length = len-3. The shared ring is 0x2000 bytes at 0x0048F868, masked with 0x1FFF.
- Tree at 0x00477858-0x0048F863: 0x2001 nodes of {parent, left, right}; sentinel is tree[0x2000] (right at 0x48F860). Encoder 0x4359B0 is 1029 bytes, `ret 0xC`, allocates 2*inputSize.
- Helpers: AddString 0x436000, DeleteString 0x436210 (`__fastcall`, ECX = node), ContractNode 0x436260, ReplaceNode 0x4362B0, FindNextNode 0x436330. They use private EDX/ECX/ESI/EAX inputs. InitTree 0x435FA0 takes EAX = root. The strings `.\src\pack\LzssUtil.cpp` exist but have no xrefs.
- FileSystem: WriteFile @0x44B540 stdcall RET8 (EDI size from WinMain), returns 0 / -1 open fail / -2 short write; OpenWriteFile @0x44B620 RET4 keeps handle/lock/count for CloseWriteFile; shared storage 0x4922A4/0x49231E.
- Archive rows are 16 bytes at 0x4923B0, with count 0x477850 and filename at +8. Matching is case-sensitive, and a matched archive owns its failures (no disk fallback).
  - Mode 0 is the "r" pointer at 0x474908.
  - Load @0x434C30 takes ECX archive and EAX filename; startup at 42011F loads th10.dat from 0x497990.
  - Decrypt/encrypt calls pass an AL seed plus five stack arguments (RET20).
  - Profile view 0x474BD8 rows are 12 bytes (+8/+4/+1/+0).
  - LoadFromResource @0x434CA0 has no observed caller.

### Text

- `g_TextRenderFormats` records are 0x18 bytes.
- Text: the face name "MS Gothic" (CP932) is at 0x46F36C. Buffer is 1024x64, format 0x1A with 0x15 fallback, and 15 fonts with heights 32-60. TryAllocateBuffer uses EAX receiver and RET 0xC.

### Score, statistics, GameManager and mode owners

- Ending VM opcodes 8/15/16/17 read a 16-byte prefix with destination, resource and script dwords at +4/+8/+C.
- GameManager update callees: ResetPool 405ED0, EnemyManagerClear 409F90 (also on terminal cleanup), PlayMusic 420B10, Stage dtor 402440, factory 43C8B0, Main mode setter 40AC90 (writes nextGameMode). Demo transition: priority43/kind5/60.
- `GameClearLinkedObjects` 417770: EAX; head 18, prev 4, next 8; virtual slot4; scalar delete 4524A1.
- The default record has five groups of ten rows. The score drops by 100000 per row. The 9-byte default name is copied as two DWORDs plus a late ninth byte stored to the same record after the cursor advances; an 8-byte copy is wrong. Row fields sit at `activeByte-4` (score) and `+12` (zero), and the name starts at `activeByte+2`.
- Statistics rows are 0x437C bytes, selected by `index1 + index0*3`; the loader also selects by an unchecked `recordIndex` and accumulates two serialized checksums.
- GameManagerUpdateBody reads `activeFrames` from that row and increments two global frame counters.
- The update core @0x00418190 uses a stack owner and RET4, with stat row stride 0x437C (+0x4CC). Init registers update 0x4187C0 at priority 10 and draw 0x4187D0 at priority 4.
- InitializeGameStatisticsRecord is called seven times with a stack argument (RET4). The kind cursor is at record+628.
- Score/state global 0x00474C40 (`GameScoreStateView`): score +4 (cap 999999999), faith +0xC (cap 99999, floor 5000; amount/10), timer +0x14/+0x18 (cap 130), rank +0x58 (= 0x474C98, clamped to ±0x400). AddRank uses EAX state and ECX delta.
- GameScoreState global 0x474C40: score +0x04, faith +0x0C, embedded timer +0x14..+0x24 (previous/current/subframe/scale/flags), character +0x28, shot +0x2C, lives +0x30, difficulty +0x34, stage +0x3C, rank +0x58. SetFaith @0x418B80 writes amount/10 and resets timer; SetTimerCurrent @0x42A930 current=v, previous=v-1, subframe=float(v). Score receiver for GuiScoreView::Add is 0x474C40.
- ScriptViewer worker member at +0x10: Worker member at +0x10: handle +4, thread ID +8, stop +12, running +16, entry +0x18.
  - Stop repeats stop=1/running=0 before each Sleep(1)/Wait(200).
  - Registration uses flags |2 and priorities 5/39.
  - Draw @0x40A940 owns a 4-slot table at 0x40ABC8 (state-1 in 0..3).
  - MarkGameChains @0x409E20 sets node +4 |= 2 for the +8/+12 callbacks.
- Ending: The interpreter's 18-way table runs 0x40C358..0x40C39F, giving a 1568-byte extent.
  - Records have a 4-byte header and a byte payload length.
  - Creation opcodes call primary CreateVmVariant0 @0x448D00, not Variant1.
  - Init allocates text via 452493 before reading scriptData+14 / header+4.
  - The shared worker prefix is 0x1C; browser trailing word sits at +2C.
- GameScoreState: character +0x28, shot +0x2C, selectedStage +0x3C (0x474C7C), nextStage +0x40.
- Startup: 0x3F0 owner: worker +0x10, VM +0x30, id +0x3DC.
- Statistics tail initializer 0x42ADD0 (448 bytes, RNG words). Table 0x4743C0 has 110 signed bytes. Global 0x47783C.

### ANM

- **AsciiManagerView::ResetStrings @0x4014D0 (26)** and **OnUpdate
  @0x4014F0 (26)**: [2026-11-17] both natural C++ queue-reset owners now
  publish the frame counter through an unsigned 32-bit increment and
  `memcpy`. This models the retail `INC` even across `INT_MAX` without
  signed-overflow undefined behavior, while preserving both regular and
  GUI string-count resets. The two selected VC7.1 SP1 unit cold links
  reproduce all 26+26 bytes exactly, with no relocated fields. The
  existing exact-byte tally is unchanged by this source-semantic fix.

- ANM render vertices are 0x1C bytes.
- `StartPositionInterpolation`: EAX VM, EDX initial, ECX final, duration/mode on stack, RET8. Tangent vector at 491C14.
- ANM VM: scriptIndex +0x38A, activeSpriteIndex +0x384, pending interrupt +0x304, delete mark +0x35C.
- **AnmVmStartScript @0x00404F30 (202 bytes)**: target-private entry with the VM in ESI, the script index in EAX and the loaded resource as the only stack argument (`RET 4`). Its REL32 calls bind `AnmVmInitialize @0x00401DE0` and `LoadScript_0043E710 @0x0043E710`. The 2026-11-21 exact result used a fake ECX context parameter. It was withdrawn when the owner became the natural free function on 2026-11-22. [2026-11-22]
- ASCII manager: managed VM id +0x89A4, asciiAnm +0x8994.
- `AnmRenderManagerView::AddVmVariant0 @0x004489D0` returns `AnmVmIdView` through the hidden struct-return pointer in EAX. Ghidra's `in_EAX` is that pointer, not an explicit parameter.
- Timer flags at +0x6C/+0xB0/+0xFC/+0x128/+0x174/+0x1B0/+0x1FC/+0x228/+0x378; owner +0x20 (also the layer value); manager node +0x04; layer node +0x10; nextInDrawLayer +0x1C; scale +0x3C/+0x40; scroll +0x54/+0x58; matrices +0x23C/+0x27C; colors +0x2FC/+0x300; pending interrupt +0x304 (short); anmFile +0x308.
- Int locals +0x30C..+0x318, float locals +0x31C..+0x328, counters +0x32C/+0x330, positionOffset +0x340 (+0x340..+0x348 preserved by reset), spriteOffset +0x34C..+0x354, generatedVertices +0x358, flags +0x35C (bit 14 suppresses matrix recompute, bits 18-21 anchor, bits 22-25 render mode, bit 30 selects RNG, 0x04000000 deletes), +0x360 bit 1 selects the alternate text rasterizer, sprite index +0x384 (-1 = none), callbacks +0x398/+0x39C, glyph width/height +0x3A0/+0x3A1.
- Reset 0x401DE0 receives the VM in EDX and writes WORD +0x35C = 7. The no-round draw path reads spriteOffset.y for Z (+0x350); the rounded path reads +0x354.
- Script variable IDs: float 10000-10021 (RNG owners 0x004918A8/0x004918B0, camera 0x00491D7C, Float3 0x00491DA0), int 10000-10009.
- Renderer pointer 0x00491C10. Vertex buffer of 0x20000 × 0x1C bytes at +0x3ADACC, end cursor +0x72DACC, start cursor +0x72DAD0, sprite count +0x3ADAC8, flush count +0x58, shake +0x5C/+0x60, mix color +0x732458, cached world matrix +0x3AD0F0, source vertices (0x14 stride) +0x3ADA78. Viewport owner global 0x00491FAC: camera-right +0x30, view +0x4C, projection +0x8C, viewport +0xCC. Fog: near 0x491E78, BGR floats 0x491E80/84/88, far color 0x491E90.
- The manager is 0x732460 bytes: pool of 4096 VMs at +0x68, occupancy +0x3AC068, next index +0x3AD068, 33 × 0x130-byte loaded ANMs at +0x3AD06C, embedded VM +0x3AD130, surface arrays +0x3AD4E0/+0x3AD560/+0x3AD5E0/+0x3AD660, image info (0x1C stride) +0x3AD6E0, list heads +0x72DAD4..+0x72DAE0, 20 layer sentinels +0x72DAE4, next id +0x732454, counter +0x4C, visit count +0x64. World creators add (+224, +16). The ExecuteScript entry is callee-cleans `RET 4`. Writable zero vector at 0x00491C14.
- ExecuteScript table: 94 entries (signed opcode+1, bounded to 0x5D) → 92 groups. END/DELETE and NOP/INTERRUPT_LABEL share bodies. Child opcodes 88/91/90/92 call Variant0/2/1/3. Tails are shared 88/90 after 90 and 91/92 after 92. STOP_HIDE falls through to STOP. Mode 9 calls script polar 0x441EF0, not radial 0x4458B0. The position callback is invoked by the update owners, not by the executor.
- Polar helpers 0x408750 (used by the bullet transform interpreter 0x406D90) and 0x441EF0 (ANM) are separate.
- ASCII manager (0x89AC bytes, global 0x4776E0): queues of 0x68-byte strings at +0x76C (256) and +0x6F6C (64), counts +0x896C/+0x8970; VMs at +0x14/+0x3C0; +0x8974 color (-1), +0x8978/+0x897C scales, +0x898C=9 char width. String record: 64-byte text, Float3 pos, color, two scales, dword, viewport index, font bank, shadow flag. Regular glyph = smallFont*0x62 + ch-0x20 from 0x44-byte table, advances 7/9 or scaled/14, two-pixel half-alpha shadow; GUI renderer uses first bank, rounded vs not via exact scaleX==1.0f.
- RGB evaluator @0x441600: EAX receiver, stack result, RET4; four Int3Scale @0x441E50 calls.
- Child opcodes 88/90/91/92 call 448D00/448F60/448E30/449090; older 90/91 comments were reversed.
  - FlipX/FlipY XOR direction bits in flags +35C and multiply scale by the float -1.0 constant at 0x470B60 (FMUL, not FCHS).
  - Operator* @0x441E50 is called only from RGB Evaluate @0x441600 (four calls).
  - The common tail is 1441 bytes; jump tables sit at function offset 9588.
- ANM manager: nextVmId serial at +0x732454. The ctor allocates 20 ChainElems. Callbacks 0x4485D0..0x448857 are layer callbacks.
- `DrawColoredRectangle`: EDI rect, EBX color; 0x5C frame (four 14-byte vertices plus XYZ temp). Flush 442F50 takes ESI. Draw 216: ECX manager, EAX VM. Floor 21: stack float, ST0 double, RET4.

### Main and supervisor

- Main worker stopRequested/running at 638/63C. Startup-state offsets 0/5C/60. 42B660 and StageCreate converge at 417B1C.
- Mode-object globals `0x477810` (GameManager), `0x47784C` (FrontEnd), `0x477820` (StartupWorker), `0x477700` (Ending), `0x477838` (ReplayManager). Their cleanups are at `0x417C80/0x42CB60/0x41FB50/0x40B7B0/0x4294A0`; scalar delete is at `0x4524A1`.
  - `ResetRenderState` sends the float bits of `1.0f/1000.0f/5000.0f`.
- `malloc @0x452706`, `free @0x452422`.
- GameWindowView @0x004924F0, 0x58 bytes: HWND +0, closing +4, HINSTANCE +8, frame counter +0x14, QPC freq/start +0x18/+0x20, launch state +0x28, SPI saves +0x2C..+0x34, timing doubles +0x38..+0x48, time origin +0x50. The next static object starts at 0x492548.
- MainSupervisorView @0x00491C28: next mode +0x390, flags +0x3CC (bit 0x1000 forces mode 2), critical section +0x64C, lock depth +0x6F4. SetNextGameMode @0x0040AC90 takes EAX as the receiver. MainAllocationRegistryView is 0xA004 bytes.
- Private ABIs: CreateGameWindow takes HINSTANCE in EBX. ResolveShortcut takes the destination in EBX, 0x104 in EDI, the path on the stack, RET4. 0x0043A1B0 takes HWND in EAX. The frame dispatcher @0x00439390 takes a stack pointer 0x4924F0, RET4. The checksum is stored at 0x492380/0x492384.
- Shared data: g_GameScoreState base 0x474C40 (+4 used by replay), RNG 0x4918B0, g_AnmGameSpeed 0x00476F78 (also the shared 1.0f constant), g_GameErrorContext 0x474F70 (Fatal @0x0044B8E0), Player lives 0x474C70 (volatile).
- Cameras: `MainCameraRecordView` is 0x118 bytes: an 0xE4-byte
  `AnmViewportOwnerView` prefix, `unknown0E4`, screen-shake X/Y at +0xE8/+0xEC,
  and 0x28 trailing bytes. `MainSupervisorView::cameraRecords[0/1]` start at
  +0x154/+0x26C; slot 1 ends exactly at `activeViewport` +0x384. Slot 0 is
  the published camera (0x491D7C), slot 1 the secondary camera (0x491E94).
  `MainConfigureGameplayViewport` and `EnemyPrepareProjection` index these
  records directly; ANM layer 3/14 use slot 0 and layers 15/16/19 plus
  `GameWindowView::Render` use slot 1. The complete affected owners all pass:
  Main ctor/dtor, SetNextGameMode, OnUpdate, ReleaseGame, StartModeThread,
  LeaveLock, Render, MainConfigureGameplayViewport, EnemyPrepareProjection,
  and ANM DrawLayer 3/14/15/16/19 (15 functions total).
- FPS sampler @0x004134B0 treats >65 FPS as an anomaly and is gated by gameplay state +0x58. Its display @0x004135D0 uses EDI owner, ESI ASCII manager and EBX position.
- Error log: `GameErrorContext::Log/Fatal` use critical section 0x4922BC, depth 0x49231F, and a fatal flag at +0x2004.
- RNG (8 bytes: u16 seed, saved seed, u32 counter): GetRandomU16 @0x44B9B0 = ((seed^0x9630)-0x6553) rotated left 2, counter++. U32 @0x44B9E0 concatenates two; F32 @0x44BB20 /UINT_MAX; F32Signed @0x44BB90 /INT_MAX-1. 0x44BA30 is an unreferenced 78-byte duplicate body.
- Supervisor (MainSupervisorView) base 0x491C28: +0 instance, +4 D3D9 (0x491C2C), +8 device (0x491C30); constructor clears 0x1E1 dwords; config record at +0x120 (0x491D48, 13 dwords/34 bytes saved at shutdown); colorMode +0x13A, windowed +0x13D, frameskip +0x13E, bgmVolume +0x140, sfxVolume +0x141 (signed chars; defaults 100/80), hintMode +0x142 (==2 gates hint_auto.txt export at 0x419009), options +0x150. ANM manager 0x491C10; sound owner 0x492590.
- GameWindow+28 (0x492518) is a byte; +29..2B unknown. GameWindow 0x4924F0 +0C/+10 are DWORDs. Supervisor +13D (windowed) is a byte.
  - WindowProc is a standard stdcall callback (RET16). ActivateWindow @0x43A1B0 is an 8-byte private EAX wrapper. MainConfigurationDialogProc returns 0 on every path (RET16).
  - Checksum: GetModuleFileNameA via IAT 0x4660DC. Size is stored to 0x492384, then the checksum to 0x492380, after freeing the buffer.
  - Sleep is IAT 0x4660AC. Critical-section IATs are 0x4660B0/0x4660B4.
- Screenshot: Supervisor +50C pending (also the global 0x492134), +510 14-byte file header, +520 info, +524 pixels, +528 filename; device at +8, format at +EC.
  - Format 22 allocates 44+921600 bytes and converts bottom-up 640x480 BGRA to BGR. Format 23 logs; default logs and returns 1.
  - The worker 0x420540 (302 bytes) is a cdecl(void*) `_beginthread` entry, published at 42083F.
  - Capture request: +0 surface index, +4 ANM index, +8..24 source then destination rects, +28 texture entry, +2C..48 surface coordinates. Texture is at manager+3AD06C, entry +120.
- Supervisor: device +8; `cameraRecords[2]` at +0x154 (stride 0x118),
  active camera +0x384, viewport index +0x388; nextGameMode +0x390 (=0x491FB8),
  mode +0x38C; config view (52 bytes) +0x120, worker +0x62C (running +0x63C);
  fog cache is a full DWORD at +0x750; clearColor +0x780; disableVsync +0x3B4;
  temp storage +0x764; ctor clears 0x784 bytes and sets flags 0x140. Static
  init 0x4658F0; dtor 0x41FF60 stops worker +0x62C. The two camera records are
  now explicit `MainCameraRecordView` members rather than anonymous byte arrays.
