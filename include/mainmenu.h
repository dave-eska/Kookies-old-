#pragma once

#include<raylib.h>

class StartGame{
    private:
        Rectangle body;
        Texture2D texture;
    protected:
        void onClick(int &game_state);
    public:
        void Draw();
        void Update(int &game_state);

        StartGame(Vector2 position);
        StartGame();
};

//Variables
inline bool menu_should_load=true;
inline StartGame *Start_Game_Button;
inline Texture2D main_menu_texture;

//Functions
void loadvars(int &game_state);
void main_menu_code(int &game_state);
