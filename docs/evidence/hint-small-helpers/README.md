# Hint whitespace and table lookup helpers

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent source checkpoint: `e0dbf48`.

## Target and source observations

`HintTrimWhitespace` at 0x0041A0A0 owns 114 bytes. Its existing natural source
already reproduces the entire owner, without relocation normalization. It
trims space, tab, LF and CR in place and returns the original pointer.

`HintLookupValue` at 0x004198C0 owns 98 bytes. At both real calls in
LoadHintFile (0x00419D11 and 0x00419D39), the caller pushes count and table,
puts the name pointer in EDX, and does not clean those arguments afterward.
The two target returns pop eight bytes. The former cdecl source generated
94 bytes: its instruction stream matched except for plain returns and the
resulting local branch displacement.

Declaring the ordinary three-argument static helper `__stdcall` recovers both
returns. LTCG chooses the EDX name argument itself. No register binding,
synthetic arguments, padding or injected instructions are used. This is a
source candidate supported by complete code generation, not proof of an
original identifier, source declaration or production translation unit.

## Complete-owner replay

Both canonical units use the real LoadHintFile entry, Hint.cpp plus
FileSystem.cpp and the pinned VC7.1 SP1 build-6030 /GL profile. Qualified
static MAP symbols must join one same-start complete PDB contribution in
source.ltcg.obj; selection does not introduce a retention wrapper.

The first independent cold is retained under
`build/gpt-6.1-sol/hint-small-exact-20261020/lookup-stdcall`; its strict replay
uses the canonical recipes with only the output directory changed. The second
cold uses the canonical `build/match-linked/HintLoad` directory. See
[native summary](native-summary.json) for source, artifact and owner identities.

The existing 79-byte ClearHintLists unit is replayed in both images as a direct
same-TU guard, including its two independently bound release calls. No credit
is claimed for LoadHintFile, HintSplitLine, HintLookupValue's tables, or
HintAppendLink. The first diagnostic still emits 71/75 bytes for SplitLine
and 40/52 for AppendLink. Original production ownership, native runtime,
whole-product closure and the three large interpreter owners remain open.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit hint-trim-whitespace --unit hint-lookup-value \
  --unit hint-clear-lists --json
scripts/repo-python scripts/test-hint-load.py
```

Only the affected loader graph is cold-replayed; the full historical exact
cohort is not rebuilt. Private controls and recovery notes are under
`.analysis/gpt-6.1-sol/hint-small-exact-20261020/`.

The actual-source Hint parser suite passes its baseline and all 22 strict
mutation controls under UBSan. All 97 standard CI checks, toolchain declaration
checks, tracking, generated progress and whitespace checks pass. See the
[validation summary](validation-summary.json). Seven restored negative
large-owner build directories were removed, reclaiming 5,399,291 bytes.
