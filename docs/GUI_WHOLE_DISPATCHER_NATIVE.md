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
