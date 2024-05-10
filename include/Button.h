#pragma once

#include<raylib.h>

class Button{
    private:
        Rectangle body;

        Vector2 prev_pos;
        Vector2 hold_pos;

        int draw_scale;

        Texture2D texture;
    protected:
        virtual void onClick(int &game_state);
    public:
        void Draw();
        void Update(int &game_state);

        Button(Rectangle body, Texture2D texture, int draw_scale, int hold_press_val);
        Button();
};
