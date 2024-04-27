#pragma once

#include<string>
#include<vector>

#include<raylib.h>

#include"tile.h"

class SeedTile: public Tile{
    private:
        int state;
        TileID fruitID;
        float timer;
        bool hasBeenHarved;

        std::vector<Texture2D> state_textures;
    public:

        int getFruitID(){return fruitID;}

        void Update() override;
        std::string Interact() override;
        void Draw(bool is_debugging) override;

        SeedTile();
        SeedTile(int id, Vector2 pos, int z_level);
};
