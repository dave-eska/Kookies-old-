#include"seed.h"

#include<raylib.h>

#include<fstream>

#include<json/json.h>
#include<json/value.h>

#include"global_func.h"

#define GROW_TIME 2.0f

void SeedTile::Update(){
    timer += GetFrameTime();

    if(timer >= GROW_TIME){
        state += 1;
        timer = 0.0f;
    }
}

std::string SeedTile::Interact(){
    return "";
}

void SeedTile::Draw(bool is_debugging){
    Tile::Draw(is_debugging);
    DrawText(std::to_string(state).c_str(), body.x, body.y, 40, WHITE);
}

SeedTile::SeedTile(){
}

SeedTile::SeedTile(int id, Vector2 pos, int z_level){
    //Debug Variables
    isTouchingMouse=false;
    isRunningAnimation=true;

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

            if(jsonvalue.isMember("collision"))
                this->collision = jsonvalue["collision"].asBool();
            else
                this->collision = false;

            if(type == "BagOfSeed")
                this->seedID = jsonvalue["seed"].asInt();

            this->filename = "res/items/"+e;

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

    state = 0;
    timer = 0.0f;
}
