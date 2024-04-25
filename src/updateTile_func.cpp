#include "updateTile_func.h"

void TileUpdateFunction::Interact(std::unique_ptr<Tile>& tile, std::string& tile_interect_return_code){
    if(tile->getIsTouchingPlayer() && tile->getIsTouchingMouse() && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        if(!tile->Interact().empty()){
            tile_interect_return_code = tile->Interact();
        }else
            tile->Interact();
    }
}

void TileUpdateFunction::TakeItem(std::unique_ptr<Tile>& tile){
}

void TileUpdateFunction::UseCraftingTable(std::unique_ptr<Tile>& tile){
}

void TileUpdateFunction::PlaceItem(std::unique_ptr<Tile>& tile){
}

void TileUpdateFunction::PlantSeed(std::unique_ptr<Tile>& tile){
}
