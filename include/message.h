#pragma once

#include<string>
#include<vector>

#include<raylib.h>

class Message{
    private:
        Rectangle body;
        Texture2D texture;
        Texture2D faceTexture;

        std::string jsonfile;

        std::vector<std::string> text;
        std::vector<std::string> responeses;
        
    public:

        void Draw();

        Message();
        Message(std::string filename);
};
