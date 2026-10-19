# Ending initialization sampling (2026-10-20)

Two target-backed sampling corrections are now maintained in
`EndingView::Initialize @ 0x40B560`. **No new exact bytes are claimed**:
the complete initializer remains584/584 and nonexact. The existing exact
ledger, origins, boundaries and denominator are unchanged.

## Target contracts and source

The input is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh local read-only Ghidra checks the complete mapped text before/after;
its temporary changes are rolled back. The JVM is limited to768MiB and one
processor. Raw target code provides both contracts:

1. Before calling the screen VM creator448D50, the initializer captures the
   address of the current ASCII owner's auxiliary ID at+89A4. After creation,
   it writes the returned ID through that captured address. The old native
   output instead reloads the global ASCII pointer after the call. A normal
   reference to the actual ID field now preserves the original destination.
2. After successful text-object allocation452493, the target reads the current
   scriptData member at+14 and its four-byte header offset at+4, computes the
   instruction pointer and calls the text constructor40BA90. The old source
   precomputed the offset before allocation. The new-expression argument now
   reads the current file through the small ordinary EndingScriptStart helper.
   The same helper expresses the existing reload format in opcode12, where
   the byte-level behavior is unchanged. No buffer bounds or input rules are
   invented.

The selected source also follows observed node next/previous and callback
clear order, and writes the observed difficulty>=1 /ending-index>=6 conditions.
These preserve the defined behavior and require no new fields, compiler
attributes, ABI declaration changes or output-slot machinery. The shared
game-state storage experiment is rejected; GameScoreState.hpp is unchanged.

## Native evidence and limits

`audit-ending-run-native.py --include-initialize` extends the existing four-case
Run audit with these two initialization checks. It binds unique PE/MAP/PDB
owners, follows the captured destination register through the creator call,
and requires the buffer/header reads after the last text allocation and before
construction. The source-base carrier from b35f147 passes the four Run checks
but fails both initializer properties. The selected private model and final
fresh native carrier pass all of them. This target-specific auditor is not a
general semantic equivalence checker and grants no parent exactness credit.

One fresh full18-TU /GL /GS /EHs cold link freezes all52 source/header inputs.
It retains the real Main checksum entry and actual archive dependency. Ten
complete existing guards pass **943 bytes/all57 fields**: loader106,
OpenFile360, primary creator73, ReleaseAnm55 and the six older Ending owners349.
The byte-identical current artifacts are adopted for the existing loader recipe;
no recipe migration or duplicate credit occurs. No full historical native
cohort is replayed. The diagnostic image still has non-runnable unresolved
symbol anchors and is not a product build.

```sh
scripts/repo-python scripts/build-match-unit.py --unit ending-load-script
scripts/repo-python scripts/compare-linked-function.py --unit ending-load-script --json
scripts/repo-python scripts/audit-ending-run-native.py --directory build/match-linked/EndingLoadScript --include-initialize
```

The source controls compile one Ending TU and reuse17 hash-checked support
objects, with maintained inputs checked before/after. Sampling corrections,
ordered clears and observed range forms remain584 bytes. A byte-safe common
configuration view regresses ordinary comparison and is discarded. Such
diagnostic scores are not exact percentages or partial-case credit. Node flag
scheduling, persistent-record dataflow and other register choices remain open.
Run1576/1568, the text constructor772/648, destructor families and bank loader
also remain nonexact.

## Host regression scope

The selection witness swaps the global ASCII pointer during screen creation.
It checks that the original ID is written and the replacement owner's sentinel
is unchanged. The old source fails the named observable. The allocation witness
replaces the current script buffer at the modeled text allocation event and
checks the instruction pointer comes from the replacement header. The tests
make no claim that normal target allocation or VM creation performs these swaps.

Direct replacement-new and tagged-allocation attempts in this host fixture
still passed the old constructor argument even after the hook changed the
member. GDB confirms the changed member and old argument in the same execution.
This observation does not establish a language rule or compiler defect. Those
host models are not used as evidence for historical allocation order.

The accepted host copy explicitly models the allocation event, then uses
placement construction and a matching catch/delete path for constructor
failure. The production new-expression is unchanged by this host transformation.
The native auditor independently checks its actual VC7.1 ordering. This is an
explicit regression seam, not proof of unmodified host-new-expression behavior.
Vptr-preserving clears, defined timer/node prefill, omitted native-layout
assertions, SDK/assets/list roles and simulated worker startup retain their
previous limits. Both actual VM creator bodies remain in the host compilation.

ASan/UBSan and leak detection pass the positive suite and seven exact named
mutant failures: the existing five plus rebinding the selection destination
and reading the header early. CI runs the positive suite. Temporary default
host artifacts clean themselves up. The native pass and native legacy failure
remain independent of those host seams.

Original class/TU/data ownership, modern C++ lifetime, faithful Windows product
closure, target runtime, dual-Oracle semantics and portability remain open.
The known401100 boundary metadata discrepancy is not rewritten. Pre-existing
D3D evidence remains untouched.
