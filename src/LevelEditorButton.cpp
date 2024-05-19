#include"LevelEditorButton.h"

LevelEditorButton::LevelEditorButton(Vector2 position):Button({position.x, position.y, 430, 150}, LoadTexture("res/img/level_editor_button.png"), 
        LoadTexture("res/img/level_editor_button.png"), 10, 10){
}

LevelEditorButton::LevelEditorButton(){
}

void LevelEditorButton::onClick(int &game_state) {
    game_state=1;
}
