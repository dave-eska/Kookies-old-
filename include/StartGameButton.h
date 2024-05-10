#pragma once

#include<raylib.h>

#include"Button.h"

class StartGameButton : public Button{
    protected:
        void onClick(int &game_state) override;
    public:

        StartGameButton(Vector2 position);
        StartGameButton();
};
