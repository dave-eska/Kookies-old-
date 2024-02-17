#pragma once

#include<raylib.h>

class StartGameButton{
    private:
        Rectangle body;
        Texture2D texture;
    protected:
        void onClick(int &game_state);
    public:
        void Draw();
        void Update(int &game_state);

        StartGameButton(Vector2 position);
        StartGameButton();
};
