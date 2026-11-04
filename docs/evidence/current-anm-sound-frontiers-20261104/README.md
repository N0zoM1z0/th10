# TH10 current large ANM and sound frontiers — fresh source-epoch controls (2026-11-04)

Target: attested original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
All recorded controls used real pinned VC7.1 SP1 `/GL` source plus explicit
current compiler profiles, full PDB-owned functions, and independently
identified TH10 code/data boundaries. All unselected source experiments
were restored in `finally` and then verified against the maintained Git
worktree. This is **new native experimental evidence only**; no artifact is
an exact new owner, a complete game product or a runtime proof. The 350-field
ANM executor and four-field ANM script binder used strict linked comparisons;
Sound and texture format checks retain their documented original field
geometry but must also pass full symbol-to-target binding before any new
exactness claim.

Source SHA-256 after *all* controls:

| Primary source | SHA-256 |
| --- | --- |
| `src/AnmManager.cpp` | `a78dec3762559df708ceec3157a338fcda6fd1a01464a28a4e57b0699685a754` |
| `src/Sound.cpp` | `842f410ed44363232813df85ce910432573b90366428ac0bb4942ad31ab8bd2c` |

## ANM `ExecuteScript` — full 9,964 bytes, 350 fields, 95 locals

The actual five-source `/GL /GS` graph was compiled using
`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`, and
`AnmInt3Scale.cpp`, with the latter compiled with its known `/Ob1` support
profile. Hash-checked current-source support objects were reused only within
the bounded panel. Four new ways to express opcode-91's real
`AnmLoadedView::CreateVmVariant2` call were tried, preserving the original
script/layer parameter meaning:

| Source variant | Whole PDB | Bound fields | Strict ordinary match |
| --- | ---: | ---: | ---: |
| Loaded resource named after script/layer record | 9,964 | 350, including 95 locals | 9,962/9,964 |
| Loaded resource retrieved *before* script arguments | Different required offsets | mismatched | rejected |
| Layer addressed through pointer alias | 9,964 | 350, including 95 locals | 9,954/9,964 |
| Layer copied to a separate named argument | 9,964 | 350, including 95 locals | 9,954/9,964 |

No natural variant solved the original `+0x113`/`+0x115` EDX-versus-ECX
child-layer register pair. The 73-byte exact `CreateVmVariant2` callee was
not changed or credited again. The early-loaded-resource form is rejected
because original field locations, not mere field counts, are an acceptance
obligation. The maintained executor source remains unchanged and **NONEXACT**.

## ANM `LoadTextureRegion` — 320-byte target / 319-byte maintained

A fresh single-TU `AnmManager.cpp` `/GL` compilation through the actual
`AsciiManagerView::Initialize` entry tested four meaningful dataflow changes
in the embedded-texture rectangle branch:

| Change | Actual PDB | Candidate external fields |
| --- | ---: | ---: |
| Named width then height | 313 | 8; shifted geometry |
| Named height then width | 313 | 8; shifted geometry |
| Assign destination rectangle before source rectangle | 319 | 8; original one-byte frontier unchanged |
| Assign source height before source width | 319 | 8; original one-byte frontier unchanged |

The original `+0xA3` area uses `MOV ECX,[EDI+0x30]` plus
`LEA EAX,[ECX+EDI]`; current source uses `MOV EAX,[EDI+0x30]` then
`ADD EAX,EDI`. The extra target instruction byte changes subsequent
address-field positions and ECX/EDX schedules. No trial closes the full
320-byte size and all original fields. Existing selected texture exact
owner claims are unchanged; **NONEXACT**.

## ANM `InitializeAndExecuteScriptIndex` — 203 bytes / 4 fields

A separate fresh single-TU `/GL` native context using the true
`AnmVmView::Initialize` entry was recompiled four ways. Named VM receiver
pointer, scoped VM reference, named loaded-resource pointer, and const
beginning-of-script pointer all retained **203/203 PDB extent**, all
**four/four target-bound fields**, and precisely **202/203 ordinary bytes**.
The one difference remains `+0x27`, `MOV ECX,EDI` versus target
`MOV EDX,EDI` ahead of the actual VM initializer. Different contexts may
produce a different callee/private ABI: this one-byte discrepancy is not
waived. No source change was retained.

## Sound `PreloadBgm` — 369-byte owner / 10 fields

The real `Sound.cpp` primary plus freshly compiled current `ZWave.cpp`
support was linked through genuine `SoundPlayerView::ProcessQueues`, retaining
`/GL /GS /EHsc`. The first run compiled ZWave anew; its object and source
hash were checked before reuse for the next three variants. Three forms
for the `ReadFile` size/output-lifetime declaration order (size first,
output declared after the compiler barrier, and two-step size assignment)
all gave **369/369 PDB extent, ten fields at the expected original offsets,
365/369 ordinary identical bytes**, unchanged. Naming one shared
`ThBgmFormat *` across `SetFilePointer`, allocation and ReadFile instead
emitted **375 bytes** and moved the external-field geometry. The original
ReadFile setup still uses the requested count and output-DWORD addresses
in the opposite EAX/EDX order; semantic return/read checking was not altered.
The retained source is **NONEXACT** and none of these experiments changes
`config/matches.csv`.

## Reproducibility and limits

The source-edit-and-restore drivers and full JSON receipts are intentionally
ignored under `.analysis/gpt-web/20261104-next/`:
`anm-child91-owner-lifetimes.py` and `*-results.json`,
`anm-texture-width-height-panel.py` and its results,
`anm-binder-receiver-panel.py` and its results,
`sound-preload-lifetimes-panel.py` and its results.
The actual native PDB/PE objects are under matching
`build/gpt-web/20261104-next-*` directories, not Git. Some compiler
support objects were reused only after SHA checks of their current source.
The ANM binder command's outer tool wrapper timed out after the job finished;
its log and result JSON independently show **four completed strict
comparisons** and the final original-source SHA, with no running compiler.

No historical all-unit cold replay ran. The already-admitted exact owners,
source presence, target boundary counts and authored/source-origin categories
are unaffected. These trials neither establish the original compiler source
spelling nor justify forcing an invented register ABI, overriding linked
addresses or inserting inert code. Follow-up hypotheses should target an
actual TH10 caller/context/liveness observation, not repeat byte-neutral
alias or declaration-order sweeps.
