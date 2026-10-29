# Stage dispatcher source and alias review — 2026-10-29

The complete StageExecuteScript403C80..4043CB remains **nonexact1868/1868**.
This checkpoint improves the maintained source and records discriminating native
controls; it adds **zero exact bytes/owners**, source mappings or denominator.
[Results](results.json) retain frozen inputs, complete candidate extents, table
review, bounded byte observation and affected canonical replay results.

The source now gives the28-byte StageFogView an actual packed-color union member
at18. Opcode8 accesses its four byte members directly. VC7.1 then loads all four
channels before writing their float conversions, matching the target operation
sequence. An equivalent-looking local AnmColorView reference recreates the old
interleaved loads/stores, even when it refers to the typed member. This isolates
the alias-analysis sensitivity without volatile, padding or fake ABI controls.
Native size/color-offset assertions preserve the existing layout.

The ordinary107-byte opcode8 prefix at owner+1FF is raw-equal after this change.
Its following jump still reaches a different owner-relative offset because the
complete owner differs later. No case, prefix or partial-owner exact credit is
claimed. The unbound same-offset diagnostic improves from1241/1724 to1301/1724;
shifted later instructions/fields make that score unsuitable for acceptance.

Opcode13's4923A8 destination is the already maintained Main.clearColor at
491C28+780. The source now uses that actual member and its existing native offset
assertion. This semantic binding is byte-neutral in the current dispatcher.
The separate GameManager alias and physical data/storage ownership remain open.
Main/product/runtime closure does not follow from this source access.

The complete owner still has frame8C and stack StageScript/RET4. Target owns
1806 code bytes, MOV EDI,EDI alignment2, and15 absolute indexed-table slots at
owner+710. The current candidate has1807 code bytes, NOP alignment1 and the same
complete60-byte table extent. All target and candidate slots are independently
checked against their respective instruction starts. Later local destinations
and opcode10/11/12/13 registers remain unequal. The actual VM initializer has
200/202 bytes and its binder197/203 in this graph, with incoming conventions
that differ from the target. Both complete callees remain nonexact.

Measured controls are retained and restored:

- A real Hermite setup helper is outlined; dispatcher1704 and its boundary change.
- Implicit vector-record copies change the frame to74 and dispatcher to1824.
- Capturing cubic duration is byte-neutral.
- Seven actual GUI-reset binder calls are verified in the target. Adding real
  Gui.cpp with its existing profile is neutral for dispatcher/initializer/binder;
  that twelve-input graph is removed. This differs from the historical experiment
  adding callers of the upper wrapper, but supplies no ABI or exactness credit.
- Reordering the initializer's meaningful inputs and expressing the same complete
  operation as a three-meaningful-input stdcall function are neutral. The actual
  maintained member, prototype and callers are restored. No unused context,
  forced register, fabricated return or synthetic caller is retained.
- A local typed color reference recreates the old channel schedule; direct
  typed member access is retained.

One targeted canonical replay selects nine affected exact units,4430/all155,
in their existing three recipes: Early/Instances/Bounds; Update/Tick; four Fog
constructor/arithmetic owners. Twenty-two explicitly selected old same-image
contracts pass6613/all282 in the eleven-input image. This includes the full
updated-source parents, Fog helpers and draw/interface guards. Header-inline
SetCurrent uses its physically verified StageScript.cpp PDB owner there.
No full historical cohort or unrelated Main recipe is replayed. The manifest,
match ledger, source profiles and comparator are unchanged.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit stage-draw-early --unit stage-draw-instances --unit stage-object-bounds-culled --unit stage-update --unit anm-timer-tick --unit stage-fog-constructor --unit stage-fog-subtract --unit stage-fog-add --unit stage-fog-scale --json
scripts/repo-python scripts/ci.py
git diff --check
```

The remaining dispatcher problem is its coupled register/callee context, rather
than size. Reopen the parked controls only with new discriminating source or
ABI evidence. Original spelling/classes/TUs, physical global aliases,
product/runtime validation and portability remain independent.
