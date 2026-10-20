# ASCII viewport and glyph paths, 2026-10-21

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting checkpoint: `277ab88`.

This checkpoint reconstructs missing effects inside the two ASCII draw owners.
It adds **no exact functions or bytes**. Both complete draw owners and the
10-byte high-priority callback remain nonexact. The original three large
owners remain in scope and are not complete.

## Target-backed source change

The former `AsciiConfigureBackgroundViewport` interface had a declaration and
calls, but no maintained definition. The target has expanded camera and D3D
viewport operations at those sites. Treating this as only a callback register
allocation problem omitted part of the caller/callee context.

Both draw functions now capture the selected record's viewport index **before**
FlushVertexBuffer. They publish the selected camera, call the maintained
StageConfigureCamera, reload the active viewport and D3D device, submit the
viewport and finally write the configured flag. Any nonzero index selects the
background camera and flag0; zero selects the default camera and flag1. The
nonzero final state restores the default camera after flushing.

Both newline paths reset X and advance Y, then jump past horizontal spacing.
The earlier source added a space after every newline. The regular renderer
also republishes VM dirty bit8 after copying each glyph's dimensions; this
write was missing. The target loads height from sprite+30 and width from+34,
storing them at VM+50/+4C respectively. GUI intentionally retains its existing
dimensions and per-record dirty publication. Its ordered scaleX==1 branch
uses rounded drawing; other values, including unordered NaN, use no-round.
This draw-mode source was already correct and is retained.

The [target facts](target-facts.json) bind these effects to complete reviewed
owners: regular draw738, GUI draw570, low callback7, high callback10 and
camera configuration330. Local read-only Ghidra confirms the boundaries,
callers and instructions. Its inclusive412289-byte mapped text hash matches
the target before/after review; temporary metadata is rolled back.

The partial Main view has cameras at154/26C, active viewport at384 and the
dword flag at388, relative to target491C28. Existing StageCameraStateView
supplies their118-byte layout. Native offset assertions preserve these facts.
This is a storage view, not recovered original class identity or allocated,
alias-coherent data ownership. The Main, Stage and ANM global aliases remain
an open product-closure obligation.

An ordinary helper initially remained outlined at the variable-index sites.
The maintained macro expresses the target's expanded operations without a
forced inlining attribute. Its original source spelling is unknown. The
unimplemented external interface is removed. The existing global draw-function
signatures are retained; member/fastcall controls did not improve them.

## Compiler evidence and limits

The [native receipt](native-summary.json) records the actual profiles, all input
pins, object hashes and comparison scopes. The six-TU graph adds real
StageDraw.cpp to the existing ANM/RandomMath/VM/Int3 scale context. All inputs
retain /GL /GS; only the existing Int3 scale support retains its /Ob1 override.
StageConfigureCamera naturally receives its camera in EDI in this graph, but
its complete330-byte body remains nonexact.

| Owner | Old five-TU source | New six-TU source | Target |
| --- | ---: | ---: | ---: |
| Regular draw | 542 | 726 | 738 |
| GUI draw | 417 | 572 | 570 |
| Low callback | 10 | 7 | 7 |
| High callback | 10 | 7 | 10 |

These are complete PDB extents, not exact percentages or partial credit. The
high callback now pushes a stack receiver. Its renderer keeps manager in EBP
and the record pointer in EBX; target expects EBX manager and EBP record.
Size proximity does not resolve those differences or prove complete renderer
behavior.

The final diagnostic recompiles the primary and reuses only five support
objects whose recursive inputs, profiles and hashes are unchanged from the
preceding cold build. It is not claimed as a new cold exact proof. Seventeen
complete existing guards pass6589 bytes/all265 fields in that actual image,
including both draw methods and FlushVertexBuffer;
the low callback adds a complete7-byte/one-field linked guard. Separately,
the existing low-callback normal-COFF unit and Initialize330 linked unit
cold-replay exactly:337 bytes across two artifacts. Their canonical recipes
and existing credit are unchanged.

ExecuteScript remains9964 bytes with90/92 target-sized spans, absolute gap2
and7478/7995 case-aligned comparable bytes. These headline diagnostics do not
establish code neutrality or full equality. ECL's26/27 residual counts are
historical measurements from277ab88; verify current dependencies before
reusing those carriers.

## Host contract and restored controls

`scripts/test-ascii-viewport.py` extracts the actual macro and both functions'
transition/tail blocks. Its72 scenarios cover zero/nonzero indices, unchanged
state, final restoration, selection changes during flush, camera/device
replacement during configuration, and flag publication order. Eight mutants
must fail their designated functional predicates; compilation failures,
signals and other assertions are not accepted as kills. ASan/UBSan/leak
checks pass. Camera internals, target ABI/layout and original/native-product
execution are outside this source projection.

`scripts/test-ascii-glyphs.py` extracts actual per-record setup and character
loops. Its198 scenarios cover newline, space, successive glyphs, font banks,
shadow, two scales and GUI unordered comparison. Three mutants must fail the
designated position, dirty-bit or draw-mode predicate. ASan/UBSan/leaks pass.
A renderer role clears dirty after each observed draw to expose publication;
this is not an assertion that the original service always clears it. Integer
geometry and source traces do not prove x87 precision or whole-renderer equality.

The transition-effect fixture's timer, viewport type and RNG helper extracts
are independently unchanged. Only reviewed whole-file pins are refreshed;
strict normalization and rejection of unreviewed inputs remain intact.

Earlier controls in this bounded investigation are restored: static integer
adapter orders, static ReadInt context orders, a FORMAT state object, an
ordinary FLIP_X helper and a captured flag word. The static ReadInt variants
preserve the complete144-byte callee/all1 field but grow the dispatcher and do
not recover its adapter. The FLIP_X target uses accumulator XOR; the candidate's
EDX encoding is one byte longer. No register, padding or fake ABI is introduced
to bypass that constraint. Results describe only the recorded contexts.

## Reproduction

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ascii-manager-on-draw-low --unit ascii-manager-initialize
scripts/repo-python scripts/test-ascii-viewport.py --sanitize
scripts/repo-python scripts/test-ascii-glyphs.py --sanitize
```

The private six-TU runner, frozen inputs and unresolved diagnostics remain in
`.analysis/gpt-6.1-sol/ascii-render-member-20261021/`. Earlier negative controls
are in the sibling `enemy-int-adapter-20261021` pack. Read their recovery notes;
runners do not restore source. Superseded owned native outputs are removed
after retaining source controls and compact reports. No executable, database,
generated decompilation or toolchain is committed.

The first CI run was deliberately terminated after new target evidence
identified the newline omission. It is recorded as cancelled/exit143, not
passed. Final validation and cleanup are recorded separately in
[validation-summary.json](validation-summary.json).
