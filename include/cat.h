#pragma once

#include <raylib.h>

#include "animation.h"
#include "entity.h"

#include "player.h"

class Cat: public Entity{
    private:
        int health;
        bool isFollowing;
        Vector2 pos_diff;

        bool isDrawingRedBox;
        float drawRedBoxTimer;

        SpriteAnimation animation;
        SpriteAnimation idle;
        bool isPlayingAnimation;
    public:
        void Update(Player& plr, Camera2D& camera) override;
        void Draw() override;

        Cat();
        Cat(Vector2 max_diff_pos, Player& player, std::string levelName);
};
