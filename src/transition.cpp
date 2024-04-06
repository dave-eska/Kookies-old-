#include"transition.h"

#include<fstream>

#include<json/json.h>
#include<json/value.h>

#include"global_func.h"
#include"raylib.h"

void TransitionTile::attachLevel(std::string levelName){
    this->dest = "res/maps/" + levelName + ".json";
}

void TransitionTile::Update(){
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        changeMainLevel(dest);
    }
}

void TransitionTile::Draw(bool is_debugging){
    Tile::Draw(is_debugging);

    DrawText(dest.c_str(), body.x, body.y, 20, BLACK);
}

TransitionTile::TransitionTile(){
}

TransitionTile::TransitionTile(Vector2 pos, int z_level){
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

            this->filename = "res/items/"+e;
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
