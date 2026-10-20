# Replay load factory and allocation EH (2026-10-20)

`ReplayManager::Load @ 0x4296F0` closes **181 authored exact bytes**. The
maintained declaration now expresses its observed single stack path/RET4
protocol, and its body directly sets mode2 and calls the replay reader. It no
longer routes through the shared initializer's error-result normalization.
No register ABI, forced inlining, padding or target-byte source is introduced.

## Independent target evidence

The original Japanese1.00a executable is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local read-only Ghidra attests it and the complete mapped .text before/after;
temporary analysis metadata is rolled back. Factory ownership remains181 bytes
through4297A4, followed by11 CC bytes. All seven direct callers push a path;
both factory exits use RET4. The body allocates2D4 bytes, constructs eight24-byte
stage-state members at+A0, clears the manager and writes mode2 before reader
42A200. A nonzero result destroys/frees the object and returns NULL. Its source
mapping/name are maintained descriptions, not original identifiers or TU proof.

The prior native179-byte body instead has plain RET and a NEG/SBB normalization
before its condition. Fresh raw target review and the separate MIT hypothesis
source agree on the stack factory and direct reader shape. The adopted source
keeps the target's allocation-failure behavior; it adds no invented safe return.
The general shared Initialize body remains unchanged.

The12 independent fields bind the parent EH handler46564B, four FS exception-list
references to0, scalar allocator452493, stage destructor42AC60/constructor42AC20,
vector-construction helper45252D, reader42A200, replay destructor4294A0 and scalar
delete4524A1. The constructor callback roles follow the target's actual vector
arguments, element count/size and member destination. Callee exactness and
original source ownership are separate claims.

## Bounded allocation-EH address binding

The handler appears in the MAP **Static symbols** section under the exact name
`__ehhandler$` plus its parent decorated symbol. Its PDB region groups the
11-byte allocation unwind action with the10-byte dispatch handler. Ordinary
static C++ function selectors require a same-start function contribution and
remain unchanged; this generated region needs its own explicit role check.

`allocation_eh_handler = true` is permitted only on a separate DIR32 entry
field with zero addend and the exact parent handler name. The comparator joins
the unique unaliased MAP entry to one complete21-byte executable contribution
in the same PDB object/module as its already bound parent. It then verifies
both native and target images:

- the handler loads a readable immutable FuncInfo and jumps to the independently
  bound `___CxxFrameHandler` role (target46276D);
- FuncInfo has magic19930520, one unwind state, no try/IP maps and the supported
  terminal fields; its immutable map transitions to state-1;
- the sole action immediately precedes the handler, loads saved storage atEBP-10,
  calls scalar delete (target4524A1), balances the argument and returns.

The target handler's FuncInfo47169C points through map471694 to action465640.
All bytes of the21-byte protocol and its metadata roles are inspected, but
this grants **address binding only**. No generated child, data-owner, whole-EH
runtime, or executable-product credit is entered in the ledgers. Other handler
shapes fail closed. The full parent comparison still requires all12 fields and
all181 bytes to agree; the new lane supplies no masks or caller bytes.

A target-independent synthetic suite passes its positive case and35 rejection
controls covering opt-in/type/local/addend rules, parent names, aliased MAP
starts, producer/PDB/module/extent errors, mutable regions, missing/ambiguous
runtime symbols, wrong handler targets, FuncInfo/state maps, saved slots and
scalar-delete destinations. Two actual native cold proofs supply independent
integration evidence. The linked-image/static/public/local-field suites pass.

## Cold proof and preserved dependencies

Two independent full18-TU /GL /GS /EHs cold links use the real front-end update
callback and freeze the same53 source/header inputs. Each strict PE/MAP/PDB
comparison passes **759 complete bytes/all17 fields**: new Load181/all12,
existing Destroy18/all2, real sound queue123/all1, repeated-input33, three cursor
owners241 and two screen setters163/all2. Only181 new authored bytes are added.
[Compact proof metadata](native-proof.json) records both runs. The old Destroy
carrier is refreshed from the same current bytes without changing its recipe.
The unresolved-symbol anchor harness is non-runnable; these are bounded exact
functions, not a Windows product or runtime test.

```sh
scripts/repo-python scripts/report-replay-load.py
scripts/repo-python scripts/build-match-unit.py --unit replay-manager-load
scripts/repo-python scripts/compare-linked-function.py --unit replay-manager-load --json
scripts/repo-python scripts/test-linked-allocation-eh.py
scripts/repo-python scripts/test-frontend-replay-save.py --sanitize
```

Private controls reuse source/header- and object-hash-checked supports and
recompile the two affected TUs; they do not count as cold proofs. Direct input
and real sound queue calls change the save updater1364 to1452 bytes. Correcting
the factory protocol yields1444/1460; direct shared cursor-setter syntax is
byte-neutral and restored. No predicate-count, dummy-register, annotation or
flag/partition sweep follows. The draw page remains961/955. Whole owners remain
nonexact with no partial credit.

## FPS source closure and host scope

The reviewed182-byte owner429A70..429B25 now maps to natural `DrawFps()` source.
It gates on the existing game pointer477810 and playback mode1, prints unsigned
FPS byte+1C4 at383/450 using `%3d`, selects red below30 and yellow below50, then
reloads the current ASCII manager for a white reset. Its only observed incoming
code edge is the28-byte draw adapter42A430. The current native body172/182 stays
nonexact; that adapter and its fastcall chain contract still need source recovery.
No unused ECX argument is copied from the hypothesis reference. The current
Replay-tagged ledger now has all10 authored rows/5586 bytes source-present;
this does not close untagged inventory, indeterminate adapters or the product.

Host tests now extract the actual factory/FPS bodies and actual sound queue
alongside the previous save/cursor/input/delete bodies. The queue's entry
observation is explicit; real typed queue objects, metadata, sample counts and
sample0 are checked. The factory's lower reader is an I/O role checking mode2,
nonzero failure cleanup and preservation of the active global. FPS tests cover
mode/game gates, byte boundaries, format/position, colors and post-format global
replacement. The positive suite and14 strict named mutants pass ASan/UBSan/leak
detection. Layout/prefill/int32-time, SDK/renderer/I/O/allocation/transition roles
retain their stated host limits. No native lifetime, x87 or target runtime claim
follows from them. SDK imports use the existing Main.hpp declaration shield.

MIT hypothesis material remains pinned at
`c6791d82dc9f02e55eca45a5314f900a85481efd`, with
[its license retained](../../licenses/th10-decomp-MIT.txt). Its code placement,
forced attributes, dummy ABI parameters and symbol-number tuning are not adopted.
Pre-existing D3D evidence is untouched. The known401100 boundary-evidence metadata
mismatch is unchanged. Native product/runtime, dual-Oracle semantics and port
gates remain open.
