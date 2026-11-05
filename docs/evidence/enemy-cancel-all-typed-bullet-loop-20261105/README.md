# TH10 full bullet-cancel manager loop: typed scan of 2,000 runtime records

Original target input is the hash-attested TH10 Japanese v1.00a,
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This batch changes the **complete** source for
`EnemyCancelManagerView::CancelAllBullets`, not a stub/fragment. Its
current eleven-source PDB-linked compiler candidate is **418 bytes**;
the independent original physical target boundary is **not yet
established**, so the function is **NONEXACT/uncredited**.

The manager scans 2,000 bullet records using the target-established
`0x7F0` record stride. Rather than cast each field through raw byte
offsets, the loop views each record as the established
`BulletRuntimeView`. Typed accesses now cover state at +0x446,
world position at +0x3B4, collision width/height at +0x3F0/+0x3F4,
flags at +0, sprite color at +0x7EC and the cleared unknown word at
+0x450. Its exact iteration order, bounds comparisons, optional visual
spawn, color-table selection and existing unrecovered sprite data
accesses remain unchanged. The raw `bulletStorage`/byte-stride cursor
is retained specifically to preserve the native pointer lifetime.

A full real pinned VC7.1 eleven-source /GL graph rebuild with a freshly
compiled modified dispatcher and ten source/header-hash-checked support
objects verifies the complete **418-byte** cancel owner. All **12**
linked-field offsets/types remain identical to the previous 418-byte
candidate and **zero** ordinary candidate bytes differ. The same
graph retains the complete 2,424-byte `EnemyBeginSpell` candidate
(original 2,432) and complete 14,352-byte ECL interpreter (original
14,416), with 108 and 716 identical linkage fields and zero ordinary
candidate changes. Those large owners remain NONEXACT.

The selected independent original-target native replays also pass
`enemy-cancel-bullet-record` **199/199** and
`enemy-cancel-bullet-pattern` **260/260**, totaling **459/459** exact
bytes. Both had previous exact credit; no newly exact owners are claimed.
The seven-stage lightweight CI and diff checks passed. No full cold
cohort, product closure, compiler-source provenance or runtime claim.
See `native-result.json` and ignored full native link artifact under
`build/gpt-web/20261105-bullet-loop/typed-cancel-all-418/`.
