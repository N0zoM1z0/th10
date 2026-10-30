# Complete Player drawing callback exact — 2026-10-30

`PlayerDrawCallbackBody` at426360..4264F8 is complete exact:409 bytes,
98 instructions, all23 address fields,11 fixed local branches and plain RET.
Its registered ECX-to-EAX tail adapter426510..426516 is independently exact:
7 bytes and its sole REL32 field. The adapter retains indeterminate origin;
only409 newly exact bytes count as authored. No boundary or origin changes.

## Complete source and target scope

The previous source combined the callback and body, emitting415 bytes in the
current seventeen-input graph. Separating the ordinary body from its real
fastcall entry naturally recovers private EAX Player and the seven-byte adapter.
The target body has eight preceding and seven following CC bytes; the adapter
has nine on either side. Whole-text encoded E8/E9 scanning finds one transfer
into the body, the adapter's entry JMP, and none into either interior. The
adapter address is registered by the immediate operand at42487F.

A raw unaligned value at470BA9 resembles interior address426400. It overlaps
the float57 literal470BA8/word42640000; it is retained as an uncorroborated raw
value, not evidence for a pointer, caller or separate owner.

The maintained body skips state2, positions its VM, draws it through the real
ANM manager, invokes up to four option callbacks, and conditionally draws a
two-pass auxiliary rectangle. The actual VM stores are at VM+340/+344/+348,
the maintained `positionOffset` field, hence Player+354/+358/+35C. A small
ordinary Float3 destination helper restores their x87 schedule. The gate tests
bit7 of the byte at+54; its original signedness/name remain unknown.

The rectangle captures its integer width with the first coordinate calculation,
before Floor, then uses that value for the right edge. This restores the last
stack-store schedule. Bounds use X+225-16 and Y+17-24, width factor3E95A95B and
height2. The second pass subtracts1 in left/right/top/bottom order. All five
direct calls, the option callback, all gates, literals and exits belong to the
complete comparison. The frame14 contains the real16-byte rectangle and4-byte
width value; no inert storage, forced inline, fake ABI or new flags are used.

`PlayerDrawManagedVm` and `PlayerDrawAuxiliaryRectangle` declarations are removed
in favor of the genuine `AnmRenderManagerView::Draw` and `DrawColoredRectangle`.
Actual interfaces are manager ECX/VM EAX/plain RET, rectangle EDI/color EBX/plain
RET, and Floor stack float/ST0 double/RET4. The first two new canonical units
share only Player.cpp, AnmManager.cpp and AnmRectangle.cpp, with the existing
plain /GL and ANM /GL /GS profiles. The real registered callback is the root.

The rectangle callee remains407/483 NONEXACT and gains no credit. Its actual
incoming ABI agrees; Draw216 and Floor21 independently pass complete strict
checks in the new three-input image. Original helper boundaries, class/type/TU
and source-written versus compiler-emitted adapter ownership remain unknown.

## Cold reproduction and scoped guards

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-draw-callback-body --unit player-draw-callback --unit anm-render-draw --unit player-floor --json
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-draw-callback-body --unit player-draw-callback --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --json
```

Two independent zero-reuse cold builds pass409/all23 and7/all1. The first
explicitly replays Draw216 and Floor21:653 bytes/all46 fields. The second adds
only the two affected existing damage units1170+202:1788/all58. No historical
cohort replay is required. Native jobs are serial, CPU0, nice10 and bounded;
source and manifest hashes are frozen during each job.

The full seventeen-input Player cold preserves the current3108-byte main,
624-byte hit and1616-byte movement receipts. Smaller drawing graphs are scoped
recipes, not replacements for those parents' real contexts. The parent main,
movement, hit, state2, script initializer and rectangle remain NONEXACT.

The first complete genuine body emits407 bytes. Width capture restores409;
the destination helper, correct VM field, bit gate and capture timing close it.
Whole/horizontal bounds helpers outline and are restored; a coordinate wrapper
and assigning the width in the gate are neutral and restored. Rejected private
artifacts are archived after hash verification. `results.json` retains compact
target, field, ABI, source-pin, cold and control evidence; private code/artifacts
stay in `.analysis/` and `build/`.

New codegen credit is416 bytes/two owners, of which409/one owner is authored.
Authored exact is101049/275817 across494 owners; total codegen169153/1371.
Source mappings1207/authored689/backlog195. Product/runtime validation, original
data/TU ownership and portability remain separate and open.
