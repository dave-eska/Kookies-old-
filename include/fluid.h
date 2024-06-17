#pragma once

#include<string>

#include<raylib.h>

#include"tile.h"

class FluidTile: public Tile{
    private:
        bool hasWave;
        bool hasFish;

        float timer;
        int fishSlot;

        int old_fps;
    public:
        void Update() override;
        std::string Interact() override;
        void Draw(bool is_debugging) override;

        FluidTile();
        FluidTile(int id, Vector2 pos, int z_level);
};
