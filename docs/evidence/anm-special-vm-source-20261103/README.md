# TH10 special ANM VM descriptor factory 0x0040B110 — complete source, NONEXACT

Target: original Japanese TH10 v1.00a PE pinned by `config/target.toml`.
A full manual original i386 decode confirms the **148-byte**, CC-delimited
body `0x0040B110..0x0040B1A3`, ending in `RET 0x0C`, physically separate
from its neighboring functions. This is a real multi-call owner, **not** a
small leaf or a wrapper that can be replaced with a single creator call.

## Target-observed business logic

The original receives a descriptor index privately in **EAX** and three
stack words (returned ID storage, ANM loaded source, and callback argument).
It multiplies index by `0x10` and adds the **real** descriptor-table base
`0x00474430`. The table record has a signed 16-bit script number at +0,
optional setup callback pointer at +4, position update callback pointer at
+8, and draw callback pointer at +0xC. Descriptor count and allocation
ownership are not established; the source declares a table view only, with
compile-time size and offset assertions.

It then uses the actual `g_AnmRenderManagerView` at **0x00491C10** to
allocate an ANM VM (`0x00449950`), sets render layer 15 and ORs bit
`0x40000000` into VM flags at +0x35C. The real loaded ANM source's
`InitializeVm` at **0x00449870** initializes that VM with the signed
script index. It appends the VM to the primary list via true
`AddVmVariant0` **0x004489D0**, captures its generated 4-byte ID, and
resolves the VM by real `FindVm` **0x004491C0**. When the lookup returns
NULL, the return ID is cleared; the target nevertheless continues through
its optional callback and callback-field writes without an invented null
check. If present, the table's setup callback gets the resolved VM and the
caller's setup argument. The position/draw callback fields are copied from
record +8/+0xC to VM+0x398/+0x39C, then the ID is written through its
hidden return-storage argument.

Maintained `src/AnmSpecialVmCreate.cpp` implements all those operations
with real `AnmVmView`, `AnmVmIdView`, `AnmLoadedView` and
`AnmRenderManagerView` classes, and a typed 16-byte descriptor. Function
name `CreateConfiguredAnmVm` is descriptive, not a claim about original C++
spelling, owner class, original physical TU or calling convention. In
particular, the source declaration is an external `__stdcall` prototype
with a conventional descriptor-index parameter and returns by value;
the **native index is passed in EAX**, not via a normal stack argument.
The original's private EAX/hidden-pointer three-stack-word/RET12 ABI must
be discovered from a real caller or WPO context before claiming exactness.

## Independent VC7.1 compilation and rejected controls

Four actual pinned VC7.1 SP1 `/GL` compiler-source controls were run against
the **complete 148-byte target extent**:

| Form | Full PDB bytes vs target | Preliminary ordinary bytes |
| --- | --- | --- |
| Natural `__stdcall`, directly initialized ID | 156 vs 148 | 10/124 |
| `__fastcall` signature | 164 vs 148 | 7/124 |
| C++ default calling convention | 154 vs 148 | 10/124 |
| Explicit zero-initialized local ID, then assignment (**selected**) | **148 vs 148** | **33/124** |
| Equivalent null-ID constructor expression | 156 vs 148 | 10/124 |

The selected explicit local ID source form improves the PDB-owned extent to
the target size and the ordinary instruction count by 23 bytes. This is
**NONEXACT**: the six observed candidate address fields are not bound to
complete target-field geometry; the original target's private EAX receiver,
caller stack/return convention, exact register liveness and helper call
scheduling are still different. A separate true three-source ANM manager +
VM creation compiler context emitted **199 vs 148**, so it is rejected as
an incorrect original-call graph rather than credited. None of these
results are a canonical match, and no partial exactness or runtime claim
is made. Receipts retained privately under
`.analysis/gpt-web/20261103/anm-special-abi-controls.{py,json}` and
`anm-special-vm-{first,real-anm,selected}.log`.

The maintained source is now mapped in `config/functions.csv`,
`config/implemented.csv` and `config/reccmp-functions.csv` as a complete
semantic source owner. No new match-unit or exact-codegen-byte credit is
added; existing native allocator, AddVmVariant0 and FindVm exact functions
are separately protected by focused cold replay. No historical full cold
replay or full-game product verification is inferred.
