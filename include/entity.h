#pragma once

#include<string>

#include<raylib.h>

#include"player.h"

class Entity{
    protected:
        Rectangle body;
        int speed;
        Texture2D texture;
        std::string levelName;
    public:
        virtual void Update(Player& plr);
        virtual void Draw();

        virtual void Draw_UI();

        std::string getLevelName(){return levelName;}

        Entity();
};
