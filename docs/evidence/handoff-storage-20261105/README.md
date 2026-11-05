# TH10 2026-11-05 handoff storage cleanup — attested and selective

This is a **local private-artifact** cleanup, independent of source recovery
and of canonical byte-exact function credit. It accompanies the corrected
[agent handoff](../../RE_HANDOFF.md) and
[artifact restoration instructions](../../RE_ARTIFACTS.md).

The TH10 original Japanese v1.00a target identity remains
SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

## Audited generated-image archive

Only selected **regenerable compiler experiments** from the ignored
build/gpt-web subtree were candidates for pruning. We retained
**13 current frontier witness subtrees (199 files / 9,042,285 payload
bytes)**, including the full 14KB Enemy ECL dispatcher, ANM 9,964-byte
executor, ECL database inserter, near-exact text, sound, popup, Item
draw and Chain images. Their precise relative paths are in
[cleanup-summary.json](cleanup-summary.json).

The remaining **7,243 generated files** comprised **254,774,405
expanded payload bytes**. Before removing any expanded file:

1. A private gzip-compressed JSONL manifest recorded each relative path,
   size and SHA-256 for every selected original file.
2. A 13,907,599-byte zstd tar archive was created containing exactly
   those paths, no source code, no target and no protected toolchain data.
3. A separate streaming tar reader decompressed and independently hashed
   **every one** of its 7,243 regular members and compared each length,
   digest and path against the original manifest. All checks passed.
4. Immediately before deletion, every selected expanded file was re-hashed
   and checked against the verified manifest again; only identical
   build/gpt-web files were unlinked. Empty experiment directories were
   removed, but all 13 preserved witness directories remained.
5. One actual member was independently restored to a temporary directory,
   matched by size and SHA-256, and the temporary copy was removed.

New ignored local archive:

    .analysis/archive/gpt-web-regenerable-experiments-20261105.tar.zst

Its SHA-256 is:

    c3c38c20fa7c6f303911973988b4582f0d012a62f0dd2c9fbde24fcc9f03ebde

Private checksum manifest:

    .analysis/maintenance-20261105/gpt-web-regenerable-manifest.jsonl.gz

No Git commit contains the archive or private manifest. The exact paths,
source/witness retention decisions, archive checksum, audit results and
byte accounting are in the tracked
[cleanup-summary.json](cleanup-summary.json). The private scripts
for compression, byte verification and guarded deletion are under
.analysis/maintenance-20261105 and remain locally available.

## Space saved and excluded data

Immediately after archiving and before deletion, combined build and
.analysis occupied **611,584,760 bytes**, including the new archive.
After removing the verified generated copies and the separately regenerable
build/pycache (300 files, 5,809,259 du-bytes), the same measured trees
occupied **353,014,236 bytes**. The **post-archive measured saving was
258,570,524 bytes (~246.6 MiB)**. Subtracting the 13,907,599-byte new
archive yields an approximate **244,662,925-byte (~233.3 MiB) net
overall saving**, with small private manifest/verification-log overhead
not included in that approximation.

**Protected and unchanged:** resources/th10.exe, .tools, Ghidra project,
_reference, earlier compressed archives, .analysis/gpt-web research
scripts and machine receipts, all tracked source/config/docs/Git history,
and the entire build/match-linked selected canonical producer tree.
The new archive augments the previously protected 2026-11-04 archive;
it does not replace it. The Nov 4 savings are historical and must not
be double-counted as Nov 5 savings.

## Future agent instructions

Start with docs/RE_HANDOFF.md and docs/RE_ARTIFACTS.md; verify the
private target and current Git tree, then choose **specific** match units
rather than running a full-cohort replay. The archive and manifest are
ignored and must be transferred separately to a new machine. Use
selective zstd/tar extraction only for a missing experiment path and
verify its recorded source/header epoch before trusting the old
PE/MAP/PDB as current compiler evidence. An archived PDB, a matching
length or a same-candidate byte comparison is never a new exact match.

The seven-stage repository CI/ledger checks were run after this change.
There is no claim to have linked or run the full reconstructed game.
