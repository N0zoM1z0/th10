# TH10 ECL life-gauge reference at EnemyRuntime +0x13C4

The attested original Japanese TH10 v1.00a executable contains two direct
accesses to the 32-bit word at `EnemyRuntimeView+0x13C4` in the 14,416-byte
`EnemyRuntimeView::DispatchEclInstruction` owner at `0x0040E770`:

- At original `0x0040FC69`, `MOV EDX,[EBX+0x13C4]` loads the stored
  baseline and divides an ECL-supplied floating life marker by that value
  to populate a GUI boss gauge.
- At original `0x0040FF38`, `MOV [EBX+0x13C4],EAX` stores the same integer
  supplied to `ENEMY_ECL_SET_LIFE` and to the current `life` member.

The full Enemy's ECL reset also zeroes the reference. Recover the explicitly
layout-asserted `int lifeGaugeReference` in place of anonymous `unknown13C4`
and bind the writer, denominator read and lifecycle reset to that member.
The preserved type is integer: the marker path explicitly converts it to
floating point for division. This is an observed role, not original-source
spelling or a claim that the gauge marker denominator is never zero.

Fresh **11-TU** pinned VC7.1 /GL cold compilation preserved the complete
14,352-byte native dispatcher, all 716 candidate linkage locations/types,
and zero ordinary differences against the immediately prior source graph.
The 2,432-byte BeginSpell, 672-byte generated constructor, 330-byte viewport
and 162-byte death-finalizer carriers likewise preserved all bytes and fields.
Private native PE/MAP/PDB are retained at
`build/gpt-web/20261105-next-boss/life-gauge-reference/`.

Three selected original-bound exact cold units replayed **231/231 bytes**
(`enemy-runtime-constructor`, `enemy-ecl-reset-state`,
`enemy-ecl-context-constructor`); lightweight seven-stage CI passed.
The full ECL dispatcher is **still NONEXACT** (14,352 candidate vs 14,416
original), and no canonical exact, product, runtime or portability credit
is added by this source recovery.
