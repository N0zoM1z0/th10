# TH10 next-agent quick resume — 2026-11-10

Current detail: [RE_HANDOFF.md](RE_HANDOFF.md). Read [AGENTS.md](../AGENTS.md)
before code changes; [November 9 quick resume](NEXT_AGENT_20261109.md) is a
**historical snapshot**.

- Checkpoint `558e58a`, `main` clean, **37 local commits ahead** of
  `origin/main` at inspection. Check actual `git status --short --branch`
  and continue dirty files directly; do not reset or push.
- Target `resources/th10.exe`, Japanese v1.00a SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Use pinned VC7.1 SP1 and source-specific /GL recipes.
- Canonical [PROGRESS.md](PROGRESS.md): **1,514 exact functions /
  199,496 codegen bytes**; authored subtotal **601 / 129,209**.
  Older 1,510 / 198,606 statistics are obsolete.
- New exact Ending LoadAnimation `0x40C3C0`: **126/126**, all 7 fields,
  two selected native cold replays, with exact 1568-byte Ending Run guard.
  [Proof](evidence/ending-loadanimation-126-exact-20261110/README.md).
  New Sound LoadSoundSample: **684/684**, 25 fields.
  Two input merge helpers added 37 + 43 exact bytes.
- Attack both bosses and leaves. ANM ExecuteScript is **9962/9964**,
  350 fields (opcode91 EDX/ECX); ECL Run **6997/7020**, 189 fields
  (23 stack/x87/FORMAT differences). Bullet absolute direction **333/335**;
  Sound PreloadBgm **365/369**; Seek **100/107**.
- Other nonexact medium/large work: ECL AddScriptData **510/589**,
  Statistics factory 98/97, Bullet SpawnSingleBullet 1504/1413,
  Spell HUD StartSpell 2252/2432. Verify current source contexts.
- Use `scripts/repo-python scripts/replay-exact-units.py --unit NAME --json`
  for **selected affected** units. Never run the full historical cold replay;
  source edits need whole-owner original-bound comparison.
- Wine/LTCG can sporadically fail to open an OBJ after a page fault;
  this is a failed build, not a mismatch. Retry the selected unit.
- Private inputs and snapshots are ignored by Git. For preservation,
  cleanup and transfer use [RE_ARTIFACTS.md](RE_ARTIFACTS.md).

Make meaningful `gpt-web: ...` commits, preferably several for distinct
verified batches, but never invent acceptance credit merely for formatting
or for byte-neutral source-semantic improvement.
