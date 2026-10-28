# Complete sound queue and streaming reset exact, 2026-10-28

**1,665 authored exact bytes / two complete owners** are admitted:

| Unit | Target | Owned bytes | Fields |
| --- | --- | ---: | ---: |
| `sound-process-queues` | 43DDF0..43E39B | 1452 | 43, including9 fixed local fields |
| `streaming-sound-reset` | 44DAD0..44DBA4 | 213 | 4 |

Original Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local read-only Ghidra six-sample attestation and raw target review pass. No
Factory provider, target patch, ABI register hint or comparison exception is used.

## Complete ownership and source corrections

The old queue ledger explicitly left its adjacent table unresolved. Fresh
review assigns **1419 code bytes +1 NOP +32 table bytes =1452** to the owner.
DEC/CMP7 constrains all eight entries, every destination is an internal
instruction start, and read-only references over all33 suffix bytes find only
the owner's indexed dispatch. Four CC bytes precede the independent BGM worker.
The authored denominator increases33; this is not new instruction/source credit.

The missing213-byte Reset was previously an unresolved link anchor. Its natural
ZWave implementation resets four streaming counters, restores/refills the primary
buffer, rewinds the custom wave and sets its position to zero. Ghidra's apparent
extra integer argument is actually RestoreBuffer's local BOOL output. The real
private receiver is EDI. TH08/TH095 utility implementations suggested source
shape only; every accepted byte, field and ABI conclusion is TH10-local.

Both actual queue callers, WinMain438DC5 and Render43943B, push the sound owner;
the target returns with RET4. An ordinary member `__stdcall` declaration expresses
that observed interface. Main now calls the actual typed member instead of an
unimplemented free-function seam. The two existing shared-group entry selectors
change from QAE to QAG accordingly; compiler/linker switches remain unchanged.

Target-backed source recovery also restores:

- the DWORD continue flag, physical case order and branch-local preload calls;
- the common BGM release clear and fade-argument evaluation before its guard;
- live duplicate-buffer reloads between COM calls and direct global-volume reads;
- direct count-field reads, retaining division before the count clear/COM calls;
- two independent GetBuffer early checks. A positive-bound rewrite was
  whole-body byte-neutral and restored. The separate negative checks, corroborated
  by adjacent SDK-family source, reproduce both actual inlined TH10 paths.

The source remains natural C++. Original declaration spelling, classes, physical
production TU and storage ownership remain provisional. In particular, exactness
of the ordinary fade helper's inline result does not create a new target owner.

## Reproduction and proof scope

Two independent final-source **Sound + ZWave** cold builds use the existing
`/GL /GS /EHsc` profile. Each passes all four complete contracts: **2192 bytes /
60 fields**, comprising the new1665/all47 and existing InitSoundBuffers338/all6
plus BgmPlayerThread189/all7. Their final source/header pins are identical and
neither reuses objects. The shared canonical output stays SoundInitBuffers;
its preceding artifact group is retained separately.

A separate real-WinMain15-input diagnostic recompiles the three affected inputs
and reuses twelve hash-verified unchanged objects. It passes these four contracts
and three existing Main guards: **2582 bytes /82 fields /seven owners**. Both
new owners therefore also match in their real maintained caller context. This
is an incremental caller proof, not a second15-TU cold or a WinMain exact claim.

The canonical units contain every field, and all9 local addresses retain their
exact destinations. Supervisor/sound-owner bindings use independently attested
bases491C28/492590 plus reviewed offsets; float payloads are checked against raw
target values. Full extents, ordinary bytes, PDB identity and field inventory use
the unchanged Oracle. Reproduce only the affected shared group:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit sound-process-queues --unit streaming-sound-reset --unit streaming-sound-init-buffers --unit sound-bgm-player-thread
```

[results.json](results.json) records concise source pins, full proof scope and
model outcomes. Ignored producers, target/native listings and logs are below
`.analysis/gpt-6.1-sol/sound-queues-20261028/`. The first baseline observer's
code-only scope and wrong LoadBgm address are explicitly corrected; no earlier
invalid observer result is promoted. A mislabeled duplicate compile is excluded
from the distinct source models. Equal extents were never treated as equality.

## Remaining ownership and reconstruction work

Raw review also discovers two untracked, CC-delimited utility bodies:
GetBuffer44D3E0..44D3F2/19 and Get3DBuffer44D400..44D433/52. Complete decode and
all-byte read-only references establish disjoint extents and no observed
references. The second queries the raw target IID_IDirectSound3DBuffer, matching
the pinned SDK GUID. Both enter the ledger as **origin indeterminate, source
unmapped, nonexact**; their names are descriptive and contribute no exact/source
or authored-byte credit.

Preload369/369, LoadBgm304/298, Initialize754/786 and sample loading684/684 remain
nonexact. The outer WinMain1533/1389 and Render419/425 also remain nonexact.
Do not infer equality from their lengths or resume parked register spelling
panels without new evidence. Product build/runtime and original TU remain open.
