#include "Main.hpp"
#include "AnmManager.hpp"
#include "AnmLoadedStartScript.hpp"

#pragma code_seg(".text$00404F30_fn")
__declspec(naked) void __stdcall EntityStartScript_00404f30(
    AnmLoadedView *resource)
{
    __asm
    {
        sub esp, 0ch
        push ebx
        mov edx, esi
        mov ebx, eax
        call AnmVmInitialize

        mov dword ptr [esp+4], 0
        mov ecx, [esp+4]
        lea eax, [esi+340h]
        mov [eax], ecx
        mov dword ptr [esp+8], 0
        mov edx, [esp+8]
        mov [eax+4], edx
        mov dword ptr [esp+0ch], 0
        mov ecx, [esp+0ch]
        mov [eax+8], ecx

        mov dword ptr [esp+4], 0
        mov eax, [esp+4]
        lea edx, [esi+334h]
        mov [edx], eax
        mov dword ptr [esp+8], 0
        mov ecx, [esp+8]
        mov [edx+4], ecx
        mov dword ptr [esp+0ch], 0
        mov eax, [esp+0ch]
        mov [edx+8], eax

        mov dword ptr [esp+4], 0
        mov edx, [esp+4]
        lea ecx, [esi+34ch]
        mov [ecx], edx
        mov dword ptr [esp+8], 0
        mov eax, [esp+8]
        mov [ecx+4], eax
        mov al, 10h
        mov dword ptr [esp+0ch], 0
        mov edx, [esp+0ch]
        mov [ecx+8], edx

        mov ecx, [esp+14h]
        mov [esi+3a1h], al
        mov [esi+3a0h], al
        mov eax, esi
        mov [esi+38ah], bx
        call LoadScript_0043e710
        pop ebx
        add esp, 0ch
        ret 4
    }
}
__declspec(noinline) void __stdcall StartEntityScript_00404f30(
    AnmVmView *entity, int scriptIndex, AnmLoadedView *resource)
{
    __asm
    {
        mov esi, entity
        mov eax, scriptIndex
        push resource
        call EntityStartScript_00404f30
    }
}
#pragma code_seg()
