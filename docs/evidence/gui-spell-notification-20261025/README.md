# GUI spell notification and EnemyEndSpell: bounded nonexact pilot

Baseline `e937e097704fcbc1a0471813e1c4ef63e9d5842b`; original Japanese
TH10 1.00a target. **Isolated proposals only: zero new exact/source bytes,
no ledger or denominator change, no runtime/product claim.**

## Complete results

| Model | Notification / proposed668 | EndSpell /399 | Decision |
| --- | ---: | ---: | --- |
| First complete natural cdecl model | 652 (624 code +28 table) | 452 | Structural rejection |
| Separate carried `remaining=value` local | 652 (622 code +2 alignment +28 table) | 452 | Structural rejection; glue raw-identical |
| Ordinary stdcall declaration-only hypothesis | 660 (632 code +28 table) | 447 | RET8 recovered naturally; still structural rejection |

All three links preserve **11 complete same-image guards,1078 bytes/all21
fields**. Each native operation preserves1839 frozen inputs, including all
tracked inputs, protected nine D3D files, full input overlay, and the original
GUI creation canonical artifact directory. There is no production source edit.
The new guards do not imply whole-image or runtime equality.

The actual ECL dispatcher root calls the complete new EndSpell model, which
calls the complete notification. Six TUs are compiled under the canonical
Dispatcher helper `/GL /GS` graph; GUI alone uses its existing `/GL` without
`/GS` profile. No synthetic caller, register annotation, noinline addition,
constant-event specialization prevention, or callee byte substitute is used.
All seven switch arms survive naturally despite the sole observed event0 call.

## Boundary and bindings

The independent [boundary proposal](boundary-proposal.json) assigns
4172E0..41755F code640 and417560..41757B seven absolute table words28 to the
notification. The unsigned selector guard is0..6; the sole table-base xref is
4172F2. All seven words land on instruction starts within the code; four CC
bytes follow before independent417580. The maintained640-byte row is unchanged;
668 is a reviewed future proposal, not admitted denominator or exact credit.
EndSpell remains the complete reviewed399-byte extent409C00..409D8E.

Each `*-comparison.json` separately retains every target/candidate field,
fixed local destination, and all seven table offsets. Notification has35/35
fields and7/7 local branches; EndSpell has20 target/23 candidate fields and6/6
branches. Source bases/addends are explicit, never nearest-symbol substitutions:
score474C40+4, chapter state474C40+28/+2C, stage4776E8, GUI47770C,
playback477838, statistics47783C, sound492590, manager491C10.

The original target PE lacks a usable relocation inventory for these absolute
operands. A separate bounded observer enumerates complete code operands plus
seven raw table words. Generic decoder output that walks into table data is
retained only as a labeled observation, not complete-code/normalization proof.
All corrected inventories keep `normalization_complete=false`: these are
structural rejections, not accepted normalization or byte-equivalence claims.

## Recovered behavior and remaining uncertainty

Notification0 retires/zeros9E14 and creates header47/layer15. It then processes
8 signed decimal digits with divisors10000000..1, scripts27..2E, and sprites
`digit+8`; leading zeros stay hidden, including all-zero input. The manager is
captured after header creation, retained for digit retirement/allocation,
reloaded after InitializeVm, and retained for Add/Find and the next iteration.
Front ANM is captured after retirement and before allocation. IDs publish before
lookup; SetSprite uses the found VM's ANM; flag helpers perform separate lookups.
Cases1/6 use9E14 with48/4C;2/3/4 use9E18 with49/4A/4B;5 and out-of-range do nothing.

EndSpell preserves active-bit gating, stage flag, three explicit panel interrupt
calls, active-bit clear, managed-ID retirement/zero, post-retirement success-bit
reread, signed bonus/10 and score cap, bonus reread before notification, replay
mode1 statistics bypass, two bounded99999 counters, captured statistics pointer,
and sound2D/sample0. Failure uses the manager captured before the first retirement
for the GUI ID retirement, then reloads front ANM and creates script48.

The carried-local model is supported by actual target capture/store/reuse timing;
it changes the notification but not the glue. Ordinary stdcall is only a
falsifiable logical declaration hypothesis grounded by caller/callee cleanup,
not proof of the original public convention. It restores the private EAX event
and RET8 naturally, but scheduling/extent/table destinations remain different.
No further calling-convention or spelling panel is justified.

Glue still inlines the failure creator where the target calls it. Existing
AllocateVm is153/161 and InitializeVm207/217 in these graphs. Their identities
and ABI are distinct from callee exactness; nonexact callees alone do not bar a
fully bound exact caller, but neither callee nor product closure is established.
No new semantic defect justifies forced noinline or broad profile searches.

## Reproduction, failures, and review

Each standalone `*.patch` reconstructs its complete three-file proposal from the
baseline, in an isolated source overlay. The [summary](validation-summary.json)
records profiles, actual root/support order, source/image hashes and result
receipts. Retained producers, full raw disassembly and comparison observers live
under `.analysis/gpt-dots/gui-spell-notification-20261025/`; images live under
`build/gpt-dots/gui-spell-notification-20261025/`. Do not apply these patches to
production or treat their source presence as admitted coverage.

The original first attempt failed before producing an image because StageScript
preceded Main and conflicted with SDK critical-section declarations. Its exit1
and log are retained. A distinct include-order-only retry produced the first
image. An observer named `inspect.py` initially shadowed Python's standard
module; renaming it and replaying the retained image fixed observation only,
without another native build. JSON dictionary Path keys were normalized before
all expensive builds. All three successful links ran serially, one CPU,nice10,
with bounded timeout and retained exits; no run was duplicated after timeout.

Independent review approved complete source semantics/captures, all field/local
bindings, the668 proposal and isolated diagnostic scope—not exactness, public ABI,
or source admission. Park these three finite models. Reopen only with new
independent source-lifetime/callee or actual graph evidence and a falsifiable
whole-owner prediction. Focused documentation checks apply; the prior full117
CI pass remains scoped to `e937e097`, not relabeled as a new run.
