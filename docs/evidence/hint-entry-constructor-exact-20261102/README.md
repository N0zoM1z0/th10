# HintEntryView constructor: complete 51-byte native closure (2026-11-02)

The original Japanese TH10 v1.00a function
HintEntryView::HintEntryView @ 0x00418D00..0x00418D32 owns 51 bytes.
It is now **canonical linked-PE exact: all 51 bytes and zero external
fields**, validated against the independently hash-attested original EXE.

This was not an address normalization or a cropped matching fragment:
the full 51-byte PDB-owned body has byte-for-byte identical machine code,
one ordinary return, no fixups and a complete 51-byte instruction decode.
The class/name is still a reconstruction description. The target's source
authorship versus compiler-generated constructor **remains indeterminate**;
no authored exact credit or attribution change is claimed.

## Recovered real dataflow

The constructor clears its 0x88-byte entry with memset, zeroes the two
link pointers, writes its own address into the embedded list payload,
initializes packed BGRA to 0xffffffff, sets remaining to -1, time to 300,
and scale to 1.0f. Target instructions at +0x0C and +0x0F zero the links,
at +0x12 write the self-link, at +0x15 set the color accumulator to -1,
and at +0x18/+0x1E publish color/remaining in that order.

The prior natural spelling link.payload = this was moved after the packed
color and remaining stores by VC7.1 (37/51 bytes raw-equal). The retained
*ordinary, nonvolatile C++ reference* to the real link member:

    HintLinkView &linkView = link;
    linkView.payload = this;

preserves the target's source-visible memory lifetime and naturally produces
**all 51 bytes identical** in the selected actual Hint file loader context.
No target instruction, dummy argument, forced register annotation, inline
assembly, padding, source ABI, or original game binary has been changed.

Controlled variants include pointer/explicit reference access, read/write
memory barriers, pointer/cast and memset spelling, and volatile views.
The nonvolatile reference and pointer local are both raw-equal. Memory
barriers alone reproduce the self-link/store order but swap the first
XOR EAX,EAX and PUSH EDI instructions, leaving 48/51; volatile
source access remains at 37/51. These rejected variants are restored.

## Two independent focused cold replays

The real LoadHintFile loader remains the link entry. Its native five-TU
compiler graph is HintLoad.cpp with Hint.cpp, FileSystem.cpp,
PbgArchive.cpp and Main.cpp, using the canonical VC7.1 SP1 /GL /GS
recipe. The selected PDB identifies the constructor as a support-sourced
51-byte function with its actual ??0HintEntryView@@QAE@XZ symbol.

Two **independent** cold replays select only the new constructor and
two existing parser/link units. Both pass exactly 363/363 bytes:

- constructor 51 / 51
- HintReadLine 260 / 260
- HintLinkView::Append 52 / 52

The native PE hashes and PDB GUIDs differ between builds; both report
the constructor raw-equal with zero field normalization. Additional selected
source-consumer guards also pass: real Hint owner/draw callbacks 73/73 and
the Caution/clear-list owners 183/183 across two existing producers.

[The native summary](native-summary.json) includes target/source pins,
per-cold identities, native PDB owner identity, complete field/return
diagnostic and scoped guards. The old 1308/1317-byte ConsumeEntries,
1498-byte loader, 212/220-byte AddAutomaticEntry, and other unrelated
nonexact bodies receive **zero** new exact credit.

## Reproduction

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit hint-entry-constructor --unit hint-read-line \
      --unit hint-append-link --json
    scripts/repo-python scripts/replay-exact-units.py \
      --unit hint-create-owner --unit hint-draw-callback --json
    scripts/repo-python scripts/replay-exact-units.py \
      --unit hint-clear-lists --unit hint-add-caution --json
    scripts/repo-python scripts/compare-linked-function.py \
      --unit hint-entry-constructor --json
    scripts/repo-python scripts/ci.py
    git diff --check

This closes one canonical codegen owner and adds 51 codegen-exact bytes, **not**
an authored function, a whole-program/game build, source provenance, or
runtime/portability certification.
