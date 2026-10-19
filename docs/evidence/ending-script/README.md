# Ending controller and text interpreter (2026-10-20)

`Ending.cpp` / `Ending.hpp` recover all ten reviewed authored owners in this
subsystem: **3,889 owned bytes now have maintained source**. Six complete
functions pass two independent native cold links: **303 authored exact bytes**
and **46 origin-indeterminate exact bytes**. Source presence and exactness remain
separate. The 1,568-byte interpreter is source-present and nonexact.

## Target, boundary and hypotheses

The input is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local read-only Ghidra checks the target identity and complete mapped text
before/after analysis; temporary callback functions are rolled back. Raw target
control flow, references, neighboring entries and padding establish ownership.
Names and original production translation units remain unknown.

The guarded indexed jump at `0x40BDF0` consumes 18 absolute destinations from
`0x40C358..0x40C39F`. All destinations belong to Run, and the next callback starts
at `0x40C3A0`. Run's complete physical extent is **1,568**, comprising 1,488
reachable bytes, six internal alignment bytes, two tail alignment bytes
(`MOV EDI,EDI`), and the 72-byte table. The former 1,494-byte extent omitted the
tail/table. This correction adds 74 bytes to the authored denominator without
granting exactness. The original executable has **no PE base relocations**;
table ownership follows the guarded control flow and references. Candidate
linked images use `/fixed:no` and have their own relocation inventory.

Two previously unmapped callbacks are separately reviewed: update forwarding
at `0x40BA70` (10 bytes), referenced by initialization, and background loading at
`0x40C3A0` (18 bytes), referenced by opcode 7. Their mechanical wrapper shapes
leave authorship indeterminate. The existing nullable delete wrapper at
`0x40B9D0` (18 bytes) also retains its indeterminate origin.

The fresh global boundary checker reports a pre-existing metadata discrepancy
at `0x401100`: the reviewed two-byte extent is unchanged, but its evidence ID
and notes differ between the recorded and generated inventories. No checker
weakening or bulk regeneration is performed. These Ending rows instead have
focused raw-target and fresh Ghidra ownership reviews.

The MIT reference `levzzz5154/th10-decomp`, pinned at
`c6791d82dc9f02e55eca45a5314f900a85481efd`, supplies source hypotheses only.
Its license is retained at `docs/licenses/th10-decomp-MIT.txt`. Reference volatile
aliases, forced helper controls, assembly and explicit return-output stack
slots are not adopted. TH10-local observations decide the maintained behavior.

## Maintained contracts and shared layout

Records have a four-byte header and a byte-sized payload length at +3.
Only integer-bearing operations require integer operands. A terminating or
unsupported header-only record must not acquire an artificial eight-byte size.
Run retains the observed stack receiver and RET4 interface.

The controller selects the ending message, registers real calc/draw callbacks,
creates five text VMs, interprets text/waits/animation/music/effects/reloads and
performs terminal cleanup. Bank loading at `0x40C3C0` is a background loader,
not a drawing routine. Callback-sensitive cursor and buffer reads remain live:
first-line decoding follows intervening drawing, animation destination selection
follows VM creation, reload continues with the first new command, LoadScript
opens before freeing the current member buffer, and background loading samples
the destination cursor after PreloadAnm. Pressed-edge and held input are distinct.

Ending allocates 0xEC bytes with the shared worker at +0xD0. Observed worker
operations consume through +0x18, establishing a 0x1C prefix. The former extra
unknown word moves into `ScriptViewerView` at +0x2C, preserving its established
native offsets and size. No worker method or ABI declaration changes. The
original complete worker type and the original animation-ID capacity remain
unknown; the views describe consumed intervals.

The target constructs the worker, then clears the complete text object,
including its vptr. Reload also clears the object. These native quirks are
retained; they do not establish valid modern C++ lifetime or runtime closure.
Ending globals, script-path table, message storage and real asset ownership
remain external and unresolved.

## Complete native proofs

Both cold links rebuild the same 17 maintained TUs under `/GL /GS /EHs` with
the historical pinned VC7.1 compiler/linker. Ending.cpp is primary, with real
EndingCreate entry and ordered supports recorded in the six canonical recipes.
All 49 source/header hashes agree between proofs. Complete PE/MAP/PDB ownership,
ordinary bytes and every declared link field pass the strict comparator.

| Canonical unit | Target | Complete bytes | Fields | Origin |
| --- | --- | ---: | ---: | --- |
| ending-create | 0x40B940 | 99 | 5 | authored |
| ending-delete | 0x40B9D0 | 18 | 2 | indeterminate |
| ending-update | 0x40B9F0 | 111 | 4 | authored |
| ending-update-callback | 0x40BA70 | 10 | 1 | indeterminate |
| ending-text-tick | 0x40BD20 | 93 | 5 | authored |
| ending-load-animation-callback | 0x40C3A0 | 18 | 2 | indeterminate |

Each image additionally passes complete worker Stop101, Start56, destructor16,
browser constructor234 and the actual outgoing ReleaseAnm55: **462 existing
guard bytes /11 fields**, with no duplicate credit or canonical recipe
migration. Combined validation is **811 complete bytes /30 fields per image**.
The new functions contribute 349 complete bytes /19 fields. Target field
bindings are reviewed independently of diagnostic normalization: Ending pointer
477700, Main base491C28 with flags+3CC /mode+390, input474E30, three timer
constants, existing allocator/delete/ftol2 and the genuine recovered callees.

The diagnostic carrier contains data anchors for unresolved dependencies and
is not executable product evidence. Init, destructor, text constructor,
LoadScript, Run and bank loader remain nonexact. An exact caller does not make
its nonexact callees exact. The actual Run-to-ReleaseAnm source edge is now
maintained; Run itself and original production partition remain open.

```sh
scripts/repo-python scripts/build-match-unit.py --unit ending-create
scripts/repo-python scripts/compare-linked-function.py --unit ending-create --json
```

The other five units share that build graph. Their full field manifests are
in `config/match-units.toml`; `native-replay.json` records both proofs and frozen
inputs. No full historical cohort is rebuilt.

## Host regression scope

`scripts/test-ending-state.py` compiles the actual Ending source and canonical
headers, alongside real selected VM, timer, worker, interrupt and decoder bodies.
Explicit host roles provide SDK calls, assets, allocation, rendering, music and
effects. The host clear preserves the live worker vptr while zeroing ordinary
fields; timer/node prefill is defined. Native layout assertions are omitted
only in the host copy. Simulated thread startup records its callback/context
without creating a thread. These are semantic regression seams, not native
lifetime, scheduling, ABI or target-runtime proof.

ASan/UBSan and leak detection pass the positive suite and four strict mutants:
stale text cursor, stale animation destination, skipped first reloaded command,
and held input replacing pressed-edge input. Each mutant must fail its named
observable with exactly the expected process result. CI runs the positive
suite; default temporary artifacts clean themselves up.

Product/runtime closure, dual-Oracle semantic reconstruction and portability
remain open. Existing D3D evidence is preserved untouched.

One separate private source-order control snapshots `backgroundPath` before
clearing the shared path buffer, following the target's initial load. It compiles
only that private Ending.cpp and reuses 16 hash-checked support objects, with all
49 maintained inputs checked unchanged before/after. Loader size remains
131/126; ordinary comparable positions improve from 61/98 to 69/98 without full
equality. This is neither a cold proof nor exact credit. Maintained source stays
unchanged. The remaining register/flag-write differences need fresh evidence;
repeating this path-only control cannot close them. Duplicate control outputs
are removed after recording hashes and the compact outcome.
