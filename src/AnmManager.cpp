#include "AnmManager.hpp"
#include "FileSystem.hpp"

#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AsciiManagerView *g_AsciiManagerView;

struct MainSupervisorView;
extern MainSupervisorView g_MainSupervisorView;

struct MainSupervisorAnmPrefixView
{
    void *instance;
    void *d3dInterface;
    D3d9DeviceView *d3dDevice;
    unsigned char padding00C[0xe0];
    unsigned int backbufferFormat;
};

typedef char MainSupervisorAnmPrefixBackbufferFormatAt0EC[
    (offsetof(MainSupervisorAnmPrefixView, backbufferFormat) == 0x0ec) ? 1 : -1];

// Target 0x0044BC10 adds an angular delta and bounds the result to the
// engine's signed-pi interval. The loop cap is part of the target body.
float __stdcall AddNormalizeAngle(float angle, float delta)
{
    int i = 0;

    angle += delta;
    while (angle > 3.1415927f)
    {
        angle -= 6.2831855f;
        if (i++ > 32)
            break;
    }
    while (angle < -3.1415927f)
    {
        angle += 6.2831855f;
        if (i++ > 32)
            break;
    }
    return angle;
}

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

// Target 0x0043EAC0-0x0043EC67 resolves the float-valued script variables.
// TH10 extends the earlier engine's VM-local set with two global Float3
// sources. The second owner's semantics remain unknown, so its address stays
// in the maintained name rather than being inferred from adjacent games.
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

// Target 0x0043EC70-0x0043ECFF converts the four float locals through the
// compiler's normal float-to-int helper and directly returns integer locals.
int AnmVmView::GetIntVar(int variable)
{
    switch (variable)
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
        return static_cast<int>(floatVar0);
    case ANM_VAR_F1:
        return static_cast<int>(floatVar1);
    case ANM_VAR_F2:
        return static_cast<int>(floatVar2);
    case ANM_VAR_F3:
        return static_cast<int>(floatVar3);
    case ANM_VAR_IC0:
        return counterVar0;
    case ANM_VAR_IC1:
        return counterVar1;
    default:
        return variable;
    }
}

// Target 0x0043ED00-0x0043ED9B redirects a masked float instruction argument
// to one of the VM's writable float or position slots.
float *AnmVmView::GetFloatVarPtr(
    float *value, unsigned short variableMask, unsigned int argumentNumber)
{
    if ((variableMask & (1u << argumentNumber)) == 0)
        return value;

    switch (static_cast<int>(*value))
    {
    case ANM_VAR_F0:
        return &floatVar0;
    case ANM_VAR_F1:
        return &floatVar1;
    case ANM_VAR_F2:
        return &floatVar2;
    case ANM_VAR_F3:
        return &floatVar3;
    case ANM_VAR_POSITION_X:
        return &position.x;
    case ANM_VAR_POSITION_Y:
        return &position.y;
    case ANM_VAR_POSITION_Z:
        return &position.z;
    default:
        return value;
    }
}

// Target 0x0043EDA0-0x0043EE23 is the integer counterpart. Only the four
// integer locals and two counter locals are legal writable destinations.
int *AnmVmView::GetIntVarPtr(
    int *value, unsigned short variableMask, unsigned int argumentNumber)
{
    if ((variableMask & (1u << argumentNumber)) == 0)
        return value;

    switch (*value)
    {
    case ANM_VAR_I0:
        return &intVar0;
    case ANM_VAR_I1:
        return &intVar1;
    case ANM_VAR_I2:
        return &intVar2;
    case ANM_VAR_I3:
        return &intVar3;
    case ANM_VAR_IC0:
        return &counterVar0;
    case ANM_VAR_IC1:
        return &counterVar1;
    default:
        return value;
    }
}

// Target 0x0044C350 computes the normalized interpolation progress used by
// the scalar and vector interpolation evaluators. Modes 8, 7, and 17 are
// handled by the evaluators because they also consume or mutate endpoints.
float CalculateAnmInterpolation(int mode, float current, float duration)
{
    float value = current / duration;

    switch (mode)
    {
    case ANM_INTERPOLATION_EASE_IN:
        return value * value;
    case ANM_INTERPOLATION_EASE_IN_CUBIC:
        return value * value * value;
    case ANM_INTERPOLATION_EASE_IN_QUARTIC:
        return value * value * value * value;
    case ANM_INTERPOLATION_EASE_OUT:
        return 1.0f - (1.0f - value) * (1.0f - value);
    case ANM_INTERPOLATION_EASE_OUT_CUBIC:
        value = 1.0f - value;
        return 1.0f - value * value * value;
    case ANM_INTERPOLATION_EASE_OUT_QUARTIC:
        value = 1.0f - value;
        return 1.0f - value * value * value * value;
    case ANM_INTERPOLATION_EASE_IN_OUT:
        value += value;
        if (value < 1.0f)
            return value * value * 0.5f;
        return (2.0f - (2.0f - value) * (2.0f - value)) * 0.5f;
    case ANM_INTERPOLATION_EASE_IN_OUT_CUBIC:
        value += value;
        if (value < 1.0f)
            return value * value * value * 0.5f;
        value = 2.0f - value;
        return (2.0f - value * value * value) * 0.5f;
    case ANM_INTERPOLATION_EASE_IN_OUT_QUARTIC:
        value += value;
        if (value < 1.0f)
            return value * value * value * value * 0.5f;
        value = 2.0f - value;
        return (2.0f - value * value * value * value) * 0.5f;
    case ANM_INTERPOLATION_EASE_OUT_IN:
        value += value;
        if (value < 1.0f)
            return 0.5f - (1.0f - value) * (1.0f - value) * 0.5f;
        value -= 1.0f;
        return value * value * 0.5f + 0.5f;
    case ANM_INTERPOLATION_EASE_OUT_IN_CUBIC:
        value += value;
        if (value < 1.0f)
        {
            value = 1.0f - value;
            return 0.5f - value * value * value * 0.5f;
        }
        value -= 1.0f;
        return value * value * value * 0.5f + 0.5f;
    case ANM_INTERPOLATION_EASE_OUT_IN_QUARTIC:
        value += value;
        if (value < 1.0f)
        {
            value = 1.0f - value;
            return 0.5f - value * value * value * value * 0.5f;
        }
        value -= 1.0f;
        return value * value * value * value * 0.5f + 0.5f;
    case ANM_INTERPOLATION_CONSTANT_ZERO:
        return 0.0f;
    case ANM_INTERPOLATION_CONSTANT_ONE:
        return 1.0f;
    default:
        return value;
    }
}

// Target 0x00404610 evaluates TH10's 0x4C-byte three-component
// interpolation object. The same body serves ANM position/rotation and two
// earlier engine users; modes 7 and 17 update their stored state each tick.
AnmFloat3View *AnmVmFloat3InterpolationView::Evaluate(
    AnmFloat3View *output)
{
    if (duration > 0)
    {
        timer.Tick();
        if (timer.current >= duration)
        {
            timer.SetCurrent(duration);
            duration = 0;
            if (mode == ANM_INTERPOLATION_ADD)
            {
                *output = initial;
                return output;
            }
            *output = final;
            return output;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        initial = initial + final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        initial = initial + finalTangent;
        finalTangent = finalTangent + final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_HERMITE)
    {
        float value = timer.subframe / static_cast<float>(duration);
        float minusOne = value - 1.0f;
        float initialWeight = (1.0f + value + value) * minusOne * minusOne;
        float finalWeight = (3.0f - value - value) * value * value;
        float initialTangentWeight = (1.0f - value) * (1.0f - value) * value;
        float finalTangentWeight = minusOne * value * value;

        *output = initial * initialWeight + final * finalWeight +
            initialTangent * initialTangentWeight +
            finalTangent * finalTangentWeight;
        return output;
    }

    float value = CalculateAnmInterpolation(
        mode, timer.subframe, static_cast<float>(duration));
    *output = (final - initial) * value + initial;
    return output;
}

// Target 0x00441600 is the integer-triplet counterpart used by the two RGB
// slots. Hermite mode truncates each weighted triplet before summing it.
AnmInt3View *AnmVmColorInterpolationView::Evaluate(AnmInt3View *output)
{
    if (duration > 0)
    {
        timer.Tick();
        if (timer.current >= duration)
        {
            timer.SetCurrent(duration);
            duration = 0;
            *output = mode == ANM_INTERPOLATION_ADD ? initial : final;
            return output;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        initial.x += final.x;
        initial.y += final.y;
        initial.z += final.z;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        initial.x += finalTangent.x;
        initial.y += finalTangent.y;
        initial.z += finalTangent.z;
        finalTangent.x += final.x;
        finalTangent.y += final.y;
        finalTangent.z += final.z;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_HERMITE)
    {
        float value = timer.subframe / static_cast<float>(duration);
        float minusOne = value - 1.0f;
        float initialWeight = (1.0f + value + value) * minusOne * minusOne;
        float finalWeight = (3.0f - value - value) * value * value;
        float initialTangentWeight = (1.0f - value) * (1.0f - value) * value;
        float finalTangentWeight = minusOne * value * value;
        AnmInt3View finalPart = final * finalWeight;
        AnmInt3View initialPart = initial * initialWeight;
        AnmInt3View initialTangentPart =
            initialTangent * initialTangentWeight;
        AnmInt3View finalTangentPart =
            finalTangent * finalTangentWeight;

        output->x = initialPart.x + finalPart.x +
            initialTangentPart.x + finalTangentPart.x;
        output->y = initialPart.y + finalPart.y +
            initialTangentPart.y + finalTangentPart.y;
        output->z = initialPart.z + finalPart.z +
            initialTangentPart.z + finalTangentPart.z;
        return output;
    }

    float value = CalculateAnmInterpolation(
        mode, timer.subframe, static_cast<float>(duration));
    output->x = static_cast<int>((final.x - initial.x) * value) + initial.x;
    output->y = static_cast<int>((final.y - initial.y) * value) + initial.y;
    output->z = static_cast<int>((final.z - initial.z) * value) + initial.z;
    return output;
}

// Target 0x00441950 evaluates either primary or secondary alpha through the
// scalar 0x2C-byte slot.
int AnmVmAlphaInterpolationView::Evaluate()
{
    if (duration > 0)
    {
        timer.Tick();
        if (timer.current >= duration)
        {
            timer.SetCurrent(duration);
            duration = 0;
            return mode == ANM_INTERPOLATION_ADD ? initial : final;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        initial += final;
        return initial;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        initial += finalTangent;
        finalTangent += final;
        return initial;
    }

    if (mode == ANM_INTERPOLATION_HERMITE)
    {
        float value = timer.subframe / static_cast<float>(duration);
        float minusOne = value - 1.0f;
        return static_cast<int>(
            (1.0f + value + value) * minusOne * minusOne * initial +
            (3.0f - value - value) * value * value * final +
            (1.0f - value) * (1.0f - value) * value * initialTangent +
            minusOne * value * value * finalTangent);
    }

    float value = CalculateAnmInterpolation(
        mode, timer.subframe, static_cast<float>(duration));
    return static_cast<int>((final - initial) * value) + initial;
}

// Target 0x00441AD0 evaluates the 0x3C-byte scale slot.
AnmFloat2View *AnmVmFloat2InterpolationView::Evaluate(AnmFloat2View *output)
{
    if (duration > 0)
    {
        timer.Tick();
        if (timer.current >= duration)
        {
            timer.SetCurrent(duration);
            duration = 0;
            if (mode == ANM_INTERPOLATION_ADD)
            {
                *output = initial;
                return output;
            }
            *output = final;
            return output;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        initial = initial + final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        initial = initial + finalTangent;
        finalTangent = finalTangent + final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_HERMITE)
    {
        float value = timer.subframe / static_cast<float>(duration);
        float minusOne = value - 1.0f;
        float initialWeight = (1.0f + value + value) * minusOne * minusOne;
        float finalWeight = (3.0f - value - value) * value * value;
        float initialTangentWeight = (1.0f - value) * (1.0f - value) * value;
        float finalTangentWeight = minusOne * value * value;
        AnmFloat2View initialPart = initial * initialWeight;
        AnmFloat2View finalPart = final * finalWeight;
        AnmFloat2View initialTangentPart =
            initialTangent * initialTangentWeight;
        AnmFloat2View finalTangentPart =
            finalTangent * finalTangentWeight;
        output->x = initialPart.x + finalPart.x +
            initialTangentPart.x + finalTangentPart.x;
        output->y = initialPart.y + finalPart.y +
            initialTangentPart.y + finalTangentPart.y;
        return output;
    }

    float value = CalculateAnmInterpolation(
        mode, timer.subframe, static_cast<float>(duration));
    output->x = (final.x - initial.x) * value + initial.x;
    output->y = (final.y - initial.y) * value + initial.y;
    return output;
}

// Target 0x0041AB70 installs the two-component scale interpolation. The
// target's private LTCG seam keeps the VM in EAX and the final/initial vectors
// in ECX/EDX, but the state written here is independent of that optimization.
void AnmVmView::StartScaleInterpolation(
    const AnmFloat2View *initial, const AnmFloat2View *final,
    int duration, unsigned char mode)
{
    scaleInterpolation.duration = duration;
    scaleInterpolation.mode = mode;
    scaleInterpolation.initial = *initial;
    scaleInterpolation.final = *final;
    scaleInterpolation.timer.SetCurrent(0);
}

// Target 0x00442050 converts the packed secondary BGRA endpoints to the
// integer triplets consumed by TH10's color interpolator. Alpha has its own
// adjacent scalar slot and is intentionally left alone.
void AnmVmView::StartSecondaryColorInterpolation(
    const AnmColorView *initial, const AnmColorView *final,
    int duration, unsigned char mode)
{
    secondaryColorInterpolation.duration = duration;
    secondaryColorInterpolation.initialTangent = AnmInt3View(0, 0, 0);
    secondaryColorInterpolation.finalTangent = AnmInt3View(0, 0, 0);
    secondaryColorInterpolation.mode = mode;

    AnmInt3View initialValue;
    initialValue.z = initial->red;
    initialValue.y = initial->green;
    initialValue.x = initial->blue;
    AnmInt3View finalValue;
    finalValue.z = final->red;
    finalValue.y = final->green;
    finalValue.x = final->blue;
    secondaryColorInterpolation.initial = initialValue;
    secondaryColorInterpolation.final = finalValue;
    secondaryColorInterpolation.timer.SetCurrent(0);
}

// Target 0x00441F50 installs the scalar secondary-alpha interpolation.
void AnmVmView::StartSecondaryAlphaInterpolation(
    int duration, unsigned char mode,
    unsigned char initial, unsigned char final)
{
    secondaryAlphaInterpolation.duration = duration;
    secondaryAlphaInterpolation.mode = mode;
    secondaryAlphaInterpolation.initial = initial;
    secondaryAlphaInterpolation.final = final;
    secondaryAlphaInterpolation.timer.SetCurrent(0);
}

// Target 0x00442220 is the primary-color counterpart of 0x00442050.
void AnmVmView::StartPrimaryColorInterpolation(
    const AnmColorView *initial, const AnmColorView *final,
    int duration, unsigned char mode)
{
    primaryColorInterpolation.duration = duration;
    primaryColorInterpolation.initialTangent = AnmInt3View(0, 0, 0);
    primaryColorInterpolation.finalTangent = AnmInt3View(0, 0, 0);
    primaryColorInterpolation.mode = mode;

    AnmInt3View initialValue;
    initialValue.z = initial->red;
    initialValue.y = initial->green;
    initialValue.x = initial->blue;
    AnmInt3View finalValue;
    finalValue.z = final->red;
    finalValue.y = final->green;
    finalValue.x = final->blue;
    primaryColorInterpolation.initial = initialValue;
    primaryColorInterpolation.final = finalValue;
    primaryColorInterpolation.timer.SetCurrent(0);
}

// Target 0x00442300 installs the scalar primary-alpha interpolation.
void AnmVmView::StartPrimaryAlphaInterpolation(
    int duration, unsigned char mode,
    unsigned char initial, unsigned char final)
{
    primaryAlphaInterpolation.duration = duration;
    primaryAlphaInterpolation.mode = mode;
    primaryAlphaInterpolation.initial = initial;
    primaryAlphaInterpolation.initialTangent = 0;
    primaryAlphaInterpolation.finalTangent = 0;
    primaryAlphaInterpolation.final = final;
    primaryAlphaInterpolation.timer.SetCurrent(0);
}

// Target 0x00413200 links a child VM's secondary list node immediately after
// its parent. The node addresses, rather than their owning VM bases, are what
// the intrusive list stores.
void AnmVmLayerNodeView::InsertAfter(AnmVmLayerNodeView *node)
{
    if (next != NULL)
    {
        node->next = next;
        next->previous = node;
    }
    next = node;
    node->previous = this;
}

// Target 0x004491C0 resolves ids across the two independently ordered VM
// lists. A zero id is always invalid.
AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)
{
    AnmVmLayerNodeView *node;

    if (id.value == 0)
        return NULL;

    node = primaryVmListHead;
    while (node != NULL)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == id.value)
            return static_cast<AnmVmView *>(node->owner);
        node = node->next;
    }

    node = secondaryVmListHead;
    while (node != NULL)
    {
        if (static_cast<AnmVmView *>(node->owner)->id == id.value)
            return static_cast<AnmVmView *>(node->owner);
        node = node->next;
    }
    return NULL;
}

// Target 0x00449210 sends one interrupt to a VM and, for a root VM, to every
// following node in its child layer chain.
void AnmRenderManagerView::SetVmPendingInterrupt(int id, short interrupt)
{
    AnmVmView *vm = FindVm(id);
    if (vm == NULL)
        return;

    vm->pendingInterrupt = interrupt;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->pendingInterrupt = interrupt;
        node = node->next;
    }
}

// Target 0x00449250 performs the same propagation and immediately advances
// each affected VM through the ANM executor.
void AnmRenderManagerView::SetVmPendingInterruptAndExecute(
    int id, short interrupt)
{
    AnmVmView *vm = FindVm(id);
    if (vm == NULL)
        return;

    vm->pendingInterrupt = interrupt;
    AnmRenderManagerView::ExecuteScript(vm);
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        AnmVmView *child = static_cast<AnmVmView *>(node->owner);
        child->pendingInterrupt = interrupt;
        AnmRenderManagerView::ExecuteScript(child);
        node = node->next;
    }
}

// Target 0x004492A0 marks a VM tree for removal during the manager update.
void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)
{
    AnmVmView *vm = FindVm(id);
    if (vm == NULL)
        return;

    const unsigned int deletionFlag = 0x04000000u;
    vm->flags35C |= deletionFlag;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->flags35C |= deletionFlag;
        node = node->next;
    }
}

// Target 0x004492F0 updates the screen-space offset of a VM tree.
void AnmRenderManagerView::SetVmPosition(
    int id, const AnmFloat3View *position)
{
    AnmVmView *vm = FindVm(id);
    if (vm == NULL)
        return;

    vm->positionOffset = *position;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->positionOffset = *position;
        node = node->next;
    }
}

// Target 0x00449350 applies TH10's playfield origin before propagating the
// position through the same VM tree.
void AnmRenderManagerView::SetVmWorldPosition(
    int id, const AnmFloat3View *position)
{
    AnmVmView *vm = FindVm(id);
    if (vm == NULL)
        return;

    vm->positionOffset.x = position->x + 224.0f;
    vm->positionOffset.y = position->y + 16.0f;
    vm->positionOffset.z = position->z;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        AnmVmView *child = static_cast<AnmVmView *>(node->owner);
        child->positionOffset.x = position->x + 224.0f;
        child->positionOffset.y = position->y + 16.0f;
        child->positionOffset.z = position->z;
        node = node->next;
    }
}

AnmFloat3View *AnmRenderManagerView::GetVmPosition(AnmVmIdView id)
{
    AnmVmView *vm = FindVm(id);
    if (vm != NULL)
        return &vm->positionOffset;
    return NULL;
}

// Target 0x004493E0 scans both manager-order lists and marks every VM backed
// by the selected loaded ANM resource.
void AnmRenderManagerView::MarkLoadedVmsForDeletion(AnmLoadedView *loaded)
{
    const unsigned int deletionFlag = 0x04000000u;
    AnmVmLayerNodeView *node = primaryVmListHead;
    while (node != NULL)
    {
        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
        if (vm->anmFile == loaded)
            vm->flags35C |= deletionFlag;
        node = node->next;
    }

    node = secondaryVmListHead;
    while (node != NULL)
    {
        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
        if (vm->anmFile == loaded)
            vm->flags35C |= deletionFlag;
        node = node->next;
    }
}

// Target 0x00449950 first tries the current slot in the 4096-VM inline pool,
// then the next slot, and falls back to a separately allocated VM when both
// are occupied. Removed inline VMs are reset before their occupancy byte is
// cleared, so allocation need only initialize the heap fallback.
AnmVmView *AnmRenderManagerView::AllocateVm()
{
    int index = nextVmPoolIndex;
    AnmVmView *vm = &vmPool[index];

    if (vmPoolUsed[index] != 0)
    {
        index = (index + 1) % 0x1000;
        nextVmPoolIndex = index;
        vm = &vmPool[index];
        if (vmPoolUsed[index] != 0)
        {
            vm = new AnmVmView;
            vm->Initialize();
        }
        else
        {
            vmPoolUsed[index] = 1;
        }
    }
    else
    {
        vmPoolUsed[index] = 1;
    }

    nextVmPoolIndex = (nextVmPoolIndex + 1) % 0x1000;
    return vm;
}

AnmVmIdView AnmRenderManagerView::AddVmVariant0(AnmVmView *vm)
{
    AnmVmLayerNodeView *node = &vm->managerNode;
    node->owner = vm;
    node->next = NULL;
    node->previous = NULL;

    if (primaryVmListHead == NULL)
    {
        primaryVmListHead = node;
    }
    else
    {
        primaryVmListTail->InsertAfter(node);
    }
    primaryVmListTail = node;

    AnmVmIdView *incrementReceiver = &nextVmId;
    (*incrementReceiver)++;
    if (nextVmId == AnmVmIdView())
        nextVmId++;
    vm->id = nextVmId.value;
    return nextVmId;
}

AnmVmIdView AnmRenderManagerView::AddVmVariant1(AnmVmView *vm)
{
    AnmVmLayerNodeView *node = &vm->managerNode;
    node->owner = vm;
    node->next = NULL;
    node->previous = NULL;

    if (secondaryVmListHead == NULL)
    {
        secondaryVmListHead = node;
    }
    else
    {
        secondaryVmListTail->InsertAfter(node);
    }
    secondaryVmListTail = node;

    AnmVmIdView *incrementReceiver = &nextVmId;
    (*incrementReceiver)++;
    if (nextVmId == AnmVmIdView())
        nextVmId++;
    vm->id = nextVmId.value;
    return nextVmId;
}

AnmVmIdView AnmRenderManagerView::AddVmVariant2(AnmVmView *vm)
{
    AnmVmLayerNodeView *node = &vm->managerNode;
    node->owner = vm;
    node->next = NULL;
    node->previous = NULL;

    if (primaryVmListHead == NULL)
    {
        primaryVmListTail = node;
    }
    else
    {
        vm->managerNode.InsertAfter(primaryVmListHead);
    }
    primaryVmListHead = node;

    AnmVmIdView *incrementReceiver = &nextVmId;
    (*incrementReceiver)++;
    if (nextVmId == AnmVmIdView())
        nextVmId++;
    vm->id = nextVmId.value;
    return nextVmId;
}

AnmVmIdView AnmRenderManagerView::AddVmVariant3(AnmVmView *vm)
{
    AnmVmLayerNodeView *node = &vm->managerNode;
    node->owner = vm;
    node->next = NULL;
    node->previous = NULL;

    if (secondaryVmListHead == NULL)
    {
        secondaryVmListTail = node;
    }
    else
    {
        vm->managerNode.InsertAfter(secondaryVmListHead);
    }
    secondaryVmListHead = node;

    AnmVmIdView *incrementReceiver = &nextVmId;
    (*incrementReceiver)++;
    if (nextVmId == AnmVmIdView())
        nextVmId++;
    vm->id = nextVmId.value;
    return nextVmId;
}

// Target 0x00448860 rebuilds draw layers 0 through 18 from the primary VM
// ordering. Deleted or completed VMs are removed immediately; surviving VMs
// are appended to the transient draw chain selected by renderLayer.
int AnmRenderManagerView::UpdatePrimaryVms()
{
    AnmVmView *drawLayerTails[20];
    int layer = 0;
    AnmVmView *sentinel = drawLayerSentinels;

    do
    {
        drawLayerTails[layer] = sentinel;
        sentinel->nextInDrawLayer = NULL;
        ++layer;
        ++sentinel;
    } while (layer < 19);

    AnmVmLayerNodeView *node = primaryVmListHead;
    if (node != NULL)
    {
        do
        {
            AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
            AnmVmLayerNodeView *next = node->next;

            if ((vm->flags35C & 0x04000000u) != 0)
            {
                RemoveVm(vm);
            }
            else
            {
                if (vm->positionCallback != NULL)
                    vm->positionCallback(vm);

                if (ExecuteScript(vm) != 0)
                {
                    RemoveVm(vm);
                }
                else
                {
                    AnmVmView *tail = drawLayerTails[vm->renderLayer];
                    tail->nextInDrawLayer = vm;
                    drawLayerTails[vm->renderLayer] = vm;
                    vm->nextInDrawLayer = NULL;
                }
            }

            ++vmsProcessedThisFrame;
            node = next;
        } while (node != NULL);
    }
    return 1;
}

// Target 0x00448900 rebuilds the secondary manager list into reserved draw
// layer 19. This pass resets the shared processed count before visiting VMs.
int AnmRenderManagerView::UpdateSecondaryVms()
{
    AnmVmLayerNodeView *node = secondaryVmListHead;

    drawLayerSentinels[19].nextInDrawLayer = NULL;
    AnmVmView *tail = &drawLayerSentinels[19];
    vmsProcessedThisFrame = 0;
    if (node != NULL)
    {
        do
        {
            AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
            AnmVmLayerNodeView *next = node->next;

            if ((vm->flags35C & 0x04000000u) != 0)
            {
                RemoveVm(vm);
            }
            else
            {
                if (vm->positionCallback != NULL)
                    vm->positionCallback(vm);

                if (ExecuteScript(vm) != 0)
                {
                    RemoveVm(vm);
                }
                else
                {
                    tail->nextInDrawLayer = vm;
                    tail = vm;
                    vm->nextInDrawLayer = NULL;
                }
            }

            ++vmsProcessedThisFrame;
            node = next;
        } while (node != NULL);
    }
    return 1;
}

// Target 0x00448980 walks one transient layer. Removal-marked VMs are not
// drawn; each surviving VM may update its generated draw payload first.
int AnmRenderManagerView::DrawLayer(int layer)
{
    AnmVmView *vm = drawLayerSentinels[layer].nextInDrawLayer;
    while (vm != NULL)
    {
        if ((vm->flags35C & 0x04000000u) == 0)
        {
            if (vm->drawCallback != NULL)
                vm->drawCallback(vm);
            Draw(vm);
        }
        vm = vm->nextInDrawLayer;
    }
    return 1;
}

// Target 0x00448BB0 unlinks both independent VM nodes. Inline-pool VMs are
// reset in place and made available to AllocateVm; heap fallbacks are deleted.
int AnmRenderManagerView::RemoveVm(AnmVmView *vm)
{
    AnmVmLayerNodeView *node = &vm->managerNode;

    if (node == primaryVmListTail)
        primaryVmListTail = vm->managerNode.previous;
    if (node == primaryVmListHead)
        primaryVmListHead = vm->managerNode.next;
    if (node == secondaryVmListTail)
        secondaryVmListTail = vm->managerNode.previous;
    if (node == secondaryVmListHead)
        secondaryVmListHead = vm->managerNode.next;

    if (node->next != NULL)
        node->next->previous = node->previous;
    if (node->previous != NULL)
        node->previous->next = node->next;
    node->next = NULL;
    node->previous = NULL;

    node = &vm->layerNode;
    if (node->next != NULL)
        node->next->previous = node->previous;
    if (node->previous != NULL)
        node->previous->next = node->next;
    node->next = NULL;
    node->previous = NULL;

    if (vm >= vmPool && vm < vmPool + 0x1000)
    {
        vmPoolUsed[vm - vmPool] = 0;
        if (vm->generatedVertices != NULL)
            free(vm->generatedVertices);
        vm->generatedVertices = NULL;
        vm->Initialize();
    }
    else
    {
        delete vm;
    }
    return 0;
}

// Target 0x00445900 constructs the 4096-entry VM pool, the embedded primary
// VM at +0x3AD130 and the twenty draw-layer sentinels before clearing the
// complete 0x732460-byte manager. The explicit Initialize pass then restores
// the reusable state of every pool VM. The callback priorities and layer set
// below are direct TH10 observations; TH095 supplied only the source-family
// hypothesis for the repeated chain registration shape.
AnmRenderManagerView::AnmRenderManagerView()
{
    AnmChainElementView *element;

    memset(this, 0, sizeof(AnmRenderManagerView));

    g_AnmQuadVerticesNoDiffuse[0].rhw =
        g_AnmQuadVerticesNoDiffuse[1].rhw =
        g_AnmQuadVerticesNoDiffuse[2].rhw =
        g_AnmQuadVerticesNoDiffuse[3].rhw = 1.0f;
    g_AnmQuadVerticesNoDiffuse[0].u = 0.0f;
    g_AnmQuadVerticesNoDiffuse[0].v = 0.0f;
    g_AnmQuadVerticesNoDiffuse[1].u = 1.0f;
    g_AnmQuadVerticesNoDiffuse[1].v = 0.0f;
    g_AnmQuadVerticesNoDiffuse[2].u = 0.0f;
    g_AnmQuadVerticesNoDiffuse[2].v = 1.0f;
    g_AnmQuadVerticesNoDiffuse[3].u = 1.0f;
    g_AnmQuadVerticesNoDiffuse[3].v = 1.0f;

    g_AnmQuadVertices[0].rhw = g_AnmQuadVertices[1].rhw =
        g_AnmQuadVertices[2].rhw = g_AnmQuadVertices[3].rhw = 1.0f;
    g_AnmQuadVertices[0].u = 0.0f;
    g_AnmQuadVertices[0].v = 0.0f;
    g_AnmQuadVertices[1].u = 1.0f;
    g_AnmQuadVertices[1].v = 0.0f;
    g_AnmQuadVertices[2].u = 0.0f;
    g_AnmQuadVertices[2].v = 1.0f;
    g_AnmQuadVertices[3].u = 1.0f;
    g_AnmQuadVertices[3].v = 1.0f;

    quadVertexBuffer = NULL;
    currentTexture = NULL;
    currentBlendMode = 0;
    currentColorOperation = 0;
    currentTextureFactor = 1;
    currentVertexShader = 0;
    currentCameraMode = 0xff;
    currentZWrite = 0;
    currentTextureFilter = 0;
    captureAnmIndex = -1;
    captureSurfaceIndex = -1;

    for (int i = 0; i < 0x1000; ++i)
        vmPool[i].Initialize();

    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(OnUpdatePrimary));
    element->argument = this;
    AnmAddCalcChainElement(element, 0x1a, g_AnmChainView);

    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(OnUpdateSecondary));
    element->argument = this;
    AnmAddCalcChainElement(element, 0x08, g_AnmChainView);

    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer0));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x09, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer1));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x0b, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer2));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x0d, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer3));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x0f, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer4));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x10, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer5));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x11, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer6));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x12, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer7));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x13, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer8));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x15, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer9));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x17, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer10));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x18, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer11));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x1a, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer12));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x1c, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer13));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x21, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer14));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x24, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer15));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x29, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer16));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x2a, g_AnmChainView);
    element = AnmCreateChainElement(
        reinterpret_cast<AnmChainCallback>(DrawLayer19));
    element->argument = this;
    AnmAddDrawChainElement(element, 0x2d, g_AnmChainView);

    g_Direct3DDevice->vtable->SetVertexShader(g_Direct3DDevice, NULL);
}

// Target 0x004462F0 builds the manager's fixed -128..+128 XYZ/UV quad,
// mirrors it to the otherwise independent background-quad storage, uploads
// the 80-byte array into a managed vertex buffer and binds the stream with a
// 0x14-byte stride. The target does not check any of the COM return values.
void AnmRenderManagerView::SetupVertexBuffer()
{
    void *lockedVertices;

    untexturedVertices[0].x = untexturedVertices[2].x = -128.0f;
    untexturedVertices[1].x = untexturedVertices[3].x = 128.0f;
    untexturedVertices[0].y = untexturedVertices[1].y = -128.0f;
    untexturedVertices[2].y = untexturedVertices[3].y = 128.0f;
    untexturedVertices[0].z = untexturedVertices[1].z =
        untexturedVertices[2].z = untexturedVertices[3].z = 0.0f;
    untexturedVertices[0].u = 0.0f;
    untexturedVertices[0].v = 0.0f;
    untexturedVertices[1].u = 1.0f;
    untexturedVertices[1].v = 0.0f;
    untexturedVertices[2].u = 0.0f;
    untexturedVertices[2].v = 1.0f;
    untexturedVertices[3].u = 1.0f;
    untexturedVertices[3].v = 1.0f;

    for (int i = 0; i < 4; ++i)
    {
        g_AnmBackgroundQuadVertices[i].x = untexturedVertices[i].x;
        g_AnmBackgroundQuadVertices[i].y = untexturedVertices[i].y;
        g_AnmBackgroundQuadVertices[i].z = untexturedVertices[i].z;
        g_AnmBackgroundQuadVertices[i].u = untexturedVertices[i].u;
        g_AnmBackgroundQuadVertices[i].v = untexturedVertices[i].v;
    }

    g_Direct3DDevice->vtable->CreateVertexBuffer(
        g_Direct3DDevice, sizeof(untexturedVertices), 0,
        D3D9_VIEW_FVF_XYZ | D3D9_VIEW_FVF_TEX1,
        D3D9_VIEW_POOL_MANAGED, &quadVertexBuffer, NULL);
    quadVertexBuffer->vtable->Lock(
        quadVertexBuffer, 0, 0, &lockedVertices, 0);
    memcpy(lockedVertices, untexturedVertices, sizeof(untexturedVertices));
    quadVertexBuffer->vtable->Unlock(quadVertexBuffer);
    g_Direct3DDevice->vtable->SetStreamSource(
        g_Direct3DDevice, 0, quadVertexBuffer, 0,
        sizeof(AnmUntexturedVertexView));
}

struct AnmArgb1555PixelView
{
    unsigned short blue : 5;
    unsigned short green : 5;
    unsigned short red : 5;
    unsigned short alpha : 1;
};

struct AnmArgb4444PixelView
{
    unsigned short blue : 4;
    unsigned short green : 4;
    unsigned short red : 4;
    unsigned short alpha : 4;
};

static void __fastcall AccumulateArgb8888Neighbor(
    unsigned int *sums, unsigned char *pixel, unsigned int *count)
{
    if (pixel[3] != 0)
    {
        sums[0] += pixel[2];
        sums[1] += pixel[1];
        sums[2] += pixel[0];
        ++*count;
    }
}

static void __fastcall AccumulateArgb1555Neighbor(
    unsigned int *sums, AnmArgb1555PixelView *pixel, unsigned int *count)
{
    if (pixel->alpha != 0)
    {
        sums[0] += pixel->red;
        sums[1] += pixel->green;
        sums[2] += pixel->blue;
        ++*count;
    }
}

static void __fastcall AccumulateArgb4444Neighbor(
    unsigned int *sums, AnmArgb4444PixelView *pixel, unsigned int *count)
{
    if (pixel->alpha != 0)
    {
        sums[0] += pixel->red;
        sums[1] += pixel->green;
        sums[2] += pixel->blue;
        ++*count;
    }
}

// Target 0x004465B0 fills the RGB channels of transparent texels from their
// nontransparent orthogonal neighbors. This prevents colored fringes after
// filtered sampling while retaining the original zero alpha. TH10 handles
// 32-bit ARGB, A1R5G5B5 and A4R4G4B4 surfaces and treats format zero like
// A8R8G8B8. The surface calls, format cases and every edge condition below
// are present in the target; TH095 was consulted only for natural source shape.
void AnmRenderManagerView::ApplyTextureAlphaBleed(
    AnmTextureEntryView *entry)
{
    D3d9SurfaceView *surface = NULL;
    D3d9SurfaceDescriptionView description;
    D3d9LockedRectView locked;

    entry->texture->vtable->GetSurfaceLevel(entry->texture, 0, &surface);
    surface->vtable->GetDesc(surface, &description);
    surface->vtable->LockRect(surface, &locked, NULL, 0);

    switch (description.format)
    {
    case D3D9_VIEW_FMT_UNKNOWN:
    case D3D9_VIEW_FMT_A8R8G8B8:
        for (unsigned int y = 0; y < description.height; ++y)
        {
            unsigned int *pixel = reinterpret_cast<unsigned int *>(
                static_cast<unsigned char *>(locked.bits) + locked.pitch * y);
            for (unsigned int x = 0; x < description.width; ++x, ++pixel)
            {
                unsigned char *components =
                    reinterpret_cast<unsigned char *>(pixel);
                if (components[3] == 0)
                {
                    unsigned int sums[3] = {0, 0, 0};
                    unsigned int neighborCount = 0;
                    if (x > 0)
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<unsigned char *>(pixel - 1),
                            &neighborCount);
                    if (x < description.width - 1)
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<unsigned char *>(pixel + 1),
                            &neighborCount);
                    if (y > 0)
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<unsigned char *>(pixel) -
                                locked.pitch,
                            &neighborCount);
                    if (y < description.height - 1)
                        AccumulateArgb8888Neighbor(
                            sums, reinterpret_cast<unsigned char *>(pixel) +
                                locked.pitch,
                            &neighborCount);
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    components[2] = static_cast<unsigned char>(sums[0]);
                    components[1] = static_cast<unsigned char>(sums[1]);
                    components[0] = static_cast<unsigned char>(sums[2]);
                }
            }
        }
        break;

    case D3D9_VIEW_FMT_A1R5G5B5:
        for (unsigned int y = 0; y < description.height; ++y)
        {
            AnmArgb1555PixelView *pixel =
                reinterpret_cast<AnmArgb1555PixelView *>(
                    static_cast<unsigned char *>(locked.bits) +
                    locked.pitch * y);
            for (unsigned int x = 0; x < description.width; ++x, ++pixel)
            {
                if (pixel->alpha == 0)
                {
                    unsigned int sums[3] = {0, 0, 0};
                    unsigned int neighborCount = 0;
                    if (x > 0)
                        AccumulateArgb1555Neighbor(
                            sums, pixel - 1, &neighborCount);
                    if (x < description.width - 1)
                        AccumulateArgb1555Neighbor(
                            sums, pixel + 1, &neighborCount);
                    if (y > 0)
                        AccumulateArgb1555Neighbor(
                            sums, reinterpret_cast<AnmArgb1555PixelView *>(
                                reinterpret_cast<unsigned char *>(pixel) -
                                locked.pitch),
                            &neighborCount);
                    if (y < description.height - 1)
                        AccumulateArgb1555Neighbor(
                            sums, reinterpret_cast<AnmArgb1555PixelView *>(
                                reinterpret_cast<unsigned char *>(pixel) +
                                locked.pitch),
                            &neighborCount);
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    pixel->red = static_cast<unsigned short>(sums[0]);
                    pixel->green = static_cast<unsigned short>(sums[1]);
                    pixel->blue = static_cast<unsigned short>(sums[2]);
                }
            }
        }
        break;

    case D3D9_VIEW_FMT_A4R4G4B4:
        for (unsigned int y = 0; y < description.height; ++y)
        {
            AnmArgb4444PixelView *pixel =
                reinterpret_cast<AnmArgb4444PixelView *>(
                    static_cast<unsigned char *>(locked.bits) +
                    locked.pitch * y);
            for (unsigned int x = 0; x < description.width; ++x, ++pixel)
            {
                if (pixel->alpha == 0)
                {
                    unsigned int sums[3] = {0, 0, 0};
                    unsigned int neighborCount = 0;
                    if (x > 0)
                        AccumulateArgb4444Neighbor(
                            sums, pixel - 1, &neighborCount);
                    if (x < description.width - 1)
                        AccumulateArgb4444Neighbor(
                            sums, pixel + 1, &neighborCount);
                    if (y > 0)
                        AccumulateArgb4444Neighbor(
                            sums, reinterpret_cast<AnmArgb4444PixelView *>(
                                reinterpret_cast<unsigned char *>(pixel) -
                                locked.pitch),
                            &neighborCount);
                    if (y < description.height - 1)
                        AccumulateArgb4444Neighbor(
                            sums, reinterpret_cast<AnmArgb4444PixelView *>(
                                reinterpret_cast<unsigned char *>(pixel) +
                                locked.pitch),
                            &neighborCount);
                    if (neighborCount > 1)
                    {
                        sums[0] /= neighborCount;
                        sums[1] /= neighborCount;
                        sums[2] /= neighborCount;
                    }
                    pixel->red = static_cast<unsigned short>(sums[0]);
                    pixel->green = static_cast<unsigned short>(sums[1]);
                    pixel->blue = static_cast<unsigned short>(sums[2]);
                }
            }
        }
        break;
    }

    surface->vtable->UnlockRect(surface);
    surface->vtable->Release(surface);
}

static int NormalizeAnmTextureFormat(int format)
{
    if ((g_AnmHardwareFlags & 1) != 0)
    {
        unsigned int deviceFormat = g_AnmTextureFormats[format];
        if (deviceFormat == D3D9_VIEW_FMT_A8R8G8B8 ||
            deviceFormat == D3D9_VIEW_FMT_UNKNOWN)
        {
            format = 5;
        }
        else if (deviceFormat == 20)
        {
            format = 3;
        }
    }
    return format;
}

// Target 0x00446C70 uploads either an external image or the embedded texture
// payload of one raw ANM entry into an existing texture. The target keeps the
// otherwise-unused argument present in the source ABI.
int AnmRenderManagerView::LoadTexture(
    AnmTextureEntryView *entry, unsigned char *data, int size, int format,
    int unused, int hasData)
{
    D3d9SurfaceView *surface = NULL;
    D3d9RectView sourceRect;
    AnmRawEntryView *rawEntry;
    AnmTextureHeaderView *header;

    (void)unused;
    format = NormalizeAnmTextureFormat(format);
    entry->rawDataSize = size;
    entry->texture->vtable->GetSurfaceLevel(entry->texture, 0, &surface);

    if (hasData == 0)
    {
        D3DXLoadSurfaceFromFileInMemory(
            surface, NULL, NULL, data, size, NULL,
            D3D9_VIEW_TEXF_NONE, 0, NULL);
    }
    else
    {
        rawEntry = reinterpret_cast<AnmRawEntryView *>(data);
        header = reinterpret_cast<AnmTextureHeaderView *>(
            data + rawEntry->textureOffset);
        sourceRect.left = 0;
        sourceRect.top = 0;
        sourceRect.right = header->width;
        sourceRect.bottom = header->height;
        D3DXLoadSurfaceFromMemory(
            surface, NULL, NULL,
            reinterpret_cast<unsigned char *>(header) + sizeof(*header),
            g_AnmTextureFormats[header->format],
            g_AnmTextureBytesPerPixel[header->format] * header->width,
            NULL, &sourceRect, D3D9_VIEW_TEXF_NONE, 0);
    }

    surface->vtable->Release(surface);
    ApplyTextureAlphaBleed(entry);
    entry->bytesPerPixel = g_AnmTextureBytesPerPixel[format];
    return 0;
}

// Target 0x00446D70 is the vertically-offset form used when several source
// images share one destination texture.
int AnmRenderManagerView::LoadTextureRegion(
    AnmTextureEntryView *entry, unsigned char *data, int size, int format,
    int unused, int hasData, int top)
{
    D3d9SurfaceView *surface = NULL;
    D3d9SurfaceDescriptionView description;
    D3d9RectView fileDestinationRect;
    D3d9RectView sourceRect;
    D3d9RectView dataDestinationRect;
    AnmRawEntryView *rawEntry;
    AnmTextureHeaderView *header;

    (void)unused;
    format = NormalizeAnmTextureFormat(format);
    entry->rawDataSize = size;
    entry->texture->vtable->GetSurfaceLevel(entry->texture, 0, &surface);

    if (hasData == 0)
    {
        surface->vtable->GetDesc(surface, &description);
        fileDestinationRect.left = 0;
        fileDestinationRect.top = top;
        fileDestinationRect.right = description.width;
        fileDestinationRect.bottom = description.height;
        D3DXLoadSurfaceFromFileInMemory(
            surface, NULL, &fileDestinationRect, data, size, NULL,
            D3D9_VIEW_TEXF_NONE, 0, NULL);
    }
    else
    {
        rawEntry = reinterpret_cast<AnmRawEntryView *>(data);
        header = reinterpret_cast<AnmTextureHeaderView *>(
            data + rawEntry->textureOffset);
        sourceRect.left = 0;
        sourceRect.top = 0;
        sourceRect.right = header->width;
        sourceRect.bottom = header->height;
        dataDestinationRect.left = 0;
        dataDestinationRect.top = top;
        dataDestinationRect.right = header->width;
        dataDestinationRect.bottom = header->height + top;
        D3DXLoadSurfaceFromMemory(
            surface, NULL, &dataDestinationRect,
            reinterpret_cast<unsigned char *>(header) + sizeof(*header),
            g_AnmTextureFormats[header->format],
            g_AnmTextureBytesPerPixel[header->format] * header->width,
            NULL, &sourceRect, D3D9_VIEW_TEXF_NONE, 0);
    }

    surface->vtable->Release(surface);
    ApplyTextureAlphaBleed(entry);
    entry->bytesPerPixel = g_AnmTextureBytesPerPixel[format];
    return 0;
}

// Target 0x00446EB0 creates an explicitly-sized managed texture from an
// external image already held in memory.
int AnmRenderManagerView::CreateTextureFromFile(
    AnmTextureEntryView *entry, int format, unsigned int colorKey,
    int width, int height)
{
    format = NormalizeAnmTextureFormat(format);
    if (D3DXCreateTextureFromFileInMemoryEx(
            g_Direct3DDevice, entry->rawData, entry->rawDataSize,
            width, height, 0, 0, g_AnmTextureFormats[format],
            D3D9_VIEW_POOL_MANAGED, D3D9_VIEW_TEXF_NONE,
            static_cast<unsigned int>(-1), colorKey, NULL, NULL,
            &entry->texture) != 0)
    {
        return -1;
    }

    ApplyTextureAlphaBleed(entry);
    entry->bytesPerPixel = g_AnmTextureBytesPerPixel[format];
    return 0;
}

// Target 0x00446F40 creates the destination at the dimensions requested by
// the caller, then uploads the dimensions and pixel format serialized in the
// embedded ANM texture header.
int AnmRenderManagerView::CreateTextureFromAnm(
    D3d9TextureView **texture, void *textureData, int format,
    int width, int height)
{
    D3d9SurfaceView *surface = NULL;
    D3d9RectView sourceRect;
    AnmTextureHeaderView *header =
        static_cast<AnmTextureHeaderView *>(textureData);

    format = NormalizeAnmTextureFormat(format);
    sourceRect.left = 0;
    sourceRect.top = 0;
    sourceRect.right = header->width;
    sourceRect.bottom = header->height;

    if (D3DXCreateTexture(
            g_Direct3DDevice, width, height, 1, 0,
            g_AnmTextureFormats[format], D3D9_VIEW_POOL_MANAGED,
            texture) != 0)
    {
        if (surface != NULL)
            surface->vtable->Release(surface);
        return -1;
    }

    (*texture)->vtable->GetSurfaceLevel(*texture, 0, &surface);
    D3DXLoadSurfaceFromMemory(
        surface, NULL, &sourceRect,
        reinterpret_cast<unsigned char *>(header) + sizeof(*header),
        g_AnmTextureFormats[header->format],
        g_AnmTextureBytesPerPixel[header->format] * header->width,
        NULL, &sourceRect, D3D9_VIEW_TEXF_NONE, 0);
    reinterpret_cast<AnmTextureEntryView *>(texture)->bytesPerPixel =
        g_AnmTextureBytesPerPixel[format];

    if (surface != NULL)
        surface->vtable->Release(surface);
    return 0;
}

// Target 0x00447050 creates a blank managed texture and records its stride.
int AnmRenderManagerView::CreateEmptyTexture(
    D3d9TextureView **texture, int width, int height, int format)
{
    D3DXCreateTexture(
        g_Direct3DDevice, width, height, 1, 0,
        g_AnmTextureFormats[format], D3D9_VIEW_POOL_MANAGED, texture);
    reinterpret_cast<AnmTextureEntryView *>(texture)->bytesPerPixel =
        g_AnmTextureBytesPerPixel[format];
    return 0;
}

// Target 0x00447080 performs the synchronous form of the two-phase ANM load.
// It is a retained CC-delimited body that Ghidra's function inventory missed.
AnmLoadedView *AnmRenderManagerView::LoadAnm(
    int index, const char *path)
{
    AnmLoadedView *loaded = ReadAnmEntries(index, path);
    if (loaded != NULL)
    {
        loaded->pendingLoadCount = 1;
        while (loaded->pendingLoadCount != 0)
            loaded = PostloadAnmEntry(loaded);
    }
    return loaded;
}

// Target 0x004470C0 reads and inventories all linked version-four raw ANM
// entries, then allocates the three aggregate tables consumed by postload.
AnmLoadedView *AnmRenderManagerView::ReadAnmEntries(
    int index, const char *path)
{
    char filePath[256];
    AnmRawEntryView *rawData;
    AnmRawEntryView *entry;
    AnmLoadedView *loaded;
    int totalEntries = 0;
    int totalScripts = 0;
    int totalSprites = 0;

    if (index >= 33)
    {
        g_AnmErrorLoggerView.Log(
            "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x8a\x69"
            "\x94\x5b\x90\xe6\x82\xaa\x91\xab\x82\xe8\x82\xdc"
            "\x82\xb9\x82\xf1\r\n");
        return NULL;
    }

    sprintf(filePath, "%s", path);
    rawData = reinterpret_cast<AnmRawEntryView *>(
        FileSystem::OpenFile(filePath, NULL, 0));
    loaded = new AnmLoadedView;
    if (loaded != NULL)
        memset(loaded, 0, sizeof(*loaded));
    loadedAnms[index] = loaded;
    if (rawData == NULL)
        return NULL;

    loaded->anmFileIndex = index;
    loaded->rawData = rawData;
    strcpy(loaded->path, path);

    entry = rawData;
    do
    {
        ++totalEntries;
        totalScripts += entry->numScripts;
        totalSprites += entry->numSprites;
        if (entry->nextOffset == 0)
            break;
        entry = reinterpret_cast<AnmRawEntryView *>(
            reinterpret_cast<unsigned char *>(entry) + entry->nextOffset);
    } while (true);

    loaded->totalEntries = totalEntries;
    loaded->textures = static_cast<AnmTextureEntryView *>(
        malloc(totalEntries * sizeof(AnmTextureEntryView)));
    memset(loaded->textures, 0,
           totalEntries * sizeof(AnmTextureEntryView));
    loaded->sprites = static_cast<AnmSpriteView *>(
        malloc(totalSprites * sizeof(AnmSpriteView)));
    loaded->scripts = static_cast<AnmRawInstructionView **>(
        malloc(totalScripts * sizeof(AnmRawInstructionView *)));
    loaded->totalScripts = totalScripts;
    loaded->totalSprites = totalSprites;

    entry = rawData;
    for (int entryNumber = 0;; ++entryNumber)
    {
        if (LoadExternalTextureData(
                loaded, entryNumber, &totalSprites, &totalScripts,
                entry) < 0)
        {
            return NULL;
        }
        if (entry->nextOffset == 0)
            break;
        entry = reinterpret_cast<AnmRawEntryView *>(
            reinterpret_cast<unsigned char *>(entry) + entry->nextOffset);
    }
    return loaded;
}

// Target 0x00447280 publishes one pending entry and waits for the main-thread
// service callback to consume it. Existing slots are returned immediately.
AnmLoadedView *AnmRenderManagerView::PreloadAnm(
    int index, const char *path)
{
    AnmLoadedView *loaded = loadedAnms[index];
    if (loaded != NULL)
        return loaded;

    loaded = ReadAnmEntries(index, path);
    if (loaded == NULL)
        return NULL;

    loaded->pendingLoadCount = 1;
    do
    {
        if (g_AnmPreloadStopRequested < 0)
            break;
        Sleep(1);
    } while (loaded->pendingLoadCount != 0);
    return loaded;
}

// Target 0x004472E0 acquires external image data during the read phase. The
// aggregate-count pointers survive in the source interface but are optimized
// away because this phase does not modify them.
int AnmRenderManagerView::LoadExternalTextureData(
    AnmLoadedView *loaded, int entryNumber, int *spriteCount,
    int *scriptCount, AnmRawEntryView *rawEntry)
{
    char filePath[256];
    const char *texturePath;
    unsigned char *fileData;
    int fileSize;

    (void)spriteCount;
    (void)scriptCount;
    if (rawEntry == NULL)
    {
        g_AnmErrorLoggerView.Log(
            "\x83\x41\x83\x6a\x83\x81\x82\xaa\x93\xc7\x82\xdd"
            "\x8d\x9e\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\xb8\x82\xed"
            "\x82\xea\x82\xc4\x82\xe9\x82\xa9\x89\xf3\x82\xea"
            "\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    if (rawEntry->version != 4)
    {
        g_AnmErrorLoggerView.Log(
            "\x83\x41\x83\x6a\x83\x81\x82\xcc\x83\x6f\x81\x5b"
            "\x83\x57\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return -1;
    }

    if (rawEntry->hasData == 0)
    {
        texturePath = reinterpret_cast<const char *>(rawEntry) +
            rawEntry->nameOffset;
        if (texturePath[0] != '@')
        {
            sprintf(filePath, "%s", texturePath);
            fileData = FileSystem::OpenFile(filePath, &fileSize, 1);
            if (fileData == NULL)
            {
                g_AnmErrorLoggerView.Log(
                    "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83 %s "
                    "\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xdc"
                    "\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e"
                    "\x82\xaa\x8e\xb8\x82\xed\x82\xea\x82\xc4\x82\xe9"
                    "\x82\xa9\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc"
                    "\x82\xb7\r\n",
                    texturePath);
                return -1;
            }
            loaded->textures[entryNumber].rawDataSize = fileSize;
            loaded->textures[entryNumber].rawData = fileData;
        }
    }
    return 1;
}

// Target 0x004473C0 advances the two-phase loader by exactly one raw entry.
AnmLoadedView *AnmRenderManagerView::PostloadAnmEntry(
    AnmLoadedView *loaded)
{
    AnmRawEntryView *rawEntry =
        static_cast<AnmRawEntryView *>(loaded->rawData);
    int entryLoadNumber = 0;
    int currentScripts = 0;
    int currentSprites = 0;
    int entryNumber = 0;
    int loadedOne = 0;

    for (;;)
    {
        if (entryLoadNumber == loaded->pendingLoadCount - 1)
        {
            if (LoadTextureData(
                    loaded, entryNumber, currentSprites, currentScripts,
                    rawEntry) < 0)
            {
                loaded->pendingLoadCount = 0;
                return NULL;
            }
            loadedOne = 1;
        }

        currentSprites += rawEntry->numSprites;
        currentScripts += rawEntry->numScripts;
        ++entryNumber;
        if (rawEntry->nextOffset == 0)
            break;
        rawEntry = reinterpret_cast<AnmRawEntryView *>(
            reinterpret_cast<unsigned char *>(rawEntry) +
            rawEntry->nextOffset);
        ++entryLoadNumber;
        if (entryLoadNumber == loaded->pendingLoadCount || loadedOne != 0)
        {
            ++loaded->pendingLoadCount;
            return loaded;
        }
    }

    loaded->pendingLoadCount = 0;
    return loaded;
}

// Target 0x00447470 creates one entry texture, publishes all of its sprites,
// and records the instruction pointers for all scripts following the sprite
// offset table.
int AnmRenderManagerView::LoadTextureData(
    AnmLoadedView *loaded, int entryNumber, int spriteCount,
    int scriptCount, AnmRawEntryView *rawEntry)
{
    D3d9SurfaceDescriptionView description;
    AnmSpriteView sprite;
    const char *texturePath;
    unsigned int *offset;

    if (rawEntry == NULL)
    {
        g_AnmErrorLoggerView.Log(
            "\x83\x41\x83\x6a\x83\x81\x82\xaa\x93\xc7\x82\xdd"
            "\x8d\x9e\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8e\xb8\x82\xed"
            "\x82\xea\x82\xc4\x82\xe9\x82\xa9\x89\xf3\x82\xea"
            "\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    if (rawEntry->version != 4)
    {
        g_AnmErrorLoggerView.Log(
            "\x83\x41\x83\x6a\x83\x81\x82\xcc\x83\x6f\x81\x5b"
            "\x83\x57\x83\x87\x83\x93\x82\xaa\x88\xe1\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return -1;
    }

    if (rawEntry->hasData == 0)
    {
        texturePath = reinterpret_cast<const char *>(rawEntry) +
            rawEntry->nameOffset;
        if (texturePath[0] == '@')
        {
            CreateEmptyTexture(
                &loaded->textures[entryNumber].texture,
                rawEntry->width, rawEntry->height, rawEntry->format);
        }
        else if (CreateTextureFromFile(
                     &loaded->textures[entryNumber], rawEntry->format,
                     rawEntry->colorKey, rawEntry->width,
                     rawEntry->height) != 0)
        {
            g_AnmErrorLoggerView.Log(
                "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83 %s "
                "\x82\xaa\x8d\xec\x90\xac\x82\xc5\x82\xab\x82\xdc"
                "\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e"
                "\x82\xaa\x8e\xb8\x82\xed\x82\xea\x82\xc4\x82\xe9"
                "\x82\xa9\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc"
                "\x82\xb7\r\n",
                texturePath);
            return -1;
        }
    }
    else if (CreateTextureFromAnm(
                 &loaded->textures[entryNumber].texture,
                 reinterpret_cast<unsigned char *>(rawEntry) +
                     rawEntry->textureOffset,
                 rawEntry->format, rawEntry->width,
                 rawEntry->height) != 0)
    {
        g_AnmErrorLoggerView.Log(
            "\x83\x65\x83\x4e\x83\x58\x83\x60\x83\x83\x82\xaa"
            "\x93\xc7\x82\xdd\x8d\x9e\x82\xdf\x82\xdc\x82\xb9"
            "\x82\xf1\x81\x42\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8e\xb8\x82\xed\x82\xea\x82\xc4\x82\xe9\x82\xa9"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return -1;
    }

    D3d9TextureView *texture = loaded->textures[entryNumber].texture;
    texture->vtable->SetPriority(texture, rawEntry->priority);
    texture->vtable->PreLoad(texture);
    texture->vtable->GetLevelDesc(texture, 0, &description);

    offset = reinterpret_cast<unsigned int *>(rawEntry + 1);
    for (int i = 0; i < rawEntry->numSprites; ++i, ++offset)
    {
        AnmRawSpriteView *rawSprite =
            reinterpret_cast<AnmRawSpriteView *>(
                reinterpret_cast<unsigned char *>(rawEntry) + *offset);
        sprite.anmFileIndex = loaded->anmFileIndex;
        sprite.texture = texture;
        sprite.horizontalScale =
            static_cast<float>(description.width) / rawEntry->width;
        sprite.verticalScale =
            static_cast<float>(description.height) / rawEntry->height;
        sprite.sourceX = rawSprite->x * sprite.horizontalScale;
        sprite.sourceY = rawSprite->y * sprite.verticalScale;
        sprite.sourceRight =
            (rawSprite->x + rawSprite->width) * sprite.horizontalScale;
        sprite.sourceBottom =
            (rawSprite->y + rawSprite->height) * sprite.verticalScale;
        sprite.textureWidth = static_cast<float>(description.width);
        sprite.textureHeight = static_cast<float>(description.height);
        loaded->LoadSprite(spriteCount++, &sprite);
    }

    for (int i = 0; i < rawEntry->numScripts; ++i, offset += 2)
    {
        loaded->scripts[scriptCount++] =
            reinterpret_cast<AnmRawInstructionView *>(
                reinterpret_cast<unsigned char *>(rawEntry) + offset[1]);
    }
    return 1;
}

// Target 0x00447700 services one pending entry per manager tick. Its release
// branch really clears the pointer and then writes through that cleared slot;
// this apparent target bug is preserved as observed.
int AnmRenderManagerView::ServicePreloadedAnms()
{
    for (int i = 0; i < 33; ++i)
    {
        AnmLoadedView *loaded = loadedAnms[i];
        if (loaded == NULL)
            continue;
        if (loaded->releasePending != 0)
        {
            ReleaseAnm(i);
            loadedAnms[i]->releasePending = 0;
        }
        else if (loaded->pendingLoadCount != 0)
        {
            return PostloadAnmEntry(loaded) == NULL ? -1 : 0;
        }
    }
    return 0;
}

// Target 0x004477D0 validates a 33-slot index, releases the loaded owner, and
// deletes the 0x130-byte allocation.
void AnmRenderManagerView::ReleaseAnm(int index)
{
    if (index < 0 || index >= 33)
        return;
    AnmLoadedView *loaded = loadedAnms[index];
    if (loaded != NULL)
    {
        loaded->Release();
        delete loaded;
        loadedAnms[index] = NULL;
    }
}

// Target 0x00447810 tears down every allocation owned by one loaded ANM. Raw
// image buffers belong to their texture entry; the parsed ANM blob is freed
// only after the aggregate tables and optional map data.
void AnmLoadedView::Release()
{
    if (rawData == NULL)
        return;

    g_AnmRenderManagerView->MarkLoadedVmsForDeletion(this);
    for (int i = 0; i < totalEntries; ++i)
    {
        if (textures[i].texture != NULL)
        {
            textures[i].texture->vtable->Release(textures[i].texture);
            textures[i].texture = NULL;
        }
        if (textures[i].rawData != NULL)
        {
            free(textures[i].rawData);
            textures[i].rawData = NULL;
        }
    }

    if (textures != NULL)
    {
        free(textures);
        textures = NULL;
    }
    if (sprites != NULL)
    {
        free(sprites);
        sprites = NULL;
    }
    if (scripts != NULL)
    {
        free(scripts);
        scripts = NULL;
    }
    if (mapData != NULL)
    {
        free(mapData);
        mapData = NULL;
    }
    if (rawData != NULL)
    {
        free(rawData);
        rawData = NULL;
    }
}

// Target 0x00447940 publishes one 0x44-byte sprite and derives normalized UV
// coordinates plus the unscaled pixel dimensions used by ANM VMs.
void AnmLoadedView::LoadSprite(
    int spriteIndex, AnmSpriteView *sprite)
{
    sprites[spriteIndex] = *sprite;
    sprites[spriteIndex].uStart =
        sprites[spriteIndex].sourceX / sprites[spriteIndex].textureWidth;
    sprites[spriteIndex].uEnd =
        sprites[spriteIndex].sourceRight / sprites[spriteIndex].textureWidth;
    sprites[spriteIndex].vStart =
        sprites[spriteIndex].sourceY / sprites[spriteIndex].textureHeight;
    sprites[spriteIndex].vEnd =
        sprites[spriteIndex].sourceBottom / sprites[spriteIndex].textureHeight;
    sprites[spriteIndex].width =
        (sprites[spriteIndex].sourceRight - sprites[spriteIndex].sourceX) /
        sprite->horizontalScale;
    sprites[spriteIndex].height =
        (sprites[spriteIndex].sourceBottom - sprites[spriteIndex].sourceY) /
        sprite->verticalScale;
}

// Target 0x004479D0 converts one sprite's source rectangle to integer pixels
// and routes formatted text through one of the two target text rasterizers.
void AnmRenderManagerView::DrawTextInner(
    D3d9TextureView *texture, AnmSpriteView *sprite,
    int x, int textGlyphWidth,
    unsigned int color, const char *text,
    int useAlternateRenderer)
{
    D3d9RectView rectangle;

    if (textGlyphWidth <= 0)
        textGlyphWidth = 17;
    else if (textGlyphWidth <= 8)
        return;

    rectangle.left = static_cast<int>(sprite->sourceX);
    rectangle.top = static_cast<int>(sprite->sourceY);
    rectangle.right = static_cast<int>(sprite->sourceRight);
    rectangle.bottom = static_cast<int>(sprite->sourceBottom);
    if (useAlternateRenderer == 0)
    {
        TextHelperView::RenderTextToTexture(
            &rectangle, x, textGlyphWidth, color, text, texture);
    }
    else
    {
        TextHelperView::RenderTextToTextureAlternate(
            &rectangle, x, textGlyphWidth, color, text, texture);
    }
}

#define textBuffer restartCommandProcessingLocal05
#define textGlyphWidth averagedPanLocal12
#define textX textXLocal00

// Target 0x00447A50 formats into the sprite rectangle with no horizontal
// offset. The renderer selector is VM textFlags360 bit one.
#pragma var_order(textBuffer, textGlyphWidth)
void AnmRenderManagerView::DrawTextLeft(
    AnmVmView *vm, unsigned int color, const char *format, ...)
{
    char textBuffer[128];
    int textGlyphWidth = vm->glyphWidth;
    va_list args;

    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);

    DrawTextInner(
        vm->loadedSprite->texture, vm->loadedSprite, 0,
        textGlyphWidth, color, textBuffer,
        (vm->textFlags360 >> 1) & 1);
    vm->flags35C |= 1;
}

// Target 0x00447AE0 right-aligns using the unscaled sprite width and a
// target-observed one-pixel overlap between adjacent glyph cells.
#pragma var_order(textBuffer, textGlyphWidth)
void AnmRenderManagerView::DrawTextRight(
    AnmVmView *vm, unsigned int color, const char *format, ...)
{
    char textBuffer[128];
    int textX;
    int textGlyphWidth = vm->glyphWidth <= 0 ? 17 : vm->glyphWidth;
    va_list args;

    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);

    textX = static_cast<int>(
        vm->loadedSprite->width -
        strlen(textBuffer) * (textGlyphWidth - 1) / 2);
    DrawTextInner(
        vm->loadedSprite->texture, vm->loadedSprite, textX,
        textGlyphWidth, color, textBuffer,
        (vm->textFlags360 >> 1) & 1);
    vm->flags35C |= 1;
}

// Target 0x00447BB0 centers text in the integer sprite width. Division of
// both the sprite and glyph span is integral in the target.
#pragma var_order(textBuffer, textGlyphWidth)
void AnmRenderManagerView::DrawTextCentered(
    AnmVmView *vm, unsigned int color, const char *format, ...)
{
    char textBuffer[128];
    int textX;
    int textGlyphWidth = vm->glyphWidth <= 0 ? 17 : vm->glyphWidth;
    va_list args;

    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);

    textX = static_cast<int>(vm->loadedSprite->width) / 2 -
        strlen(textBuffer) * (textGlyphWidth - 1) / 4;
    DrawTextInner(
        vm->loadedSprite->texture, vm->loadedSprite, textX,
        textGlyphWidth, color, textBuffer,
        (vm->textFlags360 >> 1) & 1);
    vm->flags35C |= 1;
}

#undef textBuffer
#undef textGlyphWidth
#undef textX

// Target 0x00447C80 consumes either a preloaded image or a synchronous file,
// decodes it through a temporary 640x1024 surface, and preserves two copies:
// a render-target-capable primary surface and an offscreen backup surface.
int AnmRenderManagerView::LoadSurface(int surfaceIndex, const char *path)
{
    char filePath[256];
    unsigned char *fileData;
    int fileSize;
    D3d9SurfaceView *surface;

    if (surfaces[surfaceIndex] != NULL)
        ReleaseSurface(surfaceIndex);

    if (surfaceData[surfaceIndex] == NULL)
    {
        sprintf(filePath, "%s", path);
        fileData = FileSystem::OpenFile(filePath, &fileSize, 0);
        if (fileData == NULL)
        {
            g_AnmErrorLoggerView.Log(
                "%s\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf"
                "\x82\xc8\x82\xa2\x82\xc5\x82\xb7\x81\x42\r\n",
                path);
            return -1;
        }
    }
    else
    {
        fileData = surfaceData[surfaceIndex];
        fileSize = surfaceDataSizes[surfaceIndex];
        surfaceData[surfaceIndex] = NULL;
    }

    if (g_Direct3DDevice->vtable->CreateOffscreenPlainSurface(
            g_Direct3DDevice, 640, 1024, g_AnmBackbufferFormat, 3,
            &surface, NULL) != 0)
    {
        return -1;
    }

    if (D3DXLoadSurfaceFromFileInMemory(
            surface, NULL, NULL, fileData, fileSize, NULL, 1, 0,
            &surfaceInfo[surfaceIndex]) != 0)
    {
        goto error;
    }

    if (g_Direct3DDevice->vtable->CreateRenderTarget(
            g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
            surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
            0, 0, 1, &surfaces[surfaceIndex], NULL) != 0)
    {
        if (g_Direct3DDevice->vtable->CreateOffscreenPlainSurface(
                g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
                surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
                3, &surfaces[surfaceIndex], NULL) != 0)
        {
            goto error;
        }
    }

    if (g_Direct3DDevice->vtable->CreateOffscreenPlainSurface(
            g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
            surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
            3, &secondarySurfaces[surfaceIndex], NULL) != 0)
    {
        goto error;
    }

    if (D3DXLoadSurfaceFromSurface(
            surfaces[surfaceIndex], NULL, NULL, surface, NULL, NULL,
            1, 0) != 0)
    {
        goto error;
    }
    if (D3DXLoadSurfaceFromSurface(
            secondarySurfaces[surfaceIndex], NULL, NULL, surface,
            NULL, NULL, 1, 0) != 0)
    {
        goto error;
    }

    if (surface != NULL)
    {
        surface->vtable->Release(surface);
        surface = NULL;
    }
    free(fileData);
    return 0;

error:
    if (surface != NULL)
    {
        surface->vtable->Release(surface);
        surface = NULL;
    }
    free(fileData);
    return -1;
}

// Target 0x00447EC0 stages raw surface bytes for a later LoadSurface call.
int AnmRenderManagerView::PreloadSurface(int surfaceIndex, const char *path)
{
    char filePath[256];
    int fileSize;
    unsigned char *fileData;

    if (surfaces[surfaceIndex] != NULL)
        ReleaseSurface(surfaceIndex);

    sprintf(filePath, "%s", path);
    fileData = FileSystem::OpenFile(filePath, &fileSize, 0);
    if (fileData == NULL)
    {
        g_AnmErrorLoggerView.Log(
            "%s\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf"
            "\x82\xc8\x82\xa2\x82\xc5\x82\xb7\x81\x42\r\n",
            path);
        return -1;
    }

    surfaceData[surfaceIndex] = fileData;
    surfaceDataSizes[surfaceIndex] = fileSize;
    return 0;
}

// Target 0x00447F70 releases the live pair and any still-staged file buffer.
void AnmRenderManagerView::ReleaseSurface(int surfaceIndex)
{
    if (surfaces[surfaceIndex] != NULL)
    {
        surfaces[surfaceIndex]->vtable->Release(surfaces[surfaceIndex]);
        surfaces[surfaceIndex] = NULL;
    }
    if (secondarySurfaces[surfaceIndex] != NULL)
    {
        secondarySurfaces[surfaceIndex]->vtable->Release(
            secondarySurfaces[surfaceIndex]);
        secondarySurfaces[surfaceIndex] = NULL;
    }
    if (surfaceData[surfaceIndex] != NULL)
    {
        free(surfaceData[surfaceIndex]);
        surfaceData[surfaceIndex] = NULL;
    }
    surfaceData[surfaceIndex] = NULL;
}

// Target 0x00447FD0 restores the primary surface lazily from its offscreen
// backup, then updates the requested point on the current backbuffer.
void AnmRenderManagerView::CopySurfaceToBackbuffer(
    int surfaceIndex, int left, int top, int x, int y)
{
    D3d9SurfaceView *backbuffer;
    D3d9RectView sourceRect;
    D3d9PointView destinationPoint;

    if (secondarySurfaces[surfaceIndex] == NULL)
        return;
    if (g_Direct3DDevice->vtable->GetBackBuffer(
            g_Direct3DDevice, 0, 0, 0, &backbuffer) != 0)
    {
        return;
    }

    if (surfaces[surfaceIndex] == NULL)
    {
        if (g_Direct3DDevice->vtable->CreateRenderTarget(
                g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
                surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
                0, 0, 1, &surfaces[surfaceIndex], NULL) != 0)
        {
            if (g_Direct3DDevice->vtable->CreateOffscreenPlainSurface(
                    g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
                    surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
                    3, &surfaces[surfaceIndex], NULL) != 0)
            {
                backbuffer->vtable->Release(backbuffer);
                return;
            }
        }
        if (D3DXLoadSurfaceFromSurface(
                surfaces[surfaceIndex], NULL, NULL,
                secondarySurfaces[surfaceIndex], NULL, NULL, 1, 0) != 0)
        {
            backbuffer->vtable->Release(backbuffer);
            return;
        }
    }

    sourceRect.left = left;
    sourceRect.top = top;
    sourceRect.right = surfaceInfo[surfaceIndex].width;
    sourceRect.bottom = surfaceInfo[surfaceIndex].height;
    destinationPoint.x = x;
    destinationPoint.y = y;
    g_Direct3DDevice->vtable->UpdateSurface(
        g_Direct3DDevice, surfaces[surfaceIndex], &sourceRect,
        backbuffer, &destinationPoint);
    backbuffer->vtable->Release(backbuffer);
}

// Target 0x00448120 is the bounded-rectangle variant of the same operation.
void AnmRenderManagerView::CopySurfaceToBackbuffer2(
    int surfaceIndex, int destinationX, int destinationY,
    int sourceX, int sourceY, int width, int height)
{
    D3d9SurfaceView *backbuffer;
    D3d9RectView sourceRect;
    D3d9PointView destinationPoint;

    if (secondarySurfaces[surfaceIndex] == NULL)
        return;
    if (g_Direct3DDevice->vtable->GetBackBuffer(
            g_Direct3DDevice, 0, 0, 0, &backbuffer) != 0)
    {
        return;
    }

    if (surfaces[surfaceIndex] == NULL)
    {
        if (g_Direct3DDevice->vtable->CreateRenderTarget(
                g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
                surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
                0, 0, 1, &surfaces[surfaceIndex], NULL) != 0)
        {
            if (g_Direct3DDevice->vtable->CreateOffscreenPlainSurface(
                    g_Direct3DDevice, surfaceInfo[surfaceIndex].width,
                    surfaceInfo[surfaceIndex].height, g_AnmBackbufferFormat,
                    3, &surfaces[surfaceIndex], NULL) != 0)
            {
                backbuffer->vtable->Release(backbuffer);
                return;
            }
        }
        if (D3DXLoadSurfaceFromSurface(
                surfaces[surfaceIndex], NULL, NULL,
                secondarySurfaces[surfaceIndex], NULL, NULL, 1, 0) != 0)
        {
            backbuffer->vtable->Release(backbuffer);
            return;
        }
    }

    sourceRect.left = sourceX;
    sourceRect.top = sourceY;
    sourceRect.right = sourceX + width;
    sourceRect.bottom = sourceY + height;
    destinationPoint.x = destinationX;
    destinationPoint.y = destinationY;
    g_Direct3DDevice->vtable->UpdateSurface(
        g_Direct3DDevice, surfaces[surfaceIndex], &sourceRect,
        backbuffer, &destinationPoint);
    backbuffer->vtable->Release(backbuffer);
}

// Target 0x00448270 copies a backbuffer rectangle into one ANM texture entry.
void AnmRenderManagerView::CaptureToTexture(
    int anmIndex, int entryIndex, int sourceX, int sourceY,
    int sourceWidth, int sourceHeight, int destinationX,
    int destinationY, int destinationWidth, int destinationHeight)
{
    D3d9SurfaceView *backbuffer;
    D3d9SurfaceView *textureSurface;
    D3d9RectView sourceRect;
    D3d9RectView destinationRect;
    D3d9TextureView *texture = loadedAnms[anmIndex]->textures[entryIndex].texture;

    if (texture == NULL)
        return;
    FlushVertexBuffer();
    if (g_Direct3DDevice->vtable->GetBackBuffer(
            g_Direct3DDevice, 0, 0, 0, &backbuffer) != 0)
    {
        return;
    }
    if (texture->vtable->GetSurfaceLevel(
            texture, 0, &textureSurface) != 0)
    {
        backbuffer->vtable->Release(backbuffer);
        return;
    }

    sourceRect.left = sourceX;
    sourceRect.top = sourceY;
    sourceRect.right = sourceX + sourceWidth;
    sourceRect.bottom = sourceY + sourceHeight;
    destinationRect.left = destinationX;
    destinationRect.top = destinationY;
    destinationRect.right = destinationX + destinationWidth;
    destinationRect.bottom = destinationY + destinationHeight;
    if (D3DXLoadSurfaceFromSurface(
            textureSurface, NULL, &destinationRect, backbuffer, NULL,
            &sourceRect, 2, 0) != 0)
    {
        textureSurface->vtable->Release(textureSurface);
        backbuffer->vtable->Release(backbuffer);
        return;
    }
    textureSurface->vtable->Release(textureSurface);
    backbuffer->vtable->Release(backbuffer);
}

// Target 0x00448360 copies arbitrary rectangles between two loaded ANM
// texture entries. This complete function was absent from the initial Ghidra
// candidate inventory but is delimited by its RET 0x10 and following padding.
void AnmRenderManagerView::CopyTextureRect(
    int destinationAnmIndex, int destinationEntryIndex,
    int sourceAnmIndex, int sourceEntryIndex,
    D3d9RectView *destinationRect, D3d9RectView *sourceRect)
{
    D3d9SurfaceView *destinationSurface;
    D3d9SurfaceView *sourceSurface;
    D3d9TextureView *destinationTexture =
        loadedAnms[destinationAnmIndex]->textures[destinationEntryIndex].texture;
    D3d9TextureView *sourceTexture =
        loadedAnms[sourceAnmIndex]->textures[sourceEntryIndex].texture;

    if (destinationTexture == NULL || sourceTexture == NULL)
        return;
    FlushVertexBuffer();
    if (destinationTexture->vtable->GetSurfaceLevel(
            destinationTexture, 0, &destinationSurface) != 0)
    {
        return;
    }
    if (sourceTexture->vtable->GetSurfaceLevel(
            sourceTexture, 0, &sourceSurface) != 0)
    {
        destinationSurface->vtable->Release(destinationSurface);
        return;
    }

    if (D3DXLoadSurfaceFromSurface(
            destinationSurface, NULL, destinationRect, sourceSurface,
            NULL, sourceRect, static_cast<unsigned int>(-1), 0) != 0)
    {
        destinationSurface->vtable->Release(destinationSurface);
        sourceSurface->vtable->Release(sourceSurface);
        return;
    }
    destinationSurface->vtable->Release(destinationSurface);
    sourceSurface->vtable->Release(sourceSurface);
}

// Target 0x00448450 captures a backbuffer rectangle into a fresh primary
// surface and mirrors it into the offscreen backup used across device loss.
void AnmRenderManagerView::CaptureToSurface(
    int surfaceIndex, int sourceX, int sourceY,
    int sourceWidth, int sourceHeight, int destinationX,
    int destinationY, int destinationWidth, int destinationHeight)
{
    MainSupervisorAnmPrefixView *supervisor =
        reinterpret_cast<MainSupervisorAnmPrefixView *>(
            &g_MainSupervisorView);
    D3d9SurfaceView *backbuffer;
    D3d9RectView sourceRect;
    D3d9RectView destinationRect;

    FlushVertexBuffer();
    if (surfaces[surfaceIndex] != NULL)
        ReleaseSurface(surfaceIndex);

    sourceRect.left = sourceX;
    sourceRect.top = sourceY;
    sourceRect.right = sourceX + sourceWidth;
    sourceRect.bottom = sourceY + sourceHeight;
    destinationRect.left = destinationX;
    destinationRect.top = destinationY;
    destinationRect.right = destinationX + destinationWidth;
    destinationRect.bottom = destinationY + destinationHeight;

    if (supervisor->d3dDevice->vtable->GetBackBuffer(
            supervisor->d3dDevice, 0, 0, 0, &backbuffer) != 0)
    {
        return;
    }

    surfaceInfo[surfaceIndex].width = destinationWidth;
    surfaceInfo[surfaceIndex].height = destinationHeight;
    if (supervisor->d3dDevice->vtable->CreateRenderTarget(
            supervisor->d3dDevice, surfaceInfo[surfaceIndex].width,
            surfaceInfo[surfaceIndex].height, supervisor->backbufferFormat,
            0, 0, 1, &surfaces[surfaceIndex], NULL) != 0)
    {
        if (supervisor->d3dDevice->vtable->CreateOffscreenPlainSurface(
                supervisor->d3dDevice, surfaceInfo[surfaceIndex].width,
                surfaceInfo[surfaceIndex].height, supervisor->backbufferFormat,
                3, &surfaces[surfaceIndex], NULL) != 0)
        {
            goto out;
        }
    }

    D3d9SurfaceView **secondarySurface =
        &secondarySurfaces[surfaceIndex];
    D3d9DeviceView *secondaryDevice = supervisor->d3dDevice;
    if (secondaryDevice->vtable->CreateOffscreenPlainSurface(
            secondaryDevice, surfaceInfo[surfaceIndex].width,
            surfaceInfo[surfaceIndex].height, supervisor->backbufferFormat,
            3, secondarySurface, NULL) != 0)
    {
        goto out;
    }
    if (D3DXLoadSurfaceFromSurface(
            surfaces[surfaceIndex], NULL, &destinationRect,
            backbuffer, NULL, &sourceRect,
            static_cast<unsigned int>(-1), 0) != 0)
    {
        goto out;
    }
    D3DXLoadSurfaceFromSurface(
        secondarySurfaces[surfaceIndex], NULL, NULL,
        surfaces[surfaceIndex], NULL, NULL,
        static_cast<unsigned int>(-1), 0);

out:
    if (backbuffer != NULL)
    {
        backbuffer->vtable->Release(backbuffer);
        backbuffer = NULL;
    }
}

// Target 0x00446220 drains both manager-order lists. Compiler-generated member
// destruction then releases the 20 sentinel VMs, primaryVm and 4096 inline-
// pool VMs in that target-observed order.
AnmRenderManagerView::~AnmRenderManagerView()
{
    while (primaryVmListHead != NULL)
        RemoveVm(static_cast<AnmVmView *>(primaryVmListHead->owner));
    while (secondaryVmListHead != NULL)
        RemoveVm(static_cast<AnmVmView *>(secondaryVmListHead->owner));
}

// Target 0x0043E5A0 binds one loaded sprite to a VM and rebuilds the two
// texture-space matrices derived from its dimensions.
int AnmLoadedView::SetSprite(AnmVmView *vm, int spriteIndex)
{
    if (rawData == NULL || pendingLoadCount != 0)
        return -1;

    vm->activeSpriteIndex = static_cast<short>(spriteIndex);
    vm->anmFile = this;
    vm->loadedSprite = &sprites[spriteIndex];
    vm->spriteWidth = vm->loadedSprite->width;
    vm->spriteHeight = vm->loadedSprite->height;

    vm->matrix23C.SetIdentity();
    vm->textureMatrix2BC.SetIdentity();
    vm->matrix23C.values[0] = vm->spriteWidth * (1.0f / 256.0f);
    vm->matrix23C.values[5] = vm->spriteHeight * (1.0f / 256.0f);
    vm->textureMatrix2BC.values[0] =
        vm->loadedSprite->horizontalScale /
        vm->loadedSprite->textureWidth * vm->spriteWidth;
    vm->textureMatrix2BC.values[5] =
        vm->loadedSprite->verticalScale /
        vm->loadedSprite->textureHeight * vm->spriteHeight;
    vm->matrix27C = vm->matrix23C;
    return 0;
}

// Target 0x0043E7E0 binds a VM to one script owned by this loaded ANM. Invalid
// or still-loading entries clear the whole VM; a valid entry resets the script
// timer, executes frame zero, and accounts for the newly started script.
void AnmLoadedView::SetAndExecuteScriptIndex(
    AnmVmView *vm, int scriptIndex)
{
    AnmRawInstructionView *beginningOfScript = scripts[scriptIndex];

    if (beginningOfScript == NULL || pendingLoadCount != 0)
    {
        memset(vm, 0, sizeof(AnmVmView));
    }
    else
    {
        vm->scriptIndex = static_cast<short>(scriptIndex);
        vm->anmFileIndex = anmFileIndex;
        unsigned int flags = vm->flags35C;
        flags &= ~0x600u;
        vm->anmFile = this;
        vm->flags35C = flags;
        vm->beginningOfScript = scripts[scriptIndex];
        vm->currentInstruction = vm->beginningOfScript;
        vm->scriptTimer.SetCurrent(0);
        vm->visible = 0;
        AnmRenderManagerView::ExecuteScript(vm);
        ++g_AnmRenderManagerView->scriptsStartedThisFrame;
    }
}

// Target 0x0043E710 is the allocating-spawn counterpart of the binder above.
// It preserves the caller-selected position and layer through Initialize,
// then installs and starts the requested script.
void AnmLoadedView::InitializeAndExecuteScriptIndex(
    AnmVmView *vm, int scriptIndex)
{
    AnmRawInstructionView *beginningOfScript = scripts[scriptIndex];

    if (beginningOfScript == NULL || pendingLoadCount != 0)
    {
        memset(vm, 0, sizeof(AnmVmView));
    }
    else
    {
        vm->Initialize();
        vm->scriptIndex = static_cast<short>(scriptIndex);
        vm->anmFileIndex = anmFileIndex;
        unsigned int flags = vm->flags35C;
        flags &= ~0x600u;
        vm->anmFile = this;
        vm->flags35C = flags;
        vm->beginningOfScript = scripts[scriptIndex];
        vm->currentInstruction = vm->beginningOfScript;
        vm->scriptTimer.SetCurrent(0);
        vm->visible = 0;
        AnmRenderManagerView::ExecuteScript(vm);
        ++g_AnmRenderManagerView->scriptsStartedThisFrame;
    }
}

// Target 0x0043E8B0 switches an existing VM to another script while retaining
// its placement and list identity. The script-local timer, color, interpolation
// state and flip transform are rebuilt before frame zero executes.
void AnmLoadedView::SetAndExecuteScriptIdx(
    AnmVmView *vm, int scriptIndex)
{
    AnmRawInstructionView *beginningOfScript = scripts[scriptIndex];
    if (beginningOfScript == NULL)
        return;
    if (pendingLoadCount != 0)
        return;

    vm->scriptIndex = static_cast<short>(scriptIndex);
    unsigned int flags = vm->flags35C;
    vm->anmFile = this;
    if ((flags & 0x200u) != 0)
    {
        flags |= 8u;
        vm->scaleX *= -1.0f;
        flags ^= 0x200u;
        vm->flags35C = flags;
    }

    *reinterpret_cast<unsigned short *>(&vm->flags35C) = 7;
    vm->primaryColor.value = 0xffffffffu;
    vm->scriptTimer.Initialize();
    vm->positionInterpolation.duration = 0;
    vm->primaryColorInterpolation.duration = 0;
    vm->primaryAlphaInterpolation.duration = 0;
    vm->rotationInterpolation.duration = 0;
    vm->scaleInterpolation.duration = 0;
    vm->secondaryColorInterpolation.duration = 0;
    vm->secondaryAlphaInterpolation.duration = 0;

    vm->anmFileIndex = anmFileIndex;
    flags = vm->flags35C;
    flags &= ~0x600u;
    vm->anmFile = this;
    vm->flags35C = flags;
    vm->beginningOfScript = beginningOfScript;
    vm->currentInstruction = beginningOfScript;
    vm->scriptTimer.SetCurrent(0);
    vm->visible = 0;
    AnmRenderManagerView::ExecuteScript(vm);
    ++g_AnmRenderManagerView->scriptsStartedThisFrame;
}

// Target 0x00449870 prepares a newly allocated VM for a loaded script. The
// allocator owns the full VM reset; this phase initializes spawn-local state
// before handing the script index to the loaded-resource executor.
void AnmLoadedView::InitializeVm(AnmVmView *vm, int scriptIndex)
{
    vm->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->position = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->alternatePosition = AnmFloat3View(0.0f, 0.0f, 0.0f);
    vm->flags35C |= 0x40000000u;
    vm->scriptIndex = static_cast<short>(scriptIndex);
    vm->glyphWidth = 0x10;
    vm->glyphHeight = 0x10;
    SetAndExecuteScriptIndex(vm, scriptIndex);
}

AnmVmIdView AnmLoadedView::CreateVmAtScreenVariant0(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset = *position;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant0(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset.x = position->x + 224.0f;
    vm->positionOffset.y = position->y + 16.0f;
    vm->positionOffset.z = position->z;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
}

AnmVmIdView AnmLoadedView::CreateVmVariant1(
    int scriptIndex, unsigned int renderLayer)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->renderLayer = renderLayer;
    vm->flags35C |= 0x40000000u;
    InitializeVm(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant1(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtScreenVariant1(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset = *position;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant1(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant1(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset.x = position->x + 224.0f;
    vm->positionOffset.y = position->y + 16.0f;
    vm->positionOffset.z = position->z;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant1(vm);
}

AnmVmIdView AnmLoadedView::CreateVmVariant2(
    int scriptIndex, unsigned int renderLayer)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->renderLayer = renderLayer;
    vm->flags35C |= 0x40000000u;
    InitializeVm(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant2(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtScreenVariant2(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset = *position;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant2(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant2(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset.x = position->x + 224.0f;
    vm->positionOffset.y = position->y + 16.0f;
    vm->positionOffset.z = position->z;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant2(vm);
}

AnmVmIdView AnmLoadedView::CreateVmVariant3(
    int scriptIndex, unsigned int renderLayer)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->renderLayer = renderLayer;
    vm->flags35C |= 0x40000000u;
    InitializeVm(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant3(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtScreenVariant3(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset = *position;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant3(vm);
}

AnmVmIdView AnmLoadedView::CreateVmAtWorldVariant3(
    int scriptIndex, const AnmFloat3View *position)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->flags35C |= 0x40000000u;
    vm->renderLayer = 0;
    vm->positionOffset.x = position->x + 224.0f;
    vm->positionOffset.y = position->y + 16.0f;
    vm->positionOffset.z = position->z;
    InitializeAndExecuteScriptIndex(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant3(vm);
}

// Target 0x00449450 clears a stale id after the two manager lists fail to
// resolve it, making future lookups cheap and deterministic.
AnmVmView *AnmVmIdView::GetVm()
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*this);
    if (vm == NULL)
        value = 0;
    return vm;
}

void AnmVmIdView::SetFlag2()
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*this);
    if (vm == NULL)
        return;

    const unsigned int mask = 2u;
    vm->flags35C |= mask;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->flags35C |= mask;
        node = node->next;
    }
}

void AnmVmIdView::ClearFlag2()
{
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(*this);
    if (vm == NULL)
        return;

    const unsigned int mask = 0xfffffffdu;
    vm->flags35C &= mask;
    if (vm->layerNode.previous != NULL)
        return;

    AnmVmLayerNodeView *node = vm->layerNode.next;
    while (node != NULL)
    {
        static_cast<AnmVmView *>(node->owner)->flags35C &= mask;
        node = node->next;
    }
}

void AnmVmIdView::Release()
{
    g_AnmRenderManagerView->MarkVmForDeletion(*this);
    value = 0;
}

// Target 0x0043EE30 is TH10's complete variable-length ANM instruction
// executor. The adjacent TH095 source supplies control-flow hypotheses; every
// opcode, VM offset, interpolation call and frame-end update below is checked
// against the TH10 v1.00a target.
int __stdcall AnmRenderManagerView::ExecuteScript(AnmVmView *vm)
{
    AnmRawInstructionView *currentInstruction;
    AnmRawInstructionView *fallbackInterrupt;
    AnmVmView *createdChild;
    float savedGameSpeed;

    if (vm->currentInstruction == NULL)
        return 1;
    if (vm->scriptDisabled)
        return 0;

    savedGameSpeed = g_AnmGameSpeed;
    if (vm->useUnitSpeed)
        g_AnmGameSpeed = 1.0f;

    if (vm->pendingInterrupt != 0)
        goto handleInterrupt;

    while ((currentInstruction = vm->currentInstruction),
           currentInstruction->time <= vm->scriptTimer.current)
    {
#define GET_INT_VAR(argumentNumber)                                      \
    ((currentInstruction->variableMask & (1 << (argumentNumber))) != 0   \
        ? vm->GetIntVar(currentInstruction->intArgs[(argumentNumber)])   \
        : currentInstruction->intArgs[(argumentNumber)])
#define GET_FLOAT_VAR(argumentNumber)                                    \
    ((currentInstruction->variableMask & (1 << (argumentNumber))) != 0   \
        ? vm->GetFloatVar(currentInstruction->floatArgs[(argumentNumber)]) \
        : currentInstruction->floatArgs[(argumentNumber)])
#define GET_INT_VAR_PTR(argumentNumber)                                  \
    vm->GetIntVarPtr(                                                    \
        &currentInstruction->intArgs[(argumentNumber)],                  \
        currentInstruction->variableMask, (argumentNumber))
#define GET_FLOAT_VAR_PTR(argumentNumber)                                \
    vm->GetFloatVarPtr(                                                  \
        &currentInstruction->floatArgs[(argumentNumber)],                \
        currentInstruction->variableMask, (argumentNumber))

        switch (currentInstruction->opcode)
        {
        case ANM_OP_UNIT_SPEED:
            vm->useUnitSpeed = GET_INT_VAR(0);
            break;
        case ANM_OP_CREATE_CHILD_88:
        {
            AnmVmIdView childId = vm->anmFile->CreateVmVariant0(
                GET_INT_VAR(0), vm->renderLayer);
            createdChild = childId.GetVm();
            goto child_variant_0_or_1;
        }
        case ANM_OP_CREATE_CHILD_91:
        {
            AnmVmIdView childId = vm->anmFile->CreateVmVariant2(
                GET_INT_VAR(0), vm->renderLayer);
            createdChild = childId.GetVm();
            goto child_variant_2_or_3;
        }
        case ANM_OP_CREATE_CHILD_90:
        {
            AnmVmIdView childId = vm->anmFile->CreateVmVariant1(
                GET_INT_VAR(0), vm->renderLayer);
            createdChild = childId.GetVm();
        }
        child_variant_0_or_1:
            vm->layerNode.InsertAfter(&createdChild->layerNode);
            createdChild->alternatePosition = vm->position;
            createdChild->positionOffset = vm->positionOffset;
            break;
        case ANM_OP_CREATE_CHILD_92:
        {
            AnmVmIdView childId = vm->anmFile->CreateVmVariant3(
                GET_INT_VAR(0), vm->renderLayer);
            createdChild = childId.GetVm();
        }
        child_variant_2_or_3:
            vm->layerNode.InsertAfter(&createdChild->layerNode);
            createdChild->alternatePosition = vm->position;
            createdChild->positionOffset = vm->positionOffset;
            break;
        case ANM_OP_SPRITE:
            vm->visible = 1;
            vm->anmFile->SetSprite(vm, GET_INT_VAR(0));
            vm->timeOfLastSpriteSet = vm->scriptTimer.current;
            break;

        case ANM_OP_SCALE:
            vm->scaleX = GET_FLOAT_VAR(0);
            vm->scaleY = GET_FLOAT_VAR(1);
            vm->updateScale = 1;
            break;
        case ANM_OP_ALPHA1:
            vm->primaryColor.alpha =
                static_cast<unsigned char>(GET_INT_VAR(0));
            break;
        case ANM_OP_COLOR1:
            vm->primaryColor.red =
                static_cast<unsigned char>(GET_INT_VAR(0));
            vm->primaryColor.green =
                static_cast<unsigned char>(GET_INT_VAR(1));
            vm->primaryColor.blue =
                static_cast<unsigned char>(GET_INT_VAR(2));
            break;
        case ANM_OP_ALPHA2:
            vm->secondaryColor.alpha =
                static_cast<unsigned char>(GET_INT_VAR(0));
            break;
        case ANM_OP_COLOR2:
            vm->secondaryColor.red =
                static_cast<unsigned char>(GET_INT_VAR(0));
            vm->secondaryColor.green =
                static_cast<unsigned char>(GET_INT_VAR(1));
            vm->secondaryColor.blue =
                static_cast<unsigned char>(GET_INT_VAR(2));
            break;
        case ANM_OP_JUMP:
            vm->scriptTimer.SetCurrent(currentInstruction->intArgs[1]);
            vm->currentInstruction = reinterpret_cast<AnmRawInstructionView *>(
                reinterpret_cast<unsigned char *>(vm->beginningOfScript) +
                currentInstruction->intArgs[0]);
            continue;

        case ANM_OP_JUMP_DEC:
            --*GET_INT_VAR_PTR(0);
            if (GET_INT_VAR(0) > 0)
            {
                vm->scriptTimer.SetCurrent(currentInstruction->intArgs[2]);
                vm->currentInstruction =
                    reinterpret_cast<AnmRawInstructionView *>(
                        reinterpret_cast<unsigned char *>(
                            vm->beginningOfScript) +
                        currentInstruction->intArgs[1]);
                continue;
            }
            break;

        case ANM_OP_FLIP_X:
            vm->flipX ^= 1;
            vm->scaleX *= -1.0f;
            vm->updateScale = 1;
            break;
        case ANM_OP_FLIP_Y:
            vm->flipY ^= 1;
            vm->scaleY *= -1.0f;
            vm->updateScale = 1;
            break;

        case ANM_OP_ROTATION:
            vm->rotation.x = GET_FLOAT_VAR(0);
            vm->rotation.y = GET_FLOAT_VAR(1);
            vm->rotation.z = GET_FLOAT_VAR(2);
            vm->updateRotation = 1;
            break;
        case ANM_OP_ANGULAR_VELOCITY:
            vm->angularVelocity.x = GET_FLOAT_VAR(0);
            vm->angularVelocity.y = GET_FLOAT_VAR(1);
            vm->angularVelocity.z = GET_FLOAT_VAR(2);
            vm->updateRotation = 1;
            break;
        case ANM_OP_SCALE_GROWTH:
            vm->scaleGrowth.x = GET_FLOAT_VAR(0);
            vm->scaleGrowth.y = GET_FLOAT_VAR(1);
            break;

        case ANM_OP_ALPHA1_TIME_LINEAR:
            vm->StartPrimaryAlphaInterpolation(
                GET_INT_VAR(1), ANM_INTERPOLATION_LINEAR,
                vm->primaryColor.alpha,
                static_cast<unsigned char>(currentInstruction->intArgs[0]));
            break;
        case ANM_OP_BLEND_MODE:
            vm->blendMode = currentInstruction->intArgs[0];
            break;
        case ANM_OP_POSITION:
            if (!vm->useAlternatePosition)
            {
                vm->position = AnmFloat3View(
                    GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            }
            else
            {
                vm->alternatePosition = AnmFloat3View(
                    GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            }
            break;
        case ANM_OP_NOP:
        case ANM_OP_INTERRUPT_LABEL:
            vm->currentInstruction = reinterpret_cast<AnmRawInstructionView *>(
                reinterpret_cast<unsigned char *>(currentInstruction) +
                currentInstruction->size);
            continue;
        case ANM_OP_WAIT:
            vm->scriptTimer.Decrement(GET_INT_VAR(0));
            break;

        case ANM_OP_STOP_HIDE:
            vm->visible = 0;
        case ANM_OP_STOP:
            if (vm->pendingInterrupt == 0)
            {
                vm->stopped = 1;
                vm->scriptTimer.Add(-1.0f);
                goto stop;
            }
        handleInterrupt:
            fallbackInterrupt = NULL;
            currentInstruction = vm->beginningOfScript;
            while (!((currentInstruction->opcode == ANM_OP_INTERRUPT_LABEL) &&
                     (vm->pendingInterrupt ==
                      currentInstruction->intArgs[0])) &&
                   currentInstruction->opcode != ANM_OP_END)
            {
                if (currentInstruction->opcode == ANM_OP_INTERRUPT_LABEL &&
                    currentInstruction->intArgs[0] == -1)
                {
                    fallbackInterrupt = currentInstruction;
                }
                currentInstruction =
                    reinterpret_cast<AnmRawInstructionView *>(
                        reinterpret_cast<unsigned char *>(currentInstruction) +
                        currentInstruction->size);
            }
            vm->pendingInterrupt = 0;
            vm->stopped = 0;
            if (currentInstruction->opcode != ANM_OP_INTERRUPT_LABEL)
            {
                if (fallbackInterrupt == NULL)
                {
                    vm->scriptTimer.Add(-1.0f);
                    goto stop;
                }
                currentInstruction = fallbackInterrupt;
            }
            vm->interruptReturnTimer = vm->scriptTimer;
            vm->interruptReturnInstruction = vm->currentInstruction;
            vm->scriptTimer.SetCurrent(currentInstruction->time);
            vm->currentInstruction =
                reinterpret_cast<AnmRawInstructionView *>(
                    reinterpret_cast<unsigned char *>(currentInstruction) +
                    currentInstruction->size);
            vm->visible = 1;
            continue;

        case ANM_OP_RETURN:
            vm->scriptTimer = vm->interruptReturnTimer;
            vm->currentInstruction = vm->interruptReturnInstruction;
            continue;
        case ANM_OP_VISIBLE:
            vm->visible = currentInstruction->intArgs[0];
            break;
        case ANM_OP_RENDER_STATE:
            vm->renderStateA = currentInstruction->shortArgs[0];
            vm->renderStateB = currentInstruction->shortArgs[1];
            break;
        case ANM_OP_U_SCROLL:
            vm->uvScrollVelocityX = GET_FLOAT_VAR(0);
            break;
        case ANM_OP_V_SCROLL:
            vm->uvScrollVelocityY = GET_FLOAT_VAR(0);
            break;
        case ANM_OP_Z_WRITE_DISABLE:
            vm->zWriteDisabled = currentInstruction->intArgs[0];
            break;
        case ANM_OP_FLAG13:
            vm->flag13 = currentInstruction->intArgs[0];
            break;
        case ANM_OP_POINT_TEXTURE_FILTER:
            vm->usePointTextureFilter = currentInstruction->intArgs[0];
            break;

        case ANM_OP_POSITION_TIME:
            vm->positionInterpolation.duration = GET_INT_VAR(0);
            vm->positionInterpolation.initialTangent =
                AnmFloat3View(0.0f, 0.0f, 0.0f);
            vm->positionInterpolation.finalTangent =
                AnmFloat3View(0.0f, 0.0f, 0.0f);
            vm->positionInterpolation.mode =
                currentInstruction->byteArgs[4];
            vm->positionInterpolation.initial = vm->useAlternatePosition
                ? vm->alternatePosition : vm->position;
            vm->positionInterpolation.final.x = GET_FLOAT_VAR(2);
            vm->positionInterpolation.final.y = GET_FLOAT_VAR(3);
            vm->positionInterpolation.final.z = GET_FLOAT_VAR(4);
            vm->positionInterpolation.timer.SetCurrent(0);
            break;
        case ANM_OP_COLOR1_TIME:
        {
            AnmColorView initialColor = vm->primaryColor;
            AnmColorView finalColor;
            finalColor.value = 0;
            finalColor.red = static_cast<unsigned char>(GET_INT_VAR(2));
            finalColor.green = static_cast<unsigned char>(GET_INT_VAR(3));
            finalColor.blue = static_cast<unsigned char>(GET_INT_VAR(4));
            vm->StartPrimaryColorInterpolation(
                &initialColor, &finalColor, GET_INT_VAR(0),
                currentInstruction->byteArgs[4]);
            break;
        }
        case ANM_OP_ALPHA1_TIME:
            vm->StartPrimaryAlphaInterpolation(
                GET_INT_VAR(0), currentInstruction->byteArgs[4],
                vm->primaryColor.alpha,
                static_cast<unsigned char>(GET_INT_VAR(2)));
            break;
        case ANM_OP_COLOR2_TIME:
        {
            AnmColorView initialColor = vm->secondaryColor;
            AnmColorView finalColor;
            finalColor.value = 0;
            finalColor.red = static_cast<unsigned char>(GET_INT_VAR(2));
            finalColor.green = static_cast<unsigned char>(GET_INT_VAR(3));
            finalColor.blue = static_cast<unsigned char>(GET_INT_VAR(4));
            vm->StartSecondaryColorInterpolation(
                &initialColor, &finalColor, GET_INT_VAR(0),
                currentInstruction->byteArgs[4]);
            break;
        }
        case ANM_OP_ALPHA2_TIME:
            vm->StartSecondaryAlphaInterpolation(
                GET_INT_VAR(0), currentInstruction->byteArgs[4],
                vm->secondaryColor.alpha,
                static_cast<unsigned char>(GET_INT_VAR(2)));
            break;
        case ANM_OP_ROTATION_TIME:
            vm->rotationInterpolation.duration = GET_INT_VAR(0);
            vm->rotationInterpolation.initialTangent =
                AnmFloat3View(0.0f, 0.0f, 0.0f);
            vm->rotationInterpolation.finalTangent =
                AnmFloat3View(0.0f, 0.0f, 0.0f);
            vm->rotationInterpolation.mode = currentInstruction->byteArgs[4];
            vm->rotationInterpolation.initial = vm->rotation;
            vm->rotationInterpolation.final.x = GET_FLOAT_VAR(2);
            vm->rotationInterpolation.final.y = GET_FLOAT_VAR(3);
            vm->rotationInterpolation.final.z = GET_FLOAT_VAR(4);
            vm->rotationInterpolation.timer.SetCurrent(0);
            vm->updateRotation = 1;
            break;
        case ANM_OP_SCALE_TIME:
        {
            AnmFloat2View finalScale;
            finalScale.x = GET_FLOAT_VAR(2);
            finalScale.y = GET_FLOAT_VAR(3);
            vm->StartScaleInterpolation(
                reinterpret_cast<AnmFloat2View *>(&vm->scaleX),
                &finalScale,
                GET_INT_VAR(0), currentInstruction->byteArgs[4]);
            vm->updateScale = 1;
            break;
        }

        case ANM_OP_RENDER_MODE:
            vm->renderMode = currentInstruction->intArgs[0];
            if (vm->renderMode == 10)
                vm->InitializePulsingRadialTrail();
            break;
        case ANM_OP_COMMIT_POSITION:
            vm->position = vm->positionOffset;
            vm->positionOffset = AnmFloat3View(0.0f, 0.0f, 0.0f);
            break;
        case ANM_OP_ALLOC_VERTICES:
            vm->renderMode = 9;
            vm->generatedVertices = malloc(
                GET_INT_VAR(0) * sizeof(AnmRenderVertexView) * 2);
            break;
        case ANM_OP_I_SET:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1);
            break;
        case ANM_OP_F_SET:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_SET_ADD:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) + GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_ADD:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) + GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_SUB:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) - GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_SUB:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) - GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_MUL:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) * GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_MUL:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) * GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_DIV:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) / GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_DIV:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) / GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_MOD:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) % GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_MOD:
            *GET_FLOAT_VAR_PTR(0) =
                fmodf(GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            break;

        case ANM_OP_I_ADD:
            *GET_INT_VAR_PTR(0) += GET_INT_VAR(1);
            break;
        case ANM_OP_F_ADD:
            *GET_FLOAT_VAR_PTR(0) += GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_SUB:
            *GET_INT_VAR_PTR(0) -= GET_INT_VAR(1);
            break;
        case ANM_OP_F_SUB:
            *GET_FLOAT_VAR_PTR(0) -= GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_MUL:
            *GET_INT_VAR_PTR(0) *= GET_INT_VAR(1);
            break;
        case ANM_OP_F_MUL:
            *GET_FLOAT_VAR_PTR(0) *= GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_DIV:
            *GET_INT_VAR_PTR(0) /= GET_INT_VAR(1);
            break;
        case ANM_OP_F_DIV:
            *GET_FLOAT_VAR_PTR(0) /= GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_MOD:
            *GET_INT_VAR_PTR(0) %= GET_INT_VAR(1);
            break;
        case ANM_OP_F_MOD:
            *GET_FLOAT_VAR_PTR(0) =
                fmodf(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1));
            break;

        case ANM_OP_I_SET_RANDOM:
            *GET_INT_VAR_PTR(0) = vm->useAlternateRng
                ? g_AlternateRngView.GetRandomU32InRange(GET_INT_VAR(1))
                : g_RngView.GetRandomU32InRange(GET_INT_VAR(1));
            break;
        case ANM_OP_F_SET_RANDOM:
            *GET_FLOAT_VAR_PTR(0) = vm->useAlternateRng
                ? g_AlternateRngView.GetRandomF32InRange(GET_FLOAT_VAR(1))
                : g_RngView.GetRandomF32InRange(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_SIN:
            *GET_FLOAT_VAR_PTR(0) = sinf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_COS:
            *GET_FLOAT_VAR_PTR(0) = cosf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_TAN:
            *GET_FLOAT_VAR_PTR(0) = tanf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_ACOS:
            *GET_FLOAT_VAR_PTR(0) = acosf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_ATAN:
            *GET_FLOAT_VAR_PTR(0) = atanf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_NORMALIZE_ANGLE:
            *GET_FLOAT_VAR_PTR(0) =
                AddNormalizeAngle(GET_FLOAT_VAR(0), 0.0f);
            break;

        case ANM_OP_FLAG28:
            vm->flag28 = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_RENDER_LAYER:
            vm->renderLayer = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_USE_SECONDARY_COLOR:
            vm->useSecondaryColor = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_FLAG27:
            vm->flag27 = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_ALTERNATE_RNG:
            vm->useAlternateRng = currentInstruction->byteArgs[0];
            break;

        case ANM_OP_I_JUMP_EQ:
            if (GET_INT_VAR(0) == GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_EQ:
            if (GET_FLOAT_VAR(0) == GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_NE:
            if (GET_INT_VAR(0) != GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_NE:
            if (GET_FLOAT_VAR(0) != GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_LT:
            if (GET_INT_VAR(0) < GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_LT:
            if (GET_FLOAT_VAR(0) < GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_LE:
            if (GET_INT_VAR(0) <= GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_LE:
            if (GET_FLOAT_VAR(0) <= GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_GT:
            if (GET_INT_VAR(0) > GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_GT:
            if (GET_FLOAT_VAR(0) > GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_GE:
            if (GET_INT_VAR(0) >= GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_GE:
            if (GET_FLOAT_VAR(0) >= GET_FLOAT_VAR(1)) goto jump;
            break;

        jump:
            vm->scriptTimer.SetCurrent(currentInstruction->intArgs[3]);
            vm->currentInstruction = reinterpret_cast<AnmRawInstructionView *>(
                reinterpret_cast<unsigned char *>(vm->beginningOfScript) +
                currentInstruction->intArgs[2]);
            continue;

        case ANM_OP_END:
        case ANM_OP_DELETE:
            vm->visible = 0;
        case ANM_OP_STATIC:
            vm->currentInstruction = NULL;
            g_AnmGameSpeed = savedGameSpeed;
            return 1;

        default:
            break;
        }

#undef GET_FLOAT_VAR_PTR
#undef GET_INT_VAR_PTR
#undef GET_FLOAT_VAR
#undef GET_INT_VAR

        vm->currentInstruction = reinterpret_cast<AnmRawInstructionView *>(
            reinterpret_cast<unsigned char *>(currentInstruction) +
            currentInstruction->size);
        continue;
    }

stop:
    if (vm->angularVelocity.x != 0.0f)
    {
        vm->rotation.x = AddNormalizeAngle(
            vm->rotation.x, g_AnmGameSpeed * vm->angularVelocity.x);
        vm->updateRotation = 1;
    }
    if (vm->angularVelocity.y != 0.0f)
    {
        vm->rotation.y = AddNormalizeAngle(
            vm->rotation.y, g_AnmGameSpeed * vm->angularVelocity.y);
        vm->updateRotation = 1;
    }
    if (vm->angularVelocity.z != 0.0f)
    {
        vm->rotation.z = AddNormalizeAngle(
            vm->rotation.z, g_AnmGameSpeed * vm->angularVelocity.z);
        vm->updateRotation = 1;
    }

    if (vm->scaleGrowth.y != 0.0f)
    {
        vm->scaleY += g_AnmGameSpeed * vm->scaleGrowth.y;
        vm->updateScale = 1;
    }
    if (vm->scaleGrowth.x != 0.0f)
    {
        vm->scaleX += g_AnmGameSpeed * vm->scaleGrowth.x;
        vm->updateScale = 1;
        vm->updateRotation = 1;
    }

    vm->uvScrollX += g_AnmGameSpeed * vm->uvScrollVelocityX;
    if (vm->uvScrollX >= 1.0f)
        vm->uvScrollX -= 1.0f;
    else if (vm->uvScrollX < 0.0f)
        vm->uvScrollX += 1.0f;
    vm->uvScrollY += g_AnmGameSpeed * vm->uvScrollVelocityY;
    if (vm->uvScrollY >= 1.0f)
        vm->uvScrollY -= 1.0f;
    else if (vm->uvScrollY < 0.0f)
        vm->uvScrollY += 1.0f;

    if (vm->flag13)
        vm->positionOffset += g_AnmPositionOffsetDelta;

    if (vm->positionInterpolation.duration != 0)
    {
        AnmFloat3View value;
        if (!vm->useAlternatePosition)
            vm->position = *vm->positionInterpolation.Evaluate(&value);
        else
            vm->alternatePosition =
                *vm->positionInterpolation.Evaluate(&value);
    }
    if (vm->primaryColorInterpolation.duration != 0)
    {
        AnmInt3View value;
        vm->primaryColorInterpolation.Evaluate(&value);
        vm->primaryColor.blue = static_cast<unsigned char>(value.x);
        vm->primaryColor.green = static_cast<unsigned char>(value.y);
        vm->primaryColor.red = static_cast<unsigned char>(value.z);
    }
    if (vm->primaryAlphaInterpolation.duration != 0)
        vm->primaryColor.alpha = static_cast<unsigned char>(
            vm->primaryAlphaInterpolation.Evaluate());
    if (vm->scaleInterpolation.duration != 0)
    {
        AnmFloat2View value;
        AnmFloat2View *evaluated = vm->scaleInterpolation.Evaluate(&value);
        vm->scaleX = evaluated->x;
        vm->scaleY = evaluated->y;
        vm->updateScale = 1;
    }
    if (vm->rotationInterpolation.duration != 0)
    {
        AnmFloat3View value;
        vm->rotation = *vm->rotationInterpolation.Evaluate(&value);
        vm->updateRotation = 1;
    }
    if (vm->secondaryColorInterpolation.duration != 0)
    {
        AnmInt3View value;
        vm->secondaryColorInterpolation.Evaluate(&value);
        vm->secondaryColor.blue = static_cast<unsigned char>(value.x);
        vm->secondaryColor.green = static_cast<unsigned char>(value.y);
        vm->secondaryColor.red = static_cast<unsigned char>(value.z);
    }
    if (vm->secondaryAlphaInterpolation.duration != 0)
        vm->secondaryColor.alpha = static_cast<unsigned char>(
            vm->secondaryAlphaInterpolation.Evaluate());

    if (vm->renderMode == 9)
    {
        int segmentCount = vm->intVar0 - 1;
        float angleValue = vm->rotation.z;
        float angleStep = 6.2831855f / segmentCount;
        AnmRenderVertexView *vertex =
            static_cast<AnmRenderVertexView *>(vm->generatedVertices);
        float textureV = 0.0f;
        float textureStep =
            static_cast<float>(vm->intVar1) / segmentCount;

        for (int i = 0; i < segmentCount; ++i)
        {
            vertex[0].rhw = 1.0f;
            vertex[0].diffuse.value = vm->primaryColor.value;
            vertex[0].u = vm->loadedSprite->uStart + vm->uvScrollX;
            vertex[0].v = textureV + vm->uvScrollY;
            reinterpret_cast<AnmFloat3View *>(&vertex[0].x)
                ->FromAngleMagnitude(
                    angleValue, vm->scaleX * 0.5f + vm->scaleY);
            vertex[0].z = 0.0f;
            *reinterpret_cast<AnmFloat3View *>(&vertex[0].x) +=
                vm->position + vm->positionOffset;
            ++vertex;

            vertex[0].rhw = 1.0f;
            vertex[0].diffuse.value = vm->primaryColor.value;
            vertex[0].u = vm->loadedSprite->uEnd + vm->uvScrollX;
            vertex[0].v = textureV + vm->uvScrollY;
            reinterpret_cast<AnmFloat3View *>(&vertex[0].x)
                ->FromAngleMagnitude(
                    angleValue, vm->scaleY - vm->scaleX * 0.5f);
            vertex[0].z = 0.0f;
            *reinterpret_cast<AnmFloat3View *>(&vertex[0].x) +=
                vm->position + vm->positionOffset;
            ++vertex;

            textureV += textureStep;
            angleValue = AddNormalizeAngle(angleValue, angleStep);
        }

        vertex[0] = static_cast<AnmRenderVertexView *>(
            vm->generatedVertices)[0];
        vertex[0].v = textureV + vm->uvScrollY;
        vertex[1] = static_cast<AnmRenderVertexView *>(
            vm->generatedVertices)[1];
        vertex[1].v = textureV + vm->uvScrollY;
    }

    vm->scriptTimer.Tick();
    g_AnmGameSpeed = savedGameSpeed;
    return 0;
}

// Target 0x004458B0-0x004458CD is the retained polar-vector primitive used by
// generated radial trails. VC7.1 otherwise lowers separate sin/cos calls and
// loses the target's single x87 FSINCOS operation.
void AnmFloat3View::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp [eax]
        fmul magnitude
        fstp [eax + 4]
    }
#else
    x = static_cast<float>(cos(angle)) * magnitude;
    y = static_cast<float>(sin(angle)) * magnitude;
#endif
}

// The target has separate, byte-identical polar helpers for the ANM opcode
// path and the script executor path. These local views preserve those entries;
// their original class names and production translation units are unknown.
struct AnmOpcodeVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};
typedef char AnmOpcodeVectorViewSizeIs0C[
    (sizeof(AnmOpcodeVectorView) == 0x0c) ? 1 : -1];

void AnmOpcodeVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp [eax]
        fmul magnitude
        fstp [eax + 4]
    }
#else
    x = static_cast<float>(cos(angle)) * magnitude;
    y = static_cast<float>(sin(angle)) * magnitude;
#endif
}

struct AnmScriptVectorView
{
    float x;
    float y;
    float z;

    void FromAngleMagnitude(float angle, float magnitude);
};
typedef char AnmScriptVectorViewSizeIs0C[
    (sizeof(AnmScriptVectorView) == 0x0c) ? 1 : -1];

void AnmScriptVectorView::FromAngleMagnitude(float angle, float magnitude)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        mov eax, this
        fld angle
        fsincos
        fmul magnitude
        fstp [eax]
        fmul magnitude
        fstp [eax + 4]
    }
#else
    x = static_cast<float>(cos(angle)) * magnitude;
    y = static_cast<float>(sin(angle)) * magnitude;
#endif
}

// These two target helpers are also inlined by AnmVmView::Initialize. Their
// chained assignments preserve VC7.1's target-observed right-to-left stores.
void AnmMatrixView::SetIdentity()
{
    values[1] = values[2] = values[3] =
        values[4] = values[6] = values[7] =
        values[8] = values[9] = values[11] =
        values[12] = values[13] = values[14] = 0.0f;
    values[0] = values[5] = values[10] = values[15] = 1.0f;
}

void AnmVmTimerView::Initialize()
{
    current = 0;
    previous = -999999;
    subframe = 0.0f;
}

// Target 0x0044BF40 updates the fractional and integer timer values while
// respecting the timer's selected speed multiplier.
void AnmVmTimerView::Add(float value)
{
    previous = current;
    if (*scale > 0.99f && *scale < 1.01f)
        subframe += value;
    else
        subframe += value * *scale;
    current = static_cast<int>(subframe);
}

// Target 0x00402050-0x004020A7 first runs the nine embedded timer
// constructors, then clears the complete 0x3AC-byte VM and marks its active
// sprite index invalid.
AnmVmView::AnmVmView()
{
    memset(this, 0, sizeof(AnmVmView));
    activeSpriteIndex = -1;
}

// Target 0x00401FF0-0x00402011 owns the optional generated-vertex buffer.
AnmVmView::~AnmVmView()
{
    if (generatedVertices != NULL)
        free(generatedVertices);
    generatedVertices = NULL;
}

// Target 0x00401DE0-0x00401EF0 resets the reusable VM while retaining the
// externally owned pointers at +0x20 and +0x340..+0x348.
void AnmVmView::Initialize()
{
    float savedX = positionOffset.x;
    float savedY = positionOffset.y;
    float savedZ = positionOffset.z;
    unsigned int savedLayer = renderLayer;

    memset(this, 0, sizeof(AnmVmView));

    positionOffset.x = savedX;
    positionOffset.y = savedY;
    renderLayer = savedLayer;
    positionOffset.z = savedZ;

    primaryColor.value = 0xffffffff;
    scaleX = 1.0f;
    scaleY = 1.0f;
    matrix23C.SetIdentity();
    flags35C = 7;
    scriptTimer.Initialize();

    positionInterpolation.duration = 0;
    primaryColorInterpolation.duration = 0;
    primaryAlphaInterpolation.duration = 0;
    rotationInterpolation.duration = 0;
    scaleInterpolation.duration = 0;
    secondaryColorInterpolation.duration = 0;
    secondaryAlphaInterpolation.duration = 0;

    managerNode.owner = this;
    managerNode.next = NULL;
    managerNode.previous = NULL;
    layerNode.owner = this;
    layerNode.next = NULL;
    layerNode.previous = NULL;
}

// Target 0x00401000-0x004010FA receives this through a private ESI register.
// C++ construction accounts for the manager vtable and both embedded VM
// constructors before the body clears the complete manager state.
AsciiManagerView::AsciiManagerView()
{
    memset(this, 0, sizeof(AsciiManagerView));
    flags004 |= 2;
    g_AsciiManagerView = this;
    color = 0xffffffff;
    scaleX = 1.0f;
    scaleY = 1.0f;
    unknown8984 = 0;
    spaceWidth = 9;
}

// The manager's single-slot table at 0x0046CB14 points to target 0x00401C90.
size_t AsciiManagerView::GetSize()
{
    return sizeof(AsciiManagerView);
}

// Target 0x00401440-0x00401484 keeps the allocated manager in ESI, allowing
// LTCG to lower construction to the private ESI receiver used at 0x00401000.
AsciiManagerView *AsciiManagerCreate()
{
    AsciiManagerView *manager = new AsciiManagerView;
    if (manager->Initialize() != 0)
    {
        delete manager;
        return NULL;
    }
    return manager;
}

int AsciiManagerView::Initialize()
{
    AnmChainElementView *element;

    asciiAnm = AnmLoadResource(2, g_AnmFileSystemView, "ascii.anm");
    if (asciiAnm == NULL)
        goto fail;
    textAnm = AnmLoadResource(0, g_AnmFileSystemView, "text.anm");
    if (textAnm == NULL)
        goto fail;
    captureAnm = AnmLoadResource(3, g_AnmFileSystemView, "capture.anm");
    if (captureAnm == NULL)
        goto fail;

    element = AnmCreateChainElement(OnUpdate);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddCalcChainElement(element, 4, g_AnmChainView);
    calcChainElement = element;

    element = AnmCreateChainElement(OnDrawLowPriority);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddDrawChainElement(element, 0x30, g_AnmChainView);
    drawChainElement0 = element;

    element = AnmCreateChainElement(OnDrawHighPriority);
    element->flags &= ~2u;
    element->argument = this;
    AnmAddDrawChainElement(element, 0x26, g_AnmChainView);
    drawChainElement = element;

    primaryVm014.Initialize();
    primaryVm014.anmFile = asciiAnm;
    AnmLoadedSetScript(asciiAnm, &primaryVm014, 0);

    secondaryVm3C0.Initialize();
    secondaryVm3C0.anmFile = asciiAnm;
    AnmLoadedSetScript(asciiAnm, &secondaryVm3C0, 0x62);
    return 0;

fail:
    // CP932: "data is corrupted" followed by CRLF.
    g_AnmErrorLoggerView.Log(
        "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea"
        "\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
    return -1;
}

static void RemoveAnmChainElement(AnmChainElementView *element)
{
    if (element == NULL)
        return;
    EnterCriticalSection(g_AnmChainCriticalSection);
    ++g_AnmChainMutationDepth;
    AnmRemoveChainElement(element, g_AnmChainView);
    LeaveCriticalSection(g_AnmChainCriticalSection);
    --g_AnmChainMutationDepth;
}

static void ReleaseAnmResourceSlot(unsigned int offset)
{
    void **slot = reinterpret_cast<void **>(
        static_cast<unsigned char *>(g_AnmFileSystemView) + offset);
    if (*slot == NULL)
        return;
    AnmReleaseResource(*slot);
    free(*slot);
    *slot = NULL;
}

AsciiManagerView::~AsciiManagerView()
{
    RemoveAnmChainElement(
        static_cast<AnmChainElementView *>(calcChainElement));
    RemoveAnmChainElement(
        static_cast<AnmChainElementView *>(drawChainElement0));
    RemoveAnmChainElement(
        static_cast<AnmChainElementView *>(drawChainElement));

    ReleaseAnmResourceSlot(0x3ad074);
    ReleaseAnmResourceSlot(0x3ad06c);
    ReleaseAnmResourceSlot(0x3ad078);
    g_AsciiManagerView = NULL;
}

// Target 0x004014D0-0x004014E9 is a retained member form of the queue reset.
int AsciiManagerView::ResetStrings()
{
    numStrings = 0;
    numGuiStrings = 0;
    ++frameCounter;
    return 1;
}

// Target 0x004014F0-0x00401509 is the calculation-chain callback form.
int __fastcall AsciiManagerView::OnUpdate(AsciiManagerView *manager)
{
    manager->numStrings = 0;
    manager->numGuiStrings = 0;
    ++manager->frameCounter;
    return 1;
}

// Target 0x00401510-0x00401516 adapts the fastcall chain ABI to the stack
// receiver used by the primary ANM draw-layer owner at 0x00401760.
int __fastcall AsciiManagerView::OnDrawLowPriority(AsciiManagerView *manager)
{
    return AsciiManagerDrawStrings(manager);
}

// Target 0x00401520-0x00401529 preserves the manager in EBX for the secondary
// draw owner at 0x00401A50. The natural wrapper keeps that optimizer choice open.
int __fastcall AsciiManagerView::OnDrawHighPriority(AsciiManagerView *manager)
{
    return AsciiManagerDrawGuiStrings(manager);
}

// Target 0x00401530-0x004015B3 appends one of at most 256 regular strings.
// Its register-only EAX/EBX text and position arguments are an LTCG lowering
// of this ordinary member call, as shown by both variadic formatter callers.
void AsciiManagerView::AddString(
    AnmFloat3View *position, const char *text)
{
    AsciiManagerStringView *nextString;

    if (numStrings >= 256)
        return;

    nextString = &strings[numStrings];
    ++numStrings;
    strcpy(nextString->text, text);
    nextString->position = *position;
    nextString->color = color;
    nextString->scaleX = scaleX;
    nextString->scaleY = scaleY;
    nextString->viewportIndex = viewportIndex;
    nextString->smallFont = 0;
    nextString->drawShadow = drawShadow;
}

// Target 0x004015C0-0x0040162C is the analogous 64-entry GUI queue append.
// This CC-delimited owner was absent from the initial Ghidra export; the
// target-bound ledger tracks it.
void AsciiManagerView::AddGuiString(
    AnmFloat3View *position, const char *text)
{
    AsciiManagerStringView *nextString;

    if (numGuiStrings >= 64)
        return;

    nextString = &guiStrings[numGuiStrings];
    ++numGuiStrings;
    strcpy(nextString->text, text);
    nextString->position = *position;
    nextString->color = color;
    nextString->scaleX = scaleX;
    nextString->scaleY = scaleY;
    nextString->viewportIndex = viewportIndex;
    nextString->smallFont = 0;
}

// Target 0x00401630-0x00401684 formats into the target-observed 512-byte
// stack buffer and then appends to the regular queue.
void AsciiManagerView::AddFormatText(
    AnmFloat3View *position, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    AddString(position, buffer);
    va_end(args);
}

// Target 0x00401690-0x004016F3 uses the same regular queue and marks the
// newly appended record to select the second 0x62-glyph font bank.
void AsciiManagerView::AddSmallFormatText(
    AnmFloat3View *position, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    AddString(position, buffer);
    va_end(args);
    strings[numStrings - 1].smallFont = 1;
}

// Target 0x00401700-0x0040175D appends to the GUI queue and returns the
// formatted byte count. The compiler inlines strlen after the queue call.
int AsciiManagerView::AddGuiFormatText(
    AnmFloat3View *position, const char *format, ...)
{
    char buffer[512];
    va_list args;

    va_start(args, format);
    vsprintf(buffer, format, args);
    AddGuiString(position, buffer);
    va_end(args);
    return strlen(buffer);
}

static void ConfigureAsciiViewport(int viewportIndex)
{
    if (viewportIndex != 0)
        AsciiConfigureBackgroundViewport(0);
    else
        AsciiConfigureBackgroundViewport(1);
}

// Target 0x004423C0-0x004423D8 multiplies two color channels with 7-bit
// normalization and saturates the result to one byte.
unsigned char __fastcall MixAnmColor(
    unsigned char source, unsigned char multiplier)
{
    unsigned int mixed = (source * multiplier) / 128u;
    if (mixed >= 256u)
        mixed = 255u;
    return static_cast<unsigned char>(mixed);
}

// Target 0x004423E0-0x00442597 applies the shared blend/filter cache and
// publishes the selected, optionally mixed VM color as D3D texture factor.
void AnmRenderManagerView::SetRenderStateForVm3D(AnmVmView *vm)
{
    AnmColorView color;

    if (currentBlendMode != vm->blendMode)
    {
        FlushVertexBuffer();
        currentBlendMode = static_cast<unsigned char>(vm->blendMode);

        switch (currentBlendMode)
        {
        case 0:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_INVSRCALPHA);
            break;
        case 1:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_ONE);
            break;
        case 2:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_ONE);
            break;
        }
    }

    color.value = vm->useSecondaryColor
        ? vm->secondaryColor.value : vm->primaryColor.value;
    if (useMixColor)
    {
        color.red = MixAnmColor(color.red, mixColor.red);
        color.green = MixAnmColor(color.green, mixColor.green);
        color.blue = MixAnmColor(color.blue, mixColor.blue);
        color.alpha = MixAnmColor(color.alpha, mixColor.alpha);
    }

    if (currentTextureFactor != color.value)
    {
        FlushVertexBuffer();
        currentTextureFactor = color.value;
        MainSupervisorAnmPrefixView *supervisor =
            reinterpret_cast<MainSupervisorAnmPrefixView *>(
                &g_MainSupervisorView);
        supervisor->d3dDevice->vtable->SetRenderState(
            supervisor->d3dDevice, D3D9_VIEW_RS_TEXTUREFACTOR,
            currentTextureFactor);
    }

    if (currentTextureFilter != vm->usePointTextureFilter)
    {
        FlushVertexBuffer();
        currentTextureFilter =
            static_cast<unsigned char>(vm->usePointTextureFilter);

        if (currentTextureFilter == 0)
        {
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MAGFILTER,
                D3D9_VIEW_TEXF_LINEAR);
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MINFILTER,
                D3D9_VIEW_TEXF_LINEAR);
        }
        else
        {
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MAGFILTER,
                D3D9_VIEW_TEXF_POINT);
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MINFILTER,
                D3D9_VIEW_TEXF_POINT);
        }
    }

    ++renderStateChangesThisFrame;
}

// Target 0x004425A0-0x00442668 batches sprites until either the blend mode or
// texture filter changes. The VM remains live in EDI in the target LTCG seam.
void AnmRenderManagerView::SetRenderStateForVm(AnmVmView *vm)
{
    if (currentBlendMode != vm->blendMode)
    {
        FlushVertexBuffer();
        currentBlendMode = static_cast<unsigned char>(vm->blendMode);

        switch (currentBlendMode)
        {
        case 0:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_INVSRCALPHA);
            break;
        case 1:
            g_Direct3DDevice->vtable->SetRenderState(
                g_Direct3DDevice, D3D9_VIEW_RS_DESTBLEND,
                D3D9_VIEW_BLEND_ONE);
            break;
        }
    }

    if (currentTextureFilter != vm->usePointTextureFilter)
    {
        FlushVertexBuffer();
        currentTextureFilter =
            static_cast<unsigned char>(vm->usePointTextureFilter);

        if (currentTextureFilter == 0)
        {
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MAGFILTER,
                D3D9_VIEW_TEXF_LINEAR);
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MINFILTER,
                D3D9_VIEW_TEXF_LINEAR);
        }
        else
        {
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MAGFILTER,
                D3D9_VIEW_TEXF_POINT);
            g_Direct3DDevice->vtable->SetSamplerState(
                g_Direct3DDevice, 0, D3D9_VIEW_SAMP_MINFILTER,
                D3D9_VIEW_TEXF_POINT);
        }
    }

    ++renderStateChangesThisFrame;
}

extern const float g_AnmHalfPixel = 0.5f;

// Target 0x00442670-0x00442AC6 applies screen shake and the D3D9 half-pixel
// convention, rejects quads outside the active viewport, updates batched
// texture/render state, resolves VM color, and queues the resulting vertices.
int AnmRenderManagerView::DrawInner(AnmVmView *vm, int flags)
{
    AnmColorView color;
    float maxX;
    float minX;
    float maxY;
    float minY;

    g_AnmQuadVertices[0].x += screenShakeX;
    g_AnmQuadVertices[0].y += screenShakeY;
    g_AnmQuadVertices[1].x += screenShakeX;
    g_AnmQuadVertices[1].y += screenShakeY;
    g_AnmQuadVertices[2].x += screenShakeX;
    g_AnmQuadVertices[2].y += screenShakeY;
    g_AnmQuadVertices[3].x += screenShakeX;
    g_AnmQuadVertices[3].y += screenShakeY;

    if ((flags & 1) != 0)
    {
#if defined(_MSC_VER) && defined(_M_IX86)
        // VC7.1 has no C/C++ intrinsic that emits the target's four FRNDINT
        // operations while retaining all values on the x87 stack. This narrow
        // source-level x87 block is also the form preserved by the adjacent
        // engine family; every address below remains a normal linker field.
        __asm
        {
            fld g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmQuadVertices[1 * TYPE g_AnmQuadVertices].x
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].y
            frndint
            fsub g_AnmHalfPixel
            fld g_AnmQuadVertices[2 * TYPE g_AnmQuadVertices].y
            frndint
            fsub g_AnmHalfPixel
            fst g_AnmQuadVertices[2 * TYPE g_AnmQuadVertices].y
            fstp g_AnmQuadVertices[3 * TYPE g_AnmQuadVertices].y
            fst g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].y
            fstp g_AnmQuadVertices[1 * TYPE g_AnmQuadVertices].y
            fst g_AnmQuadVertices[1 * TYPE g_AnmQuadVertices].x
            fstp g_AnmQuadVertices[3 * TYPE g_AnmQuadVertices].x
            fst g_AnmQuadVertices[0 * TYPE g_AnmQuadVertices].x
            fstp g_AnmQuadVertices[2 * TYPE g_AnmQuadVertices].x
        }
#else
        maxX = static_cast<float>(floor(g_AnmQuadVertices[0].x + 0.5f)) -
            g_AnmHalfPixel;
        minX = static_cast<float>(floor(g_AnmQuadVertices[1].x + 0.5f)) -
            g_AnmHalfPixel;
        maxY = static_cast<float>(floor(g_AnmQuadVertices[0].y + 0.5f)) -
            g_AnmHalfPixel;
        minY = static_cast<float>(floor(g_AnmQuadVertices[2].y + 0.5f)) -
            g_AnmHalfPixel;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y = minY;
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y = maxY;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x = minX;
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x = maxX;
#endif
    }

    g_AnmQuadVertices[0].u = g_AnmQuadVertices[2].u =
        vm->loadedSprite->uStart + vm->uvScrollX;
    g_AnmQuadVertices[1].u = g_AnmQuadVertices[3].u =
        vm->loadedSprite->uEnd + vm->uvScrollX;
    g_AnmQuadVertices[0].v = g_AnmQuadVertices[1].v =
        vm->loadedSprite->vStart + vm->uvScrollY;
    g_AnmQuadVertices[2].v = g_AnmQuadVertices[3].v =
        vm->loadedSprite->vEnd + vm->uvScrollY;

    maxX = g_AnmQuadVertices[0].x > g_AnmQuadVertices[1].x
        ? g_AnmQuadVertices[0].x : g_AnmQuadVertices[1].x;
    maxX = g_AnmQuadVertices[2].x > maxX
        ? g_AnmQuadVertices[2].x : maxX;
    maxX = g_AnmQuadVertices[3].x > maxX
        ? g_AnmQuadVertices[3].x : maxX;

    maxY = g_AnmQuadVertices[0].y > g_AnmQuadVertices[1].y
        ? g_AnmQuadVertices[0].y : g_AnmQuadVertices[1].y;
    maxY = g_AnmQuadVertices[2].y > maxY
        ? g_AnmQuadVertices[2].y : maxY;
    maxY = g_AnmQuadVertices[3].y > maxY
        ? g_AnmQuadVertices[3].y : maxY;

    minX = g_AnmQuadVertices[0].x < g_AnmQuadVertices[1].x
        ? g_AnmQuadVertices[0].x : g_AnmQuadVertices[1].x;
    minX = g_AnmQuadVertices[2].x < minX
        ? g_AnmQuadVertices[2].x : minX;
    minX = g_AnmQuadVertices[3].x < minX
        ? g_AnmQuadVertices[3].x : minX;

    minY = g_AnmQuadVertices[0].y < g_AnmQuadVertices[1].y
        ? g_AnmQuadVertices[0].y : g_AnmQuadVertices[1].y;
    minY = g_AnmQuadVertices[2].y < minY
        ? g_AnmQuadVertices[2].y : minY;
    minY = g_AnmQuadVertices[3].y < minY
        ? g_AnmQuadVertices[3].y : minY;

    if (maxX < g_AnmViewportOwner->viewport.x ||
        maxY < g_AnmViewportOwner->viewport.y ||
        minX > g_AnmViewportOwner->viewport.x +
            g_AnmViewportOwner->viewport.width ||
        minY > g_AnmViewportOwner->viewport.y +
            g_AnmViewportOwner->viewport.height)
        return 0;

    if (currentTexture != vm->loadedSprite->texture)
    {
        currentTexture = vm->loadedSprite->texture;
        FlushVertexBuffer();
        g_Direct3DDevice->vtable->SetTexture(
            g_Direct3DDevice, 0, currentTexture);
    }

    if (currentVertexShader != 1)
    {
        FlushVertexBuffer();
        currentVertexShader = 1;
    }

    if ((flags & 2) == 0)
    {
        color.value = vm->useSecondaryColor
            ? vm->secondaryColor.value : vm->primaryColor.value;
        if (useMixColor)
        {
            color.red = MixAnmColor(color.red, mixColor.red);
            color.green = MixAnmColor(color.green, mixColor.green);
            color.blue = MixAnmColor(color.blue, mixColor.blue);
            color.alpha = MixAnmColor(color.alpha, mixColor.alpha);
        }

        g_AnmQuadVertices[0].color = color.value;
        g_AnmQuadVertices[1].color = color.value;
        g_AnmQuadVertices[2].color = color.value;
        g_AnmQuadVertices[3].color = color.value;
    }

    SetRenderStateForVm(vm);
    AddSpriteToDrawBuffer(g_AnmQuadVertices);
    return 0;
}

// Target 0x00442F30-0x00442F4C resets the shared packed-vertex range. This
// CC-delimited owner was absent from the initial Ghidra export; the target-bound
// ledger tracks it.
void AnmRenderManagerView::ClearVertexBuffer()
{
    spritesToDraw = 0;
    vertexBufferStart = vertexBufferEnd = vertexBuffer;
}

// Target 0x00442F50-0x00442FD4 submits two triangle-list primitives per
// queued sprite, advances the submitted range, and records one flush.
void AnmRenderManagerView::FlushVertexBuffer()
{
    if (spritesToDraw == 0)
        return;

    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetFVF(
        g_Direct3DDevice,
        D3D9_VIEW_FVF_XYZRHW |
        D3D9_VIEW_FVF_DIFFUSE |
        D3D9_VIEW_FVF_TEX1);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLELIST,
        spritesToDraw * 2, vertexBufferStart,
        sizeof(AnmRenderVertexView));

    vertexBufferStart = vertexBufferEnd;
    spritesToDraw = 0;
    ++flushesThisFrame;
}

// Target 0x00442FE0-0x0044307C expands one four-corner quad into the six
// packed vertices consumed as two triangles by FlushVertexBuffer.
int AnmRenderManagerView::AddSpriteToDrawBuffer(
    AnmRenderVertexView *vertices)
{
    vertexBufferEnd[0] = vertices[0];
    vertexBufferEnd[1] = vertices[1];
    vertexBufferEnd[2] = vertices[2];
    vertexBufferEnd[3] = vertices[1];
    vertexBufferEnd[4] = vertices[2];
    vertexBufferEnd[5] = vertices[3];
    vertexBufferEnd += 6;
    ++spritesToDraw;
    return 0;
}

// Target 0x00443080-0x00443282 places one axis-aligned quad. Centered axes
// are rounded down before the common render-state/draw path is entered.
int AnmRenderManagerView::DrawNoRotation(AnmVmView *vm)
{
    float spriteWidth;
    float spriteHeight;
    float spriteHalfHeight;

    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;
    spriteHalfHeight = spriteHeight * 0.5f;

    switch ((vm->flags35C >> 18) & 3)
    {
    case 1:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x +
            spriteWidth;
        break;
    case 0:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            static_cast<float>(floor(
                vm->position.x + vm->positionOffset.x +
                vm->alternatePosition.x - spriteWidth * 0.5f));
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            g_AnmQuadVertices[0].x + spriteWidth;
        break;
    case 2:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x -
            spriteWidth;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x;
        break;
    }

    switch ((vm->flags35C >> 20) & 3)
    {
    case 1:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            vm->position.y + vm->positionOffset.y + vm->alternatePosition.y;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            vm->position.y + vm->positionOffset.y + vm->alternatePosition.y +
            spriteHeight;
        break;
    case 0:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            static_cast<float>(floor(
                vm->position.y + vm->positionOffset.y +
                vm->alternatePosition.y - spriteHalfHeight));
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            g_AnmQuadVertices[0].y + spriteHeight;
        break;
    case 2:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            vm->position.y + vm->positionOffset.y + vm->alternatePosition.y -
            spriteHeight;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            vm->position.y + vm->positionOffset.y + vm->alternatePosition.y;
        break;
    }

    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->alternatePosition.z + vm->positionOffset.z + vm->position.z;
    return DrawInner(vm, 1);
}

// Target 0x00443290-0x00443475 preserves sub-pixel centered coordinates and
// requests the non-rounded common draw path.
int AnmRenderManagerView::DrawNoRotationNoRound(AnmVmView *vm)
{
    float spriteWidth;
    float spriteHeight;
    float spriteHalfHeight;

    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;
    spriteHalfHeight = spriteHeight * 0.5f;

    switch ((vm->flags35C >> 18) & 3)
    {
    case 1:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x +
            spriteWidth;
        break;
    case 0:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x -
            spriteWidth * 0.5f;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            g_AnmQuadVertices[0].x + spriteWidth;
        break;
    case 2:
        g_AnmQuadVertices[0].x = g_AnmQuadVertices[2].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x -
            spriteWidth;
        g_AnmQuadVertices[1].x = g_AnmQuadVertices[3].x =
            vm->position.x + vm->positionOffset.x + vm->alternatePosition.x;
        break;
    }

    switch ((vm->flags35C >> 20) & 3)
    {
    case 1:
    {
        float y = vm->alternatePosition.y;
        y += vm->position.y;
        y += vm->positionOffset.y;
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            y;
        y = vm->alternatePosition.y;
        y += vm->position.y;
        y += spriteHeight;
        y += vm->positionOffset.y;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            y;
        break;
    }
    case 0:
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            vm->position.y + vm->positionOffset.y + vm->alternatePosition.y -
            spriteHalfHeight;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            g_AnmQuadVertices[0].y + spriteHeight;
        break;
    case 2:
    {
        float y = vm->alternatePosition.y;
        y += vm->position.y;
        y += vm->positionOffset.y;
        y -= spriteHeight;
        g_AnmQuadVertices[0].y = g_AnmQuadVertices[1].y =
            y;
        y = vm->alternatePosition.y;
        y += vm->position.y;
        y += vm->positionOffset.y;
        g_AnmQuadVertices[2].y = g_AnmQuadVertices[3].y =
            y;
        break;
    }
    }

    // TH10's no-round path intentionally reads alternatePosition.y here; the
    // rounded sibling at 0x00443080 reads alternatePosition.z.
    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->alternatePosition.y + vm->positionOffset.z + vm->position.z;
    return DrawInner(vm, 0);
}

// Target 0x00443680-0x004436B1 applies the renderer's two-dimensional
// rotation and translation to one shared textured vertex. The member receiver
// is unused and disappears from the target's LTCG-internal calling convention.
void AnmRenderManagerView::TranslateRotation(
    AnmRenderVertexView *vertex, float x, float y, float sine,
    float cosine, float xOffset, float yOffset)
{
    vertex->x = x * cosine - y * sine + xOffset;
    vertex->y = x * sine + y * cosine + yOffset;
}

// Target 0x004436C0-0x0044390E is the first of two instruction-identical
// rotated-quad blocks reached by render modes 1 and 3 in the Draw dispatcher.
// TH10 accumulates all three VM position vectors on every axis before rotating
// the anchor-selected corners. A zero rotation enters the established
// sub-pixel axis-aligned path.
int AnmRenderManagerView::Draw2D(AnmVmView *vm)
{
    float rotation;
    float sine;
    float cosine;
    float xOffset;
    float yOffset;
    float spriteWidth;
    float spriteHeight;
    float vertexX[4];
    float vertexY[4];
    int i;

    if (vm->rotation.z == 0.0f)
        return DrawNoRotationNoRound(vm);

    rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    // Keep each three-term sum in one expression: splitting it into compound
    // assignments changes VC7.1's anchor-mask/Y-load schedule.
    xOffset = vm->alternatePosition.x + vm->positionOffset.x + vm->position.x;
    yOffset = vm->alternatePosition.y + vm->positionOffset.y + vm->position.y;
    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;

    switch (vm->renderStateA)
    {
    case 1:
        vertexX[0] = vertexX[2] = 0.0f;
        vertexX[1] = vertexX[3] = spriteWidth;
        break;
    case 0:
        vertexX[0] = vertexX[2] = -spriteWidth * 0.5f;
        vertexX[1] = vertexX[3] = spriteWidth * 0.5f;
        break;
    case 2:
        vertexX[0] = vertexX[2] = -spriteWidth;
        vertexX[1] = vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertexY[0] = vertexY[1] = 0.0f;
        vertexY[2] = vertexY[3] = spriteHeight;
        break;
    case 0:
        vertexY[0] = vertexY[1] = -spriteHeight * 0.5f;
        vertexY[2] = vertexY[3] = spriteHeight * 0.5f;
        break;
    case 2:
        vertexY[0] = vertexY[1] = -spriteHeight;
        vertexY[2] = vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; ++i)
    {
        TranslateRotation(
            &g_AnmQuadVertices[i], vertexX[i], vertexY[i], sine, cosine,
            xOffset, yOffset);
    }

    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->alternatePosition.z + vm->positionOffset.z + vm->position.z;
    return DrawInner(vm, 0);
}

// Target 0x00443910-0x00443B5E is the render-mode 3 sibling retained as a
// separate method. Its complete target body is instruction-identical to
// Draw2D apart from address-dependent REL32 displacements. The descriptive
// name follows the mode-3 owner in TH08 and remains a cross-game hypothesis.
int AnmRenderManagerView::Draw2DRotatedOrAxisAligned(AnmVmView *vm)
{
    float rotation;
    float sine;
    float cosine;
    float xOffset;
    float yOffset;
    float spriteWidth;
    float spriteHeight;
    float vertexX[4];
    float vertexY[4];
    int i;

    if (vm->rotation.z == 0.0f)
        return DrawNoRotationNoRound(vm);

    rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    // Keep each three-term sum in one expression: splitting it into compound
    // assignments changes VC7.1's anchor-mask/Y-load schedule.
    xOffset = vm->alternatePosition.x + vm->positionOffset.x + vm->position.x;
    yOffset = vm->alternatePosition.y + vm->positionOffset.y + vm->position.y;
    spriteWidth = vm->spriteWidth * vm->scaleX;
    spriteHeight = vm->spriteHeight * vm->scaleY;

    switch (vm->renderStateA)
    {
    case 1:
        vertexX[0] = vertexX[2] = 0.0f;
        vertexX[1] = vertexX[3] = spriteWidth;
        break;
    case 0:
        vertexX[0] = vertexX[2] = -spriteWidth * 0.5f;
        vertexX[1] = vertexX[3] = spriteWidth * 0.5f;
        break;
    case 2:
        vertexX[0] = vertexX[2] = -spriteWidth;
        vertexX[1] = vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertexY[0] = vertexY[1] = 0.0f;
        vertexY[2] = vertexY[3] = spriteHeight;
        break;
    case 0:
        vertexY[0] = vertexY[1] = -spriteHeight * 0.5f;
        vertexY[2] = vertexY[3] = spriteHeight * 0.5f;
        break;
    case 2:
        vertexY[0] = vertexY[1] = -spriteHeight;
        vertexY[2] = vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; ++i)
    {
        TranslateRotation(
            &g_AnmQuadVertices[i], vertexX[i], vertexY[i], sine, cosine,
            xOffset, yOffset);
    }

    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            vm->alternatePosition.z + vm->positionOffset.z + vm->position.z;
    return DrawInner(vm, 0);
}

static __forceinline float AnmFloat3Length(const AnmFloat3View &value)
{
    return static_cast<float>(sqrt(
        value.x * value.x + value.y * value.y + value.z * value.z));
}

// Target 0x00443B60-0x00443F76 projects the VM's accumulated world position
// and the current camera-right reference through the active background
// viewport. The projected reference fixes the screen-space sprite scale; the
// VM's Z rotation and two anchor fields then place the shared quad. Unlike the
// adjacent TH095 implementation, TH10 stores the projected depth in every
// vertex.
int AnmRenderManagerView::ProjectCameraFacingQuad(AnmVmView *vm)
{
    float rotation;
    float sine;
    float cosine;
    float xOffset;
    float yOffset;
    float spriteHeight;
    float spriteWidth;
    AnmFloat3View projectedReference;
    AnmMatrixView worldMatrix;
    AnmFloat3View projectedPosition;
    AnmFloat3View origin;
    AnmFloat3View delta;
    float vertexX[4];
    float vertexY[4];
    int i;

    rotation = vm->rotation.z;
#if defined(_MSC_VER) && defined(_M_IX86)
    // Both target sites use one x87 FSINCOS and pop cosine before sine. VC7.1
    // has no C/C++ intrinsic that retains this instruction shape.
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    // These two local identifiers retain the original build-6030 LTCG stack
    // coloring: this first vector supplies the zero projection input, while
    // origin below receives the projected camera-right reference.
    projectedReference.x = 0.0f;
    projectedReference.y = 0.0f;
    projectedReference.z = 0.0f;

    worldMatrix.SetIdentity();
    worldMatrix.values[12] =
        vm->alternatePosition.x + vm->positionOffset.x + vm->position.x;
    worldMatrix.values[13] =
        vm->alternatePosition.y + vm->positionOffset.y + vm->position.y;
    worldMatrix.values[14] =
        vm->alternatePosition.z + vm->positionOffset.z + vm->position.z;

    D3DXVec3Project(
        &projectedPosition, &projectedReference,
        &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);
    if (projectedPosition.z < 0.0f || projectedPosition.z > 1.0f)
        return -1;

    D3DXVec3Project(
        &origin, &g_AnmViewportOwner->cameraRight,
        &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);

    delta = origin - projectedPosition;
    xOffset = AnmFloat3Length(delta) * 0.5f;
    spriteWidth = vm->spriteWidth * vm->scaleX * xOffset;
    spriteHeight = vm->spriteHeight * vm->scaleY * xOffset;
    xOffset = projectedPosition.x;
    yOffset = projectedPosition.y;
    g_AnmQuadVertices[0].z = g_AnmQuadVertices[1].z =
        g_AnmQuadVertices[2].z = g_AnmQuadVertices[3].z =
            projectedPosition.z;

#if defined(_MSC_VER) && defined(_M_IX86)
    __asm
    {
        fld rotation
        fsincos
        fstp cosine
        fstp sine
    }
#else
    cosine = static_cast<float>(cos(rotation));
    sine = static_cast<float>(sin(rotation));
#endif

    switch (vm->renderStateA)
    {
    case 1:
        vertexX[0] = vertexX[2] = 0.0f;
        vertexX[1] = vertexX[3] = spriteWidth;
        break;
    case 0:
        vertexX[0] = vertexX[2] = -spriteWidth * 0.5f;
        vertexX[1] = vertexX[3] = spriteWidth * 0.5f;
        break;
    case 2:
        vertexX[0] = vertexX[2] = -spriteWidth;
        vertexX[1] = vertexX[3] = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertexY[0] = vertexY[1] = 0.0f;
        vertexY[2] = vertexY[3] = spriteHeight;
        break;
    case 0:
        vertexY[0] = vertexY[1] = -spriteHeight * 0.5f;
        vertexY[2] = vertexY[3] = spriteHeight * 0.5f;
        break;
    case 2:
        vertexY[0] = vertexY[1] = -spriteHeight;
        vertexY[2] = vertexY[3] = 0.0f;
        break;
    }

    for (i = 0; i < 4; ++i)
    {
        TranslateRotation(
            &g_AnmQuadVertices[i], vertexX[i], vertexY[i], sine, cosine,
            xOffset, yOffset);
    }

    return 0;
}

// Target 0x00443F80-0x00443FA0 draws the projected quad through the common
// non-rounded renderer path and propagates projection failure.
int AnmRenderManagerView::DrawCameraFacingQuad(AnmVmView *vm)
{
    if (ProjectCameraFacingQuad(vm) != 0)
        return -1;
    return DrawInner(vm, 0);
}

struct AnmPhotoBlendDrawLocals
{
    AnmFloat3View cameraDelta;
    AnmColorView color;
    float distanceRange;
    float distance;
};

// Target 0x00443FB0-0x00444232 is render mode 6. It first places a
// camera-facing quad, then fades its color and alpha across the configured
// camera-distance interval before asking DrawInner to preserve that color.
int AnmRenderManagerView::DrawMode6(AnmVmView *vm)
{
    AnmPhotoBlendDrawLocals draw;

    if (ProjectCameraFacingQuad(vm) != 0)
        return -1;

    draw.distanceRange =
        g_AnmPhotoBlend.nearDistance - g_AnmPhotoBlend.farDistance;
    draw.color.value = vm->useSecondaryColor
        ? vm->secondaryColor.value : vm->primaryColor.value;
    draw.cameraDelta =
        vm->position + vm->positionOffset + vm->alternatePosition -
        g_AnmBackgroundCameraPosition;
    draw.distance = AnmFloat3Length(draw.cameraDelta);

    if (useMixColor)
    {
        draw.color.red = MixAnmColor(draw.color.red, mixColor.red);
        draw.color.green = MixAnmColor(draw.color.green, mixColor.green);
        draw.color.blue = MixAnmColor(draw.color.blue, mixColor.blue);
        draw.color.alpha = MixAnmColor(draw.color.alpha, mixColor.alpha);
    }

    if (g_AnmPhotoBlend.nearDistance < draw.distance)
    {
        draw.distance =
            (g_AnmPhotoBlend.nearDistance - draw.distance) /
            draw.distanceRange;
        if (draw.distance >= 1.0f)
            return -1;

        reinterpret_cast<AnmColorView *>(&g_AnmQuadVertices[0].color)->blue =
            draw.color.blue - static_cast<unsigned char>(
                (draw.color.blue -
                 static_cast<int>(g_AnmPhotoBlend.blue)) *
                draw.distance);
        reinterpret_cast<AnmColorView *>(&g_AnmQuadVertices[0].color)->green =
            draw.color.green - static_cast<unsigned char>(
                (draw.color.green -
                 static_cast<int>(g_AnmPhotoBlend.green)) *
                draw.distance);
        reinterpret_cast<AnmColorView *>(&g_AnmQuadVertices[0].color)->red =
            draw.color.red - static_cast<unsigned char>(
                (draw.color.red -
                 static_cast<int>(g_AnmPhotoBlend.red)) *
                draw.distance);
        reinterpret_cast<AnmColorView *>(&g_AnmQuadVertices[0].color)->alpha =
            static_cast<unsigned char>(
                draw.color.alpha * (1.0f - draw.distance));
    }
    else
    {
        g_AnmQuadVertices[0].color = draw.color.value;
    }

    g_AnmQuadVertices[1].color = g_AnmQuadVertices[0].color;
    g_AnmQuadVertices[2].color = g_AnmQuadVertices[0].color;
    g_AnmQuadVertices[3].color = g_AnmQuadVertices[0].color;
    return DrawInner(vm, 2);
}

// Target 0x00444240-0x00444571 maintains a VM-local transform matrix, applies
// scale and any dirty Euler rotations, translates a local 256-by-256 quad by
// all three VM position vectors, and projects its four corners through the
// active background viewport. The completed world matrix is cached on the
// renderer for the following 3D draw modes. X/Y extend their matrix translation
// while TH10 replaces Z with the accumulated VM position.
int AnmRenderManagerView::Project3DQuad(AnmVmView *vm)
{
    AnmMatrixView rotationMatrix;
    AnmMatrixView worldMatrix;
    AnmFloat3View vertices[4];

    if (!vm->useStaticMatrix && (vm->updateScale || vm->updateRotation))
    {
        vm->matrix27C = vm->matrix23C;
        vm->matrix27C.values[0] *= vm->scaleX;
        vm->matrix27C.values[5] *= vm->scaleY;
        vm->updateScale = 0;

        if (vm->rotation.x != 0.0)
        {
            D3DXMatrixRotationX(&rotationMatrix, vm->rotation.x);
            D3DXMatrixMultiply(
                &vm->matrix27C, &vm->matrix27C, &rotationMatrix);
        }
        if (vm->rotation.y != 0.0)
        {
            D3DXMatrixRotationY(&rotationMatrix, vm->rotation.y);
            D3DXMatrixMultiply(
                &vm->matrix27C, &vm->matrix27C, &rotationMatrix);
        }
        if (vm->rotation.z != 0.0)
        {
            D3DXMatrixRotationZ(&rotationMatrix, vm->rotation.z);
            D3DXMatrixMultiply(
                &vm->matrix27C, &vm->matrix27C, &rotationMatrix);
        }
        vm->updateRotation = 0;
    }

    worldMatrix = vm->matrix27C;
    worldMatrix.values[12] +=
        vm->alternatePosition.x + vm->positionOffset.x + vm->position.x;
    worldMatrix.values[13] +=
        vm->alternatePosition.y + vm->positionOffset.y + vm->position.y;
    worldMatrix.values[14] =
        vm->alternatePosition.z + vm->positionOffset.z + vm->position.z;

    switch (vm->renderStateA)
    {
    case 1:
        vertices[0].x = vertices[2].x = 0.0f;
        vertices[1].x = vertices[3].x = 256.0f;
        break;
    case 0:
        vertices[0].x = vertices[2].x = -128.0f;
        vertices[1].x = vertices[3].x = 128.0f;
        break;
    case 2:
        vertices[0].x = vertices[2].x = -256.0f;
        vertices[1].x = vertices[3].x = 0.0f;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        vertices[0].y = vertices[1].y = 0.0f;
        vertices[2].y = vertices[3].y = 256.0f;
        break;
    case 0:
        vertices[0].y = vertices[1].y = -128.0f;
        vertices[2].y = vertices[3].y = 128.0f;
        break;
    case 2:
        vertices[0].y = vertices[1].y = -256.0f;
        vertices[2].y = vertices[3].y = 0.0f;
        break;
    }

    vertices[0].z = vertices[1].z =
        vertices[2].z = vertices[3].z = 0.0f;

    D3DXVec3Project(
        reinterpret_cast<AnmFloat3View *>(&g_AnmQuadVertices[0]),
        &vertices[0], &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);
    D3DXVec3Project(
        reinterpret_cast<AnmFloat3View *>(&g_AnmQuadVertices[1]),
        &vertices[1], &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);
    D3DXVec3Project(
        reinterpret_cast<AnmFloat3View *>(&g_AnmQuadVertices[2]),
        &vertices[2], &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);
    D3DXVec3Project(
        reinterpret_cast<AnmFloat3View *>(&g_AnmQuadVertices[3]),
        &vertices[3], &g_AnmViewportOwner->viewport,
        &g_AnmViewportOwner->projectionMatrix,
        &g_AnmViewportOwner->viewMatrix, &worldMatrix);

    cachedWorldMatrix = worldMatrix;
    return 0;
}

// Target 0x00444580-0x004445BB restores the projected RHW values after the
// common renderer has consumed the quad and propagates DrawInner's result.
int AnmRenderManagerView::DrawProjected3DQuad(AnmVmView *vm)
{
    int result;

    Project3DQuad(vm);
    result = DrawInner(vm, 0);
    g_AnmQuadVertices[0].rhw = g_AnmQuadVertices[1].rhw =
        g_AnmQuadVertices[2].rhw = g_AnmQuadVertices[3].rhw = 1.0f;
    return result;
}

struct AnmProjectedPhotoBlendDrawLocals
{
    // VC7.1 reuses the live component slot for each byte-to-float conversion.
    // This is target arithmetic storage, not inert stack-shape padding.
    AnmColorView color;
    float distanceRange;
    int colorComponent;
};

// Target 0x004445C0-0x00444751 is render mode 7. It transforms each local
// source vertex through the matrix cached by Project3DQuad and applies the
// background photo blend independently from that vertex's camera distance.
int AnmRenderManagerView::DrawMode7(AnmVmView *vm)
{
    Project3DQuad(vm);

    AnmFloat4View transformedVertices[4];
    AnmProjectedPhotoBlendDrawLocals draw;
    AnmFloat3View cameraDelta;
    float distance;
    int result;
    int i;
    // Red and alpha are consumed together after the final float conversion.
    // Preserve that packed color pair while publishing the two result bytes.
    unsigned short redAlpha;

    draw.distanceRange =
        g_AnmPhotoBlend.nearDistance - g_AnmPhotoBlend.farDistance;
    draw.color.value = vm->useSecondaryColor
        ? vm->secondaryColor.value : vm->primaryColor.value;

    for (i = 0; i < 4; ++i)
    {
        D3DXVec4Transform(
            &transformedVertices[i],
            reinterpret_cast<const AnmFloat4View *>(&untexturedVertices[i]),
            &cachedWorldMatrix);
        cameraDelta.x = transformedVertices[i].x -
            g_AnmBackgroundCameraPosition.x;
        cameraDelta.y = transformedVertices[i].y -
            g_AnmBackgroundCameraPosition.y;
        cameraDelta.z = transformedVertices[i].z -
            g_AnmBackgroundCameraPosition.z;
        distance = AnmFloat3Length(cameraDelta);

        if (g_AnmPhotoBlend.nearDistance < distance)
        {
            distance =
                (g_AnmPhotoBlend.nearDistance - distance) /
                draw.distanceRange;
            if (distance >= 1.0f)
            {
                g_AnmQuadVertices[i].color =
                    g_AnmPhotoBlend.farColor.value;
                g_AnmQuadVertices[i].diffuse.alpha = draw.color.alpha;
            }
            else
            {
                draw.colorComponent = draw.color.blue;
                g_AnmQuadVertices[i].diffuse.blue =
                    draw.color.blue - static_cast<unsigned char>(
                        (static_cast<float>(draw.colorComponent) -
                         g_AnmPhotoBlend.blue) * distance);
                draw.colorComponent = draw.color.green;
                g_AnmQuadVertices[i].diffuse.green =
                    draw.color.green - static_cast<unsigned char>(
                        (static_cast<float>(draw.colorComponent) -
                         g_AnmPhotoBlend.green) * distance);
                draw.colorComponent = draw.color.red;
                redAlpha = *reinterpret_cast<unsigned short *>(
                    &draw.color.red);
                g_AnmQuadVertices[i].diffuse.red =
                    static_cast<unsigned char>(redAlpha) -
                    static_cast<unsigned char>(
                        (static_cast<float>(draw.colorComponent) -
                         g_AnmPhotoBlend.red) * distance);
                g_AnmQuadVertices[i].diffuse.alpha =
                    static_cast<unsigned char>(redAlpha >> 8);
            }
        }
        else
        {
            g_AnmQuadVertices[i].color = draw.color.value;
        }
    }

    result = DrawInner(vm, 2);
    g_AnmQuadVertices[0].rhw = g_AnmQuadVertices[1].rhw =
        g_AnmQuadVertices[2].rhw = g_AnmQuadVertices[3].rhw = 1.0f;
    return result;
}

// Target 0x00444760-0x00444B00 flushes the two-dimensional batch, prepares a
// VM world transform and texture transform, installs the direct-3D stream and
// fixed-function state, and submits the renderer's four-vertex strip.
int AnmRenderManagerView::Draw3D(AnmVmView *vm)
{
    AnmMatrixView textureMatrix;
    AnmMatrixView rotationMatrix;
    AnmMatrixView worldMatrix;

    if (!vm->visible)
        return -1;
    if (!vm->drawEnabled)
        return -1;
    if (vm->primaryColor.alpha == 0)
        return -1;

    if (spritesToDraw != 0)
        FlushVertexBuffer();

    if (!vm->useStaticMatrix && (vm->updateScale || vm->updateRotation))
    {
        vm->matrix27C = vm->matrix23C;
        vm->matrix27C.values[0] *= vm->scaleX;
        vm->matrix27C.values[5] *= vm->scaleY;
        vm->updateScale = 0;

        if (vm->rotation.x != 0.0)
        {
            D3DXMatrixRotationX(&rotationMatrix, vm->rotation.x);
            D3DXMatrixMultiply(
                &vm->matrix27C, &vm->matrix27C, &rotationMatrix);
        }
        if (vm->rotation.y != 0.0)
        {
            D3DXMatrixRotationY(&rotationMatrix, vm->rotation.y);
            D3DXMatrixMultiply(
                &vm->matrix27C, &vm->matrix27C, &rotationMatrix);
        }
        if (vm->rotation.z != 0.0)
        {
            D3DXMatrixRotationZ(&rotationMatrix, vm->rotation.z);
            D3DXMatrixMultiply(
                &vm->matrix27C, &vm->matrix27C, &rotationMatrix);
        }
        vm->updateRotation = 0;
    }

    worldMatrix = vm->matrix27C;
    switch (vm->renderStateA)
    {
    case 1:
        worldMatrix.values[12] =
            vm->alternatePosition.x + vm->positionOffset.x + vm->position.x -
            static_cast<float>(fabs(
                vm->spriteWidth * vm->scaleX * 0.5f));
        break;
    case 0:
        worldMatrix.values[12] =
            vm->alternatePosition.x + vm->positionOffset.x + vm->position.x;
        break;
    case 2:
        worldMatrix.values[12] =
            static_cast<float>(fabs(
                vm->spriteWidth * vm->scaleX * 0.5f)) +
            vm->alternatePosition.x + vm->positionOffset.x + vm->position.x;
        break;
    }

    switch (vm->renderStateB)
    {
    case 1:
        worldMatrix.values[13] =
            vm->alternatePosition.y + vm->positionOffset.y + vm->position.y -
            static_cast<float>(fabs(
                vm->spriteHeight * vm->scaleY * 0.5f));
        break;
    case 0:
        worldMatrix.values[13] =
            vm->alternatePosition.y + vm->positionOffset.y + vm->position.y;
        break;
    case 2:
        worldMatrix.values[13] =
            static_cast<float>(fabs(
                vm->spriteHeight * vm->scaleY * 0.5f)) +
            vm->alternatePosition.y + vm->positionOffset.y + vm->position.y;
        break;
    }

    worldMatrix.values[14] = vm->position.z + vm->positionOffset.z;
    SetRenderStateForVm3D(vm);
    worldMatrix.values[14] =
        vm->alternatePosition.z + vm->position.z + vm->positionOffset.z;
    g_Direct3DDevice->vtable->SetTransform(
        g_Direct3DDevice, D3D9_VIEW_TS_WORLD, &worldMatrix);

    if (currentTexture != vm->loadedSprite->texture)
    {
        currentTexture = vm->loadedSprite->texture;
        MainSupervisorAnmPrefixView *supervisor =
            reinterpret_cast<MainSupervisorAnmPrefixView *>(
                &g_MainSupervisorView);
        supervisor->d3dDevice->SetTexture(0, currentTexture);
    }

    if (currentSprite != vm->loadedSprite ||
        vm->uvScrollX != 0.0f || vm->uvScrollX != 0.0f)
    {
        currentSprite = vm->loadedSprite;
        textureMatrix = vm->textureMatrix2BC;
        textureMatrix.values[8] = vm->loadedSprite->uStart + vm->uvScrollX;
        textureMatrix.values[9] = vm->loadedSprite->vStart + vm->uvScrollY;
        g_Direct3DDevice->vtable->SetTransform(
            g_Direct3DDevice, D3D9_VIEW_TS_TEXTURE0, &textureMatrix);
    }

    if (currentVertexShader != 2)
    {
        g_Direct3DDevice->vtable->SetStreamSource(
            g_Direct3DDevice, 0, quadVertexBuffer, 0,
            sizeof(AnmUntexturedVertexView));
        g_Direct3DDevice->vtable->SetFVF(
            g_Direct3DDevice, D3D9_VIEW_FVF_XYZ | D3D9_VIEW_FVF_TEX1);
        g_Direct3DDevice->vtable->SetTextureStageState(
            g_Direct3DDevice, 0,
            D3D9_VIEW_TSS_ALPHAARG2, D3D9_VIEW_TA_TFACTOR);
        g_Direct3DDevice->vtable->SetTextureStageState(
            g_Direct3DDevice, 0,
            D3D9_VIEW_TSS_COLORARG2, D3D9_VIEW_TA_TFACTOR);
        currentVertexShader = 2;
    }

    g_Direct3DDevice->vtable->DrawPrimitive(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLESTRIP, 0, 2);
    return 0;
}

// Target 0x00444B10-0x00444BD8 initializes alternating top and bottom rows
// across a horizontal generated-vertex strip.
int AnmRenderManagerView::InitializeHorizontalTextureStrip(
    AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount)
{
    float y;
    int i;
    AnmRenderVertexView *vertex;
    float x;
    float currentX;
    float step;
    float xSpan;

    if (vertexCount < 3)
        return -1;

    x = vm->loadedSprite->uEnd + vm->uvScrollX;
    xSpan = vm->loadedSprite->uEnd - vm->loadedSprite->uStart;
    y = vm->loadedSprite->vStart + vm->uvScrollY;
    vertex = vertices;
    step = xSpan / ((vertexCount + 1) / 2 - 1);
    i = 0;
    currentX = x;
    for (; i < vertexCount; i += 2, vertex += 2, currentX -= step)
    {
        vertex->u = currentX;
        vertex->v = y;
        vertex->color = vm->primaryColor.value;
        vertex->rhw = 1.0f;
    }

    y = vm->loadedSprite->vEnd + vm->uvScrollY;
    vertex = vertices + 1;
    i = 1;
    currentX = x;
    for (; i < vertexCount; i += 2, vertex += 2, currentX -= step)
    {
        vertex->u = currentX;
        vertex->v = y;
        vertex->color = vm->primaryColor.value;
        vertex->rhw = 1.0f;
    }
    return 0;
}

// Target 0x00444BE0-0x00444CA8 is the vertical counterpart: alternating
// left and right columns receive decreasing V coordinates.
int AnmRenderManagerView::InitializeVerticalTextureStrip(
    AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount)
{
    float x;
    int i;
    AnmRenderVertexView *vertex;
    float y;
    float currentY;
    float step;
    float ySpan;

    if (vertexCount < 3)
        return -1;

    y = vm->loadedSprite->vEnd + vm->uvScrollY;
    ySpan = vm->loadedSprite->vEnd - vm->loadedSprite->vStart;
    x = vm->loadedSprite->uStart + vm->uvScrollX;
    vertex = vertices;
    step = ySpan / ((vertexCount + 1) / 2 - 1);
    i = 0;
    currentY = y;
    for (; i < vertexCount; i += 2, vertex += 2, currentY -= step)
    {
        vertex->v = currentY;
        vertex->u = x;
        vertex->color = vm->primaryColor.value;
        vertex->rhw = 1.0f;
    }

    x = vm->loadedSprite->uEnd + vm->uvScrollX;
    vertex = vertices + 1;
    i = 1;
    currentY = y;
    for (; i < vertexCount; i += 2, vertex += 2, currentY -= step)
    {
        vertex->v = currentY;
        vertex->u = x;
        vertex->color = vm->primaryColor.value;
        vertex->rhw = 1.0f;
    }
    return 0;
}

// Target 0x00444CB0-0x00444CD1 copies the VM primary color into an arbitrary
// generated-vertex range. LTCG removes the unused manager receiver.
int AnmRenderManagerView::SetGeneratedVertexColor(
    AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount)
{
    while (vertexCount-- > 0)
    {
        vertices->color = vm->primaryColor.value;
        ++vertices;
    }
    return 0;
}

// Target 0x00444CE0-0x00444DBF is the render-mode-9 textured triangle-strip
// submitter. Vertex colors are consumed directly through D3DTA_DIFFUSE.
int AnmRenderManagerView::DrawGeneratedVertices(
    AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount)
{
    if (!vm->visible)
        return -1;
    if (!vm->drawEnabled)
        return -1;
    if (vm->primaryColor.alpha == 0)
        return -1;

    if (spritesToDraw != 0)
        FlushVertexBuffer();

    if (currentTexture != vm->loadedSprite->texture)
    {
        currentTexture = vm->loadedSprite->texture;
        MainSupervisorAnmPrefixView *supervisor =
            reinterpret_cast<MainSupervisorAnmPrefixView *>(
                &g_MainSupervisorView);
        supervisor->d3dDevice->SetTexture(0, currentTexture);
    }

    if (currentVertexShader != 3)
    {
        g_Direct3DDevice->vtable->SetFVF(
            g_Direct3DDevice,
            D3D9_VIEW_FVF_XYZRHW | D3D9_VIEW_FVF_DIFFUSE |
                D3D9_VIEW_FVF_TEX1);
        currentVertexShader = 3;
    }

    SetRenderStateForVm(vm);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLESTRIP,
        vertexCount - 2, vertices, sizeof(AnmRenderVertexView));
    return 0;
}

// Target 0x00444DC0-0x00444E4E queues one caller-supplied textured quad in
// the shared six-vertex batch after applying the VM's texture/render state.
int AnmRenderManagerView::QueueSpriteQuad(
    AnmVmView *vm, AnmRenderVertexView *vertices)
{
    if (!vm->visible)
        return -1;
    if (!vm->drawEnabled)
        return -1;
    if (vm->primaryColor.alpha == 0)
        return -1;

    if (currentTexture != vm->loadedSprite->texture)
    {
        currentTexture = vm->loadedSprite->texture;
        FlushVertexBuffer();
        g_Direct3DDevice->SetTexture(0, currentTexture);
    }

    if (currentVertexShader != 1)
    {
        FlushVertexBuffer();
        currentVertexShader = 1;
    }

    SetRenderStateForVm(vm);
    AddSpriteToDrawBuffer(vertices);
    return 0;
}

// Targets 0x00444E60 and 0x00444FA0 install the untextured diffuse pipeline,
// temporarily disable Z writes, submit a strip or fan, and restore the normal
// texture-stage operations and renderer caches.
int AnmRenderManagerView::DrawUntexturedTriangleStrip(
    AnmVmView *vm, AnmUntexturedVertexView *vertices, int vertexCount)
{
    if (spritesToDraw != 0)
        FlushVertexBuffer();

    if (currentVertexShader != 4)
    {
        g_Direct3DDevice->vtable->SetFVF(
            g_Direct3DDevice,
            D3D9_VIEW_FVF_XYZRHW | D3D9_VIEW_FVF_DIFFUSE);
        currentVertexShader = 4;
    }

    SetRenderStateForVm(vm);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAOP, D3D9_VIEW_TOP_SELECTARG1);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLOROP, D3D9_VIEW_TOP_SELECTARG1);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG1, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG1, D3D9_VIEW_TA_DIFFUSE);

    g_AnmRenderManagerView->FlushVertexBuffer();
    g_Direct3DDevice->vtable->SetRenderState(
        g_Direct3DDevice, D3D9_VIEW_RS_ZWRITEENABLE, 0);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLESTRIP,
        vertexCount - 2, vertices, sizeof(AnmUntexturedVertexView));

    g_AnmRenderManagerView->currentVertexShader = 0xff;
    g_AnmRenderManagerView->currentColorOperation = 0xff;
    g_AnmRenderManagerView->currentBlendMode = 3;
    g_AnmRenderManagerView->currentZWrite = 0xff;
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAOP, D3D9_VIEW_TOP_MODULATE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLOROP, D3D9_VIEW_TOP_MODULATE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG1, D3D9_VIEW_TA_TEXTURE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG1, D3D9_VIEW_TA_TEXTURE);
    return 0;
}

int AnmRenderManagerView::DrawUntexturedTriangleFan(
    AnmVmView *vm, AnmUntexturedVertexView *vertices, int vertexCount)
{
    if (spritesToDraw != 0)
        FlushVertexBuffer();

    if (currentVertexShader != 4)
    {
        g_Direct3DDevice->vtable->SetFVF(
            g_Direct3DDevice,
            D3D9_VIEW_FVF_XYZRHW | D3D9_VIEW_FVF_DIFFUSE);
        currentVertexShader = 4;
    }

    SetRenderStateForVm(vm);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAOP, D3D9_VIEW_TOP_SELECTARG1);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLOROP, D3D9_VIEW_TOP_SELECTARG1);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG1, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG1, D3D9_VIEW_TA_DIFFUSE);

    g_AnmRenderManagerView->FlushVertexBuffer();
    g_Direct3DDevice->vtable->SetRenderState(
        g_Direct3DDevice, D3D9_VIEW_RS_ZWRITEENABLE, 0);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLEFAN,
        vertexCount - 2, vertices, sizeof(AnmUntexturedVertexView));

    g_AnmRenderManagerView->currentVertexShader = 0xff;
    g_AnmRenderManagerView->currentColorOperation = 0xff;
    g_AnmRenderManagerView->currentBlendMode = 3;
    g_AnmRenderManagerView->currentZWrite = 0xff;
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAOP, D3D9_VIEW_TOP_MODULATE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLOROP, D3D9_VIEW_TOP_MODULATE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG1, D3D9_VIEW_TA_TEXTURE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG1, D3D9_VIEW_TA_TEXTURE);
    return 0;
}

// Target 0x004450E0-0x004451B2 is the textured fan submitter used by the
// 33-vertex generated trail callback at 0x00445880.
int AnmRenderManagerView::DrawTexturedTriangleFan(
    AnmVmView *vm, AnmRenderVertexView *vertices, int vertexCount)
{
    if (spritesToDraw != 0)
        FlushVertexBuffer();

    if (currentVertexShader != 3)
    {
        g_Direct3DDevice->vtable->SetFVF(
            g_Direct3DDevice,
            D3D9_VIEW_FVF_XYZRHW | D3D9_VIEW_FVF_DIFFUSE |
                D3D9_VIEW_FVF_TEX1);
        currentVertexShader = 3;
    }

    SetRenderStateForVm(vm);

    if (currentTexture != vm->loadedSprite->texture)
    {
        currentTexture = vm->loadedSprite->texture;
        MainSupervisorAnmPrefixView *supervisor =
            reinterpret_cast<MainSupervisorAnmPrefixView *>(
                &g_MainSupervisorView);
        supervisor->d3dDevice->SetTexture(0, currentTexture);
    }

    g_AnmRenderManagerView->FlushVertexBuffer();
    g_Direct3DDevice->vtable->SetRenderState(
        g_Direct3DDevice, D3D9_VIEW_RS_ZWRITEENABLE, 0);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_ALPHAARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->SetTextureStageState(
        g_Direct3DDevice, 0,
        D3D9_VIEW_TSS_COLORARG2, D3D9_VIEW_TA_DIFFUSE);
    g_Direct3DDevice->vtable->DrawPrimitiveUP(
        g_Direct3DDevice, D3D9_VIEW_PT_TRIANGLEFAN,
        vertexCount - 2, vertices, sizeof(AnmRenderVertexView));
    return 0;
}

// Target 0x004451C0-0x0044526C rejects inactive or transparent VMs and
// dispatches the four-bit render mode through a ten-entry jump table.
int AnmRenderManagerView::Draw(AnmVmView *vm)
{
    // The target keeps one shared rejection block immediately before the
    // render-mode dispatch.
    if (!vm->visible)
        goto invalid_vm;
    if (!vm->drawEnabled)
        goto invalid_vm;
    if (vm->primaryColor.alpha == 0)
    {
    invalid_vm:
        return -1;
    }

    switch (vm->renderMode)
    {
    case 0:
        return DrawNoRotation(vm);
    case 1:
        return Draw2D(vm);
    case 4:
        return DrawCameraFacingQuad(vm);
    case 5:
        return DrawProjected3DQuad(vm);
    case 6:
        return DrawMode6(vm);
    case 7:
        return DrawMode7(vm);
    case 8:
        return Draw3D(vm);
    case 9:
        return DrawGeneratedVertices(
            vm, reinterpret_cast<AnmRenderVertexView *>(vm->generatedVertices),
            vm->generatedVertexCount * 2);
    case 2:
        return DrawNoRotationNoRound(vm);
    case 3:
        return Draw2DRotatedOrAxisAligned(vm);
    default:
        return 0;
    }
}

// Target 0x004452F0-0x00445612 allocates the complete 33-vertex radial-fan
// payload and installs its update/draw callbacks. The first vertex is the fan
// center; vertices 1..31 form the ring and the updater closes it at index 32.
int AnmVmView::InitializePulsingRadialTrail()
{
    PulsingRadialTrailDataView *data;
    AnmRenderVertexView *vertex;
    float angle;
    float radialVelocity;
    int i;

    if (generatedVertices != NULL)
    {
        free(generatedVertices);
        generatedVertices = NULL;
    }

    generatedVertices = malloc(sizeof(PulsingRadialTrailDataView));
    positionCallback = UpdatePulsingRadialTrail;
    drawCallback = DrawPulsingRadialTrail;

    data = static_cast<PulsingRadialTrailDataView *>(generatedVertices);
    data->uvVelocity.x =
        g_RngView.GetRandomF32Signed() * (1.0f / 120.0f);
    data->uvVelocity.y =
        g_RngView.GetRandomF32Signed() * (1.0f / 120.0f);

    angle = -3.1415927f;
    vertex = data->vertices;
    *reinterpret_cast<AnmFloat3View *>(&vertex->x) =
        position + positionOffset;
    vertex->rhw = 1.0f;
    vertex->uv.x = 0.5f;
    vertex->uv.y = 0.5f;
    ++vertex;

    radialVelocity =
        g_RngView.GetRandomF32Signed() * (1.0f / 15.0f);
    for (i = 1; i < 32; ++i)
    {
        AnmFloat3View direction;

        if (angle >= 3.1415927f)
            angle -= 6.2831855f;

        vertex->rhw = 1.0f;
        direction.FromAngleMagnitude(angle, 0.5f);
        vertex->z = 0.0f;
        vertex->uv.x = direction.x + 0.5f;
        vertex->uv.y = direction.y + 0.5f;

        data->radii[i] =
            g_RngView.GetRandomF32Signed() * 8.0f + 80.0f;
        data->radialVelocities[i] = radialVelocity;
        radialVelocity +=
            g_RngView.GetRandomF32Signed() * (1.0f / 30.0f);
        if (radialVelocity < -(1.0f / 15.0f))
            radialVelocity = -(1.0f / 15.0f);
        else if (radialVelocity > (1.0f / 15.0f))
            radialVelocity = (1.0f / 15.0f);

        reinterpret_cast<AnmFloat3View *>(&vertex->x)->FromAngleMagnitude(
            angle, data->radii[i]);
        *reinterpret_cast<AnmFloat3View *>(&vertex->x) +=
            this->position + positionOffset;
        ++vertex;
        angle += 0.2026834041f;
    }

    return 0;
}

// Target 0x00445620-0x00445875 advances the ring radii and scrolls its UVs.
// TH10 uses the X velocity for both U and V, including the center vertex.
int __fastcall UpdatePulsingRadialTrail(AnmVmView *vm)
{
    int i;
    AnmRenderVertexView *vertex;
    PulsingRadialTrailDataView *data;
    float angleStep;
    float angle;
    int firstUWrapIndex;
    int firstVWrapIndex;
    int uWrapIndex;
    int vWrapIndex;

    data = static_cast<PulsingRadialTrailDataView *>(vm->generatedVertices);
    angleStep = 0.2026834041f;
    angle = -3.1415927f;
    vertex = data->vertices;

    *reinterpret_cast<AnmFloat3View *>(&vertex->x) =
        vm->position + vm->positionOffset;
    vertex->uv.x += data->uvVelocity.x;
    if (vertex->uv.x < 0.0f)
    {
        for (firstUWrapIndex = 0; firstUWrapIndex < 33; ++firstUWrapIndex)
            data->vertices[firstUWrapIndex].uv.x += 1.0f;
    }
    vertex->uv.y += data->uvVelocity.x;
    if (vertex->uv.y < 0.0f)
    {
        for (firstVWrapIndex = 0; firstVWrapIndex < 33; ++firstVWrapIndex)
            data->vertices[firstVWrapIndex].uv.y += 1.0f;
    }
    vertex->diffuse.value = vm->primaryColor.value;
    ++vertex;

    for (i = 1; i < 32; ++i)
    {
        vertex->uv.x += data->uvVelocity.x;
        if (vertex->uv.x < 0.0f)
        {
            for (uWrapIndex = 0; uWrapIndex < 33; ++uWrapIndex)
                data->vertices[uWrapIndex].uv.x += 1.0f;
        }
        vertex->uv.y += data->uvVelocity.x;
        if (vertex->uv.y < 0.0f)
        {
            for (vWrapIndex = 0; vWrapIndex < 33; ++vWrapIndex)
                data->vertices[vWrapIndex].uv.y += 1.0f;
        }

        vertex->diffuse.value = vm->primaryColor.value;
        vertex->diffuse.alpha = 0;
        data->radii[i] += data->radialVelocities[i];
        reinterpret_cast<AnmFloat3View *>(&vertex->x)->FromAngleMagnitude(
            angle, data->radii[i]);
        *reinterpret_cast<AnmFloat3View *>(&vertex->x) +=
            vm->position + vm->positionOffset;
        ++vertex;
        angle += angleStep;
    }

    *vertex = data->vertices[1];
    return 0;
}

// Target 0x00445880-0x00445898 submits the generated fan as 31 triangles.
int __fastcall DrawPulsingRadialTrail(AnmVmView *vm)
{
    PulsingRadialTrailDataView *data =
        static_cast<PulsingRadialTrailDataView *>(vm->generatedVertices);
    g_AnmRenderManagerView->DrawTexturedTriangleFan(vm, data->vertices, 33);
    return 0;
}

// Target 0x00409230-0x00409261 is a registered-style draw callback for an
// owner with two adjacent ANM VMs. The concrete subsystem owner is still
// unknown; the field names describe only the target-proved view used here.
struct AnmPairDrawOwnerView
{
    unsigned char unknown000[0x010];
    AnmVmView firstVm;
    AnmVmView secondVm;
    unsigned char unknown768[0x3024];
    unsigned int drawFlags;
};

typedef char AnmPairDrawOwnerFlagsAt378C[
    (offsetof(AnmPairDrawOwnerView, drawFlags) == 0x378c) ? 1 : -1];

int __fastcall AnmPairOwnerDrawCallback(AnmPairDrawOwnerView *owner)
{
    if (owner->drawFlags & 1)
    {
        g_AnmRenderManagerView->Draw(&owner->firstVm);
        g_AnmRenderManagerView->Draw(&owner->secondVm);
    }
    return 1;
}

// Target 0x00401760-0x00401A41 renders the 256-entry regular queue. Every
// field read below is independently visible in the target body; the renderer
// and viewport helper names remain semantic until their own owners are closed.
int __stdcall AsciiManagerDrawStrings(AsciiManagerView *manager)
{
    float spaceWidth;
    float lineHeight;
    int currentViewport = 1;
    int index;
    AsciiManagerStringView *string = manager->strings;

    // The target writes the complete containing dword and establishes the
    // two high render-state bits as well as the low visible bit.
    *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) =
        (*reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) &
         0xffd7ffffu) | 0x00140001u;

    for (index = 0; index < manager->numStrings; ++index, ++string)
    {
        unsigned char *text;

        manager->primaryVm014.position = string->position;
        manager->primaryVm014.scaleX = string->scaleX;
        manager->primaryVm014.scaleY = string->scaleY;
        *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) |= 8;

        if (string->smallFont == 1)
        {
            spaceWidth = 7.0f;
            lineHeight = 9.0f;
        }
        else
        {
            spaceWidth = manager->spaceWidth * string->scaleX;
            lineHeight = 14.0f;
        }

        if (currentViewport != string->viewportIndex)
        {
            g_AnmRenderManagerView->FlushVertexBuffer();
            currentViewport = string->viewportIndex;
            ConfigureAsciiViewport(currentViewport);
        }

        text = reinterpret_cast<unsigned char *>(string->text);
        while (*text != '\0')
        {
            if (*text == '\n')
            {
                manager->primaryVm014.position.y +=
                    lineHeight * string->scaleY;
                manager->primaryVm014.position.x = string->position.x;
            }
            else if (*text != ' ')
            {
                AnmSpriteView *sprite =
                    &manager->asciiAnm->sprites[
                        string->smallFont * 0x62 + *text - ' '];
                manager->primaryVm014.loadedSprite = sprite;
                manager->primaryVm014.spriteWidth = sprite->width;
                manager->primaryVm014.spriteHeight = sprite->height;

                if (string->drawShadow != 0)
                {
                    manager->primaryVm014.primaryColor.value =
                        static_cast<int>(string->color & 0xff000000u);
                    reinterpret_cast<unsigned char *>(
                        &manager->primaryVm014.primaryColor.value)[3] =
                            static_cast<unsigned char>(string->color >> 25);
                    manager->primaryVm014.position.x += 2.0f;
                    manager->primaryVm014.position.y += 2.0f;
                    g_AnmRenderManagerView->DrawNoRotation(
                        &manager->primaryVm014);
                    manager->primaryVm014.position.x -= 2.0f;
                    manager->primaryVm014.position.y -= 2.0f;
                }

                manager->primaryVm014.primaryColor.value = string->color;
                g_AnmRenderManagerView->DrawNoRotation(
                    &manager->primaryVm014);
            }
            manager->primaryVm014.position.x += spaceWidth;
            ++text;
        }
    }

    if (currentViewport != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        ConfigureAsciiViewport(0);
    }
    return 1;
}

// Target 0x00401A50-0x00401C89 renders the 64-entry GUI queue. The sole draw
// mode split is the exact floating-point comparison against scale 1.0.
int __stdcall AsciiManagerDrawGuiStrings(AsciiManagerView *manager)
{
    float spaceWidth;
    int currentViewport = 1;
    int index;
    AsciiManagerStringView *string = manager->guiStrings;

    *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) =
        (*reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) &
         0xffd7ffffu) | 0x00140001u;

    for (index = 0; index < manager->numGuiStrings; ++index, ++string)
    {
        unsigned char *text;

        manager->primaryVm014.position = string->position;
        manager->primaryVm014.scaleX = string->scaleX;
        manager->primaryVm014.scaleY = string->scaleY;
        *reinterpret_cast<unsigned int *>(&manager->primaryVm014.flags35C) |= 8;
        spaceWidth = manager->spaceWidth * string->scaleX;

        if (currentViewport != string->viewportIndex)
        {
            g_AnmRenderManagerView->FlushVertexBuffer();
            currentViewport = string->viewportIndex;
            ConfigureAsciiViewport(currentViewport);
        }

        text = reinterpret_cast<unsigned char *>(string->text);
        while (*text != '\0')
        {
            if (*text == '\n')
            {
                manager->primaryVm014.position.y += 14.0f * string->scaleY;
                manager->primaryVm014.position.x = string->position.x;
            }
            else if (*text != ' ')
            {
                AnmSpriteView *sprite =
                    &manager->asciiAnm->sprites[*text - ' '];
                manager->primaryVm014.loadedSprite = sprite;
                manager->primaryVm014.primaryColor.value = string->color;
                if (manager->primaryVm014.scaleX == 1.0f)
                    g_AnmRenderManagerView->DrawNoRotation(
                        &manager->primaryVm014);
                else
                    g_AnmRenderManagerView->DrawNoRotationNoRound(
                        &manager->primaryVm014);
            }
            manager->primaryVm014.position.x += spaceWidth;
            ++text;
        }
    }

    if (currentViewport != 0)
    {
        g_AnmRenderManagerView->FlushVertexBuffer();
        ConfigureAsciiViewport(0);
    }
    return 1;
}
