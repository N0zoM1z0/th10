# TH10 four-advance RNG /GS owner — full 155-byte canonical exact

Target is the original SHA-256 pinned Japanese TH10 v1.00a, physical reviewed
owner `0x0044BA80..0x0044BB1A`, **155 bytes**. Ghidra's former opaque
"state helper" attribution hid four inline RNG steps. The member receiver
is ECX; `seed` is at +0, `generationCount` at +4. The target constructs a
32-bit result from the FIRST two 16-bit draws (first low, second high),
then performs TWO additional seed-advance steps and increments the counter
by FOUR, not two. It returns through a VC7.1 `/GS` security-cookie tail.
There is no independent incoming native xref, so original source name,
physical TU and authored/compiler origin remain unknown.

The natural source `RngView::GetRandomU32SkipNextPair` in
`src/RandomAdvance.cpp` keeps all four genuine `GetRandomU16()` calls and
four semantically meaningful array entries; it reads the first two short
results as a single 32-bit word. Unlike a simple shift/OR of promoted
locals, this source preserves the target's source-level result buffer and
causes MSVC 7.1 SP1's real `/GL /GS` pipeline to create the native stack
cookie, 12-byte local frame and identical register-allocation schedule.
It does not force registers, insert machine-code bytes or patch the target.

## Complete proof

Two independent zero-reuse **selected** cold PDB/PE builds using the true
`src/RandomAdvance.cpp` + `src/RandomMath.cpp` compiler graph, pinned
VC7.1 SP1 `/GL /GS`, produce the complete original 155-byte body and
exactly two externally resolved fields. The full body, including every
ordinary byte, prologue, individual draw, counter update and tail, matches:

| Native field | Target address | Binding |
| --- | --- | --- |
| +0x04 `DIR32` | `0x00473660` | `___security_cookie` |
| +0x97 `REL32` | `0x00458EA5` | `@__security_check_cookie@4` |

Both cold results: **155/155 exact, 2/2 fields, 0 differences**.
Cold 1: PE SHA-256 `acc6e06429ad1c680c45d85535739941d4c5b23dfabf141dec218ade1744f4f1`,
PDB SHA-256 `8a70c2de9703f3222d3938f55c34926d4aa7b898c8fd305d908be27e96caf0b8`.
Cold 2: PE SHA-256 `76404af0f3efb07736118da36fd7782a8fffeb7139502045f4e09b136676740e`,
PDB SHA-256 `b4d466599b0dc260c1944cd89824ee96de7000e14f6ca1841e7a39f197a793d7`.
The independent PDB and PE identities differ while all native bindings agree.

The second selected cold also replayed the five affected previously exact
RNG functions, all of which remain exact: U16 39, U32 78, duplicate U32 78,
F32 109 and signed F32 115. **574/574 bytes across six owners** in that
replay. The four other natural array/result-source controls produced 137,
145, 161 or 166 bytes or different field geometry; no results were cherry
picked by cropping the target extent.

Scoped reproduction only (never default/all-units replay):

    scripts/repo-python scripts/replay-exact-units.py \
      --unit rng-get-random-u32-skip-next-pair \
      --unit rng-get-random-u16 --unit rng-get-random-u32 \
      --unit rng-get-random-u32-duplicate --unit rng-get-random-f32 \
      --unit rng-get-random-f32-signed --json

Ignored local evidence: `.analysis/gpt-web/20261104/rng-four-cold1.json`,
`rng-four-cold2-plus-guards.json`, `rng-diff-155.txt`, and
`rng-array-results.json`. The original executable, toolchain and compiled
artifacts remain private. This is **exact owner codegen only**; authored
origin, native whole-product closure, runtime correctness and portability
are distinct and remain unproven.
