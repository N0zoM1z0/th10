# Native input source checkpoint (a003)

Diagnostic source only. These owners are **not byte-exact** and earn no exactness
credit toward the 90% goal. No runtime execution, relocation acceptance or
production integration has occurred. Keep the immutable a001/a002/a003 experiment
packets and full binary/MAP/PDB/log evidence in their existing remote locations.
This package deliberately retains no generated native artifacts or tool copies.

## Reconstruct and test

Requires Python 3, GNU-compatible `patch`, and `g++` for the finite key tests.
From the repository root, use an existing parent directory outside the repository:

```sh
python3 probes/input-native/reconstruct.py --base-tree . --output ../input-a003-overlay
python3 probes/input-native/test_reconstruct.py --base-tree .
python3 probes/input-native/run_tests.py
```

The output must be absent and separate from both the base tree and this package.
Only five source overlays are created, not a complete build tree. The script first
checks the raw Main.cpp/Main.hpp SHA-256 values in `manifest.json`, applies the
compact patches with zero fuzz to LF-canonical temporary copies, restores final
newline conventions and verifies all five final raw hashes before writing.
Main.cpp stays LF; Main.hpp and the three Input files stay CRLF. No source or
production file is edited. Do not let checkout newline conversion alter the three
stored Input files; the final hash check fails closed if that happens.

Pinned base: `6892fa5dbe5cae64c3f6ac4bea187ea83145a5ec`. Main.cpp raw SHA-256:
`fcf7ba4fa5b6af9ce447aa77f2da58eae3c2d53f52b1666b592e9c86b41a40a4`.
Main.hpp raw SHA-256:
`ae49087657d26b2068e9ad73e6994f5fbf1efaebbafed5fad92e508c7938cb03`.
The patches target those production-baseline files, not the intermediate schema
header used during callback derivation. The manifest pins remaining dependencies,
validation inputs, symbols and final source hashes.

`run_tests.py` extracts the two actual 19-condition blocks from the shipped
Input.cpp, compiles those extracted conditions and compares them with an independent
key oracle. It checks 270,848 finite cases and eight semantic mutants, requiring
actual mismatches rather than compile failures. This is not a full Input.cpp native
compile, SDK/ABI proof, mapper/callback test or universal input-state proof.

## Diagnostic build graph

Reuse the existing pinned `scripts/ltcg_link.py` cold-link driver in a separately
prepared isolated diagnostic workspace, with the existing pinned VC7.1 SP1 tools.
Main.cpp is primary; Input.cpp is the sole /GL support source; entry is
`?MainInputCalcCallback@@YIHPAX@Z`. Driver call shape:

```python
ltcg_link.cold_link(main_cpp, isolated_build_dir,
    '?MainInputCalcCallback@@YIHPAX@Z', linker, env,
    ['/TP', '/MT', '/O2', '/Gy', '/GF', '/Oi', '/DNDEBUG', '/Isrc', '/GL', '/GS'],
    [input_cpp], [])
```

The path variables denote the prepared isolated tree and existing tool references;
this is documentation, not turnkey remote build automation. Check all manifest
build-input/validation hashes before reusing that driver. Exclude CL and _CL_
environment switches. Never copy toolchains/target images into this package or
run its diagnostic images: data anchors and unresolved dependencies are harness
objects, not a runnable game. Owner identity requires unique complete same-start
MAP/PDB contributions, using the manifest's owner symbols and object names.

The sole a002→a003 experiment changed the profile by explicitly adding /GS. Source
bytes, graph, signatures and owner selection were unchanged. No annotation,
padding, synthetic registration/retention wrapper or register adapter was added.
See `CONTRACTS.md` and `RESULTS.md` before interpreting these measurements.
