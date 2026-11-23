#ifndef TH10_ITEM_RESOURCES_HPP
#define TH10_ITEM_RESOURCES_HPP

struct AnmLoadedView;

struct ItemPrimaryResourceOwnerView
{
    unsigned char unknown000000[0x3e0b50];
    AnmLoadedView *primaryEnemyResource;
};

extern ItemPrimaryResourceOwnerView *g_EnemyPrimaryResourceOwner;

#endif
