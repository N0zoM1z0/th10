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

## Retired host validation

The host construction/lifecycle suites, exception injection and strict mutation
requirements have been removed. Their historical reports are recoverable at
`47d9993`. Current acceptance uses complete native byte comparisons.

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

## Historical native diagnostic

The isolated /EHsc diagnostic is complete and independently reviewed. All three
real contributions are retained, including the static caller without noinline;
all are non-exact. See GUI_MESSAGE_NATIVE_BASELINE.md for702/683 constructor,
404/182 helper,124/124 caller and storage-only EH topology findings. Anchored
runtime dependencies and nonzero __except_list FS offset forbid execution and
runtime-EH claims. Source-ledger mappings are decompiled/0.00 only.

The tracked fixed diagnostic script preserves graph and input gates; prior
artifacts are retained as evidence. The compact
config/gui-message-producer-impact.json records the historical90-unit/41-producer
selection. Current replay scope follows changed owners and affected interfaces;
no profile or accepted match is changed.

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
