# ECL Run layout controls and explicit source routing

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Started at `15cd8b1`. No game source, header, canonical match recipe, boundary,
origin or exact credit changes. The maintained diagnostic gains an explicit
private-source selector and corresponding guards.

## Source-transform correction, 2026-10-22

The later [source-route audit](../ecl-scratch-route-audit/README.md) reconstructs
the recorded source patch and verifies its compiled-source hash. Three labels
(shared-scalar-scratch, reference-cursor, named-math-destinations) actually repeat
Run-before-HostRun because an earlier broad elif intercepts them. The four
profile controls also include that reordering. Their recorded native results
remain valid for those actual inputs; they do not establish neutrality of the
three unexecuted source forms. The table below reflects the corrected scope.

## Fresh target and baseline observations

The selected ten-TU context is `enemy-drop-item-counts`, with the real Enemy
dispatcher entry and the existing scale support profile. Its entire recipe,
32 source/header pins and 15 native artifact pins freshly equal the earlier
[actual cold](../chain-source-closure/native-summary.json). All 107 maintained
native source/header files are also pinned unchanged. This campaign does not
perform another complete cold or claim that inspection alone proves a producer.

Fresh local Ghidra reads attest the inclusive 412289-byte mapped text range
401000..465A80 before/after, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The readOnly/noanalysis job rolls back its transaction and uses one CPU/768 MiB.
It reviews 12 bounded residual spans and the HostRun callers. Run has two direct
callers,44FD33/44FD45, inside HostRun44FD10; HostRun's two calls originate inside
40DC80. Ghidra signatures and source names remain provisional.

The unmodified baseline diagnostic reproduces all 7020 owned bytes with 6993
matching bytes, 189 bound fields/62 local destinations and the complete exact
171-byte ReadFloatValue callee. The 27 ordinary differences comprise four thread
register bytes, five commuted-addressing bytes, 16 stack-home bytes and two
common-latch register bytes. All 60 table words and 88 selectors participate.
Generic decoding reaches7019; the explicit one-byte selector annotation covers
the suffix. The generic flag is not a claim that data was left untested.

## Executed controls

| Control family | Executed models | Complete Run outcome |
| --- | ---: | --- |
| Wider math/scratch/result declarations |3|7020/6993, same entire replayed body|
| Move Run before context methods or before HostRun |2|Same entire replayed body|
| Labels for shared scalar scratch, cursor reference and named destinations |3|Actually repeat Run-before-HostRun; intended transforms not executed|
| Add EHs, G6 or G7, or disable GS; also move Run before HostRun |4|Same entire replayed body in that reordered-source context|
| Move ECL last, reverse supports, or select the real runtime caller as entry |3|Same entire replayed body|
| Select HostRun itself as external entry |1|Run unchanged; complete HostRun guard rejected151 versus146 bytes|

All 16 comparisons bind the same complete target field contract. After replay,
each entire 7020-byte Run has SHA-256
`830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472`.
This compares complete bodies, not merely equal scores. Fifteen models also
pass nine selected complete same-image ECL guards:1137 bytes/all18 fields.
Those guard templates are independently checked equivalent to current canonical
semantic descriptors. The HostRun-entry model stops at its extent rejection;
no nine-guard pass is claimed for it.

Source/profile controls recompile only the ECL object and reuse nine verified
game objects. Link controls reuse all ten objects. None is a full cold. The
tested neutral forms do not establish original declarations, temporary lifetimes,
types, flags, TU partition or ownership. Unsupported original-source hypotheses
remain unknown. No partial byte or case credit enters the ledgers.

## One maintained diagnostic

Earlier private trials copied the diagnostic to change two source selectors
and ROOT discovery. Every historical copy is checked byte-for-byte against
exactly that transformation; no comparison rule differs. The new explicit option
removes this copying step:

```sh
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit enemy-drop-item-counts
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit context \
  --context-manifest .analysis/gpt-6.1-sol/ecl-run-register-pressure-20261022/reference-cursor-context.toml \
  --ecl-source .analysis/gpt-6.1-sol/ecl-run-register-pressure-20261022/reference-cursor/EclVm.cpp
```

The private input must exist inside the repository and be compiled exactly once
by the selected context. Missing, duplicate, outside-root and escaping-symlink
routes are rejected; there is no implicit private-source fallback. The input and
both manifests are checked unchanged during comparison. Its observed source
hash does not attest artifact/source correspondence; producer pins remain a
separate requirement. Target/callee/field/extent/table/selector rules are intact.

Both actual default/private CLI checks retain7020/6993, all189/62 and the exact
171-byte callee. Omitting the selector for the private context returns unsupported
before owner comparison. Source-selection tests cover three valid routes/eight
rejections, and all16 existing table/selector coverage controls pass. Fresh full
CI passes109 groups with all369 frozen inputs unchanged. Classified cleanup
removes27654818 logical file bytes across29 owned trees and95
superseded files. The baseline, one private-source API positive and the actual
HostRun extent failure remain. See [validation](validation-summary.json).

[Compact evidence and source transformations](evidence.json) retain the current
baseline, all executed cases and explicit limitations. Private recovery:
`.analysis/gpt-6.1-sol/ecl-run-register-pressure-20261022/RECOVERY.md`.
Authored exact remains401/73225; overall1242/139735. Windows product/runtime
closure remains open; semantic reconstruction and portability have not started.
