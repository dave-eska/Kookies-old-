#include"inventory.h"

#include<iostream>
#include<algorithm>
#include<string>

#include<raylib.h>

#include"global_func.h"
#include"item.h"
#include"timer.h"
#include"tile.h"

#define OUTLINE_SIZE 32*2

//Getters;
InventoryItem Inventory::getItemIdFromSlot(int slot){
    return items[slot];
}

InventoryItem Inventory::getItemFromCurrentSot(){
    return items[current_slot];
}

void Inventory::printItems(){
    std::cout<<"Slot[id:total]"<<std::endl;
    for(const auto& item:items)
        std::cout<<item.item_invslot<<"["<<item.tileID<<":"<<item.item_count<<"]"<<std::endl;
}

void Inventory::DrawItemName(){
    if(GetMouseWheelMove() != 0){
        startTimer(&drawingNameTimer, 3.0f);
    }
}

int Inventory::getFirstEmptySlot(){
    int targetId = 0;
    auto it = std::find_if(items.begin(), items.end(),
            [targetId](const InventoryItem& item) {
            return item.tileID == targetId;
            });
    return it->item_invslot;
}

bool Inventory::has(int id){
    auto it = std::find_if(items.begin(), items.end(),
            [id](const InventoryItem& item) {
            return item.tileID == id;
            });
    return it != items.end();
}

bool Inventory::has(RecipeItem item_data){
    int id = item_data.id;
    int count = item_data.count;
    auto it = std::ranges::find_if(items,
            [id](const InventoryItem& item) {
            return item.tileID == id;
            });

    return it->item_count >= count;
}

int Inventory::getSlotWithItem(int id){
    auto it = std::find_if(items.begin(), items.end(),
            [id](const InventoryItem& item) {
            return item.tileID == id;
            });
    return it->item_invslot;
}

void Inventory::changeCurrentSlot(){
    current_slot -= (GetMouseWheelMove() * 1);
    clamp(current_slot, 0, items.size()-1);
    DrawItemName();
    updateTimer(&drawingNameTimer);
}

void Inventory::craft(InventoryItem item){
    bool has_all_ingredients = false;
    if(has(item.recipe[0]) && has(item.recipe[1])){
        has_all_ingredients = true;

        addItem(item);
        decreaseItemCount(getSlotWithItem(item.recipe[0].id), item.recipe[0].count);
        decreaseItemCount(getSlotWithItem(item.recipe[1].id), item.recipe[0].count);
    }
}

void Inventory::deleteItem(int slot){
    items[slot] = {
        .tileID=0,
        .item_type="Block",
        .item_name="air",

        .item_invslot=slot,
        .item_count=0,
    };
}

//Function;
void Inventory::addItem(InventoryItem item){
    for(auto& inv_item:items){
        if(inv_item.tileID==0){
            assignInvSlot(item, inv_item.item_invslot);
            inv_item=item;
            break;
        }else if(inv_item.tileID!=0 && inv_item.tileID==item.tileID){
            inv_item.item_count++;
            break;
        }
    }
}

void Inventory::decreaseItemCount(int slot){
    if(items[slot].item_count > 1)
        items[slot].item_count--;
    else
        deleteItem(slot);
}

void Inventory::decreaseItemCount(int slot, int count){
    if(items[slot].item_invslot > count){
        items[slot].item_count -= count;
    }else{
        deleteItem(slot);
    }
}

void Inventory::toggleDrawUI(){
    if(IsKeyPressed(KEY_E))
        isDrawingUI = !isDrawingUI;
}

void Inventory::Draw(Camera2D& camera){
    for(int i=0;i<items.size();i++){
        DrawTextureEx(Outline_texture, {(float)i*OUTLINE_SIZE+pos.x, pos.y}, 0, 2, WHITE);
        DrawTextureEx(items[i].iconTexture, {(float)i*OUTLINE_SIZE+pos.x+3, pos.y+2}, 0, 1.8, WHITE);

        DrawText(std::to_string(items[i].item_count).c_str(), (float)i*OUTLINE_SIZE+pos.x+20, pos.y+45, 20, LIGHTGRAY);
        if(CheckCollisionPointRec(GetMousePosition(), {(float)i*OUTLINE_SIZE+pos.x, pos.y,
                    OUTLINE_SIZE, OUTLINE_SIZE}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            outline_transparancy = 255;
            current_slot = i;
        }
    }

    DrawTextureEx(SelectOutline_texture, {(float)current_slot*OUTLINE_SIZE+pos.x, pos.y}, 0, 2, {255,255,255,outline_transparancy});
    if(!timerDone(&drawingNameTimer) && getItemFromCurrentSot().item_name!="air")
        DrawText(getItemFromCurrentSot().item_name.c_str(), 20, GetScreenHeight()-60, 40, BLACK);

    if(isDrawingUI){
        for(int i=0;i<5;i++){
            for(int j=0;j<items.size()-1;j++){
                float x = j*OUTLINE_SIZE+pos.x;
                float y = i*OUTLINE_SIZE+pos.y+(OUTLINE_SIZE+pos.x);
                DrawTextureEx(Extra_Inv_Texture, {x, y}, 0, 2, WHITE);

                if(CheckCollisionPointRec(GetMousePosition(), {x,y,OUTLINE_SIZE,OUTLINE_SIZE}) &&
                        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    outline_transparancy = 150;
                }
            }
        }
    }else{
        outline_transparancy = 255;
    }
}

//Constructors
Inventory::Inventory(){
}

Inventory::Inventory(Vector2 pos, int slots, Texture2D Outline_texture,
        Texture2D SelectOutline_texture, Texture2D Extra_Inv_Texture)
    :pos{pos},Outline_texture{Outline_texture},SelectOutline_texture{SelectOutline_texture},Extra_Inv_Texture{Extra_Inv_Texture}
{
    current_slot=0;
    isDrawingUI=false;
    outline_transparancy=255;

    for(int i=0;i<slots;i++){
        items.push_back({
                .tileID=0,
                .item_type="Block",
                .item_name="air",

                .item_invslot=i,
                .item_count=0,
                });
    }
}
