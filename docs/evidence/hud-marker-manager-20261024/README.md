# HUD marker routing and manager capture, October 24–25, 2026

## Three separately measured models

After checkpoint **9fb5a3d**, three natural source models pass native and
independent review. Every model preserves **13 exact guards, 1468 bytes / all
79 fields**, all seven HUD table words and the complete **4480-byte Run owner**.
Final HUD remains **3824 versus 3836 bytes, nonexact**: 3796 code bytes, no
alignment and 28 table bytes. Target retains 3805 code, three alignment bytes
and 28 table bytes. Inventory remains 72 fields / 249 local branches / 21 calls;
it is not a canonical binding contract. **Zero new exact/source/denominator
credit** follows. Full CI passed all 112 stages for this batch, exit 0, with
frozen inputs preserved, verified at 2026-10-25 00:36:31 UTC. Only CI-result
documentation changed afterward.

1. **Default-first marker comparison.** Reordering the arms recovers the target
   status routing: less/unordered reaches multiplication, equal/greater reaches
   default alpha 255. Exactly 34 localized HUD byte positions change from the
   previous image. This is not byte-neutral merely because the extent is fixed.
   The machine-routing proof does not establish runtime NaN-to-integer semantics.
2. **Float distance local.** Only the local declaration changes from double to
   float; the explicit widening before subtraction, fabs, double 64.0 comparison
   and float coefficient remain. Native code uses the target four-byte -2.984375
   coefficient with **no F32 store/reload roundtrip**. This is not the earlier
   float-before-subtraction model and proves no original source type. Sixteen
   raw HUD positions change, including literal-address relocation and incidental
   target-supported timer operand order; the timer source was not changed.
3. **Hidden manager capture.** Moving the manager load inside the existing
   nonzero ID gate swaps one six-byte load and two-byte branch: **exactly eight
   HUD byte positions** change. Zero ID bypasses capture/list/clear; every
   terminating nonzero path captures once before both list-head reads. Three
   invalid-graph controls reject. Every other HUD byte, cleanup/reset sequence
   and table word stays identical to the float candidate.

The changed-position counts are baseline-to-candidate accounting, **not target
mismatch counts**. The 12-byte extent gap is not a similarity score.

## Original failure and same-source retry

Preserve the original `hud-default-first-trial-20261024` exit **1**, receipts,
logs and incomplete outputs. Winepath returned page-fault text before its path;
the contaminated linker argument caused LNK1104. No usable image or model audit
result existed, so this was a native/tool failure, not a rejected source model.
A retained clean-path health check preceded the successful unique
`hud-default-first-retry-20261024` run, with identical source and profiles.
Neither the failed attempt nor its evidence was overwritten or relabeled.

## Identity, graph and binding limits

Final Gui.cpp is
`70184d8834dc24998debe484a67a6643b8e55a6327a6115835353e008a68309b`;
final image is
`b2fd9d94f5af6b52652494268c50c7a229792a245dd6447b79d2d3432ef54410`.
The actual fourteen-TU Gui graph remains `/GL` **without /GS**, with no support
overrides. [Summary JSON](summary.json) records complete order/profiles, each
source/image identity and hashed receipts. Independent reviews preserve 5635,
5768 and 5903 frozen inputs respectively, including original artifacts.

The original 72-field draft remains pinned to its earlier excess-clear carrier.
A **fresh final-carrier review** now declares all 72 target fields against the
source/image above: 21 calls, 20 constants, 13 existing global-contract references,
10 occurrences of six explicit role-supported aliases, and eight fixed local/table
fields. The deallocator and coefficient-width/payload blockers are resolved.
`hud-final-binding-review-20261025/target-binding-draft.json` remains
**active=false, accepted=false**: extent, alignment and local-layout rejection
persist. Final per-site field/ABI assignments and explicit alias acceptance are
still required; named roles do not prove shared runtime storage or parent exactness.

## Host precision test boundary

Focused host tests exposed a GCC float-local spill absent from the independently
observed native sequence. A real host x87 probe still stores/reloads float at
PC64, round-to-nearest; changing host arithmetic flags did not reproduce the
native no-roundtrip contract. The original failing probes are retained.

The scoped lifetime-test split leaves portable actual-source expressions
unchanged. Only the precision-specific finite witness moves to a separately
labeled source-derived no-roundtrip model, representing PC53/PC64 evaluation
under round-to-nearest. Expected **254** and the rounded-before-subtraction
negative control remain, with the same exact failure classifier and UBSan.
The model explicitly uses a double host local; it is not actual-source host
codegen. Root's applied suite passes **7 tests**; independent review approves the split
with no blocking concern. Classifier negatives and source/header identity checks
are also preserved.
Production Gui.cpp is unchanged. Game FPU state, finite-witness reachability,
nonfinite conversion and runtime closure remain unproved.

Ignored receipts under `.analysis/gpt-dots/` are retained in:

- `hud-default-first-trial-20261024/` and `hud-default-first-retry-20261024/`
- `hud-float-distance-trial-20261024/`, including `timer-order-supplement.json`
  under its independent review
- `hud-hidden-manager-trial-20261024/`
- `hud-full-binding-review-20261024/` and `hud-final-binding-review-20261025/`
- `hud-host-precision-audit-20261025/`, retaining original host probes and the
  applied split's `root-focused.log`

Each successful packet has `candidate/receipt.json` and
`independent-review/receipt.json`. These artifacts are not supplied by a Git
clone; no disassembly or target byte payload is copied here. The prior
[clear/delete checkpoint](../hud-clear-delete-identity-20261024/README.md)
retains its 112-stage CI pass at 2026-10-24 23:33:14 UTC and its original failed
observer audit. These later source changes have their own 112-stage pass recorded above.

## Stop condition and next evidence

No additional semantic factor was identified. Visible residuals concern
register/save scheduling and separately tested versus folded boss-flag gates;
these alone do not justify spelling, register or flag matrices. Park HUD and
keep WinMain parked unless new independent evidence supports a falsifiable
model. The ANM/ECL native-input audit confirms only two changes in their shared
ten-TU / 32-file native closure: Enemy.cpp's two deallocator corrections and
GameErrorContext.hpp's Flush declaration. Main.cpp, Gui.cpp and Chain.cpp are
not compiled there. The old 4/25 residuals are historical until a fresh shared
baseline is measured. That baseline runner is staged under
`anm-ecl-source-current-20261025/`, **not frozen or compiled**. No new source
model or assumption that the old residual counts persist follows from the audit.
