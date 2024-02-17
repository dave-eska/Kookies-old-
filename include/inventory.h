#pragma once

#include<vector>

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

        int getCurrentSlot(){return current_slot;}

        void printItems();

        //Setters;
        void setPos(Vector2 newPos){pos=newPos;}
        void setItems(std::vector<InventoryItem> newItems){items=newItems;}
        void setItemInSlot(int slot, InventoryItem item){items[slot]=item;}
        void deleteItem(int slot);

        //Function;
        void addItem(InventoryItem item);
        void decreaseItemCount(int slot);
        void toggleDrawUI();

        void changeCurrentSlot();

        void Draw(Camera2D& camera);
        void DrawItemName();

        //Constructors
        Inventory();

        Inventory(Vector2 pos, int slots, Texture2D Outline_texture,
                Texture2D SelectOutline_texture, Texture2D Extra_Inv_Texture);
};
