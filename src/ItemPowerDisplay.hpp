#pragma once

#include "AnmManager.hpp"

#include <stddef.h>

struct ItemPowerDisplayOwnerView
{
    unsigned char unknown0000[0x6a8c];
    AnmVmView powerVms[4];
    unsigned char unknown793C[0x9e18 - 0x793c];
    AnmVmIdView powerNoticeVmId;
    unsigned char unknown9E1C[0x9ec8 - 0x9e1c];
    AnmLoadedView *frontAnm;
    void UpdatePowerDisplay(int value, int percent);
};
typedef char ItemPowerVmsAt6A8C[
    (offsetof(ItemPowerDisplayOwnerView, powerVms) == 0x6a8c) ? 1 : -1];
typedef char ItemPowerFrontAnmAt9EC8[
    (offsetof(ItemPowerDisplayOwnerView, frontAnm) == 0x9ec8) ? 1 : -1];
typedef char ItemPowerNoticeVmAt9E18[
    (offsetof(ItemPowerDisplayOwnerView, powerNoticeVmId) == 0x9e18) ? 1 : -1];
extern ItemPowerDisplayOwnerView *g_ItemPowerDisplayOwner;
