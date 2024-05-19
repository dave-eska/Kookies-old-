#include "OptionButton.h"

OptionButton::OptionButton(Vector2 pos):Button({pos.x, pos.y, 15*6, 12*6}, LoadTexture("res/img/OptionButton.png"), LoadTexture("res/img/OptionButton.png"), 5, 5){
}

OptionButton::OptionButton(){
}

void OptionButton::onClick(int &game_state){
    game_state = 3;
}
