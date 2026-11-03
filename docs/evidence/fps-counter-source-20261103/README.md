# TH10 FPS/replay timing counter creator — complete nonexact source

The reviewed authored 151-byte target at 0x00413350..0x004133E6 was absent
from maintained source. It is now `CreateFpsCounter()` in the *existing*
`src/Main.cpp`, alongside the true typed `FpsCounterView`, `CalculateFps`,
`DrawFpsCounter`, registered callback, and destructor. No parallel opaque
shadow class or duplicated global allocation was invented.

Direct original disassembly and Ghidra establish a 0x8C-byte allocation,
full 0x23-dword zero fill, setting owner flag bit 2, and publication at the
actual replay timing global 0x00477708. It constructs a 0x24-byte
`th10::ChainElem` by its real default constructor, sets a callback to
0x00413690 and its owner at ChainElem+0x20, ORs flags 3, registers the
node on draw chain priority **47** through the real 0x00449B70 method, and
stores the node at owner+0x0C. The function returns the owner, and retains
the original unchecked child allocation behavior; it does not call the
higher-level `Chain::AllocElem` in place of that actual constructor.

The pinned VC7.1 scoped `Main.cpp + Chain.cpp` `/GL` entry diagnostic produces
**145** PDB bytes vs **151** native, 39/127 preliminary comparable ordinary
bytes and six candidate fields. This is **source-present NONEXACT**; no
`config/matches.csv` row or new byte credit. The calling site's separate
complete 186-byte startup function is exact in its own three-source graph,
which does not prove this 151-byte dependency. The original TU/name,
product/runtime behavior and source-level constructor origin remain open.
Private probe log: `.analysis/gpt-web/20261103/fps-constructor-first.log`.
