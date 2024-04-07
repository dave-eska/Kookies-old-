#pragma once

#include<vector>
#include<memory>
#include<string>

#include"tile.h"
#include"transition.h"

struct Level{
    std::vector<std::unique_ptr<Tile>> tiles;
    std::string level_name;

    void changeLevel(std::string levelName);
    void changeLevel(std::unique_ptr<TransitionTile> tile);
};
