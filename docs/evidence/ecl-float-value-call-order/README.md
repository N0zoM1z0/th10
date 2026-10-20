# ECL float-value call order, 2026-10-21

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting checkpoint: `42369b0`.

The one-byte annotation described below is specific to these recorded images.
The [updated diagnostic](../ecl-current-dependencies/README.md) also supports
an earlier decode stop in the PE-covered jump table, while comparing all7020
bytes and every unlinked selector. The original proof and its credit are unchanged.

The complete ECL Run remains **nonexact**. This checkpoint removes two ordinary
byte differences while preserving the existing exact callee and dependent
units. It adds no exact functions, bytes or denominator credit. The other two
large owners, native product, runtime, semantic and portability gates remain open.

## Retained source and target evidence

The provisional implicit receiver of `EclVmContext::ReadFloatValue` becomes an
explicit context parameter in ordinary static stdcall source:
`ReadFloatValue(flagIndex, context, value)`. Its definition, StartSubroutine,
Run FORMAT arm and Enemy raw-float adapter agree on this declaration. These are
all three real callers found in the maintained source.

VC7.1 LTCG preserves the target private ABI: EAX context, ECX flag index, one
stack float and RET4. In Run, it now prepares EAX before ECX, recovering the
target `MOV EAX,ESI; MOV ECX,EBX` sequence at relative offsets 1990/1992.
The two differing ModRM bytes at 1991/1993 disappear. No fixed registers,
dummy arguments, padding, assembly or target patching are used.

This is compiler evidence for a compatible source model. It does not establish
the original static declaration, class identity, source parameter spelling or
production translation unit. The target ABI evidence remains independent of
that source hypothesis.

## Complete measurements

The [native receipt](native-summary.json) records recursive input hashes,
artifact hashes, complete owner extents, comparison results and residual offsets.

| Context | Before | After |
| --- | --- | --- |
| EclVm + Enemy, /GL, real HostRun entry | 6992/7020; 28 ordinary differences | 6994/7020; 26 ordinary differences |
| Dispatcher plus seven supports, /GL /GS | 6991/7020; 29 ordinary differences | 6993/7020; 27 ordinary differences |

The fresh two-TU baseline recompiles the primary and reuses only hash-pinned,
unchanged support. Two independent complete two-TU cold builds reproduce the
retained result and identical Run/callee owner hashes. The eight-TU old inputs
are checked against every baseline source/header hash; its new build recompiles
each changed dependency closure and checks every reused support object's hash.

All diagnostics compare the complete 7020-byte PDB owner and validate all 189
fields, including 62 local destinations. Linear decoding still stops at byte
7019 and reports `normalization_complete=false`. The explicit one-byte trailing
selector data span closes that coverage in the strict comparator and is itself
compared. No suffix or differing instruction bytes are excluded. The eight-TU
graph has one additional SIB byte at relative 146E; it must not be described as
the same 26-difference result. The 6692-byte pre-table span and 328-byte table
and selector suffix remain unchanged.

The remaining two-TU differences are spawn/stop-thread registers, two metadata
SIB encodings, two stack-address SIB encodings, length/angle/FORMAT stack homes,
and the final instruction-cursor register. Their precise offsets are in the
receipt. Same total size is not exactness.

## Existing exact boundaries preserved

The changed 171-byte ReadFloatValue unit passes two further independent
canonical single-TU cold replays, with all four fields and identical function
hashes. Only its decorated symbol changes in the canonical recipe; target
extent, profile, linkage fields and existing credit remain unchanged.

The focused canonical ECL source gate passes all 16 existing units, 1617 bytes
and all 21 fields across its three graphs. Six affected Enemy helper units pass
their canonical four-TU cold graph: 478 bytes and all eight fields. In the
actual eight-TU image, 14 complete existing guards pass 1452 bytes and all 27
fields, including ReleaseAnm55/all2 fields. Its complete dispatcher14524 and
raw-float forwarder26 hashes equal the old graph's hashes; both remain nonexact.

The FORMAT host projection's float service now accepts the explicit context.
Its existing assertions pass all ten scenarios and eight functional mutants
under ASan/UBSan/leak checking, including live instruction replacement. It
remains a source projection, not original/native-product execution or a test
of the i386 operand implementation.

## Restored controls

An inline angle-from-offsets helper retains the baseline 28 differences after
complete normalization. Some raw hashes change with link layout, so it is not
raw-byte neutral. A static integer-value adapter and y/dy/counter declaration
order each retain the improved source's 26 differences and all seven helper
guards. A direct raw-float forwarding expression changes the receiver from
target EAX to ECX. All four controls are restored.

## Reproduction

Use `scripts/repo-python`, the pinned target and VC7.1 toolchain:

```sh
scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp
scripts/repo-python scripts/diagnose-ecl-run-native.py
```

The diagnostic returns 1 for the current ordinary mismatch, 0 for complete
equality, or 2 for an unsupported/unbound comparison. Its output grants no
canonical credit. It requires the complete same-image ReadFloatValue callee
to pass, binds external fields only through existing canonical target pairs,
and checks the reviewed complete local-field/data layout.

For the actual eight-TU graph, build `anm-release-slot`, then run the diagnostic
with `--unit anm-release-slot`. `--artifact-dir` can inspect an already built
artifact from that same graph. An existing artifact alone does not attest
current source inputs; use a fresh build or the receipt's recorded pins.

Private controls and detailed reports remain below
`.analysis/gpt-6.1-sol/ecl-point-angle-20261021/`; native outputs remain below
`build/`. Superseded negative native outputs are removed after recording their
results. No original executable, database or generated decompilation is committed.

All 97 standard CI checks pass, together with toolchain declarations, tracking,
generated progress and whitespace checks. Five superseded owned directories
were removed (3,376,996 bytes); source controls and comparison receipts remain.
