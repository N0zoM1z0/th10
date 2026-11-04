# TH10 derived resource-file destructor: distinct 49-byte exact owner

Target: original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Native `0x00436680..0x004366B0` is a separate, CC-delimited **49-byte**
function, byte-identical apart from relocated call distances to the existing
`0x004363C0` `CMemoryPbgFile` base destructor. It is not treated as a slice
of that earlier owner. The preexisting ledger marked it unclassified and
source-missing because the derived class had no explicit destructor
implementation in the maintained source.

## Actual C++ restoration

The real reconstruction has `CWin32ResourcePbgFile : CMemoryPbgFile : IPbgFile`.
Adding the natural `virtual ~CWin32ResourcePbgFile()` declaration in
`src/ResFile.hpp` and the empty out-of-line derived destructor in
`src/ResFile.cpp` makes VC7.1 SP1 emit a **separate** normal-COFF
`??1CWin32ResourcePbgFile@@UAE@XZ` method. The destructor body inlines the
base destructor's resource cleanup; it does not require manufactured
register inputs, dummy calls or pasted target instructions. The original
first sets the base memory-file vtable, conditionally frees the real owned
`+0x0C` allocation, zeros size/cursor/data fields at `+4/+8/+0x0C`, and
then restores the `IPbgFile` vtable before a plain `RET`.

The target owns exactly three relocated fields in its **entire** 49 bytes:

| Offset | COFF relocation | Original target |
| --- | --- | --- |
| `+0x0D` | `DIR32 ??_7CMemoryPbgFile@@6B@` | `0x0046F308` |
| `+0x15` | `REL32 _free` | `0x00452422` |
| `+0x2B` | `DIR32 ??_7IPbgFile@@6B@` | `0x0046F210` |

A direct original/COFF probe initially found **37/37 ordinary comparable**
bytes, **three/three** relocations and a complete 49-byte section. Two
**independent selected cold** builds then passed canonical **49/49 exact**:
first compiled COFF SHA-256
`7faa152a446e4865b361fc18bdf6425c135a9a3106fdda7e5aa8c675bbd7ef25`;
second SHA-256
`06dc88a0e45fd8db1eb930d1111ad4a5f56d473148b0b28daeeeeaff9eb8543f`.
They are distinct objects and were each compared to the complete original
function, not checked by length alone. No external fields or trailing
instruction bytes were discarded.

Scoped existing-owner regressions passed across these cold builds:
base `CMemoryPbgFile::~CMemoryPbgFile` **49/49**, derived resource `Open`
**168/168**, base constructor **18/18**. Combined selected cold #2:
**284/284** owner bytes. Other classes' exact source/ledger rows were not
promoted from this match. Since the native physical call/ownership graph and
historical original TU are not completely established, the new *derived*
function's original hand-authored versus compiler-synthesized provenance
remains **indeterminate**; exact codegen is separate from that question.

Only selected canonical replays are required, never the full cold cohort:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit win32-resource-pbgfile-destructor-00436680 \
  --unit memory-pbgfile-constructor --unit memory-pbgfile-destructor \
  --unit win32-resource-pbgfile-open --json
scripts/repo-python scripts/ci.py
```

Private ignored evidence is in `.analysis/gpt-web/20261104/`:
`resource-derived-cold1-guards.json`, `resource-derived-cold2-guards.json`,
`resource-derived-destructor-first.json`, and the first cold object copy.
The private original executable and compiler outputs are not committed;
product completeness, actual in-game runtime behavior and portability
remain open.
