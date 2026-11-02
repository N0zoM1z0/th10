# Enemy ECL file loader: repair the actual FileSystem call dependency

Original Japanese TH10 v1.00a `0x0040CD20..0x0040CD7E` is a complete
**95-byte** derived-ECL-resource `LoadFile` virtual owner. It clears the real
`0x00497C38` filename buffer, concatenates the caller filename, calls the
retail file loader at **`0x0044B360`**, passes the result to the real ECL
script-data parser at **`0x00450220`**, and returns 0 or -1 according to the
parser result. The *actual* `0x0044B360` callee is the already maintained
`FileSystem::OpenFile` implementation. A previous local declaration
`EnemyLoadFileBytes` incorrectly represented this genuine dependency as an
independent function. That symbol was replaced by the real
`FileSystem::OpenFile` declaration in `src/FileSystem.hpp`, and the actual
owner call in `src/Enemy.cpp` now names the real implementation.

## Full target codegen and what remains

The pinned original target has a **private** ABI for `FileSystem::OpenFile`:
filename in **EAX**, and mode/size-output on the stack. The maintained
public C++ `FileSystem::OpenFile` declaration is `__stdcall` with all three
arguments. In the selected `Enemy.cpp` LTCG owner, the function still emits
**95 PDB-contribution bytes** (all fully decoded) and matches **85/95** after
strictly binding all **five** external address fields. Its remaining ten
ordinary byte differences at `+0x32..+0x3B` are a single contiguous call-setup
region: original `MOV ECX,EAX; PUSH 0; AND ECX,3; PUSH 0; MOV EAX,buffer`, versus
native `PUSH 0; MOV ECX,EAX; PUSH 0; AND ECX,3; PUSH buffer`. This is not a
semantically acceptable codegen exact match, nor is it just an address patch.
The exact register ABI must be recovered through the real caller/callee
source dependency, not fake register assignments.

The local **noncanonical diagnostic** in `verify-native.py` names
the exact target and five original links:
three `g_EnemyEclFilenameBuffer` fields at +0x0B/+0x1A/+0x3C,
`FileSystem::OpenFile` at +0x43, and `EclVmScriptDatabase::AddScriptData`
at +0x4B. The [full strict comparison](native-summary.json) and the independently executable
[verifier](verify-native.py) record the
complete owner, original image hash, PDB contribution and ten mismatch bytes.
No entry was added to the canonical exact matches.

Several concrete source/compiler models were rejected on real owner results:
linking the complete actual `FileSystem.cpp` without an inline boundary
expanded the owner to 247 bytes under `/GL`; preventing inlining with the
actual callee under alternate `/GS` profiles generated 97 or 95 bytes but
still failed to reproduce the original filename-in-EAX calling schedule.
These are **experimental failures**, not confirmed original source signatures.

Three directly affected source-owner exact guards were recompiled via a
single selected cold: EnemyManager constructor **29/29**, ECL resource
constructor **34/34**, and ECL host constructor **21/21**, for **84/84 bytes
exact** with original link fields intact. No full cold replay ran. Original
source ancestry, physical TU, product link and runtime remain outside the
scope of this checkpoint.

Reproduce narrowly:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit enemy-manager-constructor
scripts/repo-python docs/evidence/enemy-ecl-load-file-identity-20261102/verify-native.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-manager-constructor --unit enemy-ecl-resource-constructor \
  --unit enemy-ecl-host-constructor --json
scripts/repo-python scripts/ci.py
git diff --check
```
