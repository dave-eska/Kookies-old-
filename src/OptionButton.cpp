#include "OptionButton.h"

OptionButton::OptionButton(Vector2 pos):Button({pos.x, pos.y, 15*5, 12*5}, LoadTexture("res/img/OptionButton.png"), 5, 10){
}

OptionButton::OptionButton(){
}

void OptionButton::onClick(int &game_state){
    game_state = 3;
}
