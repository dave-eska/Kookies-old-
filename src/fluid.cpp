#include "fluid.h"

#include <raylib.h>
#include <string>

#include "fish.h"
#include "global_func.h"
#include "item.h"
#include "tile.h"

void FluidTile::Update(){
    if(getPlayer().isFishing() && isTouchingMouse && !hasWave){
        timer -= GetFrameTime();
        if(timer <= 0){
            hasWave = GetRandomValue(false, true);
            timer = 2.0f;
        }
    }
    if(getPlayer().isFishing() && hasWave && IsKeyPressed(KEY_SPACE)){
        hasFish = true;
        getCurrentLevel().AddEntity<Fish>((Fish({body.x, body.y}, "save.json")), fishSlot);
    }

    if(hasFish && isTouchingMouse && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        getCurrentLevel().removeEntity(fishSlot);
        getPlayer().addItemInv(newItem<Tile>(RawFish_Tile));
        hasWave = false;
        hasFish = false;
        animation.framesPerSecond = old_fps;
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

    if(hasWave)
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
    hasWave = false;
    timer = 2.0f;
    old_fps = animation.framesPerSecond;
}
