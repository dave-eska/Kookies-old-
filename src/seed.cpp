#include"seed.h"

#include <iostream>
#include<raylib.h>

#include<fstream>

#include<json/json.h>
#include<json/value.h>
#include <string>
#include <vector>

#include"global_func.h"

#define GROW_TIME 2.0f
#define MAX_STATE 4

void SeedTile::Update(){
    if(state < MAX_STATE && state < state_textures.size())
        timer += GetFrameTime();

    if(timer >= GROW_TIME){
        state += 1;
        timer = 0.0f;
    }
}

std::string SeedTile::Interact(){
    if(state >= MAX_STATE){
        getPlayer().addItemInv(Tile(fruitID, {}, 1).asItem(0));
        hasBeenHarved = true;
        typeInChat("Taken Some Cherries");
    }

    if(hasBeenHarved){
        std::string tile_return_code = "x001 " + std::to_string(slot);
        std::cout<<tile_return_code<<std::endl;
        return tile_return_code;
    }

    return "";
}

void SeedTile::Draw(bool is_debugging){
    if(!state_textures.empty())
        DrawTexturePro(state_textures[state], {0,0,32,32}, {body.x,body.y,TILE_SIZE,TILE_SIZE}, {0,0}, 0, WHITE);

    if(is_debugging){
        DrawTextureEx(debugbox, {body.x, body.y}, 0, 3, WHITE);
        //DrawText(std::to_string((int)body.x).c_str(), body.x, body.y, 25, BLACK);
        //DrawText(std::to_string((int)body.y).c_str(), body.x, body.y+30, 25, BLACK);

        if(isTouchingPlayer){
            DrawRectangleRec(body, {0, 200, 10, 255/2});
        }
        if(isTouchingSelectAreaPlayer){
            DrawRectangleRec(body, {0, 0, 210, 255/2});
        }
        if(isTouchingMouse){
            DrawRectangleRec({(float)body.x+(body.width/2 - 15), body.y+(body.height/2-15), 30, 30}, BLACK);
        }

        DrawText(std::to_string(state).c_str(), body.x, body.y, 40, WHITE);
    }
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
            this->fruitID = (TileID)jsonvalue["fruitID"].asInt();

            this->collision = false;
            this->filename = "res/items/"+e;

            for(int i=0;i<jsonvalue["textures"].size();i++){
                state_textures.push_back(LoadTexture(jsonvalue["textures"][i].asString().c_str()));
            }
        }
    }

    state = 0;
    timer = 0.0f;
    hasBeenHarved = false;
}
