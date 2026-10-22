# ECL math local lifetimes and integer pop storage

2026-10-22, starting6500ef8. Five actual source transformations test the complete
7020-byte EclVmContext::Run in the maintained ten-source LTCG context. No source,
ABI, compiler flags, manifest or exact count changes are adopted.

| Source transformation | Run extent | Strict complete-owner result |
| --- | ---: | --- |
| LENGTH_SQUARED x/y const float references |7020|6992 matching bytes; baseline27 differences plus one at+97|
| POINT_ANGLE dx/dy const float references |7020|External ResolveFloat field at+18BC lacks corresponding target binding|
| Both reference transformations |7020|Same field rejection; raw body differs from angle-only at+97|
| Two existing scalar union locals shared only across the math cases |7044|Complete extent rejected|
| PopInt local uses existing scalar union integer member |7020|6993 matching bytes; baseline27 differences unchanged|

The LENGTH reference control preserves every baseline differing offset and byte
value. Its added byte at+97 belongs to the instruction at+95 on the RETURN path:
target MOV EDX,[EAX+EDI], candidate MOV EDX,[EDI+EAX]. It does not fix the math stack
homes. The angle-only and combined reference bodies differ at exactly that same
raw byte, despite both being rejected at the call field. No ordinary-byte score
is assigned after a field or extent rejection.

The math-only scalar pair leaves FORMAT and the existing payload union definition
unchanged. It therefore isolates a narrower storage hypothesis from the earlier
shared-scratch controls that also changed FORMAT. Its7044-byte result does not
establish an original-source constraint; it rejects this particular candidate.
The PopInt trial changes only the local storage and its member access, retaining
argument ordering, tag, width and the full Run source. All three large native
bodies are raw-identical to the baseline in this trial.

Source-shape assertions check the actual transformations, FORMAT preservation,
union definition and definition order. See the five adjacent patches and
[evidence.json](evidence.json) for source hashes, raw owner hashes, complete
bounded differences and guard results. Unlike the historical misrouted controls,
these conclusions are tied to the expressions actually compiled.

## Validation and retention

Disk target and pinned toolchain verification, tracking and a fresh read-only
Ghidra review preceded the builds. Ghidra verified the complete mapped code hash
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
Each trial compiled only the changed ECL object and reused nine hash-verified game
objects and two harness inputs, serially at nice10 with unchanged /GL and /GS.
All five passed the nine complete ECL guards:1137 bytes and18 bound fields each.
The two scored Run trials also retained the exact171-byte float callee.

The frozen108 source inputs,373 broader inputs,19 canonical artifacts and nine
unrelated untracked D3D files were verified unchanged before documentation edits.
Private producers, complete reports and source copies remain under
.analysis/gpt-6-astra-medium/ecl-math-local-lifetimes-20261022. All80 new native
carrier files were hash-inventoried and removed, reclaiming5332921 bytes. Earlier
useful unresolved candidates remain untouched. Final focused checks cover the
toolchain, tracking, progress, documentation hygiene and diff whitespace; full CI
is omitted under the operator's focused-replay instruction.
