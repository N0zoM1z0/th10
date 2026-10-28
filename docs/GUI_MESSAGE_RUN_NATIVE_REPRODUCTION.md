# GUI message Run: fixed native reproduction and integration gates

Historical test record: the construction/lifecycle host suites cited here were
retired on 2026-10-28. Commands for deleted scripts apply to commit `47d9993`,
not the current tree. Their results are historical evidence, not requirements
for exact reconstruction; follow [the current workflow](RE_WORKFLOW.md).

The tracked diagnostic is `scripts/diagnostics/gui-message-run/diagnose-gui-message-run-native.py`,
with adjacent `native-inputs.json`. Both are byte-identical to the executed v2
baseline. This is a code-generation diagnostic, not a runnable rebuilt game or
an exactness certificate. The native findings are in GUI_MESSAGE_RUN_NATIVE_BASELINE.md.

## Pinned inputs and actual graph

Baseline checkpoint 50b971feb9999a14d75461f238f059b2970fa690 plus reviewed v2 patch
bf0d7404b116e55b37546a92a6feccd10a804e6e64fd39f94b9089681b71987d,
manifest f8fa8f095d5ab2af644371367f4c16796a7b5b59656cd83e034f32353c44b8e4.
The subsequent host-only non-PIE patch bfd4cf0ad257e7ac1df801b3bb9ecc4304ed4e82bd3e173c661304e7c6b1a3fe
changes neither production inputs nor native flags. The original v2 manifest
identifies pre-portability host driver bytes; do not claim those two current
host scripts still have their original hashes.

Driver SHA256: 241a51b768ac4e12f14b60e80cb4d2e207a80b31a52441f3258d155c4d28576c.
Inputs SHA256: 29b400654df8abb13d736ad07a4d9ed4e1bf5d413c8630c6362bd7428885044e.
The input map checks 43 raw files: ten graph TUs and 25 quoted headers, the fourth
non-graph changed source GameManager.cpp, and seven tool/config files. Canonical
match-units.toml also has its own complete-file hash gate:
2a0a98cb1963bf6d8daea859a8d4f06b1e87badfa64ccb166ed59ef3bae1beed.
Do not silently update this pin if future label-only binding migrations occur;
record and review that delta separately while retaining the executed baseline.

Primary Gui.cpp uses actual GuiView::UpdateStageElements→GuiMessageVmView::Run.
Ordered full-TU supports are AnmManager, AnmVmCreate, AnmVmId, EnemyEclDispatcher,
Player, FrontEnd, DecomphelpLeafAccessors, Main and Sound. Each has a real helper
or principal dependency role; no helper body was copied or retained artificially.
The accepted 417010/4175E0 definitions remain in th10::leaf in their original TU.
The canonical gui-score-add profile stays
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, with unchanged link profile. There
is no /GS or /EHsc delta, mixed normal support, profile sweep or new attribute.

## Commands from repository root

Use the pinned repository Python wrapper. System Python's Capstone version may
not satisfy the lock; this was observed on the remote host and is not a compiler
or source failure.

```
scripts/repo-python scripts/diagnostics/gui-message-run/diagnose-gui-message-run-native.py --root . --check-only
scripts/repo-python scripts/diagnostics/gui-message-run/diagnose-gui-message-run-native.py --root .
```

Perform ordinary target/toolchain preflight first. The dedicated output
build/diagnostic-gui-message-run-updater must be empty. The existing baseline
output is intentionally preserved; do not delete it for a blind rerun. Reproduce
in a separate compatible checkout or obtain review of any necessary new attempt.
The driver does not execute the image. Generated run-build.json, PE/PDB/map,
compiler/linker logs and local review manifests are ignored evidence, not files
shipped with this source patch.

## Required interpretation

A successful link still requires map plus same-start PDB contribution/function
proof for naturally retained owners. Do not infer extent from the next map
symbol. Account for actual static helper contributions separately; do not add
unrelated outlined helper bytes to Run or award fragment credit. Target Run owns
all 4,480 bytes: 4,306 executable bytes, 2 alignment bytes and 172 selector bytes.
Review all four target
tables (24/7/7/5 entries at 416F64/416FC4/416FE0/416FFC), candidate table ownership,
relocations/destinations, updater edge and pending/END cleanup. Table restructuring
is a difference to report, not a requirement to force target-shaped tables.

Required real helpers may naturally inline but need actual source/object/PDB
provenance; merely being absent from the unresolved list is insufficient. Only
423370 transition, 420C30 scaled audio and 43C8B0 effect are qualified *direct Run
source-empty* obligations. Transitive graphics/OS/CRT/global and unrelated-owner
anchors remain separate and must be inventoried. The linked image is nonrunnable;
logical declarations and accepted raw-address leaves do not resolve private ABI
or physical aliasing. Missing/inlined standalone Run or uncertain ownership is
inconclusive. Compile/link/input/required-real-anchor failure remains failure.
No equal-size or successful-build observation grants exactness.

## Host CI and full local commands

CI adds only the two unsanitized positive baselines, serially. Drivers retain
flock plus nice for host compiler resource use, and place generated inputs,
logs and binaries under ignored build/. Full local verification commands are:

```
scripts/repo-python scripts/test-gui-message-run.py --root . --no-mutations --out build/host-tests/gui-message-run-ci
scripts/repo-python scripts/test-gui-message-lifecycle.py --root . --no-mutations --out build/host-tests/gui-message-lifecycle-ci
scripts/repo-python scripts/test-gui-message-run.py --root . --out build/host-tests/gui-message-run-mutants
scripts/repo-python scripts/test-gui-message-run.py --root . --sanitize --no-mutations --out build/host-tests/gui-message-run-sanitized
scripts/repo-python scripts/test-gui-message-lifecycle.py --root . --sanitize --out build/host-tests/gui-message-lifecycle-sanitized
scripts/repo-python scripts/test-gui-message-constructor.py --root . --no-mutations
scripts/repo-python scripts/test-gui-message-dependency-linkage.py --root .
```

The lifecycle driver's summary now reports `0 strict controls executed
(--no-mutations)` for the CI baseline and `1 strict control accepted` for a
successful full run. This is a reporting-only change, separate from sanitizer
portability: no test, extraction, assertion or failure classifier changed.
A fresh local baseline exits 0, records zero negative controls and accepts the
positive lifecycle. The existing non-PIE flags remain intact.

The new Run/lifecycle drivers use actual -O1 host commands; their sanitizer
variants add fatal ASan+UBSan and host-only -fno-pie/-no-pie. LeakSanitizer is
explicitly disabled. The existing constructor driver has its own disclosed
-fno-strict-aliasing scope. Neither host optimization mode, live-object fixtures
nor sanitizer success proves general ISO alias legality, native layout, unchanged
raw-integer timer default construction, original-language UB, runtime closure,
leak freedom or byte exactness. Keep all dependency/lifetime seams in the existing
test docs. Original failed PIE evidence is preserved separately; see
GUI_MESSAGE_RUN_SANITIZER_PORTABILITY.md.

## Conservative canonical replay selection

config/gui-message-run-producer-impact.json selects 89 units / 40 producers
(37 linked PE, 3 COFF). It binds the four changed source hashes and full canonical
manifest; build settings remain solely in that manifest. The computation follows
transitive quoted-include closure over the union of baseline and candidate graphs
without pruning conditional includes, and tests every primary/support/normal
support source. It expands to every canonical unit sharing an affected producer.
No Gui.hpp consumer is excluded; no quoted include is unresolved or producer
setting conflicts found. Unlike the previous constructor 90/41 set, Enemy.hpp is
unchanged, so the independent EnemyMotion COFF producer is no longer affected.

Serially replay each group's canonical unit names with existing settings and
retain failure receipts. No replay or full CI success is asserted by this
integration preparation; parent records those outcomes after execution.

## Integration regression result (2026-10-17)

All 89 conservatively affected existing exact units passed across 40 cold
producer groups, preserving 16,039 bytes under unchanged canonical settings.
The canonical manifest remains
2a0a98cb1963bf6d8daea859a8d4f06b1e87badfa64ccb166ed59ef3bae1beed;
no relocation-label migration or comparator change was needed for this packet.
The final group returned a transport timeout, but its complete durable exact
report was recovered before aggregation. No duplicate build was run.
This preserves prior credit; Run and its updater remain non-exact as documented.

Full target-independent CI passed with scripts/repo-python scripts/ci.py at
2026-10-17 06:42 UTC, after the declaration-only GNU i386 harness adaptation
documented in GUI_MESSAGE_RUN_RECORD_DESIGN.md. Production inputs, all43 native
input pins and the canonical match configuration stayed unchanged throughout.
