#include "AnmManager.hpp"
#include <new>
#include <string.h>

// Target 0x0040B250..0x0040B28B.  The native caller supplies the ANM VM in
// ECX and eleven dwords of vertex data in EDX.  The source's original name and
// compiler owner are not independently known; this function preserves the
// observed allocation, copy, state updates and return value.
int __fastcall InstallAnmVmVertexBuffer(
    AnmVmView *vm, const unsigned int *vertices)
{
    unsigned int *stored = static_cast<unsigned int *>(::operator new(44));
    vm->generatedVertices = stored;
    memcpy(stored, vertices, 44);

    // These three words are at +0x340..+0x348 in the existing VM layout.
    memcpy(&vm->positionOffset, stored, sizeof(vm->positionOffset));
    return 0;
}
