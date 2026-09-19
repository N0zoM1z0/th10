# TH10 exact reconstruction handoff

Updated 2026-09-19. This file is the current recovery snapshot. Historical
observations belong in `docs/KNOWLEDGE_BASE.md`; accepted implementation history
belongs in Git. Do not append another chronological session transcript here.

## Authority and recovery

- Work only against original Japanese TH10 v1.00a.
- Canonical target: `resources/th10.exe`.
- Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Current branch: `main`.
- Last code-changing checkpoint before this handoff:
  `07e2a37 gpt-5.6-sol: restore Enemy integer adapter call graph`.
- `origin/main` was `cc7c686` when this handoff was written. Nothing in this
  campaign was pushed; recover the live unpublished distance with
  `git status --short --branch`.
- The adjacent reference repository is available at
  `/home/pentester/coding/codex_ida/th10-decomphelp-forN0`. It is hypothesis
  material only.
- The maintained source and ledgers are authoritative for repository state.
  Decompiler output, adjacent games and the reference repository never establish
  TH10 bytes, ownership or exactness.

Before editing, read `AGENTS.md`, this file, `docs/RE_WORKFLOW.md`,
`docs/ORACLES.md`, the relevant source and the latest knowledge-base rows. Then
recover the tree and attest the target:

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

For Web/Factory work, also run the Factory-native `th10-ghidra` preflight with
exact arguments `{}` and require a passed attestation for `target:th10-main`
with `attestation.provider_transport=factory-native-command`. IDA observations
from the previous session have been reduced to durable target facts below and in
`docs/KNOWLEDGE_BASE.md`; IDA is not required to resume.

## Current repository state

The tracked inventory is fully reviewed on its present denominator:

| Measure | Current value |
| --- | ---: |
| Function candidates | 1,317 |
| Boundary reviewed | 1,317 |
| Origin reviewed | 1,317 |
| Authored / excluded / indeterminate | 709 / 518 / 90 |
| Source-present mappings | 311 |
| Canonical exact functions | 163 |
| Canonical exact authored bytes | 22,480 |
| Authored source-present exact backlog | 142 |

The whole Windows i386 product build remains open. Semantic reconstruction and
portability have not started. Equal function size, equal table order or complete
semantic source coverage does not promote an exact unit.

## Active exact frontiers

| Owner | Target | Selected candidate | Current evidence | Main open issue |
| --- | ---: | ---: | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | 14,416 bytes | 14,020 bytes | 636/11,664 normalized comparable bytes; 181/181 selector; all 108 physical groups in target order | private operand-helper ABIs, frame/register allocation and several large case intervals |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | 9,587-byte executable owner | 9,960-byte linked contribution including its 376-byte table | 705/8,608 comparable bytes; candidate pre-table 9,584 vs target 9,588; all 92 physical groups in order | END/stop block placement, float temporary slots and child helper private ABIs |
| `EclVmContext::Run @ 0x0044E1A0` | 7,020 bytes | 7,020 bytes | 795/6,264 comparable bytes; 6,692/6,692 pre-table; all 59 physical groups in order | arithmetic stack slots, format-parser registers and `ReadInt` private allocation |

All three remain non-exact. Their target sizes must not be inferred from a PDB
contribution that includes or excludes an adjacent table differently.
In particular, the ANM target's separate table follows one alignment byte, so
its comparable code/alignment/table span is 9,964 bytes; the selected candidate
is four bytes shorter at 9,960. The 9,587 owner row deliberately excludes that
separate target table.

## Enemy dispatcher: selected checkpoint

The current source checkpoint consists of these commits:

| Commit | Retained result |
| --- | --- |
| `b5ef42d` | models the 0x1F8 laser scratch as a scalar-field struct, restores target scratch-object order, and joins 0x1B4 through one reader call per field |
| `c816a5c` | restores the rank-float shared tails used by 0x164, 0x15D and 0x15E |
| `f0024b0` | restores the 0x1AE edge into the second shared float resolve/store tail |
| `cc7c686` | keeps generic `EclVmContext::ResolveFloat` out of line, restoring the target adapter/generic-resolver boundary |
| `07e2a37` | restores the target-observed 30 direct calls to `ReadIntArgument` |

The target dispatcher has exactly 30 direct calls to the 16-byte integer reader
adapter at `0x00412A00`. The selected candidate now also has 30 such calls and a
16-byte adapter. Opcodes 0x161 and 0x162 are exact-sized at 229/229 and 97/97
bytes. The adapter is still non-exact because the candidate enters with
`ECX=runtime, EAX=index`; the target enters with `EAX=runtime, ECX=index`.

The target `ResolveFloat` adapter at `0x00412A70` is a 14-byte tail adapter to
the separately exact 93-byte generic resolver at `0x00450070`. The explicit
generic no-inline boundary is required. Removing it expands the generic resolver
into the adapter and changes dispatcher tail topology.

Opcode 0x1B3 remains 169 candidate bytes versus 170 target bytes. Its branch
conditions already agree. The one-byte interval difference is a whole-function
allocation effect:

- target keeps the unscaled bullet index in ESI, calls the float reader, then
  multiplies it by `0x210`;
- candidate scales the index before that call;
- target reaches the case with the full owner in EDX and begins by loading the
  active context from `[EDX+4]`;
- candidate reaches it with the active context in EAX and uses `MOV EDX,EAX`.

Do not add padding, inline assembly, volatile-only shaping or fake dependencies
to close this byte. Fix the surrounding natural source and private call graph.

The selected dispatcher prologue still exposes the broader allocator gap:

- target frame allocation is `SUB ESP,0x2C4`;
- candidate frame allocation is `SUB ESP,0x2BC`;
- target laser/spawn scratch addresses are `ESP+0x50` and `ESP+0x248`;
- candidate addresses are `ESP+0x40` and `ESP+0x238`;
- target retains the owner in EDX while decoding the opcode through EAX;
- candidate retains the owner in EDI and the active context in EAX, decoding the
  opcode through DX.

The largest remaining Enemy intervals include the laser request families. Use
the case-layout report to rank them again after every allocator or ABI change;
avoid optimizing only total size.

### Focused Enemy probe

```sh
scripts/repo-python scripts/probe-ltcg-backlog.py \
  --source src/EnemyEclDispatcher.cpp \
  --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' \
  --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' \
  --profile-flag=/GS --json > .analysis/enemy-probe.json

scripts/repo-python scripts/report-ecl-dispatch-table.py \
  --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe \
  --candidate-function-address 0x00401180 --json \
  > .analysis/enemy-layout.json
```

The linked candidate entry address can move when source function order changes.
Read it from the probe report instead of assuming `0x00401180`.

### Enemy experiments already exhausted

Do not repeat these unless a preceding retained change alters the allocator:

- signed/unsigned/int opcode carriers, low-word casts and a typed instruction
  pointer;
- moving the 0x1B3 bullet index among locals or carrying it as a pointer;
- local float results, cached difficulty and explicit four-way difficulty
  branches;
- volatile owner, active-context, instruction and operand-read variants;
- split spawn-index calculation around `memset`;
- static raw wrappers and forced direct float resolver variants;
- free/swapped/unsigned integer-reader signatures;
- moving the six adapter definitions after the dispatcher: normalized agreement
  and the private `ReadIntArgument` ABI were unchanged;
- rewriting `ReadIntArgument` as a raw self-chain: this put runtime in EAX but
  tail-called the generic reader, shrank the adapter to 14 bytes and changed the
  generic reader to a stack-index `RET 4` shape. It was reverted;
- force-inline helpers taking the full owner: the first tested form grew the
  owner to 14,116 bytes and reduced normalized agreement to 604/11,660. It was
  reverted; the second queued probe was interrupted during handoff cleanup and
  has no valid result.

## ANM executor: resume point

The maintained executor covers opcodes `-1..92`. The target's 94-entry table has
92 unique destinations because END/DELETE share one body and NOP/interrupt-label
share the ordinary advance body.

Current hard facts:

- target places float-jump opcode 39's 105-byte body before the END/DELETE return
  at `0x00440DFC`, then places the stop-update block at `0x00440E2B`;
- candidate places the stop block before END, expanding the corresponding
  interval to 192 bytes;
- target/candidate first-operand temporary slots for float-jump opcodes
  33/35/37/39 are `+0x84/+0x4C`, `+0x8C/+0x54`, `+0x94/+0x5C`, and
  `+0x2C/+0x24`;
- `FindVm` is 65/73 candidate/target with EAX+EDX versus EDX+stack;
- `GetVm` is 19/21;
- `CreateVmVariant3` is 74/73 and uses EBX rather than target EDI for hidden
  return storage.

Moving END outside the loop, chaining `CreateVmVariant3(...).GetVm()`, reordering
independent child fields, moving the saved-speed declaration, inverting POSITION
and rewriting the loop as an infinite loop with an explicit stop edge all failed
to recover the target layout and were reverted.

Use `scripts/report-anm-execute-table.py` after every selected linked probe. The
current natural source is the baseline; do not promote the owner from its four
byte pre-table gap.

## Generic ECL runner: resume point

The runner's total size, pre-table span and physical group order all match, but
its bytes do not. The most useful remaining local gaps are:

- target float ADD/SUB/MUL/DIV first-pop slots are
  `+0x60/+0xB8/+0x68/+0xF8`;
- candidate slots are `+0xE0/+0x60/+0xB8/+0x68`;
- the displacement-width changes explain several offsetting 12- and 15-byte
  physical-case differences;
- target format opcode 0x1E carries metadata in EDX and uses EDI for the cursor;
- candidate carries metadata in EDI, uses ECX for the cursor tail and emits
  `LEA/TEST` plus three alignment bytes;
- `ReadInt @ 0x0044FDB0` is target-sized at 144 bytes and has the target EDX
  receiver plus stack index and `RET 4`, but three register-choice fields remain
  different.

Interleaving arithmetic cases increased raw agreement but broke target physical
order. Cursor assignment/increment splits, explicit metadata-address locals,
metadata-pointer advancement, support-source reordering, merged `ReadInt`
returns and tagged stack-top locals were byte-identical or worse and were
reverted.

## Exact regression gates

Run focused gates while iterating:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --source src/EnemyEclDispatcher.cpp

scripts/repo-python scripts/replay-exact-units.py \
  --source src/EclVm.cpp

scripts/repo-python scripts/replay-exact-units.py \
  --source src/AnmManager.cpp
```

A clean handoff audit rebuilt all three selected owners and then replayed every
canonical unit sourced from `EnemyEclDispatcher.cpp`, `EclVm.cpp` and
`AnmManager.cpp`. It passed 95 units across 14 artifacts, 17,865/17,865 bytes
and every declared linkage field. The receipt is
`.analysis/gpt-5.6-sol/20260919-handoff-audit/three-source-exact-replay.json`.
Re-run the relevant source gate after any code edit because the linked contexts
are sensitive.

Before a checkpoint, also run:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Commit local checkpoints as `gpt-5.6-sol: ...` for this campaign. Do not push.

## Retained local analysis

Scratch was pruned at this handoff. The retained local index is
`.analysis/README.md`. Current unresolved evidence is kept only for the three
large owners and the latest Enemy checkpoints. Accepted exact work can be
reproduced from tracked match units and does not need old candidate binaries.

Generated executables, objects, PDBs, maps, copied source snapshots, interrupted
reports and superseded negative probes were removed. If a knowledge-base row
names a deleted scratch file, the row remains the durable conclusion; reproduce
the observation from its tracked source and stated command when needed.
