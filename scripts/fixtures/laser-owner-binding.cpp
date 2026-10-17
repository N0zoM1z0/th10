#include <cstdio>
#include <cstdlib>
#include <cstdint>
#define __declspec(x)
#define CHECK(condition, message) do { if (!(condition)) { std::fprintf(stderr, "LASER_OWNER_CHECK_FAILED: %s\n", message); std::exit(1); } } while (0)
namespace routing {
// Audited host seam: opaque owner identities only. No target layout claim.
struct Resource { unsigned int canary; };
struct BulletSpawnDescriptorView { unsigned int canary; };
struct HostOwner {
    Resource *effectResource; unsigned int canary;
    int spawns = 0; BulletSpawnDescriptorView *last = nullptr;
    void SpawnBulletPattern(BulletSpawnDescriptorView *request) { ++spawns; last = request; }
};
using EnemyEffectWaitManager = HostOwner;
using EnemyLaserManagerResourceView = HostOwner;
using GameChainOwnerView = HostOwner;
using BulletManagerView = HostOwner;
void run() {
    Resource laserResource{0x1234}, bulletResource{0x9876};
    HostOwner laser{&laserResource, 0x1111}, bullet{&bulletResource, 0x2222};
    HostOwner *g_EnemyLaserManager = &laser;
    void *g_EnemyBulletManager = &bullet;
    BulletSpawnDescriptorView descriptor{0x5555};
    std::uintptr_t iVar26 = reinterpret_cast<std::uintptr_t>(&descriptor) - 0x2c4;
    @ROUTE_CHECKS@
    CHECK(bullet.effectResource == &bulletResource && bullet.canary == 0x2222 && bulletResource.canary == 0x9876, "bullet_canary");
}
}
namespace traversal {
@PLAYER_FLOAT3@
@DECLARATIONS@
// Define only external virtual seams; concrete callbacks below are live C++ objects.
void EnemyEffectWaitNode::UnknownVirtual0() {}
void EnemyEffectWaitNode::UnknownVirtual1() {}
void EnemyEffectWaitNode::UnknownVirtual2() {}
void EnemyEffectWaitNode::UnknownVirtual3() {}
void EnemyEffectWaitNode::UnknownVirtual4() {}
unsigned int EnemyEffectWaitNode::ApplyClear(int) { return 0; }
void EnemyEffectWaitNode::UnknownVirtual6() {}
unsigned int EnemyEffectWaitNode::ApplyPatternCancel(const PlayerFloat3 *, float, int) { return 0; }
@BODIES@
struct Node : EnemyEffectWaitNode {
    int clearCalls = 0, patternCalls = 0, seenMode = -1;
    unsigned int ApplyClear(int mode) override { ++clearCalls; seenMode = mode; next = nullptr; return 3; }
    unsigned int ApplyPatternCancel(const PlayerFloat3 *position, float radius, int mode) override {
        CHECK(position->x == 7 && radius == 9, "pattern_arguments");
        ++patternCalls; seenMode = mode; next = nullptr; return 3;
    }
};
void run() {
    EnemyEffectWaitManager manager{};
    Node a{}, b{}, c{};
    a.state = 0; b.state = 1; c.state = 2;
    a.flag50 = b.flag50 = c.flag50 = 0;
    a.next = &b; b.next = &c; c.next = nullptr; manager.head = &a;
    CHECK(EnemyWaitForEffect(&manager) == 0, "wait_return");
    CHECK(a.flag50 == 1 && b.flag50 == 0 && c.flag50 == 1, "wait_skip");
    CHECK(EnemyApplyBulletClear(&manager, 0) == 1, "clear_return");
    CHECK(a.clearCalls == 1 && b.clearCalls == 0 && c.clearCalls == 1, "clear_visits");
    CHECK(a.seenMode == 0 && c.seenMode == 0, "clear_mode");
    a.next = &b; b.next = &c; c.next = nullptr;
    PlayerFloat3 position{7, 8, 10};
    CHECK(manager.ApplyPatternCancel(&position, 9, 1) == 6, "pattern_total");
    CHECK(a.patternCalls == 1 && b.patternCalls == 0 && c.patternCalls == 1, "pattern_visits");
    CHECK(a.seenMode == 1 && c.seenMode == 1, "pattern_mode");
    CHECK(manager.cancelPosition.x == 7 && manager.cancelPosition.y == 8 && manager.cancelPosition.z == 10, "cancel_position");
    manager.head = nullptr;
    CHECK(EnemyWaitForEffect(&manager) == 0 && EnemyApplyBulletClear(&manager, 1) == 1 && manager.ApplyPatternCancel(&position, 9, 0) == 0, "empty_list");
}
}
int main() { routing::run(); traversal::run(); std::puts("PASS laser owner fixture"); }
