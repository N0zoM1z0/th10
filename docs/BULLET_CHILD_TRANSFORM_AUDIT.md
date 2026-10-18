# Child-pattern transform: bounded conditional audit

Status: local target predicates support the maintained source roles. No source correction or exactness credit is justified by this audit. This is not a proof of full transform-program equivalence.

## Scope and evidence

Audited `BulletRuntimeView::AdvanceTransformProgram`, owner `0x00406D90`, 1454 bytes, at repository HEAD `e94559cc95d583b702660b72bbc605e5dfc5743a`.

- Owner SHA256: `7febc838c4339dd1c8f53ae2cc12348f54b7a81ebcb2139fcf5d825da6028f9d`.
- Whole target PE SHA256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Source `BulletTransform.cpp` SHA256: `49142d9f3329defafa29e53f1993fcaec17c1042142f010576e00ecf5d5b27ee`.
- Bounded CFG closure: 1454 reachable bytes, 336 instructions, 380 edges; one resolved indirect dispatch. Guard non-bypass checked after closure.
- Only auxiliary reads: 128-byte remap at `0x00407354`, SHA256 `a82463843ab4bcf47c8611e41926084c1b55330f6675fac171965ad29e4e3717`; 20-byte pointer table at `0x00407340`, SHA256 `9339bb47a5e46ed5dc8f9b8be47018e240ad41129e230e75e865aadcbb908bfd`.

Auxiliary spans were ledger-disjoint and inside validated executable raw/virtual section extents. Selected remap targets were validated inside the owner and on instruction boundaries; unused pointer slots were not promoted to CFG roots. No callee body was inspected. Target results contain fixed predicates, roles, counts and hashes; no target bytes, tables, disassembly, decompilation or ordered instruction trace were exported.

## Descriptor fields and packed-byte addresses

Successful command: `repository-command:21ceb4f094a04fa4a9b84b29db445ba8` (exit 0).
Reviewed verifier SHA256: `bc8d8419f8ede2ff90c518a19304f56a70c580cf1e5dfc49a5b3c3fb9098ca1c` (`compact_packed_source_identity.py`).

The 46-instruction preparation corridor contains a 0x210-byte descriptor zero fill and one live descriptor-pointer GPR candidate. Under the premises below, final local field predicates are true:

- Descriptor+0: signed16 `bulletType` is zero.
- Descriptor+2: signed16 `color` sign-extends a source byte.
- Descriptor+0x1F8: unsigned16 `aimMode` is zero.
- Descriptor+0x208: int32 `transformStartIndex` zero-extends a source byte.

Start-index and color source addresses use the same normalized corridor-entry pointer, at offsets +8 and +9 respectively. Their address difference is one byte. They are different loads in different memory epochs: equality of their relevant memory snapshots is unproven. Exact identity with the current transform record is also unproven.

The pre-spawn receiver+0x45C write is exempted from private-descriptor havoc only under independently checked receiver origin and the explicit object/frame disjointness premise. Without that premise, conservative alias havoc leaves the field predicates unresolved; this is not a contradiction.

The descriptor's 0x210-byte zero fill is distinct from the maintained source transform-array copy: 18 records × 0x18 bytes = 0x1B0 bytes, at descriptor+0x20.

## Post-spawn live cursor and direct cancellation order

Successful command: `repository-command:f70f2219d5804532b8f3f7ca7d18dc13` (exit 0).
Reviewed verifier SHA256: `77bce050e9dd8c93552020aa60fcaf391c252c16411a5ec892e75383a86571df` (`postspawn_live_cursor.py`).

After normal return from the unique direct call to maintained `SpawnBulletPattern` (`0x004073E0`), a fixed four-instruction window proves a fresh receiver+0x45C dword load, increment by one, and store back to that receiver field, with one comparison and no other memory effect or call. Exact operand widths/access and register effects are checked. Every window instruction has only its required predecessor. Receiver origin is checked at both load and store.

Consequently, under the nonvolatile-pointer premise, the first post-return cursor write uses live post-call cursor memory plus one (modulo 2^32), rather than a saved pre-call index expression. No preservation of cursor memory across Spawn is assumed.

For the owner CFG following that normal return, the increment occurs before any later direct owner call to `EnemyCancelBulletRecord` (`0x00408030`) until another Spawn call. A Cancel call is CFG-reachable after the increment without another Spawn. This does not prove branch feasibility, cancellation on every path, or the identity of the cancellation condition. Callee-internal cancellation and reentrant execution are outside this theorem.

## Premises and open obligations

- Immutable pinned owner code/tables; ordinary normal instruction execution; opaque calls return normally.
- Ordinary flat 32-bit addressing; flat DS/ES and clear direction flag for descriptor string operations; valid nonwrapping addresses and private descriptor storage.
- Opaque calls preserve EBX/ESI/EDI/EBP values only. Actual private LTCG helper contracts remain unverified. Volatile registers and memory provenance are not preserved.
- For descriptor-field recovery only, the valid parent runtime object and cursor do not overlap the fresh private descriptor frame.

Open: exact current-record identity, packed-byte cross-epoch stability, formal descriptor argument ABI, actual private-helper preservation, saved fade-bit/cancellation-condition identity, and full transform-program equivalence. Local pointer carriers are not promoted to formal arguments.

## Validation and change boundary

Independent review approved both exact final verifier hashes before parent-only execution. Synthetic validation includes:

- Descriptor effects/alias/prefix controls: 19 negative and 10 positive/mutation cases.
- Packed source identity: 12 cases; compact schema/semantic-AST preservation: 3 cases.
- Post-spawn chain: 15 negative and 3 positive cases, including wrong source/base/value, extra memory effect, hidden register clobber, wrong access/width, prefix, and CFG bypass/entry-backedge rejection.
- Optimized Python is rejected; failures suppress semantic output.

The five host-test files committed at `e94559cc95d583b702660b72bbc605e5dfc5743a` are `probes/bullet-transform/{.gitattributes,README.md,harness.cpp,manifest.json,run.py}`. Run `python3 -O probes/bullet-transform/run.py --mutations` against the default repository source. Their finite host coverage and 10 mutation controls are separate source-behavior evidence; they do not cover all actions or establish target equivalence. This audit changes neither production source nor the exact-885 runtime support graph. Native build status is tracked separately and is not inferred from these verifier successes.
