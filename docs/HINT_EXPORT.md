# Automatic hint export

`ExportAutomaticHints` in `src/Hint.cpp` reconstructs the hint-file writer at
`0x0041A200–0x0041AAC6` (2,247 bytes). The original identifier and translation
unit are unknown; the source and view names are descriptive.

All native observations refer to the original Japanese TH10 v1.00a executable,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Boundary and caller

The main return is `RET 8` at `0x0041AAAC`. Two lookup-match tails after that
return remain inside the owner, ending at `0x0041AAC6`; nine following `CC`
bytes are outside it. The function takes an owner and output path on the stack
and returns zero.

The sole observed direct call is at `0x0041901C`, inside cleanup owner
`0x00418EE0`. The caller compares supervisor `hintMode` at `0x00491D6A` with 2,
writes `hint/hint_auto.txt` only when equal, then cleans the owner's lists.
This is automatic serialization and overwrite, **not file loading**. The
separate input loader is `0x00419960`.

## Partial layout

`HintOwnerView` preserves a constructor-observed size of `0x1A8`. Export walks
eight intrusive-list sentinels beginning at `+0x78`, with next pointers at
`+0x7C` and a 12-byte stride. Loader reconstruction now types the first list group as well; remaining
owner storage is still partial. See HINT_LOAD.md.

`HintEntryView` is `0x88` bytes. Relevant fields are position at `+0x00/+0x04`,
an embedded entry/next/previous link at `+0x0C`, text beginning at `+0x1C`,
count/alignment/base/time/remaining at `+0x60/+0x64/+0x68/+0x6C/+0x70`, scale
at `+0x7C`, and BGRA bytes at `+0x84–+0x87`. i386 assertions guard the layout.
This is an exporter view, not a claim that every field or original class is
recovered. Loader review established a 65-byte copy and subsequent string read. The
current text[68] view includes constructor-zeroed trailing storage without
claiming the original array declaration; see HINT_LOAD.md.

## Preserved serialization behavior

- Allocate a 4,096-byte temporary buffer, attempt to create `hint`, then open
  the output with the existing `OpenWriteFile` overwrite protocol. Failed
  open skips serialization; final close handling and free still occur.
- Preserve the CP932 comments, local timestamp, CRLF layout, and literal
  `Version = 0.0`. No UTF-8 substitution or escaping is added.
- Visit stages 0 through 7. Skip entries whose remaining count is zero; emit
  at most 255 surviving entries per stage. Empty stages have no section.
- Snapshot each node's next pointer before formatting or writing its entry.
- Negative remaining counts reproduce the target's second `Tips` line:
  formatting is conditional, but the subsequent buffer write is not.
- Preserve coordinate truncation, RGB output order, one-decimal scale, and
  name tables. The base table intentionally skips `boss_card_11`. Unknown
  base/alignment values pass a null pointer to `%s`, without a new fallback.
- Keep the existing **void** `FileSystem::WriteToOpenFile` API. Its result is
  unused here. Writes compare byte counts rather than the Windows BOOL result.
  A short write closes the reloaded global handle, releases the critical
  section and decrements its count without invalidating the handle or stopping
  export. Later writes and final cleanup can repeat those operations.

These observations preserve target behavior; they are not recommendations for
a safe general-purpose parser or file-writing API. Allocation/localtime checks,
bounded formatting and text escaping have not been invented.

## Tests and limits

`scripts/test-hint-export.py` compiles the actual exporter with synthetic
dependencies and extracts the actual unchanged writer and close bodies from
`src/FileSystem.cpp`. Its fixture checks the complete positive byte stream,
183 base/alignment combinations, all eight stages, emitted-count boundaries,
negative remaining, next-link mutation, open/allocation ordering and repeated
short-write cleanup. It does not execute the game or write actual hint files.

The default command runs 12 functional mutation controls. A mutation counts
as detected only with exit code 1 and the labeled `HINT_CHECK_FAILED`
diagnostic; sanitizer failures are rejected as mutation evidence. Explicit
Python checks remain active under `python -O`. `--skip-mutations` runs the
same baseline fixture for lightweight CI, explicitly omitting the controls.

Null `%s` arguments are observed before host formatting, and float conversion
fixtures use finite representable values. Tests do not establish VC7 CRT
exceptional conversion/formatting equivalence or original runtime safety.

The native cleanup caller, full object lifetime graph, production global/table
storage, Windows runtime closure and exact code generation remain unresolved.
Standalone writer `0x0044B740` receives no new source or exactness claim from
this exporter. A successful host fixture or diagnostic compile is not native
exactness evidence.

The first diagnostic link used `Hint.cpp` with `FileSystem.cpp` and the real
decorated `ExportAutomaticHints` entry. It compiled successfully, with a
2,669-byte PDB-owned contribution versus the 2,247-byte target. This establishes
syntax/layout compatibility in that diagnostic context only; it supplies no
exactness or native caller-graph closure. The diagnostic PE SHA-256 was
`49dfedd3c1f62fce3233b7ff4e809cbbdca615bb0fddb5a8ec41aca500cd4aeb`.
