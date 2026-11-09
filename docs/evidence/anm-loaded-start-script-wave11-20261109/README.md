# ANM loaded-script start recovery — wave 11

Date: 2026-11-09

## Recovered target

0x0043E710..0x0043E7DA, 203 bytes, implemented in
src/AnmLoadedStartScript.cpp as the native C++ function
LoadScript_0043e710.

Canonical compare-linked-function.py --unit anm-loaded-start-script-wave11
--json result: **exact, 203/203 matched bytes**, all 203 bytes decoded, no
ordinary differences, and complete relocation normalization. The four fields
are:

| Offset | Kind | Target | Role |
|---:|---|---:|---|
| 0x29 | REL32 | 0x00401DE0 | AnmVmInitialize |
| 0x85 | DIR32 | 0x00476F78 | global ANM game-speed float |
| 0xAA | REL32 | 0x0043EE30 | ANM script executor |
| 0xAF | DIR32 | 0x00491C10 | ANM render manager |

The PDB owns the complete 203-byte contribution. raw_equal is false only
because the focused link uses diagnostic anchor addresses; normalized
comparison binds every field to the independently observed target address.

## Source-level decisions that reproduce the target

- The source parameter order is resource, object, index.
- AnimationObject_0043E710 derives from the exact-size AnmVmView, and the
  resource overlay places scripts at +0x11C and loading at +0x124.
- The first flags mask is a named local and is stored after the script index
  and resource fields.
- The final bit clear uses a plain unsigned int * read/modify/write. Direct
  &= and volatile alternatives changed the register schedule.
- The body naturally selects resource in ECX (saved in ESI), VM in EAX (saved
  in EDI), script index in EBX and zero in EBP. The source uses ordinary C++
  fields and accessors; there is no inline assembly.

The synthetic compiler context is retained as
compiler-context.cpp in this evidence directory, so the selected exact
unit does not depend on an ignored scratch source file. It is a proof-only
caller, not game runtime code.

## Initializer restoration

The previous source rename to AnmRenderManagerView::InitializeVm was
incorrect. 0x00401DE0 is the independent fastcall entry
AnmVmInitialize(void *unused, AnmVmView *vm), with the VM in EDX. Its
canonical replay is raw-equal 273/273, with no relocation fields.
See initializer-link-report.json.

## Caller integration

The production source callsites now use StartAnmLoadedScript, which forwards
to the recovered entry. This replaces the former
AnmLoadedView::InitializeAndExecuteScriptIndex body and updates its callers
in the manager, player, GUI, laser and enemy-laser paths.

All eight positioned-VM creator owners were rebuilt in a focused linked-PE
group with src/AnmLoadedStartScript.cpp as a support source. Their canonical
replays are exact:

- screen variants 0–3: 95/95 bytes each;
- world variants 0–3: 118/118 bytes each.

The eight receipts are in callers/; caller-summary.json is the compact
index. No full cold replay was run.

## Reproduction

    scripts/repo-python scripts/build-match-unit.py --unit anm-loaded-start-script-wave11
    scripts/repo-python scripts/compare-linked-function.py --unit anm-loaded-start-script-wave11 --json
    scripts/repo-python scripts/build-match-unit.py --unit anm-create-vm-screen-variant0
    scripts/repo-python scripts/compare-linked-function.py --unit anm-create-vm-screen-variant0 --json

The remaining seven creator receipts use the same shared linked artifact and
their corresponding unit names.

## Later seven-TU compiler-context limit

On the restored source, the original two-TU exact unit still owns and matches
all 203 bytes, with four independently bound fields. Eight selected VM creator
owners also remain exact: four 95-byte screen variants and four 118-byte world
variants. However, a larger seven-TU spell-HUD diagnostic compiles this
same natural loader as **216 bytes instead of 203**. The seven-TU result is
nonexact, and the private caller ABI is not established by the two-TU proof.
A previous 8-byte jump shim did not solve this and has been removed from
production source. Continue investigating LTCG ownership and source-level
parameter lowering without replacing the complete natural function.
