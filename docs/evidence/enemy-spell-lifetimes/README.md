# Spell source lifetimes and register visibility (2026-10-19)

The complete EnemyBeginSpell owner remains nonexact. **All experimental source
is restored; no boundary, origin, source mapping or exact credit changes.**
This review extends the earlier parked [local hypotheses](../enemy-spell-local/README.md)
with fresh target/candidate instruction alignment, rather than rerunning its
uniform-copy or per-arm lifecycle source patches.

Read-only local Ghidra reattests the disk target and complete mapped `.text`
before and after a rolled-back review of BeginSpell, EndSpell, the shared ANM
script initializer and registrar. Target BeginSpell still owns2403 code bytes,
one NOP and seven table entries:2432 total. Complete strict acceptance is not
claimed for any experimental spell body or fragment.

| Control | Complete native / target | Ordinary comparable diagnostic |
| --- | --- | --- |
| Existing real four-TU ECL graph | 2476 /2432 | 771 /2032 |
| Ordinary child-pointer return helper | 2472 /2432 | 746 /2032 |
| Return the child node's owner directly | 2484 /2432 | 1109 /2036 |
| Above, source flags read before two short writes | 2484 /2432 | unchanged |
| Above, three separately scoped name-copy forms | 2484 /2432 | unchanged |

Each control is one cold CL13.10.6030/LINK7.10.6030 link in the existing
EnemyDispatcherHelpers context: EnemyEclDispatcher.cpp, EclVm.cpp,
AnmManager.cpp, AnmVmCreate.cpp, all `/GL /GS`, real DispatchEclInstruction
entry. `evidence.json` pins profiles, inputs and images. The four-TU baseline
and preceding genuine ten-TU StageCreate image have identical normalized
spell instruction/size sequences. This does not exclude every other possible
production partition or justify a caller/flag sweep.

Target successful child-search arms re-read the node's owner into EAX before
the value store. The maintained cached child local instead persists another
register. An ordinary return helper naturally inlines without new attributes;
returning `node->owner` recovers the observed re-read shape and more ordinary
positions, but complete bytes and size remain wrong. This does not establish
the original helper, source identity, operation coverage or runtime behavior,
so the source is restored. No forced inline policy, register annotation, fake
argument, padding or byte copying is introduced.

The target shared initializer reads flags immediately after Initialize and
before short script/resource metadata writes. Its old maintained source put
the flags read later, but the compiler already hoists it. Moving that statement
is byte-neutral. Distinct panel/per-selection/aggregate copy forms are also
byte-neutral in this graph; they are restored. These controls do not reopen
the earlier per-arm lifecycle factoring experiment.

The more concrete remaining limitation is register-preservation visibility.
Both target and native AnmVmView::Initialize are raw-equal across273 bytes,
contain no calls, and have no instruction defining EDX. The current shared
initializer keeps the VM in that preserved EDX, saves only EBX/EDI and emits197
bytes. Target uses incoming EAX/ECX/EBX, saves EBP/ESI/EDI and emits203 bytes.
The embedded initializer likewise uses native EDX versus target ESI, emitting
200 versus202 bytes. Direct calls and named link destinations cannot waive
these unlike caller instructions. The original compiler visibility, physical
partition and interprocedural policy remain unknown. Do not manufacture a
clobber or add the reference's unused leading initializer argument to force it.

The local MIT reference suggests ordinary cursor and returned-node shapes.
TH08 StartSpell uses a different encoded-name/bonus protocol and supplies no
TH10 ABI/layout evidence; TH09/TH095 routing did not yield a closer spell
analogue. Target assembly and native observations remain the authority.

The initial observer accidentally used76 for the shared initializer's target
size. It was corrected to the complete203-byte extent using the completed
PE/MAP/PDB, without rebuilding. The corrected metadata also names the actual
primary EnemyEclDispatcher.cpp. Only corrected complete measurements appear
above. No malformed diagnostic was promoted.

Source/tool checks and tracking remain current. Native builds run serially at
nice10; Ghidra uses768MiB and one active processor. Superseded controls and
source snapshots are removed; one minimal ECL PE/MAP/PDB remains for the open
compiler-visibility claim. Product, runtime, semantic and port gates stay open.
