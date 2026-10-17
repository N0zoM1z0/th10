# Fresh v2 message record domain

This revision is reconstructed from committed 50b971feb9999a14d75461f238f059b2970fa690 and newly retrieved target bytes. It is not byte recovery of the lost v1 packet. Old hashes and reported results are historical provenance only.

GuiMessageInstructionView is an unsigned-char byte alias. Its public currentInstruction member retains one pointer and the target +0x5c offset. C++03 inline accessors copy scalar bytes into live unsigned-short/int objects; no fictitious variable struct, out-of-bounds payload[1], unaligned int lvalue, or hidden record sidecar is used in repaired production code. The ABI is still subject to native verification.

Domain: live byte-array storage, complete four-byte headers and declared payload extents, four available bytes for integer-bearing opcodes, little-endian scalar representation, CHAR_BIT8/ushort2/int4, valid cursor destinations in the same backing storage and arithmetic within represented ranges. No-argument opcodes must not read an integer operand. Production is the known little-endian x86 target; standalone host tests must check widths and endianness. No malformed/truncated-file safety claim.

Consumer audit against the fresh complete source/scripts export finds only Gui.hpp, Gui.cpp, GuiMessage.cpp, the full-header constructor fixture, and the older standalone synthetic gui-lifecycle fixture. GuiMessage.cpp only casts the initialized file cursor; its production body is unchanged. Updater code does not dereference record fields. Constructor fixture uses an actual four-byte byte array after this API change. The older isolated gui-lifecycle fixture declares its own independent record placeholder and does not include the changed canonical header; it is not evidence of the repaired record model.

Profile count accesses use the existing g_GuiPersistentData byte-buffer contract, memcpy into a live int, signed comparison against99999, increment, memcpy back. Formula is P+(3*character+subtype)*0x437c+0x4d0+4*difficulty. The target slot47783C identity is known; the spelling's physical alias to canonical statistics storage remains an inherited obligation. No fabricated count-array layout or host canonical-header offset assumption is introduced.

## GNU i386 layout harness integration

The full CI initially stopped because the freestanding (-nostdinc) GUI draw
layout harness supplied neither limits.h nor memcpy, newly needed by this
header. Its declaration-only standard headers now expose CHAR_BIT via the GNU
compiler's __CHAR_BIT__ and the standard C memcpy prototype. No game type,
width/offset assertion, compiler target or production source was changed.
Both complete-header GuiDraw.cpp and AnmRectangle.cpp GNU i386 checks passed
afterward; this remains a layout diagnostic, not VC7.1 ABI/codegen evidence.
