#include"tile.h"

#include<vector>

#include<fstream>
#include<string>
#include<string>

#include<raylib.h>

#include<json/value.h>
#include<json/json.h>

#include"animation.h"

#include"global_func.h"

bool compareTiles(Tile& tile1, Tile& tile2){
    return tile1.getZ() < tile2.getZ();
}

InventoryItem Tile::asItem(int total_count){
    std::vector<RecipeItem> resep;

    Json::Reader jsonreader;

    std::ifstream file(filename);
    Json::Value jsonvalue;
    jsonreader.parse(file, jsonvalue);

    if(jsonvalue.isMember("recipe")){
        const Json::Value recipeArray = jsonvalue["recipe"];

        // Iterate through the JSON array and populate the vector
        for (const auto& elem : recipeArray) {
            RecipeItem arr;
            arr.id = elem[0].asInt();
            arr.count = elem[1].asInt();

            resep.push_back(arr);
        }
    }

    return{
        .tileID=id,
            .item_type=type,
            .item_name=name,
            .item_invslot=0,
            .item_count=total_count,
            .recipe = resep,
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
        DrawText(std::to_string((int)body.x).c_str(), body.x, body.y, 25, BLACK);
        DrawText(std::to_string((int)body.y).c_str(), body.x, body.y+30, 25, BLACK);
    }
}

void Tile::Update(){
}

Tile::Tile(){
    typeInChat("WARNING: CREATED A TILE WITH DEFAULT CONSTRUCTOR", YELLOW);
}

Tile::Tile(int id, Vector2 pos, int z_level){
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
}
