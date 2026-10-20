# Hint input reconstruction

## Scope and native evidence

`LoadHintFile` is a descriptive source name for the complete native owner
`0x00419960–0x00419F39` (1,498 bytes). Its main return at `0x00419AEF`
and open-failure return at `0x00419995` pop 12 bytes. The final jump at
`0x00419F35` returns to shared cleanup within that owner; six `CC` bytes follow.
The three-argument free-function declaration is a natural source candidate,
not a recovered original identifier or class declaration.

All addresses refer to `resources/th10.exe`, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The initializer at `0x00418E30` checks hint mode `0x00491D6A` at
`0x00418E8B–0x00418E94`. A nonzero mode permits two calls whose results are
ignored: `0x00418E9E` loads `hint/hint_auto.txt` with mode 0, and
`0x00418EAB` loads `hint/hint_user.txt` with mode 1. The path strings are at
`0x0046D6D8` and `0x0046D6C4`, respectively. The initializer and its scheduler
registration remain outside this source patch.

The coherent dependency set is:

| Target | Source candidate | Native extent | Ledger origin |
| --- | --- | ---: | --- |
| `0x00419960` | `LoadHintFile` | 1,498 | authored |
| `0x00418D00` | `HintEntryView::HintEntryView` | 51 | indeterminate |
| `0x00419F40` | `HintReadLine` | 260 | authored |
| `0x0041A050` | `HintSplitLine` | 75 | authored |
| `0x0041A0A0` | `HintTrimWhitespace` | 114 | authored |
| `0x004198C0` | `HintLookupValue` | 98 | authored |
| `0x0041AB10` | `HintLinkView::Append` | 52 | authored |
| `0x00419040` | `ClearHintLists` | 79 | authored |

Source presence does not change these origin classifications. Private helper
register conventions are not forced into source. Whole-loader exact native code
generation, original ownership declarations, and the wider lifecycle caller
graph remain unresolved. The bounded cleanup and two helper units below have
separate canonical replay evidence.

## Observed storage and lifetime

The 32-bit entry is `0x88` bytes: position at `+0x00`, embedded link at
`+0x0C`, text storage at `+0x1C`, count at `+0x60`, alignment at `+0x64`,
base at `+0x68`, time at `+0x6C`, remaining at `+0x70`, scale at `+0x7C`,
text width at `+0x80`, and BGRA bytes at `+0x84`. The constructor clears the
entry, sets the link payload to itself, sets all color bytes to 255, and sets
remaining to -1, time to 300, and scale to 1.

The observed `strncpy` count is **65** at `0x00419C99–0x00419CA3`. The view
uses `char text[68]` through `+0x5F`, including constructor-zeroed bytes after
that copy. This makes the later string access well-defined for 65-byte text;
it does not establish the original array declaration. Count remains at `+0x60`.

The loader appends entries to `owner + 0x18 + stage*12`, establishing the
first group's payload type. Mode 0 alone allocates a separate entry, copies
all 136 bytes, resets the copied link to self/null/null, and appends it to
`owner + 0x78 + stage*12`. Nonzero modes do not clone. The owner view remains
partial; its size is `0x1A8` with unresolved surrounding storage.

Cleanup `0x00419040` visits stages 0 through 7, first the loaded list and then
the export list for each stage. It saves next before releasing the payload
and leaves sentinel pointers unchanged. Do not traverse or clean those
retained dangling pointers again. The native release target `0x004524A1`
tail-jumps to `_free` at `0x00452422`; its original delete/free identity is
unresolved. Natural `delete` with a trivial destructor is the source candidate,
not additional symbol provenance. The fixture uses matching `malloc`, placement
construction, explicit trivial destruction, and `free`, with release deferred
until reset so retained pointer values can be checked safely.

## Preserved parsing behavior

- `OpenFile(path, &size, 1)` at `0x0041997F` failing returns -1 before scratch
  allocation. Success allocates three 4,096-byte buffers and frees all three
  plus the original file buffer. A successful zero-size input still performs
  this allocation/free sequence and returns 0. No allocation-failure recovery
  is added.
- Outer directives use a colon and perform two split passes. A recognized
  `Version` other than `0.0` clears both groups and returns 0 after freeing
  input and scratch. The exporter's `Version = 0.0` is not recognized as that
  directive. Colonless directives retain the previous value buffer.
- Only stages 1 through 7 accept `Tips`. A new `Stage` resets the entry count;
  `StageEnd` invalidates the stage. After 255 entries, a further `Tips`
  invalidates the stage until another `Stage` directive.
- The line helper searches for LF anywhere before considering CR. Delimited
  lines cap the copy at capacity minus one, consume the whole original line
  and subsequent CR/LF run, strip `#`, and trim literal space/tab/LF/CR only.
  It skips blank results. VT and FF are preserved.
- A final undelimited tail copies all remaining bytes without a capacity cap,
  does no comment removal or trim, sets remaining to zero, and returns the
  original tail cursor unchanged.
- `Pos` parses signed integer text and converts to float. Other integer fields
  stay integers. Name lookup is exact and case-sensitive; unknown names map
  to zero. The verified 61 base names and three alignment names are reused.
- Text uses the first and last quotes. A missing opening quote ignores the
  field; a missing closing quote after an opening quote finalizes the partial
  entry. EOF also finalizes an entry. Unknown inner directives are ignored.
- Colors and alpha wrap to an unsigned byte. One comma updates red even if the
  second comma is absent; green and blue require that second comma.
- Scale is stored as float but bounded using the live parsed value against
  -4 and 4. Unordered comparisons allow NaN to survive. Width is length times
  stored scale times 7.5, with a final float destination store. The native
  float expression reproduces the target's dword operands and unspilled x87
  operation sequence. The host parser suite explicitly requests x87 evaluation;
  it does not force a double expression into production source.
  Under the host x87 model, length 3 and scale `1.829711675643921f` yield
  `41.168514251708984f`; an explicit intermediate float store instead yields
  `41.16851043701172f`. Native x87 precision and
  control-word assumptions are not fully established; universal floating-point
  runtime equality is not claimed.

## Host verification and limits

Run from the repository root:

```
python scripts/test-hint-load.py
python -O scripts/test-hint-load.py
python scripts/test-hint-export.py
python -O scripts/test-hint-export.py
```

Both runners support `--skip-mutations` for lightweight CI. Default runs use
UBSan and execute 22 loader controls or 12 exporter controls. Mutation success
requires exit status 1, exactly one labeled CHECK diagnostic, and no UBSan
message. Python checks remain active under `-O`. The exporter fixture retains
source-extracted, unchanged void FileSystem writer/close effects; its added
OpenFile stub fails immediately if the loader is unexpectedly called.

The loader fixture compiles the maintained source, substituting only its two
entry allocation and two release call sites with tracking dependencies. It
executes the actual constructor, parser, list insertion, cleanup and loader.
It checks 183 base/alignment pairs, boundary stages/counts, clone independence,
missing/empty input, partial entries, stale values, 65-byte strings, cleanup
order, final-tail cursor/copy behavior, NaN comparisons, and width rounding.

These are defined-domain synthetic fixtures, not execution of the game or its
input files. Inputs have accessible NUL padding; helper capacity is at least
one and remaining is nonnegative and consistent with accessible input.
Oversized final-tail behavior is tested using sufficient physical destination
storage. Real OpenFile allocates the file size without adding a sentinel, so
arbitrary native files do not acquire this fixture guarantee. Signed integer
fixtures stay within 32-bit range because host `atol` may be 64-bit. CRT
integer overflow, locale/NaN text parsing, allocation failure, malformed input
that overflows buffers, and repeated cleanup of dangling heads are excluded.
NaN is injected at the atof dependency, without claiming the target CRT accepts
that textual spelling. No original executable, Windows API, real hint-file IO,
or native compiler build is used by these host tests.


## Native diagnostic and bounded cleanup replay

The genuine LoadHintFile entry with Hint.cpp and FileSystem.cpp, using the
normal pinned LTCG profile, compiled successfully. Initial PE SHA-256:
9f760846528c1461a10e4c8ecc4adbcf35641a0476706956460fa6daf383a5f3.
The loader contribution is 1,556 bytes versus the 1,498-byte target; the entry
constructor is 72 versus 51. Neither is exact.

ClearHintLists is a complete 79-byte contribution with all 71 nonlinkage bytes
equal to target 0x00419040. Its only two REL32 fields at offsets 0x1E and 0x38
bind candidate operator delete to target 0x004524A1, the observed free-tail
wrapper, not directly to 0x00452422. This is a per-edge relocation binding,
not recovery of the original runtime identifier. The real loader calls it
on version rejection; no synthetic retention wrapper was introduced.

Canonical unit hint-clear-lists preserves that actual loader entry and source
graph. Two independent cold links replay all 79 bytes exactly, with PE hashes:
- 3b3890af2be5c40a30e88cf0dcb120e708b22fd9cc7189b3c6335a55a1df390b
- 36299cee151807049b5483945e60bd862ef262515bea16a826865264e3d39b6e

Factory acceptance is checked separately against the committed snapshot;
local replay and manifest registration alone are not an accepted receipt.
The constructor's origin remains indeterminate. No native runtime, original
translation-unit ownership, or entire-loader exactness follows from cleanup.

## 2026-10-20 exact whitespace and lookup helpers

Two fresh cold links in this same real loader graph reproduce the complete
HintTrimWhitespace (114 bytes) and HintLookupValue (98 bytes) owners raw-equal.
Qualified static MAP/PDB selection checks their actual contribution ownership;
neither needs linkage normalization. ClearHintLists remains exact in both images.

Whitespace source is unchanged. The lookup helper now uses an ordinary
`__stdcall` declaration: target calls419D11/419D39 push table/count and pass name
in EDX, and both target returns clean eight bytes. LTCG independently selects
that private register argument from the three-argument declaration.
The prior cdecl candidate differed only in return cleanup and its consequent
local branch displacement. This does not establish the original declaration.
See [complete helper evidence](evidence/hint-small-helpers/README.md).

The subsequent HintSplitLine checkpoint uses an explicit colonless early
return to recover its complete75-byte target layout. Both cold links replay
all4 fields: strchr, one static trim CALL and two explicit static trim tail
JMPs. The latter require the complete114-byte trim callee to be raw-equal and
its maintained target source/name to agree. Existing cleanup, lookup and trim
guards retain291 bytes/all2 fields in both images. See
[split proof and binding limits](evidence/hint-split-tail/README.md).

## Exact line reader and native arithmetic source

HintReadLine419F40/260 now has two independent full cold replays with all4
fields in the real Hint/Main/FileSystem four-TU /GL /GS graph. Ordinary
stdcall source recovers RET12; target calls4199FD/419BE3 push three arguments,
carry cursor in EAX and perform no caller cleanup. The complete static trim
callee is checked raw-equal. Five existing guards retain726 bytes/all32 fields.

The broader context also restores the exact360-byte OpenFile boundary. The
two-TU context specializes that dependency differently and passes sizeOut in
EBX. Current LoadHintFile is1533/1498 bytes and remains nonexact. Its natural
float width expression matches the target operand widths. The actual-source
host test uses x87, checks a stored float reference value, and rejects an
explicit intermediate-store mutation; the pre-existing numeric witness is
unchanged. This is a bounded host model, with the original control word and
native runtime still unknown.

The entry color now has packed/byte BGRA views matching the target32-bit
initialization and parser byte accesses. The constructor is51 bytes but still
has self-link scheduling differences; no exact or origin credit follows.
See [line-reader identities and limits](evidence/hint-read-line/README.md).

## Exact tail insertion member source

The tail insertion at41AB10 is now maintained as HintLinkView::Append. Natural
member source reproduces the whole52 bytes raw-equal in two independent full
cold links, with no linkage fields. The former free HintAppendLink emits40;
adding fastcall to that free form leaves all nine inspected owner hashes
unchanged. The member preserves the final next-pointer recheck and incoming
next-link behavior; production and fixture calls both use that member API.

Six existing same-image guards retain986 bytes/all36 fields. The source view's
class identity is descriptive, and original class/name/visibility/TU remain
unknown. Loader and constructor exactness are separate open claims.
[Full member replay](evidence/hint-list-member/README.md).
