#include "fish.h"

#include <raylib.h>

void Fish::Update(Player& plr, Camera2D& camera){
}

void Fish::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 3, variant);
}

Fish::Fish(){
}

Fish::Fish(Vector2 pos, std::string levelName){
    body = {pos.x, pos.y, 96, 96};
    unsigned char r = GetRandomValue(0, 255);
    unsigned char g = GetRandomValue(0, 255);
    unsigned char b = GetRandomValue(0, 255);
    variant = {r, g, b, 255};
    texture = LoadTexture("res/img/jerry.png");
    this->levelName = levelName;
}
