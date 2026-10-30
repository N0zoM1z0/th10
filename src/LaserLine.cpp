#include "Main.hpp"
#include "EnemyLaserLine.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"
#include "Gui.hpp"
#include <math.h>

extern unsigned char g_MainSoundOwner[0x52d0];
// Target426CF0: one stack Player argument, RET4; complete body in Player.cpp.
extern void __stdcall PlayerHandleCollisionHit(Player *player);

static inline EnemyLaserVectorView &AddLaserPosition(
    EnemyLaserVectorView &position, const EnemyLaserVectorView &delta)
{
    position.x += delta.x;
    position.y += delta.y;
    position.z += delta.z;
    return position;
}

static inline void SetLaserScaleComponent(
    float &component, unsigned int &flags, float value)
{
    flags |= 8u;
    component = value;
}

// Rotate X/Y while the caller retains relative Z for its complete bounds copies.
static void RotateContactPosition(AnmFloat3View *result,
    const AnmFloat3View *position, float angle)
{
    const double inverseAngle = -static_cast<double>(angle);
    float sine = static_cast<float>(sin(inverseAngle));
    float cosine = static_cast<float>(cos(inverseAngle));
    result->x = static_cast<float>(position->x * cosine - sine * position->y);
    result->y = static_cast<float>(sine * position->x + cosine * position->y);
}

// Target4267F0. Keep rejection polarity: unordered alone does not reject.
int Player::CheckLaserContact(
    const PlayerFloat3 *origin, float angle, float width, float length)
{
    AnmFloat3View rotated(drawPosition.x - origin->x,
        drawPosition.y - origin->y, drawPosition.z - origin->z);
    AnmFloat3View lower = rotated;
    RotateContactPosition(&rotated, &lower, angle);
    const AnmFloat3View &halfSize =
        *reinterpret_cast<const AnmFloat3View *>(&extent0);
    lower = rotated - halfSize;
    AnmFloat3View upper;
    upper = rotated + halfSize;
    if (!(lower.x > length) && !(width * 0.5f < lower.y) &&
        !(upper.x < 0.0f) && !(width * -0.5f > upper.y))
    {
        if (g_GuiView != NULL && g_GuiView->message != NULL)
            return 0;
        if (runtimeState == 2 || runtimeState == 4 || runtimeState == 3)
            return 0;
        if (highlightTimer.current > 0)
            return 0;
        PlayerHandleCollisionHit(this);
        return 1;
    }
    if (!(lower.x > length) && !(upper.x < 0.0f) &&
        !(width * 1.5f < lower.y) && !(width * -1.5f > lower.y))
        return 2;
    return 0;
}

// Target41D3D0. Virtual callback implementations and construction stay separate.
int EnemyLaserLineView::Update()
{
    RunCommands();
    if (flags != 0)
    {
        if ((flags & 1u) != 0) Motion0001();
        if ((flags & 0x10u) != 0) Motion0010();
        if ((flags & 0x20u) != 0) Motion0020();
        if ((flags & 0x40u) != 0) Motion0040();
        if ((flags & 0x100u) != 0) Motion0100();
        if ((flags & 0x80u) != 0) Motion0080();
        if ((flags & 0x08000c00u) != 0) Bounce();
        if ((flags & 0x100000u) != 0) Motion100000();
        if ((flags & 0x200000u) != 0) Motion200000();
        if ((flags & 0x04000000u) != 0) Motion4000000();
        if ((flags & 0x8000u) != 0)
        {
            if (patterns[5].timer.current <= 0)
                flags ^= 0x8000u;
            else
                patterns[5].timer.Add(-1.0f);
        }
    }
    if (length < request.maximumLength)
    {
        length += g_AnmGameSpeed * growSpeed;
        if (length > request.maximumLength)
            length = request.maximumLength;
    }
    else
    {
        traveled += g_AnmGameSpeed * growSpeed;
        EnemyLaserVectorView delta;
        delta.x = g_AnmGameSpeed * velocity.x;
        delta.y = g_AnmGameSpeed * velocity.y;
        delta.z = g_AnmGameSpeed * velocity.z;
        AddLaserPosition(position, delta);
        if (request.terminalDistance > 0.0f &&
            traveled + length > request.terminalDistance)
        {
            length = request.terminalDistance - traveled;
            request.maximumLength = length;
            if (length <= 0.0f)
                return 1;
        }
    }
    if (holdTimer.current <= 0)
    {
        // The native polar helper writes only X/Y. Z prefill is unknown;
        // the following bounds helper consumes X/Y only.
        EnemyLaserVectorView head;
        head.FromAngleMagnitude(angle, length);
        AddLaserPosition(head, position);
        if (position.IsOutsidePlayfield(width, width) &&
            head.IsOutsidePlayfield(width, width))
            return 1;
    }
    else
        holdTimer.Add(-1.0f);

    if (length > 16.0f && width > 3.0f)
    {
        EnemyLaserVectorView point;
        point.FromAngleMagnitude(angle, length * 0.1f);
        AddLaserPosition(point, position);
        float collisionWidth;
        if (width < 32.0f)
            collisionWidth = width * 0.5f;
        else
            collisionWidth = width - (width + 16.0f) * 0.5f;
        int collision = g_Player->CheckLaserContact(
            reinterpret_cast<const PlayerFloat3 *>(&point),
            angle, collisionWidth, length * 0.800000011920929f);
        if (collision == 1)
        {
            EnemyLaserVectorView hitSize;
            hitSize.x = 32.0f;
            hitSize.y = 32.0f;
            hitSize.z = 0.0f;
            CheckCollisionBox(
                reinterpret_cast<EnemyLaserVectorView *>(&g_Player->drawPosition),
                &hitSize, 0);
        }
        else if (collision == 2 && age.current % 5 == 0)
        {
            g_EnemyPrimaryResourceOwner->primaryEnemyResource->CreateVmAtWorldVariant0(
                434, reinterpret_cast<const AnmFloat3View *>(&g_Player->drawPosition));
            reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->
                QueueSoundCue(28, position.x);
        }
    }
    AnmSpriteView *sprite = primaryVm.loadedSprite;
    AnmVmView *vm = &primaryVm;
    SetLaserScaleComponent(vm->scaleX, vm->flags35C, width / sprite->width);
    sprite = primaryVm.loadedSprite;
    SetLaserScaleComponent(vm->scaleY, vm->flags35C, length / sprite->height);
    AnmRenderManagerView::ExecuteScript(vm);
    if (traveled == 0.0f)
        AnmRenderManagerView::ExecuteScript(&secondaryVm);
    return 0;
}

// Target41D2C0, current origin classification remains indeterminate.
void EnemyLaserLineView::Motion0010()
{
    EnemyLaserAccelerationRecordView &motion =
        *reinterpret_cast<EnemyLaserAccelerationRecordView *>(&patterns[1]);
    if (motion.timer.current >= motion.duration)
        flags &= ~0x10u;
    else
    {
        growSpeed += g_AnmGameSpeed * motion.growAcceleration;
        EnemyLaserVectorView delta;
        delta.x = g_AnmGameSpeed * motion.velocityAcceleration.x;
        delta.y = g_AnmGameSpeed * motion.velocityAcceleration.y;
        delta.z = g_AnmGameSpeed * motion.velocityAcceleration.z;
        AddLaserPosition(velocity, delta);
        if (fabs(velocity.x) > 0.0001f || fabs(velocity.y) > 0.0001f)
            angle = static_cast<float>(atan2(velocity.y, velocity.x));
    }
    motion.timer.Tick();
}

static void SetLaserScreenPosition(AnmFloat3View *destination,
    const EnemyLaserVectorView *position)
{
    destination->x = position->x + 224.0f;
    destination->y = position->y + 16.0f;
    destination->z = position->z;
}

// Target41D7C0. Draw and post-draw tail selection use the current global owner.
int EnemyLaserLineView::Draw()
{
    AnmVmView *vm = &primaryVm;
    SetLaserScreenPosition(&primaryVm.position, &position);
    vm->rotation.z = AddNormalizeAngle(angle, 1.5707964f);
    vm->flags35C |= 4u;
    g_AnmRenderManagerView->Draw(vm);
    if (traveled == 0.0f)
    {
        SetLaserScreenPosition(&secondaryVm.position, &position);
        g_AnmRenderManagerView->Draw(&secondaryVm);
    }
    return 0;
}
