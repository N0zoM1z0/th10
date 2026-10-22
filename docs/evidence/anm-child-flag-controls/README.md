# ANM child initialization and FlipX controls

2026-10-22, baseline `b20ce15`. **Diagnostic only; no source or exact-credit
change.** [Compact facts](facts.json) bind the target, current recipe, source
inputs, native artifacts, transformations and complete guard results.

The ten-TU `enemy-drop-item-counts` recipe remains identical to the previous
actual cold: all 32 source/header and 15 artifact hashes match. All 107 maintained
native source/header files and 369 source/test/config inputs stay unchanged.
Each control compiles only a private copy of AnmManager with the current
`/GL /GS` profile, reuses nine hash-verified objects, and links serially at
nice10. This is not a new full cold or a production executable.

## Target observations

Fresh local read-only/noanalysis Ghidra checks the original executable and all
412,289 mapped text bytes before and after the review, with rollback, one CPU
and a 768 MiB heap. Its names, signatures and body ranges are provisional.

Child91 at 43EF32 is 47 bytes. After the optional GetIntVar call, the target
loads renderLayer into EDX, pushes layer and script, then loads anmFile into EAX
and pushes it. Native uses ECX for layer and loads the file into EDX before the
pushes. Both retain creator2 and the correct shared child tail. This is a
register/scheduling difference, not evidence for a different creator mapping.

FlipX61 at 43F276 is 52 bytes. Target flags use EAX and its five-byte accumulator
XOR; current native uses EDX and a six-byte XOR, producing 53 bytes. Common
tail2 is one byte shorter, so the complete ExecuteScript owners are both 9,964
bytes. Equal total size does not establish local or whole-function equality.

## Executed source controls

| Private control | ANM size | Same-sized groups / 92 | Aggregate absolute gap | Diagnostic positions |
| --- | ---: | ---: | ---: | ---: |
| Current baseline | 9964 | 90 | 2 | 7541 / 7995 |
| Child91 default construction then assignment | 9964 | 88 | 22 | 7309 / 8001 |
| Child91 explicit script and layer locals | 9964 | 90 | 2 | 7541 / 7995 |
| FlipX bitfield toggle, updateScale, multiply | 9964 | 89 | 22 | 7338 / 8000 |
| FlipX bitfield toggle, multiply, updateScale | 9964 | 89 | 22 | 7338 / 8000 |

Default construction plus assignment creates an extra return-value copy and
separate GetVm storage: child91 grows to 55 bytes. The explicit argument locals
preserve getter-before-layer evaluation and reproduce every raw byte of the
complete baseline ANM owner. They do not repair the push/load scheduling.
Both bitfield orders grow FlipX to 61 bytes and produce raw-identical complete
ANM, ECL and Enemy owners to each other. None is adopted. TH095's nearby flip
source is hypothesis material only; its layout and names are not TH10 evidence.

The positional reporter masks fields and aligns physical groups; it supplies
no semantic binding or exactness authority. No per-case credit is claimed.
All 92 physical groups remain in target order. Child routing independently
passes four opcodes/eight static paths, assuming calls return; it does not prove
complete creator bodies or runtime behavior.

Every image passes 35 complete same-image guards: 4,424 bytes/all 118 fields.
Their complete extents and semantic fields are freshly checked against current
canonical contracts. Five canonical units are normal COFF; their additional
linked guards here do not establish original GL/TU ownership or replace their
canonical replay. An old diagnostic template's cancel-record `functions`
metadata is refreshed from the current canonical mapping; its symbol, target,
extent and fields were already identical.

The unmodified maintained ECL diagnostic checks complete Run7020/6993,
189 fields/62 local destinations, all 60 table words/88 selectors and the
complete exact171-byte ReadFloatValue callee in each image. Replaying all fields
reproduces Run hash `830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472`.
The generic normalization flag remains false: decoding covers7019 bytes and
the final selector byte is explicitly annotated. No generic flag is waived or
changed. Enemy stays14352 versus14416, nonexact.

## Recovery and limits

The private producer, input manifest and one child91-assignment failure carrier
remain under `.analysis/gpt-6.1-sol/anm-child-tail-20261022/` and the corresponding
`build/gpt-6.1-sol/` campaign. Duplicate private source, build and report trees
are disposable after these compact transformations and results are retained.
The canonical carrier, unrelated D3D files and provider/legacy state are preserved.

No new canonical cold, separate ANM host campaign, exhaustive ANM field
normalization, data owner, runtime, boundary, origin or phase-gate claim follows.
The next target-local review isolates argument MOV/PUSH carriers: groups49
(rotation) and53 (angular velocity) use EDX/EAX/ECX in target versus ECX/EDX/EAX
in native; group54 (scale growth) uses the first two corresponding registers.
Their sizes already agree112/112/92; these pairs account for6/6/4 ordinary byte
differences. Named float result locals were already neutral in the earlier
count/residual campaign. Investigate a distinct target-supported lifetime or
common-tail hypothesis; do not repeat those locals or these four forms.
Original spelling, private allocation cause, production TU and flags remain unknown.

[Validation](validation-summary.json): all109 CI groups pass, with all369 frozen
source/test/config inputs unchanged before and after. Target/toolchain execution,
tracking, generated progress and whitespace checks pass. Cleanup removes6,427,081
logical bytes from33 explicit producer-owned paths, retaining one actual failure.
