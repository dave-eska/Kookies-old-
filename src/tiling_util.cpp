#include"tiling_util.h"

#include "json/writer.h"
#include<algorithm>
#include <cctype>
#include<iostream>
#include<fstream>
#include <limits>
#include<memory>
#include <string>
#include <vector>

#include"tile.h"
#include"transition.h"
#include"seed.h"

static bool compareTiles(std::unique_ptr<Tile>& tile1, std::unique_ptr<Tile>& tile2){
    return tile1->getZ() < tile2->getZ();
}

static std::string readFile(const std::string& filename){
    std::ifstream file(filename);
    std::string fileContents, line;

    if (file.is_open()) {
        while (getline(file, line)) {
            fileContents += line + "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file " << filename << std::endl;
    }

    return fileContents;
}

static bool isEnglishAlphabet(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) != 0;
}

static bool isStrDigit(std::string str){
    bool val;
    for(char c : str){
        val = std::isdigit(c);
        if(val == false)
            break;
    }
    return val;
}

std::vector<std::unique_ptr<Tile>> loadLevelFromFile(std::string file_path, int& highet_z, Vector2& cvs_size){
    std::vector<std::unique_ptr<Tile>> vec;
    std::string text = readFile(file_path);
    Json::Value root;
    Json::Reader jsonreader;

    Vector2 canvas_size{0, 0};

    jsonreader.parse(text, root);

    Vector3 pos;
    int x = 0;
    int y = 0;
    int z = 0;

    std::string id;
    std::string destination;

    bool isReadingDestination;

    Vector2 starting_pos={root["x"].asFloat(), root["y"].asFloat()};

    for (const auto& layer : root["layers"]) {
        y = 0;
        x = 0;
        for (const auto& e : layer.asString()) {
            if((e == ' ' or e == '\n' or e == ',') && !id.empty()){
                int tile_id = std::stoi(id);
                if(tile_id == Cherryseeds_Tile){
                    SeedTile tile = SeedTile(tile_id, {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                    tile.setSlot(vec.size());
                    vec.push_back(std::make_unique<SeedTile>(tile));
                    id.clear();
                }else if(tile_id == Transition_Tile){
                    TransitionTile tile = TransitionTile({starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                    tile.setSlot(vec.size());
                    if(!destination.empty()){
                        tile.attachLevel(destination);
                        destination.clear();
                    }
                    vec.push_back(std::make_unique<TransitionTile>(tile));
                    id.clear();
                }else{
                    if(isStrDigit(id)){
                        Tile tile = Tile(tile_id, {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                        tile.setSlot(vec.size());
                        vec.push_back(std::make_unique<Tile>(tile));
                        id.clear();
                    }
                }

                if(e == ' '){
                    x++;
                    canvas_size.x = x;
                }else if(e == '\n' or ','){
                    y++;
                    canvas_size.y = y;
                    x = 0;
                }

            }else if (isdigit(e)) {
                id.push_back(e);
            }else if(isEnglishAlphabet(e)){
                destination.push_back(e);
            }
            canvas_size.y = y;
        }
        if(!id.empty()){
            Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
            tile.setSlot(vec.size());
            vec.push_back(std::make_unique<Tile>(tile));
            id.clear();
        }
        z++;
    }

    highet_z = z;
    cvs_size = {canvas_size.x+1, canvas_size.y+1};

    std::sort(vec.begin(), vec.end(), compareTiles);

    return vec;
}

std::vector<std::string> concatenateTileIds(const std::vector<std::unique_ptr<Tile>>& tiles, int max_x) {
    std::vector<std::string> result;
    std::stringstream ss;
    int current_layer = std::numeric_limits<int>::min(); // Initialize current_layer with smallest possible integer
    int count = 0;
    for (const auto& tilePtr : tiles) {
        if(tilePtr->getZ() != current_layer) {
            if (!ss.str().empty()) {
                result.push_back(ss.str());
                ss.str(""); // Clear stringstream for new string
            }
            current_layer = tilePtr->getZ();
            count = 0; // Reset count for new layer
        }
        ss << tilePtr->getID() << " ";
        count++;
        if(count == max_x) {
            ss.seekp(-1, std::ios_base::end);
            ss << '\n'; // Add new line after max_x tile IDs
            count = 0; // Reset count
        }
    }
    if(!ss.str().empty()) {
        result.push_back(ss.str());
    }
    return result;
}

void writeTileJson(const std::vector<std::unique_ptr<Tile>>& tiles, Vector2 pos, int max_x, const std::string& filename) {
    Json::Value root;

    root["x"] = (int)pos.x;
    root["y"] = (int)pos.y;

    auto strTiles = concatenateTileIds(tiles, max_x);;
    for(int i=0; i<strTiles.size(); i++){
        root["layers"][i] = strTiles[i];
    }

    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, root);

    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << jsonString;
        outFile.close();
        std::cout << "JSON file written successfully." << std::endl;
    } else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}
