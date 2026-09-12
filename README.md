# 東方風神録 ～ Mountain of Faith

<p align="center">
  <img src="resources/progress.svg" alt="TH10 reconstruction progress">
</p>

This project reconstructs the original Japanese TH10 version 1.00a executable.
It tracks target identity, candidate boundaries, origin, source presence,
codegen exactness, native-product closure, runtime semantics, and portability as
separate facts.

## Exact target

Supply your own legal copy at the ignored path `resources/th10.exe`.

| Property | Required value |
| --- | --- |
| Version | original Japanese 1.00a |
| Size | `487,936` bytes |
| SHA-256 | `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040` |
| MD5 | `7dc488d82c81dd4aee4ba098b8804d83` |
| Image base | `0x00400000` |
| Entry point | `0x004537DC` |

The pinned [thcrap version database](https://github.com/thpatch/thcrap-tsa/blob/f08b582ce57bce800955dd371fc9a68dbad5b324/base_tsa/versions.js)
classifies this exact SHA-256 as `th10`, `v1.00a`, `(original)`; patched
and Steam executables have different digests.

```bash
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
```

Original executables, game data, Ghidra projects, decompiler output, and
downloaded tools are private and never committed.

## Current status

The target is attested and the initial Ghidra inventory is provisional. Every
candidate starts with unknown origin, unreviewed boundaries, no source-presence
claim, and no exactness credit. PE linker 7.10 and dominant Rich build-6030
records support a VC7.1 SP1-era compiler-family hypothesis. Exact compiler
binaries, flags, translation-unit boundaries, libraries, resources, and link
order remain unknown.

## Required phase order

1. Reconcile authored boundaries and recover exact historical-toolchain units.
2. Close and exercise the faithful Windows i386 product, including data owners,
   initializers, resources, libraries, and runtime scenarios.
3. Perform semantic reconstruction using both the original target and the
   reconstructed native i386 product as runtime Oracles.
4. Start portability only after the native semantic baseline is trustworthy.

## Project map

- [`AGENTS.md`](AGENTS.md) — mandatory session and evidence rules.
- [`config/target.toml`](config/target.toml) — target and observed PE facts.
- [`config/build.toml`](config/build.toml) — explicit open whole-build graph.
- [`docs/RE_WORKFLOW.md`](docs/RE_WORKFLOW.md) — reconstruction loop and gates.
- [`docs/ORACLES.md`](docs/ORACLES.md) — claim-specific falsification rules.
- [`docs/RE_HANDOFF.md`](docs/RE_HANDOFF.md) — current state and next work.
- [`docs/KNOWLEDGE_BASE.md`](docs/KNOWLEDGE_BASE.md) — TH10-only knowledge.
- [Touhou Reconstruction Factory](https://github.com/N0zoM1z0/touhou-reconstruction-factory) — shared ontology and workflow.

## License

Repository-authored code and documentation are provided under the MIT License.
This does not grant rights to the original game or its assets.
