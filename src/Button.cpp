#include "Button.h"
#include "raylib.h"

void Button::Draw(){
    if(isHovered)
        DrawTextureEx(Hoverred_texture, {body.x, body.y}, 0, draw_scale, WHITE);
    else{
        DrawTextureEx(texture, {body.x, body.y}, 0, draw_scale, WHITE);
    }
}

void Button::Update(int &game_state){
    isHovered = false;
    body.x = prev_pos.x;
    body.y = prev_pos.y;

    if(CheckCollisionPointRec(GetMousePosition(), body)){
        body.x = hold_pos.x;
        body.y = hold_pos.y;
        isHovered = true;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onClick(game_state);
            body.x = prev_pos.x;
            body.y = prev_pos.y;
        }
    } 
}

void Button::onClick(int &game_state){
}

Button::Button(Rectangle body, Texture2D texture, Texture2D Hoverred_texture, int draw_scale, int hold_press_val):
    body{body}, prev_pos{body.x, body.y}, hold_pos{body.x+hold_press_val, body.y+hold_press_val}, texture{texture}, Hoverred_texture{Hoverred_texture},
    draw_scale{draw_scale}{
        isHovered = false;
    }

Button::Button(){
}
