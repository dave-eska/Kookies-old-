#include"message.h"

#include<json/json.h>
#include<json/value.h>
#include <string>

#include"global_func.h"
#include"raylib.h"

Message::Message(){
}

Message::Message(std::string filename){
    std::string text = readFile(filename);
    Json::Value root;
    Json::Reader jsonreader;

    jsonreader.parse(text, root);

    this->body = {275, 538, 1050, 336};
    this->button_dark_body = {1010, 811, 301, 35};

    this->texture = LoadTexture("res/img/npc-convo.png");

    std::string temp_text;
    for(char c : root["text"].asString()){
        temp_text.push_back(c);
        if((temp_text.size() > 45 && c == ' ') || c == '\n'){
            this->text.push_back(temp_text);
            temp_text.clear();
        }
    }
    this->text.push_back(temp_text);
    this->text_pos = {331, 607};
    this->scroll_level = 0;

    for(int i=0;i<root["response"].size();i++){
        responses.push_back(root["response"][i][0].asString());
        next_file.push_back(root["response"][i][1].asInt());
    }

    has_responded = false;
}

int Message::getUserResponse(){
    return user_response;
}

bool Message::hasResponded(){
    return has_responded;
}

std::vector<int> Message::getNextFiles(){
    return next_file;
}

void Message::respond(){
    isDrawingDarkButton = false;

    if(CheckCollisionPointRec(GetMousePosition(), {604, 811, button_dark_body.width, button_dark_body.height})){
        isDrawingDarkButton = true;
        dark_button_pos = {604, 811};
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            user_response = next_file[0];;
            has_responded = true;
        }
    }else if(CheckCollisionPointRec(GetMousePosition(), {926, 811, button_dark_body.width, button_dark_body.height})){
        isDrawingDarkButton = true;
        dark_button_pos = {926, 811};
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            user_response = next_file[1];
            has_responded = true;
        }
    }
}


void Message::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 7, WHITE);
    if(isDrawingDarkButton) DrawRectangleV(dark_button_pos, {button_dark_body.width, button_dark_body.height}, {66, 20, 13, 255});

    for(int i=0;i<3;i++){
        DrawText(text[i+scroll_level].c_str(), text_pos.x, text_pos.y+(i*64), 30, BLACK);
    }

    if(IsKeyPressed(KEY_UP) && scroll_level > 0)
        scroll_level--;
    if(IsKeyPressed(KEY_DOWN) && scroll_level < text.size()-3)
        scroll_level++;

    DrawText(responses[0].c_str(), 604, 814, 30, BLACK);
    DrawText(responses[1].c_str(), 926, 814, 30, BLACK);
}
