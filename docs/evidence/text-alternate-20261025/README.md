# Alternate text rasterizer and real color-fill dependency

Only **FillColor at 0x436CA0, 72 authored bytes**, is admitted. The first complete
alternate rasterizer model is **575/509**, not exact; the two alpha-inversion
models are **165/157** and **173/157**, also not exact. No alternate/inversion
source mapping or credit is added. Authored exact becomes **75782/273658,416
owners**; the denominator is unchanged. The goal remains98%.

## Target and first complete models

Fresh target/toolchain/tracking and Factory-native `th10-ghidra` check passed.
All target reads bind SHA256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The existing reviewed dense boundaries and authored origins are unchanged:
436CA0..436CE7/72,437160..4371FC/157,437FE0..4381DC/509. The alternate
six-argument RET24 entry is independently called by DrawTextInner4479D0;
existing anm-text-inner-gs binds that call. Fill uses private ESI owner/ECX color;
Invert uses ordinary thiscall plus processed-row stack argument.

The complete alternate model recovers signed15-font selection/min17 clamp,
RGB444 fill, two inversions around one foreground draw, two old-font restores,
bleed and surface upload/release. The Ghidra upload decompilation has a broken
stack model; raw instructions establish the actual RECT and output homes.
Target has32 address/control fields and31 fixed local branches; first/word
candidates have33 fields and34 locals, fully retained in
[owned-inventories.json](owned-inventories.json). All helpers are dense code,
with zero address fields; Fill has2 and inversion10 fixed local branches.

The first ordinary member model includes real Fill and Invert definitions, not
anchors for those dependencies. Its two-TU TextRenderer+RandomMath /GL noGS
link uses the real alternate entry; Fill inlines into the parent despite the
separate exact72-byte contribution. Parent frame18 differs from target14.
The sole subsequent semantic-width hypothesis follows target whole-word NOT
and RGB remerge: a union WORD/bitfield pixel snapshots RGB, complements the
word and restores RGB. It predicts elimination of alpha extraction/insertion;
actual Invert grows to173 with repeated word loads. Parent remains575.
No forced registers, noinline, flag panels, fake ABI or patched bytes are used.

The target GetSurfaceLevel output reuses the incoming color argument slot.
If the API fails without writing it, raw target retains those incoming bits;
the candidate's separate uninitialized local does not establish that behavior.
Failure-path/source-lifetime equivalence is unresolved, not silently accepted.
No type-punned parameter-home workaround was attempted.

The historical shadow554 TEXT-002 (db84352) already parked13 byte-neutral
parameter-home/scope/RECT/debug/COM/context controls. Its frame18 versus14
mismatch remains historical. This experiment does not reopen that panel.
Both complete alternate models are retained as patches against source at
6063f12619f398e7b38a950385e11eaadae66497: first-model.patch and word-model.patch,
with shared model-header.patch. They are proposals, not maintained game source.
Reopen parent/inversion only with new independently grounded evidence.

## Narrow admitted dependency

Fill packs the top four bits of each RGB byte into a zero-alpha16-bit pixel,
then writes successive WORDs while a signed byte counter is below imageSize,
rereading imageSize after each store. Odd positive sizes write the final WORD;
nonpositive signed sizes write none. The ordinary C++ member naturally lowers
to target ESI/ECX; no special calling-convention annotation is imposed.

A separate **helpers-only** cold graph contains only original TextRenderer
plus this Fill body/declaration, with unchanged RandomMath and the original
CreateTextBuffer root. It omits both rejected alternate and inversion bodies.
The complete unique PE/map/PDB contribution is raw72-equal, including both
local destinations. A further canonical cold build independently reproduces
72/all0 in that original graph. Source/TU/profile origin remains unknown;
these anchor images are not runnable products.

Independent read-only review confirms source semantics, physical extents,
private ABI, raw72 equality, both locals and the distinct rejected parent.
Canonical guards pass Create781/all40,Release107/all3,Allocate421/all7.
A separate canonical normal-COFF cold bleed946/all0 passes. These2255 guard
bytes retain existing credit only. See canonical-summary.json.

The first canonical build attempt failed before compilation because the new
unit incorrectly used a distinct output for an identical existing build group.
The failure log/exit1 are retained; the corrected shared-output retry exits0.
Initial tracking checks separately caught incomplete mapping/implemented rows;
those were corrected, and unrelated CSV quoting churn was removed. No failed
run is relabeled successful.

## Reproduction and retained artifacts

```
scripts/repo-python scripts/build-match-unit.py --unit text-fill-color
scripts/repo-python scripts/compare-linked-function.py --unit text-fill-color
scripts/repo-python scripts/build-match-unit.py --unit text-buffer-apply-alpha-bleed
scripts/repo-python scripts/compare-coff-function.py --unit text-buffer-apply-alpha-bleed
```

Native runs were serial, one CPU at nice10 with180-second caps and durable
logs/exits. Ignored evidence lives in `.analysis/text-alternate-20261025`;
first, word and helper-only images are respectively
`build/text-alternate-first-20261025`,
`build/text-alternate-word-inversion-20261025`,
`build/text-fill-helper-only-20261025`. Canonical artifacts use existing
`build/match-linked/TextBufferLifecycle`. Original artifact hashes are retained
in the analysis directory. Full CI result is recorded in validation-summary.json;
no Windows runtime, original physical-TU or phase-gate closure is claimed.

All113 full-CI stages passed at07:49:18 UTC, exit0. The1542 frozen
inputs (including nine protected files) remained unchanged. RPC and interim
read/status timeouts were recovered from durable terminal logs without rerun.
Only result documentation changed after that frozen pass.
