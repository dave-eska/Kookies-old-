#include"npc.h"
#include "raylib.h"
#include <string>

void NPC::Update(Player& plr){
    if(CheckCollisionRecs(body, plr.getBody())){
        if(IsKeyPressed(KEY_I)){
            if(conv.hasFinised()){
                conv.restart();
            }
            isTalking = true;
        }
    }else{
        isTalking = false;
    }

    if(isTalking && !conv.hasFinised()){
        conv.respond();
    }
}

void NPC::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 9, WHITE);
}

void NPC::Draw_UI(){
    if(isTalking && !conv.hasFinised()){
        conv.Draw();
    }
}

NPC::NPC(){
}

NPC::NPC(Vector2 pos, std::string conv_dir, std::string first_message, int last_message_id){
    body = {pos.x, pos.y, 13*9, 34*9};
    texture = LoadTexture("res/img/npc.png");

    conv = Conversation(conv_dir, first_message, last_message_id);
    isTalking = false;
}