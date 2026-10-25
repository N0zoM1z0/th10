# Separate Spt Test Draw owner

The first complete natural model and a separate canonical cold link reproduce
**531 bytes / all 32 fields** at 0x42C360, the real **10-byte callback / one
field** at 0x42C590, and the existing **100-byte AddSmallFormatText / four
fields**. All seven fixed internal branch destinations are independently
checked. The callback is origin-indeterminate: its ten bytes are not authored
credit. The body was already reviewed authored; its denominator is unchanged.

This is distinct from `ScriptViewerView::Draw` at 0x40A940. The title is
`Spt Test\n`; state 1 uses this object's ECL database at +0x2D8 and prints
`Ecl not load` without an argument if absent. State 3 truncates coordinates
at +0x2C4/+0x2C8. States 2 and 3 restore the currently published ASCII manager
to white and return early. There is no state-4/embedded-VM/flag tail.

## Evidence and natural context

`src/SptTest.cpp` is a complete low-level view using existing ASCII, cursor,
and ECL declarations. The unknown prefix is storage, not instructions or
allocation padding; full original class/TU ownership remains unknown.
The exact ScriptViewer source is a same-target source-shape hypothesis, not
this owner's proof. No forced private ABI, noinline, register hint, assembly,
byte emission or target patch is used. LTCG naturally selects the target's
EDI owner, ESI ASCII manager and EBX position pointer.

Both builds use only SptTest.cpp plus the real AnmManager.cpp, VC7.1 SP1
`/GL /GS /EHs` and the real fastcall callback as root, serially on one CPU at
nice 10. The callback is independently referenced by the registration code at
0x42BBF5; the task receives the object's context and draw priority 0x27.
This small context suffices; the unrelated fourteen-TU ScriptViewer graph was
not transplanted. The linked diagnostic image is never executed.

The complete body is dense code through RET 0x42C572; thirteen CC bytes
separate it from the update adapter at 0x42C580. The draw callback ends at
0x42C599 before six CC bytes and the separate entry at 0x42C5A0. There is no
owned table or omitted alignment. Existing body origin is preserved; adapter
source-written versus compiler-outlined origin remains unresolved.

Fresh target/toolchain/tracking and Factory-native Ghidra preflight passed.
A fresh read-only architecture export passed. The global automatic boundary
check still reports the pre-existing 0x401100 evidence-text mismatch; no broad
ledger regeneration or checker relaxation is used. Independent read-only
review verifies source semantics, all fields, string/float payloads, complete
local destinations, callback registration and the real callee.

[Compact native results](native-summary.json) and [field bindings](field-bindings.json)
retain target-bound observations. Ignored full native artifacts remain at
`build/spt-test-draw-first-20261025` and `build/match-linked/SptTestDraw-cold`.
The former's 113 frozen inputs are unchanged; source is unchanged between
both native runs. Original logs/exits are under
`.analysis/spt-test-draw-20261025`.

```sh
scripts/repo-python scripts/build-match-unit.py --unit spt-test-draw
scripts/repo-python scripts/verify-spt-test-draw.py
scripts/repo-python scripts/test-spt-test-draw.py
```

The focused host test extracts actual source and checks all states, absent
files/database, finite signed-coordinate formatting, cursor positions, the
callback, and synchronous sink mutations of state, owner fields and published
ASCII managers. An initial fixture expected sign-inclusive decimal precision;
that expectation was corrected to standard `%.3d` sign-plus-three-digits, with
no source change. Host observations are not x87 edge or Windows runtime proof.
Whole-product closure, semantic reconstruction and portability remain open.

## Final validation

All 113 target-independent CI stages pass, exit 0, verified
2026-10-25 06:51:37 UTC. All 1515 frozen inputs, including nine protected
D3D evidence files, are unchanged. Only CI-result documentation changes follow.
The first CI run also passed, but its preceding metadata/freeze setup failed
because a SHA property was called as a method; it has no all-input-freeze claim.
The corrected fail-closed setup and distinct final frozen pass establish this
checkpoint. Original logs/exits and transport timeouts remain separate; no
active CI was duplicated. See [validation details](validation-summary.json).
