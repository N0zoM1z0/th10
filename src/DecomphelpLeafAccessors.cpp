// Reconstructed C++ leaf candidates imported from th10-decomphelp-forN0.
// Exactness is granted only by canonical target-bound replay; see docs/DECOMPHELP_AUDIT.md.
#include "DecomphelpLeafTypes.hpp"

// The /Yu PCH swallows the command line optimization flags, so the original
// /O2 (+/GL, whole program) codegen has to be forced per TU. See the Chain TU
// for the same trick.
#pragma optimize("gty", on)

extern "C" void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)

// LEAF_GLOB addresses an absolute image VA. The verifier (scripts/verify_leaves.py)
// remaps it into its mmap via leaf_img_base; in the real rebuild the image
// base is 0x400000, so the mapping is the identity.
#ifndef LEAF_GLOB
#define LEAF_GLOB(a) ((u8 *)(uintptr_t)(a))
#endif

// ---------------------------------------------------------------------------
// Real C++ reconstruction of the small leaf accessors that used to live in
// Leaves.cpp as naked __asm transcriptions.
//
// These functions are the trivial getters/setters/flag helpers ZUN wrote as
// one-line member functions; the original build (cl 13.10 /O2 /GL /LTCG) gave
// them a custom register calling convention (first argument in eax, second in
// ecx). Under our own /GL + /LTCG link the optimizer derives the very same
// convention, so plain C++ reproduces the original bytes without any asm.
//
// The owning structures are not identified yet, so the functions keep an
// address based name and take a raw byte pointer. They are real C++ though -
// no __asm, no naked - and each one is byte compared by reccmp.
// ---------------------------------------------------------------------------

namespace th10
{
namespace leaf
{

// 0x404990: return this->field_44
__declspec(noinline) u32 Fn00404990(u8 *pThis)
{
    return *(u32 *)(pThis + 0x44);
}

// 0x404c90: this->field_84 = value
__declspec(noinline) void Fn00404C90(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x84) = value;
}

// 0x404d30: this->field_84 = 0
__declspec(noinline) void Fn00404D30(u8 *pThis)
{
    *(u32 *)(pThis + 0x84) = 0;
}

// 0x404f20: this->flags_35c &= ~1
__declspec(noinline) void Fn00404F20(u8 *pThis)
{
    *(u32 *)(pThis + 0x35c) &= ~1u;
}

// 0x405150: pOther->field_48 = value (value in eax, object in ecx)
__declspec(noinline) void Fn00405150(u8 *pObj, u32 value)
{
    *(u32 *)(pObj + 0x48) = value;
}

// 0x408790: bit 0 of (flags_58 >> 2 | flags_58)
__declspec(noinline) u32 Fn00408790(u8 *pThis)
{
    u32 flags;

    flags = *(u32 *)(pThis + 0x58);
    return ((flags >> 2) | flags) & 1;
}

// 0x4087c0: bit 1 of flags_58
__declspec(noinline) u32 Fn004087C0(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x58) >> 1) & 1;
}

// 0x4087d0: this->word_304 = value
__declspec(noinline) void Fn004087D0(u16 value, u8 *pThis)
{
    *(u16 *)(pThis + 0x304) = value;
}

// 0x408820: &this->field_1068
__declspec(noinline) u8 *Fn00408820(u8 *pThis)
{
    return pThis + 0x1068;
}

// 0x408830: this->array_30[index]
__declspec(noinline) u32 Fn00408830(i32 index, u8 *pThis)
{
    return ((u32 *)(pThis + 0x30))[index];
}

// 0x408840: this->flags_2a18 &= ~1
__declspec(noinline) void Fn00408840(u8 *pThis)
{
    *(u32 *)(pThis + 0x2a18) &= ~1u;
}

// 0x408850: this->flags_2a18 |= 1
__declspec(noinline) void Fn00408850(u8 *pThis)
{
    *(u32 *)(pThis + 0x2a18) |= 1;
}

// 0x4088a0: &pArray->entries[index] with a 0x437c stride and a 8 byte header
__declspec(noinline) u8 *Fn004088A0(i32 index, u8 *pArray)
{
    return pArray + index * 0x437c + 8;
}

// 0x408aa0: return 0
__declspec(noinline) i32 Fn00408AA0(void)
{
    return 0;
}

// 0x408800: empty body
__declspec(noinline) void Fn00408800(void)
{
}

// 0x40acc0: this->word_8 & mask
__declspec(noinline) u32 Fn0040ACC0(u8 *pThis, u32 mask)
{
    return *(u16 *)(pThis + 8) & mask;
}

// 0x40ad10: this->field_4 = this->field_0
__declspec(noinline) void Fn0040AD10(u8 *pThis)
{
    *(u32 *)(pThis + 4) = *(u32 *)pThis;
}

// 0x40b030: return 1
__declspec(noinline) i32 Fn0040B030(void)
{
    return 1;
}

// 0x40b500: bit 0 of flags_20
__declspec(noinline) u32 Fn0040B500(u8 *pThis)
{
    return *(u32 *)(pThis + 0x20) & 1;
}

// 0x40c5d0: zero the two dwords at +0x1000
__declspec(noinline) void Fn0040C5D0(u8 *pThis)
{
    *(u32 *)(pThis + 0x1000) = 0;
    *(u32 *)(pThis + 0x1004) = 0;
}

// 0x40c940: zero the first three dwords (a vector reset)
__declspec(noinline) void Fn0040C940(u8 *pThis)
{
    *(u32 *)(pThis + 0) = 0;
    *(u32 *)(pThis + 4) = 0;
    *(u32 *)(pThis + 8) = 0;
}

// 0x40cc40: this->field_4->field_4
__declspec(noinline) u32 Fn0040CC40(u8 *pThis)
{
    return *(u32 *)(*(u8 **)(pThis + 4) + 4);
}

// 0x40ce40: this->field_9eb8->field_78
__declspec(noinline) u32 Fn0040CE40(u8 *pThis)
{
    return *(u32 *)(*(u8 **)(pThis + 0x9eb8) + 0x78);
}

// 0x421cd0: return this->byte_13d
__declspec(noinline) u32 Fn00421CD0(u8 *pThis)
{
    return *(u8 *)(pThis + 0x13d);
}

// 0x42a810: write the 'USER' magic
__declspec(noinline) void Fn0042A810(u8 *pThis)
{
    *(u32 *)pThis = 0x52455355;
}

// 0x434bf0: pTable->entries_b0[index * 9]
__declspec(noinline) u32 Fn00434BF0(i32 index, u8 *pTable)
{
    return ((u32 *)(pTable + 0xb0))[index * 9];
}

// 0x4381e0: reset a small string-ish header (capacity 0xf, size 0, empty)
__declspec(noinline) void Fn004381E0(u8 *pThis)
{
    *(u32 *)(pThis + 0x18) = 0xf;
    *(u32 *)(pThis + 0x14) = 0;
    *(u8 *)(pThis + 4) = 0;
}

// 0x449860: *a == *b
__declspec(noinline) i32 Fn00449860(u8 *a, u8 *b)
{
    return *(i32 *)a == *(i32 *)b;
}

// 0x44c620: reset a (scale, bias) pair to (1.0f, 0)
__declspec(noinline) void Fn0044C620(u8 *pThis)
{
    *(u32 *)(pThis + 4) = 0;
    *(u32 *)(pThis + 0) = 0x3f800000;
}

// 0x404410: mov dword ptr [eax + 1ee4h],1h; ret
__declspec(noinline) void Fn00404410(u8 *pThis)
{
    *(u32 *)(pThis + 0x1ee4) = 0x1;
}

// 0x404ce0: mov eax,dword ptr [eax + 84h]; ret
__declspec(noinline) u32 Fn00404CE0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x84);
}

// 0x405160: mov dword ptr [eax + 44h],ecx; ret
__declspec(noinline) void Fn00405160(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x44) = value;
}

// 0x4051b0: mov byte ptr [eax + 3ada6ch],cl; ret
__declspec(noinline) void Fn004051B0(u8 value, u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada6c) = value;
}

// 0x405340: mov dword ptr [eax + 73245ch],1h; mov dword ptr [eax + 732458h],ecx; ret
__declspec(noinline) void Fn00405340(u8 *pThis, u32 value)
{
    *(u32 *)(pThis + 0x73245c) = 0x1;
    *(u32 *)(pThis + 0x732458) = value;
}

// 0x405360: mov eax,dword ptr [eax + 8h]; ret
__declspec(noinline) u32 Fn00405360(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8);
}

// 0x405390: mov eax,dword ptr [eax + 4h]; ret
__declspec(noinline) u32 Fn00405390(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x4053a0: mov eax,dword ptr [eax + 60h]; and eax,1h; ret
__declspec(noinline) u32 Fn004053A0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x60) & 1;
}

// 0x4053d0: mov eax,dword ptr [eax + 3ch]; ret
__declspec(noinline) u32 Fn004053D0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x3c);
}

// 0x4054a0: add eax,3c0h; ret
__declspec(noinline) u8 * Fn004054A0(u8 *pThis)
{
    return pThis + 0x3c0;
}

// 0x405560: mov eax,dword ptr [eax + 378ch]; and eax,1h; ret
__declspec(noinline) u32 Fn00405560(u8 *pThis)
{
    return *(u32 *)(pThis + 0x378c) & 1;
}

// 0x405580: mov eax,dword ptr [eax + 3788h]; ret
__declspec(noinline) u32 Fn00405580(u8 *pThis)
{
    return *(u32 *)(pThis + 0x3788);
}

// 0x405590: mov eax,dword ptr [eax + ecx*4+10h]; ret
__declspec(noinline) u32 Fn00405590(i32 index, u8 *pThis)
{
    return ((u32 *)(pThis + 0x10))[index];
}

// 0x405850: mov eax,1h; ret
__declspec(noinline) i32 Fn00405850(void)
{
    return 1;
}

// 0x405b90: mov ax,word ptr [eax + 8h]; ret
__declspec(noinline) u16 Fn00405B90(u8 *pThis)
{
    return *(u16 *)(pThis + 0x8);
}

// 0x405bb0: mov eax,dword ptr [eax + 28h]; ret
__declspec(noinline) u32 Fn00405BB0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x28);
}

// 0x405ca0: mov eax,dword ptr [eax + 3e0b50h]; ret
__declspec(noinline) u32 Fn00405CA0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x3e0b50);
}

// 0x408780: mov eax,dword ptr [eax + 58h]; shr eax,2h; and eax,1h; ret
__declspec(noinline) u32 Fn00408780(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x58) >> 2) & 1;
}

// 0x4088b0: mov eax,dword ptr [eax + 9ec8h]; ret
__declspec(noinline) u32 Fn004088B0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x9ec8);
}

// 0x408ab0: xor eax,eax; ret
__declspec(noinline) i32 Fn00408AB0(void)
{
    return 0;
}

// 0x40ac00: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn0040AC00(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x40ac10: mov dword ptr [eax + 8974h],ecx; ret
__declspec(noinline) void Fn0040AC10(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x8974) = value;
}

// 0x40acd0: movzx  eax,word ptr [eax + 6h]; and eax,ecx; ret
__declspec(noinline) u32 Fn0040ACD0(u8 *pThis, u32 mask)
{
    return *(u16 *)(pThis + 0x6) & mask;
}

// 0x40ad50: mov dword ptr [eax + 0d0h],ecx; ret
__declspec(noinline) void Fn0040AD50(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0xd0) = value;
}

// 0x40ad80: mov dword ptr [eax + 8h],ecx; ret
__declspec(noinline) void Fn0040AD80(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x8) = value;
}

// 0x40ad90: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn0040AD90(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x40ae30: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn0040AE30(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x40ae40: mov eax,dword ptr [eax + 4h]; ret
__declspec(noinline) u32 Fn0040AE40(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x40b040: mov eax,1h; ret
__declspec(noinline) i32 Fn0040B040(void)
{
    return 1;
}

// 0x40b050: mov eax,1h; ret
__declspec(noinline) i32 Fn0040B050(void)
{
    return 1;
}

// 0x40b060: mov eax,1h; ret
__declspec(noinline) i32 Fn0040B060(void)
{
    return 1;
}

// 0x40b310: xor eax,eax; ret
__declspec(noinline) i32 Fn0040B310(void)
{
    return 0;
}

// 0x40b470: mov eax,dword ptr [eax + 74h]; shr eax,2h; and eax,1h; ret
__declspec(noinline) u32 Fn0040B470(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x74) >> 2) & 1;
}

// 0x40b510: mov eax,dword ptr [eax + 20h]; shr eax,1; and eax,1h; ret
__declspec(noinline) u32 Fn0040B510(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x20) >> 1) & 1;
}

// 0x40b520: add eax,1d86ch; ret
__declspec(noinline) u8 * Fn0040B520(u8 *pThis)
{
    return pThis + 0x1d86c;
}

// 0x40c460: mov dword ptr [eax],ecx; ret
__declspec(noinline) void Fn0040C460(u32 value, u8 *pThis)
{
    *(u32 *)pThis = value;
}

// 0x40c520: mov eax,dword ptr [eax + 4h]; ret
__declspec(noinline) u32 Fn0040C520(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x40c530: movzx  eax,word ptr [eax]; and eax,ecx; ret
__declspec(noinline) u32 Fn0040C530(u8 *pThis, u32 mask)
{
    return *(u16 *)pThis & mask;
}

// 0x40c5b0: mov eax,dword ptr [eax + 50h]; ret
__declspec(noinline) u32 Fn0040C5B0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x50);
}

// 0x40c5c0: mov eax,dword ptr [eax + 34h]; ret
__declspec(noinline) u32 Fn0040C5C0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x34);
}

// 0x40c5e0: xor eax,eax; ret
__declspec(noinline) i32 Fn0040C5E0(void)
{
    return 0;
}

// 0x40c820: mov eax,dword ptr [eax + ecx*4+0ch]; ret
__declspec(noinline) u32 Fn0040C820(i32 index, u8 *pThis)
{
    return ((u32 *)(pThis + 0xc))[index];
}

// 0x40c890: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn0040C890(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x40c8a0: mov dword ptr [eax],ecx; ret
__declspec(noinline) void Fn0040C8A0(u32 value, u8 *pThis)
{
    *(u32 *)pThis = value;
}

// 0x40c8b0: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn0040C8B0(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x40c8c0: mov dword ptr [eax],ecx; ret
__declspec(noinline) void Fn0040C8C0(u32 value, u8 *pThis)
{
    *(u32 *)pThis = value;
}

// 0x40c8d0: mov eax,dword ptr [eax + 4h]; ret
__declspec(noinline) u32 Fn0040C8D0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x40c8e0: mov dword ptr [eax + 4h],ecx; ret
__declspec(noinline) void Fn0040C8E0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x4) = value;
}

// 0x40c8f0: mov eax,dword ptr [eax + 8h]; ret
__declspec(noinline) u32 Fn0040C8F0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8);
}

// 0x40c900: mov dword ptr [eax + 8h],ecx; ret
__declspec(noinline) void Fn0040C900(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x8) = value;
}

// 0x40c980: mov dword ptr [eax],ecx; ret
__declspec(noinline) void Fn0040C980(u32 value, u8 *pThis)
{
    *(u32 *)pThis = value;
}

// 0x40c990: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn0040C990(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x40cc10: add eax,2ch; ret
__declspec(noinline) u8 * Fn0040CC10(u8 *pThis)
{
    return pThis + 0x2c;
}

// 0x40cc20: add eax,13c0h; ret
__declspec(noinline) u8 * Fn0040CC20(u8 *pThis)
{
    return pThis + 0x13c0;
}

// 0x40cc30: add eax,23fch; ret
__declspec(noinline) u8 * Fn0040CC30(u8 *pThis)
{
    return pThis + 0x23fc;
}

// 0x40cd10: and dword ptr [eax + 30h],0fffffffeh; ret
__declspec(noinline) void Fn0040CD10(u8 *pThis)
{
    *(u32 *)(pThis + 0x30) &= ~0x1u;
}

// 0x40cda0: mov eax,dword ptr [eax + 458h]; ret
__declspec(noinline) u32 Fn0040CDA0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x458);
}

// 0x40cdb0: mov eax,dword ptr [eax + 3504h]; ret
__declspec(noinline) u32 Fn0040CDB0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x3504);
}

// 0x40cde0: mov byte ptr [eax + 3508h],1h; ret
__declspec(noinline) void Fn0040CDE0(u8 *pThis)
{
    *(u8 *)(pThis + 0x3508) = 0x1;
}

// 0x40cdf0: mov eax,dword ptr [eax + 78h]; ret
__declspec(noinline) u32 Fn0040CDF0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x78);
}

// 0x40ce00: mov dword ptr [eax + 9e90h],ecx; ret
__declspec(noinline) void Fn0040CE00(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x9e90) = value;
}

// 0x40ce70: add eax,9a48h; ret
__declspec(noinline) u8 * Fn0040CE70(u8 *pThis)
{
    return pThis + 0x9a48;
}

// 0x40ce90: or  dword ptr [eax + 378ch],8h; ret
__declspec(noinline) void Fn0040CE90(u8 *pThis)
{
    *(u32 *)(pThis + 0x378c) |= 0x8u;
}

// 0x40ced0: mov eax,dword ptr [eax + 378ch]; shr eax,3h; and eax,1h; ret
__declspec(noinline) u32 Fn0040CED0(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x378c) >> 3) & 1;
}

// 0x412da0: mov dword ptr [eax + 34h],ecx; ret
__declspec(noinline) void Fn00412DA0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x34) = value;
}

// 0x412df0: mov eax,dword ptr [eax + 34h]; ret
__declspec(noinline) u32 Fn00412DF0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x34);
}

// 0x412e00: mov dword ptr [eax + 34h],0h; ret
__declspec(noinline) void Fn00412E00(u8 *pThis)
{
    *(u32 *)(pThis + 0x34) = 0x0;
}

// 0x412f50: or  dword ptr [eax + 28h],1h; ret
__declspec(noinline) void Fn00412F50(u8 *pThis)
{
    *(u32 *)(pThis + 0x28) |= 0x1u;
}

// 0x412f60: mov eax,dword ptr [eax + 28h]; and eax,1h; ret
__declspec(noinline) u32 Fn00412F60(u8 *pThis)
{
    return *(u32 *)(pThis + 0x28) & 1;
}

// 0x412fa0: and dword ptr [eax + 28h],0fffffffeh; ret
__declspec(noinline) void Fn00412FA0(u8 *pThis)
{
    *(u32 *)(pThis + 0x28) &= ~0x1u;
}

// 0x41ff30: mov dword ptr [eax + 390h],ecx; ret
__declspec(noinline) void Fn0041FF30(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x390) = value;
}

// 0x41ff40: mov eax,dword ptr [eax + 150h]; shr eax,4h; and eax,1h; ret
__declspec(noinline) u32 Fn0041FF40(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x150) >> 4) & 1;
}

// 0x4200c0: mov eax,1h; ret
__declspec(noinline) i32 Fn004200C0(void)
{
    return 1;
}

// 0x421c90: mov eax,dword ptr [eax + 150h]; shr eax,6h; and eax,1h; ret
__declspec(noinline) u32 Fn00421C90(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x150) >> 6) & 1;
}

// 0x421ca0: mov eax,dword ptr [eax + 150h]; shr eax,5h; and eax,1h; ret
__declspec(noinline) u32 Fn00421CA0(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x150) >> 5) & 1;
}

// 0x421cb0: mov eax,dword ptr [eax + 150h]; shr eax,3h; and eax,1h; ret
__declspec(noinline) u32 Fn00421CB0(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x150) >> 3) & 1;
}

// 0x421cc0: mov eax,dword ptr [eax + 150h]; shr eax,1; and eax,1h; ret
__declspec(noinline) u32 Fn00421CC0(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x150) >> 1) & 1;
}

// 0x421ce0: mov eax,dword ptr [eax + 150h]; and eax,1h; ret
__declspec(noinline) u32 Fn00421CE0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x150) & 1;
}

// 0x421cf0: mov eax,dword ptr [eax + 150h]; shr eax,2h; and eax,1h; ret
__declspec(noinline) u32 Fn00421CF0(u8 *pThis)
{
    return (*(u32 *)(pThis + 0x150) >> 2) & 1;
}

// 0x421d70: mov word ptr [eax],cx; ret
__declspec(noinline) void Fn00421D70(u16 value, u8 *pThis)
{
    *(u16 *)pThis = value;
}

// 0x421d80: mov byte ptr [eax + 3ada6ah],0ffh; ret
__declspec(noinline) void Fn00421D80(u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada6a) = 0xff;
}

// 0x421d90: mov byte ptr [eax + 3ada6eh],0ffh; ret
__declspec(noinline) void Fn00421D90(u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada6e) = 0xff;
}

// 0x421da0: mov byte ptr [eax + 3ada6ch],0ffh; ret
__declspec(noinline) void Fn00421DA0(u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada6c) = 0xff;
}

// 0x421db0: mov byte ptr [eax + 3ada6bh],0ffh; ret
__declspec(noinline) void Fn00421DB0(u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada6b) = 0xff;
}

// 0x421dc0: mov byte ptr [eax + 3ada68h],3h; ret
__declspec(noinline) void Fn00421DC0(u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada68) = 0x3;
}

// 0x421dd0: mov byte ptr [eax + 3ada69h],0ffh; ret
__declspec(noinline) void Fn00421DD0(u8 *pThis)
{
    *(u8 *)(pThis + 0x3ada69) = 0xff;
}

// 0x421de0: mov dword ptr [eax + 3ada64h],0h; ret
__declspec(noinline) void Fn00421DE0(u8 *pThis)
{
    *(u32 *)(pThis + 0x3ada64) = 0x0;
}

// 0x421df0: mov dword ptr [eax + 3ada70h],0h; ret
__declspec(noinline) void Fn00421DF0(u8 *pThis)
{
    *(u32 *)(pThis + 0x3ada70) = 0x0;
}

// 0x421ef0: mov eax,dword ptr [eax + 10h]; ret
__declspec(noinline) u32 Fn00421EF0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x10);
}

// 0x4221e0: xor eax,eax; ret
__declspec(noinline) i32 Fn004221E0(void)
{
    return 0;
}

// 0x4221f0: xor eax,eax; ret
__declspec(noinline) i32 Fn004221F0(void)
{
    return 0;
}

// 0x424590: mov eax,dword ptr [eax + 18h]; ret
__declspec(noinline) u32 Fn00424590(u8 *pThis)
{
    return *(u32 *)(pThis + 0x18);
}

// 0x4245a0: mov dword ptr [eax + 8988h],ecx; ret
__declspec(noinline) void Fn004245A0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x8988) = value;
}

// 0x424600: mov eax,dword ptr [eax + 8998h]; ret
__declspec(noinline) u32 Fn00424600(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8998);
}

// 0x424610: add eax,3cch; ret
__declspec(noinline) u8 * Fn00424610(u8 *pThis)
{
    return pThis + 0x3cc;
}

// 0x424640: mov eax,dword ptr [eax + 4h]; ret
__declspec(noinline) u32 Fn00424640(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x424eb0: xor eax,eax; ret
__declspec(noinline) i32 Fn00424EB0(void)
{
    return 0;
}

// 0x424ec0: xor eax,eax; ret
__declspec(noinline) i32 Fn00424EC0(void)
{
    return 0;
}

// 0x427c40: sub dword ptr [eax + 30h],ecx; ret
__declspec(noinline) void Fn00427C40(u8 *pThis, u32 value)
{
    *(u32 *)(pThis + 0x30) -= value;
}

// 0x427df0: mov eax,dword ptr [eax + 2ch]; ret
__declspec(noinline) u32 Fn00427DF0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x2c);
}

// 0x42a970: mov dword ptr [eax + 2ch],ecx; ret
__declspec(noinline) void Fn0042A970(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x2c) = value;
}

// 0x42a980: mov dword ptr [eax + 28h],ecx; ret
__declspec(noinline) void Fn0042A980(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x28) = value;
}

// 0x42a9c0: mov dword ptr [eax + 4h],0h; ret
__declspec(noinline) void Fn0042A9C0(u8 *pThis)
{
    *(u32 *)(pThis + 0x4) = 0x0;
}

// 0x42a9d0: mov ax,word ptr [eax]; ret
__declspec(noinline) u16 Fn0042A9D0(u8 *pThis)
{
    return *(u16 *)pThis;
}

// 0x434b70: mov dword ptr [eax + 0d4h],0h; ret
__declspec(noinline) void Fn00434B70(u8 *pThis)
{
    *(u32 *)(pThis + 0xd4) = 0x0;
}

// 0x434b90: mov eax,dword ptr [eax]; add eax,ecx; ret
__declspec(noinline) u32 Fn00434B90(u8 *pThis, u32 offset)
{
    return *(u32 *)pThis + offset;
}

// 0x434be0: mov eax,dword ptr [eax + ecx*4+3ad06ch]; ret
__declspec(noinline) u32 Fn00434BE0(i32 index, u8 *pThis)
{
    return ((u32 *)(pThis + 0x3ad06c))[index];
}

// 0x4359a0: mov eax,dword ptr [eax + 8h]; ret
__declspec(noinline) u32 Fn004359A0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8);
}

// 0x4367e0: mov dword ptr [eax + 4h],0h; mov dword ptr [eax + 8h],ecx; ret
__declspec(noinline) void Fn004367E0(u8 *pThis, u32 value)
{
    *(u32 *)(pThis + 0x4) = 0x0;
    *(u32 *)(pThis + 0x8) = value;
}

// 0x4367f0: mov eax,dword ptr [eax]; ret
__declspec(noinline) u32 Fn004367F0(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x436800: mov eax,dword ptr [eax + 120h]; ret
__declspec(noinline) u32 Fn00436800(u8 *pThis)
{
    return *(u32 *)(pThis + 0x120);
}

// 0x436810: mov eax,dword ptr [eax + 100h]; ret
__declspec(noinline) u32 Fn00436810(u8 *pThis)
{
    return *(u32 *)(pThis + 0x100);
}

// 0x436820: mov eax,dword ptr [eax + 114h]; ret
__declspec(noinline) u32 Fn00436820(u8 *pThis)
{
    return *(u32 *)(pThis + 0x114);
}

// 0x436830: mov eax,dword ptr [eax + 110h]; ret
__declspec(noinline) u32 Fn00436830(u8 *pThis)
{
    return *(u32 *)(pThis + 0x110);
}

// 0x436840: mov eax,dword ptr [eax + 104h]; ret
__declspec(noinline) u32 Fn00436840(u8 *pThis)
{
    return *(u32 *)(pThis + 0x104);
}

// 0x436850: mov eax,dword ptr [eax + 108h]; ret
__declspec(noinline) u32 Fn00436850(u8 *pThis)
{
    return *(u32 *)(pThis + 0x108);
}

// 0x4389c0: mov dword ptr [eax + 4h],ecx; ret
__declspec(noinline) void Fn004389C0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x4) = value;
}

// 0x4389d0: mov eax,dword ptr [eax + 4h]; ret
__declspec(noinline) u32 Fn004389D0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x4389e0: add eax,8h; ret
__declspec(noinline) u8 * Fn004389E0(u8 *pThis)
{
    return pThis + 0x8;
}

// 0x43a490: mov dword ptr [eax],0h; mov dword ptr [eax + 4h],0h; ret
__declspec(noinline) void Fn0043A490(u8 *pThis)
{
    *(u32 *)pThis = 0x0;
    *(u32 *)(pThis + 0x4) = 0x0;
}

// 0x43bbf0: mov eax,dword ptr [eax + 0ch]; ret
__declspec(noinline) u32 Fn0043BBF0(u8 *pThis)
{
    return *(u32 *)(pThis + 0xc);
}

// 0x43bc00: mov dword ptr [eax + 0ch],ecx; ret
__declspec(noinline) void Fn0043BC00(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0xc) = value;
}

// 0x43bc10: add eax,0ch; ret
__declspec(noinline) u8 * Fn0043BC10(u8 *pThis)
{
    return pThis + 0xc;
}

// 0x43e520: mov eax,dword ptr [eax + 90h]; ret
__declspec(noinline) u32 Fn0043E520(u8 *pThis)
{
    return *(u32 *)(pThis + 0x90);
}

// 0x43e530: mov eax,dword ptr [eax + 0ch]; ret
__declspec(noinline) u32 Fn0043E530(u8 *pThis)
{
    return *(u32 *)(pThis + 0xc);
}

// 0x441940: mov eax,dword ptr [eax + 44h]; ret
__declspec(noinline) u32 Fn00441940(u8 *pThis)
{
    return *(u32 *)(pThis + 0x44);
}

// 0x441d60: mov eax,dword ptr [eax + 34h]; ret
__declspec(noinline) u32 Fn00441D60(u8 *pThis)
{
    return *(u32 *)(pThis + 0x34);
}

// 0x441fd0: mov dword ptr [eax + 24h],ecx; ret
__declspec(noinline) void Fn00441FD0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x24) = value;
}

// 0x4452e0: mov eax,dword ptr [eax + 35ch]; and eax,1h; ret
__declspec(noinline) u32 Fn004452E0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x35c) & 1;
}

// 0x4458a0: mov eax,dword ptr [eax + 2fch]; ret
__declspec(noinline) u32 Fn004458A0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x2fc);
}

// 0x44c250: or  dword ptr [eax],2h; ret
__declspec(noinline) void Fn0044C250(u8 *pThis)
{
    *(u32 *)pThis |= 0x2u;
}

// 0x44c600: dec dword ptr [eax + 4h]; ret
__declspec(noinline) void Fn0044C600(u8 *pThis)
{
    *(u32 *)(pThis + 0x4) -= 1;
}

// 0x44c760: mov dword ptr [eax],0h; ret
__declspec(noinline) void Fn0044C760(u8 *pThis)
{
    *(u32 *)pThis = 0x0;
}

// 0x44dd30: mov eax,dword ptr [eax + 2ch]; ret
__declspec(noinline) u32 Fn0044DD30(u8 *pThis)
{
    return *(u32 *)(pThis + 0x2c);
}

// 0x44df40: mov eax,dword ptr [eax + 1000h]; ret
__declspec(noinline) u32 Fn0044DF40(u8 *pThis)
{
    return *(u32 *)(pThis + 0x1000);
}

// 0x44df50: mov dword ptr [eax + 1000h],ecx; ret
__declspec(noinline) void Fn0044DF50(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x1000) = value;
}

// 0x44df60: mov eax,dword ptr [eax + 102ch]; ret
__declspec(noinline) u32 Fn0044DF60(u8 *pThis)
{
    return *(u32 *)(pThis + 0x102c);
}

#pragma optimize("", on)

// ---- Additional leaves auto-generated by scripts/generate_portable_leaves.py
// 0x00004014d0: generated
__declspec(noinline) u32 Fn004014D0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x896c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8970) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)((u8*)((uintptr_t)p0+0x8990)) = *(u32*)((u8*)((uintptr_t)p0+0x8990)) + 1;
    return 0x1u;
}








// 0x000040add0: generated
__declspec(noinline) u8 * Fn0040ADD0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x8c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xd4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xd0) = (u32)((0x1u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)((0x3e7u & 0xffffffff));
    return (u8*)(p0);
}








// 0x000040c910: generated
__declspec(noinline) u32 Fn0040C910(u32 p0, u8 *p1)
{
    *(u32*)(u8*)((uintptr_t)p1) = (u32)((p0 & 0xffffffff));
    return p0;
}



// 0x000041f820: generated
__declspec(noinline) u8 * Fn0041F820(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x35c) = (u32)((((*(u32*)((u8*)((uintptr_t)p0+0x35c)) & 0xffffffdfu) | 0x10u) & 0xffffffff));
    return (u8*)(p0);
}




// 0x0000424620: generated
__declspec(noinline) u32 Fn00424620(u32 p0, u8 *p1)
{
    *(u32*)(u8*)((uintptr_t)p1+0x332c) = (u32)((p0 & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p1+0x33c4) = (u32)((p0 & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p1+0x345c) = (u32)((p0 & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p1+0x34f4) = (u32)((p0 & 0xffffffff));
    return p0;
}

















// 0x00004369e0: generated
__declspec(noinline) u8 * Fn004369E0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x100) = (u32)((0xffffffffu & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x104) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x108) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x114) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x11c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x118) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x120) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}






// 0x0000438940: generated
__declspec(noinline) u8 * Fn00438940(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x2000) = (u32)(((u32)(uintptr_t)p0 & 0xffffffff));
    *(u8*)(u8*)((uintptr_t)p0) = (u8)((0x0u & 0xff));
    return (u8*)(p0);
}




// 0x000043cc30: generated
__declspec(noinline) u8 * Fn0043CC30(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x5224) = (u32)((0x2u & 0xffffffff));
    return (u8*)(((u32)(uintptr_t)p0 ^ (u32)(uintptr_t)p0));
}









// 0x000044c640: generated
__declspec(noinline) u8 * Fn0044C640(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x2000) = (u32)(((u32)(uintptr_t)p0 & 0xffffffff));
    *(u8*)(u8*)((uintptr_t)p0) = (u8)((0x0u & 0xff));
    *(u8*)(u8*)((uintptr_t)p0+0x2004) = (u8)((0x0u & 0xff));
    return (u8*)(p0);
}



// 0x000044c6d0: generated
__declspec(noinline) u8 * Fn0044C6D0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)((p1 & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xc) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x10) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}

// 0x000044dbb0: generated
__declspec(noinline) u8 * Fn0044DBB0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x90) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x2c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x7c) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}



// -----------------------------------------------------------------
// Machine-cloned conversions: generated by scripts/leaf_convert.py.
// Every function below has the exact codegen template of a th10::leaf
// function reported at 100.00% by reccmp, and its semantics are proven
// by scripts/verify_leaves.py --from-exe against resources/th10.exe.
// -----------------------------------------------------------------

// 0x401cd0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00401CD0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x394);
}

// 0x401d90: cloned shape of th10::leaf::Fn0040C5D0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401D90(u8 *pThis)
{
    *(u32 *)(pThis + 0x896c) = 0;
    *(u32 *)(pThis + 0x8970) = 0;
}

// 0x401f60: cloned shape of th10::leaf::Fn00404410 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401F60(u8 *pThis)
{
    *(u32 *)(pThis + 0x34) = 0x0;
}

// 0x401f70: cloned shape of th10::leaf::Fn00404410 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401F70(u8 *pThis)
{
    *(u32 *)(pThis + 0x44) = 0x0;
}

// 0x401f80: cloned shape of th10::leaf::Fn00404410 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401F80(u8 *pThis)
{
    *(u32 *)(pThis + 0x24) = 0x0;
}

// 0x409dc0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409DC0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x44);
}

// 0x409dd0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409DD0(u8 *pThis)
{
    return *(u32 *)(pThis + 0xc);
}

// 0x409de0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409DE0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x899c);
}

// 0x409df0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409DF0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8994);
}

// 0x409f30: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409F30(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8);
}

// 0x409f60: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409F60(u8 *pThis)
{
    return *(u32 *)(pThis + 0x60);
}

// 0x409f70: cloned shape of th10::leaf::Fn0040CE40 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409F70(u8 *pThis)
{
    return *(u32 *)(*(u8 **)(pThis + 0x54) + 0x8);
}

// 0x409f80: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00409F80(u8 *pThis)
{
    return *(u32 *)(pThis + 0x54);
}

// 0x40c800: cloned shape of th10::leaf::Fn0040C5D0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0040C800(u8 *pThis)
{
    *(u32 *)(pThis + 0x1008) = 0;
    *(u32 *)(pThis + 0x100c) = 0;
}

// 0x40d4f0: cloned shape of th10::leaf::Fn00408AA0 (verify_leaves --from-exe)
__declspec(noinline) i32 Fn0040D4F0(void)
{
    return 0;
}

// 0x40d500: cloned shape of th10::leaf::Fn00408AA0 (verify_leaves --from-exe)
__declspec(noinline) i32 Fn0040D500(void)
{
    return 0;
}

// 0x4130c0: cloned shape of th10::leaf::Fn004054A0 (verify_leaves --from-exe)
__declspec(noinline) u8 * Fn004130C0(u8 *pThis)
{
    return pThis + 0xc;
}

// 0x4131f0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004131F0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x8);
}

// 0x4136a0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004136A0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x390);
}

// 0x413730: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00413730(u8 *pThis)
{
    return *(u32 *)(pThis + 0x9e80);
}

// 0x4137e0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004137E0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x10);
}

// 0x417590: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00417590(u8 *pThis)
{
    return *(u32 *)(pThis + 0x5c);
}

// 0x4175a0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004175A0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x54);
}

// 0x417610: cloned shape of th10::leaf::Fn0040ACD0 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00417610(u8 *pThis, u32 mask)
{
    return *(u16 *)(pThis + 0x32) & mask;
}

// 0x417620: cloned shape of th10::leaf::Fn0040ACD0 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00417620(u8 *pThis, u32 mask)
{
    return *(u16 *)(pThis + 0x2c) & mask;
}

// 0x417630: cloned shape of th10::leaf::Fn004054A0 (verify_leaves --from-exe)
__declspec(noinline) u8 * Fn00417630(u8 *pThis)
{
    return pThis + 0x2fc;
}

// 0x417640: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00417640(u8 *pThis)
{
    return *(u32 *)(pThis + 0x18);
}

// 0x417650: cloned shape of th10::leaf::Fn004054A0 (verify_leaves --from-exe)
__declspec(noinline) u8 * Fn00417650(u8 *pThis)
{
    return pThis + 0x340;
}

// 0x417680: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00417680(u8 *pThis)
{
    return *(u32 *)(pThis + 0x5c);
}

// 0x4176a0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004176A0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x30);
}

// 0x4176d0: cloned shape of th10::leaf::Fn0040AC00 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004176D0(u8 *pThis)
{
    return *(u32 *)pThis;
}

// 0x4176e0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004176E0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x4);
}

// 0x418ab0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn00418AB0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x40);
}

// 0x418c10: cloned shape of th10::leaf::Fn0040C5D0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418C10(u8 *pThis)
{
    *(u32 *)(pThis + 0x48) = 0;
    *(u32 *)(pThis + 0x4c) = 0;
}

// 0x41ab50: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn0041AB50(u8 *pThis)
{
    return *(u32 *)(pThis + 0x12c);
}

// 0x41ab60: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn0041AB60(u8 *pThis)
{
    return *(u32 *)(pThis + 0x24);
}

// 0x41aca0: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn0041ACA0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x45c);
}

// 0x41bf10: cloned shape of th10::leaf::Fn00408AA0 (verify_leaves --from-exe)
__declspec(noinline) i32 Fn0041BF10(void)
{
    return 0;
}

// 0x41bf20: cloned shape of th10::leaf::Fn00408AA0 (verify_leaves --from-exe)
__declspec(noinline) i32 Fn0041BF20(void)
{
    return 0;
}

// 0x41d870: cloned shape of th10::leaf::Fn00408AA0 (verify_leaves --from-exe)
__declspec(noinline) i32 Fn0041D870(void)
{
    return 0;
}

// 0x41eaf0: cloned shape of th10::leaf::Fn00408AA0 (verify_leaves --from-exe)
__declspec(noinline) i32 Fn0041EAF0(void)
{
    return 0;
}

// 0x42c830: cloned shape of th10::leaf::Fn00404990 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn0042C830(u8 *pThis)
{
    return *(u32 *)(pThis + 0x3ec);
}

// -----------------------------------------------------------------
// Machine-cloned conversions: generated by scripts/leaf_convert.py.
// Every function below has the exact codegen template of a th10::leaf
// function reported at 100.00% by reccmp, and its semantics are proven
// by scripts/verify_leaves.py --from-exe against resources/th10.exe.
// -----------------------------------------------------------------

// 0x401cb0: cloned shape of th10::leaf::Fn004051B0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401CB0(u8 value, u8 *pThis)
{
    *(u8 *)(pThis + 0x2ff) = value;
}

// 0x401cc0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401CC0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x2fc) = value;
}

// 0x401d10: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401D10(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x394) = value;
}

// 0x401fb0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00401FB0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x898c) = value;
}

// 0x412d50: cloned shape of th10::leaf::Fn00405150 (verify_leaves --from-exe)
__declspec(noinline) void Fn00412D50(u8 *pObj, u32 value)
{
    *(u32 *)(pObj + 0x38) = value;
}

// 0x4130a0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn004130A0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x300) = value;
}

// 0x4130d0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn004130D0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x10) = value;
}

// 0x417580: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00417580(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x5c) = value;
}

// 0x4175d0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn004175D0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x54) = value;
}

// 0x4176c0: cloned shape of th10::leaf::Fn0040C460 (verify_leaves --from-exe)
__declspec(noinline) void Fn004176C0(u32 value, u8 *pThis)
{
    *(u32 *)pThis = value;
}

// 0x418b40: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418B40(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x58) = value;
}

// 0x418b60: cloned shape of th10::leaf::Fn004087D0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418B60(u16 value, u8 *pThis)
{
    *(u16 *)(pThis + 0x8) = value;
}

// 0x418b70: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418B70(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x30) = value;
}

// 0x418bd0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418BD0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x4) = value;
}

// 0x418be0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418BE0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x50) = value;
}

// 0x418bf0: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn00418BF0(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x34) = value;
}

// 0x41ac00: cloned shape of th10::leaf::Fn00404C90 (verify_leaves --from-exe)
__declspec(noinline) void Fn0041AC00(u32 value, u8 *pThis)
{
    *(u32 *)(pThis + 0x34) = value;
}

// 0x41ac10: cloned shape of th10::leaf::Fn00405150 (verify_leaves --from-exe)
__declspec(noinline) void Fn0041AC10(u8 *pObj, u32 value)
{
    *(u32 *)(pObj + 0x38) = value;
}

// 0x441fe0: cloned shape of th10::leaf::Fn00405150 (verify_leaves --from-exe)
__declspec(noinline) void Fn00441FE0(u8 *pObj, u32 value)
{
    *(u32 *)(pObj + 0x28) = value;
}

// 0x442170: cloned shape of th10::leaf::Fn00405150 (verify_leaves --from-exe)
__declspec(noinline) void Fn00442170(u8 *pObj, u32 value)
{
    *(u32 *)(pObj + 0x48) = value;
}






// -----------------------------------------------------------------
// Straight-line candidates generated from th10.exe by
// scripts/generate_leaves_exe.py. They are verified by
// scripts/verify_leaves.py --from-exe; any that fail will be removed.
// -----------------------------------------------------------------

// 0x000000401100: target is a one-argument __thiscall identity helper.
// The original owner is not identified; the address-based view keeps the
// member ABI explicit without claiming a semantic class name.
class LeafIdentity00401100View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00401100View::Get()
{
    return (u32)this;
}

// 0x000000401c90: generated
__declspec(noinline) u32 Fn00401C90()
{
    return 0x89acu;
}

// 0x000000401db0: generated
__declspec(noinline) void Fn00401DB0(void)
{

}

// 0x000000401f00: generated
__declspec(noinline) u8 * Fn00401F00(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x38) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x34) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x30) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x2c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x24) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x20) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x1c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x18) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x10) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xc) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x3c) = (u32)((0x3f800000u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x28) = (u32)((0x3f800000u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x14) = (u32)((0x3f800000u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0x3f800000u & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000401f40: generated
__declspec(noinline) u8 * Fn00401F40(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0xfff0bdc1u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000401f90: generated
__declspec(noinline) u8 * Fn00401F90(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((p1 & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000401fa0: target is a one-argument __thiscall identity helper.
// The original owner is not identified; the address-based view keeps the
// member ABI explicit without claiming a semantic class name.
class LeafIdentity00401FA0View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00401FA0View::Get()
{
    return (u32)this;
}

// 0x000000401fc0: generated
__declspec(noinline) void Fn00401FC0(void)
{

}

// 0x0000004020f0: generated
__declspec(noinline) u8 * Fn004020F0(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x40) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x40) & 0xfffffffeu));
    return (u8*)(p0);
}

// 0x000000402100: generated
__declspec(noinline) u8 * Fn00402100(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x30) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x30) & 0xfffffffeu));
    return (u8*)(p0);
}

// 0x000000402110: generated
__declspec(noinline) u8 * Fn00402110(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x40) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x40) & 0xfffffffeu));
    return (u8*)(p0);
}

// 0x000000402120: generated
__declspec(noinline) void Fn00402120(void)
{

}

// 0x000000402130: generated
__declspec(noinline) u8 * Fn00402130(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x20) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x20) & 0xfffffffeu));
    return (u8*)(p0);
}

// 0x000000402140: generated
__declspec(noinline) void Fn00402140(void)
{

}

// 0x000000402150: generated
__declspec(noinline) void Fn00402150(void)
{

}

// 0x000000402220: generated
__declspec(noinline) u8 * Fn00402220(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x80) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x80) & 0xfffffffeu));
    return (u8*)(p0);
}

// 0x000000405400: target is a one-argument __thiscall identity helper.
class LeafIdentity00405400View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00405400View::Get()
{
    return (u32)this;
}


// 0x000000409e40: generated
__declspec(noinline) void Fn00409E40(void)
{

}

// 0x000000409ea0: generated
__declspec(noinline) void Fn00409EA0(void)
{

}

// 0x00000040c710: generated
__declspec(noinline) u8 * Fn0040C710(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0x46d0d8u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x1010) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x1014) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}

// 0x00000040d800: generated
__declspec(noinline) u32 Fn0040D800()
{
    return 0x1u;
}

// 0x00000040d820: generated
__declspec(noinline) u32 Fn0040D820()
{
    return 0x1u;
}


// 0x0000004131b0: generated
__declspec(noinline) void Fn004131B0(void)
{

}

// 0x000000413240: target is a one-argument __thiscall identity helper.
class LeafIdentity00413240View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00413240View::Get()
{
    return (u32)this;
}

// 0x0000004136b0: generated
__declspec(noinline) void Fn004136B0(void)
{

}

// 0x000000413740: generated
__declspec(noinline) u8 * Fn00413740(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x9eb4) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x9eb4) | 0x10u));
    *(u32*)(u8*)((uintptr_t)p0+0x9ecc) = (u32)((0x0u & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000413760: generated
__declspec(noinline) u8 * Fn00413760(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)p0 & 0x1u));
}

// 0x000000413770: generated
__declspec(noinline) u8 * Fn00413770(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x9eb4) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x9eb4) | 0x20u));
    return (u8*)(p0);
}

// 0x000000413780: generated
__declspec(noinline) u8 * Fn00413780(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)p0 & 0x1u));
}


// 0x0000004175c0: generated
__declspec(noinline) u8 * Fn004175C0(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x60) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x60) | 0x4u));
    return (u8*)(p0);
}



// 0x0000004176b0: generated
__declspec(noinline) void Fn004176B0(void)
{

}

// 0x0000004176f0: generated
__declspec(noinline) u8 * Fn004176F0(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x35c) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x35c) & 0xfffffffdu));
    return (u8*)(p0);
}

// 0x000000417700: generated
__declspec(noinline) u8 * Fn00417700(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x35c) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x35c) | 0x2u));
    return (u8*)(p0);
}


// 0x0000004189e0: generated
__declspec(noinline) u8 * Fn004189E0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x50) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x54) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x4c) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x58) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}

// 0x0000004189f0: generated
__declspec(noinline) u8 * Fn004189F0(u8 *p0)
{
    *(u32*)((u8*)((uintptr_t)p0+0x48)) = *(u32*)((u8*)((uintptr_t)p0+0x48)) + 1;
    *(u32*)((u8*)((uintptr_t)p0+0x4c)) = *(u32*)((u8*)((uintptr_t)p0+0x4c)) + 1;
    return (u8*)(p0);
}

// 0x000000418ac0: generated
__declspec(noinline) u8 * Fn00418AC0(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x10) = (u32)((0x0u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xc) = (u32)((0x1u & 0xffffffff));
    return (u8*)(p0);
}


// 0x000000418b50: generated
__declspec(noinline) u8 * Fn00418B50(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0+0x60) = (u32)((*(u32*)(u8*)((uintptr_t)p0+0x60) & 0xfffffffbu));
    return (u8*)(p0);
}

// 0x000000418c20: generated
__declspec(noinline) void Fn00418C20(void)
{

}

// 0x000000419890: generated
__declspec(noinline) u32 Fn00419890()
{
    return 0x1u;
}

// 0x0000004198b0: generated
__declspec(noinline) u32 Fn004198B0()
{
    return 0x1u;
}

// 0x00000041bee0: generated
__declspec(noinline) u8 * Fn0041BEE0(u8 *p0, u8 *p1)
{
    *(u32*)(u8*)((uintptr_t)p1+0x4) = (u32)(((u32)(uintptr_t)p0 & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((u32)(uintptr_t)p1 & 0xffffffff));
    return (u8*)(p0);
}

// 0x00000041bef0: generated
__declspec(noinline) void Fn0041BEF0(void)
{

}

// 0x00000041bf90: generated
__declspec(noinline) void Fn0041BF90(void)
{

}

// 0x00000041bfa0: generated
__declspec(noinline) void Fn0041BFA0(void)
{

}

// 0x00000041bfb0: generated
__declspec(noinline) void Fn0041BFB0(void)
{

}

// 0x00000041bfc0: generated
__declspec(noinline) void Fn0041BFC0(void)
{

}

// 0x00000041bfd0: generated
__declspec(noinline) void Fn0041BFD0(void)
{

}

// 0x00000041bfe0: generated
__declspec(noinline) void Fn0041BFE0(void)
{

}

// 0x00000041bff0: generated
__declspec(noinline) void Fn0041BFF0(void)
{

}

// 0x00000041c000: generated
__declspec(noinline) void Fn0041C000(void)
{

}

// 0x00000041c010: generated
__declspec(noinline) void Fn0041C010(void)
{

}

// 0x00000041c020: generated
__declspec(noinline) void Fn0041C020(void)
{

}

// 0x00000041f840: generated
__declspec(noinline) void Fn0041F840(void)
{

}




// 0x000000427e10: target is a one-argument __thiscall identity helper.
class LeafIdentity00427E10View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00427E10View::Get()
{
    return (u32)this;
}



// 0x00000042a820: generated
__declspec(noinline) u8 * Fn0042A820(u32 p0, u8 *p1)
{
    return (u8*)(((*(u32*)((u8*)((uintptr_t)p1+0x6274)) - (u32)(uintptr_t)p1) - 0x5464u));
}

// 0x00000042b420: generated
__declspec(noinline) void Fn0042B420(void)
{

}


// 0x00000042bab0: generated
__declspec(noinline) void Fn0042BAB0(void)
{

}

// 0x00000042c5a0: generated
__declspec(noinline) void Fn0042C5A0(void)
{

}

// 0x00000042c5b0: generated
__declspec(noinline) u32 Fn0042C5B0()
{
    return 0x5accu;
}




// 0x000000435300: generated
__declspec(noinline) u8 * Fn00435300(u8 *p0)
{
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0x46f230u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)((0xffffffffu & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)((0x0u & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000435690: generated
__declspec(noinline) void Fn00435690(u32 p0, u8 *p1)
{
    *(u32*)(u8*)((uintptr_t)p1) = (u32)((0x46f210u & 0xffffffff));
}

// 0x000000436380: generated
__declspec(noinline) u8 * Fn00436380(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0x46f308u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xc) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000436640: generated
__declspec(noinline) u8 * Fn00436640(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xc) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0x46f328u & 0xffffffff));
    return (u8*)(p0);
}

// 0x000000438400: target is a one-argument __thiscall identity helper.
class LeafIdentity00438400View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00438400View::Get()
{
    return (u32)this;
}

// 0x00000043bf90: target is a one-argument __thiscall identity helper.
class LeafIdentity0043BF90View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity0043BF90View::Get()
{
    return (u32)this;
}

// 0x0000004458d0: target is a one-argument __thiscall identity helper.
class LeafIdentity004458D0View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity004458D0View::Get()
{
    return (u32)this;
}

// 0x0000004458e0: target is a one-argument __thiscall identity helper.
class LeafIdentity004458E0View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity004458E0View::Get()
{
    return (u32)this;
}

// 0x0000004458f0: target is a one-argument __thiscall identity helper.
class LeafIdentity004458F0View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity004458F0View::Get()
{
    return (u32)this;
}

// 0x000000446210: target is a one-argument __thiscall identity helper.
class LeafIdentity00446210View
{
public:
    __declspec(noinline) u32 Get();
};

__declspec(noinline) u32 LeafIdentity00446210View::Get()
{
    return (u32)this;
}

// 0x00000044c0e0: generated
__declspec(noinline) u8 * Fn0044C0E0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0) = (u32)((0x4703e4u & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x4) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x8) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0xc) = (u32)(((p1 ^ p1) & 0xffffffff));
    *(u32*)(u8*)((uintptr_t)p0+0x10) = (u32)(((p1 ^ p1) & 0xffffffff));
    return (u8*)(p0);
}


// 0x0000004501e0: generated
__declspec(noinline) u8 * Fn004501E0(u8 *p0, u32 p1)
{
    *(u32*)(u8*)((uintptr_t)p0+0x102c) = (u32)((p1 & 0xffffffff));
    return (u8*)(p0);
}


// 0x00000045e3a0: generated
__declspec(noinline) u32 Fn0045E3A0(u32 p0)
{
    return (p0 ^ p0);
}



// -----------------------------------------------------------------
// Straight-line candidates (push/pop/test/cmp) generated from exe.
// Verified below; failed ones are removed before commit.
// -----------------------------------------------------------------





















// -----------------------------------------------------------------
// Machine-cloned conversions: generated by scripts/generate_verified_clones.py.
// Every function below has the exact codegen template of a th10::leaf
// function reported at 100.00% by reccmp, and its semantics are proven
// by scripts/verify_leaves.py --from-exe against resources/th10.exe.
// -----------------------------------------------------------------











// 0x4053e0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn004053E0(void)
{

}

// 0x4053f0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn004053F0(void)
{

}


// 0x405bc0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00405BC0(void)
{

}

// 0x408810: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00408810(void)
{

}




// 0x40adc0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0040ADC0(void)
{

}

// 0x40b290: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0040B290(void)
{

}

// 0x40b390: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0040B390(void)
{

}

// 0x412fe0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00412FE0(void)
{

}







// 0x420dc0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00420DC0(void)
{

}

// 0x421d40: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00421D40(void)
{

}

// 0x4245f0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn004245F0(void)
{

}

// 0x427e00: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00427E00(void)
{

}


// 0x42ac10: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0042AC10(void)
{

}






// 0x434c00: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00434C00(void)
{

}

// 0x4352d0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn004352D0(void)
{

}

// 0x4352e0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn004352E0(void)
{

}

// 0x4352f0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn004352F0(void)
{

}


// 0x438250: cloned shape of th10::leaf::Fn004364E0 (verify_leaves --from-exe)
__declspec(noinline) u8 * Fn00438250(u32 p0, u8 *p1)
{
    return (u8*)(*(u32*)((u8*)((uintptr_t)p1+0x14)));
}


// 0x438870: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn00438870(void)
{

}

// 0x43b8c0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0043B8C0(void)
{

}


// 0x43cbe0: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0043CBE0(void)
{

}

// 0x43e570: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0043E570(void)
{

}







// 0x44c610: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0044C610(void)
{

}

// 0x44c660: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0044C660(void)
{

}

// 0x44df10: cloned shape of th10::leaf::Fn004053E0 (verify_leaves --from-exe)
__declspec(noinline) void Fn0044DF10(void)
{

}

// 0x44df30: cloned shape of th10::leaf::Fn0044DF30 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn0044DF30(u32 p0, u32 p1)
{
    return (p0 + p1);
}

// 0x4364e0: cloned shape of th10::leaf::Fn004364E0 (verify_leaves --from-exe)
__declspec(noinline) u32 Fn004364E0(u8 *pThis)
{
    return *(u32 *)(pThis + 4);
}

// -----------------------------------------------------------------
// Machine-cloned conversions: generated by scripts/generate_verified_clones.py.
// Every function below has the exact codegen template of a th10::leaf
// function reported at 100.00% by reccmp, and its semantics are proven
// by scripts/verify_leaves.py --from-exe against resources/th10.exe.
// -----------------------------------------------------------------

// 0x004175b0: generated
__declspec(noinline) u8 * Fn004175B0(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) >> 2)) & 0x1));
}
// 0x00417690: generated
__declspec(noinline) u8 * Fn00417690(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) >> 5)) & 0x1));
}
// 0x00417710: generated
__declspec(noinline) u8 * Fn00417710(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x2a18))) >> 3)) & 0x1));
}
// 0x00418b30: generated
__declspec(noinline) u8 * Fn00418B30(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) >> 1)) & 0x1));
}
// 0x00417600: generated
__declspec(noinline) u8 * Fn00417600(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) >> 4)) & 0x1));
}
// 0x004137f0: generated
__declspec(noinline) u8 * Fn004137F0(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(~(u32)((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x2480))) >> 4)))) & 0x1));
}
// 0x004131a0: generated
__declspec(noinline) u8 * Fn004131A0(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) >> 3)) & 0x1));
}
// 0x00413800: generated
__declspec(noinline) u8 * Fn00413800(u8 *p0)
{
    return (u8*)(((u32)(uintptr_t)(~(u32)((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x2480))))) & 0x1));
}
// 0x00405e10: generated
__declspec(noinline) u8 * Fn00405E10(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x10) = (u32)(*(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x10) & 0xfffffffe);
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x004247c0: generated
__declspec(noinline) u8 * Fn004247C0(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x10) = (u32)(*(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x10) & 0xfffffffe);
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x004247d0: generated
__declspec(noinline) u8 * Fn004247D0(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x80) = (u32)(*(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x80) & 0xfffffffe);
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x004247e0: generated
__declspec(noinline) u8 * Fn004247E0(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x54) = (u32)(*(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x54) & 0xfffffffe);
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x00434c10: generated
__declspec(noinline) u8 * Fn00434C10(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x4) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0xc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x004352c0: generated
__declspec(noinline) u8 * Fn004352C0(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))) = (u32)((0x0 & 0xffffffff));
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x00442f30: generated
__declspec(noinline) u8 * Fn00442F30(u8 *p0, u32 p1)
{
    *(u32*)((uintptr_t)(p0)+0x3adac8) = (u32)((0x0 & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x72dacc) = (u32)(((u32)(uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x3adacc)) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x72dad0) = (u32)(((u32)(uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x3adacc)) & 0xffffffff));
    return (u8*)(p0);
}
// 0x0044c670: generated
__declspec(noinline) u8 * Fn0044C670(u8 *p0, u8 *p1)
{
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x58) = (u16)((0x0 & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x5a) = (u16)((0x1 & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x5c) = (u16)((0x2 & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x5e) = (u16)((0x3 & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x60) = (u16)((((u32)(uintptr_t)(((u32)(uintptr_t)(p1) | 0xffffffff)) & 0xffff) & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x62) = (u16)((((u32)(uintptr_t)(((u32)(uintptr_t)(p1) | 0xffffffff)) & 0xffff) & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x64) = (u16)((((u32)(uintptr_t)(((u32)(uintptr_t)(p1) | 0xffffffff)) & 0xffff) & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x66) = (u16)((((u32)(uintptr_t)(((u32)(uintptr_t)(p1) | 0xffffffff)) & 0xffff) & 0xffff));
    *(u16*)((uintptr_t)((u32)(uintptr_t)(p1))+0x68) = (u16)((0x4 & 0xffff));
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x0042b4c0: generated
__declspec(noinline) u8 * Fn0042B4C0(u8 *p0, u8 *p1)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x2c) = (u32)(*(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x2c) & 0xfffffffe);
    return (u8*)((u32)(uintptr_t)(p1));
}

// 0x00427cb0: generated
__declspec(noinline) u8 * Fn00427CB0(u8 *p0)
{
    *(u16*)((uintptr_t)(p0)+0x8) = (u16)(*(u16*)((uintptr_t)(p0)+0x8) + 0xffffffec);
    return (u8*)(p0);
}
// 0x00427e40: generated
__declspec(noinline) u8 * Fn00427E40(u8 *p0, u8 *p1)
{
    return (u8*)((u32)(uintptr_t)((uintptr_t)((u32)((u32)(uintptr_t)(p0) * 0x98))+((uintptr_t)(p1)*1)+0x32dc));
}
// 0x00428e70: generated
__declspec(noinline) u8 * Fn00428E70(u8 *p0, u8 *p1)
{
    return (u8*)((u32)(uintptr_t)((uintptr_t)((u32)((u32)(uintptr_t)(p0) * 0x98))+((uintptr_t)(p1)*1)+0x32d4));
}
// 0x004364d0: generated
__declspec(noinline) u8 * Fn004364D0(u32 p0, u8 *p1)
{
    return (u8*)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p1)+0x8))) - *(u32*)(((uintptr_t)(p1)+0xc))));
}

// 0x0042c840: generated
__declspec(noinline) u8 * Fn0042C840(u32 p0, u8 *p1)
{
    return (u8*)((u32)(i32)*(u8*)(((uintptr_t)(p1)+((uintptr_t)(p0)*1)+0x1d888)));
}

// 0x00408860: generated
__declspec(noinline) void Fn00408860(u8 *p0)
{
    u32 v = *(u32 *)(p0 + 0x80);
    if ((i32)v < 0x1869f)
        *(u32 *)(p0 + 0x80) = v + 1;
}
// 0x00408880: generated
__declspec(noinline) void Fn00408880(u8 *p0)
{
    u32 v = *(u32 *)(p0 + 0x84);
    if ((i32)v < 0x1869f)
        *(u32 *)(p0 + 0x84) = v + 1;
}
// 0x0040ce50: generated
__declspec(noinline) void Fn0040CE50(u8 *p0, u32 p1)
{
    u32 v = p1;
    if (v > 0x63)
        v = 0x63;
    *(u32 *)(p0 + 0x9ec0) = v;
}
// 0x00412ed0: generated
__declspec(noinline) void Fn00412ED0(u8 *p0, u32 p1)
{
    u32 old = *(u32 *)(p0 + 0x44);
    *(u32 *)(p0 + 0x44) = p1;
    if (old != p1)
        *(u32 *)(p0 + 0x4c) = 0;
}
// 0x00436560: generated
__declspec(noinline) u32 Fn00436560(u8 *p0)
{
    u32 x = *(u32 *)p0;
    if ((i32)x < 0)
        x = (u32)(-(i32)x);
    return x;
}

// 0x0040cf90: generated
__declspec(noinline) u8 * Fn0040CF90(u8 *p0, u32 p1)
{
    u8 *cur = *(u8 **)(p0 + 0x18);
    while (cur) {
        if (*(u32 *)(cur + 0x54) == p1)
            return cur;
        cur = *(u8 **)(cur + 8);
    }
    return 0;
}
// 0x00418a90: generated
__declspec(noinline) u8 * Fn00418A90(u8 *p0)
{
    u32 v = *(u32 *)(p0 + 0x50) + 1;
    *(u32 *)(p0 + 0x50) = v;
    if ((i32)v >= 0xa)
        *(u32 *)(p0 + 0x50) = 9;
    return p0;
}
// 0x004423c0: generated
__declspec(noinline) u32 Fn004423C0(u32 p0, u32 p1)
{
    u32 e = ((p0 & 0xff) * (p1 & 0xff)) >> 7;
    return e < 0x100 ? e : 0xff;
}

// 0x004020b0: generated
__declspec(noinline) u8 * Fn004020B0(u8 *p0, u32 p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x6c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x6c))) & (u32)(uintptr_t)(0xfffffffe))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0xb0) = (u32)(*(u32*)((uintptr_t)(p0)+0xb0) & (u32)(uintptr_t)(0xfffffffe));
    *(u32*)((uintptr_t)(p0)+0xfc) = (u32)(*(u32*)((uintptr_t)(p0)+0xfc) & (u32)(uintptr_t)(0xfffffffe));
    *(u32*)((uintptr_t)(p0)+0x128) = (u32)(*(u32*)((uintptr_t)(p0)+0x128) & (u32)(uintptr_t)(0xfffffffe));
    *(u32*)((uintptr_t)(p0)+0x174) = (u32)(*(u32*)((uintptr_t)(p0)+0x174) & (u32)(uintptr_t)(0xfffffffe));
    *(u32*)((uintptr_t)(p0)+0x1b0) = (u32)(*(u32*)((uintptr_t)(p0)+0x1b0) & (u32)(uintptr_t)(0xfffffffe));
    *(u32*)((uintptr_t)(p0)+0x1fc) = (u32)(*(u32*)((uintptr_t)(p0)+0x1fc) & (u32)(uintptr_t)(0xfffffffe));
    *(u32*)((uintptr_t)(p0)+0x228) = (u32)(*(u32*)((uintptr_t)(p0)+0x228) & (u32)(uintptr_t)(0xfffffffe));
    return (u8*)(p0);
}
// 0x00405000: generated
__declspec(noinline) u8 * Fn00405000(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00405020: generated
__declspec(noinline) u8 * Fn00405020(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x18))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x18))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x18))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00405110: generated
// 0x00405130: generated
__declspec(noinline) u8 * Fn00405130(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00409e10: generated
__declspec(noinline) u8 * Fn00409E10(u8 *p0, u8 *p1, u32 p2)
{
    return (u8*)(((u32)(uintptr_t)((u32)(uintptr_t)((uintptr_t)(*(u32*)(((uintptr_t)(p1)+0x28)))+((uintptr_t)(*(u32*)(((uintptr_t)(p1)+0x28)))*2))) + (u32)(uintptr_t)(*(u32*)(((uintptr_t)(p1)+0x2c)))));
}
// 0x0040b370: generated
__declspec(noinline) u8 * Fn0040B370(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x340))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x340))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x340))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x0040c950: generated
__declspec(noinline) u8 * Fn0040C950(u8 *p0, u32 index, u32 value)
{
    *(u32*)((uintptr_t)(p0)+((uintptr_t)(index)*4)) = (u32)(((u32)(uintptr_t)(value) & 0xffffffff));
    return (u8*)(p0);
}
// 0x0040cd90: generated
__declspec(noinline) u8 * Fn0040CD90(u8 *p0, u32 index, u32 value)
{
    *(u32*)((uintptr_t)(p0)+((uintptr_t)(index)*4)+0x10) = (u32)(((u32)(uintptr_t)(value) & 0xffffffff));
    return (u8*)(p0);
}
// 0x0040cee0: generated
__declspec(noinline) u8 * Fn0040CEE0(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x0040cf60: generated
__declspec(noinline) u8 * Fn0040CF60(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x430))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x430))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x430))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00412d60: generated
__declspec(noinline) u8 * Fn00412D60(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x4)));
}
// 0x00412d70: generated
__declspec(noinline) u8 * Fn00412D70(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0xc) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x4)));
}
// 0x00412d80: generated
__declspec(noinline) u8 * Fn00412D80(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)+0x10) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x14) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x4)));
}
// 0x00412d90: generated
__declspec(noinline) u8 * Fn00412D90(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)+0x18) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x1c) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x4)));
}
// 0x00413120: generated
// 0x00413220: generated
__declspec(noinline) u8 * Fn00413220(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00418af0: generated
__declspec(noinline) u8 * Fn00418AF0(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x60) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) ^ (u32)(uintptr_t)(((u32)(uintptr_t)(((u32)(uintptr_t)((u32)(uintptr_t)((uintptr_t)(p1)+((uintptr_t)(p1)*1))) ^ (u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))))) & 0x2)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x00418b10: generated
__declspec(noinline) u8 * Fn00418B10(u8 *p0, u32 p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x60) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) ^ (u32)(uintptr_t)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x60))) ^ (u32)(uintptr_t)(p2))) & 0x1)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x0041ac20: generated
__declspec(noinline) u8 * Fn0041AC20(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x4)));
}
// 0x0041ac30: generated
__declspec(noinline) u8 * Fn0041AC30(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0xc) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x4)));
}
// 0x00427d70: generated
__declspec(noinline) u8 * Fn00427D70(u8 *p0, u32 p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)) = (u32)(((u32)(uintptr_t)(p1) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x4) = (u32)(((u32)(uintptr_t)(p2) & 0xffffffff));
    return (u8*)(p0);
}
// 0x0042a990: generated
__declspec(noinline) u8 * Fn0042A990(u8 *p0, u32 p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0xc) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x14) = (u32)((0x0 & 0xffffffff));
    return (u8*)(p0);
}
// 0x0042aa10: generated
__declspec(noinline) u8 * Fn0042AA10(u8 *p0, u32 p1, u8 *p2)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x4) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0xc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x10) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x14) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x18) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x1c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p0))+0x20) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)) = (u32)((0x72303174 & 0xffffffff));
    *(u16*)((uintptr_t)(p0)+0x4) = (u16)((0x5 & 0xffff));
    *(u32*)((uintptr_t)(p0)+0x10) = (u32)((0x100 & 0xffffffff));
    return (u8*)(p0);
}
// 0x0042ac20: generated
__declspec(noinline) u8 * Fn0042AC20(u8 *p0, u8 *p1, u8 *p2)
{
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x4) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0xc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x10) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x14) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x18) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x1c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((u32)(uintptr_t)(p1)))+0x20) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)((u32)(uintptr_t)(p1))))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0xc) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)((u32)(uintptr_t)(p1))+0x8))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x18) = (u32)(((u32)(uintptr_t)((u32)(uintptr_t)(p1)) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x1c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)((u32)(uintptr_t)(p1))))) ^ (u32)(uintptr_t)(*(u32*)(((uintptr_t)((u32)(uintptr_t)(p1))))))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)(p1))+0x20) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)((u32)(uintptr_t)(p1))))) ^ (u32)(uintptr_t)(*(u32*)(((uintptr_t)((u32)(uintptr_t)(p1))))))) & 0xffffffff));
    return (u8*)((u32)(uintptr_t)(p1));
}
// 0x0043af00: generated
__declspec(noinline) u8 * Fn0043AF00(u8 *p0, u32 p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x2c8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x2e4) = (u32)(((u32)(uintptr_t)(p2) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x2e8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x2dc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x2e0) = (u32)((0x1 & 0xffffffff));
    return (u8*)(((u32)(uintptr_t)(p0) ^ (u32)(uintptr_t)(p0)));
}
// 0x00441ff0: generated
__declspec(noinline) u8 * Fn00441FF0(u8 *p0, u32 p1, u8 *p2)
{
    *(u32*)((uintptr_t)(p2)) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x00442000: generated
__declspec(noinline) u8 * Fn00442000(u8 *p0, u32 p1, u8 *p2)
{
    *(u32*)((uintptr_t)(p2)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x00442130: generated
__declspec(noinline) u8 * Fn00442130(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x18))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x18))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x18))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00442150: generated
__declspec(noinline) u8 * Fn00442150(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(((u32)(uintptr_t)(p1) + 0x24))+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x00442180: generated
__declspec(noinline) u8 * Fn00442180(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x8) = (u32)(((u32)(uintptr_t)(*(u8*)(((uintptr_t)(p1)+0x2))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x4) = (u32)(((u32)(uintptr_t)(*(u8*)(((uintptr_t)(p1)+0x1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)) = (u32)(((u32)(uintptr_t)(*(u8*)(((uintptr_t)(p1)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x004421a0: generated
__declspec(noinline) u8 * Fn004421A0(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p1)) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x4) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xffffffff));
    *(u32*)((uintptr_t)(p1)+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x8))) & 0xffffffff));
    return (u8*)(*(u32*)(((uintptr_t)(p0)+0x8)));
}
// 0x004421c0: generated
// 0x00442390: generated
__declspec(noinline) u8 * Fn00442390(u8 *p0, u32 p1, u8 *p2)
{
    *(u32*)((uintptr_t)(p2)+0x8) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x004423a0: generated
__declspec(noinline) u8 * Fn004423A0(u8 *p0, u32 p1, u8 *p2)
{
    *(u32*)((uintptr_t)(p2)+0xc) = (u32)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x00449a50: generated
__declspec(noinline) u8 * Fn00449A50(u8 *p0, u32 p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0xc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x10) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x4) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xfffffffe)) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x14) = (u32)(((u32)(uintptr_t)(p0) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x18) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x1c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p1) ^ (u32)(uintptr_t)(p1))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x00449aa0: generated
__declspec(noinline) u8 * Fn00449AA0(u8 *p0, u8 *p1, u32 p2)
{
    *(u32*)((uintptr_t)(p0)+0x8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0xc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x10) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x4) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p0)+0x4))) & 0xfffffffe)) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x14) = (u32)(((u32)(uintptr_t)(p0) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x18) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)(p0)+0x1c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x4) = (u32)(*(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x4) & 0xfffffffe);
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x8) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0xc) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x10) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x14) = (u32)(((u32)(uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24)) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x18) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    *(u32*)((uintptr_t)((u32)(uintptr_t)((uintptr_t)(p0)+0x24))+0x1c) = (u32)(((u32)(uintptr_t)(((u32)(uintptr_t)(p2) ^ (u32)(uintptr_t)(p2))) & 0xffffffff));
    return (u8*)(p0);
}
// 0x0044df20: generated
__declspec(noinline) u8 * Fn0044DF20(u32 p0, u8 *p1, u32 p2)
{
    return (u8*)(((u32)(uintptr_t)(((u32)(uintptr_t)(*(u32*)(((uintptr_t)(p1)+0x1004))) + (u32)(uintptr_t)(p2))) + (u32)(uintptr_t)(p1)));
}

// -----------------------------------------------------------------
// Night batch: manual transcriptions from resources/th10.exe; semantics
// proven by scripts/verify_leaves.py --from-exe, bytes judged by CI reccmp.
// -----------------------------------------------------------------

// 0x4051c0: this->field_390 != 0
__declspec(noinline) u32 Fn004051C0(u8 *pThis)
{
    return *(u32 *)(pThis + 0x390) != 0;
}

// 0x405570: signed field_3738 >= 0x3c
__declspec(noinline) u32 Fn00405570(u8 *pThis)
{
    return (i32)*(u32 *)(pThis + 0x3738) >= 0x3c;
}

// 0x40c920: *p - value, store and return
__declspec(noinline) u32 Fn0040C920(u8 *pThis, u32 value)
{
    u32 t = *(u32 *)pThis - value;
    *(u32 *)pThis = t;
    return t;
}

// 0x40c930: signed *p > 0
__declspec(noinline) u32 Fn0040C930(u8 *pThis)
{
    return (i32)*(u32 *)pThis > 0;
}

// 0x40ce30: field_9eb8 != 0
__declspec(noinline) u32 Fn0040CE30(u8 *pThis)
{
    return *(u32 *)(pThis + 0x9eb8) != 0;
}

// 0x40ce80: field_28 != 0
__declspec(noinline) u32 Fn0040CE80(u8 *pThis)
{
    return *(u32 *)(pThis + 0x28) != 0;
}

// 0x412790: arr[(idx+0x24a)<<4] = value
__declspec(noinline) void Fn00412790(u32 index, u32 value, u8 *pThis)
{
    *(u32 *)(pThis + ((index + 0x24a) << 4)) = value;
}

// 0x428ec0: arr[idx*0x98 + 0x32d4] = *src; [+4] = src[4]
__declspec(noinline) void Fn00428EC0(u32 index, u8 *src, u8 *arr)
{
    u8 *dst = arr + index * 0x98 + 0x32d4;
    *(u32 *)dst = *(u32 *)src;
    *(u32 *)(dst + 4) = *(u32 *)(src + 4);
}

// 0x428ee0: same family, offset 0x32e4
__declspec(noinline) void Fn00428EE0(u32 index, u8 *src, u8 *arr)
{
    u8 *dst = arr + index * 0x98 + 0x32e4;
    *(u32 *)dst = *(u32 *)src;
    *(u32 *)(dst + 4) = *(u32 *)(src + 4);
}

// 0x428f00: same family, offset 0x32ec
__declspec(noinline) void Fn00428F00(u32 index, u8 *src, u8 *arr)
{
    u8 *dst = arr + index * 0x98 + 0x32ec;
    *(u32 *)dst = *(u32 *)src;
    *(u32 *)(dst + 4) = *(u32 *)(src + 4);
}


// 0x434ba0: toggle bit 4 in field_60, return the mask
__declspec(noinline) u32 Fn00434BA0(u32 value, u8 *pThis)
{
    u32 f = *(u32 *)(pThis + 0x60);
    u32 t = ((value << 4) ^ f) & 0x10;
    *(u32 *)(pThis + 0x60) = f ^ t;
    return t;
}


// 0x435270: pop-like: advance pointer at *p, return *new
__declspec(noinline) u32 Fn00435270(u8 *pThis)
{
    u32 *q = *(u32 **)pThis;
    q++;
    *(u32 **)pThis = q;
    return *q;
}

// 0x436860: field_11c != 0
__declspec(noinline) u32 Fn00436860(u8 *pThis)
{
    return *(u32 *)(pThis + 0x11c) != 0;
}

// 0x449a20: *p = *p + 1, return old value
__declspec(noinline) u32 Fn00449A20(u8 *pThis)
{
    u32 old = *(u32 *)pThis;
    *(u32 *)pThis = old + 1;
    return old;
}

// 0x44b9b0: mixed 16/32 ladder
__declspec(noinline) u32 Fn0044B9B0(u8 *pThis)
{
    u32 w = *(u16 *)pThis;
    u32 v = (w ^ 0x9630) & 0xffffffff;
    u32 e = (v - 0x6553) & 0xffffffff;
    u32 dx = (e & 0xffff) >> 0xe;
    u32 out = ((e << 2) + dx) & 0xffffffff;
    u32 d4 = *(u32 *)(pThis + 4) + 1;
    *(u16 *)pThis = (u16)out;
    *(u32 *)(pThis + 4) = d4;
    return out;
}


// 0x004043d0: generated2
__declspec(noinline) u32 Fn004043D0(u8 * p0, u8 * p1, u8 * p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 t6;
    u32 t7;
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x1eec) = (u32)(0x1u);
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x1ef0)*1) = (u32)(t1);
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x1ef0)*1+0x4) = (u32)(t2);
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x8));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x1ef0)*1+0x8) = (u32)(t3);
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t5 = ((u32)(uintptr_t)p2 + 0x1efcu);
    *(u32*)(u8*)((uintptr_t)t5*1) = (u32)(t4);
    t6 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    *(u32*)(u8*)((uintptr_t)t5*1+0x4) = (u32)(t6);
    t7 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    *(u32*)(u8*)((uintptr_t)t5*1+0x8) = (u32)(t7);
    return t7;
}

// 0x00405cb0: generated2
__declspec(noinline) u32 Fn00405CB0(u32 p0)
{
    u32 t1;
    t1 = *(u32*)((u8*)((LEAF_GLOB(0x474170)) + ((uintptr_t)p0*4)));
    return t1;
}

// 0x00409e00: generated2
__declspec(noinline) u32 Fn00409E00(u8 * p0)
{
    u32 t1;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x10));
    return (((0 ^ 0) & ~0xff) | ((((t1 == 0x1u) ? 1 : 0)) & 0xff));
}

// 0x00409f40: generated2
__declspec(noinline) u32 Fn00409F40(u8 * p0, u32 p1)
{
    u32 t1;
    u32 t2;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8c));
    t2 = *(u32*)((u8*)((uintptr_t)t1*1+(uintptr_t)p1*8));
    return t2;
}

// 0x0040ada0: generated2
__declspec(noinline) u8 * Fn0040ADA0(u8 * p0, u8 * p1)
{
    *(u32*)LEAF_GLOB(0x477848) = (u32)(((((u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+(uintptr_t)(u32)(uintptr_t)p0*2) << 4) & 0xffffffff) + 0x474788u));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x3c) = (u32)((u32)(uintptr_t)p0);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x40) = (u32)((u32)(uintptr_t)p0);
    return (u8*)(p0);
}

// 0x0040ae50: generated2
__declspec(noinline) u8 * Fn0040AE50(u8 * p0)
{
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x10) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x14) = (u32)((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    *(u32*)LEAF_GLOB(0x4776fc) = (u32)((u32)(uintptr_t)p0);
    return (u8*)(p0);
}

// 0x0040b530: generated2
__declspec(noinline) u8 * Fn0040B530(u8 * p0)
{
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x10) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x14) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x18) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1c) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x20) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x24) = (u32)((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    *(u32*)LEAF_GLOB(0x477700) = (u32)((u32)(uintptr_t)p0);
    return (u8*)(p0);
}

// 0x0040c730: generated2
__declspec(noinline) u8 * Fn0040C730(u8 * p0)
{
    u32 t1;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1028));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1028) = (u32)((t1 & 0xfffffffeu));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8)*1) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x101c) = (u32)((u32)(uintptr_t)p0);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1018) = (u32)(0xffffffffu);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1020) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)((u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1030) = (u32)((u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1034) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1038) = (u32)((0 ^ 0));
    return (u8*)(p0);
}

// 0x004177c0: generated2
__declspec(noinline) u32 Fn004177C0(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    t2 = *(u32*)((u8*)((uintptr_t)t1*1+0x4));
    *(u32*)(u8*)((uintptr_t)t1*1+0x4) = (u32)((t2 | 0x2u));
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc));
    *(u32*)((u8*)((uintptr_t)t3*1+0x4)) = (*(u32*)((u8*)((uintptr_t)t3*1+0x4)) | 0x2u);
    return t3;
}

// 0x004177e0: generated2
__declspec(noinline) u32 Fn004177E0(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    t2 = *(u32*)((u8*)((uintptr_t)t1*1+0x4));
    *(u32*)(u8*)((uintptr_t)t1*1+0x4) = (u32)((t2 | 0x2u));
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc));
    *(u32*)((u8*)((uintptr_t)t3*1+0x4)) = (*(u32*)((u8*)((uintptr_t)t3*1+0x4)) | 0x2u);
    return t3;
}

// 0x00418c00: generated2
__declspec(noinline) u32 Fn00418C00()
{
    u32 t1;
    t1 = *(u32*)(LEAF_GLOB(0x491fc4));
    return t1;
}

// 0x0041ff00: generated2
__declspec(noinline) u32 Fn0041FF00(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc));
    t2 = *(u32*)((u8*)((uintptr_t)t1*1+0x4));
    *(u32*)(u8*)((uintptr_t)t1*1+0x4) = (u32)((t2 | 0x2u));
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x10));
    *(u32*)((u8*)((uintptr_t)t3*1+0x4)) = (*(u32*)((u8*)((uintptr_t)t3*1+0x4)) | 0x2u);
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x89a8));
    *(u32*)((u8*)((uintptr_t)t4*1+0x4)) = (*(u32*)((u8*)((uintptr_t)t4*1+0x4)) | 0x2u);
    return t4;
}

// 0x00421b70: generated2
__declspec(noinline) u32 Fn00421B70(u8 * p0)
{
    u32 t1;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x40));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x3c) = (u32)(t1);
    *(u32*)LEAF_GLOB(0x477848) = (u32)(((((u32)(uintptr_t)(u8*)((uintptr_t)t1*1+(uintptr_t)t1*2) << 4) & 0xffffffff) + 0x474788u));
    return ((((u32)(uintptr_t)(u8*)((uintptr_t)t1*1+(uintptr_t)t1*2) << 4) & 0xffffffff) + 0x474788u);
}

// 0x00421f50: generated2
__declspec(noinline) u32 Fn00421F50()
{
    u32 t1;
    u32 t2;
    t1 = *(u32*)(LEAF_GLOB(0x4776e0));
    t2 = *(u32*)((u8*)((uintptr_t)t1*1+0x8998));
    return t2;
}

// 0x00424440: generated2
__declspec(noinline) u32 Fn00424440(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t3 = (((0 ^ 0) & ~0xff) | ((((t1 != t2) ? 1 : 0)) & 0xff));
    return t3;
}

// 0x00427d00: generated2
__declspec(noinline) u8 * Fn00427D00(u8 * p0, u8 * p1, u8 * p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 t6;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p1*1) = (u32)((t2 + t1));
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4) = (u32)((t3 + t4));
    t5 = (t2 + t1);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(t5);
    t6 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)(t6);
    return (u8*)(p0);
}

// 0x00427d50: generated2
__declspec(noinline) u8 * Fn00427D50(u8 * p0, u8 * p1, u8 * p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t4 = (t2 - t1);
    t5 = (t3 - *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1)));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(t5);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)(t4);
    return (u8*)(p0);
}

// 0x00427d80: generated2
__declspec(noinline) u8 * Fn00427D80(u8 * p0, u8 * p1, u32 p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t3 = (u32)(t1 * p2);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)((u32)(t2 * p2));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)(t3);
    return (u8*)(p0);
}

// 0x00427dc0: generated2
__declspec(noinline) u8 * Fn00427DC0(u8 * p0, u8 * p1, u8 * p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t3 = (t1 + *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4)));
    t4 = (t2 + *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1)));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(t4);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)(t3);
    return (u8*)(p0);
}

// 0x00427e20: generated2
__declspec(noinline) u8 * Fn00427E20(u8 * p0, u8 * p1, u8 * p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p1*1) = (u32)(t1);
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4) = (u32)(t2);
    t3 = t1;
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(t3);
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)(t4);
    return (u8*)(p0);
}

// 0x00428df0: generated2
__declspec(noinline) u32 Fn00428DF0(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t3 = (((0 ^ 0) & ~0xff) | ((((t1 != t2) ? 1 : 0)) & 0xff));
    return t3;
}

// 0x0042a830: generated2
__declspec(noinline) u32 Fn0042A830(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 t6;
    u32 t7;
    u32 t8;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5460));
    t2 = 0x2aaaaaabu;
    t3 = (u32)(((uint64_t)(t2) * (uint64_t)((t1 - (u32)(uintptr_t)p0))));
    t4 = (u32)(((uint64_t)(t2) * (uint64_t)((t1 - (u32)(uintptr_t)p0))) >> 32);
    t5 = (t4 >> 31);
    t6 = (t5 + (u32)(((uint64_t)(t2) * (uint64_t)((t1 - (u32)(uintptr_t)p0))) >> 32));
    t7 = (u32)(uintptr_t)(u8*)((uintptr_t)t6*1+(uintptr_t)t6*2);
    t8 = ((t7 << 1) & 0xffffffff);
    return t8;
}

// 0x0042a850: generated2
__declspec(noinline) u32 Fn0042A850(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 t6;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5460));
    t2 = 0x2aaaaaabu;
    // 1-operand imul is a SIGNED multiply (edx:eax = (i32)eax*(i32)ecx):
    // the ecx factor must be sign-extended, not zero-extended. t2 itself
    // is positive, so only the (t1 - p0) side changes. The old unsigned
    // form matched the verifier's own unsigned imul emulation and passed
    // by luck while (t1 - p0) stayed positive; hash-valued plants drew a
    // negative difference and exposed both.
    t3 = (u32)(((uint64_t)(t2) * (uint64_t)(int64_t)((i32)(t1 - (u32)(uintptr_t)p0))));
    t4 = (u32)(((uint64_t)(t2) * (uint64_t)(int64_t)((i32)(t1 - (u32)(uintptr_t)p0))) >> 32);
    t5 = (t4 >> 31);
    t6 = (t5 + (u32)(((uint64_t)(t2) * (uint64_t)(int64_t)((i32)(t1 - (u32)(uintptr_t)p0))) >> 32));
    return t6;
}

// 0x0042a870: generated2
__declspec(noinline) u8 * Fn0042A870(u8 * p0)
{
    u32 t1;
    u32 t2;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5460));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6274));
    return (u8*)(((u32)(uintptr_t)(u8*)((uintptr_t)t2*1+(uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(((uint64_t)(0x2aaaaaabu) * (uint64_t)((t1 - (u32)(uintptr_t)p0))) >> 32)*1+(uintptr_t)((u32)(((uint64_t)(0x2aaaaaabu) * (uint64_t)((t1 - (u32)(uintptr_t)p0))) >> 32) >> 31)*1+0x2aaa9c9a)*1+(uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(((uint64_t)(0x2aaaaaabu) * (uint64_t)((t1 - (u32)(uintptr_t)p0))) >> 32)*1+(uintptr_t)((u32)(((uint64_t)(0x2aaaaaabu) * (uint64_t)((t1 - (u32)(uintptr_t)p0))) >> 32) >> 31)*1+0x2aaa9c9a)*2)*2) - (u32)(uintptr_t)p0));
}

// 0x0042a900: generated2
__declspec(noinline) u32 Fn0042A900(u8 * p0, u32 p1)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6274));
    *(u8*)(u8*)((uintptr_t)t1*1) = (u8)((p1 & 0xff));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6274));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6274) = (u32)((t2 + 1));
    t3 = ((u32)(uintptr_t)p0 ^ (u32)(uintptr_t)p0);
    t4 = (t3 & ~0xff) | ((((i32)((((t2 + 1) - (u32)(uintptr_t)p0) - 0x5464u)) >= (i32)(0x78u) ? 1 : 0)) & 0xff);
    return t4;
}

// 0x0042a9b0: generated2
__declspec(noinline) u32 Fn0042A9B0()
{
    u32 t1;
    t1 = *(u32*)(LEAF_GLOB(0x491fc4));
    return t1;
}

// 0x0042bac0: generated2
__declspec(noinline) u32 Fn0042BAC0(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)t1*1+0x4));
    t3 = ((u32)(uintptr_t)p0 ^ (u32)(uintptr_t)p0);
    t4 = (t3 & ~0xff) | ((((0 != ((t2) & (t2))) ? 1 : 0)) & 0xff);
    return t4;
}

// 0x00434bc0: generated2
__declspec(noinline) u8 * Fn00434BC0(u8 * p0, u32 p1)
{
    u32 t1;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xd4));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+(uintptr_t)t1*4+0x90) = (u32)(p1);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xd4)) = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xd4)) + 1;
    return (u8*)(p0);
}

// 0x00435fa0: generated2
__declspec(noinline) u32 Fn00435FA0(u32 p0)
{
    u32 t1;
    u32 t2;
    *(u32*)LEAF_GLOB(0x48f860) = (u32)(p0);
    t1 = (u32)(uintptr_t)(u8*)((uintptr_t)p0*1+(uintptr_t)p0*2);
    t2 = ((t1 << 2) & 0xffffffff);
    *(u32*)(u8*)((LEAF_GLOB(0x477858)) + ((uintptr_t)t2*1)) = (u32)(0x2000u);
    *(u32*)(u8*)((LEAF_GLOB(0x477860)) + ((uintptr_t)t2*1)) = (u32)((0 ^ 0));
    *(u32*)(u8*)((LEAF_GLOB(0x47785c)) + ((uintptr_t)t2*1)) = (u32)((0 ^ 0));
    return t2;
}

// 0x00441e90: generated2
__declspec(noinline) u8 * Fn00441E90(u8 * p0, u8 * p1, u8 * p2)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    u32 t5;
    u32 t6;
    u32 t7;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4));
    t2 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x8));
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t4 = (t3 + t1);
    t5 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x8));
    t6 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t7 = (t6 + *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1)));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(t7);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4) = (u32)(t4);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8) = (u32)((t5 + t2));
    return (u8*)(p0);
}

// 0x0044b9e0: generated2
__declspec(noinline) u32 Fn0044B9E0(u8 * p0)
{
    u32 t1;
    u32 t2;
    t1 = ((((((0 ^ 0) & ~0xffff) | ((*(u16*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | ((*(u16*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))) & 0xffff)) & 0xffff) ^ 0x9630u)) & 0xffff)) - 0x6553u);
    *(u16*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u16)((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) & 0xffff));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) + 0x2u);
    *(u16*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u16)((((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) << 2) & 0xffffffff)) & 0xffff));
    t2 = (((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) << 2) & 0xffffffff)) & 0xffff);
    return ((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) & 0xffff) << 16) & 0xffffffff) | t2);
}

// 0x0044ba30: generated2
__declspec(noinline) u32 Fn0044BA30(u8 * p0)
{
    u32 t1;
    u32 t2;
    t1 = ((((((0 ^ 0) & ~0xffff) | ((*(u16*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | ((*(u16*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))) & 0xffff)) & 0xffff) ^ 0x9630u)) & 0xffff)) - 0x6553u);
    *(u16*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u16)((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) & 0xffff));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) + 0x2u);
    *(u16*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u16)((((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) << 2) & 0xffffffff)) & 0xffff));
    t2 = (((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) ^ 0x9630u) - 0x6553u) << 2) & 0xffffffff)) & 0xffff);
    return ((((((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & ~0xffff) | (((((((0 ^ 0) & ~0xffff) | (((t1 & 0xffff)) & 0xffff)) & 0xffff) >> 14)) & 0xffff)) + ((t1 << 2) & 0xffffffff)) & 0xffff) << 16) & 0xffffffff) | t2);
}
// 0x00401ca0: generated2
__declspec(noinline) f32 Fn00401CA0(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x3c));
    return (float)(t1);
}

// 0x00404ec0: generated2
__declspec(noinline) f32 Fn00404EC0(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    return (float)(t1);
}

// 0x00405260: generated2
__declspec(noinline) u8 * Fn00405260(u8 * p0, u8 * p1, u8 * p2)
{
    double t1;
    double t2;
    double t3;
    double t4;
    double t5;
    double t6;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x8));
    t2 = (double)((t1) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x8)));
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t4 = (double)((t3) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4)));
    t5 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t6 = (double)((t5) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t6);
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t4);
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8)) = (float)(t2);
    return (u8*)(p0);
}

// 0x00405380: generated2
__declspec(noinline) f32 Fn00405380(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    return (float)(t1);
}

// 0x00405490: generated2
__declspec(noinline) f32 Fn00405490(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    return (float)(t1);
}

// 0x00405ba0: generated2
__declspec(noinline) f32 Fn00405BA0(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x40));
    return (float)(t1);
}

// 0x00408730: generated2
__declspec(noinline) u8 * Fn00408730(u8 * p0, u8 * p1)
{
    double t1;
    double t2;
    double t3;
    double t4;
    double t5;
    double t6;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t2 = (double)((t1) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t2);
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    t4 = (double)((t3) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t4);
    t5 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    t6 = (double)((t5) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x8)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8)) = (float)(t6);
    return (u8*)(p0);
}

// 0x004087e0: generated2
__declspec(noinline) u8 * Fn004087E0(u8 * p0, u8 * p1)
{
    double t1;
    double t2;
    double t3;
    double t4;
    double t5;
    double t6;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t2 = (double)((t1) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t2);
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t4 = (double)((t3) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t4);
    t5 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x8));
    t6 = (double)((t5) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8)) = (float)(t6);
    return (u8*)(p0);
}

// 0x00412e30: generated2
__declspec(noinline) u8 * Fn00412E30(u8 * p0, u8 * p1, u8 * p2)
{
    double t1;
    double t2;
    double t3;
    double t4;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4));
    t2 = (double)((t1) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4)));
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1));
    t4 = (double)((t3) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t4);
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t2);
    return (u8*)(p0);
}

// 0x00412e60: generated2
__declspec(noinline) u8 * Fn00412E60(u8 * p0, u8 * p1, u8 * p2)
{
    double t1;
    double t2;
    double t3;
    double t4;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t2 = (double)((t1) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4)));
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t4 = (double)((t3) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t4);
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t2);
    return (u8*)(p0);
}

// 0x00412f30: generated2
__declspec(noinline) f32 Fn00412F30(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x20));
    return (float)(t1);
}

// 0x00412f40: generated2
__declspec(noinline) f32 Fn00412F40(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x24));
    return (float)(t1);
}

// 0x00412f90: generated2
__declspec(noinline) f32 Fn00412F90(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1c));
    return (float)(t1);
}

// 0x00412fb0: generated2
__declspec(noinline) f32 Fn00412FB0(u8 * p0)
{
    double t1;
    double t2;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4c));
    t2 = (double)((t1) * (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x3c)));
    return (float)(t2);
}

// 0x00412fc0: generated2
__declspec(noinline) f32 Fn00412FC0(u8 * p0)
{
    double t1;
    double t2;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x50));
    t2 = (double)((t1) * (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x40)));
    return (float)(t2);
}

// 0x00412fd0: generated2
__declspec(noinline) f32 Fn00412FD0(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    return (float)(t1);
}

// 0x004130f0: generated2
__declspec(noinline) f32 Fn004130F0(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    return (float)(t1);
}

// 0x00413110: generated2
__declspec(noinline) f32 Fn00413110(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    return (float)(t1);
}

// 0x00428d60: generated2
__declspec(noinline) f32 Fn00428D60(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x2c));
    return (float)(t1);
}

// 0x00428f20: generated2
__declspec(noinline) f32 Fn00428F20(u8 * p0)
{
    double t1;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x34));
    return (float)(t1);
}

// 0x00441dd0: generated2
__declspec(noinline) u8 * Fn00441DD0(u8 * p0, u8 * p1, u8 * p2)
{
    double t1;
    double t2;
    double t3;
    double t4;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4));
    t2 = (double)((t1) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4)));
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1));
    t4 = (double)((t3) - (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t4);
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t2);
    return (u8*)(p0);
}

// 0x00441e00: generated2
__declspec(noinline) u8 * Fn00441E00(u8 * p0, u8 * p1, u8 * p2)
{
    double t1;
    double t2;
    double t3;
    double t4;
    t1 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1+0x4));
    t2 = (double)((t1) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1+0x4)));
    t3 = (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p1*1));
    t4 = (double)((t3) + (double)*(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p2*1)));
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (float)(t4);
    *(float*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4)) = (float)(t2);
    return (u8*)(p0);
}

// 0x0044bff0: generated2
__declspec(noinline) f32 Fn0044BFF0(u8 * p0)
{
    u32 t1;
    double t2;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc));
    t2 = (double)*(float*)((u8*)((uintptr_t)t1*1));
    return (float)(t2);
}
// 0x00402050: generated2

// 0x00404c60: generated2

// 0x004055a0: generated2

// 0x00405c80: generated2

// 0x00409f10: generated2

// 0x00409f50: generated2

// 0x0040c830: generated2

// 0x0040cbd0: generated2

// 0x0040cbf0: generated2

// 0x0040cf30: generated2

// 0x0040cf40: generated2

// 0x0040d260: generated2

// 0x0040d3d0: generated2

// 0x00413190: generated2

// 0x004132a0: generated2

// 0x00418d00: generated2

// 0x0041acf0: generated2

// 0x0041c0a0: generated2

// 0x004220e0: generated2

// 0x0042a9e0: generated2

// 0x0042aae0: generated2

// 0x004388f0: generated2

// 0x004389a0: generated2

// 0x0043bc20: generated2

// 0x0043e580: generated2

// 0x00449a30: generated2
// 0x00402050: generated2
__declspec(noinline) u8 * Fn00402050(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6c));
    t2 = (t1 & 0xfffffffeu);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6c) = (u32)(t2);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xb0)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xb0)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xfc)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xfc)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x128)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x128)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x174)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x174)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1b0)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1b0)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1fc)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1fc)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x228)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x228)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x378)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x378)) & 0xfffffffeu);
    t3 = 0xebu;
    for (u32 i=0; i<0xebu; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0xfffffffeu ^ 0xfffffffeu));
    *(u16*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x384) = (u16)(0xffffu);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x00404c60: generated2
__declspec(noinline) u8 * Fn00404C60(u8 * p0, u8 * p1)
{
    for (u32 i=0; i<0x7u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p1*1))+(uintptr_t)i*4) = *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4);
    return (u8*)(p0);
}

// 0x00405110: generated2
__declspec(noinline) u32 Fn00405110(u8 * p0, u8 * p1)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t2 = ((u32)(uintptr_t)p1 + 0xc);  // was p1+ch: the decoder printed 0xc as `ch`
    *(u32*)(u8*)((uintptr_t)t2*1) = (u32)(t1);
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    *(u32*)(u8*)((uintptr_t)t2*1+0x4) = (u32)(t3);
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    *(u32*)(u8*)((uintptr_t)t2*1+0x8) = (u32)(t4);
    return t4;
}

// 0x004055a0: generated2
__declspec(noinline) u8 * Fn004055A0(u8 * p0)
{
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x24)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x24)) & 0xfffffffeu);
    for (u32 i=0; i<0x12u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    *(u32*)LEAF_GLOB(0x4776ec) = (u32)((u32)(uintptr_t)p0);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x00405c80: generated2
__declspec(noinline) u8 * Fn00405C80(u8 * p0)
{
    u32 t1;
    t1 = 0x84u;
    for (u32 i=0; i<0x84u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x204) = (u32)(0xffffffffu);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x00409f10: generated2
__declspec(noinline) u32 Fn00409F10(u8 * p0)
{
    for (u32 i=0; i<0x873a8u; i++) *(u32*)(((u8*)((uintptr_t)((u32)(uintptr_t)p0 + 0x14u)*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (0 ^ 0);
}

// 0x00409f50: generated2
__declspec(noinline) u8 * Fn00409F50(u8 * p0)
{
    for (u32 i=0; i<0x10u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0040c830: generated2
__declspec(noinline) u8 * Fn0040C830(u8 * p0)
{
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(0x46d0f0u);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1090) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1094) = (u32)((0 ^ 0));
    for (u32 i=0; i<0x426u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0040cbd0: generated2
__declspec(noinline) u32 Fn0040CBD0(u8 * p0)
{
    for (u32 i=0; i<0xfu; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x38) = (u32)(0x42000000u);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x34) = (u32)(0x42000000u);
    return 0x42000000u;
}

// 0x0040cbf0: generated2
__declspec(noinline) u32 Fn0040CBF0(u8 * p0)
{
    for (u32 i=0; i<0; i++) *(u32*)(((u8*)((uintptr_t)((u32)(uintptr_t)p0 + 0x4u)*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (0 ^ 0);
}

// 0x0040cf30: generated2
__declspec(noinline) u8 * Fn0040CF30(u8 * p0)
{
    for (u32 i=0; i<0x77u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0040cf40: generated2
__declspec(noinline) u8 * Fn0040CF40(u8 * p0)
{
    for (u32 i=0; i<0x7eu; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x2c) = (u32)(0x41000000u);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0040d260: generated2
__declspec(noinline) u8 * Fn0040D260(u8 * p0)
{
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x50)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x50)) & 0xfffffffeu);
    for (u32 i=0; i<0x1au; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    *(u32*)LEAF_GLOB(0x477704) = (u32)((u32)(uintptr_t)p0);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0040d3d0: generated2
__declspec(noinline) u8 * Fn0040D3D0(u8 * p0)
{
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1090) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1094) = (u32)((0 ^ 0));
    for (u32 i=0; i<0x426u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1) = (u32)(0x46d0b4u);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x00413120: generated2
__declspec(noinline) u32 Fn00413120(u8 * p0, u8 * p1)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t2 = ((u32)(uintptr_t)p1 + 0xc);  // was p1+ch: the decoder printed 0xc as `ch`
    *(u32*)(u8*)((uintptr_t)t2*1) = (u32)(t1);
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    *(u32*)(u8*)((uintptr_t)t2*1+0x4) = (u32)(t3);
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    *(u32*)(u8*)((uintptr_t)t2*1+0x8) = (u32)(t4);
    return t4;
}

// 0x00413190: generated2
__declspec(noinline) u32 Fn00413190(u8 * p0, u32 p1)
{
    for (u32 i=0; i<0xb; i++)  // was (p1>>8)&0xff: the decoder printed 0xb as `bh` *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (0 ^ 0);
}

// 0x004132a0: generated2
__declspec(noinline) u8 * Fn004132A0(u8 * p0)
{
    for (u32 i=0; i<0x23u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    *(u32*)LEAF_GLOB(0x477708) = (u32)((u32)(uintptr_t)p0);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x00418d00: generated2
__declspec(noinline) u8 * Fn00418D00(u8 * p0)
{
    for (u32 i=0; i<0x22u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x10) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x14) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xc) = (u32)((u32)(uintptr_t)p0);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x84) = (u32)(((0 ^ 0) | 0xffffffffu));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x70) = (u32)(((0 ^ 0) | 0xffffffffu));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6c) = (u32)(0x12cu);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x7c) = (u32)(0x3f800000u);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0041acf0: generated2
__declspec(noinline) u8 * Fn0041ACF0(u8 * p0)
{
    u32 t1;
    u32 t2;
    u32 t3;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6c));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6c) = (u32)((t1 & 0xfffffffeu));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xb0)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xb0)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xfc)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xfc)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x128)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x128)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x174)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x174)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1b0)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1b0)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1fc)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1fc)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x228)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x228)) & 0xfffffffeu);
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x378)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x378)) & 0xfffffffeu);
    t2 = 0xebu;
    for (u32 i=0; i<0xebu; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = (((t1 & 0xfffffffeu) ^ (t1 & 0xfffffffeu)));
    *(u16*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x384) = (u16)(0xffffu);
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x3d8));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x3d8) = (u32)((t3 & 0xfffffffeu));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0041c0a0: generated2
__declspec(noinline) u8 * Fn0041C0A0(u8 * p0, u8 * p1)
{
    for (u32 i=0; i<0x77u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p1*1))+(uintptr_t)i*4);
    return (u8*)(p0);
}

// 0x004220e0: generated2
__declspec(noinline) u8 * Fn004220E0(u8 * p0)
{
    u32 t1;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x20));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x20) = (u32)((t1 & 0xfffffffeu));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xb0) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x24) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xf8) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xf4) = (u32)(0x1u);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x2c) = (u32)(0x3e7u);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x104) = (u32)(0x3e7u);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x188) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xfc) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1d0) = (u32)((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x1cc) = (u32)(0x1u);
    for (u32 i=0; i<0xb2u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    *(u32*)LEAF_GLOB(0x477830) = (u32)((u32)(uintptr_t)p0);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0042a9e0: generated2
__declspec(noinline) u8 * Fn0042A9E0(u8 * p0)
{
    for (u32 i=0; i<0x71u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0042aae0: generated2
__declspec(noinline) u8 * Fn0042AAE0(u8 * p0)
{
    for (u32 i=0; i<0x18a1u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6274) = (u32)((u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5464));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5460) = (u32)((u32)(uintptr_t)p0);
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x627c) = (u32)(((u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5464) ^ (u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5464)));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6280) = (u32)(((u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5464) ^ (u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x5464)));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x6278) = (u32)((u32)(uintptr_t)p0);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x004388f0: generated2
__declspec(noinline) u8 * Fn004388F0(u8 * p0)
{
    for (u32 i=0; i<0x2801u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0xa000) = (u32)((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x004389a0: generated2
__declspec(noinline) u8 * Fn004389A0(u8 * p0)
{
    for (u32 i=0; i<0x16u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0043bc20: generated2
__declspec(noinline) u8 * Fn0043BC20(u8 * p0)
{
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x40)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x40)) & 0xfffffffeu);
    for (u32 i=0; i<0x11u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) = (*(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1)) | 0x2u);
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x0043e580: generated2
__declspec(noinline) u8 * Fn0043E580(u8 * p0)
{
    for (u32 i=0; i<0x14b4u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    for (u32 i=0; i<0x80u; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)(u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x408)*1))+(uintptr_t)i*4) = (((0 ^ 0) | 0xffffffffu));
    return (u8*)((u32)(uintptr_t)p0);
}

// 0x004421c0: generated2
__declspec(noinline) u32 Fn004421C0(u8 * p0, u8 * p1)
{
    u32 t1;
    u32 t2;
    u32 t3;
    u32 t4;
    t1 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1));
    t2 = ((u32)(uintptr_t)p1 + 0xc);  // was p1+ch: the decoder printed 0xc as `ch`
    *(u32*)(u8*)((uintptr_t)t2*1) = (u32)(t1);
    t3 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x4));
    *(u32*)(u8*)((uintptr_t)t2*1+0x4) = (u32)(t3);
    t4 = *(u32*)((u8*)((uintptr_t)(u32)(uintptr_t)p0*1+0x8));
    *(u32*)(u8*)((uintptr_t)t2*1+0x8) = (u32)(t4);
    return t4;
}

// 0x00449a30: generated2
__declspec(noinline) u8 * Fn00449A30(u8 * p0)
{
    for (u32 i=0; i<0x4cu; i++) *(u32*)(((u8*)((uintptr_t)(u32)(uintptr_t)p0*1))+(uintptr_t)i*4) = ((0 ^ 0));
    return (u8*)((u32)(uintptr_t)p0);
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0040ba80: ghidra (inventoried as th10::Ending::OnDraw)
__declspec(noinline) u32 Fn0040BA80(void)
{
    return 1;
}

// 0x0041feb0: ghidra (inventoried as FUN_0041FEB0)
__declspec(noinline) u32 Fn0041FEB0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 puVar1;
    i32 iVar2;
    iVar2 = (*(u32 *)LEAF_GLOB(0x4776e0u));
    if (((*(u32 *)(uintptr_t)(u32)(p0_)) & 2) != 0) {
        puVar1 = (*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776e0u)) + 0xc)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
        puVar1 = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x10)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
        puVar1 = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x89a8)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
        (*(u32 *)LEAF_GLOB(0x491ff4u)) = (*(u32 *)LEAF_GLOB(0x491ff4u)) & 0xffffefff;
        (*(u32 *)LEAF_GLOB(0x491fb8u)) = 4;
        (*(u32 *)(uintptr_t)(u32)(p0_)) = (*(u32 *)(uintptr_t)(u32)(p0_)) & 0xfffffffd;
    }
    return 1;
}

// 0x004246c0: ghidra (inventoried as FUN_004246C0)
__declspec(noinline) void Fn004246C0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 puVar1;
    i32 iVar2;
    u32 puVar3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x80)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x80)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc4)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x13c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x13c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x188)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x188)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c4)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x210)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x210)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x23c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x23c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38c)) & 0xfffffffe;
    puVar3 = p0_ + 0x14;
    for (iVar2 = 0xeb; iVar2 != 0; iVar2 = iVar2 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = 0;
        puVar3 = puVar3 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x398)) = 0xffff;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x470)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x470)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x484)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x484)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x498)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x498)) & 0xfffffffe;
    puVar1 = p0_ + 0x4ac;
    iVar2 = 0x80;
    do {
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) & 0xfffffffe;
        puVar1 = puVar1 + 0x5c;
        iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3320)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3320)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x33b8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x33b8)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3450)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3450)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34e8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34e8)) & 0xfffffffe;
    puVar1 = p0_ + 0x3560;
    iVar2 = 0x21;
    do {
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) & 0xfffffffe;
        puVar1 = puVar1 + 0x6c;
        iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x431c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x431c)) & 0xfffffffe;
    puVar3 = p0_;
    for (iVar2 = 0x111e; iVar2 != 0; iVar2 = iVar2 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = 0;
        puVar3 = puVar3 + 0x4;
    }
    (*(u32 *)LEAF_GLOB(0x477834u)) = p0_;
    return;
}

// 0x00429a30: ghidra (inventoried as FUN_00429A30)
__declspec(noinline) u32 Fn00429A30(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 uVar1;
    bool bVar2;
    if ((((*(u32 *)LEAF_GLOB(0x477810u)) != 0) && ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) == 1)) && (((*(u32 *)LEAF_GLOB(0x474e30u)) & 0x100) != 0)) {
        uVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c8)) & 0x80000003;
        bVar2 = uVar1 == 0;
        if (uVar1 < 0) {
            bVar2 = (uVar1 - 1 | 0xfffffffc) == 0xffffffff;
        }
        if (!bVar2) {
            return 6;
        }
    }
    return 1;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00412300: ghidra (inventoried as FUN_00412300)
__declspec(noinline) i32 Fn00412300(i32 p0, u32 p1)
{
    switch(p1) {
        case 0xffffd8ff:
        return p0 + 0x1138;
        case 0xffffd900:
        return p0 + 0x113c;
        case 0xffffd901:
        return p0 + 0x1140;
        case 0xffffd902:
        return p0 + 0x1144;
        default:
        return 0;
    }
}

// 0x004126d0: ghidra (inventoried as FUN_004126D0)
__declspec(noinline) i32 Fn004126D0(i32 p0, u32 p1)
{
    switch(p1) {
        case 0xffffd903:
        return p0 + 0x1148;
        case 0xffffd904:
        return p0 + 0x114c;
        case 0xffffd905:
        return p0 + 0x1150;
        case 0xffffd906:
        return p0 + 0x1154;
        default:
        return 0;
    }
}

// 0x00420b60: ghidra (inventoried as FUN_00420B60)
__declspec(noinline) u32 Fn00420B60(void)
{
    if (((*(u8 *)LEAF_GLOB(0x491d63u)) != '\x02') && ((*(u8 *)LEAF_GLOB(0x491d63u)) != '\x01')) {
        return 0xffffffff;
    }
    return 0;
}

// 0x00427c70: ghidra (inventoried as FUN_00427C70)
__declspec(noinline) u32 Fn00427C70(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(i32 *)(uintptr_t)(u32)(p0_ + 0x4)) != (*(i32 *)(uintptr_t)(u32)(p0_))) && ((*(i32 *)(uintptr_t)(u32)(p0_ + 0x4)) % p1 == 0)) {
        return 1;
    }
    return 0;
}

// 0x00427d30: ghidra (inventoried as FUN_00427D30)
__declspec(noinline) u32 Fn00427D30(i32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    if (((*(i32 *)(uintptr_t)(u32)(p1_)) == p0) && ((*(i32 *)(uintptr_t)(u32)(p1_ + 0x4)) == p0)) {
        return 0;
    }
    return 1;
}

// 0x004464d0: ghidra (inventoried as FUN_004464D0)
__declspec(noinline) i32 Fn004464D0(i32 p0)
{
    i32 iVar1;
    if (((*(u8 *)LEAF_GLOB(0x491d78u)) & 1) != 0) {
        iVar1 = (*(u32 *)(uintptr_t)(u32)(p0 * 4 + ((u32)(uintptr_t)LEAF_GLOB(0x46f288u))));
        if ((iVar1 == 0x15) || (iVar1 == 0)) {
            p0 = 5;
        }
        else if (iVar1 == 0x14) {
            return 3;
        }
    }
    return p0;
}

// 0x00449a00: ghidra (inventoried as FUN_00449A00)
__declspec(noinline) u32 Fn00449A00(u32 p0, i32 p1)
{
    if ((p1 + 0x68U <= p0) && (p0 < p1 + 0x3ac068U)) {
        return 1;
    }
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004059f0: ghidra (inventoried as FUN_004059F0)
__declspec(noinline) i32 Fn004059F0(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    f32 fVar1;
    f32 fVar2;
    i32 iVar3;
    if ((*(u32 *)(uintptr_t)(u32)(p1_ + 0x28)) == 0) {
        return 0;
    }
    iVar3 = 0;
    fVar1 = (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) - (*(f32 *)(uintptr_t)(u32)(p1_ + 0x34));
    fVar2 = (*(f32 *)(uintptr_t)(u32)(p0_)) - (*(f32 *)(uintptr_t)(u32)(p1_ + 0x30));
    if (fVar1 * fVar1 + fVar2 * fVar2 < (*(f32 *)(uintptr_t)(u32)(p1_ + 0x3c)) * (*(f32 *)(uintptr_t)(u32)(p1_ + 0x3c))) {
        if ((*(u32 *)(uintptr_t)(u32)(p1_ + 0x44)) == 0) {
            if (((*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776f4u)) + 0x378c)) & 1) != 0) {
                return 3;
            }
        }
        else if (((*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776f4u)) + 0x378c)) & 1) != 0) {
            return 0x26;
        }
        return (-((*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x477704u)) + 0x10)) != 0) & 0x21) + 5;
    }
    if (((*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776f4u)) + 0x378c)) & 1) == 0) {
        if (((*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x477704u)) + 0x10)) != 0) && ((*(u32 *)(uintptr_t)(u32)(p1_ + 0x44)) == 0)) {
            iVar3 = ((*(u32 *)LEAF_GLOB(0x474c68u)) == 0) * 4 + 0x10;
        }
    }
    else if ((*(u32 *)(uintptr_t)(u32)(p1_ + 0x44)) == 1) {
        return (-((*(u32 *)LEAF_GLOB(0x474c68u)) != 0) & 0xfffffffd) + 0x1a;
    }
    return iVar3;
}

// 0x00405b60: ghidra (inventoried as FUN_00405B60)
__declspec(noinline) void Fn00405B60(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    p1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x58)) + p1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x58)) = p1;
    if (0x400 < p1) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x58)) = 0x400;
        return;
    }
    if (p1 < -0x400) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x58)) = 0xfffffc00;
    }
    return;
}

// 0x0040cc70: ghidra (inventoried as FUN_0040CC70)
__declspec(noinline) i32 Fn0040CC70(u32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    i32 iVar2;
    u32 puVar3;
    u32 puVar4;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 300)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 300)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x17c)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x17c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1c8)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1c8)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x204)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x204)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x240)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x240)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x27c)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x27c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2b8)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2b8)) & 0xfffffffe;
    puVar3 = p1_ + 0x2c4;
    iVar2 = 8;
    do {
        puVar4 = puVar3;
        for (iVar1 = 0x84; iVar1 != 0; iVar1 = iVar1 + -1) {
            (*(u32 *)(uintptr_t)(u32)(puVar4)) = 0;
            puVar4 = puVar4 + 0x4;
        }
        (*(u32 *)(uintptr_t)(u32)(puVar3 + 0x204)) = 0xffffffff;
        puVar3 = puVar3 + 0x210;
        iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x142c)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x142c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1440)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1440)) & 0xfffffffe;
    return p1_;
}

// 0x00418ec0: ghidra (inventoried as th10::Hint::MarkChainFinished)
__declspec(noinline) void Fn00418EC0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 puVar1;
    if ((*(u32 *)(uintptr_t)(u32)(p0_ + 8)) != 0) {
        puVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 8)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
    }
    if ((*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) != 0) {
        puVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
    }
    return;
}

// 0x00419930: ghidra (inventoried as FUN_00419930)
__declspec(noinline) u32 Fn00419930(u32 p0, i32 p1, i32 p2, u8 *p3)
{
    u32 p3_ = (u32)(uintptr_t)p3;
    i32 iVar1;
    u32 piVar2;
    iVar1 = 0;
    if (0 < p1) {
        piVar2 = p3_ + 4;
        do {
            if ((*(i32 *)(uintptr_t)(u32)(piVar2)) == p2) {
                return (*(u32 *)(uintptr_t)(u32)(p3_ + iVar1 * 8));
            }
            iVar1 = iVar1 + 1;
            piVar2 = piVar2 + 0x8;
        } while (iVar1 < p1);
    }
    return 0;
}

// 0x0041ab00: ghidra (inventoried as FUN_0041AB00)
__declspec(noinline) u32 Fn0041AB00(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x44)) != 0) {
        return (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4c));
    }
    return (*(u32 *)(uintptr_t)(u32)(p0_ + 0x48));
}

// 0x00421f60: ghidra (inventoried as FUN_00421F60)
__declspec(noinline) void Fn00421F60(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    i32 iVar1;
    iVar1 = (*(u32 *)LEAF_GLOB(0x474c44u));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x9e78)) = (*(u32 *)LEAF_GLOB(0x474c44u));
    if ((*(u32 *)LEAF_GLOB(0x474c40u)) < iVar1) {
        (*(u32 *)LEAF_GLOB(0x474c40u)) = iVar1;
    }
    return;
}

// 0x00427e50: ghidra (inventoried as FUN_00427E50)
__declspec(noinline) u32 Fn00427E50(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1444)) & 0x11) == 0) && (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1444)) & 0xc0000) == 0)) {
        return 0;
    }
    return 1;
}

// 0x00427e70: ghidra (inventoried as FUN_00427E70)
__declspec(noinline) u32 Fn00427E70(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((((*(u32 *)(uintptr_t)(u32)(p0_ + 0x2480)) & 0x11) == 0) && (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x2480)) & 0xc0000) == 0)) {
        return 0;
    }
    return 1;
}

// 0x0042a3e0: ghidra (inventoried as FUN_0042A3E0)
__declspec(noinline) u32 Fn0042A3E0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 uVar1;
    u32 uVar2;
    bool bVar3;
    if ((*(u32 *)LEAF_GLOB(0x477810u)) != 0) {
        uVar1 = 1;
        if (((((*(u8 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x477810u)) + 0x58)) & 4) == 0) && ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) == 1)) && (((*(u32 *)LEAF_GLOB(0x474e30u)) & 0x100) != 0)) {
            uVar2 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c8)) & 0x80000003;
            bVar3 = uVar2 == 0;
            if (uVar2 < 0) {
                bVar3 = (uVar2 - 1 | 0xfffffffc) == 0xffffffff;
            }
            if (!bVar3) {
                uVar1 = 6;
            }
        }
        return uVar1;
    }
    return 1;
}

// 0x00438240: ghidra (inventoried as FUN_00438240)
__declspec(noinline) i32 Fn00438240(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (0xf < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18))) {
        return (*(u32 *)(uintptr_t)(u32)(p0_ + 4));
    }
    return p0_ + 4;
}

// 0x004383e0: ghidra (inventoried as FUN_004383E0)
__declspec(noinline) i32 Fn004383E0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (0xf < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18))) {
        return (*(u32 *)(uintptr_t)(u32)(p0_ + 4));
    }
    return p0_ + 4;
}

// 0x00438680: ghidra (inventoried as FUN_00438680)
__declspec(noinline) i32 Fn00438680(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (0xf < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18))) {
        return (*(u32 *)(uintptr_t)(u32)(p0_ + 4));
    }
    return p0_ + 4;
}

// 0x0044d3e0: ghidra (inventoried as FUN_0044D3E0)
__declspec(noinline) u32 Fn0044D3E0(u8 *p0, u32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 4)) != 0) && (p1 < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)))) {
        return (*(u32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p0_ + 4)) + p1 * 4));
    }
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004045b0: ghidra (inventoried as FUN_004045B0)
__declspec(noinline) void Fn004045B0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a2c)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a20)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a1c)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a24)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a28)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a2c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a2c)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a20)) = 0x3c;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a24)) = 0x42700000;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a1c)) = 0x3b;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a18)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2a18)) | 4;
    return;
}

// 0x00404ca0: ghidra (inventoried as FUN_00404CA0)
__declspec(noinline) void Fn00404CA0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x80)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x74)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x70)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x78)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x7c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x80)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x80)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x74)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x78)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x70)) = 0xffffffff;
    return;
}

// 0x004050d0: ghidra (inventoried as FUN_004050D0)
__declspec(noinline) void Fn004050D0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = 0xffffffff;
    return;
}

// 0x00405be0: ghidra (inventoried as FUN_00405BE0)
__declspec(noinline) void Fn00405BE0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    (*(u16 *)(uintptr_t)(u32)(p0_ + 0x446)) = 0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x408)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3fc)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3f8)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x400)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x404)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x408)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x408)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3fc)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x400)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3f8)) = 0xffffffff;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x41c)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x410)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40c)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x414)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x418)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x41c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x41c)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x410)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x414)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40c)) = 0xffffffff;
    return;
}

// 0x00412db0: ghidra (inventoried as FUN_00412DB0)
__declspec(noinline) void Fn00412DB0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x28)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x28)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0xffffffff;
    return;
}

// 0x004175e0: ghidra (inventoried as FUN_004175E0)
__declspec(noinline) void Fn004175E0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((i32)(*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) < 7) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) + 1;
    }
    (*(u32 *)LEAF_GLOB(0x477848u)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) * 0x30 + 0x474788;
    return;
}

// 0x00417800: ghidra (inventoried as FUN_00417800)
__declspec(noinline) u32 Fn00417800(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x50)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x44)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x48)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x50)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x50)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x44)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x48)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) = 0xffffffff;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x58)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x5c)) = 0;
    return 0;
}

// 0x00418b80: ghidra (inventoried as FUN_00418B80)
__declspec(noinline) void Fn00418B80(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) = p1 / 10;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0xffffffff;
    return;
}

// 0x00418cc0: ghidra (inventoried as FUN_00418CC0)
__declspec(noinline) void Fn00418CC0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 uVar1;
    uVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = uVar1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0xfff0bdc1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0x476f78;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = uVar1 | 1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0xffffffff;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 1;
    return;
}

// 0x0041ac40: ghidra (inventoried as FUN_0041AC40)
__declspec(noinline) void Fn0041AC40(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x28)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x28)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0xffffffff;
    return;
}

// 0x0043c930: ghidra (inventoried as FUN_0043C930)
__declspec(noinline) void Fn0043C930(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2c)) = 1;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = 0xffffffff;
    return;
}

// 0x00442010: ghidra (inventoried as FUN_00442010)
__declspec(noinline) void Fn00442010(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) = 0xffffffff;
    return;
}

// 0x004421e0: ghidra (inventoried as FUN_004421E0)
__declspec(noinline) void Fn004421E0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x40)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x30)) = 0xffffffff;
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0044bc10: ghidra (inventoried as FUN_0044BC10)
__declspec(noinline) void Fn0044BC10(f32 p0, f32 p1)
{
    bool bVar1;
    int iVar2;
    int iVar3;
    p0 = p0 + p1;
    iVar2 = 0;
    do {
        if (p0 <= (*(u32 *)LEAF_GLOB(0x470b18u))) break;
        p0 = p0 - (*(u32 *)LEAF_GLOB(0x470b14u));
        iVar3 = iVar2 + 1;
        bVar1 = iVar2 < 0x21;
        iVar2 = iVar3;
    } while (bVar1);
    do {
        if ((*(u32 *)LEAF_GLOB(0x470b10u)) <= p0) {
            return;
        }
        p0 = p0 + (*(u32 *)LEAF_GLOB(0x470b14u));
        bVar1 = iVar2 < 0x21;
        iVar2 = iVar2 + 1;
    } while (bVar1);
    return;
}

// 0x0044bc70: ghidra (inventoried as FUN_0044BC70)
__declspec(noinline) void Fn0044BC70(f32 p0)
{
    bool bVar1;
    int iVar2;
    int iVar3;
    iVar2 = 0;
    do {
        if (p0 <= (*(u32 *)LEAF_GLOB(0x470b18u))) break;
        p0 = p0 - (*(u32 *)LEAF_GLOB(0x470b14u));
        iVar3 = iVar2 + 1;
        bVar1 = iVar2 < 0x21;
        iVar2 = iVar3;
    } while (bVar1);
    do {
        if ((*(u32 *)LEAF_GLOB(0x470b10u)) <= p0) {
            return;
        }
        p0 = p0 + (*(u32 *)LEAF_GLOB(0x470b14u));
        bVar1 = iVar2 < 0x21;
        iVar2 = iVar2 + 1;
    } while (bVar1);
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004245d0: ghidra (inventoried as FUN_004245D0)
__declspec(noinline) u32 Fn004245D0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 8)) != 0) && (((*(u8 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p0_ + 8)) + 4)) & 2) != 0)) {
        return 1;
    }
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004216f0: ghidra (inventoried as FUN_004216F0)
__declspec(noinline) void Fn004216F0(void)
{
    (*(u32 *)LEAF_GLOB(0x491e94u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491e98u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491e9cu)) = 0x447a0000;
    (*(u32 *)LEAF_GLOB(0x491ea0u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491ea4u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491ea8u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491eacu)) = 0;
    (*(u32 *)LEAF_GLOB(0x491eb0u)) = 0x3f800000;
    (*(u32 *)LEAF_GLOB(0x491eb4u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491ed0u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491ed4u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491ed8u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d7cu)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d80u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d88u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d90u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d84u)) = 0x447a0000;
    (*(u32 *)LEAF_GLOB(0x491d98u)) = 0x3f800000;
    (*(u32 *)LEAF_GLOB(0x491edcu)) = 0x3f060a92;
    (*(u32 *)LEAF_GLOB(0x491f60u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491f64u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491f68u)) = 0x280;
    (*(u32 *)LEAF_GLOB(0x491f6cu)) = 0x1e0;
    (*(u32 *)LEAF_GLOB(0x491f70u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491f74u)) = 0x3f800000;
    (*(u32 *)LEAF_GLOB(0x491f78u)) = 1;
    (*(u32 *)LEAF_GLOB(0x491d8cu)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d94u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491d9cu)) = 0;
    (*(u32 *)LEAF_GLOB(0x491dc4u)) = 0x3f060a92;
    (*(u32 *)LEAF_GLOB(0x491e48u)) = 0x20;
    (*(u32 *)LEAF_GLOB(0x491e4cu)) = 0x10;
    (*(u32 *)LEAF_GLOB(0x491e50u)) = 0x180;
    (*(u32 *)LEAF_GLOB(0x491e54u)) = 0x1c0;
    (*(u32 *)LEAF_GLOB(0x491e58u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491e5cu)) = 0x3f800000;
    (*(u32 *)LEAF_GLOB(0x491e60u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491db8u)) = 0;
    (*(u32 *)LEAF_GLOB(0x491dbcu)) = 0;
    (*(u32 *)LEAF_GLOB(0x491dc0u)) = 0;
    return;
}

// 0x00428ad0: ghidra (inventoried as FUN_00428AD0)
__declspec(noinline) u32 Fn00428AD0(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4c)) = 0;
    iVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3504));
    if ((iVar1 != 0) && ((*(u32 *)(uintptr_t)(u32)(p1_ + 0x4c)) = iVar1, (*(u32 *)LEAF_GLOB(0x470b4cu)) < ABS((*(u32 *)(uintptr_t)(u32)(iVar1 + 0x1068))))) {
        (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4c)) = 0;
    }
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004086b0: ghidra (inventoried as FUN_004086B0)
__declspec(noinline) u32 Fn004086B0(u8 *p0, f32 p1, f32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((((p1 + (*(f32 *)(uintptr_t)(u32)(p0_)) < (*(u32 *)LEAF_GLOB(0x470b40u)) == (p1 + (*(f32 *)(uintptr_t)(u32)(p0_)) == (*(u32 *)LEAF_GLOB(0x470b40u)))) && ((*(f32 *)(uintptr_t)(u32)(p0_)) - p1 < (*(u32 *)LEAF_GLOB(0x470b3cu)))) &&
    (p2 + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) < (*(u32 *)LEAF_GLOB(0x470b04u)) == (p2 + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) == (*(u32 *)LEAF_GLOB(0x470b04u))))) && ((*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) - p2 < (*(u32 *)LEAF_GLOB(0x470b38u)))
    ) {
        return 0;
    }
    return 1;
}

// 0x0041f7a0: ghidra (inventoried as FUN_0041F7A0)
__declspec(noinline) u32 Fn0041F7A0(u8 *p0, f32 p1, f32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((((p1 + (*(f32 *)(uintptr_t)(u32)(p0_)) < (*(u32 *)LEAF_GLOB(0x470b40u)) == (p1 + (*(f32 *)(uintptr_t)(u32)(p0_)) == (*(u32 *)LEAF_GLOB(0x470b40u)))) && ((*(f32 *)(uintptr_t)(u32)(p0_)) - p1 < (*(u32 *)LEAF_GLOB(0x470b3cu)))) &&
    (p2 + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) < (*(u32 *)LEAF_GLOB(0x470b04u)) == (p2 + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) == (*(u32 *)LEAF_GLOB(0x470b04u))))) && ((*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) - p2 < (*(u32 *)LEAF_GLOB(0x470b38u)))
    ) {
        return 0;
    }
    return 1;
}

// 0x00428d70: ghidra (inventoried as FUN_00428D70)
__declspec(noinline) u32 Fn00428D70(u8 *p0, f32 p1, f32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if ((((p1 + (*(f32 *)(uintptr_t)(u32)(p0_)) < (*(u32 *)LEAF_GLOB(0x470b40u)) == (p1 + (*(f32 *)(uintptr_t)(u32)(p0_)) == (*(u32 *)LEAF_GLOB(0x470b40u)))) && ((*(f32 *)(uintptr_t)(u32)(p0_)) - p1 < (*(u32 *)LEAF_GLOB(0x470b3cu)))) &&
    (p2 + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) < (*(u32 *)LEAF_GLOB(0x470b04u)) == (p2 + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) == (*(u32 *)LEAF_GLOB(0x470b04u))))) && ((*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) - p2 < (*(u32 *)LEAF_GLOB(0x470b38u)))
    ) {
        return 0;
    }
    return 1;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00436da0: ghidra (inventoried as FUN_00436DA0)
__declspec(noinline) u32 Fn00436DA0(u8 *p0, u32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u16 uVar1;
    u32 puVar2;
    i32 iVar3;
    u32 uVar4;
    u32 puVar5;
    u32 uVar6;
    u32 uVar7;
    u32 puVar8;
    u32 uVar9;
    u32 puVar10;
    u32 uStack_14;
    u32 uStack_10;
    u32 uStack_8;
    u32 uStack_4;
    if ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x100)) == 0x15) {
        puVar8 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x120));
        uStack_14 = 0;
        if (p1 != 0) {
            do {
                uVar6 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104));
                uStack_10 = 0;
                if (uVar6 != 0) {
                    puVar5 = puVar8 + -2;
                    do {
                        uVar4 = 0;
                        if ((*(u8 *)(uintptr_t)(u32)(puVar5 + 0x5)) == 0) {
                            uVar7 = 0;
                            uStack_4 = 0;
                            uStack_8 = 0;
                            if ((uStack_10 != 0) && ((*(u8 *)(uintptr_t)(u32)(puVar5 + 0x1)) != 0)) {
                                uStack_8 = (*(u8 *)(uintptr_t)(u32)(puVar5 + -0x1));
                                uVar4 = (*(u8 *)(uintptr_t)(u32)(puVar5));
                                uStack_4 = (*(u8 *)(uintptr_t)(u32)(puVar5 + -0x2));
                                uVar7 = 1;
                            }
                            if ((uStack_10 < uVar6 - 1) && ((*(u8 *)(uintptr_t)(u32)(puVar5 + 0x9)) != 0)) {
                                uVar4 = uVar4 + (*(u8 *)(uintptr_t)(u32)(puVar5 + 0x8));
                                uStack_8 = uStack_8 + (*(u8 *)(uintptr_t)(u32)(puVar5 + 0x7));
                                uStack_4 = uStack_4 + (*(u8 *)(uintptr_t)(u32)(puVar5 + 0x6));
                                uVar7 = uVar7 + 1;
                            }
                            if ((uStack_14 != 0) &&
                            (puVar2 = puVar8 + ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) + ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) >> 0x1f & 3U) >> 2) * -4,
                            (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x3)) != 0)) {
                                uVar4 = uVar4 + (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x2));
                                uStack_8 = uStack_8 + (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x1));
                                uStack_4 = uStack_4 + (*(u8 *)(uintptr_t)(u32)(puVar2));
                                uVar7 = uVar7 + 1;
                            }
                            if ((uStack_14 < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x108)) - 1U) &&
                            (iVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) + ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) >> 0x1f & 3U) >> 2,
                            puVar2 = puVar8 + iVar3 * 4, (*(u8 *)(uintptr_t)(u32)(puVar8 + (iVar3 * 4 + 3) * 1)) != '\0')) {
                                uVar4 = uVar4 + (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x2));
                                uStack_8 = uStack_8 + (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x1));
                                uStack_4 = uStack_4 + (*(u8 *)(uintptr_t)(u32)(puVar2));
                                uVar7 = uVar7 + 1;
                            }
                            if (1 < uVar7) {
                                uVar4 = uVar4 / uVar7;
                                uStack_8 = uStack_8 / uVar7;
                                uStack_4 = uStack_4 / uVar7;
                            }
                            (*(u8 *)(uintptr_t)(u32)(puVar5 + 0x4)) = uVar4;
                            (*(u8 *)(uintptr_t)(u32)(puVar5 + 0x3)) = uStack_8;
                            (*(u8 *)(uintptr_t)(u32)(puVar8)) = uStack_4;
                        }
                        uVar6 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104));
                        puVar8 = puVar8 + 4;
                        puVar5 = puVar5 + 4;
                        uStack_10 = uStack_10 + 1;
                    } while (uStack_10 < uVar6);
                }
                uStack_14 = uStack_14 + 1;
            } while (uStack_14 < p1);
        }
    }
    else if ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x100)) == 0x1a) {
        puVar10 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x120));
        uStack_14 = 0;
        if (p1 != 0) {
            do {
                uVar6 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104));
                uStack_10 = 0;
                if (uVar6 != 0) {
                    do {
                        if (((*(u16 *)(uintptr_t)(u32)(puVar10)) & 0xf000) == 0) {
                            uVar4 = 0;
                            uVar7 = 0;
                            uVar9 = 0;
                            uStack_4 = 0;
                            if (uStack_10 != 0) {
                                uVar1 = (*(u16 *)(uintptr_t)(u32)(puVar10 + -0x2));
                                if ((uVar1 & 0xf000) != 0) {
                                    uVar4 = uVar1 >> 8 & 0xf;
                                    uVar7 = uVar1 >> 4 & 0xf;
                                    uStack_4 = uVar1 & 0xf;
                                    uVar9 = 1;
                                }
                            }
                            if (uStack_10 < uVar6 - 1) {
                                uVar1 = (*(u16 *)(uintptr_t)(u32)(puVar10 + 0x2));
                                if ((uVar1 & 0xf000) != 0) {
                                    uVar4 = uVar4 + (uVar1 >> 8 & 0xf);
                                    uVar7 = uVar7 + (uVar1 >> 4 & 0xf);
                                    uStack_4 = uStack_4 + (uVar1 & 0xf);
                                    uVar9 = uVar9 + 1;
                                }
                            }
                            if (uStack_14 != 0) {
                                uVar1 = (*(u16 *)(uintptr_t)(u32)(puVar10 + (-((*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) / 2)) * 2));
                                if ((uVar1 & 0xf000) != 0) {
                                    uVar4 = uVar4 + (uVar1 >> 8 & 0xf);
                                    uVar7 = uVar7 + (uVar1 >> 4 & 0xf);
                                    uStack_4 = uStack_4 + (uVar1 & 0xf);
                                    uVar9 = uVar9 + 1;
                                }
                            }
                            if ((uStack_14 < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x108)) - 1U) &&
                            (uVar1 = (*(u16 *)(uintptr_t)(u32)(puVar10 + ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) / 2) * 2)), (uVar1 & 0xf000) != 0)) {
                                uVar4 = uVar4 + (uVar1 >> 8 & 0xf);
                                uVar7 = uVar7 + (uVar1 >> 4 & 0xf);
                                uStack_4 = uStack_4 + (uVar1 & 0xf);
                                uVar9 = uVar9 + 1;
                            }
                            if (1 < uVar9) {
                                uVar4 = uVar4 / uVar9;
                                uVar7 = uVar7 / uVar9;
                                uStack_4 = uStack_4 / uVar9;
                            }
                            uStack_4 = (uStack_4 & 0xffffff00u) | (((uStack_4 >> 1) << 0) & 0xffu);
                            (*(u16 *)(uintptr_t)(u32)(puVar10)) = ((uVar4 >> 1 & 0xffffff0f) << 4 | uVar7 >> 1 & 0xffffff0f) << 4 |
                            (*(u16 *)(uintptr_t)(u32)(puVar10)) & 0xf000 | uStack_4 & 0xf;
                        }
                        uVar6 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104));
                        puVar10 = puVar10 + 0x2;
                        uStack_10 = uStack_10 + 1;
                    } while (uStack_10 < uVar6);
                }
                uStack_14 = uStack_14 + 1;
            } while (uStack_14 < p1);
            return 1;
        }
    }
    return 1;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004088c0: ghidra (inventoried as FUN_004088C0)
__declspec(noinline) i32 Fn004088C0(u32 p0, i32 p1)
{
    i32 iVar1;
    i32 iVar2;
    iVar1 = 0;
    iVar2 = 0;
    do {
        if ((*(i8 *)(uintptr_t)(u32)(iVar2 + ((u32)(uintptr_t)LEAF_GLOB(0x4743c0u)))) == p1) {
            iVar1 = iVar1 + 1;
        }
        if ((*(i8 *)(uintptr_t)(u32)(iVar2 + ((u32)(uintptr_t)LEAF_GLOB(0x4743c1u)))) == p1) {
            iVar1 = iVar1 + 1;
        }
        if ((*(i8 *)(uintptr_t)(u32)(iVar2 + ((u32)(uintptr_t)LEAF_GLOB(0x4743c2u)))) == p1) {
            iVar1 = iVar1 + 1;
        }
        if ((*(i8 *)(uintptr_t)(u32)(iVar2 + ((u32)(uintptr_t)LEAF_GLOB(0x4743c3u)))) == p1) {
            iVar1 = iVar1 + 1;
        }
        if ((*(i8 *)(uintptr_t)(u32)(iVar2 + ((u32)(uintptr_t)LEAF_GLOB(0x4743c4u)))) == p1) {
            iVar1 = iVar1 + 1;
        }
        iVar2 = iVar2 + 5;
    } while (iVar2 < 0x6e);
    return iVar1;
}

// 0x00418d40: ghidra (inventoried as FUN_00418D40)
__declspec(noinline) u32 Fn00418D40(u32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    u32 puVar2;
    puVar2 = p1_;
    for (iVar1 = 0x6a; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p1_)) = (*(u32 *)(uintptr_t)(u32)(p1_)) | 2;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x18)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x24)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x28)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x30)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x34)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x38)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3c)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x40)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x44)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x48)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x50)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x54)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x58)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x60)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x64)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x68)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x6c)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x70)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x74)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x78)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x84)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x7c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x80)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x88)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x8c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x90)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x94)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x98)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x9c)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xa0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xa4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xa8)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xac)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xb0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xb4)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xb8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xbc)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xc0)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xc4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xc8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xcc)) = p1_;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xd0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xd4)) = 0;
    (*(u32 *)LEAF_GLOB(0x477814u)) = p1_;
    return p1_;
}

// 0x0042c920: ghidra (inventoried as FUN_0042C920)
__declspec(noinline) u32 Fn0042C920(u32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    u32 puVar2;
    (*(u32 *)(uintptr_t)(u32)(p1_)) = 0x46ecf0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xb0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x24)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xf8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xf4)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2c)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x188)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xfc)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1d0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1cc)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x104)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x260)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1d4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2a8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2a4)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1dc)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2c0)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2c0)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5980)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x58f4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x59c8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x58fc)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x59c4)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5ab4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5ab8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5abc)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5ac0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x5ab0)) = 0x4703e4;
    puVar2 = p1_;
    for (iVar1 = 0x16b3; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4)) | 2;
    (*(u32 *)LEAF_GLOB(0x47784cu)) = p1_;
    return p1_;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0040c6b0: ghidra (inventoried as FUN_0040C6B0)
__declspec(noinline) f64 Fn0040C6B0(void)
{
    return (*(u32 *)LEAF_GLOB(0x470b04u));
}

// 0x004505b0: ghidra (inventoried as FUN_004505B0)
__declspec(noinline) u32 Fn004505B0(u8 *p0, u32 p1, i8 p2, u32 p3, u8 *p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p4_ = (u32)(uintptr_t)p4;
    u32 uVar1;
    u32 puVar2;
    if (0xfff < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + p3) {
        return 0xffffffff;
    }
    if (p2 != '\0') {
        (*(u32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + p0_)) = p2;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + 4;
    }
    puVar2 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + p0_;
    for (uVar1 = p3 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = (*(u32 *)(uintptr_t)(u32)(p4_));
        p4_ = p4_ + 0x4;
        puVar2 = puVar2 + 0x4;
    }
    for (uVar1 = p3 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
        (*(u8 *)(uintptr_t)(u32)(puVar2)) = (*(u8 *)(uintptr_t)(u32)(p4_));
        p4_ = p4_ + 0x1;
        puVar2 = puVar2 + 0x1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + p3;
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00450690: ghidra (inventoried as FUN_00450690)
__declspec(noinline) u32 Fn00450690(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    iVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000));
    p1_ = p1_ + iVar1;
    if (0xfff < p1_) {
        return 0xffffffff;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) = p1_;
    if (p1_ + 4 < 0x1000) {
        (*(u32 *)(uintptr_t)(u32)(p1_ + p0_)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1004));
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + 4;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1004)) = iVar1;
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00405410: ghidra (inventoried as FUN_00405410)
__declspec(noinline) void Fn00405410(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) & 1U) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) = 0;
        (*(i32 *)(uintptr_t)(u32)(p0_)) = -999999;
        (*(f32 *)(uintptr_t)(u32)(p0_ + 0x8)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) = p1;
    (*(i32 *)(uintptr_t)(u32)(p0_)) = p1 + -1;
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x8)) = p1;
    return;
}

// 0x00405450: ghidra (inventoried as FUN_00405450)
__declspec(noinline) void Fn00405450(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) & 1U) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) = 0;
        (*(i32 *)(uintptr_t)(u32)(p0_)) = -999999;
        (*(f32 *)(uintptr_t)(u32)(p0_ + 0x8)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) = p1;
    (*(i32 *)(uintptr_t)(u32)(p0_)) = p1 + -1;
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x8)) = p1;
    return;
}

// 0x0042a930: ghidra (inventoried as FUN_0042A930)
__declspec(noinline) void Fn0042A930(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0xfff0bdc1;
        (*(f32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = p1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = p1 + -1;
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = p1;
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00406160: ghidra (inventoried as FUN_00406160)
__declspec(noinline) u32 Fn00406160(u8 *p0, f32 p1, f32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    f32 fVar1;
    fVar1 = p1 * (*(u32 *)LEAF_GLOB(0x470b0cu)) + (*(f32 *)(uintptr_t)(u32)(p0_));
    if ((((fVar1 < (*(u32 *)LEAF_GLOB(0x470b40u)) == (fVar1 == (*(u32 *)LEAF_GLOB(0x470b40u)))) && ((*(f32 *)(uintptr_t)(u32)(p0_)) - p1 * (*(u32 *)LEAF_GLOB(0x470b0cu)) < (*(u32 *)LEAF_GLOB(0x470b3cu)))) &&
    (fVar1 = p2 * (*(u32 *)LEAF_GLOB(0x470b0cu)) + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)), fVar1 < (*(u32 *)LEAF_GLOB(0x470b5cu)) == (fVar1 == (*(u32 *)LEAF_GLOB(0x470b5cu))))) &&
    ((*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) - p2 * (*(u32 *)LEAF_GLOB(0x470b0cu)) < (*(u32 *)LEAF_GLOB(0x470b38u)))) {
        return 0;
    }
    return 1;
}

// 0x004061d0: ghidra (inventoried as FUN_004061D0)
__declspec(noinline) u32 Fn004061D0(u8 *p0, f32 p1, f32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    f32 fVar1;
    fVar1 = p1 * (*(u32 *)LEAF_GLOB(0x470b0cu)) + (*(f32 *)(uintptr_t)(u32)(p0_));
    if ((((fVar1 < (*(u32 *)LEAF_GLOB(0x470b40u)) == (fVar1 == (*(u32 *)LEAF_GLOB(0x470b40u)))) && ((*(f32 *)(uintptr_t)(u32)(p0_)) - p1 * (*(u32 *)LEAF_GLOB(0x470b0cu)) < (*(u32 *)LEAF_GLOB(0x470b3cu)))) &&
    (fVar1 = p2 * (*(u32 *)LEAF_GLOB(0x470b0cu)) + (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)), fVar1 < (*(u32 *)LEAF_GLOB(0x470b04u)) == (fVar1 == (*(u32 *)LEAF_GLOB(0x470b04u))))) &&
    ((*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) - p2 * (*(u32 *)LEAF_GLOB(0x470b0cu)) < (*(u32 *)LEAF_GLOB(0x470b38u)))) {
        return 0;
    }
    return 1;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004285f0: ghidra (inventoried as FUN_004285F0)
__declspec(noinline) void Fn004285F0(u8 *p0, u8 *p1, u8 *p2, u8 *p3)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 p2_ = (u32)(uintptr_t)p2;
    u32 p3_ = (u32)(uintptr_t)p3;
    (*(f32 *)(uintptr_t)(u32)(p3_)) = ((f64)(*(f32 *)(uintptr_t)(u32)(p1_))) - ((f64)(*(f32 *)(uintptr_t)(u32)(p0_))) * (*(f32 *)LEAF_GLOB(0x470b0cu));
    (*(f32 *)(uintptr_t)(u32)(p3_ + 0x4)) = ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x4))) - ((f64)(*(f32 *)(uintptr_t)(u32)(p0_ + 0x4))) * (*(f32 *)LEAF_GLOB(0x470b0cu));
    (*(f32 *)(uintptr_t)(u32)(p2_)) = ((f64)(*(f32 *)(uintptr_t)(u32)(p0_))) * (*(f32 *)LEAF_GLOB(0x470b0cu)) + ((f64)(*(f32 *)(uintptr_t)(u32)(p1_)));
    (*(f32 *)(uintptr_t)(u32)(p2_ + 0x4)) = ((f64)(*(f32 *)(uintptr_t)(u32)(p0_ + 0x4))) * (*(f32 *)LEAF_GLOB(0x470b0cu)) + ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x4)));
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00418810: ghidra (inventoried as FUN_00418810)
__declspec(noinline) u32 Fn00418810(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (((*(u32 *)LEAF_GLOB(0x497d8cu)) & 1) == 0) {
        (*(u32 *)LEAF_GLOB(0x497d8cu)) = (*(u32 *)LEAF_GLOB(0x497d8cu)) | 1;
    }
    (*(f32 *)LEAF_GLOB(0x497d80u)) = ((f64)(*(f32 *)(uintptr_t)(u32)(p0_))) + (*(f32 *)LEAF_GLOB(0x470b4cu));
    (*(f32 *)LEAF_GLOB(0x497d84u)) = ((f64)(*(f32 *)(uintptr_t)(u32)(p0_ + 0x4))) + (*(f32 *)LEAF_GLOB(0x470b48u));
    (*(u32 *)LEAF_GLOB(0x497d88u)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x8));
    return 0x497d80;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00405d00: ghidra (inventoried as FUN_00405D00)
__declspec(noinline) i32 Fn00405D00(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    i32 iVar1;
    u32 puVar2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x74)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x74)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xb8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0xb8)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x130)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x130)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x17c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x17c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b8)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x204)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x204)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x230)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x230)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x380)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x380)) & 0xfffffffe;
    puVar2 = p0_ + 8;
    for (iVar1 = 0xeb; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x38c)) = 0xffff;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x408)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x408)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x41c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x41c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x624)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x624)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x658)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x658)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x68c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x68c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x6c0)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x6c0)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x6f4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x6f4)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x728)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x728)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x75c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x75c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x790)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x790)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x7c4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x7c4)) & 0xfffffffe;
    return p0_;
}

// 0x0040a040: ghidra (inventoried as FUN_0040A040)
__declspec(noinline) u32 Fn0040A040(u32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    u32 puVar2;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x10)) = 0x4703e4;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x14)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0xc8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x110)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x10c)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x44)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x11c)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1a0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x114)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1e8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1e4)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1f4)) = 999;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x278)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1ec)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2c0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x2bc)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x334)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x334)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x378)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x378)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3c4)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3c4)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3f0)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3f0)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x43c)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x43c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x478)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x478)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4c4)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4c4)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4f0)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4f0)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x640)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x640)) & 0xfffffffe;
    puVar2 = p1_ + 0x2c8;
    for (iVar1 = 0xeb; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x64c)) = 0xffff;
    puVar2 = p1_;
    for (iVar1 = 0x1a2; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p1_)) = (*(u32 *)(uintptr_t)(u32)(p1_)) | 2;
    (*(u32 *)LEAF_GLOB(0x4776f8u)) = p1_;
    return p1_;
}

// 0x004187a0: ghidra (inventoried as FUN_004187A0)
__declspec(noinline) u32 Fn004187A0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    i32 iVar1;
    iVar1 = (*(u32 *)LEAF_GLOB(0x491c10u));
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x58)) & 4) == 0) {
        (*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x491c10u)) + 0x50)) = 0;
        (*(u32 *)(uintptr_t)(u32)(iVar1 + 0x54)) = 0;
        (*(u32 *)(uintptr_t)(u32)(iVar1 + 0x4c)) = 0;
        (*(u32 *)(uintptr_t)(u32)(iVar1 + 0x58)) = 0;
    }
    return 1;
}

// 0x0041f850: ghidra (inventoried as FUN_0041F850)
__declspec(noinline) void Fn0041F850(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    i32 iVar1;
    u32 puVar2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) = 0x4703e4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x9c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x9c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xe0)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0xe0)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x12c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x12c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x158)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x158)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a4)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1e0)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1e0)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x22c)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x22c)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x258)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x258)) & 0xfffffffe;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3a8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3a8)) & 0xfffffffe;
    puVar2 = p0_ + 0x30;
    for (iVar1 = 0xeb; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3b4)) = 0xffff;
    puVar2 = p0_;
    for (iVar1 = 0xfc; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar2)) = 0;
        puVar2 = puVar2 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_)) = (*(u32 *)(uintptr_t)(u32)(p0_)) | 2;
    (*(u32 *)LEAF_GLOB(0x477820u)) = p0_;
    return;
}

// 0x00438590: ghidra (inventoried as FUN_00438590)
__declspec(noinline) void Fn00438590(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = p1;
    if (0xf < (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18))) {
        (*(u32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p0_ + 4)) + p1)) = 0;
        return;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 4 + p1)) = 0;
    return;
}

// 0x00441f50: ghidra (inventoried as FUN_00441F50)
__declspec(noinline) void Fn00441F50(u8 *p0, u32 p1, u32 p2, u8 p3, u8 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x230)) = p2 & 0xff;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x22c)) = p1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20c)) = p4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x208)) = p3;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x228)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x21c)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x218)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x220)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x224)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x228)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x228)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x21c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x220)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x218)) = 0xffffffff;
    return;
}

// 0x00442300: ghidra (inventoried as FUN_00442300)
__declspec(noinline) void Fn00442300(u8 *p0, u32 p1, u8 p2, u8 p3, u8 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 300)) = p1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x130)) = p2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x108)) = p3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x110)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x114)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10c)) = p4;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x128)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x11c)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x118)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x120)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x124)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x128)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x128)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x11c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x120)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x118)) = 0xffffffff;
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0041ab70: ghidra (inventoried as FUN_0041AB70)
__declspec(noinline) void Fn0041AB70(u8 *p0, u8 *p1, u8 *p2, u32 p3, u8 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 p2_ = (u32)(uintptr_t)p2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b4)) = p3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b8)) = p4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x180)) = (*(u32 *)(uintptr_t)(u32)(p2_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x184)) = (*(u32 *)(uintptr_t)(u32)(p2_ + 0x4));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x188)) = (*(u32 *)(uintptr_t)(u32)(p1_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18c)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4));
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b0)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a4)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a0)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a8)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1ac)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b0)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1b0)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1a0)) = 0xffffffff;
    return;
}

// 0x0041c030: ghidra (inventoried as FUN_0041C030)
__declspec(noinline) u32 Fn0041C030(u32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 puVar1;
    i32 iVar2;
    u32 puVar3;
    (*(u32 *)(uintptr_t)(u32)(p1_)) = 0x46dab0;
    _ReadWriteBarrier();
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) & 0xfffffffe;
    puVar1 = p1_ + 0x68;
    iVar2 = 0x12;
    do {
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) & 0xfffffffe;
        puVar1 = puVar1 + 0x34;
        iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x420)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x420)) & 0xfffffffe;
    puVar3 = p1_;
    for (iVar2 = 0x109; iVar2 != 0; iVar2 = iVar2 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = 0;
        puVar3 = puVar3 + 0x4;
    }
    if (((*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p1_ + 0x14)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p1_ + 0x10)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p1_ + 0x18)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p1_ + 0x1c)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x20)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x14)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x18)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p1_ + 0x10)) = 0xffffffff;
    return p1_;
}

// 0x00421c00: ghidra (inventoried as FUN_00421C00)
__declspec(noinline) u32 Fn00421C00(u8 *p0, u32 p1, i32 p2, i32 p3, i32 p4, i32 p5, i32 p6, i32 p7, i32 p8, i32 p9, i32 p10)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    if (-1 < (*(i32 *)(uintptr_t)(u32)(p0_))) {
        return 0xffffffff;
    }
    (*(i32 *)(uintptr_t)(u32)(p0_)) = p3;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x2c)) = p2;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x30)) = p4;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x34)) = p5;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x38)) = p6;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x3c)) = p7;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x40)) = p8;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x44)) = p9;
    (*(i32 *)(uintptr_t)(u32)(p0_ + 0x48)) = p10;
    return 0;
}

// 0x00434a80: ghidra (inventoried as FUN_00434A80)
__declspec(noinline) void Fn00434A80(u8 *p0, u8 *p1, u8 *p2, u32 p3, u8 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 p2_ = (u32)(uintptr_t)p2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xb4)) = p3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x88)) = (*(u32 *)LEAF_GLOB(0x491c14u));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x8c)) = (*(u32 *)LEAF_GLOB(0x491c18u));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x90)) = (*(u32 *)LEAF_GLOB(0x491c1cu));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x94)) = (*(u32 *)LEAF_GLOB(0x491c14u));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x98)) = (*(u32 *)LEAF_GLOB(0x491c18u));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x9c)) = (*(u32 *)LEAF_GLOB(0x491c1cu));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xb8)) = p4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x70)) = (*(u32 *)(uintptr_t)(u32)(p2_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x74)) = (*(u32 *)(uintptr_t)(u32)(p2_ + 0x4));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x78)) = (*(u32 *)(uintptr_t)(u32)(p2_ + 0x8));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x7c)) = (*(u32 *)(uintptr_t)(u32)(p1_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x80)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x84)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x8));
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0xb0)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xa4)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xa0)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xa8)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xac)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xb0)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0xb0)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xa4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xa8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xa0)) = 0xffffffff;
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0041a0a0: ghidra (inventoried as FUN_0041A0A0)
__declspec(noinline) void Fn0041A0A0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    i8 cVar1;
    u32 pcVar2;
    i32 iVar3;
    u32 pcVar4;
    while ((((cVar1 = (*(i8 *)(uintptr_t)(u32)(p0_)), cVar1 == ' ' || (cVar1 == '\t')) || (cVar1 == '\n')) || (cVar1 == '\r'))) {
        pcVar2 = p0_;
        do {
            cVar1 = (*(i8 *)(uintptr_t)(u32)(pcVar2));
            pcVar2 = pcVar2 + 1;
        } while (cVar1 != '\0');
        iVar3 = pcVar2 - (p0_ + 1);
        if (0 < iVar3) {
            pcVar4 = p0_;
            pcVar2 = p0_;
            for (; pcVar2 = pcVar2 + 1, iVar3 != 0; iVar3 = iVar3 + -1) {
                (*(i8 *)(uintptr_t)(u32)(pcVar4)) = (*(i8 *)(uintptr_t)(u32)(pcVar2));
                pcVar4 = pcVar4 + 1;
            }
        }
    }
    pcVar2 = p0_;
    do {
        cVar1 = (*(i8 *)(uintptr_t)(u32)(pcVar2));
        pcVar2 = pcVar2 + 1;
    } while (cVar1 != '\0');
    iVar3 = pcVar2 - (p0_ + 1);
    while ((iVar3 = iVar3 + -1, -1 < iVar3 &&
    (((cVar1 = (*(i8 *)(uintptr_t)(u32)(p0_ + (iVar3) * 1)), cVar1 == ' ' || (cVar1 == '\t')) || ((cVar1 == '\n' || (cVar1 == '\r'))))))) {
        (*(i8 *)(uintptr_t)(u32)(p0_ + (iVar3) * 1)) = '\0';
    }
    return;
}

// 0x00421f80: ghidra (inventoried as FUN_00421F80)
__declspec(noinline) f64 Fn00421F80(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    return (*(f32 *)LEAF_GLOB(0x470b44u)) - ((*(f64 *)(uintptr_t)(u32)(p0_ + 0x24)) / (*(f64 *)(uintptr_t)(u32)(p0_ + 0x2c))) * (*(f32 *)LEAF_GLOB(0x470b44u));
}

// 0x0043e460: ghidra (inventoried as FUN_0043E460)
__declspec(noinline) void Fn0043E460(u8 *p0, u32 p1, u8 *p2, u32 p3, u32 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p2_ = (u32)(uintptr_t)p2;
    i8 cVar1;
    i32 iVar2;
    u32 piVar3;
    i32 iVar4;
    iVar2 = 0;
    piVar3 = p0_ + 0x1f88;
    do {
        if ((*(i32 *)(uintptr_t)(u32)(piVar3)) == 0) {
            iVar2 = iVar2 * 0x10c + p0_;
            (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1f88)) = p3;
            (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1f8c)) = p4;
            iVar4 = (iVar2 + 0x1f94) - p2_;
            do {
                cVar1 = (*(i8 *)(uintptr_t)(u32)(p2_));
                (*(i8 *)(uintptr_t)(u32)(p2_ + (iVar4) * 1)) = cVar1;
                p2_ = p2_ + 1;
            } while (cVar1 != '\0');
            (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1f90)) = 0;
            return;
        }
        iVar2 = iVar2 + 1;
        piVar3 = piVar3 + 0x10c;
    } while (iVar2 < 0x1f);
    return;
}

// 0x0043eda0: ghidra (inventoried as FUN_0043EDA0)
__declspec(noinline) u32 Fn0043EDA0(u8 *p0, u8 p1, u8 *p2, u16 p3)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p2_ = (u32)(uintptr_t)p2;
    if ((p3 & 1 << (p1 & 0x1f)) != 0) {
        switch((*(u32 *)(uintptr_t)(u32)(p0_))) {
            case 10000:
            return p2_ + 0x30c;
            case 0x2711:
            return p2_ + 0x310;
            case 0x2712:
            return p2_ + 0x314;
            case 0x2713:
            return p2_ + 0x318;
            case 0x2718:
            return p2_ + 0x32c;
            case 0x2719:
            p0_ = p2_ + 0x330;
        }
    }
    return p0_;
}

// 0x00442050: ghidra (inventoried as FUN_00442050)
__declspec(noinline) void Fn00442050(u8 *p0, u8 *p1, u8 *p2, u32 p3, u8 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 p2_ = (u32)(uintptr_t)p2;
    u8 uVar1;
    u8 uVar2;
    u8 uVar3;
    u8 uVar4;
    u8 uVar5;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x200)) = p3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1d4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1d8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1dc)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1e0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1e4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1e8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x204)) = p4;
    uVar1 = (*(u8 *)(uintptr_t)(u32)(p1_ + 0x2));
    uVar2 = (*(u8 *)(uintptr_t)(u32)(p1_ + 0x1));
    uVar3 = (*(u8 *)(uintptr_t)(u32)(p1_));
    uVar4 = (*(u8 *)(uintptr_t)(u32)(p2_ + 0x2));
    uVar5 = (*(u8 *)(uintptr_t)(u32)(p2_ + 0x1));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1bc)) = (*(u8 *)(uintptr_t)(u32)(p2_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c0)) = uVar5;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c4)) = uVar4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c8)) = uVar3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1cc)) = uVar2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1d0)) = uVar1;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1fc)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1f0)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1ec)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 500)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1f8)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1fc)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1fc)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1f0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 500)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1ec)) = 0xffffffff;
    return;
}

// 0x00442220: ghidra (inventoried as FUN_00442220)
__declspec(noinline) void Fn00442220(u8 *p0, u8 *p1, u8 *p2, u32 p3, u8 p4)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 p2_ = (u32)(uintptr_t)p2;
    u8 uVar1;
    u8 uVar2;
    u8 uVar3;
    u8 uVar4;
    u8 uVar5;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x100)) = p3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xd4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xd8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xdc)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xe0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xe4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xe8)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x104)) = p4;
    uVar1 = (*(u8 *)(uintptr_t)(u32)(p1_ + 0x2));
    uVar2 = (*(u8 *)(uintptr_t)(u32)(p1_ + 0x1));
    uVar3 = (*(u8 *)(uintptr_t)(u32)(p1_));
    uVar4 = (*(u8 *)(uintptr_t)(u32)(p2_ + 0x2));
    uVar5 = (*(u8 *)(uintptr_t)(u32)(p2_ + 0x1));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xbc)) = (*(u8 *)(uintptr_t)(u32)(p2_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc0)) = uVar5;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc4)) = uVar4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 200)) = uVar3;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xcc)) = uVar2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xd0)) = uVar1;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0xfc)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xf0)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xec)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xf4)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xf8)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0xfc)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0xfc)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xf0)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xf4)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xec)) = 0xffffffff;
    return;
}

// 0x00447790: ghidra (inventoried as FUN_00447790)
__declspec(noinline) u32 Fn00447790(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    i32 iVar1;
    u32 piVar2;
    u32 uVar3;
    uVar3 = 0;
    piVar2 = p0_ + 0x3ad06c;
    while ((iVar1 = (*(i32 *)(uintptr_t)(u32)(piVar2)), iVar1 == 0 || (((*(u32 *)(uintptr_t)(u32)(iVar1 + 0x128)) == 0 && ((*(u32 *)(uintptr_t)(u32)(iVar1 + 0x124)) == 0))))) {
        uVar3 = uVar3 + 1;
        piVar2 = piVar2 + 0x4;
        if (0x20 < uVar3) {
            return 1;
        }
    }
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004051f0: ghidra (inventoried as FUN_004051F0)
__declspec(noinline) void Fn004051F0(u8 *p0, u8 *p1, f32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    f32 fVar1;
    f32 fVar2;
    p2 = (*(f32 *)LEAF_GLOB(0x470afcu)) / p2;
    fVar1 = ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x8)));
    fVar2 = ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x4)));
    (*(f32 *)(uintptr_t)(u32)(p0_)) = p2 * ((f64)(*(f32 *)(uintptr_t)(u32)(p1_)));
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) = p2 * fVar2;
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x8)) = p2 * fVar1;
    return;
}

// 0x00408660: ghidra (inventoried as FUN_00408660)
__declspec(noinline) f64 Fn00408660(f32 p0, f32 p1)
{
    f64 fVar1;
    // x87 computes in 80-bit extended precision; the C++ must use double
    // arithmetic (plain `p0 - p1` stays float and rounds twice). Double is
    // what the verifier's interpreter computes, and the closest C++ can get
    // to the original's 80-bit result.
    fVar1 = (f64)p0 - (f64)p1;
    if ((f64)(*(f32 *)LEAF_GLOB(0x470b18u)) < fVar1) {
        return (f64)p0 - ((f64)p1 + (f64)(*(f32 *)LEAF_GLOB(0x470b14u)));
    }
    if ((f64)(*(f32 *)LEAF_GLOB(0x470b18u)) < (f64)p1 - (f64)p0) {
        fVar1 = (f64)p0 - ((f64)p1 - (f64)(*(f32 *)LEAF_GLOB(0x470b14u)));
    }
    return fVar1;
}

// 0x00428ce0: ghidra (inventoried as FUN_00428CE0)
__declspec(noinline) f64 Fn00428CE0(f32 p0, f32 p1)
{
    f64 fVar1;
    // x87 computes in 80-bit extended precision; the C++ must use double
    // arithmetic (plain `p0 - p1` stays float and rounds twice). Double is
    // what the verifier's interpreter computes, and the closest C++ can get
    // to the original's 80-bit result.
    fVar1 = (f64)p0 - (f64)p1;
    if ((f64)(*(f32 *)LEAF_GLOB(0x470b18u)) < fVar1) {
        return (f64)p0 - ((f64)p1 + (f64)(*(f32 *)LEAF_GLOB(0x470b14u)));
    }
    if ((f64)(*(f32 *)LEAF_GLOB(0x470b18u)) < (f64)p1 - (f64)p0) {
        fVar1 = (f64)p0 - ((f64)p1 - (f64)(*(f32 *)LEAF_GLOB(0x470b14u)));
    }
    return fVar1;
}

// 0x0044bb20: ghidra (inventoried as FUN_0044BB20)
__declspec(noinline) f64 Fn0044BB20(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u16 uVar1;
    u16 uVar2;
    u32 uVar3;
    i32 iVar4;
    f64 fVar5;
    uVar3 = ((*(u16 *)(uintptr_t)(u32)(p0_)) ^ 0x9630) - 0x6553;
    iVar4 = (uVar3 >> 0xe & 3) + uVar3 * 4;
    uVar1 = iVar4;
    uVar2 = (uVar1 ^ 0x9630) + 0x9aad;
    (*(u16 *)(uintptr_t)(u32)(p0_)) = uVar1;
    uVar2 = uVar2 * 4 + (uVar2 >> 0xe);
    (*(u16 *)(uintptr_t)(u32)(p0_)) = uVar2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) + 2;
    uVar3 = iVar4 * 0x10000 | uVar2;
    fVar5 = (i32)uVar3;
    if (((i32)uVar3 < 0)) {
        fVar5 = fVar5 + (*(f32 *)LEAF_GLOB(0x470b98u));
    }
    return fVar5 * (*(f32 *)LEAF_GLOB(0x470bf0u));
}

// 0x0044bb90: ghidra (inventoried as FUN_0044BB90)
__declspec(noinline) f64 Fn0044BB90(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u16 uVar1;
    u16 uVar2;
    u32 uVar3;
    i32 iVar4;
    f64 fVar5;
    uVar3 = ((*(u16 *)(uintptr_t)(u32)(p0_)) ^ 0x9630) - 0x6553;
    iVar4 = (uVar3 >> 0xe & 3) + uVar3 * 4;
    uVar1 = iVar4;
    uVar2 = (uVar1 ^ 0x9630) + 0x9aad;
    (*(u16 *)(uintptr_t)(u32)(p0_)) = uVar1;
    uVar2 = uVar2 * 4 + (uVar2 >> 0xe);
    (*(u16 *)(uintptr_t)(u32)(p0_)) = uVar2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x4)) + 2;
    uVar3 = iVar4 * 0x10000 | uVar2;
    fVar5 = (i32)uVar3;
    if (((i32)uVar3 < 0)) {
        fVar5 = fVar5 + (*(f32 *)LEAF_GLOB(0x470b98u));
    }
    return fVar5 * (*(f32 *)LEAF_GLOB(0x470becu)) - (*(f32 *)LEAF_GLOB(0x470afcu));
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0040ad20: ghidra (inventoried as FUN_0040AD20)
__declspec(noinline) u32 Fn0040AD20(u32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 uVar1;
    /* regs: p0=ecx p1=edx */
    uVar1 = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x8));
    if (uVar1 == 0) {
        (*(u32 *)(uintptr_t)(u32)(p1_)) = (i32)p0;
        return (i32)p0;
    }
    if ((i32)uVar1 <= (i32)p0) {
        (*(u32 *)(uintptr_t)(u32)(p1_)) = uVar1 - 1;
        return uVar1 - 1;
    }
    p0 = (((i32)p0 < 0)) - 1 & p0;
    (*(u32 *)(uintptr_t)(u32)(p1_)) = (i32)p0;
    return (i32)p0;
}

// 0x004127a0: ghidra (inventoried as FUN_004127A0)
__declspec(noinline) u32 Fn004127A0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 puVar1;
    i32 iVar2;
    u32 uVar3;
    u32 piVar4;
    i32 iVar5;
    /* regs: p0=ecx */
    uVar3 = 0;
    piVar4 = p0_ + 0x2494;
    do {
        if (-1 < (*(i32 *)(uintptr_t)(u32)(piVar4))) {
            iVar2 = uVar3 * 0x10 + p0_;
            (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2404)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x23fc)) - (*(u32 *)(uintptr_t)(u32)(uVar3 * 0x10 + 0x2494 + p0_));
            if ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x23fc)) <= (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2494))) {
                (*(u32 *)(uintptr_t)(u32)(p0_ + 0x23fc)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2494));
                (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2494)) = 0xffffffff;
                if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1168)) & 1) == 0) {
                    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x115c)) = 0;
                    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1158)) = 0xfff0bdc1;
                    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1160)) = 0;
                    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1164)) = 0x476f78;
                    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1168)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1168)) | 1;
                }
                (*(u32 *)(uintptr_t)(u32)(p0_ + 0x115c)) = 0;
                (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1160)) = 0;
                (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1158)) = 0xffffffff;
                (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2480)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2480)) & 0xfffeffff;
                return (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x249c));
            }
            break;
        }
        uVar3 = uVar3 + 1;
        piVar4 = piVar4 + 0x10;
    } while (uVar3 < 8);
    uVar3 = 0;
    piVar4 = p0_ + 0x2498;
    while (((*(u32 *)(uintptr_t)(u32)(piVar4 + -0x4)) < 0 || ((*(i32 *)(uintptr_t)(u32)(piVar4)) < 1))) {
        uVar3 = uVar3 + 1;
        piVar4 = piVar4 + 0x10;
        if (7 < uVar3) {
            return 0;
        }
    }
    iVar2 = uVar3 * 0x10 + p0_;
    iVar5 = (i32)(((*(u32 *)(uintptr_t)(u32)(uVar3 * 0x10 + 0x2498 + p0_)) - (*(u32 *)(uintptr_t)(u32)(p0_ + 0x115c))) + 0x3b) / 0x3c;
    if (99 < iVar5) {
        iVar5 = 99;
    }
    (*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x47770cu)) + 0x9ec0)) = iVar5;
    if ((*(u32 *)(uintptr_t)(u32)(p0_ + 0x115c)) < (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2498))) {
        return 0;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x23fc)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2494));
    (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2494)) = 0xffffffff;
    if (((*(u32 *)(uintptr_t)(u32)(p0_ + 0x1168)) & 1) == 0) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x115c)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1158)) = 0xfff0bdc1;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1160)) = 0;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1164)) = 0x476f78;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1168)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1168)) | 1;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x115c)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1160)) = 0;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1158)) = 0xffffffff;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2480)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x2480)) | 0x10000;
    iVar2 = (*(u32 *)LEAF_GLOB(0x4776f4u));
    (*(u32 *)LEAF_GLOB(0x474c4cu)) = (*(u32 *)LEAF_GLOB(0x474c4cu)) + -3000;
    if ((*(u32 *)LEAF_GLOB(0x474c4cu)) < 5000) {
        (*(u32 *)LEAF_GLOB(0x474c4cu)) = 5000;
    }
    puVar1 = (*(u32 *)LEAF_GLOB(0x4776f4u)) + 0x378c;
    if ((((*(u32 *)(uintptr_t)(u32)(puVar1)) & 8) == 0) && (0x3b < (*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776f4u)) + 0x3738)))) {
        (*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776f4u)) + 0x3790)) = 0;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x378c)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0xad4)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0xad4)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0xe80)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0xe80)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x15d8)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x15d8)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1984)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1984)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1d30)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x1d30)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x20dc)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x20dc)) & 0xfffffffd;
        (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2488)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x2488)) & 0xfffffffd;
    }
    return (*(u32 *)(uintptr_t)(u32)((uVar3 + 0x24a) * 0x10 + p0_));
}

// 0x0041fd90: ghidra (inventoried as FUN_0041FD90)
__declspec(noinline) u32 Fn0041FD90(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 puVar1;
    i32 iVar2;
    /* regs: p0=edx */
    iVar2 = (*(u32 *)LEAF_GLOB(0x4776e0u));
    if (((*(u32 *)(uintptr_t)(u32)(p0_)) & 2) != 0) {
        puVar1 = (*(u32 *)(uintptr_t)(u32)((*(u32 *)LEAF_GLOB(0x4776e0u)) + 0xc)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
        puVar1 = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x10)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
        puVar1 = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x89a8)) + 4;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = (*(u32 *)(uintptr_t)(u32)(puVar1)) | 2;
        (*(u32 *)LEAF_GLOB(0x491ff4u)) = (*(u32 *)LEAF_GLOB(0x491ff4u)) & 0xffffefff;
        (*(u32 *)LEAF_GLOB(0x491fb8u)) = 4;
        (*(u32 *)(uintptr_t)(u32)(p0_)) = (*(u32 *)(uintptr_t)(u32)(p0_)) & 0xfffffffd;
    }
    return 1;
}

// 0x00424530: ghidra (inventoried as FUN_00424530)
__declspec(noinline) u32 Fn00424530(u8 *p0, u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7, u32 p8, u32 p9, u32 p10)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    /* regs: p0=eax p1=edx */
    if (-1 < (*(i32 *)(uintptr_t)(u32)(p0_ + 4))) {
        return 0xffffffff;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 4)) = (i32)p2;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 8)) = (i32)p1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) = (i32)p4;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x10)) = (i32)p5;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x14)) = (i32)p6;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x18)) = (i32)p7;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1c)) = (i32)p8;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x20)) = (i32)p9;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x24)) = (i32)p10;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x28)) = (i32)p3;
    return 0;
}

// 0x00427ce0: ghidra (inventoried as FUN_00427CE0)
__declspec(noinline) void Fn00427CE0(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    /* regs: p0=eax p1=ecx */
    (*(f32 *)(uintptr_t)(u32)(p0_)) = (*(i32 *)(uintptr_t)(u32)(p1_)) * ((f64)(*(f32 *)LEAF_GLOB(0x470b00u)));
    iVar1 = (*(i32 *)(uintptr_t)(u32)(p1_ + 0x4));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x8)) = 0.0;
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x4)) = iVar1 * ((f64)(*(f32 *)LEAF_GLOB(0x470b00u)));
    return;
}

// 0x00428e10: ghidra (inventoried as FUN_00428E10)
__declspec(noinline) void Fn00428E10(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    /* regs: p0=eax p1=ecx */
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3cc)) = (*(u32 *)(uintptr_t)(u32)(p1_));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3d0)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4));
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x3c0)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3cc)) * ((f64)(*(f32 *)LEAF_GLOB(0x470b00u)));
    (*(f32 *)(uintptr_t)(u32)(p0_ + 0x3c4)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3d0)) * ((f64)(*(f32 *)LEAF_GLOB(0x470b00u)));
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x332c)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x33c4)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x345c)) = 1;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x34f4)) = 1;
    return;
}

// 0x00436260: ghidra (inventoried as FUN_00436260)
__declspec(noinline) void Fn00436260(i32 p0, i32 p1)
{
    i32 iVar1;
    /* regs: p0=edx p1=esi */
    (*(u32 *)(uintptr_t)(u32)(p0 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = (*(u32 *)(uintptr_t)(u32)(p1 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u))));
    iVar1 = (*(u32 *)(uintptr_t)(u32)(p1 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) * 0xc;
    if ((*(u32 *)(uintptr_t)(u32)(iVar1 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u)))) == p1) {
        (*(u32 *)(uintptr_t)(u32)(iVar1 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u)))) = p0;
        (*(u32 *)(uintptr_t)(u32)(p1 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = 0;
        return;
    }
    (*(u32 *)(uintptr_t)(u32)(iVar1 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu)))) = p0;
    (*(u32 *)(uintptr_t)(u32)(p1 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = 0;
    return;
}

// 0x00436460: ghidra (inventoried as FUN_00436460)
__declspec(noinline) u32 Fn00436460(u8 *p0, u8 *p1, u32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 uVar1;
    u32 uVar2;
    u32 puVar3;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 8));
    uVar1 = ((*(u32 *)(uintptr_t)(u32)(p0_ + 0xc)) + (*(u32 *)(uintptr_t)(u32)(p0_ + 4))) - puVar3;
    if (p2 <= uVar1) {
        for (uVar1 = p2 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
            (*(u32 *)(uintptr_t)(u32)(p1_)) = (*(u32 *)(uintptr_t)(u32)(puVar3));
            puVar3 = puVar3 + 0x4;
            p1_ = p1_ + 0x4;
        }
        for (uVar1 = p2 & 3; uVar1 != 0; uVar1 = uVar1 - 1) {
            (*(u8 *)(uintptr_t)(u32)(p1_)) = (*(u8 *)(uintptr_t)(u32)(puVar3));
            puVar3 = puVar3 + 0x1;
            p1_ = p1_ + 0x1;
        }
        (*(u32 *)(uintptr_t)(u32)(p0_ + 8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 8)) + p2;
        return (i32)p2;
    }
    if (uVar1 != 0) {
        for (uVar2 = uVar1 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
            (*(u32 *)(uintptr_t)(u32)(p1_)) = (*(u32 *)(uintptr_t)(u32)(puVar3));
            puVar3 = puVar3 + 0x4;
            p1_ = p1_ + 0x4;
        }
        for (uVar2 = uVar1 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
            (*(u8 *)(uintptr_t)(u32)(p1_)) = (*(u8 *)(uintptr_t)(u32)(puVar3));
            puVar3 = puVar3 + 0x1;
            p1_ = p1_ + 0x1;
        }
        (*(u32 *)(uintptr_t)(u32)(p0_ + 8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 8)) + uVar1;
        return (i32)uVar1;
    }
    return 0;
}

// 0x00449770: ghidra (inventoried as FUN_00449770)
__declspec(noinline) void Fn00449770(u8 *p0, u8 *p1, i32 p2)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 piVar1;
    /* regs: p0=eax p1=ecx p2=edx */
    piVar1 = p1_ + 0x10;
    while( true ) {
        if (piVar1 == NULL) {
            (*(u32 *)(uintptr_t)(u32)(p0_)) = 0;
            return;
        }
        if ((*(i16 *)(uintptr_t)(u32)((*(i32 *)(uintptr_t)(u32)(piVar1)) + 0x38a)) == p2) break;
        piVar1 = (*(u32 *)(uintptr_t)(u32)(piVar1 + 0x4));
    }
    (*(u32 *)(uintptr_t)(u32)(p0_)) = (*(u32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(piVar1))));
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00441ec0: ghidra (inventoried as FUN_00441EC0)
__declspec(noinline) u8 Fn00441EC0(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    /* regs: p0=eax p1=ecx */
    if ((((*(i32 *)(uintptr_t)(u32)(p0_ + 0x394)) != NULL) && (iVar1 = (*(i32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p0_ + 0x394)))), -1 < iVar1)) &&
    (iVar1 = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x3ad06c + iVar1 * 4)), iVar1 != 0)) {
        return (*(u32 *)(uintptr_t)(u32)(iVar1 + 0x120)) != 0;
    }
    return false;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00427b50: ghidra (inventoried as FUN_00427B50)
__declspec(noinline) u32 Fn00427B50(u8 *p0, u8 *p1, u32 p2, u32 p3, i32 p4, u32 p5)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    u32 puVar2;
    u32 puVar3;
    /* regs: p0=edx */
    puVar2 = p0_ + 0x350c;
    iVar1 = 0;
    do {
        if (((*(u8 *)(uintptr_t)(u32)(puVar2 + 0x68)) & 1) == 0) {
            puVar3 = puVar2;
            for (iVar1 = 0x1b; iVar1 != 0; iVar1 = iVar1 + -1) {
                (*(u32 *)(uintptr_t)(u32)(puVar3)) = 0;
                puVar3 = puVar3 + 0x4;
            }
            (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x68)) = (*(u8 *)(uintptr_t)(u32)(puVar2 + 0x68)) | 3;
            puVar3 = puVar2 + 0x18;
            for (iVar1 = 0xb; iVar1 != 0; iVar1 = iVar1 + -1) {
                (*(u32 *)(uintptr_t)(u32)(puVar3)) = 0;
                puVar3 = puVar3 + 0x4;
            }
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x18)) = (*(u32 *)(uintptr_t)(u32)(p1_));
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x1c)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x4));
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x20)) = (*(u32 *)(uintptr_t)(u32)(p1_ + 0x8));
            (*(u32 *)(uintptr_t)(u32)(puVar2)) = (i32)p2;
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x4)) = (i32)p3;
            if (((*(u32 *)(uintptr_t)(u32)(puVar2 + 0x54)) & 1) == 0) {
                (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x48)) = 0;
                (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x44)) = 0xfff0bdc1;
                (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x4c)) = 0;
                (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x50)) = 0x476f78;
                (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x54)) = (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x54)) | 1;
            }
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x48)) = p4;
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x44)) = p4 + -1;
            (*(f32 *)(uintptr_t)(u32)(puVar2 + 0x4c)) = (f32)(p4);
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x58)) = (i32)p5;
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x5c)) = 0;
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x60)) = 999999;
            (*(u32 *)(uintptr_t)(u32)(puVar2 + 0x64)) = 4;
            return puVar2;
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 0x6c;
    } while (iVar1 < 0x20);
    return puVar2;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x004506d0: ghidra (inventoried as FUN_004506D0)
__declspec(noinline) u32 Fn004506D0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 uVar1;
    i32 iVar2;
    /* regs: p0=eax */
    iVar2 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) + -4;
    uVar1 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1004));
    if (-1 < iVar2) {
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) = iVar2;
        (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1004)) = (*(u32 *)(uintptr_t)(u32)(iVar2 + p0_));
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x1000)) = (i32)uVar1;
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00417010: ghidra (inventoried as FUN_00417010)
__declspec(noinline) u8 * Fn00417010(u8 *p0)
{
    u8 key = 0x77;
    u8 increment = 7;
    u8 *output = LEAF_GLOB(0x497d40u);
    u8 decoded;
    do {
        decoded = static_cast<u8>(key ^ *p0);
        *output++ = decoded;
        _ReadWriteBarrier();
        key = static_cast<u8>(key + increment);
        ++p0;
        increment = static_cast<u8>(increment + 0x10);
    } while (decoded != 0);
    return LEAF_GLOB(0x497d40u);
}

// 0x00435fd0: ghidra (inventoried as FUN_00435FD0)
__declspec(noinline) void Fn00435FD0(void)
{
    u32 puVar1;
    i32 iVar2;
    puVar1 = ((u32)(uintptr_t)LEAF_GLOB(0x48f868u));
    for (iVar2 = 0x800; iVar2 != 0; iVar2 = iVar2 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = 0;
        puVar1 = puVar1 + 0x4;
    }
    puVar1 = ((u32)(uintptr_t)LEAF_GLOB(0x47785cu));
    do {
        (*(u32 *)(uintptr_t)(u32)(puVar1 + -0x4)) = 0;
        (*(u32 *)(uintptr_t)(u32)(puVar1)) = 0;
        (*(u32 *)(uintptr_t)(u32)(puVar1 + 0x4)) = 0;
        puVar1 = puVar1 + 0xc;
    } while (puVar1 < ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)));
    return;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x0041aad0: ghidra (inventoried as FUN_0041AAD0)
__declspec(noinline) void Fn0041AAD0(u8 *p0)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 piVar1;
    i32 iVar2;
    i32 iVar3;
    u32 piVar4;
    /* regs: p0=ecx */
    piVar4 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x7c + (*(u32 *)LEAF_GLOB(0x474c7cu)) * 0xc));
    while (piVar4 != NULL) {
        piVar1 = (*(u32 *)(uintptr_t)(u32)(piVar4 + 0x4));
        iVar2 = (*(i32 *)(uintptr_t)(u32)(piVar4));
        iVar3 = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x70));
        piVar4 = piVar1;
        if (0 < iVar3) {
            (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x70)) = iVar3 + -1;
        }
    }
    return;
}

// 0x004497b0: ghidra (inventoried as FUN_004497B0)
__declspec(noinline) i32 Fn004497B0(u8 *p0, i32 p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 piVar1;
    /* regs: p0=eax p1=ecx */
    piVar1 = p0_ + 0x10;
    while( true ) {
        if (piVar1 == NULL) {
            return 0;
        }
        if ((*(i16 *)(uintptr_t)(u32)((*(i32 *)(uintptr_t)(u32)(piVar1)) + 0x38a)) == p1) break;
        piVar1 = (*(u32 *)(uintptr_t)(u32)(piVar1 + 0x4));
    }
    return (*(i32 *)(uintptr_t)(u32)(piVar1));
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00428c20: ghidra (inventoried as FUN_00428C20)
__declspec(noinline) u32 Fn00428C20(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    u32 pfVar1;
    f32 fVar2;
    i32 iVar3;
    /* regs: p0=ecx p1=edx */
    iVar3 = (*(i8 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p1_ + 0x58)) + 0x1c)) * 0x98;
    fVar2 = ((f64)(*(i32 *)(uintptr_t)(u32)(iVar3 + 0x3248 + p0_))) * ((f64)(*(f32 *)LEAF_GLOB(0x470b00u)));
    pfVar1 = p1_ + 0x14;
    (*(f32 *)(uintptr_t)(u32)(pfVar1)) = (*(i32 *)(uintptr_t)(u32)(iVar3 + 0x3244 + p0_)) * ((f64)(*(f32 *)LEAF_GLOB(0x470b00u)));
    (*(f32 *)(uintptr_t)(u32)(p1_ + 0x18)) = fVar2;
    (*(i8 *)(uintptr_t)(u32)(p1_ + 0x1c)) = 0;
    (*(f32 *)(uintptr_t)(u32)(pfVar1)) = ((*(f32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p1_ + 0x58)) + 4)) - ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x20)))) + ((f64)(*(f32 *)(uintptr_t)(u32)(pfVar1)));
    (*(f32 *)(uintptr_t)(u32)(p1_ + 0x18)) = ((*(f32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(p1_ + 0x58)) + 8)) - ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x24)))) + ((f64)(*(f32 *)(uintptr_t)(u32)(p1_ + 0x18)));
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00436000: ghidra (inventoried as FUN_00436000)
__declspec(noinline) i32 Fn00436000(i32 p0, u8 *p1)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    u32 piVar2;
    i32 iVar3;
    i32 iVar4;
    i32 iVar5;
    /* regs: p0=edx */
    if (p0 == 0) {
        return 0;
    }
    iVar5 = 0;
    iVar4 = (*(u32 *)LEAF_GLOB(0x48f860u));
    do {
        iVar1 = 0;
        do {
            iVar3 = (*(u32 *)(uintptr_t)(u32)((iVar1 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)))) -
            (*(u32 *)(uintptr_t)(u32)(((iVar4 - p0) + iVar1 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u))));
            if (iVar3 != 0) break;
            iVar3 = (*(u32 *)(uintptr_t)(u32)((iVar1 + 1 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)))) - (*(u32 *)(uintptr_t)(u32)((iVar1 + 1 + iVar4 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u))));
            if (iVar3 != 0) {
                iVar1 = iVar1 + 1;
                break;
            }
            iVar3 = (*(u32 *)(uintptr_t)(u32)((iVar1 + 2 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)))) - (*(u32 *)(uintptr_t)(u32)((iVar1 + 2 + iVar4 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u))));
            if (iVar3 != 0) {
                iVar1 = iVar1 + 2;
                break;
            }
            iVar3 = (*(u32 *)(uintptr_t)(u32)((iVar1 + 3 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)))) - (*(u32 *)(uintptr_t)(u32)((iVar1 + 3 + iVar4 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u))));
            if (iVar3 != 0) {
                iVar1 = iVar1 + 3;
                break;
            }
            iVar3 = (*(u32 *)(uintptr_t)(u32)((iVar1 + 4 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)))) - (*(u32 *)(uintptr_t)(u32)((iVar1 + 4 + iVar4 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u))));
            if (iVar3 != 0) {
                iVar1 = iVar1 + 4;
                break;
            }
            iVar3 = (*(u32 *)(uintptr_t)(u32)((iVar1 + 5 + p0 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u)))) - (*(u32 *)(uintptr_t)(u32)((iVar1 + 5 + iVar4 & 0x1fffU) + ((u32)(uintptr_t)LEAF_GLOB(0x48f868u))));
            if (iVar3 != 0) {
                iVar1 = iVar1 + 5;
                break;
            }
            iVar1 = iVar1 + 6;
        } while (iVar1 < 0x12);
        if ((iVar5 <= iVar1) && ((*(i32 *)(uintptr_t)(u32)(p1_)) = iVar4, iVar5 = iVar1, 0x11 < iVar1)) {
            iVar5 = iVar4 * 0xc;
            piVar2 = iVar5 + ((u32)(uintptr_t)LEAF_GLOB(0x477858u));
            iVar3 = (*(i32 *)(uintptr_t)(u32)(piVar2)) * 0xc;
            if ((*(u32 *)(uintptr_t)(u32)(iVar3 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu)))) == iVar4) {
                (*(u32 *)(uintptr_t)(u32)(iVar3 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu)))) = p0;
            }
            else {
                (*(u32 *)(uintptr_t)(u32)(iVar3 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u)))) = p0;
            }
            iVar4 = p0 * 0xc;
            (*(u32 *)(uintptr_t)(u32)(iVar4 + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = (*(i32 *)(uintptr_t)(u32)(piVar2));
            (*(u32 *)(uintptr_t)(u32)(iVar4 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu)))) = (*(u32 *)(uintptr_t)(u32)(iVar5 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu))));
            (*(u32 *)(uintptr_t)(u32)(iVar4 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u)))) = (*(u32 *)(uintptr_t)(u32)(iVar5 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u))));
            (*(u32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(iVar4 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu)))) * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = p0;
            (*(u32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(iVar4 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u)))) * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = p0;
            (*(i32 *)(uintptr_t)(u32)(piVar2)) = 0;
            return iVar1;
        }
        if (iVar3 < 0) {
            piVar2 = iVar4 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu));
        }
        else {
            piVar2 = iVar4 * 0xc + ((u32)(uintptr_t)LEAF_GLOB(0x477860u));
        }
        if ((*(i32 *)(uintptr_t)(u32)(piVar2)) == 0) {
            (*(i32 *)(uintptr_t)(u32)(piVar2)) = p0;
            p0 = p0 * 0xc;
            (*(u32 *)(uintptr_t)(u32)(p0 + ((u32)(uintptr_t)LEAF_GLOB(0x477858u)))) = iVar4;
            (*(u32 *)(uintptr_t)(u32)(p0 + ((u32)(uintptr_t)LEAF_GLOB(0x477860u)))) = 0;
            (*(u32 *)(uintptr_t)(u32)(p0 + ((u32)(uintptr_t)LEAF_GLOB(0x47785cu)))) = 0;
            return iVar5;
        }
        iVar4 = (*(i32 *)(uintptr_t)(u32)(piVar2));
    } while( true );
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00442fe0: ghidra (inventoried as FUN_00442FE0)
__declspec(noinline) u32 Fn00442FE0(u8 *p0, u8 *p1)
{
    u32 p0_ = (u32)(uintptr_t)p0;
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    u32 puVar2;
    u32 puVar3;
    /* regs: p0=eax p1=edx */
    puVar2 = p1_;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc));
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = (*(u32 *)(uintptr_t)(u32)(puVar2));
        puVar2 = puVar2 + 0x4;
        puVar3 = puVar3 + 0x4;
    }
    puVar2 = p1_ + 0x1c;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) + 0x1c;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = (*(u32 *)(uintptr_t)(u32)(puVar2));
        puVar2 = puVar2 + 0x4;
        puVar3 = puVar3 + 0x4;
    }
    puVar2 = p1_ + 0x38;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) + 0x38;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = (*(u32 *)(uintptr_t)(u32)(puVar2));
        puVar2 = puVar2 + 0x4;
        puVar3 = puVar3 + 0x4;
    }
    puVar2 = p1_ + 0x1c;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) + 0x54;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = (*(u32 *)(uintptr_t)(u32)(puVar2));
        puVar2 = puVar2 + 0x4;
        puVar3 = puVar3 + 0x4;
    }
    puVar2 = p1_ + 0x38;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) + 0x70;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = (*(u32 *)(uintptr_t)(u32)(puVar2));
        puVar2 = puVar2 + 0x4;
        puVar3 = puVar3 + 0x4;
    }
    puVar2 = p1_ + 0x54;
    puVar3 = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) + 0x8c;
    for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        (*(u32 *)(uintptr_t)(u32)(puVar3)) = (*(u32 *)(uintptr_t)(u32)(puVar2));
        puVar2 = puVar2 + 0x4;
        puVar3 = puVar3 + 0x4;
    }
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x72dacc)) + 0xa8;
    (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3adac8)) = (*(u32 *)(uintptr_t)(u32)(p0_ + 0x3adac8)) + 1;
    return 0;
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00436cf0: ghidra (inventoried as FUN_00436CF0)
__declspec(noinline) i32 Fn00436CF0(i32 p0)
{
    i32 iVar1;
    i32 iVar2;
    /* regs: p0=edx */
    iVar1 = 0;
    if ((*(u32 *)LEAF_GLOB(0x474918u)) != -1) {
        iVar2 = (*(u32 *)LEAF_GLOB(0x474918u));
        do {
            if (iVar2 == p0) break;
            iVar1 = iVar1 + 1;
            iVar2 = (*(u32 *)(uintptr_t)(u32)(iVar1 * 0x18 + ((u32)(uintptr_t)LEAF_GLOB(0x474918u))));
        } while (iVar2 != -1);
    }
    if (p0 == -1) {
        return 0;
    }
    return iVar1 * 0x18 + ((u32)(uintptr_t)LEAF_GLOB(0x474918u));
}


// ---------------------------------------------------------------------------
// Recovered with the Ghidra decompiler (scripts/ghidra_decompile.py)
// and translated by scripts/ghidra_to_leaf.py. Every function below
// was executed against the original instruction bytes out of
// resources/th10.exe and matched on every randomized trial.
// ---------------------------------------------------------------------------

// 0x00441520: ghidra (inventoried as FUN_00441520)
__declspec(noinline) void Fn00441520(i32 p0, u8 *p1, u16 p2)
{
    u32 p1_ = (u32)(uintptr_t)p1;
    i32 iVar1;
    i32 iVar2;
    u32 piVar3;
    /* regs: p0=eax */
    iVar2 = (*(u32 *)LEAF_GLOB(0x491c10u));
    if (p0 != 0) {
        piVar3 = p1_ + 0x394;
        do {
            if (((((*(i32 *)(uintptr_t)(u32)(piVar3)) != NULL) && (iVar1 = (*(i32 *)(uintptr_t)(u32)((*(u32 *)(uintptr_t)(u32)(piVar3)))), -1 < iVar1)) &&
            (iVar1 = (*(u32 *)(uintptr_t)(u32)(iVar2 + 0x3ad06c + iVar1 * 4)), iVar1 != 0)) && ((*(u32 *)(uintptr_t)(u32)(iVar1 + 0x120)) != 0)) {
                (*(u16 *)(uintptr_t)(u32)(piVar3 - 0x90)) = p2;
            }
            piVar3 = piVar3 + 0x3ac;
            p0 = p0 + -1;
        } while (p0 != 0);
    }
    return;
}

} // namespace leaf
} // namespace th10
