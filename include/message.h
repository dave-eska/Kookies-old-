#pragma once

#include<string>
#include<vector>

#include<raylib.h>

class Message{
    private:
        Rectangle body;
        Rectangle button_dark_body;

        Texture2D texture;

        Texture2D faceTexture;
        Texture2D response_texture;

        Vector2 dark_button_pos;

        bool isDrawingDarkButton;

        int id;
        std::vector<std::string> text;
        std::vector<std::string> responses;
        std::vector<int> next_file;
        bool hasResponse ;

        Vector2 text_pos;
        int timer;

        bool animationDone;

        int scroll_level;

        int user_response;
        bool has_responded;
    public:

        int getUserResponse();
        bool hasResponded();

        int getID(){return id;}

        std::vector<int> getNextFiles();

        void respond();
        void Draw();

        Message();
        Message(std::string filename, int id);
};
