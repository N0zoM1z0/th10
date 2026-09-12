# TH10 reconstruction handoff

## Current phase

Exact reconstruction remains in boundary/origin review with early pinned-VC7.1
feedback. This checkpoint reviews the executable entry seam and the application
entry hard frontier. It does not claim source presence, function exactness,
whole-product closure, runtime validation, semantic reconstruction, or ports.

## Session identity and recovery

- Repository: `th10`; branch: `main`.
- Starting HEAD: `1c89a91a57d9ff45cace3308f3c0e143b4657d42`.
- Starting tracked/untracked state: clean; 0 staged, 0 unstaged, 0 untracked,
  0 conflicts; upstream `origin/main`, ahead/behind `0/0`.
- Mandatory repository guidance and Factory contracts were all mounted and read.
- No interrupted tracked or untracked work required recovery.
- Preserved ignored/private state: `resources/th10.exe`, `.tools/`,
  `ghidra-project/`, pre-existing `.analysis/bootstrap/` and
  `.analysis/factory-native-ghidra/`, and pre-existing generated `build/` state.
  None was reset, replaced, deleted, staged, or committed.
- `.analysis/` measured 4,222 bytes at entry. Current-session scratch is isolated
  under `.analysis/gpt-web/20260912-main-frontier/`.

Two read-only Factory repository-shell requests encountered transient transport
failures and returned no command ID. After each failure the live repository
status/diffs were re-read before continuing; no unexpected filesystem change was
observed. The Ghidra attestation was also rechecked after the first failure.

## Attested baseline and preflights

The Factory binding is exactly `target:th10-main` through read-only
`th10-ghidra`. `factory_analysis_call(check, {})` passed with
`attestation.provider_transport=factory-native-command`, the pinned target hash,
image layout, and entry point. Useful Ghidra results in this packet carried the
same passed attestation and have `exactness_credit=none`.

Repository preflights passed:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

The pinned VC7.1 SP1 smoke still executes normal C/C++ COFF, C++ LTCG,
resources, and PE32 i386 linking. Rich evidence remains 131 normal-C, 15
normal-C++, and 52 LTCG-C++ build-6030 records; per-function input ownership and
profiles remain unknown.

## Reviewed hard-frontier packet

### `0x004537DC` — CRT entry

The reviewed extent remains `0x004537DC-0x004539B0`. Target-local disassembly
shows VC runtime startup: runtime/heap/thread/IO initialization, argv/environment
setup, WinMain argument preparation, the single call to `0x00438AD0`, and CRT
exit handling. It is now classified `library / CRT / exclude / high` rather than
authored game code. The exact original CRT source file/library member remains
unknown.

### `0x00438AD0` — application entry / `WinMain`

The reviewed authored extent is `0x00438AD0-0x0043903C` (1,389 bytes). The
unique CRT caller supplies the four GUI-entry arguments and the target returns
with `ret 0x10`; `0x0043903D-0x0043903F` is three-byte `0xCC` padding. The body
owns the target-local D3D9 cooperative-level/reset message loop, restart path,
window teardown, configuration writeback, and process-level cleanup. The ledger
therefore proposes `WinMain` with `__stdcall` and the four-argument WinMain
signature, but has no source mapping and no exact claim.

Target strings inspected in this packet include `th10.cfg`, `./log.txt`, the
startup log banner, the Direct3D creation error, and the option-change restart
message. They corroborate the application-entry lifecycle but do not establish
TU ownership.

### `0x00438A30` — reset-path resource-release seam

The reviewed extent is `0x00438A30-0x00438A5A` (43 bytes), followed by five
`0xCC` bytes before independent `0x00438A60`. Both target-local callers
(`0x00438AD0` and `0x004391F0`) load the same manager pointer into EAX before the
call. The callee uses that EAX object base and releases/clears 32 COM resource
pointers at object offset `+0x3AD4E0`. Authorship is high-confidence, but the
physical object owner/name and source-level ABI remain unknown.

The EAX object convention is a material optimizer-context signal. It is not a
normal out-of-line MSVC `__thiscall` boundary, so this packet does not strip an
LTCG hypothesis merely to fit the standalone normal-COFF Oracle. Which Rich
record/TU owns the code is still unknown.

### Denominator challenge after `WinMain`

Raw target code starts at `0x00439040`, `0x00439060`, `0x00439080`, and
`0x004390A0` after WinMain padding. Ghidra reports no function containing any of
those addresses, `xrefs_to` reports no direct references, and a bounded raw
little-endian VA scan finds no literal pointers to them. Their register-dependent
entry shapes are compatible with cleanup/compiler funclets, but that origin is
not proved. They therefore remain documented unknowns and are not added to the
function denominator yet.

## Compiler feedback for this packet

A current-session ABI probe is retained under
`.analysis/gpt-web/20260912-main-frontier/`. With the pinned compiler:

- `/MT /O2 /Gy /GF /Oi /DNDEBUG` emits normal i386 COFF with symbol
  `_WinMain@16` and `ret 0x10`.
- the same probe with `/GL` emits a VC7.1 LTCG intermediate object rather than
  normal COFF.

This confirms the compiler-side WinMain ABI and exercises both candidate
artifact classes. It is diagnostic only: the probe is not reconstructed TH10
source and has no exactness or source-presence credit.

Adjacent-game committed source was consulted only as hypothesis material:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean.
- TH09 HEAD `943ec407a9429379fa9fdbcde97dd246e51f10a8`, clean; no relevant WinMain hit.
- TH095 HEAD `ca16d9dd06101ae317b873069e30b6f61b801f20`; its worktree had unrelated
  untracked files, so only committed `HEAD:src/Main.cpp` was read.

TH08/TH095 supplied a useful lifecycle/name hypothesis, but every promoted TH10
claim above is independently supported by TH10 target evidence.

## Ledger state after this checkpoint

After regenerating progress:

- tracked candidates: 1,195;
- origin/boundary pending: 1,192;
- reviewed authored: 2 functions / 1,432 bytes;
- reviewed exclusions: 1;
- source-present authored mappings: 0;
- canonical exact functions/bytes: 0 / 0.

Source presence remains false for this packet because the `0x732460` manager
layout, relevant globals/owners, TU boundary, and original source-level private
ABI are not established. No opaque padding, address-encoded fake object, target
bytes, or forced return was introduced merely to create a mapping.

The whole Windows i386 graph remains explicitly open. `python3 scripts/build.py
--check` passes, while the honest `python3 scripts/build.py` diagnostic exits 2
because compile flags, TU partition, libraries, resources, and link order remain
unknown. Runtime validation has not started. No new Factory Truth Kernel exact
claim is implied by these ledger edits.

## Validation at checkpoint

The following all passed after the edits unless an expected nonzero result is
shown:

```text
python3 scripts/validate-tracking.py --require-target       # pass
python3 scripts/progress.py --check                         # pass
python3 scripts/build-match-unit.py --check                 # pass; 0 units
python3 scripts/build.py --check                            # pass; graph open
python3 scripts/build.py                                    # expected RC=2; unavailable whole build
python3 scripts/report-reconstruction-status.py             # pass
python3 scripts/ci.py                                       # pass
git diff --check                                            # pass
```

No canonical `config/match-units.toml` row or `config/matches.csv` row was added.

## Next evidence-connected packet

Stay on the same D3D/application-entry owner seam rather than switching to a
small easy function. Review `0x004391F0` together with the larger
`0x00439890`/`0x00439D20` cohort: `0x004391F0` is the second caller of the EAX
resource-release routine and contains device-present/reset behavior;
`0x00439890` is a 1,159-byte central initialization candidate; `0x00439D20` is a
710-byte reset/render-state candidate. Reconcile their boundaries, caller/callee
ownership, COM slots, and shared globals, then test whether one coherent TU/LTCG
profile explains the private ABI before writing owner-bearing C++ or attempting
exactness.

This is deliberately another hard, central packet: it is selected for direct
evidence connectivity to the unresolved optimizer/owner boundary, not for small
function size. Do not declare the exact phase complete even if later reviewed
ratios cross the campaign pressure target; operator audit remains authoritative.
