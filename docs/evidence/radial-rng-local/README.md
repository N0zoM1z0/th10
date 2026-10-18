# Radial initializer and signed-RNG checkpoint

The complete radial initializer passed the canonical linked-PE comparison at **803/803 bytes**. All **29 affected existing canonical units**, grouped into **12 recipes**, were preserved. The generated unit is `anm-initialize-pulsing-radial-trail`. Factory-controlled replay/acceptance is **pending**; these authored repository results do not grant Factory credit.

`ExecuteScript` remains **9948 bytes against a 9964-byte target**, with unchanged owner hash. Its exactness is not established by the radial result. No target runtime was executed.

## Maintained change and scope

`GetRandomF32Signed` introduces `const float roll = static_cast<float>(GetRandomU32());` and uses `roll` in the existing division/subtraction. It preserves interfaces, the single RNG call and arithmetic spelling. No forced inline, specialized RNG, register/padding tricks, vector-order changes or identifier tuning were added.

The source-level float-local idea came from the [authored reference at immutable commit 4e4ab0b](https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/core/sprtfunc/effect_ring.cpp) and its [recovery note](https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/reference/recovery_notes/effect_ring_build_004452f0_2026_10_05.md). Its other mechanisms and matching claims were not independently adopted or verified.

RandomMath.cpp baseline SHA256: `88af248abd6b7e5732ae9c34eadbd3fb524ff35d5740432762c23dd9e35466c4`.
Adopted SHA256: `5866b8573e21b45ff421d4e3930a1d58660a4cf7725e39fd9ec319cc04f7fca1`.

The complete binding covers 43 address fields / 172 encoded bytes: 27 known identities, eight typed constant instances and eight explicitly reviewed RNG source-object address bindings. The RNG exception requires pinned source/layout/config/call-chain authority, exact MOV/GPR32/flat-memory forms, independently expected target members and verified per-field alias pairs. Its scope is compile-address identity only. It does not prove a physical eight-byte diagnostic anchor, original relocation symbols, original source typing or runtime semantic equivalence.

C++98/x87 excess precision does not guarantee that the local forces a spill or binary32 rounding. Directed host seams agreed between original and candidate in tested configurations; both x87-fast variants differed from the binary32 oracle in 1049 cases. These finite return-boundary checks do not prove every inlined caller, rounding mode or gameplay behavior. The 29 preserved units cover their registered boundaries and profiles, not all noncanonical callers or arbitrary future builds.

## Durable files and evidence

`scripts/radial_reproduction.json` is the bounded manifest. It contains all 83 adopted source pins, nine canonical tool/config pins, 29 original unit hashes, the exact 12 recipes, final helper hashes and historical evidence hashes. The entry verifies its fixed SHA256 before using it. The two helper modules are byte-identical final reviewed derivation sources, retained once each; the entry does not import or execute them.

The unit is exactly 7453 bytes, SHA256 `e4ca5113035a8876e752f729fdc189945beaabc8e2a97f05de24251fc2b9c5b6`. Config construction is the pinned baseline bytes, one LF, then this exact LF-terminated unit. Every existing parsed unit remains unchanged. Baseline config SHA256 is `8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88`.

Successful historical commands, with full saved receipt hashes in the manifest:
- Complete independent binding: `b8e9e93e885647e887d7a9f80b55d707`.
- Canonical complete-owner replay: `fc309434dbb1496aad942db711ef2761`.
- Additional ten recipes / 23 guards finalized: `3f15976898af4bbd951471e67e66a12e`; metadata receipt: `bc866e56e69c45a792ae7342eefb6729`.
- Deterministic unit writer and serialization replay: `599164954cf04fa1baa620055690e4c0`.

The manifest also records the original six-guard command, image/MAP/PDB hashes, complete candidate and target radial-owner hashes, ExecuteScript hash, graph hash, private-binding inventory hash, field-structure/spec hashes and RNG policy-proof hash. It contains no native payload or ordered private field table. Historical receipt hashes identify the producer records; this source verifier does not authenticate those external records or rerun their derivation.

## Source-only verification

From the adopted repository root, Python 3.11+ is sufficient:

```sh
python3 -B -m unittest discover -s scripts -p test_radial_reproduce.py
python3 -B scripts/radial_reproduce.py verify-source
python3 -B scripts/radial_reproduce.py native-plan > radial-native-plan.json
```

These commands check bounded source/config/helper files only. They do not read the target image, build artifacts or toolchain binaries, import a native decoder, or compile/link. Verification checks the exact unit suffix, unchanged prior config, all source/tool pins and all 29 guarded recipe identities. `native-plan` emits deterministic argv data after the same verification. No historical ignored packet path is required.

## Separately gated native reproduction

The plan describes fresh compilation from the adopted repository sources/config and the repository's locked VC7.1/Wine/Capstone environment, with the target identified by SHA256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

There is **no native executor in this bundle**. `reproduce-native` always exits with `native-executor-not-implemented`. Do not treat plan emission as permission or as a completed replay. A separately authorized and reviewed executor must:

1. Verify target/toolchain/decoder identities against the pinned target/tools configs; use an interpreter satisfying those decoder pins. Freeze source/config/tools and check them before and after each phase.
2. Run the 12 phases serially, with CPU 120 seconds, wall 150 seconds, one CPU, nice increment 10 and process-group cleanup. Before any build, durably create an exclusive phase claim. A prior claim, including an interrupted one, forbids automatic repetition. Stop on first failure; no variants, retries or sweeps.
3. Call `scripts/compile-probe.sh` once for the normal-COFF RNG object. For each of the other 11 recipes call canonical `ltcg_link.cold_link` once. Its own pinned bounded unresolved-anchor resolution remains unchanged. One recipe call does not mean one linker subprocess.
4. Keep the genuine radial four-TU /GL /GS ExecuteScript graph: AnmManager.cpp plus RandomMath.cpp, AnmVmCreate.cpp and AnmVmId.cpp. The radial compared owner is not a substitute link entry.
5. Freeze each object or image/MAP/PDB set after building, run all listed read-only canonical `--unit ... --json` comparisons, and verify identical artifact hashes afterward. Require the complete 803-byte radial result and all 29 exact guard results, including complete linked normalization. Summarize only fixed statuses/counts/hashes; keep raw native diagnostics private.

The planner uses canonical compile/link/comparison tools already in the repository. It supplies no replacement normalizer. The copied binder/typed helpers document the final reviewed derivation policy but do not form a fresh private-row generator on their own; their immutable authority includes historical artifact pins. A newly adapted and reviewed envelope would be needed to regenerate independent bindings for newly built artifacts. The existing canonical unit can be replayed without regenerating it.

`verify-receipt --receipt FILE` accepts a bounded `radial-native-summary-v1` summary tied to the exact emitted plan hash. It requires ordered 12 phases, unique claim hashes, one build call each, prescribed limits, matching pre/post artifact hashes and exact safe comparison summaries for all 30 units. The portable test fixture shows the exact JSON schema. Its result is **receipt consistency only**, always `execution_authenticated: false` and Factory pending. A fabricated internally consistent summary can pass; trusted execution/claim provenance requires the separate runner and its independent receipts.

## Integration handoff

The docs-only ECL checkpoint is integration base `4068024da61967dbfef3b496c2476ef1e24bfa91`. Historical producers remain recorded at `65e3fd3c127c7efe190ed9ac2cd9b82d31bd2725`; never rewrite those receipts to the new base. Before adoption, prove the unchanged 83-source/tool/config pin bridge across that docs-only commit, then verify the single adopted RandomMath hash and additive unit against this manifest.

The parent-owned integration driver stages all payloads before replacement and requires absent bundle destinations. Source-only bundle verification is not an adopted-source-to-fresh-candidate proof. Complete the separately authorized replay bridge before claiming integration validated. Preserve prior behavioral provenance, regenerate ledger-derived progress with the canonical tool, and retain Factory-pending wording in the existing handoff. Commit subject starts `gpt-dots:` and contains the exact trailer `Co-authored-by: gensokyo-dev <342580529+gensokyo-dev@users.noreply.github.com>`. No push is implied.
