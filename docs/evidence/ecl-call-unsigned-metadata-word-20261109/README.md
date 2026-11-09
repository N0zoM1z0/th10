# Native unsigned CALL metadata cursor in ECL StartSubroutine

Original Japanese TH10 v1.00a SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmContext::StartSubroutine @0x0044DF70` handles counted subroutine names and 8-byte tagged argument records. The original machine code advances each metadata address as a native **32-bit address word**, even for script-supplied inline name lengths that cross the signed `int` range. The old maintained C++ stored this cursor in `int` but computed it with unsigned arithmetic, then converted the result back to signed at initialization and on every record. That conversion is implementation-defined out of range, despite all three later metadata reads casting the value back to unsigned.

The source now declares `unsigned int metadataOffset` and increments it directly by `sizeof(EclVmCallArgumentView)`; all three type-tag address consumers use the unsigned word directly. This removes the conversion dependency without adding bounds checks, changing the stream layout, altering the signed argument-index comparison, or changing the original 8-byte stride. The explicit initial unsigned cast retains negative inline-name bit patterns and wraparound. The original Win32 little-endian/i386 layout remains the target rather than a portable file-parser guarantee.

## Full-owner validation

Actual pinned MSVC VC7.1 SP1 `/GL` builds of the real `EclVm.cpp`/`Enemy.cpp` graph compared four natural forms: the installed prior signed local, unsigned declaration only, unsigned declaration plus direct increment, and the full unsigned metadata expression in all tag reads. All four emitted the same **551-byte** `StartSubroutine` candidate and four linked field positions, with no change in candidate bytes. The original is **550 bytes**, so **this function is still nonexact** and receives zero new exact codegen credit. Each variant's entire `Run @0x0044E1A0` candidate remained **7,020 bytes**, **6997/7020 original-bound bytes** and all **189/189 original link fields**. The comparison is not a prefix or a byte-patch shortcut.

The installed-source selected replay **passed** four already accepted ECL units: `ReadIntValue` 124/124 (one field), `ReadFloatValue` 171/171 (four fields), `EclVmStackView::Pop` 122/122 (one field), and ECL host `Run` 146/146 (four fields). This totals **563/563 original bytes, 10 original-bound fields**. An independent second rebuild of the host and stack-Pop pair passed **268/268**. Current-source diagnostics again show the 7,020-byte large `Run` matching **6,997/7,020**, all **189 original-bound fields**, and its 171-byte float callee exact. No full historical cold replay was executed. The ignored receipts are `.analysis/gpt-web/20261109/ecl-call-word-installed-guards1.json`, `ecl-call-word-installed-guards2.json`, and `ecl-call-word-large-diagnostic.json`.

Reproduce the selected checks with:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-int-value --unit ecl-vm-read-float-value \
  --unit ecl-vm-stack-pop --unit ecl-vm-host-run --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run
scripts/repo-python scripts/ci.py
```

The noncanonical large-owner diagnostic returns nonzero until `Run` itself becomes fully exact. The source/target distinction, original physical translation unit, full product/runtime, and portability remain open.
