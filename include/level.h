#pragma once

#include<vector>
#include<memory>
#include<string>

#include"tile.h"

struct Level{
    std::vector<std::unique_ptr<Tile>> tiles;
    std::string level_name;
    
    Vector2 canvas_size;
    int total_layers;

    void changeLevel(std::string levelName);
};
