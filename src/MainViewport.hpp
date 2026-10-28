#pragma once

struct AnmViewportOwnerView;
struct MainSupervisorView;

// Distinct target entries: camera matrices at421480, selection at405300.
// Ordinary source declarations leave private register transport to LTCG.
void MainConfigureCamera(AnmViewportOwnerView *camera);
void MainConfigureGameplayViewport(MainSupervisorView *supervisor, int index);
long StageDisableSupervisorFog(MainSupervisorView *supervisor);
