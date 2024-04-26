#include"item.h"

#include"tile.h"

void assignInvSlot(InventoryItem& item, int slot){
    item.item_invslot=slot;
}

InventoryItem newItem(int tileID){
    return Tile(tileID, {0,0}, 0).asItem(1);
}

InventoryItem newItem(int tileID, int count){
    return Tile(tileID, {0,0}, 0).asItem(count);
}
