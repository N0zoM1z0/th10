#include "AnmManager.hpp"

// The native 0x00474430 table is indexed at 16 bytes per record. Only the
// consumed fields and the first record's address are target-established;
// original table count and physical source owner are not asserted.
typedef void (__fastcall *AnmSpecialSetupCallback)(AnmVmView *vm, int argument);
struct AnmSpecialVmDescriptorView
{
    short scriptIndex;
    unsigned short reserved02;
    AnmSpecialSetupCallback setupCallback;
    AnmVmCallback positionCallback;
    AnmVmCallback drawCallback;
};
typedef char AnmSpecialVmDescriptorSizeIs10[
    sizeof(AnmSpecialVmDescriptorView) == 0x10 ? 1 : -1];
typedef char AnmSpecialVmDescriptorCallAt04[
    offsetof(AnmSpecialVmDescriptorView, setupCallback) == 0x04 &&
    offsetof(AnmSpecialVmDescriptorView, positionCallback) == 0x08 &&
    offsetof(AnmSpecialVmDescriptorView, drawCallback) == 0x0C ? 1 : -1];

extern const AnmSpecialVmDescriptorView g_AnmSpecialVmDescriptors[];

// Target 0x0040B110 is a complete 148-byte specialized ANM VM factory.
// Original private ABI: descriptor index arrives in EAX; three stack words
// carry result storage, source ANM and setup argument, followed by RET 12.
// This descriptive C++ interface is only a compiler-testable semantic model:
// natural external-source ABI spelling and the real callers remain unknown.
AnmVmIdView __stdcall CreateConfiguredAnmVm(
    int descriptorIndex, AnmLoadedView *loaded, int setupArgument)
{
    const AnmSpecialVmDescriptorView *const entry =
        &g_AnmSpecialVmDescriptors[descriptorIndex];
    const int scriptIndex = entry->scriptIndex;
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm = manager->AllocateVm();
    vm->renderLayer = 15;
    vm->flags35C |= 0x40000000u;
    loaded->InitializeVm(vm, scriptIndex);

    AnmVmIdView id;
    id = manager->AddVmVariant0(vm);
    AnmVmView *created = manager->FindVm(id);
    if (created == NULL)
        id.value = 0;
    if (entry->setupCallback != NULL)
        entry->setupCallback(created, setupArgument);
    created->positionCallback = entry->positionCallback;
    created->drawCallback = entry->drawCallback;
    return id;
}

// Target 0x0040B070 takes a descriptor index in EAX, then an owner, result
// storage and callback argument on the stack. The owner's loaded ANM is at
// +0x10; its exact enclosing class is still unidentified.
struct AnmSpecialVmOwnerView
{
    unsigned char unknown000[0x10];
    AnmLoadedView *loaded;
};
typedef char AnmSpecialVmOwnerLoadedAt10[
    offsetof(AnmSpecialVmOwnerView, loaded) == 0x10 ? 1 : -1];

void __stdcall CreateConfiguredAnmVmFromOwner(
    int descriptorIndex, AnmSpecialVmOwnerView *owner,
    AnmVmIdView *result, int setupArgument)
{
    const AnmSpecialVmDescriptorView *const entry =
        &g_AnmSpecialVmDescriptors[descriptorIndex];
    AnmLoadedView *loaded = owner->loaded;
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm = manager->AllocateVm();
    vm->renderLayer = 15;
    vm->flags35C |= 0x40000000u;
    loaded->InitializeVm(vm, entry->scriptIndex);

    manager = g_AnmRenderManagerView;
    AnmVmIdView id = manager->AddVmVariant0(vm);
    AnmVmView *created = manager->FindVm(id);
    if (created == NULL)
        id.value = 0;
    if (entry->setupCallback != NULL)
        entry->setupCallback(created, setupArgument);
    created->positionCallback = entry->positionCallback;
    created->drawCallback = entry->drawCallback;
    *result = id;
}

// Target 0x0040B1B0 accepts the script and three callback slots explicitly.
// Its original owner/class name is not established; the first stack operand
// is result storage, followed by the loaded resource, script and callback data.
void __stdcall CreateConfiguredAnmVmWithCallbacks(
    AnmVmIdView *result, AnmLoadedView *loaded, int scriptIndex,
    int setupArgument, AnmSpecialSetupCallback setupCallback,
    AnmVmCallback positionCallback, AnmVmCallback drawCallback)
{
    AnmRenderManagerView *manager = g_AnmRenderManagerView;
    AnmVmView *vm = manager->AllocateVm();
    vm->renderLayer = 15;
    vm->flags35C |= 0x40000000u;
    loaded->InitializeVm(vm, scriptIndex);

    manager = g_AnmRenderManagerView;
    AnmVmIdView id = manager->AddVmVariant0(vm);
    AnmVmView *created = manager->FindVm(id);
    if (created == NULL)
        id.value = 0;
    if (setupCallback != NULL)
        setupCallback(created, setupArgument);
    created->positionCallback = positionCallback;
    created->drawCallback = drawCallback;
    *result = id;
}
