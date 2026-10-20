#include "Main.hpp"
#include "EnemyLaserLine.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"

extern unsigned char g_MainSoundOwner[0x52d0];
extern int g_BulletSpriteScriptBase[];
extern float EnemyAngleFromPlayer(Player *player, const PlayerFloat3 *position);

// Target41CA80, virtual slot0. Commands are the type-0 request's live24-byte
// records at450. Keep signed PC, mutable arguments and post-call PC reads.
void EnemyLaserLineView::RunCommands()
{
    while (commandIndex < 18)
    {
        unsigned int opcode = request.commands[commandIndex].opcode;
        EnemyLaserCommandView &command = request.commands[commandIndex];
        if (opcode == 0)
            return;
        if (command.allowWhileActive == 0 && flags != 0)
            return;
        if (opcode == 0x80000000u)
        {
            ++commandIndex;
            continue;
        }
        switch (opcode)
        {
        case 1u:
            flags |= 1u;
            patterns[0].timer.SetCurrent(0);
            patterns[0].values[4].integer = 0;
            break;
        case 0x10u:
            flags |= 0x10u;
            patterns[1].values[0].scalar = command.arguments[0].scalar;
            {
                float selectedAngle;
                if (command.arguments[1].scalar <= -990.0f)
                    selectedAngle = angle;
                else if (command.arguments[1].scalar >= 990.0f)
                    selectedAngle = EnemyAngleFromPlayer(g_Player,
                        reinterpret_cast<const PlayerFloat3 *>(&position));
                else
                    selectedAngle = command.arguments[1].scalar;
                patterns[1].values[1].scalar = selectedAngle;
            }
            patterns[1].timer.SetCurrent(0);
            patterns[1].values[5].integer = command.arguments[2].integer;
            reinterpret_cast<EnemyLaserVectorView *>(&patterns[1].values[2])->
                FromAngleMagnitude(patterns[1].values[1].scalar,
                    patterns[1].values[0].scalar);
            if (commandIndex != 0 && soundId >= 0)
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundSample(soundId, 0);
            break;
        case 0x20u:
            flags |= 0x20u;
            patterns[2].values[0].scalar = command.arguments[0].scalar;
            patterns[2].values[1].scalar = command.arguments[1].scalar;
            patterns[2].timer.SetCurrent(0);
            patterns[2].values[5].integer = command.arguments[2].integer;
            if (commandIndex != 0 && soundId >= 0)
                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                    QueueSoundSample(soundId, 0);
            break;
        case 0x40u:
        case 0x80u:
        case 0x100u:
            flags |= opcode;
            patterns[3].values[1].integer = command.arguments[0].integer;
            patterns[3].values[0].scalar = command.arguments[1].scalar > -999.0f
                ? command.arguments[1].scalar : growSpeed;
            patterns[3].timer.SetCurrent(0);
            patterns[3].values[5].integer = command.arguments[2].integer;
            patterns[3].values[6].integer = command.arguments[3].integer;
            patterns[3].values[7].integer = 0;
            break;
        case 0x4000u:
            {
                int script = g_BulletSpriteScriptBase[command.arguments[2].integer] +
                    command.arguments[3].integer;
                primaryVm.InitializeForLoadedScript(
                    g_EnemyPrimaryResourceOwner->primaryEnemyResource, script);
            }
            break;
        case 0x8000u:
            flags |= opcode;
            patterns[5].timer.SetCurrent(command.arguments[2].integer);
            break;
        case 0x10000u:
            unknown00C = 3;
            break;
        case 0x20000u:
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundCue(command.arguments[2].integer, position.x);
            break;
        case 0x100000u:
            flags |= opcode;
            patterns[6].timer.SetCurrent(command.arguments[2].integer);
            break;
        case 0x200000u:
            flags |= opcode;
            patterns[6].timer.SetCurrent(command.arguments[2].integer);
            break;
        case 0x1000000u:
            id = command.arguments[2].bits;
            ++commandIndex;
            continue;
        case 0x2000000u:
            commandIndex = command.arguments[2].integer;
            continue;
        case 0x400u:
        case 0x800u:
        case 0x8000000u:
            if (command.arguments[2].integer > 0)
            {
                flags |= opcode;
                if (command.arguments[0].scalar >= 0.0f)
                    patterns[4].values[0].scalar = command.arguments[0].scalar;
                else
                    patterns[4].values[0].scalar = growSpeed;
                --command.arguments[2].integer;
                patterns[4].values[6].integer = 0;
                patterns[4].values[5].integer = 0;
            }
            break;
        case 0x4000000u:
            flags |= 0x4000000u;
            patterns[8].values[0].scalar = command.arguments[0].scalar;
            patterns[8].values[1].scalar = command.arguments[1].scalar;
            patterns[8].timer.SetCurrent(0);
            patterns[8].values[5].integer = command.arguments[2].integer;
            break;
        default:
            break;
        }
        ++commandIndex;
    }
}
