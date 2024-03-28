#include"message.h"

#include<iostream>

#include<json/json.h>
#include<json/value.h>

#include"global_func.h"
#include"raylib.h"

Message::Message(){
}

Message::Message(std::string filename){
    std::string text = readFile(filename);
    Json::Value root;
    Json::Reader jsonreader;

    jsonreader.parse(text, root);

    this->body = {275, 540, 1050, 336};
    this->texture = LoadTexture("res/img/npc-convo.png");

    std::string temp_text;
    for(char c : root["text"].asString()){
        std::cout<<c<<std::endl;
        temp_text.push_back(c);
        if((temp_text.size() > 40 && c == ' ') || c == '\n'){
            std::cout<<temp_text<<std::endl;
            this->text.push_back(temp_text);
            temp_text.clear();
        }
    }
    this->text.push_back(temp_text);
}

void Message::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 7, WHITE);
    //DrawText(text.c_str(), 467, 607, 30, BLACK);
    for(int i=0;i<text.size();i++){
        DrawText(text[i].c_str(), 331, 607+(i*64), 30, BLACK);
    }
}
