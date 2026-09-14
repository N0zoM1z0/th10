#include "AnmManager.hpp"

#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AsciiManagerView *g_AsciiManagerView;

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
    case ANM_VAR_RANDOM_ANGLE:
        return (flags35C & 0x40000000u) != 0
            ? g_AlternateRngView.GetRandomF32Signed() * 3.1415927f
            : g_RngView.GetRandomF32Signed() * 3.1415927f;
    case ANM_VAR_RANDOM:
        return (flags35C & 0x40000000u) != 0
            ? g_AlternateRngView.GetRandomF32()
            : g_RngView.GetRandomF32();
    case ANM_VAR_RANDOM_SIGNED:
        return (flags35C & 0x40000000u) != 0
            ? g_AlternateRngView.GetRandomF32Signed()
            : g_RngView.GetRandomF32Signed();
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
            *output = mode == ANM_INTERPOLATION_ADD ? initial : final;
            return output;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        initial += final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        initial += finalTangent;
        finalTangent += final;
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

static AnmInt3View ScaleAnmInt3(const AnmInt3View &input, float scale)
{
    AnmInt3View output;
    output.x = static_cast<int>(input.x * scale);
    output.y = static_cast<int>(input.y * scale);
    output.z = static_cast<int>(input.z * scale);
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
        AnmInt3View initialPart = ScaleAnmInt3(initial, initialWeight);
        AnmInt3View finalPart = ScaleAnmInt3(final, finalWeight);
        AnmInt3View initialTangentPart =
            ScaleAnmInt3(initialTangent, initialTangentWeight);
        AnmInt3View finalTangentPart =
            ScaleAnmInt3(finalTangent, finalTangentWeight);

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
            *output = mode == ANM_INTERPOLATION_ADD ? initial : final;
            return output;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        initial.x += final.x;
        initial.y += final.y;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        initial.x += finalTangent.x;
        initial.y += finalTangent.y;
        finalTangent.x += final.x;
        finalTangent.y += final.y;
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
AnmVmView *AnmRenderManagerView::FindVm(int id)
{
    AnmVmLayerNodeView *node;

    if (id == 0)
        return NULL;

    node = primaryVmListHead;
    while (node != NULL)
    {
        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
        if (vm->id == id)
            return vm;
        node = node->next;
    }

    node = secondaryVmListHead;
    while (node != NULL)
    {
        AnmVmView *vm = static_cast<AnmVmView *>(node->owner);
        if (vm->id == id)
            return vm;
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
void AnmRenderManagerView::MarkVmForDeletion(int id)
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

AnmFloat3View *AnmRenderManagerView::GetVmPosition(int id)
{
    AnmVmView *vm = FindVm(id);
    return vm == NULL ? NULL : &vm->positionOffset;
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
        node->InsertAfter(primaryVmListHead);
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
        node->InsertAfter(secondaryVmListHead);
    }
    secondaryVmListHead = node;

    AnmVmIdView *incrementReceiver = &nextVmId;
    (*incrementReceiver)++;
    if (nextVmId == AnmVmIdView())
        nextVmId++;
    vm->id = nextVmId.value;
    return nextVmId;
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
    vm->unknown3A0[0] = 0x10;
    vm->unknown3A0[1] = 0x10;
    SetAndExecuteScriptIndex(vm, scriptIndex);
}

AnmVmIdView AnmLoadedView::CreateVmVariant0(
    int scriptIndex, unsigned int renderLayer)
{
    AnmVmView *vm = g_AnmRenderManagerView->AllocateVm();
    vm->renderLayer = renderLayer;
    vm->flags35C |= 0x40000000u;
    InitializeVm(vm, scriptIndex);
    return g_AnmRenderManagerView->AddVmVariant0(vm);
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
    AnmVmView *vm = g_AnmRenderManagerView->FindVm(value);
    if (vm == NULL)
        value = 0;
    return vm;
}

// Target 0x0043EE30 is TH10's complete variable-length ANM instruction
// executor. The adjacent TH095 source supplies control-flow hypotheses; every
// opcode, VM offset, interpolation call and frame-end update below is checked
// against the TH10 v1.00a target.
int __stdcall AnmRenderManagerView::ExecuteScript(AnmVmView *vm)
{
    AnmRawInstructionView *currentInstruction;
    AnmRawInstructionView *fallbackInterrupt;
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
        case ANM_OP_END:
        case ANM_OP_DELETE:
            vm->visible = 0;
        case ANM_OP_STATIC:
            vm->currentInstruction = NULL;
            g_AnmGameSpeed = savedGameSpeed;
            return 1;

        case ANM_OP_SPRITE:
            vm->visible = 1;
            vm->anmFile->SetSprite(vm, GET_INT_VAR(0));
            vm->timeOfLastSpriteSet = vm->scriptTimer.current;
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

        case ANM_OP_I_SET:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1);
            break;
        case ANM_OP_F_SET:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1);
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

        case ANM_OP_POSITION:
            if (!vm->useAlternatePosition)
            {
                vm->position.x = GET_FLOAT_VAR(0);
                vm->position.y = GET_FLOAT_VAR(1);
                vm->position.z = GET_FLOAT_VAR(2);
            }
            else
            {
                vm->alternatePosition.x = GET_FLOAT_VAR(0);
                vm->alternatePosition.y = GET_FLOAT_VAR(1);
                vm->alternatePosition.z = GET_FLOAT_VAR(2);
            }
            break;
        case ANM_OP_ROTATION:
            vm->rotation.x = GET_FLOAT_VAR(0);
            vm->rotation.y = GET_FLOAT_VAR(1);
            vm->rotation.z = GET_FLOAT_VAR(2);
            vm->updateRotation = 1;
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

        case ANM_OP_RENDER_STATE:
            vm->renderStateA = currentInstruction->shortArgs[0];
            vm->renderStateB = currentInstruction->shortArgs[1];
            break;
        case ANM_OP_BLEND_MODE:
            vm->blendMode = currentInstruction->intArgs[0];
            break;
        case ANM_OP_RENDER_MODE:
            vm->renderMode = currentInstruction->intArgs[0];
            if (vm->renderMode == 10)
                vm->InitializePulsingRadialTrail();
            break;
        case ANM_OP_RENDER_LAYER:
            vm->renderLayer = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_STOP_HIDE:
            vm->visible = 0;
            if (vm->pendingInterrupt == 0)
            {
                vm->stopped = 1;
                vm->scriptTimer.Add(-1.0f);
                goto stop;
            }
            goto handleInterrupt;
        case ANM_OP_U_SCROLL:
            vm->uvScrollVelocityX = GET_FLOAT_VAR(0);
            break;
        case ANM_OP_V_SCROLL:
            vm->uvScrollVelocityY = GET_FLOAT_VAR(0);
            break;
        case ANM_OP_VISIBLE:
            vm->visible = currentInstruction->intArgs[0];
            break;
        case ANM_OP_Z_WRITE_DISABLE:
            vm->zWriteDisabled = currentInstruction->intArgs[0];
            break;
        case ANM_OP_FLAG13:
            vm->flag13 = currentInstruction->intArgs[0];
            break;
        case ANM_OP_WAIT:
            vm->scriptTimer.Decrement(GET_INT_VAR(0));
            break;

        case ANM_OP_COLOR2:
            vm->secondaryColor.red =
                static_cast<unsigned char>(GET_INT_VAR(0));
            vm->secondaryColor.green =
                static_cast<unsigned char>(GET_INT_VAR(1));
            vm->secondaryColor.blue =
                static_cast<unsigned char>(GET_INT_VAR(2));
            break;
        case ANM_OP_ALPHA2:
            vm->secondaryColor.alpha =
                static_cast<unsigned char>(GET_INT_VAR(0));
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
        case ANM_OP_USE_SECONDARY_COLOR:
            vm->useSecondaryColor = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_RETURN:
            vm->scriptTimer = vm->interruptReturnTimer;
            vm->currentInstruction = vm->interruptReturnInstruction;
            continue;
        case ANM_OP_FLAG27:
            vm->flag27 = currentInstruction->byteArgs[0];
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
        case ANM_OP_FLAG28:
            vm->flag28 = currentInstruction->byteArgs[0];
            break;
        case ANM_OP_UNIT_SPEED:
            vm->useUnitSpeed = GET_INT_VAR(0);
            break;
        case ANM_OP_ALTERNATE_RNG:
            vm->useAlternateRng = currentInstruction->byteArgs[0];
            break;

        case ANM_OP_CREATE_CHILD_88:
        case ANM_OP_CREATE_CHILD_90:
        case ANM_OP_CREATE_CHILD_91:
        case ANM_OP_CREATE_CHILD_92:
        {
            AnmVmIdView childId;
            int scriptIndex = GET_INT_VAR(0);
            switch (currentInstruction->opcode)
            {
            case ANM_OP_CREATE_CHILD_88:
                childId = vm->anmFile->CreateVmVariant0(
                    scriptIndex, vm->renderLayer);
                break;
            case ANM_OP_CREATE_CHILD_90:
                childId = vm->anmFile->CreateVmVariant2(
                    scriptIndex, vm->renderLayer);
                break;
            case ANM_OP_CREATE_CHILD_91:
                childId = vm->anmFile->CreateVmVariant1(
                    scriptIndex, vm->renderLayer);
                break;
            default:
                childId = vm->anmFile->CreateVmVariant3(
                    scriptIndex, vm->renderLayer);
                break;
            }
            AnmVmView *child = childId.GetVm();
            vm->layerNode.InsertAfter(&child->layerNode);
            child->alternatePosition = vm->position;
            child->positionOffset = vm->positionOffset;
            break;
        }
        case ANM_OP_POINT_TEXTURE_FILTER:
            vm->usePointTextureFilter = currentInstruction->intArgs[0];
            break;

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
        vm->positionInterpolation.Evaluate(&value);
        if (!vm->useAlternatePosition)
            vm->position = value;
        else
            vm->alternatePosition = value;
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
        vm->scaleInterpolation.Evaluate(&value);
        vm->scaleX = value.x;
        vm->scaleY = value.y;
        vm->updateScale = 1;
    }
    if (vm->rotationInterpolation.duration != 0)
    {
        vm->rotationInterpolation.Evaluate(&vm->rotation);
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

    if (vm->positionCallback != NULL)
        vm->positionCallback(vm);

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
// This CC-delimited owner is absent from Ghidra's current function inventory.
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
        g_Direct3DDevice->vtable->SetRenderState(
            g_Direct3DDevice, D3D9_VIEW_RS_TEXTUREFACTOR,
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
// CC-delimited retained owner is absent from Ghidra's current inventory.
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

    xOffset = vm->alternatePosition.x;
    xOffset += vm->positionOffset.x;
    xOffset += vm->position.x;
    yOffset = vm->alternatePosition.y;
    yOffset += vm->positionOffset.y;
    yOffset += vm->position.y;
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

    xOffset = vm->alternatePosition.x;
    xOffset += vm->positionOffset.x;
    xOffset += vm->position.x;
    yOffset = vm->alternatePosition.y;
    yOffset += vm->positionOffset.y;
    yOffset += vm->position.y;
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
    int vertexAlpha;
    AnmUntexturedVertexView *sourceVertex;
    AnmFloat4View *transformedVertex;

    draw.distanceRange =
        g_AnmPhotoBlend.nearDistance - g_AnmPhotoBlend.farDistance;
    draw.color.value = vm->useSecondaryColor
        ? vm->secondaryColor.value : vm->primaryColor.value;

    vertexAlpha = reinterpret_cast<int>(g_AnmQuadVertices) +
        offsetof(AnmRenderVertexView, color) + 3;
    sourceVertex = untexturedVertices;
    transformedVertex = transformedVertices;
    do
    {
        D3DXVec4Transform(
            transformedVertex,
            reinterpret_cast<const AnmFloat4View *>(sourceVertex),
            &cachedWorldMatrix);
        cameraDelta.x = transformedVertex->x -
            g_AnmBackgroundCameraPosition.x;
        cameraDelta.y = transformedVertex->y -
            g_AnmBackgroundCameraPosition.y;
        cameraDelta.z = transformedVertex->z -
            g_AnmBackgroundCameraPosition.z;
        distance = AnmFloat3Length(cameraDelta);

        if (g_AnmPhotoBlend.nearDistance < distance)
        {
            distance =
                (g_AnmPhotoBlend.nearDistance - distance) /
                draw.distanceRange;
            if (distance >= 1.0f)
            {
                *reinterpret_cast<unsigned int *>(vertexAlpha - 3) =
                    g_AnmPhotoBlend.farColor.value;
                *reinterpret_cast<unsigned char *>(vertexAlpha) =
                    draw.color.alpha;
            }
            else
            {
                draw.colorComponent = draw.color.blue;
                reinterpret_cast<unsigned char *>(vertexAlpha)[-3] =
                        draw.color.blue - static_cast<unsigned char>(
                            (static_cast<float>(draw.colorComponent) -
                             g_AnmPhotoBlend.blue) * distance);
                draw.colorComponent = draw.color.green;
                reinterpret_cast<unsigned char *>(vertexAlpha)[-2] =
                        draw.color.green - static_cast<unsigned char>(
                            (static_cast<float>(draw.colorComponent) -
                             g_AnmPhotoBlend.green) * distance);
                draw.colorComponent = draw.color.red;
                reinterpret_cast<unsigned char *>(vertexAlpha)[-1] =
                        draw.color.red - static_cast<unsigned char>(
                            (static_cast<float>(draw.colorComponent) -
                             g_AnmPhotoBlend.red) * distance);
                *reinterpret_cast<unsigned char *>(vertexAlpha) =
                    draw.color.alpha;
            }
        }
        else
        {
            *reinterpret_cast<unsigned int *>(vertexAlpha - 3) =
                draw.color.value;
        }
        vertexAlpha += sizeof(AnmRenderVertexView);
        ++sourceVertex;
        ++transformedVertex;
    } while (vertexAlpha <
        reinterpret_cast<int>(g_AnmQuadVertices) +
            sizeof(g_AnmQuadVertices) +
            offsetof(AnmRenderVertexView, color) + 3);

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
    void *texture;

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

    texture = vm->loadedSprite->texture;
    if (currentTexture != texture)
    {
        currentTexture = texture;
        g_Direct3DDevice->SetTexture(0, texture);
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
        g_Direct3DDevice->SetTexture(0, currentTexture);
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
        g_Direct3DDevice->SetTexture(0, currentTexture);
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
    if (!vm->visible)
        return -1;
    if (!vm->drawEnabled)
        return -1;
    if (vm->primaryColor.alpha == 0)
        return -1;

    switch (vm->renderMode)
    {
    case 0:
        return DrawNoRotation(vm);
    case 1:
        return Draw2D(vm);
    case 2:
        return DrawNoRotationNoRound(vm);
    case 3:
        return Draw2DRotatedOrAxisAligned(vm);
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
    int wrapIndex;
    AnmRenderVertexView *vertex;
    PulsingRadialTrailDataView *data;
    float angleStep;
    float angle;

    data = static_cast<PulsingRadialTrailDataView *>(vm->generatedVertices);
    angleStep = 0.2026834041f;
    angle = -3.1415927f;
    vertex = data->vertices;

    *reinterpret_cast<AnmFloat3View *>(&vertex->x) =
        vm->position + vm->positionOffset;
    vertex->uv.x += data->uvVelocity.x;
    if (vertex->uv.x < 0.0f)
    {
        for (wrapIndex = 0; wrapIndex < 33; ++wrapIndex)
            data->vertices[wrapIndex].uv.x += 1.0f;
    }
    vertex->uv.y += data->uvVelocity.x;
    if (vertex->uv.y < 0.0f)
    {
        for (wrapIndex = 0; wrapIndex < 33; ++wrapIndex)
            data->vertices[wrapIndex].uv.y += 1.0f;
    }
    vertex->diffuse.value = vm->primaryColor.value;
    ++vertex;

    for (i = 1; i < 32; ++i)
    {
        vertex->uv.x += data->uvVelocity.x;
        if (vertex->uv.x < 0.0f)
        {
            for (wrapIndex = 0; wrapIndex < 33; ++wrapIndex)
                data->vertices[wrapIndex].uv.x += 1.0f;
        }
        vertex->uv.y += data->uvVelocity.x;
        if (vertex->uv.y < 0.0f)
        {
            for (wrapIndex = 0; wrapIndex < 33; ++wrapIndex)
                data->vertices[wrapIndex].uv.y += 1.0f;
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
