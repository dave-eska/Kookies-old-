#include "cat.h"

#include <raylib.h>

#define CAT_WIDTH 26*4
#define CAT_HEIGHT 29*4

#define REDBOX_TIMER 0.5f

Cat::Cat(){
}

/*
Cat::Cat(Vector2 pos, std::string levelName){
    body = {pos.x, pos.y, CAT_WIDTH, CAT_HEIGHT};
    speed = 200;
    texture = LoadTexture("res/img/cat.png");
    isFollowing=false;
    this->levelName = levelName;
}
*/

Cat::Cat(Vector2 max_diff_pos, Player& player, std::string levelName){
    body = {max_diff_pos.x, max_diff_pos.y, CAT_WIDTH, CAT_HEIGHT};
    pos_diff = max_diff_pos;

    speed = 200;
    isFollowing = true;

    health = 10;
    drawRedBoxTimer = REDBOX_TIMER;
    isDrawingRedBox = false;

    texture = LoadTexture("res/img/cat.png");
    this->levelName = levelName;
}

void Cat::Update(Player& plr, Camera2D& camera){
    if(body.x > plr.getBody().x + plr.getBody().width + pos_diff.x)
        body.x -= speed * GetFrameTime();
    else if(body.x + body.width < plr.getBody().x - pos_diff.x)
        body.x += speed * GetFrameTime();

    if(body.y + body.height < plr.getBody().y)
        body.y += speed * GetFrameTime();
    else if(body.y > plr.getBody().y + plr.getBody().height)
        body.y -= speed * GetFrameTime();

    if(CheckCollisionRecs(body, plr.getSelectArea()) 
            && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), body)
            && plr.getInv().getItemFromCurrentSlot().filename == "res/tools.json"
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        health -= plr.getInv().getItemFromCurrentSlot().damage;
        isDrawingRedBox = true;
    }
    if(isDrawingRedBox){
        drawRedBoxTimer -= GetFrameTime()/10;
    }
    if(drawRedBoxTimer <= 0){
        isDrawingRedBox = false;
        drawRedBoxTimer = REDBOX_TIMER;
    }
}

void Cat::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 3.5, WHITE);
    if(isDrawingRedBox) DrawRectangleRec(body, {255, 0, 0, 255/2});
}
