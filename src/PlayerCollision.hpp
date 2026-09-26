#ifndef TH10_RECONSTRUCTION_PLAYER_COLLISION_HPP
#define TH10_RECONSTRUCTION_PLAYER_COLLISION_HPP

struct Player;

// Maintained position view for the shared bullet/enemy collision callee.
// The original class and translation unit are not established.
struct PlayerCollisionPositionView
{
    float x;
    float y;
    float z;

    int __fastcall CheckPlayerCollision(Player *player, const float *collisionSize);
};

typedef char PlayerCollisionPositionViewSizeIs0C[
    (sizeof(PlayerCollisionPositionView) == 0x0c) ? 1 : -1];

#endif
