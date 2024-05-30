#include"tiling_util.h"

#include <cctype>
#include <iostream>
#include <fstream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "json/writer.h"

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

std::vector<std::unique_ptr<Tile>> loadLevelFromFile(std::string file_path, int& highet_z, Vector2& cvs_size, Vector2& str_pos){
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

    for(const auto& layer : root["layers"]) {
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
                    TransitionTile tile = TransitionTile(tile_id, {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
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
    str_pos = starting_pos;

    return vec;
}

std::vector<std::string> tilesToStrings(std::vector<std::unique_ptr<Tile>>& tiles, Vector2 canvas_size, int total_layers){
    std::vector<std::string> result;

    std::string layer;

    for(int j=0;j<total_layers;j++){
        for(int i=0;i<tiles.size();++i){
            if(tiles[i]->getZ() == j){
                std::string tileID = std::to_string(tiles[i]->getID());
                for(const char& c:tileID)
                    layer.push_back(c);
                if(tileID == "9"){
                    auto transitionTile = dynamic_cast<TransitionTile*>(tiles[i].get());
                    std::string temp  = transitionTile->getDestination();
                    for(const char& c:temp){
                        if(c=='.') break;
                        layer.push_back(c);
                    }
                }
                layer.push_back(' ');
                if((i+1) % (int)canvas_size.x == 0 && i != 0){
                    layer.push_back('\n');
                }
            }
        }

        // Remove the spaces behind newlines
        for(int i=0;i<layer.size();i++){
            if(layer[i] == ' ' && layer[i+1] == '\n'){
                layer.erase(layer.begin() + i);
            }
        }

        result.push_back(layer);
        layer.clear();
    }

    return result;
}

void writeTileJson(Level &level, Vector2 pos, std::string filename){
    Json::Value root;

    root["x"] = (int)pos.x;
    root["y"] = (int)pos.y;

    std::vector<std::string> strTiles = tilesToStrings(level.tiles, level.canvas_size, level.total_layers);
    for(int i=0;i<strTiles.size();i++)
        root["layers"][i] = strTiles[i];

    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, root);

    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << jsonString;
        outFile.close();
        std::cout << "JSON file written successfully." << std::endl;
    }else {
        std::cerr << "Error opening file for writing." << std::endl;
    }
}
