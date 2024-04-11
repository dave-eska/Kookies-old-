#pragma once

#include<string>

#include"tile.h"

class SeedTile: public Tile{
    private:
        int state;
        float timer;
    public:
        void Update() override;
        std::string Interact() override;
        void Draw(bool is_debugging) override;

        SeedTile();
        SeedTile(int id, Vector2 pos, int z_level);
};
