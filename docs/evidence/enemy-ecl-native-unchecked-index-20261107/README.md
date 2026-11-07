# TH10 Enemy ECL indexed gauge, drop, slot and callback records

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EnemyRuntimeView::DispatchEclInstruction @0x0040E770` is a **14,416-byte
original boss**. The actual eleven-source VC7.1 SP1 `/GL /GS` candidate
has **14,352 bytes with 716 candidate address fields**, still NONEXACT.

## Source semantics recovered

Unchecked ECL indices select the typed boss gauge segment (+0x9E94 in Gui),
raw drop DWORDs (+0x13CC in runtime), two special-enemy pointer slots in
`EnemyManagerView`, and a timeout callback record (+0x1458 in runtime).
The old C++ indexed these bounded array declarations with script-provided
values even when the source does not establish a bounds constraint. C++
pointer arithmetic beyond those declared arrays is undefined; original
x86 constructs the low-32-bit address word by multiplying the index by the
record width and adding the base. The installed `EnemyNativeWordIndex<T>`
helper expresses that native i386 word calculation before forming the typed
pointer. It is used at all six publication sites. Original read/write
order, field offsets and element types remain unchanged; no new bounds
check, fallback, allocation, fake ABI or target-byte patch was added.
Out-of-range accesses are **not** made safe by this change.

## Fresh native graph and narrow exact regressions

An initial trial explicitly rejected a stale eleven-TU support cache:
`src/AnmManager.cpp` and `src/EnemyEclDispatcher.cpp` no longer matched
its earlier pins. The current-source base rebuilt both affected support
objects and revalidated all source/header hashes before further trials.
Individual gauge, drop, slot and timeout rewrites, then all six together,
were separately compiled and linked with the *same* remaining ten actual
current-support objects. The installed combined source was linked again.
The complete PDB owner code and candidate code-field geometry compare:

| Complete candidate owner | PDB bytes | Candidate fields | Non-field changes vs current baseline |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | **0** |
| `EnemyBeginSpell` | 2,432 | 108 | **0** |
| `EnemyFinalizeDeath` | 162 | 9 | **0** |
| `EnemyConfigureViewportOwner` | 330 | 10 | **0** |
| Full-object constructor | 672 | 5 | **0** |

Candidate fields are *not* independently original-bound field obligations.
Original dispatcher length remains 14,416; zero new exact credit. An
initial malformed timeout pointer-to-struct test failed to compile, was
fixed to typed `->timerCallbackName` and then completed independently.

From the **installed** source a separate selected cold replay verified only
five previously accepted ECL argument units: `enemy-read-int-argument`,
`enemy-read-float-argument`, `enemy-resolve-float-argument`,
`enemy-read-raw-int-argument`, `enemy-read-raw-float-argument`.
All pass **87/87** original-target bytes and their declared call fields.
No full historical replay, game product or runtime behavior is claimed.

Reproducible private evidence:
`.analysis/gpt-web/20261107-turn4/enemy-native-v3.py`,
`enemy-unchecked-index-trials.py`, `enemy-index-results.json`,
`enemy-native-index-guards.json`; current actual PE/MAP/PDB under
`build/gpt-web/20261107-turn4-enemy/installed-native-index/`.
