# Startup state-field reconstruction — 2026-10-29

The registered Startup update callback41FEB0..41FEEE is now complete/exact:
**63 bytes/all3 fields**, reproduced by two independent canonical nineteen-input
cold links. Its origin remains indeterminate; authored totals are unchanged.

The old source stored state4 through an independent `g_GameTransitionState`
external int and emitted67 bytes. Target491FB8 is supervisor base491C28 plus390,
already represented by `MainSupervisorView::nextGameMode`. Using that actual
field for both Startup state stores recovers the target's direct final flag clear
and the full63-byte callback. This changes the source storage model without a
register, ABI, padding or compiler-profile control.

The strict unit binds the ASCII-manager slot4776E0, supervisor flags+3CC and
supervisor next-mode+390. Both supervisor fields use the actual base symbol with
explicit addends, rather than independent diagnostic anchors. The sole conditional
destination and every ordinary instruction remain fixed in the full comparison.
The ECX fastcall receiver and plain RET agree. The existing complete boundary and
registered-entry reference review is retained.

Both cold images also pass seven explicit prior owners556/all33: statistics
cleanup121 and Startup constructor/launcher/factory/release/draw-adapter435.
Twenty-three older same-image contracts6235/all179 pass separately. No objects
are reused in either cold producer; no full historical cohort is replayed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit startup-update-callback \
  --unit statistics-owner-destructor --unit statistics-release \
  --unit startup-worker-constructor --unit startup-register-tasks \
  --unit startup-worker-create --unit startup-release-resources \
  --unit startup-draw-callback --json
```

Private source snapshots, pins, full receipts/listings and exit logs are under
`.analysis/gpt-6.1-sol/startup-state-field-20261029/`. First witness:
`build/gpt-6.1-sol/startup-state-field-20261029/cold-a/`; second witness:
`build/match-linked/StartupWorkerLifecycle/`. Public identities and source pins
are in [results.json](results.json).

The whole startup destructor remains433/417, Draw218/220 and thread292/299.
The unreferenced EDX update copy41FD90 is not claimed exact. This source repair
does not settle original class/TU, the origin of the two update copies, the
physical ownership of all other legacy state aliases, product or runtime closure.
Lightweight CI and whitespace checks pass. The comparator is unchanged.
