#include "StageScript.hpp"
#include <string.h>

// Target aliases within MainSupervisor's storage. Allocation ownership open.
extern StageCameraStateView g_StagePublishedCamera; // 0x491D7C
extern StageCameraStateView g_SceneGameplayCamera; // 0x491E94

// Target 0x4216F0, complete 465-byte owner. Diagnostic proposal only.
void MainInitializeSceneCameras()
{
    g_SceneGameplayCamera.position = AnmFloat3View(0.0f, 0.0f, 1000.0f);
    g_SceneGameplayCamera.direction = AnmFloat3View(0.0f, 0.0f, 0.0f);
    g_SceneGameplayCamera.up = AnmFloat3View(0.0f, 1.0f, 0.0f);
    g_SceneGameplayCamera.rockingOffset = g_SceneGameplayCamera.rockingOffsetY = g_SceneGameplayCamera.rockingOffsetZ = 0.0f;
    g_SceneGameplayCamera.value048 = 0.52359879016876220703125f;
    g_SceneGameplayCamera.viewport.x = 0;
    g_SceneGameplayCamera.viewport.y = 0;
    g_SceneGameplayCamera.viewport.width = 640;
    g_SceneGameplayCamera.viewport.height = 480;
    g_SceneGameplayCamera.viewport.minZ = 0.0f;
    g_SceneGameplayCamera.viewport.maxZ = 1.0f;
    const unsigned int gameplayFlag = 1;
    memcpy(g_SceneGameplayCamera.unknown0E4, &gameplayFlag, sizeof(gameplayFlag));

    g_StagePublishedCamera.position = AnmFloat3View(0.0f, 0.0f, 1000.0f);
    g_StagePublishedCamera.direction = AnmFloat3View(0.0f, 0.0f, 0.0f);
    g_StagePublishedCamera.up = AnmFloat3View(0.0f, 1.0f, 0.0f);
    g_StagePublishedCamera.value048 = 0.52359879016876220703125f;
    g_StagePublishedCamera.viewport.x = 32;
    g_StagePublishedCamera.viewport.y = 16;
    g_StagePublishedCamera.viewport.width = 384;
    g_StagePublishedCamera.viewport.height = 448;
    g_StagePublishedCamera.viewport.minZ = 0.0f;
    g_StagePublishedCamera.viewport.maxZ = 1.0f;
    const unsigned int backgroundFlag = 0;
    memcpy(g_StagePublishedCamera.unknown0E4, &backgroundFlag, sizeof(backgroundFlag));
    g_StagePublishedCamera.rockingOffset = g_StagePublishedCamera.rockingOffsetY = g_StagePublishedCamera.rockingOffsetZ = 0.0f;
}
