#pragma once

#include<string>

#include<raylib.h>

struct InventoryItem{
    int tileID{};
    std::string item_type{};
    std::string item_name{};

    int item_invslot{};
    int item_count{};

    Texture2D iconTexture{};
};

void assignInvSlot(InventoryItem& item, int slot);
