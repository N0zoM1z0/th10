# TH10 next-agent cleanup — 2026-11-07

**Purpose:** prepare for a new reconstruction agent, correct live-vs-historical
handoff claims, and reclaim allocated storage without changing exactness or
removing essential analysis. Private ignored state does not travel in Git.

## Verified deletion

A guarded private cleanup driver chose the generated native `.exe/.obj/.pdb`
files of **26 inspected rejected/superseded experiment variant families**.
The experiment drivers, current-source snapshots and small MAP/JSON/log
results were kept. For unresolved owners, a useful baseline or current
witness was retained; wholly rejected panels can be rebuilt from their saved
trial source/driver and other current-owner witnesses. Complete first-exact
colds, canonical native artifacts,
current ANM/ECL/Enemy heavyweight frontiers and real six-source statistics
caller proof are explicitly outside the selected deletion path set.

- **1,342 generated native files** deleted, not tracked in Git;
- **74,380,757 payload bytes** removed;
- **76,615,680 allocated bytes** reclaimed from `build` (73.1 MiB);
- **no duplicate archive created**; historical private archives untouched.

A separate local cache pass deleted **289 `.pyc/.pyo` files**
(**5,033,902 payload bytes**) and removed **60 empty cache directories**,
reclaiming **5,894,144 allocated bytes**. The combined cleanup removed
**1,631 files / 79,414,659 payload bytes**, reducing allocated file and
cache footprint by **82,509,824 bytes (78.7 MiB)**. After adding compact
private audit manifests, the *net* `build/` + `.analysis/` + `scripts/`
allocation decrease was **82,108,416 bytes (78.3 MiB)**.
The final seven-stage CI did recreate **248 Python cache files**; they
were pruned a second time after CI, recorded separately in the private
`cache-post-ci-result.json`. These regenerated files are **not counted a
second time** toward cumulative storage savings. Future Python execution
may recreate bytecode without invalidating the cleanup evidence.

The private one-use driver and SHA-256 manifest are
`.analysis/maintenance-handoff-20261107/prune-native.py` and
`native-deleted.jsonl` (manifest SHA-256
`314ef076e4ff105628ae88ab8d33f61b33bb3f71c1f547a384fb247958c8e2d9`).
**This manifest is not a restorable archive.** Selected rejected binaries
must be regenerated with the original pinned toolchain and retained
`.analysis/gpt-web/` experiments. Do not expect earlier PDB/MAP/OBJ source
epochs to compile against new source. Previous archived cleanup experiments
*are* recoverable from their separately retained archives/receipts.

## Protection and validation

The driver verified target/canonical/statistics exact proof hashes before
and after native cleanup, verified the entire `build/match-linked` allocation
was unchanged, and did not traverse `.tools`, `_reference`, Ghidra,
`.analysis/archive`, the private original executable or source/Git data.
The selected 2026-11-07 canonical exact count is still **1,496 functions,
195,808 bytes**, including **587 confirmed-authored exact /125,718 bytes**.
This is ledger credit, not proof of a runnable reconstructed game.

Read [RE_ARTIFACTS.md](../../RE_ARTIFACTS.md) for current disk snapshots and
archived recovery. Read [RE_HANDOFF.md](../../RE_HANDOFF.md) for current
source/hash pins, unresolved large owner/frontier context, and bounded
replay command. For changes to documentation or deletion inventories use
lightweight CI only; **never run the complete historical cold replay**.
