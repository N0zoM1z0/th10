#include "PopupNumber.hpp"
#include "Player.hpp"
extern unsigned char g_PopupRenderFlags; // 0x491D78, alias storage unresolved
extern int g_StageFogEnabled; // 0x492378, full DWORD
int PopupNumberView::Draw()
{
    PopupNumberRecordView *record = records;
    if (!(g_PopupRenderFlags & 4) && g_StageFogEnabled != 0) {
        g_AnmRenderManagerView->FlushVertexBuffer();
        g_StageFogEnabled = 0;
        g_Direct3DDevice->vtable->SetRenderState(g_Direct3DDevice,28,0);
    }
    Player *player = g_Player;
    for (int i=0; i<723; ++i, ++record) {
        if (!record->active) continue;
        float spacing = record->age < 8 ? 8.0f / record->ageFloat : 8.0f;
        vm.positionOffset.x = record->position.x - (int)record->digitCount * spacing * 0.5f + 224.0f;
        vm.positionOffset.y = record->position.y + 16.0f;
        vm.primaryColor.value = record->color;
        const float dx=player->drawPosition.x-record->position.x;
        const float dy=player->drawPosition.y-record->position.y;
        int distance = (int)(dy*dy+dx*dx);
        int alpha;
        if (distance > 4096) alpha=208;
        else if (distance > 1024) alpha=80+((distance-1024)<<7)/3072;
        else alpha=80;
        int count=record->digitCount;
        unsigned char *digit=record->digits+count-1;
        if (count>0) {
            do {
                if (record->age>=52 && *digit!=10) {
                    if (record->age<56)
                        vm.loadedSprite=&loaded->sprites[*digit+207];
                    else
                        vm.loadedSprite=&loaded->sprites[*digit+217];
                } else
                    vm.loadedSprite=&loaded->sprites[*digit+196];
                vm.primaryColor.alpha=(unsigned char)alpha;
                vm.spriteWidth=vm.loadedSprite->width;
                vm.flags35C |= 8;
                g_AnmRenderManagerView->DrawNoRotation(&vm);
                vm.positionOffset.x += spacing;
                --digit;
            } while (--count);
            player=g_Player;
        }
    }
    return 1;
}
int __fastcall PopupNumberDrawCallback(void *context) { return static_cast<PopupNumberView *>(context)->Draw(); }
