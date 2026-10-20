#include "EnemyLaserType1.hpp"
#include "Enemy.hpp"
#include "EnemyPrimaryResources.hpp"
#include "EnemySoundQueue.hpp"

extern int g_BulletSpriteScriptBase[];
extern EnemyManagerView *g_EnemyManager;
extern unsigned char g_MainSoundOwner[0x52d0];

static inline EnemyLaserVectorView &AddType1Position(
    EnemyLaserVectorView &position, const EnemyLaserVectorView &delta)
{
    position.x += delta.x;
    position.y += delta.y;
    position.z += delta.z;
    return position;
}

static inline void SetType1ScaleComponent(
    float &component, unsigned int &flags, float value)
{
    flags |= 8u;
    component = value;
}

// Target41E5C0: slot4, copies126 dwords, RET4/EAX0. Width starts at2;
// request2C is growth speed, independent of request28's eventual beam width.
int EnemyLaserType1View::Initialize(const EnemyLaserType1RequestView *initial)
{
    request = *initial;
    state = 3;
    AnmVmView *vm = &primaryVm;
    vm->InitializeForLoadedScript(
        reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource,
        request.color + g_BulletSpriteScriptBase[request.type]);
    primaryVm.pendingInterrupt = 2;
    AnmRenderManagerView::ExecuteScript(vm);
    if ((request.flags44 & 2u) != 0)
        vm->flags35C = (vm->flags35C & ~0x20u) | 0x10u;
    primaryVm.flags35C = (primaryVm.flags35C & 0xfc63ffffu) | 0x00600000u;
    vm = &secondaryVm;
    vm->InitializeForLoadedScript(
        reinterpret_cast<EnemyLaserManagerStorageView *>(g_EnemyLaserManager)->effectResource,
        request.color + 0x103);
    secondaryVm.pendingInterrupt = 2;
    AnmRenderManagerView::ExecuteScript(vm);
    vm->flags35C = (vm->flags35C & ~0x20u) | 0x10u;
    secondaryVm.flags35C = (secondaryVm.flags35C & 0xfc7fffffu) | 0x00400000u;
    position = request.position;
    soundId = 24;
    length = request.initialLength;
    width = 2.0f;
    growSpeed = request.growSpeed;
    angle = request.angle;
    return 0;
}

// Target41E700 plus its four-pointer state table41EA30, complete832 bytes.
// This callback resets age at phase changes; it does not tick it.
int EnemyLaserType1View::Update()
{
    if (length < request.maximumLength)
    {
        length += g_AnmGameSpeed * growSpeed;
        if (length > request.maximumLength)
            length = request.maximumLength;
    }
    angle = AddNormalizeAngle(angle, g_AnmGameSpeed * request.angularVelocity);
    if ((request.flags44 & 1u) != 0 && g_EnemyManager->specialEnemySlots[0] != 0)
    {
        const PlayerFloat3 &follow =
            g_EnemyManager->specialEnemySlots[0]->runtime.worldMotion.position;
        // Both maintained views are12-byte float triples. The original common
        // vector class is not yet established; copy through the laser view.
        position = *reinterpret_cast<const EnemyLaserVectorView *>(&follow);
    }
    EnemyLaserVectorView delta;
    delta.x = g_AnmGameSpeed * request.velocity.x;
    delta.y = g_AnmGameSpeed * request.velocity.y;
    delta.z = g_AnmGameSpeed * request.velocity.z;
    AddType1Position(position, delta);
    switch (state)
    {
    case 3:
        if (age.current >= request.waitFrames)
        {
            age.SetCurrent(0);
            state = 4;
        }
        break;
    case 4:
        if (age.current >= request.growFrames)
        {
            age.SetCurrent(0);
            state = 2;
            width = request.width;
        }
        else
        {
            width = request.width * age.subframe / request.growFrames;
            break;
        }
        // Growth completion immediately tests the holding phase.
    case 2:
        if (age.current < request.holdFrames)
            break;
        age.SetCurrent(0);
        state = 5;
        // Holding completion immediately tests the shrinking phase.
    case 5:
        if (age.current >= request.shrinkFrames)
            return 1;
        {
            float fullWidth = request.width;
            width = fullWidth - fullWidth * age.subframe / request.shrinkFrames;
        }
        break;
    }
    if ((state == 4 || state == 2) && length > 16.0f)
    {
        EnemyLaserVectorView point;
        point.FromAngleMagnitude(angle, length * 0.1f);
        AddType1Position(point, position);
        float collisionWidth;
        if (width < 32.0f)
            collisionWidth = width * 0.5f;
        else
            collisionWidth = width - (width + 16.0f) * 0.3333333432674408f;
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
    SetType1ScaleComponent(vm->scaleX, vm->flags35C, width / sprite->width);
    sprite = primaryVm.loadedSprite;
    SetType1ScaleComponent(vm->scaleY, vm->flags35C, length / sprite->height);
    AnmRenderManagerView::ExecuteScript(vm);
    if (traveled == 0.0f)
        AnmRenderManagerView::ExecuteScript(&secondaryVm);
    return 0;
}

// Target41EA40: position334 is distinct from the allocating creator's offset340.
int EnemyLaserType1View::Draw()
{
    AnmVmView *vm = &primaryVm;
    primaryVm.position.x = position.x + 224.0f;
    primaryVm.position.y = position.y + 16.0f;
    primaryVm.position.z = position.z;
    vm->rotation.z = AddNormalizeAngle(angle, 1.5707964f);
    vm->flags35C |= 4u;
    g_AnmRenderManagerView->Draw(vm);
    if (traveled == 0.0f)
    {
        secondaryVm.position.x = position.x + 224.0f;
        secondaryVm.position.y = position.y + 16.0f;
        secondaryVm.position.z = position.z;
        g_AnmRenderManagerView->Draw(&secondaryVm);
    }
    return 0;
}
