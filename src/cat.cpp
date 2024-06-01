#include "cat.h"
#include "screens.h"

#include <raylib.h>

#define REDBOX_TIMER 0.5f

Cat::Cat(){
}

Cat::Cat(Vector2 max_diff_pos, Player& player, std::string levelName){
    speed = 200;
    isFollowing = true;

    health = 30;
    drawRedBoxTimer = REDBOX_TIMER;
    isDrawingRedBox = false;

    texture = LoadTexture("res/img/cat.png");
    redBox = LoadTexture("res/img/redCat.png");
    this->levelName = levelName;

    body = {max_diff_pos.x, max_diff_pos.y, (float)texture.width/3, (float)texture.height};
    pos_diff = max_diff_pos;

    Rectangle walkingRect[] = {
        {body.width, 0, body.width, body.height},
        {body.width*2, 0, body.width, body.height}};

    Rectangle idleRect[] = {
        {0, 0, body.width, body.height},
    };

    animation = CreateSpriteAnimation(texture, 4, walkingRect, 2);
    idle = CreateSpriteAnimation(texture, 4, idleRect, 1);
    isPlayingAnimation = false;
}

void Cat::Update(Player& plr, Camera2D& camera){
    isPlayingAnimation = true;
    if(body.x > plr.getBody().x + plr.getBody().width + pos_diff.x){
        body.x -= speed * GetFrameTime();
        isPlayingAnimation = true;
    }
    else if(body.x + body.width < plr.getBody().x - pos_diff.x){
        body.x += speed * GetFrameTime();
        isPlayingAnimation = true;
    }

    if(body.y + body.height < plr.getBody().y){
        body.y += speed * GetFrameTime();
        isPlayingAnimation = true;
    }
    else if(body.y > plr.getBody().y + plr.getBody().height){
        body.y -= speed * GetFrameTime();
        isPlayingAnimation = true;
    }

    if(CheckCollisionRecs(body, plr.getSelectArea()) 
            && CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), camera), body)
            && plr.getInv().getItemFromCurrentSlot().filename == "res/tools.json"
            && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int plr_damage = plr.getInv().getItemFromCurrentSlot().damage;
        health -= plr_damage;
        isDrawingRedBox = true;
    }
    if(isDrawingRedBox){
        drawRedBoxTimer -= GetFrameTime()*4;
    }
    if(drawRedBoxTimer <= 0){
        isDrawingRedBox = false;
        drawRedBoxTimer = REDBOX_TIMER;
    }
}

void Cat::Draw(){
    DrawSpriteAnimationPro(isPlayingAnimation ? animation : idle, {body.x, body.y, body.width, body.width}, {0,0}, 0, WHITE);
    if(isDrawingRedBox){
        DrawTexture(redBox, body.x, body.y, WHITE);
        DrawTextEx(font, std::to_string(health).c_str(), {body.x, body.y}, 25, 0, WHITE);
    }
}
