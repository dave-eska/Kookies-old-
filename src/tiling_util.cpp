#include"tiling_util.h"

#include<algorithm>
#include<iostream>
#include<fstream>
#include<memory>

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
            // TODO: Fix the \n bug ;) (ill do it later i promise ;])
            if (e == '\n') {
                Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                vec.push_back(std::make_unique<Tile>(tile));
                id.clear();
                y++;
                canvas_size.x = x;
                x=0;
            }else if(e == ' ' && !id.empty()){
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
                    Tile tile = Tile(tile_id, {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                    tile.setSlot(vec.size());
                    vec.push_back(std::make_unique<Tile>(tile));
                    id.clear();
                }
                x++;
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

void writeTileJson(const std::vector<std::unique_ptr<Tile>>& tiles, int x, int y, const std::string& filename) {
    Json::Value root;

    std::string layer;
    for (const auto& tile : tiles) {
        layer += std::to_string(tile->getID()) + " ";
    }
    root["layer"] = layer;

    root["x"] = x;
    root["y"] = y;

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return;
    }

    Json::StyledStreamWriter writer;
    writer.write(file, root);

    std::cout << "JSON data written to file: " << filename << std::endl;
}
