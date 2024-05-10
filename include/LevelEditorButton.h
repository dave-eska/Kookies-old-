#pragma once

#include<raylib.h>

#include "Button.h"

class LevelEditorButton : public Button{
    protected:
        void onClick(int &game_state) override;
    public:

        LevelEditorButton(Vector2 position);
        LevelEditorButton();
};
