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

void TileUpdateFunction::PlaceItem(std::unique_ptr<Tile>& tile){
}

void TileUpdateFunction::PlantSeed(std::unique_ptr<Tile>& tile){
}
