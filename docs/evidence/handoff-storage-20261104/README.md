# 2026-11-04 TH10 selective storage cleanup — verified archive, 347 MB reclaimed

Purpose: prepare an accurate local Git and private-toolchain handoff without
retaining hundreds of megabytes of obsolete **expanded** compiler permutations.
This operation did **not** alter the original TH10 PE, Ghidra project, locked
compiler/Wine toolchain, source files, `build/match-linked/` accepted output
producers, `.analysis/gpt-web/` research scripts/receipts, or older archives.

## Audited compression and deletion

Two old-source-epoch experiment trees:

- `.analysis/gpt-6.1-sol/` — 7,651 files;
- `build/gpt-6.1-sol/` — 3,410 files.

Their **11,061 regular files** contained exactly **333,340,746 payload
bytes**. Before deleting their expanded copies, we created
`.analysis/archive/historical-compiler-graphs-20261104.tar.zst`, a
**20,253,646-byte** zstd-compressed tar archive with SHA-256
`7917cbed088dbff5d12ec914e33975c79fc9999bc6ded0266b94da0a8472e513`.

A first pass recorded **each expanded original file's content SHA-256 and
size** in the ignored compressed manifest
`.analysis/maintenance-20261104/historical-compiler-graphs-manifest.jsonl.gz`.
A separate streamed tar verification decompressed **every archive member**,
computed its own SHA-256, compared the expected size and hash, and verified
no missing, duplicate/unexpected or non-regular members. The archive passed
**11,061/11,061 checks**, including all 333,340,746 bytes. Its own SHA-256
was rechecked immediately before deleting the two expanded roots.
This is a **recoverable archive, not a pointer list**; restore precisely
scoped old paths using the commands in [RE_ARTIFACTS.md](../../RE_ARTIFACTS.md).
Original source-epoch hash/pin contexts must still be revalidated when
restored; the archive is not a new exact codegen claim.

Only after this verification, we removed the two expanded directories and
two **regenerable and unarchived** cache trees, `build/pycache/` and
`build/probe-ltcg/`. The measured combined `.analysis` + `build` size went
from **679,305,329 to 332,262,934 bytes**, a net reduction of
**347,042,395 bytes (~331 MiB)** *after* counting the new compressed archive.
The compressed archive adds about 19.3 MiB while replacing 318 MiB of old
payload. Caches account for the remaining savings. No source or canonical
proof was classified as expendable simply because it was nonexact.

The tracked [machine-readable summary](cleanup-summary.json) records archive
identity, roots, counts, checksums, before/after sizes and deletion status.
The private file-level manifest and verification log remain alongside the
archive but **are ignored by Git**. For another agent on a different
workspace or machine, transfer the private archive, original target and
pinned tools via an approved channel. The Git repository itself contains
this receipt and the exact rebuild recipes but **not** the archived bytes.
