# Fresh v2 independent host methodology review

Historical test record: the construction/lifecycle host suites cited here were
retired on 2026-10-28. Commands for deleted scripts apply to commit `47d9993`,
not the current tree. Their results are historical evidence, not requirements
for exact reconstruction; follow [the current workflow](RE_WORKFLOW.md).

Final evidence-qualified disposition: no blocking type, lifetime, domain, extraction, or strict-classifier issue found in inspected exercised paths. Independently checked all seven fresh manifests against current source-input hashes and raw run logs, the packaged broad driver/fixture/catalog copies, all four signed production hashes, and all 28 exact mutation transforms. No old-v1 results are used. This reviewer did not build or run tests.

Fresh evidence confirms 13 broad positive selections both unsanitized and ASan+UBSan, 28 strict unsanitized mutants, and a packaged unsanitized 13-selection smoke run. Both baseline modes show the control passing and the three assigned source failures. Both lifecycle modes show the exact positive success line and the strict sixth-deletion omission failure. Detailed independently checked manifest hashes are in GUI_MESSAGE_RUN_TEST_REVIEW_HASHES.json. These are scoped host semantic results under the seams below, not native acceptance or exactness.

The broad driver extracts complete Run/local helpers and selected actual canonical dependency bodies. It recursively retains canonical fields, with target-layout assertions omitted and three inventoried timer constructor substitutions. Scalar-width assertions and runtime endian checks remain. Explicit out-of-line message/dependency constructors/destructors and external seams are disclosed; this is post-construction execution, not real message-constructor coverage. Static Player zero-initialization precedes unchanged raw-flag member constructors. Sound storage is aligned and placement-constructed as the actual canonical owner before QueueSoundSample executes.

Profile scrutiny requested by parent: fixture profile and swapped profile are live0x20000 unsigned-char vectors; count initialization, inspection and expected-state writes use memcpy with live int locals. All exercised offsets remain inside storage, including difficulty-1/5 cases. The full-vector sentinel comparison checks that only expected counter/unlock bytes change. There are no counter reinterpret-cast int lvalues or invented typed arrays. This validates bounded byte-storage access, not the production physical alias between g_GuiPersistentData and canonical47783C, nor a host GameStatisticsOwner layout. That inherited alias remains open.

Instruction programs are built completely before publication of their byte cursor, so vector reallocation does not invalidate the active cursor. Mutating callbacks modify only live record bytes and valid live resources/owners, without resizing buffers. Odd-offset reads use production memcpy helpers. Score inputs inspected avoid overflow. Text payloads are bounded plain literals for the explicit decode seam; production decoding is not tested.

The stage-advance seam records the actual g_GameStageState byte-array address and published mode. The driver checks the source's canonical declaration/call. It does not execute or establish leaf4175E0 stage increment, cap, descriptor publication, raw globals or private ABI. This is correctly distinguished from decode evidence.

The separate lifecycle fixture retains actual constructor/factory, full Run, destructor and selected real dependencies, under timer/resource/SDK seams. Its file contains a real aligned int offset table and a live byte-record subarray. Actual WAIT/END run; two text IDs are constructor outputs and four other IDs are explicitly seeded. A forwarding linker wrapper observes deletion calls and uses a second live manager for a labeled synthetic cached-receiver check. Ordinary new/delete observation does not replace constructor exception cleanup. Owner unpublication remains explicit harness action, not production updater coverage.

Every mutant must compile and then return1 with empty stdout and exactly its assigned CHECK plus newline. Compile errors, signals, unexpected output, incorrect checks and timeouts cannot pass; classifier rejection checks are present. Positive cases require return0 and empty outputs (lifecycle has its separately exact success line). Final evidence must distinguish sanitized positive runs from unsanitized mutant campaigns.

Remaining exclusions: malformed/truncated input, stale text/invalid portrait-parent ownership, arithmetic overflow, ordinary-game reachability of unusual scalar cases, true decoder/advance and unresolved external owners, private ABI/native layout, exactness, leak detection and unchanged raw-integer constructor proof. Cached side-position manager/live activeSide and zero-ID lookup differences remain source-only where real dependencies provide no observation seam.

## Methodology snapshot SHA256

- repaired-tests/test-gui-message-run.py: a498c6c233f209b9aae3cd586f78975b8e8319c255130126e6bd3c7f77a45297
- repaired-tests/gui-message-run.cpp: 7cad58f93d856b2fa005f98a1920d3170bc577156e700ad832b08e8428ec0f43
- repaired-tests/gui_message_run_mutations.py: b405139301bb2f8bc4a8a5a44c43a55dc7221db6b696e94df0d6338f8dc44659
- scripts/test-gui-message-lifecycle.py: 4372e0ef5f9d25efd552e6560d04cd43d033827d12549df60884fc78adab51ee
- scripts/fixtures/gui-message-lifecycle.cpp: 6fd605061b3e4a3ab7cc497053e294f15a61a32234d66a1c4ed433e68b779d48
