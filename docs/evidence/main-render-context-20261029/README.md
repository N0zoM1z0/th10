# Complete frame dispatcher — 2026-10-29

`GameWindowView::Render`, 439390..439538, adds **425 authored exact bytes**.
Two independent canonical 30-input cold links reproduce the full owner and all
34 address fields. This was already a source mapping; the authored denominator
and mapping count do not change. [Results](results.json) retain source/include
pins, profiles, PE/MAP/PDB identities and both sets of semantic field bindings.

The real WinMain call at438D31 pushes window/timing storage4924F0; the target
returns with RET4. Natural explicit-stdcall member source supplies that receiver
on the stack. The first receiver-only control leaves the complete normalized
419-byte body and its field inventory unchanged. Original static/member spelling,
class identifiers and source partition therefore remain unproven.

The complete dispatcher handles backward time, catches its stored frame clock up
in 1/60-second steps, selects the gameplay viewport, runs calculation and sound,
stops Main's worker for exit/restart, performs signed-byte frameskip accounting,
submits drawing and records elapsed frame duration. All target branches belong
to the byte comparison, including Sleep and both exit statuses.

Three source details resolve the old candidate:

- The positive currentTimestamp > lastFrameTime branch sends nonadvancing or
  unordered values to Sleep. The previous early <= test has a different x87
  unordered branch; size proximity did not establish the same behavior.
- The catch-up loop has a pretest. Replacing it with do/while also changed the
  compiler's floating-point storage: the old body accumulated in registers and
  used 16 bytes of spill space. The target writes/reloads the clock each step.
- Exit and restart call the actual Main mode worker at491C28+62C. The guessed
  no-argument MainStopReplayScan bridge lacked that real receiver. Normal and
  Sleep paths share the source return. This naturally reproduces entry register
  saves and loop alignment; the complete positive-block/early-return425 and
  negated-guard410 controls remain nonexact.

No register forcing, padding, ABI substitute or compiler-profile change is used.
The final425-byte body is proved by the ordinary strict comparator, including
all internal destinations, every field and the complete PDB-owned extent. The
8-byte scalar at470C90 is independently raw-equal in both cold images.

Each cold image also strictly reproduces Present236/all20 and worker Stop101/all3.
GetTimestamp282 passes a separate cold replay of its existing single-input FPS
recipe. In the full Render graph, its282-byte contribution still differs in
frame allocation; this is not a new same-image exact claim for that callee.
WinMain remains1390/1389 and receives no partial credit. Only these selected
related contracts are replayed; no historical cohort or host scenarios are run.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit main-render --json
scripts/repo-python scripts/replay-exact-units.py --unit main-get-timestamp-fps --json
scripts/repo-python scripts/ci.py
git diff --check
```

The comparator is unchanged. Data ownership, original names/flags/TUs,
whole-product closure, runtime behavior and portability remain separate claims.
