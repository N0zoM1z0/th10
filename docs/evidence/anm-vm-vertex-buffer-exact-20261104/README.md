# TH10 ANM VM generated-vertex buffer: 60-byte exact owner

Verified Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original CC-delimited function `0x0040B250..0x0040B28B` is **60 bytes**;
its direct caller, authored/generated origin, and original production TU are
not independently established. This claim is about its *entire* codegen owner,
not an ANM subsystem-wide match or a runnable Windows product.

The original accepts the real `AnmVmView *` in ECX and a source pointer in
EDX. It calls scalar `operator new(44)` at **0x00452493**, stores the allocated
pointer into the VM's `generatedVertices` at **+0x358**, copies **11 dwords**
from source to allocation using `REP MOVSD`, and copies the first **three**
new dwords into the VM's existing `positionOffset` at **+0x340..+0x348**.
It returns zero through a plain `RET`, with no pointer-null/failure guards
present in this physical target body. The maintained natural function
`InstallAnmVmVertexBuffer` in `src/AnmVmVertexBuffer.cpp` uses the existing
`AnmManager.hpp` view rather than a shadow VM or synthetic allocation.
Ghidra's initial description as an unidentified object copy was too vague.

## Complete target-bound proof

The pinned VC7.1 SP1 `/GL` source-only graph retains the real 60-byte PDB
contribution and 60 decoded instruction bytes. The *only* external field is
**+0x0A REL32 -> scalar `operator new` `??2@YAPAXI@Z` at `0x00452493`**.
All ordinary bytes match after linking that independently verified field;
no prologue/tail is cropped, and no unexplained field is masked. An initial
manual native candidate and **two separate canonical selected cold replays**
all report 60/60 complete exact with one/one field and zero differences.

| Selected cold | PE SHA-256 | PDB GUID |
| --- | --- | --- |
| 1 | `a2ae1d923c90200199dcff55d8762afbcde7816d1aca3222dfaff5aeaf5ea75b` | `00b34be821c9eb46addad5dbe9b39d32` |
| 2 | `1ae986b0b823aff6dbe4c0da7903ded280b230943e3e87be6c67a5008e617e09` | `8e9c9039a7020c42a7b6b3f3c0b2726c` |

Different PE hashes and PDB GUIDs exclude reuse of the first linked build.
The original source spelling/ABI and product/runtime behavior remain unknown;
no exactness credit is granted to the adjacent 148-byte ANM creator.

Reproduce only this changed complete unit (no all-unit replay):

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-vertex-buffer-0040b250 --json
scripts/repo-python scripts/ci.py
```

Ignored native receipts:
`.analysis/gpt-web/20261104/anm-vertex-60-first.log`,
`anm-vertex-cold1.json`, and `anm-vertex-cold2.json`.
