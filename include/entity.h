#pragma once

#include<raylib.h>

#include"player.h"

class Entity{
    protected:
        Rectangle body;
        int speed;
        Texture2D texture;
    public:
        virtual void Update(Player& plr);
        virtual void Draw();

        virtual void Draw_UI();

        Entity();
};
