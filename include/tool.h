#pragma once

#include <string>

#include <raylib.h>

#include "enchant.h"
#include "item.h"

class Tool{
    private:
        std::string name;
        std::string type;
        int id;

        std::vector<Enchant> enchants;
        int damage;

        Texture2D texture;
    public:
        InventoryItem asItem(int count);

        Tool();
        Tool(int id);
};

enum ToolID{
    Hoe_Tool,
    Sword_Tool
};
