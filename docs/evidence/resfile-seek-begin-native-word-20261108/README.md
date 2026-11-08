# TH10 MemoryPbgFile::Seek: native FILE_BEGIN 32-bit address publication

Original TH10 Japanese v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`CMemoryPbgFile::Seek @0x004364F0` occupies a complete **107-byte**
original function with a `RET 8` and zero external relocation fields.
The source already recovered signed offset acceptance, native remaining
length arithmetic for FILE_CURRENT, negative FILE_END words and shared
successful `m_Current` publication. Its FILE_BEGIN path still used
`position = m_Data + offset`, potentially constructing an abstract C++
out-of-array pointer when an unchecked backing allocation pointer or size
is malformed. The original i386 simply performs `ADD` on low 32-bit
address words. The maintained source now explicitly forms
`reinterpret_cast<DWORD>(m_Data) + offset` and casts the native
word to the byte pointer. This is *not* a bounds repair, a portable
pointer claim or a rewrite of FILE_BEGIN's existing signed and size
validation.

## Strict 107-byte compiler exploration

Earlier branch-sharing trials (CURRENT+END shared labels, early per-case
returns and altered dispatch shapes) changed codegen and failed.
This continuation tried 13 additional natural FILE_BEGIN base/offset
forms. Ten local-init attempts without case braces were invalid under
VC7.1's switch jump rules and were rejected; two pointer-expression
forms preserved the existing 107-byte candidate with **100/107** raw
original bytes. Nine further scoped-case variants with braces and
initialized base/offset locals all compiled and preserved **100/107**.
No variant passed the original 107-byte owner or changed its seven
remaining mismatches at `+0x49,+0x51,+0x55,+0x59,+0x5E,+0x61,+0x63`.
The original shares the FILE_END and FILE_CURRENT success add/store tail,
whereas the current compiler shares FILE_BEGIN and FILE_CURRENT.
This register/success-tail difference remains **NONEXACT** and earns no
additional canonical byte credit.

The chosen simplest FILE_BEGIN low-word expression was installed. A
**fresh complete normal-COFF** function comparison preserved **107 bytes,
100/107 raw target bytes, zero relocations**. All ten affected previously
accepted file-interface owners were separately replayed against the
original with the installed source in their two declared compile graphs:
**491/491 original bytes**, all required fields intact.

Reproduce only the relevant units:

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

Local variant sources and original-bound result receipts (not canonical
source artifacts): `.analysis/gpt-web/20261107-active/seek-begin-register-results.json`,
`seek-scoped-results.json`, `seek-begin-word-selected-replay.json`,
`build/gpt-web/20261108-seek-begin-{reg,scoped}/`. This source change
does not imply a whole-game rebuilt executable or runtime equivalence.
