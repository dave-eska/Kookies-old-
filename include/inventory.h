#pragma once

#include<vector>
#include<array>

#include<raylib.h>

#include"item.h"
#include"timer.h"

class Inventory{
    private:
        std::vector<InventoryItem> items;

        Texture2D Outline_texture;
        Texture2D Extra_Inv_Texture;
        Texture2D SelectOutline_texture;

        Vector2 pos;
        int current_slot;

        Timer drawingNameTimer;

        unsigned char outline_transparancy;

        bool isDrawingUI;

    public:
        //Getters;
        InventoryItem getItemIdFromSlot(int slot);
        InventoryItem getItemFromCurrentSot();

        int getSlotWithItem(int id);

        int getCurrentSlot(){return current_slot;}

        int getFirstEmptySlot(); //Returns slot

        void printItems();

        bool has(int id);
        bool has(RecipeItem item_data);

        //Setters;
        void setPos(Vector2 newPos){pos=newPos;}
        void setItems(std::vector<InventoryItem> newItems){items=newItems;}
        void setItemInSlot(int slot, InventoryItem item){items[slot]=item;}
        void deleteItem(int slot);

        //Function;
        void addItem(InventoryItem item);
        void decreaseItemCount(int slot, int count);
        void decreaseItemCount(int slot);
        void toggleDrawUI();

        void changeCurrentSlot();

        void craft(InventoryItem item);

        void Draw(Camera2D& camera);
        void DrawItemName();

        //Constructors
        Inventory();

        Inventory(Vector2 pos, int slots, Texture2D Outline_texture,
                Texture2D SelectOutline_texture, Texture2D Extra_Inv_Texture);
};
