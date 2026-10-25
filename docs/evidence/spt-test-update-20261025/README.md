# Spt Test update and concrete ECL-host factory, 2026-10-25

## Scope and independent raw review

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting checkpoint: `c7e3f6e487f03b725bb799e6e9b1f9230c111cb2`.

- Update `42BFC0..42C35D`, **926 bytes**, is authored game-specific state logic.
  Its six-byte LEA at `42C26A` is internal alignment, included in the owner.
  No jump table; final RET followed by two CC bytes. All39 link fields and48
  fixed local destinations are inventoried. No global absence of external
  interior references or historical TU ownership is claimed.
- `450500..450558`, **89 bytes**, is the authored database-to-concrete-host
  factory. RET4 is followed by seven CC bytes. Its inlined constructor work
  does not turn the whole game-specific factory into a generic generated body.
- Registered update callback `42C580..42C584`, **5 bytes**, is a direct JMP
  to the update owner, followed by eleven CC bytes. Source callback versus
  compiler veneer origin remains **indeterminate**, with no authored credit.

Fresh mandatory target/toolchain/tracking preflight passed 11:28:20 UTC;
Factory-native Ghidra check passed 11:27:44. Fresh read-only architecture export
passed 11:38. The separate automatic boundary-ledger check still rejects the
pre-existing `401100` evidence-text mismatch; this is not a passed global gate.

## Complete models, not fragments

| Graph | Update | Factory | Required same-image guards |
| --- | --- | --- | --- |
| First natural free factory | 929/926, structural reject | 87/89, structural reject | twelve exact,1492 bytes |
| Natural database-member factory | 926/926, nonexact | 89/all3/all2 locals exact | twelve exact,1492 bytes |
| Separate canonical-source cold graph | 926/926, nonexact | 89/all3/all2 locals exact | twelve exact,1492 bytes |

The first comparison observer exited1 on an unrelated canonical linkage without
`symbol`; its original script/log are retained. A corrected observer reads the
same image, with no native rerun or rewritten failure. The first native itself
completed0 and its freeze passed.

The single tested correction changes the factory from a free function to
`EclVmScriptDatabase::CreateHost(name)`. Target receiver dataflow and RET4
independently support this natural member contract. It restores the factory's
RET4 and removes the update's three-byte caller cleanup. Private EDI allocation
emerges from LTCG; there is no forced ABI, new noinline, register hint, padding,
or profile sweep.

Update remains nonexact despite equal extent and all48 identical local targets.
Its second FindFirstFileA pattern address field is at candidate+0x271 versus
target+0x274 because array-pointer publication moves across argument setup.
Other ordinary differences are register allocation in file enumeration.
All39 candidate fields have explicit symbol/base/addend identity, but the
shifted field is diagnostic only and is **not normalized into acceptance**.
Independent review found no missing effect or supported further semantic factor;
park spelling/register/scheduling panels until new target evidence appears.

Actual graph: SptTest primary with AnmManager, Chain, ScriptViewerWorker,
ScriptViewer, FrontEnd, Main, EclVm and Enemy support; all /GL /GS /EHs,
actual `SptTestUpdateCallback` entry. Original helper/Draw recipes are preserved.
The new declaration does not alter vtable layout. Constructor definition is the
existing concrete Enemy host constructor, shared unchanged; no abstract host is
instantiated and no alternate constructor is invented.

## Failure semantics and unresolved dependencies

State0 retains two-pass `../../data/*.ecl` enumeration, invalid FindClose,
unchecked second search and allocation, uninitialized pointer slots, and the
first count even if the second pass stops early. Cursor counts3/count/1, wrapping
and signed clamps are retained. State1 rereads input separately for each menu
operation. File selection clears the distinct+2D4 flag, starts the real worker,
then publishes state2: the async publication race is unchanged. Callback42BE10
is still only a declared dependency; its target uses the global owner rather
than the supplied context. The parked LoadScript261/callback16 source proposal
is not admitted or claimed exact by this work.

State4 calls Run(1.0) before any null check, ignores its result, rereads activeVm,
and dereferences its active context without a null check. Factory89 allocates
the existing103C concrete host, assigns the database, calls actual FindSubroutine,
rereads active context and publishes instruction/time0. It does **not** call
ResetEclState or initialize active context. The target's uninitialized-pointer
and failed-allocation behavior is preserved, not repaired. Original Spt constructor
still erases its embedded worker vptr with memset. Unresolved Spt destructor
remains declared. External declarations/anchors prove no ABI/runtime closure.

## Accounting and validation

Admission is scoped to **89 new authored exact bytes**, not update926.
Authored denominator and maintained source both increase **1015 bytes /2 owners**;
callback5 is separately bounded/source-mapped and origin-indeterminate.
No product, runtime, semantic-phase or portability claim follows.

Actual canonical build-match-unit replay passes89/all3/all2 plus callback5.
Separate original helper315 and Draw641 cold contexts pass. The final native
wrapper exited1 only at its ASCII observer (wrong primary versus support PDB
owner); original exit/log are retained. A separate same-image metadata supplement
passes ASCII100/all4 without any native rerun, preserving all2104 frozen inputs.
Transport timeout was recovered from durable state, not retried.

Final canonical recipe replay, source-host checks, frozen full-CI and checkpoint
results are recorded in `validation-summary.json`. Host tests use explicit
synthetic dependency/layout/allocation seams; they never execute known null or
uninitialized target UB and are not original-game runtime evidence.

All **116 full-CI stages passed**, terminal0 verified12:00:29 UTC, with all
**2492 frozen inputs unchanged**. The RPC timeout was recovered from durable
logs/exit/freeze; no duplicate CI was launched. Only CI-result documentation and AST-identical fixture EOF whitespace
changed afterward; focused sanitizer/documentation/tracking/progress checks passed.
