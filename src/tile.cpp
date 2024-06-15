#include"tile.h"

#include<vector>
#include<fstream>
#include<string>

#include<raylib.h>

#include<json/value.h>
#include<json/json.h>

#include"animation.h"

bool compareTiles(Tile& tile1, Tile& tile2){
    return tile1.getZ() < tile2.getZ();
}

InventoryItem Tile::asItem(int total_count){
    std::vector<RecipeItem> resep;

    Json::Reader jsonreader;

    std::ifstream file("res/yes.json");
    Json::Value jsonvalue;
    jsonreader.parse(file, jsonvalue);

    if(jsonvalue[id].isMember("recipe")){
        const Json::Value recipeArray = jsonvalue[id]["recipe"];

        // Iterate through the JSON array and populate the vector
        for(const auto& elem : recipeArray) {
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
            .filename = "res/yes.json",
            .item_invslot=0,
            .item_count=total_count,
            .damage=1,
            .recipe = resep,
            .iconTexture=texture
    };
}

void Tile::Draw(bool is_debugging){
    if(runningAnimationOnDefault){
        DrawSpriteAnimationPro(animation, {body.x, body.y, TILE_SIZE, TILE_SIZE}, {0, 0}, rotation, WHITE);
    }
    else if(isRunningAnimation) {
        DrawSpriteAnimationPro(animation, {body.x, body.y, TILE_SIZE, TILE_SIZE}, {0, 0}, rotation, WHITE, isRunningAnimation);
    }
    else DrawTexturePro(texture, {0,0,32,32}, {body.x,body.y,TILE_SIZE,TILE_SIZE}, {0,0}, rotation, WHITE);

    if(is_debugging){
        DrawTextureEx(debugbox, {body.x, body.y}, 0, 3, WHITE);
        //DrawText(std::to_string((int)body.x).c_str(), body.x, body.y, 25, BLACK);
        //DrawText(std::to_string((int)body.y).c_str(), body.x, body.y+30, 25, BLACK);

        if(isTouchingPlayer){
            DrawRectangleRec(body, {0, 200, 10, 255/2});
        }
        if(isTouchingSelectAreaPlayer){
            DrawRectangleRec(body, {100, 100, 100, 255/4});
        }
        if(isTouchingMouse){
            DrawRectangleRec({(float)body.x+(body.width/2 - 15), body.y+(body.height/2-15), 30, 30}, BLACK);
        }
    }
}

void Tile::Update(){
}

std::string Tile::Interact(){
    return "";
}

Tile::Tile(){
    //typeInChat("WARNING: CREATED A TILE WITH DEFAULT CONSTRUCTOR", YELLOW);
}

Tile::Tile(int id, Vector2 pos, int z_level){
    //Debug Variables
    isTouchingMouse = false;
    isTouchingPlayer = false;
    isTouchingSelectAreaPlayer = false;
    isRunningAnimation = false;
    collision = false;
    rotation = 0;
    totalFrame = 1;
    runningAnimationOnDefault = false;
    fps = 4;

    body = { pos.x, pos.y, TILE_SIZE, TILE_SIZE };

    this->id = id;
    this->z_level = z_level;

    debugbox=LoadTexture("res/img/debugbox.png");

    Json::Reader jsonreader;

    std::ifstream file("res/yes.json");
    Json::Value jsonvalue;
    jsonreader.parse(file, jsonvalue);

    //typeInChat(std::to_string(jsonvalue.size()));

    if(!jsonvalue.isArray()) return;
    if(id <= jsonvalue.size() && id >= 0){
        auto tileData = jsonvalue[id];

        name = tileData["name"].asString();
        type = tileData["type"].asString();

        if(tileData.isMember("collision")) collision = jsonvalue[id]["collision"].asBool();
        if(tileData.isMember("seed")) fruitID = jsonvalue[id]["seed"].asInt();
        if(tileData.isMember("animateByDefault")) runningAnimationOnDefault = jsonvalue[id]["animateByDefault"].asBool();

        if(tileData.isMember("fps")){
            if(tileData["fps"].isArray()){
                int probability = GetRandomValue(0,tileData["fps"].size()-1);
                fps = tileData["fps"][probability].asInt();
            }else{
                fps = tileData["fps"].asInt();
            }
        }

        if(tileData.isMember("texture")){
            if(tileData["texture"].isArray()){
                int probability = GetRandomValue(0,tileData["texture"].size()-1);
                texture = LoadTexture(tileData["texture"][probability].asString().c_str());
            }
            else
                texture = LoadTexture(tileData["texture"].asString().c_str());
        }else{
            texture = LoadTexture("");
        }
    }
    hasAnimation = texture.width/32 > 1 ? true : false;
    totalFrame = texture.width/32;

    Rectangle animRect[totalFrame];
    for(int i=0;i<totalFrame;i++){
        animRect[i] = {(float)32*i, 0, 32, 32};
    }

    animation = CreateSpriteAnimation(texture, fps, animRect, totalFrame);
}

Tile::Tile(int id) : Tile(id, {0,0}, 0){
}
