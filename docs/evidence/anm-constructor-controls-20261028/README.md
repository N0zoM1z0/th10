# Complete ANM constructor controls, 2026-10-28

This follow-up inspects the complete2315-byte manager constructor at0x445900,
removes one source-only cache write, and falsifies two tempting source models.
**No new exact bytes or source owners are admitted.** Current maintained source
emits2305 bytes versus2315 target and remains nonexact. Extent differences are
not ordinary-byte mismatch counts.

Original Japanese TH10 v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Disk identity, pinned VC7.1 SP1 execution, tracking and local read-only Ghidra
six-sample attestation pass at starting HEAD `d1d5fd6`. Direct local tools are
used throughout. The prior worktree is clean; protected unrelated evidence is
untouched. The previous goal turn is substantive progress, not an unverified
wait or stalled build.

## Target review and retained baseline

Fresh full target decode and Ghidra review confirm the2315-byte boundary and
member construction preceding the whole-manager clear. Prior prose calling
manager+0x732454 a capture ID was wrong: its maintained name is **nextVmId**,
and four AddVm functions increment it, skip zero, store the VM ID, and copy the
returned handle. This corrects the name; original source representation remains
unknown. The existing AnmVmIdView default constructor emits a pre-clear zero
store absent target. That observation alone does not justify changing every
handle constructor or introducing an uninitialized special tag.

The retained prior13-TU image's source/header pins match the saved pre-edit
files. Its four complete registration owners pass460 bytes. Its actual273-byte
Initialize callee is raw-equal as well. Consequently the constructor loop's
pointer/counter allocation discrepancy cannot be explained by different bytes
in that callee in this context.

## Directed controls

All main inputs keep `/GL /GS`, the existing AnmInt3Scale `/Ob1` override and
the previously reviewed AnmManager `/EHs` override. No global profile or old
canonical recipe is migrated.

| Model | Build scope | Complete constructor | Complete registration owners |
| --- | --- | ---: | --- |
| Starting retained image | Prior independently compiled13-TU graph | 2308/2315 | 118/118,118/118,112/112,112/112 exact |
| Plain unsigned nextVmId counter | Fresh independent13-TU cold compilation | 2305/2315 nonexact | 117/118,117/118,102/112,102/112, all rejected |
| Remove redundant texture-filter store | Fresh independent13-TU cold compilation | 2305/2315 nonexact | All four remain exact |
| Descending pointer loop | Recompile only AnmManager, reuse12 pinned objects | 2305/2315 nonexact | All four remain exact |
| Real CRT absolute exception-list definition | Relink13 retained objects plus pinned exsup.obj | 2305/2315 nonexact | All four remain exact |

The plain-counter candidate replaces the member with unsigned storage and
updates its four registration methods and two lexical HUD uses consistently.
It removes the unwanted pre-clear initialization, but loses the target's
copy/test/reload sequence and changes every complete AddVm body. The twenty
callbacks and six existing Main/ANM guards still pass1314/all81; this is not a
registration pass. The candidate is rejected. The original header, four
registration bodies and Gui.cpp are restored byte-for-byte. TH095's real
manager uses an integer counter while a separate exact lifecycle view uses an
ID wrapper; this is corroboration of a modeling ambiguity, not TH10 evidence
or a transferable layout/type conclusion.

Fresh target constructor decode contains no direct byte store at+0x3ADA6E
between the whole clear and VM initialization. The maintained source's explicit
`currentTextureFilter = 0` produces that extra store. Removing it leaves the
field zero through the existing whole-object clear and removes the native
instruction. This one-line deletion is the only maintained code change. Native
size falls2308 to2305 because instruction removal also changes internal
alignment; a six-byte store deletion is not a six-byte owner match gain.

The natural pointer/countdown loop produces **the same complete2305 native
bytes** as the maintained indexed loop, including all linked fields in these
images. Both code SHA-256 values are
`0fb2669e314b35161d80e12040866798905d4016773285dc6b554d2481f26aa4`.
The pointer source is restored. This control is parked; do not add volatile,
extra arguments, fake register hints or padding to force the target's stack
counter. Reopen only with independent source-lifetime or actual context evidence.

## Real CRT exception-list evidence

The pinned libcmt.lib archive hash is
`6bed2e73e7ecf52818133011058e240cd38b122f51302c0c0194d5ff3167357e`.
Its unmodified `build\intel\mt_obj\exsup.obj` defines `__except_list` as an
external **COFF absolute symbol, section=-1, value=0**. Existing TH10 archive
matches already cover this member's local-unwind and abnormal-termination code;
no new runtime-code exact credit is added here.

The control extracts that entire attested normal-COFF member and adds it to a
private relink. It preserves all13 authored-source object hashes. All three
constructor FS operands become FS:[0], replacing the diagnostic data-anchor
address. The constructor extent and initial instruction order remain unchanged;
this does not repair the target's EH prologue, stack local, loop allocation or
Chain store scheduling. The private link remains non-runnable and does not
identify original production object order or validate EH runtime. No hand-made
absolute symbol, copied target code, source flag change or comparator exception
is introduced. This is a concrete library-integration lead for a later declared
product/constructor graph, not canonical acceptance of this unsupported complex
EH parent.

## Focused validation and disposition

The maintained-source cold image, pointer control and CRT relink each pass
**31 complete existing contracts,2047 bytes/all81 fields**: twenty callbacks585,
Main/ANM guards729, four registration owners460 and Initialize273. Target
bindings remain canonical; only source/context/output selection changes in the
private observer. PDB extents, fields and fixed local destinations retain their
usual strict checks. The constructor is structurally diagnosed only and receives
zero exact credit; its complex EH contract remains unsupported.

The constructor, callbacks and Main-camera host checks remain source-contract
checks. The unchanged RNG extraction and six other active dependencies justify
updating only AnmManager.cpp's strict LF pin after proving the entire file
changes by exactly that one deletion. Host algorithms and CRLF/content rejection
rules stay intact. Final selected CI and terminal receipts are recorded in
[results.json](results.json); no complete cohort or full-suite run is claimed.

All16 selected CI stages pass, including both ANM host groups, Main camera,
transition baseline/19 EOL controls, eight Chain dispatcher mutations, supervisor
bindings, ledgers, source completeness, graphs, progress and docs/whitespace.
The transition baseline's existing kind8 FP rounding gap remains explicit.

Exact totals stay **77,823/275,434 authored bytes,434 owners**. Source coverage
is unchanged. Remaining constructor type/EH/frame/loop/Chain scheduling questions
are parked with these falsifiers; next choose a substantial complete owner with
a new concrete semantic uncertainty. The complete1568-byte EndingRun remains a
triage candidate, not an asserted solution. Product, runtime, semantic and port
gates remain open.

Working producers, full target/native listings, source snapshots, source/object
pins and terminal logs are below
`.analysis/gpt-6.1-sol/anm-construction-id-20261028/`. Builds stay under the
corresponding `build/gpt-6.1-sol/` directory. The selected source is reproduced
by the existing `anm-callback-*` shared graph; private `cache-only` is the fresh
source-current cold image, while canonical output paths retain historical
artifacts. No historical artifact is relabeled as a new source-current build.
