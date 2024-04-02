#include"conversation.h"

#include"global_func.h"
#include <string>

void Conversation::restart(){
    current_message = Message("res/texts/" + directory + "/" + first_file);
    has_finished = false;
}

void Conversation::respond(){
    current_message.respond();
    if(current_message.hasResponded()){
        if(current_message.getUserResponse() == last_message_id){
            has_finished = true;
        }else{
            int next_file = current_message.getUserResponse();
            std::string file = "res/texts/" + directory + "/" + std::to_string(next_file) + ".json";
            current_message = Message(file);
        }
    }
}

void Conversation::Draw(){
    current_message.Draw();
}

Conversation::Conversation(){
}

Conversation::Conversation(std::string dir, std::string first_message, int last_message_id):last_message_id{last_message_id}{
    directory = dir;
    filenames = getAllFileNamesInDirectory("res/texts/" + dir);
    first_file = first_message;
    current_message = Message("res/texts/" + dir + "/" + first_message);

    has_finished = false;
}
