#include "StageScript.hpp"

// The original executable publishes two adjacent 0x118-byte camera states.
// Their identities and field positions come from the StageDraw/StageScript
// consumers; their physical TU and allocation ownership remain unknown.
extern StageCameraStateView g_StagePublishedCamera; // native 0x00491D7C
extern StageCameraStateView g_StageSecondaryCamera; // native 0x00491E94

typedef char StageCameraDefaultsLayout[
    (sizeof(StageCameraStateView) == 0x118 &&
     offsetof(StageCameraStateView, unknown0E4) == 0xe4) ? 1 : -1];

// Original TH10 v1.00a at 0x004216F0. This resets the two published views,
// NOT the stage's per-instance matrix state or the unrelated interpolators.
void StageResetPublishedCameras()
{
    g_StageSecondaryCamera.position = AnmFloat3View(0.0f, 0.0f, 1000.0f);
    g_StageSecondaryCamera.direction = AnmFloat3View(0.0f, 0.0f, 0.0f);
    g_StageSecondaryCamera.up = AnmFloat3View(0.0f, 1.0f, 0.0f);
    g_StageSecondaryCamera.value048 = 0.5235988f;
    g_StageSecondaryCamera.viewport.x = 0;
    g_StageSecondaryCamera.viewport.y = 0;
    g_StageSecondaryCamera.viewport.width = 640;
    g_StageSecondaryCamera.viewport.height = 480;
    g_StageSecondaryCamera.viewport.minZ = 0.0f;
    g_StageSecondaryCamera.viewport.maxZ = 1.0f;
    g_StageSecondaryCamera.unknown0E4 = 1;
    g_StageSecondaryCamera.rockingOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    g_StagePublishedCamera.position = AnmFloat3View(0.0f, 0.0f, 1000.0f);
    g_StagePublishedCamera.direction = AnmFloat3View(0.0f, 0.0f, 0.0f);
    g_StagePublishedCamera.up = AnmFloat3View(0.0f, 1.0f, 0.0f);
    g_StagePublishedCamera.value048 = 0.5235988f;
    g_StagePublishedCamera.viewport.x = 32;
    g_StagePublishedCamera.viewport.y = 16;
    g_StagePublishedCamera.viewport.width = 384;
    g_StagePublishedCamera.viewport.height = 448;
    g_StagePublishedCamera.viewport.minZ = 0.0f;
    g_StagePublishedCamera.viewport.maxZ = 1.0f;
    g_StagePublishedCamera.unknown0E4 = 0;
    g_StagePublishedCamera.rockingOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
}
