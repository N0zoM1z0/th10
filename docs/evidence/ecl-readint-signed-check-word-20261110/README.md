# ECL ReadInt: signed interpretation of a native stack-top DWORD

Target: Japanese TH10 v1.00a, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

In EclVmContext::ReadInt at 0x0044FDB0, the -1 operand path first checks whether subtracting four from the low 32-bit stack-top word yields a nonnegative signed result. The previous natural C++ performed unsigned modulo arithmetic but converted the result with static_cast<int>, an implementation-defined conversion if the high bit is set. The revised C++ holds the native unsigned word in topBits and checks it through the corresponding signed int type without narrowing. This is allowed signed/unsigned integer aliasing of the same 32-bit object on the pinned i386 compiler. Both payload and type-slot decrements, type-tag interpretation and calls remain unchanged; there is no new overflow check, guard, branch or target-byte patch.

Five pinned VC7.1 SP1 two-TU EclVm/Enemy /GL trials tested baseline, a named signed alias, a union, a direct signed counterpart read and an explicit sign-bit predicate. Named signed-alias and signed-counterpart versions retained the complete ReadInt PDB contribution (144/144 original bytes and one bound field) and the full 7,020-byte EclVmContext::Run (6,997/7,020 matching, all 189 bound original fields and the same 23 differences), with the 171-byte ReadFloatValue exact. The explicit sign-bit expression changed ReadInt to 147 bytes and was rejected.

Trial/receipt: .analysis/gpt-web/20261109/ecl-readint-check-word-alias-v2.py and build/gpt-web/20261109-live/ecl-readint-guard-alias/results.jsonl. Rebuild only the selected ReadInt/ReadFloat/stack/host exact units plus the complete Run comparator. The boss remains NONEXACT and no additional exact-ledger credit is claimed.
