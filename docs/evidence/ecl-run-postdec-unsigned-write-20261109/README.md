# ECL Run: native DWORD post-decrement publication

Japanese TH10 v1.00a SHA-256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The full 7,020-byte EclVmContext::Run at 0x0044E1A0 interprets ECL_VM_POST_DECREMENT_INT as a modulo-32-bit word decrement of the lvalue, then pushes the *original* value back to the typed VM stack. The original machine code subtracts and stores one DWORD. Previous C++ computed the unsigned decrement but converted the result back to signed int using static_cast<int>, which is implementation-defined above INT_MAX.

The installed code writes the native DWORD through the corresponding unsigned int type of the int destination. The signed int and corresponding unsigned int are permissible aliases in C++, so no separate destination object, input bounds repair, null check, memory order constraint, artificial calling convention, inline assembly or binary patch is introduced. All signed-bit representations are interpreted in the actual pinned i386/two's-complement execution model.

Seven separately compiled VC7.1 SP1 /GL two-source EclVm/Enemy graphs tested baseline, two memcpy forms, direct unsigned store, unsigned reference, union result and an unsigned temp. Both memcpy forms regressed by one matching byte. The selected direct unsigned assignment and unsigned reference preserve the full 7,020-byte extent, all 189 original-bound local/external fields, the exact ReadFloatValue callee and the same 23 remaining ordinary byte differences: 6,997/7,020. No additional canonical exact codegen credit is claimed.

Only affected exact units are replayed: ecl-vm-read-int-value (124), ecl-vm-read-float-value (171), ecl-vm-stack-push (96), ecl-vm-stack-pop (122) and ecl-vm-host-run (146-byte separate host entry). The full Run remains NONEXACT. Complete source variants and native comparison receipts are private at build/gpt-web/20261109-live/ecl-postdec-native-word/results.jsonl. No all-unit cold replay, full game run or original source identity claim.
