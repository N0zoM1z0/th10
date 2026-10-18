# Explicit pinned-source host replay adapter

This adapter leaves archived host script bytes unchanged. It verifies all four raw source dependencies plus frozen script, patch and manifest hashes, imports the scripts under non-main module names, changes only test_contract.SNAPSHOT and compiled_contract.BASE, runs the semantic suite and calls execute(). It never calls either original script main or writes the archived compiled result. Compiler outputs use the frozen harness's temporary directory and limits.

From the final archive, place this directory at host-replay/. Run:

    python host-replay/replay_host.py --repo-root /path/to/repository

Optional explicit older snapshot mode:

    python host-replay/replay_host.py --repo-root '/path with spaces/source snapshot' --source-snapshot

Optional integration identity gate:

    python host-replay/replay_host.py --repo-root /path/to/repository --require-head a4edda20e434cd40ee99ea3b6b68673a80c3e3be

Use --archive-root when the adapter is outside the final archive. Default host-only replay validates only EclVm.cpp, EclVm.hpp, Enemy.cpp and EnemyEclDispatcher.cpp. It does not claim the supplied root is the current repository, inspect unrelated source files, or require an old HEAD forever. --require-head additionally requires the specified exact clean Git HEAD. The native baseline runner has its separate strict full-source/HEAD gates.

Symlink paths and optimized Python are rejected; frozen file/source pins and archived result are checked before and after. Arguments use argv lists, including roots containing spaces. Host compiler remains the frozen /usr/bin/g++ command; no package installation occurs.

Portable synthetic controls:

    python -m unittest discover -s host-replay -p 'test_replay_host.py' -v

Actual local replay used an explicitly labeled older source snapshot whose four required raw files match. Eight semantic tests and seven compiled seam variants passed. This neither promotes that snapshot to current production nor establishes VC7.1 behavior, target equality, or authority to build the canceled candidate. actual-host-result.json contains the host-only report. No source copies or native outputs are packaged.
