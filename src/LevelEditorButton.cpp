#include"LevelEditorButton.h"

LevelEditorButton::LevelEditorButton(Vector2 position){
    body={
        position.x,
        position.y,
        430,
        150
    };
    texture=LoadTexture("res/img/level_editor_button.png");
}

void LevelEditorButton::onClick(int &game_state) {
    body.x=110;
    body.y=590;
    game_state=1;
}

LevelEditorButton::LevelEditorButton(){
}

void LevelEditorButton::Update(int &game_state){
    body.x = 110;
    body.y = 590;
    if(CheckCollisionPointRec(GetMousePosition(), body)){
        body.x = 130;
        body.y = 610;
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onClick(game_state);
        }
    }
}

void LevelEditorButton::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 10, WHITE);
}
