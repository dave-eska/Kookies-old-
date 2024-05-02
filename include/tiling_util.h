#pragma once

#include<raylib.h>

#include<json/value.h>
#include<json/json.h>

#include"tile.h"

std::vector<std::unique_ptr<Tile>> loadLevelFromFile(std::string file_path, int& highet_z, Vector2& cvs_size, Vector2& startingPos);
