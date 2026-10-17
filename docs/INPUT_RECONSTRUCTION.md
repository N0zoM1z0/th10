# Input reconstruction: host-test model checkpoint

This checkpoint preserves independently authored C++03 models of the keyboard owner at `0x44A5F0` and joystick mapper at `0x44A190`. It is a self-contained source-and-test checkpoint, **not production integration, a native COM/layout/ABI implementation, a source-mapping update, or byte-exactness credit**. No configuration ledger or production `src` file is changed.

## Run from a clean checkout

Requirements: Python 3 and `g++` on an LP64 host (8-bit bytes, 16-bit short, 32-bit int, and at least 64-bit unsigned long for the independent WinMM test oracle). Linux x86-64 is the validated host. The model itself uses guarded 16-/32-bit types; it does not rely on 64-bit unsigned long. Windows LLP64 is not a supported host for that particular oracle as currently packaged.

    python3 -O probes/input-model/run_tests.py

The script resolves all inputs relative to itself, so any current directory works. It needs no game binary, network, SDK, private receipts, sibling workspace, certificates or preexisting report. It checks `SOURCE_SHA256.json` using explicit UTF-8 canonical-LF hashes (CRLF pairs become LF; invalid UTF-8, NUL bytes and bare CR are rejected), compiles the actual model in C++03 mode with `-pedantic-errors -Wall -Wextra -Werror -O2`, and runs six suites serially. Temporary executables/mutants are automatically removed. Each compile/run has a 30-/20-second timeout. Python lowers process priority when supported; no external `nice` utility is required. Optional `--report results.json` saves the printed JSON report to a chosen location.

Intentional source changes require regenerating the source-hash manifest, then reviewing and rerunning tests. The manifest hashes are canonical-LF text hashes, not checkout-byte hashes: the repository’s CRLF policy for C/C++ files does not change them. The same canonical text is used when constructing mutants. Other text changes still fail the integrity gate. The manifest is an integrity record, not a proof. This public runner **does not rerun the original target proofs**.

To regression-test repository line endings:

    python3 -O probes/input-model/run_tests.py --check-packaging

This runs all six suites/64 mutants in a disposable relocated copy with every C/C++ file converted to CRLF, then changes a real action constant and requires the source-integrity gate to reject it. The original checkout is untouched; no `.gitattributes` change is needed.

## Source organization and interface boundary

`components/` contains the real keyboard, button and transition algorithms. `di_axes.*` and `winmm_axes.*` contain real axis algorithms. `input_owner.*` composes them without missing dependencies, axis stubs or test doubles. `tests/` contains independent finite oracles and typed mock platform responses. `test_cases.json` defines a finite source-mutant set; the runner compiles each mutated actual implementation against the unchanged tests.

`Environment`, `InputRecord` and `AxisCapabilities` are logical, fixture-owned host containers. They do not assert a 106-byte native input record, a 404-byte native capability record, or real process-global allocation. The virtual platform interfaces are typed test seams, **not DirectInput vtables or recovered x86 calling conventions**. Their destructors and method order must never be used as a COM layout. No pointer cast can turn these host types into native devices. Sampled value structures assert keyboard256, WinMM52 and DI272 sizes and relevant field offsets.

## Target-derived contracts retained

All arithmetic below is part of the documented normal domain, not guessed convenience behavior.

### Keyboard and orchestration

Disabled keyboard supplies zero but still invokes joystick mapping. Mode bit `0x200` selects DirectInput; clear selects GetKeyboardState. The Win32 buffer receives no initialization before the API, and the API's BOOL is ignored when output bytes are defined. Do not add a zeroing fallback or early failure branch. DirectInput keyboard status **exactly zero** maps keys; every nonzero status invokes Acquire exactly once and supplies zero, regardless of Acquire's result.

Only each key byte's high bit contributes. The following rows name action bits 0 through 14; entries are hexadecimal byte indices, with alternatives ORed:

| Bit | Win32 | DirectInput |
|---|---|---|
| 0 | 5A | 2C |
| 1 | 58 | 2D |
| 2 | 10 | 2A |
| 3 | 1B | 01 |
| 4 | 26,68 | C8,48 |
| 5 | 28,62 | 50,D0 |
| 6 | 25,64 | CB,4B |
| 7 | 27,66 | CD,4D |
| 8 | 11 | 1D |
| 9 | 51 | 10 |
| 10 | 53 | 1F |
| 11 | 24,50 | C7,19 |
| 12 | 0D | 1C |
| 13 | 44 | 20 |
| 14 | 52 | 13 |

Diagonal contributions: Win32 `69→90, 67→50, 61→60, 63→A0`; DI `49→90, 47→50, 4F→60, 51→A0`. Keyboard bit15 is clear. `tests/key_weights.hpp` freezes independently receipt-derived per-byte weights; it is not generated from the implementation.

UpdateInput passes the same logical record index and controller selector0 to MapJoystick, then updates that record's transition prefix. The relevant owner contract is low16 input/output; full32 return/high-word equality is not claimed.

### Mapper and buttons

Mode bit `0x400` selects DI; clear selects WinMM. WinMM ID is `rawPad != 0`, while capability/device selection uses rawPad. Native observations bind record selection to base `0x474E30 + 106*index`, capability records to `0x4918B8 + 404*rawPad`; these address facts do not create storage in this model.

API-visible WinMM input is size52, flags255, remaining44 bytes zero. Its nonzero result returns incoming low16. DI Poll negative triggers one Acquire plus at most400 additional attempts, continuing only for exact `0x8007001E`, then returns incoming low16 without another Poll/GetState. Nonnegative Poll proceeds to GetState with an all-zero272-byte buffer; negative GetState returns incoming low16, nonnegative proceeds to mapping. This deliberately differs from keyboard's exact-zero predicate.

Five signed16 bindings at native offsets `58/5A/5C/5E/68` select action masks `1/2/4/8/100` (hex). Negative values contribute nothing. Valid nonnegative indices are WinMM0..31 and DI0..127. WinMM uses the indexed bit of an unsigned32 word; DI uses the selected byte's high bit. All contributions OR into incoming mask. No clamping, modulo-indexing or behavior for invalid positive bindings is invented.

### Axes

DI uses signed32 X/Y and signed16 thresholds at native words `0x491D5E/0x491D60`. Independently OR `0x40` if X < -int32(tX), `0x80` if X > int32(tX), `0x10` if Y < -int32(tY), `0x20` if Y > int32(tY). Promote before negation: threshold -32768 negates to +32768. Negative thresholds can set both opposing bits; do not use else-if.

WinMM uses unsigned32 position/min/max. For each axis:

    midpoint = uint32(min + max) >> 1
    quarter = uint32(max - min) >> 2
    lower = uint32(midpoint - quarter)
    upper = uint32(midpoint + quarter)

Independently OR the same X/Y direction bits for axis < lower and axis > upper. Overflow reduction occurs **before** shifts. Do not widen/reassociate arithmetic, assume min <= max, assume lower <= upper, or make equality active. The verified domain includes all unsigned32 values and inverted/wrapped ranges. Native field links bind X/Y to WinMM offsets8/12, capabilities to offsets36/40/44/48, and DI X/Y to offsets0/4, buttons48.

### Transition

The explicit prefix has current/previous/repeated/pressed/released unsigned16 fields at offsets0/2/4/6/8 and sixteen unsigned16 counters at10; size42 is compile-guarded. Save old current, set previous=old/current=now, clear repeated. For every bit0..15, clear an unheld counter; otherwise h=uint16(counter+1), and if unsigned h>=26 set its repeat bit and store h-8, otherwise store h. Counter65535 wraps to0 before the comparison. Pressed=(now XOR old)&now; released=(now XOR old)&uint16(~now). Return now.

## Evidence provenance and limits

Reference target SHA-256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`; inspected clean source baseline: `08950988561fe23cb89b634778b554c250a57f90`. Target owner byte extents were833 bytes at44A190 and850 at44A5F0. Independent target analysis established conditional bit-vector/formula equivalence and then composed API argument/buffer links, success/failure predicates, five-button continuity, axis boundary identities, normal cookie return, caller footprint and full frame restoration. This is stronger than matching a handful of examples, but remains conditional on the normal domain.

Useful audit identifiers, retained for continuity rather than as runnable dependencies:
- key mapping `d089dbafbd6b45d481dd91edfafbd89c`
- transition `47bf577403924cad9cea954f286a3513`
- button mini-kernels `52c04f67a370409a8f800dd75cec2259`, continuity `c640e040bd01434e827c4f50c536b25a`
- success/initialization `337afc32d0404e77859b1b68760785c9`, axis/return bridge `c57ad098eecb4418b3b8b5e03c437804`
- DI axes `ef068e6f37164934890275ebc6a7f0fd`, WinMM axes `12103789c7f647a1bf0cf36049049599`
- reviewed mapper composition SHA-256 `8257740cbd0c9bb053b0ccbc2c55c5248015ed1ed60de86be03bb022b6217dbb`

The original target-verification programs, target data and detailed receipts are intentionally not included. This package preserves source contracts, provenance and independently executable finite tests, not reproducibility of those separate target proofs. No unlicensed upstream source body was imported.

Require valid typed APIs, defined matching output bytes, valid object/device/capability indices and binding domains, accessible stable disjoint storage, nonwrapping addresses, no asynchronous mutation/reentrancy, coupled API responses, ordinary admitted ABI/DF/string-store conditions and normal cookie return. Failed/unpopulated Win32 output lies outside the equivalence domain. Hardware determinism, device creation, complete process allocation and abnormal cookie failure behavior are not established.

## Validation and next integration step

Six baselines: keys270,848 cases; transition1,378,304; buttons804,864; DI axes10,878,976; WinMM axes827,560; owner65,578. All64 finite mutants must produce a normal expected mismatch exit and marker. Compile failure, crash, timeout, survival or unrelated exit never count as kills. Tests execute real source, preserve independent oracles, cover counter wrap and bit15, signed/unsigned threshold boundaries, negative overlap, malformed-order ranges within valid arithmetic domains, incoming mask preservation, acquisition counts and all four keyboard/mapper combinations. They are not a universal proof against every possible source bug.

Before native integration: use one canonical real input declaration/storage owner, preserve `FrontEndInputView::IsRepeated` class/member identity and +4/+6 meanings, reconcile FrontEnd’s current-word consumer, GameManager’s current-word alias, Main’s input-edge/aggregate fields and all affected source users, replace host seams with real supported platform interfaces/call graph, replay canonical helper and impacted FrontEnd guards, then measure native bytes. Do not add aliases, diagnostic placeholder storage, arbitrary array extents, ABI pointer casts, assembly bridges or codegen variants. Record source recovery and byte-exactness separately.
