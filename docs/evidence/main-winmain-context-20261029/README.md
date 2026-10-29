# Complete WinMain — 2026-10-29

WinMain438AD0..43903C adds **1389 authored exact bytes**. Two independent
canonical 30-input cold links reproduce the complete owner and all146 address
fields. This was already a source mapping; mappings and the reviewed authored
denominator remain unchanged. [Results](results.json) retain source/include and
Oracle pins, profiles, artifact identities and the canonical field-contract hash.
The complete binding contract is `units.main-winmain` in match-units.toml.

The real CRT call at45395B supplies WinMain's four arguments; the owner ends in
`RET 0x10`. Bytes43903D..43903F are padding. The following439040/60/80/A0 blocks remain
outside this reviewed extent and receive no credit; their ownership is unknown.

The complete source covers allocation and initialization, configuration/dialog,
window/device/audio setup, timing, message/device-loss/render dispatch, restart,
game/Chain/renderer/MIDI teardown, log flushing, system-parameter restoration,
critical-section release and registered-allocation cleanup. Actual null/error
branches are included in the comparison. No host failure/lifecycle campaign is
required or executed.

The previously complete1390-byte candidate had substantial ordinary differences.
Four natural source details recover the target:

- Both critical-section walks advance through the seven records while counting
  down. This replaces the candidate's end-pointer comparison. The compiler then
  naturally restores the target0x20 frame, entry saves and status local on stack.
  No volatile local or forced register/spill is used.
- The allocation registry has actual constructor/destructor roles: zero the
  0xA004-byte object, retain the hasEntries clear, and free its0x2800 slots only
  when the flag is set. `delete` retains the target nullable teardown. This role
  was reopened after the countdown changed the complete native context; its
  earlier no-match was not evidence against every later use of the same model.
- Registry, Chain and ANM construction results first enter typed local pointers,
  then publish to their globals. This reproduces the real null-result joins and
  common stores. Direct global new assignments duplicated those stores.
- Restart resets the existing log buffer through ordinary `ResetBuffer`, leaving
  the fatal flag alone. Its receiver is naturally reused by the log call, giving
  the target pointer store and subsequent alignment. No fake ABI or explicit
  inline/register annotation is added.

The full native sequence is1390,1389(countdown),1405(registry),1389(locals,
still nonexact), then1389 exact after buffer reset. Complete comparisons, not
sizes, determine acceptance. Existing pinned compiler/linker profiles remain
unchanged; all146 fields bind reviewed semantic bases and member offsets.
Nearby public names in the diagnostic image are not object identity. Six
literal/scalar occurrences are independently raw-equal in both cold images.

Each image also strictly reproduces eight selected existing owners1935/all160:
Render425, Present236, configuration534, Log201, Fatal201, Flush79, Chain
teardown103 and MIDI output teardown156. The explicit canonical cold selection
is WinMain plus Render; the other retained complete contracts are selected by
name in that same image. No historical cohort is rebuilt.

Camera remains290/287. Direct/half/unsigned dimensions and a variant omitting
explicit `/GS` do not recover it; those controls are restored. The full graph also has shutdown321/293
and timestamp282/282 nonexact. Their other accepted recipes remain separate;
this is not a claim of one fully exact executable. Original source names/TUs,
complete data ownership, product/runtime closure and portability remain open.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit main-winmain --unit main-render --json
scripts/repo-python scripts/ci.py
git diff --check
```

The comparator is unchanged. Only the new1389-byte owner adds exact credit.
