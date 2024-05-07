#pragma once

#include<raylib.h>

#include<json/value.h>
#include<json/json.h>

#include "level.h"
#include "tile.h"

std::vector<std::unique_ptr<Tile>> loadLevelFromFile(std::string file_path, int& highet_z, Vector2& cvs_size);
std::vector<std::string> tilesToStrings(std::vector<std::unique_ptr<Tile>>& tiles, Vector2 canvas_size, int total_layers);
void writeTileJson(Level &level, Vector2 pos, std::string filename);
