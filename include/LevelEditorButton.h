#pragma once

#include<raylib.h>

class LevelEditorButton{
    private:
        Rectangle body;
        Texture2D texture;
    protected:
        void onClick(int &game_state);
    public:
        void Draw();
        void Update(int &game_state);

        LevelEditorButton(Vector2 position);
        LevelEditorButton();
};
