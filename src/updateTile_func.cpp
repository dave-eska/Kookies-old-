#include "updateTile_func.h"
#include "global_func.h"

void TileUpdateFunction::Interact(std::unique_ptr<Tile>& tile, std::string& tile_interect_return_code){
    if(tile->getIsTouchingPlayer() && tile->getIsTouchingMouse() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        if(!tile->Interact().empty()){
            tile_interect_return_code = tile->Interact();
        }else
            tile->Interact();
    }
}

void TileUpdateFunction::UseCraftingTable(std::unique_ptr<Tile>& tile){
    if(tile->getID() == Craftingtable_Tile){
        if(tile->getIsTouchingPlayer()){
            if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && tile->getIsTouchingMouse()){
                getPlayer().toggleInvenCrafting();
            }
        }else{
            getPlayer().getInv().setIsCrafting(false);
        }
    }
}

void TileUpdateFunction::PlaceItem(std::unique_ptr<Tile>& tile, Level& level){
    if(tile->getID() == PlaceArea_Tile && tile->getIsTouchinSelectAreaPlayer()){
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                tile->getIsTouchingMouse()){
            Vector2 belowPos = {tile->getBody().x, tile->getBody().y};
            int below_z = tile->getZ();
            auto it = std::find_if(level.tiles.begin(), level.tiles.end(),
                    [belowPos, below_z](const auto& item) {
                    return item->getPos().x == belowPos.x && item->getPos().y == belowPos.y && item->getZ() > below_z && item->getType() == "Item";
                    });
            if(it == level.tiles.end()){
                level.tiles.push_back(std::make_unique<Tile>
                        (Tile(getPlayer().getCurrentInvIDSlot(), {tile->getX(), tile->getY()}, tile->getZ()+1)));
                getPlayer().decreaseItemInv(getPlayer().getCurrentInvSlot());
            }
        }
    }
}

void TileUpdateFunction::PlantSeed(std::unique_ptr<Tile>& tile){
}
