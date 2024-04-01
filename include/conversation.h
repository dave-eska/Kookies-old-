#pragma once

#include<vector>
#include<string>

#include"message.h"

class Conversation{
    private:
        std::vector<std::string> filenames;
        std::string directory;

        Message current_message;

        int last_message_id;
        bool has_finished;
    public:

        bool hasFinised(){return has_finished;}

        void respond();
        void Draw();

        Conversation();
        Conversation(std::string dir, std::string first_message, int last_message_id);
};
