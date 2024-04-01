#pragma once

#include<raylib.h>

#include"entity.h"

#include"player.h"

class NPC: public Entity{
    private:
        int health;
        bool isFollowing;
        Vector2 pos_diff;
    public:
        void Update(Player& plr) override;
        void Draw() override;
        
        NPC();
        NPC(Vector2 pos);
        NPC(Vector2 max_diff_pos, Player& player);
};
