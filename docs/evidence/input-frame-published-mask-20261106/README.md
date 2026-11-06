# TH10 input frame: separate publication from the repeat cursor

Target: original Japanese TH10 v1.00a `PollInputFrame` at
`0x0044A5F0..0x0044A941`, **850 complete bytes**, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The fresh current-source baseline emitted **855 bytes**, rather than the
854-byte result from the older input-polling source epoch. Its keyboard paths
and joystick call already agree with the original instruction sequence. The
remaining structural departure starts after the joystick call: the single
mutable `bits` local was spilled before publication, then reloaded to store
`binding->current`.

The original publishes the return word directly from AX at +0x2D9, then
homes the destructive bit cursor at +0x2DE. The maintained source now captures
`const unsigned short merged`, publishes previous/current, and initializes a
separate `unsigned short bits` for the sixteen repeat-counter iterations.
Both values have real uses; no volatile, register declaration, ABI change,
padding, target bytes, or extra hardware access is introduced.

A fresh final-source canonical cold of **only** `input-controller-buttons`
rebuilds this single-TU `/GL /GS` graph. The affected existing query passes
**264/264 with its 12 fields**. In that same PE/MAP/PDB, a complete independent
original-bound comparison of the open frame owner now measures:

- **850/850 extent**, fully decoded;
- **10/10 independently original-checked fields**;
- **807/850 equal bytes**, 43 ordinary differences, first at +0x2DD;
- remaining differences in repeat-mask/pointer/counter registers and edge-word
  calculation/store scheduling.

The [compact native record](native-result.json) pins current source/headers
and this exact artifact. [The candidate contract](candidate-contract.json)
records all ten target fields; it is deliberately outside the canonical unit
ledger. **NONEXACT; zero new exact credit.** No partial prefix or size-only
acceptance is claimed.

XOR operand reordering and the explicit countdown loop were byte-neutral.
Wider repeat masks plus explicit word snapshots did not reduce the 43 ordinary
differences. These controls were restored; do not repeat them without a new
source/context premise. The keyboard diagonal spelling is not the current
mismatch: both complete keyboard branches already align in the baseline.

Reproduce from the repository root:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit input-controller-buttons --json
scripts/repo-python - <<'PY'
import json, runpy, sys, tomllib
sys.path.insert(0, 'scripts')
manifest = tomllib.load(open('config/match-units.toml', 'rb'))
manifest['units']['diagnostic-input-frame'] = json.load(open(
    'docs/evidence/input-frame-published-mask-20261106/candidate-contract.json'))
compare = runpy.run_path('scripts/compare-linked-function.py')['compare_unit']
print(json.dumps(compare('diagnostic-input-frame', _unit_manifest=manifest), indent=2))
PY
```

Private investigation: `.analysis/gpt-web/20261106-input-frame/`, including
`final-guard.json`, `final-strict.json` and the rejected controls. No historical
cohort, host failure scenarios, game runtime or product-closure tests were run.
