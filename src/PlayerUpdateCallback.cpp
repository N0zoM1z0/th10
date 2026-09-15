#include "Player.hpp"


int __stdcall PlayerUpdateCallbackBody(Player *player);

// Target 0x00426500 is the update-chain entry. The callee-clean body consumes
// the pushed Player owner and leaves this adapter with only a plain return.
int __fastcall PlayerUpdateCallback(Player *player)
{
    return PlayerUpdateCallbackBody(player);
}
