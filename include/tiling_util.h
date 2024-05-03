#pragma once

#include<raylib.h>

#include<json/value.h>
#include<json/json.h>

#include"tile.h"

std::vector<std::unique_ptr<Tile>> loadLevelFromFile(std::string file_path, int& highet_z, Vector2& cvs_size);
std::vector<std::string> concatenateTileIds(const std::vector<std::unique_ptr<Tile>>& tiles, int max_x);
void writeTileJson(const std::vector<std::unique_ptr<Tile>>& tiles, Vector2 pos, int max_x, const std::string& filename);
