#pragma once

#include <raylib.h>

#include "Button.h"

class OptionButton : public Button{
    protected:
        void onClick(int &game_state) override;
    public:
        OptionButton(Vector2 pos);
        OptionButton();
};
