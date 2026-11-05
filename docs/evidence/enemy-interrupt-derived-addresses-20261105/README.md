# TH10 enemy interrupt helper: derive target offsets from the native callback record

Original: Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The **48-byte complete** `EnemyConfigureInterrupt @ 0x00412720` has an
existing canonical exact unit. The helper writes one of eight timer/life
callback records in the full Enemy object; it must also retain its original
private register and address-expression lifetime, not merely field semantics.

The real full-object memory layout already independently establishes:
`offsetof(EnemyFullObjectView, runtime)==0x103C`,
`offsetof(EnemyRuntimeView, callbackThresholds)==0x1458`, and
`sizeof(EnemyCallbackThresholdView)==0x10`. This yields **full owner
+0x2494**, and the `timerCallbackName` member is at `+0x0C`, yielding
**full owner +0x24A0**. The native last store uses a record-scaled origin
`(slot + 0x24A) * 0x10`. The maintained helper now derives both the
record origin and last store's shifted record-base constant from those
checked C++ offsets; the `slot` is still multiplied as a **signed integer**,
and the original first/last pointer expressions remain separate. The
compiler-time assertions bind all three original numerical facts, rather
than repeating unexplained numeric offsets.

An actual pinned VC7.1 SP1 normal-COFF compilation of both the before/after
source produces **48-byte complete owners with zero relocations**. The
before and after bodies are byte-identical and each compares **48/48 exact**
against the original target, with no instruction or byte excluded. The
selected canonical exact replay again passes `enemy-configure-interrupt`
48/48, `enemy-read-int-argument` 16/16,
`enemy-read-raw-float-argument` 26/26 and the ECL forwarding owner 11/11,
**101/101 previously credited original bytes**. No new exact credit.

To ensure the shared boss did not change, a full *complete-owner* pinned
VC7.1 eleven-source `/GL` linker run recompiled the modified dispatcher
with ten unchanged object files reused only after verifying every source
and public header hash against the immediately preceding fresh 11-TU graph.
The entire ECL dispatcher remains **14,352-byte candidate, 716** candidate
linkage fields with identical positions/types and zero ordinary
candidate differences; it is **NONEXACT**, 64 bytes smaller than its
14,416-byte target. `EnemyBeginSpell` remains 2,432 candidate bytes, with
108 unchanged candidate fields. Finalizer and viewport complete owners
are similarly unchanged. Source pins and comparison counts are in
`native-result.json`, with ignored native artifacts in
`build/gpt-web/20261105-interrupt-layout/`.

No historical all-unit cold replay, native product closure or runtime
semantic-equivalence claim is made.
