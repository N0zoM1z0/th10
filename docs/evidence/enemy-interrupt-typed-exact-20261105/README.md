# TH10 48-byte exact EnemyConfigureInterrupt: typed callback fields

Target SHA-256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The target `0x00412720..0x0041274F` is a complete **48-byte** helper,
currently canonically exact in `enemy-configure-interrupt`.

The genuine `EnemyCallbackThresholdView` is a 16-byte record within
`EnemyFullObjectView::runtime.callbackThresholds[8]` at full-owner
`+0x2494`. The helper writes record-relative life threshold `+0`, timer
threshold `+4`, life callback name `+8` and timer callback name `+0xC`.
The maintained source now expresses the **timer threshold** and **life
callback** stores through those existing typed members. The life-threshold
and last callback writes still preserve their old raw-address form, as
required for the native exact address/register lifetime at this epoch.

A local five-variant *real* pinned MSVC7.1 normal COFF comparison was used,
not a source-only reasoning claim. Changing all four stores to a typed
indexed record emits only **32 bytes**; typing the first three but keeping
the final old pointer expression emits **43 bytes**. The selected mixed
pointer-lifetime form emits the **entire original 48/48 byte body**, with
**zero relocations and zero differing original-target bytes**. A separate
canonical selected cold replay of `enemy-configure-interrupt` confirmed the
same 48/48 target-exact result, along with two neighboring exact argument
helpers (16 and 26 bytes). No new exact credit is added because the helper
was already credited before this change.

The same change was rebuilt in the **real eleven-source VC7.1 /GL** ECL
graph, comparing complete linked-PDB functions: `EnemyBeginSpell` remains
**2,424** candidate bytes (target 2,432) with the same 108 linked fields,
and `EnemyRuntimeView::DispatchEclInstruction` remains **14,352** candidate
bytes (target 14,416) with the same 716 fields. In both, all ordinary
candidate bytes and field positions/types are preserved. These large
functions remain **NONEXACT**. Only scoped affected replay and seven-stage
lightweight CI were run; no full historical cold replay or product claim.

Reproduced full graph: `build/gpt-web/20261105-followup-ecl/typed-interrupt-exact-leaf/`;
local five-variant COFF evidence is in
`.analysis/gpt-web/20261105-followup/interrupt-typed-results.json`.
