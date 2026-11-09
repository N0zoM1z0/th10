# ECL ReadInt: native DWORD type-slot decrement

TH10 Japanese v1.00a original SHA-256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The signed ECL stack top is a native 32-bit i386 word. During ReadInt(-1), the stack pops one 4-byte scalar payload and then decrements four bytes again to read the preceding type-tag slot. Previously this second decrement computed unsigned modulo-32 arithmetic and converted it to signed int, an implementation-defined narrowing for high-bit values. The maintained C++ now decrements the existing signed int through its corresponding unsigned int lvalue, preserving the native bit pattern and original signed handling without an out-of-range cast. There is no new bounds check, extra stack consumption, rewritten type-tag conditional, forced register or binary patch.

Five genuine VC7.1 SP1 EclVm/Enemy /GL trials compared baseline, first-decrement-only, second-decrement-only, both direct-word and two memcpy bit-copy forms. Second-only retained the complete original-sized ReadInt (144/144, one target-bound field), ReadFloatValue (171/171, four fields) and EclVmContext::Run (7,020 original bytes, 6,997 equal, 189 original-bound fields and the same 23 ordinary mismatches). Changing the first decrement as well makes ReadInt 147 bytes and was rejected. The complete boss remains NONEXACT; no exact ledger credit is claimed. Trial script/receipts: .analysis/gpt-web/20261109/ecl-readint-native-pop-publication.py and build/gpt-web/20261109-live/ecl-readint-word-pop/results.jsonl.

Reproduce with selected canonical ecl-vm-read-int, ecl-vm-read-float-value, ecl-vm-stack-pop, and ecl-vm-host-run exact replays plus the complete ECL Run diagnostic. No historical full cold replay or runnable product claim is involved.
