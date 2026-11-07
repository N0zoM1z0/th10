# ANM 9,964-byte ExecuteScript: named child-91 operand and file lifetimes

Original Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This is **new work on the complete original-backed large owner**, not a
small helper-only test or a new exactness claim.

At original `AnmRenderManagerView::ExecuteScript @0x0043EE30`, opcode 91
gets the possibly indirect child script operand before loading the current
layer and invoking the real `AnmLoadedView::CreateVmVariant2` member. The
baseline has precisely two remaining codegen bytes in the **9,964-byte**
whole owner: candidate ECX versus original EDX at `+0x113` and `+0x115`.
The original register choices may reflect original variable lifetimes or
optimizer context; they are not a license to impose an artificial ABI.

## Six genuinely recompiled natural C++ controls

Each control changed only the actual `src/AnmManager.cpp` opcode-91 case.
The four unchanged support sources were source-hash verified against the
retained prior **current-source** cold and their same objects copied into
a separate linked image. The one changed primary TU was recompiled from
source and linked with the pinned VC7.1 SP1 `/GL /GS` profile, real
`ExecuteScript` entry, and genuine `AnmInt3Scale.cpp` `/Ob1` support.
Compiles/links were serial at nice10/one CPU. The comparator independently
bound **all 350 original target fields**, including **95 local destinations**,
for each entire 9,964-byte PDB extent.

| Tested source lifetime | Native size | Original-bound matched bytes | Result |
| --- | ---: | ---: | --- |
| Named `AnmLoadedView *const` after resolving child arguments | 9,964 | 9,962 | byte-neutral |
| Named `AnmLoadedView &` after child arguments | 9,964 | 9,962 | byte-neutral |
| `const AnmChildScriptLayer&` for the argument record | 9,964 | 9,960 | **regressed** Z-vector update by two more bytes |
| `const unsigned int&` for VM layer | 9,964 | 9,962 | byte-neutral |
| `const int` child script snapshot | 9,964 | 9,962 | byte-neutral |
| Script snapshot plus `const unsigned int` layer snapshot | 9,964 | 9,962 | byte-neutral |

The five neutral forms retain only original differences `+0x113` and
`+0x115`. The record-reference form also differs at `+0x2483`, `+0x248A`.
All six left 350 independently checked linked fields with unchanged
extent. No source change improved the baseline, so **all six temporary
edits were restored byte-for-byte**. `ExecuteScript` remains **NONEXACT**;
there is no new canonical match or partial-byte credit. Existing exact
callee/helper source and 9,962-byte candidate were not changed.

Reproducible ignored trial program and results:
`.analysis/gpt-web/20261107-anm-child91/trial.py`, `results.json`;
separate linked PE/MAP/PDB in
`build/gpt-web/20261107-anm-child91/{variant}/`.
Installed source remains `src/AnmManager.cpp` SHA-256
`e4c4e24b4e7c3879e2a88265c161867a3ea8db5dcfe6d9abc6bf0d833beb0707`.
Neither a full-repository cold replay nor a runnable game was attempted.
