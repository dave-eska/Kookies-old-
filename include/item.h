#pragma once

#include<string>

#include<vector>

#include<raylib.h>

struct RecipeItem{
    int id;
    int count;
};

struct InventoryItem{
    int tileID{};
    std::string item_type{};
    std::string item_name{};

    int item_invslot{};
    int item_count{};

    std::vector<RecipeItem> recipe;

    Texture2D iconTexture{};
};

InventoryItem newItem(int tileID);

void assignInvSlot(InventoryItem& item, int slot);
