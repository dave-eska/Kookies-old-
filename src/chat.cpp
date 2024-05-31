#include"chat.h"
#include "raylib.h"
#include "screens.h"

#include<iostream>

void ChatText::Update(){
    timer-=GetFrameTime();
    if(timer<=0 && transparancy > 0)
        transparancy-=120*GetFrameTime();
}

void ChatText::Draw(){
    DrawTextEx(font, text.c_str(), pos, 25, 0, {color.r,color.g,color.b,transparancy});
}

void ChatText::decreaseY(){
    pos.y=707-45;
}

ChatText::ChatText(){
}

ChatText::ChatText(std::string text):text{text}{
    pos={30,(float)GetScreenHeight()-(40+35)};
    transparancy=255;
    timer=3.3f;
    color = BLACK;
}

ChatText::ChatText(std::string text, Color color):text{text}{
    pos={30,(float)GetScreenHeight()-(40+35)};
    transparancy=255;
    timer=3.3f;
    this->color = color;
}

void addChatText(std::vector<ChatText> &cTextVec, std::string text){
    if(cTextVec.size()>0){
        for(int i=0;i<cTextVec.size();i++){
            cTextVec[i].pos.y-=25;
        }
    }
    cTextVec.push_back(ChatText(text));
}

void addChatText(std::vector<ChatText> &cTextVec, std::string text, Color color){
    if(cTextVec.size()>0){
        for(int i=0;i<cTextVec.size();i++){
            cTextVec[i].pos.y-=25;
        }
    }
    cTextVec.push_back(ChatText(text, color));
}
