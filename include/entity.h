#pragma once

#include<string>

#include<raylib.h>

#include"player.h"

class Entity{
    protected:
        Rectangle body;
        int speed;
        int health;
        Texture2D texture;
        std::string levelName;
    public:
        bool isDead(){return health <= 0;}

        virtual void Update(Player& plr, Camera2D& camera);
        virtual void Draw();

        virtual void Draw_UI();

        std::string getLevelName(){return levelName;}

        Entity();
};
