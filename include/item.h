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

    std::string filename{};

    int item_invslot{};
    int item_count{};

    std::vector<RecipeItem> recipe;

    Texture2D iconTexture{};
};

template<typename T>
InventoryItem newItem(int tileID){
    return T(tileID).asItem(1);
}

template<typename T>
InventoryItem newItem(int tileID, int count){
    return T(tileID).asItem(count);
}

void assignInvSlot(InventoryItem& item, int slot);
