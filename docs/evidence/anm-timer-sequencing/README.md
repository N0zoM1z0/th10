# ANM timer sequencing in the current link graph

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `ed17b0d`. Reviewed on2026-10-21.

Three controls and an unchanged positive yield **no source/profile adoption
and no new exact credit**. ANM ExecuteScript remains9964 bytes/nonexact;
ECL Run remains7020/all189/62locals with27 ordinary differences; the accepted
dispatcher remains14464/14416. All maintained32 source inputs and the current
canonical carrier stay unchanged. The original three large goals stay active.

## Fresh target and compiler evidence

Direct local Ghidra review confirms the slow timer tail at441377 loads the
scale through ECX first, then adds subframe at VM+64. Current native code loads
subframe first and adds through ECX. The maintained inline Tick source already
spells `*scale + subframe`; the generated load order alone therefore cannot
identify original source operand order.

An unchanged private AnmManager.cpp positive reproduces every raw byte of all
three complete native large bodies:9964,7020 and14464. The current ANM case
diagnostic is7478/7995 comparable bytes,90/92 target-sized groups, absolute
gap2. SCALE_TIME remains diagnostically equal; this is no parent exact credit.

Two source controls replace only ExecuteScript's final Tick call with its
ordinary complete mutating body. One preserves `*scale + subframe`; the other
uses `subframe += *scale`. Both produce **complete raw equality of all three
large native bodies**. Each retains35 complete target-bound helpers4424/all118.
These are unchanged complete bodies, not merely unchanged sizes or scores.
No volatile, barrier, fake return, padding or register annotation is introduced.

One additional control adds `/Op` only to the ANM support's unchanged source,
retaining /GL,/GS and every other source/profile. The compiler emits no ignored
option warning; the three old unknown-pragma warnings also occur in the positive.
The full ANM owner grows9964->10140. Target-sized groups fall90->56/92 and
absolute case-size gap grows2->570. The scale guard grows56->82; RGB Evaluate
changes820->769. Only33/35 guards,3548/all98, remain exact. This current-context
profile hypothesis is rejected, without identifying original production flags.

The first cached-anchor link fails on eight new CRT math symbols. That is a
terminal link failure, not a whole-owner comparison. The same compiled game
objects are retained and hash-verified. Regenerating only private diagnostic
anchors exposes three more symbols before the full link succeeds; all11 added
names and their bindings are recorded. No game source is recompiled during
that recovery, and no function return/ABI is changed. The anchor image remains
non-runnable. The [native summary](native-summary.json) distinguishes the failed
attempt from the completed profile control.

Complete Run still compares6993/7020 after all189 fields/62locals and table
coverage checks. All7020 target-replayed bytes equal the positive, SHA-256
`830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472`.
Under `/Op`, complete native Run also equals the positive after verified field
rewrites. The same-sized dispatcher has21 changed REL32 fields without proven
semantic identities in this comparison. No ordinary-byte score or native
identity is claimed for it; unknown fields are not blanket-masked.

Each experiment compiles only the private ANM TU at nice10 and reuses nine
hash-verified game objects. This is an incremental diagnostic, not a new
independent complete cold or canonical replay. Actual profiles, source closure,
object hashes, completed PDB extents, complete guard results and whole-body
identities are pinned in the native summary. Production source/header/recipes
and ledgers remain unchanged, so no historical canonical replay is needed.

The adjacent TH09 `int Tick()` source is hypothesis material only. TH10's
separately exact72-byte Tick and the earlier failed74-byte return-current
candidate constrain that analogy; no new inconsistent header/return-type
experiment is executed here. This does not prove original C++ return spelling.
See [the earlier Tick proof](../stage-real-callback/README.md).

Ghidra runs readOnly/noanalysis with rollback, CPU1 and768 MiB. Disk identity and
complete mapped text401000..465A80 agree before/after:412289 bytes,
SHA-256 `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
No target byte or durable database metadata is changed.

## Reproduction and limits

```sh
scripts/repo-python scripts/diagnose-ecl-run-native.py \
  --unit enemy-cancel-bullet-pattern --artifact-dir build/match-linked/EnemyItemDrops
scripts/repo-python scripts/report-anm-execute-table.py --check
```

The complete Run diagnostic exits1 for its known27 ordinary differences.
Private source/control recipes and receipts are under
`.analysis/gpt-6.1-sol/anm-timer-sequencing-20261021/`; read its recovery note
before using a runner. The three terminal duplicate native directories are
pruned/3124040 bytes; the completed `/Op` failure, source copies and complete
source/producer/strict/guard reports remain. Original class/TU/profile,
native-product/runtime, semantic and port gates stay open. The
[validation receipt](validation-summary.json) records final gates and cleanup.

All104 standard CI checks pass in one invocation. Fresh current ANM9964 case
diagnosis remains7478/7995,90/92,gap2; complete Run7020/all189/62locals retains27
ordinary differences. Final toolchain/tracking/progress/docs/whitespace gates
pass. All32 maintained raw inputs,15 current canonical artifacts and the retained
profile-failure artifacts remain hash-current after the gates. No extra canonical
replay is claimed or needed for unchanged production bodies/interfaces.
