#include"StartGameButton.h"

StartGameButton::StartGameButton(Vector2 position){
    body={
        position.x,
        position.y,
        430,
        150
    };
    texture=LoadTexture("res/img/start_game_button.png");
}

void StartGameButton::onClick(int &game_state) {
    game_state=2;
    body.x=110;
    body.y=370;
}

StartGameButton::StartGameButton(){
}

void StartGameButton::Update(int &game_state){
    body.x=110;
    body.y=370;
    if(CheckCollisionPointRec(GetMousePosition(), body)){
        body.x=130;
        body.y=390;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onClick(game_state);
        }
    }
}

void StartGameButton::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 10, WHITE);
}
