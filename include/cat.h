#pragma once

#include <raylib.h>

#include "animation.h"
#include "entity.h"

#include "player.h"

class Cat: public Entity{
    private:
        bool isFollowing;
        Vector2 pos_diff;

        bool isDrawingRedBox;
        float drawRedBoxTimer;

        int rotation;

        SpriteAnimation animation;
        SpriteAnimation idle;
        bool isPlayingAnimation;

        Texture2D redBox;

        int plrDamage;
    public:
        void Update(Player& plr, Camera2D& camera) override;
        void Draw() override;

        Cat();
        Cat(Vector2 max_diff_pos, Player& player, std::string levelName);
};
