#pragma once

#include<string>
#include<vector>
#include<raylib.h>

class ChatText{
    private:
        std::string text;
        float timer;
        unsigned char transparancy;
        Color color;
    public:
        Vector2 pos;

        bool IsDone(){return transparancy <= 0;}

        void Update();
        void Draw();

        void decreaseY();

        ChatText();
        ChatText(std::string text);
        ChatText(std::string text, Color color);
};

void addChatText(std::vector<ChatText> &cTextVec, std::string text);
void addChatText(std::vector<ChatText> &cTextVec, std::string text, Color color);
