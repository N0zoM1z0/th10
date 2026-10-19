# Transition allocator and scheduler interfaces (2026-10-19)

This bounded review corrects the factory allocation/stack-cleanup hypotheses,
reviews complete initializer ownership, and closes two scheduler convenience
registrars and two effect callbacks. Factory82, initializer396 and cleanup127
remain nonexact. Whole-product, runtime, semantic and portability gates stay open.

## Independent target evidence

The original JP1.00a target is hash-attested on disk and in local read-only
Ghidra. Full mapped `.text` SHA-256 agrees before and after the review;
temporary function metadata is rolled back. No target byte is modified.
`target-facts.json` records complete decoding, hashes, padding, references and
all twelve fields of the four promoted owners.

| Target | Owned bytes | Observed interface/role |
| --- | ---: | --- |
| 0x44A000 | 43 | EAX callback, EDI priority, stack user pointer; RET4; calc registrar |
| 0x44A030 | 43 | Same real operands; draw registrar |
| 0x43BD40 | 93 | ECX effect; falling alpha, startup abort, timer tick, returns1/7 |
| 0x43C3C0 | 68 | ECX effect; 640x480 dim rectangle/color, returns1 |

The registrars store the user pointer at node+0x20, OR node flags2 and call
the corresponding existing AddTo chain method. Their prior authored-medium
origin classifications are retained. Target-specific timer/alpha/global behavior
and the game rectangle/color operation independently support authored_game,
medium confidence, for the two new callbacks. Native matching and reference
source names do not establish their origin. Full-range Ghidra reference review
finds only initializer references to callback entries, no outside interior
references. Padding separates each from its next independent owner.

Factory0x43C8B0 pushes one allocation size0x44 and calls0x452493, addsESP4,
checks null and ends with RET20. A fresh pinned libcmt.lib extraction compares
all14 allocator bytes and its __nh_malloc relocation exactly. The symbol is
ordinary `??2@YAPAXI@Z`, not the two-argument nothrow overload.
`allocator-oracle.json` proves this library binding only; allocation handlers,
throw/failure behavior, prezero timer flags and object lifetime remain separate.

Initializer0x43C970 owns **396 =357 code +3 LEA alignment +36 table bytes**,
through0x43CAFB. Unsigned selector0..8 indexes the nine internal code entries
at0x43CAD8. The physical arm order is0,1,2,3,5,4,6,7,8. Complete target decode,
Ghidra references, a supplemental whole-file literal scan and adjacent cleanup
at0x43CB00 support this ownership without overlap. This corrects the old357-byte
ledger and adds39 authored denominator bytes with zero initializer exact credit.
The two new callback extents add161 reviewed authored bytes independently.

## Maintained source and compiler observations

Factory source now uses ordinary `new`, retains every real formal, and models
the observed callee cleanup with `__stdcall`. It still tests allocation failure.
The annotation is a maintained historical-compiler model, not a recovered
original keyword. All Gui/Stage declarations agree. The two existing registrar
bodies are unchanged; only their callee-pop annotations change in Chain.hpp/cpp.
Only TransitionEffect calls these convenience wrappers; other controllers use
the existing AllocElem/AddTo methods. No fake argument, return, ABI adapter,
inline assembly, padding, forced outlining or target patch is introduced.

The controls keep the genuine Gui updater -> Run -> factory -> initializer
registration graph. The allocation expression changes factory89 to84 bytes;
factory callee-pop spelling emits88 with RET20. Registrar callee-pop spelling
restores initializer416 to396 bytes and both complete43-byte registrars.
Reordering independent source cases restores the target physical arm order.
The positive factory branch/common source return is byte-neutral; native
factory remains88/82, and initializer scheduling still differs. None of these
size/structural observations grants parent credit. No ABI/flag sweep follows.
A subsequent read of the pinned
`matching2/msvc/include/th10/core/screen.hpp` finds zero-fill/flags2 inside its
constructor. Our current source keeps these in the factory after member
construction. That distinct initialization-lifetime hypothesis is left for a
separate target/compiler control; the reference's redundant timer statement and
noinline annotations are not adopted, and no additional exact claim follows.

The sparse MIT reference at `_reference/th10-decomp`, commit
`c6791d82dc9f02e55eca45a5314f900a85481efd`, suggested allocation, real-formal
and case-order hypotheses. All conclusions above are checked against TH10-local
evidence. Reference section pragmas, assembly and forced compiler controls are
not adopted. See [the reference issue](https://github.com/N0zoM1z0/th10/issues/1)
and [the recorded MIT license](../../licenses/th10-decomp-MIT.txt).

## Focused replay and host scope

The canonical units share the existing14-TU /GL Gui updater graph and unchanged
profile, with TransitionEffect/Chain explicitly qualified as PDB support owners.
The four existing callback recipes share the same output directory to avoid
repeat builds. Static owner selection uses the existing strict MAP/PDB Oracle;
no new normalization rule is introduced. `native-summary.json` records both
independent cold results, the42 frozen CPP/header inputs, different PE/PDB
identities and all exhaustive target field bindings.

```sh
scripts/repo-python scripts/build-match-unit.py --unit chain-register-calc
for unit in chain-register-calc chain-register-draw transition-fade-out \
    transition-draw-dim-screen transition-draw-screen transition-draw-playfield \
    transition-draw-dim-playfield transition-draw-pulse; do
    scripts/repo-python scripts/compare-linked-function.py --unit "$unit" --json || exit 1
done
```

Each cold image passes **591 complete bytes/all24 fields**:247 new exact bytes
and344 existing guard bytes. The new owners are counted once; their callees
receive no duplicate credit. Diagnostic unresolved symbols are data anchors,
so these images are not executed and do not close production storage/link order.

The host baseline passes14 scenarios/843 assertions. The focused init_flags
negative control compiles and fails only with the required assertion/exit86.
The existing host allocator seam replaces the actual ordinary-new expression
exactly once; it does not prove native allocation failure or exception behavior.
Only three dependency LF pins change, with full-file reversal proof showing
callee-pop annotations are the sole changes. Historical raw receipts are not
acceptance pins. The19 EOL acceptance/rejection controls pass unchanged.

A separate cold nine-TU real-StageCreate link checks the changed external
factory declaration in StageDraw.cpp. Both existing seven-byte draw adapters
retain all14 bytes/two REL32 fields; the nonexact draw cores receive no credit.
Only the four new units, four same-graph callbacks and these two Stage guards
are replayed; no historical full native cohort is rebuilt.

Full public CI, pinned toolchain declarations, tracking, generated progress and
whitespace checks pass. `validation-summary.json` records the actual scope and
removal of10684099 bytes from five completed duplicate diagnostic builds. The
final canonical PE/MAP/PDB and the current nonexact frontier are retained. The
pre-existing untracked D3D evidence is untouched and unadopted.
