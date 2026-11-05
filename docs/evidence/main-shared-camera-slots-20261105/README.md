# TH10 shared 0x118-byte main camera slots in ECL and stage viewport selectors

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The main supervisor owns two adjacent camera records beginning at +0x154 and
+0x26C with stride 0x118. Both are selected by two independently owned, exact
60-byte functions: `EnemyPrepareProjection` at `0x00401D20` and
`MainConfigureGameplayViewport` at `0x00405300`. Before this change the
Enemy selector reconstructed the record by raw `0x154 + index*0x118`
arithmetic and `MainViewport.cpp` independently duplicated an internal
`MainCameraRecordView` definition and the same raw calculation.

The single shared `MainCameraRecordView` now lives in `Main.hpp` and models
`AnmViewportOwnerView prefix` at +0 followed by the target-observed camera
screen-shake values at +0xE8/+0xEC, with its original unknown remainder
left opaque. Assertions fix its whole 0x118 extent, both shake offsets and
the precise first/second record positions inside `MainSupervisorView`.
Both selectors now address `record[index].prefix` from the established
`supervisor->unknown154` slot; their existing 0/1 call domain, exact return
and no-bounds-check behavior are unchanged. This does not claim that the
original Japanese C++ source used this class name or representation.

After the *installed source* changes, three selected original-target pinned
VC7.1 `/GL /GS` canonical replays pass **60/60** for
`enemy-prepare-projection`, **60/60** for `main-select-viewport`, and
**53/53** for `main-disable-fog`: **173/173** previously exact bytes.
A second complete **11-source VC7.1 `/GL`** ECL graph recompiles the edited
Enemy dispatcher and all header-affected support sources, reusing nine objects
only with unchanged source/dependency inputs. Its whole owner comparisons
preserve code and field geometry for complete `EnemyBeginSpell` 2,432 bytes
(108 fields), ECL dispatcher 14,352 bytes (716 fields), 330-byte camera
configurator (10 fields) and 60-byte projection leaf (one field): **zero**
ordinary candidate changes. The ECL dispatcher original target is 14,416
bytes and remains **NONEXACT**. Other nearby nonexact owners receive no
new codegen credit.

No whole historical cold replay, product build or runtime test was run.
Seven-stage lightweight CI and diff checks passed. Current source SHA pins,
scoped receipts and comparison rows: `native-result.json`; ignored local
full link `build/gpt-web/20261105-camera-slots/complete-ecl-typed-camera-slots/`.
