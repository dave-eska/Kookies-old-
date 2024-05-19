#include"StartGameButton.h"

StartGameButton::StartGameButton(Vector2 pos):Button({pos.x, pos.y, 430, 150}, LoadTexture("res/img/start_game_button.png"),
        LoadTexture("res/img/start_game_button_hovered.png"), 10, 10){
}

StartGameButton::StartGameButton(){
}

void StartGameButton::onClick(int &game_state) {
    game_state=2;
}
