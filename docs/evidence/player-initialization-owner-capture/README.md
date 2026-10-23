# Player initialization: capture the callback owner before publication

2026-10-23, starting a21e516. Maintained PlayerInitialize4247F0 keeps its complete
1435-byte extent and improves from1195 matching/240 different bytes to
**1355 matching/80 different bytes**, after all49 independent address bindings.
It remains nonexact. Headers, layouts, ABI declarations, canonical recipes and
exact ledgers are unchanged; two candidate-ledger notes are updated.

## Maintained source and full reproduction

[The patch](maintained.patch) captures the actual registration owner through one
scoped volatile pointer read before publishing the update-node argument and
masked flags. It retains the real flags snapshot and removes the first compiler
barrier; four initializer barriers remain. All accesses serve the actual node
registration. No CPU fence, extra call, fake return, forced ABI or padding is
introduced. Original volatile qualifiers, barriers and concurrency are unknown.

The resulting first manager load uses EAX and its shorter absolute encoding.
Compiler loop alignment naturally changes from six to seven bytes; no alignment
bytes are supplied by source. Over the entire owner,200 replayed bytes change:
164 target repairs/four target-equal losses, net160. All ordinary instructions,
local control destinations and the internal alignment remain compared.
[The full80-byte residual and49 bindings](parent-differences.json) are retained.

Private and independently recompiled maintained Player objects reproduce all519
unique complete public PDB owners raw,140370 bytes. Against the preceding
maintained image, only PlayerInitialize changes among those owners. Static and
unowned fragments are outside that check; it proves neither target equality nor
product/runtime closure. Eight full shared guards pass1753 bytes/all99 fields.
The maintained reproduction compiles Player once and reuses16 pinned inputs;
this is not an independent whole-graph cold pair or a new exact acceptance.

## Controls that informed this change

[Evidence](evidence.json) binds19 complete images,55 new object compilations and
17 complete parent comparisons. Every image passes all eight full shared guards.
The two different-size owners are rejected before a parent score, never cropped.

| Control | Complete parent result | Outcome |
| --- | --- | --- |
| Actual GameManager caller; missing Stage loop dependency; all wrapper-call sources |1195 each|20/21/27 inputs respectively; complete replay neutral |
| Actual WinMain entry in the expanded graph |1195|Zero compiles/27 pinned inputs; neutral; diagnostic entry only |
| Unified canonical Chain/ANM global identities |1195|Complete replay neutral; no global/type changes adopted |
| Volatile containing-bitfield storage |1195|All17 inputs freshly compile; allocator becomes74 bytes, without a standalone exact comparison or credit |
| Inherited three-float storage / ordinary owner getter / commuted first vector sum / ANM-owner G7 |1195 each|Whole-parent neutral |
| First calc flags read through volatile view |1183|Regressive; not adopted |
| Capture manager, then compound flags and argument |1351|Recovers the short manager encoding |
| Publish argument before flags; or explicitly snapshot flags before owner capture |1355 each|Whole-parent equal; snapshot form selected |
| Also capture draw owner before its flags |1207|Coupled allocation regresses; independent gains cannot be added |
| Snapshot selector values on the selected source |1347|Regressive; original references retained |
| First timer through existing member / first extent through vector-value assignment |1438 /1446 bytes|Strict complete-extent rejection |

Prepared alternatives are listed separately in evidence.json and are not
reported as executed. The earlier [parameter/type controls](../player-initialization-parameter-controls/README.md)
retain their original inputs; its inherited-storage, bitfield and getter
suggestions are now tested here. The private Stage dependency is not adopted and
its214-byte source-presence/exactness state remains open.

## Incoming-reference pitfall and remaining large-owner work

[The binder caller observations](binder-caller-observations.json) cross-check a
new full-text-attested, read-only Ghidra review against raw TH10 instructions.
Ghidra lists21 incoming calls to43E710 but omits six real calls in complete
95/118-byte position-creator owners. All eight creators correctly call43E710;
the combined verified lower bound is27. Do not change those six correct calls
because they are missing from the reference list. The current ledger's earlier
ANM-manager-slot description of404450 is also corrected from raw operands to the
observed Stage-layout loops; original classes/TUs remain unknown.

The expanded native graph has21 direct child calls with a different owner
distribution. Neither a matching count nor all maintained source call sites
establishes target caller closure. Review real call operands and missing or
differently folded owners before inferring a source/ABI repair. The wrapper
remains200/nativeEDX versus202/targetESI, with its complete273-byte core guard exact.

Of80 differences,37 precede150,21 are in150..1FF,11 in200..3FF and11 in400..59A.
Continue with callback/VM receiver lifetimes, the early timer constant, the
commuted vector component/scratch-store schedule and selector registers. Keep
every byte and independent field; a small dependency match is not the objective.
The owner remains part of the large-function exact campaign.

## Recovery and bounded storage

Current local producers are under
`.analysis/gpt-6.1-sol/player-stage-caller-20261023`. Use manifest-final.json,
common-final.py, probe-maintained.py and bound-parent-final.py for maintained
source; initial manifests and older producers intentionally retain historical
pins and must not be weakened or rerun against changed source.

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-stage-caller-20261023/bound-parent-final.py maintained-17
```

Expected result: mismatch,1355/1435,49 fields. Keep complete maintained-17 and
calc-snapshot-before-owner carriers, plus all-wrapper-callers-27 as the old-source
caller-distribution witness. The prechange Player source is retained privately
with its exact original hash; never restore it over maintained src/.

[Cleanup](cleanup-summary.json) records16 closed builds and two completed67-file
private type snapshots archived losslessly, with every member read back and
length/SHA-256 verified before removal. Current graphs, selected source, reports,
producers, canonical caches, original target, toolchains, databases and all nine
unrelated D3D files are retained. Restore archived ignored paths when required:

```sh
tar -xJf .analysis/player-owner-capture-cleanup-20261023/completed-controls.tar.xz -C .
```

[Validation](validation-summary.json) states the actual focused scope. Full CI and
broad cold replay are omitted under the operator preference. Exact counts and
the product/runtime/semantic/port gates remain unchanged.
