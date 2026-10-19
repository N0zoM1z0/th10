# ANM executor stack and source-lifetime review (2026-10-20)

No source change or exact credit is retained. Fresh current-source four-TU
ExecuteScript /GL /GS still emits9948/9964 bytes, frameF8/targetFC, all92 ordered
physical groups and84 equal-sized spans. All11 CPP/header input bytes are
restored to785e91c after four bounded controls. Boundary/origin/denominator and
canonical recipes are unchanged.

## Target facts and falsified hypotheses

Fresh local read-only Ghidra attests the target and complete mapped `.text`
before/after analysis. `target-and-baseline.json` records selected operand
positions, calls, complete parent hashes and frame allocations.

| Observed value | Target raw ESP slot | Fresh baseline slot |
| --- | --- | --- |
| F_ADD9 RHS binary32 snapshot | 0x40 | 0xB4 |
| F_SUB11 RHS binary32 snapshot | 0x88 | 0x40 |
| F_SET_MOD27 divisor snapshot | 0x80 | 0x7C |
| F_SET_MOD27 dividend snapshot | 0xB4 | 0xC4 |

These are decoded ESP operands, not recovered original local-variable names.
Short/long displacement encodings account for the paired9-byte ADD/SUB span
changes. Getter-before-pointer ordering is already correct in the baseline;
MOD already parses operand2, then1, resolves the destination, then calls the
math intrinsic. There is no evidence here for repairing arithmetic or reversing
those calls. Both target and baseline preserve binary32 snapshots.

FLIP_X61 differs by register choice: target uses EAX and its five-byte XOR-immediate
encoding; baseline uses EDX and six bytes. FLIP_Y62 already uses target EAX.
Neither fact uniquely recovers the source statement order or field declarations.

| Control | Complete parent | Frame | Equal-sized groups | Disposition |
| --- | ---: | ---: | ---: | --- |
| Unchanged baseline | 9948/9964 | F8/FC | 84/92 | Retained source |
| Explicit F_ADD/F_SUB value and destination locals | 9948/9964 | F8/FC | 84/92 | Entire raw parent hash unchanged; restored |
| Float operand snapshots plus explicit double fmod API | 9948/9964 | F8/FC | 84/92 | Entire raw parent hash unchanged; restored |
| Existing flipX/updateScale bitfields | 9964/9964 | F8/FC | 85/92 | Extra Boolean ALU work; FLIP_X61 grows61/52; restored |
| Move FLIP_X multiplication before raw flag update | 9948/9964 | F8/FC | 84/92 | Different scheduling, no improvement; restored |

The coincidentally target-sized bitfield result does not restore the frame or
whole instructions. Its case63 shrink and update-tail growth also demonstrate
cross-region allocator coupling. The physical `[2]` span includes the shared
update tail; it is not a standalone1441-byte opcode body. No case, common tail,
matching bytes or improved diagnostic score receives separate exact credit.

The pinned MIT reference's `anm_vm_tick.cpp` explicitly identifies this owner
as equivalent recovery with assembly fallback for exact mode. It suggests
source hypotheses but supplies no natural-C++ exact proof. Its repeated flag
store, force-inline helpers and assembly are not adopted. VC7.1's actual math
header confirms fmodf is a float wrapper around double fmod; selecting the latter
explicitly is byte-neutral in this control, not a fix for the missing frame word.

## Reproducibility and actual validation scope

`source-overlays.json` reconstructs each recorded variant source hash by exact-one
replacement against the unchanged base. `controls.json` pins all source inputs, complete parent/image hashes, layout
reports and reuse receipts. Only the initial four-TU baseline is a full cold
build. Each diagnostic recompiles AnmManager.cpp and relinks with three byte-hash
checked baseline /GL objects after every non-primary source/header input is
verified unchanged. No incremental result is treated as a canonical cold proof.
This avoids twelve unnecessary support-TU recompilations. The existing pinned
compiler/linker, profile, real entry and support order are unchanged.

The fresh baseline strictly compares seven existing complete units: Initialize273,
SetupVertexBuffer472, primary-alpha130 and all four operand getters856. They
retain **1731 bytes/all107 fields**, including local jump-table destinations.
`existing-guards.json` records the actual four-TU carrier/context overrides;
these are guard observations, not canonical recipe migrations or new credit.
This rules out a getter-body byte mismatch in this carrier without establishing
original interprocedural visibility or a unique original source shape.

```sh
scripts/repo-python scripts/build-match-unit.py --unit anm-start-primary-alpha-interpolation
```

This reproduces the selected public four-TU carrier. Canonical target-field
recipes remain in config/match-units.toml at785e91c. Source models and their
rejected outcomes above identify the isolated edits; full source/header hashes
in controls.json prevent confusing them with later changes. Private generated
decompilation and experimental binaries are not committed.

Full public CI, toolchain execution/declarations, tracking, progress and whitespace
checks pass. `validation-summary.json` records restoration and cleanup; one fresh
baseline is retained for the unresolved parent and four obsolete diagnostic
builds are removed. The pre-existing untracked D3D work is untouched.
