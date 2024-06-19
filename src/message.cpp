#include "message.h"

#include <json/json.h>
#include <json/value.h>
#include  <string>

#include "global_func.h"
#include "item.h"
#include "raylib.h"
#include "screens.h"
#include "tile.h"

Message::Message(){
}

Message::Message(std::string filename, int id): id{id}{
    std::string text = readFile(filename);
    Json::Value root;
    Json::Reader jsonreader;

    jsonreader.parse(text, root);

    this->body = {275, 538, 1050, 336};
    this->button_dark_body = {1010, 811, 301, 35};

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
        auto jsondata = root["response"][i];

        MessageResponse response;
        response.response = jsondata[0].asString();
        if(jsondata.size() >= 3){
            auto responseDetail = jsondata[2];
            response.type = responseDetail["type"].asString();
            response.itemID = responseDetail["itemID"].asInt();
            response.price = responseDetail["price"].asInt();
        }

        responses.push_back(response);
        next_file.push_back(root["response"][i][1].asInt());
    }

    has_responded = false;
    animationDone = false;

    hasResponse = root["response"].size() > 0;
    this->texture = LoadTexture(hasResponse ? "res/img/npc-convo.png" : "res/img/npc-convo-no-response.png");
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

    if(hasResponse){
        if(CheckCollisionPointRec(GetMousePosition(), {604, 811, button_dark_body.width, button_dark_body.height})){
            isDrawingDarkButton = true;
            dark_button_pos = {604, 811};
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                auto &plr = getPlayer();
                if(responses[0].type == "sell" && plr.getInv().has(responses[0].itemID)){
                    plr.AddMoney(responses[0].price);
                    plr.decreaseItemCount((TileID)responses[0].itemID);
                }else if(responses[0].type == "buy" && plr.getInv().getMoney() >= responses[0].price){
                    plr.decreaseMoney(responses[0].price);
                    plr.addItemInv(newItem<Tile>(responses[0].itemID));
                }

                user_response = next_file[0];;
                scroll_level = 0;
                has_responded = true;
            }
        }else if(CheckCollisionPointRec(GetMousePosition(), {926, 811, button_dark_body.width, button_dark_body.height})){
            isDrawingDarkButton = true;
            dark_button_pos = {926, 811};
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                user_response = next_file[1];
                scroll_level = 0;
                has_responded = true;
            }
        }
    }else{
        if(IsKeyPressed(KEY_SPACE)){
            user_response = id+1;
            scroll_level = 0;
            has_responded = true;
        }
    }
}

void Message::Draw(){
    DrawTextureEx(texture, {body.x, body.y}, 0, 7, WHITE);
    if(isDrawingDarkButton) DrawRectangleV(dark_button_pos, {button_dark_body.width, button_dark_body.height}, {66, 20, 13, 255});

    if(text.size() >= 3){
        for(int i=0;i<3;i++){
            DrawTextEx(font, text[i+scroll_level].c_str(), {text_pos.x, text_pos.y+(i*64)}, 30, 0, BLACK);
        }
    }else if(text.size() < 3){
        for(int i=0;i<text.size();i++){
            DrawTextEx(font, text[i].c_str(), {text_pos.x, text_pos.y+(i*64)}, 30, 0, BLACK);
        }
    }

    if(IsKeyPressed(KEY_UP) && scroll_level > 0)
        scroll_level--;
    if(IsKeyPressed(KEY_DOWN) && scroll_level < text.size()-3)
        scroll_level++;

    if(hasResponse){
        DrawText(responses[0].response.c_str(), 604, 814, 30, BLACK);
        DrawText(responses[1].response.c_str(), 926, 814, 30, BLACK);
    }
}
