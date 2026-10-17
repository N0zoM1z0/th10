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

## Opcode2 portrait creation: source contract

Opcode 2 now directly calls CreateVmVariant0 on its selected live resource and publishes the returned ID to the enemy-portrait slot. The only production edit removes `loaded == NULL ? 0 : `. Exact character acceptance (0 or 1), stage routing, signed message-index comparison, resource loads, scripts, layer and publication destination are unchanged. All source bytes outside opcode 2, including target-supported opcode 13 guards and the preceding opcode 19 correction, remain identical. No build-profile, ABI, translation-unit, attribute, volatile, floating-point or padding changes were made.

Existing target evidence at 0x416127..0x41623B contains six unguarded calls to 0x448D00: 0x416154/0x416177 use enemy resource +0x38 and script 0x20 for stage 6; 0x4161B4/0x4161D8 use that resource and script 0x25 for stage 7 with signed messageIndex < 1; 0x416209/0x41622D otherwise use player resource +0x10 and script 0x1E. Each accepts only characters 0/1, passes layer 15 and publishes to message +0x44. The earlier native candidate contains the unsupported null-resource skip-create/zero path at 0x40124D..0x401254. This existing native evidence is not a new build of this proposal.

The maintained portrait test matrix covers 768 combinations: both supported characters and six invalid/extreme characters, twelve stage values, and eight signed message indices including INT_MIN/INT_MAX. It checks all three routes, exact scripts/resources/layer, exactly one creation, returned-ID publication, invalid-character no-op behavior, other IDs and message/profile/GUI/VM sentinels. Existing common advance/timer checks remain active. The unchanged opcode 1/3 and opcode 13 tests remain in the suite. A separate structural contract rejects the original null-fallback source; valid-object behavioral tests alone cannot distinguish that guard.

All execution assumes live owner, message and selected resource objects. No null-member C++ call is tested, and no claim is made about the target callee's behavior for a null resource. Host extraction and sanitizer tests validate only their documented semantic seams, not native ABI, exact bytes, function sizes or target execution; sanitizer leak checking is disabled by the maintained driver.

This correction is based on checkpoint b50600da8fc2adb86cddcd95f7befd2b70c313d7. The preceding opcode19 native observation belongs to its stated source hash; it is not a new native result for this correction. This section records source and host-test evidence only. Whole-owner native validation and canonical-unit replay remain separate requirements.

Validation: all 13 interpreter selections pass normally and under ASan/UBSan; all 28 existing strict mutation controls, 11 opcode 19 controls and 15 additional portrait controls are detected. Original-guard structural negative control, isolated patch application and raw output comparisons pass. Both changed files retain CRLF with zero bare LF.

src/Gui.cpp before: 0264d2a32bab3b5c359d8e9d8aba02f9ff1eaa67e0e0f1cd619f729c9e954ce1

src/Gui.cpp after: 5d6c8b4826177eb6680df37ecb75dbcfab4a57fbb6eb69dcdb2293eca1f0864c

scripts/fixtures/gui-message-run.cpp before: 4a63d766200298dfab4d0dcaf5b485c800600de58b0a5a1639cac697db94162e

scripts/fixtures/gui-message-run.cpp after: 39ef1e0ef1224e3fe9dc95c1ef421205a64edc14989c04b7990821f3a4d09f1d

The portable structural and mutation control is maintained in scripts/test-gui-portrait-creation.py. Its output path must not already exist. For a fresh run:

```sh
work=$(mktemp -d)
scripts/repo-python scripts/test-gui-portrait-creation.py --out "$work/portrait"
scripts/repo-python scripts/test-gui-portrait-creation.py --sanitize --no-mutations --out "$work/sanitized"
```

Both commands use the maintained interpreter extraction and fixture. The CI wrapper uses its own temporary parent with a fresh child path, without replacing existing output.

## Opcode 2 native observation

The subsequent canonical replay records eleven exact units across seven producers (912 matched bytes); this does not confer whole-Run exactness. Replay SHA-256: `a1c8b1a1ebfb3e13e9879b805120f4b70031a81219fb799dae18d3bf37e6a8e6`. Immutable input freeze SHA-256: `8a9cc1ae7fcaa0f5fbceeaaa2edc702080255b47095f6f08aff34b29e4b922e1`. Retained native evidence SHA-256: `e4e47690e0f6ea46b8dc0111dd85492c968f38c79a6a8b95028e7f30b88d01c9`. The observation binds Gui.cpp SHA-256 `5d6c8b4826177eb6680df37ecb75dbcfab4a57fbb6eb69dcdb2293eca1f0864c`; the PE SHA-256 is `15808f2a2e67ff28b39ac65e50e229a0c138fcf394784b13646bdf9495d6a551` (PDB GUID `3d9e3063e250ad438727bad6191fc759`, age 1).

Independent provenance review accounts for all 113 frozen inputs, fourteen full-path objects, 47 required whole owners, twelve referenced callbacks and 1076 code contributions. All 1075 non-Run contributions and all 36 selected non-Run owners retain identical raw bytes and starts; 440 anchors are unchanged.

Independent local static review confirms that opcode 2 removes exactly the earlier 12-byte resource-null skip/zero-publication alternative. Its real creation call is now at 0x401258 and full-dword enemy-portrait publication at 0x40125F. Exact character 0/1 acceptance, stage 6 enemy +0x38/script 0x20, stage 7 signed messageIndex < 1 enemy +0x38/script 0x25, otherwise player +0x10/script 0x1E, layer 15 and common-tail routing are retained. The target's six separately emitted creation arms remain distinct from the candidate's single merged call; no byte-exact equivalence is claimed.

Whole Run is 4020 bytes: 3845 executable, three padding bytes and four tables totalling 172 bytes. All 43 table destinations preserve their semantic roles, and all 163 HIGHLOW fields are checked as address operands or table words. GNU independently agrees with the retained candidate and target executable-span instruction boundaries. The production source change is confined to opcode 2. Beyond opcode 2, native code-generation differences are confined to an added ECX copy in opcode 3 and scratch-register/address-calculation scheduling in opcodes 7/8; reviewed resource/field reads, call order, arguments and publication effects are preserved. Every other physical opcode region, including opcode 13/19/20 and the common tail, retains instruction bytes and referents after masking only validated address/relative fields. Run's executable span shrinks by ten bytes; padding grows by two, for an eight-byte owner reduction.

Updater-to-Run stack receiver and RET4 remain intact, as do the creation call's stack arguments/EDI return temporary, real creation callee RET12 and both completion-route EDI receivers. These are bounded static code-generation findings. No generated PE was executed. Existing anchor/storage/runtime, private-ABI, null-callee and nonexact whole-owner limitations remain; no aggregate-progress claim follows from this observation.

## Canonical ReplayManager Run owner: source and host evidence

The replay-owner proposal replaces the local GuiModeOwnerView/g_GuiModeOwner shadow with the existing complete ReplayManager class and g_ReplayManager->mode at both Run completion-route reads. Gui.cpp adds ReplayManager.hpp; production headers are unchanged. Both equality-to-1 comparisons remain at their original post-score source evaluation points, with the same live-owner assumption. There is no null fallback, extra capture/reload, unrelated-object cast or code-generation control. HUD and the rest of the Run body, including portrait topology and floating-point expressions, are unchanged.

Gui.cpp SHA-256 changes from `dde1c03602d80e3616fe3d42c0befe186d3078723128db1c6d89fc1a598a8241` to `b571c647306049ebcbc17b101d49887e5bc00059e1da6fc49184206f0cfcc36f`. The separately retained nine-file source/test patch is SHA-256 `c25dabe3e75dc5f62dc46e5ddd20e467fcfbf60f7c336a970a4cbafc5f22d30a`.

Target identity is established by complete factory/initializer/destructor evidence, rather than a source-name inference: allocation 0x2D4 at 0x429627 flows through ESI and EBX to the initializer; 0x428F67 stores incoming mode at object+0x10; mode-0 and mode-1 paths publish that same object to 0x477838 at 0x428F77/0x429251. The destructor compares its receiver to this slot at 0x4295CB and clears only its own pointer at 0x4295D3. Run loads 0x477838 at 0x416D66/0x416E43 after the score calls and compares +0x10 with 1 at 0x416D6C/0x416E49. Retained target lifecycle evidence SHA-256 is `7fe7d8fcc1a229bec474f2927f5b2b56a8b590e5302763856bd00f4a4c3bd5ec`. Canonical symbol spelling is `?g_ReplayManager@@3PAVReplayManager@@A`.

Host fixtures retain complete actual ReplayManager/ReplayStageState declarations and construct valid objects through explicit member-initializing host constructor/destructor seams. These seams do not implement replay initialization, publication, resource ownership or cleanup. The new portable scripts/test-gui-replay-mode.py covers twelve actual-owner mode cases and 24 synthetic post-score replacement cases, plus twelve strict controls across both read sites. Nonzero/constant predicates, wrong actual owner, stale pointer and stale mode are rejected by predeclared exact failure status/stdout/stderr; compilation errors, signals, unrelated assertions and timeouts are not accepted. Its compile/run bounds are 120/15 seconds. Structural negatives reject the retired shadow and a missing canonical read; existing output is rejected without overwrite. The score replacement callback is an explicitly synthetic arithmetic-plus-callback dependency, not evidence that actual score calls replace the replay global.

All twelve replay controls, the retained 28 interpreter, 32 transition, 15 portrait, eleven name and fifty focused HUD/gauge controls pass. Lifecycle, constructor, message-phase and selector checks also pass. Interpreter, replay and message-lifecycle positives pass under ASan/UBSan with maintained leak-checking limits. Compact local results SHA-256: `2b599c715e276b5da27506e448bee8953c73ce3eb4de0367bc69b5747a73ebf1`.

```sh
work=$(mktemp -d)
scripts/repo-python scripts/test-gui-replay-mode.py --root . --out "$work/replay"
scripts/repo-python scripts/test-gui-replay-mode.py --root . --out "$work/sanitized" --sanitize --no-mutations
```

CI uses a temporary parent and a nonexistent child output path, retaining the driver's fresh-output contract. This section records source/target correspondence and bounded host evidence only. The subsequent native observation below records compilation, linked class-symbol identity, full Run ownership, include closure and canonical-unit replay; physical target storage and private-ABI/runtime closure remain unproven. No generated PE was executed; prior native observations and eleven exact regressions receive no new exactness or aggregate-progress credit from these host results.

## Canonical replay-owner native observation

The subsequent frozen replay preserves eleven exact units across seven producers
(912 matched bytes). This regression credit is separate from whole-Run/HUD
exactness. It binds `Gui.cpp` SHA-256
`b571c647306049ebcbc17b101d49887e5bc00059e1da6fc49184206f0cfcc36f`,
PE `2be4022460bfcb4159a8028d0fe1e43da525b874da8c948c47b2ebec198342d0`
(PDB GUID `5cc8d5682d5309408057eca739d0a86f`, age 1), evidence
`a92b9393f6d999cda5519784cb60ced6ad363f875cfd23893be54c47a4aa71ad`,
replay `60cb49dffb29826e33cb231d6ac2c330082e64171cfa976f282e5c8261ae86b2`
and input freeze
`8c1e424e57f619a014728a9b1bb6a46fb000a0f1cd97d0060f447bd7144274e9`.

Against the preceding player-migration artifact, whole Run remains 4,020 bytes
(3,845 executable, three padding, 172 table bytes), with owner SHA-256
`29a5e59f699c39eb1cfe287b4c2d62aaf2419687a2df79a482143356431e8336`.
Only two normalized operand roles change, both in opcode 20: loads at
`0x401DEA/0x401ECD` now use the unique canonical class-symbol `g_ReplayManager`
MAP identity at `0x42B6CC`; the retired shadow MAP identity is absent. Both loads
remain immediately after the score calls at `0x401DE5/0x401EC8`, followed by
the mode field at owner+0x10 compared with 1 at `0x401DEF/0x401ED3`. No guard or earlier capture
was introduced. All 43 table destination roles, 163 validated HIGHLOW fields,
111 call identities/order, intrafunction direct jumps, padding and ABI instruction
forms remain intact. The complete raw delta is 49 instructions / 51 bytes:
two canonical-owner substitutions and 47 same-role address shifts.

HUD remains 3,772 bytes, owner SHA-256
`b83b7fff55ff4e6cd3c0c30e21b24b6435339558f5b7e507004de37cedfe7611`.
It has no normalized code, table or branch-role changes: eleven instructions /
eleven bytes change through seven same-role absolute address shifts and four
same-original-symbol CALL shifts. All 21 call identities, seven table destinations,
padding and other bytes are preserved. Canonical enemy/player identities, three
player-read phases, retained boss, float field accesses and FP instructions remain
unchanged. GNU independently agrees with complete Run/HUD executable instruction
boundaries; owner-local table/padding boundaries are checked separately.

Independent provenance review checks 125 frozen inputs, fourteen full-path
objects, 47 required whole owners and twelve callbacks. All 1,076 contribution
starts/sizes remain unchanged (910 raw-identical, 166 changed); this campaign
introduces no ordinary-byte scheduling delta. The all-field review retains 224
broader unresolved original-base/addend cases outside the bounded Run/HUD,
required-owner and registered callback scopes. Observed MAP labels alone do not
clear those cases or establish whole-producer equivalence.

The existing HUD captured-manager reuse and less-versus-unordered alpha residual
remain. Neither these static observations nor the synthetic score dependency
witnesses prove actual callback replacement, physical target storage/alias behavior,
private-ABI/runtime closure, x87 exception/conversion behavior or gameplay
reachability. No new whole-owner exactness, whole-producer equivalence or aggregate
completion credit is claimed. Logs and binary evidence remain outside repository
documentation.

## Opcode 2 branch-structure hypothesis: source and host evidence

The next bounded proposal changes only opcode 2 production source. Target
0x416127..0x41623C has three outer stage/index branches, each with separate
character 0/1 creation and message +0x44 publication leaves: stage 6 selects enemy
resource +0x38/script 0x20; stage 7 with signed messageIndex < 1 selects the same
resource/script 0x25; otherwise player resource +0x10/script 0x1E. All use layer 15
and join the common loop at 0x416F09. Unsupported characters neither create nor
publish. Ordinary nested C++ switches express six static alternative leaves,
with exactly one runtime creation on each accepted path and direct resource
assumptions retained. Opcode 1, opcode 13 and all other production bytes are unchanged.

This is a target-CFG-supported source hypothesis, not proof of original lexical
source. There are no compiler controls, wrappers, forced registers/temporaries,
volatile, padding or type tricks; ordinary return-value expressions remain.
The compiler may merge equivalent leaves. Six emitted calls, native CFG retention
and improved byte matching are not guaranteed. Gui.cpp changes from SHA-256
`b571c647306049ebcbc17b101d49887e5bc00059e1da6fc49184206f0cfcc36f` to
`7ee6a5dbefff663040c0c0e5b6e2415436391f6344b0ea982933d32b1385d202`.
The separate three-file source/test patch is SHA-256
`8bafaa2fd0d90442e050212e39d2b6bdf56224377cc728ca6455b29703cde3d4`.

The unchanged 768-case portrait matrix and all 15 strict portrait controls pass;
all 13 interpreter selections and 28 strict Run controls remain covered and pass.
Run and portrait positives pass ASan/UBSan within the maintained host-seam and
leak-checking limits. The maintained portrait driver also deterministically
creates a separate fixture with six synthetic callback witnesses, covering each
accepted leaf while creation changes stage/character/resources and the result
slot. Selected call arguments and subsequent returned-ID publication are preserved.
These six witnesses pass normally and under sanitizers; they do not establish
that the real callee changes those globals. The checked-in 768-case fixture stays
unchanged. The existing portrait commands/CI hook reproduce these checks without
an additional helper or retained generated fixture.

The previously collapsed, semantically equivalent source separately compiles
and passes the positive runtime fixture, normally and under sanitizers. Its
rejection is solely a source-structure negative, never a functional mutant
detection. Null-guard rejection is also structural. Functional controls require
successful compilation, exit 1, empty stdout and the exact intended CHECK stderr;
compile failures, signals, timeouts and unrelated failures are not accepted.

### Native validation status for branch structure

The preceding canonical replay-owner observation applies to its own frozen
source. The fresh frozen replay and bounded whole-owner review for this branch
proposal are recorded below, including actual call/argument/publication paths
and adjacent-region changes. Host results and source leaf counts confer no new exactness,
size-ratio/helper-sum credit, whole-producer equivalence or aggregate progress.
No generated PE was executed for this proposal. Binary evidence stays outside Git.

### Branch-preserving opcode 2: bounded native observation

The fresh replay preserves eleven exact units across seven producers (912 matched
bytes), separately from whole-Run exactness. Identities are:

- `Gui.cpp`: `7ee6a5dbefff663040c0c0e5b6e2415436391f6344b0ea982933d32b1385d202`
- PE: `383eb4d3ee7625143b54d98c30ecefe30e5ba2b89925884dd8cca98c085e2685`
- PDB GUID: `e3e3a3ecdfd5fa4d9979dd6f1c116e90`, age 1
- Evidence: `39e9b44d5fd978d8927e33ce90a83ec8dc186d400743cdda3e5261725ada3f71`
- Replay: `c6b1734a7e4f2476214a83ab53cac104d724869385d68b41ed0a9fa2d352f918`
- Input freeze: `ee2e0dc72bd53bbdbd484c7b517e1de4b4fa3e3b228aa8df03a1adfe59656a77`

Actual opcode 2 contains six direct creation calls at
`0x401229/0x40124D/0x40128C/0x4012B0/0x4012E5/0x401308`.
The first pair uses enemy resource `+0x38`, script `0x20` for stage 6; the next
uses that resource and script `0x25` for stage 7 with signed message index less
than 1; the last uses player resource `+0x10`, script `0x1E` otherwise. Each
pair accepts exactly characters 0/1, passes layer 15 and a local aggregate-result
address in EDI, reads the returned dword through EAX, publishes to message+0x44,
and jumps to common tail `0x40207C`. Other characters create nothing and retain
the existing ID. A decoded static route model checks 1,536 combinations including
zero/full-width returned IDs and invalid-character sentinels; this is not native
runtime execution or proof of null-resource callee behavior.

Whole Run is 4,196 bytes: 4,021 executable, three padding, and 172 table bytes.
Opcode 2 changes from 31 instructions / 110 bytes to 82 / 286; its address fields
increase from five to ten and whole-Run calls from 111 to 116. All 43 table
destination roles and 168 validated HIGHLOW fields are accounted for. These
sizes and counts confer no ratio, helper-sum, exactness or completion credit.

Adjacent changes are explicit rather than hidden by relocation masking: the
private frame grows from 0x88 to 0x9C, the incoming receiver load after two saved
register pushes moves from `[ESP+0x94]` to `[ESP+0xA8]`, and both epilogues restore
0x9C before `RET 4`. Twenty-seven aggregate-temporary LEAs in opcodes 3/12/13/19/20
move within the local frame with their producer/consumer relationships retained.
Opcode 3 uses EDX instead of ECX for the resource-load scratch value, which is
dead afterward. Opcode 7 computes an address through EAX instead of EDX, then
resets both registers before the next call. Every non-opcode-2
intrafunction direct control edge retains its semantic region/instruction target.
GNU independently agrees with complete executable instruction boundaries.

HUD remains 3,772 bytes with zero normalized code, table or internal-branch
changes and the same 21 call identities and seven table roles. Existing canonical
owners, captured-manager reload limitation and nonfinite alpha residual remain.
Independent provenance review checks 127 frozen inputs, fourteen full-path
objects, 47 required whole owners, twelve callbacks and 1,076 contributions.
Only Run changes size; all 1,075 non-Run contributions retain ordinary bytes after separately
accounting for validated absolute and relative address operands. The all-field review
retains 196 broader unresolved referents outside the reviewed Run/HUD,
required-owner and registered-callback scopes. Correspondence is not typed
ownership; 79 baseline unowned contributions remain unowned.

These static observations do not establish original physical storage/alias behavior,
private-ABI/runtime closure, x87 exception/conversion behavior or gameplay
reachability. No generated PE was executed and no whole-producer equivalence is
claimed; logs and binary evidence remain outside repository documentation.

## Opcode 13 separate character arms: source and host evidence

This proposal expresses the existing eight opcode13 character-0/character-1 groups as ordinary separate case bodies. Each new body is an exact duplicate of its prior grouped body; collapsing the pairs reproduces the original opcode13 bytes. Gui.cpp SHA-256 changes from `7ee6a5dbefff663040c0c0e5b6e2415436391f6344b0ea982933d32b1385d202` to `79caef3b134bb04db78baa0d90a8e373bfe1cecdce44185f3da18a565573394b`. All source bytes outside opcode13, including opcode2 and the replay-owner reads, remain unchanged. No header, ABI/profile, floating-point, compiler-control, forced-temporary or padding change is made. The separate three-file source/test patch is SHA-256 `7412fbb9c793d1ab3b532bcec580d60e718851d4d7eb5a09c13d9b3ecc01bc17`.

The independently audited target region 0x416408..0x416A07 contains eight pairs of separate character arms. Each pair's complete caller instruction sequence is identical after normalizing only the two local aggregate-output addresses. Target whole-Run SHA-256 is `13c80eaec4d7e90718b107f1e37ea8de7bd57d8ac3f86f0af4b6269644614c35`; contract-audit SHA-256 is `8180fdf2a21031f05edf48415cf5e86dbd93103c4d936f338e6e092c96db7d5b`. This supports a source-topology hypothesis, not an original-lexical-source proof or demonstrated runtime defect. Compiler merging remains permissible.

Stages 1..7 and characters 0/1 retain the same eight script/offset contexts, including the signed stage7 message-index threshold at 1. Every accepted path performs FindScript/SetSpriteWithAnm/FindScript/SetSpriteWithAnm once. Each setter's resource and sprite argument are captured before its own finder; the second pair reloads canonical enemy manager/resource and currentInstruction/payload after the first setter. Both finders retain the same parent-ID address, while later lookup can observe changed ID/child-chain state. Returned child IDs are immediate setter receivers, not new portrait-ID publications. No callback redispatch or null-success alternative is added.

The portable `scripts/test-gui-enemy-portrait-sprites.py` retains complete existing declarations and maintained finder traversal/setter resolution, adding explicit host observers and synthetic callbacks. Its 1,216 cases comprise 112 value cases, 896 guard/signed-boundary cases and 208 callback cases. They cover both characters in all contexts, child/parent identity, resource and argument capture, canonical-manager and instruction replacement, parent-ID/chain changes, second-finder capture retention, call order and absence of Run-level ID publication. All cases pass normally and under ASan/UBSan, with maintained leak-checking limits. The grouped source also passes the same complete runtime matrix; its source-shape rejection is separate and never counted as a behavioral mutant kill.

All 25 predeclared strict controls pass with successful compilation, exit 1, empty stdout and exact intended CHECK stderr. Crashes, compile failures, timeouts, unrelated assertions and extra output are rejected; new-driver compile/run bounds are 120/15 seconds. Classifier self-controls and fresh-output refusal pass. The original Run fixture is unchanged; only three existing mutation tuples adapt anchors/counts, preserving all 28 names, selections and expected checks. Retained Run28, portrait15, replay12, name11 and fifty focused HUD/gauge controls pass, as do message lifecycle and normal/sanitized Run positives. Compact local results SHA-256: `a13b8e9f957f3c83dfa1d33d4e5529d0d9372cc926c3a8749d9c1ff9ad6d94eb`.

```sh
work=$(mktemp -d)
scripts/repo-python scripts/test-gui-enemy-portrait-sprites.py --root . --out "$work/opcode13"
scripts/repo-python scripts/test-gui-enemy-portrait-sprites.py --root . --out "$work/sanitized" --sanitize --no-mutations
```

CI uses a temporary parent with a nonexistent child output path. These are bounded source/target-contract and host results; callback effects are synthetic, not claims about actual callee side effects. Valid parent/resource, single-threaded and defined signed-arithmetic prerequisites remain. The following section records native compilation and canonical replay for this source snapshot; linked storage, helper private ABI/aggregate lifetime and full owner equivalence remain open. No generated PE was executed and no new native exactness or aggregate-progress credit follows from these tests.

### Split-only opcode 13: negative native experiment

The fixed seven-producer replay preserves eleven existing exact units (912 bytes),
but the split-only source `79caef3b134bb04db78baa0d90a8e373bfe1cecdce44185f3da18a565573394b`
does not converge to the target whole Run. The complete candidate owner is 5,148
bytes (4,976 executable, no padding, 172 table bytes), versus target 4,480 bytes.
Opcode 13 emits sixteen accepted leaves and 64 finder/setter calls, yet each leaf
has 33 instructions rather than the target's 23. Four spill stores, four reloads
and two receiver transfers per leaf account for ten additional instructions;
opcode 13 is 2,111 bytes versus target 1,535. The frame is 0xDC versus target 0xD8.
Matching branch or call counts is therefore not byte-exactness progress.

This observation binds PE `9cc50fc79159dd40ac1e561827d53c470f105163114db1a825bdbcb0320fc5a9`,
evidence `ad6ff2787ee9a6e9dfdb84eb13d6c89b8129ee118024c742d747fc1d91a857a5`,
replay `37444b6f5fe9c35641c2eaa0463dab5214026f96797901790d74da21c895d652`
and 128-input freeze `718dc7c0300129a9224aff03b3ea612eb7a7bb45df89b0ff835ddb3b44b7ad9a`.
Bounded static models retain both pre-finder captures, the second reload,
parent-ID address, immediate child-ID consumption and all 43 table meanings.
Collateral frame, register, flag-write and bonus-control changes were reviewed;
HUD has no normalized code/table/control-flow change. Fourteen objects, 47
required whole owners and twelve callbacks remain verified. The broader image
still has 198 unresolved referent cases outside this scope and 79
function-inventory-unowned contributions. No whole-producer equivalence follows.
The split-only snapshot and its evidence are retained as a negative experiment,
not a separately committed production success.

### Immediate returned-handle receiver hypothesis

The target stages the outer setter's captured arguments before the finder and
immediately consumes its returned handle. One subsequent ordinary-source
hypothesis writes `ResultMenuFindScript(...).SetSpriteWithAnm(resource, first)`
(and the corresponding second call) instead of assigning named child handles
before separate setter statements. The explicit first/resource/second captures,
second-pair reload, sixteen branch arms and all other production bytes remain.
This changes Gui.cpp to
`34483bcbc5bc90e502a4173af4f350fd84da8151871609551ed88cee6795dd3b`.
It does not change any helper declaration, calling convention, profile or TU,
and introduces no forced register, temporary layout, inlining control or padding.

The complete AnmVmIdView declaration contains one integer and an implicit trivial
destructor. The finder returns a value; the setter copies the ID by value into
FindVm, then forwards only the resolved VM, resource and sprite index. The
receiver address does not escape through those maintained bodies. Shortening the
handle lifetime to its full expression therefore has no observed maintained-source
side effect. Retained explicit captures make callback-sensitive values independent
of host argument-evaluation order. This is a source/dataflow hypothesis, not proof
of original lexical syntax or a prediction about the target compiler's allocation.

The actual chained source passes all 1,216 caller cases normally and under
ASan/UBSan, including the same synthetic callback cases. All 25 predeclared
controls mutate that actual source and require successful compilation, exit 1,
empty stdout and the exact intended CHECK. Where a wrong recapture/reuse requires
explicit sequencing, only the selected mutant expression materializes a local
handle; ordinary source is not normalized before mutation. Correct named and
grouped forms separately pass the same positive matrix, while their representation
rejections are structural only. Thirteen whole-Run selections, all 28 retained
controls and sanitized Run positives also pass. The original fixture and control
names/expected checks are retained; only necessary source anchors change.
Compile/run limits remain 120/15 seconds. These are host-seam and caller-contract
results, not native ABI or real callback-side-effect proof. Results SHA-256:
`b9940d8d0b44a29d1b13ee2de27a192e46e16d18ea5212e782cff08dc175db3f`.
The existing portable sprite-driver commands and CI hook reproduce these checks.

The fresh frozen replay and complete bounded owner review for this new source
are recorded below. The split-only result above remains a separate receipt.

### Receiver-chain native observation

The fresh fixed replay for Gui source
`34483bcbc5bc90e502a4173af4f350fd84da8151871609551ed88cee6795dd3b`
preserves all eleven registered exact units across seven producers (912 bytes).
The 128 frozen inputs, fourteen full-path objects, 47 required whole owners and
twelve referenced callbacks pass the bounded collection/review gates. This receipt
is separate from the failed split-only experiment above.

Complete Run is 4,548 bytes: 4,376 executable bytes, no padding and 172 table bytes.
The committed grouped baseline is 4,196, the named split experiment 5,148, and
the target 4,480; these sizes confer no exactness or progress credit. Opcode 13
has sixteen 23-instruction leaves, 414 instructions and 1,532 bytes, versus target
414 instructions and 1,535 bytes. Each of its 32 finder calls now immediately
feeds a setter call, with resource and sprite arguments already pushed. The named
experiment's extra spill/reload and receiver-transfer instructions are absent.

Complete current helper bodies establish ESI parent, EDI script and EBX aggregate
output for the finder, returning that output address in EAX. The target uses EBX
script and EDI output instead. Both setters consume the returned EAX receiver;
the explicit-resource setter consumes its two stack arguments with RET 8. This
private finder-register mismatch remains. First captures precede the finder;
second instruction/payload and resource loads occur after the first setter and
before the second finder. Pure load order differs from target without an
intervening call. No caller ID publication or callback-driven redispatch is added.

All 1,536 decoded-static route/callback cases pass; these are model evaluations,
not native execution. Ninety-seven collateral static paths per image match both
baselines, with remaining blocks and sixteen complete direct callees checked.
All 43 table destinations retain their semantic roles, including expanded stage
leaves and the difficulty cases. Frame 0xD8, incoming receiver at ESP+0xE4, saved
registers, both restores and RET 4 agree with the target frame contract; aggregate
slots remain valid but do not establish original lexical layout. Opcode 20 retains
the reviewed memory-OR/score routes; relocated entry/opcode-21 scratch storage
preserves FP operations. HUD remains 3,772 bytes with unchanged normalized
code/jump/table roles and all 21 call identities.

Evidence SHA-256:
`7495e25e18e1b28fcc8e03616121d2af358d52c713a0c24687b3c87d664adad9`.
PE: `36eb4f9ecccffa51ff6b8ba80fdea888f64377f2262ac4500fbba90138d35522`;
PDB GUID `387395d9cc65e547b20f8834eb46e7b1`, age 1. Four exact binding JSONs:
- replay `46bf0a9e1e881e06bd50da62b838da657a806ed6d76bdb3f019accbe7d06e7dc`;
- freeze `1e890048b006cb31858c4cfc391bdc77cd22a70ac4b5afd6b1d1707351ade039`;
- plan `1a9c1e559bdf728ccd566b85ed52a40d4819ab0300411ee3356b85af7d23eba8`;
- artifacts `a8afb54b8bffa8fec6ff85c9f610880efde94e1ce83a9d193afbe51c7e064169`.

Semantic report:
`1bba05ae4dadf3e4368ba3b6f2413de0fa6ec4a76268524c924140cf84d82e7f`;
provenance report:
`b1d324130542020804006480373fe4e6d370f5c450d93012013b4590d11c7e63`.
The separate read-time target-helper export
`fd31969b35e695cdf3955681b51c8e0f6d18176d35372cd1b75fc13d66bbbee0`
is not a retroactive freeze input. All 1,075 non-Run contributions retain ordinary
bytes after validated absolute and relative address operands are accounted for;
198 broader unresolved referent cases remain outside the reviewed scope. Existing
live-owner/resource assumptions, physical storage/alias limits, HUD countdown
reload and nonfinite/FP residuals persist. No whole-producer/runtime equivalence,
new exact credit or whole-Run target equality is established. Logs, binaries and
route-model artifacts remain outside Git.
