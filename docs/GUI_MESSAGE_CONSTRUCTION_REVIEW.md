# Revised member-construction validation

Historical test record: the construction/lifecycle host suites cited here were
retired on 2026-10-28. Commands for deleted scripts apply to commit `47d9993`,
not the current tree. Their results are historical evidence, not requirements
for exact reconstruction; follow [the current workflow](RE_WORKFLOW.md).

## Scope

Clean baseline24039fa95a52fa2668294e81cbad67cb10ad131d. All79 retained source
baseline files agree with the read-only checkpoint SHA256 inventory. Original
local source/host work did not edit remotely or build natively. The subsequent
parent-built isolated native diagnostic is reviewed below; no target execution,
profile sweep or new exactness credit is claimed.
Historical free-interface packet is unchanged and superseded.

## Executed local tests

- `python3 scripts/test-gui-message-constructor.py --root .`: baseline, all35
  retained behavioral controls, one new ordinary-new ownership negative control,
  one explicitly STATIC body-order control and strict-classifier selftests pass.
  All16 synchronous callback-throw cases pass (eight points, direct construction
  and actual typed caller). Bounded direct-field snippets pass; raw32-bit ECL
  scalar accesses are mapped explicitly to full host EnemyRuntimeView fields.
  This snippet coverage is separate from complete-owner extraction. Marker
  color/value store order is a separately labeled STATIC check.
- GCC14.2 ASan+UBSan baseline passes with ASAN_OPTIONS=detect_leaks=0 because LSan
  is unsupported under ptrace. This does not cover excluded production timer
  construction validity.
- `python3 scripts/test-gui-lifecycle.py --root .`: 135 scenarios,43 natural GUI
  member destructors per scenario,22 strict body and2 strict header controls pass.
  Exactly one visible message parameter-constructor seam supplies setup only.
- `python3 scripts/test-enemy-runtime-tail.py`: one unittest passes using the
  unchanged extraction helper in an evidence-only assembled root, revised
  Enemy.cpp and updated canonical GUI field fixture.

The results above are author-run local observations, reviewed independently.
Raw logs, retained inputs, binaries and SHA256 manifests are handoff evidence
outside the repository patch; none of those generated artifacts ship here.
The unchanged timer flags failure remains a separately documented diagnostic,
not a passing constructor test.

## Reproduce on the host

From the repository root, with GCC14.2 and Python3, use a fresh local directory:

```sh
python3 scripts/test-gui-message-constructor.py --root . --keep-dir /tmp/gui-message-member-check
python3 scripts/test-gui-lifecycle.py --root .
python3 scripts/test-enemy-runtime-tail.py
```

The first command runs baseline and all mutation/classifier controls, then leaves
fixture.cpp, gui-message-extracted.inc, gui-field-snippets.inc and normalized host headers
representing the baseline in the chosen directory. Lifecycle's default profile
is GCC -O1 with UBSan; runtime-tail retains its existing -O0 unittest profile.
Sanitize the retained baseline separately using the command below. Non-PIE is a
host-only sanitizer arrangement, never a target production/compiler profile.
Leak detection is disabled because LSan is unsupported under the ptraced host;
no leak-freedom or unseamed timer-construction claim follows from success.

```sh
g++ -std=c++17 -O1 -fno-strict-aliasing -fno-pie -no-pie \
  -fsanitize=address,undefined -fno-sanitize-recover=all \
  -I /tmp/gui-message-member-check /tmp/gui-message-member-check/fixture.cpp \
  -o /tmp/gui-message-member-check/sanitized-nonpie
ASAN_OPTIONS=detect_leaks=0 /tmp/gui-message-member-check/sanitized-nonpie
```

This explicit non-PIE rerun also passed with ELF type EXEC; it follows the same
baseline input generation and does not alter any source or test definition.

## Ownership and controls

Synchronous failures propagate the intended exception, free exactly one ordinary
message allocation, never invoke a whole-message destructor, retain prior VM,
bullet, laser and enemy effects, and do not publish GUI/message-index state.
The negative manual-allocation/placement-new mutant safely leaks its block until
the intended CHECK exception.free_once exits1. It performs no destructor on an
uncompleted message, double-free, use-after-free or null/crash test.

All behavioral mutants require a successful compile, exactly exit1, empty stdout
and the sole assigned CHECK stderr line; wrong assertions, extra output, signals,
compiler failures and accidental passes are failures. Static controls are not
runtime mutation kills. Explicit timer, dependency lifecycle, global allocator
and direct-field snippet seams are inventoried in GUI_MESSAGE_CONSTRUCTION.md.

## Independent review

Independent review_message_constructor_ownership found no source/fixture blocker
and recommends parent integration followed by the bounded native diagnostic,
with timer/ABI/EH gates retained. Review covered production constructor/caller,
all seven alias sites, actual fixture lifetimes/seams and strict controls. The
reviewer independently recomputed all15 member-test input hashes and the run-log
hash, matching the manifest. No independent reviewer build was performed.
The full review is retained with local handoff evidence; this disposition is
source/host approval, not native exactness.

## Producer impact

A conservative transitive quoted-include inventory of all source-backed canonical
units finds 41 producer groups covering 90 units. It includes primary, support and
normal-support translation units; archival runtime units without source are
excluded. The original impact-inventory manifest hash is
6379c4d2b0ef4bc4d636d0ecefebc8cc37ff1375bfe2b69984a8712b9ebc413f.
The compact selections below name one canonical unit per affected producer,
with the total affected units sharing that producer in parentheses. Expand the
selected producer to all its canonical units for replay; these are impact
selections, not replay claims. Object producers are grouped by source/profile;
linked producers by configured output directory. New GuiMessage.cpp remains a
separate isolated diagnostic. No canonical profile is changed.

- src/EclVm.cpp: ecl-vm-host-run (2), ecl-vm-polar-from-angle-magnitude (1).
- src/Enemy.cpp: enemy-manager-constructor (12), enemy-ecl-resource-load-package (1), enemy-ecl-reset-state (1), enemy-ecl-host-dispatch-default (9), enemy-runtime-constructor (1), enemy-manager-update (2), enemy-runtime-vector (1), enemy-angle-from-player (1), enemy-decomphelp-leaf-cc10 (3).
- src/AnmVmId.cpp: anm-vm-id-set-interrupt (1), anm-vm-id-set-flag2 (3), anm-vm-id-set-sprite-own (2).
- src/EnemyMotion.cpp: enemy-set-motion-angle (1).
- src/EnemyEclDispatcher.cpp: enemy-wrap-angle (7), enemy-mark-pending-interrupt (6), enemy-bullet-playfield-bounds (1).
- src/Player.cpp: gui-set-lives-display-count (1), player-reset-runtime-state (1).
- src/Gui.cpp: gui-score-add (1).
- src/FrontEnd.cpp: frontend-cursor-push (4), frontend-cursor-set-current (1), frontend-draw-callback (1), frontend-update-callback (1), ascii-manager-ensure-selection-vm (1), frontend-set-screen-state (2), frontend-delete-vm (1), frontend-options-refresh (1), frontend-keyconfig-display (2).
- src/Main.cpp: main-set-next-game-mode (1).
- src/AnmManager.cpp: anm-set-vm-script-index-and-execute (1), anm-texture-alpha-bleed (1).
- src/BulletManager.cpp: bullet-runtime-deactivate (3), bullet-update-horizontal-wrap (2), bullet-update-boundary-bounce (1), bullet-update-state8 (3).
- src/BulletRuntime.cpp: bullet-runtime-update (2).
- src/GameManager.cpp: decomphelp-game-manager-on-draw (1), game-manager-update-callback (1).
- src/Hint.cpp: hint-create-owner (2).

## Completed native diagnostic and remaining regression gate

The isolated /EHsc diagnostic is complete and independently reviewed. All three
real contributions are retained, including the static caller without noinline;
all are non-exact. See GUI_MESSAGE_NATIVE_BASELINE.md for702/683 constructor,
404/182 helper,124/124 caller and storage-only EH topology findings. Anchored
runtime dependencies and nonzero __except_list FS offset forbid execution and
runtime-EH claims. Source-ledger mappings are decompiled/0.00 only.

The tracked fixed diagnostic script preserves graph and input gates; prior
artifacts are protected and no automatic rerun is requested. Parent-run serial
canonical regressions and CI remain pending integration gates. The compact
config/gui-message-producer-impact.json binds all90 units/41 producers to the
canonical config hash; no profile or accepted match is changed.

Host -fno-strict-aliasing is intentional. Real lifetimes, safe failure fixtures
and sanitizer success do not prove portable ISO alias validity of existing
cross-view pointer casts. Do not use copies, volatile or opaque wrappers to mask
that separate limitation.

## Integration bookkeeping and label-binding provenance

The constructor source mappings are decompiled/0.00 only. After applying them,
regenerate tracked progress with `python3 scripts/progress.py`, then run
`python3 scripts/ci.py`; progress regeneration is not new exactness credit.

The initial replay selection was bound to canonical config SHA256
6379c4d2b0ef4bc4d636d0ecefebc8cc37ff1375bfe2b69984a8712b9ebc413f.
Group6 exposed legitimate compiler-local label-name drift in two complete80-byte
resolver owners. A separately reviewed ten-symbol-only migration preserves all
relocation offsets/types, zero addends, target identities, source/profiles and
extents. The compact impact JSON now pins the resulting manifest hash and retains
the original hash plus migration-patch identity. Its90-unit/41-producer membership
is unchanged. Preserve the original failed group6 report and rerun the strict
canonical comparator after binding migration; the byte/identity audit alone does
not claim that rerun passed.

The promoted native diagnostic script remains byte-identical to the executed
historical script. It checks selected canonical unit settings and production input
hashes rather than the whole manifest-file hash. These ten resolver label names
are outside that selected unit, so no script gate is weakened or bypassed. The
original isolated native run and artifact hashes remain historical provenance;
the existing nonempty-output guard still forbids silently overwriting that run.
