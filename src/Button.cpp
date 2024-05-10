#include "Button.h"
#include "raylib.h"

void Button::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, draw_scale, WHITE);
}

void Button::Update(int &game_state){
    body.x = prev_pos.x;
    body.y = prev_pos.y;

    if(CheckCollisionPointRec(GetMousePosition(), body)){
        body.x = hold_pos.x;
        body.y = hold_pos.y;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onClick(game_state);
            body.x = prev_pos.x;
            body.y = prev_pos.y;
        }
    }
}

void Button::onClick(int &game_state){
}

Button::Button(Rectangle body, Texture2D texture, int draw_scale, int hold_press_val):
    body{body}, prev_pos{body.x, body.y}, hold_pos{body.x+hold_press_val, body.y+hold_press_val}, texture{texture}, draw_scale{draw_scale}{
}

Button::Button(){
}
