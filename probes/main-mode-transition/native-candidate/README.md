# Conditional native mode-transition candidate

This checkpoint preserves the independently authored 572-byte-owner source proposal as three small patches and one shared header. It builds on the existing input-native a003 checkpoint. It does not contain target bytes, native artifacts, full collector certificates, or duplicate copies of Main.cpp.

The natural definition is `int MainOpaqueUpdate4218D0(MainSupervisorView*)` in Main.cpp, reached through the genuine input callback. Main.cpp remains the primary translation unit with Input.cpp as the only /GL support unit. The /GS profile stays fixed. GameManager.cpp is a declaration-extraction overlay for coherent ownership; it is not added to that compiler graph.

## Reconstruct and check

From the repository root, first reconstruct the existing input checkpoint to a new directory outside the repository:

    python probes/input-native/reconstruct.py --base-tree . --output /tmp/input-a003-source

Then reconstruct this candidate to another absent directory:

    python probes/main-mode-transition/native-candidate/reconstruct.py --base-tree . --a003-tree /tmp/input-a003-source --output /tmp/main-transition-source
    python probes/main-mode-transition/native-candidate/test_reconstruct.py --base-tree . --a003-tree /tmp/input-a003-source
    python probes/main-mode-transition/native-candidate/check_shared_layout.py --baseline-root . --overlay-tree /tmp/main-transition-source
    python probes/main-mode-transition/host-candidate/run_tests.py

Choose fresh output names if those paths already exist. Reconstruction verifies exact raw inputs, packet hashes, zero-fuzz patches, final hashes and newline identities before creating the output directory. It never edits the source repository. Main.cpp is LF; the other reconstructed C++ files are CRLF. The checked-in GameManager.hpp uses a narrow binary transport attribute to preserve its reviewed CRLF bytes.

The layout check uses g++ -m32 syntax-only compilation and small standard-header shims. Calling-convention annotations are erased for this layout check only. It verifies declaration extraction and 32-bit field layout, not MSVC ABI compatibility.

## Evidence and limits

See CONTRACTS.md for the conditional semantics and interface hypotheses, and RESULTS.json for the compact actual compiler outcome. Host tests cover 666 scenarios and reject 15 compiled source mutations. None of these checks establishes complete target effects, runtime lifecycle validity, original helper declarations, exact production ownership, or native equivalence. The diagnostic compiler result carries no exactness credit.

## Actual compiler baseline

The single a004 build compiled successfully. Corrected read-only measurement 27b6c7edd44c47b29c8924e98a830d59 established complete attribution for all four owners, with all 19 required gates passing. Candidate/target sizes are: mode transition 630/572 bytes, input callback 101/113 bytes, keyboard 745/850 bytes and mapper 709/833 bytes. None is exact, and runtime execution was not performed.

The initial attribution reader rejected unrelated module-path normalization. The corrected reader measured the same pinned executable, map and PDB; this recovery did not rebuild or change the source. RESULTS.json records the compact measurements and artifact identities.
