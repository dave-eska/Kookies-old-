#include"level.h"

#include"tiling_util.h"

void Level::changeLevel(std::string levelName){
    level_name = levelName;
    tiles = loadLevelFromFile(levelName, highest_z);
}
