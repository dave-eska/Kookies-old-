#include "item.h"
#include "enchant.h"
#include <string>

void assignInvSlot(InventoryItem& item, int slot){
    item.item_invslot=slot;
}

void InventoryItem::UpdateDrawItem(){
    drawItem = item_name + "\n";
    for(Enchant item : enchants){
        drawItem.append(EnchantToString(item) + "\n");
    }
    if(filename == "res/tools.json"){
        drawItem.append("Damage: ");
        drawItem.append(std::to_string(damage));
    }
}
