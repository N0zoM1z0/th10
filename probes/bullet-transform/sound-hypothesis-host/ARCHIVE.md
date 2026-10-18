# Replaying the unadopted typed-sound hypothesis

From a complete repository checkout:

    python3 probes/bullet-transform/sound-hypothesis-host/replay.py --mutations

Python 3.10+ and host g++ with C++17 support are required. This runs the frozen host baseline and all 52 designated semantic mutation controls. It invokes no VC7.1, Wine, native build driver, target read, or remote service.

The five historical files run.py, harness.cpp, README.md, manifest.json and .gitattributes are retained byte-for-byte. Their original README is historical documentation; its direct run.py command and relative default source path must not be used from this archive location. The new wrapper always supplies explicit --source-dir and --mutations.

The wrapper reads the existing immutable A003 archive plus the pinned recovery-history reconstruction utility/recipes. It reconstructs A007 in memory and verifies the 824e overlay hash. Only that source and pinned unchanged production BulletManager.hpp/EnemySoundQueue.hpp are written to an external temporary source directory. The five frozen host files are copied to the same external workspace, and their host run.py executes there. The reconstructed historical native runner and native manifest are never written or executed. Production BulletTransform.cpp is never read or changed. Temporary files are removed after success or failure.

Archival inputs use exact raw hashes. Header checks permit only CRLF-to-LF normalization and reject bare CR, matching the frozen host runner. The temporary copies retain the original header bytes. TMPDIR must resolve outside both the selected repository and this archive directory; an internal temporary root is rejected. No files are generated in the archive or production tree.

For a local composite checkout, add --repo /absolute/path/to/composite. That root must contain the two production headers, the three existing A003 inputs, and recovery-history/reconstruct.py plus recipes.json. There is no fallback to unpinned files or current production transform source. No source/header copies are added to this archive package.

Source 824e is an unadopted hypothesis. Its A007 native diagnostic failed the canonical 885-byte regression guard (runtime 878 bytes). The later A008 altered-graph experiment also failed (runtime 884 bytes, transform 1612 bytes). These host checks neither repair those failures nor establish target ABI, native semantics, original ownership, or exactness. Historical execution-ready metadata does not authorize another native run.

Wrapper-only tests, with no compiler invocation:

    python3 -B -m unittest discover -s probes/bullet-transform/sound-hypothesis-host -p 'test_replay.py'

When testing a partial local archive, set SOUND_HOST_ARCHIVE_REPO to the composite checkout. The wrapper test suite checks exact frozen inputs, reconstruction, source-read scope, CRLF handling, fail-closed tamper/symlink rejection, explicit host invocation and temporary-workspace cleanup. archive-inventory.json separately records frozen files, wrapper files, dependencies and their policies. It does not modify the historical manifest.
