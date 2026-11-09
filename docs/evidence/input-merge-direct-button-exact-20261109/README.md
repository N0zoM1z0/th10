# TH10 DirectInput button merge — exact private helper

The original Japanese TH10 v1.00a `MergeDirectButton @0x0044A130` is a complete
37-byte private function. The maintained C++ in `src/InputPolling.cpp` uses a
signed short binding, tests the high bit in the DirectInput state record, ORs
an action into the output word and returns the selected action. It reads the
input again for the return value, as observed in the original instructions.

Two independent VC7.1 SP1 normal-COFF builds with the canonical
`/O2 /Gy /GF /Oi` profile reproduce **37/37** original bytes, both RET
exits, and zero relocation fields. The declared unit
`input-merge-direct-button-0044a130` compiles the maintained InputPolling.cpp
without changing shared headers or replacing the executable's instructions.

```sh
scripts/repo-python scripts/build-match-unit.py --unit input-merge-direct-button-0044a130
scripts/repo-python scripts/compare-coff-function.py --unit input-merge-direct-button-0044a130 --json
```

Original source identity and whether this unreferenced private helper was
hand-written or compiler-outlined remain unresolved. Its origin stays
**indeterminate**, without adding authored-origin credit. Adjacent
`MergeWinMmButton` and the larger joystick/polling loops are not exact.
