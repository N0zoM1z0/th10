# TH10 ECL enemy world-to-screen anchor @0x00418810 — full 64-byte canonical exact

**Current result (2026-11-03): 64/64 exact; two independent cold links; 8/8 fields.**
The earlier isolated-entry NONEXACT diagnostics below remain useful as a
controlled explanation of the genuine Enemy ECL caller-context requirement.

Original Japanese TH10 1.00a, 64-byte function `0x00418810..0x0041884F`,
complete native RET and reviewed source-absent boundary. The function is
**really called twice** in the 14,416-byte `EnemyRuntimeView::DispatchEclInstruction`
at native `0x0040EE75` and `0x0040EF6B`. Its native register-private input
is EAX = `EnemyRuntimeView::worldMotion.position` (+0x2C), with no explicit
stack arguments, and its result in EAX points to shared storage 0x00497D80.

The original first reads the low byte of the state word at 0x00497D8C,
conditionally sets bit zero in that same word, then publishes three floats:
`source->x + 224.0f` to 0x00497D80, `source->y + 16.0f` to 0x00497D84,
and a raw Z copy to 0x00497D88. The two binary32 offset values are taken
from original `.rdata` at 0x00470B4C and 0x00470B48. Both call sites copy
these resulting coordinates into ANM VM position offsets at VM+0x340;
a separate flagged branch copies the original world position instead.
The state flag is **part of the function**, not a dead global write.

New `src/EclWorldAnchor.cpp` implements the complete typed source surface
as `PublishEclWorldAnchor`, using the existing 12-byte `AnmFloat3View`
layout and descriptive linkable global views. The old static `EnemyGetAnmPosition` in `EnemyEclDispatcher.cpp` was
independently found to be an **incomplete implementation** of this same
observed target function: it did not update the state flag. It is now
replaced by calls to the actual maintained `PublishEclWorldAnchor` declared
in `src/EclWorldAnchor.hpp`; no parallel shadow implementation is retained. Its original C++ source name,
actual data owner and compiler-private function declaration are not proven.

## Full-extent compiler experiments

- Pinned VC7.1 `/GL` selected single-source PDB owner: **64 vs 64** bytes,
  **21/32** ordinary comparable bytes and 8 candidate address fields.
- Retaining the actual source X/Y input reads via two scoped const-volatile
  references matches the original `FLD input; FADD constant` order and
  improves the selected owner to **64/64 size, 26/32** ordinary bytes.
- A genuine **two-TU** experiment temporarily rewired all three maintained
  Enemy ECL source references from its old static helper to this new owner
  and compiled with the actual `EnemyEclDispatcher.cpp` WPO producer.
  It retained **64-byte size, 26/32 ordinary bytes and 8 fields**;
  six register-encoding bytes remain different (native EAX input/ECX flag
  versus candidate ECX input/EAX flag). This is a compiler ABI/context
  difference, not permission to mask unreviewed bytes. The temporary
  changes to `EnemyEclDispatcher.cpp` were restored byte-for-byte.

Private reproducible two-source experiment and exact mismatch inventory:
`.analysis/gpt-web/20261103/ecl-anchor-real-caller-experiment.py` and
`ecl-anchor-real-caller.json`. A full 64-byte match and eight **independently
target-bound** fields have **not** been proven. No canonical unit/matches
row or source-authorship inference follows; this is a **source-present
NONEXACT** recovery. Existing ANM/Stage neighbors get scoped cold replay
only; the 14,416-byte parent ECL dispatcher remains NONEXACT. No full cold
product or historical cohort replay is used.

## Follow-up: maintained ECL dispatcher now consumes the real helper

After the isolated caller-context experiment, the actual maintained
`src/EnemyEclDispatcher.cpp` was corrected in the live worktree: it now
includes `src/EclWorldAnchor.hpp`, drops the old static
`EnemyGetAnmPosition` helper (which failed to publish the 0x497D8C flag),
and directly calls the typed `PublishEclWorldAnchor` in its three ANM spawn
branches. This **retains the original flag side effect** and avoids two
competing partial implementations of the same target operation. The new
helper definition also declares `__declspec(noinline)` to keep an explicit
separate compilation boundary. That declaration models the observed
function boundary; its original source qualifier and private EAX ABI remain
unproven.

Original PE disassembly establishes **two** concrete direct E8 calls to
0x00418810 within the 14,416-byte ECL dispatcher (0x0040EE75 and
0x0040EF6B). The three maintained branch-level references are not
asserted to map one-to-one to those physical call sites; compiler branch
sharing/inlining and the rest of the ECL dispatcher remain open.
A fresh **persisted-source**, true two-TU `/GL` link confirms the same
64/64-byte candidate with 26/32 comparable ordinary bytes and eight
candidate field positions. It **still does not** prove exactness, so the
previous control's `NONEXACT` status is unchanged. Retained result:
`.analysis/gpt-web/20261103/ecl-anchor-persisted-caller-probe.log`.

For the touched real ECL dispatcher TU, narrow cold replay of
`enemy-configure-interrupt` (48 bytes), `enemy-wait-for-effect` (44 bytes)
and `enemy-find-laser` (22 bytes) passes **114/114** in one cold normal-COFF
artifact. This is a scoped regression guard, **not** a substitute for a
14,416-byte canonical complete owner comparison. Full historical cold replay,
product linkage and runtime equivalence remain unclaimed.


## Canonical closure with the *real large dispatcher as entry*

The earlier two-TU test chose **the helper** as the linker entry, so VC7.1
still emitted an ordinary ECX pointer / EAX bitmask. Its observed 26/32
ordinary byte agreement was correctly nonexact but was **not evidence that
real caller WPO could not recover the ABI**. Reversing the compiler graph's
root resolved the discrepancy: `EnemyRuntimeView::DispatchEclInstruction`
(`0x0040E770`, target **14,416** bytes) is selected as the actual entry;
`EnemyEclDispatcher.cpp` is the primary `/GL` input and the genuine
`EclWorldAnchor.cpp` is the support TU. Three previous source call sites
which used the incomplete `EnemyGetAnmPosition` wrapper are now bound to
`PublishEclWorldAnchor` in the maintained dispatcher. The original target
has two direct calls into this specific 64-byte function at
`0x0040EE75` and `0x0040EF6B`, with incoming EAX pointing to actual
EnemyRuntimeView+0x2C world motion coordinates. These direct original
observations, not the old helper code, set the source direction.

Pinned VC7.1 SP1 `/GL` naturally promotes the new helper's parameter into
**EAX** and retains bit one as **ECX**, recovering the original x87 read,
FADD literal, flag-bit OR, world anchor publication, and return registers.
The full linked helper contribution is **64/64 native bytes**, every
**32/32 ordinary byte** matches, and each of **eight** independent
source/target address fields is bound:

- +0x02/+0x11: 0x00497D8C state flag;
- +0x19: 0x00470B4C exact +224.0f;
- +0x1F/+0x3B: 0x00497D80 persistent XYZ anchor base;
- +0x28: 0x00470B48 exact +16.0f;
- +0x2E/+0x36: base+4/base+8 of the **same** world anchor,
  i.e. 0x00497D84/0x00497D88. The diagnostic neighboring-data anchors
  are not substituted as separate storage owners.

Two different zero-reuse cold builds of canonical
`ecl-world-anchor-00418810` produce **distinct** PE SHA-256 and PDB GUID
identities, preserve PDB source-module provenance for the actual support TU
and pass the unchanged strict whole-owner field/byte comparator. Complete
receipts retained at
`.analysis/gpt-web/20261103/world-anchor/cold{1,2}.json`.
No asm stub, copied target bytes, artificial register declaration or
unbound field masking is used.

The **14,416-byte Enemy dispatcher itself remains NONEXACT**. Its selected
minimal two-source probe is only 12,804 bytes compared with the original
14,416 because many other actual ECL/ANM support sources are omitted. This
64-byte exact callee does **not** imply parent or whole-game exactness.
The original TU layout, physical global storage and retail runtime behavior
are unproven. Only selected affected exact helpers are cold-replayed; no
full historical replay is undertaken.
