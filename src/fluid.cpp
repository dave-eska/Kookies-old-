#include "fluid.h"

#include <raylib.h>

#include "global_func.h"

void FluidTile::Update(){
    if(getPlayer().isFishing() && isTouchingMouse && !hasFish){
        timer -= GetFrameTime();
        if(timer <= 0){
            hasFish = GetRandomValue(false, true);
            timer = 2.0f;
        }
    }
}

std::string FluidTile::Interact(){
    return "";
}

void FluidTile::Draw(bool is_debugging){
    if(runningAnimationOnDefault){
        DrawSpriteAnimationPro(animation, {body.x, body.y, TILE_SIZE, TILE_SIZE}, {0, 0}, rotation, WHITE);
    }
    else DrawTexturePro(texture, {0,0,32,32}, {body.x,body.y,TILE_SIZE,TILE_SIZE}, {0,0}, rotation, WHITE);

    if(hasFish)
        animation.framesPerSecond = 8;

    if(is_debugging){
        DrawTextureEx(debugbox, {body.x, body.y}, 0, 3, WHITE);

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

FluidTile::FluidTile(){
}

FluidTile::FluidTile(int id, Vector2 pos, int z_level) : Tile(id, pos, z_level){
    hasFish = false;
    timer = 2.0f;
}
