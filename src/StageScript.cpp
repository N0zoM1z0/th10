#include "StageScript.hpp"
#include <math.h>

extern "C" AnmFloat3View *__stdcall D3DXVec3Normalize(
    AnmFloat3View *output, const AnmFloat3View *input);
// Same target address as several smaller ANM camera views; this declaration
// does not supply storage or prove alias/runtime closure across those views.
extern StageCameraStateView g_StagePublishedCamera; // 0x00491D7C
extern int g_StageScriptGlobal4923A8;

// Target 0x00405040. Packed bytes use truncating integer conversion, not
// saturating color conversion. Input-domain safety is not established.
StageFogView::StageFogView(float start, float end, float blue, float green,
    float red, float alpha)
{
    this->start = start;
    this->end = end;
    this->blue = blue;
    this->green = green;
    this->red = red;
    this->alpha = alpha;
    unsigned char *bytes = reinterpret_cast<unsigned char *>(&color);
    bytes[0] = static_cast<unsigned char>(static_cast<int>(blue));
    bytes[1] = static_cast<unsigned char>(static_cast<int>(green));
    bytes[2] = static_cast<unsigned char>(static_cast<int>(red));
    bytes[3] = static_cast<unsigned char>(static_cast<int>(alpha));
}

// Target 0x00403990. Object/primitive counts, signed sizes and VM indices
// remain unchecked, as in the target; records terminate at a negative type.
int StageScriptView::UpdateObjectVms()
{
    for (int index = 0; index < data->objectCount; ++index)
    {
        StageObjectView *object = objects[index];
        if ((object->flags & 1) != 0)
        {
            int active = 0;
            StageObjectPrimitiveView *primitive = &object->firstPrimitive;
            while (primitive->type >= 0)
            {
                AnmVmView *vm = &objectVms[primitive->vmIndex];
                AnmRenderManagerView::ExecuteScript(vm);
                if (vm->currentInstruction != NULL)
                    ++active;
                primitive = reinterpret_cast<StageObjectPrimitiveView *>(
                    reinterpret_cast<unsigned char *>(primitive) + primitive->byteSize);
            }
            if (active == 0)
                object->flags &= ~1;
        }
    }
    return 0;
}

// Target 0x00403C80: code1806 bytes, with a separately audited15-slot table.
// Opcode0 yields without consuming the record or ticking the script timer;
// interpolation/rocking still advance. Signed record sizes are not repaired.
int __stdcall StageExecuteScript(StageScriptView *stage)
{
    while (stage->instruction->time <= stage->scriptTimer.current)
    {
        StageInstructionView *instruction = stage->instruction;
        int *integers = instruction->args.integers;
        float *values = instruction->args.reals;
        switch (instruction->opcode)
        {
        case 0:
            goto updateInterpolations;
        case 1:
            stage->scriptTimer.SetCurrent(integers[1]);
            stage->instruction = reinterpret_cast<StageInstructionView *>(
                stage->scriptBase + stage->instruction->args.integers[0]);
            continue;
        case 2:
            stage->camera.positionDelta = stage->camera.position;
            stage->camera.position.x = values[0];
            stage->camera.position.y = values[1];
            stage->camera.position.z = values[2];
            stage->camera.positionDelta = stage->camera.position - stage->camera.positionDelta;
            break;
        case 3:
        {
            AnmFloat3View final(values[2], values[3], values[4]);
            stage->positionInterpolation.duration = integers[0];
            stage->positionInterpolation.mode = integers[1];
            stage->positionInterpolation.initial = stage->camera.position;
            stage->positionInterpolation.final = final;
            stage->positionInterpolation.timer.SetCurrent(0);
            break;
        }
        case 4:
            stage->camera.direction.x = values[0];
            stage->camera.direction.y = values[1];
            stage->camera.direction.z = values[2];
            break;
        case 5:
        {
            AnmFloat3View final(values[2], values[3], values[4]);
            stage->directionInterpolation.duration = integers[0];
            stage->directionInterpolation.mode = integers[1];
            stage->directionInterpolation.initial = stage->camera.direction;
            stage->directionInterpolation.final = final;
            stage->directionInterpolation.timer.SetCurrent(0);
            break;
        }
        case 6:
            stage->camera.up.x = values[0];
            stage->camera.up.y = values[1];
            stage->camera.up.z = values[2];
            break;
        case 7:
            stage->camera.value048 = values[0];
            break;
        case 8:
        {
            stage->camera.fog.color = integers[0];
            const unsigned char *bytes = reinterpret_cast<const unsigned char *>(&stage->camera.fog.color);
            stage->camera.fog.blue = static_cast<float>(bytes[0]);
            stage->camera.fog.green = static_cast<float>(bytes[1]);
            stage->camera.fog.red = static_cast<float>(bytes[2]);
            stage->camera.fog.alpha = static_cast<float>(bytes[3]);
            stage->camera.fog.start = values[1];
            stage->camera.fog.end = values[2];
            break;
        }
        case 9:
        {
            const unsigned char *bytes = reinterpret_cast<const unsigned char *>(&integers[2]);
            StageFogView final(values[3], values[4],
                static_cast<float>(bytes[0]), static_cast<float>(bytes[1]),
                static_cast<float>(bytes[2]), static_cast<float>(bytes[3]));
            stage->fogInterpolation.duration = integers[0];
            int mode = integers[1];
            stage->fogInterpolation.initial = stage->camera.fog;
            stage->fogInterpolation.final = final;
            stage->fogInterpolation.mode = mode;
            stage->fogInterpolation.timer.SetCurrent(0);
            break;
        }
        case 10:
        {
            AnmFloat3View initialTangent(values[2], values[3], values[4]);
            AnmFloat3View final(values[5], values[6], values[7]);
            AnmFloat3View finalTangent(values[8], values[9], values[10]);
            stage->positionInterpolation.duration = integers[0];
            stage->positionInterpolation.initial = stage->camera.position;
            stage->positionInterpolation.initialTangent = initialTangent;
            stage->positionInterpolation.final = final;
            stage->positionInterpolation.mode = 8;
            stage->positionInterpolation.finalTangent = finalTangent;
            stage->positionInterpolation.timer.SetCurrent(0);
            break;
        }
        case 11:
        {
            AnmFloat3View initialTangent(values[2], values[3], values[4]);
            AnmFloat3View final(values[5], values[6], values[7]);
            AnmFloat3View finalTangent(values[8], values[9], values[10]);
            stage->directionInterpolation.duration = integers[0];
            stage->directionInterpolation.initial = stage->camera.direction;
            stage->directionInterpolation.initialTangent = initialTangent;
            stage->directionInterpolation.final = final;
            stage->directionInterpolation.mode = 8;
            stage->directionInterpolation.finalTangent = finalTangent;
            stage->directionInterpolation.timer.SetCurrent(0);
            break;
        }
        case 12:
            stage->rockingMode = static_cast<unsigned char>(integers[0]);
            stage->rockingTimer.SetCurrent(0);
            break;
        case 13:
            g_StageScriptGlobal4923A8 = integers[0];
            break;
        case 14:
            if (integers[1] >= 0)
                stage->scriptVms[integers[0]].InitializeForLoadedScript(stage->anm, integers[1]);
            else
            {
                AnmVmView *vm = &stage->scriptVms[integers[0]];
                vm->flags35C &= ~1u;
            }
            break;
        }
        stage->instruction = reinterpret_cast<StageInstructionView *>(
            reinterpret_cast<unsigned char *>(stage->instruction) + stage->instruction->byteSize);
    }
    stage->scriptTimer.Tick();
updateInterpolations:
    if (stage->directionInterpolation.duration != 0)
    {
        AnmFloat3View value;
        stage->camera.direction = *stage->directionInterpolation.Evaluate(&value);
    }
    if (stage->positionInterpolation.duration != 0)
    {
        AnmFloat3View value;
        stage->camera.position = *stage->positionInterpolation.Evaluate(&value);
    }
    if (stage->fogInterpolation.duration != 0)
    {
        StageFogView value;
        stage->camera.fog = *stage->fogInterpolation.Evaluate(&value);
    }
    if (stage->rockingMode != 0)
    {
        switch (stage->rockingMode)
        {
        case 1:
            stage->camera.rockingOffset = static_cast<float>(
                sin(stage->rockingTimer.subframe * 0.012271846644580364f - 3.1415927410125732f) * -100.0f);
            stage->rockingTimer.Tick();
            if (stage->rockingTimer.current >= 512)
                stage->rockingTimer.SetCurrent(0);
            break;
        }
    }
    return 0;
}

// Real callback0x00402720 supplies the dispatcher receiver and ANM visibility.
// The callback's registration/startup graph and global camera storage are open.
int StageScriptView::Update()
{
    if ((flags & 8) != 0 || ((flags & 4) != 0 && transitionTimer.current >= 60))
        return 1;
    camera.positionDelta = AnmFloat3View(0.0f, 0.0f, 0.0f);
    camera.value0E8 = 0;
    camera.value0EC = 0;
    D3DXVec3Normalize(&camera.normalizedDirection, &camera.direction);
    tint = 0x808080;
    UpdateObjectVms();
    StageExecuteScript(this);
    for (int i = 0; i < 8; ++i)
        AnmRenderManagerView::ExecuteScript(&scriptVms[i]);
    if (auxiliaryActive != 0)
    {
        float savedGameSpeed = g_AnmGameSpeed;
        g_AnmGameSpeed = 1.0f;
        AnmRenderManagerView::ExecuteScript(&auxiliaryVms[0]);
        AnmRenderManagerView::ExecuteScript(&auxiliaryVms[1]);
        AnmRenderManagerView::ExecuteScript(&auxiliaryVms[2]);
        g_AnmGameSpeed = savedGameSpeed;
    }
    auxiliaryActive = 0;
    g_StagePublishedCamera = camera;
    ++frame;
    return 1;
}

// Six float channels plus a derived packed-color dword, not seven independent
// floats. Every arithmetic result repacks truncating low bytes with no clamp.
StageFogView StageFogView::operator-(const StageFogView &other) const
{
    return StageFogView(start - other.start, end - other.end,
        blue - other.blue, green - other.green, red - other.red, alpha - other.alpha);
}
#if defined(_MSC_VER)
__declspec(noinline)
#endif
StageFogView StageFogView::operator+(const StageFogView &other) const
{
    return StageFogView(start + other.start, end + other.end,
        blue + other.blue, green + other.green, red + other.red, alpha + other.alpha);
}
StageFogView StageFogView::operator*(float scale) const
{
    return StageFogView(start * scale, end * scale,
        blue * scale, green * scale, red * scale, alpha * scale);
}

// Target0x004049A0. Positive durations tick before completion; additive mode7
// returns its accumulated initial value on completion, other modes return final.
// Zero/negative-duration arithmetic retains the original unchecked semantics.
StageFogView *StageFogInterpolationView::Evaluate(
    StageFogView *output)
{
    if (duration > 0)
    {
        timer.Tick();
        if (timer.current >= duration)
        {
            timer.SetCurrent(duration);
            duration = 0;
            if (mode != ANM_INTERPOLATION_ADD)
            {
                *output = final;
                return output;
            }
            *output = initial;
            return output;
        }
    }

    if (mode == ANM_INTERPOLATION_ADD)
    {
        const StageFogView initialValue = initial;
        initial = initialValue + final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_ACCELERATE)
    {
        const StageFogView initialValue = initial;
        initial = initialValue + finalTangent;
        finalTangent = finalTangent + final;
        *output = initial;
        return output;
    }

    if (mode == ANM_INTERPOLATION_HERMITE)
    {
        float value = timer.subframe / static_cast<float>(duration);
        float twice = value + value;
        float minusOne = value - 1.0f;
        float initialWeight = (1.0f + twice) * minusOne * minusOne;
        float finalWeight = (3.0f - twice) * value * value;
        float initialTangentWeight = (1.0f - value) * (1.0f - value) * value;
        float finalTangentWeight = minusOne * value * value;

        StageFogView result;
        result = initial * initialWeight + final * finalWeight +
            initialTangent * initialTangentWeight +
            finalTangent * finalTangentWeight;
        *output = result;
        return output;
    }

    float value = CalculateAnmInterpolation(
        mode, timer.subframe, static_cast<float>(duration));
    StageFogView result;
    result = (final - initial) * value + initial;
    *output = result;
    return output;
}
