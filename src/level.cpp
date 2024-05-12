#include"level.h"

#include"tiling_util.h"
#include <iostream>

void Level::changeLevel(std::string levelName){
    level_name = levelName;
    tiles = loadLevelFromFile(levelName, total_layers, canvas_size, starting_pos);
    std::cout<<"Level changed to "<<level_name<<std::endl;
}
