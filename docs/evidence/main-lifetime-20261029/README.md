# Main lifetime and complete shutdown — 2026-10-29

Five complete owners add **518 exact bytes:425 authored and93 indeterminate**.
Two independent canonical30-input colds reproduce every byte and all42 fields.
One previously omitted22-byte destructor is inventoried without changing the
reviewed authored denominator. Six source mappings646 include a complete128-byte
mode-release candidate that remains nonexact. [Results](results.json) retain
source/include pins, profiles, PE/MAP/PDB identities and the comparison scopes.

| Complete owner | Bytes | Fields | Selector |
| --- | ---: | ---: | --- |
| Main construction421F00 | 71 | 2 | `main-constructor` |
| Main member destruction41FF60 | 22 | 2 | `main-destructor` |
| Main game shutdown420270 | 293 | 25 | `main-release-game` |
| FPS deletion413450 | 84 | 10 | `main-delete-fps-counter` |
| MIDI timer destruction4203C0 | 48 | 3 | `midi-timer-destructor` |

Static initialization4658F0 constructs the actual Main global491C28 and registers
4659F0, whose jump at4659F5 reaches41FF60. All22 byte reference queries find only
that entry escape;15 preceding/10 following CC bytes separate the owner. Explicit
source lifecycle versus generated member destruction remains indeterminate.
The constructor's existing origin-indeterminate classification is also retained.

The constructor initializes configuration+120 before worker+62C, clears784h bytes
and sets flags140h. A real GameConfigurationView member replaces the POD union;
Main, FrontEnd, Sound, Hint and Startup field access now uses that member. A local
packing4 view preserves every asserted offset and the observed clear extent.
The clear alone does not establish the original complete object size, tail
padding, allocation stride, alignment or packing declaration. The native global
is a consumed reconstruction view; production storage ownership remains open.

Shutdown stops the scan worker and Main mode worker, requests sound stop, frees
buffer+764, releases five mode owners and the FPS owner, then releases the ANM
quad buffer, queues the actual dummy command and releases text resources. It
preserves separate global and receiver ownership for input interfaces and timer
storage. The timer stop is a real member operation: capturing timerId as a local
reference still gave291/293, while the actual member context gives every293 byte.
The destructor remains outlined at its observed call and executes both period-end
calls. Together with shutdown's StopTimer, that preserves all three target calls.

FPS deletion uses draw element+C, the existing Chain member and critical section0
with its byte counter. It clears the477708 pointer shared with replay timing before
freeing the captured owner. The prior PbgArchive Release recipe supplies its pinned
`/GL /EHsc /Ob1` profile and reproduces the real86-byte callee in this graph; no
foreign flags or register ABI declarations are introduced.

Each new cold also compares18 selected old contracts8415 bytes/all371 fields,
including modified configuration readers/UI, window/FPS/Sound bodies and directly
used lifetime interfaces. FrontEnd CreateVm and the Hint creation interface are
replayed separately with their original canonical recipes. The new30-input graph
emits CreateVm90/95 and OpenFile312/360: these are recorded nonmatches, and their
address bindings do not grant callee-byte credit. Existing exact recipes remain
separate contracts; this is not a whole-program closure or historical cohort pass.

The full Options controller is now measured at3196/3188, including its five-entry
local table. Changing its static explicit receiver to a natural member is a
complete native-to-native byte-neutral control and is restored. WinMain improves
1438 to1422 against1389 after restoring the actual shutdown call, but remains
nonexact. Mode release133/128, configuration reset125/125 and GameManager cleanup
1120/1105 also remain nonexact. A GameManager destructor-role attempt did not
improve that complete size and was restored. No partial exact credit is given.

Fresh inspection contradicts the old window-initialization label for438880:
WinMain passes the48-byte Chain, stops Main's worker, removes both sentinel lists
and clears their callback slots under an EH frame. This103-byte destructor is the
next concrete missing WinMain source lead; original identifiers/TU remain unknown.

Replay only the selected new owners with repeated explicit `--unit` arguments:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit main-constructor --unit main-destructor --unit main-release-game --unit main-delete-fps-counter --unit midi-timer-destructor --json
scripts/repo-python scripts/ci.py
git diff --check
```

No comparator change, host scenario, product/runtime or portability claim is made.
