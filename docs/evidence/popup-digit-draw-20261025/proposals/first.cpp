#include "PopupNumber.hpp"
#include "Player.hpp"
extern unsigned char g_PopupRenderFlags; // 0x491D78, alias storage unresolved
extern int g_StageFogEnabled; // 0x492378, full DWORD
int PopupNumberView::Draw()
{
    if (!(g_PopupRenderFlags & 4) && g_StageFogEnabled != 0) {
        g_AnmRenderManagerView->FlushVertexBuffer();
        g_StageFogEnabled = 0;
        g_Direct3DDevice->vtable->SetRenderState(g_Direct3DDevice,28,0);
    }
    Player *player = g_Player;
    for (int i=0; i<723; ++i) {
        PopupNumberRecordView *record = &records[i];
        if (!record->active) continue;
        float spacing = record->age < 8 ? 8.0f / record->ageFloat : 8.0f;
        vm.positionOffset.x = record->position.x - (int)record->digitCount * spacing * 0.5f + 224.0f;
        vm.positionOffset.y = record->position.y + 16.0f;
        vm.primaryColor.value = record->color;
        int distance = (int)((player->drawPosition.y-record->position.y)*(player->drawPosition.y-record->position.y)
                            +(player->drawPosition.x-record->position.x)*(player->drawPosition.x-record->position.x));
        int alpha;
        if (distance > 4096) alpha=208;
        else if (distance > 1024) alpha=80+((distance-1024)<<7)/3072;
        else alpha=80;
        int count=record->digitCount;
        unsigned char *digit=record->digits+count-1;
        if (count>0) {
            do {
                int bank;
                if (record->age>=52 && *digit!=10)
                    bank=record->age<56 ? 207 : 217;
                else bank=196;
                vm.loadedSprite=&loaded->sprites[*digit+bank];
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
