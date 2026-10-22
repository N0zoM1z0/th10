# Enemy laser copy, factory ABI and initializer controls

Date: 2026-10-22. Starting commit:
`a50ad2a9b81d2d25e3e950ac923d6e628801626b`.
Original Japanese TH10 v1.00a only, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Five completed source hypotheses leave maintained C++ source, headers, layouts,
interfaces, compiler profiles and exact recipes unchanged. Only two existing
ledger notes are qualified. The dispatcher remains 14,352 versus 14,416 bytes;
the factory remains nonexact despite matching the target's 153-byte size.
[Facts and source patches](facts.json) distinguish each actual build scope.
[Validation](validation-summary.json) records the final checks and cleanup.

## Fresh target observations

Direct local Ghidra review uses read-only/no-analysis execution, a rolled-back
transaction, one CPU and 768 MiB heap. All 412,289 mapped text bytes retain
SHA-256 `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The existing entry, laser cases, factory and incoming call contexts are reviewed;
Ghidra signatures and body ranges remain provisional.

Factory `41C510..41C5A8` decodes completely: 153 bytes, 44 instructions, four
external REL32 fields and two RET4 exits. Manager is carried in ESI, request in
EDI and type on the stack. The allocation results enter the two initialization
calls through EBX and return through EAX. The append sequence reads the tail
once. These are machine observations, not proof of original C++ membership,
source calling convention, constructor declarations or physical source owner.

A survey of every mapped-text E8 position for the factory destination joins
exactly ten independently recorded Ghidra CALL references. Four are in the
dispatcher; six occur in five recorded external Ghidra bodies. The boundary
handler contains two sites. The old phrase "six external caller owners" is
therefore corrected to six call sites in five provisional bodies; original
source ownership is still unknown.

Remaining pattern cases 1B1/1B2 also copy 108 dwords from runtime+2E4 using
REP MOVSD. The target source/count/destination setup is independently reviewed
at `41130B..41131A` and `411455..411467`. This does not establish an original
array declaration or imply that a shorter physical case omits behavior.

## Actual source controls

Baseline dispatcher diagnostics: 3,219/11,148 aligned positions, aggregate
absolute physical-span gap 566, 42 target-sized groups. All 181 selectors and
108 groups in target physical order remain. Every control retains frame 2C4
and 19 sentinel comparisons. Diagnostic scores grant no partial-case credit.

| Source hypothesis | Dispatcher bytes | Aligned positions | Span gap / target-sized groups | Factory bytes / exits |
| --- | ---: | --- | --- | --- |
| Remaining two typed pattern copies | 14344 | 3285/11144 | 570 / 42 | 153 / plain RET |
| Complete request reference interface | 14352 | 3219/11148 | 566 / 42 | 153 / plain RET |
| Ordinary manager member factory | 14344 | 3225/11145 | 562 / 43 | 157 / RET8 |
| Cache the actual append-tail pointer | 14352 | 3219/11148 | 566 / 42 | 147 / plain RET |
| Ordinary member initialization methods | 14352 | 3219/11148 | 566 / 42 | 153 / plain RET |

The typed-copy form increases positional agreement but worsens total span gap
and moves the parent farther from its target size. Its unchanged straight-laser
interval shrinks from 389 to 382 versus target 439; the edited 1AD/1B2 interval
shrinks from 405 to 401 versus 397. A tail interval also changes. These are
physical-layout effects, not missing-source behavior or accepted case matches.
The other three complete bodies are raw-identical to baseline.

The request-reference interface and manager-member interface each freshly
compile all ten game-source TUs from a consistent 32-file source/header overlay.
All ten source objects are fresh; only the two pinned harness inputs are reused.
Each changes three source/header roles and converts all ten real call sites,
including the six in EnemyLaser.cpp. They are distinct hypotheses, not a
same-candidate acceptance cold pair or canonical product build.

The first reference preparation missed those six calls and stopped at the fifth
compile with C2664; four objects had compiled. This incomplete overlay is not a
falsified target hypothesis. Compact failed patches/provenance and the compiler
failure log are retained. The corrected interface is rebuilt from scratch and
all ten sources complete. An initial extra-caller PDB selector is also corrected
to the original AnmManager source before its result is used.

The three other controls each freshly compile one GL input and reuse nine
hash-pinned current source objects. The copy control changes only the
dispatcher; the two initializer/tail controls change only EnemyLaser.cpp.
The member initializer methods preserve the existing explicit construction
operations; they do not introduce actual C++ constructors or establish object
lifetime closure. No register annotations or forced inlining are introduced.
Every profile retains GL/GS and the existing scale-support Ob1 setting.

Explicit qualified MAP/PDB selection independently binds both original
TU-static initializer extents before comparing them with the new public member
extents: 65 and 74 bytes, each completely raw-identical. Both still move EAX to
ESI at entry. A first external-only enumeration could not select those static
owners; it supplies no absence or equality evidence. Member syntax alone does
not recover the target EBX initializer input in this context.

## Complete checks and interpretation

Every image passes 36 whole linked guards, 4,618 bytes/all 121 fields. These
include the previous 35-guard set and the separate complete 194-byte/all-three
ANM direct caller. Five canonical normal-COFF contracts supply additional
linked guards only; their canonical artifact kinds remain unchanged.

ECL retains its complete 7,020-byte comparison: 6,993 matching bytes, 27 ordinary
differences, all 189 fields/62 local destinations and the complete 171-byte
float callee exact. Coverage is 7,019 decoded bytes plus the reviewed one-byte
selector; the generic normalization-complete flag remains false.

Both request-reference and member-initializer forms reproduce all four complete
baseline raw bodies: dispatcher, factory, ANM executor and ECL Run. The manager
member form preserves ANM/ECL raw bodies but emits a different 157-byte factory.
The tail-cache form preserves ECL raw bytes; dispatcher and ANM are fully
baseline-equal after independently identifying and replaying 17 and three
changed address fields respectively. The field binder requires equal local
destination offsets or identical qualified MAP names and leaves no changed
field unresolved. This is peer equality only, not target equality or runtime
validation. No skipped fill or ordinary byte difference is normalized away.

None restores the target request register/RET4 combination. Source reference
versus pointer, member versus free initializer/factory and original construction
shape remain unknown. The adjacent TH09 member laser-spawn implementation and
provisional TH095 argument headers provide hypothesis material only; they
transfer no TH10 layout, ownership, ABI or exactness facts.

All 108 maintained source files, 32 large source/header pins and 15 meaningful
native baseline files remain unchanged. Only dispatcher/factory notes change
among the initial 373 source/test/config inputs before the final snapshot.
Private producer manifests pin pre-note inputs; adapt them after review rather
than restoring current source or notes to satisfy historical pins.
Fresh 111-group CI passes with actual exit 0 and all 373 final source/test/config
inputs unchanged. Focused selector/table coverage, toolchain, tracking, Enemy
source completeness, generated progress, documentation and whitespace pass.

Hash-checked cleanup removes 89 owned files, 4,273,546 logical bytes: three
completed superseded carriers and the failed partial overlay/build/logs. Retain
the member-157 and initializer-153 carriers, both consistent full-source
overlays, canonical baseline, compact producers/patches/results and nine
independent untracked D3D files. No unrelated state is staged or removed.

Authored exact remains 409/74,716 bytes; overall exact 1,250/141,226 bytes.
Source remains 613, nonexact 204 and absent 121. Exact reconstruction remains
active-incomplete; Windows i386 product/runtime closure is open, and semantic
reconstruction/portability have not started. Continue a distinct target-local
initialization/caller-context investigation; do not repeat these five forms.
