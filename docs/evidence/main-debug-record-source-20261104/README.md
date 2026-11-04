# TH10 Main supervisor's debug record matcher — 205-byte source

Original game TH10 v1.00a body: `0x00420DD0..0x00420E9C`, 205 bytes,
`RET 12`, caller supplies `MainSupervisorView` in a private EAX seam.
Read-only Ghidra and full target disassembly identified the existing
supervisor's `+0x760` 32-bit remaining-length counter and `+0x764` text
buffer pointer. The latter is already used and released by the real
`MainSupervisorView::ReleaseGame` source. This work names the formerly
untyped +0x760 member in that actual class, preserves layout and adds
`src/MainDebugRecords.cpp`, instead of introducing a shadow storage owner.

Behavior covered: null buffer returns zero; any input beginning `debug`
returns zero; the original also compares its fixed `0100a` build version
against the six-byte `debug` string before scanning. For each nonempty
record, compare the first five key characters; on key match, parse two
integers via `sscanf` at record+6 with `%d %d`, and return zero only if
both equal the requested numbers. Otherwise, advance past the next `\n`
and subtract the consumed bytes from the remaining count. Return -1 if
there is no match. The original assumes newline-terminated records; it
has no `strchr` failure guard, which is intentionally not invented here.

The pinned VC7.1 SP1 `/GL` compiler accepts the new source. Its native
linked member emits a **196-byte PDB owner**, not the original 205,
with a different receiver ABI, call scheduling and register allocation.
This is **source presence only**, not a byte match or original ABI/name/TU
claim. The method is descriptive, and the exactness ledgers are unchanged.

Reproduce the scoped diagnostic with
`.analysis/gpt-web/20261104/probe-debug-record.py` and private
`build/gpt-web/20261104-debug-record/native/`; do not treat them as
canonical receipts. The affected existing exact Main release owner is
replayed separately after the header field-name refinement.
