#include "AnmManager.hpp"

enum AnmVariableView
{
    ANM_VAR_I0 = 10000,
    ANM_VAR_I1,
    ANM_VAR_I2,
    ANM_VAR_I3,
    ANM_VAR_F0,
    ANM_VAR_F1,
    ANM_VAR_F2,
    ANM_VAR_F3,
    ANM_VAR_IC0,
    ANM_VAR_IC1,
    ANM_VAR_RANDOM_ANGLE,
    ANM_VAR_RANDOM,
    ANM_VAR_RANDOM_SIGNED,
    ANM_VAR_POSITION_X,
    ANM_VAR_POSITION_Y,
    ANM_VAR_POSITION_Z,
    ANM_VAR_GLOBAL_491D7C_X,
    ANM_VAR_GLOBAL_491D7C_Y,
    ANM_VAR_GLOBAL_491D7C_Z,
    ANM_VAR_GLOBAL_491DA0_X,
    ANM_VAR_GLOBAL_491DA0_Y,
    ANM_VAR_GLOBAL_491DA0_Z
};

float AnmVmView::GetFloatVar(float variable)
{
    switch (static_cast<int>(variable))
    {
    case ANM_VAR_I0:
        return intVar0;
    case ANM_VAR_I1:
        return intVar1;
    case ANM_VAR_I2:
        return intVar2;
    case ANM_VAR_I3:
        return intVar3;
    case ANM_VAR_F0:
        return floatVar0;
    case ANM_VAR_F1:
        return floatVar1;
    case ANM_VAR_F2:
        return floatVar2;
    case ANM_VAR_F3:
        return floatVar3;
    case ANM_VAR_IC0:
        return counterVar0;
    case ANM_VAR_IC1:
        return counterVar1;
    case ANM_VAR_RANDOM:
        return (flags35C & 0x40000000u) != 0
            ? g_AlternateRngView.GetRandomF32()
            : g_RngView.GetRandomF32();
    case ANM_VAR_RANDOM_SIGNED:
        return (flags35C & 0x40000000u) != 0
            ? g_AlternateRngView.GetRandomF32Signed()
            : g_RngView.GetRandomF32Signed();
    case ANM_VAR_RANDOM_ANGLE:
        return (flags35C & 0x40000000u) != 0
            ? g_AlternateRngView.GetRandomF32Signed() * 3.1415927f
            : g_RngView.GetRandomF32Signed() * 3.1415927f;
    case ANM_VAR_POSITION_X:
        return position.x;
    case ANM_VAR_POSITION_Y:
        return position.y;
    case ANM_VAR_POSITION_Z:
        return position.z;
    case ANM_VAR_GLOBAL_491D7C_X:
        return g_AnmBackgroundCameraPosition.x;
    case ANM_VAR_GLOBAL_491D7C_Y:
        return g_AnmBackgroundCameraPosition.y;
    case ANM_VAR_GLOBAL_491D7C_Z:
        return g_AnmBackgroundCameraPosition.z;
    case ANM_VAR_GLOBAL_491DA0_X:
        return g_AnmPosition491DA0.x;
    case ANM_VAR_GLOBAL_491DA0_Y:
        return g_AnmPosition491DA0.y;
    case ANM_VAR_GLOBAL_491DA0_Z:
        return g_AnmPosition491DA0.z;
    default:
        return variable;
    }
}

