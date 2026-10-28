# Enemy item-drop wrapper and scatter core

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `7bd9e60`. Reviewed on 2026-10-21.

The already reviewed authored owners `EnemyDropItemCounts40C9A0..40C9CF`
and `EnemyDropItemCountsCore40C9D0..40CBC6` reproduce their complete
**48 and 503 bytes, all 3 and 27 link fields**. No boundary, origin,
source mapping or authored denominator is expanded. The original three large
owners remain nonexact; product, runtime, semantic and port gates remain open.

## Source and target observations

Direct local Ghidra review uses readOnly/noanalysis, rollback, one CPU and
768 MiB. Disk identity and complete inclusive mapped text401000..465A80
agree before/after:412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The wrapper has one direct call from the dispatcher; the core has calls from
the wrapper and EnemyFinalizeDeath. Existing ledger boundaries stay intact.

The target loads the real ItemManager pointer477818 and calls Spawn41BB00
with its private EAX receiver, ECX position and four stack arguments/RET16.
The old receiverless free declaration instead supplied five stack arguments.
All four genuine maintained callers now call `g_ItemManager->Spawn` through
the real declaration, with ItemManager.cpp present in the ten-TU link graph.
Spawn itself remains795/819 in that graph and receives no exact credit.

Two old manual unsigned-high-bit corrections duplicated the compiler's own
conversion correction. A normal unsigned-to-float cast emits the target's
single correction. The radius scale is U32 * 2^-33 + 0.5; angle jitter is
(U32 * 2^-31 - 1) * pi/4. A three-float vector stores the offset and then the
final position in place, reading input height before scaling. The existing
WrapAngle call stays outlined. Initial angle precedes the count loop; direct
block indexing removes a cached counts-pointer spill. Eleven item kinds are
processed, but twelve count words, including the reserved last word, are
cleared. Original names, declarations, annotations and physical TUs remain
unknown. Ordinary stdcall on the core naturally recovers ESI position,
one stack argument and RET4; the static wrapper naturally recovers EAX/EDI.

The MIT reference th10-decomp commit
`c6791d82dc9f02e55eca45a5314f900a85481efd` suggested compatible storage/order.
Its assembly/ABI scaffolding is not adopted. TH10-local bytes, boundaries,
callee ABI and full compiler comparisons establish the accepted facts.
See the retained [license](../../licenses/th10-decomp-MIT.txt).

## Complete verification

Two independent full ten-TU /GL /GS colds through the real dispatcher freeze
the same31 recursive raw inputs. Each strictly replays two new owners551/all30
plus26 prior owners2850/all63: **28 complete owners3401/all93 per image**.
This includes the polar vector, WrapAngle and all four RNG helpers. Canonical
cold2 selects only the new pair and modified WrapAngle: three units645 bytes
in two artifact groups. There is no full historical replay.

The dense503-byte core has no data spans, compare-size extension or skipped
bytes. Every field is independently reviewed. In particular generationCount
at4918B4 is bound to the actual `g_RngView` base4918B0 plus addend4. A neighboring
diagnostic cookie-anchor label happens to share that native address; its name
is not evidence of semantic identity. Float literal bit patterns are separately
checked against target memory. Both cold receipts and all declared fields are
recorded in the [native summary](native-summary.json).

Wrong RNG addend, wrong ItemManager target and a copied-native height
displacement8->4 all reject. Only the copied diagnostic image is mutated;
the original executable never changes. Earlier full native source controls
show why annotations alone and separate position temporaries do not close the
core. Exact source deltas and bounded failure receipts remain in private
working evidence.

The maintained host projection (`e1c4075:scripts/test-enemy-item-scatter.py`, retired host tooling)
passes228 scenarios under address/undefined/leak sanitizers and rejects four
mutants: duplicated radius correction, duplicated angle correction, clearing
eleven words and wrong manager. It extracts the complete core/vector/wrap,
uses scripted RNG and a synthetic Spawn sink, and adapts two float-word reads
and object representation transfers with memcpy. It proves these numeric,
sequence and clear properties under that host model; it does not establish
i386 rounding/ABI or original/native-game runtime agreement.

## Large-owner measurements

The same actual graph measures ANM ExecuteScript9964/9964, ECL Run7020/7020
and Enemy DispatchEclInstruction14468/14416. ANM and Run retain complete
native equality to the prior graph after verified field rewrites. Dispatcher
changes two ordinary bytes; it still has frame2C4, nineteen sentinel compares,
forty target-sized groups out of108 and absolute case-size gap652. The complete
Run diagnostic binds all189 fields/62 local destinations and retains27 ordinary
differences. No large or partial-case exactness is claimed. Old two-TU26 is a
separate historical context, not this current ten-TU measurement.

Fresh target prologue review also corrects a private old assumption: the initial
packet is in ESI in both target and native. Target switch index is ECX; native
uses EDI. Some later branches reload packet opcode after operand-resolution
calls. Caching that opcode is therefore not supported by the target review.
The preceding checkpoint's bounded1B4 shared-tail call-count conclusion still
applies; physical-region call count is not a missing source read.

Two further private ECL thread-expression controls recompile only EclVm.cpp,
reusing nine hash-verified native objects in this same ten-TU order. Both pass
all28 helper owners3401/all93. Naming the resolved thread ID before SpawnThread
moves the outer constant push after ReadIntValue and its CALL field247->245;
the complete7020 contract rejects it as unbound, with no ordinary-byte score.
Only18 raw bytes EF..100 change; all later Run bytes and both other complete
large native owners stay equal. Naming only the unsigned raw ID preserves the
nested call and all three complete native bodies, including all7020 target-replayed
Run bytes;27 differences remain. No maintained source is adopted. Separate
spawn-only and stop-context-only forms remain prepared/unexecuted in that
thread-expression campaign. Older standalone context-pointer controls did run;
see the later [history correction and thread controls](../ecl-thread-register-controls/README.md).
The native
summary records exact source/artifact identities and the unchanged31 current
inputs/27 final artifacts. The same101-check CI result remains applicable to
unchanged execution inputs; updated documentation is checked separately.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-drop-item-counts --unit enemy-drop-item-counts-core \
  --unit enemy-wrap-angle --json
scripts/repo-python scripts/test-enemy-item-scatter.py --mutations
```

Final gates and attributable cleanup are in the
[validation receipt](validation-summary.json). Diagnostic anchor images remain
non-product artifacts.
