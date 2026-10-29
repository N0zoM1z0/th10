# FrontEnd lifecycle and Chain receiver — 2026-10-29

Five complete owners add **1159 exact bytes:949 authored and210 indeterminate**.
Two independent canonical25-input cold links reproduce all five and the existing
18-byte nullable delete: **1177 bytes/all77 fields**. The authored denominator is
unchanged. The new six-byte inventory entry retains indeterminate origin.

| Complete owner | Bytes | Fields | Selector |
| --- | ---: | ---: | --- |
| Virtual size query `42C5B0..42C5B5` | 6 | 0 | `frontend-size` |
| Constructor `42C920..42C9EB` | 204 | 3 | `frontend-constructor` |
| Destructor `42CB60..42CD4A` | 491 | 33 | `frontend-destructor` |
| Factory `42CD50..42CDAF` | 96 | 6 | `frontend-create` |
| Ending outer destructor `40B7B0..40B919` | 362 | 33 | `ending-destructor` |
| Existing nullable delete `42CDB0..42CDC1` | 18 | 2 | `frontend-delete` |

Both outer destructors and the FrontEnd factory retain authored origin;
constructor and size query remain indeterminate. Complete codegen does not establish original declarations,
class/TU ancestry, all storage ownership, product closure or runtime behavior.

## Object construction and actual virtual slot

The object occupies5ACC bytes, including a1C-byte worker at+5AB0. Target fields
are flags+4, update task+C and draw task+10. The older transition-owner view of+10
is preserved as a union. Four existing cursor constructors, timer state and the
worker are initialized before the whole-object clear. The body then sets flags
mask2 and publishes47784C. These initialization writes belong to the complete204
bytes even though the later clear overwrites them.

The first incorrect model made the destructor virtual and gave489/491 cleanup
and469/204 construction. Raw vtable46ECF0 contains one pointer to42C5B0, whose
complete six-byte body returns5ACC. It is a size query. Natural `return sizeof`
and a nonvirtual destructor reproduce the actual slot and cleanup interface.
The query was absent from both prior function inventory and database functions.
The preceding independent42C5A0 leaf is followed by15 CC bytes; ten CC bytes
follow the size query through42C5BF. All six code bytes and four vtable bytes
were queried in attested read-only Ghidra: only the vtable references the entry,
and only the constructor/destructor reference the vtable; no observed interior
references appear. No original virtual method name or origin is inferred.

FrontEnd ID storage must remain plain four-byte storage before the whole clear.
Using the existing default-constructing ID type added target-absent zero stores.
A storage view preserves its operations through references to the existing ID
API. Returning temporary ID values instead regressed two prior UI contracts;
reference views recover them without changing the global ID constructor. The
shared worker declaration moves to a header without changing its layout or
implementation. Existing ScriptViewer constructor/task and UI contracts pass.

The96-byte factory retains a worker-subobject reference across Stop and the
actual `_beginthreadex` launch, preserving observed register lifetimes. It binds
the real callback42C9F0, still a declaration. Null allocation and subsequent
dereference branches remain in the byte comparison; no host scenario is required.

## Complete cleanup and the Chain member premise

Cleanup restores the vptr, stops the worker, removes both tasks under critical
section0/counter0, releases ANM slots25/26, deletes50 replay owners without
clearing their fields, propagates interrupt1 through the auxiliary VM and its
root children, frees/nulls music text, clears47784C and destroys the worker.
Replay's full destructor remains nonexact; its address/interface binding gives
no child-byte credit. The actual FrontEnd destructor takes its owner on the
stack and returns with RET4. Ordinary `delete controller` reproduces the existing
18-byte nullable helper with the source-backed destructor edge.

The corrected object source first reached491/491 but differed in four ordinary
bytes: two register moves at each task removal were reversed. Reordering helper
arguments, an inline node-receiver wrapper and changing the old static fastcall
annotation to static cdecl were byte-neutral. These controls are restored/parked.
The falsifiable member premise `chain->UnregisterElem(element)` removes all four
differences. The compiler naturally promotes the ordinary member to the actual
element-ECX/chain-EDX ABI; no register declaration, fake parameter or wrapper is
used. Complete Unregister114, RemoveAll82 and scheduler308/268 bytes remain exact
in their own canonical14-input cold context: **772 bytes**, all four selectors.
Every maintained call site now uses the same member API. This supports the
maintained interface, not certainty about the original source declaration.

The same receiver premise solves the earlier Ending outer destructor362/362
candidate, whose four ordinary differences were at+2F/+31/+63/+65. Its complete
33 fields and stack-owner/RET4 interface now pass in both independently cold
Main images. It unregisters two tasks, releases surface0, deletes the text owner,
releases ANM banks29..32, frees script storage and clears global/counter state.
Its existing source mapping and authored origin are retained; no new mapping or
denominator change is counted. The real192-byte text destructor also passes in
both images. ScriptViewer teardown remains375/412 and receives no partial credit.

## EH binding and focused native verification

The destructor's DIR32 handler binds one complete24-byte generated PDB region
in its own module/object. Both images require immutable one-state28-byte
FuncInfo, state-1 action, saved this atEBP+4, member+5AB0 and a tail to the real
complete16-byte worker destructor. The target handler is46562E, action465620,
FuncInfo471678 and unwind map471670. The existing selector now permits only
this named parent and the earlier Ending constructor with its literal+D0
member. All other protocol/ownership/byte checks remain strict. Five synthetic
positives and41 rejection controls pass, including rejection of the Ending
offset in either FrontEnd image. This grants handler address identity only;
generated code, metadata and EH-runtime behavior receive no credit.

Both FrontEnd colds compile every declared input afresh with the pinned source
profiles and `/GL`. Complete PDB extents, all link fields and every fixed local
destination pass. Fifty-four explicit older same-image selectors also pass in each image:
15759 bytes/all494 fields. Artifact/source identities are in [results.json](results.json).
The affected existing Ending constructor is separately cold-replayed in its
own canonical context. No full historical cohort or host lifecycle campaign runs.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-size --unit frontend-constructor \
  --unit frontend-destructor --unit frontend-create --unit frontend-delete \
  --unit ending-destructor --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit decomphelp-chain-unregister --unit decomphelp-chain-remove-all \
  --unit chain-run-calc --unit chain-run-draw --json
scripts/repo-python scripts/replay-exact-units.py --unit ending-text-constructor --json
scripts/repo-python scripts/ci.py --only 'Validate worker-member EH address binding'
```

Private snapshots, comparisons/listings, recipes and bounded exit logs are under
`.analysis/gpt-6.1-sol/frontend-lifecycle-20261029/`. The independent first
FrontEnd witness is `build/gpt-6.1-sol/frontend-lifecycle-20261029/cold-a/`;
the second is `build/match-linked/FrontEndLifecycle/`. Lightweight CI and
whitespace checks pass. A staged check caught an extra terminal blank line in
the new worker header; it was removed and both FrontEnd cold witnesses were
regenerated from the final source hashes. The mode parent678/666 and Startup/Sound residuals
receive no partial credit from this lifecycle recovery.
