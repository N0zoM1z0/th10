# MIDI exact-first pilot: restore the observed inline clock operations

This is a nonexact source checkpoint, not an exact ledger update. The complete
ProcessEvent target remains 1670 owned bytes at 0x43B110; Tick remains 473
bytes at 0x43AF30. The reviewed boundary and compiler context are unchanged.

## First complete comparison

The previous 1614-byte parser diagnostic had incomplete normalization. Its
structural position count was not a complete exact score. A fresh independent
audit derives the original and candidate code, three compressed selector groups,
all address fields and local direct-control destinations from their own PE/PDB
extents. It never decodes target fields at shifted candidate offsets.

The original is 1323 executable bytes plus 347 table/index/alignment bytes.
Its three pointer tables contain 4/5/8 entries, with 113/65/94 selector bytes.
The original has 37 link fields and 43 local direct-control fields. The original
Tick has 14 link fields and 12 local controls. Existing report-midi-events.py
independently revalidates the target tables and all complete original calls.

Root cold-built the unchanged maintained source before the experiment. It
reproduced ProcessEvent1614/35 fields/44 controls and Tick481/14/12. The parser
contained a separate 65-byte MidiElapsedTicks helper where the original instead
executes two __allmul and one __aulldiv calls inside the complete owner. There
is no target callee identity to invent for that source-only boundary. All three
candidate tables were 56 bytes earlier than the target, not silently relocated.

## One coherent source model

The change removes that helper and spells its exact existing unsigned64
arithmetic at all three uses: twice in Tick, once in the tempo-event path.
Signed-to-64-to-unsigned conversions, multiply order, modulo64 arithmetic,
unsigned divisor and actual read points are retained. No new inline attribute,
compiler flag, ABI declaration, artificial consumer or padding is introduced.
The real Tick entry and single Midi.cpp /GL context remain fixed.

The source SHA changes from
f847c1e0aa9dbd9c1ebaf796056db50a43af3ffd950fc48f38b62bfaa6566a67 to
7871c32b29e1d81a2db45a91d08bc65e0a314f37885fb3befbbea81859475439.
Midi.hpp remains
8ed76f8cc8e1c6aac287213a8fc00fa009a31303924552252a1920278e8c7f06.

## Measured result and limits

A separate root cold compile/link restores all 37 parser fields and removes
the unsupported helper. ProcessEvent is now 1690 bytes, with 1341 executable
bytes, 349 tail bytes and 45 local controls. Tick remains 481 versus 473.
All five same-image guards remain complete exact: ApplyChannelVolumes140,
ResetTracks132, ReadVariableLength26, base Tick1 and UnprepareHeader79; total
378 bytes and five fields. These are guard results, not new parser credit.

The two extra controls jump over alignment before the tempo-byte loop and
loop-save track loop. The executable-size budget is +14 alignment bytes plus
four jump bytes; +2 pretable alignment yields the +20 owned extent. This is
accounting only, NOT ordinary-byte equivalence. Every instruction and alignment
byte must still match. Both main owners fail the fixed complete contract, with
ordinary_byte_score=null and acceptance_authority=none. A matching call count
or code-byte budget cannot waive offsets, tables, registers or local targets.

The arithmetic expansion passes its specific prediction but does not solve the
owner. SysEx allocation/capture/cleanup observations have not established a new
semantic error. No register/alignment spelling panel follows from that alone.
The raw target and exact family also expose private MIDI helper register edges;
target-wide Rich records are not a reason to substitute a non-/GL profile.

## Source-test boundary

The initial host test did not compile because it called the removed private
source helper directly. That failure is retained. The fixture now observes the
real ProcessEvent tempo-event path for signed clock multiplication and a
negative divisor, rather than adding a production helper solely for a test.
The positive suite and all 13 existing negative mutation controls pass.
These host allocation/WinMM roles do not prove native x87, Windows ABI, the
untouched message high byte, arbitrary memory aliasing or whole-game runtime.

## Receipts and reproduction

Ignored packet: .analysis/gpt-dots/midi-exact-first-20261024/.
- target-contract.json, audit.py and baseline/report.json: independent retained
  baseline inventory; original bounds/identity are fixed.
- cold-baseline-v01/producer.json and baseline-v01/report.json: fresh actual
  maintained-source build and complete structural rejection.
- cold-candidate-v01/producer.json and candidate-v01/report.json: fresh changed
  source build and complete structural rejection.
- independent-candidate-v01/: separate no-compile replay and size-budget audit.
- model.json and remove-unsupported-helper.patch: the actual pre-build hypothesis
  and expected source hashes, not a claim of a subsequently unchanged status.
- root-host-mutations.log/.exit, fixture-update.json and host-v01-fixed output:
  actual source-test adaptation and results; host-v01 retains the initial failure.
- ROOT_COLD_PLAN.md gives guarded serial commands; audit.py accepts explicit
  candidate directory, producer receipt and provider receipt. Never overwrite
  frozen baseline outputs or refresh their old source pins.

Native output roots are build/gpt-dots/midi-exact-first-20261024/baseline-v01
and candidate-v01. Original/retained carriers were not overwritten. Ghidra
check is bound by .analysis/gpt-dots/th10-resume-20261024/midi-ghidra-check.json.
The comparison is a diagnostic, not a runnable linked product. No new exact
bytes, source-present owner, boundary/origin change or phase gate is claimed.

## Checkpoint validation

Full scripts/repo-python scripts/ci.py completed all111 declared stages with
exit0 against the final Midi.cpp and fixture bytes. root-ci.log and root-ci.exit
retain the completed result; transport timeout did not cause a second CI run.
Tracking, toolchain declarations, generated progress, documentation links and
whitespace pass. All nine unrelated untracked D3D files are hash-unchanged.
No new canonical exact unit or authored exact byte is added.
