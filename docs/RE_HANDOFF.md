# TH10 Reconstruction Handoff

## Session status

The exact-reconstruction campaign remains **active and incomplete**. This checkpoint adds a reviewed source-present FileSystem encrypt transform, but it does **not** establish canonical exactness, whole-product build closure, runtime closure, semantic-reconstruction readiness, port readiness, or new Truth Kernel acceptance.

Repository selection for this session:

- Repository: `th10`
- Analysis provider: `th10-ghidra`
- Target: `target:th10-main`
- Private local target: `resources/th10.exe`
- Phase: exact reconstruction with early faithful Windows i386 build feedback

The private target remained ignored and untouched. No target bytes were patched, copied into source, relocated, or committed. No push was performed.

## Recovery and starting state

Session starting source commit:

- `c4d329828015d8d23e58d6f6c33f232e3fb542ec`
- subject: `gpt-web: reconstruct TH10 LZSS encoder tree`
- branch: `main`
- upstream at entry: `origin/main = cfe26df8d99bbaeb82e93275141608482a2026c2`
- relation: ahead 1 / behind 0
- dirty counts: 0 staged / 0 unstaged / 0 untracked / 0 conflicts

The full recovery gate inspected recent commits, porcelain-v2 status, staged and unstaged diffs, untracked files, the handoff, and relevant ignored state. There was no recoverable interrupted tracked work and no unrelated or unknown dirty work.

Ignored/private/generated state was classified and preserved:

- `resources/th10.exe`: operator-supplied private target; preserve and exclude.
- `.tools/`: shared tool selectors and Wine/tool state; preserve and exclude.
- `ghidra-project/`: target-bound provider state; preserve and exclude.
- previous `.analysis/` campaigns: checkpointed or legacy ignored evidence; preserve.
- `build/`: generated/open build state; preserve.

`.analysis/` measured 103,413 bytes at session entry. `build/` measured 5,031,767 bytes.

All required repository and Factory guidance was read before editing, including the repository-local reconstruction skill and the Factory-root `docs/semantic-reconstruction.md`. One initial attempt used the nonexistent repository-relative `docs/semantic-reconstruction.md`; after the required live-status recovery, the correct Factory-root document was read completely. No required guidance document was ultimately unavailable.

## Mandatory preflight

The following repository preflights passed before target-dependent editing:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Target identity remained:

- SHA256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, 4 sections
- image base `0x00400000`
- entry point `0x004537DC`
- Rich checksum `0x3152A748`
- dominant tool build 6030

The pinned VC7.1 SP1 candidate passed its execute smoke, including normal COFF, LTCG, resource, and PE32-link checks.

`th10-ghidra` operations and schemas were rediscovered. The mandatory `check {}` passed with:

- `attestation.status = passed`
- target `target:th10-main`
- `attestation.provider_transport = factory-native-command`
- correct target hash/layout/entry
- `exactness_credit = none`

All useful Ghidra observations in this packet were target-attested and read-only. No Ghidra observation is treated as exactness evidence.

## Hard packet: FileSystem encrypt transform

This packet reviewed the central write-side transform at:

- `0x0044B220-0x0044B352`, 307 bytes

This was selected because it is directly downstream of the previously reconstructed `CompressData` owner in two real save paths and is the inverse write-side partner of the already reviewed `FileSystem::Decrypt`. It is a central owner seam, not a smallest-function or easy-exact packet.

### Physical extent

The physical boundary is target-local and closed:

- the reviewed Decrypt body ends at `0x0044B212`;
- alignment/padding separates it from the next body;
- Encrypt begins at `0x0044B220`;
- its terminal `ret 0x14` occupies `0x0044B350-0x0044B352`;
- `0x0044B353-0x0044B35F` is `CC` padding;
- the reviewed `OpenFile` body begins at `0x0044B360`.

The maintained denominator extent therefore remains exactly 307 bytes. No padding was absorbed into the function.

### Machine boundary and behavior

The target machine boundary is:

- live-in AL: initial XOR seed;
- five stack arguments: data, size, XOR increment, chunk size, max bytes;
- `ret 0x14`.

This is recorded only as an observed machine boundary. It is **not** a source-level calling-convention claim.

The target transform:

1. computes `copySize = min(size, maxBytes)`;
2. allocates that many scratch bytes and returns the original data pointer unchanged if allocation fails;
3. computes the same small-tail rule used by Decrypt: a remainder smaller than one quarter of the chunk is left outside the transform, plus one byte for odd total size;
4. copies the bounded prefix into scratch;
5. for each transformed chunk, reads scratch in reverse odd/even order (`end-1, end-3, ...` and then `end-2, end-4, ...`), XORs with the evolving key, and writes sequentially back to the original buffer;
6. advances the XOR key by the supplied increment after every byte;
7. frees scratch and returns the original buffer.

This is the data-motion inverse of the reviewed Decrypt path, which reads the scratch prefix sequentially and scatters bytes into the corresponding reverse odd/even positions.

The target does not clamp the per-iteration chunk size to the remaining `maxBytes`; all observed real callers use `maxBytes == size`. The maintained source preserves the observed algorithm rather than introducing an unobserved safety rewrite.

Target callees for the transform are `malloc` and `free` only.

### Real caller profiles

There are three direct calls from two target functions:

- replay-save writer candidate `0x00429B60-0x0042A1F0`:
  - call at `0x00429E7D`: seed `0x3D`, increment `0x7A`, chunk `0x80`, `maxBytes == size`;
  - call at `0x00429E91`: seed `0xAA`, increment `0xE1`, chunk `0x400`, `maxBytes == size`.
- score-save writer candidate `0x0042B1E0-0x0042B413`:
  - call at `0x0042B335`: seed `0xAC`, increment `0x35`, chunk `0x10`, `maxBytes == size`.

Both callers remain `unknown/review`; this packet does not promote their origins, names, source ownership, or exactness.

### Maintained source

`src/FileSystem.hpp` now declares `FileSystem::Encrypt` beside `FileSystem::Decrypt`.

`src/FileSystem.cpp` now contains a natural maintainable implementation matching the target-local transform semantics. It uses ordinary C++ source only. It contains no target bytes, fake returns, inert padding, private-register calling-convention annotation, source-level `__stdcall` assertion, or exact-mode local-variable arrangement.

There is no target string or symbol evidence binding this body to an original identifier or translation unit. `FileSystem::Encrypt` is a maintained reconstruction name supported by target-local inverse behavior and later adjacent-source corroboration; it is not an original-symbol/TU claim.

## Adjacent-game hypothesis provenance

Adjacent source was consulted only after the TH10 boundary, ABI, algorithm, and caller profiles were established.

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch `port/portable-64bit`, clean. No material FileSystem implementation was used.
- TH09: HEAD `1173d2f0f11c3abeaab0af73dbd8aee73c5d0d8f`, branch `main`, clean tracked state. Only committed `src/FileEncrypt.cpp`, `src/FileDecrypt.cpp`, and `src/FileSystem.hpp` were consulted. They corroborate naming/signature/inverse structure, but TH09 allocates a separate output buffer and therefore does not define the TH10 implementation.
- TH095: HEAD `50d930ebe3ec98f9bcd3f4ca16a5b83fc73a43c6`, branch `main`, tracked clean with unrelated untracked files (`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, `scripts/runtime-diff.py`). None of those untracked files was read. Only committed `src/FileSystem.cpp` was consulted; its bounded scratch/in-place shape corroborates the already recovered TH10 behavior.

No adjacent address, extent, object owner, translation-unit owner, ABI, exactness, or completion state was transferred into TH10. TH10 target evidence remains authoritative.

## Compiler and exactness feedback

The updated tracked `src/FileSystem.cpp` compiles successfully with the pinned VC7.1 SP1 candidate under:

- fixed normal profile: `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`;
- the same tracked source and profile plus `/GL`.

The target-bound normal-COFF diagnostic for Encrypt reports:

- target owned extent: 307 bytes;
- object owned extent: 307 bytes;
- comparable non-relocation bytes: 299;
- matching comparable bytes: 3;
- result: mismatch.

The current Decrypt body was re-probed from the same object to guard against an `objdump` section-size misinterpretation:

- target owned extent: 323 bytes;
- object owned extent: 323 bytes;
- comparable non-relocation bytes: 315;
- matching comparable bytes: 13;
- result: mismatch.

The repository comparator therefore remains the authority for owned function extent; raw `objdump` section-size output is not used as a target extent oracle.

The equal 307-byte Encrypt extent is useful source-shape feedback only. The target live-in-AL boundary and broad register/code-shape mismatch do not establish normal-COFF physical ownership. The same source compiling under `/GL` keeps an interprocedural/LTCG hypothesis viable, but there is no target-bound linked-image LTCG exactness workflow here. Physical owner, production compile profile, original TU, and exactness remain unknown.

No declaration-order, local-layout, calling-convention, padding, or target-byte tuning was attempted after the natural-source mismatch.

No row was added to `config/matches.csv`; no match unit was added; `config/build.toml` remains unchanged and honestly open.

## Durable ledger changes

The existing denominator candidate at `0x0044B220` was promoted from `unknown/review` to reviewed authored/source-present only after the boundary, caller, ABI, behavior, and compiler checks above.

Durable updates include:

- `config/functions.csv`
- `config/function-origins.csv`
- `config/reccmp-functions.csv`
- `config/implemented.csv`
- `docs/KNOWLEDGE_BASE.md`
- generated `docs/PROGRESS.md`
- generated `resources/progress.svg`
- `src/FileSystem.hpp`
- `src/FileSystem.cpp`

`FILESYSTEM-005` records the transform and real call profiles. `TOOLCHAIN-010` records the normal and `/GL` diagnostics without promoting ownership or exactness.

Current reconstruction status after this packet:

- tracked candidates: **1,229**
- origin/boundary pending: **1,149**
- reviewed authored: **74 functions / 13,864 bytes**
- exclusions: **6**
- source-present mappings: **47**
- canonical exact: **0 functions / 0 bytes**
- canonical match units: **0**
- whole Windows i386 build: **open**
- runtime validation: **not started**
- semantic reconstruction / ports: **not started**

Packet delta:

- candidates: +0
- pending: -1
- reviewed authored: +1 function / +307 bytes
- source-present: +1
- exclusions: +0
- exact: +0

## Validation planes

Focused ledger validation has passed after the edit:

- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/report-reconstruction-status.py`
- explicit uniqueness/mapping invariants for 1,229 function rows, 1,229 origin rows, and 47 source mappings/implemented names.

The exact/build control plane remains unchanged:

- `config/matches.csv`: no diff
- `config/match-units.toml`: no diff
- `config/build.toml`: no diff

The final checkpoint gate must continue to treat source presence, exactness, whole-build closure, runtime closure, and Factory acceptance as separate states.

## Truth Kernel acceptance

A Truth Kernel accepted-snapshot query succeeded during this packet. The accepted snapshot remains bound to source commit:

- `9f7886f72adcd1880c05e34c62408cf9b1dad966`

It contains exactly one accepted fact and one accepted claim: the existing `target_attested` fact for `target:th10-main`, accepted from `verify-target.py`.

There is **no accepted FileSystem Encrypt fact, source-presence fact, boundary fact, compiler-profile fact, ownership fact, or exactness fact** from this packet. The accepted snapshot is therefore older than the current source work and must not be used as acceptance evidence for this reconstruction.

## Analysis artifacts

Campaign directory:

- `.analysis/gpt-web/20260912-filesystem-encrypt/`

`.analysis/` sizes observed in this session:

- entry: 103,413 bytes
- peak: 119,224 bytes
- compact pre-checkpoint state: approximately 108 KiB; live `du -sb .analysis` is authoritative for the final closeout value

Retained campaign artifacts:

- `compiler-shape-report.txt`: compact target/compiler diagnostic
- `manifest.json`: session-scratch lifecycle and input binding

Removed only after compacting their results, because they were current-session, reproducible, and exclusively owned by this campaign:

- `filesystem-normal.obj`
- `filesystem-ltcg.obj`
- `encrypt-normal-compare.json`
- `decrypt-normal-current-compare.json`
- `decrypt-normal-current-compare.stderr`

No prior/shared analysis tree, target, Ghidra project, Wine prefix, toolchain, or unknown artifact was removed. No campaign artifact exceeded 64 MiB.

## Engineering/transport events

Several Factory transport failures occurred and were handled under the non-transactional recovery rule:

- Long guidance output paging failed multiple times. Live repository status was re-read before retrying; remaining documents were read with smaller bounded calls.
- Two transient repository-status calls failed before a later status call succeeded; no target-dependent work continued until live status was recovered.
- The first source-edit command failed at the transport layer. A live-status read showed the worktree still had no tracked edits from that command before the edit was retried.
- The first combined comparator command exited at the first Encrypt mismatch because the comparator uses nonzero status for mismatch under `set -e`. Live status was recovered before inspecting the generated diagnostic, and Decrypt was then run separately with mismatch status explicitly allowed.
- One attempted repository-relative semantic-guidance path did not exist; after live-status recovery the correct Factory-root document was read.
- During closeout, `origin/main` advanced externally from the entry value `cfe26df8d99bbaeb82e93275141608482a2026c2` to the session starting commit `c4d329828015d8d23e58d6f6c33f232e3fb542ec` while local HEAD had not moved. The remote-tracking reflog records `2026-09-12T23:20:44+08:00 update by push`. This session did not run or request a push. The relation therefore changed from ahead 1 / behind 0 to ahead 0 / behind 0 before the local packet commit; the local packet commit then restored ahead 1 / behind 0.

These are engineering events, not target facts. No failed operation was interpreted as a successful build, exactness result, acceptance result, or repository mutation without inspection.

## Checkpoint discipline

The intended local checkpoint subject is:

- `gpt-web: reconstruct TH10 FileSystem encrypt transform`

The local Git HEAD containing this handoff is the authoritative checkpoint identity once the commit is created. This handoff intentionally does not embed a self-referential final commit hash. At closeout the upstream is `origin/main = c4d329828015d8d23e58d6f6c33f232e3fb542ec`; the local packet checkpoint is expected to be ahead 1 / behind 0. Nothing is to be pushed.

## Preferred next hard packet

Preferred next packet:

- replay-save writer candidate `0x00429B60-0x0042A1F0`, 1,681 bytes, currently `unknown/review`.

It is preferred over an easy leaf because it directly owns the write-side chain already exposed by target evidence:

- staging/copy logic;
- `CompressData`;
- `FileSystem::Encrypt` pass 1;
- `FileSystem::Encrypt` pass 2;
- subsequent file output and metadata handling.

The packet should challenge the Ghidra boundary, recover the full machine ABI and data/object owners, identify error/cleanup paths, and only then decide whether a maintained replay-save source owner is supportable. The directly connected score-save writer `0x0042B1E0-0x0042B413` (564 bytes, also `unknown/review`) is a useful secondary comparator because it invokes `CompressData` and the same Encrypt transform with a different profile.

This keeps the frontier on a materially larger central caller/owner seam rather than returning to the easiest remaining function. Exact reconstruction remains incomplete after this handoff.
