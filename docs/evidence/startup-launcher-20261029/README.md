# Startup object construction and launch, 2026-10-29

Five complete owners add **435 exact bytes / 26 fields**. Existing origin
classifications give **300 authored exact bytes**; 135 constructor/adapter bytes
remain origin indeterminate and receive no authored credit.

| Owner | Target | Bytes | Fields | Unit |
| --- | --- | ---: | ---: | --- |
| StartupWorkerView constructor | 41F850..41F8CF | 128 | 2 | `startup-worker-constructor` |
| RegisterTasks | 41FAC0..41FB4B | 140 | 12 | `startup-register-tasks` |
| CreateStartupWorker | 41FD00..41FD44 | 69 | 5 | `startup-worker-create` |
| ReleaseStartupResources | 41F930..41F98A | 91 | 6 | `startup-release-resources` |
| OnDraw adapter | 41FEF0..41FEF6 | 7 | 1 | `startup-draw-callback` |

Target is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Read-only local Ghidra attestation passes; fresh complete raw decoding and all-byte
reference queries supplement the database's missing callback functions.

## Natural source and first complete comparisons

The member layout is independently supported by construction, launch and cleanup:
worker at+10, VM at+30, VM id at+3DC, signature ANM at+3E0 and owner size3F0.
The constructor itself clears3F0, so the size is not inferred solely from new's
allocation. Worker and VM construction precede the whole-owner clear, including
the target's overwritten vptr, timer flags and active sprite index. No safety
repair or extra initialization is introduced.

The first complete model emits constructor135/128, launcher150/140,
destructor376/417, Draw218/220 and update67/63. Factory69 and release91 already
match structurally. Real ScriptViewer/worker and statistics definitions are
included; fifteen unchanged inputs are reused only after recursive source/include
and profile checks. The only initial compile rejection is a C++ callback pointer
type mismatch, corrected by the same explicit ChainCallback adaptation used by
existing maintained task owners. It produces no comparison and is not counted
as a rejected codegen model.

Two target-backed changes recover complete constructor128 and launcher140:

- The id at+3DC is stored as a scalar in this view. The wrapper-type model adds
  a redundant constructor write absent from the target. Removing that write
  restores the complete constructor without padding or a manufactured return.
- The launch protocol retains a reference to the actual worker subobject across
  Stop and all entry/state/API writes. This naturally recovers the target's ESI
  subobject lifetime and complete launcher. The callback is passed unchanged to
  `_beginthreadex` with its actual CRT callback type kept separate.

Startup uses an explicit `/EHs` support profile to retain its observed destructor
EH frame. `/EHsc` for Startup and then for its VM implementation are byte-neutral
controls, restored after their predicted cleanup-state change fails to occur.
All other pinned profiles remain unchanged. An explicit ASCII snapshot before
clearing the startup global improves the destructor to433/417, still nonexact.
Named Draw result temporaries change218 to222 and are restored. No fake calling
convention, register annotation, stack slot, inline assembly or target bytes are
used. Draw's ordinary member `__stdcall` expresses the observed stack owner/RET4.

## Inventory correction and independent origins

Five independently CC-delimited extents were absent from the ledgers:

| Target | Bytes | Final disposition |
| --- | ---: | --- |
| 41FD50..41FD6A | 27 | excluded VC7.1 scalar deleting-destructor wrapper |
| 41FD70..41FD81 | 18 | indeterminate nullable deletion helper, unmapped |
| 41FD90..41FDCE | 63 | indeterminate unreferenced update body, existing helper mapped |
| 41FEB0..41FEEE | 63 | indeterminate registered update copy, nonexact |
| 41FEF0..41FEF6 | 7 | indeterminate registered Draw adapter, exact |

The registered updates and Draw entry are the two actual addresses pushed by
41FAC2/41FAE5. All-byte Ghidra queries find only those entry references and the
update's owned conditional jump to41FEE9. Queries across the three other complete
extents find no references. Dense decodes end at RET and all separating CC spans
are checked. The duplicate update body differs by its EDX receiver versus the
registered copy's ECX. Existing Decomphelp source already contains both helpers;
that legacy source is left untouched. Source presence does not establish which
target entry was original source, an adapter or an LTCG copy. The initially
considered authored classification is therefore not adopted.

Nine newly recorded mappings cover1,198 target bytes; one maps the already
maintained Fn0041FD90 helper. These are mapping/recovery claims, not1,198 newly
authored exact bytes. The authored denominator remains275467. The constructor's
existing indeterminate origin also remains unchanged.

## Complete field proof and cold reproduction

The ordinary linked comparator checks qualified PDB contributions, every owned
byte, fixed local destinations and every semantic field. The launcher's callback
and thread entry fields bind their actual public source symbols. Address binding
does not grant byte credit to the nonexact update or thread body. Likewise the
factory and Draw adapter bind their actual destructor/Draw entries without
claiming those whole callees exact. Constructor vptr identity gives no vtable-data
or physical storage ownership claim.

Two independent canonical nineteen-input cold builds reproduce all five owners
and26 fields, with no object reuse. Twenty-three selected existing contracts
also compare **6235 bytes /179 fields** in the new image, including the seven
previous streaming owners, actual worker Start/Stop/destructor and FillBuffer.
This is a bounded native comparison; no full historical replay, host failure
simulation, product build or runtime/port closure is required or claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit startup-worker-constructor --unit startup-register-tasks \
  --unit startup-worker-create --unit startup-release-resources \
  --unit startup-draw-callback
```

[results.json](results.json) retains compact pins, proof scopes, boundary/origin
decisions and rejected models. Private listings, producers and exit logs remain
under `.analysis/gpt-6.1-sol/startup-launcher-20261029/`. The first native witness
is retained below `build/gpt-6.1-sol/startup-launcher-20261029/cold-a/`; the second
is canonical `build/match-linked/StartupWorkerLifecycle/`.

The destructor remains433/417, Draw218/220, update67/63 and startup thread292/299.
The destructor's observed two-state cleanup selects worker+10 then VM+30 from
saved this atEBP+4. This diagnostic metadata agreement is not an EH address-binding
or parent exact claim. The actual factory caller is4218D0, independently queried;
it is not evidence that the unresolved MainCreateCallbackOwner seam names this
factory. Missing thread homing and destruction/return-buffer differences remain
open; byte-neutral profile controls are parked.

The paired update entries support a further, untested natural source model: an
ordinary member Update with an inlined fastcall registration adapter. The two
receiver registers are observations, not declarations to force in that model.
