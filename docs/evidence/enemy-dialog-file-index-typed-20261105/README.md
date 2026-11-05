# TH10 typed eight-byte GUI dialogue-file index and original allocation ordering

The original Japanese TH10 1.00a image is pinned by SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete original **124-byte** `EnemyReadDialog` at `0x00415DB0`
contains a real C++ exception registration/unwind sequence. The source
in this batch remains **NONEXACT**; ordinary root graphs without `/EHsc`
emit a **78-byte** static body instead and are not acceptance evidence.

Actual target instructions `0x00415DE2...0x00415DEC` load the GUI message
file **after** allocating and testing a 0x90-byte message owner, then fetch
an entry at file base + `index*8+4` and add its signed 32-bit content offset
to the same file base. `EnemyDialogFileIndexEntryView` now describes that
8-byte packed table record: unknown first dword, signed `contentOffset` at
+4; native `sizeof`/`offsetof` assertions secure the storage. In the
maintained `new GuiMessageVmView(...)` expression, the two typed file
reads remain after allocation. An earlier incomplete dirty variant cached
the file pointer *before* the new-expression; that changed the target's
observable evaluation order and was corrected before this commit.

The real 11-source VC7.1 `/GL` local ECL graph rebuilt modified
`EnemyEclDispatcher.cpp`, reusing only ten unchanged source/header-pinned
support objects. Its static map/PDB-owned non-EH `EnemyReadDialog` body
is **78 bytes**, with five unchanged field positions/types and **zero**
ordinary candidate-to-candidate byte differences. The same graph preserves
complete `EnemyBeginSpell` 2,432-byte candidate, ECL dispatcher 14,352-byte
candidate, drop core 505-byte *context-specific* candidate, 330-byte camera
and 672-byte enemy constructor, all with unchanged linked field geometry
and ordinary codegen. These do not establish original exactness.

Three selected canonical exact replay guards still pass from the current
source: `enemy-ecl-resource-load-package` **239/239**,
`enemy-set-chapter` **16/16**, and `enemy-configure-interrupt` **48/48**
(**303/303 already-exact bytes**). Seven-stage lightweight CI and whitespace
checks passed. No full cold replay, EH runtime test, whole game build, or
new exact credit was claimed. See `native-result.json` and ignored linked
artifact at `build/gpt-web/20261105-dialog-typed/message-file-post-new-lifetime/`.
