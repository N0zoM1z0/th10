# Archive lookup exact replay

2026-10-22, starting5b539f9, original Japanese TH10 v1.00a only.

The complete212-byte owner4356F0..4357C3 is reproduced by natural source in
`src/FileSystem.cpp`: a static inline entry-suffix helper and pointer iteration
over the existing archive array. No type, ABI, layout, globals or target bytes
change. The helper itself receives no source, origin or exact credit.

## Accepted scope

Canonical unit `filesystem-find-archive` keeps `/GL /GS /EHs` and independently
cold-compiles FileSystem/PbgArchive/PbgFile plus the already supported normal-COFF
ResFile caller, then links through the real Read entry. Both actual full cold
builds reproduce all212 bytes and all seven semantic fields. The complete
PDB contribution includes loop alignment and the final MOV/JMP return path;
there is no truncated RET-only comparison. Each actual image also passes the
complete46-byte CMemoryPbgFile::Open/two-call-field guard, without duplicate credit.

The seven fields bind cookie473660, strchr452730, `.dat`46F274 (also addend4),
count477850, array4923B0 and cookie-check458EA5. The five-byte literal payload
is independently checked. Source/header and compiler-context pins are in
[facts](facts.json); [validation](validation-summary.json) records executed checks.
These are bounded codegen claims. The diagnostic anchor image is not a runnable
product; original helper spelling, class, source partition, production flags,
physical data ownership and native runtime remain unknown.

Full110-group CI passes with all371 source/test/config inputs unchanged; its
actual routing test retains112 assertions/eight compiled controls. Current
canonical PDB owners independently confirm212/236/242. Authored exact is now
402/73437 bytes, overall1243/139947; authored source remains613/247945 bytes,
source-present/nonexact211 and absent121. Origins524 remain indeterminate.

## Discriminating controls

Seven private models are actually executed. Pointer iteration gives212 bytes
with28 ordinary differences; explicit branches reduce that to21. Buffer-first
and declaration-first forms retain21. Using strcpy's returned pointer fails the
exhaustive field inventory (the literal-tail field moves86->92). An explicit
copy loop fails the PDB extent at203 versus212. Factoring the ordinary suffix
helper recovers every byte and all7 fields. These private models reuse three
pinned support objects and are distinct from both full actual-source cold builds.
No branch-only, mutable-path or size-call model execution is claimed.

The host projection includes the new helper and its wrong-array-element control
now mutates the pointer return. It retains112 ASan/UBSan assertions and eight
compiled strict functional controls. Dependency seams still do not prove target
ABI, object lifetime, Windows API behavior or engine runtime.

Read/GetSize source bodies remain unchanged and nonexact (retained context236/242
versus target214/190). No broad old-unit or large ANM/ECL/Enemy replay is claimed;
the changed helper has its own complete cold comparison and same-image caller.
Whole Windows i386 build/runtime closure remains open; semantic reconstruction
and portability have not started. No Factory receipt is issued.

## Recovery

Private campaign: `.analysis/gpt-6.1-sol/filesystem-codegen-20261022/`.
Retain the first actual cold under `build/gpt-6.1-sol/filesystem-codegen-20261022/cold-1`
and second canonical carrier `build/match-linked/FileSystemArchiveLookup`, compact
producer/results/input pins, and the earlier routing baseline. Discard only
classified superseded experiment builds. Nine independent D3D evidence files,
canonical carriers from other units and provider/legacy state are preserved.
Historical probes require their frozen pre-adoption source; never restore the
maintained source or blindly rerun those old producers.
