#pragma once

#include <string>
#include <vector>

#include <raylib.h>

#include "enchant.h"

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

    int damage;

    std::vector<RecipeItem> recipe;
    std::vector<Enchant> enchants;

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
