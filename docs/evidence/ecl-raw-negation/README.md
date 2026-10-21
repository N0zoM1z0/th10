# ECL raw float-word negation and operand homes

Started 2026-10-22 at `6b4e11d`. Opcode55 in Run negates the raw float word:
target44F851 is `NEG EDX`, followed by a four-byte Push with tag66h. This is
integer word arithmetic, including the payload80000000h, rather than floating
sign inversion. The original source signedness remains unknown.

The reconstructed signed expression `value.integer = -value.integer` overflows
at that valid negative-zero payload. An actual-source union/case projection
reproduces the UBSan INT_MIN-negation diagnostic. The maintained source now uses
the existing unsigned member: `value.bits = 0u - value.bits`. Unsigned modulo
arithmetic preserves the observed target operation for every raw word. No ABI,
layout, compiler flag, target byte or acceptance rule changes.

One full canonical ten-TU cold replays five accepted units for1172 exact bytes.
The same image passes35 complete guards for4424 bytes/all118 fields. Its three
complete PDB-bound large bodies are raw-identical to both the original baseline
and private positive: ANM9964, Run7020 and Enemy14352 bytes. The unmodified
maintained Run diagnostic still has27 ordinary differences, all189 fields/62
local destinations and the complete171-byte float callee exact. No new exact
credit is claimed.

The actual-source host test extracts the real union and opcode arm. Seventeen
literal edge payloads exercise34 invocations under ASan/UBSan/leak checking.
Sign-bit inversion, complement without increment and an integer Push tag each
fail their intended functional check; compilation, signals, sanitizer failures
or unrelated checks never count as functional detections. The explicit stack
seam initializes requested host storage and projects its first dword. Its unused
pointer may widen the host union; this does not prove native layout, stack
conversion/underflow, private ABI or game runtime. Existing FORMAT contracts
also pass10 scenarios/eight functional controls with sanitizers.

Nine native source controls were evaluated in the same original ten-TU graph:

| Control | Run ordinary differences | Result |
| --- | ---: | --- |
| Group LengthSquared declarations |27|Three large raw bodies unchanged |
| Reverse those declarations |27|Three large raw bodies unchanged |
| Put FORMAT counter in a for scope |27|Three large raw bodies unchanged |
| Share first geometric temporary |31|PointAngle homes recover; NEG_FLOAT gains six differences |
| Share second geometric temporary |27|Three large raw bodies unchanged |
| Share both / reverse declarations |31 /31|Same regression as sharing the first |
| Unsigned raw-word negation |27|Three large raw bodies unchanged; adopted |
| Share first plus unsigned negation |31|Sharing regression persists; rejected |

Each private control compiles one ECL object and reuses nine verified game
objects, then relinks; none is a full cold. Private source selection requires a
transparent diagnostic adapter changing only two source-path literals and ROOT
discovery. Target bindings, comparator, all fields,60 table words,88 selectors
and the whole callee checks remain unchanged. Final maintained-source results
use the unmodified diagnostic. [Controls and exact source patches](controls.json)
record this routing and each build scope.

[Current native pins](native-summary.json) supersede the current-source meaning
of earlier32-input ten-TU receipts. Old source/artifact hashes remain historical;
their original15 outputs were copied before the canonical path was rebuilt.
The older two-TU26-difference Run result is also historical source context,
not a fresh result for this unsigned edit. No extra replay is claimed.

Reproduce the maintained source checks with:

```sh
scripts/repo-python scripts/test-ecl-raw-negation.py
scripts/repo-python scripts/test-ecl-format.py --sanitize
scripts/repo-python scripts/replay-exact-units.py --unit enemy-drop-item-counts --unit enemy-drop-item-counts-core --unit enemy-finalize-death --unit enemy-cancel-bullet-record --unit enemy-cancel-bullet-pattern --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit enemy-cancel-bullet-pattern --artifact-dir build/match-linked/EnemyItemDrops
```

The last command returns1 for the complete nonexact Run. See
[target and host scope](target-and-host.json) and
[validation and cleanup](validation-summary.json).
Private recovery is `.analysis/gpt-6.1-sol/ecl-operand-homes-20261022/RECOVERY.md`.
Windows i386 product/runtime closure remains open; semantic reconstruction and
portability have not started.
