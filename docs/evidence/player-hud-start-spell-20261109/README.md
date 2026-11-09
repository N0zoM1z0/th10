# TH10 spell-card HUD start (0x00409280)

This is the maintained source reconstruction of the 2,432-byte spell-card HUD start routine. The maintained source surface is EnemyGameStateView::StartSpell in src/PlayerHudStartSpell.cpp; the original class and translation-unit ownership remain unproven.

## 2026-11-09: spell name renderer identity

The original TH10 StartSpell call at **0x00409681** targets the exact
AnmRenderManagerView::DrawTextRight at **0x00447AE0**, after pushing
the current spell name as the format argument, 0x00FFFFFF as color and
the manager pointer; the VM is the internal ESI live-in. The previously
maintained RecoveryIndicatorVm_00419120::FormatRightAligned was an
invented source facade with the wrong receiver/argument semantics.

Production C++ now calls the actual manager DrawTextRight method,
passing GetPanelAnimation(panelVmIds[1].value), 0x00FFFFFFu and
panelNameCursor, preserving native formatting and removing the
fabricated class. Independent canonical replay of anm-text-right-gs
confirms the complete **204/204 exact** renderer; the spell HUD seven-TU
producer rebuilt successfully and still owns a **2252-byte** PDB
contribution against **2432** target bytes, with **322/1920** ordinary
bytes aligned and 83 candidate relocation fields. **NONEXACT**: no new
boss byte credit. Report: draw-text-right-seven-tu-report.json.

## Current natural C++ seven-TU candidate

A source-checked, complete seven-TU LTCG link of the current
`src/PlayerHudStartSpell.cpp` produces a **2,252-byte** StartSpell contribution
against the 2,432-byte target, with **323/1,920** ordinary bytes equal at
their original offsets after masking 83 *candidate-only* relocation fields.
This is **NONEXACT** and the 180 missing bytes are not excluded or accepted.
The actual target uses EBP for the HUD pointer; the old, hand-written
stage/digit register sequences assumed that EBP already carried `this`,
which is not true of the prior candidate's EBP-based frame. We replaced
all 13 digit initialization and 15 stage-script calls with ordinary
C++ member-field arguments and typed ANM adapters, instead of retaining
register-specific code with incorrect receiver semantics. All seven stage
cases, effect creation, history updates, and timer/bonus behavior remain.

The same seven-TU link retains a 202-byte script-start helper with
194/194 ordinary bytes and two candidate relocation fields, a 273/273
raw-equal VM initializer, and a 217/217 initialized-VM helper. The
naturally written script loader is 216 rather than the 203-byte target
**in this larger LTCG context**, while its separately reproduced two-TU
canonical unit remains 203/203 exact. Neither this source-partition
difference nor candidate-only relocation masking establishes boss exactness.

The focused recipe is retained in
`.analysis/gpt-web/20261109/relink-playerhud-restored-natural-loader.py`,
with report `playerhud-restored-natural-loader-results.json` in the same
ignored analysis directory. No full cold replay was used.

## Historical five-TU candidate

- Target extent: 0x00409280..0x00409BFF (2,432 bytes).
- Candidate extent: 2,416 bytes (16 bytes short).
- Focused link: five source TUs, 100 unresolved-symbol anchors, 32.8 seconds.
- Candidate-only relocation fields: 108 fields / 432 bytes.
- Ordinary bytes matching at equal offsets: 531 / 1,984.
- Result: NONEXACT.

The source now expresses panel creation through the existing native operations AllocateVm, InitializeVm, and AddVmVariant0, rather than maintaining a duplicate list-registration implementation. The full stage switch, replay-gated spell-history updates, digit VM setup, managed effect, child timers, bonus calculation and stage-specific scripts are present.

The exact 273-byte VM initializer at 0x00401DE0 is independently established in ../anm-vm-initialize-fastcall-20261109/README.md. The 0x00404F30 script-start adapter is separately maintained in src/AnmVmStartScript.cpp, but remains nonexact (200/202 bytes, 23/192 ordinary bytes). Neither leaf result transfers exactness to this boss.

See focused-five-tu-report.json for the selected-function linked-PE comparison and source-pins.json for the exact source/image hashes. This is a focused replay only; no full cold replay was run.

## Subsequent maintained natural C++ owner, seven-TU focused diagnostic

The maintained src/PlayerHudStartSpell.cpp has been revised **from the
currently compiled source epoch**, not copied from the old five-TU result.
A prior raw-assembly experiment assumed EBP was the HUD object even though
the observed seven-TU candidate used EBP as a stack-frame pointer; its
hard-coded EBP+778h/24D8h and EBP+10h/3BCh loads could therefore
address incorrect storage. The current source instead uses actual
frontVms[], secondaryFrontVms[], and stageVms[] object members, normal
C++ counted loops, StartAnmLoadedScript, and StartEntityScript_00404f30.
The loader and stage-start helper retain their independently recovered
semantic bodies and are called via their typed surfaces.

Selected seven-source LTCG diagnostics (same target function
0x00409280, all 7 relevant sources from the current checkout):

- Original StartSpell owner: 2,432 bytes (including original code/data).
- Current complete candidate PDB contribution: 2,252 bytes.
- Candidate-field-excluded equal bytes at equal offsets: **323/1,920**
  (83 diagnostic candidate fields); **NONEXACT**. These are not original
  target-binding or partial-owner admissions.
- Target 0x00404F30: 202 candidate bytes with 194/194 ordinary equal
  after excluding two candidate fields (no whole-image admission).
- Target 0x00401DE0: 273/273 and raw equal in the same focused link.
- Target 0x0043E710: emits 216 bytes here versus 203 retail; the
  restored exact result remains scoped to the canonical *two-TU* graph.
- Target 0x00449870: 217 candidate bytes with 213/213 ordinary equal,
  one candidate field.

Source epoch and full selected-function report are saved in
seven-tu-source-pins.json and focused-seven-tu-native-cpp.json. This
whole-owner diagnostic does not replace the independently bound
203/203 canonical ANM loader proof or give boss exact credit.

Two natural C++ source alternatives were compiled in the same seven-TU
graph: register qualifiers and the /Oy pragma were byte-neutral; an
explicit do-while copy loop lowered the equal ordinary byte count from
323 to 309. The maintained version retains the better typed source.
No full cold replay was run.
