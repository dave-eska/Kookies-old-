#include"tiling_util.h"

#include<algorithm>
#include<iostream>
#include<fstream>
#include<memory>

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

std::vector<std::unique_ptr<Tile>> loadLevelFromFile(std::string file_path){
    std::vector<std::unique_ptr<Tile>> vec;
    std::string text = readFile(file_path);
    Json::Value root;
    Json::Reader jsonreader;

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
            if (e == '\n') {
                Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                vec.push_back(std::make_unique<Tile>(tile));
                id.clear();
                y++;
                x=0;
            }else if(e == ' ' && !id.empty()){
                int tile_id = std::stoi(id);
                switch(tile_id){
                    default:
                        {
                            Tile tile = Tile(tile_id, {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                            tile.setSlot(vec.size());
                            if(!destination.empty()){
                                tile.attachLevel(destination);
                                destination.clear();
                            }
                            vec.push_back(std::make_unique<Tile>(tile));
                            id.clear();
                        }
                }
                x++;
            }else if (isdigit(e)) {
                id.push_back(e);
            }else if(isEnglishAlphabet(e)){
                destination.push_back(e);
            }
        }
        if(!id.empty()){
            Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
            vec.push_back(std::make_unique<Tile>(tile));
            id.clear();
        }

        z++;
    }

    std::sort(vec.begin(), vec.end(), compareTiles);

    return vec;
}