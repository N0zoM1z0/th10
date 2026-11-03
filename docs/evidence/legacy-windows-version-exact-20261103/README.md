# TH10 legacy Windows version classifier @0x0044BD10 — 260-byte canonical exact

Original TH10 Japanese 1.00a SHA-256 is specified in `config/target.toml`.
The complete dense-decoded target `0x0044BD10..0x0044BE13` has **260 bytes**,
followed by CC padding. The source mapping was previously absent; now
`src/LegacyWindowsVersion.cpp` contains natural, maintainable C++ for the
observed behavior. Important provenance limit: this CC-delimited target
function has no independently reviewed inbound entry reference and did not
appear in the older IDA function inventory. The original authored source
versus compiler-generated/retained origin is therefore **indeterminate** in
`config/function-origins.csv`; its original TU and name are also unproved.

## Original executable behavior — all six return paths

The function aligns the stack to 8 bytes, allocates a guarded 0xA4-byte
frame, uses exactly 37 `REP STOSD` writes to zero a 0x94-byte
`OSVERSIONINFOA`, fills the structure size field, and calls the real
`GetVersionExA` IAT slot at **0x0046612C**. It does not infer errors from
the API's return or use any modern version-mask substitutions. Native
platform-specific returns are:

- Win32s (platform 0): **31**;
- Windows 9x (platform 1), major 4: **95**, or **98** if minor != 0;
- Windows 9x otherwise: major × 100 + minor;
- Windows NT (platform 2), major 5: **2000**;
- Windows NT otherwise: `(major × 100 + minor) × 10`;
- unknown platform: **-1**.

Each of the six distinct native return tails checks the genuine stack
cookie at **0x00473660** through `@__security_check_cookie@4` at
**0x00458EA5**. The compiler generates all six epilogues independently,
including the unusual `NEG/SBB/AND 3` Windows 9x minor-version expression.

## Two independent complete codegen proofs

With pinned VC7.1 SP1 `/GL /GS`, the actual `DetectLegacyWindowsVersion`
entry in `src/LegacyWindowsVersion.cpp` compiles to **260 PDB-owned bytes**,
no cropped or alternate function. Direct original/linked decoding shows
**228/228 ordinary instruction bytes equal**. Exactly eight original target
address fields are present: +0x0D DIR32 cookie 0x00473660, +0x37 DIR32
`GetVersionExA` import 0x0046612C, and six distinct REL32
`@__security_check_cookie@4` calls at offsets +0x5C/+0x7D/+0x9E/+0xC8/
+0xE4/+0xFC to 0x00458EA5. Every field is independently bound and the
strict comparison reports **260/260 exact** with zero unmatched bytes.

Two serial **zero-reuse** cold linked-PE builds of the canonical manifest
unit `legacy-windows-version-0044bd10` produced **different** image hashes
and PDB GUIDs, yet both complete comparator receipts are exact. Retained
private reports: `.analysis/gpt-web/20261103/legacy-win-version-cold{1,2}.json`
and first compiler exploration at `legacy-version-probe.json`.
This grants codegen exactness for one physical 260-byte function; it does
**not** establish a real runtime call path, original authorship, product
linkage or modern Windows compatibility. We test this precise canonical unit
and the repository's seven exact-stage checks, not the whole cold cohort.
