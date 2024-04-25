#pragma once

#include "tile.h"
#include <memory>
#include <string>

namespace TileUpdateFunction{
    void Interact(std::unique_ptr<Tile>& tile, std::string& tile_interect_return_code);

    void UseCraftingTable(std::unique_ptr<Tile>& tile);

    void PlaceItem(std::unique_ptr<Tile>& tile);

    void PlantSeed(std::unique_ptr<Tile>& tile);

};
