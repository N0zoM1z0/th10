# ECL Run storage and result lifetime controls

Started 2026-10-21 at `768d612`. This is a diagnostic checkpoint with no source
change or new exact credit. All four source controls are restored.

The new cold baseline uses the canonical `ecl-vm-host-run` two-TU context:
EclVm.cpp, Enemy.cpp, real EnemyRuntimeUpdate entry, unchanged VC7.1 `/GL`
profile. Full Run remains 7020 bytes with 26 ordinary differences after all
189 fields, including 62 local destinations, are bound. The complete 171-byte
ReadFloatValue callee is exact in each image. All 60 table words and 88 selector
bytes participate. This is distinct from the maintained larger graph's
27-difference result.

| Natural source control | Complete differences | Raw Run body |
| --- | ---: | --- |
| Baseline cold | 26 | `38b82dac...070aa` |
| Group LengthSquared's x/y declarations before assignments | 26 | identical |
| Initialize FORMAT argument counter before cursor and allocation | 26 | identical |
| Reverse both Pop pointer additions to offset + data | 26 | identical |
| Share one integer result across float comparisons and logical opcodes | 27 | different |

The last control adds a difference at `+0xAF` in RETURN's lowering. It does not
repair the late stack homes and is not the larger graph's extra SIB byte at
`+0x146E`. The first three controls reproduce the complete raw baseline body,
so these specific declaration/expression spellings do not control the residual
under this profile. This does not rule out other source lifetimes, types, TU
partitions or optimizer contexts.

Only the baseline is a complete cold build. Each control recompiles the primary
TU and relinks with baseline support objects whose hashes and complete recursive
source/header inputs are verified. The canonical recipe is fingerprinted and
checked unchanged. No profile switch, ABI declaration, boundary, acceptance
rule or target byte is changed. No broad historical exact replay was run.

Target identity and local read-only/noanalysis Ghidra attestations pass. The
complete mapped text is 412289 bytes with SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra runs at maxcpu1/768 MiB; native builds are serial at nice10.

[Evidence and complete hashes](evidence.json) bind each result and the compact
source patches. Private inputs, full diagnostic reports, producer and the
baseline/selected failure carriers remain under the campaign named in the
handoff. Three neutral native outputs are removed after preserving their
source overlays and reports; the cleanup byte count is in the evidence.

To reproduce the baseline with maintained tools:

```sh
scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-host-run
scripts/repo-python scripts/diagnose-ecl-run-native.py
```

The latter returns1 for the complete mismatch. Apply one supplied zero-context
patch with `git apply --unidiff-zero` to its stated starting source, rebuild and
diagnose in the same context, then restore that patch before another control.
Native anchor images remain diagnostic and
cannot establish a runnable product. Source spelling, physical TU, production
ownership and runtime remain unknown.
