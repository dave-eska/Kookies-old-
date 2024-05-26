#pragma once

#include "item.h"
#include <raylib.h>
#include <string>

class Tool{
    private:
        std::string name;
        int id;

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
