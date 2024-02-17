#include"tile.h"

#include<algorithm>
#include<vector>
#include<fstream>
#include<string>
#include<string>
#include<iostream>

#include<raylib.h>

#include<json/value.h>
#include<json/json.h>

#include"animation.h"
#include"global_func.h"

bool compareTiles(Tile& tile1, Tile& tile2){
    return tile1.getZ() < tile2.getZ();
}

std::vector<Tile> loadLevelFromFile(std::string file_path){
    std::vector<Tile> vec;
    std::string text = readFile(file_path);
    Json::Value root;
    Json::Reader jsonreader;

    jsonreader.parse(text, root);

    Vector3 pos;
    int x = 0;
    int y = 0;
    int z = 0;

    std::string id;

    Vector2 starting_pos={root["x"].asFloat(), root["y"].asFloat()};
    // Vector2 relative_pos{0, 0}; // we can remake this later, just simplifying things for now
    //

    for (const auto& layer : root["layers"]) {
        y = 0;
        x = 0;
        std::cout<<layer.asString()<<std::endl;
        for (const auto& e : layer.asString()) {
            if (e == '\n') {
                Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                vec.push_back(tile);
                id.clear();
                y++;
                x=0;
            }else if(e == ' ' && !id.empty()){
                Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
                vec.push_back(tile);
                id.clear();
                x++;
            }else if (isdigit(e)) {
                id.push_back(e);
            }
        }
        if(!id.empty()){
            Tile tile = Tile(std::stoi(id), {starting_pos.x+x*TILE_SIZE, starting_pos.y+(y*TILE_SIZE)}, z);
            vec.push_back(tile);
            id.clear();
        }

        z++;
    }
   
    for(Tile e:vec){
        if(e.getName()=="itemarea"){
            int probability=GetRandomValue(0,5);
            if(probability == 2 or probability == 3){
                vec.push_back(Tile(probability, {e.getX(), e.getY()}, e.getZ()+1));
            }
        }
    }

    std::sort(vec.begin(), vec.end(), compareTiles);

    return vec;
}

InventoryItem Tile::asItem(int total_count){
    return{
        .tileID=id,
            .item_type=type,
            .item_name=name,
            .item_invslot=0,
            .item_count=total_count,
            .iconTexture=texture
    };
}

void Tile::Draw(bool is_debugging){
    if(isRunningAnimation) {
        DrawSpriteAnimationPro(animation, {body.x, body.y, TILE_SIZE, TILE_SIZE}, {0, 0}, 0, WHITE, isRunningAnimation);
    }
    else DrawTexturePro(texture, {0,0,32,32}, {body.x,body.y,TILE_SIZE,TILE_SIZE}, {0,0}, 0, WHITE);

    if(is_debugging){
        DrawTextureEx(debugbox, {body.x, body.y}, 0, 3, WHITE);
    }
}

void Tile::attachLevel(std::string levelName){
    if(name=="transitionarea")
        this->fileName = levelName;
}

Tile::Tile(){
    typeInChat("WARNING: CREATED A TILE WITH DEFAULT CONSTRUCTOR", YELLOW);
}

Tile::Tile(int id, Vector2 pos, int z_level){
    //Debug Variables
    isTouchingMouse=false;
    isRunningAnimation=false;

    debugbox=LoadTexture("res/img/debugbox.png");

    std::vector<std::string> file_names=getAllFileNamesInDirectory("res/items/");
    Json::Reader jsonreader;

    for(auto &e:file_names){
        std::ifstream file("res/items/"+e);
        Json::Value jsonvalue;
        jsonreader.parse(file, jsonvalue);

        //Cheecking if id is the same
        if(jsonvalue["id"].asInt()==id){
            body = {
                pos.x,
                pos.y,
                TILE_SIZE,
                TILE_SIZE
            };
            this->z_level=z_level;
            this->name = jsonvalue["name"].asString();
            this->id = jsonvalue["id"].asInt();
            this->type = jsonvalue["type"].asString();
            this->hasAnimation = jsonvalue["animation"].asBool();
            this->hasCollision = jsonvalue["collision"].asBool();

            if(jsonvalue.isMember("texture")){
                texture=LoadTexture(jsonvalue["texture"].asString().c_str());
            }else{
                int probability=GetRandomValue(1,2);
                if(probability==1) texture=LoadTexture(jsonvalue["texture1"].asString().c_str());
                if(probability==2) texture=LoadTexture(jsonvalue["texture2"].asString().c_str());
            }
        }
    }

    Rectangle animRect[TOTAL_ANIM_FRAME]={
        {0,0,32,32},
        {TILE_SIZE,0,32,32},
        {TILE_SIZE*2,0,32,32},
        {TILE_SIZE*3,0,32,32}
    };
    animation = CreateSpriteAnimation(texture, 8, animRect, 4);
}
