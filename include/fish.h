#pragma once

#include <raylib.h>

#include "entity.h"

#include "player.h"

class Fish: public Entity{
    private:
        Color variant;
    public:
        void Update(Player& plr, Camera2D& camera) override;
        void Draw() override;

        Fish();
        Fish(Vector2 pos, std::string levelName);
};
