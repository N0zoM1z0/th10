# Hint list tail insertion: complete member-source replay

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent source checkpoint: `791fe9a`. Reviewed on2026-10-21.

The complete52-byte owner at41AB10 is now maintained as
HintLinkView::Append. The former free function, HintAppendLink, emitted40
bytes and passed the node in ECX instead of target EDX. Merely declaring that
free function fastcall changes none of the nine inspected whole-owner hashes,
including the helper and loader. That control was restored.

Ordinary member source takes a node and uses a local head initialized from
this for traversal. The field operations and final next-pointer recheck are
unchanged. VC7.1 LTCG naturally chooses target EAX head/EDX node, tracks the
next-field address in ECX, saves/restores ESI and emits the target loop
alignment. The entire52 bytes are raw-equal, including every internal branch
and alignment byte. No register binding, padding, wrapper or target bytes are
inserted into source.

Both real loader callers now supply the node through EDX. The clone caller
also computes that node address before the copy, as in target. Clone-store
ordering, the four-byte frame gap and other loader differences remain open:
LoadHintFile is still1533/1498, not exact. An unrelated aggregate link-init
control grows the constructor51->57 and is restored. The adopted constructor
remains51/nonexact and its origin remains indeterminate.

## Full cold evidence

The new unit shares the genuine LoadHintFile entry and four-TU /GL /GS graph
with hint-read-line: Hint.cpp, FileSystem.cpp, PbgArchive.cpp and Main.cpp.
The first full cold is member-append; the second is a fresh canonical cold in
build/match-linked/HintReadLine. The complete public member MAP/PDB owner
has zero linkage fields and matches the target raw SHA-256
4110697bba59a723c55f47cef0e73c95cfe792fbdbdad62fd632b720795704b5.

Six existing same-image guards retain986 bytes/all36 fields. With the new
owner, both images pass seven complete owners1038 bytes/all36 fields; all18
recursive project inputs and all seven owner hashes agree. See
[native identities](native-summary.json). Existing guard recipes retain their
canonical contexts; five are explicitly overridden only for same-image guard
checks. The line reader already shares this canonical graph.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit hint-append-link --unit hint-read-line --json
scripts/repo-python scripts/test-hint-load.py
```

The source mapping and synthetic fixture calls now use the real maintained
member API. The original class, name, visibility, production TU and source
spelling remain unknown. This source candidate does not resolve original
ownership merely because its complete codegen matches. Native runtime,
whole-product closure and the three original large interpreter owners remain
open. Private controls are under
`.analysis/gpt-6.1-sol/hint-list-abi-20261020/`.

The updated actual-source parser/lifetime baseline and22 strict mutation
controls pass. All97 standard CI checks, toolchain declarations, tracking,
generated progress and whitespace checks pass; all18 source pins were rechecked
at the end. See [validation scope](validation-summary.json). Two inspected
rejected build directories were removed, reclaiming791356 bytes; source/report
controls and both accepted cold images remain.
