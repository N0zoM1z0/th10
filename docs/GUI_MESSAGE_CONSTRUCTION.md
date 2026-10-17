# Message construction and canonical GUI owner

The original local source/host phase performed no native build or remote edit.
The subsequent isolated native diagnostic is recorded below; no target execution
or new exactness credit is claimed. The source baseline is clean checkpoint
`24039fa95a52fa2668294e81cbad67cb10ad131d`, after approved laser-binding and GUI
lifecycle/draw work. Target SHA256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Raw baseline/proposal hashes and newline counts are in the local handoff manifest,
which is not a repository patch file.
Raw proposed files preserve baseline newline styles; new GuiMessage.cpp is CRLF.
Repository .gitattributes enforces CRLF for *.cpp/*.hpp when Git applies changes.
Therefore integration normalizes the two existing LF source files and two new LF
fixtures. That local manifest records raw proposal and expected Git-applied hashes; the
attribute-aware local patch check verifies that distinction. This is a newline
transformation, not a source-semantic or test-body change.

This supersedes the earlier free-interface proposal, whose historical patch
SHA256 is `92ffbccde2db7a8428d96795d977d9925be61d427a9b362f1d167f5bf37edcf6`.
That packet remains preserved separately and must not be integrated first.

## Real source and ownership

Gui.hpp declares `explicit GuiMessageVmView(const unsigned char *record)`;
GuiMessage.cpp defines its full target 415B00–415DAA body (683 bytes) and real
EnemyKillAll target40E6A0–40E755 helper (182 bytes). Existing actual members
construct once before the body clears the message. No nested placement new,
free construction wrapper, extra helper, fake layout, production default
constructor, private ABI, register forcing, volatile or flag sweep is added.
The lexical saved-next laser loop remains within the constructor. Existing
canonical laser declarations and dependencies are unchanged.

The real maintained ECL caller EnemyReadDialog now accepts GuiView*, allocates
with ordinary new and keeps record-table offset evaluation in the constructor
argument expression. It publishes message, writes messageIndex, stores the
next dialog index and then conditionally clears dialog state, matching target
store order. The inherited noinline annotation was removed from this changed
caller: clean-source config/scripts contain no retention dependency. Missing
native outlining must be reported, not repaired with attributes.

The target caller is independently 415DB0–415E2B, 124 bytes. Handler46582B loads
FuncInfo47180C (magic19930520, one unwind state, map471804). Map entry is
{-1,465820}; cleanup465820 loads saved allocation[ebp-10], calls4524A1, and
returns. 4524A1 jumps to deallocation452422. There is no whole-message destructor
or VM rollback. Allocation occurs at state-1; state0 begins after the allocation
and before record loads and remains active through publication/global stores.
Ordinary new provides storage cleanup when construction throws and does not call
the incomplete message destructor. Completed member destruction is trivial in
these actual types. Existing natural message destructor handles completed owners.

Allocation success, valid table/record/managers, successful VM allocations and
lookups remain normal-path preconditions. Null/crash scenarios are not tests.
Synchronous throwing dependency tests exercise C++ ownership with explicitly
seamed timers; they do not establish target Windows SEH equivalence. Native
argument ordering, null branch, EH state scope and funclet must be inspected
under the agreed profile. No arbitrary access-violation equivalence is claimed.

## Canonical GUI pointer and fields

All ten legacy alias operands across seven semantic sites use existing g_GuiView
and full fields. Three incompatible EnemyVisualStateView definitions, the callback
partial countdown view, its obsolete assert, three extern aliases and the unused
Enemy.hpp forward declaration are removed. No other global aliases or layouts
are changed.

- Marker: read arg2, float arg1 into existing rounded float temporary, snapshot
  maxLife, read arg0; write unsigned color9E98 before float value9E94.
- Set life: preserve runtime life/maxLife stores, clear all four float/color pairs
  in the same eight-word order, then preserve runtime13C8 copy from life.
- Read dialog: canonical typed GUI owner and real new-expression as above.
- Wait dialog: message9EB8 and signed inputCooldown+78 retain short-circuit test.
- Spell-timeout opcode: signed bossGaugeSegmentCount9E90, not spellSeconds.
- Damage flash: unsigned bossPositionVm.flags35C at GUI9DA4, same bit mask.
- Callback countdown: signed spellSeconds9EC0, integer calculation and maximum99
  only; negative results are not clamped away.

ECL keeps Main.hpp first before ANM/Windows dependencies. Enemy.cpp and
EnemyCallback.cpp add Gui.hpp to already portable view-header dependencies.

## Constructor-body semantics

A bounded source audit found that AnmVmTimerView and PlayerTimerView methods, and the named timer.flags consumers in Enemy.cpp, Player.cpp, and ItemManager.cpp, only test or set initialization bit 0. The audited constructor target likewise clears bit 0. A legal single-bit bitfield assignment is therefore a separate representation hypothesis consistent with these observations: a machine read-modify-write need not imply a source-level read of unrelated indeterminate bits. This is not a complete proof of all raw-offset timer consumers or of the original declaration. No union/bitfield redesign is included.

ANM sequence reads actual AsciiManagerView::textAnm twice. It caches manager before allocation 1, reloads after InitializeVm 1 before insertion, retains that manager for allocation 2, reloads after InitializeVm 2 before insertion, then retains it for four separate FindVm calls. Failed lookups still clear the relevant ID before the target's invalid dereference; failure scenarios are outside the valid domain and are never executed by tests. Layer 15, flag40000000, scripts0/1 and four independent glyph writes are preserved.

Exactly bullet indices0–1999 are visited; the real array has2001 records. States0/3 skip the real cancellation dependency; other states call it, with its existing1/2-only cancellation behavior retained. The complete lexical laser loop saves next before virtual slot5 ApplyClear(0), skips state1 and uses canonical laser owner.

EnemyKillAll saves next before effects, selects (flags&C050)==0 OR flags&80, creates script>=0 effects using GLOBAL manager resources, then ORs20000. Argument-manager list and global-resource owner are deliberately distinct. The argument timer always advances, including empty lists: current copies to previous, strict0.99<scale<1.01 increments integer and float independently, otherwise actual PlayerTimerView::Add(scale) performs the conversion. NaN-to-int behavior is outside finite/in-range test-domain evidence; no substituted exact-equality shortcut.

## Inherited timer representation qualification

The actual maintained raw-integer timer constructor is
`AnmVmTimerView() { flags &= ~1u; }`. New subobjects read indeterminate flags in
that maintained model; seeding backing storage does not establish a defined
initialized subobject value. Member construction here leaves that representation
unchanged. The retained minimal GCC14.2 -std=c++17 -O1 -Wall -Wextra probe warns
of uninitialized flags and fails CHECK timer.constructor_flags with exit1.
Its source is scripts/fixtures/gui-message-timer-construction-diagnostic.cpp;
raw probe/log are retained as local handoff evidence outside this repository patch. This is a diagnostic failure of
inherited construction coverage, never a strict mutation kill or proof for all
compilers. No -fno-lifetime-dse or opaque/volatile workaround is used.

Temporary host headers explicitly replace exactly three constructors with
flags(0): AnmVmTimerView, PlayerTimerView, EnemyInterpolationTimerView. Fields and
actual SetCurrent/Add/Tick implementations stay intact. Target-only layout
assertions are omitted and counted; host results do not prove target ABI/layout.
The bitfield hypothesis above and pinned reference SlotTimer audit do not resolve
this maintained-model issue or prove original game-source UB. No timer rewrite
is included.

## Host evidence and seams

The constructor suite extracts the real member body, real EnemyKillAll and typed
EnemyReadDialog. Its external dependencies are explicit test seams. Successful
lookup/allocation domains, safe distinct owners, callback mutations and timer
boundaries remain covered. Throw tests are a separate ownership check, retaining
earlier external effects and requiring exactly one allocation release with no
whole-message destructor or publication/global updates.

The lifecycle suite copies actual GuiMessageVmView declaration but explicitly
supplies a test-only record constructor seam. This preserves valid lifetimes
for destructor testing without adding an unsupported production default ctor.
That seam is counted and visible; lifecycle success is not real construction
coverage. Runtime-tail's existing reduced fixture now exposes canonical GUI
bossPositionVm.flags35C. Other independent reduced GUI fixture types do not
justify a production default constructor.

Strict behavioral mutations require successful compilation, exit1, empty stdout
and the single exact assigned CHECK stderr line. Compiler failures, signals,
crashes and unexpected checks are failures, not successful kills. Static source
controls are separately labeled. Final exact commands/results and independent
review disposition are in GUI_MESSAGE_CONSTRUCTION_REVIEW.md.

## Explicit host dependency seams

Out-of-line lifecycle stubs:

- AnmVmView constructor/destructor
- AnmLoadedView constructor
- AnmRenderManagerView constructor/destructor
- AsciiManagerView constructor/destructor and GetSize
- GuiView destructor
- GuiMessageVmView destructor, which only counts complete-object destructor entry
- EnemyManagerView constructor/destructor
- EnemyEclContextView constructor
- EnemyBulletPatternView constructor
- EnemyEclHostBaseView constructor/destructor and ECL virtual methods
- EnemyFullObjectView constructor/destructor and ECL virtual methods

Additional host seams:

- AllocateVm, InitializeVm, AddVmVariant0, FindVm, EnemyCancelBulletRecord, and CreateVmAtWorldVariant0 record bounded effects and return distinct live objects.
- Known EnemyEffectWaitNode virtual declarations use fixture definitions; HostLaser supplies the exercised ApplyClear override. No target concrete vtable or laser cancellation body is executed.
- Global operator new/delete use malloc/free and observe one message allocation while armed. They do not perform constructor cleanup themselves: the language's ordinary-new exception behavior invokes deallocation. The complete-message destructor stub is not called for an uncompleted object.

Every instantiated GuiView, AnmVmView, and message has a real host C++ lifetime. Live<T> placement-constructs complete declared objects, then destroys and frees them normally; all consumed fields are explicitly initialized after construction. Message objects are constructed with ordinary new. HostDialogFile has live int table entries and a live instruction-record subobject; caller offset reads are aligned and in bounds. No unsafe failed allocation or null FindVm case is run.

## Completed isolated native baseline and remaining gates

The fixed real ECL graph with only the isolated /EHsc delta completed. All three
owners are naturally retained: constructor702/683, helper404/182 and static
caller124/124 bytes, all non-exact. Equal caller length does not hide its hoisted
global load. Target-like storage-only failed-construction cleanup is emitted,
but unresolved runtime anchors, including __except_list used as a nonzero FS
offset, make this a nonrunnable linked-codegen diagnostic. Full bounded review,
artifact hashes and residual differences are in GUI_MESSAGE_NATIVE_BASELINE.md.
No retained member stores, inlining or schedule is manually forced to match.

The tracked scripts/diagnose-gui-message-member-native.py preserves the exact
fixed graph/profile/hash gates and refuses a nonempty output directory. Its
promotion does not authorize a rerun or deletion of retained native artifacts.
config/gui-message-producer-impact.json contains the complete90-unit/41-producer
canonical replay selection. Scoped cold regressions and CI remain the integration
gate; these docs do not pre-claim their success. Canonical profiles are unchanged.

## Host alias qualification

The constructor suite deliberately compiles with -fno-strict-aliasing. Genuine
host object lifetimes do not establish general portable ISO C++ alias legality
for the maintained view casts, including PlayerFloat3 storage passed through an
AnmFloat3View pointer. These are separate claims. Host success is conditional on
the disclosed compiler mode and seams; preserve target pointer identity rather
than replacing those pointers with copies to conceal the unresolved alias model.

## Integration verification (2026-10-17)

The final applied member packet passed the full host contract: 36 strictly
classified behavioral mutants, one separate static ordering control, and 16
synchronous exception cases. Existing lifecycle coverage passed 135 scenarios
and 24 strict controls; runtime-tail tests passed. The retained positive fixture
also passed non-PIE ASan/UBSan with LeakSanitizer enabled on the remote host.
These observations retain all constructor seams, valid-domain and aliasing
limitations above; they are not production runtime or leak-freedom proofs.

The conservative dependency closure replayed 90 existing exact units across
41 cold producer groups, totaling 16,059 bytes, under unchanged canonical
profiles. All passed. The initial group-6 failure is retained in local evidence:
compiler-generated COFF internal label names changed after the header update.
The integer and float lvalue resolvers each retain a complete 80-byte single-
owner contribution. All five DIR32 targets remain in the same owner at offsets
64,21,30,39,48 with zero addends; independent full-byte comparison establishes
unchanged target identity. Exactly ten symbol names in the manifest were
updated, with relocation offsets/types/targets, extents, source, compiler flags
and comparator unchanged. Strict cold replay then passed all nine group-6 units.
This preserves existing credit and adds none.

Canonical manifest after this label-only migration:
2a0a98cb1963bf6d8daea859a8d4f06b1e87badfa64ccb166ed59ef3bae1beed.
The compact producer-selection JSON records both old/new manifest provenance.
Groups 39--41 returned transport timeouts; their complete durable exact reports
were recovered before proceeding, without duplicate builds. Generated build
reports, logs, executables and PDBs remain untracked reproducible local evidence.

Full target-independent CI passed with scripts/repo-python scripts/ci.py at
2026-10-17 05:20 UTC. An initial system-python invocation correctly rejected
Capstone4.0.2 against the5.0.6 lock; that log is retained, and the successful
run used the repository-pinned interpreter without changing any test or lock.
