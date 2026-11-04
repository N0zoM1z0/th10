# TH10: recover native ECL timeout callback slot in the 14KB dispatcher

Target: Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The original EnemyRuntimeView::DispatchEclInstruction at 0x0040E770
owns 14,416 target bytes. It remains NONEXACT.

The actual opcode ENEMY_ECL_SET_TIMEOUT formerly stored to an integer address
computed as (slot+0x24A)*0x10 + the full Enemy pointer. That is exactly the
fourth word of the independently established 16-byte
EnemyCallbackThresholdView record: full Enemy +0x2494 + slot*16 +0xC.
EnemyRuntimeView already contains callbackThresholds[8] at runtime +0x1458,
and EnemyFullObjectView embeds the runtime at +0x103C. Both offsets and the
original slot layout are checked by the existing native-layout assertions.

The maintained real boss function now writes
owner->runtime.callbackThresholds[slot].timerCallbackName, passing through
the identical original instruction-payload pointer from current ECL +0x18.
It does not invent another array, change slot-selection semantics or replace
the existing callback helper.

## Native 11-input compiler result and negative controls

The actual pinned MSVC 7.1 SP1 /GL 11-input Enemy update context rebuilt the
modified dispatcher against SHA-verified unchanged support object inputs.
The complete PDB contribution is 14,348 bytes with 716 linked fields, exactly
the previous candidate field offsets/types, and zero ordinary candidate-byte
differences after identical candidate field normalization. The current source
SHA-256 and native PE/MAP/PDB are recorded in native-result.json.

The original target is 14,416 bytes, so no full-target exactness, partial
credit, invented source ancestry or product/runtime proof is claimed.

Two separate source experiments replaced six special-enemy slot comparisons
with direct typed pointer checks or typed 32-bit word reads. Both changed the
full-owner length to 14,352 and shifted linked field geometry; both were
discarded, leaving only the byte-preserving timeout-slot rewrite.

The 48-byte target-exact EnemyConfigureInterrupt helper already names the
same real 16-byte record through its existing native addressing shape. Three
separate proposals to change its body to typed reference stores were compiled:
the first two emitted 43-byte functions, and the fully typed version 32
bytes. They are rejected because the actual target helper is 48/48 exact;
the helper source is unchanged. The unsuccessful experiments are retained in
the private .analysis/gpt-web/20261104-current/interrupt-typed-panel-results.json,
not falsely treated as accepted source.

The only new source credit here is the typed timeout write inside the
complete still-NONEXACT boss. No all-unit cold replay.
