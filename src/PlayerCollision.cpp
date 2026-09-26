#include "Player.hpp"
#include "PlayerCollision.hpp"

struct PlayerDrawGate9EB8
{
    unsigned char unknown000[0x9eb8];
    int value9EB8;
};

extern PlayerDrawGate9EB8 *g_PlayerDrawGate9EB8;

// Target 0x00426CF0. The target ABI is one stack Player* argument / RET 4.
// The body is still an independent reconstruction target.
extern void __stdcall PlayerHandleCollisionHit(Player *player);

struct PlayerCollisionBounds2
{
    float x;
    float y;
};

// Target 0x004266B0. TH10 passes the three logical arguments in
// ECX=position, EDX=player and EAX=collisionSize.
int __fastcall PlayerCollisionPositionView::CheckPlayerCollision(
    Player *player, const float *collisionSize)
{
    PlayerCollisionBounds2 boundsMin;
    PlayerCollisionBounds2 boundsMax;
    boundsMin.x = x - collisionSize[0] * 0.5f;
    boundsMin.y = y - collisionSize[1] * 0.5f;
    boundsMax.x = x + collisionSize[0] * 0.5f;
    boundsMax.y = y + collisionSize[1] * 0.5f;

    if (player->derivedMin0.x > boundsMax.x ||
        player->derivedMin0.y > boundsMax.y ||
        player->derivedMax0.x < boundsMin.x ||
        player->derivedMax0.y < boundsMin.y) {
        boundsMin.x = x - 24.0f;
        boundsMin.y = y - 24.0f;
        boundsMax.x = x + 24.0f;
        boundsMax.y = y + 24.0f;
        if (player->derivedMin0.x > boundsMax.x ||
            player->derivedMin0.y > boundsMax.y ||
            player->derivedMax0.x < boundsMin.x ||
            player->derivedMax0.y < boundsMin.y)
            return 0;
        return 2;
    }

    if ((g_PlayerDrawGate9EB8 == 0 ||
         g_PlayerDrawGate9EB8->value9EB8 == 0) &&
        player->runtimeState != 2 &&
        player->runtimeState != 4 &&
        player->runtimeState != 3) {
        if (player->highlightTimer.current <= 0)
            PlayerHandleCollisionHit(player);
        return 1;
    }
    return 0;
}
