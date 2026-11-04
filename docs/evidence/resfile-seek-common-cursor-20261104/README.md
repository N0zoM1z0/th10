# TH10 memory-backed file Seek: recovered single-cursor write and 107-byte extent

Original pinned Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete independent physical function
`CMemoryPbgFile::Seek @ 0x004364F0..0x0043655A` is **107 bytes** with **no
external relocations** and a true `RET 8` stack ABI. Both maintained
memory-file vtables contain the same target function pointer at virtual
slot +0x18; this is not a copied source-only helper. The original ownership
is reviewed as authored in the tracking ledger, but original source spelling,
physical compiler TU and executable product closure remain distinct facts.

## TH10-observed boundary semantics

The original three seek origins are FILE_BEGIN (0), FILE_CURRENT (1), and
FILE_END (2). FILE_BEGIN rejects negative offsets and offsets at or past
file size; the successful cursor becomes `m_Data+offset`. FILE_CURRENT
compares `m_Data+m_Size-m_Current` *as signed* against the signed input,
rejecting offsets greater than or equal to remaining bytes while allowing
negative positions as in the native code. FILE_END rejects positive offsets
and magnitudes at least file size, computing `m_Data+m_Size+signedOffset`.
Default origins return false; successful branches store the new cursor and
return true. No extra clamp, seeking-to-exact-end allowance or length guard
has been introduced beyond the native branch behavior.

The earlier natural `switch` source used independent cursor assignments
and per-case immediate success returns. The pinned VC7.1 SP1 normal-COFF
function was **117 bytes**. Native disassembly shows the original compiler
shares the successful `m_Current` update across seek origins, including a
single `add`/store segment reached from FILE_CURRENT. The selected maintained
source computes a typed `unsigned char *position` in the three branches,
breaks, then assigns `m_Current = position` exactly once and returns true.
It preserves the target bounds and all three actual pointer expressions.

## Strict complete-function compiler comparison

A fresh ordinary MSVC 7.1 SP1 `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`
normal COFF compile of the *installed* `src/ResFile.cpp` produced:

- **107-byte actual COFF symbol extent**, verified independently through the
  object parser (not the comparator's fixed 107-byte target window).
- **0/0 relocations**: the entire 107-byte owner can be compared raw without
  address normalization, EH cropping or an inferred private register input.
- **100/107 ordinary bytes identical**, seven remaining actual differences
  at offsets `+0x49`, `+0x51`, `+0x55`, `+0x59`, `+0x5E`, `+0x61`, `+0x63`.
  The candidate uses EDX for FILE_BEGIN offset where the original uses EAX;
  that difference moves the FILE_CURRENT shared success-jump target.

This remains **NONEXACT** and is **not** added to `config/matches.csv`.
Repeated source forms (`signedOffset` vs `offset` addition, preloaded
base/offset locals, separated bound guards, pointer indexing, and early
store) were actually recompiled: six leave the same 107-byte/100-byte
frontier, a reordered bounds check regresses to 94/107, and early store
regresses to a **122-byte** COFF owner. Two preliminary common-*failure*
variants emitted **118 bytes**, not 107; their comparison's `size=107`
field describes the *requested target window*, which is why **actual COFF
extent checking is mandatory**. The incorrect initial size inference was
explicitly rejected before this source was selected.

An independent **selected real cold replay on the installed modified
source**, across the nine normal-COFF methods and one /GL-linked constructor,
passed **10/10 accepted units, 491/491 exact original bytes**. The source
SHA-256 was checked equal **before and after** that replay:
`fbb971d095319a3a5dd7257f7ae75c4b8e73439a71bbfed155a4c811154fcadd`.
The same guard-produced normal COFF retains this changed Seek as an actual
107-byte/0-relocation function, independently of the first diagnostic compile.
The ten accepted owners are: memory-file Write 5, Open 46, destructor 49,
Close 37, Read 108, Tell 7, GetSize 4, resource Open 168, resource derived
destructor 49, and memory-file constructor 18; total **491 bytes**.
No exactness credit is granted to the still-NONEXACT Seek owner.

Private evidence under `.analysis/gpt-web/20261104-next/`:
`seek-update-results.json`, `seek-final7-results.json`,
`seek-verified-json.json`,
`seek-retained-guard-cold2.json`, and actual COFF
`build/gpt-web/20261104-next-seek-verified/ResFile.obj`.
Repeat only affected accepted units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit memory-pbgfile-open --unit memory-pbgfile-destructor \
  --unit memory-pbgfile-close --unit memory-pbgfile-read \
  --unit memory-pbgfile-tell --unit memory-pbgfile-get-size \
  --unit memory-pbgfile-write --unit win32-resource-pbgfile-open \
  --unit win32-resource-pbgfile-destructor-00436680 \
  --unit memory-pbgfile-constructor --json
scripts/repo-python scripts/ci.py
```

This proves a substantial complete-owner compiler-frontier improvement,
not exact game codegen, original historical source identity, runtime
correctness, product build closure or portability.
