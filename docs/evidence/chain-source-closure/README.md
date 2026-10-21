# Chain construction and missing source mappings

Reviewed 2026-10-22 from `d30cbc5`. Four complete TH10 target owners now map to
maintained Chain source. Three insertion/allocation bodies already existed but
were unmapped; the two-sentinel constructor and WinMain's existing creation
helper now have actual source definitions. This is source recovery, with **zero
new exact functions or bytes**. [Evidence](evidence.json) records target facts,
source pins, native diagnostics and all four private source controls.

The original Japanese v1.00a target is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local Ghidra review used read-only/noanalysis/rollback, one CPU and a768 MiB heap.
All412289 mapped text bytes,401000..465A80 inclusive, retained SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`
before and after review. Decompiler names/types remain provisional.

## Complete target observations

| Entry / full bytes | Observed boundary and behavior | Maintained mapping |
| --- | --- | --- |
|449AA0 /59|Straight-line initialization through RET449ADA; object in EAX, returned in EAX; two complete element heads|`th10::Chain::Chain`|
|449AE0 /131|ESI element, EDI signed priority, stack owner, RET4; calc insertion and added-callback result|`Chain::AddToCalcChain`|
|449B70 /131|Same private ABI; draw insertion and added-callback result|`Chain::AddToDrawChain`|
|449ED0 /74|Stack callback, RET4, EAX result; allocate0x24, conditionally construct, then set callbacks and ownership|`Chain::AllocElem`|

WinMain438BA5 pushes **0x48 (72 decimal)** before calling operator new452493;
after its null guard,438BB3 calls449AA0 and the returned owner is published to
491BE4. The complete constructor initializes two **0x24-byte (36 decimal)**
elements at owner+0 and+24h. Each has priority+0, flags+4, three callbacks+8/+C/+10,
self+14, next+18, prev+1C and argument+20. Only ownership bit0 is cleared; other
flag bits survive. Callbacks, priority and next/prev are zero, self points to
the respective element, and both argument words at owner+20h/+44h are untouched.
The previous0x44-byte owner declaration omitted the final argument word.

Insertion invokes an optional added callback using ECX=argument, retains its
result, clears that callback, enters IAT4660B0 with critical section492274,
increments byte49231C, and inserts before the first next signed priority greater
than or equal to the new one. New equal-priority entries therefore precede older
entries. It leaves through IAT4660B4, decrements the byte counter and returns the
callback result. Root links are owner+14h and+38h. The allocator's null branch
still reaches unconditional callback/ownership setters; source recovery does
not invent a safe allocation-failure return.

The operator-new address is independently identified by the existing complete
14-byte canonical library unit at452493 (`libcmt.lib`, `new.obj`). That prior
library proof supplies an address identity and gains no new credit here.

## Source and origin are separate

Actual `ChainElem` construction now initializes the observed fields, with a
named ownership bitfield expressing the partial flag update. `Chain` contains
two actual elements, and its existing intrusive-list operations use their
self/next/prev views. Native MSVC i386 assertions bind0x24 and0x48 sizes.
`MainCreateCallbackOwner` implements the already declared creation expression
as `new th10::Chain`; the Main source and its existing global declaration do
not change. The diagnostic WinMain really allocates0x48 and inlines member
initialization. This helper has no separate target mapping or exact credit.

The outer449AA0 constructor may have been implicitly synthesized or explicitly
declared. Game-specific member initialization and a WinMain caller do not prove
its independent source authorship. Its former authored classification is
corrected to **origin indeterminate**, rather than asserted compiler-generated
or excluded. The other three owners retain authored classification.

Thus four mappings raise total source mappings1088->1092, while authored source
rises605->608. The authored denominator becomes734 functions/273517 bytes,
down one59-byte unknown-origin constructor. Authored source covers246753 bytes;
211 source-present functions remain nonexact and126 still lack source. The
authored exact numerator remains397 functions/72387 bytes. Its percentage change
is denominator correction only. Overall exact remains1238 functions/138897 bytes.
All524 unknown origins keep the whole-game denominator unresolved.

## Native results and controls

The actual-source Main/Chain/GameErrorContext three-TU full cold uses real
WinMain as entry and retains `/GL /GS`. The complete59-byte constructor has
43 raw-equal positions, with16 differing positions from first-head store
scheduling. The allocator remains69/74 bytes. Both insertion bodies reach131
bytes, but their linked fields occur at different offsets from the target.
The strict complete comparator rejects those inventories; masked structural
scores are **not** ordinary-byte scores. None enters the exact ledger.

| Private source control | Constructor size / raw equal positions | Allocator size | Decision |
| --- | --- | --- | --- |
|Move ownership initialization first|59 /43|64|Reject allocation regression|
|Links before callback initialization|64 /14|69|Reject constructor growth|
|Group callback initialization after links|59 /37|69|Reject store scheduling|
|Publish ownership after callback setters|59 /43|69|Adopt source ordering; still nonexact|

Each control recompiles one Chain object and reuses two verified objects;
these are not full cold builds. The selected source is then independently
compiled in the final Main graph and canonical affected-unit cold builds.
The TH09 link-first source and TH095 declarations were hypotheses only:
their0x20-byte elements/0x40-byte owners do not establish TH10 layout.
Import declaration merging and source order are compiler hypotheses, not a
claim about the original production partition. The earlier Anm-primary graph
emits129-byte insertion bodies with direct imports; the Main graph emits131
with IAT fields. The original graphs differ in more than source order.

Four full affected canonical cold groups pass **10 selected units/1784 bytes**:
two lifecycle helpers196, two registration wrappers86, five Enemy helpers1172,
and ASCII initialization330. The two first groups each compile14 TUs, Enemy10,
ASCII3. No full reconstruction or all-consumer replay is claimed. The Enemy
image also passes35 complete guards4424/all118 fields. The unchanged maintained
Run diagnostic repeats7020/6993,27 ordinary differences, all189 fields/62 local
destinations and the complete171-byte float callee exact.

[Current native pins](native-summary.json) bind the new32 source/header inputs
and15 artifacts. Only Chain.hpp differs among these inputs from the preceding
raw-NEG context. All three complete large native bodies remain raw-identical:
ANM9964, Run7020 and Enemy14352. Older raw-NEG/thread receipts are historical
source/artifact contexts, even though these whole native bodies are unchanged.

## Host scope and remaining work

The strict transition runner refreshes exactly two of seven active LF dependency
pins, Chain.cpp/Chain.hpp. CRLF-to-LF-only normalization and exact-one rewrite
checks remain strict. The actual constructors run on A5/5A-prefilled heads;
16 checks observe flags, callbacks, links, priority and retained argument bytes.
In-place destruction/reconstruction avoids copying stale self pointers from a
temporary. The old843 assertions remain, with17 additional checks, for860.
The sanitized baseline and all19 EOL guards pass. Hint and GameManager teardown
fixtures also compile the actual source-extracted constructors; their baseline
and70 teardown cases pass with sanitizers. Full CI initially stopped at group82:
the Ending fixture copied Chain declarations without either constructor
definition. Its builder now extracts the same actual constructor bodies and
records Chain.cpp as an input. This preserves the existing explicit node-prefill
seam and functional assertions; the repaired sanitized baseline passes. These
are host contracts, not native
layout/private ABI, concurrent scheduler execution or game runtime closure.

`g_MainCallbackOwner` and `th10::g_Chain` both denote target491BE4, but source
storage ownership and the physical alias remain unresolved. No duplicate
definition is introduced. RunCalc449C00/273, RunDraw449D40/241 and other still
unmapped Chain behavior remain open; the module is not source-complete.
Original type names, physical TUs, runtime allocator semantics and whole Windows
i386 product closure also remain open. Semantic reconstruction and porting have
not started. [Validation](validation-summary.json) records final CI and cleanup.

After that fixture repair, one complete fresh CI run passes107 groups with all367
maintained source/test/config raw inputs frozen. Classified cleanup removes7751908
logical file bytes from four superseded native trees, three host review trees and
four CI trees. Current canonical outputs, final Main carrier, prior ASCII baseline,
producer-dependent natural Main carrier, one failed control and compact evidence
remain. The nine independent untracked D3D files are unchanged.

Private recovery: `.analysis/gpt-6.1-sol/chain-source-closure-20261022/RECOVERY.md`.
