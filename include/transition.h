#pragma once

#include<string>

#include<raylib.h>

#include"tile.h"

class TransitionTile: public Tile{
    private:
        std::string dest /*Only being used by `transation` tile*/;
    public:

        std::string getDestination(){return dest;} /*Only being used by `transation` tile*/

        void attachLevel(std::string levelName); /*Only being used by `transation` tile*/

        std::string Interact() override;
        void Draw(bool is_debugging) override;

        TransitionTile();
        TransitionTile(Vector2 pos, int z_level);
};
