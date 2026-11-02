# TH10 duplicate 78-byte RNG target owner: canonical COFF exact (2026-11-02)

The original Japanese 1.00a PE contains two **independent** reviewed
78-byte function-like extents: 0x0044B9E0..0x0044BA2D and
0x0044BA30..0x0044BA7D. Their original machine-code bytes are
exactly identical, including the two consecutive 16-bit RNG updates,
counter increments, 32-bit concatenation and return. The second extent
is CC-delimited but has **no independently observed direct entry xref**.

The maintained src/RandomMath.cpp natural
RngView::GetRandomU32() implementation already produces the identical
78-byte relocation-free normal VC7.1 SP1 COFF contribution. This
checkpoint adds a second canonical target comparison at 0x0044BA30
to the existing source/object recipe. It does not add a fake second
function, copy target binary instructions into source, invent an entry
reference, claim the original optimizer's duplicate-emission strategy,
or change the original source's behavior.

Two separately executed *selected cold COFF compilations* check both
physical target extents against the same complete 78-byte produced symbol:

| Selected original extent | Whole bytes exact | COFF relocations |
| --- | ---: | ---: |
| Original GetRandomU32 at 0x0044B9E0 | 78 | 0 |
| Unreferenced duplicate at 0x0044BA30 | 78 | 0 |
| Per cold selected total | **156** | **0** |

There are no external link fields or target-address normalization waivers.
The original 78 bytes decode fully into 27 instructions ending in a return;
the target/source/body SHA-256 values and the two scoped compilation
receipts are in native-summary.json.

The tracking name RngView::GetRandomU32_Duplicate0044BA30 is a unique descriptive alias for the
second physical target range. The real emitted C++/COFF symbol is still
RngView::GetRandomU32. There is no second named method in the source.

This adds **one canonical exact target extent / 78 codegen bytes**.
The second extent's original authorship, entry reachability,
source file/physical TU and possible compiler duplication remain
**indeterminate**. It adds zero authored-origin credit. The comparison
does not prove a full native game/product build or runtime equivalence.

Reproduce the selected scope:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit rng-get-random-u32-duplicate \
      --unit rng-get-random-u32 --json
    scripts/repo-python scripts/compare-coff-function.py \
      --unit rng-get-random-u32-duplicate --json
    scripts/repo-python scripts/ci.py
    git diff --check
