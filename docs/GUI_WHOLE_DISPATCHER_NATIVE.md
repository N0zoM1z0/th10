# Whole-dispatcher fixed native observation

## Result and scope

One fixed ten-TU `/GL` diagnostic links successfully. The naturally emitted Run owner is **4,136 bytes**, versus the target's 4,480. Its actual caller now pushes the message pointer and both Run exits use `RET 4`; the previous EAX/plain-RET boundary mismatch is gone in this observation. This is a code-generation observation, not whole-owner equivalence, canonical byte credit, or permission to execute the diagnostic image.

The baseline is `a7437343002854cb4d5df39a373124185e3046b1`. The six-file packet is `fa4fc3b2555fc13185146573258a301e52f2de51f15ef2206aa7da006741ef4b`; only Gui.cpp changes production code. Source/test compatibility review preserves 13 interpreter selections, 28 strict interpreter mutants, lifecycle deletion checks, 32 transition mutants, 12 classifier controls and sanitized baselines. Host-region checks do not establish native callback behavior.

The exact executed driver and input map are maintained under `scripts/diagnostics/gui-whole-dispatcher/`. There are 44 native pins and six separate candidate-file pins; the graph, profile and 33 required-real fragments are unchanged from the prior transition observation. The canonical match manifest remains the sole settings authority. The conservative current-transition-to-lexical-Gui impact is six linked-PE producers/seven units; the accompanying impact file is an inventory, not a replay receipt.

## Durable identities

- Driver SHA-256: `2c6ed46cd5929f8acc9f21ad9ce772860e0712fcbf4b595784f56dbc46df8a07`
- Executed map SHA-256: `e8b5ef37fdeea0c601a3ff78bc9b49b0da1c2a87abeaa61435c24056ad91ebab`
- Build report SHA-256: `4b7ffb825a9615c7e9d6f8cde777283b480df045207f7dba787c3e4c72b0d28c`
- PE SHA-256: `d5c04342c9af5456e0963e5c63076510f1b5ff009c9a3ec5568b731341524bfc`
- MAP SHA-256: `df03634cd772f0d56e0825b3d38ba18a279f2e7c6666ceaa1b8331c3c5449380`
- PDB SHA-256: `d399ba1b2bd0678c6285488e36b3fa80b92415421ddb964873b864423a3aee49`; GUID `898a160dd9a6ee419f648a2c77522ecc`, age 1

Parent preflight verified all 44+6 raw inputs in command `f1940fa0e8384b90979ac0bc03b353c9`. Build transport timed out; recovery `a5ba00ee1cda4668af0f1587a1652459` found durable exit 0 and no rebuild was made. Read-only collection `7ad92b26d8d342f7bf89f9348aacf955` verified the exact report, source pins and PE/MAP/PDB identity; `efd749c800bb49938a187696e1f0da15` independently recorded all ten actual object hashes and rechecked 44 native inputs. Full raw bytes for all 1,046 executable PDB contributions, including 67 without public function ownership, were retained externally for review. Binary evidence and logs are not committed.

## Reproduction

Use a checkout containing the exact pinned six-file candidate and native inputs. The driver is unchanged from the executed copy. From repository root:

```sh
PYTHONDONTWRITEBYTECODE=1 scripts/repo-python -B scripts/diagnostics/gui-whole-dispatcher/diagnose-whole-dispatcher-native.py --root . --check-only
PYTHONDONTWRITEBYTECODE=1 nice -n 10 scripts/repo-python -B scripts/diagnostics/gui-whole-dispatcher/diagnose-whole-dispatcher-native.py --root .
```

The output `build/diagnostic-gui-whole-dispatcher-a74373430028-fa4fc3b2555f` must be absent or empty even for check-only. Preserve any existing observation and use a fresh worktree rather than deleting evidence or changing the executed map. Check-only performs no build. Build serially, with no other native job; ambient CL/_CL_/LINK/_LINK_ flags are rejected. Never execute the PE. After transport timeout, inspect the durable report before considering another action. No synthetic root, forced noinline/private-ABI correction, profile/TU sweep or numerical tuning is part of this reproduction.

Test/docs/ledger-only edits do not change native inputs, but edits to any of the six candidate-pinned paths intentionally fail preflight. Such a later checkout needs an explicitly reviewed, separately named reproduction map; never rewrite the executed driver/map or silently discard its candidate gates.

## Complete owner and boundary findings

Run occupies `[0x401120,0x402148)`: 3,961 executable bytes (1,061 instructions), three alignment bytes `8D4900`, then 172 selector bytes. Tables at `0x40209C/0x4020FC/0x402118/0x402134` have 24/7/7/5 entries. Every one of the 43 destination words reaches an owned instruction start and has HIGHLOW relocation. The other 123 HIGHLOWs are complete instruction address fields, giving 166 total. Target `[0x415E90,0x417010)` is 4,306 executable bytes (1,146 instructions), two alignment bytes and the same 172-byte table cardinality. Complete physical-region accounting yields −345 executable bytes, +1 padding byte and −344 total bytes. The independent next owner at `0x417010` is not included.

The emitted differences extend beyond relocations: frame `0x88` versus target `0xD8`, merged identical selector arms and shared tails, and 111 call sites versus target 150. The four source switch domains naturally produce four tables, but cardinality alone proves neither branch-body equivalence nor matching code. Independent GNU objdump decoding agrees with all 13 selected/target executable spans; owned data was excluded from decoding and reviewed separately. No removed-helper byte sum is credited to Run.

Updater `[0x4021C0,0x40307C)` remains 3,772 bytes: 3,741 executable, three padding and 28 selector bytes at `0x403060`; all seven destinations and 50 HIGHLOW fields are accounted for. `PUSH ESI` at `0x402A1C` precedes `CALL 0x401120` at `0x402A1D`. Run reads the stack argument and exits with `RET4` at `0x402070/0x402096`. This directly observed boundary agrees with the target's stack/RET4 boundary without ABI tuning.

Both completion routes now reside directly inside Run: EDI loads at `0x401D38/0x401F5E`, calls to transition `0x41F2A0` at `0x401D3E/0x401F64`. The 413-byte transition consumes EDI and plain-returns, while the target owner is 411 bytes. Transition, 78-byte audio (target 84), capture, music load/play and queue wrapper retain their previous instruction shapes with address-field changes only. AddVmVariant1 and QueueCommand are byte-identical to the prior diagnostic. This comparison does not establish equality of pointed-to data: current constant/string names were identified by map symbols, not reverified data-section bytes.

All 33 required-real fragments have unique executable same-start map/PDB owners and are absent from the unresolved list. Real capture uses ESI receiver, ECX ID, four stack dimensions and RET16; real music-load uses EAX filename, one stack argument and RET4; real queue uses EAX object, EDX string, two stack integers and RET8. These private protocols remain independent from target helper-body equivalence and physical object/storage legality.

## Remaining numerical and runtime gates

Audio still loads its argument earlier and uses an additional x87 stack depth of three versus target two. Its `TEST AH,41`/JP at `0x41D5AA` differs from target JE at `0x420C60`. The status predicate differs for mask 41; target's intervening FLD and exception/control state prevent an unconditional NaN-execution claim. Candidate FDIV stays on x87 without a binary64 spill, narrowing that specific emitted concern only. Conversion remains anchored: call `0x41D5B2 -> 0x42A764` (__ftol2), versus target helper `0x463B2C` whose body/control contract remains unproved.

Retain the finite source-level counterexample: magnitude bits `0x4EFDFFFF`, scale bits `0x3F7FFFFF`, exact quotient `2130706431 - 1/16777215`. Binary64 can round to 2130706431 while extended truncation yields 2130706430. No expression or compiler-profile tuning was used to suppress it; active precision/rounding control, conversion, load/status sequence and finite/unordered equivalence remain independent gates.

There remain 441 unresolved anchors. Stage-six effect calls anchor `0x42A3D4` at `0x401E8A`; its real body/private target ABI is still open. Allocation/CRT/OS/renderer/audio dependencies, anchored globals, aliases, physical storage extent, object lifetime and actual execution are not closed by this diagnostic. Source, host fixtures, native retention and canonical regression receipts each have separate authority.

## Source reloads versus actual emitted ordering

Native code preserves player second-operand loads at `0x401484/0x4014D7`; each of eight enemy stage/subcase paths reloads resource and cursor/operand before its second lookup. Alternating-zero text reloads manager at `0x401A93` after decoder call `0x401A88`. The effect return is followed by fresh GUI/profile inputs. These are observed instructions, not inferred callback guarantees from host mocks.

Other source-level reload expressions are optimized: target unlock stores re-read difficulty/shot/stage/character between writes at `0x416C10–0x416C37`, whereas candidate computes both addresses before stores `0x401D1E/0x401D26`. Candidate publishes the ending flag at `0x401EA2` before counter reset `0x401EAE`, opposite target `0x416D95/0x416DA2`. These scheduling differences block exact ordering claims; neither alone proves a semantic failure under ordinary disjoint nonvolatile storage. Alias legality, synchronization assumptions and target storage still need independent evidence.

Character/guard differences remain concrete: opcode 2 folds six target creation calls into one candidate call, validates character 0/1 early and adds a null-resource branch. Opcode 13 keeps the seven-stage table but merges duplicated character-0/1 bodies (candidate guards `0x401520` TEST/JL and `0x401528` CMP 1/JG); its physical span is 1,162 versus target 1,535 bytes and has 32 versus 64 calls. Opcode 19 retains source/resource null guards absent from the displayed target routes and grows 272 to 366 bytes. Opcode 21 passes its float through a stack local rather than target direct constant pushes. These are genuine whole-owner differences, not relocation noise.

The ten artificial source-static wrappers were removed; the former completion helper has no map entry and its two calls now belong to Run. Twenty genuine Gui executable contributions remain, including ordinary accessors and other owners. All 111 Run direct calls resolve to 23 real unique callees plus the one effect anchor. Source helper removal is not a claim that all helpers disappeared or that their bytes can be added to the Run owner.

Independent reload/provenance checks verify exact character 0/1 domains at opcode-1 guard `0x4011BA`, opcode-2 guard `0x4011F9`, opcode-12 guard `0x40144B` and all eight opcode-13 paths. All ten removed GUI wrapper names are absent from Gui.cpp and source.ltcg.obj map entries; the distinct FrontEnd InterruptVm owner remains. all twenty source-module contributions are named, with no hidden outgoing Run helper call in the collected ledger. Each of the 33 required-real owners positively matches map/PDB/raw bytes and the recorded ten-object fingerprints; local source bytes match the 44 native pins. These checks establish provenance and observed access sequences, not transitive runtime safety.

## Checkpoint regression outcome

The parent cold-replayed all six affected producers/seven canonical exact units;
all passed with unchanged canonical settings. Full pinned target-independent CI
passed after the lexical source and maintained suites were integrated, retaining
970 mappings and1126 configured exact units. The later native documentation/scripts
passed documentation hygiene and whitespace checks. No new exact units or bytes
are claimed. All44 native and six candidate hashes were rechecked before commit.

The remote host campaign passed13 interpreter selections and28 strict mutations,
13 ASan/UBSan positives, the actual constructor/factory-to-WAIT-to-END-to-six-ID
destruction integration with its strict deletion control, and32 transition mutants
with12 classifier rejection controls plus a separate transition UBSan baseline.
Generated artifacts remain ignored; maintained source, tests and concise evidence
are sufficient to reconstruct the observations.

## Opcode 19 source correction (2026-10-17; native verification pending)

This addendum records a later source correction; the historical observations above remain unchanged. In the subsequent effect diagnostic, the still-guarded Run owner is 4,128 bytes at `0x401120`, compared with target 4,480 bytes at `0x415E90`. Target opcode 19 spans `[0x416AC3,0x416BD3)`: stage-minus-one is bounded by 6 before any manager access, then seven arms load the manager member at `+0x38`, create with layer 15 and scripts `0x0D,0x10,0x14,0x15,0x0E,0x23,0x1D`, and publish the returned ID at message `+0x54`. The existing candidate span `[0x401B6A,0x401CD8)` instead tests the manager before dispatch and adds resource-null-to-zero alternatives. The retained target/candidate Run raw-byte SHA-256 identities for this observation are, respectively, listed below; these identify the pre-correction evidence, not a new binary.

- Target Run: `13c80eaec4d7e90718b107f1e37ea8de7bd57d8ac3f86f0af4b6269644614c35`
- Pre-correction candidate Run: `50f9dfc6fbcc081a3dcca86dfa85cb70007f164fbd640448a6aa20f08107592c`

The correction removes only that outer manager guard and the seven resource fallback expressions. It retains the seven separate case arms/resource loads, scripts, layer and ID publication; unsupported stages remain no-ops within the opcode. No source bytes outside this opcode change. Source and maintained fixture retain CRLF. No flags, translation-unit composition, ABI, attributes, volatile operations, numerical constants or layouts are tuned.

Supported stages assume a live message, a live enemy manager and a valid `AnmLoadedView` resource object. This is the C++ valid-object precondition used by the host fixture, not proof that every engine state meets it. The target's behavior for a null resource is not established or claimed. No null-member call is executed in any control, including sanitizer runs. Unsupported stages do not access the enemy resource within this opcode.

The maintained Names selection checks all seven scripts, exact resource, layer 15, one creation and returned-ID publication, plus unrelated message IDs, profile/message/GUI fields and VM sentinels. Unsupported cases `INT_MIN,-123,-1,0,8,123,INT_MAX` check no creation and unchanged enemy-name ID. Normal dispatcher advance/tick checks remain in the shared fixture. The separate `test-gui-enemy-name.py` checks source structure, rejects a guard-bearing negative control, and uses the maintained driver/fixture for 11 safe strict mutants: seven script substitutions, wrong layer, wrong resource, wrong publication destination, and unsupported-stage ID overwrite. Valid-owner behavioral tests alone cannot detect removal of redundant guards, hence the separate structural check.

Reproduce from a complete repository checkout containing the correction, using fresh output directories:

```sh
python3 scripts/test-gui-message-run.py --root . --out build/enemy-name-run
python3 scripts/test-gui-message-run.py --root . --sanitize --no-mutations --out build/enemy-name-run-sanitize
python3 scripts/test-gui-message-lifecycle.py --root . --out build/enemy-name-lifecycle
python3 scripts/test-gui-message-lifecycle.py --root . --sanitize --out build/enemy-name-lifecycle-sanitize
python3 scripts/test-result-transition.py --root . --mutations --manifest build/enemy-name-transition.json
python3 scripts/test-gui-enemy-name.py --root . --out build/enemy-name-controls
```

Local partial-snapshot verification used the maintained drivers' existing explicit source fallback to the canonical `50b971f` snapshot; no fallback semantics or canonical headers were changed. For equivalent partial-snapshot reproduction, add the relevant driver's existing `--fallback-root` source-directory option or transition driver's `--baseline-root` repository option. A complete checkout needs neither. Host compiler/SDK/timer/external-call seams remain those documented by the maintained drivers. Normal interpreter: 13 selections and 28 strict mutants passed; ASan/UBSan interpreter: 13 selections passed; lifecycle normal and sanitized: baseline plus one strict control each passed; transition: 32 strict mutants passed; separate Names: baseline plus 11 strict mutants passed. Structural guard rejection passed. Sanitizer drivers retain their documented leak-check limitation.

Raw Gui.cpp SHA-256 changes from `67beb1343d9a190a89328b070de8e1da3ef353122149013c3b6ad2dbcd086c42` to `0264d2a32bab3b5c359d8e9d8aba02f9ff1eaa67e0e0f1cd619f729c9e954ce1`. The source/test-only patch SHA-256 is `12a2d694ed7220ebdb5192d5f0af07e3f298c35f1c83d2b02ffc87433f52fc44`; it is separate from this documentation/control-script addendum.

No native build or target execution was performed for this correction. Host outcomes do not establish emitted code size, private ABI, runtime ownership or native equivalence. A separately reviewed fresh native observation and canonical regression receipts remain pending; preserve the historical driver/map pins rather than rewriting them to bypass preflight.

## Opcode19 correction: shared canonical native observation

The reviewed opcode19 packet changes Gui.cpp to stage-first dispatch with seven direct resource-create/publish arms. Gui.cpp SHA256 is `0264d2a32bab3b5c359d8e9d8aba02f9ff1eaa67e0e0f1cd619f729c9e954ce1`. The parent replay passed **7 producers / 11 existing units / 912 replayed bytes**. This is regression evidence, not new accepted credit. The four historical Factory receipts belong to checkpoint `8a634cb8eb9eb9780e0e744ab62f0196eb74ca59`; their freshness for this changed source has not been established. Fresh-fact and aggregate snapshot queries returned INVALID_ARGUMENT even at that historical checkpoint; no aggregate accepted total is asserted here.

No eighth diagnostic compilation was performed. Full Run evidence comes from the required canonical `build/match-linked/GuiTransitionEffectCallbacks` producer: the same fourteen TUs, actual updater entry, `/GL` profile, link flags and unresolved-data-anchor harness as the preceding effect observation. The separate diagnostic map remains provisional/unexecuted.

### Complete-owner findings

Run `[401120,4020DC)` is **4028 physical bytes = 3855 executable  + 1 NOP  + 172 table bytes**, versus prior 4128 and target 4480. These sizes are not a completion percentage or exact-byte award. All 1076 executable contributions have verified full raw hashes/extents/nonoverlap; GNU objdump independently agrees with all 59 selected/target code spans. The four tables at `402030/402090/4020AC/4020C8` contain 24/7/7/5 entries: all 43 destinations are owned instruction starts. All 163 Run HIGHLOW fields classify as 43 table words and 120 complete instruction address fields.

Opcode19 `[401B6A,401C77)` is 269 bytes versus prior 366 and target 272. Its 75 instructions retain stage 1–7 dispatch, seven real `CreateVmVariant0` calls to 408640, scripts `0D/10/14/15/0E/23/1D`, layer 15 and returned-ID publication at message+54. The unsupported null-success branches are gone. Target/candidate abstract instruction sequences agree after explicit linkage/branch/result-slot interpretation; this is not byte equality, and target behavior for null resources remains unclaimed.

The remaining one executable byte reduction is opcode20 register allocation: old `401CE9: 8B15CCB54200` loaded shot type into EDX, followed by `LEA EAX,[EDX+ECX*2]`; new `401C88: A1CCB54200` loads the same global into EAX, followed by `LEA EAX,[EAX+ECX*2]`. The five-byte accumulator load replaces a six-byte load. Read order is unchanged; EDX is overwritten before subsequent use. Every other non-opcode19 instruction byte agrees after individually mapped address/relative fields, retaining opcode widths, register/stack operands and literals. Total reduction 100 = opcode19−97, opcode20−1, alignment−2.

Updater `[402150,40300C)` remains 3772 bytes. Its PUSH ESI at 4029AC and CALL Run at 4029AD feed Run’s stack receiver; exits 402006/40202C use RET4. Both completion routes load EDI and call the real 413-byte transition at 41F3C0 (calls 401CDA/401EFA). Full-path MAP/PDB/module joins establish all 47 required real owners against 14 actual objects, with no retired interface. All twelve callbacks remain naturally retained; factory/initializer and chain/rectangle/RNG dependencies are real. Factory plain-RET versus target RET20, timer prezero/lifetime, nothrow allocation, x87 precision/conversion/control-word, storage/alias and 440 unresolved-anchor limits remain open. No helper-size sum, forced retention or ABI/profile tuning is used.

### Reproduction and provenance

From the integrated checkout, with pinned repository tools/target and no concurrent native job, replay the seven shared producers serially through maintained tools:

```sh
PYTHONDONTWRITEBYTECODE=1 nice -n 10 scripts/repo-python -B scripts/replay-exact-units.py --json \
 --unit anm-vm-id-set-interrupt --unit gui-set-lives-display-count \
 --unit player-reset-runtime-state --unit gui-score-add \
 --unit anm-vm-id-set-sprite-own --unit anm-vm-id-set-sprite-explicit \
 --unit main-set-next-game-mode --unit transition-draw-screen \
 --unit transition-draw-playfield --unit transition-draw-dim-playfield \
 --unit transition-draw-pulse
```

Preserve prior artifacts before replay; never execute the generated PE. The completed replay JSON SHA is `62e5d69451e7a298061fc2cc6e97f7d7604cb88905125e492dcdc834bd501ba8`; immutable 58-input freeze SHA is `f4c978c538bab0b315c1f64403854204765a3f36fa6e0ec3e9b775e8fc1f73c5`. `config/target.toml` was separately bound **after the build** to unchanged checkpoint bytes, SHA `385ecdeac9ac89bf9706a7db766ef1ad2d6d33b2f50ee360c72eae3d67932789`; it was not retroactively added to that freeze. The target executable identity remained independently checked.

Read-only collection `161538f38a9d46769fbfe0c12918a1c8` used maintained `ltcg_link.validate_link_entry`, `linked_image` PE/MAP/PDB readers/object-path rules and the actual 14 object fingerprints, plus resolution logs/aliases. PE SHA is `be9170319f40e534a154ef8985dc44d81bd98c8e89a9f658a889ba70f3fd251b`; PDB GUID `3896145b0fa939449fe955807ddbd711`, age 1. Complete external evidence SHA is `55b2a547dfbba581583c1bd7793623cb667d17e07044894f1da270d8f96e61ac`. Large binary/log/certificate bundles and failed collector revisions are not committed. A future observation must recapture actual hashes and full ownership; canonical callback replay alone does not prove whole-Run equivalence.
