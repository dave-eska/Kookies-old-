#pragma once

#include<raylib.h>

#include"entity.h"

#include"player.h"

class Cat: public Entity{
    private:
        int health;
        bool isFollowing;
        Vector2 pos_diff;
    public:
        void Update(Player& plr) override;
        void Draw() override;
        
        Cat();
        Cat(Vector2 pos);
        Cat(Vector2 max_diff_pos, Player& player);
};
