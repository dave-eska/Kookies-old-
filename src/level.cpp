#include"level.h"

#include"tiling_util.h"

void Level::changeLevel(std::string levelName){
    level_name = levelName;
    tiles = loadLevelFromFile(levelName);
}

void Level::changeLevel(std::unique_ptr<TransitionTile> tile){
    level_name = tile->getDestination();
    tiles = loadLevelFromFile(level_name);
}
