#include"item.h"

#include"tile.h"

void assignInvSlot(InventoryItem& item, int slot){
    item.item_invslot=slot;
}

InventoryItem newItem(int tileID){
    return Tile(tileID, {0,0}, 0).asItem(1);
}
