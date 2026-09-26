#include "Player.hpp"

struct PlayerDrawGate9EB8
{
    unsigned char unknown000[0x9eb8];
    int value9EB8;
};

extern PlayerDrawGate9EB8 *g_PlayerDrawGate9EB8;

// Target 0x00426CF0. The target ABI is one stack Player* argument / RET 4.
// The body is still an independent reconstruction target.
extern void __stdcall PlayerHandleCollisionHit(Player *player);

// Target 0x004266B0. TH10 passes the three logical arguments in
// ECX=position, EDX=player and EAX=collisionSize.
struct PlayerCollisionPositionView
{
    float x;
    float y;
    float z;

    int __fastcall CheckPlayerCollision(Player *player, const float *collisionSize);
};

int __fastcall PlayerCollisionPositionView::CheckPlayerCollision(
    Player *player, const float *collisionSize)
{
    float halfWidth = collisionSize[0] * 0.5f;
    float minX = x - halfWidth;
    float halfHeight = collisionSize[1] * 0.5f;
    float minY = y - halfHeight;
    float maxX = x + halfWidth;
    float maxY = y + halfHeight;

    if (player->derivedMin0.x <= maxX &&
        player->derivedMin0.y <= maxY &&
        minX <= player->derivedMax0.x &&
        minY <= player->derivedMax0.y) {
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

    minX = x - 24.0f;
    minY = y - 24.0f;
    maxX = x + 24.0f;
    maxY = y + 24.0f;

    if (player->derivedMin0.x <= maxX &&
        player->derivedMin0.y <= maxY &&
        minX <= player->derivedMax0.x &&
        minY <= player->derivedMax0.y)
        return 2;

    return 0;
}
