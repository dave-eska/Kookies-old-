#pragma once

#include<raylib.h>

#include"entity.h"

#include"player.h"

#include"conversation.h"

class NPC: public Entity{
    private:
        Conversation conv;
        bool isTalking;
    public:
        void Update(Player& plr, Camera2D& camera) override;
        void Draw() override;

        void Draw_UI() override;
        
        NPC();
        NPC(std::string levelName, Vector2 pos, std::string conv_dir, std::string first_message, int last_message_id);
};
