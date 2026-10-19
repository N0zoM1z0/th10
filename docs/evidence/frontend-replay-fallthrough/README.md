# FrontEnd Replay initialization fallthrough (2026-10-18/19)

## Disposition

Parked, unadopted source hypothesis. Removing one INITIALIZE break lets the OPENING predicate run on the same invocation. The cold pair, structural anatomy and two retained-image regression guards are complete, but no target fallthrough/semantic proof or Replay exactness gain is established. The separate CFG observation is terminal UNKNOWN and closed. Do not retry consumed runs or use this archive as build/deployment authority. No production change, Factory acceptance or gameplay validation follows.

## Source hypothesis

The only change removes `        break;\r\n` immediately after replayListOffset=0 in FrontEndControllerView::UpdateReplay: 16 bytes at baseline offset 72257, line 2098. Initialization calls, unconditional FindClose on invalid search, replay ownership, ABI, helpers, other states and every other raw byte remain unchanged. No upstream attributes or helper implementations were imported.

The baseline CPP is c26c9c1c00b3caa98a01fc210efdbb898fe6e8cace5a4d84a8072cd3f8a90d5d; reconstructed candidate is d50c95e092e8eba62f77ee25f2abf16a812ac1e5f21708edadcb1cb54c2681f7. Header 8b4e0e13312bce0b9bab71e16daff79cfa402d55a61e55cca53bd359edc7832b is unchanged. Full source/header copies are not included.

## Scoped observations

The baseline-first cold pair retained complete same-start PDB contributions: both 1,488 bytes versus the known 1,492-byte target physical owner. There are 228 unclassified positional baseline/candidate differences, no unmatched tail, and unequal raw contributions. These are not target mismatches or a target matching percentage.

A later bounded anatomy census found one unique structural layout per owner. Baseline: 1,464 decoded-prefix bytes plus 24 table bytes, 391 instructions, 53 calls. Candidate: 1,464 plus 24, 390 instructions, 53 calls. Target: 1,466 decoded-prefix bytes plus two reviewed alignment bytes plus 24 table bytes, 371 instructions, 47 calls. Each has one indexed tail reference and table targets at internal decoded instruction starts. This is linear decoding and structural-pattern evidence, not verified executable partitioning, reachability, gap safety, linkage classification or semantics.

Both canonical regression guards passed on the unchanged retained candidate image, without rebuilding:

- frontend-keyconfig-display: 2,452 matched bytes, complete normalization across 61 linkages; raw_equal=false.
- frontend-practice-unlock: 93 matched bytes, zero linkages, complete normalization; raw_equal=true.

The successful guard receipt has null primary and postcheck failure phases. These scoped existing-function passes are not Replay exactness, source adoption or Factory acceptance. They do not change the earlier cold pair's operational result or close the unknown CFG observation. Receipt identities, hashes, image binding and exact limitations are in evidence.json.

An initial metadata-only preflight failed with a finally_postpins label that did not establish the primary cause. A separately retained local mechanics reproduction found a narrow /dev/null audit rejection during the fixed Git subprocess constructor. A device/flags/constructor-scoped correction plus separate primary/postcheck labels enabled the later metadata preflight. This operational history is summarized and hash-bound; raw receipts, runtime packets and private logs are excluded.

## Source-only reproduction

From this directory:

    PYTHONDONTWRITEBYTECODE=1 python test_candidate.py --baseline-root /absolute/baseline/root

The explicit root must contain the two pinned src files. Reconstruction is in memory only; it rejects changed pins and ambiguous context and verifies exact inverse restoration. It neither writes source nor executes native code. candidate.patch retains CRLF source/context lines and LF diff metadata; its -text attribute prevents Git newline conversion. Do not normalize it.

Seven tests validate byte-exact reconstruction, inverse baseline identity, patch, unchanged header, CRLF, rejection paths and import-time source-I/O behavior. Abstract witnesses cover -1/0/6/7 and signed 32-bit boundaries, initialization ordering/counts, and callback-style mutation before the opening predicate. Without mutation, both versions finish initialization at timer zero. The injected mutation is a witness input, not a claim real file APIs advance timers. Other states are uninterpreted; unchanged source bytes establish their preservation. The model is not Win32, complete initialization, ownership execution or gameplay validation.

## Provenance and limits

Authored inspiration: levzzz5154/th10-decomp, commit 4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a, matching2/msvc/src/game/title/replay_select_update.cpp:
https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/msvc/src/game/title/replay_select_update.cpp

The documented case-zero fallthrough inspired our original one-line deletion and abstract model/tests; no reference implementation was copied. The exact same-commit root MIT text is verified against Git blob 0706df312064fec49fd1a57b311cb92d888acc2d and preserved with attribution in REFERENCE_LICENSE.txt. The ANM bundled-notice hash is distinguished from the raw root LICENSE hash in provenance.json. The licensing provenance gate is now resolved explicitly: the directly fetched exact-commit Replay source (Git blob f70d8c4ce048ccc57811a86968a6e7f56cabf808) has no file-local licensing notice, and the complete cached Git tree (11,725 entries, truncated=false) has only the root LICENSE among license/copying/copyright/notice filenames directly along the six ancestor directories. No overriding notice was found. Full source, response and tree hashes and the precise check scope are in provenance.json. The prior open-gate language is superseded; REFERENCE_LICENSE.txt is retained byte-for-byte, so its earlier applicability caveat is historical and superseded by this check. This establishes the recorded source-provenance disposition, not a broader legal warranty or deployment authority. No new license is assigned by this archive.

No binary/target bytes, executable artifacts, native runners, full reference/source copies, process inventories, or reserved execution capabilities are included. This package is separate from the ShotType arithmetic candidate. Its completion does not authorize deployment or modifications to current repository handoff documents.
